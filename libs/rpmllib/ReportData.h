#ifndef REPORTDATA_H
#define REPORTDATA_H

#include <QTextDocument>
#include <QList>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <QTextFrame>
#include <QTextTable>
#include <QTextObjectInterface>
#include <QPainter>
#include <QTextEdit>
#include <QFont>

#include "rpmllib_global.h"
#include "PageFormat.h"
#include "ReportInputParameters.h"
#include "DataSource.h"

namespace RPML {

class ReportDataTableList;
class BaseReportWidget;
class ReportDesignerWidget;
class TextPrinter;
class ReportPreferencesDialog;
class OdfWriter;

struct DataTableGroupingInfo
{
    DataTableGroupingInfo( int columnNumber_ = 1, int rowsCount_ = 1 )
        : columnNumber(columnNumber_)
        , rowsCount(rowsCount_)
    {}

    int columnNumber;
    int rowsCount;
};

class ReportDataTable
{
public:
    friend class ReportDataTableList;

    ReportDataTable( int id,
                     int sourceId,
                     int headerRowsCount = 1,
                     int detailRowsCount = 1,
                     int footerRowsCount = 0 )
        : m_id(id)
        , m_sourceId(sourceId)
        , m_headerRowsCount(headerRowsCount)
        , m_detailRowsCount(detailRowsCount)
        , m_footerRowsCount(footerRowsCount)
    {}

    virtual ~ReportDataTable() {
        qDeleteAll(m_groupings);
    }

    void addGrouping( DataTableGroupingInfo *grouping ) {
        m_groupings.push_back(grouping);
    }

    void setSourceId( int id ) {
        m_sourceId = id;
    }

    void setHeaderRowsCount( int count ) {
        m_headerRowsCount = count;
    }

    void setDetailRowsCount( int count ) {
        m_detailRowsCount = count;
    }

    void setFooterRowsCount( int count ) {
        m_footerRowsCount = count;
    }

    int rowsCount() const {
        int rows = m_headerRowsCount + m_detailRowsCount + m_footerRowsCount;
        foreach ( DataTableGroupingInfo *g, m_groupings )
            rows += g->rowsCount;
        return rows;
    }

    inline int headerRowsCount() const { return m_headerRowsCount; }

public:
    int m_id;
    int m_sourceId;
    int m_headerRowsCount;
    QList<DataTableGroupingInfo *> m_groupings;
    int m_detailRowsCount;
    int m_footerRowsCount;
};

class ReportDataTableList
{
public:
    ReportDataTableList() {}
    virtual ~ReportDataTableList();

    void clear();
    void save( QDomDocument *document, QDomElement *parentElement );
    bool load( QDomElement *parentElement, QString &errorMessage );

    ReportDataTable *operator [] ( int index );
    bool dataSourceIdUsed( int sourceId );
    int newId() const;
    int sourceIdForDataTableId( int id ) const;

    void addDataTable( ReportDataTable *dataTable );
    inline int count() const { return m_data.size(); }

private:
    QList<ReportDataTable *> m_data;
};

class FieldReportObject : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    QSizeF intrinsicSize( QTextDocument *doc, int posInDocument,
                          const QTextFormat &format );
    void drawObject( QPainter *painter, const QRectF &rect, QTextDocument *doc,
                     int posInDocument, const QTextFormat &format );
};

class RPML_EXPORT ReportData : public QObject
{
    Q_OBJECT

    friend class BaseReportWidget;
    friend class ReportDesignerWidget;
    friend class ReportEditorWidget;
    friend class ReportPreferencesDialog;
    friend class TextPrinter;
    friend class OdfWriter;

public:
    enum { FieldReportFormat = QTextFormat::UserObject + 1 };
    enum {
        ReportField = 1,
        ReportParameter
    };
    enum { ReportObjectType = 1 /* Integer */};
    enum {
        FieldReportQueryId = 2,           // String
        FieldReportQueryColumnNumber = 3, // String

        ParameterReportId = 2             // Integer
    };

    ReportData( QTextEdit *editorWidget, bool designerMode = true, QObject *parent = 0 );
    virtual ~ReportData();

    bool save( const QString &fileName , QString &errorMessage );
    bool load( const QString &fileName , QString &errorMessage );
    QTextDocument *cloneDocument( QObject *parent = 0 );

    int newDataSourceId() const;

private:
    void clear( bool onDestroy = false );

    void saveInputParameters( QDomDocument *document, QDomElement *parentElement );
    void saveDataSources( QDomDocument *document, QDomElement *parentElement );

    bool loadInputParameters( QDomElement *parentElement, QString &errorMessage );
    bool loadDataSources( QDomElement *parentElement, QString &errorMessage );
/*
    void saveHtmlContent( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument );
    void emitFontFamily( const QString &family, QString &styleText );
    void emitFrame( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument, QTextFrame::Iterator frameIt );
    void emitTable( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument, QTextTable *table );
    void emitTextFrame( QDomDocument *document, QDomElement *parentElement, QTextDocument *textDocument, QTextFrame *frame );
    void emitBlock( QDomDocument *document, QDomElement *parentElement, const QTextBlock &block );
    void emitFragment( QDomDocument *document, QDomElement *parentElement, const QTextFragment &fragment );
    void emitBlockAttributes( QDomElement *blockElement, const QTextBlock &block);
    void emitAlignment( QDomElement *blockElement, Qt::Alignment align );
    void emitMargins( const QString &top, const QString &bottom, const QString &left, const QString &right, QString &styleText );
    void emitPageBreakPolicy( QTextFormat::PageBreakFlags policy, QString &styleText );
    bool emitCharFormatStyle( const QTextCharFormat &format, QString &styleText );
    void emitFrameStyle( QDomElement *frameElement, const QTextFrameFormat &format );
    void emitBorderStyle( QTextFrameFormat::BorderStyle style, const QLatin1String &borderStylePrefix, QString &styleText );
    void emitTextLength( QDomElement *element, const char *attribute, const QTextLength &length );

    bool loadHtmlContent( QDomElement *parentElement, QString &errorMessage, QTextDocument *textDocument );
    bool loadBlock( QDomElement *blockElement, QString &errorMessage, QTextDocument *textDocument );
    bool loadTable( QDomElement *tableElement, QString &errorMessage, QTextDocument *textDocument );
    bool loadFrame( QDomElement *frameElement, QString &errorMessage, QTextDocument *textDocument );
*/

    Utils::PageFormat *m_pageFormat;

    QList<ReportInputParameter *> m_inputParameters;
    QList<DataSource *> m_dataSources;
    ReportDataTableList m_dataTables;

    QString m_headerHtml;
    QTextDocument *m_reportDocument;
    QString m_footerHtml;

    QTextEdit *m_editorWidget;

    QFont m_defaultFont;

    bool m_designerMode;
};

}   // namespace RPML

#endif // REPORTDATA_H
