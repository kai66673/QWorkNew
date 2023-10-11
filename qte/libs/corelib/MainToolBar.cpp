#include "MainToolBar.h"

namespace Core {

static int s_MainToolBarWatcherIndex = -1;

static int registerMainToolBarWatcher()
{
    if ( s_MainToolBarWatcherIndex == -1 )
        s_MainToolBarWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("MainToolBar"));
    return s_MainToolBarWatcherIndex;
}

MainToolBar::MainToolBar(const QString &title, QWidget *parent)
    : QToolBar(title, parent)
    , Utils::StyleManagedWidget(this, registerMainToolBarWatcher())
{ }

}   // namespace Core
