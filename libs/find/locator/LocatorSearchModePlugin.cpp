#include "LocatorSearchModePlugin.h"

#include "locatorfiltersfilter.h"
#include "filesystemfilter.h"
#include "opendocumentsfilter.h"

LocatorSearchModePlugin::LocatorSearchModePlugin()
    : ISearchModePlugin()
    , m_locatorManager(new  LocatorManager())
    , m_locatorWidget(new Core::Internal::LocatorWidget())
{
    m_locatorManager->registerFilter(new Core::Internal::LocatorFiltersFilter(m_locatorManager, m_locatorWidget));
    m_locatorManager->registerFilter(new Core::Internal::FileSystemFilter(m_locatorWidget));
    m_locatorManager->registerFilter(new Core::Internal::OpenDocumentsFilter);
}

LocatorSearchModePlugin::~LocatorSearchModePlugin()
{
    delete m_locatorManager;
}

QWidget *LocatorSearchModePlugin::createWidget(QWidget *parent)
{
    m_locatorWidget->setParent(parent);
    return m_locatorWidget;
}

void LocatorSearchModePlugin::startSearchText(const QString &)
{
    m_locatorWidget->startNewSearch();
}

bool LocatorSearchModePlugin::isSearchChanged(const QString &)
{
    return !m_locatorWidget->isPopupShown();
}

