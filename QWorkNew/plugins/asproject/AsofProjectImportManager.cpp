#include <QtPlugin>
#include <QPluginLoader>
#include <QDir>
#include <QApplication>

#include <QDebug>

#include "Core.h"
#include "AsofProjectImportManager.h"
#include "SelectProjectImporterDialog.h"
#include "GenericProjectImporter.h"
#include "LogManager.h"

AsofProjectImportManager::AsofProjectImportManager()
{
}

AsofProjectImportManager::~AsofProjectImportManager()
{
//    qDeleteAll(m_importers);
    delete m_importers[0];
}

IAsofProjectImporter *AsofProjectImportManager::operator []( const QString &extension )
{
    foreach ( IAsofProjectImporter *importer, m_importers )
        if ( !importer->sourceFilesExtension().compare(extension, Qt::CaseInsensitive) )
            return importer;

    return 0;
}

void AsofProjectImportManager::loadImporterPlugins()
{
    addImporter(new GenericProjectImporter);

    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("aspdbimporters");

    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("\"asproject\" Plugins Loading Started..."),
                                                    QObject::tr("Plugins"),
                                                    Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if ( plugin ) {
            IAsofProjectImporter *iLoader = qobject_cast<IAsofProjectImporter *>(plugin);
            if ( iLoader ) {
                addImporter(iLoader);
                Core::LogManager::addMessage( Core::LogMessage( QObject::tr("  \"asproject\" Plugin \"%1\" Loaded.").arg(fileName),
                                                                QObject::tr("Plugins"),
                                                                Core::LogMessage::Information,
                                                                QColor(255, 255, 192) ) );

            }
        }
    }

    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("\"asproject\" Plugins Loaded."),
                                                    QObject::tr("Plugins"),
                                                    Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );
}

IAsofProjectImporter *AsofProjectImportManager::selectImporter()
{
    if ( m_importers.size() == 1 )
        return m_importers[0];

    SelectProjectImporterDialog selImporterDialog(this, Core::Storage::mainWindow()->widget());
    if ( selImporterDialog.exec() == QDialog::Accepted )
        return selImporterDialog.selectedImporter();

    return 0;
}

IAsofProjectImporter *AsofProjectImportManager::findImporterByExtension( const QString &extension )
{
    foreach ( IAsofProjectImporter *importer, m_importers )
        if ( !importer->sourceFilesExtension().compare(extension, Qt::CaseInsensitive) )
            return importer;

    return 0;
}
