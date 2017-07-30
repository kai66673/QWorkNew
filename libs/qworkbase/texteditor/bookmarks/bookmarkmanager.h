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

#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <icontext.h>
//#include <coreplugin/inavigationwidgetfactory.h>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QMultiMap>
#include <QtCore/QList>
#include <QListView>
#include <QPixmap>
#include <QStyledItemDelegate>
#include <QDomDocument>

namespace Core {
class IEditor;
}

namespace TextEditor {
class ITextEditor;
}

namespace Bookmarks {
namespace Internal {

class Bookmark;
class BookmarkContext;

class BookmarkManager : public QAbstractItemModel
{
    Q_OBJECT

public:
    BookmarkManager();
    ~BookmarkManager();

    QIcon bookmarkIcon() const { return m_bookmarkIcon; }

    void updateBookmark(Bookmark *bookmark);
    void removeBookmark(Bookmark *bookmark); // Does not remove the mark
    void removeAllBookmarks();
    Bookmark *bookmarkForIndex(QModelIndex index);

    enum State { NoBookMarks, HasBookMarks, HasBookmarksInDocument };
    State state() const;

    // Model stuff
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // this QItemSelectionModel is shared by all views
    QItemSelectionModel *selectionModel() const;

    enum Roles {
        Filename = Qt::UserRole,
        LineNumber = Qt::UserRole + 1,
        Directory = Qt::UserRole + 2,
        LineText = Qt::UserRole + 3
    };

    void saveBookmarks( QDomDocument &document, QDomElement *root );
    void loadBookmarks( QDomElement *root );

public slots:
    void toggleBookmark();
    void toggleBookmark(const QString &fileName, int lineNumber);
    void nextInDocument();
    void prevInDocument();
    void next();
    void prev();
    void moveUp();
    void moveDown();
    bool gotoBookmark(Bookmark *bookmark);

signals:
    void updateActions(int state);
    void currentIndexChanged(const QModelIndex &);

private slots:
    void updateActionStatus();

private:
    TextEditor::ITextEditor *currentTextEditor() const;

    void documentPrevNext(bool next);

    Bookmark* findBookmark(const QString &path, const QString &fileName, int lineNumber);
    void addBookmark(Bookmark *bookmark, bool userset = true);
    void addBookmark(const QString &s);
    static QString bookmarkToString(const Bookmark *b);

    typedef QMultiMap<QString, Bookmark *> FileNameBookmarksMap;
    typedef QMap<QString, FileNameBookmarksMap *> DirectoryFileBookmarksMap;

    DirectoryFileBookmarksMap m_bookmarksMap;

    const QIcon m_bookmarkIcon;

    QList<Bookmark *> m_bookmarksList;
    QItemSelectionModel *m_selectionModel;
};

class BookmarkView : public QListView
{
    Q_OBJECT
public:
    BookmarkView(QWidget *parent = 0);
    ~BookmarkView();
    void setModel(QAbstractItemModel *model);
public slots:
    void gotoBookmark(const QModelIndex &index);
protected slots:
    void removeFromContextMenu();
    void removeAll();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void removeBookmark(const QModelIndex &index);
private:
//    BookmarkContext *m_bookmarkContext;
    QModelIndex m_contextMenuIndex;
    BookmarkManager *m_manager;
};

//class BookmarkContext : public Core::IContext
//{
//public:
//    BookmarkContext(BookmarkView *widget);
//    virtual Core::Context context() const;
//    virtual QWidget *widget();
//private:
//    BookmarkView *m_bookmarkView;
//    const Core::Context m_context;
//};

//class BookmarkViewFactory : public Core::INavigationWidgetFactory
//{
//public:
//    BookmarkViewFactory(BookmarkManager *bm);
//    QString displayName() const;
//    int priority() const;
//    QString id() const;
//    QKeySequence activationSequence() const;
//    Core::NavigationView createWidget();
//private:
//    BookmarkManager *m_manager;
//};

class BookmarkDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    BookmarkDelegate(QObject * parent = 0);
    ~BookmarkDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void generateGradientPixmap(int width, int height, QColor color, bool selected) const;
    mutable QPixmap *m_normalPixmap;
    mutable QPixmap *m_selectedPixmap;
};

} // namespace Internal
} // namespace Bookmarks

#endif // BOOKMARKMANAGER_H
