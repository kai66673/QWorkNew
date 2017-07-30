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

#ifndef CMAKEPROJECTMANAGER_H
#define CMAKEPROJECTMANAGER_H

#include <iprojectprovider.h>
#include "IDocument.h"

QT_BEGIN_NAMESPACE
class QAction;
class QDir;
QT_END_NAMESPACE

namespace ProjectExplorer { class Node; }
namespace Utils {
class Environment;
class QtcProcess;
}

namespace CMakeProjectManager {
namespace Internal {

class CMakeSettingsPage;

class CMakeManager : public ProjectExplorer::IProjectProvider
{
    Q_OBJECT
    Q_INTERFACES(ProjectExplorer::IProjectProvider)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IProjectProvider/1.0")
#endif

public:
    CMakeManager();
    virtual ~CMakeManager();

    virtual ProjectExplorer::Project *openProject(const QString &fileName, QString *errorString);
    virtual QString mimeType() const;
    virtual void initialize(ProjectExplorer::ProjectManager *manager) const;
    virtual QString buildSystemActionTitle() const;
    virtual void executeBuildSystemCommand(ProjectExplorer::Project *project, ProjectExplorer::Node *node);
    virtual bool executeBuildCommand(ProjectExplorer::Project *project, ProjectExplorer::Node *node, const QList<Core::Id> &commandId);

    virtual QList<QAction *> additionalContextMenuActions(ProjectExplorer::Node *node);

    QString cmakeExecutable() const;
    bool isCMakeExecutableValid() const;

    void setCMakeExecutable(const QString &executable);

    void createXmlFile(Utils::QtcProcess *process,
                       const QString &arguments,
                       const QString &sourceDirectory,
                       const QDir &buildDirectory,
                       const Utils::Environment &env,
                       const QString &generator);
    bool hasCodeBlocksMsvcGenerator() const;
    bool hasCodeBlocksNinjaGenerator() const;
    bool preferNinja() const;
    static QString findCbpFile(const QDir &);

private:
    void updateContextMenu(ProjectExplorer::Project *project, ProjectExplorer::Node *node);
    void runCMake(ProjectExplorer::Project *project);

private:
    CMakeSettingsPage *m_settingsPage;
    QAction *m_runCMakeAction;
    QAction *m_runCMakeActionContextMenu;
    ProjectExplorer::Project *m_contextProject;

    IDocumentPlugin *m_documentPlugin;};

} // namespace Internal
} // namespace CMakeProjectManager

#endif // CMAKEPROJECTMANAGER_H
