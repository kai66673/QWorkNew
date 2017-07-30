#include <QTableView>
#include <QAction>
#include <QHeaderView>
#include <QMenu>

#include "LogManager.h"
#include "LogFilterDialog.h"

namespace Core {

int LogMessageModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return items.size();
}

int LogMessageModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 2;
}

Qt::ItemFlags LogMessageModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant LogMessageModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= items.size() )
        return QVariant();

    if ( index.column() < 0 || index.column() >= 2 )
        return QVariant();

    if ( role == Qt::ForegroundRole ) {
        switch ( items[index.row()].level ) {
            case LogMessage::Information:   return QColor(  0, 127,   0);
            case LogMessage::Warning:       return QColor(  0,   0, 127);
            case LogMessage::Error:         return QColor(127,   0,   0);
        }
    }

    if ( role == Qt::BackgroundRole )
        return items[index.row()].backgroundColor.isValid() ? items[index.row()].backgroundColor : QVariant();

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        switch ( index.column() ) {
            case 0: return items[index.row()].message;
            case 1: return items[index.row()].tagId;
        }
    }

    return QVariant();
}

QVariant LogMessageModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal) {
        switch ( section ) {
            case 0: return QString("Message");
            case 1: return QString("Tag");
            default: return QVariant();
        }
    }

    if ( orientation == Qt::Vertical ) {
        return (section + 1);
    }

    return QVariant();
}

void LogMessageModel::addMessage( const LogMessage &message, QTableView *table, bool autoResizeRowHeight )
{
    int index = items.size();
    beginInsertRows(QModelIndex(), index, index);
    items.append(message);
    endInsertRows();

    if ( autoResizeRowHeight )
        table->resizeRowToContents(index);
    table->scrollToBottom();
}

void LogMessageModel::clearMessages()
{
    int index = items.size();
    if ( index ) {
        beginRemoveRows(QModelIndex(), 0, index - 1);
        items.clear();
        endRemoveRows();
    }
}

LogMessageFilterModel::LogMessageFilterModel( LogFilter *filter, QObject *parent )
    : QSortFilterProxyModel(parent)
    , m_filter(filter)
{ }

void LogMessageFilterModel::updateFilterModel()
{
    invalidateFilter();
}

bool LogMessageFilterModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
    Q_UNUSED(sourceParent)

    LogMessageModel *model = qobject_cast<LogMessageModel *>(sourceModel());
    if ( model ) {
        QString tagId = model->tagId(sourceRow);
        LogMessage::LogLevel level = model->logLevel(sourceRow);
        switch ( level ) {
            case LogMessage::Information:
                if ( !m_filter->informations ) return false;
                break;
            case LogMessage::Warning:
                if ( !m_filter->warnings ) return false;
                break;
            case LogMessage::Error:
                if ( !m_filter->errors ) return false;
                break;
        }
        switch ( m_filter->tagFilterType ) {
            case LogFilter::AllTags:                return true;
            case LogFilter::AllRegisteredTags:      return LogManager::isTagRegistered(tagId);
            case LogFilter::AllUnregisteredTags:    return !LogManager::isTagRegistered(tagId);
            case LogFilter::AnyRegisteredTag:       return !m_filter->selectedTagId.compare(tagId, Qt::CaseInsensitive);
            case LogFilter::AnyTag:                 return !m_filter->commonTagId.compare(tagId, Qt::CaseInsensitive);
        }
    }

    return true;
}

LogManager *LogManager::m_instance = 0;

LogManager::LogManager( IMainWindow *mainWindow, QObject *parent )
    : QObject(parent)
    , m_mainWindow(mainWindow)
    , m_autoResizeRowHeight(false)
{
    m_instance = this;

    createActions();

    m_dock = new QwCustomDockWidget( mainWindow->widget(), tr("Log"), "LogManagerDockWidget",
                                     QKeySequence(tr("Ctrl+D,L")), QIcon(":/images/log_dock.png"),
                                     Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                     QList<QAction *>() << m_cmdClear->action() << m_cmdFilter->action() );
    m_dock->setContentsMargins(0, 0, 0, 0);

    m_table = new QTableView(m_dock);
    m_model = new LogMessageModel(m_table);
    m_filterModel = new LogMessageFilterModel(&m_filter, m_table);
    m_filterModel->setSourceModel(m_model);
    m_table->setModel(m_filterModel);
#ifndef HAVE_QT5
    m_table->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
#else
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
    m_table->verticalHeader()->setDefaultSectionSize(16);

    m_dock->setWidget(m_table);
    mainWindow->addDockWidgetHandler(new DockWidgetHandler(m_dock, new EditModeDockGuiClient(m_dock)));

    m_toggleAction = m_dock->toggleViewAction();
    m_toggleAction->setIcon(QIcon(":/images/log_dock.png"));
    QString toolTipText = m_toggleAction->text();
    toolTipText.prepend(tr("Toggle "));
    toolTipText.append(tr(" Dock"));
    m_toggleAction->setStatusTip(toolTipText);
    QKeySequence shortcut(tr("Ctrl+D,L"));
    m_toggleAction->setToolTip( QString("<b>%1</b> <span style=\"color: gray; font-size: small\">%2</span>")
                        .arg(toolTipText)
                        .arg(shortcut.toString(QKeySequence::NativeText)) );
    m_toggleAction->setShortcut(shortcut);

    regesterTagInternal("<NO TAG>");

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdClear->action());
    contextMenu->addAction(m_cmdFilter->action());
    m_dock->setContextMenu(contextMenu);
}

void LogManager::registerTag( const QString &tagId )
{
    m_instance->regesterTagInternal(tagId);
}

void LogManager::addMessage( const LogMessage &message )
{
    m_instance->addMessageInternal(message);
}

bool LogManager::isTagRegistered(const QString &tagId)
{
    return m_instance->isTagRegisteredInternal(tagId);
}

void LogManager::onClearLog()
{
    m_model->clearMessages();
}

void LogManager::onFilterLog()
{
    LogFilterDialog filterDialog(&m_filter, m_registeredTags, m_mainWindow->widget());
    if ( filterDialog.exec() == QDialog::Accepted )
        m_filterModel->updateFilterModel();
}

void LogManager::regesterTagInternal( const QString &tagId )
{
    if ( !m_registeredTags.contains(tagId) )
        m_registeredTags.insert(tagId);
}

void LogManager::addMessageInternal( const LogMessage &message )
{
    m_model->addMessage(message, m_table, m_autoResizeRowHeight);
}

bool LogManager::isTagRegisteredInternal( const QString &tagId )
{
    foreach( const QString &id, m_registeredTags )
        if ( !id.compare(tagId, Qt::CaseInsensitive) )
            return true;

    return false;
}

void LogManager::setAutoResizeRowHeight( bool autoResizeRowHeight )
{
    m_autoResizeRowHeight = autoResizeRowHeight;
}

void LogManager::createActions()
{
    m_cmdClear      = new Core::Command( tr("ClearLog"),
                                         QIcon(":/images/sr_clear.png"),
                                         tr("Clear"),
                                         tr("Clear Log"),
                                         QKeySequence(),
                                         m_mainWindow->widget() );

    m_cmdFilter     = new Core::Command( tr("FilterLog"),
                                         QIcon(":/core/images/filtericon.png"),
                                         tr("Filter"),
                                         tr("Filter Log Rows"),
                                         QKeySequence(),
                                         m_mainWindow->widget() );

    Core::CommandManager::getInstance()->registerCommands( tr("LogPanelCommands"),
                                                            QList<Core::Command *>() << m_cmdClear
                                                                                     << m_cmdFilter );

    connect(m_cmdClear->action(),   SIGNAL(triggered()), this, SLOT(onClearLog()));
    connect(m_cmdFilter->action(),  SIGNAL(triggered()), this, SLOT(onFilterLog()));
}

} // Core
