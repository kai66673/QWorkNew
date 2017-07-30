#ifndef POSTGRESAST_H
#define POSTGRESAST_H

#include "TranslationUnit.h"
#include "BaseWalker.h"
#include "PostgresASTfwd.h"
#include "MemoryPool.h"

namespace Postgres {

template <typename _Tp>
class List: public Sql::SManaged
{
    List(const List &other);
    void operator =(const List &other);

public:
    List()
        : value(_Tp()), next(0)
    { }

    List(const _Tp &value)
        : value(value), next(this)
    { }

    List(List *previous, const _Tp &value)
        : value(value), next(this)
    {
        if ( previous ) {
            next = previous->next;
            previous->next = this;
        }
    }

    List *finish()
    {
        List *head = next;
        next = 0;
        return head;
    }

    int firstToken() const
    {
        if (value)
            return value->firstToken();

        // ### assert(0);
        return 0;
    }

    int lastToken() const
    {
        _Tp lv = lastValue();

        if (lv)
            return lv->lastToken();

        // ### assert(0);
        return 0;
    }

    _Tp lastValue() const
    {
        _Tp lastValue = 0;

        for (const List *it = this; it; it = it->next) {
            if (it->value)
                lastValue = it->value;
        }

        return lastValue;
    }

    _Tp value;
    List *next;
};

class AST: public Sql::SManaged
{
    AST(const AST &other);
    void operator =(const AST &other);

protected:
    template <typename T>
    static List<T> *finish(List<T> *list)
    {
        if (! list)
            return 0;
        return list->finish(); // convert the circular list with a linked list.
    }

public:
    AST();
    virtual ~AST();

    void accept(ASTVisitor *visitor);

    static void accept(AST *ast, ASTVisitor *visitor)
    { if (ast) ast->accept(visitor); }

    template <typename _Tp>
    static void accept(List<_Tp> *it, ASTVisitor *visitor)
    {
        for (; it; it = it->next)
            accept(it->value, visitor);
    }

    virtual int firstToken() const = 0;
    virtual int lastToken() const = 0;

    virtual TranslationUnitAST *asTranslationUnit() { return 0; }
    virtual StatementAST *asStatement() { return 0; }
    virtual SelectCoreAST *asSelectCore() { return 0; }
    virtual BaseSelectColumnAST *asBaseSelectColumn() { return 0; }
    virtual StarSelectColumnAST *asStarSelectColumn() { return 0; }
    virtual TableSelectColumnAST *asTableSelectColumn() { return 0; }
    virtual ExpressionSelectColumnAST *asExpressionSelectColumn() { return 0; }
    virtual TableReferenceAST *asTableReference() { return 0; }
    virtual JoinedTableAST *asJoinedTable() { return 0; }
    virtual CompoundJoinedTableAST *asCompoundJoinedTable() { return 0; }
    virtual JoinJoinedTableAST *asJoinJoinedTable() { return 0; }
    virtual DirectSingleSourceAST *asDirectSingleSource() { return 0; }
    virtual SubQuerySingleSourceAST *asSubQuerySingleSource() { return 0; }
    virtual JoinSpecAST *asJoinSpec() { return 0; }
    virtual JoinConditionAST *asJoinCondition() { return 0; }
    virtual WhereClauseAST *asWhereClause() { return 0; }
    virtual GroupByBlockAST *asGroupByBlock() { return 0; }
    virtual SetClauseAST *asSetClause() { return 0; }
    virtual BaseSetSpecAST *asBaseSetSpec() { return 0; }
    virtual MinusSetSpecAST *asMinusSetSpec() { return 0; }
    virtual IntersectSetSpecAST *asIntersectSetSpec() { return 0; }
    virtual UnionSetSpecAST *asUnionSetSpec() { return 0; }
    virtual UnknownStatementAST *asUnknownStatement() { return 0; }
    virtual DmlStatementAST *asDmlStatement() { return 0; }
    virtual DdlStatementAST *asDdlStatement() { return 0; }
    virtual SelectStatementAST *asSelectStatement() { return 0; }
    virtual DeleteStatementAST *asDeleteStatement() { return 0; }
    virtual UpdateSetClauseAST *asUpdateSetClause() { return 0; }
    virtual AssignmentExpressionAST *asAssignmentExpression() { return 0; }
    virtual UpdateStatementAST *asUpdateStatement() { return 0; }
    virtual InsertStatementAST *asInsertStatement() { return 0; }
    virtual InsertColumnsClauseAST *asInsertColumnsClause() { return 0; }
    virtual InsertClauseAST *asInsertClause() { return 0; }
    virtual InsertClauseAsValuesAST *asInsertClauseAsValues() { return 0; }
    virtual InsertClauseAsSelectAST *asInsertClauseAsSelect() { return 0; }
    virtual OrderByBlockAST *asOrderByBlock() { return 0; }
    virtual OrderingTermAST *asOrderingTerm() { return 0; }
    virtual ForUpdateClauseAST *asForUpdateClause() { return 0; }
    virtual CreateStatementAST *asCreateStatement() { return 0; }
    virtual CreateTableStatementAST *asCreateTableStatement() { return 0; }
    virtual CreateViewStatementAST *asCreateViewStatement() { return 0; }
    virtual CreateIndexStatementAST *asCreateIndexStatement() { return 0; }
    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return 0; }
    virtual TriggerWhetherClauseAST *asTriggerWhetherClause() { return 0; }
    virtual TriggerWhetherBeforeOrAfterAST *asTriggerWhetherBeforeOrAfter() { return 0; }
    virtual TriggerWhetherInsteadOfAST *asTriggerWhetherInsteadOf() { return 0; }
    virtual TriggerEventClauseAST *asTriggerEventClause() { return 0; }
    virtual TriggerEventUpdateAST *asTriggerEventUpdate() { return 0; }
    virtual TriggerEventIdtAST *asTriggerEventIdt() { return 0; }
    virtual TriggerFromClauseAST *asTriggerFromClause() { return 0; }
    virtual TriggerTimingClauseAST *asTriggerTimingClause() { return 0; }
    virtual TriggerNotDeferrableTimingAST *asTriggerNotDeferrableTiming() { return 0; }
    virtual TriggerDeferrableTimingAST *asTriggerDeferrableTiming() { return 0; }
    virtual TriggerForClauseAST *asTriggerForClause() { return 0; }
    virtual TriggerWhenClauseAST *asTriggerWhenClause() { return 0; }
    virtual CreateTableClauseAST *asCreateTableClause() { return 0; }
    virtual CreateTableAsSelectClauseAST *asCreateTableAsSelectClause() { return 0; }
    virtual CreateTableDirectClauseAST *asCreateTableDirectClause() { return 0; }
    virtual ColumnDefinitionAST *asColumnDefinition() { return 0; }
    virtual ConstraintStateAST *asConstraintState() { return 0; }
    virtual ConstraintDeferStateAST *asConstraintDeferState() { return 0; }
    virtual UsingIndexClauseAST *asUsingIndexClause() { return 0; }
    virtual ExceptionsClauseAST *asExceptionsClause() { return 0; }
    virtual BaseCreateIndexClauseAST *asBaseCreateIndexClause() { return 0; }
    virtual TableCreateIndexClauseAST *asTableCreateIndexClause() { return 0; }
    virtual DropStatementAST *asDropStatement() { return 0; }
    virtual DropTableStatementAST *asDropTableStatement() { return 0; }
    virtual DropViewStatementAST *asDropViewStatement() { return 0; }
    virtual DropIndexStatementAST *asDropIndexStatement() { return 0; }
    virtual DropTriggerStatementAST *asDropTriggerStatement() { return 0; }
    virtual TruncateStatementAST *asTruncateStatement() { return 0; }
    virtual AlterStatementAST *asAlterStatement() { return 0; }
    virtual AlterTableStatementAST *asAlterTableStatement() { return 0; }
    virtual IfExistsAST *asIfExists() { return 0; }
    virtual NotValidClauseAST *asNotValidClause() { return 0; }
    virtual DeferrableClauseAST *asDeferrableClause() { return 0; }
    virtual TriggerWildcardAST *asTriggerWildcard() { return 0; }
    virtual ConcreteTriggerAST *asConcreteTrigger() { return 0; }
    virtual AllOrUserTriggersAST *asAllOrUserTriggers() { return 0; }
    virtual AlterTableActionAST *asAlterTableAction() { return 0; }
    virtual AlterTableRenameActionAST *asAlterTableRenameAction() { return 0; }
    virtual AlterTableRenameColumnActionAST *asAlterTableRenameColumnAction() { return 0; }
    virtual AlterTableRenameConstraintActionAST *asAlterTableRenameConstraintAction() { return 0; }
    virtual AlterTableAddConstraintActionAST *asAlterTableAddConstraintAction() { return 0; }
    virtual AlterTableAddColumnActionAST *asAlterTableAddColumnAction() { return 0; }
    virtual AlterTableDropColumnActionAST *asAlterTableDropColumnAction() { return 0; }
    virtual AlterTableDropConstraintActionAST *asAlterTableDropConstraintAction() { return 0; }
    virtual AlterTableAbleTriggerActionAST *asAlterTableAbleTriggerAction() { return 0; }
    virtual AlterTableAlterColumnActionAST *asAlterTableAlterColumnAction() { return 0; }
    virtual AlterColumnActionAST *asAlterColumnAction() { return 0; }
    virtual AlterColumnSetTypeAST *asAlterColumnSetType() { return 0; }
    virtual AlterColumnSetDefaultAST *asAlterColumnSetDefault() { return 0; }
    virtual AlterColumnDropDefaultAST *asAlterColumnDropDefault() { return 0; }
    virtual AlterColumnSetOrDropNotNullAST *asAlterColumnSetOrDropNotNull() { return 0; }
    virtual CommentStatementAST *asCommentStatement() { return 0; }
    virtual CommentOnTableStatementAST *asCommentOnTableStatement() { return 0; }
    virtual CommentOnColumnStatementAST *asCommentOnColumnStatement() { return 0; }
    virtual CommitStatementAST *asCommitStatement() { return 0; }
    virtual RollbackStatementAST *asRollbackStatement() { return 0; }
    virtual NameAST *asName() { return 0; }
    virtual PlSqlBaseNameAST *asPlSqlBaseName() { return 0; }
    virtual PlSqlNameAST *asPlSqlName() { return 0; }
    virtual PlSqlTypeNameAST *asPlSqlTypeNameAST() { return 0; }

    // Expression
    virtual BaseExpressionAST *asBaseExpression() { return 0; }
    virtual ValueExpressionAST *asValueExpression() { return 0; }
    virtual CompoundValueExpressionAST *asCompoundValueExpression() { return 0; }
    virtual FunctionCallAST *asFunctionCall() { return 0; }
    virtual CountStarCallAST *asCountStarCall() { return 0; }
    virtual CountExprCallAST *asCountExprCall() { return 0; }
    virtual CastFunctionAST *asCastFunction() { return 0; }
    virtual SubstringFunctionAST *asSubstringFunction() { return 0; }
    virtual BuiltinFunctionCallAST *asBuiltinFunctionCall() { return 0; }
    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return 0; }
    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return 0; }
    virtual PrimaryValueExpressionAST *asSimpleValueExpression() { return 0; }
    virtual ConstantExpressionAST *asConstantExpression() { return 0; }
    virtual LiteralExpressionAST *asLiteralExpression() { return 0; }
    virtual NumericExpressionAST *asNumericExpression() { return 0; }
    virtual NullValueExpressionAST *asNullValueExpression() { return 0; }
    virtual ParameterExpressionAST *asParameterExpression() { return 0; }
    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return 0; }
    virtual CaseExpressionAST *asCaseExpression() { return 0; }
    virtual CaseConditionAST *asCaseCondition() { return 0; }
    virtual CompoundRValueExpressionAST *asCompoundRValueExpression() { return 0; }
    virtual ValueListExpressionAST *asValueListExpression() { return 0; }
    virtual SubQueryExpressionAST *asSubQueryExpression() { return 0; }

    // Condition
    virtual ConditionExpressionAST *asConditionExpression() { return 0; }
    virtual NotConditionExpressionAST *asNotConditionExpression() { return 0; }
    virtual TrueConditionExpressionAST *asTrueConditionExpression() { return 0; }
    virtual FalseConditionExpressionAST *asFalseConditionExpression() { return 0; }
    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return 0; }
    virtual CompareConditionExpressionAST *asCompareConditionExpression() { return 0; }
    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return 0; }
    virtual LikeConditionExpressionAST *asLikeConditionExpression() { return 0; }
    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return 0; }
    virtual InConditionExpressionAST *asInConditionExpression() { return 0; }
    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return 0; }
    virtual IsNullConditionExpressionAST *asIsNullConditionExpression() { return 0; }
    virtual ConditionAsValueExpressionAST *asConditionAsValueExpression() { return 0; }
    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return 0; }
    virtual CaseConditionClauseAST *asCaseConditionClause() { return 0; }

    // Identifiers
    virtual TableIdentifierAST *asTableIdentifier() { return 0; }
    virtual IndexIdentifierAST *asIndexIdentifier() { return 0; }
    virtual TriggerIdentifierAST *asTriggerIdentifier() { return 0; }
    virtual ColumnIdentifierAST *asColumnIdentifier() { return 0; }
    virtual FunctionIdentifierAST *asFunctionIdentifier() { return 0; }

    virtual CollateClauseAST *asCollateClause() { return 0; }

    // Type Specifiers
    virtual BaseTypeAST *asBaseType() { return 0; }
    virtual CharacterTypeAST *asCharacterType() { return 0; }
    virtual NumberTypeAST *asNumberType() { return 0; }
    virtual SimpleTypeAST *asSimpleType() { return 0; }
    virtual UserDefinedTypeAST *asUserDefinedType() { return 0; }

    // Constraints
    virtual ConstraintDeclarationAST *asConstraintDeclaration() { return 0; }
    virtual ConstraintAST *asConstraint() { return 0; }
    virtual TableConstraintAST *asTableConstraint() { return 0; }
    virtual SortedColumnNameAST *asSortedColumnName() { return 0; }
    virtual UniqueOrPkTableConstraintAST *asUniqueOrPkTableConstraint() { return 0; }
    virtual UniqueOrPkTableConstraintUsingIndexAST *asUniqueOrPkTableConstraintUsingIndex() { return 0; }
    virtual CheckTableConstraintAST *asCheckTableConstraint() { return 0; }
    virtual FkReferencesClauseAST *asFkReferencesClause() { return 0; }
    virtual FkReferencesMatchAST *asFkReferencesMatch() { return 0; }
    virtual FkReferencesActionAST *asFkReferencesAction() { return 0; }
    virtual FkTableConstraintAST *asFkTableConstraint() { return 0; }
    virtual ColumnConstraintAST *asColumnConstraint() { return 0; }
    virtual PkColumnConstraintAST *asPkColumnConstraint() { return 0; }
    virtual NotNullColumnConstraintAST *asNotNullColumnConstraint() { return 0; }
    virtual UniqueColumnConstraintAST *asUniqueColumnConstraint() { return 0; }
    virtual CheckColumnConstraintAST *asCheckColumnConstraint() { return 0; }
    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return 0; }

    // Additional for SelectStatement and InvalidSelectStatement
    virtual bool isSelectStatement() const { return false; }

protected:
    virtual void accept0( ASTVisitor *visitor ) = 0;
};

class EmptyAST: public AST
{
public:
    EmptyAST() {}

    virtual int firstToken() const { return 0; }
    virtual int lastToken() const { return 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TranslationUnitAST: public AST
{
public:
    StatementListAST *statements;

public:
    TranslationUnitAST( StatementListAST *statements )
        : statements(finish(statements))
    {}

    virtual TranslationUnitAST *asTranslationUnit() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StatementAST: public AST
{
public:
    unsigned start_line;
    unsigned start_pos;
    unsigned end_line;
    unsigned end_pos;

public:
    StatementAST( bool topLevel_ = true, bool blockMode_ = false )
        : start_line(0)
        , start_pos(0)
        , end_line(0)
        , end_pos(0)
        , topLevel(topLevel_)
        , blockMode(blockMode_)
    { }

    virtual StatementAST *asStatement() { return this; }

    inline bool isTopLevelStatement() const { return topLevel; }
    inline void setTopLevelStatement( bool topLevel_ ) { topLevel = topLevel_; }
    inline bool isBlockMode() const { return blockMode; }
    inline void setBlockMode( bool blockMode_ = true ) { blockMode = blockMode_; }

private:
    bool topLevel;
    bool blockMode;
};

class UnknownStatementAST: public StatementAST
{
public:
    int first_token;
    int last_token;
    bool isSelect;

public:
    UnknownStatementAST( int first_token_, int last_token_, bool isSelect_ )
        : StatementAST()
        , first_token(first_token_)
        , last_token(last_token_)
        , isSelect(isSelect_)
    {}

    virtual UnknownStatementAST *asUnknownStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

    virtual bool isSelectStatement() const { return isSelect; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DmlStatementAST: public StatementAST
{
public:
    DmlStatementAST() : StatementAST() {}
    virtual DmlStatementAST *asDmlStatement() { return this; }
};

class DdlStatementAST: public StatementAST
{
public:
    DdlStatementAST() : StatementAST() {}
    virtual DdlStatementAST *asDdlStatement() { return this; }
};

class SelectStatementAST: public DmlStatementAST
{
public:
    SelectCoreAST *selectCore;
    OrderByBlockAST *orderByBlock;
    ForUpdateClauseAST *forupdateClause;

public:
    SelectStatementAST( SelectCoreAST *select_core_,
                        OrderByBlockAST *order_by_block_,
                        ForUpdateClauseAST *forupdate_clause_ )
        : DmlStatementAST()
        , selectCore(select_core_)
        , orderByBlock(order_by_block_)
        , forupdateClause(forupdate_clause_)
    {}

    virtual SelectStatementAST *asSelectStatement() { return this; }
    virtual bool isSelectStatement() const { return true; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeleteStatementAST: public DmlStatementAST
{
public:
    int delete_token;
    int from_token;
    DirectSingleSourceAST *source;
    WhereClauseAST *whereClause;

public:
    DeleteStatementAST( int delete_token_,
                        int from_token_,
                        DirectSingleSourceAST *source_,
                        WhereClauseAST *whereClause_ )
        : DmlStatementAST()
        , delete_token(delete_token_)
        , from_token(from_token_)
        , source(source_)
        , whereClause(whereClause_)
    {}

    virtual DeleteStatementAST *asDeleteStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UpdateSetClauseAST: public AST
{
public:
    int set_token;
    AssignmentExpressionListAST *upd_list;

public:
    UpdateSetClauseAST( int set_token_,
                        AssignmentExpressionListAST *upd_list_ )
        : set_token(set_token_)
        , upd_list(finish(upd_list_))
    {}

    virtual UpdateSetClauseAST *asUpdateSetClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UpdateStatementAST: public DmlStatementAST
{
public:
    int update_token;
    DirectSingleSourceAST *source;
    UpdateSetClauseAST *setClause;
    WhereClauseAST *whereClause;

public:
    UpdateStatementAST( int update_token_,
                        DirectSingleSourceAST *source_,
                        UpdateSetClauseAST *setClause_,
                        WhereClauseAST *whereClause_ )
        : DmlStatementAST()
        , update_token(update_token_)
        , source(source_)
        , setClause(setClause_)
        , whereClause(whereClause_)
    {}

    virtual UpdateStatementAST *asUpdateStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertStatementAST: public DmlStatementAST
{
public:
    int insert_token;
    int into_token;
    DirectSingleSourceAST *source;
    InsertColumnsClauseAST *insertColumns;
    InsertClauseAST *insClause;

public:
    InsertStatementAST( int insert_token_,
                        int into_token_,
                        DirectSingleSourceAST *source_,
                        InsertColumnsClauseAST *insertColumns_,
                        InsertClauseAST *insClause_ )
        : DmlStatementAST()
        , insert_token(insert_token_)
        , into_token(into_token_)
        , source(source_)
        , insertColumns(insertColumns_)
        , insClause(insClause_)
    {}

    virtual InsertStatementAST *asInsertStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertColumnsClauseAST: public AST
{
public:
    int lparen_token;
    ColumnIdentifierListAST *col_list;
    int rparen_token;

public:
    InsertColumnsClauseAST( int lparen_token_,
                            ColumnIdentifierListAST *col_list_,
                            int rparen_token_ )
        : lparen_token(lparen_token_)
        , col_list(finish(col_list_))
        , rparen_token(rparen_token_)
    {}

    virtual InsertColumnsClauseAST *asInsertColumnsClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertClauseAST: public AST
{
public:
    InsertClauseAST() {}
    virtual InsertClauseAST *asInsertClause() { return this; }
};

class InsertClauseAsValuesAST: public InsertClauseAST
{
public:
    int values_token;
    int lparen_token;
    ValueExpressionListAST *val_list;
    int rparen_token;

public:
    InsertClauseAsValuesAST( int values_token_,
                             int lparen_token_,
                             ValueExpressionListAST *val_list_,
                             int rparen_token_ )
        : InsertClauseAST()
        , values_token(values_token_)
        , lparen_token(lparen_token_)
        , val_list(finish(val_list_))
        , rparen_token(rparen_token_)
    {}

    virtual InsertClauseAsValuesAST *asInsertClauseAsValues() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertClauseAsSelectAST: public InsertClauseAST
{
public:
    StatementAST *selectStatement;

public:
    InsertClauseAsSelectAST( StatementAST *selectStatement_ )
        : InsertClauseAST()
        , selectStatement(selectStatement_)
    {}

    virtual InsertClauseAsSelectAST *asInsertClauseAsSelect() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OrderByBlockAST: public AST
{
public:
    int order_token;
    int by_token;
    OrderingTermListAST *term_list;

public:
    OrderByBlockAST( int order_token_,
                     int by_token_,
                     OrderingTermListAST *term_list_ )
        : order_token(order_token_)
        , by_token(by_token_)
        , term_list(finish(term_list_))
    {}

    virtual OrderByBlockAST *asOrderByBlock() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OrderingTermAST: public AST
{
public:
    ValueExpressionAST *expr;
    int asc_or_desc_token;

public:
    OrderingTermAST( ValueExpressionAST *expr_,
                     int asc_or_desc_token_ )
        : expr(expr_)
        , asc_or_desc_token(asc_or_desc_token_)
    {}

    virtual OrderingTermAST *asOrderingTerm() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForUpdateClauseAST: public AST
{
public:
    int for_token;
    int update_token;
    int of_token;
    ColumnIdentifierListAST *col_list;
    int nowait_token;

public:
    ForUpdateClauseAST( int for_token_, int update_token_, int of_token_,
                        ColumnIdentifierListAST *col_list_, int nowait_token_ )
        : for_token(for_token_)
        , update_token(update_token_)
        , of_token(of_token_)
        , col_list(finish(col_list_))
        , nowait_token(nowait_token_)
    {}

    virtual ForUpdateClauseAST *asForUpdateClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCoreAST: public AST
{
public:
    int select_token;
    int select_spec_token;         // ALL or DISTINCT
    BaseSelectColumnListAST *selColumns;
    int from_token;
    TableReferenceListAST *tables;
    WhereClauseAST *whereClause;
    GroupByBlockAST *groupBy;
    SetClauseAST *setClause;

public:
    SelectCoreAST( int select_token_,
                   int select_spec_token_,
                   BaseSelectColumnListAST *columnList_,
                   int from_token_,
                   TableReferenceListAST *tables_,
                   WhereClauseAST *whereClause_,
                   GroupByBlockAST *groupBy_,
                   SetClauseAST *setClause_ )
        : select_token(select_token_)
        , select_spec_token(select_spec_token_)
        , selColumns(finish(columnList_))
        , from_token(from_token_)
        , tables(finish(tables_))
        , whereClause(whereClause_)
        , groupBy(groupBy_)
        , setClause(setClause_)
    {}

    virtual SelectCoreAST *asSelectCore() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseSelectColumnAST: public AST
{
public:
    BaseSelectColumnAST() {}
    virtual BaseSelectColumnAST *asBaseSelectColumn() { return this; }
};

class StarSelectColumnAST: public BaseSelectColumnAST
{
public:
    int star_token;

public:
    StarSelectColumnAST( int star_token_ )
        : BaseSelectColumnAST()
        , star_token(star_token_)
    {}

    virtual StarSelectColumnAST *asStarSelectColumn() { return this; }

    virtual int firstToken() const { return star_token; }
    virtual int lastToken() const { return star_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableSelectColumnAST: public BaseSelectColumnAST
{
public:
    TableIdentifierAST *tableIdent;
    int dot_token;
    int star_token;

public:
    TableSelectColumnAST( TableIdentifierAST *tableIdent_,
                          int dot_token_,
                          int star_token_ )
        : BaseSelectColumnAST()
        , tableIdent(tableIdent_)
        , dot_token(dot_token_)
        , star_token(star_token_)
    {}

    virtual TableSelectColumnAST *asTableSelectColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExpressionSelectColumnAST: public BaseSelectColumnAST
{
public:
    ValueExpressionAST *columnExpr;
    int as_token;
    NameAST *columnAlias;

public:
    ExpressionSelectColumnAST( ValueExpressionAST *columnExpr_,
                               int as_token_,
                               NameAST *columnAlias_ )
        : BaseSelectColumnAST()
        , columnExpr(columnExpr_)
        , as_token(as_token_)
        , columnAlias(columnAlias_)
    {}

    virtual ExpressionSelectColumnAST *asExpressionSelectColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableReferenceAST: public AST
{
public:
    TableReferenceAST() {}
    virtual TableReferenceAST *asTableReference() { return this; }
};

class JoinedTableAST: public TableReferenceAST
{
public:
    JoinedTableAST() {}
    virtual JoinedTableAST *asJoinedTable() { return this; }
};

class CompoundJoinedTableAST: public JoinedTableAST
{
public:
    int lparen_token;
    JoinedTableAST *table;
    int rparen_token;

public:
    CompoundJoinedTableAST( int lparen_token_,
                            JoinedTableAST *table_,
                            int rparen_token_ )
        : lparen_token(lparen_token_)
        , table(table_)
        , rparen_token(rparen_token_)
    {}

    virtual CompoundJoinedTableAST *asCompoundJoinedTable() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinJoinedTableAST: public JoinedTableAST
{
public:
    TableReferenceAST *ltr;
    JoinSpecAST *spec;
    TableReferenceAST *rtr;
    JoinConditionAST *condition;

public:
    JoinJoinedTableAST( TableReferenceAST *ltr_,
                        JoinSpecAST *spec_,
                        TableReferenceAST *rtr_,
                        JoinConditionAST *condition_ )
        : ltr(ltr_)
        , spec(spec_)
        , rtr(rtr_)
        , condition(condition_)
    {}

    virtual JoinJoinedTableAST *asJoinJoinedTable() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DirectSingleSourceAST: public TableReferenceAST
{
public:
    TableIdentifierAST *schemaTableName;
    int as_token;
    NameAST *tableAlias;

public:
    DirectSingleSourceAST( TableIdentifierAST *schemaTableName_,
                           int as_token_,
                           NameAST *tableAlias_ )
        : TableReferenceAST()
        , schemaTableName(schemaTableName_)
        , as_token(as_token_)
        , tableAlias(tableAlias_)
    {}

    virtual DirectSingleSourceAST *asDirectSingleSource() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubQuerySingleSourceAST: public TableReferenceAST
{
public:
    int lparen_token;
    SelectStatementAST *selectStatement;
    int rparen_token;
    int as_token;
    NameAST *tableAlias;

public:
    SubQuerySingleSourceAST( int lparen_token_,
                             SelectStatementAST *selectStatement_,
                             int rparen_token_,
                             int as_token_,
                             NameAST *tableAlias_ )
        : TableReferenceAST()
        , lparen_token(lparen_token_)
        , selectStatement(selectStatement_)
        , rparen_token(rparen_token_)
        , as_token(as_token_)
        , tableAlias(tableAlias_)
    {}

    virtual SubQuerySingleSourceAST *asSubQuerySingleSource() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinSpecAST: public AST
{
public:
    int j1_spec_token;
    int j2_spec_token;
    int join_token;

public:
    JoinSpecAST( int j1_spec_token_,
                 int j2_spec_token_,
                 int join_token_ )
        : j1_spec_token(j1_spec_token_)
        , j2_spec_token(j2_spec_token_)
        , join_token(join_token_)
    {}

    virtual JoinSpecAST *asJoinSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinConditionAST: public AST
{
public:
    int on_token;
    ConditionExpressionAST *conditionExpr;

public:
    JoinConditionAST( int on_token_,
                      ConditionExpressionAST *conditionExpr_ )
        : on_token(on_token_)
        , conditionExpr(conditionExpr_)
    {}

    virtual JoinConditionAST *asJoinCondition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WhereClauseAST: public AST
{
public:
    int where_token;
    ConditionExpressionAST *whereExpr;

public:
    WhereClauseAST( int where_token_,
                    ConditionExpressionAST *whereExpr_ )
        : where_token(where_token_)
        , whereExpr(whereExpr_)
    {}

    virtual WhereClauseAST *asWhereClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GroupByBlockAST: public AST
{
public:
    int group_token;
    int by_token;
    ValueExpressionListAST *group_items_list;
    int having_token;
    ConditionExpressionAST *havingExpr;

public:
    GroupByBlockAST( int group_token_,
                     int by_token_,
                     ValueExpressionListAST *group_items_list_,
                     int having_token_,
                     ConditionExpressionAST *havingExpr_ )
        : group_token(group_token_)
        , by_token(by_token_)
        , group_items_list(finish(group_items_list_))
        , having_token(having_token_)
        , havingExpr(havingExpr_)
    {}

    virtual GroupByBlockAST *asGroupByBlock() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetClauseAST: public AST
{
public:
    BaseSetSpecAST *spec;
    SelectCoreAST *selectCore;

public:
    SetClauseAST( BaseSetSpecAST *spec_,
                  SelectCoreAST *selectCore_ )
        : spec(spec_)
        , selectCore(selectCore_)
    {}

    virtual SetClauseAST *asSetClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseSetSpecAST: public AST
{
public:
    BaseSetSpecAST() {}
    virtual BaseSetSpecAST *asBaseSetSpec() { return this; }
};

class MinusSetSpecAST: public BaseSetSpecAST
{
public:
    int minus_token;

public:
    MinusSetSpecAST( int minus_token_ )
        : BaseSetSpecAST()
        , minus_token(minus_token_)
    {}

    virtual MinusSetSpecAST *asMinusSetSpec() { return this; }

    virtual int firstToken() const { return minus_token; }
    virtual int lastToken() const { return minus_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntersectSetSpecAST: public BaseSetSpecAST
{
public:
    int intersect_token;

public:
    IntersectSetSpecAST( int intersect_token_ )
        : BaseSetSpecAST()
        , intersect_token(intersect_token_)
    {}

    virtual IntersectSetSpecAST *asIntersectSetSpec() { return this; }

    virtual int firstToken() const { return intersect_token; }
    virtual int lastToken() const { return intersect_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UnionSetSpecAST: public BaseSetSpecAST
{
public:
    int union_token;
    int all_or_distinct_token;

public:
    UnionSetSpecAST( int union_token_,
                     int all_or_distinct_token_ )
        : BaseSetSpecAST()
        , union_token(union_token_)
        , all_or_distinct_token(all_or_distinct_token_)
    {}

    virtual UnionSetSpecAST *asUnionSetSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateStatementAST: public DdlStatementAST
{
public:
    CreateStatementAST() : DdlStatementAST() {}
    virtual CreateStatementAST *asCreateStatement() { return this; }
};

class CreateTableStatementAST: public CreateStatementAST
{
public:
    int create_token;
    int table_token;
    TableIdentifierAST *tableIdent;
    CreateTableClauseAST *tableClause;

public:
    CreateTableStatementAST( int create_token_,
                             int table_token_,
                             TableIdentifierAST *tableIdent_,
                             CreateTableClauseAST *tableClause_ )
        : CreateStatementAST()
        , create_token(create_token_)
        , table_token(table_token_)
        , tableIdent(tableIdent_)
        , tableClause(tableClause_)
    {}

    virtual CreateTableStatementAST *asCreateTableStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateViewStatementAST: public CreateStatementAST
{
public:
    int create_token;
    int view_token;
    TableIdentifierAST *viewIdent;
    int as_token;
    StatementAST *selectStatement;

public:
    CreateViewStatementAST( int create_token_,
                            int view_token_,
                            TableIdentifierAST *viewIdent_,
                            int as_token_,
                            StatementAST *selectStatement_ )
        : CreateStatementAST()
        , create_token(create_token_)
        , view_token(view_token_)
        , viewIdent(viewIdent_)
        , as_token(as_token_)
        , selectStatement(selectStatement_)
    {}

    virtual CreateViewStatementAST *asCreateViewStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateIndexStatementAST: public CreateStatementAST
{
public:
    int create_token;
    int unique_token;
    int index_token;
    IndexIdentifierAST *indexIdent;
    BaseCreateIndexClauseAST *indexClause;

public:
    CreateIndexStatementAST( int create_token_,
                             int unique_token_,
                             int index_token_,
                             IndexIdentifierAST *indexIdent_,
                             BaseCreateIndexClauseAST *indexClause_ )
        : CreateStatementAST()
        , create_token(create_token_)
        , unique_token(unique_token_)
        , index_token(index_token_)
        , indexIdent(indexIdent_)
        , indexClause(indexClause_)
    {}

    virtual CreateIndexStatementAST *asCreateIndexStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTriggerStatementAST: public CreateStatementAST
{
public:
    int create_token;
    int constraint_token;
    int trigger_token;
    TriggerIdentifierAST *triggerIdent;
    TriggerWhetherClauseAST *whetherClause;
    TriggerEventClauseAST *eventClause;
    int on_token;
    TableIdentifierAST *tableIdent;
    TriggerFromClauseAST *fromClause;
    TriggerForClauseAST *forClause;
    TriggerWhenClauseAST *whenClause;
    int execute_token;
    int procedure_token;
    FunctionCallAST *funcCall;

public:
    CreateTriggerStatementAST( int create_token_,
                               int constraint_token_,
                               int trigger_token_,
                               TriggerIdentifierAST *triggerIdent_,
                               TriggerWhetherClauseAST *whetherClause_,
                               TriggerEventClauseAST *eventClause_,
                               int on_token_,
                               TableIdentifierAST *tableIdent_,
                               TriggerFromClauseAST *fromClause_,
                               TriggerForClauseAST *forClause_,
                               TriggerWhenClauseAST *whenClause_,
                               int execute_token_,
                               int procedure_token_,
                               FunctionCallAST *funcCall_ )
        : CreateStatementAST()
        , create_token(create_token_)
        , constraint_token(constraint_token_)
        , trigger_token(trigger_token_)
        , triggerIdent(triggerIdent_)
        , whetherClause(whetherClause_)
        , eventClause(eventClause_)
        , on_token(on_token_)
        , tableIdent(tableIdent_)
        , fromClause(fromClause_)
        , forClause(forClause_)
        , whenClause(whenClause_)
        , execute_token(execute_token_)
        , procedure_token(procedure_token_)
        , funcCall(funcCall_)
    {}

    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerWhetherClauseAST: public AST
{
public:
    TriggerWhetherClauseAST() {}
    virtual TriggerWhetherClauseAST *asTriggerWhetherClause() { return this; }
};

class TriggerWhetherBeforeOrAfterAST: public TriggerWhetherClauseAST
{
public:
    int before_or_after_token;

public:
    TriggerWhetherBeforeOrAfterAST( int before_or_after_token_ )
        : TriggerWhetherClauseAST()
        , before_or_after_token(before_or_after_token_)
    {}

    virtual TriggerWhetherBeforeOrAfterAST *asTriggerWhetherBeforeOrAfter() { return this; }

    virtual int firstToken() const { return before_or_after_token; }
    virtual int lastToken() const { return before_or_after_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerWhetherInsteadOfAST: public TriggerWhetherClauseAST
{
public:
    int instead_token;
    int of_token;

public:
    TriggerWhetherInsteadOfAST( int after_token_, int of_token_ )
        : TriggerWhetherClauseAST()
        , instead_token(after_token_)
        , of_token(of_token_)
    {}

    virtual TriggerWhetherInsteadOfAST *asTriggerWhetherInsteadOf() { return this; }

    virtual int firstToken() const { return instead_token; }
    virtual int lastToken() const { if ( of_token ) return of_token + 1; return instead_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerEventClauseAST: public AST
{
public:
    TriggerEventClauseAST() {}
    virtual TriggerEventClauseAST *asTriggerEventClause() { return this; }
};

class TriggerEventUpdateAST: public TriggerEventClauseAST
{
public:
    int update_token;
    int of_token;
    ColumnIdentifierListAST *columns;

public:
    TriggerEventUpdateAST( int update_token_,
                           int of_token_,
                           ColumnIdentifierListAST *columns_ )
        : TriggerEventClauseAST()
        , update_token(update_token_)
        , of_token(of_token_)
        , columns(columns_)
    {}

    virtual TriggerEventUpdateAST *asTriggerEventUpdate() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerEventIdtAST: public TriggerEventClauseAST
{
public:
    int event_token;

public:
    TriggerEventIdtAST( int event_token_ )
        : TriggerEventClauseAST()
        , event_token(event_token_)
    {}

    virtual TriggerEventIdtAST *asTriggerEventIdt() { return this; }

    virtual int firstToken() const { return event_token; }
    virtual int lastToken() const { return event_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerFromClauseAST: public AST
{
public:
    int from_token;
    TableIdentifierAST *refTableIdent;

public:
    TriggerFromClauseAST( int from_token_,
                          TableIdentifierAST *refTableIdent_ )
        : from_token(from_token_)
        , refTableIdent(refTableIdent_)
    {}

    virtual TriggerFromClauseAST *asTriggerFromClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerTimingClauseAST: public AST
{
public:
    TriggerTimingClauseAST() {}
    virtual TriggerTimingClauseAST *asTriggerTimingClause() { return this; }
};

class TriggerNotDeferrableTimingAST: public TriggerTimingClauseAST
{
public:
    int not_token;
    int deferrable_token;

public:
    TriggerNotDeferrableTimingAST( int not_token_,
                                   int deferrable_token_ )
        : TriggerTimingClauseAST()
        , not_token(not_token_)
        , deferrable_token(deferrable_token_)
    {}

    virtual TriggerNotDeferrableTimingAST *asTriggerNotDeferrableTiming() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerDeferrableTimingAST: public TriggerTimingClauseAST
{
public:
    int deferrable_token;
    int initially_token;
    int immediate_or_deferred_token;

public:
    TriggerDeferrableTimingAST( int deferrable_token_,
                                int initially_token_,
                                int immediate_or_deferred_token_ )
        : TriggerTimingClauseAST()
        , deferrable_token(deferrable_token_)
        , initially_token(initially_token_)
        , immediate_or_deferred_token(immediate_or_deferred_token_)
    {}

    virtual TriggerDeferrableTimingAST *asTriggerDeferrableTiming() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerForClauseAST: public AST
{
public:
    int for_token;
    int each_token;
    int row_or_statement_token;

public:
    TriggerForClauseAST( int for_token_,
                         int each_token_,
                         int row_or_statement_token_ )
        : for_token(for_token_)
        , each_token(each_token_)
        , row_or_statement_token(row_or_statement_token_)
    {}

    virtual TriggerForClauseAST *asTriggerForClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerWhenClauseAST: public AST
{
public:
    TriggerWhenClauseAST() {}
    virtual TriggerWhenClauseAST *asTriggerWhenClause() { return this; }
};

class CreateTableClauseAST: public AST
{
public:
    CreateTableClauseAST() {}
    virtual CreateTableClauseAST *asCreateTableClause() { return this; }
};

class CreateTableAsSelectClauseAST: public CreateTableClauseAST
{
public:
    int as_token;
    SelectStatementAST *selectStatement;

public:
    CreateTableAsSelectClauseAST( int as_token_,
                                  SelectStatementAST *selectStatement_ )
        : CreateTableClauseAST()
        , as_token(as_token_)
        , selectStatement(selectStatement_)
    {}

    CreateTableAsSelectClauseAST *asCreateTableAsSelectClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableDirectClauseAST: public CreateTableClauseAST
{
public:
    int lparen_token;
    ColumnDefinitionListAST *column_def_list;
    TableConstraintListAST *constraintList;
    int rparen_token;

public:
    CreateTableDirectClauseAST( int lparen_token_,
                                ColumnDefinitionListAST *column_def_list_,
                                TableConstraintListAST *constraintList_,
                                int rparen_token_ )
        : CreateTableClauseAST()
        , lparen_token(lparen_token_)
        , column_def_list(finish(column_def_list_))
        , constraintList(finish(constraintList_))
        , rparen_token(rparen_token_)
    {}

    virtual CreateTableDirectClauseAST *asCreateTableDirectClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnDefinitionAST: public AST
{
public:
    NameAST *columnName;
    BaseTypeAST *columnType;
    CollateClauseAST *collate;
    int default_token;
    ValueExpressionAST *defaultExpr;
    ColumnConstraintListAST *constraintList;

public:
    ColumnDefinitionAST( NameAST *columnName_,
                         BaseTypeAST *columnType_,
                         CollateClauseAST *collate_,
                         int default_token_,
                         ValueExpressionAST *defaultExpr_,
                         ColumnConstraintListAST *constraintList_ )
        : columnName(columnName_)
        , columnType(columnType_)
        , collate(collate_)
        , default_token(default_token_)
        , defaultExpr(defaultExpr_)
        , constraintList(finish(constraintList_))
    {}

    virtual ColumnDefinitionAST *asColumnDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintStateAST: public AST
{
public:
    DeferrableClauseAST *defer;
    int relay_or_norelay_token;
    UsingIndexClauseAST *usingIndex;
    int enable_or_disable_token;
    int validate_or_novalidate_token;
    ExceptionsClauseAST *exceptionClause;

public:
    ConstraintStateAST( DeferrableClauseAST *defer_,
                        int relay_or_norelay_token_,
                        UsingIndexClauseAST *usingIndex_,
                        int enable_or_disable_token_,
                        int validate_or_novalidate_token_,
                        ExceptionsClauseAST *exceptionClause_ )
        : defer(defer_)
        , relay_or_norelay_token(relay_or_norelay_token_)
        , usingIndex(usingIndex_)
        , enable_or_disable_token(enable_or_disable_token_)
        , validate_or_novalidate_token(validate_or_novalidate_token_)
        , exceptionClause(exceptionClause_)
    {}

    virtual ConstraintStateAST *asConstraintState() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintDeferStateAST: public AST
{
public:
    int token1;
    int token2;
    int token3;
    int token4;

public:
    ConstraintDeferStateAST( int token1_,
                             int token2_,
                             int token3_,
                             int token4_ )
        : token1(token1_)
        , token2(token2_)
        , token3(token3_)
        , token4(token4_)
    {}

    virtual ConstraintDeferStateAST *asConstraintDeferState() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingIndexClauseAST: public AST
{
public:
    int using_token;
    int index_token;
    IndexIdentifierAST *indexIdent;
    int lparen_token;
    CreateIndexStatementAST *createIndexStatement;
    int rparen_token;

public:
    UsingIndexClauseAST( int using_token_,
                         int index_token_,
                         IndexIdentifierAST *indexIdent_,
                         int lparen_token_,
                         CreateIndexStatementAST *createIndexStatement_,
                         int rparen_token_ )
        : using_token(using_token_)
        , index_token(index_token_)
        , indexIdent(indexIdent_)
        , lparen_token(lparen_token_)
        , createIndexStatement(createIndexStatement_)
        , rparen_token(rparen_token_)
    {}

    virtual UsingIndexClauseAST *asUsingIndexClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExceptionsClauseAST: public AST
{
public:
    int exceptions_token;
    int into_token;
    TableIdentifierAST *tableIdent;

public:
    ExceptionsClauseAST( int exceptions_token_,
                         int into_token_,
                         TableIdentifierAST *tableIdent_ )
        : exceptions_token(exceptions_token_)
        , into_token(into_token_)
        , tableIdent(tableIdent_)
    {}

    virtual ExceptionsClauseAST *asExceptionsClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseCreateIndexClauseAST: public AST
{
public:
    BaseCreateIndexClauseAST() {}
    virtual BaseCreateIndexClauseAST *asBaseCreateIndexClause() { return this; }
};

class TableCreateIndexClauseAST: public BaseCreateIndexClauseAST
{
public:
    int on_token;
    TableIdentifierAST *tableIdent;
    int lparen_token;
    OrderingTermListAST *columns_list;
    int rparen_token;

public:
    TableCreateIndexClauseAST( int on_token_,
                               TableIdentifierAST *tableIdent_,
                               int lparen_token_,
                               OrderingTermListAST *columns_list_,
                               int rparen_token_ )
        : BaseCreateIndexClauseAST()
        , on_token(on_token_)
        , tableIdent(tableIdent_)
        , lparen_token(lparen_token_)
        , columns_list(finish(columns_list_))
        , rparen_token(rparen_token_)
    {}

    virtual TableCreateIndexClauseAST *asTableCreateIndexClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropStatementAST: public DdlStatementAST
{
public:
    DropStatementAST() : DdlStatementAST() {}
    virtual DropStatementAST *asDropStatement() { return this; }
};

class DropTableStatementAST: public DropStatementAST
{
public:
    int drop_token;
    int table_token;
    TableIdentifierAST *tableIdent;

public:
    DropTableStatementAST( int drop_token_,
                           int table_token_,
                           TableIdentifierAST *tableIdent_ )
        : DropStatementAST()
        , drop_token(drop_token_)
        , table_token(table_token_)
        , tableIdent(tableIdent_)
    {}

    virtual DropTableStatementAST *asDropTableStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropViewStatementAST: public DropStatementAST
{
public:
    int drop_token;
    int view_token;
    TableIdentifierAST *viewIdent;

public:
    DropViewStatementAST( int drop_token_,
                          int view_token_,
                          TableIdentifierAST *viewIdent_ )
        : DropStatementAST()
        , drop_token(drop_token_)
        , view_token(view_token_)
        , viewIdent(viewIdent_)
    {}

    virtual DropViewStatementAST *asDropViewStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropIndexStatementAST: public DropStatementAST
{
public:
    int drop_token;
    int index_token;
    IndexIdentifierAST *indexIdent;

public:
    DropIndexStatementAST( int drop_token_,
                           int index_token_,
                           IndexIdentifierAST *indexIdent_ )
        : DropStatementAST()
        , drop_token(drop_token_)
        , index_token(index_token_)
        , indexIdent(indexIdent_)
    {}

    virtual DropIndexStatementAST *asDropIndexStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropTriggerStatementAST: public DropStatementAST
{
public:
    int drop_token;
    int trigger_token;
    TriggerIdentifierAST *triggerIdent;

public:
    DropTriggerStatementAST( int drop_token_,
                             int trigger_token_,
                             TriggerIdentifierAST *triggerIdent_ )
        : DropStatementAST()
        , drop_token(drop_token_)
        , trigger_token(trigger_token_)
        , triggerIdent(triggerIdent_)
    {}

    virtual DropTriggerStatementAST *asDropTriggerStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TruncateStatementAST: public DdlStatementAST
{
public:
    int truncate_token;
    int table_token;
    TableIdentifierAST *tableIdent;

public:
    TruncateStatementAST( int truncate_token_,
                          int table_token_,
                          TableIdentifierAST *tableIdent_ )
        : DdlStatementAST()
        , truncate_token(truncate_token_)
        , table_token(table_token_)
        , tableIdent(tableIdent_)
    {}

    virtual TruncateStatementAST *asTruncateStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterStatementAST: public DdlStatementAST
{
public:
    AlterStatementAST()
        : DdlStatementAST()
    {}

    virtual AlterStatementAST *asAlterStatement() { return this; }
};

class AlterTableStatementAST: public AlterStatementAST
{
public:
    int alter_token;
    int table_token;
    IfExistsAST *ifExists;
    int only_token;
    TableIdentifierAST *tableIdent;
    AlterTableActionAST *action;

public:
    AlterTableStatementAST( int alter_token_,
                            int table_token_,
                            IfExistsAST *ifExists_,
                            int only_token_,
                            TableIdentifierAST *tableIdent_,
                            AlterTableActionAST *action_ )
        : AlterStatementAST()
        , alter_token(alter_token_)
        , table_token(table_token_)
        , ifExists(ifExists_)
        , only_token(only_token_)
        , tableIdent(tableIdent_)
        , action(action_)
    {}

    virtual AlterTableStatementAST *asAlterTableStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IfExistsAST: public AST
{
public:
    int if_token;
    int exists_token;

public:
    IfExistsAST( int if_token_,
                 int exists_token_ )
        : if_token(if_token_)
        , exists_token(exists_token_)
    {}

    virtual IfExistsAST *asIfExists() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotValidClauseAST: public AST
{
public:
    int not_token;
    int valid_token;

public:
    NotValidClauseAST( int not_token_,
                       int valid_token_ )
        : not_token(not_token_)
        , valid_token(valid_token_)
    {}

    virtual NotValidClauseAST *asNotValidClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeferrableClauseAST: public AST
{
public:
    int not_token;
    int deferrable_token;
    int initially_token;
    int deferred_or_immediate_token;

public:
    DeferrableClauseAST( int not_token_,
                         int deferrable_token_,
                         int initially_token_,
                         int deferred_or_immediate_token_ )
        : not_token(not_token_)
        , deferrable_token(deferrable_token_)
        , initially_token(initially_token_)
        , deferred_or_immediate_token(deferred_or_immediate_token_)
    {}

    virtual DeferrableClauseAST *asDeferrableClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerWildcardAST: public AST
{
public:
    TriggerWildcardAST() {}
    virtual TriggerWildcardAST *asTriggerWildcard() { return this; }
};

class ConcreteTriggerAST: public TriggerWildcardAST
{
public:
    NameAST *triggerName;

public:
    ConcreteTriggerAST( NameAST *triggerName_ )
        : TriggerWildcardAST()
        , triggerName(triggerName_)
    {}

    virtual ConcreteTriggerAST *asConcreteTrigger() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AllOrUserTriggersAST: public TriggerWildcardAST
{
public:
    int all_or_user_token;

public:
    AllOrUserTriggersAST( int all_or_user_token_ )
        : TriggerWildcardAST()
        , all_or_user_token(all_or_user_token_)
    {}

    virtual AllOrUserTriggersAST *asAllOrUserTriggers() { return this; }

    virtual int firstToken() const { return all_or_user_token; }
    virtual int lastToken() const { return all_or_user_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableActionAST: public AST
{
public:
    AlterTableActionAST() {}
    virtual AlterTableActionAST *asAlterTableAction() { return this; }
};

class AlterTableRenameActionAST: public AlterTableActionAST
{
public:
    int rename_token;
    int to_token;
    NameAST *newTableName;

public:
    AlterTableRenameActionAST( int rename_token_,
                               int to_token_,
                               NameAST *newTableName_ )
        : AlterTableActionAST()
        , rename_token(rename_token_)
        , to_token(to_token_)
        , newTableName(newTableName_)
    {}

    virtual AlterTableRenameActionAST *asAlterTableRenameAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableRenameColumnActionAST: public AlterTableActionAST
{
public:
    int rename_token;
    int column_token;
    NameAST *columnName;
    int to_token;
    NameAST *newColumnName;

public:
    AlterTableRenameColumnActionAST( int rename_token_,
                                     int column_token_,
                                     NameAST *columnName_,
                                     int to_token_,
                                     NameAST *newColumnName_ )
        : AlterTableActionAST()
        , rename_token(rename_token_)
        , column_token(column_token_)
        , columnName(columnName_)
        , to_token(to_token_)
        , newColumnName(newColumnName_)
    {}

    virtual AlterTableRenameColumnActionAST *asAlterTableRenameColumnAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableRenameConstraintActionAST: public AlterTableActionAST
{
public:
    int rename_token;
    int constraint_token;
    NameAST *constraintName;
    int to_token;
    NameAST *newConstraintName;

public:
    AlterTableRenameConstraintActionAST( int rename_token_,
                                         int constraint_token_,
                                         NameAST *constraintName_,
                                         int to_token_,
                                         NameAST *newConstraintName_ )
        : AlterTableActionAST()
        , rename_token(rename_token_)
        , constraint_token(constraint_token_)
        , constraintName(constraintName_)
        , to_token(to_token_)
        , newConstraintName(newConstraintName_)
    {}

    virtual AlterTableRenameConstraintActionAST *asAlterTableRenameConstraintAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAddConstraintActionAST: public AlterTableActionAST
{
public:
    int add_token;
    TableConstraintAST *constraint;
    NotValidClauseAST *notValid;

public:
    AlterTableAddConstraintActionAST( int add_token_,
                                      TableConstraintAST *constraint_,
                                      NotValidClauseAST *notValid_ )
        : AlterTableActionAST()
        , add_token(add_token_)
        , constraint(constraint_)
        , notValid(notValid_)
    {}

    virtual AlterTableAddConstraintActionAST *asAlterTableAddConstraintAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAddColumnActionAST: public AlterTableActionAST
{
public:
    int add_token;
    int column_token;
    ColumnDefinitionAST *columnDefinition;

public:
    AlterTableAddColumnActionAST( int add_token_,
                                  int column_token_,
                                  ColumnDefinitionAST *columnDefinition_ )
        : AlterTableActionAST()
        , add_token(add_token_)
        , column_token(column_token_)
        , columnDefinition(columnDefinition_)
    {}

    virtual AlterTableAddColumnActionAST *asAlterTableAddColumnAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableDropColumnActionAST: public AlterTableActionAST
{
public:
    int drop_token;
    int column_token;
    IfExistsAST *ifExists;
    NameAST *columnName;
    int restrict_or_cascade_token;

public:
    AlterTableDropColumnActionAST( int drop_token_,
                                   int column_token_,
                                   IfExistsAST *ifExists_,
                                   NameAST *columnName_,
                                   int restrict_or_cascade_token_ )
        : AlterTableActionAST()
        , drop_token(drop_token_)
        , column_token(column_token_)
        , ifExists(ifExists_)
        , columnName(columnName_)
        , restrict_or_cascade_token(restrict_or_cascade_token_)
    {}

    virtual AlterTableDropColumnActionAST *asAlterTableDropColumnAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableDropConstraintActionAST: public AlterTableActionAST
{
public:
    int drop_token;
    int constraint_token;
    IfExistsAST *ifExists;
    NameAST *constraintName;
    int restrict_or_cascade_token;

public:
    AlterTableDropConstraintActionAST( int drop_token_,
                                       int constraint_token_,
                                       IfExistsAST *ifExists_,
                                       NameAST *constraintName_,
                                       int restrict_or_cascade_token_ )
        : AlterTableActionAST()
        , drop_token(drop_token_)
        , constraint_token(constraint_token_)
        , ifExists(ifExists_)
        , constraintName(constraintName_)
        , restrict_or_cascade_token(restrict_or_cascade_token_)
    {}

    virtual AlterTableDropConstraintActionAST *asAlterTableDropConstraintAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAbleTriggerActionAST: public AlterTableActionAST
{
public:
    int enable_or_disable_token;
    int trigger_token;
    TriggerWildcardAST *wildcard;

public:
    AlterTableAbleTriggerActionAST( int enable_or_disable_token_,
                                    int trigger_token_,
                                    TriggerWildcardAST *wildcard_ )
        : AlterTableActionAST()
        , enable_or_disable_token(enable_or_disable_token_)
        , trigger_token(trigger_token_)
        , wildcard(wildcard_)
    {}

    virtual AlterTableAbleTriggerActionAST *asAlterTableAbleTriggerAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAlterColumnActionAST: public AlterTableActionAST
{
public:
    int alter_token;
    int column_token;
    NameAST *columnName;
    AlterColumnActionAST *action;

public:
    AlterTableAlterColumnActionAST( int alter_token_,
                                    int column_token_,
                                    NameAST *columnName_,
                                    AlterColumnActionAST *action_ )
        : AlterTableActionAST()
        , alter_token(alter_token_)
        , column_token(column_token_)
        , columnName(columnName_)
        , action(action_)
    {}

    virtual AlterTableAlterColumnActionAST *asAlterTableAlterColumnAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterColumnActionAST: public AST
{
public:
    AlterColumnActionAST() {}
    virtual AlterColumnActionAST *asAlterColumnAction() { return this; }
};

class AlterColumnSetTypeAST: public AlterColumnActionAST
{
public:
    int set_token;
    int data_token;
    int type_token;
    BaseTypeAST *type;
    CollateClauseAST *collate;
    int using_token;
    ValueExpressionAST *usingExpr;

public:
    AlterColumnSetTypeAST( int set_token_,
                           int data_token_,
                           int type_token_,
                           BaseTypeAST *type_,
                           CollateClauseAST *collate_,
                           int using_token_,
                           ValueExpressionAST *usingExpr_ )
        : AlterColumnActionAST()
        , set_token(set_token_)
        , data_token(data_token_)
        , type_token(type_token_)
        , type(type_)
        , collate(collate_)
        , using_token(using_token_)
        , usingExpr(usingExpr_)
    {}

    virtual AlterColumnSetTypeAST *asAlterColumnSetType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterColumnSetDefaultAST: public AlterColumnActionAST
{
public:
    int set_token;
    int default_token;
    ValueExpressionAST *defaultExpr;

public:
    AlterColumnSetDefaultAST( int set_token_,
                              int default_token_,
                              ValueExpressionAST *defaultExpr_ )
        : AlterColumnActionAST()
        , set_token(set_token_)
        , default_token(default_token_)
        , defaultExpr(defaultExpr_)
    {}

    virtual AlterColumnSetDefaultAST *asAlterColumnSetDefault() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterColumnDropDefaultAST: public AlterColumnActionAST
{
public:
    int drop_token;
    int default_token;

public:
    AlterColumnDropDefaultAST( int drop_token_,
                               int default_token_ )
        : AlterColumnActionAST()
        , drop_token(drop_token_)
        , default_token(default_token_)
    {}

    virtual AlterColumnDropDefaultAST *asAlterColumnDropDefault() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterColumnSetOrDropNotNullAST: public AlterColumnActionAST
{
public:
    int set_or_drop_token;
    int not_token;
    int null_token;

public:
    AlterColumnSetOrDropNotNullAST( int set_or_drop_token_,
                                    int not_token_,
                                    int null_token_ )
        : AlterColumnActionAST()
        , set_or_drop_token(set_or_drop_token_)
        , not_token(not_token_)
        , null_token(null_token_)
    {}

    virtual AlterColumnSetOrDropNotNullAST *asAlterColumnSetOrDropNotNull() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentStatementAST: public DdlStatementAST
{
public:
    CommentStatementAST() : DdlStatementAST() {}
    CommentStatementAST *asCommentStatement() { return this; }
};

class CommentOnTableStatementAST: public CommentStatementAST
{
public:
    int comment_token;
    int on_token;
    int table_token;
    TableIdentifierAST *tableIdent;
    int is_token;
    LiteralExpressionAST *comment;

public:
    CommentOnTableStatementAST( int comment_token_,
                                int on_token_,
                                int table_token_,
                                TableIdentifierAST *tableIdent_,
                                int is_token_,
                                LiteralExpressionAST *comment_ )
        : CommentStatementAST()
        , comment_token(comment_token_)
        , on_token(on_token_)
        , table_token(table_token_)
        , tableIdent(tableIdent_)
        , is_token(is_token_)
        , comment(comment_)
    {}

    virtual CommentOnTableStatementAST *asCommentOnTableStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentOnColumnStatementAST: public CommentStatementAST
{
public:
    int comment_token;
    int on_token;
    int column_token;
    ColumnIdentifierAST *columnIdent;
    int is_token;
    LiteralExpressionAST *comment;

public:
    CommentOnColumnStatementAST( int comment_token_,
                                 int on_token_,
                                 int column_token_,
                                 ColumnIdentifierAST *columnIdent_,
                                 int is_token_,
                                 LiteralExpressionAST *comment_ )
        : CommentStatementAST()
        , comment_token(comment_token_)
        , on_token(on_token_)
        , column_token(column_token_)
        , columnIdent(columnIdent_)
        , is_token(is_token_)
        , comment(comment_)
    {}

    virtual CommentOnColumnStatementAST *asCommentOnColumnStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommitStatementAST: public StatementAST
{
public:
    int commit_token;

public:
    CommitStatementAST( int commit_token_ )
        : StatementAST()
        , commit_token(commit_token_)
    {}

    virtual CommitStatementAST *asCommitStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RollbackStatementAST: public StatementAST
{
public:
    int rollback_token;

public:
    RollbackStatementAST( int rollback_token_ )
        : StatementAST()
        , rollback_token(rollback_token_)
    {}

    virtual RollbackStatementAST *asRollbackStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NameAST: public AST
{
public:
    enum Kind {
        Unknown = 0,
        Schema,
        Table,
        TableAlias,
        Column,
        ColumnAlias,
        Index,
        Trigger,
        Constraint,
        Binding,
        Label,
        Package,
        Routine,
        Type
    };

    Kind kind;
    int name_token;
    const Sql::Identifier *name;

public:
    NameAST( Kind kind_ = Unknown,
             int offset = 0,
             const Sql::Identifier *name_ = 0 )
        : kind(kind_)
        , name_token(offset)
        , name(name_)
    {}

    virtual NameAST *asName() { return this; }

    virtual int firstToken() const { return name_token; }
    virtual int lastToken() const { return name_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PlSqlBaseNameAST: public NameAST
{
public:
    PlSqlBaseNameAST() : NameAST() {}
    virtual PlSqlBaseNameAST *asPlSqlBaseName() { return this; }
};

class PlSqlNameAST: public PlSqlBaseNameAST
{
public:
    enum Sphere {
        Unknown = -1,
        Collection,
        Cursor,
        CursorVariable,
        CursorParameter,
        Table,
        Object,
        Record,
        RecordField,
        Variable,
        Constant,
        RoutineArgument,
        Function,
        Procedure,
        Package,
        Label,
        Savepoint,
        Exception
    };
    Sphere sphere;

public:
    PlSqlNameAST( Sphere sphere_ = Variable )
        : PlSqlBaseNameAST()
        , sphere(sphere_)
    {}

    virtual PlSqlNameAST *asPlSqlName() { return this; }
};

class PlSqlTypeNameAST: public PlSqlBaseNameAST
{
public:
    enum Sphere {
        Unknown = -1,
        Collection,
        Object,
        Record,
        RefCursor,
        Subtype,
        Element
    };
    Sphere sphere;

public:
    PlSqlTypeNameAST( Sphere sphere_ = Record )
        : PlSqlBaseNameAST()
        , sphere(sphere_)
    {}

    virtual PlSqlTypeNameAST *asPlSqlTypeNameAST() { return this; }
};

/// Expressions
class BaseExpressionAST: public AST
{
public:
    BaseExpressionAST() {}
    virtual BaseExpressionAST *asBaseExpression() { return this; }
};

class AssignmentExpressionAST: public BaseExpressionAST
{
public:
    ColumnIdentifierAST *columnIdent;
    int equal_token;
    ValueExpressionAST *rvExpr;

public:
    AssignmentExpressionAST( ColumnIdentifierAST *columnIdent_,
                             int equal_token_,
                             ValueExpressionAST *rvExpr_ )
        : BaseExpressionAST()
        , columnIdent(columnIdent_)
        , equal_token(equal_token_)
        , rvExpr(rvExpr_)
    {}

    virtual AssignmentExpressionAST *asAssignmentExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ValueExpressionAST: public BaseExpressionAST
{
public:
    ValueExpressionAST() : BaseExpressionAST() {}
    virtual ValueExpressionAST *asValueExpression() { return this; }
};

class PrimaryValueExpressionAST: public ValueExpressionAST
{
public:
    PrimaryValueExpressionAST()
        : ValueExpressionAST()
    {}

    virtual PrimaryValueExpressionAST *asSimpleValueExpression() { return this; }
};

class FunctionCallAST: public PrimaryValueExpressionAST
{
public:
    FunctionIdentifierAST *functionIdent;
    int lparen_token;
    ValueExpressionListAST *arg_list;
    int rparen_token;

public:
    FunctionCallAST( FunctionIdentifierAST *functionIdent_,
                     int lparen_token_,
                     ValueExpressionListAST *arg_list_,
                     int rparen_token_ )
        : PrimaryValueExpressionAST()
        , functionIdent(functionIdent_)
        , lparen_token(lparen_token_)
        , arg_list(finish(arg_list_))
        , rparen_token(rparen_token_)
    {}

    virtual FunctionCallAST *asFunctionCall() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CountStarCallAST: public PrimaryValueExpressionAST
{
public:
    int count_token;
    int lparen_token;
    int star_token;
    int rparen_token;

public:
    CountStarCallAST( int count_token_,
                      int lparen_token_,
                      int star_token_,
                      int rparen_token_ )
        : PrimaryValueExpressionAST()
        , count_token(count_token_)
        , lparen_token(lparen_token_)
        , star_token(star_token_)
        , rparen_token(rparen_token_)
    {}

    virtual CountStarCallAST *asCountStarCall() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CountExprCallAST: public PrimaryValueExpressionAST
{
public:
    int count_token;
    int lparen_token;
    int distinct_or_all_token;
    ValueExpressionAST *countExpr;
    int rparen_token;

public:
    CountExprCallAST( int count_token_,
                      int lparen_token_,
                      int distinct_or_all_token_,
                      ValueExpressionAST *countExpr_,
                      int rparen_token_ )
        : PrimaryValueExpressionAST()
        , count_token(count_token_)
        , lparen_token(lparen_token_)
        , distinct_or_all_token(distinct_or_all_token_)
        , countExpr(countExpr_)
        , rparen_token(rparen_token_)
    {}

    virtual CountExprCallAST *asCountExprCall() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CastFunctionAST: public PrimaryValueExpressionAST
{
public:
    int cast_token;
    int lparen_token;
    ValueExpressionAST *castExpr;
    int as_token;
    BaseTypeAST *typeSpec;
    int rparen_token;

public:
    CastFunctionAST( int cast_token_,
                     int lparen_token_,
                     ValueExpressionAST *castExpr_,
                     int as_token_,
                     BaseTypeAST *typeSpec_,
                     int rparen_token_ )
        : PrimaryValueExpressionAST()
        , cast_token(cast_token_)
        , lparen_token(lparen_token_)
        , castExpr(castExpr_)
        , as_token(as_token_)
        , typeSpec(typeSpec_)
        , rparen_token(rparen_token_)
    {}

    virtual CastFunctionAST *asCastFunction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubstringFunctionAST: public PrimaryValueExpressionAST
{
public:
    int substring_token;
    int lparen_token;
    ValueExpressionAST *strExpr;
    int from_token;
    ValueExpressionAST *startExpr;
    int for_token;
    ValueExpressionAST *lengthExpr;
    int rparen_token;

public:
    SubstringFunctionAST( int substring_token_,
                          int lparen_token_,
                          ValueExpressionAST *strExpr_,
                          int from_token_,
                          ValueExpressionAST *startExpr_,
                          int for_token_,
                          ValueExpressionAST *lengthExpr_,
                          int rparen_token_ )
        : PrimaryValueExpressionAST()
        , substring_token(substring_token_)
        , lparen_token(lparen_token_)
        , strExpr(strExpr_)
        , from_token(from_token_)
        , startExpr(startExpr_)
        , for_token(for_token_)
        , lengthExpr(lengthExpr_)
        , rparen_token(rparen_token_)
    {}

    virtual SubstringFunctionAST *asSubstringFunction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundValueExpressionAST: public PrimaryValueExpressionAST
{
public:
    int lparen_token;
    ValueExpressionAST *valExpr;
    int rparen_token;

public:
    CompoundValueExpressionAST( int lparen_token_,
                                ValueExpressionAST *valExpr_,
                                int rparen_token_ )
        : PrimaryValueExpressionAST()
        , lparen_token(lparen_token_)
        , valExpr(valExpr_)
        , rparen_token(rparen_token_)
    {}

    virtual CompoundValueExpressionAST *asCompoundValueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BuiltinFunctionCallAST: public PrimaryValueExpressionAST
{
public:
    int function_name_token;
    int lparen_token;
    ValueExpressionListAST *arg_list;
    int rparen_token;

public:
    BuiltinFunctionCallAST( int function_name_token_,
                            int lparen_token_,
                            ValueExpressionListAST *arg_list_,
                            int rparen_token_ )
        : PrimaryValueExpressionAST()
        , function_name_token(function_name_token_)
        , lparen_token(lparen_token_)
        , arg_list(finish(arg_list_))
        , rparen_token(rparen_token_)
    {}

    virtual BuiltinFunctionCallAST *asBuiltinFunctionCall() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UnaryValueExpressionAST: public ValueExpressionAST
{
public:
    int op_token;
    ValueExpressionAST *valExpr;

public:
    UnaryValueExpressionAST( unsigned op_token_,
                             ValueExpressionAST *valExpr_ )
        : ValueExpressionAST()
        , op_token(op_token_)
        , valExpr(valExpr_)
    {}

    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BinaryValueExpressionAST: public ValueExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    int op_token;
    ValueExpressionAST *rvExpr;

public:
    BinaryValueExpressionAST( ValueExpressionAST *lvExpr_,
                              int op_token_,
                              ValueExpressionAST *rvExpr_ )
        : ValueExpressionAST()
        , lvExpr(lvExpr_)
        , op_token(op_token_)
        , rvExpr(rvExpr_)
    {}

    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstantExpressionAST: public PrimaryValueExpressionAST
{
public:
    ConstantExpressionAST()
        : PrimaryValueExpressionAST()
    {}

    virtual ConstantExpressionAST *asConstantExpression() { return this; }
};

class LiteralExpressionAST: public ConstantExpressionAST
{
public:
    int liter_token;

public:
    LiteralExpressionAST( int offset )
        : ConstantExpressionAST()
        , liter_token(offset)
    {}

    virtual LiteralExpressionAST *asLiteralExpression() { return this; }

    virtual int firstToken() const { return liter_token; }
    virtual int lastToken() const { return liter_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NumericExpressionAST: public ConstantExpressionAST
{
public:
    int numeric_token;

public:
    NumericExpressionAST( int numeric_token_ )
        : ConstantExpressionAST()
        , numeric_token(numeric_token_)
    {}

    virtual NumericExpressionAST *asNumericExpression() { return this; }

    virtual int firstToken() const { return numeric_token; }
    virtual int lastToken() const { return numeric_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NullValueExpressionAST: public ConstantExpressionAST
{
public:
    int null_token;

public:
    NullValueExpressionAST( int null_token_ )
        : ConstantExpressionAST()
        , null_token(null_token_)
    {}

    virtual NullValueExpressionAST *asNullValueExpression() { return this; }

    virtual int firstToken() const { return null_token; }
    virtual int lastToken() const { return null_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ParameterExpressionAST: public ConstantExpressionAST
{
public:
    int colon_token;
    NameAST *bindName;

public:
    ParameterExpressionAST( int colon_token_,
                            NameAST *bindName_ )
        : ConstantExpressionAST()
        , colon_token(colon_token_)
        , bindName(bindName_)
    {}

    virtual ParameterExpressionAST *asParameterExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCompoundValueExpressionAST: public PrimaryValueExpressionAST
{
public:
    int lparen_token;
    SelectStatementAST *query;
    int rparen_token;

public:
    SelectCompoundValueExpressionAST( int lparen_token_,
                                      SelectStatementAST *query_,
                                      int rparen_token_ )
        : PrimaryValueExpressionAST()
        , lparen_token(lparen_token_)
        , query(query_)
        , rparen_token(rparen_token_)
    {}

    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseExpressionAST: public ValueExpressionAST
{
public:
    int case_token;
    ValueExpressionAST *caseExpr;
    CaseExpressionClauseListAST *var_list;
    int else_token;
    ValueExpressionAST *elseExpr;
    int end_token;

public:
    CaseExpressionAST( int case_token_,
                       ValueExpressionAST *caseExpr_,
                       CaseExpressionClauseListAST *var_list_,
                       int else_token_,
                       ValueExpressionAST *elseExpr_,
                       int end_token_ )
        : ValueExpressionAST()
        , case_token(case_token_)
        , caseExpr(caseExpr_)
        , var_list(finish(var_list_))
        , else_token(else_token_)
        , elseExpr(elseExpr_)
        , end_token(end_token_)
    {}

    virtual CaseExpressionAST *asCaseExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseConditionAST: public ValueExpressionAST
{
public:
    int case_token;
    CaseConditionClauseListAST *var_list;
    int else_token;
    ValueExpressionAST *elseExpr;
    int end_token;

public:
    CaseConditionAST( int case_token_,
                      CaseConditionClauseListAST *var_list_,
                      int else_token_,
                      ValueExpressionAST *elseExpr_,
                      int end_token_ )
        : ValueExpressionAST()
        , case_token(case_token_)
        , var_list(finish(var_list_))
        , else_token(else_token_)
        , elseExpr(elseExpr_)
        , end_token(end_token_)
    {}

    virtual CaseConditionAST *asCaseCondition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundRValueExpressionAST: public BaseExpressionAST
{
public:
    unsigned lparen_token;
    unsigned rparen_roken;

public:
    CompoundRValueExpressionAST( unsigned lparen_token_,
                                 unsigned rparen_token_ )
        : BaseExpressionAST()
        , lparen_token(lparen_token_)
        , rparen_roken(rparen_token_)
    {}

    virtual CompoundRValueExpressionAST *asCompoundRValueExpression() { return this; }
};

class ValueListExpressionAST: public CompoundRValueExpressionAST
{
public:
    ValueExpressionListAST *val_list;

public:
    ValueListExpressionAST( unsigned lparen_token_,
                            ValueExpressionListAST *val_list_,
                            unsigned rparen_token_ )
        : CompoundRValueExpressionAST(lparen_token_, rparen_token_)
        , val_list(finish(val_list_))
    {}

    virtual ValueListExpressionAST *asValueListExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubQueryExpressionAST: public CompoundRValueExpressionAST
{
public:
    SelectStatementAST *query;

public:
    SubQueryExpressionAST( int lparen_token_,
                           SelectStatementAST *query_,
                           int rparen_roken_ )
        : CompoundRValueExpressionAST(lparen_token_, rparen_roken_)
        , query(query_)
    {}

    virtual SubQueryExpressionAST *asSubQueryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConditionExpressionAST: public BaseExpressionAST
{
public:
    ConditionExpressionAST()
        : BaseExpressionAST()
    {}

    virtual ConditionExpressionAST *asConditionExpression() { return this; }
};

class NotConditionExpressionAST: public ConditionExpressionAST
{
public:
    int not_token;
    ConditionExpressionAST *condition;

public:
    NotConditionExpressionAST( int not_token_,
                               ConditionExpressionAST *condition_ )
        : ConditionExpressionAST()
        , not_token(not_token_)
        , condition(condition_)
    {}

    virtual NotConditionExpressionAST *asNotConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TrueConditionExpressionAST: public ConditionExpressionAST
{
public:
    int true_token;

public:
    TrueConditionExpressionAST( int true_token_ )
        : ConditionExpressionAST()
        , true_token(true_token_)
    {}

    virtual TrueConditionExpressionAST *asTrueConditionExpression() { return this; }

    virtual int firstToken() const { return true_token; }
    virtual int lastToken() const { return true_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FalseConditionExpressionAST: public ConditionExpressionAST
{
public:
    int false_token;

public:
    FalseConditionExpressionAST( int false_token_ )
        : ConditionExpressionAST()
        , false_token(false_token_)
    {}

    virtual FalseConditionExpressionAST *asFalseConditionExpression() { return this; }

    virtual int firstToken() const { return false_token; }
    virtual int lastToken() const { return false_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BinaryConditionExpressionAST: public ConditionExpressionAST
{
public:
    ConditionExpressionAST *lvCondExpr;
    int cond_op_token;
    ConditionExpressionAST *rvCondExpr;

public:
    BinaryConditionExpressionAST( ConditionExpressionAST *lvCondExpr_,
                                  int cond_op_token_,
                                  ConditionExpressionAST *rvCondExpr_ )
        : ConditionExpressionAST()
        , lvCondExpr(lvCondExpr_)
        , cond_op_token(cond_op_token_)
        , rvCondExpr(rvCondExpr_)
    {}

    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompareConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    int compare_op_token;
    ValueExpressionAST *rvExpr;

public:
    CompareConditionExpressionAST( ValueExpressionAST *lvExpr_,
                                   int compare_op_token_,
                                   ValueExpressionAST *rvExpr_)
        : ConditionExpressionAST()
        , lvExpr(lvExpr_)
        , compare_op_token(compare_op_token_)
        , rvExpr(rvExpr_)
    {}

    virtual CompareConditionExpressionAST *asCompareConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundConditionExpressionAST: public ConditionExpressionAST
{
public:
    unsigned lparen_token;
    ConditionExpressionAST *condExpr;
    unsigned rparen_token;

public:
    CompoundConditionExpressionAST( unsigned lparen_token_,
                                    ConditionExpressionAST *condExpr_,
                                    unsigned rparen_token_ )
        : ConditionExpressionAST()
        , lparen_token(lparen_token_)
        , condExpr(condExpr_)
        , rparen_token(rparen_token_)
    {}

    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LikeConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    int not_token;
    int like_token;
    ValueExpressionAST *rvExpr;

public:
    LikeConditionExpressionAST( ValueExpressionAST *lvExpr_,
                                int not_token_,
                                int like_token_,
                                ValueExpressionAST *rvExpr_ )
        : ConditionExpressionAST()
        , lvExpr(lvExpr_)
        , not_token(not_token_)
        , like_token(like_token_)
        , rvExpr(rvExpr_)
    {}

    virtual LikeConditionExpressionAST *asLikeConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BetweenConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    int not_token;
    int between_token;
    ValueExpressionAST *rv1Expr;
    int and_token;
    ValueExpressionAST *rv2Expr;

public:
    BetweenConditionExpressionAST( ValueExpressionAST *lvExpr_,
                                   int not_token_,
                                   int between_token_,
                                   ValueExpressionAST *rv1Expr_,
                                   int and_token_,
                                   ValueExpressionAST *rv2Expr_ )
        : ConditionExpressionAST()
        , lvExpr(lvExpr_)
        , not_token(not_token_)
        , between_token(between_token_)
        , rv1Expr(rv1Expr_)
        , and_token(and_token_)
        , rv2Expr(rv2Expr_)
    {}

    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    int not_token;
    int in_token;
    CompoundRValueExpressionAST *rvExpr;

public:
    InConditionExpressionAST( ValueExpressionAST *lvExpr_,
                              int not_token_,
                              int in_token_,
                              CompoundRValueExpressionAST *rvExpr_ )
        : ConditionExpressionAST()
        , lvExpr(lvExpr_)
        , not_token(not_token_)
        , in_token(in_token_)
        , rvExpr(rvExpr_)
    {}

    virtual InConditionExpressionAST *asInConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExistsConditionExpressionAST: public ConditionExpressionAST
{
public:
    int exists_token;
    SubQueryExpressionAST *rvExpr;

public:
    ExistsConditionExpressionAST( int exists_token_,
                                  SubQueryExpressionAST *rvExpr_ )
        : ConditionExpressionAST()
        , exists_token(exists_token_)
        , rvExpr(rvExpr_)
    {}

    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IsNullConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    int is_token;
    int not_token;
    int null_token;

public:
    IsNullConditionExpressionAST( ValueExpressionAST *lvExpr_,
                                  int is_token_,
                                  int not_token_,
                                  int null_token_ )
        : ConditionExpressionAST()
        , lvExpr(lvExpr_)
        , is_token(is_token_)
        , not_token(not_token_)
        , null_token(null_token_)
    {}

    virtual IsNullConditionExpressionAST *asIsNullConditionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConditionAsValueExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *expr;

public:
    ConditionAsValueExpressionAST( ValueExpressionAST *expr_ )
        : ConditionExpressionAST()
        , expr(expr_)
    {}

    virtual ConditionAsValueExpressionAST *asConditionAsValueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseExpressionClauseAST: public AST
{
public:
    int when_token;
    ValueExpressionAST *whenExpr;
    int then_token;
    ValueExpressionAST *thenExpr;

public:
    CaseExpressionClauseAST( int when_token_,
                             ValueExpressionAST *whenExpr_,
                             int then_token_,
                             ValueExpressionAST *thenExpr_ )
        : when_token(when_token_)
        , whenExpr(whenExpr_)
        , then_token(then_token_)
        , thenExpr(thenExpr_)
    {}

    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseConditionClauseAST: public AST
{
public:
    int when_token;
    ConditionExpressionAST *whenExpr;
    int then_token;
    ValueExpressionAST *thenExpr;

public:
    CaseConditionClauseAST( int when_token_,
                            ConditionExpressionAST *whenExpr_,
                            int then_token_,
                            ValueExpressionAST *thenExpr_ )
        : when_token(when_token_)
        , whenExpr(whenExpr_)
        , then_token(then_token_)
        , thenExpr(thenExpr_)
    {}

    virtual CaseConditionClauseAST *asCaseConditionClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int schema_dot_token;
    NameAST *tableName;

public:
    TableIdentifierAST( NameAST *schemaName_,
                        int schema_dot_token_,
                        NameAST *tableName_ )
        : schemaName(schemaName_)
        , schema_dot_token(schema_dot_token_)
        , tableName(tableName_)
    {}

    virtual TableIdentifierAST *asTableIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndexIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int schema_dot_token;
    NameAST *indexName;

public:
    IndexIdentifierAST( NameAST *schemaName_,
                        int schema_dot_token_,
                        NameAST *indexName_ )
        : schemaName(schemaName_)
        , schema_dot_token(schema_dot_token_)
        , indexName(indexName_)
    {}

    virtual IndexIdentifierAST *asIndexIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int schema_dot_token;
    NameAST *triggerName;

public:
    TriggerIdentifierAST( NameAST *schemaName_,
                          int schema_dot_token_,
                          NameAST *triggerName_ )
        : schemaName(schemaName_)
        , schema_dot_token(schema_dot_token_)
        , triggerName(triggerName_)
    {}

    virtual TriggerIdentifierAST *asTriggerIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnIdentifierAST: public PrimaryValueExpressionAST
{
public:
    NameAST *schemaName;
    int schema_dot_token;
    NameAST *tableName;
    int table_dot_token;
    NameAST *columnName;

public:
    ColumnIdentifierAST( NameAST *schemaName_ = 0,
                         int schema_dot_token_ = 0,
                         NameAST *tableName_ = 0,
                         int table_dot_token_ = 0,
                         NameAST *columnName_ = 0 )
        : PrimaryValueExpressionAST()
        , schemaName(schemaName_)
        , schema_dot_token(schema_dot_token_)
        , tableName(tableName_)
        , table_dot_token(table_dot_token_)
        , columnName(columnName_)
    {}

    virtual ColumnIdentifierAST *asColumnIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int schema_dot_token;
    NameAST *packageName;
    int pck_dot_token;
    NameAST *functionName;

public:
    FunctionIdentifierAST( NameAST *schemaName_,
                           int schema_dot_token_,
                           NameAST *packageName_,
                           int pck_dot_token_,
                           NameAST *functionName_ )
        : schemaName(schemaName_)
        , schema_dot_token(schema_dot_token_)
        , packageName(packageName_)
        , pck_dot_token(pck_dot_token_)
        , functionName(functionName_)
    {}

    virtual FunctionIdentifierAST *asFunctionIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollateClauseAST: public AST
{
public:
    int collate_token;
    LiteralExpressionAST *collateAst;

public:
    CollateClauseAST( int collate_token_,
                      LiteralExpressionAST *collateAst_ )
        : collate_token(collate_token_)
        , collateAst(collateAst_)
    {}

    virtual CollateClauseAST *asCollateClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseTypeAST: public AST
{
public:
    BaseTypeAST() {}
    virtual BaseTypeAST *asBaseType() { return this; }
};

class CharacterTypeAST: public BaseTypeAST
{
public:
    int type_token;
    int lparen_token;
    int size_token;
    int rparen_token;

public:
    CharacterTypeAST( int type_token_,
                      int lparen_token_,
                      int size_token_,
                      int rparen_token_ )
        : BaseTypeAST()
        , type_token(type_token_)
        , lparen_token(lparen_token_)
        , size_token(size_token_)
        , rparen_token(rparen_token_)
    {}

    virtual CharacterTypeAST *asCharacterType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NumberTypeAST: public BaseTypeAST
{
public:
    int type_token;
    int lparen_token;
    int size_token;
    int comma_token;
    int prec_token;
    int rparen_token;

public:
    NumberTypeAST( int type_token_,
                   int lparen_token_,
                   int size_token_,
                   int comma_token_,
                   int prec_token_,
                   int rparen_token_ )
        : BaseTypeAST()
        , type_token(type_token_)
        , lparen_token(lparen_token_)
        , size_token(size_token_)
        , comma_token(comma_token_)
        , prec_token(prec_token_)
        , rparen_token(rparen_token_)
    {}

    virtual NumberTypeAST *asNumberType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleTypeAST: public BaseTypeAST
{
public:
    int type_token;

public:
    SimpleTypeAST( int type_token_ )
        : BaseTypeAST()
        , type_token(type_token_)
    {}

    virtual SimpleTypeAST *asSimpleType() { return this; }

    virtual int firstToken() const { return type_token; }
    virtual int lastToken() const { return type_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UserDefinedTypeAST: public BaseTypeAST
{
public:
    NameAST *typeName;

public:
    UserDefinedTypeAST( NameAST *typeName_ )
        : BaseTypeAST()
        , typeName(typeName_)
    {}

    virtual UserDefinedTypeAST *asUserDefinedType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintDeclarationAST: public AST
{
public:
    int constraint_token;
    NameAST *constraintName;

public:
    ConstraintDeclarationAST( int constraint_token_,
                              NameAST *constraintName_ )
        : constraint_token(constraint_token_)
        , constraintName(constraintName_)
    {}

    virtual ConstraintDeclarationAST *asConstraintDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintAST: public AST
{
public:
    ConstraintDeclarationAST *constraintDecl;

public:
    ConstraintAST( ConstraintDeclarationAST *constraintDecl_ )
        : constraintDecl(constraintDecl_)
    {}

    virtual ConstraintAST *asConstraint() { return this; }
};

class TableConstraintAST: public ConstraintAST
{
public:
    enum TableConstraintType {
        Unknown = 0,
        UniqueOrPrimaryKey,
        Check,
        ForeignKey
    };

    TableConstraintAST( ConstraintDeclarationAST *constraintDecl_, TableConstraintType constraintType_ )
        : ConstraintAST(constraintDecl_)
        , constraintType(constraintType_)
    {}

    inline TableConstraintType tableConstraintType() const { return constraintType; }

    virtual TableConstraintAST *asTableConstraint() { return this; }

private:
    TableConstraintType constraintType;
};

class SortedColumnNameAST: public AST
{
public:
    NameAST *columnName;
    int asc_or_desc_token;

public:
    SortedColumnNameAST( NameAST *columnName_,
                         int asc_or_desc_token_ )
        : columnName(columnName_)
        , asc_or_desc_token(asc_or_desc_token_)
    {}

    virtual SortedColumnNameAST *asSortedColumnName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueOrPkTableConstraintAST: public TableConstraintAST
{
public:
    int unique_or_primary_token;
    int key_token;
    int lparen_token;
    SortedColumnNameListAST *column_list;
    int rparen_token;

public:
    UniqueOrPkTableConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                                  int unique_or_primary_token_,
                                  int key_token_,
                                  int lparen_token_,
                                  SortedColumnNameListAST *column_list_,
                                  int rparen_token_ )
        : TableConstraintAST(constraintDecl_, TableConstraintAST::UniqueOrPrimaryKey)
        , unique_or_primary_token(unique_or_primary_token_)
        , key_token(key_token_)
        , lparen_token(lparen_token_)
        , column_list(finish(column_list_))
        , rparen_token(rparen_token_)
    {}

    virtual UniqueOrPkTableConstraintAST *asUniqueOrPkTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueOrPkTableConstraintUsingIndexAST: public TableConstraintAST
{
public:
    int unique_or_primary_token;
    int key_token;
    int using_token;
    int index_token;
    IndexIdentifierAST *indexIdent;
    DeferrableClauseAST *defer;

public:
    UniqueOrPkTableConstraintUsingIndexAST( ConstraintDeclarationAST *constraintDecl_,
                                            int unique_or_primary_token_,
                                            int key_token_,
                                            int using_token_,
                                            int index_token_,
                                            IndexIdentifierAST *indexIdent_,
                                            DeferrableClauseAST *defer_ )
        : TableConstraintAST(constraintDecl_, TableConstraintAST::UniqueOrPrimaryKey)
        , unique_or_primary_token(unique_or_primary_token_)
        , key_token(key_token_)
        , using_token(using_token_)
        , index_token(index_token_)
        , indexIdent(indexIdent_)
        , defer(defer_)
    {}

    virtual UniqueOrPkTableConstraintUsingIndexAST *asUniqueOrPkTableConstraintUsingIndex() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckTableConstraintAST: public TableConstraintAST
{
public:
    int check_token;
    int lparen_token;
    ConditionExpressionAST *checkExpr;
    int rparen_token;

public:
    CheckTableConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                             int check_token_,
                             int lparen_token_,
                             ConditionExpressionAST *checkExpr_,
                             int rparen_token_ )
        : TableConstraintAST(constraintDecl_, TableConstraintAST::Check)
        , check_token(check_token_)
        , lparen_token(lparen_token_)
        , checkExpr(checkExpr_)
        , rparen_token(rparen_token_)
    {}

    virtual CheckTableConstraintAST *asCheckTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FkReferencesClauseAST: public AST
{
public:
    int references_token;
    TableIdentifierAST *tableIdent;
    int ref_lparen_token;
    NameListAST *ref_col_list;
    int ref_rparen_token;
    FkReferencesMatchAST *matchClause;
    FkReferencesActionAST *actionClause;

public:
    FkReferencesClauseAST( int references_token_,
                           TableIdentifierAST *tableIdent_,
                           int ref_lparen_token_,
                           NameListAST *ref_col_list_,
                           int ref_rparen_token_,
                           FkReferencesMatchAST *matchClause_,
                           FkReferencesActionAST *actionClause_ )
        : references_token(references_token_)
        , tableIdent(tableIdent_)
        , ref_lparen_token(ref_lparen_token_)
        , ref_col_list(finish(ref_col_list_))
        , ref_rparen_token(ref_rparen_token_)
        , matchClause(matchClause_)
        , actionClause(actionClause_)
    {}

    virtual FkReferencesClauseAST *asFkReferencesClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FkReferencesMatchAST: public AST
{
public:
    int match_token;
    int full_or_partial_or_simple_token;

public:
    FkReferencesMatchAST( int match_token_,
                          int full_or_partial_or_simple_token_ )
        : match_token(match_token_)
        , full_or_partial_or_simple_token(full_or_partial_or_simple_token_)
    {}

    virtual FkReferencesMatchAST *asFkReferencesMatch() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FkReferencesActionAST: public AST
{
public:
    int on_token;
    int delete_or_update_token;
    int act1_token;     // NO ACTION | RESTRICT | CASCADE | SET NULL | SET DEFAULT
    int act2_token;

public:
    FkReferencesActionAST( int on_token_,
                           int delete_or_update_token_,
                           int act1_token_,
                           int act2_token_ )
        : on_token(on_token_)
        , delete_or_update_token(delete_or_update_token_)
        , act1_token(act1_token_)
        , act2_token(act2_token_)
    {}

    virtual FkReferencesActionAST *asFkReferencesAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FkTableConstraintAST: public TableConstraintAST
{
public:
    int foreign_token;
    int key_token;
    int lparen_token;
    NameListAST *col_list;
    int rparen_token;
    FkReferencesClauseAST *refClause;

public:
    FkTableConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                          int foreign_token_,
                          int key_token_,
                          int lparen_token_,
                          NameListAST *col_list_,
                          int rparen_token_,
                          FkReferencesClauseAST *refClause_ )
        : TableConstraintAST(constraintDecl_, TableConstraintAST::ForeignKey)
        , foreign_token(foreign_token_)
        , key_token(key_token_)
        , lparen_token(lparen_token_)
        , col_list(finish(col_list_))
        , rparen_token(rparen_token_)
        , refClause(refClause_)
    {}

    virtual FkTableConstraintAST *asFkTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnConstraintAST: public ConstraintAST
{
public:
    ColumnConstraintAST( ConstraintDeclarationAST *constraintDecl_ )
        : ConstraintAST(constraintDecl_)
    {}

    virtual ColumnConstraintAST *asColumnConstraint() { return this; }
};

class PkColumnConstraintAST: public ColumnConstraintAST
{
public:
    int primary_token;
    int key_token;
    /// TODO: Add IndexParameters
    DeferrableClauseAST *defer;

public:
    PkColumnConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                           int primary_token_,
                           int key_token_,
                           DeferrableClauseAST *defer_ )
        : ColumnConstraintAST(constraintDecl_)
        , primary_token(primary_token_)
        , key_token(key_token_)
        , defer(defer_)
    {}

    virtual PkColumnConstraintAST *asPkColumnConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotNullColumnConstraintAST: public ColumnConstraintAST
{
public:
    int not_token;
    int null_token;
    DeferrableClauseAST *defer;

public:
    NotNullColumnConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                                int not_token_,
                                int null_token_,
                                DeferrableClauseAST *defer_ )
        : ColumnConstraintAST(constraintDecl_)
        , not_token(not_token_)
        , null_token(null_token_)
        , defer(defer_)
    {}

    virtual NotNullColumnConstraintAST *asNotNullColumnConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueColumnConstraintAST: public ColumnConstraintAST
{
public:
    int unique_token;
    DeferrableClauseAST *defer;

public:
    UniqueColumnConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                               int unique_token_,
                               DeferrableClauseAST *defer_ )
        : ColumnConstraintAST(constraintDecl_)
        , unique_token(unique_token_)
        , defer(defer_)
    {}

    virtual UniqueColumnConstraintAST *asUniqueColumnConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;


protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckColumnConstraintAST: public ColumnConstraintAST
{
public:
    int check_token;
    int lparen_token;
    ConditionExpressionAST *checkExpr;
    int rparen_token;
    DeferrableClauseAST *defer;

public:
    CheckColumnConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                              int check_token_,
                              int lparen_token_,
                              ConditionExpressionAST *checkExpr_,
                              int rparen_token_,
                              DeferrableClauseAST *defer_ )
        : ColumnConstraintAST(constraintDecl_)
        , check_token(check_token_)
        , lparen_token(lparen_token_)
        , checkExpr(checkExpr_)
        , rparen_token(rparen_token_)
        , defer(defer_)
    {}

    virtual CheckColumnConstraintAST *asCheckColumnConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReferencesColumnConstraintAST: public ColumnConstraintAST
{
public:
    FkReferencesClauseAST *refClause;
    DeferrableClauseAST *defer;

public:
    ReferencesColumnConstraintAST( ConstraintDeclarationAST *constraintDecl_,
                                   FkReferencesClauseAST *refClause_,
                                   DeferrableClauseAST *defer_ )
        : ColumnConstraintAST(constraintDecl_)
        , refClause(refClause_)
        , defer(defer_)
    {}

    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//class SqlCheckNamesWalker: public Sql::BaseWalker
//{
//public:
//    SqlCheckNamesWalker( AST *ast, Sql::TranslationUnit *translationUnit )
//        : BaseWalker()
//        , m_ast(ast)
//        , m_translationUnit(translationUnit)
//    {}

//    virtual void walk( Sql::BaseWalkReflector *reflector );

//private:
//    AST *m_ast;
//    Sql::TranslationUnit *m_translationUnit;
//};

}   // namespace Postgres

#endif // POSTGRESAST_H
