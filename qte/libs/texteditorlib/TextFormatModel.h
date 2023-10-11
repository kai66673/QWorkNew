#ifndef TEXTFORMATMODEL_H
#define TEXTFORMATMODEL_H

#include <QAbstractTableModel>
#include <QItemDelegate>

#include <QString>
#include <QColor>

#include "texteditor_global.h"

struct TextFormatInfo {
    TextFormatInfo( const QString &name_            = "",
                          bool     bold_            = false,
                          bool     italic_          = false,
                     const QColor &foregroundColor_ = QColor(),
                     const QColor &backgroundColor_ = QColor())
                    : name(name_)
                    , bold(bold_)
                    , italic(italic_)
                    , foregroundColor(foregroundColor_)
                    , backgroundColor(backgroundColor_)
    {}

    QString name;
    bool    bold;
    bool    italic;
    QColor  foregroundColor;
    QColor  backgroundColor;
};

class TextFormatModel;

class TEXTEDITOR_EXPORT ColorButtonDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ColorButtonDelegate( TextFormatModel *model_, QObject *parent = 0 );

    void paint( QPainter* painter,
                const QStyleOptionViewItem& option,
                const QModelIndex& index ) const;

    bool editorEvent( QEvent* ev,
                      QAbstractItemModel* model,
                      const QStyleOptionViewItem& option,
                      const QModelIndex& index);

private:
    TextFormatModel *model;
};

class TEXTEDITOR_EXPORT TextFormatModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TextFormatModel( QObject *parent = 0 );
    virtual ~TextFormatModel();

    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

    virtual void save() const = 0;
    QColor getColor( const QModelIndex &index ) const;
    void setColor( const QColor& color, const QModelIndex &index );

protected:
    QList <TextFormatInfo *> colorsList;
};

#endif // TEXTFORMATMODEL_H
