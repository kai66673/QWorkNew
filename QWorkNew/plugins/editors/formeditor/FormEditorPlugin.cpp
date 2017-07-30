#include "mimedatabase.h"

#include "Core.h"
#include "stringutils.h"
#include "CommandManager.h"
#include "ModeManager.h"
#include "FormEditorPlugin.h"
#include "DesignerIntegration.h"
#include "UiTypeDialog.h"
#include "jsonwizardfactory.h"
#include "formtemplatewizardpage.h"
#include "WizardManager.h"
#include "formclasswizard.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesignerComponents>
#include <QDesignerFormEditorPluginInterface>
#include <QDesignerWidgetBoxInterface>
#include <QDesignerObjectInspectorInterface>
#include <QDesignerPropertyEditorInterface>
#include <QDesignerActionEditorInterface>
#include <QMenu>

static void dump(const QObject *obj, int depth)
{
    QString indent;
    indent.fill(' ', depth * 2);
//    if ( dynamic_cast<const QWidget *>(obj) )
        qDebug() << qPrintable(indent) << obj << obj->objectName() << obj->metaObject()->className();
    depth++;

    foreach ( const QObject *ch, obj->children() )
        dump(ch, depth);
}

//////////////////////////////////////////////////////////////////////////////////////
/// FormDocument
FormDocument::FormDocument(FormEditorPlugin *docPlugin )
    : IDocument()
    , m_file(0)
    , m_fileName("")
    , m_documentPlugin(docPlugin)
    , m_host(0)
{
    m_mainWidget = new QWidget();
    m_toolBar = new DocumentToolBar;

    QDesignerFormWindowManagerInterface *fwm = docPlugin->formWindowManager();
    m_form = fwm->createFormWindow(0);
    m_host = new SharedTools::WidgetHost( /* parent */ 0, m_form);
    connect(m_host->formWindow(), SIGNAL(changed()), this, SLOT(onModificationChanged()));
    m_host->setFrameShape(QFrame::NoFrame);
    m_host->setLineWidth(0);

    m_toolBar->addAction(fwm->actionHorizontalLayout());
    m_toolBar->addAction(fwm->actionVerticalLayout());
    m_toolBar->addAction(fwm->actionSplitHorizontal());
    m_toolBar->addAction(fwm->actionSplitVertical());
    m_toolBar->addAction(fwm->actionFormLayout());
    m_toolBar->addAction(fwm->actionGridLayout());
    m_toolBar->addSeparator();
    m_toolBar->addAction(fwm->actionBreakLayout());

    QVBoxLayout *verticalLayout = new QVBoxLayout(m_mainWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(m_toolBar);
    verticalLayout->addWidget(m_host);
}

FormDocument::~FormDocument()
{
    if ( m_file )
        delete m_file;
}

QWidget *FormDocument::editorWidget() const
{
    return m_host;
}

QWidget *FormDocument::widget() const
{
    return m_mainWidget;
}

Core::IEditor *FormDocument::editor() const
{ return 0; }

DocumentToolBar *FormDocument::toolBar()
{ return m_toolBar; }

bool FormDocument::newDocument ( const QString &fileName )
{
    UiTypeDialog typeDialog(Core::Storage::mainWindow()->widget());
    if ( typeDialog.exec() != QDialog::Accepted )
        return false;

    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    QString className = typeDialog.selctedUiType() == UiTypeDialog::Widget ? "Form" : "Dialog";
    QString classType = typeDialog.selctedUiType() == UiTypeDialog::Widget ? "QWidget" : "QDialog";

    QString ctxt = QString( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                            "<ui version=\"4.0\">\n"
                            " <class>%1</class>\n"
                            " <widget class=\"%2\" name=\"%1\">\n"
                            "  <property name=\"geometry\">\n"
                            "   <rect>\n"
                            "    <x>0</x>\n"
                            "    <y>0</y>\n"
                            "    <width>640</width>\n"
                            "    <height>480</height>\n"
                            "   </rect>\n"
                            "  </property>\n"
                            "  <property name=\"windowTitle\">\n"
                            "   <string>%1</string>\n"
                            "  </property>\n"
                            " </widget>\n"
                            " <resources/>\n"
                            " <connections/>\n"
                            "</ui>\n" ).arg(className).arg(classType);

    m_fileName = fileName;
    m_host->formWindow()->setContents(ctxt);
    m_host->formWindow()->setDirty(false);
    m_host->formWindow()->setFocus();

    return true;
}

bool FormDocument::newDocumentFromFile( const QString &fileName, const QString &contentFilePath )
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    QFile ctxFile(contentFilePath);
    if ( !ctxFile.open(QIODevice::ReadOnly | QIODevice::Text) )
        return false;
    QString errorString;
    if ( !m_host->formWindow()->setContents(&ctxFile, &errorString) )
        return false;

    m_fileName = fileName;
    m_host->formWindow()->setDirty(true);
    m_host->formWindow()->setFocus();

    return true;
}

bool FormDocument::openDocument( const QString &filePath )
{
    QFile ctxFile(filePath);
    if ( !ctxFile.open(QIODevice::ReadOnly | QIODevice::Text) )
        return false;
    QString errorString;
    if ( !m_host->formWindow()->setContents(&ctxFile, &errorString) )
        return false;

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    m_host->formWindow()->setDirty(false);
    m_host->formWindow()->setFocus();
    return true;
}

bool FormDocument::saveDocument()
{
    if ( !m_file )
        return false;

    if ( !m_file->open(QIODevice::WriteOnly | QIODevice::Text) )
        return false;

    QTextStream ctxStream(m_file);
    ctxStream << m_host->formWindow()->contents();

    m_host->formWindow()->setDirty(false);
    m_host->formWindow()->setFocus();

    m_file->close();

    return true;
}

bool FormDocument::saveAsDocument( const QString &filePath )
{
    QFile ctxFile(filePath);
    if ( !ctxFile.open(QIODevice::WriteOnly | QIODevice::Text) )
        return false;

    QTextStream ctxStream(&ctxFile);
    ctxStream << m_host->formWindow()->contents();
    m_host->formWindow()->setDirty(false);

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    m_host->formWindow()->setDirty(false);
    m_host->formWindow()->setFocus();

    return true;
}

bool FormDocument::saveToDocument( const QString &filePath )
{
    QFile ctxFile(filePath);
    if ( !ctxFile.open(QIODevice::WriteOnly | QIODevice::Text) )
        return false;

    QTextStream ctxStream(&ctxFile);
    ctxStream << m_host->formWindow()->contents();

    return true;
}

bool FormDocument::renameTo(const QString &newFilePath)
{
    if ( m_file )
        delete m_file;

    m_file = new QFile(newFilePath);

    return true;
}

QString FormDocument::title() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.fileName();
    }

    return m_fileName;
}

QString FormDocument::toolTip() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.absoluteFilePath();
    }

    return QStringLiteral("");
}

QString FormDocument::filePath() const
{
    return toolTip();
}

bool FormDocument::isModified() const
{
    return m_host->formWindow()->isDirty();
}

void FormDocument::setModified(bool onoff)
{
    m_host->formWindow()->setDirty(onoff);
}

void FormDocument::setCursor( IDocumentCursor *position )
{
    Q_UNUSED(position)
    editorWidget()->setFocus();
}

IDocumentPlugin *FormDocument::documentPlugin() const
{ return m_documentPlugin; }

void FormDocument::onModificationChanged()
{
    if ( m_host && m_host->formWindow() )
        m_host->formWindow()->setDirty(true);
    DocumentManager::getInstance()->onDocumentModificationChanged(this);
    m_documentPlugin->updateEditorActions();
}

void FormDocument::onActiveFormWindowChanged(QDesignerFormWindowInterface *fw)
{
    if ( m_host->formWindow() == fw )
        onActivate();
}

void FormDocument::onActivate()
{
    DocumentManager::getInstance()->onCurrentEditorChanged(this);
}

void FormDocument::onFormatSettingsChanged()
{ }

void FormDocument::emitDocumentOpened( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
}

void FormDocument::emitDocumentClosed( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
}

void FormDocument::setTextCodec( QTextCodec *codec )
{
    Q_UNUSED(codec)
}

void FormDocument::saveState( QDomDocument *document, QDomElement *element ) const
{
    Q_UNUSED(document)
    Q_UNUSED(element)
}

void FormDocument::restoreState( QDomElement *element )
{
    Q_UNUSED(element)
}

QDesignerFormWindowInterface *FormDocument::formWindow()
{
    return m_host->formWindow();
}

class FormEditorMode: public Core::IMode
{
public:
    Core::Id id() const { return "Designer"; }
};

class FormModeDockGuiClient: public Core::DockModeClient
{
public:
    FormModeDockGuiClient(QDockWidget *dock, Core::IMode *mode)
        : Core::DockModeClient(dock)
        , m_mode(mode)
    { }

    virtual ~FormModeDockGuiClient() {}

    virtual Core::IMode *preferredMode(Core::ScreenMode /*screenMode*/) const
    { return m_mode; }

    virtual bool isEnabledForMode(Core::IMode *mode) const
    { return m_mode == mode; }

private:
    Core::IMode *m_mode;
};

class FormModeToolBarGuiClient: public Core::ToolBarModeClient
{
public:
    FormModeToolBarGuiClient(Core::MainToolBar *toolBar, Core::IMode *mode)
        : Core::ToolBarModeClient(toolBar)
        , m_mode(mode)
    { }

    virtual ~FormModeToolBarGuiClient() {}

    virtual Core::IMode *preferredMode(Core::ScreenMode /*screenMode*/) const
    { return m_mode; }

    virtual bool isEnabledForMode(Core::IMode *mode) const
    { return m_mode == mode; }

private:
    Core::IMode *m_mode;
};

class FormEditorUiProvider: public UiProviderInterface
{
public:
    FormEditorUiProvider(QwCustomDockWidget *dock, Core::IMode *formEditorMode)
        : UiProviderInterface(dock)
        , m_formEditorMode(formEditorMode)
    { }

    virtual Core::DockWidgetHandler *dockWidgetHandler() {
        return new Core::DockWidgetHandler(m_dock, new FormModeDockGuiClient(m_dock, m_formEditorMode));
    }

private:
    Core::IMode *m_formEditorMode;
};

class FormEditorToolBarUiProvider: public UiProviderInterface
{
public:
    FormEditorToolBarUiProvider(Core::MainToolBar *toolBar, Core::IMode *formEditorMode)
        : UiProviderInterface(0, toolBar)
        , m_formEditorMode(formEditorMode)
    { }

    virtual Core::ToolBarHandler *toolBarHandler() {
        return new Core::ToolBarHandler(m_toolBar, new FormModeToolBarGuiClient(m_toolBar, m_formEditorMode));
    }

private:
    Core::IMode *m_formEditorMode;
};

//////////////////////////////////////////////////////////////////////////////////////
/// FormEditorPlugin
FormEditorPlugin::FormEditorPlugin()
    : IDocumentPlugin()
    , m_formeditor(QDesignerComponents::createFormEditor(0))
{
    Core::MimeDatabase *mimeDatabase = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDatabase->addMimeTypes(QLatin1String(":/formeditor/formeditor.mimetypes.xml"), &errorMessage);

    m_formEditorMode = new FormEditorMode;
    Core::ModeManager::registerMode(m_formEditorMode);

    m_formeditor->setTopLevel(Core::Storage::mainWindow()->widget());

    initDesignerInterfaces();
    m_fwm = m_formeditor->formWindowManager();
    setupEditorActions();
}

void FormEditorPlugin::initialize()
{
    Core::CommandManager::getInstance()->registerCommands( tr("FormEditorCommands"),
                                                           QList<Core::Command *>() << m_cmdUndo
                                                                                    << m_cmdRedo
                                                                                    << m_cmdCut
                                                                                    << m_cmdCopy
                                                                                    << m_cmdPaste
                                                                                    << m_cmdSelectAll );

    connect(DocumentManager::getInstance(), SIGNAL(currentDocumentChanged(IDocument*)), this, SLOT(onCurrentDocumentChanged(IDocument*)));

    ProjectExplorer::JsonWizardFactory::registerPageFactory(new Designer::Internal::FormPageFactory(m_formeditor));

    Core::IWizardFactory *wizard = new Designer::Internal::FormClassWizard(m_formeditor);
    wizard->setWizardKind(Core::IWizardFactory::ClassWizard);
    wizard->setCategory(QLatin1String(Core::Constants::WIZARD_CATEGORY_QT));
    wizard->setDisplayCategory(QCoreApplication::translate("Core", Core::Constants::WIZARD_TR_CATEGORY_QT));
    wizard->setDisplayName(tr("Qt Designer Form Class"));
    wizard->setId(QLatin1String("C.FormClass"));
    wizard->setIcon(QIcon(":/formeditor/images/qt_ui.png"));
    wizard->setDescription(tr("Creates a Qt Designer form along with a matching class (C++ header and source file) "
                                       "for implementation purposes. You can add the form and class to an existing Qt Widget Project."));
    Core::WizardManager::registerWizardFactory(wizard);
}

void FormEditorPlugin::updateEditorActions()
{ }

void FormEditorPlugin::fillEditMenu( QMenu *menu )
{
    menu->addAction(m_cmdUndo->action());
    menu->addAction(m_cmdRedo->action());
    menu->addSeparator();
    menu->addAction(m_cmdCut->action());
    menu->addAction(m_cmdCopy->action());
    menu->addAction(m_cmdPaste->action());
    menu->addSeparator();
    menu->addAction(m_cmdSelectAll->action());
}

Core::IMode *FormEditorPlugin::mode() const
{
    return m_formEditorMode;
}

bool FormEditorPlugin::activateDocumentOnMouseEnterEvent() const
{
    return true;
}

QList<UiProviderInterface *> FormEditorPlugin::createUiProviders(IMainWindow *mainWindow)
{
    QMainWindow *mw = mainWindow->widget();

    QwCustomDockWidget *widgetBoxDock = new QwCustomDockWidget( mw, tr("Widget Box"), "x-designer/widgetbox",
                                                                QKeySequence(), QIcon(":/formeditor/images/fe_wb.png"),
                                                                Qt::LeftDockWidgetArea, QwCustomDockWidget::First, QList<QAction *>(), m_wbFilter );
    widgetBoxDock->setWidget(m_designerSubWindows[WidgetBoxSubWindow]);

    QwCustomDockWidget *objectInspectorDock = new QwCustomDockWidget( mw, tr("Object Inspector"), "x-designer/objectinspector",
                                                                      QKeySequence(), QIcon(":/formeditor/images/fe_oi.png"),
                                                                      Qt::RightDockWidgetArea, QwCustomDockWidget::First );
    if ( QToolBar *tb = qobject_cast<QToolBar *>(m_peToolBar) )
        tb->setIconSize(QSize(20, 20));
    objectInspectorDock->setWidget(m_designerSubWindows[ObjectInspectorSubWindow]);

    QwCustomDockWidget *propertiesDock = new QwCustomDockWidget( mw, tr("Properties"), "x-designer/propertyeditor",
                                                                 QKeySequence(), QIcon(":/formeditor/images/fe_p.png"),
                                                                 Qt::RightDockWidgetArea, QwCustomDockWidget::Second, QList<QAction *>(), m_peToolBar  );
    propertiesDock->setWidget(m_designerSubWindows[PropertyEditorSubWindow]);

    if ( QToolBar *tb = qobject_cast<QToolBar *>(m_seToolBar) )
        tb->setIconSize(QSize(20, 20));
    QwCustomDockWidget *signalSlotDock = new QwCustomDockWidget( mw, tr("Signal and slots"), "x-designer/signalsloteditor",
                                                                 QKeySequence(), QIcon(":/formeditor/images/fe_ss.png"),
                                                                 Qt::BottomDockWidgetArea, QwCustomDockWidget::First, QList<QAction *>(), m_seToolBar );
    signalSlotDock->setWidget(m_designerSubWindows[SignalSlotEditorSubWindow]);

    if ( QToolBar *tb = qobject_cast<QToolBar *>(m_aeToolBar) ) {
        tb->setIconSize(QSize(20, 20));
        if ( QWidget *w = tb->widgetForAction(tb->actions().last()) ) {
            if ( QLayout *l = w->layout() ) {
                l->setSpacing(0);
                l->setContentsMargins(0, 0, 0, 0);
            }
        }
    }
    QwCustomDockWidget *actionsDock = new QwCustomDockWidget( mw, tr("Actions"), "x-designer/actioneditor",
                                                              QKeySequence(), QIcon(":/formeditor/images/fe_a.png"),
                                                              Qt::BottomDockWidgetArea, QwCustomDockWidget::Second, QList<QAction *>(), m_aeToolBar);
    actionsDock->setWidget(m_designerSubWindows[ActionEditorSubWindow]);

    QActionGroup *modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);
    QObject::connect(modeGroup, &QActionGroup::triggered,
            [this](QAction *a) { activateEditMode(a->data().toInt()); });

    QAction *actEditMode        = new QAction(QDesignerFormEditorInterface::createIcon("widgettool.png"),
                                              tr("Edit Widgets"), modeGroup);
    QAction *actSignaSlotlMode  = new QAction(QDesignerFormEditorInterface::createIcon("signalslottool.png"),
                                              tr("Edit Signals/Slots"), modeGroup);
    QAction *actBuddyMode       = new QAction(QDesignerFormEditorInterface::createIcon("buddytool.png"),
                                              tr("Edit Buddies"), modeGroup);
    QAction *actTabOrderMode    = new QAction(QDesignerFormEditorInterface::createIcon("tabordertool.png"),
                                              tr("Edit Tab Order"), modeGroup);
    actEditMode->setCheckable(true);
    actEditMode->setData(WidgetEditor);
    actSignaSlotlMode->setCheckable(true);
    actSignaSlotlMode->setData(SignalsSlotEditor);
    actBuddyMode->setCheckable(true);
    actBuddyMode->setData(BuddyEditor);
    actTabOrderMode->setCheckable(true);
    actTabOrderMode->setData(TabOrderEditor);

    Core::MainToolBar *toolBar = new Core::MainToolBar(tr("Designer Tool"));
    toolBar->setObjectName("x-designer/current-tool");
    toolBar->addAction(actEditMode);
    toolBar->addAction(actSignaSlotlMode);
    toolBar->addAction(actBuddyMode);
    toolBar->addAction(actTabOrderMode);

    actEditMode->setChecked(true);

    return QList<UiProviderInterface *>() << new FormEditorUiProvider(widgetBoxDock, m_formEditorMode)
                                          << new FormEditorUiProvider(objectInspectorDock, m_formEditorMode)
                                          << new FormEditorUiProvider(propertiesDock, m_formEditorMode)
                                          << new FormEditorUiProvider(signalSlotDock, m_formEditorMode)
                                          << new FormEditorUiProvider(actionsDock, m_formEditorMode)
                                          << new FormEditorToolBarUiProvider(toolBar, m_formEditorMode);
}

QDesignerFormWindowManagerInterface *FormEditorPlugin::formWindowManager()
{
    return m_fwm;
}

TextEditor::IAssistProvider *FormEditorPlugin::createQuickFixProvider()
{
    return 0;
}

QStringList FormEditorPlugin::settingsKeyList() const
{
    return QStringList();
}

ISettings *FormEditorPlugin::createSettings( const QString &settingsKey ) const
{
    Q_UNUSED(settingsKey)
    return 0;
}

void FormEditorPlugin::onCurrentDocumentChanged(IDocument *document)
{
    FormDocument *fd = qobject_cast<FormDocument *>(document);
    m_fwm->setActiveFormWindow(fd ? fd->formWindow() : 0);
}

void FormEditorPlugin::activateEditMode(int id)
{
    for (int i = 0; i < m_fwm->formWindowCount(); i++)
        m_fwm->formWindow(i)->setCurrentTool(id);
}

void FormEditorPlugin::setupEditorActions()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdUndo               = new Core::Command( m_fwm->actionUndo(),
                                                 tr("Undo"),
                                                 QKeySequence(tr("Ctrl+Z")),
                                                 mainWindow );

    m_cmdRedo               = new Core::Command( m_fwm->actionRedo(),
                                                 tr("Redo"),
                                                 QKeySequence(tr("Ctrl+Y")),
                                                 mainWindow );

    m_cmdCut                = new Core::Command( m_fwm->actionCut(),
                                                 tr("Cut"),
                                                 QKeySequence(QKeySequence::Cut),
                                                 mainWindow );

    m_cmdCopy               = new Core::Command( m_fwm->actionCopy(),
                                                 tr("Copy"),
                                                 QKeySequence(QKeySequence::Copy),
                                                 mainWindow );

    m_cmdPaste              = new Core::Command( m_fwm->actionPaste(),
                                                 tr("Paste"),
                                                 QKeySequence(QKeySequence::Paste),
                                                 mainWindow );

    m_cmdSelectAll          = new Core::Command( m_fwm->actionSelectAll(),
                                                 tr("SelectAll"),
                                                 QKeySequence(QKeySequence::SelectAll),
                                                 mainWindow );

    updateEditorActions();
}

void FormEditorPlugin::initDesignerInterfaces()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDesignerComponents::createTaskMenu(m_formeditor, this);
    QDesignerComponents::initializePlugins(m_formeditor);
    QDesignerComponents::initializeResources();
    initDesignerSubWindows();
    /// TODO: QtCreatorIntegration
    m_integration = new DesignerIntegration(m_formeditor, this, this);
    m_formeditor->setIntegration(m_integration);

    /**
     * This will initialize our TabOrder, Signals and slots and Buddy editors.
     */
    QList<QObject*> plugins = QPluginLoader::staticInstances();
    plugins += m_formeditor->pluginInstances();
    foreach (QObject *plugin, plugins) {
        if (QDesignerFormEditorPluginInterface *formEditorPlugin = qobject_cast<QDesignerFormEditorPluginInterface*>(plugin)) {
            if (!formEditorPlugin->isInitialized())
                formEditorPlugin->initialize(m_formeditor);
        }
    }

    QApplication::restoreOverrideCursor();
}

void FormEditorPlugin::initDesignerSubWindows()
{
    qFill(m_designerSubWindows, m_designerSubWindows + DesignerSubWindowCount, static_cast<QWidget*>(0));

    m_wbFilter = 0;
    QDesignerWidgetBoxInterface *wb = QDesignerComponents::createWidgetBox(m_formeditor, 0);
    wb->setWindowTitle(tr("Widget Box"));
    wb->setObjectName(QLatin1String("WidgetBox"));
    m_formeditor->setWidgetBox(wb);
    wb->setStyleSheet("QWidget { border: 0px; }");
    m_designerSubWindows[WidgetBoxSubWindow] = wb;
    if ( QLayout *l = wb->layout() ) {
        if ( QWidget *w = l->itemAt(0)->widget() ) {
            l->removeWidget(w);
            m_wbFilter = w;
        }
    }

    QDesignerObjectInspectorInterface *oi = QDesignerComponents::createObjectInspector(m_formeditor, 0);
    oi->setWindowTitle(tr("Object Inspector"));
    oi->setObjectName(QLatin1String("ObjectInspector"));
    m_formeditor->setObjectInspector(oi);
    oi->setStyleSheet("QWidget { border: 0px; }");
    m_designerSubWindows[ObjectInspectorSubWindow] = oi;

    m_peToolBar = 0;
    QDesignerPropertyEditorInterface *pe = QDesignerComponents::createPropertyEditor(m_formeditor, 0);
    pe->setWindowTitle(tr("Property Editor"));
    pe->setObjectName(QLatin1String("PropertyEditor"));
    m_formeditor->setPropertyEditor(pe);
    pe->setStyleSheet("QStackedWidget { border: 0px; }"
                      "QtButtonPropertyBrowser { border: 0px; }"
                      "QtPropertyEditorView { border: 0px; }");
    m_designerSubWindows[PropertyEditorSubWindow] = pe;
    if ( QLayout *l = pe->layout() ) {
        if ( QWidget *w = l->itemAt(0)->widget() ) {
            l->removeWidget(w);
            m_peToolBar = w;
        }
    }

    m_seToolBar = 0;
    QWidget *se = QDesignerComponents::createSignalSlotEditor(m_formeditor, 0);
    se->setWindowTitle(tr("Signals && Slots Editor"));
    se->setObjectName(QLatin1String("SignalsAndSlotsEditor"));
    se->setStyleSheet("QWidget { border: 0px; }");
    m_designerSubWindows[SignalSlotEditorSubWindow] = se;
    if ( QLayout *l = se->layout() ) {
        if ( QWidget *w = l->itemAt(0)->widget() ) {
            l->removeWidget(w);
            m_seToolBar = w;
        }
    }

    m_aeToolBar = 0;
    QDesignerActionEditorInterface *ae = QDesignerComponents::createActionEditor(m_formeditor, 0);
    ae->setWindowTitle(tr("Action Editor"));
    ae->setObjectName(QLatin1String("ActionEditor"));
    m_formeditor->setActionEditor(ae);
    ae->setStyleSheet("QWidget { border: 0px; }");
    m_designerSubWindows[ActionEditorSubWindow] = ae;
    if ( QLayout *l = ae->layout() ) {
        if ( QWidget *w = l->itemAt(0)->widget() ) {
            l->removeWidget(w);
            m_aeToolBar = w;
        }
    }
}

FormEditorPlugin::~FormEditorPlugin()
{
}

bool FormEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();

    return ( ext == QLatin1String("UI") );
}

IDocument *FormEditorPlugin::createDocument()
{
    FormDocument *document = new FormDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(FormEditorPlugin, FormEditorPlugin)
#endif
