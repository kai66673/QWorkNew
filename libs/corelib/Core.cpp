#include "Core.h"
#include "qtcassert.h"

namespace Core {

IMainWindow *Storage::m_mainWindow = 0;

void Storage::registerMainWindow( IMainWindow *mainWindow )
{
    QTC_ASSERT(!m_mainWindow || mainWindow, return);
    m_mainWindow = mainWindow;
}

IMainWindow *Storage::mainWindow()
{
    QTC_ASSERT(m_mainWindow, return 0);
    return m_mainWindow;
}

QMessageBox::StandardButton Storage::warning( const QString &text,
                                              QMessageBox::StandardButtons buttons,
                                              QMessageBox::StandardButton defaultButton )
{
    QTC_ASSERT(m_mainWindow, return QMessageBox::Cancel);
    return m_mainWindow->showWarning(text, buttons, defaultButton);
}

QMessageBox::StandardButton Storage::information( const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton )
{
    QTC_ASSERT(m_mainWindow, return QMessageBox::Cancel);
    return m_mainWindow->showInformation(text, buttons, defaultButton);
}

bool Storage::question(const QString &message)
{
    QTC_ASSERT(m_mainWindow, return false);
    return m_mainWindow->question(message);
}

}   // namespace Core
