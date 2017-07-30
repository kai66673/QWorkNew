#include <QPainter>
#include <QPaintDevice>
#include <QScrollBar>
#include <QTimer>
#include <QDate>
#include <QTextDocumentFragment>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QMenu>

#include "ReportEditorWidget.h"
#include "ReportPreferencesDialog.h"
#include "richeditor_constants.h"
#include "ReportSelectInputParameterDialog.h"
#include "ReportSelectDataSourceColumnDialog.h"
#include "ReportDataTableDialog.h"
#include "DataSource.h"
#include "textprinter.h"
#include "OdfWriter.h"
#include "Core.h"
#include "IDocument.h"

namespace RPML {

namespace {

struct TableCellReplacement {
    TableCellReplacement( int type_ = 0,
                          int offset_ = 0,
                          int length_ = 0,
                          int parameter_ = 0 )
        : type(type_)
        , offset(offset_)
        , length(length_)
        , parameter(parameter_)
    {}

    int type;       // 0 - Field
    int offset;
    int length;
    int parameter;
};

class TableCell
{
public:
    TableCell( int spanRows_ = 1,
               int spanColumns_ = 1,
               bool isCellSpanned_ = false,
               const QTextTableCellFormat &cellFormat_ = QTextTableCellFormat() )
        : spanRows(spanRows_)
        , spanColumns(spanColumns_)
        , isCellSpanned(isCellSpanned_)
        , cellFormat(cellFormat_)
    {}

    virtual ~TableCell() {
        qDeleteAll(replacements);
    }

    int spanRows;
    int spanColumns;
    bool isCellSpanned;
    QTextDocumentFragment entry;
    QTextBlockFormat firstBlockFormat;
    QTextTableCellFormat cellFormat;
    QColor bacgroundColor;
    QList<TableCellReplacement *> replacements;
};

}

BaseReportWidget::BaseReportWidget( bool designerMode, QMainWindow *parent )
    : RichEditor::RichEditorWidget(parent)
    , m_reportData(new ReportData(this, designerMode, 0))
    , m_usePagination(false)
{
    setupEditorActions();
    m_mainWindow->addToolBar(editorToolBar());

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QDRuler *mHorzRuler = new QDRuler(QDRuler::Horizontal);
    mHorzRuler->setFixedHeight(RULER_BREADTH);
    QVBoxLayout *l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(0);

    l->addWidget(mHorzRuler);
    l->addWidget(this->viewport());

    setLayout(l);
    recalcPageSize();
}

BaseReportWidget::~BaseReportWidget()
{
    delete m_reportData;
}

bool BaseReportWidget::save( const QString fileName, QString &errorMessage )
{
    return m_reportData->save(fileName, errorMessage);
}

bool BaseReportWidget::load( const QString fileName, QString &errorMessage )
{
    if ( m_reportData->load(fileName, errorMessage) ) {
        recalcPageSize();
        return true;
    }

    return false;
}

void BaseReportWidget::clearReport()
{
    m_reportData->clear();
    recalcPageSize();
}

void BaseReportWidget::setPageMode( bool onoff )
{
    if ( onoff != m_usePagination )
        m_actPageMode->setChecked(onoff);
}

void BaseReportWidget::resizeEvent( QResizeEvent *event )
{
    QTextEdit::resizeEvent(event);
    recalcPageSize();
}

void BaseReportWidget::paintEvent( QPaintEvent *event )
{
    RichEditor::RichEditorWidget::paintEvent(event);

    if ( m_usePagination )
        paintPagesView();
}

void BaseReportWidget::onReportPrefs()
{
    ReportPreferencesDialog reportPrefsDialog(m_reportData, Core::Storage::mainWindow()->widget());
    if ( reportPrefsDialog.exec() == QDialog::Accepted ) {
        m_reportData->m_reportDocument->setDefaultFont(m_reportData->m_defaultFont);
        document()->setPageSize(QSizeF(width() - verticalScrollBar()->width() - 2, -1));
        recalcPageSize();
        document()->setModified(true);
    }
}

void BaseReportWidget::usePageMode( bool onoff )
{
    m_usePagination = onoff;
    recalcPageSize();
}

void BaseReportWidget::recalcPageSize()
{
    QMargins viewportMargins;

    bool oldModified = document()->isModified();
    setUndoRedoEnabled(false);

    if ( !m_usePagination ) {
        document()->setPageSize(QSizeF(width() - verticalScrollBar()->width() - 2, -1));
        {
            QTextFrame *rootFrame = document()->rootFrame();
            QTextFrameFormat tff = rootFrame->frameFormat();
            tff.setLeftMargin(4);
            tff.setTopMargin(0);
            tff.setRightMargin(4);
            tff.setBottomMargin(0);
            rootFrame->setFrameFormat(tff);
        }
        viewportMargins.setTop(RULER_BREADTH);
        setViewportMargins(viewportMargins);
    }

    else {
        m_headerDecorHeight = qRound(m_reportData->m_pageFormat->pxHeaderHeight());
        m_footerDecorHeight = qRound(m_reportData->m_pageFormat->pxFooterHeight());
        m_height = qRound(m_reportData->m_pageFormat->pageHeight()) - m_headerDecorHeight - m_footerDecorHeight;
        m_width = qRound(m_reportData->m_pageFormat->pageWidth());
        m_heightWithDecor = m_height + m_headerDecorHeight + m_footerDecorHeight + 10;
        document()->setPageSize(QSize(m_width, m_heightWithDecor));
        {
            QTextFrame *rootFrame = document()->rootFrame();
            QTextFrameFormat tff = rootFrame->frameFormat();
            tff.setLeftMargin(0);
            tff.setTopMargin(m_headerDecorHeight + 5);
            tff.setRightMargin(0);
            tff.setBottomMargin(m_footerDecorHeight + 5);
            rootFrame->setFrameFormat(tff);
        }
        if ( width() > m_width ) {
            int deltaWidth = (width() - m_width - verticalScrollBar()->width() - 2) / 2;
            viewportMargins = QMargins(deltaWidth - 1, RULER_BREADTH, deltaWidth - 1, 0);
        }
        setViewportMargins(viewportMargins);
    }

    update();

    setUndoRedoEnabled(true);
    document()->setModified(oldModified);
}

void BaseReportWidget::paintPagesView()
{
    QPainter p(viewport());
    QPen spacePen(palette().dark(), 1);
    QBrush spaceBrush(palette().dark());
    QPen borderPen(palette().dark(), 1);
    borderPen.setStyle(Qt::DotLine);

    int pageHeight = m_heightWithDecor;
    int pageWidth  = m_width;
    int curHeight  = /*pageHeight*/ - (verticalScrollBar()->value() % pageHeight);

    int pageCounter = verticalScrollBar()->value() / pageHeight + 1;
    int pageCount = document()->pageCount();
    QString dateString = QDate::currentDate().toString("dd.MM.yyyy");

    verticalScrollBar()->setRange(0, pageHeight * pageCount - viewport()->size().height());

    int hh = qRound(m_reportData->m_pageFormat->pxHeaderDrawHeight());
    int hsh = hh ? m_headerDecorHeight - hh : 0;
    Utils::PageFormat::Decoration header = m_reportData->m_pageFormat->header();

    int fh = qRound(m_reportData->m_pageFormat->pxFooterDrawHeight());
    int fsh = fh ? m_footerDecorHeight - fh : 0;
    Utils::PageFormat::Decoration footer = m_reportData->m_pageFormat->footer();

    while ( curHeight <= height() && pageCounter <= pageCount ) {
        int h = curHeight;

        if ( h + 5 > 0 && h <= height() ) {
            // Start Page Break
            p.setPen(spacePen);
            p.setBrush(spaceBrush);
            p.drawRect(0, h, pageWidth, 5);
        }
        h += 5;

        if ( hh > 0 && h + hh > 0 && h <= height() ) {
            int ih = h;
            // Header
            QPixmap headerPixmap(pageWidth, hh);
            headerPixmap.fill();
            QPainter headerPainter(&headerPixmap);

            QTextDocument doc;
            QString headerHtml = m_reportData->m_headerHtml;
            headerHtml.replace("${PN}", QString::number(pageCounter))
                      .replace("${PC}", QString::number(pageCount))
                      .replace("${D}", dateString);
            doc.setHtml( headerHtml );
            doc.setPageSize(QSize(pageWidth, hh));
            doc.setDocumentMargin(0);
            doc.drawContents(&headerPainter);
            p.drawPixmap( QRectF(0, ih, pageWidth, hh),
                          headerPixmap,
                          QRectF(0, 0, pageWidth, hh) );
            ih += hh;
            // Header Spacing & Rule
            if ( hsh > 0 && header.ruleWidth > 0) {
                int rw = qMin(hsh, qRound(header.ruleWidth));
                p.setPen(QPen(header.ruleColor, rw));
                p.drawLine( 0, ih, pageWidth, ih);
            }
            else {
                p.setPen(borderPen);
                p.drawLine(0, ih - 1, pageWidth, ih - 1);
            }
        }
        h += hh + hsh;

        // Skip Body Content
        h += m_height;

        if ( fh > 0 && h + fh > 0 && h <= height() ) {
            int ih = h;
            // Footer Spacing & Rule
            if ( fsh > 0 && footer.ruleWidth > 0) {
                int rw = qMin(fsh, qRound(footer.ruleWidth));
                p.setPen(QPen(footer.ruleColor, rw));
                p.drawLine( 0, ih + fsh - rw, pageWidth, ih + fsh - rw);
            }
            else {
                p.setPen(borderPen);
                p.drawLine(0, ih + fsh - 1, pageWidth, ih + fsh - 1);
            }
            ih += fsh;
            // Footer
            QPixmap footerPixmap(pageWidth, fh);
            footerPixmap.fill();
            QPainter footerPainter(&footerPixmap);

            QTextDocument doc;
            QString footerHtml = m_reportData->m_footerHtml;
            footerHtml.replace("${PN}", QString::number(pageCounter))
                      .replace("${PC}", QString::number(pageCount))
                      .replace("${D}", dateString);
            doc.setHtml(footerHtml);
            doc.setPageSize(QSize(pageWidth, fh));
            doc.setDocumentMargin(0);
            doc.drawContents(&footerPainter);

            p.drawPixmap( QRectF(0, ih, pageWidth, fh),
                          footerPixmap,
                          QRectF(0, 0, pageWidth, fh) );
        }
        h += fh + fsh;

        if ( h + 5 > 0 && h <= height() ) {
            // End Page Break
            p.setPen(spacePen);
            p.setBrush(spaceBrush);
            p.drawRect(0, h, pageWidth, 5);
        }

        pageCounter++;
        curHeight += pageHeight;
    }

    // Side Borders
    int h = qMin(height(), pageHeight * pageCount);
    p.setPen(borderPen);
    p.drawLine(0, 0, 0, h);
    p.drawLine(pageWidth - 1, 0, pageWidth - 1, h);
}

void BaseReportWidget::setupEditorActions()
{
    m_editorToolBar = new QToolBar(tr("Preferences"), m_mainWindow);
    m_editorToolBar->setObjectName("RichEditorWidget_EditorToolBar");
    m_editorToolBar->setFloatable(false);

    QAction *actReportPrefs = m_editorToolBar->addAction(QIcon(":/images/rpt_prefs.png"), tr("Report Preferences"));
    actReportPrefs->setToolTip(tr("Report Preferences"));
    connect(actReportPrefs, SIGNAL(triggered()), this, SLOT(onReportPrefs()));

    m_actPageMode = m_editorToolBar->addAction(QIcon(":/images/page_mode.png"), tr("Use Page Mode"));
    m_actPageMode->setToolTip("Toggle Using Page Mode");
    m_actPageMode->setCheckable(true);
    connect(m_actPageMode, SIGNAL(toggled(bool)), this, SLOT(usePageMode(bool)));
}

ReportEditorWidget::ReportEditorWidget( QMainWindow *parent )
    : BaseReportWidget(false, parent)
{
    QAction *actOdfExport = m_editorToolBar->addAction(QIcon(":/images/odf_export.png"), "Export");
    actOdfExport->setToolTip(tr("Export to ODF-text file"));
    connect(actOdfExport, SIGNAL(triggered()), this, SLOT(exportToOdf()));

    setupPrintActions();
    m_mainWindow->addToolBar(printToolBar());
}

ReportEditorWidget::~ReportEditorWidget()
{ }

void ReportEditorWidget::updateContent( QTextDocument *textDocument, ReportData *reportData )
{
    setHtml(textDocument->toHtml());

    m_reportData->m_defaultFont = reportData->m_defaultFont;

    m_reportData->m_headerHtml = reportData->m_headerHtml;
    m_reportData->m_footerHtml = reportData->m_footerHtml;

    m_reportData->m_pageFormat->update( reportData->m_pageFormat->formatId(),
                                        reportData->m_pageFormat->isPortrait(),
                                        reportData->m_pageFormat->mmMargins(),
                                        reportData->m_pageFormat->header(),
                                        reportData->m_pageFormat->footer() );

    document()->setModified(true);
}

void ReportEditorWidget::reportPrint()
{
    TextPrinter printer;
    printer.previewReport(m_reportData);
}

void ReportEditorWidget::exportToOdf()
{
    if ( IDocument *doc = DocumentManager::getInstance()->currentDocument() ) {
        QString dir = "";
        if ( !doc->isNew() ) {
            QFileInfo fi(doc->filePath());
            if ( fi.exists() )
                dir = fi.canonicalPath() + "/" + fi.completeBaseName() + ".odf";
        }
        QString fileName = QFileDialog::getSaveFileName( Core::Storage::mainWindow()->widget(),
                                                         tr("Export to ODF"), dir,
                                                         tr("ODF Files (*.odf)"));
        if ( !fileName.isEmpty() ) {
            QFile f(fileName);
            OdfWriter writer(m_reportData, &f);
            writer.writeAll();
        }
    }
}

void ReportEditorWidget::setupPrintActions()
{
    m_printToolBar = new QToolBar(tr("Print"), m_mainWindow);
    m_printToolBar->setObjectName("ReportEditorWidget_EditorToolBar");
    m_printToolBar->setFloatable(false);

    QAction *actPrint = m_printToolBar->addAction(QIcon(":/images/rpt_print.png"), tr("Print"));
    actPrint->setToolTip(tr("Print Report Document"));
    connect(actPrint, SIGNAL(triggered()), this, SLOT(reportPrint()));
}

ReportDesignerWidget::ReportDesignerWidget( QMainWindow *parent )
    : BaseReportWidget(true, parent)
    , m_dataSourcesDock(0)
    , m_inputParamsDock(0)
{
    setupReportActions();
    m_mainWindow->addToolBar(reportToolBar());
}

ReportDesignerWidget::~ReportDesignerWidget()
{
}

bool ReportDesignerWidget::load( const QString fileName, QString &errorMessage )
{
    if ( m_dataSourcesDock )
        m_dataSourcesModel->startResetModel();
    if ( m_inputParamsDock )
        m_parametersModel->startResetModel();

    bool loaded = BaseReportWidget::load(fileName, errorMessage);

    if ( m_inputParamsDock )
        m_parametersModel->finishResetModel();
    if ( m_dataSourcesDock )
        m_dataSourcesModel->finishResetModel();

    return loaded;
}

void ReportDesignerWidget::reportExecute()
{
    // 1. Input Parameters
    QList<InputParameterValue> values;
    if ( !DataSourceManager::fillInputParameterValues( &m_reportData->m_inputParameters, values, Core::Storage::mainWindow()->widget()) )
        return;

    // 2. Fill DataSource Query Results
    QMap<int, DataSourceResult *> resultsMap;
    QString errorMessage;
    foreach ( DataSource *source, m_reportData->m_dataSources ) {
        DataSourceResult *dsResult = source->fetch(&m_reportData->m_inputParameters, values, errorMessage);
        if ( dsResult )
            resultsMap[source->m_id] = dsResult;
        else
            qDebug() << "!TODO! Fetch Query Error" << source->m_id << errorMessage;
    }

    // 3. Fill Report Document
    setUndoRedoEnabled(false);
    QTextDocument *runDocument = m_reportData->cloneDocument();
    setUndoRedoEnabled(true);

    {   // 3.1. Proceed DataTables
        QTextFrame::iterator it = runDocument->rootFrame()->begin();
        while ( !(it.atEnd()) ) {
            if ( QTextTable *childTable = qobject_cast<QTextTable*>(it.currentFrame()) ) {
                QTextTableCell cell = childTable->cellAt(0, 0);
                QString cellTxt = cell.firstCursorPosition().block().text();
                if ( cellTxt.startsWith("$<") ) {
                    int endpos = cellTxt.indexOf(">$", 2);
                    if ( endpos != -1 ) {
                        int dataTableId = 0;
                        bool validId = true;
                        for (int i = 2; i < endpos; i++) {
                            if ( !cellTxt[i].isDigit() ) {
                                validId = false;
                                break;
                            }
                            dataTableId = dataTableId * 10 + cellTxt[i].toLatin1() - '0';
                        }
                        if ( validId && dataTableId ) {
                            QTextCursor c = cell.firstCursorPosition();
                            c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endpos + 2);
                            c.removeSelectedText();
                            proceedDataTable(runDocument, childTable, dataTableId, resultsMap);
                        }
                    }
                }
            }
            ++it;
        }
    }

    {   // 3.2. Proceed Other Fields
        int offset = 0;
        while ( 1 ) {
            QTextCursor cur = runDocument->find(QRegExp("\\$\\{\\{[^\\}]*\\}\\}\\$"), offset);
            if ( cur.isNull() )
                break;
            QString txt = cur.selectedText();
            offset = cur.selectionStart();
            txt.chop(3);
            txt = txt.mid(3);
            QStringList fields = txt.split('.');
            if ( fields.size() == 3 && fields[0] == QLatin1String("F") ) {
                int sId = fields[1].toInt();
                int colNumber = fields[2].toInt() /*- 1*/;
                QMap<int, DataSourceResult *>::iterator it = resultsMap.find(sId);
                if ( it == resultsMap.end() ) {
                    cur.insertText(tr("<<Err.IDF>>"));
                    offset += 11;
                } else if ( colNumber < 0 || colNumber >= (*it)->m_data[0]->size() ) {
                    cur.insertText(tr("<<Err.IDF>>"));
                    offset += 11;
                } else if ( (*it)->m_data.size() < 1 ) {
                    cur.insertText(tr("<<Err.NDF>>"));
                    offset += 11;
                } else {
                    QString fldText = (*it)->m_data[1]->at(colNumber).toString();
                    cur.insertText(fldText);
                    offset += fldText.length();
                }
            } else if ( fields.size() == 2 && fields[0] == QLatin1String("P") ) {
                int pId = fields[1].toInt();
                int pIndex = -1;
                for (int p = 0; p < m_reportData->m_inputParameters.size(); p++) {
                    if ( m_reportData->m_inputParameters[p]->id == pId ) {
                        pIndex = p;
                        break;
                    }
                }
                if ( pIndex == -1 ) {
                    cur.insertText(tr("<<Err.NPF>>"));
                    offset += 11;
                } else {
                    QString parText = values[pIndex].value.toString();
                    cur.insertText(parText);
                    offset += parText.length();
                }
            } else {
                cur.insertText(tr("<<Err.UF>>"));
                offset += 10;
            }
        }
    }

    qDeleteAll(resultsMap.values());

    DocumentManager *docManager = DocumentManager::getInstance();
    if ( docManager->newDocument("rxml") ) {
        if ( ReportEditorWidget *editor = qobject_cast<ReportEditorWidget *>(docManager->currentDocument()->editorWidget()) ) {
            editor->updateContent(runDocument, m_reportData);
        }
    }

    delete runDocument;
}

void ReportDesignerWidget::dataTableInsert()
{
    if ( m_reportData->m_dataSources.isEmpty() ) {
        Core::Storage::warning( tr("No Data Sources Found") );
        return;
    }

    QTextCursor c = textCursor();

    if ( c.hasSelection() ) {
        Core::Storage::warning( tr("Current Selection Must be Empty"));
        return;
    }

    if ( c.currentTable() ) {
        Core::Storage::warning( tr("Table Cannot be Inserted Into Table") );
        return;
    }

    ReportDataTableDialog dataTableDialog( &m_reportData->m_dataSources,
                                           Core::Storage::mainWindow()->widget() );
    if ( dataTableDialog.exec() == QDialog::Accepted ) {
        int id = m_reportData->m_dataTables.newId();
        qreal spacing;
        qreal padding;
        int columnCount;
        ReportDataTable *dataTable = new ReportDataTable(id, 0);
        dataTableDialog.fillDataTableParameters(dataTable, &columnCount, &padding, &spacing);
        qreal wp = 100.0 / columnCount;
        QTextTableFormat tf;
        tf.setHeaderRowCount(dataTable->headerRowsCount());
        tf.setCellPadding(padding);
        tf.setCellSpacing(spacing);
        QVector<QTextLength> constraints;
        for (int i = 0; i < columnCount; i++)
            constraints << QTextLength(QTextLength::PercentageLength, wp);
        tf.setColumnWidthConstraints(constraints);
        tf.setProperty(RichEditor::DataTableId, id);
        c.insertTable(dataTable->rowsCount(), columnCount, tf);
        m_reportData->m_dataTables.addDataTable(dataTable);
    }
}

void ReportDesignerWidget::fieldInsert()
{
    if ( m_reportData->m_dataSources.isEmpty() )
        return;

    QTextCursor c = textCursor();
    int sourceId = 0;
    if ( QTextTable *t = c.currentTable() ) {
        QTextTableFormat tf = t->format();
        if ( tf.hasProperty(RichEditor::DataTableId) ) {
            int dataTableId = tf.intProperty(RichEditor::DataTableId);
            sourceId = m_reportData->m_dataTables.sourceIdForDataTableId(dataTableId);
        }
    }

    ReportSelectDataSourceColumnDialog fieldDialog(&m_reportData->m_dataSources, sourceId, this);
    if ( fieldDialog.exec() == QDialog::Accepted )
        fieldInsertInternal(fieldDialog.dataSourceId(), fieldDialog.dataSourceColumnNumber());
}

void ReportDesignerWidget::parameterInsert()
{
    if ( m_reportData->m_inputParameters.isEmpty() )
        return;

    ReportSelectInputParameterDialog paramDialog(&m_reportData->m_inputParameters, this);
    if ( paramDialog.exec() == QDialog::Accepted )
        inputParameterInsert(paramDialog.index());
}

void ReportDesignerWidget::inputParameterInsert( int index )
{
    if ( index == -1 )
        return;

    parameterInsertInternal(m_reportData->m_inputParameters[index]->id);
}

void ReportDesignerWidget::setupReportActions()
{
    m_reportToolBar = new QToolBar(tr("Report"), m_mainWindow);
    m_reportToolBar->setObjectName("RichEditorWidget_ReportToolBar");
    m_reportToolBar->setFloatable(false);

    QAction *actExecute = m_reportToolBar->addAction(QIcon(":/images/rpt_run.png"), tr("Execute"));
    actExecute->setToolTip(tr("Run Report"));
    actExecute->setShortcut(tr("F8"));
    connect(actExecute,  SIGNAL(triggered()), this, SLOT(reportExecute()));

    m_reportToolBar->addSeparator();

    m_actDataTableInsert = m_reportToolBar->addAction(QIcon(":/images/add_data_table.png"), tr("Add Data Table"));
    m_actDataTableInsert->setToolTip(tr("Insert Data Table"));
    m_actDataTableInsert->setShortcut(tr("Ctrl+Shift+T"));
    connect(m_actDataTableInsert, SIGNAL(triggered()), this, SLOT(dataTableInsert()));

    m_actFieldInsert = m_reportToolBar->addAction(QIcon(":/images/field_add.png"), tr("Add &Field"));
    m_actFieldInsert->setToolTip(tr("Insert Report Field"));
    m_actFieldInsert->setShortcut(tr("Ctrl+F"));
    connect(m_actFieldInsert, SIGNAL(triggered()), this, SLOT(fieldInsert()));

    m_actParameterInsert = m_reportToolBar->addAction(QIcon(":/images/rpt_inp_par_insert.png"), tr("Add &Parameter Field"));
    m_actParameterInsert->setToolTip(tr("Insert Input Parameter"));
    connect(m_actParameterInsert, SIGNAL(triggered()), this, SLOT(parameterInsert()));
}

void ReportDesignerWidget::parameterInsertInternal( int parameterId )
{
    QTextCursor cursor = textCursor();

    QTextCharFormat fieldReportCharFormat = cursor.charFormat();
    fieldReportCharFormat.setObjectType(ReportData::FieldReportFormat);

    fieldReportCharFormat.setProperty(ReportData::ReportObjectType, ReportData::ReportParameter);
    fieldReportCharFormat.setProperty(ReportData::ParameterReportId, parameterId);
    fieldReportCharFormat.setToolTip(tr("Parameter: ID = %1").arg(parameterId));

    cursor.insertText(QString(QChar::ObjectReplacementCharacter), fieldReportCharFormat);
    setTextCursor(cursor);
}

void ReportDesignerWidget::fieldInsertInternal( int queryId, int columnNumber )
{

    QTextCursor cursor = textCursor();
    QTextCharFormat fieldReportCharFormat = cursor.charFormat();
    fieldReportCharFormat.setObjectType(ReportData::FieldReportFormat);

    fieldReportCharFormat.setProperty(ReportData::ReportObjectType, ReportData::ReportField);
    fieldReportCharFormat.setProperty(ReportData::FieldReportQueryId, queryId);
    fieldReportCharFormat.setProperty(ReportData::FieldReportQueryColumnNumber, columnNumber);
    fieldReportCharFormat.setToolTip(tr("Field: Data Souse \"%1\": Column \"%2\"").arg(queryId).arg(columnNumber));

    cursor.insertText(QString(QChar::ObjectReplacementCharacter), fieldReportCharFormat);
    setTextCursor(cursor);
}

void ReportDesignerWidget::proceedDataTable( QTextDocument *document, QTextTable *table, int id, const QMap<int, DataSourceResult *> &resultsMap )
{
    int dataSourceId = 0;
    ReportDataTable *dataTable = 0;

    for (int i = 0; i < m_reportData->m_dataTables.count(); i++) {
        if ( m_reportData->m_dataTables[i]->m_id == id ) {
            dataTable = m_reportData->m_dataTables[i];
            dataSourceId = dataTable->m_sourceId;
            break;
        }
    }

    if ( !dataTable ) {
        replaceTableFieldsWithMessage(table, tr("<<Err.DTINF>>"));
        return;
    }

    QMap<int, DataSourceResult *>::const_iterator it = resultsMap.constFind(dataSourceId);
    if ( it == resultsMap.constEnd() ) {
        replaceTableFieldsWithMessage(table, tr("<<Err.DSINF>>"));
        return;
    }
    DataSourceResult *dataResult = (*it);

    if ( dataResult->m_data.size() <= 1 ) {
        replaceTableFieldsWithMessage(table, tr("<<Err.NDF>>"));
        return;
    }

    int rowsCount = dataTable->rowsCount();
    if ( rowsCount != table->rows() ) {
        replaceTableFieldsWithMessage(table, tr("<<Err.IRC>>"));
        return;
    }

    QTextTableFormat tf = table->format();

    int currRow = dataTable->m_headerRowsCount;
    QList<QList<TableCell *> > cells;
    for (int r = currRow, rr = 0; r < table->rows(); r++, rr++) {
        cells << QList<TableCell *>();
        for (int c = 0; c < table->columns(); c++) {
            QTextTableCell cell = table->cellAt(r, c);
            bool isCellSpanned = false;
            if ( rr > 0 ) {
                QTextTableCell cell1 = table->cellAt(r - 1, c);
                if ( cell1.firstPosition() == cell.firstPosition() )
                    isCellSpanned = true;
            }
            if ( !isCellSpanned && c > 0 ) {
                QTextTableCell cell1 = table->cellAt(r, c - 1);
                if ( cell1.firstPosition() == cell.firstPosition() )
                    isCellSpanned = true;
            }
            QTextTableCellFormat tcf = cell.format().toTableCellFormat();
            if ( !tcf.hasProperty(QTextFormat::TableCellLeftPadding) )
                tcf.setLeftPadding(tf.padding());
            if ( !tcf.hasProperty(QTextFormat::TableCellTopPadding) )
                tcf.setTopPadding(tf.padding());
            if ( !tcf.hasProperty(QTextFormat::TableCellRightPadding) )
                tcf.setRightPadding(tf.padding());
            if ( !tcf.hasProperty(QTextFormat::TableCellBottomPadding) )
                tcf.setBottomPadding(tf.padding());
            TableCell *tc = new TableCell(cell.rowSpan(), cell.columnSpan(), isCellSpanned, cell.format().toTableCellFormat());
            if ( !isCellSpanned ) {
                QTextFrame::iterator it = cell.begin();
                if ( !(it.atEnd()) ) {
                    QTextBlock fb = it.currentBlock();
                    if ( fb.isValid() )
                        tc->firstBlockFormat = fb.blockFormat();
                    int cellPosition = cell.firstPosition();
                    while ( !(it.atEnd()) ) {
                        QTextBlock b = it.currentBlock();
                        if ( b.isValid() ) {
                            int  blockOffset = b.position() - cellPosition;
                            QString txt = b.text();
                            bool found = true;
                            int p = 0;
                            while ( found ) {
                                found = false;
                                int p0 = txt.indexOf("${{", p);
                                if ( p0 != -1 ) {
                                    int p1 = txt.indexOf("}}$", p0 + 3);
                                    if ( p1 != -1 ) {
                                        int length = p1 + 3 - p0;
                                        int offset = blockOffset + p0;
                                        QString fieldEntry = txt.mid(p0 + 3, length - 6);
                                        QStringList fields = fieldEntry.split(".");
                                        if ( fields.size() == 3 && fields[0] == QLatin1String("F") ) {
                                            int sId = fields[1].toInt();
                                            int colNumber = fields[2].toInt() /*- 1*/;
                                            if ( sId != dataSourceId || colNumber < 0 || colNumber >= dataResult->m_data[0]->size() ) {
                                                QTextCursor cur(document);
                                                cur.setPosition(cellPosition + offset);
                                                cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, length);
                                                cur.insertText(tr("<<Err.IDTF>>"));
                                                p1 += 12 - length;
                                            }
                                            else
                                                tc->replacements.push_back(new TableCellReplacement(0, offset, length, colNumber));
                                        }
                                        p = p1 + 3;
                                        found = true;
                                    }
                                }
                            }
                        }
                        ++it;
                    }
                }
                QTextCursor cur(document);
                cur.setPosition(cell.firstPosition());
                cur.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                                  cell.lastPosition() - cell.firstPosition() );
                tc->entry = cur.selection();
                tc->bacgroundColor = cell.format().background().color();
            }
            cells[rr] << tc;
        }
    }
    int delRowsCount = rowsCount - (currRow + dataTable->m_footerRowsCount);
    table->removeRows(currRow, delRowsCount);

    for (int j = 1; j < dataResult->m_data.size(); j++) {
        int groupLevel = 0;
        for (int g = 0; g < dataTable->m_groupings.size(); g++) {
            int groupingColumn = dataTable->m_groupings[g]->columnNumber;
            if ( dataResult->m_data[j]->at(groupingColumn) != dataResult->m_data[j - 1]->at(groupingColumn) )
                break;
            groupLevel++;
        }
        int insertRows = dataTable->m_detailRowsCount;
        for (int gi = groupLevel; gi < dataTable->m_groupings.size(); gi++)
            insertRows += dataTable->m_groupings[gi]->rowsCount;
        for (int ir = cells.size() - insertRows; ir < cells.size(); ir++) {
            table->insertRows(currRow, 1);
            for (int ic = 0; ic < table->columns(); ic++) {
                TableCell *tc = cells[ir][ic];
                if ( !tc->isCellSpanned ) {
                    QTextTableCell cell = table->cellAt(currRow, ic);
                    QTextCursor cc = cell.firstCursorPosition();
                    if ( tc->cellFormat.isValid() )
                        cell.setFormat(tc->cellFormat);
                    cc.insertFragment(tc->entry);
                    if ( tc->firstBlockFormat.isValid() ) {
                        cell.firstCursorPosition().setBlockFormat(tc->firstBlockFormat);
                    }
                    if ( tc->bacgroundColor.isValid() ) {
                        QTextCharFormat fmt = cell.format();
                        fmt.setBackground(QBrush(tc->bacgroundColor));
                        cell.setFormat(fmt);
                    }
                    if ( !tc->replacements.isEmpty() ) {
                        int offsetDelta = 0;
                        foreach ( TableCellReplacement *repl, tc->replacements ) {
                            if ( repl->type == 0 ) {
                                QTextCursor replCursor = cell.firstCursorPosition();
                                replCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, repl->offset + offsetDelta);
                                replCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, repl->length);
                                QString replText = dataResult->m_data[j]->at(repl->parameter).toString();
                                replCursor.insertText(replText);
                                offsetDelta += replText.length() - repl->length;
                            }
                        }
                    }
                }
            }
            currRow++;
        }
        for (int sr = cells.size() - insertRows, sr0 = 0; sr < cells.size(); sr++, sr0++) {
            for (int sc = 0; sc < table->columns(); sc++) {
                if ( !cells[sr][sc]->isCellSpanned &&
                     (cells[sr][sc]->spanColumns > 1 || cells[sr][sc]->spanRows > 1) )
                    table->mergeCells(currRow - insertRows + sr0, sc, cells[sr][sc]->spanRows, cells[sr][sc]->spanColumns);
            }
        }
    }

    for (int d = 0; d < cells.size(); d++)
        qDeleteAll(cells[d]);
}

void ReportDesignerWidget::replaceTableFieldsWithMessage( QTextTable *table, const QString &message )
{
    QTextDocument *document = table->document();

    for (int i = 0; i < table->rows(); i++) {
        for (int j = 0; j < table->columns(); j++) {
            QTextTableCell cell = table->cellAt(i, j);
            QTextFrame::iterator it = cell.begin();
            bool found = true;
            while ( !(it.atEnd()) || found ) {
                QTextBlock block = it.currentBlock();
                found = false;
                if ( block.isValid() ) {
                    QString txt = block.text();
                    int startPos = txt.indexOf("${{");
                    if ( startPos != -1 ) {
                        int endPos = txt.indexOf("}}$", startPos);
                        if ( endPos != -1 ) {
                            QTextCursor cursor(document);
                            cursor.setPosition(block.position() + startPos);
                            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endPos + 3 - startPos);
                            cursor.removeSelectedText();
                            cursor.insertText(message);
                            found = true;
                        }
                    }
                }
                if ( !found )
                    ++it;
            }
        }
    }
}

QDockWidget *ReportDesignerWidget::dataSourcesDock( QMainWindow *mainWindow )
{
    if ( !m_dataSourcesDock ) {
        QAction *addDataSourceAction = new QAction(QIcon(":/images/add.png"), tr("Add Data Source"), mainWindow);
        QAction *editDataSourceAction = new QAction(QIcon(":/images/edit.png"), tr("Edit Data Source"), mainWindow);
        QAction *delDataSourceAction = new QAction(QIcon(":/images/delete.png"), tr("Remove Data Source"), mainWindow);
        m_dataSourcesDock = new QwCustomDockWidget( mainWindow, tr("Data Sources"), "ReportEditorWidget_DataSourcesDock",
                                                    QKeySequence(), QIcon(),
                                                    Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                                    QList<QAction *>() << addDataSourceAction << editDataSourceAction << delDataSourceAction,
                                                    0, true );

        QMenu *dataSourceDockContextMenu = new QMenu;
        dataSourceDockContextMenu->addAction(addDataSourceAction);
        dataSourceDockContextMenu->addAction(editDataSourceAction);
        dataSourceDockContextMenu->addAction(delDataSourceAction);
        m_dataSourcesDock->setContextMenu(dataSourceDockContextMenu);

        m_dataSourcesModel = new DataSourcesModel(m_reportData->m_dataSources);
        m_dataSourcesView = new QTableView();
        m_dataSourcesView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_dataSourcesView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_dataSourcesView->setModel(m_dataSourcesModel);
        m_dataSourcesView->setEditTriggers(QAbstractItemView::AllEditTriggers);

        m_dataSourcesDock->setWidget(m_dataSourcesView);

        connect(addDataSourceAction,    SIGNAL(triggered()), this, SLOT(addDataSource()));
        connect(editDataSourceAction,   SIGNAL(triggered()), this, SLOT(editDataSource()));
        connect(delDataSourceAction,    SIGNAL(triggered()), this, SLOT(delDataSource()));
        connect(m_dataSourcesModel,     SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(reportDataChanged()));
    }

    return m_dataSourcesDock;
}

QDockWidget *ReportDesignerWidget::inputParametersDock( QMainWindow *mainWindow )
{
    if ( !m_inputParamsDock ) {
        QAction *addParamAction = new QAction(QIcon(":/images/add.png"), tr("Add Input Parameter"), mainWindow);
        QAction *delParamAction = new QAction(QIcon(":/images/delete.png"), tr("Remove Input Parameter"), mainWindow);
        m_inputParamsDock = new QwCustomDockWidget( mainWindow, tr("Input Parameters"), "ReportEditorWidget_Input_Parameters",
                                                    QKeySequence(), QIcon(),
                                                    Qt::RightDockWidgetArea, QwCustomDockWidget::First,
                                                    QList<QAction *>() << addParamAction << delParamAction,
                                                    0, true );

        QMenu *paramsDockContextMenu = new QMenu;
        paramsDockContextMenu->addAction(addParamAction);
        paramsDockContextMenu->addAction(delParamAction);
        m_inputParamsDock->setContextMenu(paramsDockContextMenu);

        m_parametersModel = new ReportInputParametersModel(m_reportData->m_inputParameters);
        m_parametersView = new ReportInputParameterTableView();
        m_parametersView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_parametersView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_parametersView->setModel(m_parametersModel);
        m_parametersView->setItemDelegate(new ReportInputParameterDelegate());
        m_parametersView->setEditTriggers(QAbstractItemView::AllEditTriggers);

        m_inputParamsDock->setWidget(m_parametersView);

        connect(addParamAction,         SIGNAL(triggered()), this, SLOT(addInputParameter()));
        connect(delParamAction,         SIGNAL(triggered()), this, SLOT(delInputParameter()));
        connect(m_parametersView, SIGNAL(insertInputParameter(int)), this, SLOT(inputParameterInsert(int)));
        connect(m_parametersModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(reportDataChanged()));
    }
    return m_inputParamsDock;
}

void ReportDesignerWidget::addInputParameter()
{
    m_parametersModel->addInputParameter();
    document()->setModified(true);
}

void ReportDesignerWidget::delInputParameter()
{
    QModelIndex index = m_parametersView->currentIndex();
    if ( index.isValid() ) {
        if ( index.row() != -1 ) {
            QMessageBox::StandardButton answer =
                    QMessageBox::question( Core::Storage::mainWindow()->widget(), Core::Storage::mainWindow()->appName(),
                                           tr("Delete the setting can make invalidated any report fields and data sources\nContinue?"),
                                           QMessageBox::Yes | QMessageBox::No );
            if ( answer == QMessageBox::Yes ) {
                m_parametersModel->delInputParameter(index.row());
                document()->setModified(true);
            }
        }
    }
}

void ReportDesignerWidget::addDataSource()
{
    if ( DataSource *dataSource = DataSourceManager::createDataSource(m_reportData->newDataSourceId()) ) {
        m_dataSourcesModel->addDataSource(dataSource);
        document()->setModified(true);
    }
}

void ReportDesignerWidget::editDataSource()
{
    QModelIndex index = m_dataSourcesView->currentIndex();
    if ( index.isValid() ) {
        int row = index.row();
        if ( row != -1 ) {
            QString errorMessage;
            if ( !m_reportData->m_dataSources[row]->editQuery(&m_reportData->m_inputParameters, errorMessage) ) {
                if ( !errorMessage.isEmpty() )
                    Core::Storage::warning(errorMessage);
            } else
                document()->setModified(true);
        }
    }
}

void ReportDesignerWidget::delDataSource()
{
    QModelIndex index = m_dataSourcesView->currentIndex();
    if ( index.isValid() ) {
        int row = index.row();
        if ( row != -1 ) {
            int dsId = m_reportData->m_dataSources[row]->m_id;
            if ( m_reportData->m_dataTables.dataSourceIdUsed(dsId) ) {
                Core::Storage::warning(tr("Data Source is Used!"));
                return;
            }
            m_dataSourcesModel->delDataSource(row);
            document()->setModified(true);
        }
    }
}

void ReportDesignerWidget::reportDataChanged()
{
    document()->setModified(true);
}

}   // namespace RPML
