#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QSet>
#include <QColor>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

#include "CommandManager.h"
#include "corelib_global.h"
#include "QwDockWidget.h"
#include "IMainWindow.h"

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

namespace Core {

struct CORELIB_EXPORT LogMessage {
    enum LogLevel {
        Information = 0,
        Warning,
        Error
    };

    LogMessage( const QString &message_, const QString &tagId_ = "<NO TAG>",
                LogLevel level_ = Information,
                const QColor &backgroundColor_ = QColor() )
        : message(message_)
        , tagId(tagId_)
        , level(level_)
        , backgroundColor(backgroundColor_)
    {}

    QString     message;
    QString     tagId;
    LogLevel    level;
    QColor      backgroundColor;
};

struct LogFilter
{
    enum TagFilterType {
        AllTags = 0,
        AllRegisteredTags,
        AllUnregisteredTags,
        AnyRegisteredTag,
        AnyTag
    };

    LogFilter()
        : informations(true)
        , warnings(true)
        , errors(true)
        , tagFilterType(AllTags)
        , selectedTagId("<NO TAG>")
        , commonTagId("")
    {}

    bool informations;
    bool warnings;
    bool errors;
    TagFilterType tagFilterType;
    QString selectedTagId;
    QString commonTagId;
};

class LogMessageModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    LogMessageModel( QObject *parent = 0 )
        : QAbstractTableModel(parent)
    {}

    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    void addMessage( const LogMessage &message , QTableView *table , bool autoResizeRowHeight );
    void clearMessages();

    LogMessage::LogLevel logLevel( int index ) const { return items[index].level; }
    QString tagId( int index ) const { return items[index].tagId; }

private:
    QList<LogMessage> items;
};

class LogMessageFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    LogMessageFilterModel( LogFilter *filter, QObject *parent = 0 );
    void updateFilterModel();

protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;

private:
    LogFilter *m_filter;
};

class CORELIB_EXPORT LogManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LogManager)

    static LogManager *m_instance;

public:
    explicit LogManager( IMainWindow *mainWindow, QObject *parent = 0 );
    static LogManager *getInstance() { return m_instance; }
    static void registerTag( const QString &tagId );
    static void addMessage( const LogMessage &message );
    static bool isTagRegistered( const QString &tagId );
    void setAutoResizeRowHeight( bool autoResizeRowHeight );

    QAction *toggleAction() { return m_toggleAction; }

signals:
    
public slots:
    void onClearLog();
    void onFilterLog();

private:
    void regesterTagInternal( const QString &tagId );
    void addMessageInternal( const LogMessage &message );
    bool isTagRegisteredInternal( const QString &tagId );

    IMainWindow *m_mainWindow;

    QSet<QString> m_registeredTags;
    QwCustomDockWidget *m_dock;
    LogMessageModel *m_model;
    LogMessageFilterModel *m_filterModel;
    QTableView *m_table;

    bool m_autoResizeRowHeight;

    void createActions();
    Core::Command *m_cmdClear;
    Core::Command *m_cmdFilter;

    LogFilter m_filter;
    QAction *m_toggleAction;
};

} // Core

#endif // LOGMANAGER_H
