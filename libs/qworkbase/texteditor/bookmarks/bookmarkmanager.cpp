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

#include "bookmarkmanager.h"
#include "bookmark.h"

#include <ifile.h>
#include <texteditor.h>
#include <utils/qtcassert.h>

#include "QWorkBase.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPainter>

Q_DECLARE_METATYPE(Bookmarks::Internal::Bookmark*)

using namespace Bookmarks;
using namespace Bookmarks::Internal;
using namespace Core;

BookmarkDelegate::BookmarkDelegate(QObject *parent)
    : QStyledItemDelegate(parent), m_normalPixmap(0), m_selectedPixmap(0)
{
}

BookmarkDelegate::~BookmarkDelegate()
{
    delete m_normalPixmap;
    delete m_selectedPixmap;
}

QSize BookmarkDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QFontMetrics fm(option.font);
    QSize s;
    s.setWidth(option.rect.width());
    s.setHeight(fm.height() * 2 + 10);
    return s;
}

void BookmarkDelegate::generateGradientPixmap(int width, int height, QColor color, bool selected) const
{

    QColor c = color;
    c.setAlpha(0);

    QPixmap *pixmap = new QPixmap(width+1, height);
    pixmap->fill(c);

    QPainter painter(pixmap);
    painter.setPen(Qt::NoPen);

    QLinearGradient lg;
    lg.setCoordinateMode(QGradient::ObjectBoundingMode);
    lg.setFinalStop(1,0);

    lg.setColorAt(0, c);
    lg.setColorAt(0.4, color);

        painter.setBrush(lg);
    painter.drawRect(0, 0, width+1, height);

    if (selected)
        m_selectedPixmap = pixmap;
    else
        m_normalPixmap = pixmap;
}

void BookmarkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);
    painter->save();

    QFontMetrics fm(opt.font);
    static int lwidth = fm.width("8888") + 18;

    QColor backgroundColor;
    QColor textColor;

    bool selected = opt.state & QStyle::State_Selected;

    if (selected) {
        painter->setBrush(opt.palette.highlight().color());
        backgroundColor = opt.palette.highlight().color();
        if (!m_selectedPixmap)
            generateGradientPixmap(lwidth, fm.height()+1, backgroundColor, selected);
    } else {
        painter->setBrush(opt.palette.background().color());
        backgroundColor = opt.palette.background().color();
        if (!m_normalPixmap)
            generateGradientPixmap(lwidth, fm.height(), backgroundColor, selected);
    }
    painter->setPen(Qt::NoPen);
    painter->drawRect(opt.rect);

    // Set Text Color
    if (opt.state & QStyle::State_Selected)
        textColor = opt.palette.highlightedText().color();
    else
        textColor = opt.palette.text().color();

    painter->setPen(textColor);


    // TopLeft
    QString topLeft = index.data(BookmarkManager::Filename ).toString();
    painter->drawText(6, 2 + opt.rect.top() + fm.ascent(), topLeft);

    QString topRight = index.data(BookmarkManager::LineNumber).toString();
    // Check whether we need to be fancy and paint some background
    int fwidth = fm.width(topLeft);
    if (fwidth + lwidth > opt.rect.width()) {
        int left = opt.rect.right() - lwidth;
        painter->drawPixmap(left, opt.rect.top(), selected? *m_selectedPixmap : *m_normalPixmap);
    }
    // topRight
    painter->drawText(opt.rect.right() - fm.width(topRight) - 6 , 2 + opt.rect.top() + fm.ascent(), topRight);

    // Directory
    QColor mix;
    mix.setRgbF(0.7 * textColor.redF()   + 0.3 * backgroundColor.redF(),
                0.7 * textColor.greenF() + 0.3 * backgroundColor.greenF(),
                0.7 * textColor.blueF()  + 0.3 * backgroundColor.blueF());
    painter->setPen(mix);
//
//    QString directory = index.data(BookmarkManager::Directory).toString();
//    int availableSpace = opt.rect.width() - 12;
//    if (fm.width(directory) > availableSpace) {
//        // We need a shorter directory
//        availableSpace -= fm.width("...");
//
//        int pos = directory.size();
//        int idx;
//        forever {
//            idx = directory.lastIndexOf("/", pos-1);
//            if (idx == -1) {
//                // Can't happen, this means the string did fit after all?
//                break;
//            }
//            int width = fm.width(directory.mid(idx, pos-idx));
//            if (width > availableSpace) {
//                directory = "..." + directory.mid(pos);
//                break;
//            } else {
//                pos = idx;
//                availableSpace -= width;
//            }
//        }
//    }
//
//    painter->drawText(3, opt.rect.top() + fm.ascent() + fm.height() + 6, directory);

    QString lineText = index.data(BookmarkManager::LineText).toString().trimmed();
    painter->drawText(6, opt.rect.top() + fm.ascent() + fm.height() + 6, lineText);

    // Separator lines
    painter->setPen(QColor::fromRgb(150,150,150));
    painter->drawLine(0, opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());
    painter->restore();
}

BookmarkView::BookmarkView(QWidget *parent)  :
    QListView(parent),
//    m_bookmarkContext(new BookmarkContext(this)),
    m_manager(0)
{
    setWindowTitle(tr("Bookmarks"));

    connect(this, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(gotoBookmark(const QModelIndex &)));

//    ICore::instance()->addContextObject(m_bookmarkContext);

    setItemDelegate(new BookmarkDelegate(this));
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
}

BookmarkView::~BookmarkView()
{
//    ICore::instance()->removeContextObject(m_bookmarkContext);
}

void BookmarkView::contextMenuEvent(QContextMenuEvent *event)
{
    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();

    QMenu menu;

    QAction *moveUp     = new QAction(QIcon(":/texteditor/images/bookmark_up.png"), tr("Move Up"), mainWindow);
    QAction *moveDown   = new QAction(QIcon(":/texteditor/images/bookmark_down.png"), tr("Move Down"), mainWindow);
    QAction *remove     = new QAction(QIcon(":/texteditor/images/bookmark_delete.png"), tr("&Remove"), mainWindow);
    QAction *removeAll  = new QAction(QIcon(":/texteditor/images/bookmarks_delete.png"), tr("Remove All"), mainWindow);

    moveUp->setStatusTip(tr("Move Selected Bookmark Up"));
    moveDown->setStatusTip(tr("Move Selected Bookmark Down"));
    remove->setStatusTip(tr("Remove Selected Boookmark"));
    removeAll->setStatusTip(tr("Remove All Bookmarks"));

    menu.addAction(moveUp);
    menu.addAction(moveDown);
    menu.addAction(remove);
    menu.addAction(removeAll);

    m_contextMenuIndex = indexAt(event->pos());
    if (!m_contextMenuIndex.isValid()) {
        moveUp->setEnabled(false);
        moveDown->setEnabled(false);
        remove->setEnabled(false);
    }

    if (model()->rowCount() == 0)
        removeAll->setEnabled(false);

    connect(moveUp, SIGNAL(triggered()),
            m_manager, SLOT(moveUp()));
    connect(moveDown, SIGNAL(triggered()),
            m_manager, SLOT(moveDown()));
    connect(remove, SIGNAL(triggered()),
            this, SLOT(removeFromContextMenu()));
    connect(removeAll, SIGNAL(triggered()),
            this, SLOT(removeAll()));

    menu.exec(mapToGlobal(event->pos()));
}

void BookmarkView::removeFromContextMenu()
{
    removeBookmark(m_contextMenuIndex);
}

void BookmarkView::removeBookmark(const QModelIndex& index)
{
    Bookmark *bm = m_manager->bookmarkForIndex(index);
    m_manager->removeBookmark(bm);
}

// The perforcemance of this function could be greatly improved.
//
void BookmarkView::removeAll()
{
    while (m_manager->rowCount()) {
        QModelIndex index = m_manager->index(0, 0);
        removeBookmark(index);
    }
}

void BookmarkView::setModel(QAbstractItemModel *model)
{
    BookmarkManager *manager = qobject_cast<BookmarkManager *>(model);
    QTC_ASSERT(manager, return);
    m_manager = manager;
    QListView::setModel(model);
    setSelectionModel(manager->selectionModel());
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

void BookmarkView::gotoBookmark(const QModelIndex &index)
{
    Bookmark *bk = m_manager->bookmarkForIndex(index);
    if (!m_manager->gotoBookmark(bk))
        m_manager->removeBookmark(bk);
}

////
// BookmarkContext
////

//BookmarkContext::BookmarkContext(BookmarkView *widget)
//    : Core::IContext(widget),
//      m_bookmarkView(widget),
//      m_context(Constants::BOOKMARKS_CONTEXT)
//{
//}

//Context BookmarkContext::context() const
//{
//    return m_context;
//}

//QWidget *BookmarkContext::widget()
//{
//    return m_bookmarkView;
//}

////
// BookmarkManager
////

BookmarkManager::BookmarkManager() :
    m_bookmarkIcon(QLatin1String(":/texteditor/images/bookmark.png")),
    m_selectionModel(new QItemSelectionModel(this, this))
{
//    connect(Core::ICore::instance(), SIGNAL(contextChanged(Core::IContext*,Core::Context)),
//            this, SLOT(updateActionStatus()));

//    connect(ProjectExplorerPlugin::instance()->session(), SIGNAL(sessionLoaded()),
//            this, SLOT(loadBookmarks()));

    updateActionStatus();
}

BookmarkManager::~BookmarkManager()
{
    DirectoryFileBookmarksMap::iterator it, end;
    end = m_bookmarksMap.end();
    for (it = m_bookmarksMap.begin(); it != end; ++it) {
        FileNameBookmarksMap *bookmarks = it.value();
        qDeleteAll(*bookmarks);
        delete bookmarks;
    }
}

QItemSelectionModel *BookmarkManager::selectionModel() const
{
    return m_selectionModel;
}

void BookmarkManager::saveBookmarks( QDomDocument &document , QDomElement *root )
{
    for (int i = 0; i < m_bookmarksList.size(); i++) {
        QDomElement itemElement = document.createElement("item");
        itemElement.setAttribute("filePath", m_bookmarksList[i]->filePath());
        itemElement.setAttribute("lineNumber", m_bookmarksList[i]->lineNumber());
        root->appendChild(itemElement);
    }
}

void BookmarkManager::loadBookmarks( QDomElement *root )
{
    QDomElement itemElement = root->firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            QWorkMainWindow::warning( tr("Error On Load Session Bookmarks\nInvalid Format of Sessions xml-file (unknown tag <%1> at line %2)")
                                      .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        Bookmark *bookmark = new Bookmark( itemElement.attribute("filePath"),
                                           itemElement.attribute("lineNumber").toInt(),
                                           this );
        addBookmark(bookmark, false);
        itemElement = itemElement.nextSiblingElement();
    }
}

QModelIndex BookmarkManager::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    else
        return createIndex(row, column);
}

QModelIndex BookmarkManager::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int BookmarkManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_bookmarksList.count();
}

int BookmarkManager::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 3;
}

QVariant BookmarkManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() !=0 || index.row() < 0 || index.row() >= m_bookmarksList.count())
        return QVariant();

    if (role == BookmarkManager::Filename)
        return m_bookmarksList.at(index.row())->fileName();
    else if (role == BookmarkManager::LineNumber)
        return m_bookmarksList.at(index.row())->lineNumber();
    else if (role == BookmarkManager::Directory)
        return m_bookmarksList.at(index.row())->path();
    else if (role == BookmarkManager::LineText)
        return m_bookmarksList.at(index.row())->lineText();
    else if (role == Qt::ToolTipRole)
        return m_bookmarksList.at(index.row())->filePath();

    return QVariant();
}

void BookmarkManager::toggleBookmark()
{
    if ( DocumentManager::getInstance()->isCurrentDocumentUntitled() ) {
        QWorkMainWindow::information(tr("Bookmarks are unavailable in Untitled documents."));
        return;
    }

    TextEditor::ITextEditor *editor = currentTextEditor();
    if (!editor)
        return;

    toggleBookmark(editor->file()->fileName(), editor->currentLine());
}

void BookmarkManager::toggleBookmark(const QString &fileName, int lineNumber)
{
    const QFileInfo fi(fileName);
    const int editorLine = lineNumber;

    // Remove any existing bookmark on this line
    if (Bookmark *mark = findBookmark(fi.path(), fi.fileName(), lineNumber)) {
        // TODO check if the bookmark is really on the same markable Interface
        removeBookmark(mark);
        return;
    }

    // Add a new bookmark if no bookmark existed on this line
    Bookmark *bookmark = new Bookmark(fi.filePath(), editorLine, this);
    addBookmark(bookmark);
}

void BookmarkManager::updateBookmark(Bookmark *bookmark)
{
    int idx = m_bookmarksList.indexOf(bookmark);
    emit dataChanged(index(idx, 0, QModelIndex()), index(idx, 2, QModelIndex()));
//    saveBookmarks();
}

void BookmarkManager::removeAllBookmarks()
{
    if (m_bookmarksList.isEmpty())
        return;
    beginRemoveRows(QModelIndex(), 0, m_bookmarksList.size() - 1);

    DirectoryFileBookmarksMap::const_iterator it, end;
    end = m_bookmarksMap.constEnd();
    for (it = m_bookmarksMap.constBegin(); it != end; ++it) {
        FileNameBookmarksMap *files = it.value();
        FileNameBookmarksMap::const_iterator jt, jend;
        jend = files->constEnd();
        for (jt = files->constBegin(); jt != jend; ++jt) {
            delete jt.value();
        }
        files->clear();
        delete files;
    }
    m_bookmarksMap.clear();
    m_bookmarksList.clear();
    endRemoveRows();
}

void BookmarkManager::removeBookmark(Bookmark *bookmark)
{
    int idx = m_bookmarksList.indexOf(bookmark);
    beginRemoveRows(QModelIndex(), idx, idx);

    const QFileInfo fi(bookmark->filePath() );
    FileNameBookmarksMap *files = m_bookmarksMap.value(fi.path());

    FileNameBookmarksMap::iterator i = files->begin();
    while (i != files->end()) {
        if (i.value() == bookmark) {
            files->erase(i);
            delete bookmark;
            break;
        }
        ++i;
    }
    if (files->count() <= 0) {
        m_bookmarksMap.remove(fi.path());
        delete files;
    }


    m_bookmarksList.removeAt(idx);
    endRemoveRows();

    if (selectionModel()->currentIndex().isValid())
        selectionModel()->setCurrentIndex(selectionModel()->currentIndex(), QItemSelectionModel::Select | QItemSelectionModel::Clear);

    updateActionStatus();
//    saveBookmarks();
}

Bookmark *BookmarkManager::bookmarkForIndex(QModelIndex index)
{
    if (!index.isValid() || index.row() >= m_bookmarksList.size())
        return 0;
    return m_bookmarksList.at(index.row());
}


bool BookmarkManager::gotoBookmark(Bookmark* bookmark)
{
    using namespace TextEditor;

    if ( IDocument *document = DocumentManager::getInstance()->openDocument( bookmark->filePath(),
                                                                             new TextDocumentCursor(bookmark->lineNumber() - 1), 0, true) ) {
        if ( Core::IEditor *editor = document->editor() )
            return (editor->currentLine() == bookmark->lineNumber());
    }
    return false;
}

void BookmarkManager::nextInDocument()
{
    documentPrevNext(true);
}

void BookmarkManager::prevInDocument()
{
    documentPrevNext(false);
}

void BookmarkManager::documentPrevNext(bool next)
{
    TextEditor::ITextEditor *editor = currentTextEditor();
    if ( !editor )
        return;

    int editorLine = editor->currentLine();
    QFileInfo fi(editor->file()->fileName());
    if (!m_bookmarksMap.contains(fi.path()))
        return;

    int firstLine = -1;
    int lastLine = -1;
    int prevLine = -1;
    int nextLine = -1;
    const QList<Bookmark*> marks = m_bookmarksMap.value(fi.path())->values(fi.fileName());
    for (int i = 0; i < marks.count(); ++i) {
        int markLine = marks.at(i)->lineNumber();
        if (firstLine == -1 || firstLine > markLine)
            firstLine = markLine;
        if (lastLine < markLine)
            lastLine = markLine;
        if (markLine < editorLine && prevLine < markLine)
            prevLine = markLine;
        if (markLine > editorLine &&
            (nextLine == -1 || nextLine > markLine))
            nextLine = markLine;
    }

    QWorkMainWindow::getInstance()->addCurrentPositionToNavigationHistory();

    if (next) {
        if (nextLine == -1)
            editor->gotoLine(firstLine);
        else
            editor->gotoLine(nextLine);
    } else {
        if (prevLine == -1)
            editor->gotoLine(lastLine);
        else
            editor->gotoLine(prevLine);
    }
}

void BookmarkManager::next()
{
    QModelIndex current = selectionModel()->currentIndex();
    if (!current.isValid())
        return;
    int row = current.row();
    ++row;
    while (true) {
        if (row == m_bookmarksList.size())
            row = 0;

        Bookmark *bk = m_bookmarksList.at(row);
        if (gotoBookmark(bk)) {
            QModelIndex newIndex = current.sibling(row, current.column());
            selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::Select | QItemSelectionModel::Clear);
            return;
        }
        removeBookmark(bk);
        if (m_bookmarksList.isEmpty()) // No bookmarks anymore ...
            return;
    }
}

void BookmarkManager::prev()
{
    QModelIndex current = selectionModel()->currentIndex();
    if (!current.isValid())
        return;

    int row = current.row();
    while (true) {
        if (row == 0)
            row = m_bookmarksList.size();
        --row;
        Bookmark *bk = m_bookmarksList.at(row);
        if (gotoBookmark(bk)) {
            QModelIndex newIndex = current.sibling(row, current.column());
            selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::Select | QItemSelectionModel::Clear);
            return;
        }
        removeBookmark(bk);
        if (m_bookmarksList.isEmpty())
            return;
    }
}

TextEditor::ITextEditor *BookmarkManager::currentTextEditor() const
{
    if ( Core::IEditor *currEditor = DocumentManager::getInstance()->currentEditor() )
        return qobject_cast<TextEditor::ITextEditor *>(currEditor);

    return 0;
}

BookmarkManager::State BookmarkManager::state() const
{
    if (m_bookmarksMap.empty())
        return NoBookMarks;

    TextEditor::ITextEditor *editor = currentTextEditor();
    if (!editor)
        return HasBookMarks;

    const QFileInfo fi(editor->file()->fileName());

    const DirectoryFileBookmarksMap::const_iterator dit = m_bookmarksMap.constFind(fi.path());
    if (dit == m_bookmarksMap.constEnd())
        return HasBookMarks;

    return HasBookmarksInDocument;
}

void BookmarkManager::updateActionStatus()
{
    emit updateActions(state());
}

void BookmarkManager::moveUp()
{
    QModelIndex current = selectionModel()->currentIndex();
    int row = current.row();
    if (row == 0)
        row = m_bookmarksList.size();
     --row;

    // swap current.row() and row

    Bookmark *b = m_bookmarksList.at(row);
    m_bookmarksList[row] = m_bookmarksList.at(current.row());
    m_bookmarksList[current.row()] = b;

    QModelIndex topLeft = current.sibling(row, 0);
    QModelIndex bottomRight = current.sibling(current.row(), 2);
    emit dataChanged(topLeft, bottomRight);
    selectionModel()->setCurrentIndex(current.sibling(row, 0), QItemSelectionModel::Select | QItemSelectionModel::Clear);
}

void BookmarkManager::moveDown()
{
    QModelIndex current = selectionModel()->currentIndex();
    int row = current.row();
    ++row;
    if (row == m_bookmarksList.size())
        row = 0;

    // swap current.row() and row
    Bookmark *b = m_bookmarksList.at(row);
    m_bookmarksList[row] = m_bookmarksList.at(current.row());
    m_bookmarksList[current.row()] = b;

    QModelIndex topLeft = current.sibling(current.row(), 0);
    QModelIndex bottomRight = current.sibling(row, 2);
    emit dataChanged(topLeft, bottomRight);
    selectionModel()->setCurrentIndex(current.sibling(row, 0), QItemSelectionModel::Select | QItemSelectionModel::Clear);
}

/* Returns the bookmark at the given file and line number, or 0 if no such bookmark exists. */
Bookmark* BookmarkManager::findBookmark(const QString &path, const QString &fileName, int lineNumber)
{
    if (m_bookmarksMap.contains(path)) {
        foreach (Bookmark *bookmark, m_bookmarksMap.value(path)->values(fileName)) {
            if (bookmark->lineNumber() == lineNumber)
                return bookmark;
        }
    }
    return 0;
}

/* Adds a bookmark to the internal data structures. The 'userset' parameter
 * determines whether action status should be updated and whether the bookmarks
 * should be saved to the session settings.
 */
void BookmarkManager::addBookmark(Bookmark *bookmark, bool userset)
{
    beginInsertRows(QModelIndex(), m_bookmarksList.size(), m_bookmarksList.size());
    const QFileInfo fi(bookmark->filePath());
    const QString &path = fi.path();

    if (!m_bookmarksMap.contains(path))
        m_bookmarksMap.insert(path, new FileNameBookmarksMap());
    m_bookmarksMap.value(path)->insert(fi.fileName(), bookmark);

    m_bookmarksList.append(bookmark);

    endInsertRows();
    if (userset) {
        updateActionStatus();
//        saveBookmarks();
    }
    selectionModel()->setCurrentIndex(index(m_bookmarksList.size()-1 , 0, QModelIndex()), QItemSelectionModel::Select | QItemSelectionModel::Clear);
}

/* Adds a new bookmark based on information parsed from the string. */
void BookmarkManager::addBookmark(const QString &s)
{
    int index2 = s.lastIndexOf(':');
    int index1 = s.indexOf(':');
    if (index2 != -1 || index1 != -1) {
        const QString &filePath = s.mid(index1+1, index2-index1-1);
        const int lineNumber = s.mid(index2 + 1).toInt();
        const QFileInfo fi(filePath);

        if (!filePath.isEmpty() && !findBookmark(fi.path(), fi.fileName(), lineNumber)) {
            Bookmark *b = new Bookmark(filePath, lineNumber, this);
            addBookmark(b, false);
        }
    } else {
        qDebug() << "BookmarkManager::addBookmark() Invalid bookmark string:" << s;
    }
}

/* Puts the bookmark in a string for storing it in the settings. */
QString BookmarkManager::bookmarkToString(const Bookmark *b)
{
    const QLatin1Char colon(':');
    // Empty string was the name of the bookmark, which now is always ""
    return QLatin1String("") + colon + b->filePath() + colon + QString::number(b->lineNumber());
}

// BookmarkViewFactory

//BookmarkViewFactory::BookmarkViewFactory(BookmarkManager *bm)
//    : m_manager(bm)
//{

//}

//QString BookmarkViewFactory::displayName() const
//{
//    return BookmarkView::tr("Bookmarks");
//}

//int BookmarkViewFactory::priority() const
//{
//    return 300;
//}

//QString BookmarkViewFactory::id() const
//{
//    return QLatin1String("Bookmarks");
//}

//QKeySequence BookmarkViewFactory::activationSequence() const
//{
//    return QKeySequence(Qt::ALT + Qt::Key_M);
//}

//Core::NavigationView BookmarkViewFactory::createWidget()
//{
//    BookmarkView *bookmarkView = new BookmarkView();
//    bookmarkView->setModel(m_manager);
//    Core::NavigationView view;
//    view.widget = bookmarkView;
//    return view;
//}
