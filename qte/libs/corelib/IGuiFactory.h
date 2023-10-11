#ifndef IGUIFACTORY_H
#define IGUIFACTORY_H

#include "IMainWindow.h"

class UiProviderInterface;

class IGuiFactory
{
public:
    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow ) = 0;
};

#endif // IGUIFACTORY_H
