#ifndef LOCATORMANAGER_H
#define LOCATORMANAGER_H

#include <QObject>

#include "ilocatorfilter.h"
#include "find_global.h"

class FIND_EXPORT LocatorManager : public QObject
{
    Q_OBJECT

public:
    explicit LocatorManager(QObject *parent = 0);
    virtual ~LocatorManager();

    static LocatorManager *instance();

    void registerFilter(Core::ILocatorFilter *filter);
    QList<Core::ILocatorFilter *> filters();

    template <typename T> T *filter() {
        foreach ( Core::ILocatorFilter *f, m_filters )
            if ( T *t = qobject_cast<T *>(f) )
                return t;
        return 0;
    }

signals:

public slots:

private:
    static LocatorManager *m_instance;
    QList<Core::ILocatorFilter *> m_filters;
};

#endif // LOCATORMANAGER_H
