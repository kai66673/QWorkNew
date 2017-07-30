#include <QSettings>

#include "Core.h"
#include "DbConnectionsHistory.h"
#include "DbConnectionsHistoryDialog.h"

DbConnectionsHistory::DbConnectionsHistory()
{
    load();
}

DbConnectionsHistory::~DbConnectionsHistory()
{
    save();
}

void DbConnectionsHistory::load()
{
    IMainWindow *mainWindow = Core::Storage::mainWindow();
    QSettings settings(mainWindow->orgName(), mainWindow->appName());

    connections.clear();

    int connCount = settings.beginReadArray("DbConnectionsHistory");

    for (int i = 0; i < connCount; i++) {
        settings.setArrayIndex(i);
        connections.append(DbConnectionAuthInfo());
        connections[i].databaseType = settings.value("DbType").toString();
        connections[i].description = settings.value("Description").toString();
        connections[i].parameters << settings.value("Parameters").toStringList();
    }

    settings.endArray();
}

void DbConnectionsHistory::save()
{
    IMainWindow *mainWindow = Core::Storage::mainWindow();
    QSettings settings(mainWindow->orgName(), mainWindow->appName());

    settings.beginWriteArray("DbConnectionsHistory", connections.size());

    for (int i = 0; i < connections.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("DbType", connections[i].databaseType);
        settings.setValue("Description", connections[i].description);
        settings.setValue("Parameters", connections[i].parameters);
    }

    settings.endArray();
}

void DbConnectionsHistory::addConnection( const DbConnectionAuthInfo &connection )
{
    int foundIndex = -1;
    for (int i = 0; i < connections.size(); i++) {
        if ( connections[i].isEqual(connection) ) {
            foundIndex = i;
            break;
        }
    }

    if ( foundIndex == -1 ) {
        connections.prepend(connection);
        while ( connections.size() > 30 )
            connections.removeLast();
    }
    else {
        connections.removeAt(foundIndex);
        connections.prepend(connection);
    }
}

bool DbConnectionsHistory::selectOrEditConnection( DbConnectionAuthInfo &connection )
{
    DbConnectionsHistoryDialog historyDialog(&connections, Core::Storage::mainWindow()->widget());
    if ( historyDialog.exec() != QDialog::Accepted )
        return false;

    int selIndex = historyDialog.selectedIndex();
    if ( selIndex < 0 || selIndex >= connections.size() )
        return false;

    connection.databaseType = connections[selIndex].databaseType;
    connection.description = connections[selIndex].description;
    connection.parameters.clear();
    connection.parameters << connections[selIndex].parameters;

    return true;
}
