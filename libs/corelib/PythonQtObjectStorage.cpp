#include "PythonQtObjectStorage.h"
#include "Core.h"

namespace Core {

PythonQtObject::PythonQtObject(PythonQtStorageDestroyDelegate *delegate, QObject *qtObject)
    : QObject()
    , m_qtObject(qtObject)
    , m_delegate(delegate)
{
    connect(m_qtObject, SIGNAL(destroyed()), this, SLOT(qtObjectDestroyed()));
}

void PythonQtObject::qtObjectDestroyed()
{
    m_delegate->qtObjectDestroyed(m_qtObject);
    m_qtObject = 0;
    clear();
}

bool PythonQtObject::isValid(bool showMessage) const
{
    if ( !m_qtObject ) {
        if ( showMessage )
            Core::Storage::mainWindow()->pythonReport(tr("Object No Longer Valid"));
        return false;
    }

    return true;
}

void PythonQtObject::clear()
{ }

}   // namespace Core
