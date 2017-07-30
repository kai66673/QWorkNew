#include "QLalrOracleAST.h"
#include "QLalrOracleASTVisitor.h"
#include "QLalrOracleSqlCheckNamesVisitor.h"
#include "MemoryPool.h"

#include <cassert>
#include <algorithm>

/*
   All firstToken/lastToken methods below which have a doxygen comment with
   \generated in it, will be re-generated when the tool "generate-ast" is run.

   For methods which are hand-coded, or which should not be changed, make sure that
   the comment is gone.
 */


using namespace QLalrOracle;

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

int TranslationUnitAST::firstToken() const
{
    if ( statement_list )
        if ( int candidate = statement_list->firstToken() )
            return candidate;
    return 0;
}

int TranslationUnitAST::lastToken() const
{
    if ( statement_list )
        if ( int candidate = statement_list->lastToken() )
            return candidate;
    return 1;
}

int UnknownStatementAST::firstToken() const
{
    return first_token;
}

int UnknownStatementAST::lastToken() const
{
    return last_token;
}

int SelectStatementAST::firstToken() const
{
    if ( select_core )
        return select_core->firstToken();
    if ( order_by_block )
        return order_by_block->firstToken();
    if ( forupdate_clause )
        return forupdate_clause->firstToken();
    return 0;
}

int SelectStatementAST::lastToken() const
{
    if ( forupdate_clause )
        return forupdate_clause->lastToken();
    if ( order_by_block )
        return order_by_block->lastToken();
    if ( select_core )
        return select_core->lastToken();
    return 1;
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
    if ( joinSource )
        return joinSource->lastToken();
    if ( from_token )
        return from_token + 1;
    if ( select_spec_token )
        return select_spec_token + 1;
    if ( selColumns )
        return selColumns->lastToken();
    return select_token + 1;
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

int LimitToBlockAST::firstToken() const
{
    return limit_token;
}

int LimitToBlockAST::lastToken() const
{
    if ( row_token )
        return row_token;
    if ( expr )
        return expr->lastToken();
    return to_token + 1;
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

int StarSelectColumnAST::firstToken() const
{
    return star_token;
}

int StarSelectColumnAST::lastToken() const
{
    return star_token + 1;
}

int ListSelectColumnAST::firstToken() const
{
    if ( columns )
        return columns->firstToken();
    return 0;
}

int ListSelectColumnAST::lastToken() const
{
    if ( columns )
        return columns->lastToken();
    return 1;
}

int TableSelectColumnsItemAST::firstToken() const
{
    if ( tableIdent )
        return tableIdent->firstToken();
    if ( dot_token )
        return dot_token;
    if ( star_token )
        return star_token;
    return 0;
}

int TableSelectColumnsItemAST::lastToken() const
{
    if ( star_token )
        return star_token + 1;
    if ( dot_token )
        return dot_token + 1;
    if ( tableIdent )
        return tableIdent->lastToken();
    return 1;
}

int ValueSelectColumnItemAST::firstToken() const
{
    if ( columnExpr )
        return columnExpr->firstToken();
    if ( as_token )
        return as_token;
    if ( columnAlias )
        return columnAlias->firstToken();
    return 0;
}

int ValueSelectColumnItemAST::lastToken() const
{
    if ( columnAlias )
        return columnAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( columnExpr )
        return columnExpr->lastToken();
    return 1;
}

int StarResultColumnAST::firstToken() const
{
    if ( tableIdent )
        return tableIdent->firstToken();
    if ( dot_token )
        return dot_token;
    return star_token;
}

int StarResultColumnAST::lastToken() const
{
    return star_token + 1;
}

int ExprResultColumnAST::firstToken() const
{
    if ( expr )
        return expr->firstToken();
    if ( as_token )
        return as_token;
    if ( columnAlias )
        return columnAlias->firstToken();
    return 0;
}

int ExprResultColumnAST::lastToken() const
{
    if ( columnAlias )
        return columnAlias->lastToken();
    if ( as_token )
        return as_token + 1;
    if ( expr )
        return expr->lastToken();
    return 1;
}

int SubQueryAST::firstToken() const
{
    return lparen_token;
}

int SubQueryAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( selectStatement )
        return selectStatement->lastToken();
    return lparen_token + 1;
}

int DirectSingleSourceAST::firstToken() const
{
    if ( schemaTableName )
        return schemaTableName->firstToken();
    if ( tableAlias )
        return tableAlias->firstToken();
    return 0;
}

int DirectSingleSourceAST::lastToken() const
{
    if ( tableAlias )
        return tableAlias->lastToken();
    if ( schemaTableName )
        return schemaTableName->lastToken();
    return 1;
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

int JoinConstraintAST::firstToken() const
{
    return on_token;
}

int JoinConstraintAST::lastToken() const
{
    if ( expr )
        return expr->lastToken();
    return on_token + 1;
}

int JoinItemAST::firstToken() const
{
    if ( joinOp )
        return joinOp->firstToken();
    if ( singleSource )
        return singleSource->firstToken();
    if ( joinConstraint )
        return joinConstraint->firstToken();
    return 0;
}

int JoinItemAST::lastToken() const
{
    if ( joinConstraint )
        return joinConstraint->lastToken();
    if ( singleSource )
        return singleSource->lastToken();
    if ( joinOp )
        return joinOp->lastToken();
    return 1;
}

int CompoundJoinListAST::firstToken() const
{
    if ( lparen_token )
        return lparen_token;
    if ( join_list )
        return join_list->firstToken();
    if ( rparen_token )
        return rparen_token;
    return 0;
}

int CompoundJoinListAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( join_list )
        return join_list->lastToken();
    if ( lparen_token )
        return lparen_token + 1;
    return 1;
}

int CommaJoinListAST::firstToken() const
{
    if ( comma_token )
        return comma_token;
    if ( source )
        return source->firstToken();
    if ( join_list )
        return join_list->firstToken();
    return 0;
}

int CommaJoinListAST::lastToken() const
{
    if ( join_list )
        return join_list->lastToken();
    if ( source )
        return source->lastToken();
    if ( comma_token )
        return comma_token + 1;
    return 1;
}

int JoinJoinListAST::firstToken() const
{
    if ( j1_spec_token )
        return j1_spec_token;
    if ( j2_spec_token )
        return j2_spec_token;
    if ( join_token )
        return join_token;
    if ( source )
        return source->firstToken();
    if ( join_list )
        return join_list->firstToken();
    return 0;
}

int JoinJoinListAST::lastToken() const
{
    if ( join_list )
        return join_list->lastToken();
    if ( source )
        return source->lastToken();
    if ( join_token )
        return join_token + 1;
    if ( j2_spec_token )
        return j2_spec_token + 1;
    if ( j1_spec_token )
        return j1_spec_token + 1;
    return 1;
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

int JoinSourceAST::firstToken() const
{
    if ( firstSource )
        return firstSource->firstToken();
    if ( join_list )
        return join_list->firstToken();
    return 0;
}

int JoinSourceAST::lastToken() const
{
    if ( join_list )
        return join_list->lastToken();
    if ( firstSource )
        return firstSource->lastToken();
    return 1;
}

int GroupByBlockAST::firstToken() const
{
    return group_token;
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

int StartConnectPriorAST::firstToken() const
{
    return prior_token;
}

int StartConnectPriorAST::lastToken() const
{
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( compare_token )
        return compare_token+1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return prior_token;
}

int EndConnectPriorAST::firstToken() const
{
    if ( lvExpr )
        return lvExpr->firstToken();
    if ( compare_token )
        return compare_token;
    if ( rvExpr )
        return rvExpr->firstToken();
    if ( prior_token )
        return prior_token;
    return 0;
}

int EndConnectPriorAST::lastToken() const
{
    if ( prior_token )
        return prior_token + 1;
    if ( rvExpr )
        return rvExpr->lastToken();
    if ( compare_token )
        return compare_token + 1;
    if ( lvExpr )
        return lvExpr->lastToken();
    return 1;
}

int StartWithClauseAST::firstToken() const
{
    return start_token;
}

int StartWithClauseAST::lastToken() const
{
    if ( conditionExpr )
        return conditionExpr->lastToken();
    if ( with_token )
        return with_token + 1;
    return start_token + 1;
}

int ConnectByClauseAST::firstToken() const
{
    return connect_token;
}

int ConnectByClauseAST::lastToken() const
{
    if ( startWithClause )
        return startWithClause->lastToken();
    if ( connectPriorClause )
        return connectPriorClause->lastToken();
    if ( by_token )
        return by_token + 1;
    return connect_token + 1;
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

int MinusSetSpecAST::firstToken() const
{
    return minus_token;
}

int MinusSetSpecAST::lastToken() const
{
    return minus_token + 1;
}

int IntersectSetSpecAST::firstToken() const
{
    return intersect_token;
}

int IntersectSetSpecAST::lastToken() const
{
    return intersect_token + 1;
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

int CreateTriggerRefClauseAST::firstToken() const
{
    return referencing_token;
}

int CreateTriggerRefClauseAST::lastToken() const
{
    int lastToken_ = 0;

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

int CreateRdbTriggerBodyAST::firstToken() const
{
    if ( item_list )
        return item_list->firstToken();
    return 0;
}

int CreateRdbTriggerBodyAST::lastToken() const
{
    if ( item_list )
        return item_list->lastToken();
    return 1;
}

int CreateRdbTriggerBodyItemAST::firstToken() const
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

int CreateRdbTriggerBodyItemAST::lastToken() const
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

int ColumnDefinitionAST::firstToken() const
{
    if ( columnName )
        return columnName->firstToken();
    return 0;
}

int ColumnDefinitionAST::lastToken() const
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

int UniqueOrPkTableConstraintAST::firstToken() const
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
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

int CheckTableConstraintAST::firstToken() const
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
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

int FkTableConstraintAST::firstToken() const
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
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
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
    return 0;
}

int FkReferencesClauseAST::lastToken() const
{
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

int PkColumnConstraintAST::firstToken() const
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
    if ( state )
        return state->firstToken();
    return 0;
}

int PkColumnConstraintAST::lastToken() const
{
    if ( state )
        return state->lastToken();
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

int NotNullColumnConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( not_token )
        return not_token;
    if ( null_token )
        return null_token;
    if ( state )
        return state->firstToken();
    return 0;
}

int NotNullColumnConstraintAST::lastToken() const
{
    if ( state )
        return state->lastToken();
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

int UniqueColumnConstraintAST::firstToken() const
{
    if ( constraint_token )
        return constraint_token;
    if ( constraintName )
        return constraintName->firstToken();
    if ( unique_token )
        return unique_token;
    if ( state )
        return state->firstToken();
    return 0;
}

int UniqueColumnConstraintAST::lastToken() const
{
    if ( state )
        return state->lastToken();
    if ( unique_token )
        return unique_token + 1;
    if ( constraintName )
        return constraintName->lastToken();
    if ( constraint_token )
        return constraint_token + 1;
    return 1;
}

int CheckColumnConstraintAST::firstToken() const
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
    if ( state )
        return state->firstToken();
    return 0;
}

int CheckColumnConstraintAST::lastToken() const
{
    if ( state )
        return state->lastToken();
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

int ReferencesColumnConstraintAST::firstToken() const
{
    if ( refClause )
        return refClause->firstToken();
    if ( state )
        return state->firstToken();
    return 0;
}

int ReferencesColumnConstraintAST::lastToken() const
{
    if ( state )
        return state->lastToken();
    if ( refClause )
        return refClause->lastToken();
    return 1;
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

int CreateTriggerStatementAST::firstToken() const
{
    return create_token;
}

int CreateTriggerStatementAST::lastToken() const
{
    if ( triggerBody )
        return triggerBody->lastToken();
    if ( triggerRefClause )
        return triggerRefClause->lastToken();
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( on_token )
        return on_token + 1;
    if ( action_token )
        return action_token + 1;
    if ( before_or_after_token )
        return before_or_after_token + 1;
    if ( triggeeIdent )
        return triggeeIdent->lastToken();
    if ( trigger_token )
        return trigger_token + 1;
    return create_token + 1;
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

int AlterTableStatementAST::firstToken() const
{
    return alter_token;
}

int AlterTableStatementAST::lastToken() const
{
    if ( alterTableClause )
        return alterTableClause->lastToken();
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( table_token )
        return table_token + 1;
    return alter_token + 1;
}

int AlterTableDropClauseAST::firstToken() const
{
    return drop_token;
}

int AlterTableDropClauseAST::lastToken() const
{
    if ( dropObjectName )
        return dropObjectName->lastToken();
    if ( drop_object_type_token )
        return drop_object_type_token + 1;
    return drop_token + 1;
}

int AlterTableAddConstraintClauseAST::firstToken() const
{
    return add_token;
}

int AlterTableAddConstraintClauseAST::lastToken() const
{
    if ( constraint )
        return constraint->lastToken();
    return add_token + 1;
}

int AlterTableAddColumnClauseAST::firstToken() const
{
    return add_token;
}

int AlterTableAddColumnClauseAST::lastToken() const
{
    if ( column )
        return column->lastToken();
    if ( column_token )
        return column_token + 1;
    return add_token + 1;
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

int CommitStatementAST::firstToken() const
{
    return commit_token;
}

int CommitStatementAST::lastToken() const
{
    return commit_token + 1;
}

int RollbackStatementAST::firstToken() const
{
    return rollback_token;
}

int RollbackStatementAST::lastToken() const
{
    return rollback_token + 1;
}

int CallStatementAST::firstToken() const
{
    return call_token;
}

int CallStatementAST::lastToken() const
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
// PL-SQL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int PlSqlBlockAST::firstToken() const
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

int PlSqlBlockAST::lastToken() const
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

int PlSqlBlockBodyAST::firstToken() const
{
    return begin_token;
}

int PlSqlBlockBodyAST::lastToken() const
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

int PlSqlStatementAST::firstToken() const
{
    if ( label )
        return label->firstToken();
    if ( statement )
        return statement->firstToken();
    return 0;
}

int PlSqlStatementAST::lastToken() const
{
    if ( statement )
        return statement->lastToken();
    if ( label )
        return label->lastToken();
    return 1;
}

int BlockDeleteStatementAST::firstToken() const
{
    if ( deleteStatement )
        return deleteStatement->firstToken();
    return 0;
}

int BlockDeleteStatementAST::lastToken() const
{
    if ( deleteStatement )
        return deleteStatement->lastToken();
    return 1;
}

int BlockInsertStatementAST::firstToken() const
{
    if ( insertStatement )
        return insertStatement->firstToken();
    return 0;
}

int BlockInsertStatementAST::lastToken() const
{
    if ( insertStatement )
        return insertStatement->lastToken();
    return 1;
}

int BlockLockTableStatementAST::firstToken() const
{
    return lock_token;
}

int BlockLockTableStatementAST::lastToken() const
{
    if ( tableIdent )
        return tableIdent->lastToken();
    if ( table_token )
        return table_token + 1;
    return lock_token + 1;
}

int BlockRollbackStatementAST::firstToken() const
{
    return rollback_token;
}

int BlockRollbackStatementAST::lastToken() const
{
    if ( rollbacClause )
        return rollbacClause->lastToken();
    if ( work_token )
        return work_token + 1;
    return rollback_token + 1;
}

int RollbackToSavepointClauseAST::firstToken() const
{
    return to_token;
}

int RollbackToSavepointClauseAST::lastToken() const
{
    if ( savepointName )
        return savepointName->lastToken();
    if ( savepoint_token )
        return savepoint_token + 1;
    return to_token + 1;
}

int RollbackForceClauseAST::firstToken() const
{
    return force_token;
}

int RollbackForceClauseAST::lastToken() const
{
    if ( stringExpr )
        return stringExpr->lastToken();
    return force_token + 1;
}

int BlockSavepointStatementAST::firstToken() const
{
    return savepoint_token;
}

int BlockSavepointStatementAST::lastToken() const
{
    if ( savepointName )
        return savepointName->lastToken();
    return savepoint_token + 1;
}

int BlockSelectIntoStatementAST::firstToken() const
{
    return select_token;
}

int BlockSelectIntoStatementAST::lastToken() const
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

int BlockSetTransactionStatementAST::firstToken() const
{
    return set_token;
}

int BlockSetTransactionStatementAST::lastToken() const
{
    if ( transaction_token )
        return transaction_token + 1;
    return set_token + 1;
}

int BlockUpdateStatementAST::firstToken() const
{
    if ( updateStatement )
        return updateStatement->firstToken();
    return 0;
}

int BlockUpdateStatementAST::lastToken() const
{
    if ( updateStatement )
        return updateStatement->lastToken();
    return 1;
}

int AssignmentStatementAST::firstToken() const
{
    if ( lvalueExpr )
        return lvalueExpr->firstToken();
    if ( assign_token )
        return assign_token;
    if ( rvalueExpr )
        return rvalueExpr->firstToken();
    return 0;
}

int AssignmentStatementAST::lastToken() const
{
    if ( rvalueExpr )
        return rvalueExpr->lastToken();
    if ( assign_token )
        return assign_token + 1;
    if ( lvalueExpr )
        return lvalueExpr->lastToken();
    return 1;
}

int LValueExpressionAST::firstToken() const
{
    if ( dotted_list )
        return dotted_list->firstToken();
    return 0;
}

int LValueExpressionAST::lastToken() const
{
    if ( dotted_list )
        return dotted_list->lastToken();
    return 1;
}

int LValueExpressionItemAST::firstToken() const
{
    if ( identName )
        return identName->firstToken();
    if ( arr_ind_list )
        return arr_ind_list->firstToken();
    return 0;
}

int LValueExpressionItemAST::lastToken() const
{
    if ( arr_ind_list )
        return arr_ind_list->lastToken();
    if ( identName )
        return identName->lastToken();
    return 1;
}

int CloseStatementAST::firstToken() const
{
    return close_token;
}

int CloseStatementAST::lastToken() const
{
    if ( cursorName )
        return cursorName->lastToken();
    return close_token + 1;
}

int ContinueOrExitStatementAST::firstToken() const
{
    return continue_or_exit_token;
}

int ContinueOrExitStatementAST::lastToken() const
{
    if ( condExpr )
        return condExpr->lastToken();
    if ( when_token )
        return when_token + 1;
    if ( labelName )
        return labelName->lastToken();
    return continue_or_exit_token + 1;
}

int ExecImmediateStatementAST::firstToken() const
{
    return exec_token;
}

int ExecImmediateStatementAST::lastToken() const
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

int FetchStatementAST::firstToken() const
{
    return fetch_token;
}

int FetchStatementAST::lastToken() const
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

int ForLoopStatementAST::firstToken() const
{
    return for_token;
}

int ForLoopStatementAST::lastToken() const
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

int ForInBoundsClauseAST::firstToken() const
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

int ForInBoundsClauseAST::lastToken() const
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

int ForInCursorClauseAST::firstToken() const
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

int ForInCursorClauseAST::lastToken() const
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

int ForInSelectClauseAST::firstToken() const
{
    return lparen_token;
}

int ForInSelectClauseAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( selectStatement )
        return selectStatement->lastToken();
    return lparen_token + 1;
}

int ForallStatementAST::firstToken() const
{
    return forall_token;
}

int ForallStatementAST::lastToken() const
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

int ForallInBoundsClauseAST::firstToken() const
{
    if ( lboundExpr )
        return lboundExpr->firstToken();
    if ( dotdot_token )
        return dotdot_token;
    if ( rboundExpr )
        return rboundExpr->firstToken();
    return 0;
}

int ForallInBoundsClauseAST::lastToken() const
{
    if ( rboundExpr )
        return rboundExpr->lastToken();
    if ( dotdot_token )
        return dotdot_token + 1;
    if ( lboundExpr )
        return lboundExpr->lastToken();
    return 1;
}

int ForallInIndicesClauseAST::firstToken() const
{
    return indices_token;
}

int ForallInIndicesClauseAST::lastToken() const
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

int ForallInValuesClauseAST::firstToken() const
{
    return values_token;
}

int ForallInValuesClauseAST::lastToken() const
{
    if ( collectionName )
        return collectionName->lastToken();
    if ( of_token )
        return of_token + 1;
    return values_token + 1;
}

int WhileLoopStatementAST::firstToken() const
{
    return while_token;
}

int WhileLoopStatementAST::lastToken() const
{
    if ( loop )
        return loop->lastToken();
    if ( condExpr )
        return condExpr->lastToken();
    return while_token + 1;
}

int GotoStatementAST::firstToken() const
{
    return goto_token;
}

int GotoStatementAST::lastToken() const
{
    if ( labelName )
        return labelName->lastToken();
    return goto_token + 1;
}

int IfStatementAST::firstToken() const
{
    return if_token;
}

int IfStatementAST::lastToken() const
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

int ElsifClauseAST::firstToken() const
{
    return elsif_token;
}

int ElsifClauseAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( condExpr )
        return condExpr->lastToken();
    return elsif_token + 1;
}

int LoopStatementAST::firstToken() const
{
    return loop_token;
}

int LoopStatementAST::lastToken() const
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

int OpenCursorStatementAST::firstToken() const
{
    return open_token;
}

int OpenCursorStatementAST::lastToken() const
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

int OpenForStatementAST::firstToken() const
{
    return open_token;
}

int OpenForStatementAST::lastToken() const
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

int OpenForStatementClauseBySelectAST::firstToken() const
{
    if ( selectStatement )
        return selectStatement->firstToken();
    return 0;
}

int OpenForStatementClauseBySelectAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    return 1;
}

int OpenForStatementClauseByStringAST::firstToken() const
{
    if ( stmtExpr )
        return stmtExpr->firstToken();
    return 0;
}

int OpenForStatementClauseByStringAST::lastToken() const
{
    if ( stmtExpr )
        return stmtExpr->lastToken();
    return 1;
}

int PlSqlBlocStatementAST::firstToken() const
{
    if ( innerBlock )
        return innerBlock->firstToken();
    return 0;
}

int PlSqlBlocStatementAST::lastToken() const
{
    if ( innerBlock )
        return innerBlock->lastToken();
    return 1;
}

int RaiseStatementAST::firstToken() const
{
    return raise_token;
}

int RaiseStatementAST::lastToken() const
{
    if ( exceptionName )
        return exceptionName->lastToken();
    return raise_token + 1;
}

int ReturnStatementAST::firstToken() const
{
    return return_token;
}

int ReturnStatementAST::lastToken() const
{
    if ( returnExpr )
        return returnExpr->lastToken();
    return return_token + 1;
}

int DeclareSectionAST::firstToken() const
{
    if ( decl_list )
        return decl_list->firstToken();
    return 0;
}

int DeclareSectionAST::lastToken() const
{
    if ( decl_list )
        return decl_list->lastToken();
    return 1;
}

int CursorDeclarationAST::firstToken() const
{
    if ( spec )
        return spec->firstToken();
    if ( is_token )
        return is_token;
    if ( selectStatement )
        return selectStatement->firstToken();
    return 0;
}

int CursorDeclarationAST::lastToken() const
{
    if ( selectStatement )
        return selectStatement->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( spec )
        return spec->lastToken();
    return 1;
}

int VariableDeclarationAST::firstToken() const
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

int VariableDeclarationAST::lastToken() const
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

int ExceptionDeclarationAST::firstToken() const
{
    if ( exceptionName )
        return exceptionName->firstToken();
    if ( exception_token )
        return exception_token;
    return 0;
}

int ExceptionDeclarationAST::lastToken() const
{
    if ( exception_token )
        return exception_token + 1;
    if ( exceptionName )
        return exceptionName->lastToken();
    return 1;
}

int FunctionDeclarationAST::firstToken() const
{
    if ( heading )
        return heading->firstToken();
    if ( spec )
        return spec->firstToken();
    return 0;
}

int FunctionDeclarationAST::lastToken() const
{
    if ( spec )
        return spec->lastToken();
    if ( heading )
        return heading->lastToken();
    return 1;
}

int FunctionDefintionAST::firstToken() const
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

int FunctionDefintionAST::lastToken() const
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

int ProcedureDefinitionAST::firstToken() const
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

int ProcedureDefinitionAST::lastToken() const
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

int ProcedureDeclarationAST::firstToken() const
{
    if ( heading )
        return heading->firstToken();
    return 0;
}

int ProcedureDeclarationAST::lastToken() const
{
    if ( heading )
        return heading->lastToken();
    return 1;
}

int SimpleDeclarationAST::firstToken() const
{
    if ( name )
        return name->firstToken();
    if ( type )
        return type->firstToken();
    return 0;
}

int SimpleDeclarationAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    if ( name )
        return name->lastToken();
    return 1;
}

int TypeDefinitionDeclarationAST::firstToken() const
{
    if ( def )
        return def->firstToken();
    return 0;
}

int TypeDefinitionDeclarationAST::lastToken() const
{
    if ( def )
        return def->lastToken();
    return 1;
}

int RecordTypeDefinitionAST::firstToken() const
{
    return type_token;
}

int RecordTypeDefinitionAST::lastToken() const
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

int RefCursorTypeDefinitionAST::firstToken() const
{
    return type_token;
}

int RefCursorTypeDefinitionAST::lastToken() const
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

int SubtypeDefinitionAST::firstToken() const
{
    return subtype_token;
}

int SubtypeDefinitionAST::lastToken() const
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

int CollectionTypeDefinitionAST::firstToken() const
{
    return type_token;
}

int CollectionTypeDefinitionAST::lastToken() const
{
    if ( collectionType )
        return collectionType->lastToken();
    if ( is_token )
        return is_token + 1;
    if ( typeName )
        return typeName->lastToken();
    return type_token + 1;
}

int TableCollectionTypeAST::firstToken() const
{
    return table_token;
}

int TableCollectionTypeAST::lastToken() const
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

int ArrayCollectionTypeAST::firstToken() const
{
    return arr0_token;
}

int ArrayCollectionTypeAST::lastToken() const
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

int CursorSpecAST::firstToken() const
{
    return cursor_token;
}

int CursorSpecAST::lastToken() const
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

int RowTypeAST::firstToken() const
{
    return typeName->firstToken();
}

int RowTypeAST::lastToken() const
{
    if ( row_or_rowtype_token )
        return row_or_rowtype_token + 1;
    if ( percent_token )
        return percent_token + 1;
    return typeName->lastToken();
}

int DataTypeByTypeNameAST::firstToken() const
{
    if ( ref_token )
        return ref_token;
    if ( typeName )
        return typeName->firstToken();
    return 0;
}

int DataTypeByTypeNameAST::lastToken() const
{
    if ( typeName )
        return typeName->lastToken();
    if ( ref_token )
        return ref_token + 1;
    return 1;
}

int DataTypeByNameAST::firstToken() const
{
    if ( variableName )
        return variableName->firstToken();
    if ( percent_token )
        return percent_token;
    if ( type_token )
        return type_token;
    return 0;
}

int DataTypeByNameAST::lastToken() const
{
    if ( type_token )
        return type_token + 1;
    if ( percent_token )
        return percent_token + 1;
    if ( variableName )
        return variableName->lastToken();
    return 1;
}

int DataTypeByTableRowAST::firstToken() const
{
    if ( tableName )
        return tableName->firstToken();
    if ( percent_token )
        return percent_token;
    if ( rowtype_token )
        return rowtype_token;
    return 0;
}

int DataTypeByTableRowAST::lastToken() const
{
    if ( rowtype_token )
        return rowtype_token + 1;
    if ( percent_token )
        return percent_token + 1;
    if ( tableName )
        return tableName->lastToken();
    return 1;
}

int DataTypeByTableColumnAST::firstToken() const
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

int DataTypeByTableColumnAST::lastToken() const
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

int ScalarDataTypeAST::firstToken() const
{
    if ( type )
        return type->firstToken();
    return 0;
}

int ScalarDataTypeAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    return 1;
}

int CursorParameterDeclarationAST::firstToken() const
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

int CursorParameterDeclarationAST::lastToken() const
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

int RecordFieldDeclarationAST::firstToken() const
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

int RecordFieldDeclarationAST::lastToken() const
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

int RoutineArgumentDeclarationAST::firstToken() const
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

int RoutineArgumentDeclarationAST::lastToken() const
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

int FunctionHeadingAST::firstToken() const
{
    return function_token;
}

int FunctionHeadingAST::lastToken() const
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

int FunctionCasheSpecAST::firstToken() const
{
    return result_cashe_token;
}

int FunctionCasheSpecAST::lastToken() const
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

int ProcedureHeadingAST::firstToken() const
{
    return procedure_token;
}

int ProcedureHeadingAST::lastToken() const
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

int NamedExceptionHandlerAST::firstToken() const
{
    return when_token;
}

int NamedExceptionHandlerAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( name_list )
        return name_list->lastToken();
    return when_token + 1;
}

int OthersExceptionHandlerAST::firstToken() const
{
    return when_token;
}

int OthersExceptionHandlerAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    if ( then_token )
        return then_token + 1;
    if ( others_token )
        return others_token + 1;
    return when_token + 1;
}

int IntoClauseAST::firstToken() const
{
    return into_token;
}

int IntoClauseAST::lastToken() const
{
    if ( names )
        return names->lastToken();
    return into_token + 1;
}

int BulkIntoClauseAST::firstToken() const
{
    return bulk_token;
}

int BulkIntoClauseAST::lastToken() const
{
    if ( names )
        return names->lastToken();
    if ( into_token )
        return into_token + 1;
    if ( collect_token )
        return collect_token + 1;
    return bulk_token + 1;
}

int UsingClauseAST::firstToken() const
{
    return using_token;
}

int UsingClauseAST::lastToken() const
{
    if ( bind_list )
        return bind_list->lastToken();
    return using_token + 1;
}

int UsingBindAST::firstToken() const
{
    if ( in_token )
        return in_token;
    if ( out_token )
        return out_token;
    if ( bindExpr )
        return bindExpr->firstToken();
    return 0;
}

int UsingBindAST::lastToken() const
{
    if ( bindExpr )
        return bindExpr->lastToken();
    if ( out_token )
        return out_token + 1;
    if ( in_token )
        return in_token + 1;
    return 1;
}

int DynamicReturningClauseAST::firstToken() const
{
    return return_token;
}

int ArrayIndexExpressionAST::firstToken() const
{
    return lparen_token;
}

int ArrayIndexExpressionAST::lastToken() const
{
    if ( rparen_token )
        return rparen_token + 1;
    if ( indexExpr )
        return indexExpr->lastToken();
    return lparen_token + 1;
}

int DynamicReturningClauseAST::lastToken() const
{
    if ( intoClause )
        return intoClause->lastToken();
    return return_token + 1;
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

int NotConditionExpressionAST::firstToken() const
{
    if ( not_token )
        return not_token;
    if ( condition )
        return condition->firstToken();
    return 0;
}

int NotConditionExpressionAST::lastToken() const
{
    if ( condition )
        return condition->lastToken();
    if ( not_token )
        return not_token + 1;
    return 1;
}

int TrueConditionExpressionAST::firstToken() const
{
    return true_token;
}

int TrueConditionExpressionAST::lastToken() const
{
    return true_token + 1;
}

int FalseConditionExpressionAST::firstToken() const
{
    return false_token;
}

int FalseConditionExpressionAST::lastToken() const
{
    return false_token + 1;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SqlCheckNamesWalker::walk( Sql::BaseWalkReflector *reflector) {
    if ( m_ast ) {
        SqlCheckNamesVisitor visitor(m_translationUnit, reflector);
        m_ast->accept(&visitor);
    }
}
