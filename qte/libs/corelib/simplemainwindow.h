#ifndef SIMPLEMAINWINDOW_H
#define SIMPLEMAINWINDOW_H

#include "IMainWindow.h"

class CORELIB_EXPORT SimpleMainWindow : public QMainWindow, public IMainWindow
{
public:
    SimpleMainWindow(QWidget *parent = nullptr);
    virtual ~SimpleMainWindow();

    // IMainWindow interface
public:
    virtual QMainWindow *widget() override;
    virtual QString orgName() const override;
    virtual QString appName() const override;
    virtual QString applicationDataLocation() const override;
    virtual Core::MimeDatabase *mimeDatabase() const override;
    virtual void addDockWidgetHandler(Core::DockWidgetHandler *dockHandler) override;
    virtual void addToggleAction(QAction *action) override;
    virtual QMessageBox::StandardButton showInformation(const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton) override;
    virtual QMessageBox::StandardButton showWarning(const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton) override;
    virtual bool question(const QString &message) override;
    virtual Core::FutureProgress *addTask(const QFuture<void> &future, const QString &title, const QString &type, bool signToAppStatusBar, bool isSyncTask) override;
    virtual Core::ProgressManager *progressManager() const override;
    virtual void setNavigationHistoryBlocked(bool onoff) override;
    virtual void cutForwardNavigationHistory() override;
    virtual void addCurrentPositionToNavigationHistory(Core::IEditor *editor, const QByteArray &saveState) override;
    virtual QMenu *editMenu() override;
    virtual QList<TextEditor::CompletionAssistProvider *> getCompletionAssistProviders() const override;
    virtual void addCompletionAssistProvider(TextEditor::CompletionAssistProvider *provider) override;
    virtual bool registerModule(Python::IPythonModule *module, bool deleteOnFail, const QStringList &classNames) override;
    virtual bool registerBuiltinScript(const QString &scriptId, const QString &scriptSource) override;
    virtual void pythonReport(const QString &message, ReportType reportType) override;
    virtual void executeBuiltinScript(const QString &scriptId) override;
    virtual QStringList pythonCodeAssist(const QString &source, int position) override;
    virtual void handleScriptChanged(const QString &scriptName, const QString &source) override;
    virtual QMap<QString, Python::IPythonModule *> *pythonModules() override;

private:
    Core::MimeDatabase *m_mimeDatabase;
    QMenu *m_editMenu;
};


#endif // SIMPLEMAINWINDOW_H
