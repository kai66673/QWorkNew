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

#include "opendocumentsfilter.h"

#include <utils/qtcfileutils.h>

#include <QFileInfo>
#include <QMutexLocker>

using namespace Core;
using namespace Core;
using namespace Core::Internal;
using namespace Utils;

OpenDocumentsFilter::OpenDocumentsFilter()
{
    setId("Open documents");
    setDisplayName(tr("Open Documents"));
    setShortcutString(QString(QLatin1Char('o')));
    setIncludedByDefault(true);

//    connect(DocumentManager::getInstance(), SIGNAL(editorOpened(Core::IEditor*)),
//            this, SLOT(refreshInternally()));
//    connect(DocumentManager::getInstance(), SIGNAL(editorClosed(Core::IEditor*)),
//            this, SLOT(refreshInternally()));
}

QList<LocatorFilterEntry> OpenDocumentsFilter::matchesFor(QFutureInterface<Core::LocatorFilterEntry> &future, const QString &entry_)
{
    QList<LocatorFilterEntry> goodEntries;
    QList<LocatorFilterEntry> betterEntries;
    QString entry = entry_;
//    const QString lineNoSuffix = EditorManager::splitLineNumber(&entry);
    const QChar asterisk = QLatin1Char('*');
    QString pattern = QString(asterisk);
    pattern += entry;
    pattern += asterisk;
    QRegExp regexp(pattern, Qt::CaseInsensitive, QRegExp::Wildcard);
    if (!regexp.isValid())
        return goodEntries;
    const Qt::CaseSensitivity caseSensitivityForPrefix = caseSensitivity(entry);
    foreach (IDocument *document, documents()) {
        if (document->isNew())
            continue;
        if (future.isCanceled())
            break;
        QString fileName = document->filePath();
        if (fileName.isEmpty())
            continue;
        QString displayName = document->title();
        if (regexp.exactMatch(displayName)) {
            QFileInfo fi(fileName);
            LocatorFilterEntry fiEntry(this, displayName, QString(fileName /*+ lineNoSuffix*/));
            fiEntry.extraInfo = FileUtils::shortNativePath(FileName(fi));
            fiEntry.fileName = fileName;
            QList<LocatorFilterEntry> &category = displayName.startsWith(entry, caseSensitivityForPrefix)
                    ? betterEntries : goodEntries;
            category.append(fiEntry);
        }
    }
    betterEntries.append(goodEntries);
    return betterEntries;
}

QList<IDocument *> OpenDocumentsFilter::documents() const
{
//    QMutexLocker lock(&m_mutex); Q_UNUSED(lock)
    return DocumentManager::getInstance()->documentsOrdered();
}

void OpenDocumentsFilter::refresh(QFutureInterface<void> &future)
{
    Q_UNUSED(future)
}

void OpenDocumentsFilter::accept(LocatorFilterEntry selection) const
{
    DocumentManager::getInstance()->openDocument(selection.internalData.toString());
}
