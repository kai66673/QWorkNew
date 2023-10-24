#include <QFileInfo>
#include <QDir>
#include <QTextStream>

#include "highlighterformatsconfigurewidget.h"
#include "highlighterformatsmanager.h"

HighlighterFormatData::HighlighterFormatData()
{ }

HighlighterFormatData::~HighlighterFormatData()
{
    qDeleteAll(m_formats);
}

bool HighlighterFormatData::fillTextCharFormat(int index, QTextCharFormat &charFormat) const
{
    if ( index < 0 || index >= m_formats.size() )
        return false;

    charFormat.setFontWeight(m_formats[index]->bold ? QFont::Bold : QFont::Normal);
    charFormat.setFontItalic(m_formats[index]->italic);
    m_formats[index]->foregroundColor.isValid()
            ? charFormat.setForeground(QBrush(m_formats[index]->foregroundColor))
            : charFormat.setForeground(QBrush());
    m_formats[index]->backgroundColor.isValid()
            ? charFormat.setBackground(QBrush(m_formats[index]->backgroundColor))
            : charFormat.setBackground(QBrush());

    return true;
}

void HighlighterFormatData::save(QDomDocument *document, QDomElement *parentElement)
{
    for (int i = 0; i < m_formats.size(); i++) {
        QDomElement formatElement = document->createElement("format");
        HighlighterFormat *format = m_formats[i];
        formatElement.setAttribute("index", i);
        formatElement.setAttribute("name", format->name);
        formatElement.setAttribute("bold", format->bold);
        formatElement.setAttribute("italic", format->italic);
        formatElement.setAttribute("foreground-color", format->foregroundColor.isValid() ? format->foregroundColor.name() : "#");
        formatElement.setAttribute("background-color", format->backgroundColor.isValid() ? format->backgroundColor.name() : "#");
        parentElement->appendChild(formatElement);
    }
}

void HighlighterFormatData::save(QSettings &settings)
{
    settings.beginWriteArray(QLatin1String("HL_FORMATS"));

    for (int i = 0; i < m_formats.size(); i++) {
        HighlighterFormat *format = m_formats[i];
        settings.setArrayIndex(i);

        settings.setValue("name", format->name);
        settings.setValue("bold", format->bold);
        settings.setValue("italic", format->italic);
        settings.setValue("foreground-color", format->foregroundColor.isValid() ? format->foregroundColor.name() : "#");
        settings.setValue("background-color", format->backgroundColor.isValid() ? format->backgroundColor.name() : "#");
    }

    settings.endArray();
}

void HighlighterFormatData::restore(QDomElement *parentElement)
{
    QDomElement formatElement = parentElement->firstChildElement();
    while ( !formatElement.isNull() ) {
        if ( formatElement.tagName() == QLatin1String("format") ) {
            if ( formatElement.hasAttribute("index") ) {
                if ( formatElement.hasAttribute("name") ) {
                    bool intOk;
                    int index = formatElement.attribute("index", "-1").toInt(&intOk);
                    if ( !(!intOk || index < 0 || index >= m_formats.size()) ) {
                        QString name = formatElement.attribute("name");
                        if ( m_formats[index]->name == name ) {
                            QString colorName;
                            if ( formatElement.hasAttribute("bold") )
                                m_formats[index]->bold = formatElement.attribute("bold").toInt();
                            if ( formatElement.hasAttribute("italic") )
                                m_formats[index]->italic = formatElement.attribute("italic").toInt();
                            if ( formatElement.hasAttribute("foreground-color") ) {
                                colorName = formatElement.attribute("foreground-color");
                                m_formats[index]->foregroundColor = colorName == QLatin1String("#") ? QColor() : QColor(colorName);
                            }
                            if ( formatElement.hasAttribute("background-color") ) {
                                colorName = formatElement.attribute("background-color");
                                m_formats[index]->backgroundColor = colorName == QLatin1String("#") ? QColor() : QColor(colorName);
                            }
                        } else {
//                            Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nInvalid value attribute \"name\" at line %1, waiting \"%2\"")
//                                                                           .arg(formatElement.lineNumber())
//                                                                           .arg(m_formats[index]->name)) );
                        }
                    } else {
//                        Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nInvalid value of attribute \"index\" at line %1")
//                                                                       .arg(formatElement.lineNumber())) );
                    }
                } else {
//                    Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nExpected attribute \"name\" for tag <format> at line %1")
//                                                                   .arg(formatElement.lineNumber())) );
                }
            } else {
//                Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nExpected attribute \"index\" for tag <format> at line %1")
//                                                               .arg(formatElement.lineNumber())) );
            }
        } else {
//            Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nUnexpected tag <%1>, waiting <format> at line %2")
//                                                           .arg(formatElement.tagName())
//                                                           .arg(formatElement.lineNumber())) );
        }
        formatElement = formatElement.nextSiblingElement();
    }
}

void HighlighterFormatData::restore(QSettings &settings)
{
    int size = settings.beginReadArray(QLatin1String("HL_FORMATS"));

    for (int i = 0; i < size; i++) {
        HighlighterFormat *format = m_formats[i];
        settings.setArrayIndex(i);

        format->name = settings.value("name").toString();
        format->bold = settings.value("bold").toBool();
        format->italic = settings.value("italic").toBool();
        QString colorStr = settings.value("foreground-color").toString();
        format->foregroundColor = colorStr == QLatin1String("#") ? QColor() : QColor(colorStr);
        colorStr = settings.value("background-color").toString();
        format->backgroundColor = colorStr == QLatin1String("#") ? QColor() : QColor(colorStr);
    }

    settings.endArray();
}

QList<HighlighterFormat *> HighlighterFormatData::cloneFormats() const
{
    QList<HighlighterFormat *> result;
    foreach (HighlighterFormat *format, m_formats)
        result << new HighlighterFormat(format->name,
                                        format->bold,
                                        format->italic,
                                        format->foregroundColor,
                                        format->backgroundColor);
    return result;
}

void HighlighterFormatData::reinit(QList<HighlighterFormat *> &formats)
{
    qDeleteAll(m_formats);
    m_formats.clear();
    foreach (HighlighterFormat *format, formats)
        m_formats << format;
    formats.clear();
}

class GeneralFormatData: public HighlighterFormatData
{
public:
    GeneralFormatData() {
        m_formats.append( new HighlighterFormat( QObject::tr("Text"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor(255, 255, 255) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Link"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0, 255),
                                                 QColor(255, 255, 255) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Selection"),
                                                 false,
                                                 false,
                                                 QColor(255, 255, 255),
                                                 QColor( 32,  32,  32) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Line Number"),
                                                 false,
                                                 false,
                                                 QColor( 32,  32,  32),
                                                 QColor(255, 255, 255) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Current Line"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor(222, 222, 222) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Current Line Number"),
                                                 true,
                                                 false,
                                                 QColor( 32,  32,  32),
                                                 QColor(255, 255, 255) ) );
    }

    virtual QString languageId() const { return QObject::tr("General"); }
    virtual int     prioritet() const { return 100; }
};

HighlighterFormatsManager *HighlighterFormatsManager::m_instance = 0;

HighlighterFormatsManager::HighlighterFormatsManager(QObject *parent)
    : QObject(parent)
{
    m_instance = this;

    m_generalFormatData = new GeneralFormatData();
    registerLanguageHighlighterInternal(m_generalFormatData);
}

HighlighterFormatsManager::~HighlighterFormatsManager()
{
    qDeleteAll(m_languageFormatMap.values());
}

void HighlighterFormatsManager::registerLanguageHighlighter(HighlighterFormatData *data)
{
    m_instance->registerLanguageHighlighterInternal(data);
}

void HighlighterFormatsManager::registerLanguageHighlighterInternal(HighlighterFormatData *data)
{
    if ( !data )
        return;

    if ( m_languageFormatMap.find(data->languageId()) != m_languageFormatMap.end() ) {
//        Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Register Language Highlighter Failed: Highlighter with Language ID \"%1\" already registered")
//                                                       .arg(data->languageId())) );
        return;
    }

    m_languageFormatMap[data->languageId()] = data;

    if ( m_languageFormatsSorted.isEmpty() ) {
        m_languageFormatsSorted.append(data);
    } else {
        int index = 1;
        for (; index < m_languageFormatsSorted.size(); index++) {
            if ( data->prioritet() > m_languageFormatsSorted[index]->prioritet() )
                break;
        }
        m_languageFormatsSorted.insert(index, data);
    }
}

HighlighterFormatData *HighlighterFormatsManager::highlighterFormatData(const QString &languageId)
{
    QMap<QString, HighlighterFormatData *>::const_iterator it = m_instance->m_languageFormatMap.find(languageId);
    return it == m_instance->m_languageFormatMap.end() ? 0 : *it;
}

HighlighterFormatsConfigureWidget *HighlighterFormatsManager::highlighterFormatsConfigureWidget()
{
    HighlighterFormatsConfigureWidget *highlighterFormatsConfigureWidget =  new HighlighterFormatsConfigureWidget(m_instance->m_languageFormatsSorted);
    connect(highlighterFormatsConfigureWidget, SIGNAL(formatsChanged()), m_instance, SLOT(save()));
    return highlighterFormatsConfigureWidget;
}

QColor HighlighterFormatsManager::defaultBackgroundColor()
{
    return m_instance->m_generalFormatData->m_formats[0]->backgroundColor;
}

void HighlighterFormatsManager::save(const QString &configFileName)
{
    const int IndentSize = 4;

    QFile file(configFileName);

    if ( !file.exists() ) {
        QFileInfo fi(configFileName);
        if ( !fi.absoluteDir().exists() ) {
            QDir dir;
            dir.mkpath(fi.absolutePath());
        }
    }

    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
//        Core::Storage::warning( QObject::tr("Error On Saving Highlighter Formats:\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
//                                .arg(configFileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("highlighter-formats");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

    QDomElement root = domDocument.createElement("highlighter-formats");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    foreach ( HighlighterFormatData *data, m_languageFormatMap.values() ) {
        QDomElement dataElement = domDocument.createElement("language");
        dataElement.setAttribute("id", data->languageId());
        data->save(&domDocument, &dataElement);
        root.appendChild(dataElement);
    }

    QTextStream out(&file);
    domDocument.save(out, IndentSize);
    file.close();
}

void HighlighterFormatsManager::restore(const QString &configFileName)
{
    QFile file(configFileName);

    if ( !file.exists() )
        return;

    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
//        Core::Storage::warning( QObject::tr("Error On Highlighter Formats Loading\nFile %1 Cannot be Loaded")
//                                .arg(configFileName) );
        return;
    }

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Load All
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
//        Core::Storage::warning( QObject::tr("Error On Highlighter Formats Loading\nCommon Error in xml-file:\nLine %1, Column %2: %3")
//                                .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Primary validation (type/version)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "highlighter-formats" ) {
//        Core::Storage::warning( QObject::tr("Error On Highlighter Formats Loading\nInvalid Format xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
//        Core::Storage::warning( QObject::tr("Error On Highlighter Formats Loading\nInvalid Format xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
//        Core::Storage::warning( QObject::tr("Error On Highlighter Formats Loading\nInvalid Format xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "language" ) {
//            Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nUnexpected tag <%1>, waiting <language> at line %2")
//                                                           .arg(itemElement.tagName())
//                                                           .arg(itemElement.lineNumber())) );
        } else if ( !itemElement.hasAttribute("id") ) {
//            Core::LogManager::addMessage( Core::LogMessage(QObject::tr("Error On Highlighter Formats Loading:\nExpected attribure \"id\" for tag <language> at line %1")
//                                                           .arg(itemElement.lineNumber())) );
        } else if ( HighlighterFormatData *data = highlighterFormatData(itemElement.attribute("id")) ) {
            data->restore(&itemElement);
        } else {
//            Core::LogManager::addMessage( QObject::tr("Error On Highlighter Formats Loading:\nHighlighter for Language with id \"%1\" not registered")
//                                          .arg(itemElement.attribute("id")));
        }
        itemElement = itemElement.nextSiblingElement();
    }
}
