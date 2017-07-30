/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
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

#ifndef BASEFILEFIND_H
#define BASEFILEFIND_H

#include <ifindfilter.h>
#include <filesearch.h>

#include "find_global.h"

#include <QtCore/QFutureWatcher>
#include <QtCore/QPointer>

#include <QStringListModel>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
QT_END_NAMESPACE

namespace Find {
    class SearchResultWindow;
    class SearchResultItem;
    class SearchResult;
}

namespace TextEditor {

class FIND_EXPORT BaseFileFind : public Find::IFindFilter, public ISearchProcess
{
    Q_OBJECT

public:
    explicit BaseFileFind(Find::SearchResultWindow *resultWindow);
    virtual ~BaseFileFind() {}

    virtual void cancelSearch() { cancel(); }

    bool isEnabled() const;
    bool canCancel() const;
    void cancel();
    bool isReplaceSupported() const { return true; }
    Find::SearchResult *findAll( const QString &txt, Find::FindFlags findFlags );
    void replaceAll(const QString &txt, Find::FindFlags findFlags);

    Find::SearchResultWindow *resultWindow() { return m_resultWindow; }

    /* returns the list of unique files that were passed in items */
    static QStringList replaceAll(const QString &text,
                                  const QList<Find::SearchResultItem> &items);

protected:
    virtual Utils::FileIterator *files() const = 0;
    void writeCommonSettings(QSettings *settings);
    void readCommonSettings(QSettings *settings, const QString &defaultFilter);
    QWidget *createPatternWidget();
    void syncComboWithSettings(QComboBox *combo, const QString &setting);
    void updateComboEntries(QComboBox *combo, bool onTop);
    QStringList fileNameFilters() const;

public slots:
    void openEditor(const Find::SearchResultItem &item);

private slots:
    void displayResult(int index);
    void searchFinished();
    void doReplace(const QString &txt,
                    const QList<Find::SearchResultItem> &items);

signals:
    void searchTextFinished();

private:
    QWidget *createProgressWidget();

    Find::SearchResultWindow *m_resultWindow;
    QFutureWatcher<Utils::FileSearchResultList> m_watcher;
    bool m_isSearching;
    QLabel *m_resultLabel;
    QStringListModel m_filterStrings;
    QString m_filterSetting;
    QPointer<QComboBox> m_filterCombo;
};

class FIND_EXPORT SimpleBaseFileFind: public BaseFileFind
{
public:
    SimpleBaseFileFind( Find::SearchResultWindow *resultWindow, Utils::FileIterator *filesList )
        : BaseFileFind(resultWindow)
        , m_files(filesList)
    {}
    virtual ~SimpleBaseFileFind() {}

    virtual QString id() const { return QStringLiteral("FindInCurrentFile"); }
    virtual QString displayName() const { return QStringLiteral("In Current Document"); }

protected:
    Utils::FileIterator *files() const { return m_files; }

private:
    Utils::FileIterator *m_files;
};

} // namespace TextEditor

#endif // BASEFILEFIND_H
