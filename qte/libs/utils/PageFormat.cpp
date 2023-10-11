#include "PageFormat.h"

namespace Utils {

static inline double mmToInches(double mm) { return mm * 0.039370147; }
static inline double inchesToMm(double inch) { return inch / 0.039370147; }

typedef struct tagPageFormatSize {
    QString formatName;
    qreal   wmm;
    qreal   hmm;
    QPrinter::PageSize size;
} PageFormatSize;

static PageFormatSize formats[30] = {
    {        "A0",  841.0, 1189.0, QPrinter::A0         },
    {        "A1",  594.0,  841.0, QPrinter::A1         },
    {        "A2",  420.0,  594.0, QPrinter::A2         },
    {        "A3",  297.0,  420.0, QPrinter::A3         },
    {        "A4",  210.0,  297.0, QPrinter::A4         },
    {        "A5",  148.0,  210.0, QPrinter::A5         },
    {        "A6",  105.0,  148.0, QPrinter::A6         },
    {        "A7",   74.0,  105.0, QPrinter::A7         },
    {        "A8",   52.0,   74.0, QPrinter::A8         },
    {        "A9",   37.0,   52.0, QPrinter::A9         },
    {        "B0", 1030.0, 1456.0, QPrinter::B0         },
    {        "B1",  728.0, 1030.0, QPrinter::B1         },
    {        "B2",  515.0,  728.0, QPrinter::B2         },
    {        "B3",  364.0,  515.0, QPrinter::B3         },
    {        "B4",  257.0,  364.0, QPrinter::B4         },
    {        "B5",  182.0,  257.0, QPrinter::B5         },
    {        "B6",  128.0,  182.0, QPrinter::B6         },
    {        "B7",   91.0,  128.0, QPrinter::B7         },
    {        "B8",   64.0,   91.0, QPrinter::B8         },
    {        "B9",   45.0,   64.0, QPrinter::B9         },
    {       "B10",   32.0,   45.0, QPrinter::B10        },
    {       "C5E",  163.0,  229.0, QPrinter::C5E        },
    {   "Comm10E",  105.0,  241.0, QPrinter::Comm10E    },
    {       "DLE",  110.0,  220.0, QPrinter::DLE        },
    { "Executive",  191.0,  254.0, QPrinter::Executive  },
    {     "Folio",  210.0,  330.0, QPrinter::Folio      },
    {    "Ledger",  432.0,  279.0, QPrinter::Ledger     },
    {     "Legal",  216.0,  356.0, QPrinter::Legal      },
    {    "Letter",  216.0,  279.0, QPrinter::Letter     },
    {   "Tabloid",  279.0,  432.0, QPrinter::Tabloid    }
};

static QSizeF sizeForPageFormat( const QString &format, bool isPortrait, int &formatId )
{
    for (int i = 0; i < 30; i++)
        if ( !format.compare(formats[i].formatName, Qt::CaseInsensitive) ) {
            formatId = i;
            return isPortrait ? QSizeF(formats[i].wmm, formats[i].hmm) : QSizeF(formats[i].hmm, formats[i].wmm);
        }

    formatId = 4;
    return isPortrait ? QSizeF(210.0, 297.0) : QSizeF(297.0, 210.0);
}

void PageFormat::Decoration::load( QDomElement *decorationElement )
{
    height = decorationElement->attribute("height", "0").toDouble();
    spacing = decorationElement->attribute("spacing", "3.0").toDouble();
    ruleWidth = decorationElement->attribute("ruleWidth", "1.0").toDouble();
    ruleColor = QColor(decorationElement->attribute("ruleColor", QColor(Qt::darkGray).name()));
}

void PageFormat::Decoration::save( QDomElement *decorationElement )
{
    decorationElement->setAttribute("height", height);
    decorationElement->setAttribute("spacing", spacing);
    decorationElement->setAttribute("ruleWidth", ruleWidth);
    decorationElement->setAttribute("ruleColor", ruleColor.name());
}

PageFormat::PageFormat( QPaintDevice *device,
                        const QString &format,
                        bool  isPortrait,
                        const Margin &mmMargins,
                        const Decoration &header,
                        const Decoration &footer )
    : m_device(device)
{
    update(format, isPortrait, mmMargins, header, footer);
}

void PageFormat::update( const QString &format,
                         bool  isPortrait,
                         const Margin &mmMargins,
                         const Decoration &header,
                         const Decoration &footer )
{
    m_format = format;
    m_isPortrait = isPortrait;

    m_mmMargins = mmMargins;
    m_pxMargins = Margin( mmToInches(m_mmMargins.left)      * m_device->logicalDpiX(),
                          mmToInches(m_mmMargins.top)       * m_device->logicalDpiY(),
                          mmToInches(m_mmMargins.right)     * m_device->logicalDpiX(),
                          mmToInches(m_mmMargins.bottom)    * m_device->logicalDpiY() );

    m_header = header;
    m_footer = footer;

    QSizeF mmSize = sizeForPageFormat(m_format, m_isPortrait, m_formatId);
    m_paperWidth = mmSize.width();
    m_paperHeight = mmSize.height();
    m_pageWidth = mmSize.width() - m_mmMargins.left - m_mmMargins.right;
    m_pageHeight = mmSize.height() - m_mmMargins.top - m_mmMargins.bottom;

    m_paperWidth = mmToInches(m_paperWidth) * m_device->logicalDpiX();
    m_pageWidth = mmToInches(m_pageWidth) * m_device->logicalDpiX();

    m_paperHeight = mmToInches(m_paperHeight) * m_device->logicalDpiY();
    m_pageHeight = mmToInches(m_pageHeight) * m_device->logicalDpiY();
}

void PageFormat::update( int formatId,
                         bool isPortrait,
                         const Margin &mmMargins,
                         const Decoration &header,
                         const Decoration &footer )
{
    m_format = formats[formatId].formatName;
    m_isPortrait = isPortrait;
    m_formatId = formatId;

    m_mmMargins = mmMargins;
    m_pxMargins = Margin( mmToInches(m_mmMargins.left)      * m_device->logicalDpiX(),
                          mmToInches(m_mmMargins.top)       * m_device->logicalDpiY(),
                          mmToInches(m_mmMargins.right)     * m_device->logicalDpiX(),
                          mmToInches(m_mmMargins.bottom)    * m_device->logicalDpiY() );

    m_header = header;
    m_footer = footer;

    if ( isPortrait ) {
        m_paperWidth  = formats[formatId].wmm;
        m_paperHeight = formats[formatId].hmm;
    }
    else {
        m_paperHeight = formats[formatId].wmm;
        m_paperWidth  = formats[formatId].hmm;
    }

    m_pageWidth = m_paperWidth - m_mmMargins.left - m_mmMargins.right;
    m_pageHeight = m_paperHeight - m_mmMargins.top - m_mmMargins.bottom;

    m_paperWidth = mmToInches(m_paperWidth) * m_device->logicalDpiX();
    m_pageWidth = mmToInches(m_pageWidth) * m_device->logicalDpiX();

    m_paperHeight = mmToInches(m_paperHeight) * m_device->logicalDpiY();
    m_pageHeight = mmToInches(m_pageHeight) * m_device->logicalDpiY();
}

void PageFormat::load( QDomElement *pageFormatElement )
{
    Decoration header;
    QDomElement headerElement = pageFormatElement->firstChildElement("header");
    if ( !headerElement.isNull() )
        header.load(&headerElement);

    Decoration footer;
    QDomElement footerElement = pageFormatElement->firstChildElement("footer");
    if ( !footerElement.isNull() )
        footer.load(&footerElement);

    update( pageFormatElement->attribute("page-format", "A4"),
            pageFormatElement->attribute("page-orientation", "portrait") == QLatin1String("portrait"),
            Margin( pageFormatElement->attribute("page-left-margin", "15.0").toDouble(),
                    pageFormatElement->attribute("page-top-margin", "15.0").toDouble(),
                    pageFormatElement->attribute("page-right-margin", "15.0").toDouble(),
                    pageFormatElement->attribute("page-bottom-margin", "15.0").toDouble() ),
            header, footer );
}

void PageFormat::save(QDomDocument *document, QDomElement *pageFormatElement )
{
    pageFormatElement->setAttribute("page-format", m_format);
    pageFormatElement->setAttribute("page-orientation", m_isPortrait ?  "portrait" : "landscape");
    pageFormatElement->setAttribute("page-left-margin", QString::number(m_mmMargins.left));
    pageFormatElement->setAttribute("page-top-margin", QString::number(m_mmMargins.top));
    pageFormatElement->setAttribute("page-right-margin", QString::number(m_mmMargins.right));
    pageFormatElement->setAttribute("page-bottom-margin", QString::number(m_mmMargins.bottom));

    QDomElement headerElement = document->createElement("header");
    m_header.save(&headerElement);
    pageFormatElement->appendChild(headerElement);

    QDomElement footerElement = document->createElement("footer");
    m_footer.save(&footerElement);
    pageFormatElement->appendChild(footerElement);
}

qreal PageFormat::pxHeaderHeight() const
{
    qreal height = 0.0;
    if ( m_header.height > 0 )
        height += mmToInches(m_header.height + m_header.spacing) * m_device->logicalDpiY();
    return height;
}

qreal PageFormat::pxFooterHeight() const
{
    qreal height = 0.0;
    if ( m_footer.height > 0 )
        height += mmToInches(m_footer.height + m_footer.spacing) * m_device->logicalDpiY();
    return height;
}

qreal PageFormat::pxDecorationsHeight() const
{
    return pxHeaderHeight() + pxFooterHeight();
}

qreal PageFormat::pxHeaderDrawHeight() const
{
    qreal height = 0.0;
    if ( m_header.height > 0 )
        height += mmToInches(m_header.height) * m_device->logicalDpiY();
    return height;
}

qreal PageFormat::pxFooterDrawHeight() const
{
    qreal height = 0.0;
    if ( m_footer.height > 0 )
        height += mmToInches(m_footer.height) * m_device->logicalDpiY();
    return height;
}

qreal PageFormat::pxHeaderSpacing() const
{
    qreal height = 0.0;
    if ( m_header.height > 0 )
        height += mmToInches(m_header.spacing) * m_device->logicalDpiY();
    return height;
}

qreal PageFormat::pxFooterSpacing() const
{
    qreal height = 0.0;
    if ( m_footer.height > 0 )
        height += mmToInches(m_footer.spacing) * m_device->logicalDpiY();
    return height;
}

QPrinter::PageSize PageFormat::printerPageSize() const
{
    if ( m_formatId >= 0 && m_formatId < 30 )
        return formats[m_formatId].size;

    return QPrinter::A4;
}

QSizeF PageFormat::sizeForFormatId( int formatId, bool isPortrait )
{
    if ( isPortrait )
        return QSizeF( formats[formatId].wmm, formats[formatId].hmm );

    return QSizeF( formats[formatId].hmm, formats[formatId].wmm );
}

QSizeF PageFormat::mmPaperSize()
{
    if ( m_isPortrait )
        return QSizeF( formats[m_formatId].wmm, formats[m_formatId].hmm );

    return QSizeF( formats[m_formatId].hmm, formats[m_formatId].wmm );
}

qreal PageFormat::pixelsToMmY( qreal px ) const
{
    return inchesToMm(px / m_device->logicalDpiY());
}

}   // namespace Utils
