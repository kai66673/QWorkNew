#ifndef GRAMMAREDITORWIDGET_H
#define GRAMMAREDITORWIDGET_H

#include <QComboBox>
#include <QToolBar>
#include <QtCore/QFutureWatcher>
#include <QWaitCondition>
#include <QThread>

#include <texteditor.h>
#include <ModelManagerInterface.h>
#include "Symbol.h"
#include "cppsemanticinfo.h"
#include "searchresultwindow.h"
#include <FindUsages.h>

#include "uncommentselection.h"
#include "GrammarHighlighter.h"
#include "GrammarRulesModel.h"
#include "cppvirtualfunctionassistprovider.h"

namespace Grammar {

class EditorWidget;

class GrammarEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit GrammarEditor( EditorWidget *editorWidget );
    virtual ~GrammarEditor();

    bool duplicateSupported() const { return false; }
    Core::IEditor *duplicate(QWidget *parent);

    QString id() const;
    bool isTemporary() const { return false; }

    /**
      Opens file for editing, actual work performed by base class
      */
    bool open(QString *errorString,
              const QString &fileName,
              const QString &realFileName);
};

class SemanticHighlighter: public QThread, CPlusPlus::TopLevelDeclarationProcessor
{
    Q_OBJECT

public:
    SemanticHighlighter(QObject *parent = 0);
    virtual ~SemanticHighlighter();

    virtual bool processDeclaration(CPlusPlus::DeclarationAST *) { return m_done; }

    void abort();

    struct Source
    {
        CPlusPlus::Snapshot snapshot;
        QString fileName;
        QString code;
        int line;
        int column;
        int revision;
        bool force;

        Source()
            : line(0), column(0), revision(0), force(false)
        { }

        Source(const CPlusPlus::Snapshot &snapshot,
               const QString &fileName,
               const QString &code,
               int line, int column,
               int revision)
            : snapshot(snapshot), fileName(fileName),
              code(code), line(line), column(column),
              revision(revision), force(false)
        { }

        void clear()
        {
            snapshot = CPlusPlus::Snapshot();
            fileName.clear();
            code.clear();
            line = 0;
            column = 0;
            revision = 0;
            force = false;
        }
    };

    CppEditor::Internal::SemanticInfo semanticInfo(const Source &source);

    void rehighlight(const Source &source);

Q_SIGNALS:
    void changed(const CppEditor::Internal::SemanticInfo &semanticInfo);

protected:
    virtual void run();

private:
    bool isOutdated();

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_done;
    Source m_source;
    CppEditor::Internal::SemanticInfo m_lastSemanticInfo;
};

class EditorWidget: public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    EditorWidget(IDocument *document, QToolBar *toolBar, QWidget *parent = 0);
    virtual ~EditorWidget();

    virtual void unCommentSelection();
    virtual void onSettingsChanged();
    void onSemanticFormatSettingsChanged();

    virtual void paste(); // reimplemented from BaseTextEditorWidget
    virtual void cut(); // reimplemented from BaseTextEditorWidget

    static Core::ILink *linkToSymbol( int begin, int end, CPlusPlus::Symbol *symbol );

    static QVector<QString> highlighterFormatCategories();

    virtual TextEditor::IAssistInterface *createAssistInterface(TextEditor::AssistKind kind,
                                                                TextEditor::AssistReason reason) const;

public slots:
    virtual void setFontSettings( const TextEditor::FontSettings &fs );
    void findUsages();
    void renameUsages();
    void jumpToDefinition();

private slots:
    void jumpToOutlineElement( int index );
    void updateOutlineNow();
    void updateOutlineIndex();
    void updateOutlineIndexNow();
    void updateUses();
    void updateUsesNow();
    void onDocumentUpdated( CPlusPlus::Document::Ptr doc );
    void onContentsChanged( int position, int charsRemoved, int charsAdded );
    void rehighlight( bool force = false );

    void semanticRehighlight();
    void updateSemanticInfo( const CppEditor::Internal::SemanticInfo &semanticInfo );
    void highlightSymbolUsages( int from, int to );
    void finishHighlightSymbolUsages();

    void markSymbolsNow();

    void openEditor( const Find::SearchResultItem &item );
    void replaceButtonClicked( const QString &text, const QList<Find::SearchResultItem> &items );

protected:
    bool event( QEvent *e );
    void keyPressEvent( QKeyEvent * );

    TextEditor::BaseTextEditor *createEditor();

    const CPlusPlus::Macro *findCanonicalMacro( const QTextCursor &cursor,
                                                CPlusPlus::Document::Ptr doc ) const;

private:
    void createToolBar( GrammarEditor *editable );
    QModelIndex outlineModelIndex();
    QModelIndex indexForPosition( int line ) const;

    void markSymbols( const QTextCursor &tc, const CppEditor::Internal::SemanticInfo &info );

    SemanticHighlighter::Source currentSource( bool force = false );
    void highlightUses( const QList<CppEditor::Internal::SemanticInfo::Use> &uses,
                        const CppEditor::Internal::SemanticInfo &semanticInfo,
                        QList<QTextEdit::ExtraSelection> *selections );
    bool checkApplySizeForSemanticHighlighter( int docSize );

    bool renameSymbolUnderCursor();

    void startRename();
    void finishRename();
    void abortRename();

    Core::ILink *attemptFuncDeclDef( const QTextCursor &cursor,
                                     const CPlusPlus::Document::Ptr &doc,
                                     CPlusPlus::Snapshot snapshot ) const;
    CPlusPlus::Symbol *findDefinition( CPlusPlus::Symbol *symbol, const CPlusPlus::Snapshot &snapshot ) const;
    QString identifierUnderCursor( QTextCursor *macroCursor ) const;
    Core::ILink *findMacroLink( int begin, int end, const QByteArray &name ) const;
    Core::ILink *findMacroLink( int begin, int end, const QByteArray &name, CPlusPlus::Document::Ptr doc,
                                const CPlusPlus::Snapshot &snapshot,
                                QSet<QString> *processed ) const;
    Core::ILink *findLinkAt( const QTextCursor &, bool resolveTarget = true );

    void showGrammarUsages( const CPlusPlus::Name *searchName, const QList<CPlusPlus::Usage> &usages );
    void showGrammarUsagesReaplaceDialog( const CPlusPlus::Name *searchName, const QList<CPlusPlus::Usage> &usages );
    void findGrammarUsages( CPlusPlus::Symbol *symbol, const CppEditor::Internal::SemanticInfo &info, bool replaceMode = false );
    void findGrammarSemanticTypeUsages( CPlusPlus::Symbol *symbol, const CppEditor::Internal::SemanticInfo &info, bool replaceMode = false );

    Utils::CommentDefinition m_commentDefinition;
    GrammarHighlighter *m_grammarHighlighter;
    CPlusPlus::CppModelManagerInterface *m_modelManager;

    QToolBar *m_toolBar;
    QComboBox *m_outlineCombo;
    GrammarRulesModel *m_outlineModel;
    QTimer *m_updateOutlineTimer;
    QTimer *m_updateOutlineIndexTimer;
    QModelIndex m_outlineModelIndex;
    QTimer *m_updateUsesTimer;

    SemanticHighlighter *m_semanticHighlighter;
    CppEditor::Internal::SemanticInfo m_lastSemanticInfo;
    QFuture<CppEditor::Internal::SemanticInfo::Use> m_highlighter;
    QFutureWatcher<CppEditor::Internal::SemanticInfo::Use> m_highlightWatcher;
    unsigned m_highlightRevision; // the editor revision that requested the highlight
    int m_nextHighlightBlockNumber;
    bool m_initialized;

    QList<QTextEdit::ExtraSelection> m_renameSelections;
    int m_currentRenameSelection;
    static const int NoCurrentRenameSelection = -1;
    bool m_inRename, m_inRenameChanged, m_firstRenameChange;
    QTextCursor m_currentRenameSelectionBegin;
    QTextCursor m_currentRenameSelectionEnd;

    QFuture<QList<int> > m_references;
    QFutureWatcher<QList<int> > m_referencesWatcher;
    unsigned m_referencesRevision;
    int m_referencesCursorPosition;

    enum CppSemanticFormat {
        CppSemanticOccurrences = 0,
        CppSemanticOccurrencesUnused,
        CppSemanticType,
        CppSemanticLocal,
        CppSemanticField,
        CppSemanticStatic,
        CppSemanticVirtualMethod,
        CppSemanticFunction,
        CppSemanticSqlTableAlias,
        CppSemanticFormatCount,
    };
    QTextCharFormat m_cppSemanticFormats[CppSemanticFormatCount];

    QTextCharFormat m_grammarTermUnusedFormat;
    QTextCharFormat m_occurrenceRenameFormat;

    CppEditor::Internal::VirtualFunctionAssistProvider *m_virtualFunctionAssistProvider;

    QTextCharFormat m_diagnosticMessageFormat;
};

}   // namespace Grammar

#endif // GRAMMAREDITORWIDGET_H
