#ifndef ASOFPROJECTIMPORTMANAGER_H
#define ASOFPROJECTIMPORTMANAGER_H

#include "IASofProjectImporter.h"

class AsofProjectImportManager
{
public:
    AsofProjectImportManager();
    ~AsofProjectImportManager();

    inline int importersCount() const { return m_importers.size(); }
    IAsofProjectImporter *operator [] ( int index ) { return m_importers[index]; }
    IAsofProjectImporter *operator [] ( const QString &extension );
    void addImporter( IAsofProjectImporter *importer ) { m_importers.append(importer); }
    void loadImporterPlugins();

    IAsofProjectImporter *selectImporter();
    IAsofProjectImporter *findImporterByExtension( const QString &extension );

private:
    QList <IAsofProjectImporter *> m_importers;
};

#endif // ASOFPROJECTIMPORTMANAGER_H
