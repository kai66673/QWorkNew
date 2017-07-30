#ifndef SQLEDITOR_H
#define SQLEDITOR_H

#include <QFutureWatcher>
#include <QTimer>

#include <texteditor.h>
#include <icontext.h>

#include "SqlSemanticHighlighter.h"

namespace Sql {
namespace Editor {

class SQLEditorWidget;
class SqlHighlighter;

class SQLEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    SQLEditor(SQLEditorWidget *);

    Core::Context context() const;

    bool duplicateSupported() const { return false; }
    Core::IEditor *duplicate(QWidget *parent) { Q_UNUSED(parent); return 0; }
    QString id() const;

    bool isTemporary() const { return false; }
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);

private:
    Core::Context m_context;
};

class SQLEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    SQLEditorWidget(SqlDocument *sqlDocument, QWidget *parent );
    ~SQLEditorWidget();

    void stopSemanticHiglighter();

    virtual void onSettingsChanged();
    void onSemanticFormatSettingsChanged();

    unsigned editorRevision() const;

    void rehighlight( bool force = false );

    void unCommentSelection();
    Core::ILink *findLinkAt(const QTextCursor &tc, bool resolveTarget = true );

public Q_SLOTS:
    void onParserEngineChanged();
    void updateSemanticInfo( const Sql::SemanticInfo &semanticInfo );
    void updateUses();
    void updateUsesNow();

    void highlightSymbolUsages(int from, int to);
    void finishHighlightSymbolUsages();

protected:
    TextEditor::BaseTextEditor *createEditor();

private:
    SqlHighlighter *m_sqlHighlighter;
    SqlDocument *m_sqlDocument;
    SemanticHighlighter *m_semanticHighlighter;
    SemanticInfo m_lastSemanticInfo;
    QFuture<CheckName::Use *> m_highlighter;
    QFutureWatcher<CheckName::Use *> m_highlightWatcher;
    unsigned m_highlightRevision;
    IParserEngine *m_highlightEngine;
    QTimer *m_updateUsesTimer;
    int m_nextHighlightBlockNumber;

    void semanticRehighlight();
    SemanticHighlighter::Source currentSource( bool force = false );
    void updateSemanticDiagnosticMessages( Document::Ptr doc );

    enum SqlSemanticFormats {
        SqlSemanticTable = 0,
        SqlSemanticAlias,
        SqlSemanticColumn,
        SqlSemanticIndex,
        SqlSemanticTrigger,
        SqlSemanticConstraint,
        SqlSemanticBind,
        SqlSemanticSchema,
        SqlSemanticIdentifier,
        SqlSemanticLabel,
        SqlSemanticFunction,
        SqlSemanticCursor,
        SqlSemanticMap,
        SqlSemanticArea,
        SqlSemanticModule,
        SqlSemanticFormatsCount
    };
    QTextCharFormat m_sqlSemanticFormats[SqlSemanticFormatsCount];

    QTextCharFormat m_diagnosticMessageFormat;
    QTextCharFormat m_errorFormat;
    QTextCharFormat m_warningFormat;
};

} // namespace Editor
} // namespace Sql

#endif // SQLEDITOR_H
