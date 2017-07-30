#ifndef SQLCODECOMPLETION_H
#define SQLCODECOMPLETION_H

#include <QObject>

#include "ASTVisitor.h"
#include "AST.h"
#include "TranslationUnit.h"
#include <texteditor/icompletioncollector.h>
#include "SQLEditor.h"

namespace Sql {

namespace Editor {

class SqlCodeCompletion;

class SqlStatementUnderCursor: protected ASTVisitor
{
public:
    SqlStatementUnderCursor( SqlCodeCompletion *completer, TextEditor::ITextEditor *editor );

    void getTablesForExpression( const QString &expr, QStringList &tables );

    inline bool isUnknownStatement() const { return m_UnknownStatement; }
    inline bool isDmlStatement() const { return m_DmlStatement; }
    inline bool isDdlStatement() const { return m_DdlStatement; }
    inline bool isBlockStatement() const { return m_BlockStatement; }
    inline bool isCommentStatement() const { return m_CommentStatement; }

    inline DdlStatementAST::ObjectKind ddlObjectKind() const { return m_DdlObjectkind; }

private:
    bool init();
    bool parse();

private:
    SQLEditor *m_editor;
    SQLEditorWidget *m_editorWidget;
    QMap<QString, QString> m_tableAliasesToNames;
    int m_startStatementPosition;

    bool m_UnknownStatement;
    bool m_DmlStatement;
    bool m_DdlStatement;
    bool m_BlockStatement;
    bool m_CommentStatement;

    DdlStatementAST::ObjectKind m_DdlObjectkind;
    SqlCodeCompletion *m_completer;

protected:
    bool visit( DirectSingleSourceAST *ast );
};

class SqlCodeCompletion : public TextEditor::ICompletionCollector
{
public:
    SqlCodeCompletion( QObject *parent = 0 );

    TextEditor::ITextEditor *editor() const;
    int startPosition() const;
    bool shouldRestartCompletion();
    QList<TextEditor::CompletionItem> getCompletions();
    bool supportsPolicy( TextEditor::CompletionPolicy policy ) const;
    bool supportsEditor( TextEditor::ITextEditor *editor ) const;
    bool triggersCompletion( TextEditor::ITextEditor *editor );
    int  startCompletion( TextEditor::ITextEditor *editor );
    void completions( QList<TextEditor::CompletionItem> *completions );

    bool typedCharCompletes( const TextEditor::CompletionItem &item, QChar typedChar );
    void complete( const TextEditor::CompletionItem &item, QChar typedChar );
    bool partiallyComplete( const QList<TextEditor::CompletionItem> &completionItems );
    void cleanup();

    void emitGetTables( QVariantList &values );

signals:
    void getTablesAndViews( QVariantList &values );
    void getTables( QVariantList &values );
    void getViews( QVariantList &values );
    void getIndices( QVariantList &values );
    void getTriggers( QVariantList &values );
    void getTableFields( QVariantList &values );

private:
    int  startCompletionHelper( TextEditor::ITextEditor *editor );
    int  findStartOfName( int pos = -1, bool *afterPoint = 0 );
    void addKeywords();
    void addTablesAndViews();
    void addTables();
    void addViews();
    void addIndices();
    void addTriggers();
    void addFields( SqlStatementUnderCursor *statement );

private:
    TextEditor::ITextEditor *m_editor;
    int  m_startPosition;
    bool m_shouldRestartCompletion;
    bool m_automaticCompletion;
    unsigned m_completionOperator;
    QString m_expr;

    QList<TextEditor::CompletionItem> m_completions;
};

} // namespace Editor
} // namespace Sql

#endif // SQLCODECOMPLETION_H
