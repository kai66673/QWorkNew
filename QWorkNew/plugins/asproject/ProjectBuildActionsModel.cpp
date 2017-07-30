#include <QSqlDatabase>
#include <QSqlQuery>

#include <QDebug>

#include "ProjectBuildActionsModel.h"

void ProjectBuildAction::recreateAction()
{
    if ( m_action )
        delete m_action;

    m_action = new QAction(m_commandName, 0);
}

ProjectBuildActionsModel::ProjectBuildActionsModel( ProjectBuildActionsManager *manager, QObject *parent )
    : QAbstractTableModel(parent)
    , m_manager(manager)
{
    connect(m_manager, SIGNAL(buildActionStarted()), this, SLOT(onBuildActionStarted()));
    connect(m_manager, SIGNAL(buildActionFinished()), this, SLOT(onBuildActionFinished()));
}

int ProjectBuildActionsModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return m_projectBuildActions.size();
}

int ProjectBuildActionsModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant ProjectBuildActionsModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_projectBuildActions.size() || index.row() < 0)
        return QVariant();

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        switch ( index.column() ) {
            case 0: return m_projectBuildActions[index.row()].m_commandName;
            case 1: return m_projectBuildActions[index.row()].m_command;
            case 2: return m_projectBuildActions[index.row()].m_commandArgs;
            case 3: return m_projectBuildActions[index.row()].m_commandWorkDir;
            case 4: {
                switch ( m_projectBuildActions[index.row()].m_priorAction ) {
                    case ProjectBuildAction::AskSaveDocuments: return tr("Ask for Saving Unsaved Documents");
                    case ProjectBuildAction::AutoSaveDocuments: return tr("Autosave All Unsaved Documents");
                    default: return tr("No Actions");
                }
        }
        }
    }

    return QVariant();
}

QVariant ProjectBuildActionsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return tr("Command Name");
            case 1: return tr("Executable File Path");
            case 2: return tr("Arguments");
            case 3: return tr("Executable Working Directory");
            case 4: return tr("Prior Action");
        }
    }

    return QVariant();
}

Qt::ItemFlags ProjectBuildActionsModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

bool ProjectBuildActionsModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    beginRemoveRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        m_projectBuildActions.removeAt(position);
    endRemoveRows();

    return true;
}

bool ProjectBuildActionsModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        m_projectBuildActions.append(ProjectBuildAction());
    endInsertRows();

    return true;
}

void ProjectBuildActionsModel::emitRowChanged( int rowIndex )
{
    QModelIndex startIndex  = index(rowIndex, 0);
    QModelIndex endIndex    = index(rowIndex, 1);
    emit(dataChanged(startIndex, endIndex));
}

QList<QAction *> ProjectBuildActionsModel::projectBuildActions() const
{
    QList<QAction *> actions;
    for (int i = 0; i < m_projectBuildActions.size(); i++)
        actions << m_projectBuildActions[i].m_action;
    return actions;
}

void ProjectBuildActionsModel::readFromDb()
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    m_projectBuildActions.clear();

    QString queryString;
    QSqlQuery query(db);

    queryString = "select action_name, exe_file_path, work_dir, prior_action, exe_args from buildaction order by idbuildaction";
    query.exec(queryString);
    int i = 0;
    while ( query.next() ) {
        m_projectBuildActions.append( ProjectBuildAction( query.value(0).toString(),
                                                          query.value(1).toString(),
                                                          query.value(4).toString(),
                                                          query.value(2).toString(),
                                                          fromInt(query.value(3).toInt())) );
        m_projectBuildActions[i].recreateAction();
        connect(m_projectBuildActions[i].m_action, SIGNAL(triggered()), this, SLOT(onBuildAction()));
        i++;
    }
}

void ProjectBuildActionsModel::writeToDb()
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    QString queryString;
    QSqlQuery query(db);

    queryString = "delete from buildaction";
    query.exec(queryString);

    for (int i = 0; i < m_projectBuildActions.size(); i++) {
        queryString = "insert into buildaction values (:id, :commandName, :commandFileName, :commandWorkDir, :priorAction, :exeArgs)";
        query.prepare(queryString);
        query.bindValue(":id", i + 1);
        query.bindValue(":commandName", m_projectBuildActions[i].m_commandName);
        query.bindValue(":commandFileName", m_projectBuildActions[i].m_command);
        query.bindValue(":commandWorkDir", m_projectBuildActions[i].m_commandWorkDir);
        query.bindValue(":priorAction", m_projectBuildActions[i].m_priorAction);
        query.bindValue(":exeArgs", m_projectBuildActions[i].m_commandArgs);
        query.exec();
        m_projectBuildActions[i].recreateAction();
        connect(m_projectBuildActions[i].m_action, SIGNAL(triggered()), this, SLOT(onBuildAction()));
    }
}

void ProjectBuildActionsModel::onBuildAction()
{
    for (int i = 0; i < m_projectBuildActions.size(); i++) {
        if ( m_projectBuildActions[i].m_action == sender() ) {
            m_manager->startBuildAction(m_projectBuildActions[i]);
            return;
        }
    }
}

void ProjectBuildActionsModel::onBuildActionStarted()
{
     for (int i = 0; i < m_projectBuildActions.size(); i++)
         m_projectBuildActions[i].m_action->setEnabled(false);
}

void ProjectBuildActionsModel::onBuildActionFinished()
{
    for (int i = 0; i < m_projectBuildActions.size(); i++)
        m_projectBuildActions[i].m_action->setEnabled(true);
}

ProjectBuildAction::PriorAction ProjectBuildActionsModel::fromInt( int value )
{
    switch ( value ) {
        case 1: return ProjectBuildAction::AskSaveDocuments;
        case 2: return ProjectBuildAction::AutoSaveDocuments;
    }

    return ProjectBuildAction::NoActions;
}

