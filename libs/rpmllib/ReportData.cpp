#include <QTextCharFormat>
#include <QLatin1String>
#include <QFile>
#include <QTextStream>
#include <QVarLengthArray>
#include <QTextCursor>

#include "ReportData.h"
#include "stringutils.h"
#include "richeditor_constants.h"

namespace RPML {

static QTextFormat formatDifference( const QTextFormat &from, const QTextFormat &to )
{
    QTextFormat diff = to;

    const QMap<int, QVariant> props = to.properties();
    for (QMap<int, QVariant>::ConstIterator it = props.begin(), end = props.end();
         it != end; ++it)
        if (it.value() == from.property(it.key()))
            diff.clearProperty(it.key());

    return diff;
}

ReportDataTableList::~ReportDataTableList()
{
    qDeleteAll(m_data);
}

void ReportDataTableList::clear()
{
    qDeleteAll(m_data);
    m_data.clear();
}

void ReportDataTableList::save( QDomDocument *document, QDomElement *parentElement )
{
    QDomElement dataTablesElement = document->createElement("DataTables");
    foreach ( ReportDataTable *dataTable, m_data ) {
        QDomElement dataTableElement = document->createElement("DataTable");
        dataTableElement.setAttribute("id", dataTable->m_id);
        dataTableElement.setAttribute("DataSourceId", dataTable->m_sourceId);
        dataTableElement.setAttribute("HeaderRowsCount", dataTable->m_headerRowsCount);
        dataTableElement.setAttribute("DetailRowsCount", dataTable->m_detailRowsCount);
        dataTableElement.setAttribute("FooterRowsCount", dataTable->m_footerRowsCount);
        foreach ( DataTableGroupingInfo *grouping, dataTable->m_groupings ) {
            QDomElement groupingElement = document->createElement("Grouping");
            groupingElement.setAttribute("DataSourceColumnNumber", grouping->columnNumber);
            groupingElement.setAttribute("TableRowsCount", grouping->rowsCount);
            dataTableElement.appendChild(groupingElement);
        }

        dataTablesElement.appendChild(dataTableElement);
    }

    parentElement->appendChild(dataTablesElement);
}

bool ReportDataTableList::load( QDomElement *parentElement, QString &errorMessage )
{
    QDomElement dataTableElement = parentElement->firstChildElement();

    while ( !dataTableElement.isNull() ) {
        if ( dataTableElement.tagName() != "DataTable" ) {
            errorMessage = QObject::tr("Unexpected Tag <%1> at line %2, waiting for <DataTable>")
                    .arg(dataTableElement.tagName()).arg(dataTableElement.lineNumber());
            return false;
        }
        if ( !dataTableElement.hasAttribute("id") ) {
            errorMessage = QObject::tr("Expected attribute 'id' for Tag <DataTable> at line %1").arg(dataTableElement.lineNumber());
            return false;
        }
        if ( !dataTableElement.hasAttribute("DataSourceId") ) {
            errorMessage = QObject::tr("Expected attribute 'DataSourceId' for Tag <DataTable> at line %1").arg(dataTableElement.lineNumber());
            return false;
        }
        if ( !dataTableElement.hasAttribute("HeaderRowsCount") ) {
            errorMessage = QObject::tr("Expected attribute 'HeaderRowsCount' for Tag <DataTable> at line %1").arg(dataTableElement.lineNumber());
            return false;
        }
        if ( !dataTableElement.hasAttribute("DetailRowsCount") ) {
            errorMessage = QObject::tr("Expected attribute 'DetailRowsCount' for Tag <DataTable> at line %1").arg(dataTableElement.lineNumber());
            return false;
        }
        if ( !dataTableElement.hasAttribute("FooterRowsCount") ) {
            errorMessage = QObject::tr("Expected attribute 'FooterRowsCount' for Tag <DataTable> at line %1").arg(dataTableElement.lineNumber());
            return false;
        }

        ReportDataTable *newDataTable = new ReportDataTable( dataTableElement.attribute("id").toInt(),
                                                             dataTableElement.attribute("DataSourceId").toInt(),
                                                             dataTableElement.attribute("HeaderRowsCount").toInt(),
                                                             dataTableElement.attribute("DetailRowsCount").toInt(),
                                                             dataTableElement.attribute("FooterRowsCount").toInt() );
        m_data.push_back(newDataTable);

        QDomElement groupingElement = dataTableElement.firstChildElement();
        while ( !groupingElement.isNull() ) {
            if ( groupingElement.tagName() != "Grouping" ) {
                errorMessage = QObject::tr("Unexpected Tag <%1> at line %2, waiting for <Grouping>")
                        .arg(groupingElement.tagName()).arg(groupingElement.lineNumber());
                return false;
            }
            if ( !groupingElement.hasAttribute("DataSourceColumnNumber") ) {
                errorMessage = QObject::tr("Expected attribute 'DataSourceColumnNumber' for Tag <DataTable> at line %1").arg(groupingElement.lineNumber());
                return false;
            }
            if ( !groupingElement.hasAttribute("TableRowsCount") ) {
                errorMessage = QObject::tr("Expected attribute 'TableRowsCount' for Tag <DataTable> at line %1").arg(groupingElement.lineNumber());
                return false;
            }
            newDataTable->addGrouping( new DataTableGroupingInfo( groupingElement.attribute("DataSourceColumnNumber").toInt(),
                                                                  groupingElement.attribute("TableRowsCount").toInt() ) );
            groupingElement = groupingElement.nextSiblingElement();
        }

        dataTableElement = dataTableElement.nextSiblingElement();
    }

    return true;
}

ReportDataTable *ReportDataTableList::operator []( int index )
{
    if ( index >= 0 && index < m_data.size() )
        return m_data[index];
    return 0;
}

bool ReportDataTableList::dataSourceIdUsed( int sourceId )
{
    foreach ( ReportDataTable *t, m_data )
        if ( t->m_sourceId == sourceId )
            return true;
    return false;
}

int ReportDataTableList::newId() const
{
    int id = 0;
    foreach ( ReportDataTable *dt, m_data )
        if ( id < dt->m_id )
            id = dt->m_id;
    id++;
    return id;
}

int ReportDataTableList::sourceIdForDataTableId( int id ) const
{
    foreach ( ReportDataTable *dt, m_data )
        if ( dt->m_id == id )
            return dt->m_sourceId;

    return 0;
}

void ReportDataTableList::addDataTable( ReportDataTable *dataTable )
{
    m_data.push_back(dataTable);
}

QSizeF FieldReportObject::intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)

    QString s = QString("$%1.%2").arg(format.property(ReportData::FieldReportQueryId).toInt())
                                 .arg(format.property(ReportData::FieldReportQueryColumnNumber).toInt() + 1);

    const QTextCharFormat &tf = *(const QTextCharFormat*)(&format);
    QFontMetrics fm(tf.font());
    return fm.boundingRect(s).size() + QSize(tf.font().pointSize() / 3, 1);
}

void FieldReportObject::drawObject( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)

    painter->setFont(format.toCharFormat().font());
    int reportObjectType = format.property(ReportData::ReportObjectType).toInt();

    QRectF r(rect);
    const QTextCharFormat &tf = *(const QTextCharFormat*)(&format);
    QFontMetrics fm(tf.font());
    int d = fm.descent();
    r.adjust(0, d, 0, d);

    switch ( reportObjectType ) {
        case ReportData::ReportField:
            painter->drawText( r, Qt::AlignCenter,
                               QString("$%1.%2").arg(format.property(ReportData::FieldReportQueryId).toInt())
                                                .arg(format.property(ReportData::FieldReportQueryColumnNumber).toInt() + 1) );
            break;

        case ReportData::ReportParameter:
            painter->drawText( r, Qt::AlignCenter,
                               QString(":%1").arg(format.property(ReportData::ParameterReportId).toInt()) );
            break;
    }

    painter->drawRoundedRect(r, 2, 2);
}

static QString initialSampleHeader( "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
                                    "p, li { white-space: pre-wrap; }"
                                    "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
                                    "<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"0\" cellpadding=\"3\"><tr>"
                                    "<td width=\"75%\" bgcolor=\"#d8d8d8\">"
                                    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                    "<span style=\" font-family:'Times New Roman'; font-size:9pt; font-weight:600; background-color:#d8d8d8;\">Simple Header (Current Date)</span></p></td>"
                                    "<td width=\"25%\" bgcolor=\"#d8d8d8\">"
                                    "<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                    "<span style=\" font-family:'Times New Roman'; font-size:9pt; font-weight:600; color:#680000; background-color:#d8d8d8;\">${D}</span></p></td></tr></table></body></html>" );

static QString initialSampleFooter( "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
                                    "p, li { white-space: pre-wrap; }"
                                    "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
                                    "<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"0\" cellpadding=\"3\"><tr>"
                                    "<td width=\"75%\" bgcolor=\"#e1e1e1\">"
                                    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                    "<span style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; background-color:#e1e1e1;\">Simple Footer (Page Number/Page Count)</span></p></td>"
                                    "<td width=\"25%\" bgcolor=\"#e1e1e1\">"
                                    "<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                    "<span style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; color:#55007f; background-color:#e1e1e1;\">${PN}/${PC}</span></p></td></tr></table></body></html>" );

ReportData::ReportData( QTextEdit *editorWidget, bool designerMode, QObject *parent )
    : QObject(parent)
    , m_headerHtml(initialSampleHeader)
    , m_reportDocument(editorWidget->document())
    , m_footerHtml(initialSampleFooter)
    , m_editorWidget(editorWidget)
    , m_designerMode(designerMode)
{
    if ( m_designerMode )
        m_reportDocument->documentLayout()->registerHandler( FieldReportFormat,
                                                             new FieldReportObject()) ;

    {
        QPainter p(m_editorWidget);
        m_pageFormat = new Utils::PageFormat(p.device());
    }
}

ReportData::~ReportData()
{
    clear(true);
    delete m_pageFormat;
}

static void fieldInsert( int queryId, int columnNumber, QTextCursor *cursor )
{
    QTextCharFormat fieldReportCharFormat = cursor->charFormat();
    fieldReportCharFormat.setObjectType(ReportData::FieldReportFormat);

    fieldReportCharFormat.setProperty(ReportData::ReportObjectType, ReportData::ReportField);
    fieldReportCharFormat.setProperty(ReportData::FieldReportQueryId, queryId);
    fieldReportCharFormat.setProperty(ReportData::FieldReportQueryColumnNumber, columnNumber);
    fieldReportCharFormat.setToolTip(QObject::tr("Field: Data Souse \"%1\": Column \"%2\"").arg(queryId).arg(columnNumber));

    cursor->insertText(QString(QChar::ObjectReplacementCharacter), fieldReportCharFormat);
}

static void parameterInsert( int parameterId, QTextCursor *cursor )
{
    QTextCharFormat fieldReportCharFormat = cursor->charFormat();
    fieldReportCharFormat.setObjectType(ReportData::FieldReportFormat);

    fieldReportCharFormat.setProperty(ReportData::ReportObjectType, ReportData::ReportParameter);
    fieldReportCharFormat.setProperty(ReportData::ParameterReportId, parameterId);
    fieldReportCharFormat.setToolTip(QObject::tr("Parameter: ID = %1").arg(parameterId));

    cursor->insertText(QString(QChar::ObjectReplacementCharacter), fieldReportCharFormat);
}

static void afterLoadReport( QTextDocument *document )
{
    document->setUndoRedoEnabled(false);

    {   // Proceed Tables
        QTextFrame::iterator it = document->rootFrame()->begin();
        while ( !(it.atEnd()) ) {
            if ( QTextTable *childTable = qobject_cast<QTextTable*>(it.currentFrame()) ) {
                QTextTableCell cell = childTable->cellAt(0, 0);
                QTextCursor cc = cell.firstCursorPosition();
                QString c00 = cc.block().text();
                if ( c00.startsWith("$<") ) {
                    int endpos = c00.indexOf(">$", 2);
                    if ( endpos != -1 ) {
                        int id = 0;
                        bool validNumber = true;
                        for (int i = 2; i < endpos; i++) {
                            if ( !c00[i].isDigit() ) {
                                validNumber = false;
                                break;
                            }
                            id = id * 10 + c00[i].toLatin1() - '0';
                        }
                        if ( validNumber ) {
                            QTextTableFormat tf = childTable->format();
                            tf.setProperty(RichEditor::DataTableId, id);
                            childTable->setFormat(tf);
                            cc.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endpos + 2);
                            cc.removeSelectedText();
                        }
                    }
                }
            }
            ++it;
        }
    }

    while ( 1 ) {
        QTextCursor cursor = document->find(QRegExp("\\$\\{\\{[^\\}]*\\}\\}\\$"));
        if ( cursor.isNull() )
            break;
        QString txt = cursor.selectedText();
        txt.chop(3);
        txt = txt.mid(3);
        QStringList fields = txt.split('.');
        if ( fields.size() == 3 && fields[0] == QLatin1String("F") ) {
            fieldInsert(fields[1].toInt(), fields[2].toInt(), &cursor);
        } else if ( fields.size() == 2 && fields[0] == QLatin1String("P") ) {
            parameterInsert(fields[1].toInt(), &cursor);
        } else {
            cursor.removeSelectedText();
        }
    }

    document->setUndoRedoEnabled(true);
}

static void beforeSaveReport( QTextDocument *document )
{
    {   // Proceed Tables
        QTextFrame::iterator it = document->rootFrame()->begin();
        while ( !(it.atEnd()) ) {
            if ( QTextTable *childTable = qobject_cast<QTextTable*>(it.currentFrame()) ) {
                QTextTableFormat tf = childTable->format();
                if ( tf.hasProperty(RichEditor::DataTableId) ) {
                    int id = tf.intProperty(RichEditor::DataTableId);
                    QTextTableCell cell = childTable->cellAt(0, 0);
                    QTextCursor cc = cell.firstCursorPosition();
                    cc.insertText(QString("$<%1>$").arg(id));
                }
            }
            ++it;
        }
    }

    while ( 1 ) {
        QTextCursor cursor = document->find(QString(QChar::ObjectReplacementCharacter));
        if ( cursor.isNull() )
            break;
        QTextCharFormat format = cursor.charFormat();
        int reportObjectType = format.property(ReportData::ReportObjectType).toInt();
        switch ( reportObjectType ) {
            case ReportData::ReportField: {
                int queryId = format.property(ReportData::FieldReportQueryId).toInt();
                int columnNumber = format.property(ReportData::FieldReportQueryColumnNumber).toInt();
                cursor.insertText(QString("${{F.%1.%2}}$").arg(queryId).arg(columnNumber));
                break;
            }
            case ReportData::ReportParameter: {
                int id = format.property(ReportData::ParameterReportId).toInt();
                cursor.insertText(QString("${{P.%1}}$").arg(id));
                break;
            }
            default:
                cursor.removeSelectedText();
                break;
        }
    }
}

bool ReportData::save( const QString &fileName, QString &errorMessage )
{
    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        errorMessage = QObject::tr("File '%1' Cannot be Opened for Writing:\n%2.")
                .arg(fileName).arg(file.errorString()) ;
        return false;
    }

    QString extension = m_designerMode ? "rpml" : "rxml";

    QDomImplementation implementation;
    QDomDocumentType docType = implementation.createDocumentType( extension + "-document", extension, "QxAdvice/" + extension);
    QDomDocument document(docType);
    QDomProcessingInstruction process = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    document.appendChild(process);

    QDomElement documentElement = document.createElement(extension + "-document");
    documentElement.setAttribute("version", "1.0");
    m_pageFormat->save(&document, &documentElement);
    documentElement.setAttribute("font-family", m_defaultFont.family());
    documentElement.setAttribute("font-size", m_defaultFont.pointSize());
    documentElement.setAttribute("font-bold", m_defaultFont.bold() ? "1" : "0");
    documentElement.setAttribute("font-italic", m_defaultFont.italic() ? "1" : "0");
    document.appendChild(documentElement);

    if ( m_designerMode ) {
        QDomElement parametersElement = document.createElement("InputParameters");
        documentElement.appendChild(parametersElement);
        saveInputParameters(&document, &parametersElement);

        QDomElement dataSourcesElement = document.createElement("DataSources");
        documentElement.appendChild(dataSourcesElement);
        saveDataSources(&document, &dataSourcesElement);

        m_dataTables.save(&document, &documentElement);
    }

    QDomElement headerElement = document.createElement(extension + "-header");
    QDomCDATASection headerData = document.createCDATASection(m_headerHtml);
    headerElement.appendChild(headerData);
    documentElement.appendChild(headerElement);

    QDomElement bodyElement = document.createElement(extension + "-body");
    beforeSaveReport(m_reportDocument);
    QDomCDATASection bodyData = document.createCDATASection(m_reportDocument->toHtml());
    if ( m_designerMode )
        afterLoadReport(m_reportDocument);
    bodyElement.appendChild(bodyData);
    documentElement.appendChild(bodyElement);

    QDomElement footerElement = document.createElement(extension + "-footer");
    QDomCDATASection footerData = document.createCDATASection(m_footerHtml);
    footerElement.appendChild(footerData);
    documentElement.appendChild(footerElement);

    const int IndentSize = 4;
    QTextStream out(&file);
    document.save(out, IndentSize);

    file.close();

    return true;
}

bool ReportData::load( const QString &fileName, QString &errorMessage )
{
    clear();

    QFile file(fileName);
    if ( !file.exists() ) {
        errorMessage = QObject::tr("File '%1' not Exists").arg(fileName);
        return false;
    }

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;
    if ( !domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        errorMessage = QObject::tr("Common Error in xml-file:\nLine %1, Column %2: %3")
                .arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QString extension = m_designerMode ? "rpml" : "rxml";

    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != extension + "-document" ) {
        errorMessage = QObject::tr("Expected Tag <%1-document> at line %2").arg(extension).arg(root.lineNumber());
        return false;
    }
    if ( !root.hasAttribute("version") ) {
        errorMessage = QObject::tr("Expected Attribute 'version' for Tag <%1-document> at line %2").arg(extension).arg(root.lineNumber());
        return false;
    }
    if ( root.attribute("version") != "1.0" ) {
        errorMessage = QObject::tr("Unsupported version (%1) of Document specified at line").arg(root.lineNumber());
        return false;
    }
    m_pageFormat->load(&root);
    m_defaultFont.setFamily(root.attribute("font-family", "Courier"));
    m_defaultFont.setPointSize(root.attribute("font-size", "9").toInt());
    m_defaultFont.setBold(root.attribute("font-bold", "0").toInt());
    m_defaultFont.setItalic(root.attribute("font-italic", "0").toInt());
    m_reportDocument->setDefaultFont(m_defaultFont);

    if ( m_designerMode ) {
        QDomElement parametersElement = root.firstChildElement("InputParameters");
        if ( parametersElement.isNull() ) {
            errorMessage = QObject::tr("Invalid Format: Expected Tag <InputParameters>");
            return false;
        }
        if ( !loadInputParameters(&parametersElement, errorMessage) )
            return false;

        QDomElement dataSourcesElement = root.firstChildElement("DataSources");
        if ( dataSourcesElement.isNull() ) {
            errorMessage = QObject::tr("Invalid Format: Expected Tag <DataSources>");
            return false;
        }
        if ( !loadDataSources(&dataSourcesElement, errorMessage) )
            return false;

        QDomElement dataTablesElement = root.firstChildElement("DataTables");
        if ( dataTablesElement.isNull() ) {
            errorMessage = QObject::tr("Invalid Format: Expected Tag <DataTables>");
            return false;
        }
        if ( !m_dataTables.load(&dataTablesElement, errorMessage) )
            return false;
    }

    QDomElement headerElement = root.firstChildElement(extension + "-header");
    if ( headerElement.isNull() ) {
        errorMessage = QObject::tr("Invalid Format: Expected Tag <%1-header>").arg(extension);
        return false;
    }
    m_headerHtml = headerElement.text().replace("\r\n", "\n");

    QDomElement bodyElement = root.firstChildElement(extension + "-body");
    if ( bodyElement.isNull() ) {
        errorMessage = QObject::tr("Invalid Format: Expected Tag <%1-body>").arg(extension);
        return false;
    }
    m_reportDocument->setHtml(bodyElement.text());
    if ( m_designerMode )
        afterLoadReport(m_reportDocument);

    QDomElement footerElement = root.firstChildElement(extension + "-footer");
    if ( footerElement.isNull() ) {
        errorMessage = QObject::tr("Invalid Format: Expected Tag <%1-footer>").arg(extension);
        return false;
    }
    m_footerHtml = footerElement.text().replace("\r\n", "\n");

    return true;
}

QTextDocument *ReportData::cloneDocument( QObject *parent )
{
    bool oldModified = m_reportDocument->isModified();

    beforeSaveReport(m_reportDocument);
    QTextDocument *result = m_reportDocument->clone(parent);
    afterLoadReport(m_reportDocument);

    m_reportDocument->setModified(oldModified);

    return result;
}

int ReportData::newDataSourceId() const
{
    int maxId = 0;
    foreach ( DataSource *ds, m_dataSources )
        if ( ds->m_id > maxId )
            maxId = ds->m_id;

    return (maxId + 1);
}

void ReportData::clear( bool onDestroy )
{

    qDeleteAll(m_inputParameters);
    qDeleteAll(m_dataSources);
    if ( onDestroy )
        return;

    m_pageFormat->update();
    m_inputParameters.clear();
    m_dataSources.clear();
    m_dataTables.clear();

    m_headerHtml = initialSampleHeader;
    m_reportDocument->clear();
    m_footerHtml = initialSampleFooter;
}

void ReportData::saveInputParameters( QDomDocument *document, QDomElement *parentElement )
{
    foreach ( ReportInputParameter *par, m_inputParameters ) {
        QDomElement inputParameterElement = document->createElement("InputParameter");
        inputParameterElement.setAttribute("id", par->id);
        inputParameterElement.setAttribute("description", par->description);
        inputParameterElement.setAttribute("type", par->type);
        inputParameterElement.setAttribute("mandatory", par->mandatory);
        inputParameterElement.setAttribute("default", par->defaultValue.toString());
        parentElement->appendChild(inputParameterElement);
    }
}

void ReportData::saveDataSources( QDomDocument *document, QDomElement *parentElement )
{
    foreach ( DataSource *source, m_dataSources ) {
        QDomElement dataSourceElement = document->createElement("DataSource");
        dataSourceElement.setAttribute("id", source->m_id);
        dataSourceElement.setAttribute("typeId", source->m_provider->typeId());
        dataSourceElement.setAttribute("description", source->m_description);
        QDomElement connParametersElement = document->createElement("ConnectParameters");
        for (int i = 0; i < source->m_connectParameters.size(); i++) {
            QDomElement connParameterElement = document->createElement("ConnectParameter");
            connParameterElement.setAttribute("name", source->m_connectParameters[i].name);
            connParameterElement.setAttribute("value", source->m_connectParameters[i].value);
            connParametersElement.appendChild(connParameterElement);
        }
        dataSourceElement.appendChild(connParametersElement);
        if ( source->m_query )
            source->m_query->save(document, &dataSourceElement);
        parentElement->appendChild(dataSourceElement);
    }
}

bool ReportData::loadInputParameters( QDomElement *parentElement, QString &errorMessage )
{
    QDomElement inputParameterElement = parentElement->firstChildElement();
    while ( !inputParameterElement.isNull() ) {
        if ( inputParameterElement.tagName() != "InputParameter" ) {
            errorMessage = QObject::tr("Unexpected Tag '%1' at line %2")
                    .arg(inputParameterElement.tagName())
                    .arg(inputParameterElement.lineNumber());
            return false;
        }
        m_inputParameters << new ReportInputParameter( inputParameterElement.attribute("id").toInt(),
                                                       inputParameterElement.attribute("description"),
                                                       inputParameterElement.attribute("type").toInt(),
                                                       inputParameterElement.attribute("mandatory").toInt(),
                                                       inputParameterElement.attribute("default") );
        inputParameterElement = inputParameterElement.nextSiblingElement();
    }

    return true;
}

bool ReportData::loadDataSources( QDomElement *parentElement, QString &errorMessage )
{
    QDomElement dataSourceElement = parentElement->firstChildElement();
    while ( !dataSourceElement.isNull() ) {
        if ( dataSourceElement.tagName() != "DataSource" ) {
            errorMessage = QObject::tr("Unexpected Tag '%1' at line %2")
                    .arg(dataSourceElement.tagName())
                    .arg(dataSourceElement.lineNumber());
            return false;
        }
        if ( IDataSourceProvider *provider = DataSourceManager::providerForTypeId(dataSourceElement.attribute("typeId").toInt()) ) {
            QList<DataSource::ConnectParameter> connectParameters;
            QDomElement connParametersElement = dataSourceElement.firstChildElement("ConnectParameters");
            if ( !connParametersElement.isNull() ) {
                QDomElement connParameterElement = connParametersElement.firstChildElement("ConnectParameter");
                while ( !connParameterElement.isNull() ) {
                    connectParameters.push_back( DataSource::ConnectParameter( connParameterElement.attribute("name"),
                                                                               connParameterElement.attribute("value") ) );
                    connParameterElement = connParameterElement.nextSiblingElement("ConnectParameter");
                }
            }
            DataSource *ds = new DataSource( dataSourceElement.attribute("id").toInt(),
                                             dataSourceElement.attribute("description"),
                                             provider, connectParameters );
            if ( ds->m_query )
                ds->m_query->load(&dataSourceElement);
            m_dataSources.push_back(ds);
        }

        dataSourceElement = dataSourceElement.nextSiblingElement();
    }

    return true;
}

/*
void ReportData::saveHtmlContent( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument )
{
    m_defaultCharFormat.setFont(textDocument->defaultFont());
    m_defaultCharFormat.clearProperty(QTextFormat::FontUnderline);
    m_defaultCharFormat.clearProperty(QTextFormat::FontOverline);
    m_defaultCharFormat.clearProperty(QTextFormat::FontStrikeOut);
    m_defaultCharFormat.clearProperty(QTextFormat::TextUnderlineStyle);

    QDomElement bodyElement = document->createElement("body");

    QString styleText("");
    emitFontFamily(m_defaultCharFormat.fontFamily(), styleText);
    if (m_defaultCharFormat.hasProperty(QTextFormat::FontPointSize)) {
        styleText += QLatin1String(" font-size:");
        styleText += QString::number(m_defaultCharFormat.fontPointSize());
        styleText += QLatin1String("pt;");
    } else if (m_defaultCharFormat.hasProperty(QTextFormat::FontPixelSize)) {
        styleText += QLatin1String(" font-size:");
        styleText += QString::number(m_defaultCharFormat.intProperty(QTextFormat::FontPixelSize));
        styleText += QLatin1String("px;");
    }

    styleText += QLatin1String(" font-weight:");
    styleText += QString::number(m_defaultCharFormat.fontWeight() * 8);
    styleText += QLatin1Char(';');

    styleText += QLatin1String(" font-style:");
    styleText += (m_defaultCharFormat.fontItalic() ? QLatin1String("italic") : QLatin1String("normal"));
    styleText += QLatin1Char(';');
    bodyElement.setAttribute("style", styleText);

    emitFrame(document, &bodyElement, textDocument, textDocument->rootFrame()->begin());

    parentElement->appendChild(bodyElement);
}

void ReportData::emitFontFamily(const QString &family, QString &styleText )
{
    styleText += QLatin1String(" font-family:");

    QLatin1String quote("\'");
    if (family.contains(QLatin1Char('\'')))
        quote = QLatin1String("&quot;");

    styleText += quote;
#ifndef HAVE_QT5
    styleText += Qt::escape(family);
#else
    styleText += family.toHtmlEscaped();
#endif
    styleText += quote;
    styleText += QLatin1Char(';');
}

void ReportData::emitFrame( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument, QTextFrame::Iterator frameIt )
{
    if (!frameIt.atEnd()) {
        QTextFrame::Iterator next = frameIt;
        ++next;
        if (next.atEnd()
            && frameIt.currentFrame() == 0
            && frameIt.parentFrame() != textDocument->rootFrame()
            && frameIt.currentBlock().begin().atEnd())
            return;
    }

    for (QTextFrame::Iterator it = frameIt;
         !it.atEnd(); ++it) {
        if (QTextFrame *frame = it.currentFrame()) {
            if (QTextTable *table = qobject_cast<QTextTable *>(frame)) {
                emitTable(document, parentElement, textDocument, table);
            } else {
                emitTextFrame(document, parentElement, textDocument, frame);
            }
        } else if (it.currentBlock().isValid()) {
            emitBlock(document, parentElement, it.currentBlock());
        }
    }
}

void ReportData::emitTable( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument, QTextTable *table )
{
    QTextTableFormat format = table->format();

    QDomElement tableElement = document->createElement("table");
    if ( format.hasProperty(RichEditor::DataTableId) )
        tableElement.setAttribute("DataTableId", format.intProperty(RichEditor::DataTableId));

    if ( format.hasProperty(QTextFormat::FrameBorder) )
        tableElement.setAttribute("border", QString::number(format.border()));

    emitFrameStyle(&tableElement, format);
    emitAlignment(&tableElement, format.alignment());

    if ( format.hasProperty(QTextFormat::TableCellSpacing) )
        tableElement.setAttribute("cellspacing", QString::number(format.cellSpacing()));
    if ( format.hasProperty(QTextFormat::TableCellPadding) )
        tableElement.setAttribute("cellpadding", QString::number(format.cellPadding()));

    const int rows = table->rows();
    const int columns = table->columns();

    QVector<QTextLength> columnWidths = format.columnWidthConstraints();
    if (columnWidths.isEmpty()) {
        columnWidths.resize(columns);
        columnWidths.fill(QTextLength());
    }
    Q_ASSERT(columnWidths.count() == columns);

    QVarLengthArray<bool> widthEmittedForColumn(columns);
    for (int i = 0; i < columns; ++i)
        widthEmittedForColumn[i] = false;

    const int headerRowCount = qMin(format.headerRowCount(), rows);
    QDomElement theadElement;

    if ( headerRowCount > 0 ) {
        theadElement = document->createElement("thead");
        tableElement.appendChild(theadElement);
    }

    for (int row = 0; row < rows; row++) {
        QDomElement rowElement = document->createElement("tr");

        for (int col = 0; col < columns; col++) {
            const QTextTableCell cell = table->cellAt(row, col);
            if ( cell.row() != row || cell.column() != col )
                continue;

            QDomElement cellElement = document->createElement("td");

            if (!widthEmittedForColumn[col] && cell.columnSpan() == 1) {
                emitTextLength(&cellElement, "width", columnWidths.at(col));
                widthEmittedForColumn[col] = true;
            }

            if ( cell.columnSpan() > 1 )
                cellElement.setAttribute("colspan", QString::number(cell.columnSpan()));

            if ( cell.rowSpan() > 1 )
                cellElement.setAttribute("rowspan", QString::number(cell.rowSpan()));

            QTextCharFormat oldDefaultCharFormat = m_defaultCharFormat;

            const QTextTableCellFormat cellFormat = cell.format().toTableCellFormat();
            QTextCharFormat::VerticalAlignment valign = cellFormat.verticalAlignment();

            QString styleText("");

            if (valign >= QTextCharFormat::AlignMiddle && valign <= QTextCharFormat::AlignBottom) {
                styleText += QLatin1String(" vertical-align:");
                switch (valign) {
                case QTextCharFormat::AlignMiddle:
                    styleText += QLatin1String("middle");
                    break;
                case QTextCharFormat::AlignTop:
                    styleText += QLatin1String("top");
                    break;
                case QTextCharFormat::AlignBottom:
                    styleText += QLatin1String("bottom");
                    break;
                default:
                    break;
                }
                styleText += QLatin1Char(';');

                QTextCharFormat temp;
                temp.setVerticalAlignment(valign);
                m_defaultCharFormat.merge(temp);
            }

            if (cellFormat.hasProperty(QTextFormat::TableCellLeftPadding))
                styleText += QLatin1String(" padding-left:") + QString::number(cellFormat.leftPadding()) + QLatin1Char(';');
            if (cellFormat.hasProperty(QTextFormat::TableCellRightPadding))
                styleText += QLatin1String(" padding-right:") + QString::number(cellFormat.rightPadding()) + QLatin1Char(';');
            if (cellFormat.hasProperty(QTextFormat::TableCellTopPadding))
                styleText += QLatin1String(" padding-top:") + QString::number(cellFormat.topPadding()) + QLatin1Char(';');
            if (cellFormat.hasProperty(QTextFormat::TableCellBottomPadding))
                styleText += QLatin1String(" padding-bottom:") + QString::number(cellFormat.bottomPadding()) + QLatin1Char(';');

            cellElement.setAttribute("style", styleText);

            emitFrame(document, &cellElement, textDocument, cell.begin());

            rowElement.appendChild(cellElement);

            m_defaultCharFormat = oldDefaultCharFormat;
        }

        if ( row < headerRowCount )
            theadElement.appendChild(rowElement);
        else
            tableElement.appendChild(rowElement);
    }

    parentElement->appendChild(tableElement);
}

void ReportData::emitTextFrame( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument, QTextFrame *frame )
{
    QTextFrameFormat format = frame->frameFormat();

    QDomElement frameElement = document->createElement("frame");

    if ( format.hasProperty(QTextFormat::FrameBorder) ) {
        frameElement.setAttribute("border", QString::number(format.border()));
    }

    emitFrameStyle(&frameElement, format);

    QDomElement trFrameElement = document->createElement("tr");
    QDomElement tdFrameElement = document->createElement("td");
    tdFrameElement.setAttribute("style", "border: none;");
    emitFrame(document, &tdFrameElement, textDocument, frame->begin());
    trFrameElement.appendChild(tdFrameElement);
    frameElement.appendChild(trFrameElement);

    parentElement->appendChild(frameElement);
}

void ReportData::emitBlock( QDomDocument *document, QDomElement *parentElement, const QTextBlock &block )
{
    QTextCharFormat oldDefaultCharFormat = m_defaultCharFormat;

    QDomElement blockElement = document->createElement("p");
    emitBlockAttributes(&blockElement, block);
    for ( QTextBlock::Iterator it = block.begin();
          !it.atEnd(); ++it )
        emitFragment(document, &blockElement, it.fragment());

//    if (block.begin().atEnd()) {
//        QDomElement endBlockElement = document->createElement("br");
//        parentElement->appendChild(endBlockElement);
//    }

    parentElement->appendChild(blockElement);

    m_defaultCharFormat = oldDefaultCharFormat;
}

void ReportData::emitFragment( QDomDocument *document, QDomElement *parentElement, const QTextFragment &fragment )
{
    const QTextCharFormat format = fragment.charFormat();
    const bool isObject = fragment.text().contains(QChar::ObjectReplacementCharacter);

    if ( isObject ) {
        int reportObjectType = format.property(ReportObjectType).toInt();
        switch ( reportObjectType ) {
            case ReportField: {
                int queryId = format.property(FieldReportQueryId).toInt();
                int columnNumber = format.property(FieldReportQueryColumnNumber).toInt();
                QDomElement fieldElement = document->createElement("ReportField");
                fieldElement.setAttribute("DataSourceId", queryId);
                fieldElement.setAttribute("ColumnNumber", columnNumber);
                parentElement->appendChild(fieldElement);
                break;
            }
            case ReportParameter: {
                int parameterId = format.property(ParameterReportId).toInt();
                QDomElement paramElement = document->createElement("ReportParameter");
                paramElement.setAttribute("ParameterId", parameterId);
                parentElement->appendChild(paramElement);
                break;
            }
        }
    }

    else {
        QDomElement fragmentElement = document->createElement("span");
#ifndef HAVE_QT5
        fragmentElement.setNodeValue(Qt::escape(fragment.text()));
#else
        fragmentElement.setNodeValue(fragment.text().toHtmlEscaped());
#endif
    }
}

void ReportData::emitBlockAttributes( QDomElement *blockElement, const QTextBlock &block )
{
    QTextBlockFormat format = block.blockFormat();

    emitAlignment(blockElement, format.alignment());

    if ( block.textDirection() == Qt::RightToLeft )
        blockElement->setAttribute("dir", "rtl");

    QString styleText("");

    emitMargins( QString::number(format.topMargin()),
                 QString::number(format.bottomMargin()),
                 QString::number(format.leftMargin()),
                 QString::number(format.rightMargin()),
                 styleText);

    styleText += QLatin1String(" -qt-block-indent:");
    styleText += QString::number(format.indent());
    styleText += QLatin1Char(';');

    styleText += QLatin1String(" text-indent:");
    styleText += QString::number(format.textIndent());
    styleText += QLatin1String("px;");

    emitPageBreakPolicy(format.pageBreakPolicy(), styleText);

    const QTextCharFormat blockCharFmt = block.charFormat();
    QTextCharFormat diff = formatDifference(m_defaultCharFormat, blockCharFmt).toCharFormat();
    diff.clearProperty(QTextFormat::BackgroundBrush);
    if ( format.hasProperty(QTextFormat::BackgroundBrush) ) {
        QBrush bg = format.background();
        if (bg.style() != Qt::NoBrush)
            diff.setProperty(QTextFormat::BackgroundBrush, format.property(QTextFormat::BackgroundBrush));
    }
    if ( !diff.properties().isEmpty() )
        emitCharFormatStyle(diff, styleText);

    blockElement->setAttribute("style", styleText);
}

void ReportData::emitAlignment( QDomElement *blockElement, Qt::Alignment align )
{
    if ( align & Qt::AlignRight )
        blockElement->setAttribute("align", "right");
    else if ( align & Qt::AlignHCenter )
        blockElement->setAttribute("align", "center");
    else if (align & Qt::AlignJustify)
        blockElement->setAttribute("align", "justify");
}

void ReportData::emitMargins( const QString &top, const QString &bottom, const QString &left, const QString &right, QString &styleText )
{
    styleText += QLatin1String(" margin-top:");
    styleText += top;
    styleText += QLatin1String("px;");

    styleText += QLatin1String(" margin-bottom:");
    styleText += bottom;
    styleText += QLatin1String("px;");

    styleText += QLatin1String(" margin-left:");
    styleText += left;
    styleText += QLatin1String("px;");

    styleText += QLatin1String(" margin-right:");
    styleText += right;
    styleText += QLatin1String("px;");
}

void ReportData::emitPageBreakPolicy( QTextFormat::PageBreakFlags policy, QString &styleText )
{
    if ( policy & QTextFormat::PageBreak_AlwaysBefore )
        styleText += QLatin1String(" page-break-before:always;");

    if ( policy & QTextFormat::PageBreak_AlwaysAfter )
        styleText += QLatin1String(" page-break-after:always;");
}

bool ReportData::emitCharFormatStyle( const QTextCharFormat &format, QString &styleText )
{
    bool attributesEmitted = false;

    {
        const QString family = format.fontFamily();
        if (!family.isEmpty() && family != m_defaultCharFormat.fontFamily()) {
            emitFontFamily(family, styleText);
            attributesEmitted = true;
        }
    }

    if (format.hasProperty(QTextFormat::FontPointSize)
        && format.fontPointSize() != m_defaultCharFormat.fontPointSize()) {
        styleText += QLatin1String(" font-size:");
        styleText += QString::number(format.fontPointSize());
        styleText += QLatin1String("pt;");
        attributesEmitted = true;
    } else if (format.hasProperty(QTextFormat::FontSizeAdjustment)) {
        static const char * const sizeNames[] = {
            "small", "medium", "large", "x-large", "xx-large"
        };
        const char *name = 0;
        const int idx = format.intProperty(QTextFormat::FontSizeAdjustment) + 1;
        if (idx >= 0 && idx <= 4) {
            name = sizeNames[idx];
        }
        if (name) {
            styleText += QLatin1String(" font-size:");
            styleText += QLatin1String(name);
            styleText += QLatin1Char(';');
            attributesEmitted = true;
        }
    } else if (format.hasProperty(QTextFormat::FontPixelSize)) {
        styleText += QLatin1String(" font-size:");
        styleText += QString::number(format.intProperty(QTextFormat::FontPixelSize));
        styleText += QLatin1String("px;");
    }

    if (format.hasProperty(QTextFormat::FontWeight)
        && format.fontWeight() != m_defaultCharFormat.fontWeight()) {
        styleText += QLatin1String(" font-weight:");
        styleText += QString::number(format.fontWeight() * 8);
        styleText += QLatin1Char(';');
        attributesEmitted = true;
    }

    if (format.hasProperty(QTextFormat::FontItalic)
        && format.fontItalic() != m_defaultCharFormat.fontItalic()) {
        styleText += QLatin1String(" font-style:");
        styleText += (format.fontItalic() ? QLatin1String("italic") : QLatin1String("normal"));
        styleText += QLatin1Char(';');
        attributesEmitted = true;
    }

    QLatin1String decorationTag(" text-decoration:");
    styleText += decorationTag;
    bool hasDecoration = false;
    bool atLeastOneDecorationSet = false;

    if ((format.hasProperty(QTextFormat::FontUnderline) || format.hasProperty(QTextFormat::TextUnderlineStyle))
        && format.fontUnderline() != m_defaultCharFormat.fontUnderline()) {
        hasDecoration = true;
        if (format.fontUnderline()) {
            styleText += QLatin1String(" underline");
            atLeastOneDecorationSet = true;
        }
    }

    if (format.hasProperty(QTextFormat::FontOverline)
        && format.fontOverline() != m_defaultCharFormat.fontOverline()) {
        hasDecoration = true;
        if (format.fontOverline()) {
            styleText += QLatin1String(" overline");
            atLeastOneDecorationSet = true;
        }
    }

    if (format.hasProperty(QTextFormat::FontStrikeOut)
        && format.fontStrikeOut() != m_defaultCharFormat.fontStrikeOut()) {
        hasDecoration = true;
        if (format.fontStrikeOut()) {
            styleText += QLatin1String(" line-through");
            atLeastOneDecorationSet = true;
        }
    }

    if (hasDecoration) {
        if (!atLeastOneDecorationSet)
            styleText += QLatin1String("none");
        styleText += QLatin1Char(';');
        attributesEmitted = true;
    } else {
        styleText.chop(qstrlen(decorationTag.latin1()));
    }

    if (format.foreground() != m_defaultCharFormat.foreground()
        && format.foreground().style() != Qt::NoBrush) {
        styleText += QLatin1String(" color:");
        styleText += format.foreground().color().name();
        styleText += QLatin1Char(';');
        attributesEmitted = true;
    }

    if (format.background() != m_defaultCharFormat.background()
        && format.background().style() == Qt::SolidPattern) {
        styleText += QLatin1String(" background-color:");
        styleText += format.background().color().name();
        styleText += QLatin1Char(';');
        attributesEmitted = true;
    }

    if (format.verticalAlignment() != m_defaultCharFormat.verticalAlignment()
        && format.verticalAlignment() != QTextCharFormat::AlignNormal)
    {
        styleText += QLatin1String(" vertical-align:");

        QTextCharFormat::VerticalAlignment valign = format.verticalAlignment();
        if (valign == QTextCharFormat::AlignSubScript)
            styleText += QLatin1String("sub");
        else if (valign == QTextCharFormat::AlignSuperScript)
            styleText += QLatin1String("super");
        else if (valign == QTextCharFormat::AlignMiddle)
            styleText += QLatin1String("middle");
        else if (valign == QTextCharFormat::AlignTop)
            styleText += QLatin1String("top");
        else if (valign == QTextCharFormat::AlignBottom)
            styleText += QLatin1String("bottom");

        styleText += QLatin1Char(';');
        attributesEmitted = true;
    }

    if (format.fontCapitalization() != QFont::MixedCase) {
        const QFont::Capitalization caps = format.fontCapitalization();
        if (caps == QFont::AllUppercase)
            styleText += QLatin1String(" text-transform:uppercase;");
        else if (caps == QFont::AllLowercase)
            styleText += QLatin1String(" text-transform:lowercase;");
        else if (caps == QFont::SmallCaps)
            styleText += QLatin1String(" font-variant:small-caps;");
        attributesEmitted = true;
    }

    if (format.fontWordSpacing() != 0.0) {
        styleText += QLatin1String(" word-spacing:");
        styleText += QString::number(format.fontWordSpacing());
        styleText += QLatin1String("px;");
        attributesEmitted = true;
    }

    return attributesEmitted;
}

void ReportData::emitFrameStyle( QDomElement *frameElement, const QTextFrameFormat &format )
{
    QString styleText("");
    const QTextFrameFormat defaultFormat;

    emitPageBreakPolicy(format.pageBreakPolicy(), styleText);

    {   // Borders widths
#ifndef HAVE_QT5
        styleText += QLatin1String(" border-left-width:");
        styleText += QString::number(format.borderLeft());
        styleText += QLatin1Char(';');
        styleText += QLatin1String(" border-top-width:");
        styleText += QString::number(format.borderTop());
        styleText += QLatin1Char(';');
        styleText += QLatin1String(" border-right-width:");
        styleText += QString::number(format.borderRight());
        styleText += QLatin1Char(';');
        styleText += QLatin1String(" border-bottom-width:");
        styleText += QString::number(format.borderBottom());
        styleText += QLatin1Char(';');
#endif
    }

    {   // Borders Brushes
#ifndef HAVE_QT5
        styleText += QLatin1String(" border-left-color:");
        styleText += format.borderBrushLeft().color().name();
        styleText += QLatin1Char(';');
        styleText += QLatin1String(" border-top-color:");
        styleText += format.borderBrushTop().color().name();
        styleText += QLatin1Char(';');
        styleText += QLatin1String(" border-right-color:");
        styleText += format.borderBrushRight().color().name();
        styleText += QLatin1Char(';');
        styleText += QLatin1String(" border-bottom-color:");
        styleText += format.borderBrushBottom().color().name();
        styleText += QLatin1Char(';');
#else
        if ( format.borderBrush() != defaultFormat.borderBrush() ) {
            styleText += QLatin1String(" border-color:");
            styleText += format.borderBrush().color().name();
            styleText += QLatin1Char(';');
        }
#endif
    }


    {   // Borders Styles
#ifndef HAVE_QT5
        emitBorderStyle(format.borderStyleLeft(),   QLatin1String(" border-left-style:"),   styleText);
        emitBorderStyle(format.borderStyleTop(),    QLatin1String(" border-top-style:"),    styleText);
        emitBorderStyle(format.borderStyleRight(),  QLatin1String(" border-right-style:"),  styleText);
        emitBorderStyle(format.borderStyleBottom(), QLatin1String(" border-bottom-style:"), styleText);
#else
        if ( format.borderStyle() != defaultFormat.borderStyle() )
            emitBorderStyle(format.borderStyle(), QLatin1String(" border-style:"),   styleText);
#endif
    }


    if ( format.hasProperty(QTextFormat::FrameMargin) ||
         format.hasProperty(QTextFormat::FrameLeftMargin) ||
         format.hasProperty(QTextFormat::FrameRightMargin) ||
         format.hasProperty(QTextFormat::FrameTopMargin) ||
         format.hasProperty(QTextFormat::FrameBottomMargin) )
        emitMargins( QString::number(format.topMargin()),
                     QString::number(format.bottomMargin()),
                     QString::number(format.leftMargin()),
                     QString::number(format.rightMargin()),
                     styleText );

    frameElement->setAttribute("style", styleText);
}

void ReportData::emitBorderStyle( QTextFrameFormat::BorderStyle style, const QLatin1String &borderStylePrefix, QString &styleText )
{
    Q_ASSERT(style <= QTextFrameFormat::BorderStyle_Outset);

    styleText += borderStylePrefix;

    switch (style) {
        case QTextFrameFormat::BorderStyle_None:
            styleText += QLatin1String("none");
            break;
        case QTextFrameFormat::BorderStyle_Dotted:
            styleText += QLatin1String("dotted");
            break;
        case QTextFrameFormat::BorderStyle_Dashed:
            styleText += QLatin1String("dashed");
            break;
        case QTextFrameFormat::BorderStyle_Solid:
            styleText += QLatin1String("solid");
            break;
        case QTextFrameFormat::BorderStyle_Double:
            styleText += QLatin1String("double");
            break;
        case QTextFrameFormat::BorderStyle_DotDash:
            styleText += QLatin1String("dot-dash");
            break;
        case QTextFrameFormat::BorderStyle_DotDotDash:
            styleText += QLatin1String("dot-dot-dash");
            break;
        case QTextFrameFormat::BorderStyle_Groove:
            styleText += QLatin1String("groove");
            break;
        case QTextFrameFormat::BorderStyle_Ridge:
            styleText += QLatin1String("ridge");
            break;
        case QTextFrameFormat::BorderStyle_Inset:
            styleText += QLatin1String("inset");
            break;
        case QTextFrameFormat::BorderStyle_Outset:
            styleText += QLatin1String("outset");
            break;
        default:
            Q_ASSERT(false);
            break;
    };

    styleText += QLatin1Char(';');
}

void ReportData::emitTextLength( QDomElement *element, const char *attribute, const QTextLength &length )
{
    if (length.type() == QTextLength::VariableLength) // default
        return;

    if (length.type() == QTextLength::PercentageLength)
        element->setAttribute(attribute, QString::number(length.rawValue()) + "%");
    else
        element->setAttribute(attribute, QString::number(length.rawValue()));
}

static bool tokenizeStyleAttribute( const QString &styleText, QList<QPair<QString, QString> > &options )
{
    QStringList pairs = styleText.split(";");
    if ( pairs.isEmpty() )
        return false;

    foreach ( const QString &pair, pairs ) {
        QStringList option = pair.split(":");
        if ( option.size() != 2 )
            return false;
        options.push_back(qMakePair(option[0].trimmed(), option[1].trimmed()));
    }

    return true;
}

bool ReportData::loadHtmlContent( QDomElement *parentElement, QString &errorMessage, QTextDocument *textDocument )
{
    QDomElement bodyElement = parentElement->firstChildElement();
    if ( bodyElement.isNull() ) {
        errorMessage = QObject::tr("Expected Child Element with Tag <body> of Element with Tag <%1> at line %2")
                .arg(parentElement->tagName()).arg(parentElement->lineNumber());
        return false;
    }
    if ( bodyElement.tagName() != "body" ) {
        errorMessage = QObject::tr("Unexpected Tag <%1> at line %2. Waiting <body>")
                .arg(bodyElement.tagName()).arg(bodyElement.lineNumber());
        return false;
    }
    QString styleText = bodyElement.attribute("style", "");
    styleText = styleText.trimmed();
    QString fontFamily = "Courier";
    int fontSize = 10;
    bool isFontSizePt = true;
    int fontWeight = 24;
    QString fontStyle = "normal";
    if ( !styleText.isEmpty() ) {
        QList<QPair<QString, QString> > options;
        if ( !tokenizeStyleAttribute(styleText, options) ) {
            errorMessage = QObject::tr("Error on Recognizing Style-attribute for Tag <body> at line %1")
                    .arg(bodyElement.lineNumber());
            return false;
        }
        bool styleOk = true;
        for (int i = 0; i < options.size() && styleOk; i++) {
            if ( options[i].first == "font-family" ) {
                fontFamily = options[i].second;
            } else if ( options[i].first == "font-size" ) {
                QString strFontSize = options[i].second;
                if ( strFontSize.endsWith("px") ) {
                    strFontSize.chop(2);
                    isFontSizePt = false;
                } else if ( strFontSize.endsWith("pt") ) {
                    strFontSize.chop(2);
                }
                bool okInt;
                fontSize = strFontSize.toInt(&okInt);
                if ( !okInt )
                    styleOk = false;
            } else if ( options[i].first == "font-weight" ) {
                bool okInt;
                fontWeight = options[i].second.toInt(&okInt);
                if ( !okInt )
                    styleOk = false;
            } else if ( options[i].first == "font-style" ) {
                fontStyle = options[i].second;
                if ( fontStyle != "normal" && fontStyle != "italic" )
                    styleOk = false;
            } else {
                styleOk = false;
            }
        }
        if ( !styleOk ) {
            errorMessage = QObject::tr("Error on Recognizing Style-attribute for Tag <body> at line %1")
                    .arg(bodyElement.lineNumber());
            return false;
        }
    }

    m_defaultCharFormat = QTextCharFormat();
    m_defaultCharFormat.setFontFamily(fontFamily);
    if ( isFontSizePt )
        m_defaultCharFormat.setFontPointSize(fontSize);
    else
        m_defaultCharFormat.setProperty(QTextFormat::FontPixelSize, fontSize);
    m_defaultCharFormat.setFontWeight(fontWeight / 8);
    m_defaultCharFormat.setFontItalic(fontStyle == "italic");

    QDomElement bodyItemElement = bodyElement.firstChildElement();
    while ( !bodyItemElement.isNull() ) {
        QString tagName = bodyItemElement.tagName();
        if ( tagName == "p" ) {
            loadBlock(&bodyItemElement, errorMessage, textDocument);
        } else if ( tagName == "table" ) {
            loadTable(&bodyItemElement, errorMessage, textDocument);
        } else if ( tagName == "frame" ) {
            loadFrame(&bodyItemElement, errorMessage, textDocument);
        } else {
            errorMessage = QObject::tr("Unexpected Tag <%1> at line %2. Waiting <p>|<table>|<frame>")
                    .arg(tagName).arg(bodyItemElement.lineNumber());
            return false;
        }
        bodyItemElement = bodyItemElement.nextSiblingElement();
    }

    return true;
}

bool ReportData::loadBlock( QDomElement *blockElement, QString &errorMessage, QTextDocument *textDocument )
{
    /// TODO:
    Q_UNUSED(blockElement)
    Q_UNUSED(errorMessage)
    Q_UNUSED(textDocument)

    return true;
}

bool ReportData::loadTable( QDomElement *tableElement, QString &errorMessage, QTextDocument *textDocument )
{
    /// TODO:
    Q_UNUSED(tableElement)
    Q_UNUSED(errorMessage)
    Q_UNUSED(textDocument)

    return true;
}

bool ReportData::loadFrame( QDomElement *frameElement, QString &errorMessage, QTextDocument *textDocument )
{
    /// TODO:
    Q_UNUSED(frameElement)
    Q_UNUSED(errorMessage)
    Q_UNUSED(textDocument)

    return true;
}
*/
}   // namespace RPML
