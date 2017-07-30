#include "LocatorManager.h"

LocatorManager *LocatorManager::m_instance = 0;

LocatorManager::LocatorManager(QObject *parent) : QObject(parent)
{
    m_instance = this;
}

LocatorManager::~LocatorManager()
{
    qDeleteAll(m_filters);
}

LocatorManager *LocatorManager::instance()
{
    return m_instance;
}

void LocatorManager::registerFilter(Core::ILocatorFilter *filter)
{
    m_filters.append(filter);
}

QList<Core::ILocatorFilter *> LocatorManager::filters()
{
    return m_filters;
}

