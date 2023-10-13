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

#ifndef ITEXTEDITOR_H
#define ITEXTEDITOR_H

#include "corelib_global.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QTextDocument>
#include <QTextCursor>

QT_BEGIN_NAMESPACE
class QIcon;
class QMenu;
class QPainter;
class QPoint;
class QRect;
class QTextBlock;
QT_END_NAMESPACE

namespace TextEditor {

class ITextEditor;

class CORELIB_EXPORT ITextMark : public QObject
{
    Q_OBJECT
public:
    ITextMark(QObject *parent = 0) : QObject(parent) {}

    // determine order on markers on the same line.
    enum Priority
    {
        LowPriority,
        NormalPriority,
        HighPriority // shown on top.
    };

    virtual void paint(QPainter *painter, const QRect &rect) const;
    virtual void updateLineNumber(int lineNumber);
    virtual void updateBlock(const QTextBlock &block);
    virtual void removedFromEditor();
    virtual void documentClosing();
    virtual void setIcon(const QIcon &icon);
    virtual Priority priority() const;
    virtual void setPriority(Priority prioriy);
    virtual double widthFactor() const;

private:
    QIcon m_icon;
    Priority m_priority;
};

typedef QList<ITextMark *> TextMarks;


class CORELIB_EXPORT ITextMarkable : public QObject
{
    Q_OBJECT
public:
    ITextMarkable(QObject *parent = 0) : QObject(parent) {}

    virtual bool addMark(ITextMark *mark, int line) = 0;
    virtual TextMarks marksAt(int line) const = 0;
    virtual void removeMark(ITextMark *mark) = 0;
    virtual bool hasMark(ITextMark *mark) const = 0;
    virtual void updateMark(ITextMark *mark) = 0;
};

class CORELIB_EXPORT ITextFindSupport
{
public:
    virtual QTextDocument *document() const = 0;
    virtual QTextCursor textCursor() const = 0;
    virtual void setTextCursor(const QTextCursor& cursor) = 0;
    virtual bool isReadOnly() const = 0;
};

} // namespace TextEditor

#endif // ITEXTEDITOR_H
