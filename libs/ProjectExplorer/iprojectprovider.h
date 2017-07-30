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

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "projectexplorer_export.h"
#include "id.h"

#include <QObject>
#include <QAction>


namespace ProjectExplorer {

class ProjectManager;
class Project;
class Node;

class PROJECTEXPLORER_EXPORT IProjectProvider
    : public QObject
{
    Q_OBJECT

public:
    IProjectProvider() {}

    virtual QString mimeType() const = 0;
    // fileName is a canonical path!
    virtual Project *openProject(const QString &fileName, QString *errorString) = 0;
    virtual void initialize(ProjectManager *manager) const = 0;

    virtual QString buildSystemActionTitle() const = 0;
    virtual void executeBuildSystemCommand(Project *project, Node *node) = 0;
    virtual bool executeBuildCommand(Project *project, Node *node, const QList<Core::Id> &commandId) = 0;

    virtual QList<QAction *> additionalContextMenuActions(Node *node) = 0;
};

} // namespace Project

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ProjectExplorer::IProjectProvider, "QWorkBase.IProjectProvider/1.0")
QT_END_NAMESPACE

#endif //PROJECTMANAGER_H
