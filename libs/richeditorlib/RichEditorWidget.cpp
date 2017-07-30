#include <QColorDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextCodec>

#include "RichEditorWidget.h"
#include "TablePropertiesDialog.h"
#include "CellsPropertiesDialog.h"
#include "richeditor_constants.h"
#include "Core.h"

namespace RichEditor {

RichEditorWidget::RichEditorWidget( QMainWindow *parent )
    : QTextEdit(parent)
    , m_dirtyLevel(0)
    , m_mainWindow(Core::Storage::mainWindow()->widget())
{
    init(document()->defaultFont());

    m_mainWindow->addToolBar(textToolBar());
    m_mainWindow->addToolBar(paraToolBar());
    m_mainWindow->addToolBar(tableToolBar());
}

RichEditorWidget::~RichEditorWidget()
{
}

void RichEditorWidget::setContent( const QString &htmlTxt )
{
    setHtml(htmlTxt);
    clearFormat(document()->defaultFont());
}

bool RichEditorWidget::setFileContent( const QString &fileName, QString &errorMessage )
{
    if ( !QFile::exists(fileName) ) {
        errorMessage = tr("File '%1' Not Found").arg(fileName);
        return false;
    }
    QFile file(fileName);
    if ( !file.open(QFile::ReadOnly) ) {
        errorMessage = tr("File '%1' Cannot be Opened For Read").arg(fileName);
        return false;
    }

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        setHtml(str);
    } else {
        str = QString::fromLocal8Bit(data);
        setPlainText(str);
    }

    clearFormat(document()->defaultFont());
    return true;
}

QToolBar *RichEditorWidget::textToolBar()
{ return m_textToolBar; }

QToolBar *RichEditorWidget::paraToolBar()
{ return m_paraToolBar; }

QToolBar *RichEditorWidget::tableToolBar()
{ return m_tableToolBar; }

void RichEditorWidget::handleUpdateTableActions()
{
    m_actTableProperties->setEnabled(false);
    m_actCellsProperties->setEnabled(false);
    m_actCellMerge->setEnabled(false);
    m_actCellSplit->setEnabled(false);
    m_actColumnAddBefore->setEnabled(false);
    m_actColumnAddAfter->setEnabled(false);
    m_actColumnDelete->setEnabled(false);
    m_actRowAddBefore->setEnabled(false);
    m_actRowAddAfter->setEnabled(false);
    m_actRowDelete->setEnabled(false);

    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() ) {
        QTextTableFormat tf = t->format();
        bool isDataTable = tf.hasProperty(RichEditor::DataTableId);
        QTextTableCell tc = t->cellAt(c);
        if ( !c.hasSelection() ) {
            if ( tc.columnSpan() > 1 || tc.rowSpan() > 1 )
                m_actCellSplit->setEnabled(true);
            m_actColumnAddBefore->setEnabled(true);
            m_actColumnAddAfter->setEnabled(true);
            if ( t->columns() > 1 )
                m_actColumnDelete->setEnabled(true);
            m_actRowAddBefore->setEnabled(!isDataTable);
            m_actRowAddAfter->setEnabled(!isDataTable);
            if ( t->rows() > 1 )
                m_actRowDelete->setEnabled(!isDataTable);
        } else if ( c.hasComplexSelection() ) {
            m_actCellMerge->setEnabled(true);
        }
        m_actTableProperties->setEnabled(true);
        m_actCellsProperties->setEnabled(true);
    }
}

void RichEditorWidget::init( const QFont &defaultFont_ )
{
    setupTextActions();
    setupParaActions();
    setupTableActions();

    clearFormat(defaultFont_);

    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(onCurrentCharFormatChanged(QTextCharFormat)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
}

void RichEditorWidget::onTextFamily( const QString &f )
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void RichEditorWidget::onTextSize( const QString &p )
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void RichEditorWidget::onTextBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(m_actTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void RichEditorWidget::onTextUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(m_actTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void RichEditorWidget::onTextItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(m_actTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void RichEditorWidget::onTextColor()
{
    QColor col = QColorDialog::getColor(textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void RichEditorWidget::onTextAlign( QAction *a )
{
    if ( m_dirtyLevel )
        return;

    if (a == m_actAlignLeft)
        setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == m_actAlignCenter)
        setAlignment(Qt::AlignHCenter);
    else if (a == m_actAlignRight)
        setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == m_actAlignJustify)
        setAlignment(Qt::AlignJustify);
}

void RichEditorWidget::onTextIndent( double indent )
{
    if ( m_dirtyLevel )
        return;

    QTextBlockFormat blockFormat = textCursor().blockFormat();
    if ( blockFormat.isValid() ) {
        blockFormat.setTextIndent(indent);
        textCursor().setBlockFormat(blockFormat);
    }
}

void RichEditorWidget::onLeftMargin( double margin )
{
    if ( m_dirtyLevel )
        return;

    QTextBlockFormat blockFormat = textCursor().blockFormat();
    if ( blockFormat.isValid() ) {
        blockFormat.setLeftMargin(margin);
        textCursor().setBlockFormat(blockFormat);
    }
}

void RichEditorWidget::onRightMargin( double margin )
{
    if ( m_dirtyLevel )
        return;

    QTextBlockFormat blockFormat = textCursor().blockFormat();
    if ( blockFormat.isValid() ) {
        blockFormat.setRightMargin(margin);
        textCursor().setBlockFormat(blockFormat);
    }
}

void RichEditorWidget::onTableInsert()
{
    QTextCursor c = textCursor();

    if ( c.hasSelection() ) {
        Core::Storage::warning(tr("Current Selection Must be Empty"));
        return;
    }

    if ( c.currentTable() ) {
        Core::Storage::warning(tr("Table Cannot be Inserted Into Table"));
        return;
    }

    TablePropertiesDialog tableDialog(this, 0, Core::Storage::mainWindow()->widget());
    tableDialog.exec();
}

void RichEditorWidget::onTableProperies()
{
    if ( QTextTable *t = textCursor().currentTable() ) {
        TablePropertiesDialog tablePropertiesDialog(this, t, Core::Storage::mainWindow()->widget());
        tablePropertiesDialog.exec();
    }
}

void RichEditorWidget::onCellsProperies()
{
    if ( textCursor().currentTable() ) {
        CellsPropertiesDialog cellsProperiesDialog(this, Core::Storage::mainWindow()->widget());
        cellsProperiesDialog.exec();
    }
}

void RichEditorWidget::onCellsMerge()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->mergeCells(c);
}

void RichEditorWidget::onCellSplit()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() ) {
        QTextTableCell tc = t->cellAt(c);
        t->splitCell(tc.row(), tc.column(), 1, 1);
    }
}

void RichEditorWidget::onRowInsertBefore()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->insertRows(t->cellAt(c).row(), 1);
}

void RichEditorWidget::onRowInsertAfter()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->insertRows(t->cellAt(c).row() + t->cellAt(c).rowSpan(), 1);
}

void RichEditorWidget::onRowDelete()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->removeRows(t->cellAt(c).row(), 1);
}

void RichEditorWidget::onColumnInsertBefore()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->insertColumns(t->cellAt(c).column(), 1);
}

void RichEditorWidget::onColumnInsertAfter()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->insertColumns(t->cellAt(c).column() + t->cellAt(c).columnSpan(), 1);
}

void RichEditorWidget::onColumnDelete()
{
    QTextCursor c = textCursor();
    if ( QTextTable *t = c.currentTable() )
        t->removeColumns(t->cellAt(c).column(), 1);
}

void RichEditorWidget::onCurrentCharFormatChanged( const QTextCharFormat &format )
{
    m_dirtyLevel++;
    fontChanged(format.font());
    colorChanged(format.foreground().color());
    updateTableActions();
    m_dirtyLevel--;
}

void RichEditorWidget::onCursorPositionChanged()
{
    m_dirtyLevel++;
    alignmentChanged(alignment());
    blockFormatChanged(textCursor().blockFormat());
    updateTableActions();
    m_dirtyLevel--;
}

void RichEditorWidget::setupTextActions()
{
    m_textToolBar = new QToolBar(tr("Text Format"), m_mainWindow);
    m_textToolBar->setObjectName("RichEditorWidget_TextFormatToolBar");
    m_textToolBar->setFloatable(false);

    m_actTextBold      = m_textToolBar->addAction(QIcon(":/images/textbold.png"), tr("&Bold"));
    m_actTextItalic    = m_textToolBar->addAction(QIcon(":/images/textitalic.png"), tr("&Italic"));
    m_actTextUnderline = m_textToolBar->addAction(QIcon(":/images/textunder.png"), tr("&Underline"));
    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    m_actTextColor = new QAction(pix, tr("&Color..."), this);
    m_textToolBar->addAction(m_actTextColor);

    m_actTextBold->setShortcut(tr("Ctrl+B"));
    m_actTextBold->setPriority(QAction::LowPriority);
    m_actTextBold->setCheckable(true);
    m_actTextItalic->setShortcut(tr("Ctrl+I"));
    m_actTextItalic->setPriority(QAction::LowPriority);
    m_actTextItalic->setCheckable(true);
    m_actTextUnderline->setShortcut(tr("Ctrl+U"));
    m_actTextUnderline->setPriority(QAction::LowPriority);
    m_actTextUnderline->setCheckable(true);

    m_textToolBar->addSeparator();

    m_comboFont = new QFontComboBox(m_textToolBar);
    m_textToolBar->addWidget(m_comboFont);
    m_comboSize = new QComboBox(m_textToolBar);
    m_comboSize->setObjectName("comboSize");
    m_textToolBar->addWidget(m_comboSize);
    m_comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        m_comboSize->addItem(QString::number(size));

    connect(m_actTextBold,         SIGNAL(triggered()), this, SLOT(onTextBold()));
    connect(m_actTextItalic,       SIGNAL(triggered()), this, SLOT(onTextItalic()));
    connect(m_actTextUnderline,    SIGNAL(triggered()), this, SLOT(onTextUnderline()));
    connect(m_actTextColor,        SIGNAL(triggered()), this, SLOT(onTextColor()));

    connect(m_comboFont, SIGNAL(activated(QString)), this, SLOT(onTextFamily(QString)));
    connect(m_comboSize, SIGNAL(activated(QString)), this, SLOT(onTextSize(QString)));
}

void RichEditorWidget::setupParaActions()
{
    m_paraToolBar = new QToolBar(tr("Para Format"), m_mainWindow);
    m_paraToolBar->setObjectName("RichEditorWidget_ParaFormatToolBar");
    m_paraToolBar->setFloatable(false);

    QActionGroup *alignmentActionsGroup = new QActionGroup(this);
    connect(alignmentActionsGroup, SIGNAL(triggered(QAction*)), this, SLOT(onTextAlign(QAction*)));

    m_actAlignLeft     = new QAction(QIcon(":/images/textleft.png"), tr("&Left"), alignmentActionsGroup);
    m_actAlignCenter   = new QAction(QIcon(":/images/textcenter.png"), tr("C&enter"), alignmentActionsGroup);
    m_actAlignRight    = new QAction(QIcon(":/images/textright.png"), tr("&Right"), alignmentActionsGroup);
    m_actAlignJustify  = new QAction(QIcon(":/images/textjustify.png"), tr("&Justify"), alignmentActionsGroup);

    m_paraToolBar->addActions(alignmentActionsGroup->actions());

    m_paraToolBar->addSeparator();
    m_indentSpinBox = new QDoubleSpinBox();
    m_indentSpinBox->setMinimum(-100.0);
    m_indentSpinBox->setMaximum(200.0);
    m_indentSpinBox->setValue(0.0);
    m_indentSpinBox->setToolTip(tr("Text Indent"));
    QLabel *indentLabel = new QLabel();
    indentLabel->setPixmap(QPixmap(":/images/text_indent.png"));
    QHBoxLayout *indentLayout = new QHBoxLayout();
    indentLayout->setMargin(0);
    indentLayout->setSpacing(1);
    indentLayout->addWidget(indentLabel);
    indentLayout->addWidget(m_indentSpinBox);
    QWidget *indentWidget = new QWidget();
    indentWidget->setLayout(indentLayout);
    m_paraToolBar->addWidget(indentWidget);
    connect(m_indentSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTextIndent(double)));

    m_paraToolBar->addSeparator();
    m_leftMarginSpinBox = new QDoubleSpinBox();
    m_leftMarginSpinBox->setMinimum(0.0);
    m_leftMarginSpinBox->setMaximum(200.0);
    m_leftMarginSpinBox->setValue(0.0);
    m_leftMarginSpinBox->setToolTip(tr("Left Margin of Text Block"));
    QLabel *leftMarginLabel = new QLabel();
    leftMarginLabel->setPixmap(QPixmap(":/images/left_margin.png"));
    QHBoxLayout *leftMarginLayout = new QHBoxLayout();
    leftMarginLayout->setMargin(0);
    leftMarginLayout->setSpacing(1);
    leftMarginLayout->addWidget(leftMarginLabel);
    leftMarginLayout->addWidget(m_leftMarginSpinBox);
    QWidget *leftMarginWidget = new QWidget();
    leftMarginWidget->setLayout(leftMarginLayout);
    m_paraToolBar->addWidget(leftMarginWidget);
    connect(m_leftMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onLeftMargin(double)));

    m_paraToolBar->addSeparator();
    m_rightMarginSpinBox = new QDoubleSpinBox();
    m_rightMarginSpinBox->setMinimum(0.0);
    m_rightMarginSpinBox->setMaximum(200.0);
    m_rightMarginSpinBox->setValue(0.0);
    m_rightMarginSpinBox->setToolTip(tr("Right Margin of Text Block"));
    QLabel *rightMarginLabel = new QLabel();
    rightMarginLabel->setPixmap(QPixmap(":/images/right_margin.png"));
    QHBoxLayout *rightMarginLayout = new QHBoxLayout();
    rightMarginLayout->setMargin(0);
    rightMarginLayout->setSpacing(1);
    rightMarginLayout->addWidget(rightMarginLabel);
    rightMarginLayout->addWidget(m_rightMarginSpinBox);
    QWidget *rightMatginWidget = new QWidget();
    rightMatginWidget->setLayout(rightMarginLayout);
    m_paraToolBar->addWidget(rightMatginWidget);
    connect(m_rightMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRightMargin(double)));

    m_actAlignLeft->setShortcut(tr("Ctrl+L"));
    m_actAlignLeft->setCheckable(true);
    m_actAlignLeft->setPriority(QAction::LowPriority);
    m_actAlignLeft->setToolTip(tr("Set Left-Alignment to Selected Text"));
    m_actAlignCenter->setShortcut(tr("Ctrl+E"));
    m_actAlignCenter->setCheckable(true);
    m_actAlignCenter->setPriority(QAction::LowPriority);
    m_actAlignCenter->setToolTip(tr("Set Center-Alignment to Selected Text"));
    m_actAlignRight->setShortcut(tr("Ctrl+R"));
    m_actAlignRight->setCheckable(true);
    m_actAlignRight->setPriority(QAction::LowPriority);
    m_actAlignRight->setToolTip(tr("Set Right-Alignment to Selected Text"));
    m_actAlignJustify->setShortcut(tr("Ctrl+J"));
    m_actAlignJustify->setCheckable(true);
    m_actAlignJustify->setPriority(QAction::LowPriority);
    m_actAlignJustify->setToolTip(tr("Set Full-Justify-Alignment to Selected Text"));
}

void RichEditorWidget::setupTableActions()
{
    m_tableToolBar = new QToolBar(tr("Table"), m_mainWindow);
    m_tableToolBar->setObjectName("RichEditorWidget_TableToolBar");
    m_tableToolBar->setFloatable(false);

    m_actTableInsert = m_tableToolBar->addAction(QIcon(":/images/rpt_table_add.png"), tr("Add Simple &Table"));
    m_actTableInsert->setToolTip(tr("Insert Simple Table"));
    m_actTableInsert->setShortcut(tr("Ctrl+T"));
    connect(m_actTableInsert, SIGNAL(triggered()), this, SLOT(onTableInsert()));

    m_actTableProperties = m_tableToolBar->addAction(QIcon(":/images/table_prop.png"), tr("Table Properties"));
    m_actTableProperties->setToolTip(tr("Edit Table Properies"));
    m_actTableProperties->setEnabled(false);
    connect(m_actTableProperties, SIGNAL(triggered()), this, SLOT(onTableProperies()));

    m_actCellsProperties = m_tableToolBar->addAction(QIcon(":/images/cells_prop.png"), tr("Cells Properties"));
    m_actCellsProperties->setToolTip(tr("Edit Selected Cells Properties"));
    m_actCellsProperties->setEnabled(false);
    connect(m_actCellsProperties, SIGNAL(triggered()), this, SLOT(onCellsProperies()));

    m_actCellMerge = m_tableToolBar->addAction(QIcon(":/images/merge_cell.png"), tr("Merge Cells"));
    m_actCellMerge->setToolTip(tr("Merge Table Cells"));
    m_actCellMerge->setEnabled(false);
    connect(m_actCellMerge, SIGNAL(triggered()), this, SLOT(onCellsMerge()));

    m_actCellSplit = m_tableToolBar->addAction(QIcon(":/images/split_cell.png"), tr("Split Cell"));
    m_actCellSplit->setToolTip(tr("Split Table Cell"));
    m_actCellSplit->setEnabled(false);
    connect(m_actCellSplit, SIGNAL(triggered()), this, SLOT(onCellSplit()));

    m_actRowAddBefore = m_tableToolBar->addAction(QIcon(":/images/row_add_before.png"), tr("Add Row Before"));
    m_actRowAddBefore->setToolTip(tr("Insert Table Row Before Selected Row"));
    m_actRowAddBefore->setEnabled(false);
    connect(m_actRowAddBefore, SIGNAL(triggered()), this, SLOT(onRowInsertBefore()));

    m_actRowAddAfter = m_tableToolBar->addAction(QIcon(":/images/row_add_after.png"), tr("Add Row After"));
    m_actRowAddAfter->setToolTip(tr("Insert Table Row After Selected Row"));
    m_actRowAddAfter->setEnabled(false);
    connect(m_actRowAddAfter, SIGNAL(triggered()), this, SLOT(onRowInsertAfter()));

    m_actRowDelete = m_tableToolBar->addAction(QIcon(":/images/row_delete.png"), tr("Delete Row"));
    m_actRowDelete->setToolTip(tr("Delete Selected Row"));
    m_actRowDelete->setEnabled(false);
    connect(m_actRowDelete, SIGNAL(triggered()), this, SLOT(onRowDelete()));

    m_actColumnAddBefore = m_tableToolBar->addAction(QIcon(":/images/column_add_before.png"), tr("Add Column Before"));
    m_actColumnAddBefore->setToolTip(tr("Insert Table Column Before Selected Column"));
    m_actColumnAddBefore->setEnabled(false);
    connect(m_actColumnAddBefore, SIGNAL(triggered()), this, SLOT(onColumnInsertBefore()));

    m_actColumnAddAfter = m_tableToolBar->addAction(QIcon(":/images/column_add_after.png"), tr("Add Column After"));
    m_actColumnAddAfter->setToolTip(tr("Insert Table Column After Selected Column"));
    m_actColumnAddAfter->setEnabled(false);
    connect(m_actColumnAddAfter, SIGNAL(triggered()), this, SLOT(onColumnInsertAfter()));

    m_actColumnDelete = m_tableToolBar->addAction(QIcon(":/images/column_delete.png"), tr("Delete Column"));
    m_actColumnDelete->setToolTip(tr("Delete Selected Column"));
    m_actRowDelete->setEnabled(false);
    connect(m_actColumnDelete, SIGNAL(triggered()), this, SLOT(onColumnDelete()));
}

void RichEditorWidget::mergeFormatOnWordOrSelection( const QTextCharFormat &format )
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

void RichEditorWidget::clearFormat( const QFont &defaultFont_ )
{
    document()->setDefaultFont(defaultFont_);
    fontChanged(defaultFont_);
    colorChanged(textColor());
    alignmentChanged(alignment());
    blockFormatChanged(textCursor().blockFormat());
    updateTableActions();
}

void RichEditorWidget::fontChanged( const QFont &f )
{
    m_comboFont->setCurrentIndex(m_comboFont->findText(QFontInfo(f).family()));
    m_comboSize->setCurrentIndex(m_comboSize->findText(QString::number(f.pointSize())));
    m_actTextBold->setChecked(f.bold());
    m_actTextItalic->setChecked(f.italic());
    m_actTextUnderline->setChecked(f.underline());
}

void RichEditorWidget::colorChanged( const QColor &c )
{
    QPixmap pix(16, 16);
    pix.fill(c);
    m_actTextColor->setIcon(pix);
}

void RichEditorWidget::alignmentChanged( Qt::Alignment a )
{
    if (a & Qt::AlignLeft)
        m_actAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        m_actAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        m_actAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        m_actAlignJustify->setChecked(true);
}

void RichEditorWidget::blockFormatChanged( const QTextBlockFormat &blockFormat )
{
    if ( blockFormat.isValid() ) {
        m_indentSpinBox->setValue(blockFormat.textIndent());
        m_leftMarginSpinBox->setValue(blockFormat.leftMargin());
        m_rightMarginSpinBox->setValue(blockFormat.rightMargin());
    }
}

void RichEditorWidget::updateTableActions()
{
    handleUpdateTableActions();
}

}   // RichEditor
