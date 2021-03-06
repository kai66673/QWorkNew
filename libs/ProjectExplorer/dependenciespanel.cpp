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

#include "dependenciespanel.h"
#include "project.h"
#include "projectmanager.h"
#include "Core.h"

#include <fileiconprovider.h>
#include <utils/detailswidget.h>

#include <QDebug>
#include <QSize>
#include <QCoreApplication>

#include <QHBoxLayout>
#include <QTreeView>
#include <QSpacerItem>
#include <QMessageBox>

namespace ProjectExplorer {
namespace Internal {

DependenciesModel::DependenciesModel(Project *project, QObject *parent)
    : QAbstractListModel(parent)
    , m_project(project)
    , m_projects(ProjectManager::projects())
{
    // We can't select ourselves as a dependency
    m_projects.removeAll(m_project);

    ProjectManager *projectManager = ProjectManager::instance();
    connect(projectManager, SIGNAL(projectRemoved(ProjectExplorer::Project*)),
            this, SLOT(resetModel()));
    connect(projectManager, SIGNAL(projectAdded(ProjectExplorer::Project*)),
            this, SLOT(resetModel()));
    connect(projectManager, SIGNAL(sessionLoaded()),
            this, SLOT(resetModel()));
//    qDebug()<<"Dependencies Model"<<this<<"for project"<<project<<"("<<project->file()->fileName()<<")";
}

void DependenciesModel::resetModel()
{
    beginResetModel();
    m_projects = ProjectManager::projects();
    m_projects.removeAll(m_project);
    endResetModel();
}

int DependenciesModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : m_projects.isEmpty() ? 1 : m_projects.size();
}

int DependenciesModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : 1;
}

QVariant DependenciesModel::data(const QModelIndex &index, int role) const
{
    if (m_projects.isEmpty())
        return role == Qt::DisplayRole
            ? tr("<No other projects in this session>")
            : QVariant();

    const Project *p = m_projects.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return p->displayName();
    case Qt::CheckStateRole:
        return ProjectManager::hasDependency(m_project, p) ? Qt::Checked : Qt::Unchecked;
    case Qt::DecorationRole:
        return Core::FileIconProvider::icon(p->projectFilePath().toString());
    default:
        return QVariant();
    }
}

bool DependenciesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        Project *p = m_projects.at(index.row());
        const Qt::CheckState c = static_cast<Qt::CheckState>(value.toInt());

        if (c == Qt::Checked) {
            if (ProjectManager::addDependency(m_project, p)) {
                emit dataChanged(index, index);
                return true;
            } else {
                QMessageBox::warning(Core::Storage::mainWindow()->widget(), QCoreApplication::translate("DependenciesModel", "Unable to Add Dependency"),
                                     QCoreApplication::translate("DependenciesModel", "This would create a circular dependency."));
            }
        } else if (c == Qt::Unchecked) {
            if (ProjectManager::hasDependency(m_project, p)) {
                ProjectManager::removeDependency(m_project, p);
                emit dataChanged(index, index);
                return true;
            }
        }
    }
    return false;
}

Qt::ItemFlags DependenciesModel::flags(const QModelIndex &index) const
{
    if (m_projects.isEmpty())
        return Qt::NoItemFlags;

    Qt::ItemFlags rc = QAbstractListModel::flags(index);
    if (index.column() == 0)
        rc |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    return rc;
}

//
// DependenciesView
//
DependenciesView::DependenciesView(QWidget *parent)
    : QTreeView(parent)
{
    m_sizeHint = QSize(250, 250);
    setUniformRowHeights(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    setRootIsDecorated(false);
}

QSize DependenciesView::sizeHint() const
{
    return m_sizeHint;
}

void DependenciesView::setModel(QAbstractItemModel *newModel)
{
    if (QAbstractItemModel *oldModel = model()) {
        disconnect(oldModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(updateSizeHint()));
        disconnect(oldModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
                this, SLOT(updateSizeHint()));
        disconnect(oldModel, SIGNAL(modelReset()),
                this, SLOT(updateSizeHint()));
        disconnect(oldModel, SIGNAL(layoutChanged()),
                this, SLOT(updateSizeHint()));
    }

    QTreeView::setModel(newModel);

    if (newModel) {
        connect(newModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(updateSizeHint()));
        connect(newModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
                this, SLOT(updateSizeHint()));
        connect(newModel, SIGNAL(modelReset()),
                this, SLOT(updateSizeHint()));
        connect(newModel, SIGNAL(layoutChanged()),
                this, SLOT(updateSizeHint()));
    }
    updateSizeHint();
}

void DependenciesView::updateSizeHint()
{
    if (!model()) {
        m_sizeHint = QSize(250, 250);
        return;
    }

    int heightOffset = size().height() - viewport()->height();

    int heightPerRow = sizeHintForRow(0);
    if (heightPerRow == -1)
        heightPerRow = 30;
    int rows = qMin(qMax(model()->rowCount(), 2), 10);
    int height = rows * heightPerRow + heightOffset;
    if (m_sizeHint.height() != height) {
        m_sizeHint.setHeight(height);
        updateGeometry();
    }
}

//
// DependenciesWidget
//

DependenciesWidget::DependenciesWidget(Project *project, QWidget *parent)
    : QWidget(parent)
    , m_project(project)
    , m_model(new DependenciesModel(project, this))
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0, 0, 0, 0);
    m_detailsContainer = new Utils::DetailsWidget(this);
    m_detailsContainer->setState(Utils::DetailsWidget::NoSummary);
    vbox->addWidget(m_detailsContainer);

    QWidget *detailsWidget = new QWidget(m_detailsContainer);
    m_detailsContainer->setWidget(detailsWidget);
    QHBoxLayout *layout = new QHBoxLayout(detailsWidget);
    layout->setContentsMargins(0, -1, 0, -1);
    DependenciesView *treeView = new DependenciesView(this);
    treeView->setModel(m_model);
    treeView->setHeaderHidden(true);
    layout->addWidget(treeView);
    layout->addSpacerItem(new QSpacerItem(0, 0 , QSizePolicy::Expanding, QSizePolicy::Fixed));
}

} // namespace Internal
} // namespace ProjectExplorer
