#include <QKeyEvent>
#include <QApplication>
#include <QHBoxLayout>
#include <QToolButton>

#include "CommandsTreeModel.h"

ShortcutLineEdit::ShortcutLineEdit( CommandTreeItem *item, QWidget *parent )
    : QLineEdit(parent)
    , m_item(item)
{
    reset();
    setReadOnly(true);
}

void ShortcutLineEdit::mousePressEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::RightButton ) {
        reset();
        if ( qApp->keyboardModifiers() & Qt::ControlModifier )
            setText("");
        else
            setText(m_item->defaultShortcut());
    } else {
        QLineEdit::mousePressEvent(event);
    }
}

void ShortcutLineEdit::reset()
{
    m_keyNum = m_key[0] = m_key[1] = m_key[2] = m_key[3] = 0;
}

void ShortcutLineEdit::keyPressEvent( QKeyEvent *e )
{
    int nextKey = e->key();
    if ( m_keyNum > 3 ||
         nextKey == Qt::Key_Control ||
         nextKey == Qt::Key_Shift ||
         nextKey == Qt::Key_Meta ||
         nextKey == Qt::Key_Alt )
         return;

    nextKey |= translateModifiers(e->modifiers(), e->text());
    switch (m_keyNum) {
        case 0:
            m_key[0] = nextKey;
            break;
        case 1:
            m_key[1] = nextKey;
            break;
        case 2:
            m_key[2] = nextKey;
            break;
        case 3:
            m_key[3] = nextKey;
            break;
        default:
            break;
    }
    m_keyNum++;
    QKeySequence ks(m_key[0], m_key[1], m_key[2], m_key[3]);
    setText(ks.toString());
    e->accept();
}

int ShortcutLineEdit::translateModifiers( Qt::KeyboardModifiers state, const QString &text )
{
    int result = 0;
    // The shift modifier only counts when it is not used to type a symbol
    // that is only reachable using the shift key anyway
    if ((state & Qt::ShiftModifier) && (text.size() == 0
                                        || !text.at(0).isPrint()
                                        || text.at(0).isLetterOrNumber()
                                        || text.at(0).isSpace()))
        result |= Qt::SHIFT;
    if (state & Qt::ControlModifier)
        result |= Qt::CTRL;
    if (state & Qt::MetaModifier)
        result |= Qt::META;
    if (state & Qt::AltModifier)
        result |= Qt::ALT;
    return result;
}

ShortcutEditWidget::ShortcutEditWidget(CommandTreeItem *item, QWidget *parent)
    : QWidget(parent)
    , m_item(item)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_lineEdit = new ShortcutLineEdit(item);

    QToolButton *clearButton = new QToolButton;
    clearButton->setIcon(QIcon(":/core/images/editclear.png"));
    clearButton->setToolTip(tr("<b>Clear Shortcut</b> Ctrl+[Right Mouse Button]"));

    QToolButton *resetToDefaultButton = new QToolButton;
    resetToDefaultButton->setIcon(QIcon(":/core/images/reset.png"));
    resetToDefaultButton->setToolTip(tr("<b>Reset to Default</b> [Right Mouse Button]"));

    mainLayout->addWidget(m_lineEdit);
    mainLayout->addWidget(clearButton);
    mainLayout->addWidget(resetToDefaultButton);

    setFocusProxy(m_lineEdit);
    setLayout(mainLayout);

    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearShortcut()));
    connect(resetToDefaultButton, SIGNAL(clicked()), this, SLOT(resetToDefault()));
}

void ShortcutEditWidget::setText(const QString &txt)
{
    m_lineEdit->setText(txt);
}

QString ShortcutEditWidget::text() const
{
    return m_lineEdit->text();
}

void ShortcutEditWidget::clearShortcut()
{
    m_lineEdit->reset();
    m_lineEdit->setText("");
}

void ShortcutEditWidget::resetToDefault()
{
    m_lineEdit->reset();
    m_lineEdit->setText(m_item->defaultShortcut());
}

ShortcutEditDelegate::ShortcutEditDelegate(QSortFilterProxyModel *proxyModel, QObject *parent)
    : QItemDelegate(parent)
    , m_proxyModel(proxyModel)
{
}

QWidget *ShortcutEditDelegate::createEditor( QWidget *parent,
                                             const QStyleOptionViewItem &/* option */,
                                             const QModelIndex & index ) const
{
    CommandTreeItem *item = static_cast<CommandTreeItem *>(m_proxyModel->mapToSource(index).internalPointer());
    ShortcutEditWidget *editor = new ShortcutEditWidget(item, parent);
    return editor;
}

void ShortcutEditDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    ShortcutEditWidget *edit = static_cast<ShortcutEditWidget *>(editor);
    edit->setText(value);
}

void ShortcutEditDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index ) const
{
    ShortcutEditWidget *edit = static_cast<ShortcutEditWidget *>(editor);
    model->setData(index, edit->text(), Qt::EditRole);
}

void ShortcutEditDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option,
                                                 const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

QSize ShortcutEditDelegate::sizeHint( const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/ ) const
{
    return QSize(0, 16);
}

CommandTreeItem::CommandTreeItem( const QString &groupId, CommandTreeItem *parent )
    : parentItem(parent)
    , itemType(Group)
    , name(groupId)
    , description("")
    , shortcut("")
    , m_command(0)
{
}

CommandTreeItem::CommandTreeItem( const QString &commandId, const QString &commandDescription, const QString &commandShortcut,
                                  Core::Command *command_, CommandTreeItem *parent )
    : parentItem(parent)
    , itemType(Command)
    , name(commandId)
    , description(commandDescription)
    , shortcut(commandShortcut)
    , m_command(command_)
{
}

CommandTreeItem::~CommandTreeItem()
{
    qDeleteAll(childItems);
}

void CommandTreeItem::appendChild( CommandTreeItem *item )
{
    childItems.append(item);
}

CommandTreeItem *CommandTreeItem::child( int row )
{
    return childItems.value(row);
}

int CommandTreeItem::childCount() const
{
    return childItems.count();
}

int CommandTreeItem::columnCount() const
{
    return 3;
}

QVariant CommandTreeItem::data( int column ) const
{
    switch ( column ) {
        case 0: return name;
        case 1: return description;
        case 2: return shortcut;
    }

    return QVariant();
}

CommandTreeItem *CommandTreeItem::parent()
{
    return parentItem;
}

int CommandTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CommandTreeItem *>(this));

    return 0;
}

CommandsTreeModel::CommandsTreeModel( Core::CommandManager::CommandsMap *commandsMap_, QObject *parent )
    : QAbstractItemModel(parent)
    , commandsMap(commandsMap_)
{
    rootItem = new CommandTreeItem("<ROOT>");
    setupModelData(rootItem);
}

CommandsTreeModel::~CommandsTreeModel()
{
    delete rootItem;
}

QVariant CommandsTreeModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( role == Qt::FontRole ) {
        QFont font;
        CommandTreeItem *item = static_cast<CommandTreeItem *>(index.internalPointer());
        if ( !item->isCommandRow() )
            font.setBold(true);
        else if ( index.column() == 2 ) {
            if ( item->isShortcutNotDefault() )
                font.setBold(true);
        }
        return font;
    }

    if ( role != Qt::DisplayRole )
        return QVariant();

    CommandTreeItem *item = static_cast<CommandTreeItem *>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags CommandsTreeModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return 0;

    if ( index.column() == 2 ) {
        CommandTreeItem *item = static_cast<CommandTreeItem *>(index.internalPointer());
        if ( item->isCommandRow() )
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant CommandsTreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch (section) {
            case 0: return tr("Command");
            case 1: return tr("Description");
            case 2: return tr("Shortcut");
        }
    }

    return QVariant();
}

QModelIndex CommandsTreeModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    CommandTreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<CommandTreeItem *>(parent.internalPointer());

    CommandTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex CommandsTreeModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    CommandTreeItem *childItem = static_cast<CommandTreeItem *>(index.internalPointer());
    CommandTreeItem *parentItem = childItem->parent();

    if ( parentItem == rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CommandsTreeModel::rowCount( const QModelIndex &parent ) const
{
    CommandTreeItem *parentItem;
    if ( parent.column() > 0 )
        return 0;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<CommandTreeItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int CommandsTreeModel::columnCount( const QModelIndex &/*parent*/ ) const
{
    return rootItem->columnCount();
}

bool CommandsTreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( role != Qt::EditRole )
        return false;

    if ( index.column() != 2 )
        return false;

    CommandTreeItem *commandItem = static_cast<CommandTreeItem *>(index.internalPointer());
    commandItem->setShortcut(value.toString());

    emit dataChanged(index, index);

    return true;
}

void CommandsTreeModel::saveChanges()
{
    commandsMap->clear();
    for (int i = 0; i < rootItem->childCount(); i++) {
        CommandTreeItem *groupItem = rootItem->child(i);
        QString groupId = groupItem->data(0).toString();
        QList<Core::Command *> commands;
        for (int j = 0; j < groupItem->childCount(); j++) {
            CommandTreeItem *commandItem = groupItem->child(j);
            commandItem->saveShortcut();
            commands << commandItem->command();
        }
        (*commandsMap)[groupId] << commands;
    }

    Core::CommandManager::getInstance()->saveCommands();
}

void CommandsTreeModel::resetToDefault()
{
    for (int i = 0; i < rootItem->childCount(); i++) {
        CommandTreeItem *groupItem = rootItem->child(i);
        for (int j = 0; j < groupItem->childCount(); j++) {
            CommandTreeItem *commandItem = groupItem->child(j);
            commandItem->resetToDefault();
            QModelIndex shortcutIndex = createIndex(j, 2, commandItem);
            emit dataChanged(shortcutIndex, shortcutIndex);
        }
    }

}

void CommandsTreeModel::setupModelData( CommandTreeItem *parent )
{
    QMutableMapIterator <QString, QList<Core::Command *> > i(*commandsMap);
    while ( i.hasNext() ) {
        i.next();
        CommandTreeItem *groupItem = new CommandTreeItem(i.key(), parent);
        parent->appendChild(groupItem);
        foreach ( Core::Command *command, i.value() )
            groupItem->appendChild(new CommandTreeItem(command->Id(), command->action()->statusTip(), command->currentShortcut(), command, groupItem));
    }
}
