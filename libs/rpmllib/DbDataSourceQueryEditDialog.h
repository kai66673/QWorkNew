#ifndef DBDATASOURCEQUERYEDITDIALOG_H
#define DBDATASOURCEQUERYEDITDIALOG_H

#include <QPlainTextEdit>
#include <QMainWindow>
#include <QToolBar>
#include <QDockWidget>

#include "QwDialog.h"
#include "ReportData.h"
#include "DataSourceAssistor.h"
#include "QwTableView.h"

class DbDataSourceQueryEditDialog : public QwDialog
{
    Q_OBJECT

public:
    explicit DbDataSourceQueryEditDialog( QList<RPML::ReportInputParameter *> *parameters,
                                          const QString &databaseName,
                                          RPML::IDataSourceAssistor *assistor,
                                          const QString &queryText,
                                          QWidget *parent = 0 );

    inline QString queryText() const { return m_queryText; }
    inline QStringList columnNames() const { return m_columnNames; }

protected:
    virtual void saveDialogGeomtry();
    virtual void restoreDialogGeomtry();

private slots:
    void onRevert();
    void onSave();
    void onCancel();
    void executeQuery();

private:
    void setupMainWindow();
    void setupParamsDock();
    void setupDataSourceObjectsDock();
    void setupExecuteDock();

    bool checkQuery();

    QList<RPML::ReportInputParameter *> *m_parameters;
    QString m_databaseName;
    RPML::IDataSourceAssistor *m_assistor;
    QString m_queryText;
    QStringList m_columnNames;

    QPlainTextEdit *m_textEdit;
    QMainWindow *m_mainWindow;
    QToolBar *m_actionsToolBar;
    QDockWidget *m_paramsDock;
    QDockWidget *m_dataSourceObjectsDock;

    QDockWidget *m_queryDock;
    Utils::QwTableView *m_queryTableView;
};

#endif // DBDATASOURCEQUERYEDITDIALOG_H
