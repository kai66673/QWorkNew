#ifndef PROJECTBUILDENVIRONMENTDIALOG_H
#define PROJECTBUILDENVIRONMENTDIALOG_H

#include <QDialog>
#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include <QPair>

#include "utils/environment.h"

namespace Ui {
class ProjectBuildEnvironmentDialog;
}

class BuildEnvironmentModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    BuildEnvironmentModel( Utils::Environment *environment, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role );
    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() );
    bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() );

    void onUseSystem();
    bool saveEnvironmentToDb();
    void onImport( const QString &filePath );

private:
    void initSetup();

    Utils::Environment *m_environment;
    QList <QPair <QString, QString> > m_data;
};

class ProjectBuildEnvironmentDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProjectBuildEnvironmentDialog( Utils::Environment *environment, QWidget *parent = 0 );
    ~ProjectBuildEnvironmentDialog();

public slots:
    virtual void accept();

private slots:
    void onAdd();
    void onDelete();
    void onClear();
    void onUseSystem();
    void onImport();
    
private:
    Ui::ProjectBuildEnvironmentDialog *ui;
    Utils::Environment *m_environment;
    BuildEnvironmentModel *m_model;
};

#endif // PROJECTBUILDENVIRONMENTDIALOG_H
