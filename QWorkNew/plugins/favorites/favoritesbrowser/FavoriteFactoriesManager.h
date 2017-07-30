#ifndef FAVORITEFACTORIESMANAGER_H
#define FAVORITEFACTORIESMANAGER_H

#include <QMap>

#include "IFavorite.h"

class FavoriteFactoriesManager
{
public:
    FavoriteFactoriesManager();
    virtual ~FavoriteFactoriesManager();

    inline int count() const { return m_factories.size(); }
    IFavoriteFactory *operator [] ( const QString &id );

    void loadFactoryPlugins();
    IFavoriteFactory *selectFavoriteFactory() const;

private:
    QMap<QString, IFavoriteFactory *> m_factories;
};

class OpenFileFavoriteFactory: public IFavoriteFactory
{
public:
    virtual QString id() const { return QStringLiteral("OpenFile"); }
    virtual QString description() const { return QStringLiteral("Add Current Document to Favorites"); }

protected:
    virtual IFavorite *doCreateFavorite( FavoriteData *data ) const;
    virtual FavoriteData *doCreateFavoriteData() const;
};

class SelectOpenFileFavoriteFactory: public IFavoriteFactory
{
public:
    virtual QString id() const { return QStringLiteral("SelectOpenFile"); }
    virtual QString description() const { return QStringLiteral("Add Selected File to Favorites"); }

protected:
    virtual IFavorite *doCreateFavorite( FavoriteData *data ) const;
    virtual FavoriteData *doCreateFavoriteData() const;
};

#endif // FAVORITEFACTORIESMANAGER_H
