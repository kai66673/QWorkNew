#ifndef DBDETAILSWITHSOURCEWIDGET_H
#define DBDETAILSWITHSOURCEWIDGET_H

#include <QWidget>
#include <QList>
#include <QStringList>
#include <QModelIndex>

#include "SqlTextEditorWidget.h"
#include "DbDetails.h"
#include "DbCatalog.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

namespace Database {

class DbDetailsWithSourceWidget : public QWidget, public IDetailsNavigator
{
    Q_OBJECT

public:
    explicit DbDetailsWithSourceWidget( const QStringList &headers,
                                        const QList<QStringList> &rowData,
                                        const QStringList &sources,
                                        QWidget *parent = 0 );

    virtual void navigateToItem( IDbCatalogItem *item );
    
signals:
    
public slots:
    void currentRowChanged( const QModelIndex &current, const QModelIndex &previous );
    
private:
    QStringList m_sources;
    SqlTextEditorWidget *m_viewSourceWidget;
    QTableWidget *m_table;
};

class DbDetailsWithTableWidget: public QWidget, public IDetailsNavigator
{
    Q_OBJECT

public:
    explicit DbDetailsWithTableWidget( const QStringList &headers,
                                       const QList<QStringList> &rowData,
                                       const QStringList &relHeaders,
                                       const QList<QList<QStringList> > &relRowData ,
                                       QWidget *parent = 0 );

    virtual void navigateToItem( IDbCatalogItem *item );

public slots:
    void currentRowChanged( const QModelIndex &current, const QModelIndex &previous );

private:
    QList<QList<QStringList> > m_relRowData;
    QTableWidget *m_relTable;
    QTableWidget *m_table;
};

} // Database

#endif // DBDETAILSWITHSOURCEWIDGET_H
