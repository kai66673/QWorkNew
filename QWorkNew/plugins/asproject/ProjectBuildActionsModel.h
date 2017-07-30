#ifndef PROJECTBUILDACTIONSMODEL_H
#define PROJECTBUILDACTIONSMODEL_H

#include <QAbstractTableModel>
#include <QAction>
#include <QList>

#include "ProjectBuildActionsManager.h"

class ConfigureProjectBuildActionsDialog;

class ProjectBuildAction
{
public:
    enum PriorAction {
        NoActions = 0,
        AskSaveDocuments,
        AutoSaveDocuments
    };

    ProjectBuildAction( const QString &commandName = "",
                        const QString &command = "",
                        const QString &commandArgs = "",
                        const QString &commandWorkDir = "",
                        PriorAction priorAction = NoActions )
        : m_commandName(commandName)
        , m_command(command)
        , m_commandArgs(commandArgs)
        , m_commandWorkDir(commandWorkDir)
        , m_priorAction(priorAction)
        , m_action(0)
    {}

    ~ProjectBuildAction() {
        if ( m_action ) {
            delete m_action;
        }
    }

    void recreateAction();

    QString m_commandName;
    QString m_command;
    QString m_commandArgs;
    QString m_commandWorkDir;
    PriorAction m_priorAction;
    QAction *m_action;
};

class ProjectBuildActionsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    friend class ConfigureProjectBuildActionsDialog;

    explicit ProjectBuildActionsModel( ProjectBuildActionsManager *manager, QObject *parent = 0 );
    
    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() );
    bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() );

    void emitRowChanged( int rowIndex );

    QList<QAction *> projectBuildActions() const;

    void readFromDb();
    void writeToDb();

    ProjectBuildActionsManager *buildManager() const { return m_manager; }

public slots:
    void onBuildAction();
    void onBuildActionStarted();
    void onBuildActionFinished();

private:
    ProjectBuildAction::PriorAction fromInt( int value );

    ProjectBuildActionsManager *m_manager;
    QList<ProjectBuildAction> m_projectBuildActions;
};

#endif // PROJECTBUILDACTIONSMODEL_H
