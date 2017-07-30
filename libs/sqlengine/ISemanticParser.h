#ifndef ISEMANTICPARSER_H
#define ISEMANTICPARSER_H

#include <QList>
#include <QTextCursor>

#include "sqlengine_global.h"
#include "sql_constants.h"
#include "ASTfwd.h"
#include "basicproposalitem.h"
#include "SqlStatement.h"
#include "ILink.h"
#include "DbConnection.h"

class ITextDocument;

namespace Sql {

class SQL_EXPORT ISemanticParser
{
public:
    virtual ~ISemanticParser() {}
    virtual BaseWalker *parseDocument( ParseTarget target ) = 0;
    virtual void fillSqlStatements( QList<SqlStatement *> *statements,
                                    Database::ExecuteType executeType, ITextDocument *textDocument ) = 0;
    virtual void fillStatementFromText( QList<SqlStatement *> *statements, const QString &statementText ) = 0;
    virtual int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, unsigned position,
                                 int startPosition, unsigned completionOperator ) = 0;
    virtual Core::ILink *findLinkAt( const QTextCursor &/*tc*/ ) { return 0; }
};

}   // namespace Sql

#endif // ISEMANTICPARSER_H
