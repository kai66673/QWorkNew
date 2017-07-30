#ifndef DBDETAILS_H
#define DBDETAILS_H

#include <QWidget>
#include <QList>
#include <QStringList>
#include <QTabWidget>
#include <QTableWidget>

#include "sqlengine_global.h"

namespace Database {

class IDbCatalogItem;

class SQL_EXPORT IDetailsNavigator
{
public:
    virtual void navigateToItem( IDbCatalogItem *item ) = 0;
};

class SQL_EXPORT DetailsTabWidget: public QTabWidget, public IDetailsNavigator
{
public:
    DetailsTabWidget( QWidget *parent );

    virtual void navigateToItem( IDbCatalogItem *item );
};

class SQL_EXPORT DetailsTableWidget: public QTableWidget, public IDetailsNavigator
{
public:
    DetailsTableWidget( int rowCount, int colCount, QWidget *parent )
        : QTableWidget(rowCount, colCount, parent)
        , IDetailsNavigator()
    {}

    virtual void navigateToItem( IDbCatalogItem *item );
};

class SQL_EXPORT IDbDetailsFactory
{
public:
    virtual ~IDbDetailsFactory() {}
    virtual QWidget *createDetailsWidget( QWidget *parent = 0 ) const = 0;
};

class SQL_EXPORT DbDetailsEmptyFactory: public IDbDetailsFactory
{
    virtual QWidget *createDetailsWidget( QWidget *parent = 0 ) const { return new QWidget(parent); }
};

class SQL_EXPORT DbDetailsTabbedFactory: public IDbDetailsFactory
{
public:
    DbDetailsTabbedFactory()
        : IDbDetailsFactory()
    {}

    virtual ~DbDetailsTabbedFactory();

    void addTab( const QString &name, IDbDetailsFactory *tab );

    virtual QWidget *createDetailsWidget( QWidget *parent = 0 ) const;

private:
    QList<IDbDetailsFactory *> m_tabs;
    QStringList m_tabNames;
};

class SQL_EXPORT DbDetailsSourceFactory: public IDbDetailsFactory
{
public:
    DbDetailsSourceFactory( const QString &source )
        : IDbDetailsFactory()
        , m_source(source)
    {}

    virtual QWidget *createDetailsWidget( QWidget *parent = 0 ) const;

private:
    QString m_source;
};

class SQL_EXPORT DbDetailsTableFactory: public IDbDetailsFactory
{
public:
    DbDetailsTableFactory( const QStringList &headers )
        : IDbDetailsFactory()
        , m_headers(headers)
    {}

    virtual QWidget *createDetailsWidget( QWidget *parent ) const;

    void addRow( const QStringList &rowData );

private:
    QStringList m_headers;
    QList<QStringList> m_data;
};

class SQL_EXPORT DbDetailsTableWithSourceFactory: public IDbDetailsFactory
{
public:
    DbDetailsTableWithSourceFactory( const QStringList &headers )
        : IDbDetailsFactory()
        , m_headers(headers)
    {}

    virtual QWidget *createDetailsWidget( QWidget *parent ) const;

    void addRow( const QStringList &rowData, const QString &source );

private:
    QStringList m_headers;
    QList<QStringList> m_data;
    QStringList m_sources;
};

class SQL_EXPORT DbDetailsTableWithTableFactory: public IDbDetailsFactory
{
public:
    DbDetailsTableWithTableFactory( const QStringList &headers, const QStringList &relHeaders )
        : IDbDetailsFactory()
        , m_headers(headers)
        , m_relHeaders(relHeaders)
    {}

    virtual QWidget *createDetailsWidget( QWidget *parent ) const;

    void addRow( const QStringList &rowData, const QList<QStringList> &rowRelData );

private:
    QStringList m_headers;
    QList<QStringList> m_data;
    QStringList m_relHeaders;
    QList<QList<QStringList> > m_relData;
};

class SQL_EXPORT DbDetailsFormViewFactory: public IDbDetailsFactory
{
public:
    enum FormItemType {
        NormalText = 0,
        SourceText
    };

    struct FormItem {
        FormItem( const QString &label_, const QString &text_, FormItemType itemType_ = NormalText )
            : label(label_)
            , text(text_)
            , itemType(itemType_)
        {}

        QString label;
        QString text;
        FormItemType itemType;
    };

    virtual QWidget *createDetailsWidget( QWidget *parent ) const;

    void addItem( const FormItem &item ) { m_items.append(item); }

private:
    QList<FormItem> m_items;
};

} // Database

#endif // DBDETAILS_H
