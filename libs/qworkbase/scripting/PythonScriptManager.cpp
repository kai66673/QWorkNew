#include <QComboBox>
#include <QStackedWidget>
#include <QTableView>
#include <QTreeView>
#include <QHeaderView>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QTextCodec>
#include <QApplication>
#include <QStringRef>
#include <QMenu>

#include "PythonScriptManager.h"
#include "QWorkBase.h"
#include "LogManager.h"
#include "CommandManager.h"
#include "AddPythonScriptDialog.h"
#include "ConfirmRemoveScriptDialog.h"
#include "QwGuiPythonModule.h"

namespace Python {

PythonScriptSlotDelegate::PythonScriptSlotDelegate( QObject *parent )
    : QItemDelegate(parent)
{ }

QWidget *PythonScriptSlotDelegate::createEditor( QWidget *parent,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *typeCombo = new QComboBox(parent);
    typeCombo->addItem(tr("<No Slot>"));
    for (int i = 0; i < 10; i++)
        typeCombo->addItem(tr("Slot %1").arg(i));

    return typeCombo;
}

void PythonScriptSlotDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    QComboBox *typeCombo = static_cast<QComboBox *>(editor);
    typeCombo->setCurrentIndex(index.model()->data(index, Qt::EditRole).toInt() + 1);
    return;
}

void PythonScriptSlotDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,
                                             const QModelIndex &index ) const
{
    QComboBox *typeCombo = static_cast<QComboBox *>(editor);
    model->setData(index, typeCombo->currentIndex() - 1, Qt::EditRole);
    return;
}

void PythonScriptSlotDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index ) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

PythonScriptModel::PythonScriptModel( QMap<QString, PythonScript *> *scripts, QObject *parent )
    : QAbstractTableModel(parent)
    , m_scripts(scripts)
{
    setupModel();
}

void PythonScriptModel::setupModel()
{
    foreach ( PythonScript *script, *m_scripts )
        m_scriptList.append(script);
}

bool PythonScriptModel::checkNewSlotNumber( int scriptIndex, int newSlotNumber ) const
{
    if ( newSlotNumber == -1 )
        return true;

    for (int i = 0; i < m_scriptList.size(); i++)
        if ( scriptIndex != i && m_scriptList[i]->slotNumber() == newSlotNumber )
            return false;

    return true;
}

int PythonScriptModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return m_scriptList.size();
}

int PythonScriptModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant PythonScriptModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_scriptList.size() || index.row() < 0)
        return QVariant();

    if ( role == Qt::ToolTipRole )
        return m_scriptList[index.row()]->filePath();

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        switch ( index.column() ) {
            case 0: return m_scriptList[index.row()]->name();
            case 1: return m_scriptList[index.row()]->description();
            case 2:
                if ( role == Qt::DisplayRole ) {
                    if ( m_scriptList[index.row()]->slotNumber() == -1 )
                        return tr("<No Slot>");
                    return tr("Slot %1").arg(m_scriptList[index.row()]->slotNumber());
                }
                else {
                    return m_scriptList[index.row()]->slotNumber();
                }
        }
    }

    return QVariant();
}

QVariant PythonScriptModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal ) {
        switch ( section ) {
            case 0: return tr("Name");
            case 1: return tr("Description");
            case 2: return tr("Slot");
        }
    }

    return QVariant();
}

Qt::ItemFlags PythonScriptModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if ( index.column() != 0 )
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;

    return QAbstractTableModel::flags(index);
}

PythonScript *PythonScriptModel::appendScript(const QString &name, const QString &description , const QString &filePath, QTableView *view )
{
    int pos = m_scriptList.size();
    beginInsertRows(QModelIndex(), pos, pos);
    PythonScript *newScript = new PythonScript(name, description, filePath, -1);
    m_scriptList.append(newScript);
    endInsertRows();
    view->resizeRowToContents(pos);
    return newScript;
}

int PythonScriptModel::rowForScript( PythonScript *script )
{
    int row = 0;
    foreach ( PythonScript *s, m_scriptList ) {
        if ( s == script )
            return row;
        row++;
    }

    return -1;
}

bool PythonScriptModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    beginRemoveRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++) {
        QString scriptName = m_scriptList[position]->name();
        m_scriptList.removeAt(position);
        m_scripts->remove(scriptName);
    }
    endRemoveRows();

    return true;
}

bool PythonScriptModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( role != Qt::EditRole || index.column() == 0 )
        return false;

    if ( index.column() == 1 ) {
        m_scriptList[index.row()]->setDescription(value.toString().trimmed());
    } else /*if ( index.column() == 2 )*/ {
        int newSlotNumber = value.toInt();
        if ( !checkNewSlotNumber(index.row(), newSlotNumber) )
            return false;
        m_scriptList[index.row()]->setSlotNumber(value.toInt());
    }

    emit dataChanged(index, index);
    return true;
}

PythonScriptManagerModule::PythonScriptManagerModule()
    : IPythonModule("QwPythonScriptManager", "pythonScriptManager", "Python Script Manager public Methods")
{
    m_functions << new PythonModuleFunction("executeScript", "Execute Python Script Named by Argument")
                << new PythonModuleFunction("executeBuiltinScript", "Execute Builtin Python Script Identified By Argument")
                << new PythonModuleFunction("editScript", "Open Script Named by Argument in Editor")
                << new PythonModuleFunction("createScript", "Create New Python Script Named by Argument")
                << new PythonModuleFunction("scriptNames", "Get All Puthon Script Names")
                << new PythonModuleFunction("mainWindow", "Returns Main Window of Application")
                << new PythonModuleFunction("setErrorReportLevel", "Set Report Level for Errors")
                << new PythonModuleFunction("setWarningReportLevel", "Set Report Level for Warnings");
}

PythonScriptManagerModule::~PythonScriptManagerModule()
{
    qDeleteAll(m_functions);
}

QList<PythonModuleFunction *> PythonScriptManagerModule::functions() const
{
    return m_functions;
}

QString PythonScriptManagerModule::sourceCodeForAssist() const
{
    return QString("class QwPythonScriptManager():\n"
                   "    LogManager = 0\n"
                   "    Message = 1\n"
                   "    Console = 2\n"
                   "    Warning = 0\n"
                   "    Error = 1\n"
                   "    PythonConsole = 3\n"
                   "    def executeScript(self, scriptName):\n"
                   "        return True\n"
                   "    def executeBuiltinScript(self, scriptId):\n"
                   "        return True\n"
                   "    def editScript(self,scriptName):\n"
                   "        return True\n"
                   "    def createScript(self, scriptName, scriptDescription, open):\n"
                   "        return True\n"
                   "    def scriptNames(self):\n"
                   "        return ['s1', 's2', 's3']\n"
                   "    def setErrorReportLevel(self, level):\n"
                   "        return None\n"
                   "    def setWarningReportLevel(self, level):\n"
                   "        return None\n");
}

bool PythonScriptManagerModule::executeScript( const QString &scriptName )
{
    PythonScriptManager *manager = PythonScriptManager::getInstance();

    QMap<QString, PythonScript *> *scripts = &(manager->m_scripts);
    if ( scripts->find(scriptName) == scripts->end() ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Python Script With Name \"%1\" Not Exists").arg(scriptName));
        return false;
    }

    PythonQt::self()->getMainModule().evalFile(scripts->value(scriptName)->filePath());
    manager->m_pythonConsole->appendCommandPrompt();
    return true;
}

bool PythonScriptManagerModule::executeBuiltinScript( const QString &scriptId )
{
    PythonScriptManager *manager = PythonScriptManager::getInstance();
    if ( manager->m_builtinScripts.find(scriptId) == manager->m_builtinScripts.end() ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Builtin Python Script With Id '%1' Not Exists").arg(scriptId));
        return false;
    }

    PythonQt::self()->getMainModule().evalFile( manager->m_builtinScripts[scriptId]->scriptFile() );
    return true;
}

bool PythonScriptManagerModule::editScript( const QString &scriptName )
{
    QMap<QString, PythonScript *> *scripts = &(PythonScriptManager::getInstance()->m_scripts);
    if ( scripts->find(scriptName) == scripts->end() ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Script With Name \"%1\" Not Exists").arg(scriptName));
        return false;
    }

    PythonScript *script = (*scripts)[scriptName];

    QString errorMessage;
    if ( IDocument *document = DocumentManager::getInstance()->openDocument(script->filePath(), 0, &errorMessage, false) ) {
        document->setTextCodec(QTextCodec::codecForName("UTF-8"));
        return true;
    }

    PythonScriptManager::getInstance()->pythonReport(tr("Erroron Opening Document: %1").arg(errorMessage));
    return false;
}

bool PythonScriptManagerModule::createScript( const QString &scriptName, const QString &scriptDescription, bool open )
{
    QMap<QString, PythonScript *> *scripts = &(PythonScriptManager::getInstance()->m_scripts);
    if ( scripts->find(scriptName) != scripts->end() ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Script With Name \"%1\" Exists").arg(scriptName));
        return false;
    }

    PythonScriptManager *scriptManager = PythonScriptManager::getInstance();
    QString scriptFilePath = scriptManager->m_scriptsDir + "/" + scriptName + ".py";
    QFile scriptFile(scriptFilePath);
    if ( !scriptFile.exists() ) {
        if ( !scriptFile.open(QFile::WriteOnly | QFile::Text) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Create File \"%1\" Failed").arg(scriptFilePath));
            return false;
        }
        scriptFile.close();
    }

    (*scripts)[scriptName] = scriptManager->m_scriptsModel->appendScript(scriptName, scriptDescription, scriptFilePath, scriptManager->m_scriptsView);
    if ( open )
        return editScript(scriptName);
    return true;
}

QStringList PythonScriptManagerModule::scriptNames()
{
    return PythonScriptManager::getInstance()->m_scripts.keys();
}

void PythonScriptManagerModule::setErrorReportLevel( ReportLevel reportLevel )
{
    PythonScriptManager::getInstance()->setErrorReportLevel(reportLevel);
}

void PythonScriptManagerModule::setWarningReportLevel( ReportLevel reportLevel )
{
    PythonScriptManager::getInstance()->setWarningReportLevel(reportLevel);
}

ApplicationScriptModule::ApplicationScriptModule()
    : IPythonModule("QwApplication", "application", "Application Public Methods")
{
    m_functions << new PythonModuleFunction("mainWindow", "Returns Main Window of Application")
                << new PythonModuleFunction("executeDialog", "Execute Dialog")
                << new PythonModuleFunction("warning", "Show Warning Message")
                << new PythonModuleFunction("close", "Close Application")
                << new PythonModuleFunction("test", "Some Test Function");
}

ApplicationScriptModule::~ApplicationScriptModule()
{
    qDeleteAll(m_functions);
}

QList<PythonModuleFunction *> ApplicationScriptModule::functions() const
{
    return m_functions;
}

QString ApplicationScriptModule::sourceCodeForAssist() const
{
    return QString("from PythonQt import QtGui\n"
                   "class QwApplication():\n"
                   "    def mainWindow(self):\n"
                   "        return QtGui.QWidget()\n"
                   "    def executeDialog(self, dialog):\n"
                   "        return 1\n"
                   "    def warning(self, text):\n"
                   "        return None\n"
                   "    def close(self):\n"
                   "        return None\n"
                   "    def test(self):\n"
                   "        return None\n");
}

QWidget *ApplicationScriptModule::mainWindow()
{
    return QWorkMainWindow::getInstance();
}

int ApplicationScriptModule::executeDialog( QDialog *dialog )
{
    return dialog->exec();
}

void ApplicationScriptModule::warning( const QString &text )
{
    QWorkMainWindow::warning(text);
}

void ApplicationScriptModule::close()
{
    mainWindow()->close();
}

void ApplicationScriptModule::test()
{
    QWorkMainWindow::getInstance()->showWarning("Test Internal");
}

PluginScriptModule::PluginScriptModule( QList<IPlugin *> *plugins )
    : IPythonModule("QwPluginManager", "pluginManager", "Plugin Manager Public Methods")
    , m_plugins(plugins)
{
    m_functions << new PythonModuleFunction("pluginIdentifiers", "Returns List of Supported Plugin Identifiers")
                << new PythonModuleFunction("isPluginSupported", "Returns True If Plugin Identified by String Argument is Supported");
}

PluginScriptModule::~PluginScriptModule()
{
    qDeleteAll(m_functions);
}

QList<PythonModuleFunction *> PluginScriptModule::functions() const
{
    return m_functions;
}

QString PluginScriptModule::sourceCodeForAssist() const
{
    return QString("class QwPluginManager():\n"
                   "    def pluginIdentifiers(self):\n"
                   "        return ['p1', 'p2', 'p3']\n"
                   "    def isPluginSupported(self, pluginId):\n"
                   "        return True\n");
}

QStringList PluginScriptModule::pluginIdentifiers()
{
    QStringList result;
    foreach ( IPlugin *plugin, *m_plugins )
        result << plugin->id();
    return result;
}

bool PluginScriptModule::isPluginSupported( const QString &pluginId )
{
    foreach ( IPlugin *plugin, *m_plugins )
        if ( !plugin->id().compare(pluginId, Qt::CaseSensitive) )
            return true;
    return false;
}

PythonScriptManager *PythonScriptManager::m_instance = 0;

PythonScriptManager::PythonScriptManager( QMenu *scriptsMenu, QObject *parent )
    : QObject(parent)
    , m_scriptsMenu(scriptsMenu)
    , m_errorReportLevel(PythonScriptManagerModule::Message)
    , m_warningReportLevel(PythonScriptManagerModule::Console)
{
    m_instance = this;

    PythonQt::init(/*PythonQt::IgnoreSiteModule |*/ PythonQt::RedirectStdOut);
    PythonQt_QtAll::init();

    m_pythonConsole = new PythonQtScriptingConsole(NULL, PythonQt::self()->getMainModule());

    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();

    m_pythonConsoleDock = new QwCustomDockWidget( mainWindow, tr("Python Console"), "PythonConsoleDock",
                                                  QKeySequence(), QIcon(),
                                                  Qt::BottomDockWidgetArea, QwCustomDockWidget::Second );
    m_pythonConsoleDock->setContentsMargins(0, 0, 0, 0);
    m_pythonConsoleDock->setWidget(m_pythonConsole);
    mainWindow->addDockWidgetHandler(new Core::DockWidgetHandler(m_pythonConsoleDock,
                                                                 new Core::EditModeDockGuiClient(m_pythonConsoleDock)));
    m_pythonConsoleDock->hide();

    m_cmdTogglePythonConsole = new Core::Command( m_pythonConsoleDock->toggleViewAction(),
                                                  tr("TogglePythonConsole"),
                                                  QIcon(":/images/py_console.png"),
                                                  tr("Python Console"),
                                                  tr("Toggle Python Console Dock"),
                                                  QKeySequence("Ctrl+D,C"),
                                                  mainWindow );

    m_cmdExecuteScript = new Core::Command( tr("ExecutePythonScript"),
                                            QIcon(":/images/py_exec_script.png"),
                                            tr("Execute Script"),
                                            tr("Execute Selected Python Script"),
                                            QKeySequence(),
                                            mainWindow );

    m_cmdAddScript = new Core::Command( tr("AddPythonScript"),
                                        QIcon(":/images/py_add_script.png"),
                                        tr("Add Script"),
                                        tr("Create New Python Script"),
                                        QKeySequence(),
                                        mainWindow );

    m_cmdDeleteScript = new Core::Command( tr("DeletePythonScript"),
                                           QIcon(":/images/py_del_script.png"),
                                           tr("Delete Script"),
                                           tr("Delete Selected Python Script"),
                                           QKeySequence(),
                                           mainWindow );

    m_cmdEditScript = new Core::Command( tr("EditPythonScript"),
                                         QIcon(":/images/py_edit_script.png"),
                                         tr("Edit Script"),
                                         tr("Edit Selected Python Script"),
                                         QKeySequence(),
                                         mainWindow );

    m_combo = new QComboBox();
    m_combo->addItem(tr("Scripts"));
    m_combo->addItem(tr("Modules"));
    m_combo->setCurrentIndex(0);

    initScriptCommands();
    loadScripts();

    QStackedWidget *stackedWidget = new QStackedWidget();

    m_scriptsView = new QTableView();
    m_scriptsView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    m_scriptsModel = new PythonScriptModel(&m_scripts);
    m_scriptsView->setModel(m_scriptsModel);
    m_scriptsView->setItemDelegateForColumn(2, new PythonScriptSlotDelegate(m_scriptsView));
    connect(m_scriptsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onScriptsSlotsChanged(QModelIndex,QModelIndex)));

    m_scriptsView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_scriptsView->setSelectionBehavior(QAbstractItemView::SelectRows);
#ifndef HAVE_QT5
    m_scriptsView->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    m_scriptsView->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
#else
    m_scriptsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_scriptsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
    m_scriptsView->resizeColumnsToContents();
    m_scriptsView->resizeRowsToContents();
    m_scriptsView->setFrameShape(QFrame::NoFrame);
    m_scriptsView->setLineWidth(0);

    m_modulesView = new QTreeView();
    m_modulesView->header()->hide();
    m_modulesView->setFrameShape(QFrame::NoFrame);
    m_modulesView->setLineWidth(0);

    stackedWidget->addWidget(m_scriptsView);
    stackedWidget->addWidget(m_modulesView);
    stackedWidget->setCurrentIndex(0);

    m_pythonScriptingDock = new QwCustomDockWidget( mainWindow, tr("Python Scripting"), "PythonScriptingDock",
                                                    QKeySequence(tr("Ctrl+D,S")), QIcon(":/images/py_scripting.png"),
                                                    Qt::BottomDockWidgetArea, QwCustomDockWidget::Second,
                                                    QList<QAction *>() << m_cmdTogglePythonConsole->action()
                                                                       << m_cmdExecuteScript->action()
                                                                       << m_cmdAddScript->action()
                                                                       << m_cmdDeleteScript->action()
                                                                       << m_cmdEditScript->action(),
                                                    m_combo );
    m_pythonScriptingDock->setContentsMargins(0, 0, 0, 0);
    m_pythonScriptingDock->setWidget(stackedWidget);
    mainWindow->addDockWidgetHandler(new Core::DockWidgetHandler(m_pythonScriptingDock,
                                                                 new Core::EditModeDockGuiClient(m_pythonScriptingDock)));

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdTogglePythonConsole->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdExecuteScript->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdAddScript->action());
    contextMenu->addAction(m_cmdDeleteScript->action());
    contextMenu->addAction(m_cmdEditScript->action());
    m_pythonScriptingDock->setContextMenu(contextMenu);

    connect(m_combo, SIGNAL(currentIndexChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

    connect(m_cmdExecuteScript->action(),   SIGNAL(triggered()), this, SLOT(executePythonScript()));
    connect(m_cmdAddScript->action(),       SIGNAL(triggered()), this, SLOT(addPythonScript()));
    connect(m_cmdDeleteScript->action(),    SIGNAL(triggered()), this, SLOT(deletePythonScript()));
    connect(m_cmdEditScript->action(),      SIGNAL(triggered()), this, SLOT(editPythonScript()));

    m_modulesModel = new PythonModuleModel(&m_modules);
    m_modulesView->setModel(m_modulesModel);

    Core::CommandManager::getInstance()->registerCommands( tr("PythonScriptingCommands"),
                                                            QList<Core::Command *>() << m_cmdTogglePythonConsole
                                                                                     << m_cmdExecuteScript
                                                                                     << m_cmdAddScript
                                                                                     << m_cmdDeleteScript
                                                                                     << m_cmdEditScript );

    registerModule(new PythonScriptManagerModule());
    registerModule(new ApplicationScriptModule());
    registerModule(new QwGuiPythonModule(), true, QStringList() << "QwTableView");
}

PythonScriptManager::~PythonScriptManager()
{
    saveScripts();
    freeScritingRopeProject();

    qDeleteAll(m_modules.values());
    qDeleteAll(m_scripts.values());
    qDeleteAll(m_builtinScripts.values());
}

static bool checkIdentifierName( const QString &name )
{
    if ( name.isEmpty() )
        return false;

    if ( !name[0].isLetter() && name[0] != QChar('_') )
        return false;

    for (int i = 1; i < name.length(); i++)
        if ( !name[i].isLetterOrNumber() && name[i] != '_' )
            return false;

    return true;
}

bool PythonScriptManager::registerModule( IPythonModule *module, bool deleteOnFail, const QStringList &classNames )
{
    if ( !module )
        return false;

    if ( !checkIdentifierName(module->moduleName()) ) {
        Core::LogManager::addMessage( Core::LogMessage(tr("Python Module '%1' Not Registered: Incorrect Module Name").arg(module->moduleName()), "scripting") );
        if ( deleteOnFail )
            delete module;
        return false;
    }


    if ( m_modules.find(module->moduleName()) != m_modules.end() ) {
        Core::LogManager::addMessage( Core::LogMessage(tr("Python Module '%1' Not Registered: Module with such Name Exists").arg(module->moduleName()), "scripting") );
        if ( deleteOnFail )
            delete module;
        return false;
    }

    PythonQtObjectPtr pyModule = PythonQt::self()->createModuleFromScript(module->moduleName());
    pyModule.addObject(module->objName(), module);
    PythonQt::self()->registerQObjectClassNames(classNames);

    m_modulesModel->appendModule(module);
    m_modules[module->moduleName()] = module;

    Core::LogManager::addMessage( Core::LogMessage(tr("Python Module '%1' Succefully Registered").arg(module->moduleName()), "scripting") );

    return true;
}

bool PythonScriptManager::registerBuiltinScript( const QString &scriptId, const QString &scriptSource )
{
    if ( m_builtinScripts.find(scriptId) != m_builtinScripts.end() ) {
        Core::LogManager::addMessage( Core::LogMessage(tr("Builtin Python Script With Id '%1' Not Registered: Id Already Used").arg(scriptId), "scripting") );
        return false;
    }

    m_builtinScripts[scriptId] = new BuiltinPythonScript(scriptSource);
    Core::LogManager::addMessage( Core::LogMessage(tr("Builtin Python Script With Id '%1' Succefully Registered").arg(scriptId), "scripting") );
    return true;
}

void PythonScriptManager::registerQObjectClassNames( const QStringList &names )
{
    PythonQt::self()->registerQObjectClassNames(names);
}

QAction *PythonScriptManager::toggleAction()
{
    return m_pythonScriptingDock->toggleViewAction();
}

void PythonScriptManager::addPythonScript()
{
    m_combo->setCurrentIndex(0);

    AddPythonScriptDialog addScriptDialog(&m_scripts, m_scriptsDir, QWorkMainWindow::getInstance());
    if ( addScriptDialog.exec() != QDialog::Accepted )
        return;

    QString scriptName = addScriptDialog.scriptName();
    QString scriptFilePath = m_scriptsDir + "/" + scriptName + ".py";
    QFile scriptFile(scriptFilePath);
    if ( !scriptFile.exists() ) {
        scriptFile.open(QFile::WriteOnly | QFile::Text);
        scriptFile.close();
    }

    m_scripts[scriptName] = m_scriptsModel->appendScript(scriptName, addScriptDialog.scriptDescription(), scriptFilePath, m_scriptsView);
    handleScriptChanged(scriptName + ".py", "");
}

void PythonScriptManager::deletePythonScript()
{
    if ( m_combo->currentIndex() != 0 ) {
        m_combo->setCurrentIndex(0);
        return;
    }

    int index = m_scriptsView->currentIndex().row();
    if ( index == -1 )
        return;

    PythonScript *script = m_scriptsModel->scriptAt(index);
    ConfirmRemoveScriptDialog confirmDialog( QString("Do You Really Want Remove Script '%1'?").arg(script->name()),
                                             QWorkMainWindow::getInstance() );

    if ( confirmDialog.exec() == QDialog::Accepted ) {
        DocumentManager *docManager = DocumentManager::getInstance();
        if ( IDocument *document = docManager->findDocument(script->filePath()) )
            docManager->closeDocument(document, false);
        if ( confirmDialog.removeFromDisk() ) {
            QFile removeFile(script->filePath());
            removeFile.remove();
        }
        m_scriptsModel->removeRows(index, 1);
    }
}

void PythonScriptManager::editPythonScript()
{
    if ( PythonScript *script = currentScript() ) {
        if ( IDocument *document = DocumentManager::getInstance()->openDocument(script->filePath(), 0, 0, false) )
            document->setTextCodec(QTextCodec::codecForName("UTF-8"));
    }
}

void PythonScriptManager::executePythonScript()
{
    if ( PythonScript *script = currentScript() ) {
        PythonQt::self()->getMainModule().evalFile(script->filePath());
        m_pythonConsole->appendCommandPrompt();
    }
}

void PythonScriptManager::onScriptsAction( int slotIndex )
{
    if ( slotIndex < 0 || slotIndex > 9 ) {
        qDebug() << "! Script SLOT ! Invalid slot Index" << slotIndex;
        return;
    }

    if ( PythonScript *script = m_scriptsSlots[slotIndex] ) {
        PythonQt::self()->getMainModule().evalFile(script->filePath());
        m_pythonConsole->appendCommandPrompt();
    }
    else
        QWorkMainWindow::warning(tr("No Script Asssigned to Slot %1").arg(slotIndex));
}

void PythonScriptManager::onScriptsSlotsChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight )
{
    Q_UNUSED(bottomRight)

    if ( topLeft.isValid() && topLeft.column() == 2 ) {
        for (int i = 0; i < 10; i++)
            m_scriptsSlots[i] = 0;
        foreach ( PythonScript *script, m_scripts )
            if ( script->slotNumber() != -1 )
                m_scriptsSlots[script->slotNumber()] = script;
    }
}

PythonScript *PythonScriptManager::currentScript()
{
    if ( m_combo->currentIndex() != 0 ) {
        m_combo->setCurrentIndex(0);
        return 0;
    }

    int index = m_scriptsView->currentIndex().row();
    if ( index == -1 )
        return 0;

    return m_scriptsModel->scriptAt(index);
}

void PythonScriptManager::loadScripts()
{
    m_scriptsFileName = QWorkMainWindow::getInstance()->applicationDataLocation() + "/.scripts";

    m_scriptsDir = m_scriptsFileName + "/scripts";
    QDir dir(m_scriptsDir);
    if ( !dir.exists() )
        dir.mkpath(m_scriptsDir);

    m_scriptsFileName += "/scripts.xml";

    QFile file(m_scriptsFileName);

    if ( !file.exists() )
        return;

    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nFile %1 Cannot be Opened").arg(m_scriptsFileName) );
        return;
    }
    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        QWorkMainWindow::warning( tr("Error On Aliases Loading\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                  .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "scripts" ) {
        QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Format of Python Scripts xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Format of Python Scripts xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Format of Python Scripts xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Format of Python Scripts xml-file (unknown tag <%1> at line %2)")
                                      .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        if ( !itemElement.hasAttribute("name") ) {
            QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Format of Python Scripts xml-file\n(expected attribute \"name\" for tag <%1> at line %2)")
                                      .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        QString name = itemElement.attribute("name");
        if ( !checkIdentifierName(name) ) {
            QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Format of Python Scripts xml-file\n(invalid identifier \"%1\" for tag <%2> at line %3)")
                                      .arg(name).arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        QString slotNumber = itemElement.attribute("slotNumber", "-1");
        bool isValidDigit;
        int slotIndex = slotNumber.toInt(&isValidDigit);
        if ( !isValidDigit || slotIndex > 9 || slotIndex < -1 ) {
            QWorkMainWindow::warning( tr("Error On Python Scripts Loading\nInvalid Value of Script Slot Number for tag <%1> at line %2")
                                      .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        PythonScript *script = new PythonScript( name,
                                                 itemElement.attribute("description", ""),
                                                 m_scriptsDir + "/" + name + ".py",
                                                 slotIndex );
        m_scripts[name] = script;
        if ( slotIndex != -1 )
            m_scriptsSlots[slotIndex] = script;

        itemElement = itemElement.nextSiblingElement();
    }
}

void PythonScriptManager::saveScripts()
{
    const int IndentSize = 4;

    QFile file(m_scriptsFileName);

    if ( !file.exists() ) {
        QFileInfo fi(m_scriptsFileName);
        if ( !fi.absoluteDir().exists() ) {
            QDir dir;
            dir.mkpath(fi.absolutePath());
        }
    }

    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        QWorkMainWindow::warning( tr("Error On Python Scripts Saving\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                                  .arg(m_scriptsFileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("scripts");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("scripts");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    foreach ( PythonScript *script, m_scripts ) {
        QDomElement itemElement = domDocument.createElement("item");
        itemElement.setAttribute("name", script->name());
        itemElement.setAttribute("description", script->description());
        itemElement.setAttribute("slotNumber", script->slotNumber());
        root.appendChild(itemElement);
    }

    QTextStream out(&file);
    domDocument.save(out, IndentSize);
    file.close();
}

QStringList PythonScriptManager::pythonCodeAssist( const QString &source, int position )
{
    QString title = DocumentManager::getInstance()->currentDocument()->title();

    QVariantList args;
    args << source << position << title;
    QVariant result =  m_pythonCodeAssist.call( "get_completion_list", args );

    return result.toStringList();
}

void PythonScriptManager::executeBuiltinScript( const QString &scriptId )
{
    if ( m_builtinScripts.find(scriptId) == m_builtinScripts.end() ) {
        QWorkMainWindow::warning(tr("Builtin Python Script With Id '%1' Not Exists").arg(scriptId));
        return;
    }

    PythonQt::self()->getMainModule().evalFile( m_builtinScripts[scriptId]->scriptFile() );
}

void PythonScriptManager::initPythonCodeAssist()
{
    PythonQt::self()->addSysPath(QApplication::applicationDirPath() + "/python/DLLs");
    PythonQt::self()->addSysPath(QApplication::applicationDirPath() + "/python/Lib");
    m_pythonCodeAssist = PythonQt::self()->createModuleFromScript("PythonCodeAssistModule", "");
    m_pythonCodeAssist.evalFile(":/scripting/initPythonCodeAssist.py");
    m_pythonCodeAssist.call("init_code_assist", QVariantList() << m_scriptsDir << m_scriptingRopeProjectDir);
}

static bool removeDir( const QString &dirName )
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}
void PythonScriptManager::initScritingRopeProject()
{
    m_scriptingRopeProjectDir = QApplication::applicationDirPath() + "/.rope";
    QDir ropeParentRootDir(m_scriptingRopeProjectDir);
    if ( !ropeParentRootDir.exists() )
        ropeParentRootDir.mkpath(m_scriptingRopeProjectDir);

    m_scriptingRopeProjectDir += "/.pid" + QString::number(QApplication::applicationPid());
    QDir ropeRootDir(m_scriptingRopeProjectDir);
    if ( ropeRootDir.exists() )
        removeDir(m_scriptingRopeProjectDir);
    ropeRootDir.mkpath(m_scriptingRopeProjectDir);

    foreach ( IPythonModule *module, m_modules ) {
        QString moduleFileName = m_scriptingRopeProjectDir + "/" + module->moduleName() + ".py";
        QString moduleSource = module->sourceCodeForAssist();
        if ( !moduleSource.isEmpty() ) {
            QFile outFile(moduleFileName);
            if ( outFile.open(QIODevice::WriteOnly | QIODevice::Text) ) {
                QTextStream outstream(&outFile);
                outstream << moduleSource << "\n" << module->objName() << " = " << module->moduleName() << "()";
                outFile.close();
            }
        }
    }

    initPythonCodeAssist();
}

void PythonScriptManager::freeScritingRopeProject()
{
    m_pythonCodeAssist.call("free_code_assist");
    QDir ropeRootDir(m_scriptingRopeProjectDir);
    if ( ropeRootDir.exists() )
        removeDir(m_scriptingRopeProjectDir);
}

void PythonScriptManager::initScriptCommands()
{
    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();

    int i;
    QList<Core::Command *> scriptSlots;

    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(int)), this, SLOT(onScriptsAction(int)));

    for (i = 0; i < 10; i++) {
        m_scriptsSlots[i] = 0;
        m_cmdScriptSlots[i] = new Core::Command( tr("PythonScriptSlots%1").arg(i),
                                                 tr("Script Slot %1").arg(i),
                                                 tr("Script Slot %1").arg(i),
                                                 QKeySequence(tr("Ctrl+Alt+%1").arg(i)),
                                                 mainWindow );
        scriptSlots << m_cmdScriptSlots[i];
        m_scriptsMenu->addAction(m_cmdScriptSlots[i]->action());
        m_signalMapper->setMapping(m_cmdScriptSlots[i]->action(), i);
        connect(m_cmdScriptSlots[i]->action(), SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    }

    Core::CommandManager::getInstance()->registerCommands( tr("PythonScriptSlots"),
                                                           QList<Core::Command *>() << scriptSlots );
}

void PythonScriptManager::setErrorReportLevel( PythonScriptManagerModule::ReportLevel reportLevel )
{
    m_errorReportLevel = reportLevel;
}

void PythonScriptManager::setWarningReportLevel( PythonScriptManagerModule::ReportLevel reportLevel )
{
    m_warningReportLevel = reportLevel;
}

void PythonScriptManager::reportInternal( const QString &message, IMainWindow::ReportType reportType )
{
    PythonScriptManagerModule::ReportLevel level = ( reportType == IMainWindow::Warning ? m_warningReportLevel : m_errorReportLevel );

    switch ( level ) {
        case PythonScriptManagerModule::LogManager:
            Core::LogManager::addMessage( Core::LogMessage(message, "scripting") );
            break;
        case PythonScriptManagerModule::Message:
            QWorkMainWindow::warning(message);
            break;
        case PythonScriptManagerModule::Console:
        case PythonScriptManagerModule::PythonConsole:  // TODO: python print script
            qDebug() << "!Python Script Report!" << message;
            break;
    }
}

void PythonScriptManager::handleScriptChanged( const QString &scriptName, const QString &source )
{
    m_pythonCodeAssist.call( "handle_script_source_changed",
                             QVariantList() << scriptName << source );
}

void PythonScriptManager::pythonReport( const QString &message, IMainWindow::ReportType reportType )
{
    reportInternal(message, reportType);
}

void PythonScriptManager::currentDocumentChanged( IDocument *document )
{
    if ( !document )
        return;
    foreach ( PythonScript *script, m_scripts ) {
#ifdef Q_OS_WIN
        if ( script->filePath().toUpper() == document->filePath().toUpper() )
#else
        if ( script->filePath() == document->filePath() )
#endif
        {
            int row = m_scriptsModel->rowForScript(script);
            if ( row != -1 )
                m_scriptsView->selectRow(row);
            break;
        }
    }
}

PythonModuleModel::PythonModuleModel( QMap<QString, IPythonModule *> *modules, QObject *parent )
    : QAbstractItemModel(parent)
    , m_modules(modules)
{ setupModel(); }

QVariant PythonModuleModel::data( const QModelIndex &index, int role ) const
{
    if ( index.isValid() && index.column() == 0 ) {
        PythonModuleTreeItem *item = static_cast<PythonModuleTreeItem *>(index.internalPointer());
        return item->data(role);
    }
    return QVariant();
}

Qt::ItemFlags PythonModuleModel::flags( const QModelIndex & ) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
}

QVariant PythonModuleModel::headerData( int, Qt::Orientation, int ) const
{
    return QVariant();
}

QModelIndex PythonModuleModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    PythonModuleTreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<PythonModuleTreeItem *>(parent.internalPointer());

    PythonModuleTreeItem *childItem = parentItem->child(row);
    if ( childItem )
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex PythonModuleModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    PythonModuleTreeItem *childItem = static_cast<PythonModuleTreeItem *>(index.internalPointer());
    PythonModuleTreeItem *parentItem = childItem->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int PythonModuleModel::rowCount( const QModelIndex &parent ) const
{
    PythonModuleTreeItem *parentItem;
    if ( parent.column() > 0 )
        return 0;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<PythonModuleTreeItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int PythonModuleModel::columnCount( const QModelIndex & ) const
{ return 1; }

void PythonModuleModel::appendModule( IPythonModule *module )
{
    PythonModuleTreeItem *moduleItem = new PythonModuleTreeItem( module->objName() + ": " + module->moduleName(),
                                                                 module->description(), m_rootItem );
    QList<PythonModuleFunction *> functions = module->functions();
    foreach ( PythonModuleFunction *function, functions ) {
        moduleItem->appendChild( new PythonModuleTreeItem(function->name(), function->description(), moduleItem) );
    }
    int pos = m_rootItem->childCount();
    beginInsertRows(QModelIndex(), pos, pos);
    m_rootItem->appendChild(moduleItem);
    endInsertRows();
}

void PythonModuleModel::setupModel()
{
    m_rootItem = new PythonModuleTreeItem("<Root>", "No need Description");
    foreach ( IPythonModule *module, *m_modules ) {
        m_moduleList.append(module);
        PythonModuleTreeItem *moduleItem = new PythonModuleTreeItem( module->objName() + ": " + module->moduleName(),
                                                                     module->description(), m_rootItem );
        m_rootItem->appendChild(moduleItem);
        QList<PythonModuleFunction *> functions = module->functions();
        foreach ( PythonModuleFunction *function, functions ) {
            moduleItem->appendChild( new PythonModuleTreeItem( function->name(), function->description(), moduleItem ) );
        }
    }
}

PythonModuleTreeItem::PythonModuleTreeItem( const QString &name, const QString &description, PythonModuleTreeItem *parent )
    : m_name(name)
    , m_description(description)
    , m_parent(parent)
{}

PythonModuleTreeItem::~PythonModuleTreeItem()
{
    qDeleteAll(m_childrens);
}

void PythonModuleTreeItem::appendChild( PythonModuleTreeItem *child )
{
    m_childrens.append(child);
}

PythonModuleTreeItem *PythonModuleTreeItem::child( int row )
{
    return m_childrens.value(row);
}

int PythonModuleTreeItem::childCount() const
{
    return m_childrens.count();
}

QVariant PythonModuleTreeItem::data( int role ) const
{
    switch ( role ) {
        case Qt::DisplayRole: return m_name;
        case Qt::ToolTipRole: return m_description;
    }
    return QVariant();
}

int PythonModuleTreeItem::row() const
{
    if ( m_parent )
        return m_parent->m_childrens.indexOf(const_cast<PythonModuleTreeItem *>(this));

    return 0;
}

PythonModuleTreeItem *PythonModuleTreeItem::parent()
{
    return m_parent;
}

}   // namespace Python
