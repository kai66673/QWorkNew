#include "DocumentToolBar.h"

static int s_DocumentToolBarWatcherIndex = -1;

static int registerDocumentToolBarWatcher()
{
    if ( s_DocumentToolBarWatcherIndex == -1 )
        s_DocumentToolBarWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("DocumentToolBar"));
    return s_DocumentToolBarWatcherIndex;
}

DocumentToolBar::DocumentToolBar(QWidget *parent)
    : QToolBar(parent)
    , Utils::StyleManagedWidget(this, registerDocumentToolBarWatcher())
{ }

