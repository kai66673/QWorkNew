#ifndef QLALR_ORACLE_ASTFWD_H
#define QLALR_ORACLE_ASTFWD_H

#include "sqlengine_global.h"

namespace QLalrOracle {

template <typename _Tp> class List;

enum ParseTarget {
    ParseSqlNames,
    ParseCodeAssist
};

class AST;
class ASTVisitor;
class ASTMatcher;

class BaseWalker;
class BaseWalkReflector;
class SqlCheckNamesVisitor;
class ISemanticParser;
class Parser;

class EmptyAST;

// Root Tree Node
class TranslationUnitAST;
    // List<StatementAST>

class StatementAST; // Pure... One of
    class InvalidStatementAST;
    class UnknownStatementAST;
    class DmlStatementAST;
        class SelectStatementAST;
        class UpdateStatementAST;
            class UpdateSetClauseAST;
        class InsertStatementAST;
            class InsertColumnsClauseAST;
        class DeleteStatementAST;
    class DdlStatementAST;
        class CreateStatementAST; // Pure... One of
            class CreateTableStatementAST;
            class CreateViewStatementAST;
            class CreateIndexStatementAST;
                class BaseCreateIndexClauseAST;
                    class TableCreateIndexClauseAST;
            class CreateTriggerStatementAST;
        class DropStatementAST; // Pure... One of
            class DropTableStatementAST;
            class DropViewStatementAST;
            class DropIndexStatementAST;
            class DropTriggerStatementAST;
        class TruncateStatementAST;
        class AlterStatementAST;
            class AlterTableStatementAST;
    class CommentStatementAST; // Pure... One of
        class CommentOnTableStatementAST;
        class CommentOnColumnStatementAST;
    class CommitStatementAST;
    class RollbackStatementAST;
    class CallStatementAST;

// SelectStatementAST
class SelectCoreAST;
class OrderByBlockAST;
class ForUpdateClauseAST;
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
class ConstraintStateAST;   // Contains ...
    class ConstraintDeferStateAST;
    class UsingIndexClauseAST;
    class ExceptionsClauseAST;

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


class SortedColumnNameAST;
class FkReferencesClauseAST;

class LabelAST;

class NameAST;
    // PL-SQL (enum)
    class PlSqlBaseNameAST; // Pure... One of...
        class PlSqlNameAST;
        class PlSqlTypeNameAST;

class BaseSelectColumnsAST;
    class StarSelectColumnAST;
    class ListSelectColumnAST;
        class BaseSelectColumnItemAST;
            class TableSelectColumnsItemAST;
            class ValueSelectColumnItemAST;

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
class BaseJoinListAST;
    class CompoundJoinListAST;
    class CommaJoinListAST;
    class JoinJoinListAST;
class JoinConditionAST;
class JoinSourceAST;
class GroupByBlockAST;
class WhereClauseAST;
class BaseConnectPriorClauseAST;
    class StartConnectPriorAST;
    class EndConnectPriorAST;
class StartWithClauseAST;
class ConnectByClauseAST;
class SetClauseAST;
    class BaseSetSpecAST;
        class MinusSetSpecAST;
        class IntersectSetSpecAST;
        class UnionSetSpecAST;

// Type Specifiers
class BaseTypeAST;
    class CharacterTypeAST;
    class NumberTypeAST;
    class SimpleTypeAST;

// Expression
class BaseExpressionAST;
    class ValueExpressionAST;
        class CompoundValueExpressionAST;
        class FunctionCallAST;
        class CountStarCallAST;
        class CountExprCallAST;
        class BuiltinFunctionCallAST;
        class UnaryValueExpressionAST;
        class BinaryValueExpressionAST;
        class SimpleValueExpressionAST;
            class ConstantExpressionAST;
                class LiteralExpressionAST;
                class NumericExpressionAST;
                class NullValueExpressionAST;
            class ColumnIdentifierAST;
            class FunctionExpressionAST;
            class CastFunctionAST;
            class SubstringFunctionAST;
            class ParameterExpressionAST;
            class SelectCompoundValueExpressionAST;
        class CaseExpressionAST;
        class CaseConditionAST;
    class CompoundRValueExpressionAST;
        class ValueListExpressionAST;
        class SubQueryExpressionAST;
    class ConditionExpressionAST;
        class NotConditionExpressionAST;
        class TrueConditionExpressionAST;
        class FalseConditionExpressionAST;
        class CompoundConditionExpressionAST;
        class CompareConditionExpressionAST;
        class InConditionExpressionAST;
        class LikeConditionExpressionAST;
        class BetweenConditionExpressionAST;
        class ExistsConditionExpressionAST;
        class IsNullConditionExpressionAST;
        class BinaryConditionExpressionAST;
        class ConditionAsValueExpressionAST;
    class AssignmentExpressionAST;

class CaseExpressionClauseAST;
class CaseConditionClauseAST;

// PL-SQL
class PlSqlBlockAST;    // Aggregate DeclareSectionAST, PlSqlBlockBodyAST
    class DeclareSectionAST;   // Aggregate List of DeclarationAST
        class DeclarationAST;   // Pure... one of...
            class CursorDeclarationAST;
            class VariableDeclarationAST;
            class ExceptionDeclarationAST;
            class SimpleDeclarationAST;
            class FunctionDeclarationAST;
            class FunctionDefintionAST;
            class ProcedureDeclarationAST;
            class ProcedureDefinitionAST;
            class TypeDefinitionDeclarationAST; // Aggregate TypeDefinitionAST
                class TypeDefinitionAST;    // Pure... One of...
                    class RecordTypeDefinitionAST;
                    class RefCursorTypeDefinitionAST;
                    class SubtypeDefinitionAST;
                    class CollectionTypeDefinitionAST; // Aggreagete CollectionTypeAST
                        class CollectionTypeAST;    // Pure... One of...
                            class TableCollectionTypeAST;
                            class ArrayCollectionTypeAST;
    class PlSqlBlockBodyAST;    // Aggregate List of PlSqlStatementAST
        class PlSqlStatementAST;    // Aggregate BasePlSqlStatementAST
            class BasePlSqlStatementAST;    // Pure... One of...
                class BlockSqlStatementAST; // Pure... One of...
                    class BlockCommitStatementAST;
                    class BlockDeleteStatementAST;
                    class BlockInsertStatementAST;
                    class BlockLockTableStatementAST;
                    class BlockRollbackStatementAST;   // Aggregate RollbackClauseAST
                        class RollbackClauseAST;    // Pure... One of...
                            class RollbackToSavepointClauseAST;
                            class RollbackForceClauseAST;
                    class BlockSavepointStatementAST;
                    class BlockSelectIntoStatementAST;
                    class BlockSetTransactionStatementAST;
                    class BlockUpdateStatementAST;
                class AssignmentStatementAST;   // Aggregate LValueExpressionAST
                    class LValueExpressionAST;  // Aggregate List of LValueExpressionItemAST
                        class LValueExpressionItemAST;
                class CloseStatementAST;
                class ContinueOrExitStatementAST;
                class ExecImmediateStatementAST;
                class FetchStatementAST;
                class ForLoopStatementAST;  // Aggregate ForInClauseAST
                    class ForInClauseAST;   // Pure... One of...
                        class ForInBoundsClauseAST;
                        class ForInCursorClauseAST;
                        class ForInSelectClauseAST;
                class ForallStatementAST;   // Aggregate ForallInClauseAST
                    class ForallInClauseAST;    // Pure... One of...
                        class ForallInBoundsClauseAST;
                        class ForallInIndicesClauseAST;
                        class ForallInValuesClauseAST;
                class WhileLoopStatementAST;
                class GotoStatementAST;
                class IfStatementAST;   // Aggregate List of ElsifClauseAST
                    class ElsifClauseAST;
                class LoopStatementAST;
                class NullStatementAST;
                class OpenCursorStatementAST;
                class OpenForStatementAST;  // Agrregate OpenForStatementClauseAST
                    class OpenForStatementClauseAST;    // Pure... One of...
                        class OpenForStatementClauseBySelectAST;
                        class OpenForStatementClauseByStringAST;
                class PlSqlBlocStatementAST;
                class RaiseStatementAST;
                class ReturnStatementAST;

class CursorSpecAST;
class RowTypeAST;
class DataTypeAST;  // Pure... one of...
    class DataTypeByTypeNameAST;
    class DataTypeByNameAST;
    class DataTypeByTableRowAST;
    class DataTypeByTableColumnAST;
    class ScalarDataTypeAST;
class CursorParameterDeclarationAST;
class RecordFieldDeclarationAST;
class RoutineArgumentDeclarationAST;
class FunctionHeadingAST;
class FunctionSpecAST;  // Pure... One of...
    class FunctionDeterministicOrPipelinedSpecAST;
    class FunctionCasheSpecAST;
class ProcedureHeadingAST;
class ExceptionHandlerAST;  // Pure... One of...
    class NamedExceptionHandlerAST;
    class OthersExceptionHandlerAST;
class BaseIntoClauseAST;    // Pure... One of...
    class IntoClauseAST;
    class BulkIntoClauseAST;
class UsingClauseAST;   // Aggregate List of UsingBindAST
    class UsingBindAST;
class DynamicReturningClauseAST;
class ArrayIndexExpressionAST;

class TableIdentifierAST;
class IndexIdentifierAST;
class TriggerIdentifierAST;
class FunctionIdentifierAST;

typedef List<StatementAST *> StatementListAST;
typedef List<SelectCoreAST *> SelectCoreListAST;
typedef List<OrderingTermAST *> OrderingTermListAST;
typedef List<ResultColumnAST *> ResultColumnListAST;
typedef List<JoinItemAST *> JoinItemListAST;
typedef List<SubQueryAST *> SubQueryListAST;
typedef List<NameAST *> NameListAST;

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

typedef List<PlSqlStatementAST *> PlSqlStatementListAST;
typedef List<PlSqlNameAST *> PlSqlNameListAST;
typedef List<DeclarationAST *> DeclarationListAST;
typedef List<CursorParameterDeclarationAST *> CursorParameterDeclarationListAST;
typedef List<RecordFieldDeclarationAST *> RecordFieldDeclarationListAST;
typedef List<RoutineArgumentDeclarationAST *> RoutineArgumentDeclarationListAST;
typedef List<NamedExceptionHandlerAST *> NamedExceptionHandlerListAST;
typedef List<UsingBindAST *> UsingBindListAST;
typedef List<ElsifClauseAST *> ElsifClauseListAST;
typedef List<LValueExpressionAST *> LValueExpressionListAST;
typedef List<ArrayIndexExpressionAST *> ArrayIndexExpressionListAST;
typedef List<LValueExpressionItemAST *> LValueExpressionItemListAST;

typedef List<ColumnIdentifierAST *> ColumnIdentifierListAST;

} // namespace QLalrOracle


#endif // QLALR_ORACLE_ASTFWD_H
