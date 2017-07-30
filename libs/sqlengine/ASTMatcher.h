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
#ifndef ASTMATCHER_H

#define ASTMATCHER_H

#include "ASTfwd.h"

namespace Sql {

class SQL_EXPORT ASTMatcher
{
public:
    ASTMatcher();
    virtual ~ASTMatcher();

    virtual bool match( TranslationUnitAST *node, TranslationUnitAST *pattern );
    virtual bool match( SelectStatementAST *node, SelectStatementAST *pattern );
    virtual bool match( InvalidStatementAST *node, InvalidStatementAST *pattern );
    virtual bool match( UnknownStatementAST *node, UnknownStatementAST *pattern );
    virtual bool match( SelectCompoundUnionAST *node, SelectCompoundUnionAST *pattern );
    virtual bool match( SelectCompoundIntersectAST *node, SelectCompoundIntersectAST *pattern );
    virtual bool match( SelectCompoundExceptAST *node, SelectCompoundExceptAST *pattern );
    virtual bool match( SelectCompoundMinusAST *node, SelectCompoundMinusAST *pattern );
    virtual bool match( SelectCoreAST *node, SelectCoreAST *pattern );
    virtual bool match( OrderByBlockAST *node, OrderByBlockAST *pattern );
    virtual bool match( LimitToBlockAST *node, LimitToBlockAST *pattern );
    virtual bool match( OrderingTermAST *node, OrderingTermAST *pattern );
    virtual bool match( SchemaNameAST *node, SchemaNameAST *pattern );
    virtual bool match( TableNameAST *node, TableNameAST *pattern );
    virtual bool match( TableAliasAST *node, TableAliasAST *pattern );
    virtual bool match( ColumnNameAST *node, ColumnNameAST *pattern );
    virtual bool match( ParamNameAST *node, ParamNameAST *pattern );
    virtual bool match( LabelNameAST *node, LabelNameAST *pattern );
    virtual bool match( ColumnAliasAST *node, ColumnAliasAST *pattern );
    virtual bool match( FunctionNameAST *node, FunctionNameAST *pattern );
    virtual bool match( ViewNameAST *node, ViewNameAST *pattern );
    virtual bool match( IndexNameAST *node, IndexNameAST *pattern );
    virtual bool match( TriggerNameAST *node, TriggerNameAST *pattern );
    virtual bool match( ConstraintNameAST *node, ConstraintNameAST *pattern );
    virtual bool match( SchemaTableNameAST *node, SchemaTableNameAST *pattern );
    virtual bool match( SchemaIndexNameAST *node, SchemaIndexNameAST *pattern );
    virtual bool match( SchemaTriggerNameAST *node, SchemaTriggerNameAST *pattern );
    virtual bool match( StarResultColumnAST *node, StarResultColumnAST *pattern );
    virtual bool match( ExprResultColumnAST *node, ExprResultColumnAST *pattern );
    virtual bool match( SubQueryAST *node, SubQueryAST *pattern );
    virtual bool match( DirectSingleSourceAST *node, DirectSingleSourceAST *pattern );
    virtual bool match( SubQuerySingleSourceAST *node, SubQuerySingleSourceAST *pattern );
    virtual bool match( JoinOpAST *node, JoinOpAST *pattern );
    virtual bool match( JoinConstraintAST *node, JoinConstraintAST *pattern );
    virtual bool match( JoinItemAST *node, JoinItemAST *pattern );
    virtual bool match( JoinSourceAST *node, JoinSourceAST *pattern );
    virtual bool match( GroupByBlockAST *node, GroupByBlockAST *pattern );
    virtual bool match( DeleteStatementAST *node, DeleteStatementAST *pattern );
    virtual bool match( InsertStatementAST *node, InsertStatementAST *pattern );
    virtual bool match( InsertClauseAsValuesAST *node, InsertClauseAsValuesAST *pattern );
    virtual bool match( InsertClauseAsSelectAST *node, InsertClauseAsSelectAST *pattern );
    virtual bool match( UpdateStatementAST *node, UpdateStatementAST *pattern );
    virtual bool match( CharacterTypeAST *node, CharacterTypeAST *pattern );
    virtual bool match( NumberTypeAST *node, NumberTypeAST *pattern );
    virtual bool match( SimpleTypeAST *node, SimpleTypeAST *pattern );
    virtual bool match( CreateTableStatementAST *node, CreateTableStatementAST *pattern );
    virtual bool match( CreateTableDirectClauseAST *node, CreateTableDirectClauseAST *pattern );
    virtual bool match( CreateTableAsSelectClauseAST *node, CreateTableAsSelectClauseAST *pattern );
    virtual bool match( CreateTriggerRefClauseAST *node, CreateTriggerRefClauseAST *pattern );
    virtual bool match( CreateRdbTriggerBodyAST *node, CreateRdbTriggerBodyAST *pattern );
    virtual bool match( CreateRdbTriggerBodyItemAST *node, CreateRdbTriggerBodyItemAST *pattern );
    virtual bool match( CreateViewStatementAST *node, CreateViewStatementAST *pattern );
    virtual bool match( CreateIndexStatementAST *node, CreateIndexStatementAST *pattern );
    virtual bool match( CreateTriggerStatementAST *node, CreateTriggerStatementAST *pattern );
    virtual bool match( ColumnDefinitionAST *node, ColumnDefinitionAST *pattern );
    virtual bool match( UniqueOrPkTableConstraintAST *node, UniqueOrPkTableConstraintAST *pattern );
    virtual bool match( CheckTableConstraintAST *node, CheckTableConstraintAST *pattern );
    virtual bool match( FkTableConstraintAST *node, FkTableConstraintAST *pattern );
    virtual bool match( FkReferencesClauseAST *node, FkReferencesClauseAST *pattern );
    virtual bool match( PkColumnConstraintAST *node, PkColumnConstraintAST *pattern );
    virtual bool match( NotNullColumnConstraintAST *node, NotNullColumnConstraintAST *pattern );
    virtual bool match( UniqueColumnConstraintAST *node, UniqueColumnConstraintAST *pattern );
    virtual bool match( CheckColumnConstraintAST *node, CheckColumnConstraintAST *pattern );
    virtual bool match( ReferencesColumnConstraintAST *node, ReferencesColumnConstraintAST *pattern );
    virtual bool match( SortedColumnNameAST *node, SortedColumnNameAST *pattern );
    virtual bool match( DropTableStatementAST *node, DropTableStatementAST *pattern );
    virtual bool match( DropViewStatementAST *node, DropViewStatementAST *pattern );
    virtual bool match( DropIndexStatementAST *node, DropIndexStatementAST *pattern );
    virtual bool match( DropTriggerStatementAST *node, DropTriggerStatementAST *pattern );
    virtual bool match( TruncateStatementAST *node, TruncateStatementAST *pattern );
    virtual bool match( AlterTableStatementAST *node, AlterTableStatementAST *pattern );
    virtual bool match( AlterTableDropClauseAST *node, AlterTableDropClauseAST *pattern );
    virtual bool match( AlterTableAddConstraintClauseAST *node, AlterTableAddConstraintClauseAST *pattern );
    virtual bool match( AlterTableAddColumnClauseAST *node, AlterTableAddColumnClauseAST *pattern );
    virtual bool match( CommentOnTableStatementAST *node, CommentOnTableStatementAST *pattern );
    virtual bool match( CommentOnColumnStatementAST *node, CommentOnColumnStatementAST *pattern );
    virtual bool match( CommitStatementAST *node, CommitStatementAST *pattern );
    virtual bool match( RollbackStatementAST *node, RollbackStatementAST *pattern );
    virtual bool match( CallStatementAST *node, CallStatementAST *pattern );

    // Expressions
    virtual bool match( CompoundValueExpressionAST *node, CompoundValueExpressionAST *pattern );
    virtual bool match( UnaryValueExpressionAST *node, UnaryValueExpressionAST *pattern );
    virtual bool match( BinaryValueExpressionAST *node, BinaryValueExpressionAST *pattern );
    virtual bool match( ColumnExpressionAST *node, ColumnExpressionAST *pattern );
    virtual bool match( LiteralExpressionAST *node, LiteralExpressionAST *pattern );
    virtual bool match( NumericExpressionAST *node, NumericExpressionAST *pattern );
    virtual bool match( FunctionExpressionAST *node, FunctionExpressionAST *pattern );
    virtual bool match( CastFunctionAST *node, CastFunctionAST *pattern );
    virtual bool match( SubstringFunctionAST *node, SubstringFunctionAST *pattern );
    virtual bool match( CountFunctionAST *node, CountFunctionAST *pattern );
    virtual bool match( ParameterExpressionAST *node, ParameterExpressionAST *pattern );
    virtual bool match( NullValueExpressionAST *node, NullValueExpressionAST *pattern );
    virtual bool match( SelectCompoundValueExpressionAST *node, SelectCompoundValueExpressionAST *pattern );
    virtual bool match( ValueListExpressionAST *node, ValueListExpressionAST *pattern );
    virtual bool match( SubQueryExpressionAST *node, SubQueryExpressionAST *pattern );
    virtual bool match( CompoundConditionExpressionAST *node, CompoundConditionExpressionAST *pattern );
    virtual bool match( InConditionExpressionAST *node, InConditionExpressionAST *pattern );
    virtual bool match( LikeConditionExpressionAST *node, LikeConditionExpressionAST *pattern );
    virtual bool match( BetweenConditionExpressionAST *node, BetweenConditionExpressionAST *pattern );
    virtual bool match( ExistsConditionExpressionAST *node, ExistsConditionExpressionAST *pattern );
    virtual bool match( UnaryConditionExpressionAST *node, UnaryConditionExpressionAST *pattern );
    virtual bool match( BinaryConditionExpressionAST *node, BinaryConditionExpressionAST *pattern );
    virtual bool match( AssignmentExpressionAST *node, AssignmentExpressionAST *pattern );
    virtual bool match( CaseExpressionAST *node, CaseExpressionAST *pattern );
    virtual bool match( CaseExpressionClauseAST *node, CaseExpressionClauseAST *pattern );
    virtual bool match( CaseConditionAST *node, CaseConditionAST *pattern );
    virtual bool match( CaseConditionClauseAST *node, CaseConditionClauseAST *pattern );
};

} // namespace CPlusPlus

#endif // ASTMATCHER_H
