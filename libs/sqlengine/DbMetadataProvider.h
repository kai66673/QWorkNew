#ifndef DBMETADATAPROVIDER_H
#define DBMETADATAPROVIDER_H

#include <QObject>
#include <QVariantList>

#include "sqlengine_global.h"
#include "DbConnection.h"
#include "basicproposalitem.h"

namespace Sql {

class SQL_EXPORT DbMetadataProvider : public QObject
{
    Q_OBJECT

public:
    explicit DbMetadataProvider(QObject *parent = 0);

    static DbMetadataProvider *getInstance();

    bool checkSchemaName( const QString &schemaName );
    QString defaultSchemaName() const;

    void fillSchemas( QList<TextEditor::BasicProposalItem *> &completions );
    void fillSchemaObjects( const QString &schemaName, QList<TextEditor::BasicProposalItem *> &completions , unsigned objectType );
    void fillTableColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions, const QStringList &substitutes = QStringList() );
    void fillViewColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions, const QStringList &substitutes = QStringList() );
    void fillTableConstraints( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions );
    void fillTableTriggers( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions );
    void fillTableIndexes( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> &completions );

    bool isTableInSchema( const QString &schemaName, const QString &tableName );

    Database::DbConnection *connection() const { return m_connection; }

    void fillTriggerSchemaTable( const QString &triggerName, QList<TextEditor::BasicProposalItem *> &completions );
    void fillAllTriggers( QList<TextEditor::BasicProposalItem *> &completions );

private slots:
    void onCurrentDbConnectionChanged( QVariantList &valueList );

private:
    static DbMetadataProvider *m_instance;

    Database::DbConnection *m_connection;
};

} // Sql

#endif // DBMETADATAPROVIDER_H
