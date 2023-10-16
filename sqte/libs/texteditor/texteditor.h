#pragma once

#include <QPlainTextEdit>
#include <QPrinter>
#include <QTimeLine>

#include "codeassist/assistenums.h"
#include "codeassist/completionassistprovider.h"

#include "ifindsupport.h"
#include "texteditor_global.h"

namespace Utils {
    class ChangeSet;
}

namespace TextEditor {

class RefactorOverlay;
struct RefactorMarker;
class IAssistMonitorInterface;
class IAssistInterface;
class IAssistProvider;
class ICodeStylePreferences;

class TabSettings;
class FontSettings;
class BehaviorSettings;
class CompletionSettings;
class DisplaySettings;
class TypingSettings;
class StorageSettings;
class ExtraEncodingSettings;

class Indenter;
class AutoCompleter;

namespace Internal {
    class BaseTextEditorPrivate;
    class TextEditorOverlay;
    typedef QList<RefactorMarker> RefactorMarkers;
    typedef QString (TransformationMethod)(const QString &);
}

class ITextMarkable;
class BaseTextDocument;

class TEXTEDITOR_EXPORT BaseTextEditorAnimator : public QObject
{
    Q_OBJECT

public:
    BaseTextEditorAnimator(QObject *parent);

    inline void setPosition(int position) { m_position = position; }
    inline int position() const { return m_position; }

    void setData(QFont f, QPalette pal, const QString &text);

    void draw(QPainter *p, const QPointF &pos);
    QRectF rect() const;

    inline qreal value() const { return m_value; }
    inline QPointF lastDrawPos() const { return m_lastDrawPos; }

    void finish();

    bool isRunning() const;

signals:
    void updateRequest(int position, QPointF lastPos, QRectF rect);


private slots:
    void step(qreal v);

private:
    QTimeLine *m_timeline;
    qreal m_value;
    int m_position;
    QPointF m_lastDrawPos;
    QFont m_font;
    QPalette m_palette;
    QString m_text;
    QSizeF m_size;
};

class TEXTEDITOR_EXPORT TextEditorWidget : public QPlainTextEdit
{
    Q_OBJECT
    Q_PROPERTY(int verticalBlockSelectionFirstColumn READ verticalBlockSelectionFirstColumn)
    Q_PROPERTY(int verticalBlockSelectionLastColumn READ verticalBlockSelectionLastColumn)
public:
    TextEditorWidget(QWidget *parent);
    ~TextEditorWidget();

    const Utils::ChangeSet &changeSet() const;
    void setChangeSet(const Utils::ChangeSet &changeSet);

//    // EditorInterface
//    Core::IFile * file();
    bool createNew(const QString &contents);
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    QByteArray saveState() const;
    bool restoreState(const QByteArray &state);
    QString displayName() const;

//    // ITextEditor

    void gotoLine(int line, int column = 0);
    virtual void refreshDocumentCursor();

//    int position(
//        ITextEditor::PositionOperation posOp = ITextEditor::Current
//        , int at = -1) const;
    void convertPosition(int pos, unsigned *line, unsigned *column) const;

//    BaseTextEditor *editor() const;
    ITextMarkable *markableInterface() const;

    QChar characterAt(int pos) const;

    void print(QPrinter *);

    void setSuggestedFileName(const QString &suggestedFileName);
    QString mimeType() const;
    virtual void setMimeType(const QString &mt);


    void appendStandardContextMenuActions(QMenu *);

    // Works only in conjunction with a syntax highlighter that puts
    // parentheses into text block user data
    void setParenthesesMatchingEnabled(bool b);
    bool isParenthesesMatchingEnabled() const;

    void setHighlightCurrentLine(bool b);
    bool highlightCurrentLine() const;

    void setLineNumbersVisible(bool b);
    bool lineNumbersVisible() const;

    void setMarksVisible(bool b);
    bool marksVisible() const;

    void setRequestMarkEnabled(bool b);
    bool requestMarkEnabled() const;

    void setLineSeparatorsAllowed(bool b);
    bool lineSeparatorsAllowed() const;

    void updateCodeFoldingVisible();
    bool codeFoldingVisible() const;

    void setCodeFoldingSupported(bool b);
    bool codeFoldingSupported() const;

    void setMouseNavigationEnabled(bool b);
    bool mouseNavigationEnabled() const;

    void setScrollWheelZoomingEnabled(bool b);
    bool scrollWheelZoomingEnabled() const;

    void setConstrainTooltips(bool b);
    bool constrainTooltips() const;

    void setRevisionsVisible(bool b);
    bool revisionsVisible() const;

    void setVisibleWrapColumn(int column);
    int visibleWrapColumn() const;

    int columnCount() const;
    int rowCount() const;

    void setActionHack(QObject *);
    QObject *actionHack() const;

    void setTextCodec(QTextCodec *codec);
    QTextCodec *textCodec() const;

    void setReadOnly(bool b);

    void setTextCursor(const QTextCursor &cursor);

    void insertCodeSnippet(const QTextCursor &cursor, const QString &snippet);

    void setBlockSelection(bool on);
    bool hasBlockSelection() const;

    int verticalBlockSelectionFirstColumn() const;
    int verticalBlockSelectionLastColumn() const;

    QRegion translatedLineRegion(int lineStart, int lineEnd) const;

    void setIndenter(Indenter *indenter);
    Indenter *indenter() const;

    void setAutoCompleter(AutoCompleter *autoCompleter);
    AutoCompleter *autoCompleter() const;

    QPoint toolTipPosition(const QTextCursor &c) const;

    void invokeAssist(AssistKind assistKind, IAssistProvider *provider = 0);

    virtual IAssistInterface *createAssistInterface(AssistKind assistKind,
                                                    AssistReason assistReason) const;
    void emitContentsChanged();
    void emitChanged();

    void setCompletionProvider( TextEditor::CompletionAssistProvider *completitionProvider ) { m_completitionProvider = completitionProvider; }

    void insertText(const QString &text);
    void appendText(const QString &text);

public slots:
    void setDisplayName(const QString &title);

    virtual void paste();
    virtual void cut();

    void zoomIn(int range = 1);
    void zoomOut(int range = 1);
    void zoomReset();

    void cutLine();
    void copyLine();
    void deleteLine();
    void deleteEndOfWord();
    void deleteEndOfWordCamelCase();
    void deleteStartOfWord();
    void deleteStartOfWordCamelCase();
    void unfoldAll();
    void fold();
    void unfold();
    void selectEncoding();
    void duplicateSelectedLines();
    void moveSelectedLinesUp();
    void moveSelectedLinesDown();

    void gotoBlockStart();
    void gotoBlockEnd();
    void gotoBlockStartWithSelection();
    void gotoBlockEndWithSelection();

    void gotoLineStart();
    void gotoLineStartWithSelection();
    void gotoLineEnd();
    void gotoLineEndWithSelection();
    void gotoNextLine();
    void gotoNextLineWithSelection();
    void gotoPreviousLine();
    void gotoPreviousLineWithSelection();
    void gotoPreviousCharacter();
    void gotoPreviousCharacterWithSelection();
    void gotoNextCharacter();
    void gotoNextCharacterWithSelection();
    void gotoPreviousWord();
    void gotoPreviousWordWithSelection();
    void gotoNextWord();
    void gotoNextWordWithSelection();
    void gotoPreviousWordCamelCase();
    void gotoPreviousWordCamelCaseWithSelection();
    void gotoNextWordCamelCase();
    void gotoNextWordCamelCaseWithSelection();

    void selectBlockUp();
    void selectBlockDown();

    void moveLineUp();
    void moveLineDown();

    void copyLineUp();
    void copyLineDown();

    void joinLines();

    void insertLineAbove();
    void insertLineBelow();

    void uppercaseSelection();
    void lowercaseSelection();

    void cleanWhitespace();

    virtual void onSettingsChanged();
    void initializeAsTextInfoWidget();

signals:
    void changed();

    // ITextEditor
    void contentsChanged();

    void fontSizeIncrease();
    void fontSizeDecrease();

protected:
    bool event(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
    void changeEvent(QEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void showEvent(QShowEvent *);
    void contextMenuEvent(QContextMenuEvent *);

    // reimplemented to support block selection
    QMimeData *createMimeDataFromSelection() const;
    bool canInsertFromMimeData(const QMimeData *source) const;
    void insertFromMimeData(const QMimeData *source);

    static QString msgTextTooLarge(quint64 size);

private:
    void maybeSelectLine();
    void updateCannotDecodeInfo();

    void initializeInternal( bool isInfoWidget );
    enum {
        Text = 0,
        Link,
        Selection,
        LineNumber,
        CurrentLine,
        CurrentLineNumber,
        FormatsCount
    };
    QTextCharFormat m_charFormats[FormatsCount];

public:
    void duplicateFrom(TextEditorWidget *editor);

protected:
    BaseTextDocument *baseTextDocument() const;
    void setBaseTextDocument(BaseTextDocument *doc);

    void setDefaultPath(const QString &defaultPath);

private slots:
    void editorContentsChange(int position, int charsRemoved, int charsAdded);
    void documentAboutToBeReloaded();
    void documentReloaded();
    void highlightSearchResults(const QString &txt, Find::FindFlags findFlags);
    void setFindScope(const QTextCursor &start, const QTextCursor &end, int, int);
    bool inFindScope(const QTextCursor &cursor);
    bool inFindScope(int selectionStart, int selectionEnd);
    void inSnippetMode(bool *active);
    void onCodeStylePreferencesDestroyed();

private:
    Internal::BaseTextEditorPrivate *d;
    friend class Internal::BaseTextEditorPrivate;
    friend class Internal::TextEditorOverlay;
    friend class RefactorOverlay;

public:
    QWidget *extraArea() const;
    virtual int extraAreaWidth(int *markWidthPtr = 0) const;
    virtual void extraAreaPaintEvent(QPaintEvent *);
    virtual void extraAreaLeaveEvent(QEvent *);
    virtual void extraAreaContextMenuEvent(QContextMenuEvent *);
    virtual void extraAreaMouseEvent(QMouseEvent *);

    const TabSettings &tabSettings() const;
    void setLanguageSettingsId(const QString &settingsId);
    QString languageSettingsId() const;

    void setCodeStyle(ICodeStylePreferences *settings);

    const DisplaySettings &displaySettings() const;

    void markBlocksAsChanged(QList<int> blockNumbers);

    void ensureCursorVisible();

    enum ExtraSelectionKind {
        CurrentLineSelection,
        ParenthesesMatchingSelection,
        CodeWarningsSelection,
        CodeSemanticsSelection,
        UndefinedSymbolSelection,
        UnusedSymbolSelection,
        FakeVimSelection,
        OtherSelection,
        SnippetPlaceholderSelection,
        ObjCSelection,
        DebuggerExceptionSelection,
        NExtraSelectionKinds
    };
    void setExtraSelections(ExtraSelectionKind kind, const QList<QTextEdit::ExtraSelection> &selections);
    QList<QTextEdit::ExtraSelection> extraSelections(ExtraSelectionKind kind) const;
    QString extraSelectionTooltip(int pos) const;

    Internal::RefactorMarkers refactorMarkers() const;
    void setRefactorMarkers(const Internal::RefactorMarkers &markers);
signals:
    void refactorMarkerClicked(const TextEditor::RefactorMarker &marker);

public:

    struct BlockRange
    {
        BlockRange() : first(0), last(-1) {}
        BlockRange(int first_position, int last_position)
          : first(first_position), last(last_position)
        {}
        int first;
        int last;
        inline bool isNull() const { return last < first; }
    };

    // the blocks list must be sorted
    void setIfdefedOutBlocks(const QList<BlockRange> &blocks);

public slots:
    virtual void format();
    virtual void rewrapParagraph();
    virtual void unCommentSelection();
    virtual void setFontSettings(const TextEditor::FontSettings &);
    virtual void setTabSettings(const TextEditor::TabSettings &);
    virtual void setDisplaySettings(const TextEditor::DisplaySettings &);
    virtual void setBehaviorSettings(const TextEditor::BehaviorSettings &);
    virtual void setTypingSettings(const TextEditor::TypingSettings &);
    virtual void setStorageSettings(const TextEditor::StorageSettings &);
    virtual void setCompletionSettings(const TextEditor::CompletionSettings &);
    virtual void setExtraEncodingSettings(const TextEditor::ExtraEncodingSettings &);

protected:
    bool viewportEvent(QEvent *event);

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void dragEnterEvent(QDragEnterEvent *e);

public:
    void indentInsertedText(const QTextCursor &tc);
    void indent(QTextDocument *doc, const QTextCursor &cursor, QChar typedChar);
    void reindent(QTextDocument *doc, const QTextCursor &cursor);

protected:
    /*!
       Reimplement this function to enable code navigation.

       \a resolveTarget is set to true when the target of the link is relevant
       (it isn't until the link is used).
     */
//    virtual Core::ILink *findLinkAt(const QTextCursor &, bool resolveTarget = true);

    /*!
       Reimplement this function if you want to customize the way a link is
       opened. Returns whether the link was opened successfully.
     */
//    bool openLink( Core::ILink *link );

    void maybeClearSomeExtraSelections(const QTextCursor &cursor);

protected slots:
    virtual void slotUpdateExtraAreaWidth();
    virtual void slotModificationChanged(bool);
    virtual void slotUpdateRequest(const QRect &r, int dy);
    virtual void slotCursorPositionChanged();
    virtual void slotUpdateBlockNotify(const QTextBlock &);
    virtual void slotCodeStyleSettingsChanged(const QVariant &);

signals:
    void requestFontZoom(int zoom);
    void requestZoomReset();
    void requestBlockUpdate(const QTextBlock &);

private:
    void indentOrUnindent(bool doIndent);
    void handleHomeKey(bool anchor);
    void handleBackspaceKey();
    void moveLineUpDown(bool up);
    void copyLineUpDown(bool up);
    void saveCurrentCursorPositionForNavigation();
    void updateHighlights();
    void updateCurrentLineHighlight();

    void drawFoldingMarker(QPainter *painter, const QPalette &pal,
                           const QRect &rect,
                           bool expanded,
                           bool active,
                           bool hovered) const;

    void drawCollapsedBlockPopup(QPainter &painter,
                                 const QTextBlock &block,
                                 QPointF offset,
                                 const QRect &clip);

    void toggleBlockVisible(const QTextBlock &block);
    QRect foldBox();

    QTextBlock foldedBlockAt(const QPoint &pos, QRect *box = 0) const;

//    void updateLink(QMouseEvent *e);
//    void showLink( Core::ILink *link );
    void clearLink();

    void universalHelper(); // test function for development

    bool cursorMoveKeyEvent(QKeyEvent *e);
    bool camelCaseRight(QTextCursor &cursor, QTextCursor::MoveMode mode);
    bool camelCaseLeft(QTextCursor &cursor, QTextCursor::MoveMode mode);

    void transformSelection(Internal::TransformationMethod method);

private slots:
    void handleBlockSelection(int diff_row, int diff_col);

    // parentheses matcher
    void _q_matchParentheses();
    void _q_highlightBlocks();
    void slotSelectionChanged();
    void _q_animateUpdate(int position, QPointF lastPos, QRectF rect);
    void doFoo();

private:
    TextEditor::CompletionAssistProvider *m_completitionProvider;
};

} // namespace TextEditor
