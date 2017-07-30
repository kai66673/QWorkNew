#ifndef PYTHONSCRIPTMANAGER_H
#define PYTHONSCRIPTMANAGER_H

#include <QObject>
#include <QAction>
#include <QMap>
#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QItemDelegate>

#include <PythonQt.h>
#include <PythonQt_QtAll.h>
#include <gui/PythonQtScriptingConsole.h>

#include "QWorkBase_global.h"
#include "QWorkBase.h"
#include "QwDockWidget.h"
#include "IPlugin.h"
#include "IPythonModule.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QTableView;
class QComboBox;
QT_END_NAMESPACE

namespace Python {

class PythonScriptManager;
class PythonScriptManagerModule;

class PythonScript
{
public:
    PythonScript( const QString &name, const QString &description, const QString &filePath,
                  int slotNumber, const QString &scriptSource = "")
        : m_name(name)
        , m_description(description)
        , m_filePath(filePath)
        , m_scriptSource(scriptSource)
        , m_slotNumber(slotNumber)
    {}

    QString name() const { return m_name; }
    QString description() const { return m_description; }
    QString scriptSource() const { return m_scriptSource; }
    QString filePath() const { return m_filePath; }
    int slotNumber() const { return m_slotNumber; }

    void setName( const QString &name ) { m_name = name; }
    void setDescription( const QString &description ) { m_description = description; }
    void setScriptSource( const QString &scriptSource ) { m_scriptSource = scriptSource; }
    void setSlotNumber( int slotIndex ) { m_slotNumber = slotIndex; }

private:
    QString m_name;
    QString m_description;
    QString m_filePath;
    QString m_scriptSource;
    int     m_slotNumber;
};

class PythonScriptSlotDelegate : public QItemDelegate
{
public:
    PythonScriptSlotDelegate( QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index ) const;

    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index ) const;
};

class PythonScriptModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    PythonScriptModel( QMap<QString, PythonScript *> *scripts, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() );
    bool setData( const QModelIndex &index, const QVariant &value, int role );

    PythonScript *appendScript( const QString &name, const QString &description, const QString &filePath , QTableView *view );
    PythonScript *scriptAt( int index ) { return m_scriptList[index]; }

    int rowForScript( PythonScript *script );

private:
    void setupModel();
    bool checkNewSlotNumber( int scriptIndex, int newSlotNumber ) const;
    QMap<QString, PythonScript *> *m_scripts;
    QList<PythonScript *> m_scriptList;
};

class PythonModuleTreeItem
{
public:
    PythonModuleTreeItem( const QString &name, const QString &description, PythonModuleTreeItem *parent = 0 );
    virtual ~PythonModuleTreeItem();

    void appendChild( PythonModuleTreeItem *child );

    PythonModuleTreeItem *child( int row );
    int childCount() const;
    QVariant data( int role = Qt::DisplayRole ) const;
    int row() const;
    PythonModuleTreeItem *parent();


private:
    QString m_name;
    QString m_description;

    QList<PythonModuleTreeItem *> m_childrens;
    PythonModuleTreeItem *m_parent;
};

class PythonModuleModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    PythonModuleModel( QMap<QString, IPythonModule *> *modules, QObject *parent = 0 );

    QVariant data( const QModelIndex &index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &/*index*/ ) const;
    QVariant headerData( int /*section*/, Qt::Orientation /*orientation*/, int /*role = Qt::DisplayRole*/ ) const;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &/*parent = QModelIndex()*/ ) const;

    void appendModule( IPythonModule *module );

private:
    void setupModel();

    QMap<QString, IPythonModule *> *m_modules;
    QList<IPythonModule *> m_moduleList;
    PythonModuleTreeItem *m_rootItem;
};

class PythonScriptManagerModule: public IPythonModule
{
    Q_OBJECT
    Q_ENUMS(ReportLevel)

public:
    enum ReportLevel {
        LogManager,
        Message,
        Console,
        PythonConsole
    };

    PythonScriptManagerModule();
    virtual ~PythonScriptManagerModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

public slots:
    bool executeScript( const QString &scriptName );
    bool executeBuiltinScript( const QString &scriptId );
    bool editScript( const QString &scriptName );
    bool createScript( const QString &scriptName, const QString &scriptDescription, bool open );
    QStringList scriptNames();

    void setErrorReportLevel( ReportLevel reportLevel );
    void setWarningReportLevel( ReportLevel reportLevel );

private:
    QList<PythonModuleFunction *> m_functions;
};

class ApplicationScriptModule: public IPythonModule
{
    Q_OBJECT

public:
    ApplicationScriptModule();
    virtual ~ApplicationScriptModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

public slots:
    QWidget *mainWindow();
    int executeDialog( QDialog *dialog );
    void warning( const QString &text );
    void close();
    void test();

private:
    QList<PythonModuleFunction *> m_functions;
};

class PluginScriptModule: public IPythonModule
{
    Q_OBJECT

public:
    PluginScriptModule( QList<IPlugin *> *plugins );
    virtual ~PluginScriptModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

public slots:
    QStringList pluginIdentifiers();
    bool isPluginSupported( const QString &pluginId );

private:
    QList<IPlugin *> *m_plugins;
    QList<PythonModuleFunction *> m_functions;
};

class PythonScriptManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PythonScriptManager)

    friend class ::QWorkMainWindow;
    friend class PythonScriptManagerModule;
    friend class PythonQtObject;

    static PythonScriptManager *m_instance;
    explicit PythonScriptManager( QMenu *scriptsMenu, QObject *parent = 0 );

public:
    virtual ~PythonScriptManager();
    static PythonScriptManager *getInstance() { return m_instance; }

    bool registerModule(IPythonModule *module, bool deleteOnFail = true, const QStringList &classNames = QStringList() );
    bool registerBuiltinScript( const QString &scriptId, const QString &scriptSource );
    void registerQObjectClassNames( const QStringList& names );

    template <class T, class W>
    bool registerCppClass( const char *className, const char *classModule, bool autoImport = true )
    {
        qRegisterMetaType<T>(className);
        PythonQt::self()->registerCPPClass(className, "", classModule, PythonQtCreateObject<W>);
        if ( autoImport )
            PythonQt::self()->getMainModule().evalScript(tr("from PythonQt.%1 import %2").arg(classModule).arg(className));
        return true;
    }

    QAction *toggleAction();

    QStringList pythonCodeAssist( const QString &source, int position );
    QMap<QString, IPythonModule *> *pythonModules() { return &m_modules; }

    void executeBuiltinScript( const QString &scriptId );
    void handleScriptChanged( const QString &scriptName, const QString &source );

    void pythonReport( const QString &message, IMainWindow::ReportType reportType = IMainWindow::Error );

signals:

public slots:
    void currentDocumentChanged( IDocument *document );
    
private slots:
    void addPythonScript();
    void deletePythonScript();
    void editPythonScript();
    void executePythonScript();

    void onScriptsAction( int slotIndex );
    void onScriptsSlotsChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight );

private:
    PythonScript *currentScript();

    PythonQtScriptingConsole *m_pythonConsole;

    QComboBox *m_combo;
    QwCustomDockWidget *m_pythonConsoleDock;
    QwCustomDockWidget *m_pythonScriptingDock;

    Core::Command *m_cmdTogglePythonConsole;
    Core::Command *m_cmdExecuteScript;
    Core::Command *m_cmdAddScript;
    Core::Command *m_cmdDeleteScript;
    Core::Command *m_cmdEditScript;

    QMap<QString, IPythonModule *> m_modules;
    QTreeView *m_modulesView;
    PythonModuleModel *m_modulesModel;

    void loadScripts();
    void saveScripts();
    QString m_scriptsFileName;
    QString m_scriptsDir;
    QMap<QString, PythonScript *> m_scripts;
    QTableView *m_scriptsView;
    PythonScriptModel *m_scriptsModel;

    void initPythonCodeAssist();
    PythonQtObjectPtr m_pythonCodeAssist;

    QMap<QString, BuiltinPythonScript *> m_builtinScripts;

    void initScritingRopeProject();
    void freeScritingRopeProject();
    QString m_scriptingRopeProjectDir;

    void initScriptCommands();
    QMenu *m_scriptsMenu;
    Core::Command *m_cmdScriptSlots[10];
    PythonScript *m_scriptsSlots[10];
    QSignalMapper *m_signalMapper;

    void setErrorReportLevel( PythonScriptManagerModule::ReportLevel reportLevel );
    void setWarningReportLevel( PythonScriptManagerModule::ReportLevel reportLevel );
    void reportInternal( const QString &message, IMainWindow::ReportType reportType = IMainWindow::Warning );
    PythonScriptManagerModule::ReportLevel m_errorReportLevel;
    PythonScriptManagerModule::ReportLevel m_warningReportLevel;
};

}   // namespace Python

#endif // PYTHONSCRIPTMANAGER_H
