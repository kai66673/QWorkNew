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

#ifndef QMAKEPROJECTMANAGER_H
#define QMAKEPROJECTMANAGER_H

#include <QDialog>
#include <QLineEdit>

#include "qmakeprojectmanager_global.h"

#include <iprojectprovider.h>
#include <projectnodes.h>

namespace TextEditor { class BaseTextEditor; }

namespace ProjectExplorer {
class Project;
class Node;
class ToolChain;
}

namespace QmakeProjectManager {

class QmakeProject;

class PrefixLangDialog : public QDialog
{
    Q_OBJECT
public:
    PrefixLangDialog(const QString &title, const QString &prefix, const QString &lang, QWidget *parent);
    QString prefix() const;

    QString lang() const;
private:
    QLineEdit *m_prefixLineEdit;
    QLineEdit *m_langLineEdit;
};

class QMAKEPROJECTMANAGER_EXPORT QmakeManager : public ProjectExplorer::IProjectProvider
{
    Q_OBJECT
    Q_INTERFACES(ProjectExplorer::IProjectProvider)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IProjectProvider/1.0")
#endif

public:
    QmakeManager();
    ~QmakeManager();

    static QmakeManager *instance();

    void registerProject(QmakeProject *project);
    void unregisterProject(QmakeProject *project);
    void notifyChanged(const QString &name);

    virtual QString mimeType() const;
    ProjectExplorer::Project *openProject(const QString &fileName, QString *errorString);
    virtual void initialize(ProjectExplorer::ProjectManager *manager) const;
    virtual QString buildSystemActionTitle() const;
    virtual void executeBuildSystemCommand(ProjectExplorer::Project *project, ProjectExplorer::Node *node);
    virtual bool executeBuildCommand(ProjectExplorer::Project *project, ProjectExplorer::Node *node, const QList<Core::Id> &commandId);

    virtual QList<QAction *> additionalContextMenuActions(ProjectExplorer::Node *node);

    // Context information used in the slot implementations
    ProjectExplorer::Node *contextNode() const;
    void setContextNode(ProjectExplorer::Node *node);
    ProjectExplorer::Project *contextProject() const;
    void setContextProject(ProjectExplorer::Project *project);
    ProjectExplorer::FileNode *contextFile() const;
    void setContextFile(ProjectExplorer::FileNode *file);

    enum Action { BUILD, REBUILD, CLEAN };

public slots:
    void addLibrary();
    void addLibraryContextMenu();
    void runQMake();
    void runQMakeContextMenu();
    void buildSubDirContextMenu();
    void rebuildSubDirContextMenu();
    void cleanSubDirContextMenu();
    void buildFileContextMenu();
    void buildFile();

private slots:
    void addPrefixContextMenu();
    void renamePrefixContextMenu();
    void removePrefixContextMenu();

private:
    QList<QmakeProject *> m_projects;
    void handleSubDirContextMenu(Action action, bool isFileBuild);
    void handleSubDirContextMenu(QmakeManager::Action action, bool isFileBuild,
                                 ProjectExplorer::Project *contextProject,
                                 ProjectExplorer::Node *contextNode,
                                 ProjectExplorer::FileNode *contextFile);
    void addLibrary(const QString &fileName, TextEditor::BaseTextEditor *editor = 0);
    void runQMake(ProjectExplorer::Project *p, ProjectExplorer::Node *node);

    ProjectExplorer::Node *m_contextNode;
    ProjectExplorer::Project *m_contextProject;
    ProjectExplorer::FileNode *m_contextFile;

    QAction *m_addPrefix;
    QAction *m_removePrefix;
    QAction *m_renamePrefix;

    static QmakeManager *m_instance;
};

} // namespace QmakeProjectManager

#endif // QMAKEPROJECTMANAGER_H
