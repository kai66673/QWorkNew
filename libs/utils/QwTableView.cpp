#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QDate>
#include <QPainter>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDebug>

#include "QwTableView.h"

namespace Utils {

class QwSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    QwSortFilterProxyModel( QObject *parent = 0 )
        : QSortFilterProxyModel(parent)
    {}

    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const {
        QModelIndex lIndex;
        QModelIndex rIndex;

        for (int i = 0; i < m_orderList.size(); i++) {
            if ( m_orderList[i].second == Qt::AscendingOrder ) {
                lIndex = left.sibling(left.row(), m_orderList[i].first);
                rIndex = right.sibling(right.row(), m_orderList[i].first);
            }
            else {
                rIndex = left.sibling(left.row(), m_orderList[i].first);
                lIndex = right.sibling(right.row(), m_orderList[i].first);
            }
            QVariant lValue = left.model()->data(lIndex, Qt::DisplayRole);
            QVariant rValue = right.model()->data(rIndex, Qt::DisplayRole);
            switch (lValue.type()) {
                case QVariant::Invalid:
                    return (rValue.type() == QVariant::Invalid);
                case QVariant::Int:
                    if ( lValue.toInt() == rValue.toInt()) break;
                    return lValue.toInt() < rValue.toInt();
                case QVariant::UInt:
                    if (lValue.toUInt() == rValue.toUInt()) break;
                    return lValue.toUInt() < rValue.toUInt();
                case QVariant::LongLong:
                    if (lValue.toLongLong() == rValue.toLongLong()) break;
                    return lValue.toLongLong() < rValue.toLongLong();
                case QVariant::ULongLong:
                    if (lValue.toULongLong() == rValue.toULongLong()) break;
                    return lValue.toULongLong() < rValue.toULongLong();
                case QVariant::Double:
                    if (lValue.toDouble() == rValue.toDouble()) break;
                    return lValue.toDouble() < rValue.toDouble();
                case QVariant::Char:
                    if (lValue.toChar() == rValue.toChar()) break;
                    return lValue.toChar() < rValue.toChar();
                case QVariant::Date:
                    if (lValue.toDate() == rValue.toDate()) break;
                    return lValue.toDate() < rValue.toDate();
                case QVariant::Time:
                    if (lValue.toTime() == rValue.toTime()) break;
                    return lValue.toTime() < rValue.toTime();
                case QVariant::DateTime:
                    if (lValue.toDateTime() == rValue.toDateTime()) break;
                    return lValue.toDateTime() < rValue.toDateTime();
                case QVariant::String:
                    if (!lValue.toString().localeAwareCompare(rValue.toString())) break;
                    return lValue.toString().localeAwareCompare(rValue.toString()) < 0;
                default:
                    if (lValue.toChar() == rValue.toChar()) break;
                    return lValue.toChar() < rValue.toChar();
                    break;
            }
        }
        return false;
    }

    void toggleColumnOrder( int logicalIndex ) {
        if ( logicalIndex < 0 || logicalIndex >= columnCount() )
            return;

        bool bColumnHaveOrder = false;
        for (int i = 0; i < m_orderList.size(); i++) {
            if ( m_orderList[i].first == logicalIndex ) {
                if ( m_orderList[i].second == Qt::AscendingOrder )
                    m_orderList[i].second = Qt::DescendingOrder;
                else
                    m_orderList.removeAt(i);
                bColumnHaveOrder = true;
                break;
            }
        }

        if ( !bColumnHaveOrder ) {
            QPair <int, Qt::SortOrder> newColOrder(logicalIndex, Qt::AscendingOrder);
            m_orderList.append(newColOrder);
        }

        sort(-1);
        sort(logicalIndex, Qt::AscendingOrder);
    }

private:
    QList <QPair <int, Qt::SortOrder> > m_orderList;
};

class QwHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    QwHeaderView( Qt::Orientation orientation, QWidget* parent = 0 )
        : QHeaderView(orientation, parent)
    {}

    void clearSortIndicators() {
        orderList.clear();
    }

    bool isSortIndicatorShown( int logicalIndex ) const {
        for (int i = 0; i < orderList.size(); i++) {
            if ( orderList[i].first == logicalIndex )
                return true;
        }

        return false;
    }

    Qt::SortOrder sortIndicatorOrder( int logicalIndex ) const {
        for (int i = 0; i < orderList.size(); i++) {
            if ( orderList[i].first == logicalIndex )
                return orderList[i].second;
        }

        return Qt::AscendingOrder;
    }

    int  sortIndicatorNumber( int logicalIndex ) const {
        for (int i = 0; i < orderList.size(); i++) {
            if ( orderList[i].first == logicalIndex )
                return i + 1;
        }

        return 0;
    }

    void paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const {
        if ( isSortIndicatorShown(logicalIndex) ) {
            QRect sortInfoRect(rect.topRight() - QPoint(35,0), rect.bottomRight());

            QStyleOptionHeader opt;
            initStyleOption(&opt);
            QStyle::State state = QStyle::State_None;
            if ( isEnabled() )
                state |= QStyle::State_Enabled;
            if ( window()->isActiveWindow() )
                state |= QStyle::State_Active;

            // not good worked
            QPoint cursorPos = mapFromGlobal(cursor().pos());
            if (
    #ifndef HAVE_QT5
                 isClickable()
    #else
                 sectionsClickable()
    #endif
               ) {
                if ( sortInfoRect.contains(cursorPos) ) {
                    state |= QStyle::State_MouseOver;
                    if ( QApplication::mouseButtons() & Qt::LeftButton )
                        state |= QStyle::State_Sunken;
                }
            }

            opt.rect = sortInfoRect;
            opt.section = logicalIndex;
            opt.state |= state;

            opt.text = QString::number(sortIndicatorNumber(logicalIndex));
            opt.sortIndicator = ( sortIndicatorOrder(logicalIndex) == Qt::AscendingOrder
                                    ? QStyleOptionHeader::SortDown
                                    : QStyleOptionHeader::SortUp );

            QVariant foregroundBrush = model()->headerData(logicalIndex, orientation(), Qt::ForegroundRole);
            if (
    #ifdef HAVE_QT5
                    foregroundBrush.canConvert(QMetaType::QBrush)
    #else
                    qVariantCanConvert<QBrush>(foregroundBrush)
    #endif
               )
                opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));

            QPointF oldBO = painter->brushOrigin();
            QVariant backgroundBrush = model()->headerData(logicalIndex, orientation(), Qt::BackgroundRole);
            if (
    #ifdef HAVE_QT5
                    backgroundBrush.canConvert(QMetaType::QBrush)
    #else
                    qVariantCanConvert<QBrush>(backgroundBrush)
    #endif
               ) {
                opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
                opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
                painter->setBrushOrigin(opt.rect.topLeft());
            }

            opt.position = QStyleOptionHeader::End;

            opt.orientation = orientation();
            style()->drawControl(QStyle::CE_Header, &opt, painter, this);
            painter->setBrushOrigin(oldBO);

            QRect newRect(rect.topLeft(), rect.bottomRight() - QPoint(35, 0));
            QHeaderView::paintSection(painter, newRect, logicalIndex);
        }
        else
            QHeaderView::paintSection(painter, rect, logicalIndex);
    }

    void setModel( QAbstractItemModel* model ) {
        clearSortIndicators();
        QHeaderView::setModel(model);
    }

    void toggleColumnOrder( int logicalIndex ) {
        if ( logicalIndex < 0 || logicalIndex >= count() )
            return;

        bool bColumnHaveOrder = false;
        for (int i = 0; i < orderList.size(); i++) {
            if ( orderList[i].first == logicalIndex ) {
                if ( orderList[i].second == Qt::AscendingOrder )
                    orderList[i].second = Qt::DescendingOrder;
                else
                    orderList.removeAt(i);
                bColumnHaveOrder = true;
                break;
            }
        }

        if ( !bColumnHaveOrder ) {
            QPair <int, Qt::SortOrder> newColOrder(logicalIndex, Qt::AscendingOrder);
            orderList.append(newColOrder);
        }

        repaint();
    }

protected:
    QList <QPair <int, Qt::SortOrder> > orderList;
};

class QwTableCellViewDialog: public QDialog
{
public:
    QwTableCellViewDialog(const QString &cellText, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        QHBoxLayout *buttonsLayout = new QHBoxLayout();
        QPushButton *closeButton = new QPushButton(tr("Close"));
        buttonsLayout->addStretch();
        buttonsLayout->addWidget(closeButton);

        QPlainTextEdit *cellEdit = new QPlainTextEdit();
        cellEdit->setPlainText(cellText);
        cellEdit->setReadOnly(true);

        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->addWidget(cellEdit);
        mainLayout->addLayout(buttonsLayout);

        setLayout(mainLayout);

        connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    }

};

QwTableView::QwTableView( const QString &clipboardSplitter, QWidget *parent )
    : QTableView(parent)
    , m_clipboardSplitter(clipboardSplitter)
{
    QwHeaderView *horizontalHeader = new QwHeaderView(Qt::Horizontal, this);
#ifndef HAVE_QT5
    horizontalHeader->setClickable(true);
    horizontalHeader->setMovable(true);
#else
    horizontalHeader->setSectionsClickable(true);
    horizontalHeader->setSectionsMovable(true);
#endif
    setHorizontalHeader(horizontalHeader);

    connect(horizontalHeader, SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));
}

void QwTableView::setModel( QAbstractItemModel *model )
{
    while (model->canFetchMore(QModelIndex()))
        model->fetchMore(QModelIndex());

    QwSortFilterProxyModel *proxyModel = new QwSortFilterProxyModel();

    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(model);

    QTableView::setModel(proxyModel);
}

void QwTableView::headerClicked( int logicalIndex )
{
    QwHeaderView *horHeader = qobject_cast<QwHeaderView *>(horizontalHeader());
    QwSortFilterProxyModel *sortModel = qobject_cast<QwSortFilterProxyModel *>(model());
    horHeader->toggleColumnOrder(logicalIndex);
    sortModel->toggleColumnOrder(logicalIndex);
}

void QwTableView::clipboardSplitterChanged( const QString &clipboardSplitter )
{
    m_clipboardSplitter = clipboardSplitter;
}

void QwTableView::keyPressEvent( QKeyEvent * event )
{
    if ( event->matches(QKeySequence::Copy) ) {
        copy();
    }
    else {
        QTableView::keyPressEvent(event);
    }
}

void QwTableView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        QModelIndexList indexes = selectionModel()->selectedIndexes();
        if (indexes.size() == 1) {
            QModelIndex index = indexes.at(0);
            if (index.isValid()) {
                ev->accept();
                auto cellData =  model()->data(index);
                QByteArray ba = cellData.toByteArray();
                qDebug() << "---" << ba;
                QwTableCellViewDialog cellEditDialog(ba);
                cellEditDialog.exec();
            }
        }
    }
}

void QwTableView::copy()
{
    QStringList list;
    foreach ( const QModelIndex& index, selectionModel()->selectedIndexes() ) {
       list << index.data().toString().trimmed();
    }

    if ( list.size() ) {
        if ( m_clipboardSplitter == QLatin1String("") )
            m_clipboardSplitter = QStringLiteral(" ");
        QApplication::clipboard()->setText(list.join(m_clipboardSplitter));
    }
}

}   // namespace Utils

#include "QwTableView.moc"
