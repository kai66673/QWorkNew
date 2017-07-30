#ifndef __URLFAVORITEFACTORY_H

#define __URLFAVORITEFACTORY_H

#include <QObject>

#include "IFavorite.h"

class UrlFavoriteFactory: public IFavoriteFactory
{
    Q_OBJECT
    Q_INTERFACES(IFavoriteFactory)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.Favorites.IFavoriteFactory/1.0")
#endif

public:
    virtual QString id() const { return QStringLiteral("OpenUrl"); }
    virtual QString description() const { return tr("Open URL"); }

protected:
    virtual IFavorite *doCreateFavorite( FavoriteData *data ) const;
    virtual FavoriteData *doCreateFavoriteData() const;
};

#endif // __URLFAVORITEFACTORY_H
