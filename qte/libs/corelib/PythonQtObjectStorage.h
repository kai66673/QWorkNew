#ifndef PYTHONQTOBJECTSTORAGE_H
#define PYTHONQTOBJECTSTORAGE_H

#include <QObject>

#include "corelib_global.h"

namespace Core {

class CORELIB_EXPORT PythonQtStorageDestroyDelegate
{
public:
    virtual void qtObjectDestroyed(QObject *qtObject) = 0;
};

class CORELIB_EXPORT PythonQtObject: public QObject
{
    Q_OBJECT

public:
    PythonQtObject(PythonQtStorageDestroyDelegate *delegate, QObject *qtObject);

private slots:
    void qtObjectDestroyed();

protected:
    bool isValid(bool showMessage = true) const;
    virtual void clear();
    QObject *m_qtObject;
    PythonQtStorageDestroyDelegate *m_delegate;
};

template <class Q, class Py> class PythonQtObjectStorage: public PythonQtStorageDestroyDelegate
{
public:
    PythonQtObjectStorage()
        : PythonQtStorageDestroyDelegate()
    { }

    virtual ~PythonQtObjectStorage() {
        qDeleteAll(m_pythonQtObjectsMap.values());
        qDeleteAll(m_pythonQtObjectsDestroyed);
    }

    void qtObjectDestroyed(QObject *qtObject) {
        Q *obj = qobject_cast<Q *>(qtObject);
//        QMap<Q *, Py *>::iterator it = m_pythonQtObjectsMap.find(obj);
        if ( m_pythonQtObjectsMap.find(obj) == m_pythonQtObjectsMap.end() )
            return;

        Py *pyObject = m_pythonQtObjectsMap[obj];
        m_pythonQtObjectsMap.remove(obj);
        m_pythonQtObjectsDestroyed << pyObject;
    }

    Py *findOrCreatePythonQtObject(Q *qtObject) {
//        QMap<Q *, Py *>::iterator it = m_pythonQtObjectsMap.find(qtObject);
        if ( m_pythonQtObjectsMap.find(qtObject) != m_pythonQtObjectsMap.end() )
            return m_pythonQtObjectsMap[qtObject];
        Py *pyObject = new Py(this, qtObject);
        m_pythonQtObjectsMap[qtObject] = pyObject;
        return pyObject;
    }

private:
    QMap<Q *, Py *> m_pythonQtObjectsMap;
    QList<Py *> m_pythonQtObjectsDestroyed;
};

}   // namespace Core

#endif // PYTHONQTOBJECTSTORAGE_H
