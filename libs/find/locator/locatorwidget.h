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

#ifndef LOCATORWIDGET_H
#define LOCATORWIDGET_H

#include"LocatorManager.h"
#include "ilocatorfilter.h"

#include <QWidget>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QLineEdit;
class QMenu;
class QTreeView;
QT_END_NAMESPACE

namespace Utils { class FancyLineEdit; }

namespace Core {
namespace Internal {

class LocatorModel;
class CompletionList;

class LocatorWidget
  : public QWidget
{
    Q_OBJECT

public:
    explicit LocatorWidget();

    void updateFilterList();

    void show(const QString &text, int selectionStart = -1, int selectionLength = 0);

    void setPlaceholderText(const QString &text);

    void startNewSearch();
    bool isPopupShown() const;

private slots:
    void showPopup();
    void showPopupNow();
    void acceptCurrentEntry();
    void filterSelected();
    void showConfigureDialog();
    void updateEntries();
    void scheduleAcceptCurrentEntry();
    void setFocusToCurrentMode();

private:
    bool eventFilter(QObject *obj, QEvent *event);

    void showEvent(QShowEvent *e);

    void showCompletionList();
    void updateCompletionList(const QString &text);
    QList<ILocatorFilter*> filtersFor(const QString &text, QString &searchText);

    LocatorManager *m_locatorManager;
    LocatorModel *m_locatorModel;

    CompletionList *m_completionList;
    QMenu *m_filterMenu;
    QAction *m_refreshAction;
//    QAction *m_configureAction;
    Utils::FancyLineEdit *m_fileLineEdit;
    QTimer *m_showPopupTimer;
    QFutureWatcher<LocatorFilterEntry> *m_entriesWatcher;
    QMap<Core::Id, QAction *> m_filterActionMap;
    QString m_requestedCompletionText;
    bool m_updateRequested;
    bool m_acceptRequested;
    bool m_possibleToolTipRequest;
};

} // namespace Internal
} // namespace Core

#endif // LOCATORWIDGET_H
