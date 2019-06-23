/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "AST.h"
#include "ASTVisitor.h"
#include "ASTMatcher.h"
#include "MemoryPool.h"
#include "sqlchecknamesvisitor.h"
#include "SqlStatementsBindVisitor.h"

#include <cassert>
#include <algorithm>

/*
   All firstToken/lastToken methods below which have a doxygen comment with
   \generated in it, will be re-generated when the tool "generate-ast" is run.

   For methods which are hand-coded, or which should not be changed, make sure that
   the comment is gone.
 */


using namespace Sql;

AST::AST()
{ }

AST::~AST()
{ assert(0); }

void AST::accept(ASTVisitor *visitor)
{
    if (visitor->preVisit(this))
        accept0(visitor);
    visitor->postVisit(this);
}

bool AST::match(AST *ast, AST *pattern, ASTMatcher *matcher)
{
    if (ast == pattern)
        return true;

    else if (! ast || ! pattern)
        return false;

    return ast->match(pattern, matcher);
}

bool AST::match(AST *pattern, ASTMatcher *matcher)
{
    return match0(pattern, matcher);
}

unsigned TranslationUnitAST::firstToken() const
{
    if ( statement_list )
        if ( unsigned candidate = statement_list->firstToken() )
            return candidate;
    return 0;
}

unsigned TranslationUnitAST::lastToken() const
{
    if ( statement_list )
        if ( unsigned candidate = statement_list->lastToken() )
            return candidate;
    return 1;
}

unsigned UnknownStatementAST::firstToken() const
{
    return first_token;
}

unsigned UnknownStatementAST::lastToken() const
{
    return last_token;
}

unsigned SelectStatementAST::firstToken() const
{
    return select_core_list->firstToken();
}

unsigned SelectStatementAST::lastToken() const
{
    if ( last_token )
        return last_token;
    if ( limit_to_block )
        return limit_to_block->lastToken();
    if ( order_by_block )
        return order_by_block->lastToken() + 1;
    unsigned _last_token1 = select_compound_list->lastToken() + 1;
    unsigned _last_token2 = select_core_list->lastToken() + 1;
    return (_last_token1 > _last_token2 ? _last_token1 : _last_token2);
}

unsigned SelectCoreAST::firstToken() const
{
    return select_token;
}

unsigned SelectCoreAST::lastToken() const
{
    if ( groupBy )
        return groupBy->lastToken();
    if ( whereExpr )
        return whereExpr->lastToken();
    if ( where_token )
        return where_token + 1;
    if ( joinSource )
        return joinSource->lastToken();
    if ( from_token )
        return from_token + 1;
    if ( select_spec_token )
        return select_spec_token + 1;
    if ( columnList )
        return columnList->lastToken();
    return select_token + 1;
}

unsigned SelectCompoundUnionAST::firstToken() const
{
    return union_token;
}

unsigned SelectCompoundUnionAST::lastToken() const
{
    if ( union_spec_token )
        return union_spec_token;
    return union_token + 1;
}
unsigned SelectCompoundIntersectAST::firstToken() const
{
    return intersect_token;
}

unsigned SelectCompoundIntersectAST::lastToken() const
{
    return intersect_token + 1;
}

unsigned SelectCompoundExceptAST::firstToken() const
{
    return except_token;
}

unsigned SelectCompoundExceptAST::lastToken() const
{
    return except_token + 1;
}

unsigned SelectCompoundMinusAST::firstToken() const
{
    return minus_token;
}

unsigned SelectCompoundMinusAST::lastToken() const
{
    return minus_token + 1;
}

unsigned OrderByBlockAST::firstToken() const
{
    return order_token;
}

unsigned OrderByBlockAST::lastToken() const
{
    if ( term_list )
        return term_list->lastToken();
    if ( by_token )
        return by_token + 1;
    return order_token + 1;
}

unsigned LimitToBlockAST::firstToken() const
{
    return limit_token;
}

unsigned LimitToBlockAST::lastToken() const
{
    if ( row_token )
        return row_token;
    if ( expr )
        return expr->lastToken();
    return to_token + 1;
}

unsigned OrderingTermAST::firstToken() const
{
    if ( expr )
        return expr->firstToken();
    if ( asc_or_desc_token )
        return asc_or_desc_token;
    return 0;
}

unsigned OrderingTermAST::lastToken() const
{
    if ( asc_or_desc_token )
        return asc_or_desc_token + 1;
    if ( expr )
        return expr->lastToken();
    return 1;
}

unsigned StarResultColumnAST::firstToken() const
{
    if ( schemaTableName )
        return schemaTableName->firstToken();
    if ( dot_token )
        return dot_token;
    return star_token;
}

unsigned StarResultColumnAST::lastToken() const
{
    return star_token + 1;
}

unsigned SchemaTableNameAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( dot_token )
        return dot_token;
    if ( tableName )
        return tableName->firstToken();
    return 0;
}

unsigned SchemaTableNameAST::lastToken() const
{
    if ( tableName )
        return tableName->lastToken();
    if ( dot_token )
        return dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

unsigned SchemaIndexNameAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( dot_token )
        return dot_token;
    if ( indexName )
        return indexName->firstToken();
    return 0;
}

unsigned SchemaIndexNameAST::lastToken() const
{
    if ( indexName )
        return indexName->lastToken();
    if ( dot_token )
        return dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

unsigned SchemaTriggerNameAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( dot_token )
        return dot_token;
    if ( triggerName )
        return triggerName->firstToken();
    return 0;
}

unsigned SchemaTriggerNameAST::lastToken() const
{
    if ( triggerName )
        return triggerName->lastToken();
    if ( dot_token )
        return dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

unsigned ColumnNameAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( tableOfColumn )
        return tableOfColumn->firstToken();
    return name_token;
}

unsigned ColumnNameAST::lastToken() const
{
    if ( name_token )
        return name_token + 1;
    if ( tableOfColumn )
        return tableOfColumn->lastToken();
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

unsigned ExprResultColumnAST::firstToken() const
{
    if ( expr )
        return expr->firstToken();
    if ( as_token )
        return as_token;
    if ( columnAlias )
        return columnAlias->firstToken();
    return 0;
}

unsigned ExprResultColumnAST::lastToken() const
{
    if ( columnAlias )
        return columnAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( expr )
        return expr->lastToken();
    return 1;
}

unsigned SubQueryAST::firstToken() const
{
    return lparen_token;
}

unsigned SubQueryAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( selectStatement )
        return selectStatement->lastToken();
    return lparen_token + 1;
}

unsigned DirectSingleSourceAST::firstToken() const
{
    if ( schemaTableName )
        return schemaTableName->firstToken();
    if ( as_token )
        return as_token;
    if ( tableAlias )
        return tableAlias->firstToken();
    return 0;
}

unsigned DirectSingleSourceAST::lastToken() const
{
    if ( tableAlias )
        return tableAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( schemaTableName )
        return schemaTableName->lastToken();
    return 1;
}

unsigned SubQuerySingleSourceAST::firstToken() const
{
    if ( subQuery )
        return subQuery->firstToken();
    if ( as_token )
        return as_token;
    if ( tableAlias )
        return tableAlias->firstToken();
    return 0;
}

unsigned SubQuerySingleSourceAST::lastToken() const
{
    if ( tableAlias )
        return tableAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( subQuery )
        return subQuery->lastToken();
    return 1;
}

unsigned JoinConstraintAST::firstToken() const
{
    return on_token;
}

unsigned JoinConstraintAST::lastToken() const
{
    if ( expr )
        return expr->lastToken();
    return on_token + 1;
}

unsigned JoinItemAST::firstToken() const
{
    if ( joinOp )
        return joinOp->firstToken();
    if ( singleSource )
        return singleSource->firstToken();
    if ( joinConstraint )
        return joinConstraint->firstToken();
    return 0;
}

unsigned JoinItemAST::lastToken() const
{
    if ( joinConstraint )
        return joinConstraint->lastToken();
    if ( singleSource )
        return singleSource->lastToken();
    if ( joinOp )
        return joinOp->lastToken();
    return 1;
}

unsigned JoinSourceAST::firstToken() const
{
    if ( firstSource )
        return firstSource->firstToken();
    if ( source_list )
        return source_list->firstToken();
    return 0;
}

unsigned JoinSourceAST::lastToken() const
{
    if ( source_list )
        return source_list->lastToken();
    if ( firstSource )
        return firstSource->lastToken();
    return 1;
}

unsigned GroupByBlockAST::firstToken() const
{
    return group_token;
}

unsigned GroupByBlockAST::lastToken() const
{
    if ( havingExpr )
        return havingExpr->lastToken();
    if ( having_token )
        return having_token + 1;
    if ( group_items_list )
        return group_items_list->lastToken();
    if ( by_token )
        return by_token + 1;
    return group_token + 1;
}

unsigned DeleteStatementAST::firstToken() const
{
    return delete_token;
}

unsigned DeleteStatementAST::lastToken() const
{
    if ( whereExpr )
        return whereExpr->lastToken();
    if ( where_token )
        return where_token + 1;
    if ( source )
        return source->lastToken();
    if ( from_token )
        return from_token + 1;
    return delete_token + 1;
}

unsigned InsertStatementAST::firstToken() const
{
    return insert_token;
}

unsigned InsertStatementAST::lastToken() const
{
    if ( insClause )
        return insClause->lastToken();
    if ( rparen_columns_token )
        return rparen_columns_token + 1;
    if ( columns_list )
        return columns_list->lastToken();
    if ( lparen_columns_token )
        return lparen_columns_token + 1;
    if ( source )
        return source->lastToken();
    if ( into_token )
        return into_token + 1;
    return insert_token + 1;
}

unsigned UpdateStatementAST::firstToken() const
{
    return update_token;
}

unsigned UpdateStatementAST::lastToken() const
{
    if ( whereExpr )
        return whereExpr->lastToken();
    if ( where_token )
        return where_token + 1;
    if ( upd_list )
        return upd_list->lastToken();
    if ( set_token )
        return set_token + 1;
    if ( source )
        return source->lastToken();
    return update_token + 1;
}

unsigned CreateTableDirectClauseAST::firstToken() const
{
    return lparen_token;
}

unsigned CreateTableDirectClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( column_def_list )
        return column_def_list->lastToken();
    if ( constraintList )
        constraintList->lastToken();
    return lparen_token + 1;
}

unsigned CreateTableAsSelectClauseAST::firstToken() const
{
    return as_token;
}

unsigned CreateTableAsSelectClauseAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    return as_token + 1;
}

unsigned CreateTriggerRefClauseAST::firstToken() const
{
    return referencing_token;
}

unsigned CreateTriggerRefClauseAST::lastToken() const
{
    unsigned lastToken_ = 0;

    if ( oldAlias )
        lastToken_ = oldAlias->lastToken() - 1;
    if ( aso_token )
        if ( aso_token > lastToken_ )
            lastToken_ = aso_token;
    if ( old_token )
        if ( old_token > lastToken_ )
            lastToken_ = old_token;
    if ( newAlias )
        if ( newAlias->lastToken() > lastToken_ )
            lastToken_ = newAlias->lastToken() - 1;
    if ( asn_token )
        if ( asn_token > lastToken_ )
            lastToken_ = asn_token;
    if ( new_token )
        if ( new_token > lastToken_ )
            lastToken_ = new_token;

    if ( lastToken_ )
        return lastToken_ + 1;

    return referencing_token + 1;
}

unsigned CreateRdbTriggerBodyAST::firstToken() const
{
    if ( item_list )
        return item_list->firstToken();
    return 0;
}

unsigned CreateRdbTriggerBodyAST::lastToken() const
{
    if ( item_list )
        return item_list->lastToken();
    return 1;
}

unsigned CreateRdbTriggerBodyItemAST::firstToken() const
{
    if ( when_token )
        return when_token;
    if ( lparen_when_token )
        return lparen_when_token;
    if ( whenExpr )
        return whenExpr->firstToken();
    if ( rparen_when_token )
        return rparen_when_token;
    if ( lparen_token )
        return lparen_token;
    if ( stat_list )
        return stat_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    if ( for_token )
        return for_token;
    if ( each_token )
        return each_token;
    if ( row_or_statement_token )
        return row_or_statement_token;
    return 0;
}

unsigned CreateRdbTriggerBodyItemAST::lastToken() const
{
    if ( row_or_statement_token )
        return row_or_statement_token + 1;
    if ( each_token )
        return each_token + 1;
    if ( for_token )
        return for_token + 1;
    if ( rparen_token )
        return rparen_token + 1;
    if ( stat_list )
        return stat_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( rparen_when_token )
        return rparen_when_token + 1;
    if ( whenExpr )
        return whenExpr->lastToken();
    if ( lparen_when_token )
        return lparen_when_token + 1;
    if ( when_token )
        return when_token + 1;
    return 1;
}

unsigned ColumnDefinitionAST::firstToken() const
{
    if ( columnName )
        return columnName->firstToken();
    return 0;
}

unsigned ColumnDefinitionAST::lastToken() const
{
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( default_token )
        return default_token + 1;
    if ( columnType )
        return columnType->lastToken();
    if ( columnName )
        return columnName->lastToken();
    if ( constraintList )
        return constraintList->lastToken();
    return 0;
}

unsigned UniqueOrPkTableConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( unique_or_primary_token )
        return unique_or_primary_token;
    if ( key_token )
        return key_token;
    if ( lparen_token )
        return lparen_token;
    if ( column_list )
        return column_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

unsigned UniqueOrPkTableConstraintAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( column_list )
        return column_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( key_token )
        return key_token + 1;
    if ( unique_or_primary_token )
        return unique_or_primary_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned CheckTableConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( check_token )
        return check_token;
    if ( lparen_token )
        return lparen_token;
    if ( checkExpr )
        return checkExpr->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

unsigned CheckTableConstraintAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( checkExpr )
        return checkExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( check_token )
        return check_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned FkTableConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( foreign_token )
        return foreign_token;
    if ( key_token )
        return key_token;
    if ( lparen_token )
        return lparen_token;
    if ( col_list )
        return col_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    if ( refClause )
        return refClause->firstToken();
    return 0;
}

unsigned FkTableConstraintAST::lastToken() const
{
    if ( refClause )
        return refClause->lastToken();
    if ( rparen_token )
        return rparen_token + 1;
    if ( col_list )
        return col_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( key_token )
        return key_token + 1;
    if ( foreign_token )
        return foreign_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned FkReferencesClauseAST::firstToken() const
{
    if ( references_token )
        return references_token;
    if ( refSchemaTableName )
        return refSchemaTableName->firstToken();
    if ( ref_lparen_token )
        return ref_lparen_token;
    if ( ref_col_list )
        return ref_col_list->firstToken();
    if ( ref_rparen_token )
        return ref_rparen_token;
    return 0;
}

unsigned FkReferencesClauseAST::lastToken() const
{
    if ( ref_rparen_token )
        return ref_rparen_token + 1;
    if ( ref_col_list )
        return ref_col_list->lastToken();
    if ( ref_lparen_token )
        return ref_lparen_token + 1;
    if ( refSchemaTableName )
        return refSchemaTableName->lastToken();
    if ( references_token )
        return references_token + 1;
    return 1;
}

unsigned PkColumnConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( primary_token )
        return primary_token;
    if ( key_token )
        return key_token;
    if ( asc_or_desc_token )
        return asc_or_desc_token;
    return 0;
}

unsigned PkColumnConstraintAST::lastToken() const
{
    if ( asc_or_desc_token )
        return asc_or_desc_token + 1;
    if ( key_token )
        return key_token + 1;
    if ( primary_token )
        return primary_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned NotNullColumnConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( not_token )
        return not_token;
    if ( null_token )
        return null_token;
    return 0;
}

unsigned NotNullColumnConstraintAST::lastToken() const
{
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned UniqueColumnConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( unique_token )
        return unique_token;
    return 0;
}

unsigned UniqueColumnConstraintAST::lastToken() const
{
    if ( unique_token )
        return unique_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned CheckColumnConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( check_token )
        return check_token;
    if ( lparen_token )
        return lparen_token;
    if ( checkExpr )
        return checkExpr->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

unsigned CheckColumnConstraintAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( checkExpr )
        return checkExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( check_token )
        return check_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

unsigned ReferencesColumnConstraintAST::firstToken() const
{
    if ( refClause )
        return refClause->firstToken();
    return 0;
}

unsigned ReferencesColumnConstraintAST::lastToken() const
{
    if ( refClause )
        return refClause->lastToken();
    return 1;
}

unsigned SortedColumnNameAST::firstToken() const
{
    return columnName->firstToken();
}

unsigned SortedColumnNameAST::lastToken() const
{
    if ( asc_or_desc_token )
        return asc_or_desc_token + 1;
    return columnName->lastToken();
}

unsigned InsertClauseAsValuesAST::firstToken() const
{
    return values_token;
}

unsigned InsertClauseAsValuesAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( val_list )
        return val_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return values_token;
}

unsigned InsertClauseAsSelectAST::firstToken() const
{
    if ( selectStatement )
        return selectStatement->firstToken();
    return 0;
}

unsigned InsertClauseAsSelectAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    return 1;
}

bool CreateTableStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
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

unsigned CreateTableStatementAST::firstToken() const
{
    return create_token;
}

unsigned CreateTableStatementAST::lastToken() const
{
    if ( tableClause )
        return tableClause->lastToken();
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( table_token )
        return table_token + 1;
    return create_token + 1;
}

bool CreateViewStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
    if ( !schemaViewName )
        return false;

    if ( !schemaViewName->tableName )
        return false;

    *objectName = schemaViewName->tableName->name->chars();

    if ( schemaViewName->schemaName )
        *schemaName = schemaViewName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

unsigned CreateViewStatementAST::firstToken() const
{
    return create_token;
}

unsigned CreateViewStatementAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( schemaViewName )
        return schemaViewName->lastToken();
    return create_token + 1;
}

bool CreateIndexStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
    if ( !schemaIndexName )
        return false;

    if ( !schemaIndexName->indexName )
        return false;

    *objectName = schemaIndexName->indexName->name->chars();

    if ( schemaIndexName->schemaName )
        *schemaName = schemaIndexName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

bool CreateIndexStatementAST::targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const
{
    if ( !schemaTableName )
        return false;

    if ( !schemaTableName->tableName )
        return false;

    *tableName = schemaTableName->tableName->name->chars();
    *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_INDEX;

    return true;
}

unsigned CreateIndexStatementAST::firstToken() const
{
    return create_token;
}

unsigned CreateIndexStatementAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( columns_list )
        return columns_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( on_token )
        return on_token + 1;
    if ( schemaIndexName )
        return schemaIndexName->lastToken();
    if ( index_token )
        return index_token + 1;
    if ( unique_token )
        return unique_token + 1;
    return create_token + 1;
}

bool CreateTriggerStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
    if ( !schemaTriggerName )
        return false;

    if ( !schemaTriggerName->triggerName )
        return false;

    *objectName = schemaTriggerName->triggerName->name->chars();

    if ( schemaTriggerName->schemaName )
        *schemaName = schemaTriggerName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

bool CreateTriggerStatementAST::targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const
{
    if ( !schemaTableName )
        return false;

    if ( !schemaTableName->tableName )
        return false;

    *tableName = schemaTableName->tableName->name->chars();
    *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_TRIGGER;

    return true;
}

unsigned CreateTriggerStatementAST::firstToken() const
{
    return create_token;
}

unsigned CreateTriggerStatementAST::lastToken() const
{
    if ( triggerBody )
        return triggerBody->lastToken();
    if ( triggerRefClause )
        return triggerRefClause->lastToken();
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( on_token )
        return on_token + 1;
    if ( action_token )
        return action_token + 1;
    if ( before_or_after_token )
        return before_or_after_token + 1;
    if ( schemaTriggerName )
        return schemaTriggerName->lastToken();
    if ( trigger_token )
        return trigger_token + 1;
    return create_token + 1;
}

bool DropTableStatementAST::targetSchemaObjectName(QString *schemaName, QString *objectName) const
{
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

unsigned DropTableStatementAST::firstToken() const
{
    return drop_token;
}

unsigned DropTableStatementAST::lastToken() const
{
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( table_token )
        return table_token + 1;
    return drop_token + 1;
}

bool DropViewStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
    if ( !schemaViewName )
        return false;

    if ( !schemaViewName->tableName )
        return false;

    *objectName = schemaViewName->tableName->name->chars();

    if ( schemaViewName->schemaName )
        *schemaName = schemaViewName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

unsigned DropViewStatementAST::firstToken() const
{
    return drop_token;
}

unsigned DropViewStatementAST::lastToken() const
{
    if ( schemaViewName )
        return schemaViewName->lastToken();
    if ( view_token )
        return view_token + 1;
    return drop_token + 1;
}

bool DropIndexStatementAST::targetSchemaObjectName(QString *schemaName, QString *objectName) const
{
    if ( !schemaIndexName )
        return false;

    if ( !schemaIndexName->indexName )
        return false;

    *objectName = schemaIndexName->indexName->name->chars();

    if ( schemaIndexName->schemaName )
        *schemaName = schemaIndexName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

unsigned DropIndexStatementAST::firstToken() const
{
    return drop_token;
}

unsigned DropIndexStatementAST::lastToken() const
{
    if ( schemaIndexName )
        return schemaIndexName->lastToken();
    if ( index_token )
        return index_token + 1;
    return drop_token + 1;
}

bool DropTriggerStatementAST::targetSchemaObjectName( QString *schemaName, QString *objectName ) const
{
    if ( !schemaTriggerName )
        return false;

    if ( !schemaTriggerName->triggerName )
        return false;

    *objectName = schemaTriggerName->triggerName->name->chars();

    if ( schemaTriggerName->schemaName )
        *schemaName = schemaTriggerName->schemaName->name->chars();
    else
        *schemaName = "";

    return true;
}

unsigned DropTriggerStatementAST::firstToken() const
{
    return drop_token;
}

unsigned DropTriggerStatementAST::lastToken() const
{
    if ( schemaTriggerName )
        return schemaTriggerName->lastToken();
    if ( trigger_token )
        return trigger_token + 1;
    return drop_token + 1;
}

unsigned TruncateStatementAST::firstToken() const
{
    return truncate_token;
}

unsigned TruncateStatementAST::lastToken() const
{
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( table_token )
        return table_token + 1;
    return truncate_token + 1;
}

unsigned AlterTableStatementAST::firstToken() const
{
    return alter_token;
}

unsigned AlterTableStatementAST::lastToken() const
{
    if ( alterTableClause )
        return alterTableClause->lastToken();
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( table_token )
        return table_token + 1;
    return alter_token + 1;
}

unsigned AlterTableDropClauseAST::firstToken() const
{
    return drop_token;
}

unsigned AlterTableDropClauseAST::lastToken() const
{
    if ( dropObjectName )
        return dropObjectName->lastToken();
    if ( drop_object_type_token )
        return drop_object_type_token + 1;
    return drop_token + 1;
}

unsigned AlterTableAddConstraintClauseAST::firstToken() const
{
    return add_token;
}

unsigned AlterTableAddConstraintClauseAST::lastToken() const
{
    if ( constraint )
        return constraint->lastToken();
    return add_token + 1;
}

unsigned AlterTableAddColumnClauseAST::firstToken() const
{
    return add_token;
}

unsigned AlterTableAddColumnClauseAST::lastToken() const
{
    if ( column )
        return column->lastToken();
    if ( column_token )
        return column_token + 1;
    return add_token + 1;
}

unsigned AlterTableModifyColumnClauseAST::firstToken() const
{
    return modify_token;
}

unsigned AlterTableModifyColumnClauseAST::lastToken() const
{
    if (column)
        return column->lastToken();
    if (column_token)
        return column_token + 1;
    return modify_token + 1;
}

unsigned CommentOnTableStatementAST::firstToken() const
{
    return comment_token;
}

unsigned CommentOnTableStatementAST::lastToken() const
{
    if ( comment )
        return comment->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( table_token )
        return table_token + 1;
    if ( on_token )
        return on_token + 1;
    return comment_token + 1;
}

unsigned CommentOnColumnStatementAST::firstToken() const
{
    return comment_token;
}

unsigned CommentOnColumnStatementAST::lastToken() const
{
    if ( comment )
        return comment->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( column )
        return column->lastToken();
    if ( dot_token )
        return dot_token + 1;
    if ( schemaTableName )
        return schemaTableName->lastToken();
    if ( column_token )
        return column_token + 1;
    if ( on_token )
        return on_token + 1;
    return comment_token + 1;
}

unsigned CommitStatementAST::firstToken() const
{
    return commit_token;
}

unsigned CommitStatementAST::lastToken() const
{
    return commit_token + 1;
}

unsigned RollbackStatementAST::firstToken() const
{
    return rollback_token;
}

unsigned RollbackStatementAST::lastToken() const
{
    return rollback_token + 1;
}

unsigned CallStatementAST::firstToken() const
{
    return call_token;
}

unsigned CallStatementAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( arg_list )
        return arg_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( funcName )
        return funcName->lastToken();
    return call_token + 1;
}

unsigned CharacterTypeAST::firstToken() const
{
    return type_token;
}

unsigned CharacterTypeAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( size_token )
        return size_token + 1;
    if ( lparen_token )
        return lparen_token + 1;
    return type_token + 1;
}

unsigned NumberTypeAST::firstToken() const
{
    return type_token;
}

unsigned NumberTypeAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( prec_token )
        return prec_token + 1;
    if ( comma_token )
        return comma_token + 1;
    if ( size_token )
        return size_token + 1;
    if ( lparen_token )
        return lparen_token + 1;
    return type_token + 1;
}

unsigned CaseExpressionClauseAST::firstToken() const
{
    return when_token;
}

unsigned CaseExpressionClauseAST::lastToken() const
{
    if ( thenExpr )
        return thenExpr->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( whenExpr )
        return whenExpr->lastToken();
    return when_token + 1;
}

unsigned CaseConditionClauseAST::firstToken() const
{
    return when_token;
}

unsigned CaseConditionClauseAST::lastToken() const
{
    if ( thenExpr )
        return thenExpr->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( whenExpr )
        return whenExpr->lastToken();
    return when_token + 1;
}

void SqlCheckNamesWalker::walk( BaseWalkReflector *reflector) {
    if ( m_ast ) {
        SqlCheckNamesVisitor visitor(m_translationUnit, reflector);
        m_ast->accept(&visitor);
    }
}
