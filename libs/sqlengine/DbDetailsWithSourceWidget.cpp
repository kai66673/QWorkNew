#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>

#include "DbDetailsWithSourceWidget.h"

namespace Database {

DbDetailsWithSourceWidget::DbDetailsWithSourceWidget( const QStringList &headers,
                                                      const QList<QStringList> &rowData,
                                                      const QStringList &sources,
                                                      QWidget *parent )
    : QWidget(parent)
    , m_sources(sources)
{
    int rowCount = rowData.size();
    int colCount = headers.size();

    QHBoxLayout *l = new QHBoxLayout(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    m_table = new QTableWidget(rowCount, colCount, splitter);
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);
    m_table->setHorizontalHeaderLabels(headers);
    for (int r = 0; r < rowCount; r++)
        for (int c = 0; c < colCount; c++)
            m_table->setItem(r, c, new QTableWidgetItem(rowData[r][c]));

    m_viewSourceWidget = new SqlTextEditorWidget(0, splitter);
    m_viewSourceWidget->initializeAsTextInfoWidget();
    m_viewSourceWidget->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    splitter->addWidget(m_table);
    splitter->addWidget(m_viewSourceWidget);
    l->addWidget(splitter);
    setLayout(l);
    l->setMargin(2);
    l->setSpacing(1);

    connect(m_table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
}

void DbDetailsWithSourceWidget::navigateToItem( IDbCatalogItem *dbItem )
{
    for (int i = 0; i < m_table->rowCount(); i++) {
        if ( !m_table->item(i, 0)->text().compare(dbItem->name()) ) {
            m_table->selectRow(i);
            break;
        }
    }
}

void DbDetailsWithSourceWidget::currentRowChanged( const QModelIndex &current, const QModelIndex &previous )
{
    Q_UNUSED(previous)

    if ( !current.isValid() ) {
        m_viewSourceWidget->setPlainText("");
        return;
    }

    int rowIndex = current.row();
    if ( rowIndex >= 0 && rowIndex < m_sources.size() )
        m_viewSourceWidget->setPlainText(m_sources[rowIndex]);
    else
        m_viewSourceWidget->setPlainText("");
}

DbDetailsWithTableWidget::DbDetailsWithTableWidget( const QStringList &headers,
                                                    const QList<QStringList> &rowData,
                                                    const QStringList &relHeaders,
                                                    const QList<QList<QStringList> > &relRowData,
                                                    QWidget *parent )
    : QWidget(parent)
    , m_relRowData(relRowData)
{
    int rowCount = rowData.size();
    int colCount = headers.size();
    QHBoxLayout *l = new QHBoxLayout(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    m_table = new QTableWidget(rowCount, colCount, splitter);
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);
    m_table->setHorizontalHeaderLabels(headers);
    for (int r = 0; r < rowCount; r++)
        for (int c = 0; c < colCount; c++)
            m_table->setItem(r, c, new QTableWidgetItem(rowData[r][c]));

    m_relTable = new QTableWidget(0, relHeaders.size(), splitter);
    m_relTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_relTable->setHorizontalHeaderLabels(relHeaders);

    splitter->addWidget(m_table);
    splitter->addWidget(m_relTable);
    l->addWidget(splitter);
    setLayout(l);
    l->setMargin(2);
    l->setSpacing(1);

    connect(m_table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
}

void DbDetailsWithTableWidget::navigateToItem( IDbCatalogItem *dbItem )
{
    for (int i = 0; i < m_table->rowCount(); i++) {
        if ( !m_table->item(i, 0)->text().compare(dbItem->name()) ) {
            m_table->selectRow(i);
            break;
        }
    }
}

void DbDetailsWithTableWidget::currentRowChanged( const QModelIndex &current, const QModelIndex &previous )
{
    Q_UNUSED(previous)

    for (int i = m_relTable->rowCount() - 1; i >= 0; i--)
        m_relTable->removeRow(i);

    if ( !current.isValid() )
        return;

    int rowIndex = current.row();
    if ( rowIndex < 0 || rowIndex >= m_relRowData.size() )
        return;


    for (int r = 0; r < m_relRowData[rowIndex].size(); r++) {
        m_relTable->insertRow(r);
        for (int c = 0; c < m_relRowData[rowIndex][r].size(); c++)
            m_relTable->setItem(r, c, new QTableWidgetItem(m_relRowData[rowIndex][r][c]));
    }
}

} // Database
