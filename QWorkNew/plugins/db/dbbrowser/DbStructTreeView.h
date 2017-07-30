#ifndef DBSTRUCTTREEVIEW_H
#define DBSTRUCTTREEVIEW_H

#include <QTreeView>

QT_BEGIN_NAMESPACE
class QMainWindow;
QT_END_NAMESPACE

namespace Database {
    class IDbLoader;
    class IDbCatalogItem;
}
class IDatabaseLoaderPlugin;

class DbStructTreeView : public QTreeView
{
    Q_OBJECT

public:
    DbStructTreeView( QMainWindow *mainWindow_, QWidget *parent = 0 );

    Database::IDbCatalogItem *getCurrentItem( int childIndex = -1 ) const;
    Database::IDbLoader *getCurrentLoader() const;

    QMenu *getContextMenu( Database::IDbLoader *loader, Database::IDbCatalogItem *item );

protected:
    void contextMenuEvent( QContextMenuEvent * event );
    void keyPressEvent( QKeyEvent *e );

private:
    QMainWindow *mainWindow;
};

#endif // DBSTRUCTTREEVIEW_H
