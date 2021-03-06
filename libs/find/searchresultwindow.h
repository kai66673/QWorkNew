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

#ifndef SEARCHRESULTWINDOW_H
#define SEARCHRESULTWINDOW_H

#include "find_global.h"

#include <ioutputpane.h>

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtGui/QIcon>

#include "SearchEngine.h"

QT_BEGIN_NAMESPACE
class QFont;
QT_END_NAMESPACE

namespace Find {
namespace Internal {
    class SearchResultTreeView;
    struct SearchResultWindowPrivate;
}
class SearchResultWindow;

class FIND_EXPORT SearchResultItem
{
public:
    SearchResultItem()
        : textMarkPos(-1)
        , textMarkLength(0)
        , lineNumber(-1)
        , useTextEditorFont(false)
    {
    }

    SearchResultItem(const SearchResultItem &other)
        : path(other.path)
        , text(other.text)
        , textMarkPos(other.textMarkPos)
        , textMarkLength(other.textMarkLength)
        , icon(other.icon)
        , lineNumber(other.lineNumber)
        , useTextEditorFont(other.useTextEditorFont)
        , userData(other.userData)
        , qualifiedName(other.qualifiedName)
    {
    }

    QStringList path; // hierarchy to the parent item of this item
    QString text; // text to show for the item itself
    int textMarkPos; // 0-based starting position for a mark (-1 for no mark)
    int textMarkLength; // length of the mark (0 for no mark)
    QIcon icon; // icon to show in front of the item (by be null icon to hide)
    int lineNumber; // (0 or -1 for no line number)
    bool useTextEditorFont;
    QVariant userData; // user data for identification of the item
    QString qualifiedName;
};

class FIND_EXPORT SearchResult : public QObject
{
    Q_OBJECT

signals:
    void activated(const Find::SearchResultItem &item);
    void replaceButtonClicked(const QString &replaceText, const QList<Find::SearchResultItem> &checkedItems);

    friend class SearchResultWindow;
};

class FIND_EXPORT SearchResultWindow : public Core::IOutputPane
{
    Q_OBJECT

public:
    enum SearchMode {
        SearchOnly,
        SearchAndReplace
    };

    enum AddMode {
        AddSorted,
        AddOrdered
    };

    SearchResultWindow( ISearchProcess *search = 0 );
    virtual ~SearchResultWindow();
    static SearchResultWindow *instance();

    void setSearchProcess( ISearchProcess *search ) { m_searchProcess = search; }

    QWidget *outputWidget(QWidget *);
    QList<QAction*> toolBarActions() const;

    QString displayName() const { return tr("Search Results"); }
    int priorityInStatusBar() const;
    void visibilityChanged(bool visible);
    bool isEmpty() const;
    int numberOfResults() const;
    bool hasFocus();
    bool canFocus();
    void setFocus();

    bool canNext();
    bool canPrevious();
    void goToNext();
    void goToPrev();
    bool canNavigate();

    void setTextEditorFont(const QFont &font);

    void setTextToReplace(const QString &textToReplace);
    QString textToReplace() const;

    // search result object only lives till next startnewsearch call
    SearchResult *startNewSearch(SearchMode searchOrSearchAndReplace = SearchOnly);

    void addResults(QList<SearchResultItem> &items, AddMode mode);
    bool isSearchFinished() const;

    QList<SearchResultItem> checkedItems() const;

    ISearchProcess *searchProcess() { return m_searchProcess; }
    bool cancelSearch();

public slots:
    void clearContents();
    void addResult(const QString &fileName, int lineNumber, const QString &lineText,
                   int searchTermStart, int searchTermLength, const QVariant &userData = QVariant());
    void finishSearch();

    void onToggleExpandCollapseAll( bool checked );

private slots:
    void handleExpandCollapseToolButton(bool checked);
    void handleJumpToSearchResult(const SearchResultItem &item);
    void handleReplaceButton();
    void showNoMatchesFound();

private:
    void setShowReplaceUI(bool show);
    void readSettings();
    void writeSettings();

    Internal::SearchResultWindowPrivate *d;
    static SearchResultWindow *m_instance;

    ISearchProcess *m_searchProcess;
};

} // namespace Find

Q_DECLARE_METATYPE(Find::SearchResultItem)

#endif // SEARCHRESULTWINDOW_H
