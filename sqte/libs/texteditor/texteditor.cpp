#include <QTimer>
#include <QCoreApplication>

#include "basetexteditor_p.h"
#include "texteditoroverlay.h"
#include "refactoroverlay.h"

using namespace TextEditor;
using namespace TextEditor::Internal;

namespace TextEditor {

namespace Internal {

class TextEditExtraArea : public QWidget {
    TextEditorWidget *textEdit;
public:
    TextEditExtraArea(TextEditorWidget *edit):QWidget(edit) {
        textEdit = edit;
        setAutoFillBackground(true);
    }
public:

    QSize sizeHint() const {
        return QSize(textEdit->extraAreaWidth(), 0);
    }
protected:
    void paintEvent(QPaintEvent *event){
        textEdit->extraAreaPaintEvent(event);
    }
    void mousePressEvent(QMouseEvent *event){
        textEdit->extraAreaMouseEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event){
        textEdit->extraAreaMouseEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event){
        textEdit->extraAreaMouseEvent(event);
    }
    void leaveEvent(QEvent *event){
        textEdit->extraAreaLeaveEvent(event);
    }
    void contextMenuEvent(QContextMenuEvent *event) {
        textEdit->extraAreaContextMenuEvent(event);
    }

    void wheelEvent(QWheelEvent *event) {
        QCoreApplication::sendEvent(textEdit->viewport(), event);
    }
};

} // namespace Internal

TextEditorWidget::TextEditorWidget(QWidget *parent )
    : QPlainTextEdit(parent)
    , m_completitionProvider(0)
{
    d = new BaseTextEditorPrivate;
    d->q = this;
    d->m_extraArea = new TextEditExtraArea(this);
    d->m_extraArea->setMouseTracking(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    d->m_overlay = new TextEditorOverlay(this);
    d->m_snippetOverlay = new TextEditorOverlay(this);
    d->m_searchResultOverlay = new TextEditorOverlay(this);
    d->m_refactorOverlay = new RefactorOverlay(this);

    d->setupDocumentSignals(d->m_document);

    d->m_lastScrollPos = -1;

    // from RESEARCH

    setLayoutDirection(Qt::LeftToRight);
    viewport()->setMouseTracking(true);
    d->extraAreaSelectionAnchorBlockNumber
        = d->extraAreaToggleMarkBlockNumber
        = d->extraAreaHighlightFoldedBlockNumber
        = -1;

    d->visibleFoldedBlockNumber = d->suggestedVisibleFoldedBlockNumber = -1;

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(slotUpdateExtraAreaWidth()));
    connect(this, SIGNAL(modificationChanged(bool)), this, SLOT(slotModificationChanged(bool)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(slotUpdateRequest(QRect, int)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));

//     (void) new QShortcut(tr("CTRL+L"), this, SLOT(centerCursor()), 0, Qt::WidgetShortcut);
//     (void) new QShortcut(tr("F9"), this, SLOT(slotToggleMark()), 0, Qt::WidgetShortcut);
//     (void) new QShortcut(tr("F11"), this, SLOT(slotToggleBlockVisible()));

#ifdef DO_FOO
    (void) new QShortcut(tr("CTRL+D"), this, SLOT(doFoo()));
#endif


    // parentheses matcher
    d->m_formatRange = true;
    d->m_matchFormat.setForeground(Qt::red);
    d->m_rangeFormat.setBackground(QColor(0xb4, 0xee, 0xb4));
    d->m_mismatchFormat.setBackground(Qt::magenta);
    d->m_parenthesesMatchingTimer = new QTimer(this);
    d->m_parenthesesMatchingTimer->setSingleShot(true);
    connect(d->m_parenthesesMatchingTimer, SIGNAL(timeout()), this, SLOT(_q_matchParentheses()));

    d->m_highlightBlocksTimer = new QTimer(this);
    d->m_highlightBlocksTimer->setSingleShot(true);
    connect(d->m_highlightBlocksTimer, SIGNAL(timeout()), this, SLOT(_q_highlightBlocks()));

    d->m_animator = 0;

    d->m_searchResultFormat.setBackground(QColor(0xffef0b));

    slotUpdateExtraAreaWidth();
    updateHighlights();
    setFrameStyle(QFrame::NoFrame);

    d->m_delayedUpdateTimer = new QTimer(this);
    d->m_delayedUpdateTimer->setSingleShot(true);
    connect(d->m_delayedUpdateTimer, SIGNAL(timeout()), viewport(), SLOT(update()));

    d->m_moveLineUndoHack = false;
}

TextEditorWidget::~TextEditorWidget()
{
    delete d;
    d = 0;
}

void TextEditorWidget::appendStandardContextMenuActions(QMenu *)
{ }

void TextEditorWidget::setParenthesesMatchingEnabled(bool b)
{
    d->m_parenthesesMatchingEnabled = b;
}

bool TextEditorWidget::isParenthesesMatchingEnabled() const
{
    return d->m_parenthesesMatchingEnabled;
}

void TextEditorWidget::setHighlightCurrentLine(bool b)
{
    d->m_highlightCurrentLine = b;
    updateCurrentLineHighlight();
}

bool TextEditorWidget::highlightCurrentLine() const
{
    return d->m_highlightCurrentLine;
}

}   // namespace TextEditor
