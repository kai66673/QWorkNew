#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDomDocument>
#include <QDir>

#include "Core.h"
#include "SettingsManager.h"
#include "QSqlHistoryModel.h"
#include "dbbrowser_global.h"
#include "dbbrowser_constants.h"

void SqlHistoryInfoList::load()
{
    clear();

    QFile file(fileName);

    if ( !file.exists() )
        return;

    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        Core::Storage::warning( tr("Error On Aliases Loading\nFile %1 Cannot be Loaded").arg(fileName) );
        return;
    }
    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        Core::Storage::warning( tr("Error On Aliases Loading\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                  .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "sqlHistory" ) {
        Core::Storage::warning( tr("Error On Sql History Loading\nInvalid Format of xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        Core::Storage::warning( tr("Error On Sql History Loading\nInvalid Format of xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        Core::Storage::warning( tr("Error On Sql History Loading\nInvalid Format of xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            Core::Storage::warning( tr("Error On Sql History Loading\nInvalid Format of xml-file (unknown tag <%1> at line %2)")
                                      .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        SqlHistoryInfo sqlInfo( itemElement.attribute("text", ""),
                                QDateTime::fromString(itemElement.attribute("date", "01.01.1900 00-00-00"), "dd.MM.yyyy HH-mm-ss") );
        items.append(sqlInfo);

        itemElement = itemElement.nextSiblingElement();
    }
}

void SqlHistoryInfoList::save()
{
    const int IndentSize = 4;

    QFile file(fileName);

    if ( !file.exists() ) {
        QFileInfo fi(fileName);
        if ( !fi.absoluteDir().exists() ) {
            QDir dir;
            dir.mkpath(fi.absolutePath());
        }
    }

    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        Core::Storage::warning( tr("Error On Sql History Saving\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                                  .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("sqlHistory");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("sqlHistory");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    for (int i = 0; i < items.size(); i++) {
        QDomElement itemElement = domDocument.createElement("item");
        itemElement.setAttribute("text", items[i].text);
        itemElement.setAttribute("date", items[i].date.toString("dd.MM.yyyy HH-mm-ss"));
        root.appendChild(itemElement);
    }

    QTextStream out(&file);
    domDocument.save(out, IndentSize);
    file.close();
}


QSqlHistoryModel::QSqlHistoryModel(SqlHistoryInfoList *sqlList_, QObject *parent )
    : QAbstractTableModel(parent)
    , sqlList(sqlList_)
{
}

int QSqlHistoryModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return sqlList->size();
}

int QSqlHistoryModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant QSqlHistoryModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= sqlList->size() || index.row() < 0)
        return QVariant();

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        switch ( index.column() ) {
        case 0: return (*sqlList)[index.row()].text;
        case 1: return (*sqlList)[index.row()].date;
        }
    }

    return QVariant();
}

QVariant QSqlHistoryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("Sql Statement Source");
        case 1: return tr("Execution Date/time");
        }
    }

    return QVariant();
}

Qt::ItemFlags QSqlHistoryModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

bool QSqlHistoryModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    beginRemoveRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        sqlList->remove(position);
    endRemoveRows();

    return true;
}

bool QSqlHistoryModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        sqlList->append();
    endInsertRows();

    return true;
}

void QSqlHistoryModel::addSql( const QString &text )
{
    if ( !SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "SqlHistory").toBool() )
        return;

    int addIndex = rowCount(QModelIndex());
    insertRows(addIndex, 1);

    (*sqlList)[addIndex].text = text;
    (*sqlList)[addIndex].date = QDateTime::currentDateTime();

    emit dataChanged(index(addIndex, 0), index(addIndex, 0));
}
