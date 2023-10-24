#ifndef HIGHLIGHTERFORMATSMANAGER_H
#define HIGHLIGHTERFORMATSMANAGER_H

#include <QString>
#include <QColor>
#include <QMap>
#include <QList>
#include <QTextCharFormat>
#include <QDomDocument>
#include <QSettings>

#include "corelib_global.h"

class HighlighterFormatsManager;
class HighlighterFormatsConfigureWidget;

struct CORELIB_EXPORT HighlighterFormat {
    HighlighterFormat( const QString &name_            = "",
                          bool     bold_            = false,
                          bool     italic_          = false,
                     const QColor &foregroundColor_ = QColor(),
                     const QColor &backgroundColor_ = QColor())
                    : name(name_)
                    , bold(bold_)
                    , italic(italic_)
                    , foregroundColor(foregroundColor_)
                    , backgroundColor(backgroundColor_)
    {}

    QString name;
    bool    bold;
    bool    italic;
    QColor  foregroundColor;
    QColor  backgroundColor;
};

class CORELIB_EXPORT HighlighterFormatData
{
    friend class HighlighterFormatsManager;
    friend class HighlighterFormatsConfigureWidget;

public:
    HighlighterFormatData();
    virtual ~HighlighterFormatData();

    virtual QString languageId() const = 0;
    virtual int     prioritet() const { return 40; }

    bool fillTextCharFormat(int index, QTextCharFormat &charFormat) const;

    void save(QDomDocument *document, QDomElement *parentElement);
    void save(QSettings &settings);
    void restore(QDomElement *parentElement);
    void restore(QSettings &settings);

    QList<HighlighterFormat *> cloneFormats() const;
    void reinit(QList<HighlighterFormat *> &formats);

protected:
    QList<HighlighterFormat *> m_formats;
};

class CORELIB_EXPORT HighlighterFormatsManager: public QObject
{
    Q_OBJECT

public:
    HighlighterFormatsManager(QObject *parent = 0);
    virtual ~HighlighterFormatsManager();

    static void registerLanguageHighlighter(HighlighterFormatData *data);
    static HighlighterFormatData *highlighterFormatData(const QString &languageId);

    static HighlighterFormatsConfigureWidget *highlighterFormatsConfigureWidget();
    static QColor defaultBackgroundColor();

    void restore(const QString &configFileName);

private slots:
    void save(const QString &configFileName);

private:
    void registerLanguageHighlighterInternal(HighlighterFormatData *data);
    QMap<QString, HighlighterFormatData *> m_languageFormatMap;
    QList<HighlighterFormatData *> m_languageFormatsSorted;
    HighlighterFormatData *m_generalFormatData;

    static HighlighterFormatsManager *m_instance;
};

#endif // HIGHLIGHTERFORMATSMANAGER_H
