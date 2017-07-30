#include <QMenu>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMainWindow>

#include "DbCatalog.h"
#include "DbStructTreeView.h"
#include "DbManager.h"
#include "sql_constants.h"

DbStructTreeView::DbStructTreeView( QMainWindow *mainWindow_, QWidget *parent )
    : QTreeView(parent)
    , mainWindow(mainWindow_)
{
}

QMenu *DbStructTreeView::getContextMenu( Database::IDbLoader *loader, Database::IDbCatalogItem *item )
{
    QMenu *menu = 0;

    if ( item->dbDetailItem() ) {
        menu = new QMenu(mainWindow);
        menu->addAction(Database::DbManager::getInstance()->itemAction(Sql::Constants::DbItemAction::C_DETAILS_ACTION));
    }

    if ( item->isNameDetailed() ) {
        if ( !menu ) {
            menu = new QMenu(mainWindow);
        }
        else
            menu->addSeparator();
        menu->addAction(Database::DbManager::getInstance()->itemAction(Sql::Constants::DbItemAction::C_COPY_NAME_ACTION));
    }

    int k = 0;
    foreach ( int index, item->actions() ) {
        if ( !menu )
            menu = new QMenu(mainWindow);
        else if ( k == 0 )
            menu->addSeparator();
        menu->addAction(Database::DbManager::getInstance()->itemAction(index));
        k++;
    }

    QList<QAction *> loaderActions = loader->actionsForItem(item);
    if ( loaderActions.size() > 0 ) {
        if ( !menu )
            menu = new QMenu(mainWindow);
        else
            menu->addSeparator();
        menu->addActions(loaderActions);
    }

    return menu;
}

void DbStructTreeView::contextMenuEvent( QContextMenuEvent *event )
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    DbFilterModel *filterModel = qobject_cast<DbFilterModel *>(model());
    if ( !filterModel )
        return;

    Database::DbCatalogModel *structModel = qobject_cast<Database::DbCatalogModel *>(filterModel->sourceModel());
    if ( !structModel )
        return;

    Database::IDbCatalogItem *item = static_cast<Database::IDbCatalogItem *>(filterModel->mapToSource(index).internalPointer());
    if ( !item )
        return;

    QMenu *menu = getContextMenu(structModel->loader(), item);

    if ( menu )
        menu->exec(event->globalPos());
}

void DbStructTreeView::keyPressEvent( QKeyEvent *e )
{
    if ( !e->modifiers() && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) ) {
        Database::DbManager::getInstance()->onDetails();
        e->accept();
        return;
    }

    if ( e->matches(QKeySequence::Copy) ) {
        Database::DbManager::getInstance()->onCopyName();
        e->accept();
        return;
    }

    QTreeView::keyPressEvent(e);
}

Database::IDbCatalogItem *DbStructTreeView::getCurrentItem( int childIndex ) const
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return NULL;

    DbFilterModel *filterModel = qobject_cast<DbFilterModel *>(model());
    if ( !filterModel )
        return 0;

    Database::DbCatalogModel *structModel = qobject_cast<Database::DbCatalogModel *>(filterModel->sourceModel());
    if ( !structModel )
        return 0;

    if ( childIndex != -1 )
        return structModel->getItemChild(childIndex, filterModel->mapToSource(index));

    return structModel->getItem(filterModel->mapToSource(index));

}

Database::IDbLoader *DbStructTreeView::getCurrentLoader() const
{
    DbFilterModel *filterModel = qobject_cast<DbFilterModel *>(model());
    if ( !filterModel )
        return 0;

    Database::DbCatalogModel *structModel = qobject_cast<Database::DbCatalogModel *>(filterModel->sourceModel());
    if ( !structModel )
        return 0;

    return structModel->loader();
}
