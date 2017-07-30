#ifndef RICHEDITORWIDGET_H
#define RICHEDITORWIDGET_H

#include <QTextEdit>
#include <QToolBar>
#include <QFontComboBox>
#include <QAction>
#include <QDoubleSpinBox>
#include <QMainWindow>

#include "richeditor_global.h"
#include "IDocument.h"

namespace RichEditor {

class RICHEDITOR_EXPORT RichDocumentPosition : public IDocumentCursor
{
public:
    RichDocumentPosition( int pos = 0 )
        : m_position(pos)
    {}

    inline int position() const { return m_position; }

private:
    int m_position;
};

class RICHEDITOR_EXPORT RichEditorWidget : public QTextEdit
{
    Q_OBJECT

public:
    explicit RichEditorWidget( QMainWindow *parent );

    virtual ~RichEditorWidget();

    void setContent( const QString &htmlTxt );
    bool setFileContent( const QString &fileName, QString &errorMessage );

    QToolBar *textToolBar();
    QToolBar *paraToolBar();
    QToolBar *tableToolBar();

protected:
    virtual void handleUpdateTableActions();
    void init( const QFont &defaultFont_ );

signals:

private slots:
    void onTextFamily( const QString &f );
    void onTextSize( const QString &p );

    void onTextBold();
    void onTextUnderline();
    void onTextItalic();
    void onTextColor();

    void onTextAlign( QAction *a );
    void onTextIndent( double indent );
    void onLeftMargin( double margin );
    void onRightMargin( double margin );

    void onTableInsert();
    void onTableProperies();
    void onCellsProperies();
    void onCellsMerge();
    void onCellSplit();
    void onRowInsertBefore();
    void onRowInsertAfter();
    void onRowDelete();
    void onColumnInsertBefore();
    void onColumnInsertAfter();
    void onColumnDelete();

    void onCurrentCharFormatChanged(const QTextCharFormat &format);
    void onCursorPositionChanged();

private:
    void setupTextActions();
    void setupParaActions();
    void setupTableActions();

    void mergeFormatOnWordOrSelection( const QTextCharFormat &format );
    void clearFormat( const QFont &defaultFont_ );
    void fontChanged( const QFont &f );
    void colorChanged( const QColor &c );
    void alignmentChanged( Qt::Alignment a );
    void blockFormatChanged( const QTextBlockFormat &blockFormat );
    void updateTableActions();

    QToolBar        *m_textToolBar;
    QToolBar        *m_paraToolBar;
    QToolBar        *m_tableToolBar;

    QFontComboBox   *m_comboFont;
    QComboBox       *m_comboSize;

    QDoubleSpinBox  *m_indentSpinBox;
    QDoubleSpinBox  *m_leftMarginSpinBox;
    QDoubleSpinBox  *m_rightMarginSpinBox;

    QAction *m_actTextBold;
    QAction *m_actTextUnderline;
    QAction *m_actTextItalic;
    QAction *m_actTextColor;

    QAction *m_actAlignLeft;
    QAction *m_actAlignCenter;
    QAction *m_actAlignRight;
    QAction *m_actAlignJustify;

    QAction *m_actTableInsert;
    QAction *m_actTableProperties;
    QAction *m_actCellsProperties;
    QAction *m_actCellMerge;
    QAction *m_actCellSplit;
    QAction *m_actColumnAddBefore;
    QAction *m_actColumnAddAfter;
    QAction *m_actColumnDelete;
    QAction *m_actRowAddBefore;
    QAction *m_actRowAddAfter;
    QAction *m_actRowDelete;

    int m_dirtyLevel;

protected:
    QMainWindow *m_mainWindow;
};

}   // RichEditor

#endif // RICHEDITORWIDGET_H
