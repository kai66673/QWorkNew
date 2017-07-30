#include <QDesktopServices>
#include <QUrl>

#include "Core.h"
#include "UrlFavoriteFactory.h"
#include "NewUrlDialog.h"

class UrlFavorite: public IFavorite
{
public:
    UrlFavorite( FavoriteData *data )
        : IFavorite(data)
    { }

    virtual void execute() const {
        QDesktopServices::openUrl(QUrl(data()->description()));
    }
    virtual QString iconName() const { return QStringLiteral(":/images/fav_open_url.png"); }
};

IFavorite *UrlFavoriteFactory::doCreateFavorite( FavoriteData *data ) const
{
    return new UrlFavorite(data);
}

FavoriteData *UrlFavoriteFactory::doCreateFavoriteData() const
{
    NewUrlDialog urlDialog(Core::Storage::mainWindow()->widget());
    if ( urlDialog.exec() == QDialog::Accepted )
        return urlDialog.favoriteData(id());

    return 0;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(UrlFavoriteFactory, UrlFavoriteFactory)
#endif

