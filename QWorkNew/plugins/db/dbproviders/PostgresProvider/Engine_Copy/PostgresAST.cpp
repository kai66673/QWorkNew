#include <assert.h>

#include <QDebug>

#include "PostgresAST.h"
#include "PostgresASTVisitor.h"
//#include "PostgresSqlCheckNamesVisitor.h"

namespace Postgres {

AST::AST()
{ }

AST::~AST()
{ assert(0); }

void AST::accept( ASTVisitor *visitor )
{
    if ( visitor->preVisit(this) )
        accept0(visitor);
    visitor->postVisit(this);
}

void EmptyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TranslationUnitAST::firstToken() const
{
    if ( statements )
        return statements->firstToken();
    return 0;
}

int TranslationUnitAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    return 1;
}

void TranslationUnitAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(statements, visitor);
    }
    visitor->endVisit(this);
}

int UnknownStatementAST::firstToken() const
{
    return first_token;
}

int UnknownStatementAST::lastToken() const
{
    return last_token;
}

void UnknownStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SelectStatementAST::firstToken() const
{
    if ( selectCore )
        return selectCore->firstToken();
    if ( orderByBlock )
        return orderByBlock->firstToken();
    if ( forupdateClause )
        return forupdateClause->firstToken();
    return 0;
}

int SelectStatementAST::lastToken() const
{
    if ( forupdateClause )
        return forupdateClause->lastToken();
    if ( orderByBlock )
        return orderByBlock->lastToken();
    if ( selectCore )
        return selectCore->lastToken();
    return 1;
}

void SelectStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(selectCore, visitor);
        accept(orderByBlock, visitor);
        accept(forupdateClause, visitor);
    }
    visitor->endVisit(this);
}

int DeleteStatementAST::firstToken() const
{
    return delete_token;
}

int DeleteStatementAST::lastToken() const
{
    if ( whereClause )
        return whereClause->lastToken();
    if ( source )
        return source->lastToken();
    if ( from_token )
        return from_token + 1;
    return delete_token + 1;
}

void DeleteStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(source, visitor);
        accept(whereClause, visitor);
    }
    visitor->endVisit(this);
}

int UpdateSetClauseAST::firstToken() const
{
    if ( set_token )
        return set_token;
    if ( upd_list )
        return upd_list->firstToken();
    return 0;
}

int UpdateSetClauseAST::lastToken() const
{
    if ( upd_list )
        return upd_list->lastToken();
    if ( set_token )
        return set_token + 1;
    return 1;
}

void UpdateSetClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(upd_list, visitor);
    }
    visitor->endVisit(this);
}

int AssignmentExpressionAST::firstToken() const
{
    if ( columnIdent )
        return columnIdent->firstToken();
    if ( equal_token )
        return equal_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    return 0;
}

int AssignmentExpressionAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( equal_token )
        return equal_token + 1;
    if ( columnIdent )
        return columnIdent->lastToken();
    return 1;
}

void AssignmentExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnIdent, visitor);
        accept(rvExpr, visitor);
    }
    visitor->endVisit(this);
}

int UpdateStatementAST::firstToken() const
{
    return update_token;
}

int UpdateStatementAST::lastToken() const
{
    if ( whereClause )
        return whereClause->lastToken();
    if ( setClause )
        return setClause->lastToken();
    if ( source )
        return source->lastToken();
    return update_token + 1;
}

void UpdateStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(source, visitor);
        accept(setClause, visitor);
        accept(whereClause, visitor);
    }
    visitor->endVisit(this);
}

int InsertStatementAST::firstToken() const
{
    return insert_token;
}

int InsertStatementAST::lastToken() const
{
    if ( insClause )
        return insClause->lastToken();
    if ( insertColumns )
        return insertColumns->lastToken();
    if ( source )
        return source->lastToken();
    if ( into_token )
        return into_token + 1;
    return insert_token + 1;
}

void InsertStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(source, visitor);
        accept(insertColumns, visitor);
        accept(insClause, visitor);
    }
    visitor->endVisit(this);
}

int InsertColumnsClauseAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( col_list )
        return col_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int InsertColumnsClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( col_list )
        return col_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 1;
}

void InsertColumnsClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(col_list, visitor);
    }
    visitor->endVisit(this);
}

int InsertClauseAsValuesAST::firstToken() const
{
    return values_token;
}

int InsertClauseAsValuesAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( val_list )
        return val_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return values_token;
}

void InsertClauseAsValuesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(val_list, visitor);
    }
    visitor->endVisit(this);
}

int InsertClauseAsSelectAST::firstToken() const
{
    if ( selectStatement )
        return selectStatement->firstToken();
    return 0;
}

int InsertClauseAsSelectAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    return 1;
}

void InsertClauseAsSelectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(selectStatement, visitor);
    }
    visitor->endVisit(this);
}

int SelectCoreAST::firstToken() const
{
    return select_token;
}

int SelectCoreAST::lastToken() const
{
    if ( setClause )
        return setClause->lastToken();
    if ( groupBy )
        return groupBy->lastToken();
    if ( whereClause )
        return whereClause->lastToken();
    if ( tables )
        return tables->lastToken();
    if ( from_token )
        return from_token + 1;
    if ( select_spec_token )
        return select_spec_token + 1;
    if ( selColumns )
        return selColumns->lastToken();
    return select_token + 1;
}

void SelectCoreAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(selColumns, visitor);
        accept(tables, visitor);
        accept(whereClause, visitor);
        accept(groupBy, visitor);
        accept(setClause, visitor);
    }
    visitor->endVisit(this);
}

int OrderByBlockAST::firstToken() const
{
    return order_token;
}

int OrderByBlockAST::lastToken() const
{
    if ( term_list )
        return term_list->lastToken();
    if ( by_token )
        return by_token + 1;
    return order_token + 1;
}

void OrderByBlockAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(term_list, visitor);
    }
    visitor->endVisit(this);
}

int OrderingTermAST::firstToken() const
{
    if ( expr )
        return expr->firstToken();
    if ( asc_or_desc_token )
        return asc_or_desc_token;
    return 0;
}

int OrderingTermAST::lastToken() const
{
    if ( asc_or_desc_token )
        return asc_or_desc_token + 1;
    if ( expr )
        return expr->lastToken();
    return 1;
}

void OrderingTermAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int ForUpdateClauseAST::firstToken() const
{
    return for_token;
}

int ForUpdateClauseAST::lastToken() const
{
    if ( nowait_token )
        return nowait_token + 1;
    if ( col_list )
        return col_list->lastToken();
    if ( of_token )
        return of_token + 1;
    if ( update_token )
        return update_token + 1;
    return for_token + 1;
}

void ForUpdateClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(col_list, visitor);
    }
    visitor->endVisit(this);
}

void StarSelectColumnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TableSelectColumnAST::firstToken() const
{
    if ( tableIdent )
        return tableIdent->firstToken();
    if ( dot_token )
        return dot_token;
    if ( star_token )
        return star_token;
    return 0;
}

int TableSelectColumnAST::lastToken() const
{
    if ( star_token )
        return star_token + 1;
    if ( dot_token )
        return dot_token + 1;
    if ( tableIdent )
        return tableIdent->lastToken();
    return 1;
}

void TableSelectColumnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int ExpressionSelectColumnAST::firstToken() const
{
    if ( columnExpr )
        return columnExpr->firstToken();
    if ( as_token )
        return as_token;
    if ( columnAlias )
        return columnAlias->firstToken();
    return 0;
}

int ExpressionSelectColumnAST::lastToken() const
{
    if ( columnAlias )
        return columnAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( columnExpr )
        return columnExpr->lastToken();
    return 1;
}

void ExpressionSelectColumnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnExpr, visitor);
        accept(columnAlias, visitor);
    }
    visitor->endVisit(this);
}

int CompoundJoinedTableAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( table )
        return table->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int CompoundJoinedTableAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( table )
        return table->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 1;
}

void CompoundJoinedTableAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(table, visitor);
    }
    visitor->endVisit(this);
}

int JoinJoinedTableAST::firstToken() const
{
    if ( ltr )
        return ltr->firstToken();
    if ( spec )
        return spec->firstToken();
    if ( rtr )
        return rtr->firstToken();
    if ( condition )
        return condition->firstToken();
    return 0;
}

int JoinJoinedTableAST::lastToken() const
{
    if ( condition )
        return condition->lastToken();
    if ( rtr )
        return rtr->lastToken();
    if ( spec )
        return spec->lastToken();
    if ( ltr )
        return ltr->lastToken();
    return 1;
}

void JoinJoinedTableAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ltr, visitor);
        accept(spec, visitor);
        accept(rtr, visitor);
        accept(condition, visitor);
    }
    visitor->endVisit(this);
}

int DirectSingleSourceAST::firstToken() const
{
    if ( schemaTableName )
        return schemaTableName->firstToken();
    if ( as_token )
        return as_token;
    if ( tableAlias )
        return tableAlias->firstToken();
    return 0;
}

int DirectSingleSourceAST::lastToken() const
{
    if ( tableAlias )
        return tableAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( schemaTableName )
        return schemaTableName->lastToken();
    return 1;
}

void DirectSingleSourceAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaTableName, visitor);
        accept(tableAlias, visitor);
    }
    visitor->endVisit(this);
}

int SubQuerySingleSourceAST::firstToken() const
{
    return lparen_token;
}

int SubQuerySingleSourceAST::lastToken() const
{
    if ( tableAlias )
        return tableAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( rparen_token )
        return rparen_token + 1;
    if ( selectStatement )
        return selectStatement->lastToken();
    return lparen_token + 1;
}

void SubQuerySingleSourceAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(selectStatement, visitor);
        accept(tableAlias, visitor);
    }
    visitor->endVisit(this);
}

int JoinSpecAST::firstToken() const
{
    if ( j1_spec_token )
        return j1_spec_token;
    if ( j2_spec_token )
        return j2_spec_token;
    if ( join_token )
        return join_token;
    return 0;
}

int JoinSpecAST::lastToken() const
{
    if ( join_token )
        return join_token + 1;
    if ( j2_spec_token )
        return j2_spec_token + 1;
    if ( j1_spec_token )
        return j1_spec_token + 1;
    return 1;
}

void JoinSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int JoinConditionAST::firstToken() const
{
    return on_token;
}

int JoinConditionAST::lastToken() const
{
    if ( conditionExpr )
        return conditionExpr->lastToken();
    return on_token + 1;
}

void JoinConditionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(conditionExpr, visitor);
    }
    visitor->endVisit(this);
}

int WhereClauseAST::firstToken() const
{
    return where_token;
}

int WhereClauseAST::lastToken() const
{
    if ( whereExpr )
        return whereExpr->lastToken();
    return where_token + 1;
}

void WhereClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(whereExpr, visitor);
    }
    visitor->endVisit(this);
}

int GroupByBlockAST::firstToken() const
{
    return group_token;
}

int GroupByBlockAST::lastToken() const
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

void GroupByBlockAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(group_items_list, visitor);
        accept(havingExpr, visitor);
    }
    visitor->endVisit(this);
}

int SetClauseAST::firstToken() const
{
    if ( spec )
        return spec->firstToken();
    if ( selectCore )
        return selectCore->firstToken();
    return 0;
}

int SetClauseAST::lastToken() const
{
    if ( selectCore )
        return selectCore->lastToken();
    if ( spec )
        return spec->lastToken();
    return 1;
}

void SetClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(spec, visitor);
        accept(selectCore, visitor);
    }
    visitor->endVisit(this);
}

void MinusSetSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void IntersectSetSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UnionSetSpecAST::firstToken() const
{
    return union_token;
}

int UnionSetSpecAST::lastToken() const
{
    if ( all_or_distinct_token )
        return all_or_distinct_token + 1;
    return union_token + 1;
}

void UnionSetSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateTableStatementAST::firstToken() const
{
    return create_token;
}

int CreateTableStatementAST::lastToken() const
{
    if ( tableClause )
        return tableClause->lastToken();
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( table_token )
        return table_token + 1;
    return create_token + 1;
}

void CreateTableStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(tableClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateViewStatementAST::firstToken() const
{
    return create_token;
}

int CreateViewStatementAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( viewIdent )
        return viewIdent->lastToken();
    return create_token + 1;
}

void CreateViewStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewIdent, visitor);
        accept(selectStatement, visitor);
    }
    visitor->endVisit(this);
}

int CreateIndexStatementAST::firstToken() const
{
    return create_token;
}

int CreateIndexStatementAST::lastToken() const
{
    if ( indexClause )
        return indexClause->lastToken();
    if ( indexIdent )
        return indexIdent->lastToken();
    if ( index_token )
        return index_token + 1;
    if ( unique_token )
        return unique_token + 1;
    return create_token + 1;
}

void CreateIndexStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexIdent, visitor);
        accept(indexClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateTriggerStatementAST::firstToken() const
{
    return create_token;
}

int CreateTriggerStatementAST::lastToken() const
{
    if ( funcCall )
        return funcCall->lastToken();
    if ( procedure_token )
        return procedure_token + 1;
    if ( execute_token )
        return execute_token + 1;
    if ( whenClause )
        return whenClause->lastToken();
    if ( forClause )
        return forClause->lastToken();
    if ( fromClause )
        return fromClause->lastToken();
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( on_token )
        return on_token + 1;
    if ( eventClause )
        return eventClause->lastToken();
    if ( whetherClause )
        return whetherClause->lastToken();
    if ( triggerIdent )
        return triggerIdent->lastToken();
    if ( trigger_token )
        return trigger_token + 1;
    if ( constraint_token )
        return constraint_token + 1;
    return create_token + 1;
}

void CreateTriggerStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerIdent, visitor);
        accept(whetherClause, visitor);
        accept(eventClause, visitor);
        accept(tableIdent, visitor);
        accept(fromClause, visitor);
        accept(forClause, visitor);
        accept(whenClause, visitor);
        accept(funcCall, visitor);
    }
    visitor->endVisit(this);
}

void TriggerWhetherBeforeOrAfterAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void TriggerWhetherInsteadOfAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TriggerEventUpdateAST::firstToken() const
{
    return update_token;
}

int TriggerEventUpdateAST::lastToken() const
{
    if ( columns )
        return columns->lastToken();
    if ( of_token )
        return of_token + 1;
    return update_token + 1;
}

void TriggerEventUpdateAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void TriggerEventIdtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TriggerFromClauseAST::firstToken() const
{
    return from_token;
}

int TriggerFromClauseAST::lastToken() const
{
    if ( refTableIdent )
        return refTableIdent->lastToken();
    return from_token + 1;
}

void TriggerFromClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(refTableIdent, visitor);
    }
    visitor->endVisit(this);
}

int TriggerNotDeferrableTimingAST::firstToken() const
{
    return not_token;
}

int TriggerNotDeferrableTimingAST::lastToken() const
{
    if ( deferrable_token )
        return deferrable_token + 1;
    return not_token + 1;
}

void TriggerNotDeferrableTimingAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TriggerDeferrableTimingAST::firstToken() const
{
    return deferrable_token;
}

int TriggerDeferrableTimingAST::lastToken() const
{
    if ( immediate_or_deferred_token )
        return immediate_or_deferred_token + 1;
    if ( initially_token )
        return initially_token + 1;
    return deferrable_token + 1;
}

void TriggerDeferrableTimingAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TriggerForClauseAST::firstToken() const
{
    return for_token;
}

int TriggerForClauseAST::lastToken() const
{
    if ( row_or_statement_token )
        return row_or_statement_token + 1;
    if ( each_token )
        return each_token + 1;
    return for_token + 1;
}

void TriggerForClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateTableAsSelectClauseAST::firstToken() const
{
    return as_token;
}

int CreateTableAsSelectClauseAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    return as_token + 1;
}

void CreateTableAsSelectClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(selectStatement, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableDirectClauseAST::firstToken() const
{
    return lparen_token;
}

int CreateTableDirectClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( column_def_list )
        return column_def_list->lastToken();
    if ( constraintList )
        constraintList->lastToken();
    return lparen_token + 1;
}

void CreateTableDirectClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(column_def_list, visitor);
        accept(constraintList, visitor);
    }
    visitor->endVisit(this);
}

int ColumnDefinitionAST::firstToken() const
{
    if ( columnName )
        return columnName->firstToken();
    return 0;
}

int ColumnDefinitionAST::lastToken() const
{
    if ( constraintList )
        return constraintList->lastToken();
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( default_token )
        return default_token + 1;
    if ( collate )
        return collate->lastToken();
    if ( columnType )
        return columnType->lastToken();
    if ( columnName )
        return columnName->lastToken();
    return 0;
}

void ColumnDefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(columnType, visitor);
        accept(collate, visitor);
        accept(defaultExpr, visitor);
        accept(constraintList, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintStateAST::firstToken() const
{
    if ( defer )
        return defer->firstToken();
    if ( relay_or_norelay_token )
        return relay_or_norelay_token;
    if ( usingIndex )
        return usingIndex->firstToken();
    if ( enable_or_disable_token )
        return enable_or_disable_token;
    if ( validate_or_novalidate_token )
        return validate_or_novalidate_token;
    if ( exceptionClause )
        return exceptionClause->firstToken();
    return 0;
}

int ConstraintStateAST::lastToken() const
{
    if ( exceptionClause )
        return exceptionClause->lastToken();
    if ( validate_or_novalidate_token )
        return validate_or_novalidate_token + 1;
    if ( enable_or_disable_token )
        return enable_or_disable_token + 1;
    if ( usingIndex )
        return usingIndex->lastToken();
    if ( relay_or_norelay_token )
        return relay_or_norelay_token + 1;
    if ( defer )
        return defer->lastToken();
    return 1;
}

void ConstraintStateAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(defer, visitor);
        accept(usingIndex, visitor);
        accept(exceptionClause, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintDeferStateAST::firstToken() const
{
    if ( token1 )
        return token1;
    if ( token2 )
        return token2;
    if ( token3 )
        return token3;
    if ( token4 )
        return token4;
    return 0;
}

int ConstraintDeferStateAST::lastToken() const
{
    if ( token4 )
        return token4;
    if ( token3 )
        return token3;
    if ( token2 )
        return token2;
    if ( token1 )
        return token1;
    return 1;
}

void ConstraintDeferStateAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UsingIndexClauseAST::firstToken() const
{
    return using_token;
}

int UsingIndexClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( createIndexStatement )
        return createIndexStatement->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( indexIdent )
        return indexIdent->lastToken();
    if ( index_token )
        return index_token + 1;
    return using_token + 1;
}

void UsingIndexClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexIdent, visitor);
        accept(createIndexStatement, visitor);
    }
    visitor->endVisit(this);
}

int ExceptionsClauseAST::firstToken() const
{
    return exceptions_token;
}

int ExceptionsClauseAST::lastToken() const
{
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( into_token )
        return into_token + 1;
    return exceptions_token + 1;
}

void ExceptionsClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int TableCreateIndexClauseAST::firstToken() const
{
    if ( on_token )
        return on_token;
    if ( tableIdent )
        return tableIdent->firstToken();
    if ( lparen_token )
        return lparen_token;
    if ( columns_list )
        return columns_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int TableCreateIndexClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( columns_list )
        return columns_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( on_token )
        return on_token + 1;
    return 1;
}

void TableCreateIndexClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(columns_list, visitor);
    }
    visitor->endVisit(this);
}

int DropTableStatementAST::firstToken() const
{
    return drop_token;
}

int DropTableStatementAST::lastToken() const
{
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( table_token )
        return table_token + 1;
    return drop_token + 1;
}

void DropTableStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int DropViewStatementAST::firstToken() const
{
    return drop_token;
}

int DropViewStatementAST::lastToken() const
{
    if ( viewIdent )
        return viewIdent->lastToken();
    if ( view_token )
        return view_token + 1;
    return drop_token + 1;
}

void DropViewStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewIdent, visitor);
    }
    visitor->endVisit(this);
}

int DropIndexStatementAST::firstToken() const
{
    return drop_token;
}

int DropIndexStatementAST::lastToken() const
{
    if ( indexIdent )
        return indexIdent->lastToken();
    if ( index_token )
        return index_token + 1;
    return drop_token + 1;
}

void DropIndexStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexIdent, visitor);
    }
    visitor->endVisit(this);
}

int DropTriggerStatementAST::firstToken() const
{
    return drop_token;
}

int DropTriggerStatementAST::lastToken() const
{
    if ( triggerIdent )
        return triggerIdent->lastToken();
    if ( trigger_token )
        return trigger_token + 1;
    return drop_token + 1;
}

void DropTriggerStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerIdent, visitor);
    }
    visitor->endVisit(this);
}

int TruncateStatementAST::firstToken() const
{
    return truncate_token;
}

int TruncateStatementAST::lastToken() const
{
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( table_token )
        return table_token + 1;
    return truncate_token + 1;
}

void TruncateStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableStatementAST::firstToken() const
{
    return alter_token;
}

int AlterTableStatementAST::lastToken() const
{
    if ( action )
        return action->lastToken();
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( only_token )
        return only_token + 1;
    if ( ifExists )
        return ifExists->lastToken();
    if ( table_token )
        return table_token + 1;
    return alter_token + 1;
}

void AlterTableStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ifExists, visitor);
        accept(tableIdent, visitor);
        accept(action, visitor);
    }
    visitor->endVisit(this);
}

int IfExistsAST::firstToken() const
{
    return if_token;
}

int IfExistsAST::lastToken() const
{
    if ( exists_token )
        return exists_token + 1;
    return if_token + 1;
}

void IfExistsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int NotValidClauseAST::firstToken() const
{
    return not_token;
}

int NotValidClauseAST::lastToken() const
{
    if ( valid_token )
        return valid_token + 1;
    return not_token + 1;
}

void NotValidClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DeferrableClauseAST::firstToken() const
{
    if ( not_token )
        return not_token;
    if ( deferrable_token )
        return deferrable_token;
    if ( initially_token )
        return initially_token;
    if ( deferred_or_immediate_token )
        return deferred_or_immediate_token;
    return 0;
}

int DeferrableClauseAST::lastToken() const
{
    if ( deferred_or_immediate_token )
        return deferred_or_immediate_token + 1;
    if ( initially_token )
        return initially_token + 1;
    if ( deferrable_token )
        return deferrable_token + 1;
    if ( not_token )
        return not_token + 1;
    return 1;
}

void DeferrableClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ConcreteTriggerAST::firstToken() const
{
    if ( triggerName )
        return triggerName->firstToken();
    return 0;
}

int ConcreteTriggerAST::lastToken() const
{
    if ( triggerName )
        return triggerName->lastToken();
    return 1;
}

void ConcreteTriggerAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
    }
    visitor->endVisit(this);
}

void AllOrUserTriggersAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableRenameActionAST::firstToken() const
{
    return rename_token;
}

int AlterTableRenameActionAST::lastToken() const
{
    if ( newTableName )
        return newTableName->lastToken();
    if ( to_token )
        return to_token + 1;
    return rename_token + 1;
}

void AlterTableRenameActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(newTableName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableRenameColumnActionAST::firstToken() const
{
    return rename_token;
}

int AlterTableRenameColumnActionAST::lastToken() const
{
    if ( newColumnName )
        return newColumnName->lastToken();
    if ( to_token )
        return to_token + 1;
    if ( columnName )
        return columnName->lastToken();
    if ( column_token )
        return column_token + 1;
    return rename_token + 1;
}

void AlterTableRenameColumnActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(newColumnName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableRenameConstraintActionAST::firstToken() const
{
    return rename_token;
}

int AlterTableRenameConstraintActionAST::lastToken() const
{
    if ( newConstraintName )
        return newConstraintName->lastToken();
    if ( to_token )
        return to_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return rename_token + 1;
}

void AlterTableRenameConstraintActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintName, visitor);
        accept(newConstraintName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableAddConstraintActionAST::firstToken() const
{
    return add_token;
}

int AlterTableAddConstraintActionAST::lastToken() const
{
    if ( notValid )
        return notValid->lastToken();
    if ( constraint )
        return constraint->lastToken();
    return add_token + 1;
}

void AlterTableAddConstraintActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraint, visitor);
        accept(notValid, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableAddColumnActionAST::firstToken() const
{
    return add_token;
}

int AlterTableAddColumnActionAST::lastToken() const
{
    if ( columnDefinition )
        return columnDefinition->lastToken();
    if ( column_token )
        return column_token + 1;
    return add_token + 1;
}

void AlterTableAddColumnActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnDefinition, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableDropColumnActionAST::firstToken() const
{
    return drop_token;
}

int AlterTableDropColumnActionAST::lastToken() const
{
    if ( restrict_or_cascade_token )
        return restrict_or_cascade_token + 1;
    if ( columnName )
        return columnName->lastToken();
    if ( ifExists )
        return ifExists->lastToken();
    if ( column_token )
        return column_token + 1;
    return drop_token + 1;
}

void AlterTableDropColumnActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ifExists, visitor);
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableDropConstraintActionAST::firstToken() const
{
    return drop_token;
}

int AlterTableDropConstraintActionAST::lastToken() const
{
    if ( restrict_or_cascade_token )
        return restrict_or_cascade_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( ifExists )
        return ifExists->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return drop_token + 1;
}

void AlterTableDropConstraintActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ifExists, visitor);
        accept(constraintName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableAbleTriggerActionAST::firstToken() const
{
    return enable_or_disable_token;
}

int AlterTableAbleTriggerActionAST::lastToken() const
{
    if ( wildcard )
        return wildcard->lastToken();
    if ( trigger_token )
        return trigger_token + 1;
    return enable_or_disable_token + 1;
}

void AlterTableAbleTriggerActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(wildcard, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableAlterColumnActionAST::firstToken() const
{
    return alter_token;
}

int AlterTableAlterColumnActionAST::lastToken() const
{
    if ( action )
        return action->lastToken();
    if ( columnName )
        return columnName->lastToken();
    if ( column_token )
        return column_token + 1;
    return alter_token + 1;
}

void AlterTableAlterColumnActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(action, visitor);
    }
    visitor->endVisit(this);
}

int AlterColumnSetTypeAST::firstToken() const
{
    if ( set_token )
        return set_token;
    if ( data_token )
        return data_token;
    if ( type_token )
        return type_token;
    if ( type )
        return type->firstToken();
    if ( collate )
        return collate->firstToken();
    if ( using_token )
        return using_token;
    if ( usingExpr )
        return usingExpr->firstToken();
    return 0;
}

int AlterColumnSetTypeAST::lastToken() const
{
    if ( usingExpr )
        return usingExpr->lastToken();
    if ( using_token )
        return using_token + 1;
    if ( collate )
        return collate->lastToken();
    if ( type )
        return type->lastToken();
    if ( type_token )
        return type_token + 1;
    if ( data_token )
        return data_token + 1;
    if ( set_token )
        return set_token + 1;
    return 1;
}

void AlterColumnSetTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(type, visitor);
        accept(collate, visitor);
        accept(usingExpr, visitor);
    }
    visitor->endVisit(this);
}

int CommentOnTableStatementAST::firstToken() const
{
    return comment_token;
}

int CommentOnTableStatementAST::lastToken() const
{
    if ( comment )
        return comment->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( table_token )
        return table_token + 1;
    if ( on_token )
        return on_token + 1;
    return comment_token + 1;
}

void CommentOnTableStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int CommentOnColumnStatementAST::firstToken() const
{
    return comment_token;
}

int CommentOnColumnStatementAST::lastToken() const
{
    if ( comment )
        return comment->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( columnIdent )
        return columnIdent->lastToken();
    if ( column_token )
        return column_token + 1;
    if ( on_token )
        return on_token + 1;
    return comment_token + 1;
}

void CommentOnColumnStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnIdent, visitor);
    }
    visitor->endVisit(this);
}

int CommitStatementAST::firstToken() const
{
    return commit_token;
}

int CommitStatementAST::lastToken() const
{
    return commit_token + 1;
}

void CommitStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RollbackStatementAST::firstToken() const
{
    return rollback_token;
}

int RollbackStatementAST::lastToken() const
{
    return rollback_token + 1;
}

void RollbackStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void NameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CompoundValueExpressionAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( valExpr )
        return valExpr->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int CompoundValueExpressionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( valExpr )
        return valExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 1;
}

void CompoundValueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(valExpr, visitor);
    }
    visitor->endVisit(this);
}

int FunctionCallAST::firstToken() const
{
    if ( functionIdent )
        return functionIdent->firstToken();
    if ( lparen_token )
        return lparen_token;
    if ( arg_list )
        return arg_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int FunctionCallAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( arg_list )
        return arg_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( functionIdent )
        return functionIdent->lastToken();
    return 1;
}

void FunctionCallAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionIdent, visitor);
        accept(arg_list, visitor);
    }
    visitor->endVisit(this);
}

int CountStarCallAST::firstToken() const
{
    return count_token;
}

int CountStarCallAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( star_token )
        return star_token + 1;
    if ( lparen_token )
        return lparen_token + 1;
    return count_token + 1;
}

void CountStarCallAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CountExprCallAST::firstToken() const
{
    return count_token;
}

int CountExprCallAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( countExpr )
        return countExpr->lastToken();
    if ( distinct_or_all_token )
        return distinct_or_all_token + 1;
    if ( lparen_token )
        return lparen_token + 1;
    return count_token + 1;
}

void CountExprCallAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(countExpr, visitor);
    }
    visitor->endVisit(this);
}

int CastFunctionAST::firstToken() const
{
    return cast_token;
}

int CastFunctionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( typeSpec )
        return typeSpec->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( castExpr )
        return castExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return cast_token + 1;
}

void CastFunctionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(castExpr, visitor);
        accept(typeSpec, visitor);
    }
    visitor->endVisit(this);
}

int SubstringFunctionAST::firstToken() const
{
    return substring_token;
}

int SubstringFunctionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( lengthExpr )
        return lengthExpr->lastToken();
    if ( for_token )
        return for_token + 1;
    if ( startExpr )
        return startExpr->lastToken();
    if ( from_token )
        return from_token + 1;
    if ( strExpr )
        return strExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return substring_token + 1;
}

void SubstringFunctionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(strExpr, visitor);
        accept(startExpr, visitor);
        accept(lengthExpr, visitor);
    }
    visitor->endVisit(this);
}

int BuiltinFunctionCallAST::firstToken() const
{
    if ( function_name_token )
        return function_name_token;
    if ( lparen_token )
        return lparen_token;
    if ( arg_list )
        return arg_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int BuiltinFunctionCallAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( arg_list )
        return arg_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( function_name_token )
        return function_name_token + 1;
    return 1;
}

void BuiltinFunctionCallAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arg_list, visitor);
    }
    visitor->endVisit(this);
}

int UnaryValueExpressionAST::firstToken() const
{
    if ( op_token )
        return op_token;
    if ( valExpr )
        return valExpr->firstToken();
    return 0;
}

int UnaryValueExpressionAST::lastToken() const
{
    if ( valExpr )
        return valExpr->lastToken();
    if ( op_token )
        return op_token + 1;
    return 1;
}

void UnaryValueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(valExpr, visitor);
    }
    visitor->endVisit(this);
}

int BinaryValueExpressionAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( op_token )
        return op_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    return 0;
}

int BinaryValueExpressionAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( op_token )
        return op_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

void BinaryValueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvExpr, visitor);
        accept(rvExpr, visitor);
    }
    visitor->endVisit(this);
}

void LiteralExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void NumericExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void NullValueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ParameterExpressionAST::firstToken() const
{
    if ( colon_token )
        return colon_token;
    if ( bindName )
        return bindName->firstToken();
    return 0;
}

int ParameterExpressionAST::lastToken() const
{
    if ( bindName )
        return bindName->lastToken();
    if ( colon_token )
        return colon_token + 1;
    return 1;
}

int SelectCompoundValueExpressionAST::firstToken() const
{
    return lparen_token;
}

int SelectCompoundValueExpressionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( query )
        return query->lastToken();
    return lparen_token + 1;
}

void SelectCompoundValueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(query, visitor);
    }
    visitor->endVisit(this);
}

int CaseExpressionAST::firstToken() const
{
    return case_token;
}

int CaseExpressionAST::lastToken() const
{
    if ( end_token )
        return end_token + 1;
    if ( elseExpr )
        return elseExpr->lastToken();
    if ( else_token )
        return else_token + 1;
    if ( var_list )
        return var_list->lastToken();
    if ( caseExpr )
        return caseExpr->lastToken();
    return case_token + 1;
}

void CaseExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(caseExpr, visitor);
        accept(var_list, visitor);
        accept(elseExpr, visitor);
    }
    visitor->endVisit(this);
}

int CaseConditionAST::firstToken() const
{
    return case_token;
}

int CaseConditionAST::lastToken() const
{
    if ( end_token )
        return end_token + 1;
    if ( elseExpr )
        return elseExpr->lastToken();
    if ( else_token )
        return else_token + 1;
    if ( var_list )
        return var_list->lastToken();
    return case_token + 1;
}

void CaseConditionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(var_list, visitor);
        accept(elseExpr, visitor);
    }
    visitor->endVisit(this);
}

int ValueListExpressionAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( val_list )
        return val_list->firstToken();
    if ( rparen_roken )
        return rparen_roken;
    return 0;
}

int ValueListExpressionAST::lastToken() const
{
    if ( rparen_roken )
        return rparen_roken + 1;
    if ( val_list )
        return val_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 1;
}

void ValueListExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(val_list, visitor);
    }
    visitor->endVisit(this);
}

int SubQueryExpressionAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( query )
        return query->firstToken();
    if ( rparen_roken )
        return rparen_roken;
    return 0;
}

int SubQueryExpressionAST::lastToken() const
{
    if ( rparen_roken )
        return rparen_roken + 1;
    if ( query )
        return query->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 1;
}

void SubQueryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(query, visitor);
    }
    visitor->endVisit(this);
}

void ParameterExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(bindName, visitor);
    }
    visitor->endVisit(this);
}

int NotConditionExpressionAST::firstToken() const
{
    return not_token;
}

int NotConditionExpressionAST::lastToken() const
{
    if ( condition )
        return condition->lastToken();
    return not_token + 1;
}

void NotConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(condition, visitor);
    }
    visitor->endVisit(this);
}

void TrueConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}
void FalseConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int BinaryConditionExpressionAST::firstToken() const
{
    if ( lvCondExpr )
        return lvCondExpr->firstToken();
    if ( cond_op_token )
        return cond_op_token;
    if ( rvCondExpr )
        return rvCondExpr->firstToken();
    return 0;
}

int BinaryConditionExpressionAST::lastToken() const
{
    if ( rvCondExpr )
        return rvCondExpr->lastToken();
    if ( cond_op_token )
        return cond_op_token + 1;
    if ( lvCondExpr )
        return lvCondExpr->lastToken();
    return 1;
}

void BinaryConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvCondExpr, visitor);
        accept(rvCondExpr, visitor);
    }
    visitor->endVisit(this);
}

int CompareConditionExpressionAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( compare_op_token )
        return compare_op_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    return 0;
}

int CompareConditionExpressionAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( compare_op_token )
        return compare_op_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

void CompareConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvExpr, visitor);
        accept(rvExpr, visitor);
    }
    visitor->endVisit(this);
}

int CompoundConditionExpressionAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( condExpr )
        return condExpr->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int CompoundConditionExpressionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( condExpr )
        return condExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 0;
}

void CompoundConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(condExpr, visitor);
    }
    visitor->endVisit(this);
}

int LikeConditionExpressionAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( not_token )
        return not_token;
    if ( like_token )
        return like_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    return 0;
}

int LikeConditionExpressionAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( like_token )
        return like_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

void LikeConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvExpr, visitor);
        accept(rvExpr, visitor);
    }
    visitor->endVisit(this);
}

int BetweenConditionExpressionAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( not_token )
        return not_token;
    if ( between_token )
        return between_token;
    if ( rv1Expr )
        return rv1Expr->firstToken();
    if ( and_token )
        return and_token;
    if ( rv2Expr )
        return rv2Expr->firstToken();
    return 0;
}

int BetweenConditionExpressionAST::lastToken() const
{
    if ( rv2Expr )
        return rv2Expr->lastToken();
    if ( and_token )
        return and_token + 1;
    if ( rv1Expr )
        return rv1Expr->lastToken();
    if ( between_token )
        return between_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

void BetweenConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvExpr, visitor);
        accept(rv1Expr, visitor);
        accept(rv2Expr, visitor);
    }
    visitor->endVisit(this);
}

int InConditionExpressionAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( not_token )
        return not_token;
    if ( in_token )
        return in_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    return 0;
}

int InConditionExpressionAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( in_token )
        return in_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

void InConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvExpr, visitor);
        accept(rvExpr, visitor);
    }
    visitor->endVisit(this);
}

int ExistsConditionExpressionAST::firstToken() const
{
    if ( exists_token )
        return exists_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    return 0;
}

int ExistsConditionExpressionAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( exists_token )
        return exists_token + 1;
    return 1;
}

void ExistsConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rvExpr, visitor);
    }
    visitor->endVisit(this);
}

int IsNullConditionExpressionAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( is_token )
        return is_token;
    if ( not_token )
        return not_token;
    if ( null_token )
        return null_token;
    return 0;
}

int IsNullConditionExpressionAST::lastToken() const
{
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( is_token )
        return is_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

void IsNullConditionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lvExpr, visitor);
    }
    visitor->endVisit(this);
}

int ConditionAsValueExpressionAST::firstToken() const
{
    if ( expr )
        return expr->firstToken();
    return 0;
}

int ConditionAsValueExpressionAST::lastToken() const
{
    if ( expr )
        return expr->lastToken();
    return 1;
}

void ConditionAsValueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CaseExpressionClauseAST::firstToken() const
{
    return when_token;
}

int CaseExpressionClauseAST::lastToken() const
{
    if ( thenExpr )
        return thenExpr->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( whenExpr )
        return whenExpr->lastToken();
    return when_token + 1;
}

void CaseExpressionClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(whenExpr, visitor);
        accept(thenExpr, visitor);
    }
    visitor->endVisit(this);
}

int CaseConditionClauseAST::firstToken() const
{
    return when_token;
}

int CaseConditionClauseAST::lastToken() const
{
    if ( thenExpr )
        return thenExpr->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( whenExpr )
        return whenExpr->lastToken();
    return when_token + 1;
}

void CaseConditionClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(whenExpr, visitor);
        accept(thenExpr, visitor);
    }
    visitor->endVisit(this);
}

int TableIdentifierAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( schema_dot_token )
        return schema_dot_token;
    if ( tableName )
        return tableName->firstToken();
    return 0;
}

int TableIdentifierAST::lastToken() const
{
    if ( tableName )
        return tableName->lastToken();
    if ( schema_dot_token )
        return schema_dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

void TableIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(tableName, visitor);
    }
    visitor->endVisit(this);
}

int IndexIdentifierAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( schema_dot_token )
        return schema_dot_token;
    if ( indexName )
        return indexName->firstToken();
    return 0;
}

int IndexIdentifierAST::lastToken() const
{
    if ( indexName )
        return indexName->lastToken();
    if ( schema_dot_token )
        return schema_dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

void IndexIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(indexName, visitor);
    }
    visitor->endVisit(this);
}

int TriggerIdentifierAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( schema_dot_token )
        return schema_dot_token;
    if ( triggerName )
        return triggerName->firstToken();
    return 0;
}

int TriggerIdentifierAST::lastToken() const
{
    if ( triggerName )
        return triggerName->lastToken();
    if ( schema_dot_token )
        return schema_dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

void TriggerIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(triggerName, visitor);
    }
    visitor->endVisit(this);
}

int ColumnIdentifierAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( schema_dot_token )
        return schema_dot_token;
    if ( tableName )
        return tableName->firstToken();
    if ( table_dot_token )
        return table_dot_token;
    if ( columnName )
        return columnName->firstToken();
    return 0;
}

int ColumnIdentifierAST::lastToken() const
{
    if ( columnName )
        return columnName->lastToken();
    if ( table_dot_token )
        return table_dot_token + 1;
    if ( tableName )
        return tableName->lastToken();
    if ( schema_dot_token )
        return schema_dot_token + 1;
    if ( schemaName )
        return schemaName->lastToken();
    return 1;
}

void ColumnIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(tableName, visitor);
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int FunctionIdentifierAST::firstToken() const
{
    if ( schemaName )
        return schemaName->firstToken();
    if ( schema_dot_token )
        return schema_dot_token;
    if ( packageName )
        return packageName->firstToken();
    if ( pck_dot_token )
        return pck_dot_token;
    if ( functionName )
        return  functionName->firstToken();
    return 0;
}

int FunctionIdentifierAST::lastToken() const
{
    if ( functionName )
        return  functionName->firstToken();
    if ( pck_dot_token )
        return pck_dot_token;
    if ( packageName )
        return packageName->firstToken();
    if ( schema_dot_token )
        return schema_dot_token;
    if ( schemaName )
        return schemaName->firstToken();
    return 1;
}

void FunctionIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(packageName, visitor);
        accept(functionName, visitor);
    }
    visitor->endVisit(this);
}

int CollateClauseAST::firstToken() const
{
    return collate_token;
}

int CollateClauseAST::lastToken() const
{
    if ( collateAst )
        return collateAst->lastToken();
    return collate_token + 1;
}

void CollateClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collateAst, visitor);
    }
    visitor->endVisit(this);
}

int AlterColumnSetDefaultAST::firstToken() const
{
    return set_token;
}

int AlterColumnSetDefaultAST::lastToken() const
{
    if ( defaultExpr )
        return defaultExpr->lastToken();
    if ( default_token )
        return default_token + 1;
    return set_token + 1;
}

void AlterColumnSetDefaultAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(defaultExpr, visitor);
    }
    visitor->endVisit(this);
}

int AlterColumnDropDefaultAST::firstToken() const
{
    return drop_token;
}

int AlterColumnDropDefaultAST::lastToken() const
{
    if ( default_token )
        return default_token + 1;
    return drop_token + 1;
}

void AlterColumnDropDefaultAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterColumnSetOrDropNotNullAST::firstToken() const
{
    return set_or_drop_token;
}

int AlterColumnSetOrDropNotNullAST::lastToken() const
{
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    return set_or_drop_token + 1;
}

void AlterColumnSetOrDropNotNullAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CharacterTypeAST::firstToken() const
{
    return type_token;
}

int CharacterTypeAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( size_token )
        return size_token + 1;
    if ( lparen_token )
        return lparen_token + 1;
    return type_token + 1;
}

void CharacterTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int NumberTypeAST::firstToken() const
{
    return type_token;
}

int NumberTypeAST::lastToken() const
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

void NumberTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void SimpleTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UserDefinedTypeAST::firstToken() const
{
    if ( typeName )
        return typeName->firstToken();
    return 0;
}

int UserDefinedTypeAST::lastToken() const
{
    if ( typeName )
        return typeName->lastToken();
    return 1;
}

void UserDefinedTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintDeclarationAST::firstToken() const
{
    return constraint_token;
}

int ConstraintDeclarationAST::lastToken() const
{
    if ( constraintName )
        return constraintName->lastToken();
    return constraint_token + 1;
}

void ConstraintDeclarationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintName, visitor);
    }
    visitor->endVisit(this);
}

int SortedColumnNameAST::firstToken() const
{
    return columnName->firstToken();
}

int SortedColumnNameAST::lastToken() const
{
    if ( asc_or_desc_token )
        return asc_or_desc_token + 1;
    return columnName->lastToken();
}

void SortedColumnNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int UniqueOrPkTableConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
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

int UniqueOrPkTableConstraintAST::lastToken() const
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
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void UniqueOrPkTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(column_list, visitor);
    }
    visitor->endVisit(this);
}

int UniqueOrPkTableConstraintUsingIndexAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
    if ( unique_or_primary_token )
        return unique_or_primary_token;
    if ( key_token )
        return key_token;
    if ( using_token )
        return using_token;
    if ( index_token )
        return index_token;
    if ( indexIdent )
        return indexIdent->firstToken();
    if ( defer )
        return defer->firstToken();
    return 0;
}

int UniqueOrPkTableConstraintUsingIndexAST::lastToken() const
{
    if ( defer )
        return defer->lastToken();
    if ( indexIdent )
        return indexIdent->lastToken();
    if ( index_token )
        return index_token + 1;
    if ( using_token )
        return using_token + 1;
    if ( key_token )
        return key_token + 1;
    if ( unique_or_primary_token )
        return unique_or_primary_token + 1;
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void UniqueOrPkTableConstraintUsingIndexAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(indexIdent, visitor);
        accept(defer, visitor);
    }
    visitor->endVisit(this);
}

int CheckTableConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
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

int CheckTableConstraintAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( checkExpr )
        return checkExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( check_token )
        return check_token + 1;
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void CheckTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(checkExpr, visitor);
    }
    visitor->endVisit(this);
}

int FkReferencesClauseAST::firstToken() const
{
    if ( references_token )
        return references_token;
    if ( tableIdent )
        return tableIdent->firstToken();
    if ( ref_lparen_token )
        return ref_lparen_token;
    if ( ref_col_list )
        return ref_col_list->firstToken();
    if ( ref_rparen_token )
        return ref_rparen_token;
    if ( matchClause )
        return matchClause->firstToken();
    if ( actionClause )
        return actionClause->firstToken();
    return 0;
}

int FkReferencesClauseAST::lastToken() const
{
    if ( actionClause )
        return actionClause->lastToken();
    if ( matchClause )
        return matchClause->lastToken();
    if ( ref_rparen_token )
        return ref_rparen_token + 1;
    if ( ref_col_list )
        return ref_col_list->lastToken();
    if ( ref_lparen_token )
        return ref_lparen_token + 1;
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( references_token )
        return references_token + 1;
    return 1;
}

void FkReferencesClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(ref_col_list, visitor);
        accept(matchClause, visitor);
        accept(actionClause, visitor);
    }
    visitor->endVisit(this);
}

int FkReferencesMatchAST::firstToken() const
{
    return match_token;
}

int FkReferencesMatchAST::lastToken() const
{
    if ( full_or_partial_or_simple_token )
        return full_or_partial_or_simple_token + 1;
    return match_token + 1;
}

void FkReferencesMatchAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int FkReferencesActionAST::firstToken() const
{
    return on_token;
}

int FkReferencesActionAST::lastToken() const
{
    if ( act2_token )
        return act2_token + 1;
    if ( act1_token )
        return act1_token + 1;
    if ( delete_or_update_token )
        return delete_or_update_token + 1;
    return on_token + 1;
}

void FkReferencesActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int FkTableConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
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

int FkTableConstraintAST::lastToken() const
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
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void FkTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(col_list, visitor);
        accept(refClause, visitor);
    }
    visitor->endVisit(this);
}

int PkColumnConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
    if ( primary_token )
        return primary_token;
    if ( key_token )
        return key_token;
    if ( defer )
        return defer->firstToken();
    return 0;
}

int PkColumnConstraintAST::lastToken() const
{
    if ( defer )
        return defer->lastToken();
    if ( key_token )
        return key_token + 1;
    if ( primary_token )
        return primary_token + 1;
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void PkColumnConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(defer, visitor);
    }
    visitor->endVisit(this);
}

int NotNullColumnConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
    if ( not_token )
        return not_token;
    if ( null_token )
        return null_token;
    if ( defer )
        return defer->firstToken();
    return 0;
}

int NotNullColumnConstraintAST::lastToken() const
{
    if ( defer )
        return defer->lastToken();
    if ( null_token )
        return null_token + 1;
    if ( not_token )
        return not_token + 1;
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void NotNullColumnConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(defer, visitor);
    }
    visitor->endVisit(this);
}

int UniqueColumnConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
    if ( unique_token )
        return unique_token;
    if ( defer )
        return defer->firstToken();
    return 0;
}

int UniqueColumnConstraintAST::lastToken() const
{
    if ( defer )
        return defer->lastToken();
    if ( unique_token )
        return unique_token + 1;
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void UniqueColumnConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(defer, visitor);
    }
    visitor->endVisit(this);
}

int CheckColumnConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
    if ( check_token )
        return check_token;
    if ( lparen_token )
        return lparen_token;
    if ( checkExpr )
        return checkExpr->firstToken();
    if ( rparen_token )
        return rparen_token;
    if ( defer )
        return defer->firstToken();
    return 0;
}

int CheckColumnConstraintAST::lastToken() const
{
    if ( defer )
        return defer->lastToken();
    if ( rparen_token )
        return rparen_token + 1;
    if ( checkExpr )
        return checkExpr->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    if ( check_token )
        return check_token + 1;
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void CheckColumnConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(checkExpr, visitor);
        accept(defer, visitor);
    }
    visitor->endVisit(this);
}

int ReferencesColumnConstraintAST::firstToken() const
{
    if ( constraintDecl )
        return constraintDecl->firstToken();
    if ( refClause )
        return refClause->firstToken();
    if ( defer )
        return defer->firstToken();
    return 0;
}

int ReferencesColumnConstraintAST::lastToken() const
{
    if ( defer )
        return defer->lastToken();
    if ( refClause )
        return refClause->lastToken();
    if ( constraintDecl )
        return constraintDecl->lastToken();
    return 1;
}

void ReferencesColumnConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintDecl, visitor);
        accept(refClause, visitor);
        accept(defer, visitor);
    }
    visitor->endVisit(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//void SqlCheckNamesWalker::walk( Sql::BaseWalkReflector *reflector) {
//    if ( m_ast ) {
//        SqlCheckNamesVisitor visitor(m_translationUnit, reflector);
//        m_ast->accept(&visitor);
//    }
//}

}   // namespace Postgres
