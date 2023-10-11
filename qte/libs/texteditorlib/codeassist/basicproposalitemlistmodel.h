/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef BASICPROPOSALITEMLISTMODEL_H
#define BASICPROPOSALITEMLISTMODEL_H

#include "igenericproposalmodel.h"

#include <texteditor_global.h>
#include <qtcoverride.h>

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QPair>

namespace TextEditor {

class BasicProposalItem;

class TEXTEDITOR_EXPORT BasicProposalItemListModel : public IGenericProposalModel
{
public:
    BasicProposalItemListModel();
    BasicProposalItemListModel(const QList<BasicProposalItem *> &items);
    ~BasicProposalItemListModel();

    void reset() QTC_OVERRIDE;
    int size() const QTC_OVERRIDE;
    QString text(int index) const QTC_OVERRIDE;
    QIcon icon(int index) const QTC_OVERRIDE;
    QString detail(int index) const QTC_OVERRIDE;
    int persistentId(int index) const QTC_OVERRIDE;
    void removeDuplicates() QTC_OVERRIDE;
    void filter(const QString &prefix) QTC_OVERRIDE;
    bool isSortable(const QString &prefix) const QTC_OVERRIDE;
    void sort(const QString &prefix) QTC_OVERRIDE;
    bool supportsPrefixExpansion() const QTC_OVERRIDE;
    QString proposalPrefix() const QTC_OVERRIDE;
    bool keepPerfectMatch(AssistReason reason) const QTC_OVERRIDE;
    IAssistProposalItem *proposalItem(int index) const QTC_OVERRIDE;

    void loadContent(const QList<BasicProposalItem *> &items);
    void setSortingAllowed(bool isAllowed);
    bool isSortingAllowed() const;

protected:
    typedef QList<BasicProposalItem *>::iterator ItemIterator;
    QPair<ItemIterator, ItemIterator> currentItems();
    QList<BasicProposalItem *> m_currentItems;

private:
    void mapPersistentIds();

    QHash<QString, int> m_idByText;
    QList<BasicProposalItem *> m_originalItems;
};

} // TextEditor

#endif // BASICPROPOSALITEMLISTMODEL_H
