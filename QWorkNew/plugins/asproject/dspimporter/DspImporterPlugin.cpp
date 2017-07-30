#include <QFileDialog>

#include "Core.h"
#include "DspImporterPlugin.h"
#include "DspConfigDialog.h"

DspImporter::DspImporter ( QObject *parent )
    : QObject(parent)
{
}

QString DspImporter::description() const
{
    return QObject::tr("Import MS Developer Studio Project File (*.DSP)");
}

QString DspImporter::sourceFilesDescription() const
{
    return QObject::tr("MS Developer Studio Project File");
}

QString DspImporter::sourceFilesExtension() const
{
    return "dsp";
}

bool DspImporter::getImportSourceFilePath( QString &sourceFilePath ) const
{
    sourceFilePath = QFileDialog::getOpenFileName( Core::Storage::mainWindow()->widget(),
                                                   QObject::tr("Open File"),
                                                   QString(),
                                                   QObject::tr("MS Developer Studio Project File (*.dsp)") );
    return !sourceFilePath.isEmpty();
}

bool DspImporter::executeImport( QString sourceFilePath,
                                 QString &dbFilePath,
                                 QStringList &includePaths,
                                 QStringList &defines,
                                 QString &precompiledHeader,
                                 ProjectTreeItem *projectItem ) const
{
    Q_UNUSED(dbFilePath)

    QString cfgName;
    if ( !getDspCfgName(sourceFilePath, cfgName) )
        return false;

    QFileInfo sourceFilePathInfo(sourceFilePath);

    if ( !readDspCfgProperties(sourceFilePathInfo.canonicalPath(), sourceFilePath, cfgName, includePaths, defines, precompiledHeader) )
        return false;

    if ( !readDspSourceStructure(sourceFilePathInfo.canonicalPath(), sourceFilePath, projectItem) )
        return false;

    return true;
}

bool DspImporter::getDspCfgName( const QString &filePath, QString &cfgName ) const
{
    QFile f(filePath);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        Core::Storage::warning(QObject::tr("Open file \"%1\" for read Failed!").arg(filePath));
        return false;
    }

    QTextStream stream(&f);
    QString line;
    QSet <QString> cfgNamesSet;
    do {
        line = stream.readLine();
        if ( line.startsWith("# Name ") )
            cfgNamesSet << line.mid(7);
    } while (!line.isNull());

    DspConfigDialog cfgDialog(cfgNamesSet, Core::Storage::mainWindow()->widget());
    if ( cfgDialog.exec() != QDialog::Accepted )
        return false;

    cfgName = cfgDialog.configurationName();

    f.close();

    return true;
}

bool DspImporter::readDspCfgProperties( const QString &path, const QString &filePath,
                                        const QString &cfgName, QStringList &includes,
                                        QStringList &defines, QString &precompiledHeader ) const
{
    QFile f(filePath);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        Core::Storage::warning(QObject::tr("Open file \"%1\" for read Failed!").arg(filePath));
        return false;
    }

    QTextStream stream(&f);
    QString line;
    int state = 0;

    includes << path;

    do {
        line = stream.readLine();

        if ( state == 0 && line[0] == '!' ) {
            if ( (line == "!IF  \"$(CFG)\" == " + cfgName) ||
                 (line == "!ELSEIF  \"$(CFG)\" == " + cfgName) ) {
                state = 1;
                continue;
            }
        }

        if ( state == 1 && line.startsWith("# ADD CPP ")) {
            QStringList words = line.split(' ');
            for (int i = 3; i < words.size() - 1; i++) {
                if ( words[i] == "/I" ) {
                    i++;
                    if ( !(words[i] == "\".\"" || words[i] == "\".\\\"" || words[i] == "\"./\"" || words[i] == "\".\\\\\"") )
                        includes << path + "/" + words[i].mid(1, words[i].length() - 2);
                    continue;
                }
                if ( words[i] == "/D" ) {
                    i++;
                    defines << words[i].mid(1, words[i].length() - 2);
                    continue;
                }
                if ( words[i].startsWith("/Yu\"") ) {
                    precompiledHeader = words[i].mid(4, words[i].length() - 5);
                }
            }
            break;
        }

        if ( line == "!ENDIF" )
            break;

    } while (!line.isNull());

    f.close();
    return true;
}

bool DspImporter::readDspSourceStructure( const QString &path, const QString &dspFilePath, ProjectTreeItem *projectItem ) const
{
    QFile f(dspFilePath);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        Core::Storage::warning(QObject::tr("Open file \"%1\" for read Failed!").arg(dspFilePath));
        return false;
    }

    QString line;
    QString groupName;
    QString fileName;
    QTextStream stream(&f);
    ProjectTreeItem *item;
    ProjectTreeItem *parentItem = projectItem;
    int idfolder = 0;
    int idfile = 0;

    do {
        line = stream.readLine();

        if ( line.startsWith("# Begin Group \"") ) {
            groupName = line.mid(15);
            groupName.chop(1);
            idfolder++;
            item = new ProjectTreeItem(new FolderNode(idfolder, groupName, groupName, parentItem->id(), ""), parentItem);
            parentItem->appendChildFolder(item);
            parentItem = item;
        }

        else if ( line == "# End Group" ) {
            parentItem = parentItem->parent();
        }

        else if ( line.startsWith("SOURCE=") ) {
            fileName = line.mid(7);
            if ( fileName.startsWith(".\\") )
                fileName = fileName.mid(2);
            fileName.prepend(path + "/");
            QFileInfo fi(fileName);
            idfile++;
            item = new ProjectTreeItem(new FileNode(idfile, fi.fileName(), fi.canonicalFilePath(), parentItem->id()), parentItem);
            parentItem->appendChildFile(item);
        }

    } while ( !line.isNull() );

    f.close();
    return true;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(DspImporter, DspImporter)
#endif
