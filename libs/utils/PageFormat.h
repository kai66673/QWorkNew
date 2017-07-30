#ifndef PAGEFORMAT_H
#define PAGEFORMAT_H

#include <QString>
#include <QPaintDevice>
#include <QDomElement>
#include <QPrinter>
#include <QColor>

#include "utils_global.h"

namespace Utils {

class QTCREATOR_UTILS_EXPORT PageFormat
{
public:
    struct Margin {
        Margin( qreal left_ = 0, qreal top_ = 0, qreal right_ = 0, qreal bottom_ = 0 )
            : left(left_)
            , top(top_)
            , right(right_)
            , bottom(bottom_)
        {}

        qreal left;
        qreal top;
        qreal right;
        qreal bottom;
    };

    struct Decoration {
        Decoration( qreal  height_ = 0,
                    qreal  spacing_  = 3.0,
                    qreal  ruleWidth_ = 1.0 ,
                    QColor ruleColor_ = QColor(Qt::darkGray) )
            : height(height_)
            , spacing(spacing_)
            , ruleWidth(ruleWidth_)
            , ruleColor(ruleColor_)
        {}

        void load( QDomElement *decorationElement );
        void save( QDomElement *decorationElement );

        qreal  height;
        qreal  spacing;
        qreal  ruleWidth;
        QColor ruleColor;
    };

    PageFormat( QPaintDevice *device,
                const QString &format = "A4",
                bool  isPortrait = true,
                const Margin &mmMargins = Margin(15.0, 15.0, 15.0, 15.0),
                const Decoration &header = Decoration(),
                const Decoration &footer = Decoration() );

    void update( const QString &format = "A4",
                 bool  isPortrait = true,
                 const Margin &mmMargins = Margin(15.0, 15.0, 15.0, 15.0),
                 const Decoration &header = Decoration(),
                 const Decoration &footer = Decoration() );

    void update( int   formatId,
                 bool  isPortrait = true,
                 const Margin &mmMargins = Margin(15.0, 15.0, 15.0, 15.0),
                 const Decoration &header = Decoration(),
                 const Decoration &footer = Decoration() );

    void load( QDomElement *pageFormatElement );
    void save( QDomDocument *document, QDomElement *pageFormatElement );

    inline qreal paperWidth() const { return m_paperWidth; }
    inline qreal pageWidth() const { return m_pageWidth; }
    inline qreal paperHeight() const { return m_paperHeight; }
    inline qreal pageHeight() const { return m_pageHeight; }

    inline int formatId() const { return m_formatId; }
    inline bool isPortrait() const { return m_isPortrait; }

    inline Margin mmMargins() const { return m_mmMargins; }
    inline Margin pxMargins() const { return m_pxMargins; }

    qreal pxHeaderHeight() const;
    qreal pxFooterHeight() const;
    qreal pxDecorationsHeight() const;
    qreal pxHeaderDrawHeight() const;
    qreal pxFooterDrawHeight() const;
    qreal pxHeaderSpacing() const;
    qreal pxFooterSpacing() const;

    inline Decoration header() const { return m_header; }
    inline Decoration footer() const { return m_footer; }

    QPrinter::PageSize printerPageSize() const;

    static QSizeF sizeForFormatId( int formatId, bool isPortrait );

    QSizeF mmPaperSize();

    qreal pixelsToMmY( qreal px ) const;

private:
    QString m_format;       // Name of Page Format <=> QPrinter::PaperSize ("A4 (default), "Letter", "B5"...)
    bool m_isPortrait;      // true for Portrait Orientation

    Margin m_mmMargins;     // Margins in mm
    Margin m_pxMargins;     // Matgins in px

    qreal m_paperWidth;     // Paper Width in px
    qreal m_pageWidth;      // Page Width in px (m_paperWidth - m_pxMargins.left - m_pxMargins.right)
    qreal m_paperHeight;    // Paper Height in px
    qreal m_pageHeight;     // Page Height in px (m_paperHeight - m_pxMargins.top - m_pxMargins.bottom)

    Decoration m_header;
    Decoration m_footer;

    QPaintDevice *m_device;
    int m_formatId;
};

}   // namespace Utils

#endif // PAGEFORMAT_H
