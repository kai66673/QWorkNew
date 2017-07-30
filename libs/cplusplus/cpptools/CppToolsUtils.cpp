#include <QFileInfo>

#include "CppToolsUtils.h"
#include "projectmanager.h"
#include "cppprojectfile.h"
#include "cpptoolsconstants.h"
#include "cppmodelmanager.h"
#include "Core.h"

namespace CppTools {

CppToolsUtils::CppToolsUtils()
{

}

CppToolsUtils::~CppToolsUtils()
{

}

namespace Internal {

static QHash<QString, QString> m_headerSourceMapping;

// Return the suffixes that should be checked when trying to find a
// source belonging to a header and vice versa
static QStringList matchingCandidateSuffixes(ProjectFile::Kind kind)
{
    Core::MimeDatabase *mdb = Core::Storage::mainWindow()->mimeDatabase();

    switch (kind) {
     // Note that C/C++ headers are undistinguishable
    case ProjectFile::CHeader:
    case ProjectFile::CXXHeader:
    case ProjectFile::ObjCHeader:
    case ProjectFile::ObjCXXHeader:
        return mdb->findByType(QLatin1String(Constants::C_SOURCE_MIMETYPE)).suffixes()
                + mdb->findByType(QLatin1String(Constants::CPP_SOURCE_MIMETYPE)).suffixes()
                + mdb->findByType(QLatin1String(Constants::OBJECTIVE_C_SOURCE_MIMETYPE)).suffixes()
                + mdb->findByType(QLatin1String(Constants::OBJECTIVE_CPP_SOURCE_MIMETYPE)).suffixes();
    case ProjectFile::CSource:
    case ProjectFile::ObjCSource:
        return mdb->findByType(QLatin1String(Constants::C_HEADER_MIMETYPE)).suffixes();
    case ProjectFile::CXXSource:
    case ProjectFile::ObjCXXSource:
    case ProjectFile::CudaSource:
    case ProjectFile::OpenCLSource:
        return mdb->findByType(QLatin1String(Constants::CPP_HEADER_MIMETYPE)).suffixes();
    default:
        return QStringList();
    }
}

static QStringList baseNameWithAllSuffixes(const QString &baseName, const QStringList &suffixes)
{
    QStringList result;
    const QChar dot = QLatin1Char('.');
    foreach (const QString &suffix, suffixes) {
        QString fileName = baseName;
        fileName += dot;
        fileName += suffix;
        result += fileName;
    }
    return result;
}

static QStringList baseNamesWithAllPrefixes(const QStringList &baseNames, bool isHeader)
{
    QStringList result;
    const QStringList &sourcePrefixes = ProjectExplorer::ProjectManager::sourcePrefixes();
    const QStringList &headerPrefixes = ProjectExplorer::ProjectManager::headerPrefixes();

    foreach (const QString &name, baseNames) {
        foreach (const QString &prefix, isHeader ? headerPrefixes : sourcePrefixes) {
            if (name.startsWith(prefix)) {
                QString nameWithoutPrefix = name.mid(prefix.size());
                result += nameWithoutPrefix;
                foreach (const QString &prefix, isHeader ? sourcePrefixes : headerPrefixes)
                    result += prefix + nameWithoutPrefix;
            }
        }
        foreach (const QString &prefix, isHeader ? sourcePrefixes : headerPrefixes)
            result += prefix + name;

    }
    return result;
}

static QStringList baseDirWithAllDirectories(const QDir &baseDir, const QStringList &directories)
{
    QStringList result;
    foreach (const QString &dir, directories)
        result << QDir::cleanPath(baseDir.absoluteFilePath(dir));
    return result;
}

static QStringList findFilesInProject(const QString &name,
                                   const ProjectExplorer::Project *project)
{
    if (!project)
        return QStringList();

    QString pattern = QString(1, QLatin1Char('/'));
    pattern += name;
    const QStringList projectFiles = project->files(ProjectExplorer::Project::AllFiles);
    const QStringList::const_iterator pcend = projectFiles.constEnd();
    QStringList candidateList;
    for (QStringList::const_iterator it = projectFiles.constBegin(); it != pcend; ++it) {
        if (it->endsWith(pattern))
            candidateList.append(*it);
    }
    return candidateList;
}

static int commonStringLength(const QString &s1, const QString &s2)
{
    int length = qMin(s1.length(), s2.length());
    for (int i = 0; i < length; ++i)
        if (s1[i] != s2[i])
            return i;
    return length;
}

static QString correspondingHeaderOrSourceInProject(const QFileInfo &fileInfo,
                                                    const QStringList &candidateFileNames,
                                                    const ProjectExplorer::Project *project)
{
    QString bestFileName;
    int compareValue = 0;
    const QString filePath = fileInfo.filePath();
    foreach (const QString &candidateFileName, candidateFileNames) {
        const QStringList projectFiles = findFilesInProject(candidateFileName, project);
        // Find the file having the most common path with fileName
        foreach (const QString &projectFile, projectFiles) {
            int value = commonStringLength(filePath, projectFile);
            if (value > compareValue) {
                compareValue = value;
                bestFileName = projectFile;
            }
        }
    }
    if (!bestFileName.isEmpty()) {
        const QFileInfo candidateFi(bestFileName);
        QTC_ASSERT(candidateFi.isFile(), return QString());
        m_headerSourceMapping[fileInfo.absoluteFilePath()] = candidateFi.absoluteFilePath();
        m_headerSourceMapping[candidateFi.absoluteFilePath()] = fileInfo.absoluteFilePath();
        return candidateFi.absoluteFilePath();
    }

    return QString();
}


}   // namespace Internal

QString correspondingHeaderOrSource(const QString &fileName, bool *wasHeader)
{
    using namespace Internal;

    const QFileInfo fi(fileName);
    ProjectFile::Kind kind = ProjectFile::classify(fileName);
    const bool isHeader = ProjectFile::isHeader(kind);
    if (wasHeader)
        *wasHeader = isHeader;
    if (m_headerSourceMapping.contains(fi.absoluteFilePath()))
        return m_headerSourceMapping.value(fi.absoluteFilePath());

    if (kind == ProjectFile::Unclassified)
        return QString();

    const QString baseName = fi.completeBaseName();
    const QString privateHeaderSuffix = QLatin1String("_p");
    const QStringList suffixes = matchingCandidateSuffixes(kind);

    QStringList candidateFileNames = baseNameWithAllSuffixes(baseName, suffixes);
    if (isHeader) {
        if (baseName.endsWith(privateHeaderSuffix)) {
            QString sourceBaseName = baseName;
            sourceBaseName.truncate(sourceBaseName.size() - privateHeaderSuffix.size());
            candidateFileNames += baseNameWithAllSuffixes(sourceBaseName, suffixes);
        }
    } else {
        QString privateHeaderBaseName = baseName;
        privateHeaderBaseName.append(privateHeaderSuffix);
        candidateFileNames += baseNameWithAllSuffixes(privateHeaderBaseName, suffixes);
    }

    const QDir absoluteDir = fi.absoluteDir();
    QStringList candidateDirs(absoluteDir.absolutePath());
    // If directory is not root, try matching against its siblings
    const QStringList searchPaths = isHeader ? ProjectExplorer::ProjectManager::sourceSearchPaths()
                                             : ProjectExplorer::ProjectManager::headerSearchPaths();
    candidateDirs += baseDirWithAllDirectories(absoluteDir, searchPaths);

    candidateFileNames += baseNamesWithAllPrefixes(candidateFileNames, isHeader);

    // Try to find a file in the same or sibling directories first
    foreach (const QString &candidateDir, candidateDirs) {
        foreach (const QString &candidateFileName, candidateFileNames) {
            const QString candidateFilePath = candidateDir + QLatin1Char('/') + candidateFileName;
            const QString normalized = Utils::FileUtils::normalizePathName(candidateFilePath);
            const QFileInfo candidateFi(normalized);
            if (candidateFi.isFile()) {
                m_headerSourceMapping[fi.absoluteFilePath()] = candidateFi.absoluteFilePath();
                if (!isHeader || !baseName.endsWith(privateHeaderSuffix))
                    m_headerSourceMapping[candidateFi.absoluteFilePath()] = fi.absoluteFilePath();
                return candidateFi.absoluteFilePath();
            }
        }
    }

    // Find files in the current project
    ProjectExplorer::Project *currentProject = ProjectExplorer::ProjectManager::currentProject();
    if (currentProject) {
        const QString path = correspondingHeaderOrSourceInProject(fi, candidateFileNames,
                                                                  currentProject);
        if (!path.isEmpty())
            return path;

    // Find files in other projects
    } else {
        CppModelManager *modelManager = CppModelManager::instance();
        QList<ProjectInfo> projectInfos = modelManager->projectInfos();
        foreach (const ProjectInfo &projectInfo, projectInfos) {
            const ProjectExplorer::Project *project = projectInfo.project().data();
            if (project == currentProject)
                continue; // We have already checked the current project.

            const QString path = correspondingHeaderOrSourceInProject(fi, candidateFileNames, project);
            if (!path.isEmpty())
                return path;
        }
    }

    return QString();
}

}   // namespace CppTools

