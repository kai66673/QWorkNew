#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

#include "Core.h"
#include "GenericProjectImporter.h"
#include "GenericProjectLocationDialog.h"

QString GenericProjectImporter::description() const
{
    return QObject::tr("Generic Asof Project");
}

QString GenericProjectImporter::sourceFilesDescription() const
{
    return QObject::tr("Asof Project Database File");
}

QString GenericProjectImporter::sourceFilesExtension() const
{
    return "";
}

bool GenericProjectImporter::getImportSourceFilePath( QString &sourceFilePath ) const
{
    sourceFilePath = QFileDialog::getSaveFileName( Core::Storage::mainWindow()->widget(),
                                                   QObject::tr("New Project Database"),
                                                   QString(),
                                                   QObject::tr("ASOF Project Database (*.aspdb)"),
                                                   0,
                                                   QFileDialog::ShowDirsOnly );
    return !sourceFilePath.isEmpty();
}

bool GenericProjectImporter::executeImport( QString sourceFilePath,
                                            QString &dbFilePath,
                                            QStringList &includePaths,
                                            QStringList &defines,
                                            QString &precompiledHeader,
                                            ProjectTreeItem *projectItem ) const
{
    Q_UNUSED(projectItem)

    qDebug() << "GenericProjectImporter::executeImport" << sourceFilePath << includePaths << defines << precompiledHeader;

    GenericProjectLocationDialog locationDialog(Core::Storage::mainWindow()->widget());
    if ( locationDialog.exec() != QDialog::Accepted )
        return false;

    dbFilePath = locationDialog.sourceLocation();
    QFileInfo fi(dbFilePath);

    int idfile = 0;
    int idfolder = 0;
    readDirSources(fi.absolutePath(), projectItem, idfile, idfolder);
    return true;
}

void GenericProjectImporter::readDirSources( const QString &path, ProjectTreeItem *parentItem , int &idfile, int &idfolder ) const
{
    {   // Files
        QDir dir(path);
        dir.setFilter(QDir::Files);
        QFileInfoList fileInfoList = dir.entryInfoList();
        for (int i = 0; i < fileInfoList.size(); ++i) {
            QFileInfo fileInfo = fileInfoList.at(i);
            QString extension = fileInfo.suffix().toUpper();
            if ( extension == "SC" || extension == "C" || extension == "CPP" || extension == "CXX" || extension == "CWW" ||
                 extension == "H" || extension == "HPP" || extension == "HXX" ) {
                if ( extension == "C" ) {
                    QString scFileName = fileInfo.canonicalFilePath();
                    scFileName.chop(1);
                    scFileName.append("SC");
                    QFileInfo scFileInfo(scFileName);
                    if ( scFileInfo.exists() )
                        continue;
                }
                idfile++;
                ProjectTreeItem *item = new ProjectTreeItem(new FileNode(idfile, fileInfo.fileName(), fileInfo.canonicalFilePath(), parentItem->id()), parentItem);
                parentItem->appendChildFile(item);
            }
        }
    }

    {   // Dirs
        QDir dir(path);
        QStringList dirsNames = dir.entryList(QDir::Dirs);

        foreach ( const QString &dirName, dirsNames ) {
            if ( dirName == "." || dirName == ".." )
                continue;
            idfolder++;
            QString newPath = path + "/" + dirName;
            ProjectTreeItem *item = new ProjectTreeItem(new FolderNode(idfolder, dirName, dirName, parentItem->id(), newPath), parentItem);
            parentItem->appendChildFolder(item);
            readDirSources(newPath, item, idfile, idfolder);
        }
    }
}
