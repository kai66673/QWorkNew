#ifndef COMMANDSHORTCUTSDIALOG_H
#define COMMANDSHORTCUTSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSortFilterProxyModel>

#include "CommandManager.h"
#include "CommandsTreeModel.h"
#include "QwDialog.h"

namespace Ui {
class CommandShortcutsDialog;
}

namespace Utils { class FancyLineEdit; }

class ShortcutsFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ShortcutsFilterProxyModel(QObject *parent = 0);

public slots:
    void setFilterText(const QString &filterText);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QString m_filterText;
};

class CommandShortcutsDialog : public QwDialog
{
    Q_OBJECT
    
public:
    explicit CommandShortcutsDialog( Core::CommandManager::CommandsMap *commandsMap_, QWidget *parent = 0 );
    ~CommandShortcutsDialog();

private slots:
    void onCurrentItemChanged( const QModelIndex &newIndex , const QModelIndex &oldIndex );
    void onApply();
    void onResetToDefaults();
    void setFilterText(const QString &filterText);
    
private:
    Ui::CommandShortcutsDialog *ui;
    CommandsTreeModel *m_model;
    Utils::FancyLineEdit *m_filterLineEdit;
    ShortcutsFilterProxyModel *m_proxyModel;
};

#endif // COMMANDSHORTCUTSDIALOG_H
