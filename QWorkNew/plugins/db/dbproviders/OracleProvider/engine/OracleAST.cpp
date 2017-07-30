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

#include "OracleAST.h"
#include "OracleASTVisitor.h"
#include "OracleSqlCheckNamesVisitor.h"
#include "MemoryPool.h"

#include <cassert>
#include <algorithm>

/*
   All firstToken/lastToken methods below which have a doxygen comment with
   \generated in it, will be re-generated when the tool "generate-ast" is run.

   For methods which are hand-coded, or which should not be changed, make sure that
   the comment is gone.
 */


using namespace Oracle;

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

//bool AST::match(AST *ast, AST *pattern, ASTMatcher *matcher)
//{
//    if (ast == pattern)
//        return true;

//    else if (! ast || ! pattern)
//        return false;

//    return ast->match(pattern, matcher);
//}

//bool AST::match(AST *pattern, ASTMatcher *matcher)
//{
//    return match0(pattern, matcher);
//}

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
    return from_token;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
// PL-SQL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
unsigned PlSqlBlockAST::firstToken() const
{
    if ( label )
        return label->firstToken();
    if ( declare_token )
        return declare_token;
    if ( declareSection )
        return declareSection->firstToken();
    if ( body )
        return body->firstToken();
    return 0;
}

unsigned PlSqlBlockAST::lastToken() const
{
    if ( body )
        return body->lastToken();
    if ( declareSection )
        return declareSection->lastToken();
    if ( declare_token )
        return declare_token + 1;
    if ( label )
        return label->lastToken();
    return 1;
}

unsigned PlSqlBlockBodyAST::firstToken() const
{
    return begin_token;
}

unsigned PlSqlBlockBodyAST::lastToken() const
{
    if ( endBlockName )
        return endBlockName->lastToken();
    if ( end_token )
        return end_token + 1;
    if ( othersException )
        return othersException->lastToken();
    if ( namedExceptions )
        return namedExceptions->lastToken();
    if ( exception_token )
        return exception_token + 1;
    if ( statements )
        return statements->lastToken();
    return begin_token + 1;
}

unsigned PlSqlStatementAST::firstToken() const
{
    if ( label )
        return label->firstToken();
    if ( statement )
        return statement->firstToken();
    return 0;
}

unsigned PlSqlStatementAST::lastToken() const
{
    if ( statement )
        return statement->lastToken();
    if ( label )
        return label->lastToken();
    return 1;
}

unsigned BlockDeleteStatementAST::firstToken() const
{
    if ( deleteStatement )
        return deleteStatement->firstToken();
    return 0;
}

unsigned BlockDeleteStatementAST::lastToken() const
{
    if ( deleteStatement )
        return deleteStatement->lastToken();
    return 1;
}

unsigned BlockInsertStatementAST::firstToken() const
{
    if ( insertStatement )
        return insertStatement->firstToken();
    return 0;
}

unsigned BlockInsertStatementAST::lastToken() const
{
    if ( insertStatement )
        return insertStatement->lastToken();
    return 1;
}

unsigned BlockLockTableStatementAST::firstToken() const
{
    return lock_token;
}

unsigned BlockLockTableStatementAST::lastToken() const
{
    if ( tableName )
        return tableName->lastToken();
    if ( table_token )
        return table_token + 1;
    return lock_token + 1;
}

unsigned BlockRollbackStatementAST::firstToken() const
{
    return rollback_token;
}

unsigned BlockRollbackStatementAST::lastToken() const
{
    if ( rollbacClause )
        return rollbacClause->lastToken();
    if ( work_token )
        return work_token + 1;
    return rollback_token + 1;
}

unsigned RollbackToSavepointClauseAST::firstToken() const
{
    return to_token;
}

unsigned RollbackToSavepointClauseAST::lastToken() const
{
    if ( savepointName )
        return savepointName->lastToken();
    if ( savepoint_token )
        return savepoint_token + 1;
    return to_token + 1;
}

unsigned RollbackForceClauseAST::firstToken() const
{
    return force_token;
}

unsigned RollbackForceClauseAST::lastToken() const
{
    if ( stringExpr )
        return stringExpr->lastToken();
    return force_token + 1;
}

unsigned BlockSavepointStatementAST::firstToken() const
{
    return savepoint_token;
}

unsigned BlockSavepointStatementAST::lastToken() const
{
    if ( savepointName )
        return savepointName->lastToken();
    return savepoint_token + 1;
}

unsigned BlockSelectIntoStatementAST::firstToken() const
{
    return select_token;
}

unsigned BlockSelectIntoStatementAST::lastToken() const
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
    if ( intoVars )
        return intoVars->lastToken();
    if ( into_token )
        return into_token + 1;
    if ( collect_token )
        return collect_token + 1;
    if ( bulk_token )
        return bulk_token + 1;
    if ( columnList )
        return columnList->lastToken();
    if (select_spec_token)
        return select_spec_token + 1;
    return select_token + 1;
}

unsigned BlockSetTransactionStatementAST::firstToken() const
{
    return set_token;
}

unsigned BlockSetTransactionStatementAST::lastToken() const
{
    if ( transaction_token )
        return transaction_token + 1;
    return set_token + 1;
}

unsigned BlockUpdateStatementAST::firstToken() const
{
    if ( updateStatement )
        return updateStatement->firstToken();
    return 0;
}

unsigned BlockUpdateStatementAST::lastToken() const
{
    if ( updateStatement )
        return updateStatement->lastToken();
    return 1;
}

unsigned AssignmentStatementAST::firstToken() const
{
    if ( lvalueExpr )
        return lvalueExpr->firstToken();
    if ( assign_token )
        return assign_token;
    if ( rvalueExpr )
        return rvalueExpr->firstToken();
    return 0;
}

unsigned AssignmentStatementAST::lastToken() const
{
    if ( rvalueExpr )
        return rvalueExpr->lastToken();
    if ( assign_token )
        return assign_token + 1;
    if ( lvalueExpr )
        return lvalueExpr->lastToken();
    return 1;
}

unsigned LValueExpressionAST::firstToken() const
{
    if ( dotted_list )
        return dotted_list->firstToken();
    return 0;
}

unsigned LValueExpressionAST::lastToken() const
{
    if ( dotted_list )
        return dotted_list->lastToken();
    return 1;
}

unsigned LValueExpressionItemAST::firstToken() const
{
    if ( identName )
        return identName->firstToken();
    if ( arr_ind_list )
        return arr_ind_list->firstToken();
    return 0;
}

unsigned LValueExpressionItemAST::lastToken() const
{
    if ( arr_ind_list )
        return arr_ind_list->lastToken();
    if ( identName )
        return identName->lastToken();
    return 1;
}

unsigned CloseStatementAST::firstToken() const
{
    return close_token;
}

unsigned CloseStatementAST::lastToken() const
{
    if ( cursorName )
        return cursorName->lastToken();
    return close_token + 1;
}

unsigned ContinueOrExitStatementAST::firstToken() const
{
    return continue_or_exit_token;
}

unsigned ContinueOrExitStatementAST::lastToken() const
{
    if ( condExpr )
        return condExpr->lastToken();
    if ( when_token )
        return when_token + 1;
    if ( labelName )
        return labelName->lastToken();
    return continue_or_exit_token + 1;
}

unsigned ExecImmediateStatementAST::firstToken() const
{
    return exec_token;
}

unsigned ExecImmediateStatementAST::lastToken() const
{
    if ( returningClause )
        return returningClause->lastToken();
    if ( usingClause )
        return usingClause->lastToken();
    if ( intoClause )
        return intoClause->lastToken();
    if ( stringSqlExpr )
        return stringSqlExpr->lastToken();
    if ( immediate_token )
        return immediate_token + 1;
    return exec_token + 1;
}

unsigned FetchStatementAST::firstToken() const
{
    return fetch_token;
}

unsigned FetchStatementAST::lastToken() const
{
    if ( limit )
        return limit->lastToken();
    if ( limit_token )
        return limit_token + 1;
    if ( intoClause )
        return intoClause->lastToken();
    if ( cursorName )
        return cursorName->lastToken();
    return fetch_token + 1;
}

unsigned ForLoopStatementAST::firstToken() const
{
    return for_token;
}

unsigned ForLoopStatementAST::lastToken() const
{
    if ( loop )
        return loop->lastToken();
    if ( inClause )
        return inClause->lastToken();
    if ( in_token )
        return in_token + 1;
    if ( indexName )
        return indexName->lastToken();
    return for_token + 1;
}

unsigned ForInBoundsClauseAST::firstToken() const
{
    if ( reverse_token )
        return reverse_token;
    if ( lboundExpr )
        return lboundExpr->firstToken();
    if ( dotdot_token )
        return dotdot_token;
    if ( rboundExpr )
        return rboundExpr->firstToken();
    return 0;
}

unsigned ForInBoundsClauseAST::lastToken() const
{
    if ( rboundExpr )
        return rboundExpr->lastToken();
    if ( dotdot_token )
        return dotdot_token + 1;
    if ( lboundExpr )
        return lboundExpr->lastToken();
    if ( reverse_token )
        return reverse_token + 1;
    return 1;
}

unsigned ForInCursorClauseAST::firstToken() const
{
    if ( cursorName )
        return cursorName->firstToken();
    if ( lparen_token )
        return lparen_token;
    if ( par_list )
        return par_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

unsigned ForInCursorClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( par_list )
        return par_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( cursorName )
        return cursorName->lastToken();
    return 1;
}

unsigned ForInSelectClauseAST::firstToken() const
{
    return lparen_token;
}

unsigned ForInSelectClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( selectStatement )
        return selectStatement->lastToken();
    return lparen_token + 1;
}

unsigned ForallStatementAST::firstToken() const
{
    return forall_token;
}

unsigned ForallStatementAST::lastToken() const
{
    if ( exceptions_token )
        return exceptions_token + 1;
    if ( save_token )
        return save_token + 1;
    if ( statement )
        return statement->lastToken();
    if ( inClause )
        return inClause->lastToken();
    if ( in_token )
        return in_token + 1;
    if ( indexName )
        return indexName->lastToken();
    return forall_token + 1;
}

unsigned ForallInBoundsClauseAST::firstToken() const
{
    if ( lboundExpr )
        return lboundExpr->firstToken();
    if ( dotdot_token )
        return dotdot_token;
    if ( rboundExpr )
        return rboundExpr->firstToken();
    return 0;
}

unsigned ForallInBoundsClauseAST::lastToken() const
{
    if ( rboundExpr )
        return rboundExpr->lastToken();
    if ( dotdot_token )
        return dotdot_token + 1;
    if ( lboundExpr )
        return lboundExpr->lastToken();
    return 1;
}

unsigned ForallInIndicesClauseAST::firstToken() const
{
    return indices_token;
}

unsigned ForallInIndicesClauseAST::lastToken() const
{
    if ( rboundExpr )
        return rboundExpr->lastToken();
    if ( and_token )
        return and_token + 1;
    if ( lboundExpr )
        return lboundExpr->lastToken();
    if ( between_token )
        return between_token + 1;
    if ( collectionName )
        return collectionName->lastToken();
    if ( of_token )
        return of_token + 1;
    return indices_token + 1;
}

unsigned ForallInValuesClauseAST::firstToken() const
{
    return values_token;
}

unsigned ForallInValuesClauseAST::lastToken() const
{
    if ( collectionName )
        return collectionName->lastToken();
    if ( of_token )
        return of_token + 1;
    return values_token + 1;
}

unsigned WhileLoopStatementAST::firstToken() const
{
    return while_token;
}

unsigned WhileLoopStatementAST::lastToken() const
{
    if ( loop )
        return loop->lastToken();
    if ( condExpr )
        return condExpr->lastToken();
    return while_token + 1;
}

unsigned GotoStatementAST::firstToken() const
{
    return goto_token;
}

unsigned GotoStatementAST::lastToken() const
{
    if ( labelName )
        return labelName->lastToken();
    return goto_token + 1;
}

unsigned IfStatementAST::firstToken() const
{
    return if_token;
}

unsigned IfStatementAST::lastToken() const
{
    if ( end_if_token )
        return end_if_token + 1;
    if ( end_token )
        return end_token + 1;
    if ( falseStatements )
        return falseStatements->lastToken();
    if ( else_token )
        return else_token + 1;
    if ( elsif_list )
        return elsif_list->lastToken();
    if ( trueStatements )
        return trueStatements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( condExpr )
        return condExpr->lastToken();
    return if_token + 1;
}

unsigned ElsifClauseAST::firstToken() const
{
    return elsif_token;
}

unsigned ElsifClauseAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( condExpr )
        return condExpr->lastToken();
    return elsif_token + 1;
}

unsigned LoopStatementAST::firstToken() const
{
    return loop_token;
}

unsigned LoopStatementAST::lastToken() const
{
    if ( labelName )
        return labelName->lastToken();
    if ( end_loop_token )
        return end_loop_token + 1;
    if ( end_token )
        return end_token + 1;
    if ( statements )
        return statements->lastToken();
    return loop_token + 1;
}

unsigned OpenCursorStatementAST::firstToken() const
{
    return open_token;
}

unsigned OpenCursorStatementAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( par_list )
        return par_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( cursorName )
        return cursorName->lastToken();
    return open_token + 1;
}

unsigned OpenForStatementAST::firstToken() const
{
    return open_token;
}

unsigned OpenForStatementAST::lastToken() const
{
    if ( usingClause )
        return usingClause->lastToken();
    if ( selStatement )
        return selStatement->lastToken();
    if ( for_token )
        return for_token + 1;
    if ( cursorVariableName )
        return cursorVariableName->lastToken();
    return open_token + 1;
}

unsigned OpenForStatementClauseBySelectAST::firstToken() const
{
    if ( selectStatement )
        return selectStatement->firstToken();
    return 0;
}

unsigned OpenForStatementClauseBySelectAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    return 1;
}

unsigned OpenForStatementClauseByStringAST::firstToken() const
{
    if ( stmtExpr )
        return stmtExpr->firstToken();
    return 0;
}

unsigned OpenForStatementClauseByStringAST::lastToken() const
{
    if ( stmtExpr )
        return stmtExpr->lastToken();
    return 1;
}

unsigned PlSqlBlocStatementAST::firstToken() const
{
    if ( innerBlock )
        return innerBlock->firstToken();
    return 0;
}

unsigned PlSqlBlocStatementAST::lastToken() const
{
    if ( innerBlock )
        return innerBlock->lastToken();
    return 1;
}

unsigned RaiseStatementAST::firstToken() const
{
    return raise_token;
}

unsigned RaiseStatementAST::lastToken() const
{
    if ( exceptionName )
        return exceptionName->lastToken();
    return raise_token + 1;
}

unsigned ReturnStatementAST::firstToken() const
{
    return return_token;
}

unsigned ReturnStatementAST::lastToken() const
{
    if ( returnExpr )
        return returnExpr->lastToken();
    return return_token + 1;
}

unsigned DeclareSectionAST::firstToken() const
{
    if ( decl_list )
        return decl_list->firstToken();
    return 0;
}

unsigned DeclareSectionAST::lastToken() const
{
    if ( decl_list )
        return decl_list->lastToken();
    return 1;
}

unsigned CursorDeclarationAST::firstToken() const
{
    if ( spec )
        return spec->firstToken();
    if ( is_token )
        return is_token;
    if ( selectStatement )
        return selectStatement->firstToken();
    return 0;
}

unsigned CursorDeclarationAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( spec )
        return spec->lastToken();
    return 1;
}

unsigned VariableDeclarationAST::firstToken() const
{
    if ( variableName )
        return variableName->firstToken();
    if ( constant_token )
        return constant_token;
    if ( type )
        return type->firstToken();
    if ( not_token )
        return not_token;
    if ( null_token )
        return null_token;
    if ( assign_token )
        return assign_token;
    if ( defaultExpr )
        return defaultExpr->firstToken();
    return 0;
}

unsigned VariableDeclarationAST::lastToken() const
{
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( assign_token )
        return assign_token + 1;
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( type )
        return type->lastToken();
    if ( constant_token )
        return constant_token + 1;
    if ( variableName )
        return variableName->lastToken();
    return 1;
}

unsigned ExceptionDeclarationAST::firstToken() const
{
    if ( exceptionName )
        return exceptionName->firstToken();
    if ( exception_token )
        return exception_token;
    return 0;
}

unsigned ExceptionDeclarationAST::lastToken() const
{
    if ( exception_token )
        return exception_token + 1;
    if ( exceptionName )
        return exceptionName->lastToken();
    return 1;
}

unsigned FunctionDeclarationAST::firstToken() const
{
    if ( heading )
        return heading->firstToken();
    if ( spec )
        return spec->firstToken();
    return 0;
}

unsigned FunctionDeclarationAST::lastToken() const
{
    if ( spec )
        return spec->lastToken();
    if ( heading )
        return heading->lastToken();
    return 1;
}

unsigned FunctionDefintionAST::firstToken() const
{
    if ( heading )
        return heading->firstToken();
    if ( spec )
        return spec->firstToken();
    if ( is_or_as_token )
        return is_or_as_token;
    if ( declareSection )
        return declareSection->firstToken();
    if ( body )
        return body->firstToken();
    return 0;
}

unsigned FunctionDefintionAST::lastToken() const
{
    if ( body )
        return body->lastToken();
    if ( declareSection )
        return declareSection->lastToken();
    if ( is_or_as_token )
        return is_or_as_token + 1;
    if ( spec )
        return spec->lastToken();
    if ( heading )
        return heading->lastToken();
    return 1;
}

unsigned ProcedureDefinitionAST::firstToken() const
{
    if ( heading )
        return heading->firstToken();
    if ( is_or_as_token )
        return is_or_as_token;
    if ( declareSection )
        return declareSection->firstToken();
    if ( body )
        return body->firstToken();
    return 0;
}

unsigned ProcedureDefinitionAST::lastToken() const
{
    if ( body )
        return body->lastToken();
    if ( declareSection )
        return declareSection->lastToken();
    if ( is_or_as_token )
        return is_or_as_token + 1;
    if ( heading )
        return heading->lastToken();
    return 1;
}

unsigned ProcedureDeclarationAST::firstToken() const
{
    if ( heading )
        return heading->firstToken();
    return 0;
}

unsigned ProcedureDeclarationAST::lastToken() const
{
    if ( heading )
        return heading->lastToken();
    return 1;
}

unsigned SimpleDeclarationAST::firstToken() const
{
    if ( name )
        return name->firstToken();
    if ( type )
        return type->firstToken();
    return 0;
}

unsigned SimpleDeclarationAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    if ( name )
        return name->lastToken();
    return 1;
}

unsigned TypeDefinitionDeclarationAST::firstToken() const
{
    if ( def )
        return def->firstToken();
    return 0;
}

unsigned TypeDefinitionDeclarationAST::lastToken() const
{
    if ( def )
        return def->lastToken();
    return 1;
}

unsigned RecordTypeDefinitionAST::firstToken() const
{
    return type_token;
}

unsigned RecordTypeDefinitionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( field_list )
        return field_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( record_token )
        return record_token + 1;
    if ( is_token )
        return is_token + 1;
    if ( typeName )
        return typeName->lastToken();
    return type_token + 1;
}

unsigned RefCursorTypeDefinitionAST::firstToken() const
{
    return type_token;
}

unsigned RefCursorTypeDefinitionAST::lastToken() const
{
    if ( returnType )
        return returnType->lastToken();
    if ( return_token )
        return return_token + 1;
    if ( cursor_token )
        return cursor_token + 1;
    if ( ref_token )
        return ref_token + 1;
    if ( is_token )
        return is_token + 1;
    if ( typeName )
        return typeName->lastToken();
    return type_token + 1;
}

unsigned SubtypeDefinitionAST::firstToken() const
{
    return subtype_token;
}

unsigned SubtypeDefinitionAST::lastToken() const
{
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( rparen_token )
        return rparen_token + 1;
    if ( constraintList )
        return constraintList->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( typeName )
        return typeName->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( subtypeName )
        return subtypeName->lastToken();
    return subtype_token + 1;
}

unsigned CollectionTypeDefinitionAST::firstToken() const
{
    return type_token;
}

unsigned CollectionTypeDefinitionAST::lastToken() const
{
    if ( collectionType )
        return collectionType->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( typeName )
        return typeName->lastToken();
    return type_token + 1;
}

unsigned TableCollectionTypeAST::firstToken() const
{
    return table_token;
}

unsigned TableCollectionTypeAST::lastToken() const
{
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( elementType )
        return elementType->lastToken();
    if ( of_token )
        return of_token + 1;
    return table_token + 1;
}

unsigned ArrayCollectionTypeAST::firstToken() const
{
    return arr0_token;
}

unsigned ArrayCollectionTypeAST::lastToken() const
{
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( elementType )
        return elementType->lastToken();
    if ( of_token )
        return of_token + 1;
    if ( rparen_token )
        return rparen_token + 1;
    if ( size )
        return size->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( arr1_token )
        return arr1_token + 1;
    return arr0_token + 1;
}

unsigned CursorSpecAST::firstToken() const
{
    return cursor_token;
}

unsigned CursorSpecAST::lastToken() const
{
    if ( rowType )
        return rowType->lastToken();
    if ( return_token )
        return return_token + 1;
    if ( rparen_token )
        return rparen_token + 1;
    if ( params_list )
        return params_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( cursorName )
        return cursorName->lastToken();
    return cursor_token + 1;
}

unsigned RowTypeAST::firstToken() const
{
    return typeName->firstToken();
}

unsigned RowTypeAST::lastToken() const
{
    if ( row_or_rowtype_token )
        return row_or_rowtype_token + 1;
    if ( percent_token )
        return percent_token + 1;
    return typeName->lastToken();
}

unsigned DataTypeByTypeNameAST::firstToken() const
{
    if ( ref_token )
        return ref_token;
    if ( typeName )
        return typeName->firstToken();
    return 0;
}

unsigned DataTypeByTypeNameAST::lastToken() const
{
    if ( typeName )
        return typeName->lastToken();
    if ( ref_token )
        return ref_token + 1;
    return 1;
}

unsigned DataTypeByNameAST::firstToken() const
{
    if ( variableName )
        return variableName->firstToken();
    if ( percent_token )
        return percent_token;
    if ( type_token )
        return type_token;
    return 0;
}

unsigned DataTypeByNameAST::lastToken() const
{
    if ( type_token )
        return type_token + 1;
    if ( percent_token )
        return percent_token + 1;
    if ( variableName )
        return variableName->lastToken();
    return 1;
}

unsigned DataTypeByTableRowAST::firstToken() const
{
    if ( tableName )
        return tableName->firstToken();
    if ( percent_token )
        return percent_token;
    if ( rowtype_token )
        return rowtype_token;
    return 0;
}

unsigned DataTypeByTableRowAST::lastToken() const
{
    if ( rowtype_token )
        return rowtype_token + 1;
    if ( percent_token )
        return percent_token + 1;
    if ( tableName )
        return tableName->lastToken();
    return 1;
}

unsigned DataTypeByTableColumnAST::firstToken() const
{
    if ( tableName )
        return tableName->firstToken();
    if ( dot_token )
        return dot_token;
    if ( columnName )
        return columnName->firstToken();
    if ( percent_token )
        return percent_token;
    if ( type_token )
        return type_token;
    return 0;
}

unsigned DataTypeByTableColumnAST::lastToken() const
{
    if ( type_token )
        return type_token + 1;
    if ( percent_token )
        return percent_token + 1;
    if ( columnName )
        return columnName->lastToken();
    if ( dot_token )
        return dot_token + 1;
    if ( tableName )
        return tableName->lastToken();
    return 1;
}

unsigned ScalarDataTypeAST::firstToken() const
{
    if ( type )
        return type->firstToken();
    return 0;
}

unsigned ScalarDataTypeAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    return 1;
}

unsigned CursorParameterDeclarationAST::firstToken() const
{
    if ( paramName )
        return paramName->firstToken();
    if ( in_token )
        return in_token;
    if ( type )
        return type->firstToken();
    if ( assign_token )
        return assign_token;
    if ( defaultExpr )
        return defaultExpr->firstToken();
    return 0;
}

unsigned CursorParameterDeclarationAST::lastToken() const
{
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( assign_token )
        return assign_token + 1;
    if ( type )
        return type->lastToken();
    if ( in_token )
        return in_token + 1;
    if ( paramName )
        return paramName->lastToken();
    return 1;
}

unsigned RecordFieldDeclarationAST::firstToken() const
{
    if ( fieldName )
        return fieldName->firstToken();
    if ( type )
        return type->firstToken();
    if ( not_token )
        return not_token;
    if ( null_token )
        return null_token;
    if ( assign_token )
        return assign_token;
    if ( defaultExpr )
        return defaultExpr->firstToken();
    return 0;
}

unsigned RecordFieldDeclarationAST::lastToken() const
{
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( assign_token )
        return assign_token + 1;
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( type )
        return type->lastToken();
    if ( fieldName )
        return fieldName->lastToken();
    return 1;
}

unsigned RoutineArgumentDeclarationAST::firstToken() const
{
    if ( argName )
        return argName->firstToken();
    if ( in_token )
        return in_token;
    if ( out_token )
        return out_token;
    if ( nocopy_token )
        return nocopy_token;
    if ( type )
        return type->firstToken();
    if ( assign_token )
        return assign_token;
    if ( defaultExpr )
        return defaultExpr->firstToken();
    return 0;
}

unsigned RoutineArgumentDeclarationAST::lastToken() const
{
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( assign_token )
        return assign_token + 1;
    if ( type )
        return type->lastToken();
    if ( nocopy_token )
        return nocopy_token + 1;
    if ( out_token )
        return out_token + 1;
    if ( in_token )
        return in_token + 1;
    if ( argName )
        return argName->lastToken();
    return 1;
}

unsigned FunctionHeadingAST::firstToken() const
{
    return function_token;
}

unsigned FunctionHeadingAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    if ( return_token )
        return return_token + 1;
    if ( rparen_token )
        return rparen_token + 1;
    if ( arg_list )
        return arg_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( functionName )
        return functionName->lastToken();
    return function_token + 1;
}

unsigned FunctionCasheSpecAST::firstToken() const
{
    return result_cashe_token;
}

unsigned FunctionCasheSpecAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( source_list )
        return source_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( relies_on_token )
        return relies_on_token + 1;
    return result_cashe_token + 1;
}

unsigned ProcedureHeadingAST::firstToken() const
{
    return procedure_token;
}

unsigned ProcedureHeadingAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( arg_list )
        return arg_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( procedureName )
        return procedureName->lastToken();
    return procedure_token + 1;
}

unsigned NamedExceptionHandlerAST::firstToken() const
{
    return when_token;
}

unsigned NamedExceptionHandlerAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( name_list )
        return name_list->lastToken();
    return when_token + 1;
}

unsigned OthersExceptionHandlerAST::firstToken() const
{
    return when_token;
}

unsigned OthersExceptionHandlerAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( others_token )
        return others_token + 1;
    return when_token + 1;
}

unsigned IntoClauseAST::firstToken() const
{
    return into_token;
}

unsigned IntoClauseAST::lastToken() const
{
    if ( names )
        return names->lastToken();
    return into_token + 1;
}

unsigned BulkIntoClauseAST::firstToken() const
{
    return bulk_token;
}

unsigned BulkIntoClauseAST::lastToken() const
{
    if ( names )
        return names->lastToken();
    if ( into_token )
        return into_token + 1;
    if ( collect_token )
        return collect_token + 1;
    return bulk_token + 1;
}

unsigned UsingClauseAST::firstToken() const
{
    return using_token;
}

unsigned UsingClauseAST::lastToken() const
{
    if ( bind_list )
        return bind_list->lastToken();
    return using_token + 1;
}

unsigned UsingBindAST::firstToken() const
{
    if ( in_token )
        return in_token;
    if ( out_token )
        return out_token;
    if ( bindExpr )
        return bindExpr->firstToken();
    return 0;
}

unsigned UsingBindAST::lastToken() const
{
    if ( bindExpr )
        return bindExpr->lastToken();
    if ( out_token )
        return out_token + 1;
    if ( in_token )
        return in_token + 1;
    return 1;
}

unsigned DynamicReturningClauseAST::firstToken() const
{
    return return_token;
}

unsigned ArrayIndexExpressionAST::firstToken() const
{
    return lparen_token;
}

unsigned ArrayIndexExpressionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( indexExpr )
        return indexExpr->lastToken();
    return lparen_token + 1;
}

unsigned DynamicReturningClauseAST::lastToken() const
{
    if ( intoClause )
        return intoClause->lastToken();
    return return_token + 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SqlCheckNamesWalker::walk( Sql::BaseWalkReflector *reflector) {
    if ( m_ast ) {
        SqlCheckNamesVisitor visitor(m_translationUnit, reflector);
        m_ast->accept(&visitor);
    }
}
