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

//
//  W A R N I N G
//  -------------
//
// This file is automatically generated.
// Changes will be lost.
//


#include "RdbAST.h"
#include "RdbASTMatcher.h"

using namespace RDB;

bool TranslationUnitAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( TranslationUnitAST *_other = pattern->asTranslationUnit() )
        return matcher->match(this, _other);

    return false;
}

bool SelectStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectStatementAST *_other = pattern->asSelectStatement() )
        return matcher->match(this, _other);

    return false;
}

bool SelectCoreAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectCoreAST *_other = pattern->asSelectCore() )
        return matcher->match(this, _other);

    return false;
}

bool InvalidStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( InvalidStatementAST *_other = pattern->asInvalidStatement() )
        return matcher->match(this, _other);

    return false;
}

bool UnknownStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( UnknownStatementAST *_other = pattern->asUnknownStatement() )
        return matcher->match(this, _other);

    return false;
}

bool SelectCompoundUnionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectCompoundUnionAST *_other = pattern->asSelectCompoundUnion() )
        return matcher->match(this, _other);

    return false;
}

bool SelectCompoundIntersectAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectCompoundIntersectAST *_other = pattern->asSelectCompoundIntersect() )
        return matcher->match(this, _other);

    return false;
}

bool SelectCompoundExceptAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectCompoundExceptAST *_other = pattern->asSelectCompoundExcept() )
        return matcher->match(this, _other);

    return false;
}

bool SelectCompoundMinusAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectCompoundMinusAST *_other = pattern->asSelectCompoundMinus() )
        return matcher->match(this, _other);

    return false;
}

bool OrderByBlockAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( OrderByBlockAST *_other = pattern->asOrderByBlock() )
        return matcher->match(this, _other);

    return false;
}

bool LimitToBlockAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( LimitToBlockAST *_other = pattern->asLimitToBlock() )
        return matcher->match(this, _other);

    return false;
}

bool OrderingTermAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( OrderingTermAST *_other = pattern->asOrderingTerm() )
        return matcher->match(this, _other);

    return false;
}

bool SchemaNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SchemaNameAST *_other = pattern->asSchemaName() )
        return matcher->match(this, _other);

    return false;
}

bool TableNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( TableNameAST *_other = pattern->asTableName() )
        return matcher->match(this, _other);

    return false;
}

bool ViewNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ViewNameAST *_other = pattern->asViewName() )
        return matcher->match(this, _other);

    return false;
}

bool IndexNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IndexNameAST *_other = pattern->asIndexName() )
        return matcher->match(this, _other);

    return false;
}

bool TriggerNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( TriggerNameAST *_other = pattern->asTriggerName() )
        return matcher->match(this, _other);

    return false;
}

bool ConstraintNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ConstraintNameAST *_other = pattern->asConstraintName() )
        return matcher->match(this, _other);

    return false;
}

bool CursorNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CursorNameAST *_other = pattern->asCursorName() )
        return matcher->match(this, _other);

    return false;
}

bool StorageMapNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( StorageMapNameAST *_other = pattern->asStorageMapName() )
        return matcher->match(this, _other);

    return false;
}

bool AreaNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( AreaNameAST *_other = pattern->asAreaName() )
        return matcher->match(this, _other);

    return false;
}

bool SchemaTableNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SchemaTableNameAST *_other = pattern->asSchemaTableName() )
        return matcher->match(this, _other);

    return false;
}

bool SchemaIndexNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SchemaIndexNameAST *_other = pattern->asSchemaIndexName() )
        return matcher->match(this, _other);

    return false;
}

bool SchemaTriggerNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SchemaTriggerNameAST *_other = pattern->asSchemaTriggerName() )
        return matcher->match(this, _other);

    return false;
}

bool TableAliasAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( TableAliasAST *_other = pattern->asTableAlias() )
        return matcher->match(this, _other);

    return false;
}

bool ColumnNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ColumnNameAST *_other = pattern->asColumnName() )
        return matcher->match(this, _other);

    return false;
}

bool IdentifierNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IdentifierNameAST *_other = pattern->asIdentifierName() )
        return matcher->match(this, _other);

    return false;
}

bool ParamNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ParamNameAST *_other = pattern->asParamName() )
        return matcher->match(this, _other);

    return false;
}

bool LabelNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( LabelNameAST *_other = pattern->asLabelName() )
        return matcher->match(this, _other);

    return false;
}

bool ColumnAliasAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ColumnAliasAST *_other = pattern->asColumnAlias() )
        return matcher->match(this, _other);

    return false;
}

bool ModuleNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ModuleNameAST *_other = pattern->asModuleName() )
        return matcher->match(this, _other);

    return false;
}

bool RoutineNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( RoutineNameAST *_other = pattern->asFunctionName() )
        return matcher->match(this, _other);

    return false;
}

bool StarResultColumnAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( StarResultColumnAST *_other = pattern->asStarResultColumn() )
        return matcher->match(this, _other);

    return false;
}

bool ExprResultColumnAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ExprResultColumnAST *_other = pattern->asExprResultColumn() )
        return matcher->match(this, _other);

    return false;
}

bool SubQueryAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SubQueryAST *_other = pattern->asSubQuery() )
        return matcher->match(this, _other);

    return false;
}

bool DirectSingleSourceAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DirectSingleSourceAST *_other = pattern->asDirectSingleSource() )
        return matcher->match(this, _other);

    return false;
}

bool SubQuerySingleSourceAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SubQuerySingleSourceAST *_other = pattern->asSubQuerySingleSource() )
        return matcher->match(this, _other);

    return false;
}

bool JoinOpAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( JoinOpAST *_other = pattern->asJoinOp() )
        return matcher->match(this, _other);

    return false;
}

bool JoinConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( JoinConstraintAST *_other = pattern->asJoinConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool JoinItemAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( JoinItemAST *_other = pattern->asJoinItem() )
        return matcher->match(this, _other);

    return false;
}

bool JoinSourceAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( JoinSourceAST *_other = pattern->asJoinSource() )
        return matcher->match(this, _other);

    return false;
}

bool GroupByBlockAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( GroupByBlockAST *_other = pattern->asGroupByBlock() )
        return matcher->match(this, _other);

    return false;
}

bool DeleteStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DeleteStatementAST *_other = pattern->asDeleteStatement() )
        return matcher->match(this, _other);

    return false;
}

bool InsertStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( InsertStatementAST *_other = pattern->asInsertStatement() )
        return matcher->match(this, _other);

    return false;
}

bool UpdateStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( UpdateStatementAST *_other = pattern->asUpdateStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CreateTableDirectClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateTableDirectClauseAST *_other = pattern->asCreateTableDirectClause() )
        return matcher->match(this, _other);

    return false;
}

bool CreateTableAsSelectClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateTableAsSelectClauseAST *_other = pattern->asCreateTableAsSelectClause() )
        return matcher->match(this, _other);

    return false;
}

bool CreateTriggerRefClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateTriggerRefClauseAST *_other = pattern->asCreateTriggerRefClause() )
        return matcher->match(this, _other);

    return false;
}

bool CreateRdbTriggerBodyAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateRdbTriggerBodyAST *_other = pattern->asCreateRdbTriggerBody() )
        return matcher->match(this, _other);

    return false;
}

bool CreateRdbTriggerBodyItemAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateRdbTriggerBodyItemAST *_other = pattern->asCreateRdbTriggerBodyItem() )
        return matcher->match(this, _other);

    return false;
}

bool ColumnDefinitionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ColumnDefinitionAST *_other = pattern->asColumnDefinition() )
        return matcher->match(this, _other);

    return false;
}

bool UniqueOrPkTableConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( UniqueOrPkTableConstraintAST *_other = pattern->asUniqueOrPkTableConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool CheckTableConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CheckTableConstraintAST *_other = pattern->asCheckTableConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool FkTableConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( FkTableConstraintAST *_other = pattern->asFkTableConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool FkReferencesClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( FkReferencesClauseAST *_other = pattern->asForeignKeyClause() )
        return matcher->match(this, _other);

    return false;
}

bool PkColumnConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( PkColumnConstraintAST *_other = pattern->asPkColumnConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool NotNullColumnConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( NotNullColumnConstraintAST *_other = pattern->asNotNullColumnConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool UniqueColumnConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( UniqueColumnConstraintAST *_other = pattern->asUniqueColumnConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool CheckColumnConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CheckColumnConstraintAST *_other = pattern->asCheckColumnConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool ReferencesColumnConstraintAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ReferencesColumnConstraintAST *_other = pattern->asReferencesColumnConstraint() )
        return matcher->match(this, _other);

    return false;
}

bool SortedColumnNameAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SortedColumnNameAST *_other = pattern->asSortedColumnName() )
        return matcher->match(this, _other);

    return false;
}

bool InsertClauseAsValuesAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( InsertClauseAsValuesAST *_other = pattern->asInsertClauseAsValues() )
        return matcher->match(this, _other);

    return false;
}

bool InsertClauseAsSelectAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( InsertClauseAsSelectAST *_other = pattern->asInsertClauseAsSelect() )
        return matcher->match(this, _other);

    return false;
}

bool CreateTableStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateTableStatementAST *_other = pattern->asCreateTableStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CreateViewStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateViewStatementAST *_other = pattern->asCreateViewStatement() )
        return matcher->match(this, _other);

    return false;
}

bool IndexTypeAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IndexTypeAST *_other = pattern->asIndexType() )
        return matcher->match(this, _other);

    return false;
}

bool IndexNodeSizeAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IndexNodeSizeAST *_other = pattern->asIndexNodeSize() )
        return matcher->match(this, _other);

    return false;
}

bool IndexStoreInAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IndexStoreInAST *_other = pattern->asIndexStoreIn() )
        return matcher->match(this, _other);

    return false;
}

bool IndexOptionsAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IndexOptionsAST *_other = pattern->asIndexOptions() )
        return matcher->match(this, _other);

    return false;
}

bool CreateIndexStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateIndexStatementAST *_other = pattern->asCreateIndexStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CreateTriggerStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateTriggerStatementAST *_other = pattern->asCreateTriggerStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CreateStorageMapStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateStorageMapStatementAST *_other = pattern->asCreateStorageMapStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CreateModuleStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateModuleStatementAST *_other = pattern->asCreateModuleStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CreateRoutineStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CreateRoutineStatementAST *_other = pattern->asCreateRoutineStatement() )
        return matcher->match(this, _other);

    return false;
}

bool RoutineAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( RoutineAST *_other = pattern->asRoutine() )
        return matcher->match(this, _other);

    return false;
}

bool DropTableStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DropTableStatementAST *_other = pattern->asDropTableStatement() )
        return matcher->match(this, _other);

    return false;
}

bool DropViewStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DropViewStatementAST *_other = pattern->asDropViewStatement() )
        return matcher->match(this, _other);

    return false;
}

bool DropIndexStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DropIndexStatementAST *_other = pattern->asDropIndexStatement() )
        return matcher->match(this, _other);

    return false;
}

bool DropTriggerStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DropTriggerStatementAST *_other = pattern->asDropTriggerStatement() )
        return matcher->match(this, _other);

    return false;
}

bool DropModuleStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DropModuleStatementAST *_other = pattern->asDropModuleStatement() )
        return matcher->match(this, _other);

    return false;
}

bool TruncateStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( TruncateStatementAST *_other = pattern->asTruncateStatement() )
        return matcher->match(this, _other);

    return false;
}

bool AlterTableStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( AlterTableStatementAST *_other = pattern->asAlterTableStatement() )
        return matcher->match(this, _other);

    return false;
}

bool AlterTableDropClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( AlterTableDropClauseAST *_other = pattern->asAlterTableDropClause() )
        return matcher->match(this, _other);

    return false;
}

bool AlterTableAddConstraintClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( AlterTableAddConstraintClauseAST *_other = pattern->asAlterTableAddConstraintClause() )
        return matcher->match(this, _other);

    return false;
}

bool AlterTableAddColumnClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( AlterTableAddColumnClauseAST *_other = pattern->asAlterTableAddColumnClause() )
        return matcher->match(this, _other);

    return false;
}

bool CommentOnTableStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CommentOnTableStatementAST *_other = pattern->asCommentOnTableStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CommentOnColumnStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CommentOnColumnStatementAST *_other = pattern->asCommentOnColumnStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CommitStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CommitStatementAST *_other = pattern->asCommitStatement() )
        return matcher->match(this, _other);

    return false;
}

bool RollbackStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( RollbackStatementAST *_other = pattern->asRollbackStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CallStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CallStatementAST *_other = pattern->asCallStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockStatementAST *_other = pattern->asBlockStatement() )
        return matcher->match(this, _other);

    return false;
}

bool CharacterTypeAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CharacterTypeAST *_other = pattern->asCharacterType() )
        return matcher->match(this, _other);

    return false;
}

bool NumberTypeAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( NumberTypeAST *_other = pattern->asNumericType() )
        return matcher->match(this, _other);

    return false;
}

bool SimpleTypeAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SimpleTypeAST *_other = pattern->asSimpleType() )
        return matcher->match(this, _other);

    return false;
}

bool CompoundValueExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CompoundValueExpressionAST *_other = pattern->asCompoundValueExpression() )
        return matcher->match(this, _other);

    return false;
}

bool UnaryValueExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( UnaryValueExpressionAST *_other = pattern->asUnaryValueExpression() )
        return matcher->match(this, _other);

    return false;
}

bool BinaryValueExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BinaryValueExpressionAST *_other = pattern->asBinaryValueExpression() )
        return matcher->match(this, _other);

    return false;
}

bool ColumnExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ColumnExpressionAST *_other = pattern->asColumnExpression() )
        return matcher->match(this, _other);

    return false;
}

bool LiteralExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( LiteralExpressionAST *_other = pattern->asLiteralExpression() )
        return matcher->match(this, _other);

    return false;
}

bool NumericExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( NumericExpressionAST *_other = pattern->asNumericExpression() )
        return matcher->match(this, _other);

    return false;
}

bool FunctionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( FunctionExpressionAST *_other = pattern->asFunctionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool CastFunctionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CastFunctionAST *_other = pattern->asCastFunction() )
        return matcher->match(this, _other);

    return false;
}

bool SubstringFunctionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SubstringFunctionAST *_other = pattern->asSubstringFunction() )
        return matcher->match(this, _other);

    return false;
}

bool CountFunctionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CountFunctionAST *_other = pattern->asCountFunction() )
        return matcher->match(this, _other);

    return false;
}

bool ParameterExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ParameterExpressionAST *_other = pattern->asParameterExpression() )
        return matcher->match(this, _other);

    return false;
}

bool IdentifierExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IdentifierExpressionAST *_other = pattern->asIdentifierExpression() )
        return matcher->match(this, _other);

    return false;
}

bool IdentDotColumnExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IdentDotColumnExpressionAST *_other = pattern->asIdentDotColumnExpression() )
        return matcher->match(this, _other);

    return false;
}

bool NullValueExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( NullValueExpressionAST *_other = pattern->asNullValueExpression() )
        return matcher->match(this, _other);

    return false;
}

bool SelectCompoundValueExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SelectCompoundValueExpressionAST *_other = pattern->asSelectCompoundValueExpression() )
        return matcher->match(this, _other);

    return false;
}

bool ValueListExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ValueListExpressionAST *_other = pattern->asValueListExpression() )
        return matcher->match(this, _other);

    return false;
}

bool SubQueryExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( SubQueryExpressionAST *_other = pattern->asSubQueryExpression() )
        return matcher->match(this, _other);

    return false;
}

bool CompoundConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CompoundConditionExpressionAST *_other = pattern->asCompoundConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool InConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( InConditionExpressionAST *_other = pattern->asInConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool LikeConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( LikeConditionExpressionAST *_other = pattern->asLikeConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool BetweenConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BetweenConditionExpressionAST *_other = pattern->asBetweenConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool ExistsConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ExistsConditionExpressionAST *_other = pattern->asExistsConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool UnaryConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( UnaryConditionExpressionAST *_other = pattern->asUnaryConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool BinaryConditionExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BinaryConditionExpressionAST *_other = pattern->asBinaryConditionExpression() )
        return matcher->match(this, _other);

    return false;
}

bool AssignmentExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( AssignmentExpressionAST *_other = pattern->asAssignmentExpression() )
        return matcher->match(this, _other);

    return false;
}

bool CaseExpressionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CaseExpressionAST *_other = pattern->asCaseExpression() )
        return matcher->match(this, _other);

    return false;
}

bool CaseExpressionClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CaseExpressionClauseAST *_other = pattern->asCaseExpressionClause() )
        return matcher->match(this, _other);

    return false;
}

bool CaseConditionAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CaseConditionAST *_other = pattern->asCaseCondition() )
        return matcher->match(this, _other);

    return false;
}

bool CaseConditionClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( CaseConditionClauseAST *_other = pattern->asCaseConditionClause() )
        return matcher->match(this, _other);

    return false;
}

bool DeclareAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DeclareAST *_other = pattern->asDeclare() )
        return matcher->match(this, _other);

    return false;
}

bool ArgumentAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ArgumentAST *_other = pattern->asArgument() )
        return matcher->match(this, _other);

    return false;
}

bool BlockInsertStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockInsertStatementAST *_other = pattern->asBlockInsertStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockUpdateStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockUpdateStatementAST *_other = pattern->asBlockUpdateStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockDeleteStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockDeleteStatementAST *_other = pattern->asBlockDeleteStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockSelectStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockSelectStatementAST *_other = pattern->asBlockSelectStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockSetStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockSetStatementAST *_other = pattern->asBlockSetStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockCommitStatmentAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockCommitStatmentAST *_other = pattern->asBlockCommitStatment() )
        return matcher->match(this, _other);

    return false;
}

bool BlockRollbackStatmentAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockRollbackStatmentAST *_other = pattern->asBlockRollbackStatment() )
        return matcher->match(this, _other);

    return false;
}

bool BlockCallStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockCallStatementAST *_other = pattern->asBlockCallStatement() )
        return matcher->match(this, _other);

    return false;
}

bool IfStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( IfStatementAST *_other = pattern->asIfStatement() )
        return matcher->match(this, _other);

    return false;
}

bool LoopStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( LoopStatementAST *_other = pattern->asLoopStatement() )
        return matcher->match(this, _other);

    return false;
}

bool DeclareCursorClauseAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( DeclareCursorClauseAST *_other = pattern->asDeclareCursorClause() )
        return matcher->match(this, _other);

    return false;
}

bool ForStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( ForStatementAST *_other = pattern->asForStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockLabelAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockLabelAST *_other = pattern->asBlockLabel() )
        return matcher->match(this, _other);

    return false;
}

bool LeaveStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( LeaveStatementAST *_other = pattern->asLeaveStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockBlockStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockBlockStatementAST *_other = pattern->asBlockBlockStatement() )
        return matcher->match(this, _other);

    return false;
}

bool BlockReturnStatementAST::match0( AST *pattern, ASTMatcher *matcher )
{
    if ( BlockReturnStatementAST *_other = pattern->asBlockReturnStatement() )
        return matcher->match(this, _other);

    return false;
}
