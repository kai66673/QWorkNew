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

#include "kitmodel.h"

#include "kit.h"
#include "kitmanagerconfigwidget.h"
#include "kitmanager.h"

#include <coreconstants.h>
#include <utils/algorithm.h>
#include <utils/qtcassert.h>

#include <QApplication>
#include <QLayout>

namespace ProjectExplorer {
namespace Internal {

class KitNode
{
public:
    KitNode(KitNode *kn) :
        parent(kn), widget(0)
    {
        if (kn)
            kn->childNodes.append(this);
    }

    KitNode(KitNode *kn, Kit *k) :
        parent(kn), widget(0)
    {
        widget = KitManager::createConfigWidget(k);
        if (widget) {
            if (k && k->isAutoDetected())
                widget->makeStickySubWidgetsReadOnly();
            widget->setVisible(false);
        }

        if (kn)
            kn->childNodes.append(this);
    }

    ~KitNode()
    {
        if (parent)
            parent->childNodes.removeOne(this);
        delete widget;

        // deleting a child removes it from childNodes
        // so operate on a temporary list
        QList<KitNode *> tmp = childNodes;
        qDeleteAll(tmp);
    }

    KitNode *parent;
    QList<KitNode *> childNodes;
    KitManagerConfigWidget *widget;
};

// --------------------------------------------------------------------------
// KitModel
// --------------------------------------------------------------------------

KitModel::KitModel(QBoxLayout *parentLayout, QObject *parent) :
    QAbstractItemModel(parent),
    m_parentLayout(parentLayout),
    m_defaultNode(0),
    m_keepUnique(true)
{
    m_root = new KitNode(0);
    m_autoRoot = new KitNode(m_root);
    m_manualRoot = new KitNode(m_root);

    foreach (Kit *k, KitManager::sortedKits())
        addKit(k);

    changeDefaultKit();

    connect(KitManager::instance(), SIGNAL(kitAdded(ProjectExplorer::Kit*)),
            this, SLOT(addKit(ProjectExplorer::Kit*)));
    connect(KitManager::instance(), SIGNAL(kitUpdated(ProjectExplorer::Kit*)),
            this, SLOT(updateKit(ProjectExplorer::Kit*)));
    connect(KitManager::instance(), SIGNAL(unmanagedKitUpdated(ProjectExplorer::Kit*)),
            this, SLOT(updateKit(ProjectExplorer::Kit*)));
    connect(KitManager::instance(), SIGNAL(kitRemoved(ProjectExplorer::Kit*)),
            this, SLOT(removeKit(ProjectExplorer::Kit*)));
    connect(KitManager::instance(), SIGNAL(defaultkitChanged()),
            this, SLOT(changeDefaultKit()));
}

KitModel::~KitModel()
{
    delete m_root;
}

QModelIndex KitModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        if (row >= 0 && row < m_root->childNodes.count())
            return createIndex(row, column, m_root->childNodes.at(row));
    }
    KitNode *node = static_cast<KitNode *>(parent.internalPointer());
    if (row < node->childNodes.count() && column == 0)
        return createIndex(row, column, node->childNodes.at(row));
    else
        return QModelIndex();
}

QModelIndex KitModel::parent(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return QModelIndex();
    KitNode *node = static_cast<KitNode *>(idx.internalPointer());
    if (node->parent == m_root)
        return QModelIndex();
    return index(node->parent);
}

int KitModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_root->childNodes.count();
    KitNode *node = static_cast<KitNode *>(parent.internalPointer());
    return node->childNodes.count();
}

int KitModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant KitModel::data(const QModelIndex &index, int role) const
{
    static QIcon warningIcon(QString::fromLatin1(Core::Constants::ICON_WARNING));
    static QIcon errorIcon(QString::fromLatin1(Core::Constants::ICON_ERROR));

    if (!index.isValid() || index.column() != 0)
        return QVariant();

    KitNode *node = static_cast<KitNode *>(index.internalPointer());
    QTC_ASSERT(node, return QVariant());
    if (node == m_autoRoot && role == Qt::DisplayRole)
        return tr("Auto-detected");
    if (node == m_manualRoot && role == Qt::DisplayRole)
        return tr("Manual");
    if (node->widget) {
        if (role == Qt::FontRole) {
            QFont f = QApplication::font();
            if (node->widget->isDirty())
                f.setBold(!f.bold());
            if (node == m_defaultNode)
                f.setItalic(f.style() != QFont::StyleItalic);
            return f;
        } else if (role == Qt::DisplayRole) {
            QString baseName = node->widget->displayName();
            if (node == m_defaultNode)
                //: Mark up a kit as the default one.
                baseName = tr("%1 (default)").arg(baseName);
            return baseName;
        } else if (role == Qt::DecorationRole) {
            if (!node->widget->isValid())
                return errorIcon;
            if (node->widget->hasWarning())
                return warningIcon;
            return QIcon();
        } else if (role == Qt::ToolTipRole) {
            return node->widget->validityMessage();
        }
    }
    return QVariant();
}

Qt::ItemFlags KitModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    KitNode *node = static_cast<KitNode *>(index.internalPointer());
    if (!node->widget)
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant KitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Name");
    return QVariant();
}

Kit *KitModel::kit(const QModelIndex &index)
{
    if (!index.isValid())
        return 0;
    KitNode *node = static_cast<KitNode *>(index.internalPointer());
    return node->widget->workingCopy();
}

QModelIndex KitModel::indexOf(Kit *k) const
{
    KitNode *n = findWorkingCopy(k);
    return n ? index(n) : QModelIndex();
}

void KitModel::setDefaultKit(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    KitNode *node = static_cast<KitNode *>(index.internalPointer());
    if (node->widget)
        setDefaultNode(node);
}

bool KitModel::isDefaultKit(const QModelIndex &index)
{
    return m_defaultNode == static_cast<KitNode *>(index.internalPointer());
}

KitManagerConfigWidget *KitModel::widget(const QModelIndex &index)
{
    if (!index.isValid())
        return 0;
    KitNode *node = static_cast<KitNode *>(index.internalPointer());
    return node->widget;
}

bool KitModel::isDirty() const
{
    return Utils::anyOf(m_manualRoot->childNodes, [](KitNode *n) {
        return n->widget->isDirty();
    });
}

bool KitModel::isDirty(Kit *k) const
{
    KitNode *n = findWorkingCopy(k);
    return n ? n->widget->isDirty() : false;
}

void KitModel::setDirty()
{
    KitManagerConfigWidget *w = qobject_cast<KitManagerConfigWidget *>(sender());
    QList<KitNode *> nodes = m_manualRoot->childNodes;
    nodes << m_autoRoot->childNodes;
    foreach (KitNode *n, nodes) {
        if (n->widget == w)
            emit dataChanged(index(n, 0), index(n, columnCount(QModelIndex())));
    }
}

void KitModel::validateKitNames()
{
    QList<KitNode *> nodes = m_manualRoot->childNodes;
    nodes << m_autoRoot->childNodes;
    QHash<QString, int> nameHash;
    foreach (KitNode *n, nodes) {
        const QString displayName = n->widget->displayName();
        if (nameHash.contains(displayName))
            ++nameHash[displayName];
        else
            nameHash.insert(displayName, 1);
    }

    foreach (KitNode *n, nodes) {
        const QString displayName = n->widget->displayName();
        n->widget->setHasUniqueName(nameHash.value(displayName) == 1);
    }
}

void KitModel::apply()
{
    // Remove unused kits:
    QList<KitNode *> nodes = m_toRemoveList;
    foreach (KitNode *n, nodes) {
        Q_ASSERT(!n->parent);
        n->widget->removeKit();
    }

    // Update kits:
    nodes = m_autoRoot->childNodes; // These can be dirty due to being made default!
    nodes.append(m_manualRoot->childNodes);
    foreach (KitNode *n, nodes) {
        Q_ASSERT(n);
        Q_ASSERT(n->widget);
        if (n->widget->isDirty()) {
            n->widget->apply();
            emit dataChanged(index(n, 0), index(n, columnCount(QModelIndex())));
        }
    }
}

void KitModel::markForRemoval(Kit *k)
{
    KitNode *node = findWorkingCopy(k);
    if (!node)
        return;

    if (node == m_defaultNode) {
        KitNode *newDefault = 0;
        if (!m_autoRoot->childNodes.isEmpty())
            newDefault = m_autoRoot->childNodes.at(0);
        else if (!m_manualRoot->childNodes.isEmpty())
            newDefault = m_manualRoot->childNodes.at(0);
        setDefaultNode(newDefault);
    }

    beginRemoveRows(index(m_manualRoot), m_manualRoot->childNodes.indexOf(node), m_manualRoot->childNodes.indexOf(node));
    m_manualRoot->childNodes.removeOne(node);
    node->parent = 0;
    if (node->widget->configures(0))
        delete node;
    else
        m_toRemoveList.append(node);
    endRemoveRows();
}

Kit *KitModel::markForAddition(Kit *baseKit)
{
    int pos = m_manualRoot->childNodes.size();
    beginInsertRows(index(m_manualRoot), pos, pos);

    KitNode *node = createNode(m_manualRoot, 0);
    Kit *k = node->widget->workingCopy();
    KitGuard g(k);
    if (baseKit) {
        k->copyFrom(baseKit);
        k->setAutoDetected(false); // Make sure we have a manual kit!
        k->setSdkProvided(false);
        k->setUnexpandedDisplayName(tr("Clone of %1").arg(k->unexpandedDisplayName()));
    } else {
        k->setup();
    }

    if (!m_defaultNode)
        setDefaultNode(node);

    endInsertRows();

    return k;
}

QModelIndex KitModel::index(KitNode *node, int column) const
{
    if (node->parent == 0) // is root (or was marked for deletion)
        return QModelIndex();
    else if (node->parent == m_root)
        return index(m_root->childNodes.indexOf(node), column, QModelIndex());
    else
        return index(node->parent->childNodes.indexOf(node), column, index(node->parent));
}

KitNode *KitModel::findWorkingCopy(Kit *k) const
{
    auto compareWorkingCopy = [&k](KitNode *n){ return n->widget->workingCopy() == k; };

    KitNode *n = Utils::findOrDefault(m_autoRoot->childNodes, compareWorkingCopy);
    if (!n)
        n = Utils::findOrDefault(m_manualRoot->childNodes, compareWorkingCopy);
    return n;
}

KitNode *KitModel::createNode(KitNode *parent, Kit *k)
{
    KitNode *node = new KitNode(parent, k);
    m_parentLayout->addWidget(node->widget);
    connect(node->widget, SIGNAL(dirty()), this, SLOT(setDirty()));
    return node;
}

void KitModel::setDefaultNode(KitNode *node)
{
    if (m_defaultNode) {
        m_defaultNode->widget->setIsDefaultKit(false);
        emit dataChanged(index(m_defaultNode), index(m_defaultNode));
    }
    m_defaultNode = node;
    if (m_defaultNode) {
        m_defaultNode->widget->setIsDefaultKit(true);
        emit dataChanged(index(m_defaultNode), index(m_defaultNode));
    }
}

QList<Kit *> KitModel::kitList(KitNode *node) const
{
    QList<Kit *> result;
    if (!node)
        return result;
    foreach (KitNode *n, node->childNodes)
        result.append(kitList(n));
    if (node->widget)
        result.append(node->widget->workingCopy());
    return result;
}

void KitModel::addKit(Kit *k)
{
    foreach (KitNode *n, m_manualRoot->childNodes) {
        // Was added by us
        if (n->widget->configures(k))
            return;
    }

    KitNode *parent = m_manualRoot;
    if (k->isAutoDetected())
        parent = m_autoRoot;
    int row = parent->childNodes.count();

    beginInsertRows(index(parent), row, row);
    createNode(parent, k);
    endInsertRows();

    validateKitNames();
    emit kitStateChanged();
}

void KitModel::updateKit(Kit *)
{
    validateKitNames();
    emit kitStateChanged();
}

void KitModel::removeKit(Kit *k)
{
    QList<KitNode *> nodes = m_toRemoveList;
    foreach (KitNode *n, nodes) {
        if (n->widget->configures(k)) {
            m_toRemoveList.removeOne(n);
            if (m_defaultNode == n)
                m_defaultNode = 0;
            delete n;
            return;
        }
    }

    KitNode *parent = m_manualRoot;
    if (k->isAutoDetected())
        parent = m_autoRoot;
    int row = 0;
    KitNode *node = 0;
    foreach (KitNode *current, parent->childNodes) {
        if (current->widget->configures(k)) {
            node = current;
            break;
        }
        ++row;
    }

    beginRemoveRows(index(parent), row, row);
    parent->childNodes.removeAt(row);
    if (m_defaultNode == node)
        m_defaultNode = 0;
    endRemoveRows();
    delete node;

    validateKitNames();
    emit kitStateChanged();
}

void KitModel::changeDefaultKit()
{
    Kit *defaultKit = KitManager::defaultKit();
    QList<KitNode *> nodes = m_autoRoot->childNodes;
    nodes << m_manualRoot->childNodes;
    foreach (KitNode *n, nodes) {
        if (n->widget->configures(defaultKit)) {
            setDefaultNode(n);
            break;
        }
    }
}

} // namespace Internal
} // namespace ProjectExplorer
