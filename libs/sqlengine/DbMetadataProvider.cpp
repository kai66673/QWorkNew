#include <QStringList>

#include "DbMetadataProvider.h"
#include "CommandManager.h"
#include "DbCatalog.h"

namespace Sql {

DbMetadataProvider *DbMetadataProvider::m_instance = 0;

DbMetadataProvider::DbMetadataProvider(QObject *parent)
    : QObject(parent)
    , m_connection(0)
{
    m_instance = this;
    Core::CommandManager::getInstance()->connectSlotToSignal(this, "onCurrentDbConnectionChanged", "currentDbConnectionChanged");
}

DbMetadataProvider *DbMetadataProvider::getInstance()
{
    return m_instance;
}

void DbMetadataProvider::fillSchemas( QList<TextEditor::BasicProposalItem *> &completions )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *schemas = m_connection->schemas();
    if ( !schemas )
        return;

    for (int i = 0; i < schemas->size(); i++) {
        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
        Database::IDbCatalogItem *schema = schemas->at(i);
        item->setText(schema->name());
        item->setIcon(QIcon(schema->iconName(m_connection->loader())));
        completions.append(item);
    }
}

void DbMetadataProvider::fillSchemaObjects( const QString &schemaName, QList<TextEditor::BasicProposalItem *> &completions, unsigned objectType )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *objects = m_connection->schemaObjects(schemaName, objectType);
    if ( !objects )
        return;

    for (int i = 0; i < objects->size(); i++) {
        Database::IDbCatalogItem *object = objects->at(i);
        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
        item->setText(object->name());
        item->setIcon(QIcon(object->iconName(m_connection->loader())));
        completions.append(item);
    }
}

void DbMetadataProvider::fillTableColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions, const QStringList &substitutes )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *columns = m_connection->tableColumns(schemaName, tableName);
    if ( columns ) {
        for (int i = 0; i < columns->size(); i++) {
            Database::IDbCatalogItem *column = columns->at(i);
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            if ( i < substitutes.size() )
                item->setText(substitutes[i]);
            else
                item->setText(column->name());
            item->setIcon(QIcon(column->iconName(m_connection->loader())));
            completions.append(item);
        }
    }
}

void DbMetadataProvider::fillViewColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions, const QStringList &substitutes )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *columns = m_connection->viewColumns(schemaName, tableName);
    if ( columns ) {
        for (int i = 0; i < columns->size(); i++) {
            Database::IDbCatalogItem *column = columns->at(i);
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            if ( i < substitutes.size() )
                item->setText(substitutes[i]);
            else
                item->setText(column->name());
            item->setIcon(QIcon(column->iconName(m_connection->loader())));
            completions.append(item);
        }
    }
}

void DbMetadataProvider::fillTableConstraints( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *constraints = m_connection->tableConstraints(schemaName, tableName);
    if ( constraints ) {
        for (int i = 0; i < constraints->size(); i++) {
            Database::IDbCatalogItem *constraint = constraints->at(i);
                TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                item->setText(constraint->name());
                item->setIcon(QIcon(constraint->iconName(m_connection->loader())));
                completions.append(item);
        }
    }
}

void DbMetadataProvider::fillTableTriggers( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *triggers = m_connection->tableTriggers(schemaName, tableName);
    if ( triggers ) {
        for (int i = 0; i < triggers->size(); i++) {
            Database::IDbCatalogItem *constraint = triggers->at(i);
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item->setText(constraint->name());
            item->setIcon(QIcon(constraint->iconName(m_connection->loader())));
            completions.append(item);
        }
    }
}

void DbMetadataProvider::fillTableIndexes( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions )
{
    if ( !m_connection )
        return;

    QList<Database::IDbCatalogItem *> *indexes = m_connection->tableIndexes(schemaName, tableName);
    if ( indexes ) {
        for (int i = 0; i < indexes->size(); i++) {
            Database::IDbCatalogItem *constraint = indexes->at(i);
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item->setText(constraint->name());
            item->setIcon(QIcon(constraint->iconName(m_connection->loader())));
            completions.append(item);
        }
    }
}

bool DbMetadataProvider::isTableInSchema( const QString &schemaName, const QString &tableName )
{
    if ( !m_connection )
        return false ;

    if ( QList<Database::IDbCatalogItem *> *tables = m_connection->schemaObjects(schemaName, Sql::Constants::DbSchemaObjectType::C_TABLE) ) {
        foreach ( Database::IDbCatalogItem *table, *tables )
            if ( !table->name().compare(tableName, Qt::CaseInsensitive) )
                return true;
    }
    return false;
}

void DbMetadataProvider::fillTriggerSchemaTable( const QString &triggerName, QList<TextEditor::BasicProposalItem *> &completions )
{
    QList<Database::IDbCatalogItem *> *schemas = m_connection->schemas();
    if ( !schemas )
        return;

    foreach ( Database::IDbCatalogItem *schemaItem, *schemas ) {
        QList<Database::IDbCatalogItem *> *triggers = m_connection->schemaObjects( schemaItem->name(), Sql::Constants::DbSchemaObjectType::C_TRIGGER );
        foreach ( Database::IDbCatalogItem *triggerItem, *triggers ) {
            if ( !triggerItem->name().compare(triggerName, Qt::CaseInsensitive) ) {
                TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                item->setText("ON " + schemaItem->name() + "." + triggerItem->tableName(m_connection->loader(), m_connection->connectionInfo()));
                completions.append(item);
            }
        }
    }
}

void DbMetadataProvider::fillAllTriggers( QList<TextEditor::BasicProposalItem *> &completions )
{

    QList<Database::IDbCatalogItem *> *schemas = m_connection->schemas();
    if ( !schemas )
        return;

    foreach ( Database::IDbCatalogItem *schemaItem, *schemas ) {
        QList<Database::IDbCatalogItem *> *triggers = m_connection->schemaObjects( schemaItem->name(), Sql::Constants::DbSchemaObjectType::C_TRIGGER );
        foreach ( Database::IDbCatalogItem *triggerItem, *triggers ) {
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item->setText(triggerItem->name());
            item->setIcon(QIcon(triggerItem->iconName(m_connection->loader())));
            completions.append(item);
        }
    }
}

bool DbMetadataProvider::checkSchemaName( const QString &schemaName )
{
    if ( !m_connection )
        return false;

    QList<Database::IDbCatalogItem *> *schemas = m_connection->schemas();
    if ( !schemas )
        return false;

    for (int i = 0; i < schemas->size(); i++)
        if ( !schemas->at(i)->name().compare(schemaName, Qt::CaseInsensitive) )
            return true;

    return false;
}

QString DbMetadataProvider::defaultSchemaName() const
{
    if ( !m_connection )
        return QStringLiteral("");

    return m_connection->defaultSchemaName();
}

void DbMetadataProvider::onCurrentDbConnectionChanged( QVariantList &valueList )
{
    if ( valueList.isEmpty() )
        m_connection = 0;
    else {
        m_connection = (Database::DbConnection *) valueList[0].value<void *>();
    }
}

} // Sql
