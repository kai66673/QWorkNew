#ifndef CORE_H
#define CORE_H

#include <QMessageBox>

#include "corelib_global.h"
#include "IMainWindow.h"

namespace Core {

class CORELIB_EXPORT Storage
{
public:
    // IMainWindow
    static void registerMainWindow( IMainWindow *mainWindow );
    static IMainWindow *mainWindow();
    static QMessageBox::StandardButton warning( const QString& text,
                                                QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                QMessageBox::StandardButton defaultButton = QMessageBox::Ok );
    static QMessageBox::StandardButton information( const QString& text,
                                                    QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                    QMessageBox::StandardButton defaultButton = QMessageBox::Ok );
    static bool question( const QString &message );

private:
    static IMainWindow *m_mainWindow;
};

}   // namespace Core

#endif // CORE_H
