#include "Core.h"
#include "IFavorite.h"

FavoriteData::FavoriteData( const QString &id, const QString &name, const QString &description, QByteArray *streamData )
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_streamData(streamData)
{
}

FavoriteData::~FavoriteData()
{
    if ( m_streamData )
        delete m_streamData;
}

IFavorite::IFavorite( FavoriteData *data )
    : m_data(data)
{
}

IFavorite::~IFavorite()
{
    if ( m_data )
        delete m_data;
}

NullFavorite::NullFavorite( const QString &errorDescription , FavoriteData *data )
    : IFavorite(data)
    , m_errorDescription(errorDescription)
{
}

void NullFavorite::execute() const
{
    Core::Storage::warning(m_errorDescription);
}

QString NullFavorite::iconName() const
{
    return QStringLiteral(":/images/fav_null_item.png");
}

IFavorite *IFavoriteFactory::createFavorite() const
{
    return restoreFavorite(doCreateFavoriteData());
}

IFavorite *IFavoriteFactory::restoreFavorite( FavoriteData *data ) const
{
    if ( !data )
        return 0;

    if ( IFavorite *f = doCreateFavorite(data) )
        return f;

    delete data;
    return new NullFavorite(QObject::tr("There are Errors on Creation Favorite Content.\nFavorite Item Cannon be Executed!"), data);
}
