#include <QScrollBar>
#include <QDir>
#include <QApplication>

#include "GrammarRunManager.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include "Core.h"
#include "SettingsManager.h"
#include "grammareditorconstants.h"

GrammarRunParser::GrammarRunParser( Utils::IssuesView *view , const QString &filePath )
    : m_issuesView(view)
    , m_filePath(filePath)
{
    m_posRegExp.setPattern("(\\d+)\\.(\\d+)-(\\d+)");
    m_posRegExp.setMinimal(true);
}

GrammarRunParser::~GrammarRunParser()
{
    sendQueuedTask();
}

void GrammarRunParser::stdOutput( const QString &line )
{
    processCompileLine(line);
}

void GrammarRunParser::stdError(const QString &line)
{
    processCompileLine(line);
}

void GrammarRunParser::sendQueuedTask()
{
    if (m_lastIssue.isNull())
        return;

    m_issuesView->addIssue(m_lastIssue);
    m_lastIssue = Utils::Issue();
}

void GrammarRunParser::processCompileLine( const QString &line )
{
    sendQueuedTask();

    if ( line.isEmpty() || line.at(0).isSpace() )
        return;

    QStringList reg = line.split(":");
    if ( reg.size() >= 4 ) {
        QString type = reg[2].trimmed();
        if ( type == QLatin1String("error") || type == QLatin1String("warning") ) {
            int line = -1;
            int column = 0;
            if ( m_posRegExp.indexIn(reg[1].trimmed()) > -1 ) {
                line = m_posRegExp.cap(1).toInt();
                column = m_posRegExp.cap(2).toInt();
            }
            m_lastIssue = Utils::Issue( type == QLatin1String("error") ? Utils::Issue::Error : Utils::Issue::Warning,
                                        reg[3].trimmed(), m_filePath, line, column, "Grammar.Run" );
        }
    }
}

GrammarRunOutputWindow::GrammarRunOutputWindow( QWidget *parent )
    : QPlainTextEdit(parent)
    , m_enforceNewline(false)
    , m_scrollToBottom(false)
{
    setReadOnly(true);
    QFont f = font();
    f.setFamily("Courier");
    f.setPointSize(9);
    setFont(f);
    setFrameShape(QFrame::NoFrame);
    setLineWidth(0);
}

GrammarRunOutputWindow::~GrammarRunOutputWindow()
{
}

void GrammarRunOutputWindow::scrollToBottom()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void GrammarRunOutputWindow::appendText(const QString &textIn, const QTextCharFormat &format, int maxLineCount)
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

QString GrammarRunOutputWindow::doNewlineEnfocement( const QString &out )
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

bool GrammarRunOutputWindow::isScrollbarAtBottom() const
{
    return verticalScrollBar()->value() == verticalScrollBar()->maximum();
}

GrammarRunManager::GrammarRunManager( QTabBar *buildTabBar, QWidget *parent )
    : QStackedWidget(parent)
    , m_buildTabBar(buildTabBar)
    , m_dirty(false)
    , m_blockedByAutoRunOnSave(false)
    , m_process(0)
    , m_parser(0)
    , m_futureInterface(0)
{
    m_buildActionOutput = new GrammarRunOutputWindow(this);
    m_issues = new Utils::IssuesView(this);
    m_issues->setFrameShape(QFrame::NoFrame);
    m_issues->setLineWidth(0);
    addWidget(m_buildActionOutput);
    addWidget(m_issues);

    connect(m_issues, SIGNAL(activated(QModelIndex)), this, SLOT(onCurrentBuildIssueChanged(QModelIndex)));
}

GrammarRunManager::~GrammarRunManager()
{
    if ( m_process )
        delete m_process;
    if ( m_parser )
        delete m_parser;
    if ( m_futureInterface )
        delete m_futureInterface;
}

void GrammarRunManager::execute()
{
    if ( m_blockedByAutoRunOnSave )
        return;

    if ( IDocument *document = DocumentManager::getInstance()->currentDocument() ) {
        if ( document->isModified() || document->isNew() ) {
            IMainWindow *mainWindow = Core::Storage::mainWindow();
            int ret = QMessageBox::question( mainWindow->widget(),
                                             mainWindow->appName(),
                                             tr("Document is Modified. Save It?"),
                                             QMessageBox::Save | QMessageBox::Cancel,
                                             QMessageBox::Save );
            if ( ret == QMessageBox::Cancel )
                return;
            if ( ret == QMessageBox::Save ) {
                m_blockedByAutoRunOnSave = true;
                bool saved = DocumentManager::getInstance()->trySaveDocument(document);
                m_blockedByAutoRunOnSave = false;
                if ( !saved )
                    return;
            }
        }
        executeInternal(document->filePath());
    }
}

void GrammarRunManager::cancelBuildAction()
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
    m_process = 0;
    delete m_parser;
    m_parser = 0;

    appendText(tr("Cancelled by User"), FatalError);

    m_futureInterface->reportFinished();
    delete m_futureInterface;
    m_futureInterface = 0;

    QApplication::restoreOverrideCursor();

    emit buildActionFinished();
}

void GrammarRunManager::toggleHideWarnings( bool onoff )
{ m_issues->setFilterIncludesWarnings(!onoff); }

void GrammarRunManager::executeInternal( const QString &fileName )
{
    if ( m_dirty )
        return;

    QFileInfo fi(fileName);

    SettingsManager *settingsManager = SettingsManager::instance();

    QString cmd = settingsManager->value(Grammar::Constants::C_GRAMMAR_EDITOR_SETTINGS_ID, "Command").toString();
    QString args = settingsManager->value(Grammar::Constants::C_GRAMMAR_EDITOR_SETTINGS_ID, "Arguments").toString();
    if ( cmd.isEmpty() )
        return;
    if ( !args.isEmpty() )
        args = args.arg(fi.fileName());

    m_dirty = true;


    m_process = new Utils::QtcProcess();
    m_process->setWorkingDirectory(fi.canonicalPath());

    connect(m_process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(processReadyReadStdOutput()),
            Qt::DirectConnection);
    connect(m_process, SIGNAL(readyReadStandardError()),
            this, SLOT(processReadyReadStdError()),
            Qt::DirectConnection);

    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(slotProcessFinished(int, QProcess::ExitStatus)),
            Qt::DirectConnection);

    m_issues->clearIssues("Grammar.Run");

    QString command = cmd + " " + args;

    m_process->setCommand(cmd, args);
    m_parser = new GrammarRunParser(m_issues, fileName);
    m_process->start();
    if ( !m_process->waitForStarted() ) {
        processStartupFailed(command);
        delete m_process;
        m_process = 0;
        delete m_parser;
        m_parser = 0;
        m_dirty = false;
        emit buildActionStartFailed();
        return;
    }
    processStarted(command);

    m_futureInterface = new QFutureInterface<void>();
    m_futureInterface->setProgressRange(0, 0);
    Core::Storage::mainWindow()->addTask(m_futureInterface->future(), "Grammar Run", "Grammar.Run", true, true);
    m_futureInterface->reportStarted();

    emit buildActionStarted();
}

void GrammarRunManager::processReadyReadStdOutput()
{
    m_process->setReadChannel(QProcess::StandardOutput);
    while (m_process->canReadLine()) {
        QString line = QString::fromLocal8Bit(m_process->readLine());
        appendText(line, Normal);
        m_parser->stdOutput(line);
    }
}

void GrammarRunManager::processReadyReadStdError()
{
    m_process->setReadChannel(QProcess::StandardError);
    while (m_process->canReadLine()) {
        QString line = QString::fromLocal8Bit(m_process->readLine());
        appendText(line, Error);
        m_parser->stdError(line);
    }
}

void GrammarRunManager::slotProcessFinished(int, QProcess::ExitStatus)
{
    QString line = QString::fromLocal8Bit(m_process->readAllStandardError());
    if (!line.isEmpty())
        appendText(line, Error);

    line = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    if (!line.isEmpty())
        appendText(line, Normal);

    processFinished(m_process->exitCode(), m_process->exitStatus());

    delete m_process;
    m_process = 0;
    delete m_parser;
    m_parser = 0;

    m_dirty = false;

    m_futureInterface->reportFinished();
    delete m_futureInterface;
    m_futureInterface = 0;

    emit buildActionFinished();
}

void GrammarRunManager::onCurrentBuildIssueChanged( const QModelIndex &index )
{
    if ( !index.isValid() )
        return;

    Utils::Issue issue(m_issues->issue(index));
    if ( issue.isNull() )
        return;

    if ( QFileInfo(issue.file).exists() ) {
        DocumentManager::getInstance()->openDocument(issue.file, new TextDocumentCursor(issue.line - 1, issue.column - 1));
    } else {
        m_issues->setFileNotFound(index, true);
    }
}

static QColor mix_colors(QColor a, QColor b)
{
    return QColor((a.red() + 2 * b.red()) / 3, (a.green() + 2 * b.green()) / 3,
                  (a.blue() + 2* b.blue()) / 3, (a.alpha() + 2 * b.alpha()) / 3);
}

void GrammarRunManager::appendText( const QString &text, TextFormat format )
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

void GrammarRunManager::processStarted( const QString &commandFileName )
{
    m_buildActionOutput->clear();
    appendText(tr("Starting: \"%1\"\n").arg(commandFileName), Message);
    m_buildTabBar->setCurrentIndex(0);
}

void GrammarRunManager::processStartupFailed( const QString &commandFileName )
{
    appendText(tr("Could not start process \"%1\"\n  %2").arg(commandFileName).arg(m_process->errorString()), FatalError);
}

void GrammarRunManager::processFinished( int exitCode, QProcess::ExitStatus status )
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
