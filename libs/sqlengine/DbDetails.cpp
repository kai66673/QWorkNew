#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QSplitter>
#include <QHBoxLayout>
#include <QDebug>

#include "DbDetails.h"
#include "DbCatalog.h"
#include "SqlTextEditorWidget.h"
#include "DbDetailsWithSourceWidget.h"
#include "QwDockWidget.h"

namespace Database {

DetailsTabWidget::DetailsTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , IDetailsNavigator()
{
    setTabBar(new QwTabBar(this));
}

void DetailsTabWidget::navigateToItem( IDbCatalogItem *item )
{
    int index = item->schemaObjectSubtype();
    if (index >= -1 && index < count() - 1 ) {
        setCurrentIndex(index + 1);
        if ( index >= 0 )
            if ( IDetailsNavigator *navigator = dynamic_cast<IDetailsNavigator *>(widget(index + 1)) )
                navigator->navigateToItem(item);
    }
}

void DetailsTableWidget::navigateToItem( IDbCatalogItem *dbItem )
{
    for (int i = 0; i < rowCount(); i++) {
        if ( !item(i, 0)->text().compare(dbItem->name(), Qt::CaseInsensitive) ) {
            selectionModel()->clear();
            selectRow(i);
            break;
        }
    }
}

DbDetailsTabbedFactory::~DbDetailsTabbedFactory()
{
    qDeleteAll(m_tabs);
}

void DbDetailsTabbedFactory::addTab( const QString &name, IDbDetailsFactory *tab )
{
    m_tabs.append(tab);
    m_tabNames << name;
}

QWidget *DbDetailsTabbedFactory::createDetailsWidget( QWidget *parent ) const
{
    DetailsTabWidget *t = new DetailsTabWidget(parent);

    for (int i = 0; i < m_tabs.size(); i++)
        t->addTab(m_tabs[i]->createDetailsWidget(t), m_tabNames[i]);

    return t;
}

QWidget *DbDetailsSourceFactory::createDetailsWidget( QWidget *parent ) const
{
    SqlTextEditorWidget *viewSourceWidget = new SqlTextEditorWidget(0, parent);
    viewSourceWidget->initializeAsTextInfoWidget();
    viewSourceWidget->setFrameShape(QFrame::WinPanel);
    viewSourceWidget->setFrameShadow(QFrame::Sunken);
    viewSourceWidget->setPlainText(m_source);
    return viewSourceWidget;
}

QWidget *DbDetailsTableFactory::createDetailsWidget( QWidget *parent ) const
{
    int rowCount = m_data.size();
    int colCount = m_headers.size();

    DetailsTableWidget *t = new DetailsTableWidget(rowCount, colCount, parent);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setEditTriggers(QTableWidget::NoEditTriggers);
    t->setHorizontalHeaderLabels(m_headers);
    for (int r = 0; r < rowCount; r++)
        for (int c = 0; c < colCount; c++)
            t->setItem(r, c, new QTableWidgetItem(m_data[r][c]));

    t->setFrameShape(QFrame::NoFrame);
    t->setLineWidth(0);
    return t;
}

void DbDetailsTableFactory::addRow( const QStringList &rowData )
{
    if ( rowData.size() == m_headers.size() )
        m_data << rowData;
}

QWidget *DbDetailsFormViewFactory::createDetailsWidget( QWidget *parent ) const
{
    QWidget *w = new QWidget(parent);
    QFormLayout *f = new QFormLayout(w);
    foreach ( const FormItem &item, m_items ) {
        if ( item.itemType == NormalText ) {
            QLineEdit *lw = new QLineEdit(item.text, w);
            lw->setReadOnly(true);
            f->addRow(item.label + ":", lw);
        }
        else {
            SqlTextEditorWidget *viewSourceWidget = new SqlTextEditorWidget(0, w);
            viewSourceWidget->initializeAsTextInfoWidget();
            viewSourceWidget->setFrameShape(QFrame::WinPanel);
            viewSourceWidget->setFrameShadow(QFrame::Sunken);
            QSizePolicy sizePolicy = viewSourceWidget->sizePolicy();
            sizePolicy.setVerticalStretch(1);
            viewSourceWidget->setSizePolicy(sizePolicy);
            viewSourceWidget->setPlainText(item.text);
            f->addRow(item.label + ":", viewSourceWidget);
        }
    }

    w->setLayout(f);
    return w;
}

QWidget *DbDetailsTableWithSourceFactory::createDetailsWidget( QWidget *parent ) const
{
    DbDetailsWithSourceWidget *w = new DbDetailsWithSourceWidget(m_headers, m_data, m_sources, parent);
    return w;
}

void DbDetailsTableWithSourceFactory::addRow( const QStringList &rowData, const QString &source )
{
    if ( rowData.size() == m_headers.size() ) {
        m_data << rowData;
        m_sources << source;
    }
}

QWidget *DbDetailsTableWithTableFactory::createDetailsWidget( QWidget *parent ) const
{
    DbDetailsWithTableWidget *w = new DbDetailsWithTableWidget(m_headers, m_data, m_relHeaders, m_relData, parent);
    return w;
}

void DbDetailsTableWithTableFactory::addRow(const QStringList &rowData, const QList<QStringList> &rowRelData)
{
    if ( rowData.size() == m_headers.size() ) {
        int i;
        for (i = 0; i < rowRelData.size(); i++)
            if ( rowRelData[i].size() != m_relHeaders.size() )
                return;
        m_data << rowData;
        m_relData << rowRelData;
    }
}

} // Database
