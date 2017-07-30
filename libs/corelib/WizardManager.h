#ifndef WIZARDMANAGER_H
#define WIZARDMANAGER_H

#include <QObject>

#include "featureprovider.h"
#include "corelib_global.h"
#include "iwizardfactory.h"
#include "ifilewizardextension.h"

namespace Core {

class CORELIB_EXPORT IDelayedWizardsLoader
{
public:
    virtual ~IDelayedWizardsLoader() {}
    virtual void delayedWizardsLoad() const = 0;
    virtual bool isManaged() const { return true; }
};

class CORELIB_EXPORT WizardManager : public QObject
{
    Q_OBJECT

public:
    explicit WizardManager(QObject *parent = 0);
    virtual ~WizardManager();

    static WizardManager *instance();

    static void registerFeatureProvider(IFeatureProvider *provider);
    static int featureProviderCount();
    static const IFeatureProvider *featureProviderAt(int index);

    static void registerWizardFactory(IWizardFactory *factory);
    static int wizardFactoryCount();
    static IWizardFactory *wizardFactoryAt(int index);
    static QList<IWizardFactory*> allWizardFactories();

    static void registerFileWizardExtensions(IFileWizardExtension *extension);
    static QList<IFileWizardExtension *> allFileWizardExtensions();

    static void registerDelayedWizardsLoader( IDelayedWizardsLoader *loader );

    static void showNewItemDialog(const QString &title,
                                  const QString &defaultLocation = QString(),
                                  const QVariantMap &extraVariables = QVariantMap());
    static void showNewItemDialog(const QString &title,
                                  const QList<IWizardFactory::WizardKind> &kinds,
                                  const QString &defaultLocation = QString(),
                                  const QVariantMap &extraVariables = QVariantMap());

private:
    QList<IFeatureProvider *> m_providers;
    QList<IWizardFactory *> m_wizards;
    QList<IFileWizardExtension *> m_fileWizardExtensions;
    QList<IDelayedWizardsLoader *> m_delayedWizardsLoaders;
    bool m_delayedWizardsLoaded;
    static WizardManager *m_instance;
};

}   // namespace Core

#endif // WIZARDMANAGER_H
