#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QList>
#include <QFileSystemWatcher>
#include <QMainWindow>

#include "utils_global.h"

namespace Utils {

class ManagedWidgetHolder: public QObject
{
    Q_OBJECT

public:
    ManagedWidgetHolder(QWidget *managedWidget, QObject *parent = 0);
    bool setStyleSheet(const QString &styleSheet);

private slots:
    void widgetDestroyed();

private:
    QWidget *m_managedWidget;
    bool m_destroyed;
};

class StyleManagedItem: public QObject
{
    Q_OBJECT

public:
    StyleManagedItem(const QString &qssFilePath, QObject *parent = 0);
    virtual ~StyleManagedItem();

    void addWidget(QWidget *w);
    void updateStyleSheet();

private slots:
    void qssFileChanged(const QString &);

private:
    QList<ManagedWidgetHolder *> m_managedWidgets;
    QFileSystemWatcher *m_watcher;
    QString m_qssFilePath;

    bool m_qssFileChanged;
    QString m_qssContent;
};

class QTCREATOR_UTILS_EXPORT StyleManagedWidget
{
public:
    StyleManagedWidget(QWidget *managedWidget, int watcherIndex);
};

class QTCREATOR_UTILS_EXPORT StyleManager: public QObject
{
    Q_OBJECT

public:
    StyleManager(QMainWindow *mainWindow, QObject *parent = 0);
    virtual ~StyleManager();

    static StyleManager *instance();
    static int registerQssFileWatcher(const QString &qssFileName);
    static void registerManagedWidget(QWidget *managedWidget, int watcherIndex);

private slots:
    void onMainWindowActivated();
    void updateStyleSheets();

private:
    int registerQssFileWatcherInternal(const QString &qssFileName);
    void registerManagedWidgetInternal(QWidget *managedWidget, int watcherIndex);


    static StyleManager *m_instance;
    QList<StyleManagedItem *> m_items;

    QString m_qssFolder;
    bool m_blockActivatedSignal;
};

}   // namespace Utils

#endif // STYLEMANAGER_H
