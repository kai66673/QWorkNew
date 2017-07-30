#ifndef LOCATORSEARCHMODEPLUGIN_H
#define LOCATORSEARCHMODEPLUGIN_H

#include "SearchEngine.h"
#include "locatorwidget.h"
#include "LocatorManager.h"

class LocatorSearchModePlugin : public ISearchModePlugin
{
public:
    LocatorSearchModePlugin();
    virtual ~LocatorSearchModePlugin();

    virtual QString name() const { return QStringLiteral("SMP_Locator"); }
    virtual QString menuText() const { return tr("&Locator"); }
    virtual QKeySequence shortcut() const { return QKeySequence(tr("Ctrl+K")); }
    virtual QString toolTip() const { return tr("Configurable Search"); }
    virtual QWidget *createWidget( QWidget *parent );
    virtual void startSearchText( const QString &/*searchText*/ );
    virtual bool isSearchChanged( const QString &/*searchText*/ );

    virtual QList<QAction *> actions() const { return QList<QAction *>(); }
    virtual QString iconName() const { return QStringLiteral(":/find/locator/images/locator.png"); }

protected:
    virtual QStringList settingsKeyList() const { return QStringList(); }
    virtual ISettings *createSettings( const QString &settingsKey ) const { Q_UNUSED(settingsKey); return 0; }

private:
    LocatorManager *m_locatorManager;
    Core::Internal::LocatorWidget *m_locatorWidget;
};

#endif // LOCATORSEARCHMODEPLUGIN_H
