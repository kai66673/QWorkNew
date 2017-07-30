#include <QApplication>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMouseEvent>

#include "FavoritesTreeView.h"

FavoritesTreeView::FavoritesTreeView(FavoriteModel *model, QMenu *contextMenu, QWidget *parent )
    : QTreeView(parent)
    , m_model(model)
    , m_contextMenu(contextMenu)
{
    header()->hide();
    setModel(model);
}

void FavoritesTreeView::mouseDoubleClickEvent( QMouseEvent *event )
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;
    if ( !m_model->isGroup(index) ) {
        m_model->execute(index);
        return;
    }

    QTreeView::mouseDoubleClickEvent(event);
}

void FavoritesTreeView::keyPressEvent( QKeyEvent *event )
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( index.isValid() ) {
        switch ( event->key() ) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if ( !m_model->isGroup(index) ) {
                    m_model->execute(index);
                    return;
                }
                break;
            case Qt::Key_Insert:
                if ( QApplication::keyboardModifiers() & Qt::ShiftModifier )
                    m_model->insertGroup(index);
                else
                    m_model->insertItem(index);
                return;
            case Qt::Key_Delete:
                m_model->remove(index);
                return;
        }
    }

    QTreeView::keyPressEvent(event);
}

void FavoritesTreeView::contextMenuEvent( QContextMenuEvent *event )
{
    m_contextMenu->exec(event->globalPos());
}

void FavoritesTreeView::addGroup()
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( index.isValid() )
        m_model->insertGroup(index);
}

void FavoritesTreeView::addItem()
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( index.isValid() )
        m_model->insertItem(index);
}

void FavoritesTreeView::remove()
{
    QModelIndex index = selectionModel()->currentIndex();
    if ( index.isValid() )
        m_model->remove(index);
}
