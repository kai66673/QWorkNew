/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef BASETEXTEDITOR_H
#define BASETEXTEDITOR_H

#include "itexteditor.h"
#include "codeassist/assistenums.h"
#include "ILink.h"
#include "completionassistprovider.h"

#include <ifindsupport.h>
#include "texteditor_global.h"

#include <QPlainTextEdit>
#include <QPrinter>

QT_BEGIN_NAMESPACE
class QToolBar;
class QTimeLine;
QT_END_NAMESPACE

namespace Utils {
    class LineColumnLabel;
    class ChangeSet;
}

namespace TextEditor {
class TabSettings;
class RefactorOverlay;
struct RefactorMarker;
class IAssistMonitorInterface;
class IAssistInterface;
class IAssistProvider;
class ICodeStylePreferences;

namespace Internal {
    class BaseTextEditorPrivate;
    class TextEditorOverlay;
    typedef QList<RefactorMarker> RefactorMarkers;
    typedef QString (TransformationMethod)(const QString &);
}

class ITextMarkable;

class BaseTextDocument;
class BaseTextEditor;
class FontSettings;
class BehaviorSettings;
class CompletionSettings;
class DisplaySettings;
class TypingSettings;
class StorageSettings;
class Indenter;
class AutoCompleter;
class ExtraEncodingSettings;

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

    virtual void waitForEditorSupport() {}

    // EditorInterface
    Core::IFile * file();
    bool createNew(const QString &contents);
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    QByteArray saveState() const;
    bool restoreState(const QByteArray &state);
    QString displayName() const;

    // ITextEditor

    void gotoLine(int line, int column = 0);
    virtual void refreshDocumentCursor();

    int position(
        ITextEditor::PositionOperation posOp = ITextEditor::Current
        , int at = -1) const;
    void convertPosition(int pos, unsigned *line, unsigned *column) const;

    BaseTextEditor *editor() const;
    ITextMarkable *markableInterface() const;

    QChar characterAt(int pos) const;

    void print(QPrinter *);

    void setSuggestedFileName(const QString &suggestedFileName);
    QString mimeType() const;
    virtual void setMimeType(const QString &mt);


    void appendStandardContextMenuActions(QMenu *menu);

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

    virtual BaseTextEditor *createEditor() = 0;

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
    virtual Core::ILink *findLinkAt(const QTextCursor &, bool resolveTarget = true);

    /*!
       Reimplement this function if you want to customize the way a link is
       opened. Returns whether the link was opened successfully.
     */
    bool openLink( Core::ILink *link );

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

    void updateLink(QMouseEvent *e);
    void showLink( Core::ILink *link );
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


class TEXTEDITOR_EXPORT BaseTextEditor : public ITextEditor
{
    Q_OBJECT

public:
    BaseTextEditor(TextEditorWidget *editorWidget);
    ~BaseTextEditor();

    friend class TextEditorWidget;
    TextEditorWidget *editorWidget() const { return e; }

    // ITextFindSupport implementation
    virtual QTextDocument *document() const;
    virtual QTextCursor textCursor() const;
    virtual void setTextCursor(const QTextCursor& cursor);
    virtual bool isReadOnly() const;

    // EditorInterface
    virtual void waitForEditorSupport();
    Core::IFile * file() { return e->file(); }
    bool createNew(const QString &contents) { return e->createNew(contents); }
    bool open(QString *errorString, const QString &fileName, const QString &realFileName) { return e->open(errorString, fileName, realFileName); }
    QString displayName() const { return e->displayName(); }
    void setDisplayName(const QString &title) { e->setDisplayName(title); emit changed(); }

    QByteArray saveState() const { return e->saveState(); }
    bool restoreState(const QByteArray &state) { return e->restoreState(state); }

    virtual void saveContentState( QDomDocument *document, QDomElement *element ) const;
    virtual void restoreContentState( QDomElement *element );

    QWidget *toolBar();

    enum Side { Left, Right };
    void insertExtraToolBarWidget(Side side, QWidget *widget);

    // ITextEditor
    int find(const QString &string) const;
    int currentLine() const;
    int currentColumn() const;
    void gotoLine(int line, int column = 0);
    int columnCount() const;
    int rowCount() const;

    int position(PositionOperation posOp = Current, int at = -1) const
    { return e->position(posOp, at); }
    void convertPosition(int pos, unsigned *line, unsigned *column) const
    { e->convertPosition(pos, line, column); }
    QRect cursorRect(int pos = -1) const;

    QString contents() const;
    QString selectedText() const;
    QString textAt(int pos, int length) const;
    inline QChar characterAt(int pos) const { return e->characterAt(pos); }

    inline ITextMarkable *markableInterface() { return e->markableInterface(); }

    void setContextHelpId(const QString &id) { m_contextHelpId = id; }
    QString contextHelpId() const; // from IContext

    inline void setTextCodec(QTextCodec *codec, TextCodecReason = TextCodecOtherReason) { e->setTextCodec(codec); }
    inline QTextCodec *textCodec() const { return e->textCodec(); }


    // ITextEditor
    void remove(int length);
    void insert(const QString &string);
    void replace(int length, const QString &string);
    void setCursorPosition(int pos);
    void select(int toPos);

    virtual bool canPaste() const { return e->canPaste(); }

    virtual void copy() { e->copy(); }
    virtual void paste() { e->paste(); }
    virtual void cut() { e->cut(); }
    virtual void selectAll() { e->selectAll(); }

    virtual void switchCase( CaseTransform caseType );

    void setCompletionProvider( TextEditor::CompletionAssistProvider *provider ) { m_completitionProvider = provider; }
    TextEditor::CompletionAssistProvider *completionProvider() { return m_completitionProvider; }

public slots:
    void fontSizeIncrease();
    void fontSizeDecrease();

private slots:
    void updateCursorPosition();

private:
    TextEditorWidget *e;
    mutable QString m_contextHelpId;
    QToolBar *m_toolBar;
    QWidget *m_stretchWidget;
    QAction *m_cursorPositionLabelAction;
    Utils::LineColumnLabel *m_cursorPositionLabel;

    TextEditor::CompletionAssistProvider *m_completitionProvider;
};

} // namespace TextEditor

#endif // BASETEXTEDITOR_H
