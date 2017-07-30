#ifndef RICHEDITORPLUGIN_H
#define RICHEDITORPLUGIN_H

#include "IDocument.h"

namespace RichEditor {
class RichEditorWidget;
}

class RichDocument : public IDocument
{
    Q_OBJECT

public:
    RichDocument( IDocumentPlugin *docPlugin );
    virtual ~RichDocument();

    virtual QWidget *editorWidget() const;
    virtual QWidget *widget() const;
    virtual Core::IEditor *editor() const;
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

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

    virtual void onFormatSettingsChanged();
    virtual void emitDocumentOpened( DocumentManager *documentManager );
    virtual void emitDocumentClosed( DocumentManager *documentManager );

    virtual void setTextCodec( QTextCodec *codec );

    virtual void saveState( QDomDocument *document, QDomElement *element ) const;
    virtual void restoreState( QDomElement *element );

private slots:
    void onModificationChanged();

protected:
    QFile       *m_file;
    RichEditor::RichEditorWidget *m_editor;
    QString      m_fileName;

private:
    IDocumentPlugin *m_documentPlugin;
    QMainWindow *m_editorMainWidget;
};

class RichEditorPlugin : public IDocumentPlugin
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    RichEditorPlugin();
    virtual ~RichEditorPlugin();

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("html"); }
    virtual QString description() const { return QObject::tr("Rich-text File"); }

    virtual void initialize();

    virtual void updateEditorActions();
    virtual void fillEditMenu(  QMenu *menu );

protected:
    virtual TextEditor::IAssistProvider *createQuickFixProvider();

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;
};

#endif // RICHEDITORPLUGIN_H
