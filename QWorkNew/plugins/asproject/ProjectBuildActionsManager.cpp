#include <QScrollBar>
#include <QApplication>
#include <QPainter>
#include <QTextLayout>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QDebug>

#include "Core.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include "ProjectBuildActionsManager.h"
#include "ProjectBuildActionsModel.h"
#include "progressmanager.h"

namespace {
const char * const FILE_POS_PATTERN = "(cl|LINK|[^\\(]+)(\\((\\d+)\\))?";
const char * const ERROR_PATTERN = "[A-Z]+\\d\\d\\d\\d ?:";
}

MsvcParser::MsvcParser( Utils::IssuesView *view )
    : m_issuesView(view)
{
    m_compileRegExp.setPattern(QString::fromLatin1("^") + QLatin1String(FILE_POS_PATTERN)
                               + QLatin1String(" : .*(warning|error) (")
                               + QLatin1String(ERROR_PATTERN) + QLatin1String(".*)$"));
    m_compileRegExp.setMinimal(true);
    m_additionalInfoRegExp.setPattern(QString::fromLatin1("^        ")
                                      + QLatin1String(FILE_POS_PATTERN)
                                      + QLatin1String(" : (.*)$"));
    m_additionalInfoRegExp.setMinimal(true);
}

MsvcParser::~MsvcParser()
{
    sendQueuedTask();
}

void MsvcParser::stdOutput(const QString &line)
{
    if (line.startsWith(QLatin1String("        ")) && m_additionalInfoRegExp.indexIn(line) < 0) {
        if (m_lastIssue.isNull())
            return;

        m_lastIssue.description.append(QChar('\n'));
        m_lastIssue.description.append(line.mid(8));
        // trim trailing spaces:
        int i = 0;
        for (i = m_lastIssue.description.length() - 1; i >= 0; --i) {
            if (!m_lastIssue.description.at(i).isSpace())
                break;
        }
        m_lastIssue.description.truncate(i + 1);

        if (m_lastIssue.formats.isEmpty()) {
            QTextLayout::FormatRange fr;
            fr.start = m_lastIssue.description.indexOf('\n') + 1;
            fr.length = m_lastIssue.description.length() - fr.start;
            fr.format.setFontItalic(true);
            m_lastIssue.formats.append(fr);
        } else {
            m_lastIssue.formats[0].length = m_lastIssue.description.length() - m_lastIssue.formats[0].start;
        }
        return;
    }

    if (processCompileLine(line))
        return;
    if (m_additionalInfoRegExp.indexIn(line) > -1) {
        m_lastIssue = Utils::Issue( Utils::Issue::Unknown,
                                    m_additionalInfoRegExp.cap(4).trimmed(),    /* description */
                                    m_additionalInfoRegExp.cap(1),              /* fileName */
                                    m_additionalInfoRegExp.cap(3).toInt(),      /* linenumber */
                                    0,                                          /* column */
                                    "Task.Category.Compile" );
        return;
    }
}

void MsvcParser::stdError(const QString &line)
{
    if (processCompileLine(line))
        return;
}

bool MsvcParser::processCompileLine(const QString &line)
{
    sendQueuedTask();

    if (m_compileRegExp.indexIn(line) > -1) {
        QString fileName = m_compileRegExp.cap(1);
        if (fileName == QLatin1String("LINK") || fileName == QLatin1String("cl"))
            fileName.clear();
        int linenumber = -1;
        if (!m_compileRegExp.cap(3).isEmpty())
            linenumber = m_compileRegExp.cap(3).toInt();
        m_lastIssue = Utils::Issue( Utils::Issue::Unknown,
                                    m_compileRegExp.cap(5).trimmed() /* description */,
                                    fileName, linenumber, 0,
                                    "Task.Category.Compile" );
        if (m_compileRegExp.cap(4) == QLatin1String("warning"))
            m_lastIssue.type = Utils::Issue::Warning;
        else if (m_compileRegExp.cap(4) == QLatin1String("error"))
            m_lastIssue.type = Utils::Issue::Error;

        return true;
    }
    return false;
}

void MsvcParser::sendQueuedTask()
{
    if (m_lastIssue.isNull())
        return;

    m_issuesView->addIssue(m_lastIssue);
    m_lastIssue = Utils::Issue();
}

ProjectBuildOutputWindow::ProjectBuildOutputWindow( QWidget *parent )
    : QPlainTextEdit(parent)
    , m_enforceNewline(false)
    , m_scrollToBottom(false)
{
    setReadOnly(true);
}

ProjectBuildOutputWindow::~ProjectBuildOutputWindow()
{
}

void ProjectBuildOutputWindow::scrollToBottom()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void ProjectBuildOutputWindow::appendText(const QString &textIn, const QTextCharFormat &format, int maxLineCount)
{
    QString text = textIn;
    text.remove(QLatin1Char('\r'));
    if (document()->blockCount() > maxLineCount)
        return;
    const bool atBottom = isScrollbarAtBottom();
    QTextCursor cursor = QTextCursor(document());
    cursor.movePosition(QTextCursor::End);
    cursor.beginEditBlock();
    cursor.insertText(doNewlineEnfocement(text), format);

    if (document()->blockCount() > maxLineCount) {
        QTextCharFormat tmp;
        tmp.setFontWeight(QFont::Bold);
        cursor.insertText(tr("Additional output omitted\n"), tmp);
    }

    cursor.endEditBlock();
    if (atBottom)
        scrollToBottom();
}

QString ProjectBuildOutputWindow::doNewlineEnfocement( const QString &out )
{
    m_scrollToBottom = true;
    QString s = out;
    if (m_enforceNewline)
        s.prepend(QLatin1Char('\n'));

    m_enforceNewline = true; // make appendOutputInline put in a newline next time

    if (s.endsWith(QLatin1Char('\n')))
        s.chop(1);

    return s;
}

bool ProjectBuildOutputWindow::isScrollbarAtBottom() const
{
    return verticalScrollBar()->value() == verticalScrollBar()->maximum();
}

ProjectBuildActionsManager::ProjectBuildActionsManager( QTabBar *buildTabBar, QWidget *parent )
    : QStackedWidget(parent)
    , m_dirty(false)
    , m_process(0)
    , m_parser(0)
    , m_lastBuildWorkDir("")
    , m_futureInterface(0)
    , m_buildTabBar(buildTabBar)
{
    m_buildActionOutput = new ProjectBuildOutputWindow(this);
    m_issues = new Utils::IssuesView(this);
    addWidget(m_buildActionOutput);
    addWidget(m_issues);
    m_buildTabBar->setCurrentIndex(0);

    connect(m_issues, SIGNAL(activated(QModelIndex)), this, SLOT(onCurrentBuildIssueChanged(QModelIndex)));
}

ProjectBuildActionsManager::~ProjectBuildActionsManager()
{
    if ( m_process )
        delete m_process;
    if ( m_parser )
        delete m_parser;
    if ( m_futureInterface )
        delete m_futureInterface;
}

void ProjectBuildActionsManager::startBuildAction( const ProjectBuildAction &buildAction )
{
    if ( m_dirty )
        return;

    if ( buildAction.m_priorAction == ProjectBuildAction::AskSaveDocuments ) {
        if ( DocumentManager::getInstance()->hasUnsavedDocuments() ) {
            IMainWindow *mainWindow = Core::Storage::mainWindow();
            int ret = QMessageBox::question( mainWindow->widget(),
                                             mainWindow->appName(),
                                             tr("There are Unsaved Documents. Save It?"),
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                             QMessageBox::Save );
            if ( ret == QMessageBox::Cancel )
                return;
            if ( ret == QMessageBox::Save ) {
                if ( !DocumentManager::getInstance()->trySaveAllDocuments() )
                    return;
            }
        }
    }

    if ( buildAction.m_priorAction == ProjectBuildAction::AutoSaveDocuments ) {
        if ( !DocumentManager::getInstance()->trySaveAllDocuments() )
            return;
    }

    m_dirty = true;

    m_process = new Utils::QtcProcess();
    m_process->setWorkingDirectory(buildAction.m_commandWorkDir);
    m_process->setEnvironment(m_environment);

    connect(m_process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(processReadyReadStdOutput()),
            Qt::DirectConnection);
    connect(m_process, SIGNAL(readyReadStandardError()),
            this, SLOT(processReadyReadStdError()),
            Qt::DirectConnection);

    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(slotProcessFinished(int, QProcess::ExitStatus)),
            Qt::DirectConnection);

    m_issues->clearIssues("Task.Category.Compile");

    QString command = buildAction.m_command + " " + buildAction.m_commandArgs;

    m_process->setCommand(buildAction.m_command, buildAction.m_commandArgs);
    m_parser = new MsvcParser(m_issues);
    m_process->start();
    if ( !m_process->waitForStarted() ) {
        processStartupFailed(command);
        delete m_process;
        delete m_parser;
        m_process = 0;
        m_parser = 0;
        m_dirty = false;
        emit buildActionStartFailed();
        return;
    }
    processStarted(command);

    m_lastBuildWorkDir = buildAction.m_commandWorkDir;

    m_futureInterface = new QFutureInterface<void>();
    m_futureInterface->setProgressRange(0, 0);
    Core::Storage::mainWindow()->addTask(m_futureInterface->future(), buildAction.m_commandName, "Build.Action", true, true);
    m_futureInterface->reportStarted();

    emit buildActionStarted();
}

void ProjectBuildActionsManager::processReadyReadStdOutput()
{
    m_process->setReadChannel(QProcess::StandardOutput);
    while (m_process->canReadLine()) {
        QString line = QString::fromLocal8Bit(m_process->readLine());
        appendText(line, Normal);
        m_parser->stdOutput(line);
    }
}

void ProjectBuildActionsManager::processReadyReadStdError()
{
    m_process->setReadChannel(QProcess::StandardError);
    while (m_process->canReadLine()) {
        QString line = QString::fromLocal8Bit(m_process->readLine());
        appendText(line, Error);
        m_parser->stdError(line);
    }
}

void ProjectBuildActionsManager::slotProcessFinished(int, QProcess::ExitStatus)
{
    QString line = QString::fromLocal8Bit(m_process->readAllStandardError());
    if (!line.isEmpty())
        appendText(line, Error);

    line = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    if (!line.isEmpty())
        appendText(line, Normal);

    processFinished(m_process->exitCode(), m_process->exitStatus());

    delete m_process;
    delete m_parser;
    m_process = 0;
    m_parser = 0;

    m_dirty = false;

    m_futureInterface->reportFinished();
    delete m_futureInterface;
    m_futureInterface = 0;

    emit buildActionFinished();
}

void ProjectBuildActionsManager::onCurrentBuildIssueChanged( const QModelIndex &index )
{
    if ( !index.isValid() )
        return;

    Utils::Issue issue(m_issues->issue(index));
    if ( issue.isNull() )
        return;

    if ( QFileInfo(issue.file).exists() ) {
        DocumentManager::getInstance()->openDocument(issue.file, new TextDocumentPosition(issue.line - 1));
    } else if ( QFileInfo(m_lastBuildWorkDir + "/" + issue.file).exists() ) {
        DocumentManager::getInstance()->openDocument( m_lastBuildWorkDir + "/" + issue.file,
                                                      new TextDocumentPosition(issue.line - 1) );
    } else {
        m_issues->setFileNotFound(index, true);
    }
}

void ProjectBuildActionsManager::processStarted( const QString &commandFileName )
{
    m_buildActionOutput->clear();
    appendText(tr("Starting: \"%1\"\n").arg(commandFileName), Message);
    m_buildTabBar->setCurrentIndex(0);
}

void ProjectBuildActionsManager::processStartupFailed( const QString &commandFileName )
{
    appendText(tr("Could not start process \"%1\"\n  %2").arg(commandFileName).arg(m_process->errorString()), FatalError);
}

void ProjectBuildActionsManager::processFinished( int exitCode, QProcess::ExitStatus status )
{
    if (status == QProcess::NormalExit && exitCode == 0) {
        appendText(tr("The process exited normally."), Message);
    } else if (status == QProcess::NormalExit) {
        appendText(tr("The process exited with code %1.").arg(QString::number(m_process->exitCode())), Error);
    } else {
        appendText(tr("The process crashed."), FatalError);
    }

    if ( m_issues->issuesCount() )
        m_buildTabBar->setCurrentIndex(1);

    QApplication::alert(Core::Storage::mainWindow()->widget(), 3000);
}

void ProjectBuildActionsManager::cancelBuildAction()
{
    if ( !m_dirty )
        return;

    disconnect( m_process, SIGNAL(readyReadStandardOutput()),
                this, SLOT(processReadyReadStdOutput()) );
    disconnect( m_process, SIGNAL(readyReadStandardError()),
                this, SLOT(processReadyReadStdError()) );
    disconnect( m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(slotProcessFinished(int, QProcess::ExitStatus)) );

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    m_process->terminate();
    m_process->waitForFinished(3500);
    m_process->kill();
    m_dirty = false;

    delete m_process;
    delete m_parser;
    m_process = 0;
    m_parser = 0;

    appendText(tr("Cancelled by User"), FatalError);

    m_futureInterface->reportFinished();
    delete m_futureInterface;
    m_futureInterface = 0;

    QApplication::restoreOverrideCursor();

    emit buildActionFinished();
}

void ProjectBuildActionsManager::toggleMode()
{
    m_buildTabBar->setCurrentIndex(!m_buildTabBar->currentIndex());
}

void ProjectBuildActionsManager::toggleHideWarnings( bool onoff )
{ m_issues->setFilterIncludesWarnings(!onoff); }

void ProjectBuildActionsManager::clearEnvironment()
{ m_environment.clear(); }

void ProjectBuildActionsManager::refreshEnvironment()
{
    m_environment.clear();

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.open() )
        return;

    QString queryString = "select envkey, envvalue from buildenv";
    QSqlQuery query(db);

    query.exec(queryString);
    while ( query.next() )
        m_environment.set(query.value(0).toString(), query.value(1).toString());
}

static QColor mix_colors(QColor a, QColor b)
{
    return QColor((a.red() + 2 * b.red()) / 3, (a.green() + 2 * b.green()) / 3,
                  (a.blue() + 2* b.blue()) / 3, (a.alpha() + 2 * b.alpha()) / 3);
}

void ProjectBuildActionsManager::appendText( const QString &text, ProjectBuildActionsManager::TextFormat format )
{
    QPalette p = m_buildActionOutput->palette();
    QTextCharFormat textFormat;
    switch (format) {
        case Normal:
            textFormat.setForeground(p.color(QPalette::Text));
            textFormat.setFontWeight(QFont::Normal);
            break;
        case Message:
            textFormat.setForeground(mix_colors(p.color(QPalette::Text), QColor(Qt::blue)));
            break;
        case Error:
            textFormat.setForeground(mix_colors(p.color(QPalette::Text), QColor(Qt::red)));
            textFormat.setFontWeight(QFont::Normal);
            break;
        case FatalError:
            textFormat.setForeground(mix_colors(p.color(QPalette::Text), QColor(Qt::red)));
            textFormat.setFontWeight(QFont::Bold);
            break;
    }

    m_buildActionOutput->appendText(text, textFormat, 1000);
}
