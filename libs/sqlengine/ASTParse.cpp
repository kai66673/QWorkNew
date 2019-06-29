#include "AST.h"
#include "Parser.h"
#include "SqlLexer.h"

#include <QDebug>

using namespace Sql;

void StatementAST::skip( Parser *p )
{
    if ( p->skipUntil(T_SEMICOLON) )
        p->consumeToken();
}

bool PkColumnConstraintAST::parse( Parser *p )
{
    primary_token = p->consumeToken();

    if ( p->LA1() != T_3_KEY ) {
        p->errorCursor("Expected token `KEY'");
        return false;
    }
    key_token = p->consumeToken();

    if ( p->LA1() == T_3_ASC || p->LA1() == T_4_DESC )
        asc_or_desc_token = p->consumeToken();

    return true;
}

bool NotNullColumnConstraintAST::parse( Parser *p )
{
    not_token = p->consumeToken();

    if ( p->LA1() != T_4_NULL ) {
        p->errorCursor("Expected token `NULL'");
        return false;
    }
    null_token = p->consumeToken();

    return true;
}


bool CheckColumnConstraintAST::parse( Parser *p )
{
    check_token = p->consumeToken();

    if ( p->LA() != T_LPAREN ) {
        p->errorCursor("Expected token `('");
        return false;
    }
    lparen_token = p->consumeToken();

    if ( !p->parseConditionExpression(checkExpr) )
        return false;

    if ( p->LA() != T_RPAREN ) {
        p->errorCursor("Expected token `)'");
        return false;
    }
    rparen_token = p->consumeToken();

    return true;
}

bool UniqueOrPkTableConstraintAST::parse( Parser *p )
{
    if ( p->LA1() == T_7_PRIMARY ) {
        unique_or_primary_token = p->consumeToken();
        if ( p->LA1() == T_3_KEY )
            key_token = p->consumeToken();
        else
            p->errorCursor("Expected token `KEY'");
    }
    else if ( p->LA1() == T_6_UNIQUE ) {
        unique_or_primary_token = key_token = p->consumeToken();
    }
    else
        return false;

    if ( p->LA() != T_LPAREN ) {
        p->errorCursor("Expected token `('");
        return false;
    }
    lparen_token = p->consumeToken();

    p->parseSortedColumnNameList(column_list);

    if ( p->LA() != T_RPAREN ) {
        p->errorCursor("Expected token `)'");
        return false;
    }
    rparen_token = p->consumeToken();

    return true;
}

bool CheckTableConstraintAST::parse( Parser *p )
{
    check_token = p->consumeToken();

    if ( p->LA() != T_LPAREN ) {
        p->errorCursor("Expected token `('");
        return false;
    }
    lparen_token = p->consumeToken();

    if ( !p->parseConditionExpression(checkExpr) )
        return false;

    if ( p->LA() != T_RPAREN ) {
        p->errorCursor("Expected token `)'");
        return false;
    }
    rparen_token = p->consumeToken();

    return true;
}

bool FkReferencesClauseAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    references_token = p->consumeToken();

    refSchemaTableName = new (pool) SchemaTableNameAST;
    if ( !refSchemaTableName->parse(p, pool) )
        return false;

    if ( p->LA() != T_LPAREN ) {
        p->errorCursor("Expected token `('");
        return false;
    }
    ref_lparen_token = p->consumeToken();

    if ( !p->parseColumnList(ref_col_list) )
        return false;

    if ( p->LA() != T_RPAREN ) {
        p->errorCursor("Expected token `)'");
        return false;
    }
    ref_rparen_token = p->consumeToken();

    return true;
}

bool ReferencesColumnConstraintAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    refClause = new (pool) FkReferencesClauseAST;
    return refClause->parse(p, pool);
}

bool FkTableConstraintAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    foreign_token = p->consumeToken();

    if ( p->LA1() != T_3_KEY ) {
        p->errorCursor("Expected token `KEY'");
        return false;
    }
    key_token = p->consumeToken();

    if ( p->LA() != T_LPAREN ) {
        p->errorCursor("Expected token `('");
        return false;
    }
    lparen_token = p->consumeToken();

    if ( !p->parseColumnList(col_list) )
        return false;

    if ( p->LA() != T_RPAREN ) {
        p->errorCursor("Expected token `)'");
        return false;
    }
    rparen_token = p->consumeToken();

    if ( p->LA1() != T_10_REFERENCES ) {
        p->errorCursor("Expected token `REFERENCES'");
        return false;
    }
    refClause = new (pool) FkReferencesClauseAST;
    return refClause->parse(p, pool);
}

bool AlterTableStatementAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    alter_token = p->consumeToken();
    table_token = p->consumeToken();

    if ( !p->isPatentialIdentifier() ) {
        p->errorCursor("Expected table name");
        return false;
    }
    schemaTableName = new (pool) SchemaTableNameAST;
    schemaTableName->parse(p, pool);

    switch ( p->LA1() ) {
        case T_4_DROP: {
            AlterTableDropClauseAST *dropClause = new (pool) AlterTableDropClauseAST;
            alterTableClause = dropClause;
            return dropClause->parse(p, pool);
        }
        case T_3_ADD: {
            switch ( p->LA1(2) ) {
                case T_10_CONSTRAINT: {
                    AlterTableAddConstraintClauseAST *addClause = new (pool) AlterTableAddConstraintClauseAST;
                    alterTableClause = addClause;
                    return addClause->parse(p, pool);
                }
                case T_6_COLUMN: {
                    AlterTableAddColumnClauseAST *addClause = new (pool) AlterTableAddColumnClauseAST;
                    alterTableClause = addClause;
                    return addClause->parse(p, pool);
                }
            }
            p->errorCursor("Expected `CONSTRAINT' or `COLUMN' token");
            return false;
        }
        case T_6_MODIFY:
            if ( p->LA1(2) == T_6_COLUMN ) {
                AlterTableModifyColumnClauseAST *modifyClause = new (pool) AlterTableModifyColumnClauseAST;
                alterTableClause = modifyClause;
                return modifyClause->parse(p, pool);
            }
            p->errorCursor("Expected `COLUMN' token");
            return false;
    }

    p->errorCursor("Expected `ADD' or `DROP' token");
    return false;
}

unsigned AlterTableStatementAST::targetKind() const
{
    if ( alterTableClause && alterTableClause->objectKind == Sql::AlterTableClauseAST::Constraint )
        return Sql::Constants::DbSchemaObjectType::C_CONSTRAINT;

    return Sql::Constants::DbSchemaObjectType::C_TABLE;
}

bool AlterTableStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
    if ( !alterTableClause )
        return false;

    if ( !schemaTableName )
        return false;

    if ( !schemaTableName->tableName )
        return false;

    *objectName = schemaTableName->tableName->name->chars();

    if ( schemaTableName->schemaName )
        *schemaName = schemaTableName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

bool AlterTableStatementAST::targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const
{
    if ( !schemaTableName )
        return false;

    if ( !schemaTableName->tableName )
        return false;

    if ( !alterTableClause )
        return false;

    *tableName = schemaTableName->tableName->name->chars();
    switch ( alterTableClause->objectKind ) {
        case Sql::AlterTableClauseAST::Unknown:     return false;
        case Sql::AlterTableClauseAST::Constraint:  *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_CONSTRAINT; break;
        case Sql::AlterTableClauseAST::Field:       *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_COLUMN; break;
        case Sql::AlterTableClauseAST::Index:       *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_INDEX; break;
    }

    return true;
}

bool AlterTableDropClauseAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    drop_token = p->consumeToken();

    switch ( p->LA1() ) {
        case T_5_INDEX: {
            drop_object_type_token = p->consumeToken();
            if ( !p->isPatentialIdentifier() ) {
                p->errorCursor("Expected index name");
                return false;
            }
            const Token &tk = p->tok();
            dropObjectName = new (pool) IndexNameAST;
            dropObjectName->name = tk.identifier;
            dropObjectName->name_token = p->consumeToken();
            objectKind = Index;
            break;
        }
        case T_10_CONSTRAINT: {
            drop_object_type_token = p->consumeToken();
            if ( !p->isPatentialIdentifier() ) {
                p->errorCursor("Expected constraint name");
                return false;
            }
            const Token &tk = p->tok();
            dropObjectName = new (pool) ConstraintNameAST;
            dropObjectName->name = tk.identifier;
            dropObjectName->name_token = p->consumeToken();
            objectKind = Constraint;
            break;
        }
        case T_6_COLUMN: {
            drop_object_type_token = p->consumeToken();
            if ( !p->isPatentialIdentifier() ) {
                p->errorCursor("Expected column name");
                return false;
            }
            const Token &tk = p->tok();
            dropObjectName = new (pool) ColumnNameAST;
            dropObjectName->name = tk.identifier;
            dropObjectName->name_token = p->consumeToken();
            objectKind = Field;
            break;
        }
        case T_7_FOREIGN: {
            drop_object_type_token = p->consumeToken();
            if ( p->LA1() != T_3_KEY ) {
                p->errorCursor("Expected token `KEY`");
                return false;
            }
            drop_object_type_token1 = p->consumeToken();
            if ( !p->isPatentialIdentifier() ) {
                p->errorCursor("Expected constraint name");
                return false;
            }
            const Token &tk = p->tok();
            dropObjectName = new (pool) ConstraintNameAST;
            dropObjectName->name = tk.identifier;
            dropObjectName->name_token = p->consumeToken();
            objectKind = Constraint;
            break;
        }
        default:
            p->errorCursor("Expected `CONSTRAINT' or `COLUMN' token");
            return false;
    }

    return true;
}

bool AlterTableAddConstraintClauseAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    add_token = p->consumeToken();
    unsigned constraint_token = p->consumeToken();
    ConstraintNameAST *constraintName = nullptr;
    if ( p->isPatentialIdentifier() ) {
        constraintName = new (pool) ConstraintNameAST;
        const Token &tk = p->tok();
        constraintName->name = tk.identifier;
        constraintName->name_token = p->consumeToken();
    }
    else {
        p->errorCursor("Constraint name expected");
    }
    objectKind = Constraint;

    bool notCheckEndCorrect = true;
    switch ( p->LA1() ) {
        default:
            notCheckEndCorrect = false;
            [[clang::fallthrough]];
        case T_6_UNIQUE:
        case T_7_PRIMARY: {
            UniqueOrPkTableConstraintAST *constraintPK = new (pool) UniqueOrPkTableConstraintAST;
            constraintPK->constraint_token = constraint_token;
            constraintPK->constraintName = constraintName;
            if ( notCheckEndCorrect )
                notCheckEndCorrect = constraintPK->parse(p);
            constraint = constraintPK;
            break;
        }
        case T_5_CHECK: {
            CheckTableConstraintAST *constraintCheck = new (pool) CheckTableConstraintAST;
            constraintCheck->constraint_token = constraint_token;
            constraintCheck->constraintName = constraintName;
            notCheckEndCorrect = constraintCheck->parse(p);
            constraint = constraintCheck;
            break;
        }
        case T_7_FOREIGN: {
            FkTableConstraintAST *constraintFK = new (pool) FkTableConstraintAST;
            constraintFK->constraint_token = constraint_token;
            constraintFK->constraintName = constraintName;
            notCheckEndCorrect = constraintFK->parse(p, pool);
            constraint = constraintFK;
            break;
        }
    }
    return notCheckEndCorrect;
}

bool AlterTableAddColumnClauseAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    add_token = p->consumeToken();
    column_token = p->consumeToken();
    objectKind = Field;

    if ( p->isPatentialIdentifier() ) {
        ColumnDefinitionAST *columnDef = new (pool) ColumnDefinitionAST;
        columnDef->columnName = new (pool) ColumnNameAST;
        const Token &tk = p->tok();
        columnDef->columnName->name = tk.identifier;
        columnDef->columnName->name_token = p->consumeToken();
        if ( p->LA3() ) {
            p->parseTypeSpecifier(columnDef->columnType);
        }
        else {
            p->errorCursor("Expected type specifier");
        }

        if ( p->LA1() == T_7_DEFAULT ) {
            columnDef->default_token = p->consumeToken();
            if ( p->LA1() == T_4_NULL ) {
                NullValueExpressionAST *nullExpr = new (pool) NullValueExpressionAST;
                nullExpr->null_token = p->consumeToken();
                columnDef->defaultExpr = nullExpr;
            }
            else if ( p->LA() == T_NUMERIC_LITERAL ) {
                NumericExpressionAST *numericAst = new (pool) NumericExpressionAST;
                numericAst->numeric_token = p->consumeToken();
                columnDef->defaultExpr = numericAst;
            }
            else if ( p->LA() == T_CHAR_LITERAL ) {
                LiteralExpressionAST *literAst = new (pool) LiteralExpressionAST;
                literAst->liter_token = p->consumeToken();
                columnDef->defaultExpr = literAst;
            }
            else
                p->warningCursor("Expected default value");
        }

        column = columnDef;
        return p->parseColumnConstraintList(columnDef->constraintList);
    }

    return false;
}

bool AlterTableModifyColumnClauseAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    modify_token = p->consumeToken();
    column_token = p->consumeToken();
    objectKind = Field;

    if ( p->isPatentialIdentifier() ) {
        ColumnDefinitionAST *columnDef = new (pool) ColumnDefinitionAST;
        columnDef->columnName = new (pool) ColumnNameAST;
        const Token &tk = p->tok();
        columnDef->columnName->name = tk.identifier;
        columnDef->columnName->name_token = p->consumeToken();
        if ( p->LA3() ) {
            p->parseTypeSpecifier(columnDef->columnType);
        }
        else {
            p->errorCursor("Expected type specifier");
        }

        if ( p->LA1() == T_7_DEFAULT ) {
            columnDef->default_token = p->consumeToken();
            if ( p->LA1() == T_4_NULL ) {
                NullValueExpressionAST *nullExpr = new (pool) NullValueExpressionAST;
                nullExpr->null_token = p->consumeToken();
                columnDef->defaultExpr = nullExpr;
            }
            else if ( p->LA() == T_NUMERIC_LITERAL ) {
                NumericExpressionAST *numericAst = new (pool) NumericExpressionAST;
                numericAst->numeric_token = p->consumeToken();
                columnDef->defaultExpr = numericAst;
            }
            else if ( p->LA() == T_CHAR_LITERAL ) {
                LiteralExpressionAST *literAst = new (pool) LiteralExpressionAST;
                literAst->liter_token = p->consumeToken();
                columnDef->defaultExpr = literAst;
            }
            else
                p->warningCursor("Expected default value");
        }

        column = columnDef;
        return p->parseColumnConstraintList(columnDef->constraintList);
    }

    return false;
}

bool SchemaTableNameAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    if ( !p->isPatentialIdentifier() ) {
        p->errorCursor("Expected table name");
        return false;
    }

    if ( p->LA(2) == T_DOT ) {
        const Token &stk = p->tok();
        schemaName = new (pool) SchemaNameAST;
        schemaName->name = stk.identifier;
        schemaName->name_token = p->consumeToken();
        dot_token = p->consumeToken();
        if ( !p->isPatentialIdentifier() ) {
            p->errorCursor("Expected table name");
            return true;
        }
    }

    const Token &ttk = p->tok();
    tableName = new (pool) TableNameAST;
    tableName->name = ttk.identifier;
    tableName->name_token = p->consumeToken();

    return true;
}

bool SchemaIndexNameAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    if ( !p->isPatentialIdentifier() )
        return false;

    if ( p->LA(2) == T_DOT && p->isPatentialIdentifier(3) ) {
        const Token &stk = p->tok();
        schemaName = new (pool) SchemaNameAST;
        schemaName->name = stk.identifier;
        schemaName->name_token = p->consumeToken();
        dot_token = p->consumeToken();
    }

    const Token &ttk = p->tok();
    indexName = new (pool) IndexNameAST;
    indexName->name = ttk.identifier;
    indexName->name_token = p->consumeToken();

    return true;
}

bool SchemaTriggerNameAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    if ( !p->isPatentialIdentifier() )
        return false;

    if ( p->LA(2) == T_DOT && p->isPatentialIdentifier(3) ) {
        const Token &stk = p->tok();
        schemaName = new (pool) SchemaNameAST;
        schemaName->name = stk.identifier;
        schemaName->name_token = p->consumeToken();
        dot_token = p->consumeToken();
    }

    const Token &ttk = p->tok();
    triggerName = new (pool) TriggerNameAST;
    triggerName->name = ttk.identifier;
    triggerName->name_token = p->consumeToken();

    return true;
}

bool CreateTriggerRefClauseAST::parse( Parser *p, Utils::MemoryPool *pool )
{
    bool hasNew = false;
    bool hasOld = false;

    referencing_token = p->consumeToken();

    while ( true ) {
        switch ( p->LA1() ) {
            case T_3_NEW: {
                if ( hasNew )
                    p->warningCursor("Already exists NEW referencing");
                else
                    hasNew = true;
                new_token = p->consumeToken();
                if ( p->LA1() != T_2_AS ) {
                    p->errorCursor("Expected AS token");
                    return false;
                }
                asn_token = p->consumeToken();
                if ( p->LA() != T_IDENTIFIER ) {
                    p->errorCursor("Expected Identifier");
                    return false;
                }
                const Token &tk = p->tok();
                newAlias = new (pool) TableAliasAST;
                newAlias->name = tk.identifier;
                newAlias->name_token = p->consumeToken();
                newAlias->schemaTableName = schemaTableName;
                break;
            }
            case T_3_OLD: {
                if ( hasOld )
                    p->warningCursor("Already exists OLD refderencing");
                else
                    hasOld = true;
                old_token = p->consumeToken();
                if ( p->LA1() != T_2_AS ) {
                    p->errorCursor("Expected AS token");
                    return false;
                }
                aso_token = p->consumeToken();
                if ( p->LA() != T_IDENTIFIER ) {
                    p->errorCursor("Expected Identifier");
                    return false;
                }
                const Token &tk = p->tok();
                oldAlias = new (pool) TableAliasAST;
                oldAlias->name = tk.identifier;
                oldAlias->name_token = p->consumeToken();
                oldAlias->schemaTableName = schemaTableName;
                break;
            }
            default:
                if ( !hasNew && !hasOld ) {
                    p->errorCursor("Expected NEW|OLD token");
                    return false;
                }
                return true;
        }
    }

    return false;
}
