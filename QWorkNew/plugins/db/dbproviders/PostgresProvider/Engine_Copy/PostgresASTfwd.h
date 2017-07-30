#ifndef POSTGRESASTFWD_H
#define POSTGRESASTFWD_H

namespace Postgres {

template <typename _Tp> class List;

class AST;
class ASTVisitor;

class EmptyAST;

// Root Tree Node
class TranslationUnitAST;
    // List<StatementAST>

class NameAST;
class PlSqlBaseNameAST;
class PlSqlNameAST;
class PlSqlTypeNameAST;

class StatementAST; // Pure... One of
    class UnknownStatementAST;
    class DmlStatementAST;
        class SelectStatementAST;
        class DeleteStatementAST;
        class UpdateStatementAST;
        class InsertStatementAST;
    class DdlStatementAST;
        class CreateStatementAST; // Pure... One of
            class CreateTableStatementAST;
            class CreateViewStatementAST;
            class CreateIndexStatementAST;
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


// Expression
class BaseExpressionAST;
    class ValueExpressionAST;
        class UnaryValueExpressionAST;
        class BinaryValueExpressionAST;
        class PrimaryValueExpressionAST;
            class CompoundValueExpressionAST;
            class BuiltinFunctionCallAST;
            class CountStarCallAST;
            class CountExprCallAST;
            class CastFunctionAST;
            class SubstringFunctionAST;
            class FunctionCallAST;
            class ConstantExpressionAST;
                class LiteralExpressionAST;
                class NumericExpressionAST;
                class NullValueExpressionAST;
                class ParameterExpressionAST;
            class SelectCompoundValueExpressionAST;
            class CaseExpressionAST;
            class CaseConditionAST;
            class ColumnIdentifierAST;
    class CompoundRValueExpressionAST;
        class ValueListExpressionAST;
        class SubQueryExpressionAST;
    class ConditionExpressionAST;
        class NotConditionExpressionAST;
        class TrueConditionExpressionAST;
        class FalseConditionExpressionAST;
        class BinaryConditionExpressionAST;
        class CompareConditionExpressionAST;
        class CompoundConditionExpressionAST;
        class LikeConditionExpressionAST;
        class BetweenConditionExpressionAST;
        class InConditionExpressionAST;
        class ExistsConditionExpressionAST;
        class IsNullConditionExpressionAST;
        class ConditionAsValueExpressionAST;

// SelectStatementAST
class SelectCoreAST;
class OrderByBlockAST;
class OrderingTermAST;
class ForUpdateClauseAST;
class LimitToBlockAST;
class OrderingTermAST;

// UpdateStatementAST
class UpdateSetClauseAST;
class AssignmentExpressionAST;

// InsertStatementAST
class InsertColumnsClauseAST;
class InsertClauseAST;  // Pure... One Of
    class InsertClauseAsValuesAST;
    class InsertClauseAsSelectAST;


// CreateTableStatementAST
class CreateTableClauseAST;
    class CreateTableAsSelectClauseAST;
    class CreateTableDirectClauseAST;
// CreateTableDirectClauseAST
class ColumnDefinitionAST;
class ConstraintStateAST;   // Contains ...
    class ConstraintDeferStateAST;
    class UsingIndexClauseAST;
    class ExceptionsClauseAST;

// CreateIndexStatementAST
class BaseCreateIndexClauseAST;
    class TableCreateIndexClauseAST;

// CreateTriggerStatementAST
class TriggerWhetherClauseAST;    // Pure... One Of
    class TriggerWhetherBeforeOrAfterAST;
    class TriggerWhetherAfterAST;
    class TriggerWhetherInsteadOfAST;
class TriggerEventClauseAST;
    class TriggerEventUpdateAST;
    class TriggerEventIdtAST;
class TriggerFromClauseAST;
class TriggerTimingClauseAST;
    class TriggerNotDeferrableTimingAST;
    class TriggerDeferrableTimingAST;
class TriggerForClauseAST;
class TriggerWhenClauseAST;

// AlterTableStatementAST
class IfExistsAST;
class NotValidClauseAST;
class DeferrableClauseAST;
class TriggerWildcardAST;
    class ConcreteTriggerAST;
    class AllOrUserTriggersAST;
class AlterTableActionAST;
    class AlterTableRenameActionAST;
    class AlterTableRenameColumnActionAST;
    class AlterTableRenameConstraintActionAST;
    class AlterTableAddColumnActionAST;
    class AlterTableAddConstraintActionAST;
    class AlterTableDropColumnActionAST;
    class AlterTableDropConstraintActionAST;
    class AlterTableAbleTriggerActionAST;
    class AlterTableAlterColumnActionAST;   // contains
        class AlterColumnActionAST;     // Pure... One Of
            class AlterColumnSetTypeAST;
            class AlterColumnSetDefaultAST;
            class AlterColumnDropDefaultAST;
            class AlterColumnSetOrDropNotNullAST;

class ConstraintAST;            /* Contains */ class ConstraintDeclarationAST;
    class TableConstraintAST;
        class UniqueOrPkTableConstraintAST;
        class UniqueOrPkTableConstraintUsingIndexAST;
        class CheckTableConstraintAST;
        class FkTableConstraintAST;
    class ColumnConstraintAST;
        class PkColumnConstraintAST;
        class NotNullColumnConstraintAST;
        class UniqueColumnConstraintAST;
        class CheckColumnConstraintAST;
        class ReferencesColumnConstraintAST;

class SortedColumnNameAST;
class FkReferencesClauseAST;    // Contains
    class FkReferencesMatchAST;
    class FkReferencesActionAST;

class BaseSelectColumnAST;
    class StarSelectColumnAST;
    class TableSelectColumnAST;
    class ExpressionSelectColumnAST;

class TableReferenceAST;
    class DirectSingleSourceAST;
    class SubQuerySingleSourceAST;
    class JoinedTableAST;
        class CompoundJoinedTableAST;
        class JoinJoinedTableAST;

class JoinSpecAST;
class JoinConditionAST;
class WhereClauseAST;
class GroupByBlockAST;
class SetClauseAST;
class BaseSetSpecAST;
    class MinusSetSpecAST;
    class IntersectSetSpecAST;
    class UnionSetSpecAST;

class CaseExpressionClauseAST;
class CaseConditionClauseAST;

// Identifiers
class TableIdentifierAST;
class IndexIdentifierAST;
class TriggerIdentifierAST;
class FunctionIdentifierAST;

class CollateClauseAST;

// Type Specifiers
class BaseTypeAST;
    class CharacterTypeAST;
    class NumberTypeAST;
    class SimpleTypeAST;
    class UserDefinedTypeAST;

typedef List<StatementAST *> StatementListAST;
typedef List<ValueExpressionAST *> ValueExpressionListAST;
typedef List<BaseSelectColumnAST *> BaseSelectColumnListAST;
typedef List<OrderingTermAST *> OrderingTermListAST;
typedef List<ColumnIdentifierAST *> ColumnIdentifierListAST;
typedef List<TableReferenceAST *> TableReferenceListAST;
typedef List<CaseExpressionClauseAST *> CaseExpressionClauseListAST;
typedef List<CaseConditionClauseAST *> CaseConditionClauseListAST;
typedef List<AssignmentExpressionAST *> AssignmentExpressionListAST;
typedef List<ColumnDefinitionAST *> ColumnDefinitionListAST;
typedef List<ColumnConstraintAST *> ColumnConstraintListAST;
typedef List<TableConstraintAST *> TableConstraintListAST;
typedef List<SortedColumnNameAST *> SortedColumnNameListAST;
typedef List<NameAST *> NameListAST;

}   // namespace Postgres

#endif // POSTGRESASTFWD_H
