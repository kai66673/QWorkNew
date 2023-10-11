#ifndef QRECENTFILESMENU_H
#define QRECENTFILESMENU_H

#include <QMenu>
#include <QAction>

#include "utils_global.h"

class RecentFilesAction: public QAction
{
    Q_OBJECT

public:
    RecentFilesAction( const QString &filePath, QObject *parent = 0 )
        : QAction(filePath, parent)
        , m_filePath(filePath)
    {}

    QString filePath() const { return m_filePath; }

private:
    QString m_filePath;
};

class QTCREATOR_UTILS_EXPORT IRecentFilesDelegate
{
public:
    virtual ~IRecentFilesDelegate() {}
    virtual void openFile( const QString &fileName ) const = 0;
    virtual QString menuName() const = 0;
    virtual QString settingName() const  = 0;
};

class QTCREATOR_UTILS_EXPORT RecentFilesMenu : public QMenu
{
    Q_OBJECT

public:
    explicit RecentFilesMenu( const QString &orgName, const QString &appName, IRecentFilesDelegate *delegete, int maxItems = 20, QWidget *parent = 0 );
    virtual ~RecentFilesMenu();

    void appendItem( const QString &fileName );
    void updateItem( const QString &oldFileName, const QString &newFileName );

    void setEnabledAllItems( bool onoff );
    
signals:
    
public slots:
    void openRecentFile();
    
private:
    void saveSettings();
    void loadSettings();

    IRecentFilesDelegate   *m_delegeate;
    int                     m_maxItems;
    QList<QString>          m_fileNames;

    QString                 m_orgName;
    QString                 m_appName;
};

#endif // QRECENTFILESMENU_H
