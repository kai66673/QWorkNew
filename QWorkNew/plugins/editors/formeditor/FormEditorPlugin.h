#ifndef FORMEDITORPLUGIN_H
#define FORMEDITORPLUGIN_H

#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerIntegration>

#include "IPlugin.h"
#include "IDocument.h"
#include "widgethost.h"

class DesignerIntegration;
class FormEditorPlugin;

class FormDocument : public IDocument
{
    Q_OBJECT

public:
    FormDocument( FormEditorPlugin *docPlugin );
    virtual ~FormDocument();

    virtual QWidget *editorWidget() const;
    virtual QWidget *widget() const;
    virtual Core::IEditor *editor() const;
    virtual DocumentToolBar *toolBar();
    virtual bool newDocument ( const QString &fileName );
    virtual bool newDocumentFromFile(const QString &fileName, const QString &contentFilePath);
    virtual bool openDocument( const QString &filePath );
    virtual bool saveDocument();
    virtual bool saveAsDocument( const QString &filePath );
    virtual bool saveToDocument( const QString &filePath );
    virtual bool renameTo(const QString &newFilePath);

    virtual QString title() const;
    virtual QString toolTip() const;
    virtual QString filePath() const;

    virtual bool isModified() const;
    virtual void setModified(bool onoff);
    virtual bool isNew() const { return (m_file == NULL); }

    virtual void setCursor( IDocumentCursor *position );

    virtual IDocumentPlugin *documentPlugin() const;

    virtual void onFormatSettingsChanged();
    virtual void emitDocumentOpened( DocumentManager *documentManager );
    virtual void emitDocumentClosed( DocumentManager *documentManager );

    virtual void setTextCodec( QTextCodec *codec );

    virtual void saveState( QDomDocument *document, QDomElement *element ) const;
    virtual void restoreState( QDomElement *element );
    QDesignerFormWindowInterface *formWindow();

public slots:
    virtual void onModificationChanged();
    void onActiveFormWindowChanged(QDesignerFormWindowInterface *fw);

private slots:
    void onActivate();

protected:
    QFile       *m_file;
    QString      m_fileName;

private:
    FormEditorPlugin *m_documentPlugin;
    SharedTools::WidgetHost *m_host;
    QDesignerFormWindowInterface *m_form;
    DocumentToolBar *m_toolBar;
    QWidget *m_mainWidget;
};

class FormEditorPlugin : public IDocumentPlugin
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    FormEditorPlugin();
    virtual ~FormEditorPlugin();

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("ui"); }
    virtual QString description() const { return QObject::tr("Qt Designer Form File"); }

    virtual void initialize();

    virtual void updateEditorActions();
    virtual void fillEditMenu(  QMenu *menu );
    virtual Core::IMode *mode() const;
    virtual bool activateDocumentOnMouseEnterEvent() const;

    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

    QDesignerFormWindowManagerInterface *formWindowManager();

protected:
    virtual TextEditor::IAssistProvider *createQuickFixProvider();

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

private slots:
    void onCurrentDocumentChanged(IDocument *document);
    void activateEditMode(int id);

private:
    void setupEditorActions();

    Core::Command *m_cmdUndo;
    Core::Command *m_cmdRedo;
    Core::Command *m_cmdCut;
    Core::Command *m_cmdCopy;
    Core::Command *m_cmdPaste;
    Core::Command *m_cmdSelectAll;

private:
    QDesignerFormEditorInterface *m_formeditor;
    QDesignerFormWindowManagerInterface *m_fwm;
    SharedTools::WidgetHost *m_host;
    DesignerIntegration *m_integration;

private:
    enum DesignerSubWindows
    {
        WidgetBoxSubWindow,
        ObjectInspectorSubWindow,
        PropertyEditorSubWindow,
        SignalSlotEditorSubWindow,
        ActionEditorSubWindow,
        DesignerSubWindowCount
    };

    enum FormEditMode {
        WidgetEditor,
        SignalsSlotEditor,
        BuddyEditor,
        TabOrderEditor
    };


    void initDesignerInterfaces();
    void initDesignerSubWindows();

    QWidget *m_designerSubWindows[DesignerSubWindowCount];
    Core::IMode *m_formEditorMode;

    QWidget *m_seToolBar;
    QWidget *m_wbFilter;
    QWidget *m_peToolBar;
    QWidget *m_aeToolBar;
};

#endif // FORMEDITORPLUGIN_H
