#include <QTreeView>
#include <QHeaderView>

#include "ITextDocument.h"
#include "Core.h"

#include "Core.h"
#include "FavoritesPlugin.h"
#include "FavoriteModel.h"
#include "FavoritesTreeView.h"

FavoritesPlugin::FavoritesPlugin()
    : m_favoriteFactoryManager(new FavoriteFactoriesManager)
{
    createActions();
}

FavoritesPlugin::~FavoritesPlugin()
{
}

void FavoritesPlugin::initialize()
{
    m_favoriteFactoryManager->loadFactoryPlugins();
    m_model->loadApplication(Core::Storage::mainWindow()->applicationDataLocation());
    Core::CommandManager::getInstance()->registerCommands( tr("FavoritesCommands"),
                                                           QList<Core::Command *>() << m_cmdExpandAll
                                                                                    << m_cmdCollapseAll
                                                                                    << m_cmdAddGroup
                                                                                    << m_cmdAddItem
                                                                                    << m_cmdDelete );
}

QList<UiProviderInterface *> FavoritesPlugin::createUiProviders( IMainWindow *mainWindow )
{
    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdExpandAll->action());
    contextMenu->addAction(m_cmdCollapseAll->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdAddGroup->action());
    contextMenu->addAction(m_cmdAddItem->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdDelete->action());

    QwCustomDockWidget *favoritesDock = new QwCustomDockWidget( mainWindow->widget(), QObject::tr("Favorites"), "FavoritesDockWidget",
                                                                QKeySequence(tr("Ctrl+D,F")), QIcon(":/images/dock_favorites.png"),
                                                                Qt::LeftDockWidgetArea, QwCustomDockWidget::Second,
                                                                QList<QAction *>() << m_cmdExpandAll->action()
                                                                                   << m_cmdCollapseAll->action()
                                                                                   << m_cmdAddGroup->action()
                                                                                   << m_cmdAddItem->action()
                                                                                   << m_cmdDelete->action() );

    m_model = new FavoriteModel(this);
    FavoritesTreeView *view = new FavoritesTreeView(m_model, contextMenu, favoritesDock);
    favoritesDock->setWidget(view);
    favoritesDock->setContextMenu(contextMenu);

    connect(view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));

    connect(m_cmdExpandAll->action(),   SIGNAL(triggered()), view, SLOT(expandAll()));
    connect(m_cmdCollapseAll->action(), SIGNAL(triggered()), view, SLOT(collapseAll()));
    connect(m_cmdAddGroup->action(),    SIGNAL(triggered()), view, SLOT(addGroup()));
    connect(m_cmdAddItem->action(),     SIGNAL(triggered()), view, SLOT(addItem()));
    connect(m_cmdDelete->action(),      SIGNAL(triggered()), view, SLOT(remove()));

    return QList<UiProviderInterface *>() << new UiProviderInterface(favoritesDock);
}

void FavoritesPlugin::sessionClose()
{
    m_model->sessionClose();
}

void FavoritesPlugin::saveSession( const QString &location )
{
    m_model->saveSession(location);
}

void FavoritesPlugin::loadSession( const QString &location )
{
    m_model->loadSession(location);
}

void FavoritesPlugin::applicationClose()
{
    m_model->saveApplication(Core::Storage::mainWindow()->applicationDataLocation());
}

void FavoritesPlugin::destroy()
{
    delete m_favoriteFactoryManager;
}

IFavoriteFactory *FavoritesPlugin::favoriteFactoryForId( const QString &id ) const
{
    return (*m_favoriteFactoryManager)[id];
}

IFavoriteFactory *FavoritesPlugin::selectFavoriteFactory() const
{
    return m_favoriteFactoryManager->selectFavoriteFactory();
}

void FavoritesPlugin::currentRowChanged( const QModelIndex &current, const QModelIndex &previous )
{
    Q_UNUSED(previous)

    if ( !current.isValid() ) {
        m_cmdAddGroup->action()->setEnabled(false);
        m_cmdAddItem->action()->setEnabled(false);
        m_cmdDelete->action()->setEnabled(false);
        return;
    }

    m_cmdAddGroup->action()->setEnabled(true);
    m_cmdAddItem->action()->setEnabled(true);
    m_cmdDelete->action()->setEnabled(m_model->canDelete(current));
}

void FavoritesPlugin::createActions()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdExpandAll      = new Core::Command( tr("ExpandAllFavorites"),
                                             QIcon(":/images/fav_expand.png"),
                                             tr("Expand All"),
                                             tr("Expand All"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdCollapseAll    = new Core::Command( tr("CollapseAllFavorites"),
                                             QIcon(":/images/fav_collapse.png"),
                                             tr("Collapse All"),
                                             tr("Collapse All"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdAddGroup       = new Core::Command( tr("AddFavoriteGroup"),
                                             QIcon(":/images/fav_add_folder.png"),
                                             tr("Insert Group"),
                                             tr("Insert Group"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdAddItem        = new Core::Command( tr("AddFavoriteItem"),
                                             QIcon(":/images/fav_add_item.png"),
                                             tr("Insert Favorite"),
                                             tr("Insert Favorite"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdDelete         = new Core::Command( tr("RemoveGroupOrItemFavorite"),
                                             QIcon(":/images/fav_delete.png"),
                                             tr("Remove"),
                                             tr("Remove"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdExpandAll->action()->setEnabled(true);
    m_cmdCollapseAll->action()->setEnabled(true);
    m_cmdAddGroup->action()->setEnabled(false);
    m_cmdAddItem->action()->setEnabled(false);
    m_cmdDelete->action()->setEnabled(false);
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(FavoritesPlugin, FavoritesPlugin)
#endif
