#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSettings>
#include <QCompleter>

#include "Core.h"
#include "IDocument.h"
#include "FindReplaceWidget.h"
#include "ui_FindReplaceWidget.h"

#include "SettingsManager.h"
#include "FindInDialog.h"

namespace {
    const int MAX_COMPLETIONS = 50;
}

FindReplaceWidget::FindReplaceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FindReplaceWidget)
    , m_currentDocumentFind(new Find::Internal::CurrentDocumentFind)
    , m_findFlags((Find::FindFlags)0xFFFFFF)
    , m_firstSearch(true)
{
    ui->setupUi(this);

    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();
    m_orgName = Core::Storage::mainWindow()->orgName();
    m_appName = Core::Storage::mainWindow()->appName();

    m_cmdFindNext = new Core::Command( tr("FindNext"),
                                       QIcon(":/images/fr_find_next.png"), tr("Next"),
                                       tr("Goto Next Found Item"),
                                       QKeySequence(tr("F3")),
                                       mainWindow );

    m_cmdFindPrev = new Core::Command( tr("FindPrevious"),
                                       QIcon(":/images/fr_find_prev.png"),
                                       tr("Previous"),
                                       tr("Goto Previous Found Item"),
                                       QKeySequence(tr("Shift+F3")),
                                       mainWindow );

    m_cmdReplNext = new Core::Command( tr("ReplaceFindNext"),
                                       QIcon(":/images/fr_repl_next.png"),
                                       tr("Forward Replace"),
                                       tr("Replace, Goto Next Found Item"),
                                       QKeySequence(tr("F7")),
                                       mainWindow );

    m_cmdReplPrev = new Core::Command( tr("ReplaceFindPrevious"),
                                       QIcon(":/images/fr_repl_prev.png"),
                                       tr("Backward Replace"),
                                       tr("Replace, Goto Previous Found Item"),
                                       QKeySequence(tr("Shift+F7")),
                                       mainWindow );

    m_cmdFindIn   = new Core::Command( tr("FindIn"),
                                       tr("Find In ..."),
                                       tr("Find In ..."),
                                       QKeySequence(tr("Ctrl+Shift+F")),
                                       mainWindow );

    m_cmdReplaceAll = new Core::Command( tr("ReplaceAll"),
                                         tr("Replace All"),
                                         tr("Replace All Occurence in Current Document"),
                                         QKeySequence(),
                                         mainWindow );

    Core::CommandManager::getInstance()->registerCommands( tr("FindInCurrentDocument"),
                                                            QList<Core::Command *>() << m_cmdFindNext
                                                                                     << m_cmdFindPrev
                                                                                     << m_cmdReplNext
                                                                                     << m_cmdReplPrev
                                                                                     << m_cmdFindIn
                                                                                     << m_cmdReplaceAll );

    m_actSeparator = new QAction(mainWindow);
    m_actSeparator->setSeparator(true);

    ui->findNextToolButton->setDefaultAction(m_cmdFindNext->action());
    ui->findPrevToolButton->setDefaultAction(m_cmdFindPrev->action());
    ui->replaceNextToolButton->setDefaultAction(m_cmdReplNext->action());
    ui->replacePrevToolButton->setDefaultAction(m_cmdReplPrev->action());
    ui->findInPushButton->setDefaultAction(m_cmdFindIn->action());
    ui->replaceAllPushButton->setDefaultAction(m_cmdReplaceAll->action());

    setFocusProxy(ui->findLineEdit);

    connect(ui->caseSensetiveCheckBox,          SIGNAL(toggled(bool)), this, SLOT(onCaseSensetiveToggled(bool)));
    connect(ui->wholeWordsOnlyCheckBox,         SIGNAL(toggled(bool)), this, SLOT(onWholeWordsOnlyToggled(bool)));
    connect(ui->useRegularExpressionsCheckBox,  SIGNAL(toggled(bool)), this, SLOT(onUseRegularExpressionsToggled(bool)));

    connect(ui->findLineEdit, SIGNAL(editingFinished()), this, SLOT(invokeResetIncrementalSearch()));
    connect(ui->findLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(invokeFindIncremental()));
    connect(ui->findLineEdit, SIGNAL(returnPressed()), this, SLOT(invokeFindEnter()));

    connect(m_cmdFindNext->action(),    SIGNAL(triggered()), this, SLOT(invokeFindNext()));
    connect(m_cmdFindPrev->action(),    SIGNAL(triggered()), this, SLOT(invokeFindPrev()));
    connect(m_cmdReplNext->action(),    SIGNAL(triggered()), this, SLOT(invokeReplNext()));
    connect(m_cmdReplPrev->action(),    SIGNAL(triggered()), this, SLOT(invokeReplPrev()));
    connect(m_cmdFindIn->action(),      SIGNAL(triggered()), this, SLOT(onFindIn()));
    connect(m_cmdReplaceAll->action(),  SIGNAL(triggered()), this, SLOT(invokeReplaceAll()));

    connect(DocumentManager::getInstance(), SIGNAL(currentEditorChanged(QWidget*)), this, SLOT(startSearchOnDocumentChanged()));

    ui->findLineEdit->installEventFilter(this);
    ui->replaceLineEdit->installEventFilter(this);

    initFindCompletions();
}

FindReplaceWidget::~FindReplaceWidget()
{
    saveCompletions();

    delete ui;
    delete m_currentDocumentFind;
}

void FindReplaceWidget::applySettings()
{
    Find::FindFlags flags;
    bool value;
    SettingsManager *settingsManager = SettingsManager::instance();

    value = settingsManager->value("Find_Base", "CaseSensetive").toBool();
    ui->caseSensetiveCheckBox->setChecked(value);
    if ( value ) flags |= Find::FindCaseSensitively;

    value = settingsManager->value("Find_Base", "WholeWordsOnly").toBool();
    ui->wholeWordsOnlyCheckBox->setChecked(value);
    if ( value ) flags |= Find::FindWholeWords;

    value = settingsManager->value("Find_Base", "UseRegularExpressions").toBool();
    ui->useRegularExpressionsCheckBox->setChecked(value);
    if ( value ) flags |= Find::FindRegularExpression;

    m_findFlags = flags;
}

void FindReplaceWidget::setFindText( const QString &findText )
{
    ui->findLineEdit->setText(findText);
}

QString FindReplaceWidget::findText() const
{
    return ui->findLineEdit->text();
}

QList<QAction *> FindReplaceWidget::searchActions() const
{
    return QList<QAction *>() << m_cmdFindNext->action()
                              << m_cmdFindPrev->action()
                              << m_cmdReplNext->action()
                              << m_cmdReplPrev->action()
                              << m_actSeparator
                              << m_cmdFindIn->action();
}

bool FindReplaceWidget::eventFilter( QObject *obj, QEvent *e )
{
    Q_UNUSED(obj)

    if ( e->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if ( keyEvent->key() == Qt::Key_Escape )
            if ( IDocument *document = DocumentManager::getInstance()->currentDocument() ) {
                document->editorWidget()->setFocus();
                return true;
            }
    }

    return false;
}

void FindReplaceWidget::startSearch()
{
    m_currentDocumentFind->acceptCandidate();
    m_currentDocumentFind->defineFindScope();
    m_currentDocumentFind->highlightAll(ui->findLineEdit->text(), effectiveFindFlags());

    ui->findLineEdit->setFocus();
}

void FindReplaceWidget::startSearchOnDocumentChanged()
{
    m_currentDocumentFind->acceptCandidate();
    m_currentDocumentFind->highlightAll(ui->findLineEdit->text(), effectiveFindFlags());
}

Find::FindFlags FindReplaceWidget::effectiveFindFlags()
{
    Find::FindFlags supportedFlags;
    if (m_currentDocumentFind->isEnabled())
        supportedFlags = m_currentDocumentFind->supportedFindFlags();
    else
        supportedFlags = (Find::FindFlags)0xFFFFFF;
    return supportedFlags & m_findFlags;
}

void FindReplaceWidget::setFindFlag( Find::FindFlags flag, bool enabled )
{
    bool hasFlag = hasFindFlag(flag);
    if ( (hasFlag && enabled) || (!hasFlag && !enabled) )
        return;
    if ( enabled )
        m_findFlags |= flag;
    else
        m_findFlags &= ~flag;
    if ( flag != Find::FindBackward )
        findFlagsChanged();
}

void FindReplaceWidget::findFlagsChanged()
{
    invokeClearResults();
    m_currentDocumentFind->highlightAll(ui->findLineEdit->text(), effectiveFindFlags());
}

bool FindReplaceWidget::hasFindFlag( Find::FindFlags flag )
{
    return m_findFlags & flag;
}

void FindReplaceWidget::initFindCompletions()
{
    QSettings settings(m_orgName, m_appName);
    settings.beginGroup("TextFindHistory");
    m_findCompletions = settings.value("Values", QStringList()).toStringList();
    settings.endGroup();

    m_findModel = new QStringListModel(m_findCompletions, this);
    QCompleter *completer = new QCompleter(m_findModel, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->findLineEdit->setCompleter(completer);
}

void FindReplaceWidget::updateCompletions( const QString &text )
{
    if ( text.isEmpty() )
        return;

    m_findCompletions.removeAll(text);
    m_findCompletions.prepend(text);
    while ( m_findCompletions.size() > MAX_COMPLETIONS )
        m_findCompletions.removeLast();
    m_findModel->setStringList(m_findCompletions);
}

void FindReplaceWidget::saveCompletions()
{
    QSettings settings(m_orgName, m_appName);
    settings.beginGroup("TextFindHistory");
    settings.setValue("Values", m_findCompletions);
    settings.endGroup();
}

void FindReplaceWidget::onCaseSensetiveToggled( bool checked )
{
    SettingsManager::instance()->value("Find_Base", "CaseSensetive") = checked;
    setFindFlag(Find::FindCaseSensitively, checked);
    ui->findLineEdit->setFocus();
}

void FindReplaceWidget::onWholeWordsOnlyToggled( bool checked )
{
    SettingsManager::instance()->value("Find_Base", "WholeWordsOnly") = checked;
    setFindFlag(Find::FindWholeWords, checked);
    ui->findLineEdit->setFocus();
}

void FindReplaceWidget::onUseRegularExpressionsToggled( bool checked )
{
    SettingsManager::instance()->value("Find_Base", "UseRegularExpressions") = checked;
    setFindFlag(Find::FindRegularExpression, checked);
    ui->findLineEdit->setFocus();
}

void FindReplaceWidget::invokeResetIncrementalSearch()
{
    m_findIncrementalTimer.stop();
    m_findStepTimer.stop();
    if (m_currentDocumentFind->isEnabled())
        m_currentDocumentFind->resetIncrementalSearch();
}

void FindReplaceWidget::invokeFindIncremental()
{
    m_findIncrementalTimer.stop();
    m_findStepTimer.stop();
    if ( m_currentDocumentFind->isEnabled() ) {
        QString text = ui->findLineEdit->text();
        Find::IFindSupport::Result result = m_currentDocumentFind->findIncremental(text, effectiveFindFlags());
        if ( result == Find::IFindSupport::NotYetFound )
            m_findIncrementalTimer.start(50);
        if ( text.isEmpty() )
            m_currentDocumentFind->clearResults();
    }
}

void FindReplaceWidget::invokeFindEnter()
{
    if ( QApplication::keyboardModifiers() & Qt::ShiftModifier )
        invokeFindPrev();
    else
        invokeFindNext();
}

void FindReplaceWidget::invokeClearResults()
{
    if ( m_currentDocumentFind->isEnabled() ) {
        m_currentDocumentFind->clearResults();
    }
}

void FindReplaceWidget::invokeFindNext()
{
    setFindFlag(Find::FindBackward, false);
    invokeFindStep();
}

void FindReplaceWidget::invokeFindPrev()
{
    setFindFlag(Find::FindBackward, true);
    invokeFindStep();
}

void FindReplaceWidget::invokeFindStep()
{
    if ( m_firstSearch ) {
        m_firstSearch = false;
        startSearch();
    }

    m_findStepTimer.stop();
    m_findIncrementalTimer.stop();
    if ( m_currentDocumentFind->isEnabled() ) {
        QString txt = ui->findLineEdit->text();
        if ( txt.isEmpty() || !isVisible() ) {
            txt = DocumentManager::getInstance()->selectedText();
            ui->findLineEdit->setText(txt);
        }
        updateCompletions(txt);
        Find::IFindSupport::Result result = m_currentDocumentFind->findStep(txt, effectiveFindFlags());
        if ( result == Find::IFindSupport::NotYetFound )
            m_findStepTimer.start(50);
    }
}

void FindReplaceWidget::invokeReplNext()
{
    setFindFlag(Find::FindBackward, false);
    invokeReplStep();
}

void FindReplaceWidget::invokeReplPrev()
{
    setFindFlag(Find::FindBackward, true);
    invokeReplStep();
}

void FindReplaceWidget::invokeReplStep()
{
    if ( m_currentDocumentFind->isEnabled() && m_currentDocumentFind->supportsReplace() ) {
        QString findText = ui->findLineEdit->text();
        updateCompletions(findText);
        m_currentDocumentFind->replaceStep( findText,
                                            ui->replaceLineEdit->text(),
                                            effectiveFindFlags() );
    }
}

void FindReplaceWidget::invokeReplaceAll()
{
    QString findText = ui->findLineEdit->text();
    updateCompletions(findText);
    if ( m_currentDocumentFind->isEnabled() && m_currentDocumentFind->supportsReplace() ) {
        m_currentDocumentFind->replaceAll( findText,
                                           ui->replaceLineEdit->text(),
                                           effectiveFindFlags() );
    }
}

void FindReplaceWidget::onFindIn()
{
    QString txt = ui->findLineEdit->text();
    if ( txt.isEmpty() || !isVisible() ) {
        txt = DocumentManager::getInstance()->selectedText();
        ui->findLineEdit->setText(txt);
    }

    FindInDialog findInDialog(txt, Core::Storage::mainWindow()->widget());
    findInDialog.exec();
}
