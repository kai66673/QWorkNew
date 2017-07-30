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

#include "qmakeprojectmanager.h"
#include "projectmanager.h"

#include "qmakeprojectmanagerconstants.h"
#include "qmakenodes.h"
#include "qmakeproject.h"
#include "qmakestep.h"
#include "qmakebuildconfiguration.h"
#include "Core.h"
#include "IDocument.h"
#include "makestep.h"
#include "qmakestep.h"
#include "desktopqmakerunconfiguration.h"
#include "qmakebuildconfiguration.h"
#include "resourceeditor/resourcenode.h"
#include "wizards/subdirsprojectwizard.h"
#include "wizards/emptyprojectwizard.h"
#include "WizardManager.h"
#include "wizards/guiappwizard.h"
#include "wizards/consoleappwizard.h"
#include "wizards/librarywizard.h"

#include <buildmanager.h>
#include <target.h>
#include <utils/qtcassert.h>
#include <texteditor.h>

#include <QDir>
#include <QFileInfo>
#include <QVariant>
#include <QFormLayout>
#include <QDialogButtonBox>

using namespace ProjectExplorer;
using namespace TextEditor;

namespace QmakeProjectManager {

PrefixLangDialog::PrefixLangDialog(const QString &title, const QString &prefix, const QString &lang, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(title);
    QFormLayout *layout = new QFormLayout(this);
    m_prefixLineEdit = new QLineEdit(this);
    m_prefixLineEdit->setText(prefix);
    layout->addRow(tr("Prefix:"), m_prefixLineEdit);

    m_langLineEdit = new QLineEdit(this);
    m_langLineEdit->setText(lang);
    layout->addRow(tr("Language:"), m_langLineEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                     Qt::Horizontal,
                                                     this);

    layout->addWidget(buttons);

    connect(buttons, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()),
            this, SLOT(reject()));
}

QString PrefixLangDialog::prefix() const
{ return m_prefixLineEdit->text(); }

QString PrefixLangDialog::lang() const
{ return m_langLineEdit->text(); }

QmakeManager *QmakeManager::m_instance = 0;

QmakeManager::QmakeManager()
    : m_contextNode(0),
      m_contextProject(0),
      m_contextFile(0)
{
    m_instance = this;

    Core::MimeDatabase *mimeDb = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDb->addMimeTypes(QLatin1String(":/qmakeproject/QMakeProjectProvider.mimetypes.xml"), &errorMessage);

    QWidget *mainWindow = Core::Storage::mainWindow()->widget();

    m_addPrefix = new QAction(tr("Add Prefix..."), mainWindow);
    m_removePrefix = new QAction(tr("Remove Prefix..."), mainWindow);
    m_renamePrefix = new QAction(tr("Change Prefix..."), mainWindow);

    connect(m_addPrefix,    SIGNAL(triggered()), this, SLOT(addPrefixContextMenu()));
    connect(m_removePrefix, SIGNAL(triggered()), this, SLOT(removePrefixContextMenu()));
    connect(m_renamePrefix, SIGNAL(triggered()), this, SLOT(renamePrefixContextMenu()));
}

QmakeManager::~QmakeManager()
{
}

QmakeManager *QmakeManager::instance()
{
    return m_instance;
}

void QmakeManager::registerProject(QmakeProject *project)
{
    m_projects.append(project);
}

void QmakeManager::unregisterProject(QmakeProject *project)
{
    /// TODO: ! crash
    // m_projects.removeOne(project);
}

void QmakeManager::notifyChanged(const QString &name)
{
    foreach (QmakeProject *pro, m_projects)
        pro->notifyChanged(name);
}

QString QmakeManager::mimeType() const
{
    return QLatin1String(QmakeProjectManager::Constants::PROFILE_MIMETYPE);
}

ProjectExplorer::Project *QmakeManager::openProject(const QString &fileName, QString *errorString)
{
    if (!QFileInfo(fileName).isFile()) {
        if (errorString)
            *errorString = tr("Failed opening project \"%1\": Project is not a file")
                .arg(fileName);
        return 0;
    }

    return new QmakeProject(this, fileName);
}

void QmakeManager::initialize(ProjectExplorer::ProjectManager *manager) const
{
    manager->registerBuildStepFactory(new Internal::MakeStepFactory);
    manager->registerBuildStepFactory(new Internal::QMakeStepFactory);
    manager->registerRunConfigurationFactory(new Internal::DesktopQmakeRunConfigurationFactory);
    manager->registerBuildConfigurationFactory(new QmakeBuildConfigurationFactory);

    Core::WizardManager::registerWizardFactory(new Internal::SubdirsProjectWizard);
    Core::WizardManager::registerWizardFactory(new Internal::EmptyProjectWizard);
    Core::WizardManager::registerWizardFactory(new Internal::GuiAppWizard);
    Core::WizardManager::registerWizardFactory(new Internal::ConsoleAppWizard);
    Core::WizardManager::registerWizardFactory(new Internal::LibraryWizard);

    ProjectManager::registerCustomWizardMetaFactory(new CustomWizardMetaFactory<QmakeProjectManager::Internal::CustomQmakeProjectWizard>(QLatin1String("qmakeproject"), Core::IWizardFactory::ProjectWizard));
}

QString QmakeManager::buildSystemActionTitle() const
{
    return tr("Run qmake");
}

void QmakeManager::executeBuildSystemCommand(Project *project, Node *node)
{
    runQMake(project, node);
}

bool QmakeManager::executeBuildCommand(Project *project, Node *node, const QList<Core::Id> &commandId)
{
    if ( commandId.size() < 1 || commandId.size() > 2 )
        return false;

    Action action = BUILD;

    if ( commandId.size() == 2 ) {
        if ( commandId[0] == Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN) &&
             commandId[1] == Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD) )
            action = REBUILD;
        else
            return false;
    } else {
        if ( commandId[0] == Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN) )
            action = CLEAN;
        else if ( commandId[0] == Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD) )
            action = BUILD;
        else
            return false;
    }

    handleSubDirContextMenu(action, false, project, node, 0);
    return true;
}

QList<QAction *> QmakeManager::additionalContextMenuActions(Node *node)
{
    QList<QAction *> result;
    if ( qobject_cast<ResourceEditor::ResourceTopLevelNode *>(node) ) {
        result << m_addPrefix;
    } else if ( qobject_cast<ResourceEditor::Internal::ResourceFolderNode *>(node) ) {
        result << m_renamePrefix << m_removePrefix;
    }
    return result;
}

ProjectExplorer::Node *QmakeManager::contextNode() const
{
    return m_contextNode;
}

void QmakeManager::setContextNode(ProjectExplorer::Node *node)
{
    m_contextNode = node;
}

ProjectExplorer::Project *QmakeManager::contextProject() const
{
    return m_contextProject;
}

void QmakeManager::setContextProject(ProjectExplorer::Project *project)
{
    m_contextProject = project;
}

ProjectExplorer::FileNode *QmakeManager::contextFile() const
{
    return m_contextFile;
}

void QmakeManager::setContextFile(ProjectExplorer::FileNode *file)
{
    m_contextFile = file;
}

void QmakeManager::addLibrary()
{
    /// TODO: ?
//    if (auto editor = qobject_cast<BaseTextEditor *>(DocumentManager::getInstance()->currentEditor()))
//        addLibrary(editor->document()->filePath(), editor);
}

void QmakeManager::addLibraryContextMenu()
{
    Node *node = ProjectManager::currentNode();
    if (qobject_cast<QmakeProFileNode *>(node))
        addLibrary(node->path());
}

void QmakeManager::addLibrary(const QString &fileName, BaseTextEditor *editor)
{
    /// TODO: ?
//    Internal::AddLibraryWizard wizard(fileName, Core::Storage::mainWindow()->widget());
//    if (wizard.exec() != QDialog::Accepted)
//        return;

//    if (!editor)
//        editor = qobject_cast<BaseTextEditor *>(Core::EditorManager::openEditor(fileName,
//            Constants::PROFILE_EDITOR_ID, Core::EditorManager::DoNotMakeVisible));
//    if (!editor)
//        return;

//    const int endOfDoc = editor->position(EndOfDocPosition);
//    editor->setCursorPosition(endOfDoc);
//    QString snippet = wizard.snippet();

//    // add extra \n in case the last line is not empty
//    int line, column;
//    editor->convertPosition(endOfDoc, &line, &column);
//    if (!editor->textAt(endOfDoc - column, column).simplified().isEmpty())
//        snippet = QLatin1Char('\n') + snippet;

//    editor->insert(snippet);
}


void QmakeManager::runQMake()
{
    runQMake(ProjectManager::startupProject(), 0);
}

void QmakeManager::runQMakeContextMenu()
{
    runQMake(m_contextProject, m_contextNode);
}

void QmakeManager::runQMake(ProjectExplorer::Project *p, ProjectExplorer::Node *node)
{
    if (!ProjectManager::saveModifiedFiles())
        return;
    QmakeProject *qmakeProject = qobject_cast<QmakeProject *>(p);
    QTC_ASSERT(qmakeProject, return);

    if (!qmakeProject->activeTarget() ||
        !qmakeProject->activeTarget()->activeBuildConfiguration())
        return;

    QmakeBuildConfiguration *bc = static_cast<QmakeBuildConfiguration *>(qmakeProject->activeTarget()->activeBuildConfiguration());
    QMakeStep *qs = bc->qmakeStep();
    if (!qs)
        return;

    //found qmakeStep, now use it
    qs->setForced(true);

    if (node != 0 && node != qmakeProject->rootProjectNode())
        if (QmakeProFileNode *profile = qobject_cast<QmakeProFileNode *>(node))
            bc->setSubNodeBuild(profile);

    BuildManager::appendStep(qs, tr("QMake"));
    bc->setSubNodeBuild(0);
}

void QmakeManager::buildSubDirContextMenu()
{
    handleSubDirContextMenu(BUILD, false);
}

void QmakeManager::cleanSubDirContextMenu()
{
    handleSubDirContextMenu(CLEAN, false);
}

void QmakeManager::rebuildSubDirContextMenu()
{
    handleSubDirContextMenu(REBUILD, false);
}

void QmakeManager::buildFileContextMenu()
{
    handleSubDirContextMenu(BUILD, true);
}

void QmakeManager::buildFile()
{
    if (IDocument *currentDocument= DocumentManager::getInstance()->currentDocument()) {
        const QString file = currentDocument->filePath();
        FileNode *node  = qobject_cast<FileNode *>(ProjectManager::nodeForFile(file));
        Project *project = ProjectManager::projectForFile(file);

        if (project && node)
            handleSubDirContextMenu(BUILD, true, project, node->projectNode(), node);
    }
}

void QmakeManager::addPrefixContextMenu()
{
    PrefixLangDialog dialog(tr("Add Prefix"), QString(), QString(), Core::Storage::mainWindow()->widget());
    if (dialog.exec() != QDialog::Accepted)
        return;
    QString prefix = dialog.prefix();
    if (prefix.isEmpty())
        return;
    ResourceEditor::ResourceTopLevelNode *topLevel = qobject_cast<ResourceEditor::ResourceTopLevelNode *>(ProjectManager::currentNode());
    topLevel->addPrefix(prefix, dialog.lang());
}

void QmakeManager::renamePrefixContextMenu()
{
    ResourceEditor::Internal::ResourceFolderNode *node = qobject_cast<ResourceEditor::Internal::ResourceFolderNode *>(ProjectManager::currentNode());

    PrefixLangDialog dialog(tr("Rename Prefix"), node->prefix(), node->lang(), Core::Storage::mainWindow()->widget());
    if (dialog.exec() != QDialog::Accepted)
        return;
    QString prefix = dialog.prefix();
    if (prefix.isEmpty())
        return;

    node->renamePrefix(prefix, dialog.lang());
}

void QmakeManager::removePrefixContextMenu()
{
    ResourceEditor::Internal::ResourceFolderNode *rfn = qobject_cast<ResourceEditor::Internal::ResourceFolderNode *>(ProjectManager::currentNode());
    if ( Core::Storage::question(tr("Remove prefix %1 and all its files?").arg(rfn->displayName())) ) {
        ResourceEditor::ResourceTopLevelNode *rn = rfn->resourceNode();
        rn->removePrefix(rfn->prefix(), rfn->lang());
    }
}

void QmakeManager::handleSubDirContextMenu(QmakeManager::Action action, bool isFileBuild)
{
    handleSubDirContextMenu(action, isFileBuild, m_contextProject, m_contextNode, m_contextFile);
}

void QmakeManager::handleSubDirContextMenu(QmakeManager::Action action, bool isFileBuild,
                                         ProjectExplorer::Project *contextProject,
                                         ProjectExplorer::Node *contextNode,
                                         ProjectExplorer::FileNode *contextFile)
{
    QmakeProject *qmakeProject = qobject_cast<QmakeProject *>(contextProject);
    QTC_ASSERT(qmakeProject, return);

    if (!qmakeProject->activeTarget() ||
        !qmakeProject->activeTarget()->activeBuildConfiguration())
    return;

    if (!contextNode || !contextFile)
        isFileBuild = false;
    QmakeBuildConfiguration *bc = qobject_cast<QmakeBuildConfiguration *>(qmakeProject->activeTarget()->activeBuildConfiguration());
    if (!bc)
        return;

    if (contextNode) {
        if (QmakePriFileNode *prifile = qobject_cast<QmakePriFileNode *>(contextNode)) {
            if (QmakeProFileNode *profile = prifile->proFileNode()) {
                if (profile != qmakeProject->rootProjectNode() || isFileBuild)
                    bc->setSubNodeBuild(profile);
            }
        }
    }

    if (isFileBuild)
        bc->setFileNodeBuild(contextFile);
    if (ProjectManager::saveModifiedFiles()) {
        const Core::Id buildStep = ProjectExplorer::Constants::BUILDSTEPS_BUILD;
        const Core::Id cleanStep = ProjectExplorer::Constants::BUILDSTEPS_CLEAN;
        if (action == BUILD) {
            const QString name = ProjectManager::displayNameForStepId(buildStep);
            BuildManager::buildList(bc->stepList(buildStep), name);
        } else if (action == CLEAN) {
            const QString name = ProjectManager::displayNameForStepId(cleanStep);
            BuildManager::buildList(bc->stepList(cleanStep), name);
        } else if (action == REBUILD) {
            QStringList names;
            names << ProjectManager::displayNameForStepId(cleanStep)
                  << ProjectManager::displayNameForStepId(buildStep);

            QList<ProjectExplorer::BuildStepList *> stepLists;
            stepLists << bc->stepList(cleanStep) << bc->stepList(buildStep);
            BuildManager::buildLists(stepLists, names);
        }
    }

    bc->setSubNodeBuild(0);
    bc->setFileNodeBuild(0);
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(QmakeManager, QmakeManager)
#endif

} // namespace QmakeProjectManager
