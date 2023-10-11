#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "settings_global.h"
#include "ISettingsClient.h"

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QSettings;
class QTableView;
QT_END_NAMESPACE

class SETTINGSSHARED_EXPORT ISettingsTabPage: public QWidget
{
public:
    ISettingsTabPage( QWidget *parent = 0 ) : QWidget(parent) {}
    virtual ~ISettingsTabPage() {}

    virtual void fillSettings( QMap<QString, QVariant> &valueMap ) const = 0;
};

class SETTINGSSHARED_EXPORT ISettings : public QObject
{
    friend class SettingsManager;

public:
    virtual QString iconName() const = 0;
    virtual QString title() const = 0;
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 ) = 0;
    virtual QString groupName() const = 0;
    virtual void doApplySettings() = 0;

    void readSettings( QSettings &settings );
    void writeSettings( QSettings &settings );
    void applySettings( ISettingsTabPage *page = 0 );

    QVariant &operator [] ( const QString &key ) { return valuesMap[key]; }

protected:
    QMap <QString, QVariant> valuesMap;
};

class SETTINGSSHARED_EXPORT NullSettings : public ISettings
{
public:
    virtual QString iconName() const { return QString(); }
    virtual QString title() const { return QString(); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 ) { Q_UNUSED(parent); return NULL; }
    virtual QString groupName() const { return QStringLiteral(""); }
    virtual void doApplySettings( ISettingsTabPage *settingsWidget = 0 ) { Q_UNUSED(settingsWidget); }
};

class SETTINGSSHARED_EXPORT SettingsManager : public QObject
{
public:
    SettingsManager( QObject *parent = 0 );
    ~SettingsManager();

    static SettingsManager *instance();
    static void registerSettingsClient(ISettingsClient *client);

    void readSettings( QSettings &settings );
    void writeSetings( QSettings &settings );

    void exec( QWidget *parent = 0 );

    bool hasKey( const QString &settingsKey );
    void addSettings( const QString &settingsKey, ISettings *settings );

    QVariant &value( const QString &settingsKey, const QString &key ) { return (*settingsMap[settingsKey])[key]; }
    QMap <QString, QVariant> &valuesMap( const QString &settingsKey ) { return settingsMap[settingsKey]->valuesMap; }

private:
    QMap <QString, ISettings *> settingsMap;
    QList<ISettingsClient *> m_clients;

    static SettingsManager *m_instance;
};

#endif // SETTINGSMANAGER_H
