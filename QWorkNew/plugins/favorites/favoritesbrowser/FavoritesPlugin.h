#ifndef FAVORITESPLUGIN_H
#define FAVORITESPLUGIN_H

#include <QObject>

#include "IPlugin.h"
#include "ISessionClient.h"
#include "CommandManager.h"
#include "CloseApplicationHandler.h"

#include "FavoriteModel.h"
#include "FavoriteFactoriesManager.h"

class FavoritesPlugin : public IPlugin, public ISessionClient, public ICloseApplicationClient
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IPlugin/1.0")
#endif

public:
    FavoritesPlugin();
    virtual ~FavoritesPlugin();

    virtual QString id() const { return QStringLiteral("Favorites"); }

    virtual void initialize();

    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

    // ISessionClient implementation
    virtual bool sessionAboutToClose() { return true; }
    virtual void sessionClose();
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

    // ICloseApplicationClient implementation
    virtual bool applicationAboutToClose() { return true; }
    virtual void applicationClose();
    virtual void destroy();

    IFavoriteFactory *favoriteFactoryForId( const QString &id ) const;
    IFavoriteFactory *selectFavoriteFactory() const;

private slots:
    void currentRowChanged( const QModelIndex &current, const QModelIndex &previous );

protected:
    virtual QStringList settingsKeyList() const { return QStringList(); }
    virtual ISettings *createSettings( const QString &settingsKey ) const { Q_UNUSED(settingsKey); return NULL; }

private:
    FavoriteModel *m_model;
    FavoriteFactoriesManager *m_favoriteFactoryManager;

    void createActions();
    Core::Command *m_cmdExpandAll;
    Core::Command *m_cmdCollapseAll;
    Core::Command *m_cmdAddGroup;
    Core::Command *m_cmdAddItem;
    Core::Command *m_cmdDelete;
};

#endif // FAVORITESPLUGIN_H
