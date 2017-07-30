#ifndef IFAVORITE_H
#define IFAVORITE_H

#include <QByteArray>
#include <QString>
#include <QtPlugin>

#include "favorites_global.h"

class FAVORITES_EXPORT FavoriteData
{
public:
    FavoriteData( const QString &id, const QString &name, const QString &description, QByteArray *streamData );
    virtual ~FavoriteData();

    inline QString id() const { return m_id; }
    inline QString name() const { return m_name; }
    inline QString description() const { return m_description; }
    inline const QByteArray *data() const { return m_streamData; }

private:
    QString m_id;
    QString m_name;
    QString m_description;
    QByteArray *m_streamData;
};

class FAVORITES_EXPORT IFavorite
{
public:
    IFavorite( FavoriteData *data );
    virtual ~IFavorite();

    inline const FavoriteData *data() const { return m_data; }

    virtual void execute() const = 0;
    virtual QString iconName() const = 0;

private:
    FavoriteData *m_data;
};

class NullFavorite: public IFavorite
{
public:
    NullFavorite( const QString &errorDescription, FavoriteData *data );

    virtual void execute() const;
    virtual QString iconName() const;

private:
    QString m_errorDescription;
};

class FAVORITES_EXPORT IFavoriteFactory: public QObject
{
public:
    virtual QString id() const = 0;
    virtual QString description() const = 0;

    IFavorite *createFavorite() const;
    IFavorite *restoreFavorite( FavoriteData *data ) const;

protected:
    virtual IFavorite *doCreateFavorite( FavoriteData *data ) const = 0;
    virtual FavoriteData *doCreateFavoriteData() const = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IFavoriteFactory, "QWorkBase.Favorites.IFavoriteFactory/1.0")
QT_END_NAMESPACE

#endif // IFAVORITE_H
