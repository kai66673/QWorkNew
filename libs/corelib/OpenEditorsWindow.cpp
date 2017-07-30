/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "OpenEditorsWindow.h"

#include <utils/qtcassert.h>

#include <QFocusEvent>
#include <QHeaderView>
#include <QListWidget>
#include <QVBoxLayout>

#include "IDocument.h"

const int WIDTH = 480;
const int HEIGHT = 320;

OpenEditorsWindow::OpenEditorsWindow(QWidget *parent) :
    QFrame(parent, Qt::Popup),
    m_editorList(new QListWidget(this))
{
    resize(QSize(WIDTH, HEIGHT));
    m_editorList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_editorList->setTextElideMode(Qt::ElideMiddle);
    m_editorList->installEventFilter(this);

    m_editorList->setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(m_editorList);

    connect(m_editorList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(editorClicked(QListWidgetItem*)));
}

void OpenEditorsWindow::selectAndHide()
{
    setVisible(false);
    selectEditor(m_editorList->currentItem());
}

void OpenEditorsWindow::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if (visible)
        setFocus();
}

bool OpenEditorsWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_editorList) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *ke = static_cast<QKeyEvent*>(e);
            if ( ke->key() == Qt::Key_Tab && ke->modifiers() == Qt::ControlModifier ) {
                selectUpDown(false);
                return true;
            } else if ( ke->key() == Qt::Key_Backtab && ke->modifiers() & Qt::ControlModifier ) {
                selectUpDown(true);
                return true;
            }
            if (ke->key() == Qt::Key_Escape) {
                setVisible(false);
                return true;
            }
            if (ke->key() == Qt::Key_Return) {
                selectEditor(m_editorList->currentItem());
                return true;
            }
        } else if (e->type() == QEvent::KeyRelease) {
            QKeyEvent *ke = static_cast<QKeyEvent*>(e);
            if (ke->modifiers() == 0
                    /*HACK this is to overcome some event inconsistencies between platforms*/
                    || (ke->modifiers() == Qt::AltModifier
                    && (ke->key() == Qt::Key_Alt || ke->key() == -1))) {
                selectAndHide();
            }
        }
    }
    return QWidget::eventFilter(obj, e);
}

void OpenEditorsWindow::focusInEvent(QFocusEvent *)
{
    m_editorList->setFocus();
}

void OpenEditorsWindow::selectUpDown(bool up)
{
    int itemCount = m_editorList->count();
    if (itemCount < 2)
        return;
    int index = m_editorList->currentRow();
    if (index < 0)
        return;
    QListWidgetItem *editor = 0;
    int count = 0;
    while (!editor && count < itemCount) {
        if (up) {
            index--;
            if (index < 0)
                index = itemCount-1;
        } else {
            index++;
            if (index >= itemCount)
                index = 0;
        }
        editor = m_editorList->item(index);
        count++;
    }
    if (editor) {
        m_editorList->setCurrentItem(editor);
        ensureCurrentVisible();
    }
}

void OpenEditorsWindow::selectPreviousEditor()
{
    selectUpDown(false);
}

void OpenEditorsWindow::selectNextEditor()
{
    selectUpDown(true);
}

void OpenEditorsWindow::setEditors( const QList<IDocument *> &documents )
{
    m_editorList->clear();

    if ( documents.empty() )
        return;

    IDocument *activeDocument = DocumentManager::getInstance()->currentDocument();
    QListWidgetItem *activeItem = 0;

    foreach (IDocument *document, documents) {
        QListWidgetItem *newItem = new QListWidgetItem( document->isNew() ? document->title() : document->filePath());
        newItem->setData(Qt::UserRole, QVariant::fromValue(document));
        if ( document == activeDocument )
            activeItem = newItem;
        m_editorList->addItem(newItem);
    }

    if ( activeItem ) {
        m_editorList->setCurrentItem(activeItem);
        ensureCurrentVisible();
    }
}


void OpenEditorsWindow::selectEditor(QListWidgetItem *item)
{
    if (!item)
        return;
    if (IDocument *document = item->data(Qt::UserRole).value<IDocument*>()) {
        DocumentManager::getInstance()->activateDocument(document, !isVisible());
    }
}

void OpenEditorsWindow::editorClicked(QListWidgetItem *item)
{
    selectEditor(item);
    setFocus();
}


void OpenEditorsWindow::ensureCurrentVisible()
{
    m_editorList->scrollTo(m_editorList->currentIndex(), QAbstractItemView::PositionAtCenter);
}
