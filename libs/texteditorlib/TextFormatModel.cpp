#include <QIcon>

#include "Core.h"
#include "TextFormatModel.h"
#include "SettingsManager.h"

#include <QColorDialog>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

ColorButtonDelegate::ColorButtonDelegate( TextFormatModel *model_, QObject *parent )
    : QItemDelegate(parent)
    , model(model_)
{
}

void ColorButtonDelegate::paint( QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index ) const
{
    int colIndex = index.column();

    if ( colIndex == 3 || colIndex == 4 ) {
        QStyleOptionButton button;
        button.rect = option.rect;
        button.state = QStyle::State_Raised | QStyle::State_Enabled;
        button.text = "   ";
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button , painter);
        QColor color = model->getColor(index);
        if ( color.isValid() ) {
            painter->fillRect(option.rect.adjusted(3, 3, -4, -4), color);
        }
        return;
    }

    QItemDelegate::paint(painter, option, index);
}

bool ColorButtonDelegate::editorEvent( QEvent* event,
                                       QAbstractItemModel* model,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& index )
{
    if ( !event || ! model )
        return false;

    int colIndex = index.column();

    if ( colIndex != 3 && colIndex != 4 )
        return false;

    Qt::ItemFlags flags = model->flags(index);
    if ( !(option.state & QStyle::State_Enabled) || !(flags & Qt::ItemIsEnabled) )
        return false;

    bool clearColor = false;

    switch ( event->type() ) {
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            clearColor = (mouseEvent->button() != Qt::LeftButton);
            if ( !option.rect.contains(mouseEvent->pos()) )
                return false;
            if ( event->type() == QEvent::MouseButtonDblClick )
                return true;
            break;
        }
        case QEvent::KeyPress: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( keyEvent->key() != Qt::Key_Space && keyEvent->key() != Qt::Key_Select )
                return true;
            break;
        }
        default: return false;
    }

    if ( clearColor ) {
        this->model->setColor(QColor(), index);
    }
    else {
        QColor color = QColorDialog::getColor(this->model->getColor(index), Core::Storage::mainWindow()->widget());
        if ( color.isValid() )
            this->model->setColor(color, index);
    }

    return true;
}
TextFormatModel::TextFormatModel( QObject *parent )
    : QAbstractTableModel(parent)
{
}

TextFormatModel::~TextFormatModel()
{
    qDeleteAll(colorsList);
    colorsList.clear();
}

QColor TextFormatModel::getColor( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QColor();

    int rowIndex = index.row();
    int colIndex = index.column();
    if ( rowIndex >= 0 && rowIndex < colorsList.size() ) {
        switch ( colIndex ) {
        case 3: return colorsList[rowIndex]->foregroundColor;
        case 4: return colorsList[rowIndex]->backgroundColor;
        }
    }

    return QColor();
}

void TextFormatModel::setColor( const QColor& color, const QModelIndex &index )
{
    if ( !index.isValid() )
        return;

    int rowIndex = index.row();
    int colIndex = index.column();
    if ( rowIndex >= 0 && rowIndex < colorsList.size() &&
         colIndex >= 3 && colIndex <= 4 ) {
        switch ( colIndex ) {
        case 3: colorsList[rowIndex]->foregroundColor = color; break;
        case 4: colorsList[rowIndex]->backgroundColor = color; break;
        }
        emit dataChanged(index, index);
    }
}

int TextFormatModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return colorsList.size();
}

int TextFormatModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant TextFormatModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    int rowIndex = index.row();
    if ( rowIndex >= colorsList.size() || rowIndex < 0 )
        return QVariant();

    TextFormatInfo *colorInfo = colorsList[rowIndex];

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        switch ( index.column() ) {
        case 0: return colorInfo->name;
        case 3: return colorInfo->foregroundColor;
        case 4: return colorInfo->backgroundColor;
        }
    }

    if ( role == Qt::CheckStateRole ) {
        switch ( index.column() ) {
        case 1: return colorInfo->bold          ? Qt::Checked : Qt::Unchecked;
        case 2: return colorInfo->italic        ? Qt::Checked : Qt::Unchecked;
        }
    }

    return QVariant();
}

QVariant TextFormatModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation != Qt::Horizontal )
        return QVariant();

    if ( role == Qt::DisplayRole && section == 0)
        return tr("Name");

    if ( role == Qt::DecorationRole ) {
        switch ( section ) {
        case 1: return QIcon(":/images/hl_bold.png");
        case 2: return QIcon(":/images/hl_italic.png");
        case 3: return QIcon(":/images/hl_textcolor.png");
        case 4: return QIcon(":/images/hl_fillcolor.png");
        }
    }

    return QVariant();
}

Qt::ItemFlags TextFormatModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    if ( index.column() > 0 && index.column() < 3 )
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;

    return QAbstractTableModel::flags(index);
}

bool TextFormatModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( role == Qt::CheckStateRole && index.column() > 0 && index.column() < 3 ) {
        switch ( index.column() ) {
        case 1: colorsList[index.row()]->bold   = value.toBool(); break;
        case 2: colorsList[index.row()]->italic = value.toBool(); break;
        }
        emit dataChanged(index, index);
        return true;
    }

    return false;
}
