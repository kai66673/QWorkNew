/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "cmakeprojectmanager.h"
#include "cmakeopenprojectwizard.h"
#include "cmakeprojectconstants.h"
#include "cmakeproject.h"
#include "cmakesettingspage.h"
#include "cmakerunconfiguration.h"
#include "Core.h"
#include "ProjectExplorer/projectmanager.h"

#include <utils/synchronousprocess.h>

//#include <coreplugin/actionmanager/actionmanager.h>
//#include <coreplugin/actionmanager/command.h>
//#include <coreplugin/actionmanager/actioncontainer.h>
//#include <projectexplorer/projectexplorer.h>
#include <QDateTime>

using namespace CMakeProjectManager::Internal;

class CMakeDocumentPlugin: public PlainTextDocumentPlugin
{
public:
    virtual bool acceptExtension( const QString &extension ) const { Q_UNUSED(extension); return true; }
    virtual QString defaultExtension() const { return QStringLiteral("txt"); }
    virtual QString description() const { return QObject::tr("CMake Project File"); }

    virtual QStringList settingsKeyList() const { return QStringList(); }
    virtual ISettings *createSettings( const QString &/*settingsKey*/ ) const { return 0; }

    virtual void initialize() { updateEditorCommands(); }
};

CMakeManager::CMakeManager()
    : ProjectExplorer::IProjectProvider()
{
    m_settingsPage = new CMakeSettingsPage();
    ProjectExplorer::ProjectManager::registerOptionPage(m_settingsPage);
    Core::MimeDatabase *mimeDb = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDb->addMimeTypes(QLatin1String(":/cmakeproject/CMakeProjectProvider.mimetypes.xml"), &errorMessage);
    m_documentPlugin = new CMakeDocumentPlugin();
    /// TODO: ?
//    ProjectExplorer::ProjectExplorerPlugin *projectExplorer = ProjectExplorer::ProjectExplorerPlugin::instance();
//    connect(projectExplorer, &ProjectExplorer::ProjectExplorerPlugin::aboutToShowContextMenu,
//            this, &CMakeManager::updateContextMenu);

//    Core::ActionContainer *mbuild =
//            Core::ActionManager::actionContainer(ProjectExplorer::Constants::M_BUILDPROJECT);
//    Core::ActionContainer *mproject =
//            Core::ActionManager::actionContainer(ProjectExplorer::Constants::M_PROJECTCONTEXT);
//    Core::ActionContainer *msubproject =
//            Core::ActionManager::actionContainer(ProjectExplorer::Constants::M_SUBPROJECTCONTEXT);

//    const Core::Context projectContext(CMakeProjectManager::Constants::PROJECTCONTEXT);

//    m_runCMakeAction = new QAction(QIcon(), tr("Run CMake"), this);
//    Core::Command *command = Core::ActionManager::registerAction(m_runCMakeAction,
//                                                                 Constants::RUNCMAKE, projectContext);
//    command->setAttribute(Core::Command::CA_Hide);
//    mbuild->addAction(command, ProjectExplorer::Constants::G_BUILD_DEPLOY);
//    connect(m_runCMakeAction, &QAction::triggered, [this]() {
//        runCMake(ProjectExplorer::ProjectExplorerPlugin::currentProject());
//    });

//    m_runCMakeActionContextMenu = new QAction(QIcon(), tr("Run CMake"), this);
//    command = Core::ActionManager::registerAction(m_runCMakeActionContextMenu,
//                                                  Constants::RUNCMAKECONTEXTMENU, projectContext);
//    command->setAttribute(Core::Command::CA_Hide);
//    mproject->addAction(command, ProjectExplorer::Constants::G_PROJECT_BUILD);
//    msubproject->addAction(command, ProjectExplorer::Constants::G_PROJECT_BUILD);
//    connect(m_runCMakeActionContextMenu, &QAction::triggered, [this]() {
//        runCMake(m_contextProject);
//    });

}

CMakeManager::~CMakeManager()
{
    delete m_documentPlugin;
}

void CMakeManager::updateContextMenu(ProjectExplorer::Project *project, ProjectExplorer::Node *)
{
    m_contextProject = project;
}

void CMakeManager::runCMake(ProjectExplorer::Project *project)
{
    if (!project)
        return;
    CMakeProject *cmakeProject = qobject_cast<CMakeProject *>(project);
    if (!cmakeProject || !cmakeProject->activeTarget() || !cmakeProject->activeTarget()->activeBuildConfiguration())
        return;

    if (!DocumentManager::getInstance()->trySaveAllDocuments())
        return;

    CMakeBuildConfiguration *bc
            = static_cast<CMakeBuildConfiguration *>(cmakeProject->activeTarget()->activeBuildConfiguration());

    CMakeBuildInfo info(bc);

    CMakeOpenProjectWizard copw(Core::Storage::mainWindow()->widget(),
                                this,
                                CMakeOpenProjectWizard::WantToUpdate,
                                &info);
    if (copw.exec() == QDialog::Accepted)
        cmakeProject->parseCMakeLists();
}

ProjectExplorer::Project *CMakeManager::openProject(const QString &fileName, QString *errorString)
{
    if (!QFileInfo(fileName).isFile()) {
        if (errorString)
            *errorString = tr("Failed opening project \"%1\": Project is not a file")
                .arg(fileName);
        return 0;
    }

    return new CMakeProject(this, fileName);
}

QString CMakeManager::mimeType() const
{
    return QLatin1String(Constants::CMAKEPROJECTMIMETYPE);
}

void CMakeManager::initialize(ProjectExplorer::ProjectManager *manager) const
{
    manager->registerBuildStepFactory(new MakeStepFactory);
    manager->registerRunConfigurationFactory(new CMakeRunConfigurationFactory);
    manager->registerBuildConfigurationFactory(new CMakeBuildConfigurationFactory);
}

QString CMakeManager::buildSystemActionTitle() const
{
    return tr("Run CMake");
}

void CMakeManager::executeBuildSystemCommand(ProjectExplorer::Project *project, ProjectExplorer::Node *node)
{
    Q_UNUSED(node);
    runCMake(project);
}

bool CMakeManager::executeBuildCommand(ProjectExplorer::Project *project, ProjectExplorer::Node *node, const QList<Core::Id> &commandId)
{
    Q_UNUSED(project);
    Q_UNUSED(node);
    Q_UNUSED(commandId);
    return false;
}

QList<QAction *> CMakeManager::additionalContextMenuActions(ProjectExplorer::Node *node)
{
    Q_UNUSED(node)
    return QList<QAction *>();
}

QString CMakeManager::cmakeExecutable() const
{
    return m_settingsPage->cmakeExecutable();
}

bool CMakeManager::isCMakeExecutableValid() const
{
    return m_settingsPage->isCMakeExecutableValid();
}

void CMakeManager::setCMakeExecutable(const QString &executable)
{
    m_settingsPage->setCMakeExecutable(executable);
}

bool CMakeManager::hasCodeBlocksMsvcGenerator() const
{
    return m_settingsPage->hasCodeBlocksMsvcGenerator();
}

bool CMakeManager::hasCodeBlocksNinjaGenerator() const
{
    return m_settingsPage->hasCodeBlocksNinjaGenerator();
}

bool CMakeManager::preferNinja() const
{
    return m_settingsPage->preferNinja();
}

// need to refactor this out
// we probably want the process instead of this function
// cmakeproject then could even run the cmake process in the background, adding the files afterwards
// sounds like a plan
void CMakeManager::createXmlFile(Utils::QtcProcess *proc, const QString &arguments,
                                 const QString &sourceDirectory, const QDir &buildDirectory,
                                 const Utils::Environment &env, const QString &generator)
{
    QString buildDirectoryPath = buildDirectory.absolutePath();
    buildDirectory.mkpath(buildDirectoryPath);
    proc->setWorkingDirectory(buildDirectoryPath);
    proc->setEnvironment(env);

    const QString srcdir = buildDirectory.exists(QLatin1String("CMakeCache.txt")) ?
                QString(QLatin1Char('.')) : sourceDirectory;
    QString args;
    Utils::QtcProcess::addArg(&args, srcdir);
    Utils::QtcProcess::addArgs(&args, arguments);
    Utils::QtcProcess::addArg(&args, generator);
    proc->setCommand(cmakeExecutable(), args);
    proc->start();
}

QString CMakeManager::findCbpFile(const QDir &directory)
{
    // Find the cbp file
    //   the cbp file is named like the project() command in the CMakeList.txt file
    //   so this function below could find the wrong cbp file, if the user changes the project()
    //   2name
    QDateTime t;
    QString file;
    foreach (const QString &cbpFile , directory.entryList()) {
        if (cbpFile.endsWith(QLatin1String(".cbp"))) {
            QFileInfo fi(directory.path() + QLatin1Char('/') + cbpFile);
            if (t.isNull() || fi.lastModified() > t) {
                file = directory.path() + QLatin1Char('/') + cbpFile;
                t = fi.lastModified();
            }
        }
    }
    return file;
}

//#ifndef HAVE_QT5
//Q_EXPORT_PLUGIN2(CMakeManager, CMakeManager)
//#endif
