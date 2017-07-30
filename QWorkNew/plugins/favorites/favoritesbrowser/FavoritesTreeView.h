#ifndef FAVORITESTREEVIEW_H
#define FAVORITESTREEVIEW_H

#include <QTreeView>
#include <QMenu>

#include "FavoriteModel.h"

class FavoritesTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FavoritesTreeView( FavoriteModel *model, QMenu *contextMenu, QWidget *parent = 0 );

protected:
    void mouseDoubleClickEvent ( QMouseEvent * event );
    void keyPressEvent( QKeyEvent *event );
    void contextMenuEvent( QContextMenuEvent *event );

signals:

public slots:
    void addGroup();
    void addItem();
    void remove();

private:
    FavoriteModel *m_model;
    QMenu *m_contextMenu;
};

#endif // FAVORITESTREEVIEW_H
