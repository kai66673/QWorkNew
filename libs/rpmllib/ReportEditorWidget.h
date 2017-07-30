#ifndef REPORTEDITORWIDGET_H
#define REPORTEDITORWIDGET_H

#include "RichEditorWidget.h"
#include "ReportData.h"
#include "DataSource.h"
#include "QwDockWidget.h"
#include "rpmllib_global.h"

namespace RPML {

#define RULER_BREADTH 20

class QDRuler: public QWidget
{
    Q_OBJECT
    Q_ENUMS(RulerType)
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
    Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)

public:
    enum RulerType { Horizontal, Vertical };

    QDRuler(QDRuler::RulerType rulerType, QWidget* parent = 0)
        : QWidget(parent)
        , mRulerType(rulerType)
        , mOrigin(0.)
        , mRulerUnit(1.)
        , mRulerZoom(1.)
        , mMouseTracking(false)
        , mDrawText(false)
    {
        setMouseTracking(true);
    }

    QSize minimumSizeHint() const
    { return QSize(RULER_BREADTH, RULER_BREADTH); }

    QDRuler::RulerType rulerType() const
    { return mRulerType; }

    qreal origin() const
    { return mOrigin; }

    qreal rulerUnit() const
    { return mRulerUnit; }

    qreal rulerZoom() const
    { return mRulerZoom; }

public slots:

    void setOrigin(const qreal origin) {
        if (mOrigin != origin) {
            mOrigin = origin;
            update();
        }
    }

    void setRulerUnit(const qreal rulerUnit) {
        if (mRulerUnit != rulerUnit) {
            mRulerUnit = rulerUnit;
            update();
        }
    }

    void setRulerZoom(const qreal rulerZoom) {
        if (mRulerZoom != rulerZoom) {
            mRulerZoom = rulerZoom;
            update();
        }
    }

    void setCursorPos(const QPoint cursorPos) {
        mCursorPos = this->mapFromGlobal(cursorPos);
        mCursorPos += QPoint(RULER_BREADTH, RULER_BREADTH);
        update();
    }

    void setMouseTrack(const bool track) {
        if (mMouseTracking != track) {
            mMouseTracking = track;
            update();
        }
    }

protected:
    void mouseMoveEvent(QMouseEvent* event) {
        mCursorPos = event->pos();
        update();
        QWidget::mouseMoveEvent(event);
    }

    void paintEvent(QPaintEvent* event) {
        Q_UNUSED(event)

        QPainter painter(this);
        painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
        QPen pen(Qt::black,0); // zero width pen is cosmetic pen
        //pen.setCosmetic(true);
        painter.setPen(pen);
        // We want to work with floating point, so we are considering
        // the rect as QRectF
        QRectF rulerRect = this->rect();

        // at first fill the rect
        //painter.fillRect(rulerRect,QColor(220,200,180));
        painter.fillRect(rulerRect,QColor(236,233,216));

        // drawing a scale of 25
        drawAScaleMeter(&painter,rulerRect,25,(Horizontal == mRulerType ? rulerRect.height() : rulerRect.width())/2);
        // drawing a scale of 50
        drawAScaleMeter(&painter,rulerRect,50,(Horizontal == mRulerType ? rulerRect.height() : rulerRect.width())/4);
        // drawing a scale of 100
        mDrawText = true;
        drawAScaleMeter(&painter,rulerRect,100,0);
        mDrawText = false;

        // drawing the current mouse position indicator
        painter.setOpacity(0.4);
        drawMousePosTick(&painter);
        painter.setOpacity(1.0);

        // drawing no man's land between the ruler & view
        QPointF starPt = Horizontal == mRulerType ? rulerRect.bottomLeft() : rulerRect.topRight();
        QPointF endPt = Horizontal == mRulerType ? rulerRect.bottomRight() : rulerRect.bottomRight();
        painter.setPen(QPen(Qt::black,2));
        painter.drawLine(starPt,endPt);
    }

private:
    void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin) {
        // Flagging whether we are horizontal or vertical only to reduce
        // to cheching many times
        bool isHorzRuler = Horizontal == mRulerType;

        scaleMeter  = scaleMeter * mRulerUnit * mRulerZoom;

        // Ruler rectangle starting mark
        qreal rulerStartMark = isHorzRuler ? rulerRect.left() : rulerRect.top();
        // Ruler rectangle ending mark
        qreal rulerEndMark = isHorzRuler ? rulerRect.right() : rulerRect.bottom();

        // Condition A # If origin point is between the start & end mard,
        //we have to draw both from origin to left mark & origin to right mark.
        // Condition B # If origin point is left of the start mark, we have to draw
        // from origin to end mark.
        // Condition C # If origin point is right of the end mark, we have to draw
        // from origin to start mark.
        if (mOrigin >= rulerStartMark && mOrigin <= rulerEndMark) {
            drawFromOriginTo(painter, rulerRect, mOrigin, rulerEndMark, 0, scaleMeter, startPositoin);
            drawFromOriginTo(painter, rulerRect, mOrigin, rulerStartMark, 0, -scaleMeter, startPositoin);
        } else if (mOrigin < rulerStartMark) {
            int tickNo = int((rulerStartMark - mOrigin) / scaleMeter);
            drawFromOriginTo(painter, rulerRect, mOrigin + scaleMeter * tickNo, rulerEndMark, tickNo, scaleMeter, startPositoin);
        } else if (mOrigin > rulerEndMark) {
            int tickNo = int((mOrigin - rulerEndMark) / scaleMeter);
            drawFromOriginTo(painter, rulerRect, mOrigin - scaleMeter * tickNo, rulerStartMark, tickNo, -scaleMeter, startPositoin);
        }
    }

    void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition) {
        bool isHorzRuler = Horizontal == mRulerType;
        int iterate = 0;

        for (qreal current = startMark; (step < 0 ? current >= endMark : current <= endMark); current += step) {
            qreal x1 = isHorzRuler ? current : rulerRect.left() + startPosition;
            qreal y1 = isHorzRuler ? rulerRect.top() + startPosition : current;
            qreal x2 = isHorzRuler ? current : rulerRect.right();
            qreal y2 = isHorzRuler ? rulerRect.bottom() : current;
            painter->drawLine(QLineF(x1,y1,x2,y2));
            if (mDrawText) {
                QPainterPath txtPath;
                txtPath.addText(x1 + 1,y1 + (isHorzRuler ? 7 : -2),this->font(),QString::number(qAbs(int(step) * startTickNo++)));
                painter->drawPath(txtPath);
                iterate++;
            }
        }
    }

    void drawMousePosTick(QPainter* painter) {
        if (mMouseTracking) {
            QPoint starPt = mCursorPos;
            QPoint endPt;
            if (Horizontal == mRulerType) {
                starPt.setY(this->rect().top());
                endPt.setX(starPt.x());
                endPt.setY(this->rect().bottom());
            } else {
                starPt.setX(this->rect().left());
                endPt.setX(this->rect().right());
                endPt.setY(starPt.y());
            }
            painter->drawLine(starPt,endPt);
        }
    }

private:
    RulerType mRulerType;
    qreal mOrigin;
    qreal mRulerUnit;
    qreal mRulerZoom;
    QPoint mCursorPos;
    bool mMouseTracking;
    bool mDrawText;
};

class RPML_EXPORT BaseReportWidget: public RichEditor::RichEditorWidget
{
    Q_OBJECT

public:
    explicit BaseReportWidget( bool designerMode, QMainWindow *parent );
    virtual ~BaseReportWidget();

    virtual bool save( const QString fileName , QString &errorMessage );
    virtual bool load( const QString fileName , QString &errorMessage );
    virtual void clearReport();

    inline QToolBar *editorToolBar() { return m_editorToolBar; }
    inline bool pageMode() const { return m_usePagination; }
    void setPageMode( bool onoff );

protected:
    virtual void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent* event );

private slots:
    void onReportPrefs();
    void usePageMode( bool onoff );

protected:
    void recalcPageSize();
    void paintPagesView();

    void setupEditorActions();

    ReportData *m_reportData;
    bool m_usePagination;
    QAction *m_actPageMode;

    int m_width;
    int m_height;
    int m_heightWithDecor;
    int m_headerDecorHeight;
    int m_footerDecorHeight;

    QToolBar *m_editorToolBar;
};

class RPML_EXPORT ReportEditorWidget: public BaseReportWidget
{
    Q_OBJECT

public:
    explicit ReportEditorWidget( QMainWindow *parent );
    virtual ~ReportEditorWidget();

    void updateContent( QTextDocument *textDocument , ReportData *reportData );

    inline QToolBar *printToolBar() { return m_printToolBar; }

private slots:
    void reportPrint();
    void exportToOdf();

signals:
    void documentChanged();

private:
    void setupPrintActions();
    QToolBar *m_printToolBar;
};

class RPML_EXPORT ReportDesignerWidget: public BaseReportWidget
{
    Q_OBJECT

public:
    explicit ReportDesignerWidget( QMainWindow *parent );
    virtual ~ReportDesignerWidget();

    virtual bool load( const QString fileName , QString &errorMessage );

    inline QToolBar *reportToolBar() { return m_reportToolBar; }
    QDockWidget *dataSourcesDock( QMainWindow *mainWindow );
    QDockWidget *inputParametersDock( QMainWindow *mainWindow );

private slots:
    void reportExecute();

    void dataTableInsert();
    void fieldInsert();
    void parameterInsert();
    void inputParameterInsert( int index );

    void addInputParameter();
    void delInputParameter();
    void addDataSource();
    void editDataSource();
    void delDataSource();

    void reportDataChanged();

private:
    void setupReportActions();

    void parameterInsertInternal( int parameterId );
    void fieldInsertInternal( int queryId, int columnNumber );

    void proceedDataTable( QTextDocument *document, QTextTable *table, int id, const QMap<int, DataSourceResult *> &resultsMap );
    void replaceTableFieldsWithMessage( QTextTable *table, const QString &message );

    QToolBar *m_reportToolBar;

    QAction *m_actDataTableInsert;
    QAction *m_actFieldInsert;
    QAction *m_actParameterInsert;

    QwCustomDockWidget *m_dataSourcesDock;
    DataSourcesModel *m_dataSourcesModel;
    QTableView *m_dataSourcesView;

    QwCustomDockWidget *m_inputParamsDock;
    ReportInputParametersModel *m_parametersModel;
    ReportInputParameterTableView *m_parametersView;
};

}   // namespace RPML

#endif // REPORTEDITORWIDGET_H
