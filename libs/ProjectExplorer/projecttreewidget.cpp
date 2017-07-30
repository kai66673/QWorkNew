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

#include "projecttreewidget.h"
#include "IDocument.h"

#include "projectexplorerconstants.h"
#include "coreconstants.h"
#include "projectnodes.h"
#include "projectmodels.h"
#include "project.h"
#include "SessionManager.h"
#include "projectmodels.h"
#include "projectmanager.h"
#include "icontext.h"

//#include <coreplugin/actionmanager/command.h>
//#include <coreplugin/coreconstants.h>
//#include <coreplugin/icore.h>
//#include <coreplugin/editormanager/editormanager.h>
//#include <coreplugin/editormanager/ieditor.h>
//#include <coreplugin/find/itemviewfind.h>

#include <utils/navigationtreeview.h>
#include <utils/algorithm.h>

#include <QDebug>
#include <QSettings>

#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QToolButton>
#include <QAction>
#include <QMenu>

using namespace ProjectExplorer;
using namespace ProjectExplorer::Internal;

namespace {

class ProjectTreeItemDelegate : public QStyledItemDelegate
{
public:
    ProjectTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
    { }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItem opt = option;
        if (!index.data(Project::EnabledRole).toBool())
            opt.state &= ~QStyle::State_Enabled;
        QStyledItemDelegate::paint(painter, opt, index);
    }
};

bool debug = false;
}

class ProjectTreeView : public Utils::NavigationTreeView
{
public:
    ProjectTreeView()
    {
        setEditTriggers(QAbstractItemView::EditKeyPressed);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
        m_context = new Core::IContext(this);
        m_context->setContext(Core::Context(ProjectExplorer::Constants::C_PROJECT_TREE));
        m_context->setWidget(this);
//        ICore::addContextObject(m_context);
    }
    ~ProjectTreeView()
    {
//        ICore::removeContextObject(m_context);
        delete m_context;
    }

private:
    Core::IContext *m_context;
};

/*!
  /class ProjectTreeWidget

  Shows the projects in form of a tree.
  */
ProjectTreeWidget::ProjectTreeWidget(QWidget *parent)
        : QWidget(parent),
          m_view(0),
          m_model(0),
          m_filterProjectsAction(0),
          m_autoSync(false),
          m_autoExpand(true)
{
    m_model = new FlatModel(ProjectManager::sessionNode(), this);
    Project *pro = ProjectManager::startupProject();
    if (pro)
        m_model->setStartupProject(pro->rootProjectNode());

    m_view = new ProjectTreeView;
    m_view->setModel(m_model);
    m_view->setItemDelegate(new ProjectTreeItemDelegate(this));
    setFocusProxy(m_view);
    initView();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_view);
    setLayout(layout);

    m_filterProjectsAction = new QAction(tr("Simplify Tree"), this);
    m_filterProjectsAction->setCheckable(true);
    m_filterProjectsAction->setChecked(false); // default is the traditional complex tree
    connect(m_filterProjectsAction, SIGNAL(toggled(bool)), this, SLOT(setProjectFilter(bool)));

    m_filterGeneratedFilesAction = new QAction(tr("Hide Generated Files"), this);
    m_filterGeneratedFilesAction->setCheckable(true);
    m_filterGeneratedFilesAction->setChecked(true);
    connect(m_filterGeneratedFilesAction, SIGNAL(toggled(bool)), this, SLOT(setGeneratedFilesFilter(bool)));

    // connections
    connect(m_model, SIGNAL(modelReset()),
            this, SLOT(initView()));
    connect(m_view, SIGNAL(activated(QModelIndex)),
            this, SLOT(openItem(QModelIndex)));
    connect(m_view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(handleCurrentItemChange(QModelIndex)));
    connect(m_view, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    ProjectManager *projectManager = ProjectManager::instance();
//    connect(sessionManager, SIGNAL(singleProjectAdded(ProjectExplorer::Project*)),
//            this, SLOT(handleProjectAdded(ProjectExplorer::Project*)));
    connect(projectManager, SIGNAL(projectAdded(ProjectExplorer::Project*)),
            this, SLOT(handleProjectAdded(ProjectExplorer::Project*)));
    connect(projectManager, SIGNAL(startupProjectChanged(ProjectExplorer::Project*)),
            this, SLOT(startupProjectChanged(ProjectExplorer::Project*)));

    /// TODO:
//    connect(sessionManager, SIGNAL(aboutToLoadSession(QString)),
//            this, SLOT(disableAutoExpand()));
//    connect(sessionManager, SIGNAL(sessionLoaded(QString)),
//            this, SLOT(loadExpandData()));
//    connect(sessionManager, SIGNAL(aboutToSaveSession()),
//            this, SLOT(saveExpandData()));

    m_toggleSync = new QToolButton;
    m_toggleSync->setIcon(QIcon(QLatin1String(Core::Constants::ICON_LINK)));
    m_toggleSync->setCheckable(true);
    m_toggleSync->setChecked(autoSynchronization());
    m_toggleSync->setToolTip(tr("Synchronize with Editor"));
    connect(m_toggleSync, SIGNAL(clicked(bool)), this, SLOT(toggleAutoSynchronization()));

    setAutoSynchronization(true);
}

ProjectTreeWidget::~ProjectTreeWidget()
{ }

// returns how many nodes need to be expanded to make node visible
int ProjectTreeWidget::expandedCount(Node *node)
{
    QModelIndex index = m_model->indexForNode(node);
    if (!index.isValid())
        return 0;

    int count = 0;

    QModelIndex idx = index;
    while (idx.isValid() && idx != m_view->rootIndex()) {
        if (!m_view->isExpanded(idx))
            ++count;
        idx = m_model->parent(idx);
    }

    return count;
}

void ProjectTreeWidget::rowsInserted(const QModelIndex &parent, int, int)
{
    const QString &path = m_model->nodeForIndex(parent)->path();
    if (m_toExpand.contains(path)) {
        m_view->expand(parent);
        m_toExpand.remove(path);
    }
}

Node *ProjectTreeWidget::nodeForFile(const QString &fileName, Project *project)
{
    Node *bestNode = 0;
    int bestNodeExpandCount = INT_MAX;

    foreach (Node *node, ProjectManager::nodesForFile(fileName, project)) {
        if (!bestNode) {
            bestNode = node;
            bestNodeExpandCount = expandedCount(node->parentFolderNode());
        } else if (node->nodeType() < bestNode->nodeType()) {
            bestNode = node;
            bestNodeExpandCount = expandedCount(node->parentFolderNode());
        } else if (node->nodeType() == bestNode->nodeType()) {
            int nodeExpandCount = expandedCount(node->parentFolderNode());
            if (nodeExpandCount < bestNodeExpandCount) {
                bestNode = node;
                bestNodeExpandCount = expandedCount(node->parentFolderNode());
            }
        }
    }

    return bestNode;
}

void ProjectTreeWidget::disableAutoExpand()
{
    m_autoExpand = false;
}

void ProjectTreeWidget::loadExpandData()
{
    m_autoExpand = true;
    QSet<QString> data = ProjectManager::settings()->value(QLatin1String("ProjectTree.ExpandData")).toStringList().toSet();
    recursiveLoadExpandData(m_view->rootIndex(), data);

    // store remaning nodes to expand
    m_toExpand = data;
}

void ProjectTreeWidget::recursiveLoadExpandData(const QModelIndex &index, QSet<QString> &data)
{
    const QString &path = m_model->nodeForIndex(index)->path();
    if (data.contains(path)) {
        m_view->expand(index);
        data.remove(path);
        int count = m_model->rowCount(index);
        for (int i = 0; i < count; ++i)
            recursiveLoadExpandData(index.child(i, 0), data);
    }
}

void ProjectTreeWidget::saveExpandData()
{
    QStringList data;
    recursiveSaveExpandData(m_view->rootIndex(), &data);
    // TODO if there are multiple ProjectTreeWidgets, the last one saves the data
    ProjectManager::settings()->setValue(QLatin1String("ProjectTree.ExpandData"), data);
}

void ProjectTreeWidget::recursiveSaveExpandData(const QModelIndex &index, QStringList *data)
{
    Q_ASSERT(data);
    if (m_view->isExpanded(index) || index == m_view->rootIndex()) {
        // Note: We store the path of the node, which isn't unique for e.g. .pri files
        // but works for most nodes
        data->append(m_model->nodeForIndex(index)->path());
        int count = m_model->rowCount(index);
        for (int i = 0; i < count; ++i)
            recursiveSaveExpandData(index.child(i, 0), data);
    }
}

QToolButton *ProjectTreeWidget::toggleSync()
{
    return m_toggleSync;
}

void ProjectTreeWidget::toggleAutoSynchronization()
{
    setAutoSynchronization(!m_autoSync);
}

bool ProjectTreeWidget::autoSynchronization() const
{
    return m_autoSync;
}

void ProjectTreeWidget::setAutoSynchronization(bool sync, bool syncNow)
{
    m_toggleSync->setChecked(sync);
    if (sync == m_autoSync)
        return;

    m_autoSync = sync;

    if (debug)
        qDebug() << (m_autoSync ? "Enabling auto synchronization" : "Disabling auto synchronization");
    if (m_autoSync) {
        connect(ProjectManager::instance(), &ProjectManager::currentNodeChanged,
                this, &ProjectTreeWidget::setCurrentItem);
        if (syncNow)
            setCurrentItem(ProjectManager::currentNode(), ProjectManager::currentProject());
    } else {
        disconnect(ProjectManager::instance(), &ProjectManager::currentNodeChanged,
                   this, &ProjectTreeWidget::setCurrentItem);
    }
}

void ProjectTreeWidget::collapseAll()
{
    m_view->collapseAll();
}

void ProjectTreeWidget::editCurrentItem()
{
    if (m_view->selectionModel()->currentIndex().isValid())
        m_view->edit(m_view->selectionModel()->currentIndex());
}

void ProjectTreeWidget::setCurrentItem(Node *node, Project *project)
{
    if (debug)
        qDebug() << "ProjectTreeWidget::setCurrentItem(" << (project ? project->displayName() : QLatin1String("0"))
                 << ", " <<  (node ? node->path() : QLatin1String("0")) << ")";

    if (!project)
        return;

    const QModelIndex mainIndex = m_model->indexForNode(node);

    if (mainIndex.isValid()) {
        if (mainIndex != m_view->selectionModel()->currentIndex()) {
            m_view->setCurrentIndex(mainIndex);
            m_view->scrollTo(mainIndex);
        }
    } else {
        if (debug)
            qDebug() << "clear selection";
        m_view->clearSelection();
    }

}

void ProjectTreeWidget::handleCurrentItemChange(const QModelIndex &current)
{
    Node *node = m_model->nodeForIndex(current);
    // node might be 0. that's okay
    bool autoSync = autoSynchronization();
    setAutoSynchronization(false);
    ProjectManager::setCurrentNode(node);
    setAutoSynchronization(autoSync, false);
}

void ProjectTreeWidget::showContextMenu(const QPoint &pos)
{
    QModelIndex index = m_view->indexAt(pos);
    Node *node = m_model->nodeForIndex(index);
    ProjectManager::showContextMenu(this, m_view->mapToGlobal(pos), node);
}

void ProjectTreeWidget::handleProjectAdded(Project *project)
{
    Node *node = project->rootProjectNode();
    QModelIndex idx = m_model->indexForNode(node);
    if (m_autoExpand) // disabled while session restoring
        m_view->setExpanded(idx, true);
    m_view->setCurrentIndex(idx);

    connect(m_model, &FlatModel::rowsInserted,
            this, &ProjectTreeWidget::rowsInserted);
}

void ProjectTreeWidget::startupProjectChanged(Project *project)
{
    if (project) {
        ProjectNode *node = project->rootProjectNode();
        m_model->setStartupProject(node);
    } else {
        m_model->setStartupProject(0);
    }
}

void ProjectTreeWidget::initView()
{
    QModelIndex sessionIndex = m_model->index(0, 0);

    // hide root folder
    m_view->setRootIndex(sessionIndex);

    while (m_model->canFetchMore(sessionIndex))
        m_model->fetchMore(sessionIndex);

    // expand top level projects
    for (int i = 0; i < m_model->rowCount(sessionIndex); ++i)
        m_view->expand(m_model->index(i, 0, sessionIndex));

    setCurrentItem(ProjectManager::currentNode(), ProjectManager::currentProject());
}

void ProjectTreeWidget::openItem(const QModelIndex &mainIndex)
{
    Node *node = m_model->nodeForIndex(mainIndex);
    if (node->nodeType() != FileNodeType)
        return;
    DocumentManager *documentManager = DocumentManager::getInstance();
    if ( IDocument *document = documentManager->openDocument(node->path()) )
        if ( node->line() >= 0 )
            document->editor()->gotoLine(node->line());
}

void ProjectTreeWidget::setProjectFilter(bool filter)
{
    m_model->setProjectFilterEnabled(filter);
    m_filterProjectsAction->setChecked(filter);
}

void ProjectTreeWidget::setGeneratedFilesFilter(bool filter)
{
    m_model->setGeneratedFilesFilterEnabled(filter);
    m_filterGeneratedFilesAction->setChecked(filter);
}

bool ProjectTreeWidget::generatedFilesFilter()
{
    return m_model->generatedFilesFilterEnabled();
}

bool ProjectTreeWidget::projectFilter()
{
    return m_model->projectFilterEnabled();
}
