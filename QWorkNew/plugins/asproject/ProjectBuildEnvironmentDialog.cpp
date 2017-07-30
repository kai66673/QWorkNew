#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>

#include "ProjectBuildEnvironmentDialog.h"
#include "ui_ProjectBuildEnvironmentDialog.h"

#include "Core.h"
#include "utils/qtcprocess.h"

BuildEnvironmentModel::BuildEnvironmentModel( Utils::Environment *environment, QObject *parent )
    : QAbstractTableModel(parent)
    , m_environment(environment)
{
    initSetup();
}

int BuildEnvironmentModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

int BuildEnvironmentModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant BuildEnvironmentModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_data.size() || index.row() < 0)
        return QVariant();

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        switch ( index.column() ) {
            case 0: return m_data[index.row()].first;
            case 1: return m_data[index.row()].second;
        }
    }

    return QVariant();
}

QVariant BuildEnvironmentModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal ) {
        switch (section) {
            case 0: return tr("Varible");
            case 1: return tr("Value");
        }
    }

    return QVariant();
}

Qt::ItemFlags BuildEnvironmentModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool BuildEnvironmentModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( role != Qt::EditRole )
        return false;

    switch (index.column()) {
        case 0: m_data[index.row()].first  = value.toString().trimmed(); break;
        case 1: m_data[index.row()].second = value.toString().trimmed(); break;
    }

    emit dataChanged(index, index);

    return true;
}

bool BuildEnvironmentModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    beginRemoveRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        m_data.removeAt(position);
    endRemoveRows();

    return true;
}

bool BuildEnvironmentModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        m_data.append( qMakePair( QString("<Enter Vaiable Name>"), QString("") ) );
    endInsertRows();

    return true;
}

void BuildEnvironmentModel::onUseSystem()
{
    removeRows(0, rowCount());

    Utils::Environment sysEnv(Utils::QtcProcess::systemEnvironment());
    Utils::Environment::const_iterator it = sysEnv.constBegin();

    beginInsertRows(QModelIndex(), 0, sysEnv.size() - 1);
    while ( it != sysEnv.constEnd() ) {
        m_data.append(qMakePair(it.key(), it.value()));
        ++it;
    }
    endInsertRows();
}

bool BuildEnvironmentModel::saveEnvironmentToDb()
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.open() )
        return false;

    QString queryString = "delete from buildenv";
    QSqlQuery query(db);

    if ( !query.exec(queryString) ) {
        Core::Storage::warning(tr("Clear table BUILDENV failed"));
        return false;
    }

    m_environment->clear();

    for (int i = 0; i < m_data.size(); i++) {
        queryString = "insert into buildenv values (:envkey, :envvalue)";
        query.prepare(queryString);
        query.bindValue(":envkey", m_data[i].first);
        query.bindValue(":envvalue", m_data[i].second);
        if ( query.exec() )
            m_environment->set(m_data[i].first, m_data[i].second);
        else {
            Core::Storage::warning(tr("Insert into BUILDENV failed"));
            return false;
        }
    }

    return true;
}

void BuildEnvironmentModel::onImport( const QString &filePath )
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "ASPDB_ENVIMP");
        db.setDatabaseName(filePath);
        if ( db.open() ) {
            QString queryString = "select * from buildenv";
            QSqlQuery query(db);

            if ( query.exec(queryString) ) {
                int currentRow = 0;
                removeRows(0, rowCount());
                while ( query.next() ) {
                    beginInsertRows(QModelIndex(), currentRow, currentRow);
                    m_data.append(qMakePair(query.value(0).toString(), query.value(1).toString()));
                    endInsertRows();
                    currentRow++;
                }
            }
            else
                Core::Storage::warning(tr("Extract Data From Table BUILDENV Failed"));
        }
        else
            Core::Storage::warning(tr("ASPDB-Database File \'%1\' Cannot be Opened").arg(filePath));
    }
    QSqlDatabase::removeDatabase("ASPDB_ENVIMP");
}

void BuildEnvironmentModel::initSetup()
{
    m_data.clear();
    Utils::Environment::const_iterator it = m_environment->constBegin();
    while ( it != m_environment->constEnd() ) {
        m_data.append(qMakePair(it.key(), it.value()));
        ++it;
    }
}

ProjectBuildEnvironmentDialog::ProjectBuildEnvironmentDialog( Utils::Environment *environment, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ProjectBuildEnvironmentDialog)
    , m_environment(environment)
{
    ui->setupUi(this);

    m_model = new BuildEnvironmentModel(m_environment);
    ui->envTableView->setModel(m_model);
    ui->envTableView->resizeColumnsToContents();
    ui->envTableView->resizeRowsToContents();
    ui->envTableView->horizontalHeader()->setStretchLastSection(true);

    ui->envTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->envTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->addPushButton,          SIGNAL(clicked()), this, SLOT(onAdd()));
    connect(ui->deletePushButton,       SIGNAL(clicked()), this, SLOT(onDelete()));
    connect(ui->clearPushButton,        SIGNAL(clicked()), this, SLOT(onClear()));
    connect(ui->useSystemPushButton,    SIGNAL(clicked()), this, SLOT(onUseSystem()));
    connect(ui->importPushButton,       SIGNAL(clicked()), this, SLOT(onImport()));
}

ProjectBuildEnvironmentDialog::~ProjectBuildEnvironmentDialog()
{
    delete ui;
}

void ProjectBuildEnvironmentDialog::onAdd()
{
    int index = ui->envTableView->currentIndex().row();
    if ( index == -1 ) {
        index = m_model->rowCount();
    }

    m_model->insertRows(index, 1);
}

void ProjectBuildEnvironmentDialog::onDelete()
{
    int index = ui->envTableView->currentIndex().row();
    if ( index == -1 ) {
        return;
    }

    m_model->removeRows(index, 1);
}

void ProjectBuildEnvironmentDialog::onClear()
{
    m_model->removeRows(0, m_model->rowCount());
}

void ProjectBuildEnvironmentDialog::onUseSystem()
{
    m_model->onUseSystem();
}

void ProjectBuildEnvironmentDialog::onImport()
{
    QString filePath = QFileDialog::getOpenFileName( Core::Storage::mainWindow()->widget(),
                                                     tr("Open Project Database"),
                                                     QString(), tr("ASOF Project Database (*.aspdb)") );

    if ( filePath.isEmpty() )
        return;

    m_model->onImport(filePath);
}

void ProjectBuildEnvironmentDialog::accept()
{
    if ( m_model->saveEnvironmentToDb() )
        QDialog::accept();
}

