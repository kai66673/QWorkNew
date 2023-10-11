#include <QDebug>
#include <QVBoxLayout>

#include "CommandShortcutsDialog.h"
#include "ui_CommandShortcutsDialog.h"
#include "fancylineedit.h"
#include "Core.h"

ShortcutsFilterProxyModel::ShortcutsFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{ }

void ShortcutsFilterProxyModel::setFilterText(const QString &filterText)
{
    m_filterText = filterText;
    invalidateFilter();
}

bool ShortcutsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if ( m_filterText.isEmpty() )
        return true;

    if ( source_parent.isValid() &&
         source_parent.data().toString().contains(m_filterText, Qt::CaseInsensitive) )
        return true;

    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    if ( !index0.isValid() )
        return false;
    if ( index0.data().toString().contains(m_filterText, Qt::CaseInsensitive) )
        return true;

    if ( source_parent.isValid() ) { // Second Level
        QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);
        if ( !index1.isValid() )
            return false;
        if ( index1.data().toString().contains(m_filterText, Qt::CaseInsensitive) )
            return true;
    } else {                                  // First Level
        int rows = sourceModel()->rowCount(index0);
        for (int row = 0; row < rows; row++)
            if ( filterAcceptsRow(row, index0) )
                return true;
    }

    return false;
}

CommandShortcutsDialog::CommandShortcutsDialog( Core::CommandManager::CommandsMap *commandsMap_, QWidget *parent )
    : QwDialog("CommandShortcutsDialog",
               Core::Storage::mainWindow()->orgName(),
               Core::Storage::mainWindow()->appName(),
               parent)
    , ui(new Ui::CommandShortcutsDialog)
    , m_model(new CommandsTreeModel(commandsMap_))
    , m_filterLineEdit(new Utils::FancyLineEdit)
    , m_proxyModel(new ShortcutsFilterProxyModel(this))
{
    ui->setupUi(this);

    m_filterLineEdit->setFiltering(true);
    m_filterLineEdit->setPlaceholderText(tr("Type filter here"));
    QVBoxLayout *l = qobject_cast<QVBoxLayout *>(layout());
    l->insertWidget(0, m_filterLineEdit);

    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->commandsTreeView->setModel(m_proxyModel);

    connect(m_filterLineEdit, SIGNAL(filterChanged(QString)), this, SLOT(setFilterText(QString)));

    ShortcutEditDelegate *delegate = new ShortcutEditDelegate(m_proxyModel, this);
    ui->commandsTreeView->setItemDelegateForColumn(2, delegate);
    ui->commandsTreeView->expandAll();
    ui->commandsTreeView->resizeColumnToContents(0);
    ui->commandsTreeView->resizeColumnToContents(1);
    ui->commandsTreeView->header()->setStretchLastSection(true);
    ui->commandsTreeView->setSortingEnabled(true);
    ui->commandsTreeView->sortByColumn(0);

    connect(ui->commandsTreeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onCurrentItemChanged(QModelIndex,QModelIndex)));

    connect(ui->okPushButton,       SIGNAL(clicked()), this, SLOT(onApply()));
    connect(ui->defaultsPushButton, SIGNAL(clicked()), this, SLOT(onResetToDefaults()));
}

CommandShortcutsDialog::~CommandShortcutsDialog()
{
    delete ui;
}

void CommandShortcutsDialog::onCurrentItemChanged( const QModelIndex &newIndex, const QModelIndex &oldIndex )
{
    Q_UNUSED(oldIndex)

    QModelIndex index = m_proxyModel->mapToSource(newIndex);

    if ( CommandTreeItem *item = static_cast<CommandTreeItem *>(index.internalPointer()) ) {
        if ( item->isCommandRow() && index.column() != 2 ) {
            // Force Select shortcut column...
            QModelIndex shortcutIndex = m_proxyModel->index(newIndex.row(), 2, newIndex.parent());
            ui->commandsTreeView->setCurrentIndex(shortcutIndex);
        }
    }
}

void CommandShortcutsDialog::onApply()
{
    ui->commandsTreeView->setCurrentIndex(QModelIndex());   // for apply changes of current shortcut
    m_model->saveChanges();
    accept();
}

void CommandShortcutsDialog::onResetToDefaults()
{
    m_model->resetToDefault();
}

void CommandShortcutsDialog::setFilterText(const QString &filterText)
{
    m_proxyModel->setFilterText(filterText);
    ui->commandsTreeView->expandAll();
}
