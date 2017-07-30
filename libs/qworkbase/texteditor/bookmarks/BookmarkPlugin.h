#ifndef QBOOKMARKMANAGER_H
#define QBOOKMARKMANAGER_H

#include "IPlugin.h"
#include "bookmarkmanager.h"
#include "ISessionClient.h"
#include "CommandManager.h"

QT_BEGIN_NAMESPACE
class QDockWidget;
QT_END_NAMESPACE

class BookmarkPlugin;

class BookmarkPlugin : public IPlugin, public ISessionClient
{
    Q_OBJECT

public:
    BookmarkPlugin();
    virtual ~BookmarkPlugin();

    virtual QString id() const { return QStringLiteral("BookmarkManager"); }
    virtual QMenu *menu();

    QList <QAction *> actions();

    virtual void initialize();

    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

    // QISessionClient implementaion
    virtual bool sessionAboutToClose();
    virtual void sessionClose();
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

protected:
    virtual QStringList settingsKeyList() const { return QStringList(); }
    virtual ISettings *createSettings( const QString &settingsKey ) const { Q_UNUSED(settingsKey); return NULL; }

private slots:
    void onToggleBookmark();

private:
    void createActions();

    Core::Command *m_cmdToggle;
    Core::Command *m_cmdNext;
    Core::Command *m_cmdPrev;
    Core::Command *m_cmdNextInDoc;
    Core::Command *m_cmdPrevInDoc;

    Bookmarks::Internal::BookmarkManager *m_manager;
    QwCustomDockWidget *m_bookmarksDock;
};

#endif // QBOOKMARKMANAGER_H
