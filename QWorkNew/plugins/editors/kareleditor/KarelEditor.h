#ifndef KARELEDITOR_H
#define KARELEDITOR_H

#include <QFutureWatcher>
#include <QTimer>
#include <QComboBox>

#include <texteditor.h>

#include "KarelEditorPlugin.h"
#include "KarelHighlighter.h"
#include "KarelSemanticHighlighter.h"
#include "KarelOverviewModel.h"

namespace Find {
    class SearchResultItem;
} // namespace Find

namespace Karel {
namespace Editor {

class KarelEditorWidget;

class KarelEditor: public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    KarelEditor(KarelEditorWidget *editor);

    Core::Context context() const;

    bool duplicateSupported() const { return false; }
    Core::IEditor *duplicate(QWidget *parent) { Q_UNUSED(parent); return 0; }
    QString id() const;

    bool isTemporary() const { return false; }
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);

private:
    Core::Context m_context;
};

class KarelEditorWidget: public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    KarelEditorWidget(KarelDocument *karelDocument, IDocumentPlugin *editorPlugin, QToolBar *toolBar, QWidget *parent);
    virtual ~KarelEditorWidget();

    void stopSemanticHiglighter();

    virtual void onSettingsChanged();
    void onSemanticFormatSettingsChanged();

    void unCommentSelection();

    unsigned editorRevision() const;

    virtual TextEditor::IAssistInterface *createAssistInterface(TextEditor::AssistKind kind,
                                                                TextEditor::AssistReason reason) const;

Q_SIGNALS:
    void outlineModelIndexChanged(const QModelIndex &index);

private Q_SLOTS:
    void updateOutlineNow();
    void updateOutlineIndex();
    void updateOutlineIndexNow();
    void jumpToOutlineElement(int);
    void openEditor(const Find::SearchResultItem &item);

public Q_SLOTS:
    void updateSemanticInfo( const Karel::SemanticInfo &semanticInfo );
    void highlightSymbolUsages(int from, int to);
    void finishHighlightSymbolUsages();
    void updateUses();
    void updateUsesNow();
    void jumpToDefinition();
    void findUsages();
    void replaceUsages();

protected:
    TextEditor::BaseTextEditor *createEditor();

private:
    void createToolBar(KarelEditor *editable);
    QModelIndex outlineModelIndex();
    QModelIndex indexForPosition(int pos, const QModelIndex &rootIndex = QModelIndex()) const;
    void semanticRehighlight();
    SemanticHighlighter::Source currentSource( bool force = false );
    void updateSemanticDiagnosticMessages( Document::Ptr doc );

    Core::ILink *findLinkAt(const QTextCursor &tc, bool = true);
    Scope *scopeUnderIdentifier(Document::Ptr doc, const QTextCursor &tc, bool &isStatementsArea, Token &tk);
    Core::ILink *linkToSymbol(const Symbol *symbol, TranslationUnit *unit, int beginOfToken, int endOfToken);
    void findOrReplaceUsages(bool isReplace);
    void showUsages(const Identifier *id, const QList<const Token *> tokens);
    void showReplaceUsagesDialog(const Identifier *id, const QList<const Token *> tokens);

    IDocumentPlugin *m_editorPlugin;

    KarelDocument *m_karelDocument;
    KarelHighlighter *m_karelHighlighter;
    SemanticHighlighter *m_semanticHighlighter;
    QTimer *m_updateUsesTimer;
    QFuture<SemanticInfo::Use> m_highlighter;
    QFutureWatcher<SemanticInfo::Use> m_highlightWatcher;
    SemanticInfo m_lastSemanticInfo;
    unsigned m_highlightRevision;
    int m_nextHighlightBlockNumber;
    QTextCharFormat m_diagnosticMessageFormat;
    QTextCharFormat m_errorFormat;
    QTextCharFormat m_warningFormat;

    enum SemanticFormats {
        SemanticUserType = 0,
        SemanticConstant,
        SemanticVariable,
        SemanticField,
        SemanticRoutine,
        SemanticProgramName,
        SemanticFormatsCount
    };
    QTextCharFormat m_semanticFormats[SemanticFormatsCount];

    QToolBar *m_toolBar;
    QComboBox *m_outlineCombo;
    OverviewModel *m_outlineModel;
    QTimer *m_updateOutlineTimer;
    QTimer *m_updateOutlineIndexTimer;
    QModelIndex m_outlineModelIndex;
};

}   // namespace Editor
}   // namespace Karel

#endif // KARELEDITOR_H
