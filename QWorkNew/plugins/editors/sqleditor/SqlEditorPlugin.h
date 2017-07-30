#ifndef SQLEDITORPLUGIN_H
#define SQLEDITORPLUGIN_H

#include <QFile>
#include <QFuture>

#include "SqlDocument.h"
#include "DbMetadataProvider.h"
#include "PlainTextDocumentPlugin.h"
#include "IBaseTextDocument.h"

QT_BEGIN_NAMESPACE
class QTabWidget;
class QComboBox;
class QAction;
QT_END_NAMESPACE

namespace Sql {
    namespace Editor {
        class SQLEditorWidget;
        class SqlCodeCompletion;
    }
}

class SqlDocument: public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    SqlDocument( IDocumentPlugin *docPlugin );
    virtual ~SqlDocument();

    virtual QWidget *widget() const;

    Sql::Document::Ptr sqlDocument();
    QFuture<void> refresh();

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    Sql::Editor::SQLEditorWidget *m_editor;

private:
    IDocumentPlugin *m_documentPlugin;
    QWidget     *m_mainWidget;
    Sql::Document::Ptr m_sqlDocument;
};

class SqlEditorPlugin : public PlainTextDocumentPlugin, public IDocumentHelper
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    SqlEditorPlugin();
    virtual ~SqlEditorPlugin() {}

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("SQL"); }
    virtual QString description() const { return QObject::tr("SQL-Script File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

    virtual bool documentEventFilter( IDocument *document, QEvent *event );

    virtual void initialize();
    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;

    virtual void updateEditorActions();
    virtual void fillEditMenu( QMenu *menu );

private slots:
    void toggleComments();

private:
    Sql::DbMetadataProvider *m_metadataProvider;
    Core::Command *m_cmdToggleComments;
};

#endif // SQLEDITORPLUGIN_H
