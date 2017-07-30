#include <QStandardItemModel>
#include <QMimeData>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTreeView>
#include <QSettings>
#include <QAction>
#include <QSqlError>

#include "DbDataSourceQueryEditDialog.h"
#include "SqlHighlighter.h"
#include "DataSource.h"
#include "SqlQueryModel.h"
#include "Core.h"

namespace {     // Anonimous

class ParamsModel: public QStandardItemModel
{
public:
    ParamsModel( int rows, int columns, QObject *parent = 0 )
        : QStandardItemModel(rows, columns, parent)
    {}

    virtual QMimeData *mimeData( const QModelIndexList &indexes ) const {
        QMimeData *mimeData = new QMimeData();
        QString allText = "";
        QSet<int> rowsSet;
        foreach ( QModelIndex ind, indexes )
            if ( ind.isValid() )
                rowsSet << ind.row();
        foreach ( int row, rowsSet ) {
            int id =  data(index(row, 0)).toInt();
            allText += QString("/*{{!%1 and ANY_FIELD = :P%2 }}*/\n").arg(id).arg(id);
        }
        mimeData->setText(allText);
        return mimeData;
    }
};

}   // namespace Anonimous

DbDataSourceQueryEditDialog::DbDataSourceQueryEditDialog( QList<RPML::ReportInputParameter *> *parameters,
                                                          const QString &databaseName,
                                                          RPML::IDataSourceAssistor *assistor,
                                                          const QString &queryText,
                                                          QWidget *parent )
    : QwDialog( "DbDataSourceQueryEditDialog",
                Core::Storage::mainWindow()->orgName(),
                Core::Storage::mainWindow()->appName(),
                parent )
    , m_parameters(parameters)
    , m_databaseName(databaseName)
    , m_assistor(assistor)
    , m_queryText(queryText)
{
    setWindowTitle(tr("Db Data Source Edit"));

    setupMainWindow();
    setupParamsDock();
    setupDataSourceObjectsDock();
    setupExecuteDock();

    m_actionsToolBar = m_mainWindow->addToolBar(tr("Actions"));
    m_actionsToolBar->setObjectName("DbDataSourceQueryEditDialog_ActionsToolBar");

    QAction *actRevert  = m_actionsToolBar->addAction(QIcon(":/images/dse_revert.png"), tr("Revert"));
    QAction *actSave    = m_actionsToolBar->addAction(QIcon(":/images/dse_save.png"), tr("Save"));
    QAction *actCancel  = m_actionsToolBar->addAction(QIcon(":/images/dse_cancel.png"), tr("Cancel"));
    m_actionsToolBar->addSeparator();
    QAction *actExecute = m_actionsToolBar->addAction(QIcon(":/images/dse_execute.png"), tr("Execute"));

    m_actionsToolBar->addSeparator();
    m_paramsDock->toggleViewAction()->setIcon(QIcon(":/images/dse_par"));
    m_dataSourceObjectsDock->toggleViewAction()->setIcon(QIcon(":/images/dse_obj"));
    m_actionsToolBar->addAction(m_paramsDock->toggleViewAction());
    m_actionsToolBar->addAction(m_dataSourceObjectsDock->toggleViewAction());

    actRevert->setToolTip(tr("Revert All Unsaved Changes of Data Source"));
    actRevert->setShortcut(tr("Ctrl+Esc"));
    actSave->setToolTip(tr("Save Data Source Changes And Exit from Data Source Editor"));
    actSave->setShortcut(tr("Ctrl+S"));
    actCancel->setToolTip(tr("Exit from Data Source Editor Without Saving Changes"));
    actCancel->setShortcut(tr("Ctrl+Q"));
    actExecute->setToolTip(tr("Execute Query"));
    actExecute->setShortcut(tr("F8"));

    connect(actRevert,      SIGNAL(triggered()), this, SLOT(onRevert()));
    connect(actSave,        SIGNAL(triggered()), this, SLOT(onSave()));
    connect(actCancel,      SIGNAL(triggered()), this, SLOT(onCancel()));
    connect(actExecute,     SIGNAL(triggered()), this, SLOT(executeQuery()));
}

void DbDataSourceQueryEditDialog::saveDialogGeomtry()
{
    QwDialog::saveDialogGeomtry();

    QSettings settings(m_orgName, m_appName);
    settings.beginGroup(m_className);
    settings.setValue("state", m_mainWindow->saveState());
    settings.endGroup();
}

void DbDataSourceQueryEditDialog::restoreDialogGeomtry()
{
    QwDialog::restoreDialogGeomtry();

    QSettings settings(m_orgName, m_appName);
    settings.beginGroup(m_className);
    m_mainWindow->restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}

void DbDataSourceQueryEditDialog::onRevert()
{
    m_textEdit->setPlainText(m_queryText);
}

void DbDataSourceQueryEditDialog::onSave()
{
    if ( !checkQuery() )
        return;

    m_queryText = m_textEdit->toPlainText();
    QwDialog::accept();
}

void DbDataSourceQueryEditDialog::onCancel()
{
    QwDialog::reject();
}

void DbDataSourceQueryEditDialog::executeQuery()
{
    QList<RPML::InputParameterValue> values;
    if ( !RPML::DataSourceManager::fillInputParameterValues(m_parameters, values, this) )
        return;

    QString parseText = m_textEdit->toPlainText().trimmed();

    QList<int> paramsList;

    {   // Proceed MetaSQL comments
        int pos = 0;
        while ( (pos = parseText.indexOf("/*{{!", pos)) != -1 ) {
            int numPos = pos + 5;
            int paramId = 0;
            while ( parseText[numPos].isDigit() ) {
                paramId = paramId * 10 + parseText[numPos].toLatin1() - '0';
                numPos++;
            }
            if ( numPos ) {
                int paramIndex = -1;
                for (int i = 0; i < m_parameters->size(); i++) {
                    if ( m_parameters->at(i)->id == paramId ) {
                        paramIndex = i;
                        break;
                    }
                }
                if ( paramIndex != -1 ) {
                    if ( !values[paramIndex].disabled ) {
                        int endPos = parseText.indexOf("}}*/", numPos);
                        if ( endPos != -1 ) {
                            // Replace Comments With Spaces
                            int k;
                            for (k = pos; k <= numPos; k++)
                                parseText[k] = ' ';
                            for (k = 0, pos = endPos; k < 4; k++, pos++)
                                parseText[pos] = ' ';
                            paramsList << paramId;
                            continue;
                        }
                    }
                }
            }

            // Remove Comments;
            int endPos = parseText.indexOf("}}*/", numPos);
            if ( endPos == -1 ) {
                parseText.remove(pos, -1);
                break;
            }
            parseText.remove(pos, endPos + 5 - pos);
        }
    }

    if ( !m_databaseName.isEmpty() ) {
        QSqlQuery query(QSqlDatabase::database(m_databaseName));

        if ( query.prepare(parseText) ) {
            for (int i = 0; i < m_parameters->size(); i++) {
                int pid = m_parameters->at(i)->id;
                if ( !values[i].disabled && paramsList.contains(pid) ) {
                    if ( m_parameters->at(i)->type == RPML::ReportInputParameter::Numeric )
                        query.bindValue(QString(":P%1").arg(pid), values[i].value.toInt());
                    else
                        query.bindValue(QString(":P%1").arg(pid), values[i].value.toString());
                }
            }

            Utils::SqlQueryModel *queryModel = new Utils::SqlQueryModel();

            if ( !queryModel->setQuery(&query) ) {
                Core::Storage::warning(queryModel->errorDescription());
                delete queryModel;
            }

            else {
                m_queryTableView->setModel(queryModel);
                m_queryDock->show();
            }
        }
        else
            Core::Storage::warning(query.lastError().text());
    }
}

void DbDataSourceQueryEditDialog::setupMainWindow()
{
    m_textEdit = new QPlainTextEdit(this);
    m_mainWindow = new QMainWindow();
    m_mainWindow->setCentralWidget(m_textEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_mainWindow);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    new SqlHighlighter(m_textEdit->document());
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(10);
    m_textEdit->setFont(font);
    m_textEdit->setPlainText(m_queryText);
}

void DbDataSourceQueryEditDialog::setupParamsDock()
{
    int rowCount = m_parameters->size();
    ParamsModel *model = new ParamsModel(rowCount, 2);
    model->setHorizontalHeaderLabels( QStringList() << tr("ID") << tr("Description") );
    for (int i = 0; i < rowCount; i++) {
        QStandardItem *itemId = new QStandardItem(QString("%1").arg(m_parameters->at(i)->id));
        model->setItem(i, 0, itemId);
        QStandardItem *itemDescription = new QStandardItem(m_parameters->at(i)->description);
        model->setItem(i, 1, itemDescription);
    }

    QTableView *paramsView = new QTableView();
    paramsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    paramsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    paramsView->horizontalHeader()->setStretchLastSection(true);
    paramsView->horizontalHeader()->resizeSection(0, 45);
    paramsView->setModel(model);
    paramsView->setDragEnabled(true);

    m_paramsDock = new QDockWidget(tr("Parameters"));
    m_paramsDock->setObjectName("DbDataSourceQueryEditDialog_ParametersDock");
    m_paramsDock->setWidget(paramsView);
    m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_paramsDock);
}

void DbDataSourceQueryEditDialog::setupDataSourceObjectsDock()
{
    m_dataSourceObjectsDock = new QDockWidget(tr("Objects"));
    m_dataSourceObjectsDock->setObjectName("DbDataSourceQueryEditDialog_DataSourceObjectsDock");
    QTreeView *dsoView = new QTreeView();
    dsoView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    dsoView->setDragEnabled(true);
    dsoView->header()->hide();
    m_dataSourceObjectsDock->setWidget(dsoView);
    m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_dataSourceObjectsDock);

    if ( m_assistor ) {
        RPML::DataSourceObjectModel *model = new RPML::DataSourceObjectModel(m_assistor);
        dsoView->setModel(model);
    }
}

void DbDataSourceQueryEditDialog::setupExecuteDock()
{
    m_queryTableView = new Utils::QwTableView();
    m_queryDock = new QDockWidget(tr("Query Result"));
    m_queryDock->setObjectName("DbDataSourceQueryEditDialog_QueryDock");
    m_queryDock->setWidget(m_queryTableView);
    m_mainWindow->addDockWidget(Qt::BottomDockWidgetArea, m_queryDock);
    m_queryDock->hide();
}

bool DbDataSourceQueryEditDialog::checkQuery()
{
    QString parseText = m_textEdit->toPlainText();

    QRegExp lineComment("--[^\n]*");
    QRegExp strComment("/\\*.*\\*/");   strComment.setMinimal(true);
    QRegExp stringText("\".*\"");       stringText.setMinimal(true);
    QRegExp parents("\\([^\\(]*\\)");   parents.setMinimal(true);

    parseText.remove(lineComment);
    parseText.remove(strComment);
    parseText.remove(stringText);

    int lengthBefore;
    while (1) {
        lengthBefore = parseText.length();
        parseText.remove(parents);
        if (parseText.length() == lengthBefore)
            break;
    }

    int s0 = parseText.indexOf(QRegExp("\\bselect\\b", Qt::CaseInsensitive), 0);
    if (s0 == -1) {
        QMessageBox::warning(this, tr("Query Parse"), tr("Query cannot be Parsed!\n\"SELECT\" not Found."));
        return false;
    }
    int s1 = parseText.indexOf(QRegExp("\\bfrom\\b",   Qt::CaseInsensitive), s0 + 7);
    if (s1 == -1) {
        QMessageBox::warning(this, tr("Query Parse"), tr("Query cannot be Parsed!\n\"FROM\" not Found."));
        return false;
    }
    QString columns = parseText.mid(s0 + 7, s1 - s0 - 7);

    m_columnNames = columns.split(',');
    if ( m_columnNames.isEmpty() ) {
        QMessageBox::warning(this, tr("Query Parse"), tr("Query cannot be Parsed!\nColumns not Found."));
        return false;
    }

    for (int i = 0; i < m_columnNames.size(); i++)
        m_columnNames[i] = m_columnNames[i].trimmed();

    return true;
}
