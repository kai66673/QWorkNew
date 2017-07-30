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

#ifndef RDB_ASTFWD_H
#define RDB_ASTFWD_H

#include "sqlengine_global.h"

namespace RDB {

template <typename _Tp> class List;

class AST;
class ASTVisitor;
class ASTMatcher;

class SqlCheckNamesVisitor;
class ISemanticParser;
class Parser;

// Root Tree Node
class TranslationUnitAST;
    // List<StatementAST>

class StatementAST; // Pure... One of
    class InvalidStatementAST;
    class UnknownStatementAST;
    class DmlStatementAST;
        class SelectStatementAST;
        class UpdateStatementAST;
        class InsertStatementAST;
        class DeleteStatementAST;
    class DdlStatementAST;
        class CreateStatementAST; // Pure... One of
            class CreateTableStatementAST;
            class CreateViewStatementAST;
            class CreateIndexStatementAST;
            class CreateTriggerStatementAST;
            class CreateStorageMapStatementAST;
            class CreateModuleStatementAST;
            class CreateRoutineStatementAST;
                class RoutineAST;
        class DropStatementAST; // Pure... One of
            class DropTableStatementAST;
            class DropViewStatementAST;
            class DropIndexStatementAST;
            class DropTriggerStatementAST;
            class DropModuleStatementAST;
        class TruncateStatementAST;
        class AlterStatementAST;
            class AlterTableStatementAST;
    class CommentStatementAST; // Pure... One of
        class CommentOnTableStatementAST;
        class CommentOnColumnStatementAST;
    class CommitStatementAST;
    class RollbackStatementAST;
    class CallStatementAST;

    class BlockStatementAST;
        class ProcedureStatementAST;
        class FunctionStatementAST;

// SelectStatementAST
class SelectCoreCompoundAST;
class SelectCoreAST;
class SelectCompoundAST; // Pure... One of
    class SelectCompoundUnionAST;
    class SelectCompoundIntersectAST;
    class SelectCompoundExceptAST;
    class SelectCompoundMinusAST;
class OrderByBlockAST;
class LimitToBlockAST;
class OrderingTermAST;

// InsertStatementAST
class InsertClauseAST; // Pure... One of
    class InsertClauseAsValuesAST;
    class InsertClauseAsSelectAST;

// CreateTableStatementAST
class CreateTableClauseAST; // Pure... One of
    class CreateTableDirectClauseAST;
    class CreateTableAsSelectClauseAST;

// CreateTriggerStatementAST
class CreateTriggerRefClauseAST;
class CreateTriggerBodyAST;
    class CreateRdbTriggerBodyAST;
    class CreateRdbTriggerBodyItemAST;

// CreateTableDirectClauseAST
class ColumnDefinitionAST;
class ConstraintAST;
    class TableConstraintAST;
        class UniqueOrPkTableConstraintAST;
        class CheckTableConstraintAST;
        class FkTableConstraintAST;
    class ColumnConstraintAST;
        class PkColumnConstraintAST;
        class NotNullColumnConstraintAST;
        class UniqueColumnConstraintAST;
        class CheckColumnConstraintAST;
        class ReferencesColumnConstraintAST;

// AlterTableStatementAST
class AlterTableClauseAST;
    class AlterTableDropClauseAST;
    class AlterTableAddClauseAST;
        class AlterTableAddConstraintClauseAST;
        class AlterTableAddColumnClauseAST;
    class AlterTableEnableOrDisableClauseAST;
    class AlterTableModifyeClauseAST;

// CreateIndexStatementAST
class IndexOptionsAST;
    class IndexTypeAST;
    class IndexNodeSizeAST;
    class IndexStoreInAST;

class SortedColumnNameAST;
class FkReferencesClauseAST;

class NameAST;
    class ColumnAliasAST;
    class SchemaNameAST;
    class TableNameAST;
    class TableAliasAST;
    class ColumnNameAST;
    class IdentifierNameAST;
    class ParamNameAST;
    class LabelNameAST;
    class ModuleNameAST;
    class RoutineNameAST;
    class ViewNameAST;
    class IndexNameAST;
    class TriggerNameAST;
    class ConstraintNameAST;
    class CursorNameAST;
    class StorageMapNameAST;
    class AreaNameAST;

class SchemaTableNameAST;
class SchemaIndexNameAST;
class SchemaTriggerNameAST;

class ResultColumnAST;
class StarResultColumnAST;
class ExprResultColumnAST;
class SubQueryAST;
class SingleSourceAST;
    class DirectSingleSourceAST;
    class SubQuerySingleSourceAST;
class JoinOpAST;
class JoinConstraintAST;
class JoinItemAST;
class JoinSourceAST;
class GroupByBlockAST;

// Type Specifiers
class BaseTypeAST;
    class CharacterTypeAST;
    class NumberTypeAST;
    class SimpleTypeAST;

// Expression
class BaseExpressionAST;
    class ValueExpressionAST;
        class CompoundValueExpressionAST;
        class UnaryValueExpressionAST;
        class BinaryValueExpressionAST;
        class SimpleValueExpressionAST;
            class ConstantExpressionAST;
                class LiteralExpressionAST;
                class NumericExpressionAST;
                class NullValueExpressionAST;
            class ColumnExpressionAST;
            class FunctionExpressionAST;
            class CastFunctionAST;
            class SubstringFunctionAST;
            class CountFunctionAST;
            class ParameterExpressionAST;
            class IdentifierExpressionAST;
            class IdentDotColumnExpressionAST;
            class SelectCompoundValueExpressionAST;
        class CaseExpressionAST;
        class CaseConditionAST;
    class CompoundRValueExpressionAST;
        class ValueListExpressionAST;
        class SubQueryExpressionAST;
    class ConditionExpressionAST;
        class CompoundConditionExpressionAST;
        class InConditionExpressionAST;
        class LikeConditionExpressionAST;
        class BetweenConditionExpressionAST;
        class ExistsConditionExpressionAST;
        class UnaryConditionExpressionAST;
        class BinaryConditionExpressionAST;
    class AssignmentExpressionAST;

class CaseExpressionClauseAST;
class CaseConditionClauseAST;

// BlockStatementAST entry items
class DeclareAST;
class ArgumentAST;
class BlockStatementEntryAST;
class BlockInsertStatementAST;
class BlockUpdateStatementAST;
class BlockDeleteStatementAST;
class BlockSelectStatementAST;
class BlockSetStatementAST;
class BlockCommitStatmentAST;
class BlockRollbackStatmentAST;
class BlockCallStatementAST;
class IfStatementAST;
class LoopStatementAST;
class ForStatementAST;
    class DeclareCursorClauseAST;
class BlockLabelAST;
class LeaveStatementAST;
class BlockBlockStatementAST;
class BlockReturnStatementAST;

typedef List<StatementAST *> StatementListAST;
typedef List<SelectCoreAST *> SelectCoreListAST;
typedef List<SelectCompoundAST *> SelectCompoundListAST;
typedef List<OrderingTermAST *> OrderingTermListAST;
typedef List<ResultColumnAST *> ResultColumnListAST;
typedef List<JoinItemAST *> JoinItemListAST;
typedef List<SubQueryAST *> SubQueryListAST;
typedef List<ColumnNameAST *> ColumnNameListAST;
typedef List<IdentifierNameAST *> IdentifierNameListAST;
typedef List<ParamNameAST *> ParamNameListAST;

typedef List<ValueExpressionAST *> ValueExpressionListAST;
typedef List<SimpleValueExpressionAST *> SimpleValueExpressionListAST;
typedef List<AssignmentExpressionAST *> AssignmentExpressionListAST;
typedef List<CaseExpressionClauseAST *> CaseExpressionClauseListAST;
typedef List<CaseConditionClauseAST *> CaseConditionClauseListAST;

typedef List<ColumnDefinitionAST *> ColumnDefinitionListAST;
typedef List<SortedColumnNameAST *> SortedColumnNameListAST;
typedef List<ColumnConstraintAST *> ColumnConstraintListAST;
typedef List<TableConstraintAST *> TableConstraintListAST;
typedef List<CreateRdbTriggerBodyItemAST *> CreateRdbTriggerBodyListAST;

typedef List<DeclareAST *> DeclareListAST;
typedef List<ArgumentAST *> ArgumentListAST;
typedef List<BlockStatementEntryAST *> BlockStatementEntryListAST;
typedef List<RoutineAST *> RoutineListAST;

} // namespace RDB


#endif // RDB_ASTFWD_H
