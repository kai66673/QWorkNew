#include "WizardManager.h"
#include "qtcassert.h"
#include "newdialog.h"
#include "Core.h"

namespace Core {

WizardManager *WizardManager::m_instance = 0;

WizardManager::WizardManager(QObject *parent)
    : QObject(parent)
    , m_delayedWizardsLoaded(false)
{
    m_instance = this;
}

WizardManager::~WizardManager()
{
    qDeleteAll(m_providers);
    qDeleteAll(m_wizards);
    qDeleteAll(m_fileWizardExtensions);

    foreach ( IDelayedWizardsLoader *loader, m_delayedWizardsLoaders )
        if ( !loader->isManaged() )
            delete loader;
}

WizardManager *WizardManager::instance()
{ return m_instance; }

void WizardManager::registerFeatureProvider(IFeatureProvider *provider)
{
    QTC_ASSERT(!m_instance->m_providers.contains(provider), return);
    m_instance->m_providers.append(provider);
}

int WizardManager::featureProviderCount()
{ return m_instance->m_providers.size(); }

const IFeatureProvider *WizardManager::featureProviderAt(int index)
{
    if ( index >= 0 && index < m_instance->m_providers.size() )
        return m_instance->m_providers.at(index);

    return 0;
}

void WizardManager::registerWizardFactory(IWizardFactory *factory)
{
    QTC_ASSERT(!m_instance->m_wizards.contains(factory), return);
    m_instance->m_wizards.append(factory);
}

int WizardManager::wizardFactoryCount()
{ return m_instance->m_wizards.size(); }

IWizardFactory *WizardManager::wizardFactoryAt(int index)
{
    if ( index >= 0 && index < m_instance->m_wizards.size() )
        return m_instance->m_wizards.at(index);

    return 0;
}

QList<IWizardFactory *> WizardManager::allWizardFactories()
{ return m_instance->m_wizards; }

void WizardManager::registerFileWizardExtensions(IFileWizardExtension *extension)
{
    QTC_ASSERT(!m_instance->m_fileWizardExtensions.contains(extension), return);
    m_instance->m_fileWizardExtensions.append(extension);
}

QList<IFileWizardExtension *> WizardManager::allFileWizardExtensions()
{ return m_instance->m_fileWizardExtensions; }

void WizardManager::registerDelayedWizardsLoader(IDelayedWizardsLoader *loader)
{ m_instance->m_delayedWizardsLoaders.append(loader); }

void WizardManager::showNewItemDialog(const QString &title,
                                      const QString &defaultLocation,
                                      const QVariantMap &extraVariables)
{
    if ( !m_instance->m_delayedWizardsLoaded ) {
        foreach ( IDelayedWizardsLoader *loader, m_instance->m_delayedWizardsLoaders )
            loader->delayedWizardsLoad();
        m_instance->m_delayedWizardsLoaded = true;
    }

    Internal::NewDialog *wizardDialog = new Internal::NewDialog(Core::Storage::mainWindow()->widget());
    wizardDialog->setWizardFactories(allWizardFactories(), defaultLocation, extraVariables);
    wizardDialog->setWindowTitle(title);
    wizardDialog->exec();
}

void WizardManager::showNewItemDialog(const QString &title,
                                      const QList<IWizardFactory::WizardKind> &kinds,
                                      const QString &defaultLocation,
                                      const QVariantMap &extraVariables)
{
    if ( !m_instance->m_delayedWizardsLoaded ) {
        foreach ( IDelayedWizardsLoader *loader, m_instance->m_delayedWizardsLoaders )
            loader->delayedWizardsLoad();
        m_instance->m_delayedWizardsLoaded = true;
    }

    QList<IWizardFactory *> factories;
    foreach ( IWizardFactory::WizardKind kind, kinds )
        factories.append(IWizardFactory::wizardFactoriesOfKind(kind));

    Internal::NewDialog *wizardDialog = new Internal::NewDialog(Core::Storage::mainWindow()->widget());
    wizardDialog->setWizardFactories(factories, defaultLocation, extraVariables);
    wizardDialog->setWindowTitle(title);
    wizardDialog->exec();
}

}   // namespace Core

