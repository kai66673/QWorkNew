#include <QScrollBar>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextFrame>

#include "PagesTextEdit.h"

PagesTextEdit::PagesTextEdit( bool usePageMode, int pageWidth, int pageHeight,
                              int headerHeight, int footerHeight,
                              QWidget *parent )
    : QTextEdit(parent)
    , m_usePageMode(usePageMode)
    , m_pageWidth(pageWidth + 2)
    , m_pageHeight(pageHeight + 20)
    , m_headerHeight(headerHeight)
    , m_footerHeight(footerHeight)
{
//    QTextFrame *rootFrame = document()->rootFrame();
//    QTextFrameFormat ff = rootFrame->frameFormat();
//    ff.setTopMargin(20 + m_headerHeight);
//    ff.setBottomMargin(20 + m_footerHeight);
//    rootFrame->setFrameFormat(ff);
    document()->setDocumentMargin(20);
    updateInnerGeometry();
}

void PagesTextEdit::setUsePageMode( bool usePageMode )
{
    if ( m_usePageMode != usePageMode ) {
        if ( usePageMode && m_pageWidth > 2 && m_pageHeight > 20 ) {
            m_usePageMode = true;
        } else if ( !usePageMode ) {
            m_usePageMode = false;
        }
        repaint();
    }
}

void PagesTextEdit::setPageSize( int pageWidth, int pageHeight )
{
    m_pageWidth = pageWidth + 2;
    m_pageHeight = pageHeight + 20;
    if ( m_usePageMode )
        repaint();
}

void PagesTextEdit::paintEvent( QPaintEvent *event )
{
    updateInnerGeometry();
    paintPagesView();
    QTextEdit::paintEvent(event);
}

void PagesTextEdit::updateInnerGeometry()
{
    QSizeF documentSize(width() + verticalScrollBar()->width(), -1);
    QMargins viewportMargins;

    if ( m_usePageMode ) {
        documentSize = QSizeF(m_pageWidth, m_pageHeight /*- m_footerHeight - m_headerHeight*/);

        if ( width() > m_pageWidth ) {
            viewportMargins =
                    QMargins( (width() - m_pageWidth - verticalScrollBar()->width() - 2) / 2, 20 /*+ m_footerHeight*/,
                              (width() - m_pageWidth - verticalScrollBar()->width() - 2) / 2, 20 /*+ m_headerHeight */);
        } else {
            viewportMargins = QMargins(0, 20 /*+ m_footerHeight*/, 0, 20 /*+ m_headerHeight*/);
        }

        verticalScrollBar()->setRange(0, m_pageHeight * (document()->pageCount()) - viewport()->size().height());
    }

    if ( document()->pageSize() != documentSize ) {
        document()->setPageSize(documentSize);
    }

    setViewportMargins(viewportMargins);
}

void PagesTextEdit::paintPagesView()
{
    if ( m_usePageMode ) {
        QPainter p(viewport());
        QPen spacePen(palette().window(), 9);
        QPen borderPen(palette().dark(), 1);
        borderPen.setStyle(Qt::DotLine);

        int curHeight = m_pageHeight - (verticalScrollBar()->value() % m_pageHeight);

        // Нарисовать верхнюю границу
        if ( curHeight - m_pageHeight >= 0 ) {
            p.setPen(borderPen);
            p.drawLine(0, curHeight - m_pageHeight, m_pageWidth, curHeight - m_pageHeight);
        }

        while ( curHeight < height() ) {
            // Фон разрыва страниц
            p.setPen(spacePen);
            p.drawLine(0, curHeight-4, width(), curHeight - 4);
            // Границы страницы
            p.setPen(borderPen);
            p.drawLine(0, curHeight-8, m_pageWidth, curHeight - 8);
            p.drawLine(0, curHeight, m_pageWidth, curHeight);
            p.drawLine(0, curHeight - m_pageHeight, 0, curHeight - 8);
            p.drawLine(m_pageWidth - 1, curHeight - m_pageHeight, m_pageWidth - 1, curHeight - 8);

            curHeight += m_pageHeight;
        }

        // Нарисовать боковые границы страницы, когда страница не влезает в экран
        if ( curHeight >= height() ) {
            // Границы страницы
            p.setPen(borderPen);
            p.drawLine(0, curHeight - m_pageHeight, 0, height());
            p.drawLine(m_pageWidth-1, curHeight - m_pageHeight, m_pageWidth - 1, height());
        }
    }
}
