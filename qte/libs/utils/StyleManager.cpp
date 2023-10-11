#include <QFile>
#include <QApplication>
#include <QTimer>

#include <QDebug>

#include "StyleManager.h"

namespace Utils {

StyleManager *StyleManager::m_instance = 0;

ManagedWidgetHolder::ManagedWidgetHolder(QWidget *managedWidget, QObject *parent)
    : QObject(parent)
    , m_managedWidget(managedWidget)
    , m_destroyed(false)
{
    connect(m_managedWidget, SIGNAL(destroyed(QObject*)), this, SLOT(widgetDestroyed()));
}

bool ManagedWidgetHolder::setStyleSheet(const QString &styleSheet)
{
    if ( m_destroyed )
        return false;

    m_managedWidget->setStyleSheet(styleSheet);
    return true;
}

void ManagedWidgetHolder::widgetDestroyed()
{ m_destroyed = true; }


StyleManagedItem::StyleManagedItem(const QString &qssFilePath, QObject *parent)
    : QObject(parent)
    , m_watcher(new QFileSystemWatcher(this))
    , m_qssFilePath(qssFilePath)
    , m_qssFileChanged(false)
    , m_qssContent("")
{
    QFile file(qssFilePath);
    if ( !file.open(QIODevice::ReadOnly) ) {
        qDebug() << "<!QSS!> Cannot open stylesheet file " << m_qssFilePath;
    } else {
        m_qssContent = QString::fromUtf8(file.readAll());
    }

    m_watcher->addPath(qssFilePath);
    connect(m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(qssFileChanged(QString)));
}

StyleManagedItem::~StyleManagedItem()
{
    qDeleteAll(m_managedWidgets);
}

void StyleManagedItem::addWidget(QWidget *w)
{
    ManagedWidgetHolder *wh = new ManagedWidgetHolder(w);
    m_managedWidgets.append(wh);
    wh->setStyleSheet(m_qssContent);
}

void StyleManagedItem::updateStyleSheet()
{
    if ( !m_qssFileChanged )
        return;

    QFile file(m_qssFilePath);
    if ( !file.open(QIODevice::ReadOnly) ) {
        qDebug() << "<!QSS!> Cannot open stylesheet file " << m_qssFilePath;
    } else {
        m_qssContent = QString::fromUtf8(file.readAll());
    }

    int size = m_managedWidgets.size();
    for (int i = 0; i < size; i++) {
        ManagedWidgetHolder *wh = m_managedWidgets.at(i);
        if ( !wh->setStyleSheet(m_qssContent) ) {
            m_managedWidgets.removeAt(i);
            i--;
            size--;
            delete wh;
        }
    }

    m_qssFileChanged = false;
}

void StyleManagedItem::qssFileChanged(const QString &/*filePath*/)
{
    m_qssFileChanged = true;
}

StyleManagedWidget::StyleManagedWidget(QWidget *managedWidget, int watcherIndex)
{
    StyleManager::registerManagedWidget(managedWidget, watcherIndex);
}

StyleManager::StyleManager(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent)
    , m_blockActivatedSignal(false)
{
    m_instance = this;
    m_qssFolder =  QApplication::applicationDirPath() + "/qss/";

    connect(mainWindow, SIGNAL(windowActivated()), this, SLOT(onMainWindowActivated()));
}

StyleManager::~StyleManager()
{
    qDeleteAll(m_items);
}

StyleManager *StyleManager::instance()
{ return m_instance; }

int StyleManager::registerQssFileWatcher(const QString &qssFileName)
{ return m_instance->registerQssFileWatcherInternal(qssFileName); }

void StyleManager::registerManagedWidget(QWidget *managedWidget, int watcherIndex)
{ m_instance->registerManagedWidgetInternal(managedWidget, watcherIndex); }

void StyleManager::onMainWindowActivated()
{
    QTimer::singleShot(0, this, SLOT(updateStyleSheets()));
}

int StyleManager::registerQssFileWatcherInternal(const QString &qssFileName)
{
    QString qssFilePath = m_qssFolder + qssFileName + ".qss";

    QFile qssFile(qssFilePath);
    if ( !qssFile.exists() )
        qDebug() << "<!QSS!> Style file" << qssFilePath << " not exists";

    m_items.append(new StyleManagedItem(qssFilePath));
    return m_items.size() - 1;
}

void StyleManager::registerManagedWidgetInternal(QWidget *managedWidget, int watcherIndex)
{
    if ( watcherIndex < 0 || watcherIndex >= m_items.size() ) {
        qDebug() << "<!QSS!> Invalid watcher index" << watcherIndex;
        return;
    }

    m_items[watcherIndex]->addWidget(managedWidget);
}

void StyleManager::updateStyleSheets()
{
    if ( m_blockActivatedSignal )
        return;

    m_blockActivatedSignal = true; {

        foreach (StyleManagedItem *item, m_items)
            item->updateStyleSheet();

    } m_blockActivatedSignal = false;
}

}   // namespace Utils
