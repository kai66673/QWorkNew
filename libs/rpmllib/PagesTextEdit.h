#ifndef PAGESTEXTEDIT_H
#define PAGESTEXTEDIT_H

#include <QTextEdit>

#include "rpmllib_global.h"

class RPML_EXPORT PagesTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit PagesTextEdit( bool usePageMode = false, int pageWidth = 0, int pageHeight = 0,
                            int headerHeight = 50, int footerHeight = 50,
                            QWidget *parent = 0 );

    inline bool usePageMode() const { return m_usePageMode; }
    inline int  pageWidth() const { return m_pageWidth - 2; }
    inline int  pageHeight() const { return m_pageHeight - 20; }

signals:

public slots:
    void setUsePageMode( bool usePageMode );
    void setPageSize( int pageWidth, int pageHeight );

protected:
    void paintEvent( QPaintEvent* event );

private:
    void updateInnerGeometry();
    void paintPagesView();

    bool m_usePageMode;
    int  m_pageWidth;
    int  m_pageHeight;

    int  m_headerHeight;
    int  m_footerHeight;
};

#endif // PAGESTEXTEDIT_H
