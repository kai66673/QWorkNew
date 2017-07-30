#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>

#include "corelib_global.h"
#include "mimedatabase.h"
#include "futureprogress.h"
#include "progressmanager.h"
#include "ieditor.h"
#include "completionassistprovider.h"
#include "IPythonModule.h"
#include "ModeManager.h"

class QwCustomDockWidget;

class CORELIB_EXPORT IMainWindow {
    Q_ENUMS(ReportType)

public:
    virtual QMainWindow *widget() = 0;
    virtual QString orgName() const = 0;
    virtual QString appName() const = 0;
    virtual QString applicationDataLocation() const = 0;
    virtual Core::MimeDatabase *mimeDatabase() const = 0;

    virtual void addDockWidgetHandler( Core::DockWidgetHandler *dockHandler ) = 0;
    virtual void addToggleAction( QAction *action ) = 0;

    virtual QMessageBox::StandardButton showInformation( const QString& text,
                                                         QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                         QMessageBox::StandardButton defaultButton = QMessageBox::Ok ) = 0;

    virtual QMessageBox::StandardButton showWarning( const QString& text,
                                                     QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                     QMessageBox::StandardButton defaultButton = QMessageBox::Ok ) = 0;

    virtual bool question( const QString &message ) = 0;

    virtual Core::FutureProgress *addTask( const QFuture<void> &future, const QString &title, const QString &type,
                                           bool signToAppStatusBar = true, bool isSyncTask = false ) = 0;

    virtual Core::ProgressManager *progressManager() const = 0;

    virtual void setNavigationHistoryBlocked( bool onoff = true ) = 0;
    virtual void cutForwardNavigationHistory() = 0;
    virtual void addCurrentPositionToNavigationHistory(Core::IEditor *editor = 0, const QByteArray &saveState = QByteArray()) = 0;

    virtual QMenu *editMenu() = 0;
    virtual QList<TextEditor::CompletionAssistProvider *> getCompletionAssistProviders() const = 0;
    virtual void addCompletionAssistProvider(TextEditor::CompletionAssistProvider *provider) = 0;

    enum ReportType {
        Warning,
        Error
    };

    virtual bool registerModule( Python::IPythonModule *module, bool deleteOnFail = true, const QStringList &classNames = QStringList() ) = 0;
    virtual bool registerBuiltinScript( const QString &scriptId, const QString &scriptSource ) = 0;
    virtual void pythonReport( const QString &message, IMainWindow::ReportType reportType = IMainWindow::Error ) = 0;
    virtual void executeBuiltinScript( const QString &scriptId ) = 0;
    virtual QStringList pythonCodeAssist( const QString &source, int position ) = 0;
    virtual void handleScriptChanged( const QString &scriptName, const QString &source ) = 0;
    virtual QMap<QString, Python::IPythonModule *> *pythonModules() = 0;
};

#endif // IMAINWINDOW_H
