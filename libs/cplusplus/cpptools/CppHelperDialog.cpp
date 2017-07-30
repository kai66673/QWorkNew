#include <QDebug>
#include <QKeyEvent>
#include <QPainter>

#include "CppHelperDialog.h"
#include "indexitem.h"
#include "cppindexingsupport.h"
#include "cpptoolsconstants.h"
#include "Core.h"
#include "cppprojects.h"
#include "cppmodelmanager.h"

#include "ui_CppHelperDialog.h"

CppRefDelegate::CppRefDelegate(CppRefsModel *model, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_model(model)
{ }

void CppRefDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);
    painter->save();

    QColor backgroundColor;

    const QAbstractItemView * view = qobject_cast<const QAbstractItemView *>(opt.widget);
    bool selected = view->selectionModel()->currentIndex() == index;

    // Rect
    if (selected) {
        painter->setBrush(opt.palette.highlight().color());
        backgroundColor = opt.palette.highlight().color();
    } else {
        painter->setBrush(opt.palette.background().color());
        backgroundColor = opt.palette.background().color();
    }
    painter->setPen(Qt::NoPen);
    painter->drawRect(opt.rect);

    // Icon
    QIcon icon = m_model->data(index, Qt::DecorationRole).value<QIcon>();
    painter->drawPixmap(opt.rect.left() + 2, opt.rect.top() + 2, icon.pixmap(24, 24));

    int matchingPosition = m_model->data(index, CppRefsModel::MatchingPosition).toInt();
    QString searchText = m_model->data(index, CppRefsModel::SearchString).toString();
    QString text = index.data(Qt::DisplayRole).toString();
    int dx = 26;
    QFontMetrics fm(option.font);
    if ( matchingPosition > 0 )
        dx += fm.width(text.left(matchingPosition));
    int dw = fm.width(text.mid(matchingPosition, searchText.length()));
    QRect hlRect(opt.rect.left() + dx, opt.rect.top(), dw, opt.rect.height());
    painter->setBrush(Qt::yellow);
    painter->drawRect(hlRect);

    // Text
    painter->setPen(QPen(Qt::black));
    painter->drawText(option.rect.adjusted(26, 0, 0, 0), Qt::AlignLeft, text);

    painter->restore();
}

CppRefsModel::CppRefsModel(const QString &filePath, const QString &searchString)
    : QAbstractItemModel()
    , m_filePath(filePath)
    , m_searchString(searchString)
    , m_modelManager(CPlusPlus::CppModelManagerInterface::instance())
{
    CppTools::Internal::CppModelManager *modelManager = CppTools::Internal::CppModelManager::instance();
    const QList<CppTools::ProjectPart::Ptr> projectParts = modelManager->projectPart(filePath);
    if (projectParts.isEmpty()) { // Not part of any project, better use all include paths than none
        fillQtHeaderPaths(modelManager->headerPaths());
    } else {
        foreach (const CppTools::ProjectPart::Ptr &part, projectParts)
            fillQtHeaderPaths(part->headerPaths);
    }

    refreshItems(searchString);
}

QModelIndex CppRefsModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return QModelIndex();
    else
        return createIndex(row, column);
}

QModelIndex CppRefsModel::parent( const QModelIndex & ) const
{ return QModelIndex(); }

int CppRefsModel::rowCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return 0;
    return m_items.count();
}

int CppRefsModel::columnCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return 0;
    return 1;
}

QVariant CppRefsModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() || index.column() !=0 || index.row() < 0 || index.row() >= m_items.count() )
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:       return m_items.at(index.row()).m_text;
        case Qt::DecorationRole:    return m_items.at(index.row()).m_icon;
        case SearchString:          return m_searchString;
        case MatchingPosition:      return m_items.at(index.row()).m_matchingPosition;
    }

    return QVariant();
}

void CppRefsModel::refresh(const QString &key)
{
    beginResetModel();
    refreshItems(key);
    endResetModel();
}

const CppTools::CppHelperItem &CppRefsModel::item(int row)
{
    return m_items.at(row);
}

static void fillFromQtIncludes(const QString &className,
                               const CppTools::ProjectPart::HeaderPaths &headerPaths,
                               QList<CppTools::CppHelperItem> &items)
{
   // Check for a header file with the same name in the Qt include paths
   foreach (const CppTools::ProjectPart::HeaderPath &headerPath, headerPaths) {
       QDir dir(headerPath.path);
       foreach (const QFileInfo &fi, dir.entryInfoList()) {
           if ( fi.isFile() && fi.suffix().isEmpty() ) {
               int matchingPos = fi.fileName().indexOf(className, 0, Qt::CaseInsensitive);
               if ( matchingPos != -1 ) {
                   CppTools::CppHelperItem item( CppTools::CppHelperItem::Class,
                                                 QIcon(QLatin1String(":/codemodel/images/class.png")),
                                                 fi.fileName(),
                                                 fi.fileName(),
                                                 false,
                                                 fi.filePath(),
                                                 "",
                                                 matchingPos );
                   items.prepend(item);
               }
           }
       }
   }
}


void CppRefsModel::refreshItems(const QString &key)
{
    m_items.clear();

    if ( key.length() < 2 )
        return;

    CppTools::SymbolSearcher::Parameters parameters;
    parameters.text = key;
    parameters.flags = 0;
    parameters.types = CppTools::SymbolSearcher::SymbolTypes( CppTools::SymbolSearcher::Classes |
                                                              CppTools::SymbolSearcher::Functions |
                                                              CppTools::SymbolSearcher::Enums |
                                                              CppTools::SymbolSearcher::Declarations );
    parameters.scope = CppTools::SymbolSearcher::SearchGlobal;

    QFutureInterface<void> interface;
    interface.setProgressRange(0, 0);
    Core::Storage::mainWindow()->progressManager()->addTask( interface.future(), tr("Search C++ Symbols"),
                                                             CppTools::Constants::TASK_SEARCH_SYMBOLS,
                                                             0, true, true );
    interface.reportStarted();

    CppTools::SymbolSearcher *symbolSearcher = m_modelManager->indexingSupport()->createSymbolSearcher(parameters, QSet<QString>());
    symbolSearcher->fillCppHelperItems(m_items);

    fillFromQtIncludes(key, m_QtHeaderPaths, m_items);

    interface.reportFinished();
    m_searchString = key;
}

void CppRefsModel::fillQtHeaderPaths(const CppTools::ProjectPart::HeaderPaths &paths)
{
    foreach (const CppTools::ProjectPart::HeaderPath &headerPath, paths)
        if ( headerPath.path.contains(QLatin1String("/Qt")) )
            m_QtHeaderPaths += headerPath;
}

CppHelperDialog::CppHelperDialog(const QString &filePath, const QString &searchString, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CppHelperDialog)
    , m_model(new CppRefsModel(filePath, searchString))
    , m_selectedItem(0)
    , m_blockingEventFilter(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    move( parent->geometry().center().x() - rect().center().x(),
          parent->geometry().center().y() - rect().center().y() );

    ui->listView->setTextElideMode(Qt::ElideRight);
    ui->listView->setResizeMode(QListView::Fixed);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setItemDelegate(new CppRefDelegate(m_model));
    ui->listView->setModel(m_model);
    ui->listView->setFocus();

    connect( ui->listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             this, SLOT(currentRowChanged(QModelIndex,QModelIndex)) );
    ui->listView->setCurrentIndex(m_model->index(0, 0));

    ui->searchLineEdit->setText(searchString);
    ui->searchLineEdit->setSelection(searchString.length(), searchString.length());
    ui->searchLineEdit->setFocus();

    setFocusProxy(ui->searchLineEdit);
    ui->listView->setFocusProxy(ui->searchLineEdit);

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchStringChanged(QString)));

    ui->listView->installEventFilter(this);
    ui->searchLineEdit->installEventFilter(this);
    ui->listView->viewport()->installEventFilter(this);
}

CppHelperDialog::~CppHelperDialog()
{ }

const CppTools::CppHelperItem *CppHelperDialog::selectedItem()
{
    if ( m_selectedItem )
        return new CppTools::CppHelperItem(*m_selectedItem);
    return 0;
}

void CppHelperDialog::currentRowChanged( const QModelIndex &current, const QModelIndex &/*previous*/ )
{
    ui->changesDescrLabel->setText("");
    if ( !m_model->rowCount() )
        return;

    int row = current.row();
    if ( row != -1 ) {
        const CppTools::CppHelperItem &item = m_model->item(row);
        QString refactoryAction;
        if ( !item.m_functionReturnType.isEmpty() )
            refactoryAction = QString("extern %1%2%3")
                    .arg(item.m_functionReturnType)
                    .arg(item.m_functionReturnType.endsWith("*") ? "" : " ")
                    .arg(item.m_text);
        else {
            QString include = item.m_fileName;
            int pos = item.m_fileName.lastIndexOf("/");
            if ( pos != -1 )
                include = item.m_fileName.right(item.m_fileName.length() - pos - 1);
            refactoryAction = QString("#include &lt;%1&gt;").arg(include);
        }
        ui->changesDescrLabel->setText(tr("<pre>"
                                          "Found in: %1\n"
                                          "  Insert: %2\n"
                                          "    With: %3"
                                          "</pre>")
                                       .arg(item.m_fileName)
                                       .arg(item.m_qualifiedName)
                                       .arg(refactoryAction));
    }
}

void CppHelperDialog::searchStringChanged(const QString &newKey)
{
    m_model->refresh(newKey);
    if ( m_model->rowCount() > 0 )
        ui->listView->setCurrentIndex(m_model->index(0, 0));
    else
        ui->changesDescrLabel->setText("");
}

void CppHelperDialog::selectNextItem()
{
    int rowCount = ui->listView->model()->rowCount();
    if ( rowCount > 1 ) {
        QModelIndex ci = ui->listView->currentIndex();
        int newRow = ci.row() + 1;
        if ( newRow == rowCount )
            newRow = 0;
        ui->listView->setCurrentIndex(ui->listView->model()->index(newRow, 0));
    }
}

void CppHelperDialog::selectPreviousItem()
{
    int rowCount = ui->listView->model()->rowCount();
    if ( rowCount > 1 ) {
        QModelIndex ci = ui->listView->currentIndex();
        int newRow = ci.row() - 1;
        if ( newRow == -1 )
            newRow = rowCount - 1;
        ui->listView->setCurrentIndex(ui->listView->model()->index(newRow, 0));
    }
}

void CppHelperDialog::accept()
{
    int row = ui->listView->currentIndex().row();
    if ( row == -1 )
        reject();
    else {
        m_selectedItem = &(m_model->item(row));
        QDialog::accept();
    }
}

void CppHelperDialog::reject()
{
    m_selectedItem = 0;
    QDialog::reject();
}

bool CppHelperDialog::eventFilter( QObject *obj, QEvent *event )
{
    if ( m_blockingEventFilter )
        return true;

    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        if ( obj == ui->searchLineEdit ) {
            if ( ke->key() == Qt::Key_Up ) {
                m_blockingEventFilter = true;
                selectPreviousItem();
                m_blockingEventFilter = false;
                return true;
            } else if ( ke->key() == Qt::Key_Down ) {
                m_blockingEventFilter = true;
                selectNextItem();
                m_blockingEventFilter = false;
                return true;
            }
        }
        if ( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return ) {
            accept();
            return true;
        }
    } else if ( event->type() == QEvent::MouseButtonDblClick && obj != ui->searchLineEdit ) {
        accept();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
