#ifndef COMMANDSTREEMODEL_H
#define COMMANDSTREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QLineEdit>
#include <QSortFilterProxyModel>

#include "CommandManager.h"

class CommandTreeItem;
class ShortcutEditWidget;

class ShortcutLineEdit: public QLineEdit
{
    friend class ShortcutEditWidget;

public:
    ShortcutLineEdit( CommandTreeItem *item, QWidget *parent = 0 );

protected:
    void contextMenuEvent( QContextMenuEvent * /*event*/ ) {}
    void mousePressEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *e );

private:
    int m_key[4], m_keyNum;
    CommandTreeItem *m_item;

    void reset();
    int translateModifiers( Qt::KeyboardModifiers state, const QString &text );
};

class ShortcutEditWidget: public QWidget
{
    Q_OBJECT

public:
    ShortcutEditWidget(CommandTreeItem *item, QWidget *parent = 0);

    void setText(const QString &txt);
    QString text() const;

private slots:
    void clearShortcut();
    void resetToDefault();

private:
    ShortcutLineEdit *m_lineEdit;
    CommandTreeItem *m_item;
};

class ShortcutEditDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ShortcutEditDelegate( QSortFilterProxyModel *proxyModel, QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;

    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index ) const;

    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index ) const;

    QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

private:
    QSortFilterProxyModel *m_proxyModel;
};

class CommandTreeItem
{
public:
    CommandTreeItem( const QString &groupId, CommandTreeItem *parent = 0 );
    CommandTreeItem( const QString &commandId, const QString &commandDescription,
                     const QString &commandShortcut, Core::Command *command_, CommandTreeItem *parent );

    ~CommandTreeItem();

    enum ItemType {
        Group = 0,
        Command
    };

    void appendChild( CommandTreeItem *child );
    CommandTreeItem *child( int row );
    int childCount() const;
    int columnCount() const;
    QVariant data( int column ) const;
    int row() const;
    CommandTreeItem *parent();

    inline bool isCommandRow() const { return (itemType == Command); }

    inline QString defaultShortcut() const { return m_command->defaultShortcut(); }
    inline void saveShortcut() { m_command->updateShortcut(shortcut); }
    inline void resetToDefault() { shortcut = m_command->defaultShortcut(); }

    inline void setShortcut( const QString &shortcut_ ) { shortcut = shortcut_; }
    inline Core::Command *command() const { return m_command; }
    inline bool isShortcutNotDefault() const { return (shortcut != m_command->defaultShortcut()); }

private:
    CommandTreeItem *parentItem;

    ItemType itemType;
    QString name;
    QString description;
    QString shortcut;
    Core::Command *m_command;

    QList<CommandTreeItem *> childItems;
};

class CommandsTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CommandsTreeModel( Core::CommandManager::CommandsMap *commandsMap_, QObject *parent = 0 );
    ~CommandsTreeModel();

    QVariant data( const QModelIndex &index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column,
                       const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role );

    void saveChanges();
    void resetToDefault();

private:
    void setupModelData( CommandTreeItem *parent );
    CommandTreeItem *rootItem;

    Core::CommandManager::CommandsMap *commandsMap;
};

#endif // COMMANDSTREEMODEL_H
