#include <QStandardPaths>
#include <QMenuBar>

#include "simplemainwindow.h"
#include "Core.h"

SimpleMainWindow::SimpleMainWindow(QWidget *parent):
    QMainWindow(parent)
{
    Core::Storage::registerMainWindow(this);

    m_mimeDatabase = new Core::MimeDatabase;
    m_editMenu = menuBar()->addMenu(tr("&Edit"));
}

SimpleMainWindow::~SimpleMainWindow()
{
    delete m_mimeDatabase;
}

QMainWindow *SimpleMainWindow::widget()
{ return this; }

QString SimpleMainWindow::orgName() const
{ return QLatin1String("QTE"); }

QString SimpleMainWindow::appName() const
{ return QLatin1String("QTE-simple-app"); }

QString SimpleMainWindow::applicationDataLocation() const
{
    QString result = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    return result + "/" + appName();
}

Core::MimeDatabase *SimpleMainWindow::mimeDatabase() const
{ return m_mimeDatabase; }

void SimpleMainWindow::addDockWidgetHandler(Core::DockWidgetHandler *dockHandler)
{ }

void SimpleMainWindow::addToggleAction(QAction *action)
{ }

QMessageBox::StandardButton SimpleMainWindow::showInformation(const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return QMessageBox::information(this, appName(), text, buttons, defaultButton);
}

QMessageBox::StandardButton SimpleMainWindow::showWarning(const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return QMessageBox::warning(this, appName(), text, buttons, defaultButton);
}

bool SimpleMainWindow::question(const QString &message)
{
    return (QMessageBox::Yes == QMessageBox::question(this, appName(), message));
}

Core::FutureProgress *SimpleMainWindow::addTask(const QFuture<void> &future, const QString &title, const QString &type, bool signToAppStatusBar, bool isSyncTask)
{ return nullptr; }

Core::ProgressManager *SimpleMainWindow::progressManager() const
{ return nullptr; }

void SimpleMainWindow::setNavigationHistoryBlocked(bool onoff)
{ }

void SimpleMainWindow::cutForwardNavigationHistory()
{ }

void SimpleMainWindow::addCurrentPositionToNavigationHistory(Core::IEditor *editor, const QByteArray &saveState)
{ }

QMenu *SimpleMainWindow::editMenu()
{
    return m_editMenu;
}

QList<TextEditor::CompletionAssistProvider *> SimpleMainWindow::getCompletionAssistProviders() const
{ return {}; }

void SimpleMainWindow::addCompletionAssistProvider(TextEditor::CompletionAssistProvider *provider)
{ }

bool SimpleMainWindow::registerModule(Python::IPythonModule *module, bool deleteOnFail, const QStringList &classNames)
{ return false; }

bool SimpleMainWindow::registerBuiltinScript(const QString &scriptId, const QString &scriptSource)
{ return false; }

void SimpleMainWindow::pythonReport(const QString &message, ReportType reportType)
{ }

void SimpleMainWindow::executeBuiltinScript(const QString &scriptId)
{ }

QStringList SimpleMainWindow::pythonCodeAssist(const QString &source, int position)
{ return {}; }

void SimpleMainWindow::handleScriptChanged(const QString &scriptName, const QString &source)
{ }

QMap<QString, Python::IPythonModule *> *SimpleMainWindow::pythonModules()
{ return {}; }
