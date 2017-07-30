#ifndef QLALR_ORACLE_AST_H
#define QLALR_ORACLE_AST_H

#include <QString>

#include "QLalrOracleASTfwd.h"
#include "MemoryPool.h"
#include "TranslationUnit.h"
#include "SqlToken.h"
#include "BaseWalker.h"
#include "sql_constants.h"

namespace QLalrOracle {

template <typename _Tp>
class List: public Utils::Managed
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
        next = previous->next;
        previous->next = this;
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

class AST: public Utils::Managed
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
    enum Kind {
       Kind_UnaryPlus,
       Kind_UnaryMinus
    };

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
    virtual DmlStatementAST *asDmlStatement() { return 0; }
    virtual DdlStatementAST *asDdlStatement() { return 0; }
    virtual PlSqlBlockAST *asPlSqlBlock() { return 0; }

    virtual InvalidStatementAST *asInvalidStatement() { return 0; }

    virtual SelectStatementAST *asSelectStatement() { return 0; }
    virtual SelectCoreAST *asSelectCore() { return 0; }
    virtual OrderByBlockAST *asOrderByBlock() { return 0; }
    virtual ForUpdateClauseAST *asForUpdateClause() { return 0; }
    virtual OrderingTermAST *asOrderingTerm() { return 0; }
    virtual BaseSelectColumnsAST *asBaseSelectColumns() { return 0; }
    virtual StarSelectColumnAST *asStarSelectColumn() { return 0; }
    virtual ListSelectColumnAST *asListSelectColumn() { return 0; }
    virtual BaseSelectColumnItemAST *asBaseSelectColumnItem() { return 0; }
    virtual TableSelectColumnsItemAST *asTableSelectColumnsItem() { return 0; }
    virtual ValueSelectColumnItemAST *asValueSelectColumnItem() { return 0; }
    virtual WhereClauseAST *asWhereClause() { return 0; }
    virtual BaseConnectPriorClauseAST *asBaseConnectPriorClause() { return 0; }
    virtual StartConnectPriorAST *asStartConnectPriorAST() { return 0; }
    virtual EndConnectPriorAST *asEndConnectPrior() { return 0; }
    virtual StartWithClauseAST *asStartWithClause() { return 0; }
    virtual ConnectByClauseAST *asConnectByClause() { return 0; }
    virtual BaseSetSpecAST *asBaseSetSpec() { return 0; }
    virtual MinusSetSpecAST *asMinusSetSpec() { return 0; }
    virtual IntersectSetSpecAST *asIntersectSetSpec() { return 0; }
    virtual UnionSetSpecAST *asUnionSetSpec() { return 0; }
    virtual SetClauseAST *asSetClause() { return 0; }
    virtual JoinSourceAST *asJoinSource() { return 0; }
    virtual GroupByBlockAST *asGroupByBlock() { return 0; }
    virtual BaseJoinListAST *asBaseJoinList() { return 0; }
    virtual CompoundJoinListAST *asCompoundJoinList() { return 0; }
    virtual CommaJoinListAST *asCommaJoinList() { return 0; }
    virtual JoinJoinListAST *asJoinJoinList() { return 0; }

    virtual NameAST *asName() { return 0; }

    virtual DeleteStatementAST *asDeleteStatement() { return 0; }
    virtual InsertStatementAST *asInsertStatement() { return 0; }
    virtual InsertColumnsClauseAST *asInsertColumnsClause() { return 0; }
    virtual InsertClauseAST *asInsertClause() { return 0; }
    virtual InsertClauseAsValuesAST *asInsertClauseAsValues() { return 0; }
    virtual InsertClauseAsSelectAST *asInsertClauseAsSelect() { return 0; }
    virtual CreateTableClauseAST *asCreateTableClause() { return 0; }
    virtual CreateTableDirectClauseAST *asCreateTableDirectClause() { return 0; }
    virtual CreateTableAsSelectClauseAST *asCreateTableAsSelectClause() { return 0; }
    virtual CreateTriggerRefClauseAST *asCreateTriggerRefClause() { return 0; }
    virtual CreateRdbTriggerBodyAST *asCreateRdbTriggerBody() { return 0; }
    virtual CreateRdbTriggerBodyItemAST *asCreateRdbTriggerBodyItem() { return 0; }
    virtual CreateTriggerBodyAST *asCreateTriggerBody() { return 0; }
    virtual UpdateStatementAST *asUpdateStatement() { return 0; }
    virtual UpdateSetClauseAST *asUpdateSetClause() { return 0; }
    virtual ColumnDefinitionAST *asColumnDefinition() { return 0; }
    virtual ConstraintDeferStateAST *asConstraintDeferState() { return 0; }
    virtual UsingIndexClauseAST *asUsingIndexClause() { return 0; }
    virtual ExceptionsClauseAST *asExceptionsClause() { return 0; }
    virtual ConstraintStateAST *asConstraintState() { return 0; }
    virtual ConstraintAST *asConstraint() { return 0; }
    virtual TableConstraintAST *asTableConstraint() { return 0; }
    virtual ColumnConstraintAST *asColumnConstraint() { return 0; }
    virtual UniqueOrPkTableConstraintAST *asUniqueOrPkTableConstraint() { return 0; }
    virtual CheckTableConstraintAST *asCheckTableConstraint() { return 0; }
    virtual FkTableConstraintAST *asFkTableConstraint() { return 0; }
    virtual FkReferencesClauseAST *asForeignKeyClause() { return 0; }
    virtual PkColumnConstraintAST *asPkColumnConstraint() { return 0; }
    virtual NotNullColumnConstraintAST *asNotNullColumnConstraint() { return 0; }
    virtual UniqueColumnConstraintAST *asUniqueColumnConstraint() { return 0; }
    virtual CheckColumnConstraintAST *asCheckColumnConstraint() { return 0; }
    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return 0; }
    virtual SortedColumnNameAST *asSortedColumnName() { return 0; }

    virtual CreateStatementAST *asCreateStatement() { return 0; }
    virtual CreateTableStatementAST *asCreateTableStatement() { return 0; }
    virtual CreateViewStatementAST *asCreateViewStatement() { return 0; }
    virtual CreateIndexStatementAST *asCreateIndexStatement() { return 0; }
    virtual BaseCreateIndexClauseAST *asBaseCreateIndexClause() { return 0; }
    virtual TableCreateIndexClauseAST *asTableCreateIndexClause() { return 0; }
    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return 0; }
    virtual DropStatementAST *asDropStatement() { return 0; }
    virtual DropTableStatementAST *asDropTableStatement() { return 0; }
    virtual DropViewStatementAST *asDropViewStatement() { return 0; }
    virtual DropIndexStatementAST *asDropIndexStatement() { return 0; }
    virtual DropTriggerStatementAST *asDropTriggerStatement() { return 0; }
    virtual TruncateStatementAST *asTruncateStatement() { return 0; }
    virtual AlterStatementAST *asAlterStatement() { return 0; }
    virtual AlterTableStatementAST *asAlterTableStatement() { return 0; }
    virtual AlterTableClauseAST *asAlterTableClause() { return 0; }
    virtual AlterTableDropClauseAST *asAlterTableDropClause() { return 0; }
    virtual AlterTableAddClauseAST *asAlterTableAddClause() { return 0; }
    virtual AlterTableAddConstraintClauseAST *asAlterTableAddConstraintClause() { return 0; }
    virtual AlterTableAddColumnClauseAST *asAlterTableAddColumnClause() { return 0; }

    virtual CommentStatementAST *asCommentStatement() { return 0; }
    virtual CommentOnTableStatementAST *asCommentOnTableStatement() { return 0; }
    virtual CommentOnColumnStatementAST *asCommentOnColumnStatement() { return 0; }

    virtual CommitStatementAST *asCommitStatement() { return 0; }
    virtual RollbackStatementAST *asRollbackStatement() { return 0; }
    virtual CallStatementAST *asCallStatement() { return 0; }

    virtual BaseTypeAST *asBaseType() { return 0; }
    virtual CharacterTypeAST *asCharacterType() { return 0; }
    virtual NumberTypeAST *asNumericType() { return 0; }
    virtual SimpleTypeAST *asSimpleType() { return 0; }

    // Expressions
    virtual BaseExpressionAST *asBaseExpression() { return 0; }
    virtual ValueExpressionAST *asValueExpression() { return 0; }
    virtual CompoundValueExpressionAST *asCompoundValueExpression() { return 0; }
    virtual FunctionCallAST *asFunctionCall() { return 0; }
    virtual CountStarCallAST *asCountStarCall() { return 0; }
    virtual CountExprCallAST *asCountExprCall() { return 0; }
    virtual BuiltinFunctionCallAST *asBuiltinFunctionCall() { return 0; }
    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return 0; }
    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return 0; }
    virtual SimpleValueExpressionAST *asSimpleValueExpression() { return 0; }
    virtual ConstantExpressionAST *asConstantExpression() { return 0; }
    virtual LiteralExpressionAST *asLiteralExpression() { return 0; }
    virtual NumericExpressionAST *asNumericExpression() { return 0; }
    virtual FunctionExpressionAST *asFunctionExpression() { return 0; }
    virtual CastFunctionAST *asCastFunction() { return 0; }
    virtual SubstringFunctionAST *asSubstringFunction() { return 0; }
    virtual ParameterExpressionAST *asParameterExpression() { return 0; }
    virtual NullValueExpressionAST *asNullValueExpression() { return 0; }
    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return 0; }
    virtual CompoundRValueExpressionAST *asCompoundRValueExpression() { return 0; }
    virtual ValueListExpressionAST *asValueListExpression() { return 0; }
    virtual SubQueryExpressionAST *asSubQueryExpression() { return 0; }
    virtual SingleSourceAST *asSingleSource() { return 0; }
    virtual JoinConditionAST *asJoinCondition() { return 0; }
    virtual ConditionExpressionAST *asConditionExpression() { return 0; }
    virtual NotConditionExpressionAST *asNotConditionExpression() { return 0; }
    virtual TrueConditionExpressionAST *asTrueConditionExpression() { return 0; }
    virtual FalseConditionExpressionAST *asFalseConditionExpression() { return 0; }
    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return 0; }
    virtual CompareConditionExpressionAST *asCompareConditionExpression() { return 0; }
    virtual InConditionExpressionAST *asInConditionExpression() { return 0; }
    virtual LikeConditionExpressionAST *asLikeConditionExpression() { return 0; }
    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return 0; }
    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return 0; }
    virtual IsNullConditionExpressionAST *asIsNullConditionExpression() { return 0; }
    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return 0; }
    virtual ConditionAsValueExpressionAST *asConditionAsValueExpression() { return 0; }
    virtual AssignmentExpressionAST *asAssignmentExpression() { return 0; }
    virtual CaseExpressionAST *asCaseExpression() { return 0; }
    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return 0; }
    virtual CaseConditionAST *asCaseCondition() { return 0; }
    virtual CaseConditionClauseAST *asCaseConditionClause() { return 0; }

    // PL-SQL
    virtual PlSqlBlockBodyAST *asPlSqlBlockBody() { return 0; }
    virtual PlSqlStatementAST *asPlSqlStatement() { return 0; }
    virtual BasePlSqlStatementAST *asBasePlSqlStatement() { return 0; }
    virtual BlockSqlStatementAST *asBlockSqlStatement() { return 0; }
    virtual BlockCommitStatementAST *asBlockCommitStatement() { return 0; }
    virtual BlockDeleteStatementAST *asBlockDeleteStatement() { return 0; }
    virtual BlockInsertStatementAST *asBlockInsertStatement() { return 0; }
    virtual BlockLockTableStatementAST *asBlockLockTableStatement() { return 0; }
    virtual BlockRollbackStatementAST *asBlockRollbackStatement() { return 0; }
    virtual RollbackClauseAST *asRollbackClause() { return 0; }
    virtual RollbackToSavepointClauseAST *asRollbackToSavepointClause() { return 0; }
    virtual RollbackForceClauseAST *asRollbackForceClause() { return 0; }
    virtual BlockSavepointStatementAST *asBlockSavepointStatement() { return 0; }
    virtual BlockSelectIntoStatementAST *asBlockSelectIntoStatement() { return 0; }
    virtual BlockSetTransactionStatementAST *asBlockSetTransactionStatement() { return 0; }
    virtual BlockUpdateStatementAST *asBlockUpdateStatement() { return 0; }
    virtual AssignmentStatementAST *asAssignmentStatement() { return 0; }
    virtual LValueExpressionAST *asLValueExpression() { return 0; }
    virtual LValueExpressionItemAST *asLValueExpressionItem() { return 0; }
    virtual CloseStatementAST *asCloseStatement() { return 0; }
    virtual ContinueOrExitStatementAST *asContinueOrExitStatement() { return 0; }
    virtual ExecImmediateStatementAST *asExecImmediateStatement() { return 0; }
    virtual FetchStatementAST *asFetchStatement() { return 0; }
    virtual ForLoopStatementAST *asForLoopStatement() { return 0; }
    virtual ForInClauseAST *asForInClause() { return 0; }
    virtual ForInBoundsClauseAST *asForInBoundsClause() { return 0; }
    virtual ForInCursorClauseAST *asForInCursorClause() { return 0; }
    virtual ForInSelectClauseAST *asForInSelectClause() { return 0; }
    virtual ForallStatementAST *asForallStatement() { return 0; }
    virtual ForallInClauseAST *asForallInClause() { return 0; }
    virtual ForallInBoundsClauseAST *asForallInBoundsClause() { return 0; }
    virtual ForallInIndicesClauseAST *asForallInIndicesClause() { return 0; }
    virtual ForallInValuesClauseAST *asForallInValuesClause() { return 0; }
    virtual WhileLoopStatementAST *asWhileLoopStatement() { return 0; }
    virtual GotoStatementAST *asGotoStatement() { return 0; }
    virtual IfStatementAST *asIfStatement() { return 0; }
    virtual ElsifClauseAST *asElsifClause() { return 0; }
    virtual LoopStatementAST *asLoopStatement() { return 0; }
    virtual NullStatementAST *asNullStatement() { return 0; }
    virtual OpenCursorStatementAST *asOpenCursorStatement() { return 0; }
    virtual OpenForStatementAST *asOpenForStatement() { return 0; }
    virtual OpenForStatementClauseAST *asOpenForStatementClause() { return 0; }
    virtual OpenForStatementClauseBySelectAST *asOpenForStatementClauseBySelect() { return 0; }
    virtual OpenForStatementClauseByStringAST *asOpenForStatementClauseByString() { return 0; }
    virtual PlSqlBlocStatementAST *asPlSqlBlocStatement() { return 0; }
    virtual RaiseStatementAST *asRaiseStatement() { return 0; }
    virtual ReturnStatementAST *asReturnStatement() { return 0; }
    virtual DeclareSectionAST *asDeclareSection() { return 0; }
    virtual DeclarationAST *asDeclaration() { return 0; }
    virtual CursorDeclarationAST *asCursorDeclaration() { return 0; }
    virtual VariableDeclarationAST *asVariableDeclaration() { return 0; }
    virtual ExceptionDeclarationAST *asExceptionDeclaration() { return 0; }
    virtual SimpleDeclarationAST *asSimpleDeclaration() { return 0; }
    virtual FunctionDeclarationAST *asFunctionDeclaration() { return 0; }
    virtual FunctionDefintionAST *asFunctionDefintion() { return 0; }
    virtual ProcedureDeclarationAST *asProcedureDeclaration() { return 0; }
    virtual ProcedureDefinitionAST *asProcedureDefinition() { return 0; }
    virtual TypeDefinitionDeclarationAST *asTypeDefinitionDeclaration() { return 0; }
    virtual TypeDefinitionAST *asTypeDefinition() { return 0; }
    virtual RecordTypeDefinitionAST *asRecordTypeDefinition() { return 0; }
    virtual RefCursorTypeDefinitionAST *asRefCursorTypeDefinition() { return 0; }
    virtual SubtypeDefinitionAST *asSubtypeDefinition() { return 0; }
    virtual CollectionTypeDefinitionAST *asCollectionTypeDefinition() { return 0; }
    virtual CollectionTypeAST *asCollectionType() { return 0; }
    virtual TableCollectionTypeAST *asTableCollectionType() { return 0; }
    virtual ArrayCollectionTypeAST *asArrayCollectionType() { return 0; }
    virtual CursorSpecAST *asCursorSpec() { return 0; }
    virtual RowTypeAST *asRowType() { return 0; }
    virtual DataTypeAST *asDataType() { return 0; }
    virtual DataTypeByTypeNameAST *asDataTypeByTypeName() { return 0; }
    virtual DataTypeByNameAST *asDataTypeByName() { return 0; }
    virtual DataTypeByTableRowAST *asDataTypeByTableRow() { return 0; }
    virtual DataTypeByTableColumnAST *asDataTypeByTableColumn() { return 0; }
    virtual ScalarDataTypeAST *asScalarDataType() { return 0; }
    virtual CursorParameterDeclarationAST *asCursorParameterDeclaration() { return 0; }
    virtual RecordFieldDeclarationAST *asRecordFieldDeclaration() { return 0; }
    virtual RoutineArgumentDeclarationAST *asRoutineArgumentDeclaration() { return 0; }
    virtual FunctionHeadingAST *asFunctionHeading() { return 0; }
    virtual FunctionSpecAST *asFunctionSpec() { return 0; }
    virtual FunctionDeterministicOrPipelinedSpecAST *asFunctionDeterministicOrPipelinedSpec() { return 0; }
    virtual FunctionCasheSpecAST *asFunctionCasheSpec() { return 0; }
    virtual ProcedureHeadingAST *asProcedureHeading() { return 0; }
    virtual ExceptionHandlerAST *asExceptionHandler() { return 0; }
    virtual NamedExceptionHandlerAST *asNamedExceptionHandler() { return 0; }
    virtual OthersExceptionHandlerAST *asOthersExceptionHandler() { return 0; }
    virtual BaseIntoClauseAST *asBaseIntoClause() { return 0; }
    virtual IntoClauseAST *asIntoClause() { return 0; }
    virtual BulkIntoClauseAST *asBulkIntoClause() { return 0; }
    virtual UsingClauseAST *asUsingClause() { return 0; }
    virtual UsingBindAST *asUsingBind() { return 0; }
    virtual DynamicReturningClauseAST *asDynamicReturningClause() { return 0; }
    virtual ArrayIndexExpressionAST *asArrayIndexExpression() { return 0; }
    virtual ColumnIdentifierAST *asColumnIdentifier() { return 0; }
    virtual TableIdentifierAST *asTableIdentifier() { return 0; }
    virtual IndexIdentifierAST *asIndexIdentifier() { return 0; }
    virtual TriggerIdentifierAST *asTriggerIdentifier() { return 0; }
    virtual FunctionIdentifierAST *asFunctionIdentifier() { return 0; }

    // Additional for SelectStatement and InvalidSelectStatement
    virtual bool isSelectStatement() const { return false; }

protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
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
    StatementListAST *statement_list;

public:
    TranslationUnitAST( StatementListAST *statements )
        : statement_list(finish(statements))
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
    {}
    virtual StatementAST *asStatement() { return this; }

    inline bool isTopLevelStatement() const { return topLevel; }
    inline void setTopLevelStatement( bool topLevel_ ) { topLevel = topLevel_; }
    inline bool isBlockMode() const { return blockMode; }
    inline void setBlockMode( bool blockMode_ = true ) { blockMode = blockMode_; }

private:
    bool topLevel;
    bool blockMode;
};

class InvalidStatementAST: public StatementAST
{
public:
    unsigned first_token;

public:
    InvalidStatementAST( unsigned f_token, bool isSelect_ )
        : StatementAST()
        , first_token(f_token)
        , isSelect(isSelect_)
    {}

    virtual int firstToken() const { return first_token; }
    virtual int lastToken() const { return first_token; }

    virtual bool isSelectStatement() const { return isSelect; }

    virtual InvalidStatementAST *asInvalidStatement() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );

private:
    bool isSelect;
};

class UnknownStatementAST: public StatementAST
{
public:
    unsigned first_token;
    unsigned last_token;

public:
    UnknownStatementAST()
        : first_token(0)
        , last_token(0)
    {}

    virtual UnknownStatementAST *asUnknownStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DmlStatementAST: public StatementAST
{
public:
    DmlStatementAST() : StatementAST() {}
    virtual DmlStatementAST *asDmlStatement() { return this; }
};

class SelectStatementAST: public DmlStatementAST
{
public:
    SelectCoreAST *select_core;
    OrderByBlockAST *order_by_block;
    ForUpdateClauseAST *forupdate_clause;

public:
    SelectStatementAST( SelectCoreAST *select_core_,
                        OrderByBlockAST *order_by_block_,
                        ForUpdateClauseAST *forupdate_clause_)
        : DmlStatementAST()
        , select_core(select_core_)
        , order_by_block(order_by_block_)
        , forupdate_clause(forupdate_clause_)
    {}

    virtual SelectStatementAST *asSelectStatement() { return this; }
    virtual bool isSelectStatement() const { return true; }

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
    BaseSelectColumnsAST *selColumns;
    int from_token;
    JoinSourceAST *joinSource;
    WhereClauseAST *whereClause;
    ConnectByClauseAST *connectByClause;
    GroupByBlockAST *groupBy;
    SetClauseAST *setClause;

public:
    SelectCoreAST( int select_token_,
                   int select_spec_token_,
                   BaseSelectColumnsAST *columnList_,
                   int from_token_,
                   JoinSourceAST *joinSource_,
                   WhereClauseAST *whereClause_,
                   ConnectByClauseAST *connectByClause_,
                   GroupByBlockAST *groupBy_,
                   SetClauseAST *setClause_ )
        : select_token(select_token_)
        , select_spec_token(select_spec_token_)
        , selColumns(columnList_)
        , from_token(from_token_)
        , joinSource(joinSource_)
        , whereClause(whereClause_)
        , connectByClause(connectByClause_)
        , groupBy(groupBy_)
        , setClause(setClause_)
    {}

    virtual SelectCoreAST *asSelectCore() { return this; }

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

class LimitToBlockAST: public AST
{
public:
    unsigned limit_token;
    unsigned to_token;
    ValueExpressionAST *expr;
    unsigned row_token;

public:
    LimitToBlockAST()
        : limit_token(0)
        , to_token(0)
        , expr(0)
        , row_token(0)
    {}

    virtual LimitToBlockAST *asLimitToBlock() { return this; }

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

class LabelAST: public AST
{
public:
    unsigned less0_token;
    unsigned less1_token;
    PlSqlNameAST *labelName;
    unsigned greater0_token;
    unsigned greater1_token;

public:
    LabelAST()
        : less0_token(0)
        , less1_token(0)
        , labelName(0)
        , greater0_token(0)
        , greater1_token(0)
    {}

    virtual LabelAST *asLabel() { return this; }

    virtual int firstToken() const { return less0_token; }
    virtual int lastToken() const { return greater1_token + 1; }

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
        Routine
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

    PlSqlNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
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

    PlSqlTypeNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ResultColumnAST: public AST
{
public:
    ResultColumnAST() {}
    virtual ResultColumnAST *asResultColumn() { return this; }
};

class BaseSelectColumnsAST: public AST
{
public:
    BaseSelectColumnsAST() {}
    virtual BaseSelectColumnsAST *asBaseSelectColumns() { return this; }
};

class StarSelectColumnAST: public BaseSelectColumnsAST
{
public:
    int star_token;

public:
    StarSelectColumnAST( int star_token_ )
        : BaseSelectColumnsAST()
        , star_token(star_token_)
    {}

    virtual StarSelectColumnAST *asStarSelectColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ListSelectColumnAST: public BaseSelectColumnsAST
{
public:
    List<BaseSelectColumnItemAST *> *columns;

public:
    ListSelectColumnAST( List<BaseSelectColumnItemAST *> *columns_ )
        : BaseSelectColumnsAST()
        , columns(finish(columns_))
    {}

    virtual ListSelectColumnAST *asListSelectColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseSelectColumnItemAST: public AST
{
public:
    BaseSelectColumnItemAST() {}
    virtual BaseSelectColumnItemAST *asBaseSelectColumnItem() { return this; }
};

class TableSelectColumnsItemAST: public BaseSelectColumnItemAST
{
public:
    TableIdentifierAST *tableIdent;
    int dot_token;
    int star_token;

public:
    TableSelectColumnsItemAST( TableIdentifierAST *tableIdent_,
                               int dot_token_,
                               int star_token_ )
        : BaseSelectColumnItemAST()
        , tableIdent(tableIdent_)
        , dot_token(dot_token_)
        , star_token(star_token_)
    {}

    virtual TableSelectColumnsItemAST *asTableSelectColumnsItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ValueSelectColumnItemAST: public BaseSelectColumnItemAST
{
public:
    ValueExpressionAST *columnExpr;
    int as_token;
    NameAST *columnAlias;

public:
    ValueSelectColumnItemAST( ValueExpressionAST *columnExpr_,
                              int as_token_,
                              NameAST *columnAlias_ )
        : BaseSelectColumnItemAST()
        , columnExpr(columnExpr_)
        , as_token(as_token_)
        , columnAlias(columnAlias_)
    {}

    virtual ValueSelectColumnItemAST *asValueSelectColumnItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StarResultColumnAST: public ResultColumnAST
{
public:
    TableIdentifierAST *tableIdent;
    unsigned dot_token;
    unsigned star_token;

public:
    StarResultColumnAST()
        : tableIdent(0)
        , dot_token(0)
        , star_token(0)
    {}

    virtual StarResultColumnAST *asStarResultColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprResultColumnAST: public ResultColumnAST
{
public:
    ValueExpressionAST *expr;
    unsigned as_token;
    NameAST *columnAlias;

public:
    ExprResultColumnAST()
        : expr(0)
        , as_token(0)
        , columnAlias(0)
    {}

    virtual ExprResultColumnAST *asExprResultColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubQueryAST: public AST
{
public:
    unsigned lparen_token;
    StatementAST *selectStatement;
    unsigned rparen_token;

public:
    SubQueryAST()
        : lparen_token(0)
        , selectStatement(0)
        , rparen_token(0)
    {}

    virtual SubQueryAST *asSubQuery() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SingleSourceAST: public AST
{
public:
    SingleSourceAST()
    {}

    virtual SingleSourceAST *asSingleSource() { return this; }
};

class DirectSingleSourceAST: public SingleSourceAST
{
public:
    TableIdentifierAST *schemaTableName;
    NameAST *tableAlias;

public:
    DirectSingleSourceAST( TableIdentifierAST *schemaTableName_,
                           NameAST *tableAlias_ )
        : SingleSourceAST()
        , schemaTableName(schemaTableName_)
        , tableAlias(tableAlias_)
    {}

    virtual DirectSingleSourceAST *asDirectSingleSource() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubQuerySingleSourceAST: public SingleSourceAST
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
        : SingleSourceAST()
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

class JoinOpAST: public AST
{
public:
    unsigned first_token;
    unsigned last_token;

public:
    JoinOpAST()
        : first_token(0)
        , last_token(0)
    {}

    virtual JoinOpAST *asJoinOp() { return this; }

    virtual int firstToken() const { return first_token; }
    virtual int lastToken() const { return last_token; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinConstraintAST: public AST
{
public:
    unsigned on_token;
    ConditionExpressionAST *expr;

public:
    JoinConstraintAST()
        : on_token(0)
        , expr(0)
    {}

    virtual JoinConstraintAST *asJoinConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinItemAST: public AST
{
public:
    JoinOpAST *joinOp;
    SingleSourceAST *singleSource;
    JoinConstraintAST *joinConstraint;

public:
    JoinItemAST()
        : joinOp(0)
        , singleSource(0)
        , joinConstraint(0)
    {}

    virtual JoinItemAST *asJoinItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseJoinListAST: public AST
{
public:
    BaseJoinListAST() {}
    virtual BaseJoinListAST *asBaseJoinList() { return this; }
};

class CompoundJoinListAST: public BaseJoinListAST
{
public:
    int lparen_token;
    BaseJoinListAST *join_list;
    int rparen_token;

public:
    CompoundJoinListAST( int lparen_token_,
                         BaseJoinListAST *join_list_,
                         int rparen_token_ )
        : BaseJoinListAST()
        , lparen_token(lparen_token_)
        , join_list(join_list_)
        , rparen_token(rparen_token_)
    {}

    CompoundJoinListAST *asCompoundJoinList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommaJoinListAST: public BaseJoinListAST
{
public:
    int comma_token;
    SingleSourceAST *source;
    BaseJoinListAST *join_list;

public:
    CommaJoinListAST( int comma_token_,
                      SingleSourceAST *source_,
                      BaseJoinListAST *join_list_ )
        : BaseJoinListAST()
        , comma_token(comma_token_)
        , source(source_)
        , join_list(join_list_)
    {}

    virtual CommaJoinListAST *asCommaJoinList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinJoinListAST: public BaseJoinListAST
{
public:
    int j1_spec_token;
    int j2_spec_token;
    int join_token;
    SingleSourceAST *source;
    JoinConditionAST *condition;
    BaseJoinListAST *join_list;

public:
    JoinJoinListAST( int j1_spec_token_,
                     int j2_spec_token_,
                     int join_token_,
                     SingleSourceAST *source_,
                     JoinConditionAST *condition_,
                     BaseJoinListAST *join_list_ )
        : BaseJoinListAST()
        , j1_spec_token(j1_spec_token_)
        , j2_spec_token(j2_spec_token_)
        , join_token(join_token_)
        , source(source_)
        , condition(condition_)
        , join_list(join_list_)
    {}

    virtual JoinJoinListAST *asJoinJoinList() { return this; }

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

class JoinSourceAST: public AST
{
public:
    SingleSourceAST *firstSource;
    BaseJoinListAST *join_list;

public:
    JoinSourceAST( SingleSourceAST *firstSource_,
                   BaseJoinListAST *join_list_ )
        : firstSource(firstSource_)
        , join_list(join_list_)
    {}

    virtual JoinSourceAST *asJoinSource() { return this; }

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

class WhereClauseAST: public AST
{
public:
    int where_token;
    ConditionExpressionAST *whereExpr;

public:
    WhereClauseAST( int where_token_, ConditionExpressionAST *whereExpr_ )
        : where_token(where_token_)
        , whereExpr(whereExpr_)
    {}

    virtual WhereClauseAST *asWhereClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseConnectPriorClauseAST: public AST
{
public:
    BaseConnectPriorClauseAST() {}
    virtual BaseConnectPriorClauseAST *asBaseConnectPriorClause() { return this; }
};

class StartConnectPriorAST: public BaseConnectPriorClauseAST
{
public:
    int prior_token;
    ValueExpressionAST *lvExpr;
    int compare_token;
    ValueExpressionAST *rvExpr;

public:
    StartConnectPriorAST( int prior_token_,
                          ValueExpressionAST *lvExpr_,
                          int compare_token_,
                          ValueExpressionAST *rvExpr_ )
        : BaseConnectPriorClauseAST()
        , prior_token(prior_token_)
        , lvExpr(lvExpr_)
        , compare_token(compare_token_)
        , rvExpr(rvExpr_)
    {}

    virtual StartConnectPriorAST *asStartConnectPrior() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EndConnectPriorAST: public BaseConnectPriorClauseAST
{
public:
    ValueExpressionAST *lvExpr;
    int compare_token;
    ValueExpressionAST *rvExpr;
    int prior_token;

public:
    EndConnectPriorAST( ValueExpressionAST *lvExpr_,
                        int compare_token_,
                        ValueExpressionAST *rvExpr_,
                        int prior_token_ )
        : BaseConnectPriorClauseAST()
        , lvExpr(lvExpr_)
        , compare_token(compare_token_)
        , rvExpr(rvExpr_)
        , prior_token(prior_token_)
    {}

    virtual EndConnectPriorAST *asEndConnectPrior() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StartWithClauseAST: public AST
{
public:
    int start_token;
    int with_token;
    ConditionExpressionAST *conditionExpr;

public:
    StartWithClauseAST( int start_token_, int with_token_, ConditionExpressionAST *conditionExpr_ )
        : start_token(start_token_)
        , with_token(with_token_)
        , conditionExpr(conditionExpr_)
    {}

    virtual StartWithClauseAST *asStartWithClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConnectByClauseAST: public AST
{
public:
    int connect_token;
    int by_token;
    BaseConnectPriorClauseAST *connectPriorClause;
    StartWithClauseAST *startWithClause;

public:
    ConnectByClauseAST( int connect_token_, int by_token_,
                        BaseConnectPriorClauseAST *connectPriorClause_,
                        StartWithClauseAST *startWithClause_ )
        : connect_token(connect_token_)
        , by_token(by_token_)
        , connectPriorClause(connectPriorClause_)
        , startWithClause(startWithClause_)
    {}

    virtual ConnectByClauseAST *asConnectByClause() { return this; }

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

    virtual int firstToken() const;
    virtual int lastToken() const;

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

    virtual int firstToken() const;
    virtual int lastToken() const;

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

class InsertClauseAST: public AST
{
public:
    InsertClauseAST() {}
    virtual InsertClauseAST *asInsertClause() { return this; }
};

class CreateTableClauseAST: public AST
{
public:
    CreateTableClauseAST() {}
    virtual CreateTableClauseAST *asCreateTableClause() { return this; }
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

class CreateTableAsSelectClauseAST: public CreateTableClauseAST
{
public:
    int as_token;
    StatementAST *selectStatement;

public:
    CreateTableAsSelectClauseAST( int as_token_,
                                  StatementAST *selectStatement_ )
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

class CreateTriggerRefClauseAST: public AST
{
public: // Annotations
    TableIdentifierAST *tableIdent;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    int referencing_token;
    int new_token;
    int asn_token;
    NameAST *newAlias;
    int old_token;
    int aso_token;
    NameAST *oldAlias;

public:
    CreateTriggerRefClauseAST( TableIdentifierAST *tableIdent_ )
        : tableIdent(tableIdent_)
        , referencing_token(0)
        , new_token(0)
        , asn_token(0)
        , newAlias(0)
        , old_token(0)
        , aso_token(0)
        , oldAlias(0)
    {}

    virtual CreateTriggerRefClauseAST *asCreateTriggerRefClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

    CreateTriggerRefClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTriggerBodyAST: public AST
{
public:
    CreateTriggerBodyAST() {}
    virtual CreateTriggerBodyAST *asCreateTriggerBody() { return this; }
};

class CreateRdbTriggerBodyAST: public CreateTriggerBodyAST
{
public:
    CreateRdbTriggerBodyListAST *item_list;

public:
    CreateRdbTriggerBodyAST()
        : CreateTriggerBodyAST()
        , item_list(0)
    {}

    virtual CreateRdbTriggerBodyAST *asCreateRdbTriggerBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

    CreateRdbTriggerBodyAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateRdbTriggerBodyItemAST: public AST
{
public:
    int when_token;
    int lparen_when_token;
    ConditionExpressionAST *whenExpr;
    int rparen_when_token;
    int lparen_token;
    StatementListAST *stat_list;
    int rparen_token;
    int for_token;
    int each_token;
    int row_or_statement_token;

public:
    CreateRdbTriggerBodyItemAST()
        : when_token(0)
        , lparen_when_token(0)
        , whenExpr(0)
        , rparen_when_token(0)
        , lparen_token(0)
        , stat_list(0)
        , rparen_token(0)
        , for_token(0)
        , each_token(0)
        , row_or_statement_token(0)
    {}

    virtual CreateRdbTriggerBodyItemAST *asCreateRdbTriggerBodyItem() { return this; }

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
    int default_token;
    ConstantExpressionAST *defaultExpr;
    ColumnConstraintListAST *constraintList;

public:
    ColumnDefinitionAST( NameAST *columnName_,
                         BaseTypeAST *columnType_,
                         int default_token_,
                         ConstantExpressionAST *defaultExpr_,
                         ColumnConstraintListAST *constraintList_ )
        : columnName(columnName_)
        , columnType(columnType_)
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

class ConstraintStateAST: public AST
{
public:
    ConstraintDeferStateAST *defer;
    int relay_or_norelay_token;
    UsingIndexClauseAST *usingIndex;
    int enable_or_disable_token;
    int validate_or_novalidate_token;
    ExceptionsClauseAST *exceptionClause;

public:
    ConstraintStateAST( ConstraintDeferStateAST *defer_,
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

class ConstraintAST: public AST
{
public:
    int constraint_token;
    NameAST *constraintName;

public:
    ConstraintAST( int constraint_token_, NameAST *constraintName_ )
        : constraint_token(constraint_token_)
        , constraintName(constraintName_)
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

    TableConstraintAST( int constraint_token_, NameAST *constraintName_, TableConstraintType constraintType_ )
        : ConstraintAST(constraint_token_, constraintName_)
        , constraintType(constraintType_)
    {}

    inline TableConstraintType tableConstraintType() const { return constraintType; }

    virtual TableConstraintAST *asTableConstraint() { return this; }

private:
    TableConstraintType constraintType;
};

class ColumnConstraintAST: public ConstraintAST
{
public:
    ColumnConstraintAST( unsigned constraint_token_, NameAST *constraintName_ )
        : ConstraintAST(constraint_token_, constraintName_)
    {}

    virtual ColumnConstraintAST *asColumnConstraint() { return this; }
};

class UniqueOrPkTableConstraintAST: public TableConstraintAST
{
public:
    int unique_or_primary_token;
    int key_token;
    int lparen_token;
    SortedColumnNameListAST *column_list;
    int rparen_token;

    bool parse( Parser *p );

public:
    UniqueOrPkTableConstraintAST( unsigned constraint_token_,
                                  NameAST *constraintName_,
                                  int unique_or_primary_token_,
                                  int key_token_,
                                  int lparen_token_,
                                  SortedColumnNameListAST *column_list_,
                                  int rparen_token_ )
        : TableConstraintAST(constraint_token_, constraintName_, TableConstraintAST::UniqueOrPrimaryKey)
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

class CheckTableConstraintAST: public TableConstraintAST
{
public:
    int check_token;
    int lparen_token;
    ConditionExpressionAST *checkExpr;
    int rparen_token;

    bool parse( Parser *p );

public:
    CheckTableConstraintAST( int constraint_token_,
                             NameAST *constraintName_,
                             int check_token_,
                             int lparen_token_,
                             ConditionExpressionAST *checkExpr_,
                             int rparen_token_ )
        : TableConstraintAST(constraint_token_, constraintName_, TableConstraintAST::Check)
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

class FkTableConstraintAST: public TableConstraintAST
{
public:
    int foreign_token;
    int key_token;
    int lparen_token;
    NameListAST *col_list;
    int rparen_token;
    FkReferencesClauseAST *refClause;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    FkTableConstraintAST( int constraint_token_,
                          NameAST *constraintName_,
                          int foreign_token_,
                          int key_token_,
                          int lparen_token_,
                          NameListAST *col_list_,
                          int rparen_token_,
                          FkReferencesClauseAST *refClause_ )
        : TableConstraintAST(constraint_token_, constraintName_, TableConstraintAST::ForeignKey)
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

class FkReferencesClauseAST: public AST
{
public:
    int references_token;
    TableIdentifierAST *tableIdent;
    int ref_lparen_token;
    NameListAST *ref_col_list;
    int ref_rparen_token;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    FkReferencesClauseAST( int references_token_,
                           TableIdentifierAST *tableIdent_,
                           int ref_lparen_token_,
                           NameListAST *ref_col_list_,
                           int ref_rparen_token_ )
        : references_token(references_token_)
        , tableIdent(tableIdent_)
        , ref_lparen_token(ref_lparen_token_)
        , ref_col_list(finish(ref_col_list_))
        , ref_rparen_token(ref_rparen_token_)
    {}

    virtual FkReferencesClauseAST * asForeignKeyClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PkColumnConstraintAST: public ColumnConstraintAST
{
public:
    int primary_token;
    int key_token;
    int asc_or_desc_token;
    ConstraintStateAST *state;

    bool parse( Parser *p );

public:
    PkColumnConstraintAST( int constraint_token_,
                           NameAST *constraintName_,
                           int primary_token_,
                           int key_token_,
                           int asc_or_desc_token_,
                           ConstraintStateAST *state_ )
        : ColumnConstraintAST(constraint_token_, constraintName_)
        , primary_token(primary_token_)
        , key_token(key_token_)
        , asc_or_desc_token(asc_or_desc_token_)
        , state(state_)
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
    ConstraintStateAST *state;

    bool parse( Parser *p );

public:
    NotNullColumnConstraintAST( int constraint_token_,
                                NameAST *constraintName_,
                                int not_token_,
                                int null_token_,
                                ConstraintStateAST *state_ )
        : ColumnConstraintAST(constraint_token_, constraintName_)
        , not_token(not_token_)
        , null_token(null_token_)
        , state(state_)
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
    ConstraintStateAST *state;

public:
    UniqueColumnConstraintAST( int constraint_token_,
                               NameAST *constraintName_,
                               int unique_token_,
                               ConstraintStateAST *state_ )
        : ColumnConstraintAST(constraint_token_, constraintName_)
        , unique_token(unique_token_)
        , state(state_)
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
    ConstraintStateAST *state;

    bool parse( Parser *p );

public:
    CheckColumnConstraintAST( int constraint_token_,
                              NameAST *constraintName_,
                              int check_token_,
                              int lparen_token_,
                              ConditionExpressionAST *checkExpr_,
                              int rparen_token_,
                              ConstraintStateAST *state_ )
        : ColumnConstraintAST(constraint_token_, constraintName_)
        , check_token(check_token_)
        , lparen_token(lparen_token_)
        , checkExpr(checkExpr_)
        , rparen_token(rparen_token_)
        , state(state_)
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
    ConstraintStateAST *state;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    ReferencesColumnConstraintAST( int constraint_token_,
                                   NameAST *constraintName_,
                                   FkReferencesClauseAST *refClause_,
                                   ConstraintStateAST *state_ )
        : ColumnConstraintAST(constraint_token_, constraintName_)
        , refClause(refClause_)
        , state(state_)
    {}

    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
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

class DdlStatementAST: public StatementAST
{
public:
    DdlStatementAST() : StatementAST() {}
    virtual DdlStatementAST *asDdlStatement() { return this; }
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

class CreateTriggerStatementAST: public CreateStatementAST
{
public:
    int create_token;
    int trigger_token;
    TriggerIdentifierAST *triggeeIdent;
    int before_or_after_token;
    int action_token;
    int of_token;
    NameListAST *col_list;
    int on_token;
    TableIdentifierAST *tableIdent;
    CreateTriggerRefClauseAST *triggerRefClause;
    CreateTriggerBodyAST *triggerBody;

public:
    CreateTriggerStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , trigger_token(0)
        , triggeeIdent(0)
        , before_or_after_token(0)
        , action_token(0)
        , of_token(0)
        , col_list(0)
        , on_token(0)
        , tableIdent(0)
        , triggerRefClause(0)
        , triggerBody(0)
    {}

    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return this; }

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
    TableIdentifierAST *tableIdent;
    AlterTableClauseAST *alterTableClause;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableStatementAST()
        : AlterStatementAST()
        , alter_token(0)
        , table_token(0)
        , tableIdent(0)
        , alterTableClause(0)
    {}

    virtual AlterTableStatementAST *asAlterTableStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableClauseAST: public AST
{
public:
    enum TableObjectKind {
        Unknown = 0,
        Field,
        Constraint
    };

    TableObjectKind objectKind;

public:
    AlterTableClauseAST()
        : objectKind(Unknown)
    {}

    virtual AlterTableClauseAST *asAlterTableClause() { return this; }
};

class AlterTableDropClauseAST: public AlterTableClauseAST
{
public:
    unsigned drop_token;
    unsigned drop_object_type_token;    // constraint or column
    NameAST *dropObjectName;            // ColumnNameAST or ConstraintNameAST

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableDropClauseAST()
        : AlterTableClauseAST()
        , drop_token(0)
        , drop_object_type_token(0)
        , dropObjectName(0)
    {}

    virtual AlterTableDropClauseAST *asAlterTableDropClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAddClauseAST: public AlterTableClauseAST
{
public:
    AlterTableAddClauseAST()
        : AlterTableClauseAST()
    {}

    virtual AlterTableAddClauseAST *asAlterTableAddClause() { return this; }
};

class AlterTableAddConstraintClauseAST: public AlterTableAddClauseAST
{
public:
    unsigned add_token;
    TableConstraintAST *constraint;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableAddConstraintClauseAST()
        : AlterTableAddClauseAST()
        , add_token(0)
        , constraint(0)
    {}

    virtual AlterTableAddConstraintClauseAST *asAlterTableAddConstraintClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAddColumnClauseAST: public AlterTableAddClauseAST
{
public:
    unsigned add_token;
    unsigned column_token;
    ColumnDefinitionAST *column;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableAddColumnClauseAST()
        : AlterTableAddClauseAST()
        , add_token(0)
        , column_token(0)
        , column(0)
    {}

    virtual AlterTableAddColumnClauseAST *asAlterTableAddColumnClause() { return this; }

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
    unsigned commit_token;

public:
    CommitStatementAST()
        : StatementAST()
        , commit_token(0)
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
    unsigned rollback_token;

public:
    RollbackStatementAST()
        : StatementAST()
        , rollback_token(0)
    {}

    virtual RollbackStatementAST *asRollbackStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CallStatementAST: public StatementAST
{
public:
    unsigned call_token;
    NameAST *funcName;
    unsigned lparen_token;
    ValueExpressionListAST *arg_list;
    unsigned rparen_token;

public:
    CallStatementAST()
        : StatementAST()
        , call_token(0)
        , funcName(0)
        , lparen_token(0)
        , arg_list(0)
        , rparen_token(0)
    {}

    virtual CallStatementAST *asCallStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

/// Type Specifiers
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

    virtual NumberTypeAST *asNumericType() { return this; }

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

/// Expressions
class BaseExpressionAST: public AST
{
public:
    BaseExpressionAST() {}
    virtual BaseExpressionAST *asBaseExpression() { return this; }
};

class ValueExpressionAST: public BaseExpressionAST
{
public:
    ValueExpressionAST()
        : BaseExpressionAST()
    {}

    virtual ValueExpressionAST *asValueExpression() { return this; }
};

class CompoundValueExpressionAST: public ValueExpressionAST
{
public:
    int lparen_token;
    ValueExpressionAST *valExpr;
    int rparen_token;

public:
    CompoundValueExpressionAST( int lparen_token_,
                                ValueExpressionAST *valExpr_,
                                int rparen_token_ )
        : ValueExpressionAST()
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

class FunctionCallAST: public ValueExpressionAST
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
        : ValueExpressionAST()
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

class CountStarCallAST: public ValueExpressionAST
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
        : ValueExpressionAST()
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

class CountExprCallAST: public ValueExpressionAST
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
        : ValueExpressionAST()
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

class BuiltinFunctionCallAST: public ValueExpressionAST
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
        : ValueExpressionAST()
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

class SimpleValueExpressionAST: public ValueExpressionAST
{
public:
    SimpleValueExpressionAST()
        : ValueExpressionAST()
    {}

    virtual SimpleValueExpressionAST *asSimpleValueExpression() { return this; }
};

class ConstantExpressionAST: public SimpleValueExpressionAST
{
public:
    ConstantExpressionAST()
        : SimpleValueExpressionAST()
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

class FunctionExpressionAST: public SimpleValueExpressionAST
{
public:
    NameAST *functionName;
    unsigned lparen_token;
    ValueExpressionListAST *arg_list;
    unsigned rparen_token;

public:
    FunctionExpressionAST()
        : SimpleValueExpressionAST()
        , functionName(0)
        , lparen_token(0)
        , arg_list(0)
        , rparen_token(0)
    {}

    virtual FunctionExpressionAST *asFunctionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CastFunctionAST: public SimpleValueExpressionAST
{
public:
    unsigned cast_token;
    unsigned lparen_token;
    ValueExpressionAST *castExpr;
    unsigned as_token;
    BaseTypeAST *typeSpec;
    unsigned rparen_token;

public:
    CastFunctionAST()
        : SimpleValueExpressionAST()
        , cast_token(0)
        , lparen_token(0)
        , castExpr(0)
        , as_token(0)
        , typeSpec(0)
        , rparen_token(0)
    {}

    virtual CastFunctionAST *asCastFunction() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubstringFunctionAST: public SimpleValueExpressionAST
{
public:
    unsigned substring_token;
    unsigned lparen_token;
    ValueExpressionAST *strExpr;
    unsigned from_token;
    ValueExpressionAST *startExpr;
    unsigned for_token;
    ValueExpressionAST *lengthExpr;
    unsigned rparen_token;

public:
    SubstringFunctionAST()
        : substring_token(0)
        , lparen_token(0)
        , strExpr(0)
        , from_token(0)
        , startExpr(0)
        , for_token(0)
        , lengthExpr(0)
        , rparen_token(0)
    {}

    virtual SubstringFunctionAST *asSubstringFunction() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ParameterExpressionAST: public SimpleValueExpressionAST
{
public:
    int colon_token;
    NameAST *bindName;

public:
    ParameterExpressionAST( int colon_token_,
                            NameAST *bindName_ )
        : SimpleValueExpressionAST()
        , colon_token(colon_token_)
        , bindName(bindName_)
    {}

    virtual ParameterExpressionAST *asParameterExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NullValueExpressionAST: public ConstantExpressionAST
{
public:
    unsigned not_token;
    unsigned null_token;

public:
    NullValueExpressionAST()
        : ConstantExpressionAST()
        , not_token(0)
        , null_token(0)
    {}

    virtual NullValueExpressionAST *asNullValueExpression() { return this; }

    virtual int firstToken() const { if ( not_token ) return not_token; return null_token; }
    virtual int lastToken() const { return null_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCompoundValueExpressionAST: public SimpleValueExpressionAST
{
public:
    unsigned lparen_token;
    StatementAST *query;
    unsigned rparen_token;

public:
    SelectCompoundValueExpressionAST()
        : SimpleValueExpressionAST()
        , lparen_token(0)
        , query(0)
        , rparen_token(0)
    {}

    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return this; }

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

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FalseConditionExpressionAST: public ConditionExpressionAST
{
public:
    int false_token;

public:
    FalseConditionExpressionAST( int true_token_ )
        : ConditionExpressionAST()
        , false_token(true_token_)
    {}

    virtual FalseConditionExpressionAST *asFalseConditionExpression() { return this; }

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

class CaseExpressionAST: public ValueExpressionAST
{
public:
    unsigned case_token;
    ValueExpressionAST *caseExpr;
    CaseExpressionClauseListAST *var_list;
    unsigned else_token;
    ValueExpressionAST *elseExpr;
    unsigned end_token;

public:
    CaseExpressionAST()
        : ValueExpressionAST()
        , case_token(0)
        , caseExpr(0)
        , var_list(0)
        , else_token(0)
        , elseExpr(0)
        , end_token(0)
    {}

    virtual CaseExpressionAST *asCaseExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseExpressionClauseAST: public AST
{
public:
    unsigned when_token;
    ValueExpressionAST *whenExpr;
    unsigned then_token;
    ValueExpressionAST *thenExpr;

public:
    CaseExpressionClauseAST()
        : when_token(0)
        , whenExpr(0)
        , then_token(0)
        , thenExpr(0)
    {}

    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseConditionAST: public ValueExpressionAST
{
public:
    unsigned case_token;
    CaseConditionClauseListAST *var_list;
    unsigned else_token;
    ValueExpressionAST *elseExpr;
    unsigned end_token;

public:
    CaseConditionAST()
        : ValueExpressionAST()
        , case_token(0)
        , var_list(0)
        , else_token(0)
        , elseExpr(0)
        , end_token(0)
    {}

    virtual CaseConditionAST *asCaseCondition() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseConditionClauseAST: public AST
{
public:
    unsigned when_token;
    ConditionExpressionAST *whenExpr;
    unsigned then_token;
    ValueExpressionAST *thenExpr;

public:
    CaseConditionClauseAST()
        : when_token(0)
        , whenExpr(0)
        , then_token(0)
        , thenExpr(0)
    {}

    virtual CaseConditionClauseAST *asCaseConditionClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
// PL-SQL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class PlSqlBlockAST: public StatementAST
{
public:
    LabelAST *label;
    unsigned declare_token;
    DeclareSectionAST *declareSection;
    PlSqlBlockBodyAST *body;

public:
    PlSqlBlockAST()
        : StatementAST()
        , label(0)
        , declare_token(0)
        , declareSection(0)
        , body(0)
    {}

    virtual PlSqlBlockAST *asPlSqlBlock() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PlSqlBlockBodyAST: public AST
{
public:
    unsigned begin_token;
    PlSqlStatementListAST *statements;
    unsigned exception_token;
    NamedExceptionHandlerListAST *namedExceptions;
    OthersExceptionHandlerAST *othersException;
    unsigned end_token;
    PlSqlNameAST *endBlockName;

public:
    PlSqlBlockBodyAST()
        : begin_token(0)
        , statements(0)
        , exception_token(0)
        , namedExceptions(0)
        , othersException(0)
        , end_token(0)
        , endBlockName(0)
    {}

    virtual PlSqlBlockBodyAST *asPlSqlBlockBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PlSqlStatementAST: public AST
{
public:
    LabelAST *label;
    BasePlSqlStatementAST *statement;

public:
    PlSqlStatementAST()
        : label(0)
        , statement(0)
    {}

    virtual PlSqlStatementAST *asPlSqlStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BasePlSqlStatementAST: public AST
{
public:
    BasePlSqlStatementAST() {}
    virtual BasePlSqlStatementAST *asBasePlSqlStatement() { return this; }
};

class BlockSqlStatementAST: public BasePlSqlStatementAST
{
public:
    BlockSqlStatementAST() : BasePlSqlStatementAST() {}
    virtual BlockSqlStatementAST *asBlockSqlStatement() { return this; }
};

class BlockCommitStatementAST: public BlockSqlStatementAST
{
public:
    unsigned commit_token;

public:
    BlockCommitStatementAST()
        : BlockSqlStatementAST()
        , commit_token(0)
    {}

    virtual BlockCommitStatementAST *asBlockCommitStatement() { return this; }

    virtual int firstToken() const { return commit_token; }
    virtual int lastToken() const { return commit_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockDeleteStatementAST: public BlockSqlStatementAST
{
public:
    DeleteStatementAST *deleteStatement;

public:
    BlockDeleteStatementAST()
        : BlockSqlStatementAST()
        , deleteStatement(0)
    {}

    virtual BlockDeleteStatementAST *asBlockDeleteStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockInsertStatementAST: public BlockSqlStatementAST
{
public:
    InsertStatementAST *insertStatement;

public:
    BlockInsertStatementAST()
        : BlockSqlStatementAST()
        , insertStatement(0)
    {}

    virtual BlockInsertStatementAST *asBlockInsertStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockLockTableStatementAST: public BlockSqlStatementAST
{
public:
    unsigned lock_token;
    unsigned table_token;
    TableIdentifierAST *tableIdent;

public:
    BlockLockTableStatementAST()
        : BlockSqlStatementAST()
        , lock_token(0)
        , table_token(0)
        , tableIdent(0)
    {}

    virtual BlockLockTableStatementAST *asBlockLockTableStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockRollbackStatementAST: public BlockSqlStatementAST
{
public:
    unsigned rollback_token;
    unsigned work_token;
    RollbackClauseAST *rollbacClause;

public:
    BlockRollbackStatementAST()
        : BlockSqlStatementAST()
        , rollback_token(0)
        , work_token(0)
        , rollbacClause(0)
    {}

    virtual BlockRollbackStatementAST *asBlockRollbackStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RollbackClauseAST: public AST
{
public:
    RollbackClauseAST() {}
    virtual RollbackClauseAST *asRollbackClause() { return this; }
};

class RollbackToSavepointClauseAST: public RollbackClauseAST
{
public:
    unsigned to_token;
    unsigned savepoint_token;
    PlSqlNameAST *savepointName;    // PlSqlNameAST::Savepoint

public:
    RollbackToSavepointClauseAST()
        : RollbackClauseAST()
        , to_token(0)
        , savepoint_token(0)
        , savepointName(0)
    {}

    virtual RollbackToSavepointClauseAST *asRollbackToSavepointClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RollbackForceClauseAST: public RollbackClauseAST
{
public:
    unsigned force_token;
    LiteralExpressionAST *stringExpr;

public:
    RollbackForceClauseAST()
        : RollbackClauseAST()
        , force_token(0)
        , stringExpr(0)
    {}

    virtual RollbackForceClauseAST *asRollbackForceClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockSavepointStatementAST: public BlockSqlStatementAST
{
public:
    unsigned savepoint_token;
    PlSqlNameAST *savepointName;

public:
    BlockSavepointStatementAST()
        : BlockSqlStatementAST()
        , savepoint_token(0)
        , savepointName(0)
    {}

    virtual BlockSavepointStatementAST *asBlockSavepointStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockSelectIntoStatementAST: public BlockSqlStatementAST
{
public:
    unsigned select_token;
    unsigned select_spec_token;         // ALL|DISTINCT|UNIQUE
    ResultColumnListAST *columnList;
    unsigned bulk_token;
    unsigned collect_token;
    unsigned into_token;
    LValueExpressionListAST *intoVars;
    unsigned from_token;
    JoinSourceAST *joinSource;
    unsigned where_token;
    ConditionExpressionAST *whereExpr;
    GroupByBlockAST *groupBy;

public:
    BlockSelectIntoStatementAST()
        : BlockSqlStatementAST()
        , select_token(0)
        , select_spec_token(0)
        , columnList(0)
        , bulk_token(0)
        , collect_token(0)
        , into_token(0)
        , intoVars(0)
        , from_token(0)
        , joinSource(0)
        , where_token(0)
        , whereExpr(0)
        , groupBy(0)
    {}

    virtual BlockSelectIntoStatementAST *asBlockSelectIntoStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockSetTransactionStatementAST: public BlockSqlStatementAST
{
public:
    unsigned set_token;
    unsigned transaction_token;

public:
    BlockSetTransactionStatementAST()
        : BlockSqlStatementAST()
        , set_token(0)
        , transaction_token(0)
    {}

    virtual BlockSetTransactionStatementAST *asBlockSetTransactionStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockUpdateStatementAST: public BlockSqlStatementAST
{
public:
    UpdateStatementAST *updateStatement;

public:
    BlockUpdateStatementAST()
        : BlockSqlStatementAST()
        , updateStatement(0)
    {}

    virtual BlockUpdateStatementAST *asBlockUpdateStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AssignmentStatementAST: public BasePlSqlStatementAST
{
public:
    LValueExpressionAST *lvalueExpr;
    unsigned assign_token;
    ValueExpressionAST *rvalueExpr;

public:
    AssignmentStatementAST()
        : BasePlSqlStatementAST()
        , lvalueExpr(0)
        , assign_token(0)
        , rvalueExpr(0)
    {}

    virtual AssignmentStatementAST *asAssignmentStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LValueExpressionAST: public AST
{
public:
    LValueExpressionItemListAST *dotted_list;

public:
    LValueExpressionAST()
        : dotted_list(0)
    {}

    virtual LValueExpressionAST *asLValueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LValueExpressionItemAST: public AST
{
public:
    PlSqlNameAST *identName;
    ArrayIndexExpressionListAST *arr_ind_list;

public:
    LValueExpressionItemAST()
        : identName(0)
        , arr_ind_list(0)
    {}

    virtual LValueExpressionItemAST *asLValueExpressionItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CloseStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned close_token;
    PlSqlNameAST *cursorName;

public:
    CloseStatementAST()
        : BasePlSqlStatementAST()
        , close_token(0)
        , cursorName(0)
    {}

    virtual CloseStatementAST *asCloseStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ContinueOrExitStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned continue_or_exit_token;
    PlSqlNameAST *labelName;    // PlSqlNameAST::Label
    unsigned when_token;
    ConditionExpressionAST *condExpr;

public:
    ContinueOrExitStatementAST()
        : BasePlSqlStatementAST()
        , continue_or_exit_token(0)
        , labelName(0)
        , when_token(0)
        , condExpr(0)
    {}

    virtual ContinueOrExitStatementAST *asContinueOrExitStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExecImmediateStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned exec_token;
    unsigned immediate_token;
    ValueExpressionAST *stringSqlExpr;
    BaseIntoClauseAST *intoClause;
    UsingClauseAST *usingClause;
    DynamicReturningClauseAST *returningClause;

public:
    ExecImmediateStatementAST()
        : BasePlSqlStatementAST()
        , exec_token(0)
        , immediate_token(0)
        , stringSqlExpr(0)
        , intoClause(0)
        , usingClause(0)
        , returningClause(0)
    {}

    virtual ExecImmediateStatementAST *asExecImmediateStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FetchStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned fetch_token;
    PlSqlNameAST *cursorName;
    BaseIntoClauseAST *intoClause;
    unsigned limit_token;           // only for BULK COLLECT INTO
    NumericExpressionAST *limit;    // only for BULK COLLECT INTO

public:
    FetchStatementAST()
        : BasePlSqlStatementAST()
        , fetch_token(0)
        , cursorName(0)
        , intoClause(0)
        , limit_token(0)
        , limit(0)
    {}

    virtual FetchStatementAST *asFetchStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForLoopStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned for_token;
    PlSqlNameAST *indexName;
    unsigned in_token;
    ForInClauseAST *inClause;
    LoopStatementAST *loop;

public:
    ForLoopStatementAST()
        : BasePlSqlStatementAST()
        , for_token(0)
        , indexName(0)
        , in_token(0)
        , inClause(0)
        , loop(0)
    {}

    virtual ForLoopStatementAST *asForLoopStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForInClauseAST: public AST
{
public:
    ForInClauseAST() {}
    virtual ForInClauseAST *asForInClause() { return this; }
};

class ForInBoundsClauseAST: public ForInClauseAST
{
public:
    unsigned reverse_token;
    ValueExpressionAST *lboundExpr;
    unsigned dotdot_token;
    ValueExpressionAST *rboundExpr;

public:
    ForInBoundsClauseAST()
        : ForInClauseAST()
        , reverse_token(0)
        , lboundExpr(0)
        , dotdot_token(0)
        , rboundExpr(0)
    {}

    virtual ForInBoundsClauseAST *asForInBoundsClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForInCursorClauseAST: public ForInClauseAST
{
public:
    PlSqlNameAST *cursorName;
    unsigned lparen_token;
    ValueExpressionListAST *par_list;
    unsigned rparen_token;

public:
    ForInCursorClauseAST()
        : ForInClauseAST()
        , cursorName(0)
        , lparen_token(0)
        , par_list(0)
        , rparen_token(0)
    {}

    virtual ForInCursorClauseAST *asForInCursorClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForInSelectClauseAST: public ForInClauseAST
{
public:
    unsigned lparen_token;
    SelectStatementAST *selectStatement;
    unsigned rparen_token;

public:
    ForInSelectClauseAST()
        : ForInClauseAST()
        , lparen_token(0)
        , selectStatement(0)
        , rparen_token(0)
    {}

    virtual ForInSelectClauseAST *asForInSelectClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForallStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned forall_token;
    PlSqlNameAST *indexName;
    unsigned in_token;
    ForallInClauseAST *inClause;
    BlockSqlStatementAST *statement;
    unsigned save_token;
    unsigned exceptions_token;

public:
    ForallStatementAST()
        : BasePlSqlStatementAST()
        , forall_token(0)
        , indexName(0)
        , in_token(0)
        , inClause(0)
        , statement(0)
        , save_token(0)
        , exceptions_token(0)
    {}

    virtual ForallStatementAST *asForallStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForallInClauseAST: public AST
{
public:
    ForallInClauseAST() {}
    virtual ForallInClauseAST *asForallInClause() { return this; }
};

class ForallInBoundsClauseAST: public ForallInClauseAST
{
public:
    ValueExpressionAST *lboundExpr;
    unsigned dotdot_token;
    ValueExpressionAST *rboundExpr;

public:
    ForallInBoundsClauseAST()
        : ForallInClauseAST()
        , lboundExpr(0)
        , dotdot_token(0)
        , rboundExpr(0)
    {}

    virtual ForallInBoundsClauseAST *asForallInBoundsClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForallInIndicesClauseAST: public ForallInClauseAST
{
public:
    unsigned indices_token;
    unsigned of_token;
    PlSqlTypeNameAST *collectionName;
    unsigned between_token;
    ValueExpressionAST *lboundExpr;
    unsigned and_token;
    ValueExpressionAST *rboundExpr;

public:
    ForallInIndicesClauseAST()
        : ForallInClauseAST()
        , indices_token(0)
        , of_token(0)
        , collectionName(0)
        , between_token(0)
        , lboundExpr(0)
        , and_token(0)
        , rboundExpr(0)
    {}

    virtual ForallInIndicesClauseAST *asForallInIndicesClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForallInValuesClauseAST: public ForallInClauseAST
{
public:
    unsigned values_token;
    unsigned of_token;
    PlSqlNameAST *collectionName;

public:
    ForallInValuesClauseAST()
        : ForallInClauseAST()
        , values_token(0)
        , of_token(0)
        , collectionName(0)
    {}

    virtual ForallInValuesClauseAST *asForallInValuesClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WhileLoopStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned while_token;
    ConditionExpressionAST *condExpr;
    LoopStatementAST *loop;

public:
    WhileLoopStatementAST()
        : BasePlSqlStatementAST()
        , while_token(0)
        , condExpr(0)
        , loop(0)
    {}

    virtual WhileLoopStatementAST *asWhileLoopStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GotoStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned goto_token;
    PlSqlNameAST *labelName;    // PlSqlNameAST::Label

public:
    GotoStatementAST()
        : BasePlSqlStatementAST()
        , goto_token(0)
        , labelName(0)
    {}

    virtual GotoStatementAST *asGotoStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IfStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned if_token;
    ConditionExpressionAST *condExpr;
    unsigned then_token;
    PlSqlStatementListAST *trueStatements;
    ElsifClauseListAST *elsif_list;
    unsigned else_token;
    PlSqlStatementListAST *falseStatements;
    unsigned end_token;
    unsigned end_if_token;

public:
    IfStatementAST()
        : BasePlSqlStatementAST()
        , if_token(0)
        , condExpr(0)
        , then_token(0)
        , trueStatements(0)
        , elsif_list(0)
        , else_token(0)
        , falseStatements(0)
        , end_token(0)
        , end_if_token(0)
    {}

    virtual IfStatementAST *asIfStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ElsifClauseAST: public AST
{
public:
    unsigned elsif_token;
    ConditionExpressionAST *condExpr;
    unsigned then_token;
    PlSqlStatementListAST *statements;

public:
    ElsifClauseAST()
        : elsif_token(0)
        , condExpr(0)
        , then_token(0)
        , statements(0)
    {}

    virtual ElsifClauseAST *asElsifClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LoopStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned loop_token;
    PlSqlStatementListAST *statements;
    unsigned end_token;
    unsigned end_loop_token;
    PlSqlNameAST *labelName;    // PlSqlNameAST::Label

public:
    LoopStatementAST()
        : BasePlSqlStatementAST()
        , loop_token(0)
        , statements(0)
        , end_token(0)
        , end_loop_token(0)
        , labelName(0)
    {}

    virtual LoopStatementAST *asLoopStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NullStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned null_token;

public:
    NullStatementAST()
        : BasePlSqlStatementAST()
        , null_token(0)
    {}

    virtual NullStatementAST *asNullStatement() { return this; }

    virtual int firstToken() const { return null_token; }
    virtual int lastToken() const { return null_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpenCursorStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned open_token;
    PlSqlNameAST *cursorName;   // PlSqlNameAST::Cursor
    unsigned lparen_token;
    ValueExpressionListAST *par_list;
    unsigned rparen_token;

public:
    OpenCursorStatementAST()
        : BasePlSqlStatementAST()
        , open_token(0)
        , cursorName(0)
        , lparen_token(0)
        , par_list(0)
        , rparen_token(0)
    {}

    virtual OpenCursorStatementAST *asOpenCursorStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpenForStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned open_token;
    PlSqlNameAST *cursorVariableName;
    unsigned for_token;
    OpenForStatementClauseAST *selStatement;
    UsingClauseAST *usingClause;

public:
    OpenForStatementAST()
        : BasePlSqlStatementAST()
        , open_token(0)
        , cursorVariableName(0)
        , for_token(0)
        , selStatement(0)
        , usingClause(0)
    {}

    virtual OpenForStatementAST *asOpenForStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpenForStatementClauseAST: public AST
{
public:
    OpenForStatementClauseAST() {}
    virtual OpenForStatementClauseAST *asOpenForStatementClause() { return this; }
};

class OpenForStatementClauseBySelectAST: public OpenForStatementClauseAST
{
public:
    SelectStatementAST *selectStatement;

public:
    OpenForStatementClauseBySelectAST()
        : OpenForStatementClauseAST()
        , selectStatement(0)
    {}

    virtual OpenForStatementClauseBySelectAST *asOpenForStatementClauseBySelect() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpenForStatementClauseByStringAST: public OpenForStatementClauseAST
{
public:
    ValueExpressionAST *stmtExpr;

public:
    OpenForStatementClauseByStringAST()
        : OpenForStatementClauseAST()
        , stmtExpr(0)
    {}

    virtual OpenForStatementClauseByStringAST *asOpenForStatementClauseByString() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PlSqlBlocStatementAST: public BasePlSqlStatementAST
{
public:
    PlSqlBlockAST *innerBlock;

public:
    PlSqlBlocStatementAST()
        : BasePlSqlStatementAST()
        , innerBlock(0)
    {}

    virtual PlSqlBlocStatementAST *asPlSqlBlocStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RaiseStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned raise_token;
    PlSqlNameAST *exceptionName;    // PlSqlNameAST::Exception

public:
    RaiseStatementAST()
        : BasePlSqlStatementAST()
        , raise_token(0)
        , exceptionName(0)
    {}

    virtual RaiseStatementAST *asRaiseStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReturnStatementAST: public BasePlSqlStatementAST
{
public:
    unsigned return_token;
    ValueExpressionAST *returnExpr;

public:
    ReturnStatementAST()
        : BasePlSqlStatementAST()
        , return_token(0)
        , returnExpr(0)
    {}

    virtual ReturnStatementAST *asReturnStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeclareSectionAST: public AST
{
public:
    DeclarationListAST *decl_list;

public:
    DeclareSectionAST()
        : decl_list(0)
    {}

    virtual DeclareSectionAST *asDeclareSection() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeclarationAST: public AST
{
public:
    DeclarationAST() {}
    virtual DeclarationAST *asDeclaration() { return this; }
};

class CursorDeclarationAST: public DeclarationAST
{
public:
    CursorSpecAST *spec;
    unsigned is_token;
    StatementAST *selectStatement;

public:
    CursorDeclarationAST()
        : DeclarationAST()
        , spec(0)
        , is_token(0)
        , selectStatement(0)
    {}

    virtual CursorDeclarationAST *asCursorDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableDeclarationAST: public DeclarationAST
{
public:
    PlSqlNameAST *variableName;    // PlSqlNameAST::Constant || PlSqlNameAST::Variable (constant_token == 0)
    unsigned constant_token;
    DataTypeAST *type;
    unsigned not_token;
    unsigned null_token;
    unsigned assign_token;
    ValueExpressionAST *defaultExpr;

public:
    VariableDeclarationAST()
        : DeclarationAST()
        , variableName(0)
        , constant_token(0)
        , type(0)
        , not_token(0)
        , null_token(0)
        , assign_token(0)
        , defaultExpr(0)
    {}

    virtual VariableDeclarationAST *asVariableDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExceptionDeclarationAST: public DeclarationAST
{
public:
    PlSqlNameAST *exceptionName;    // PlSqlNameAST::Exception
    unsigned exception_token;

public:
    ExceptionDeclarationAST()
        : DeclarationAST()
        , exceptionName(0)
        , exception_token(0)
    {}

    virtual ExceptionDeclarationAST *asExceptionDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleDeclarationAST: public DeclarationAST
{
public:
    PlSqlNameAST *name;
    PlSqlTypeNameAST *type;

public:
    SimpleDeclarationAST()
        : DeclarationAST()
        , name(0)
        , type(0)
    {}

    virtual SimpleDeclarationAST *asSimpleDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionDeclarationAST: public DeclarationAST
{
public:
    FunctionHeadingAST *heading;
    FunctionSpecAST *spec;

public:
    FunctionDeclarationAST()
        : DeclarationAST()
        , heading(0)
        , spec(0)
    {}

    virtual FunctionDeclarationAST *asFunctionDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionDefintionAST: public DeclarationAST
{
public:
    FunctionHeadingAST *heading;
    FunctionSpecAST *spec;
    unsigned is_or_as_token;
    DeclareSectionAST *declareSection;
    PlSqlBlockBodyAST *body;

public:
    FunctionDefintionAST()
        : DeclarationAST()
        , heading(0)
        , spec(0)
        , is_or_as_token(0)
        , declareSection(0)
        , body(0)
    {}

    virtual FunctionDefintionAST *asFunctionDefintion() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ProcedureDeclarationAST: public DeclarationAST
{
public:
    ProcedureHeadingAST *heading;

public:
    ProcedureDeclarationAST()
        : DeclarationAST()
        , heading(0)
    {}

    virtual ProcedureDeclarationAST *asProcedureDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ProcedureDefinitionAST: public DeclarationAST
{
public:
    ProcedureHeadingAST *heading;
    unsigned is_or_as_token;
    DeclareSectionAST *declareSection;
    PlSqlBlockBodyAST *body;

public:
    ProcedureDefinitionAST()
        : DeclarationAST()
        , heading(0)
        , is_or_as_token(0)
        , declareSection(0)
        , body(0)
    {}

    virtual ProcedureDefinitionAST *asProcedureDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeDefinitionDeclarationAST: public DeclarationAST
{
public:
    TypeDefinitionAST *def;

public:
    TypeDefinitionDeclarationAST()
        : DeclarationAST()
        , def(0)
    {}

    virtual TypeDefinitionDeclarationAST *asTypeDefinitionDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeDefinitionAST: public DeclarationAST
{
public:
    TypeDefinitionAST() : DeclarationAST() {}
    virtual TypeDefinitionAST *asTypeDefinition() { return this; }
};

class RecordTypeDefinitionAST: public TypeDefinitionAST
{
public:
    unsigned type_token;
    PlSqlTypeNameAST *typeName; // PlSqlTypeNameAST::Record
    unsigned is_token;
    unsigned record_token;
    unsigned lparen_token;
    RecordFieldDeclarationListAST *field_list;
    unsigned rparen_token;

public:
    RecordTypeDefinitionAST()
        : TypeDefinitionAST()
        , type_token(0)
        , typeName(0)
        , is_token(0)
        , record_token(0)
        , lparen_token(0)
        , field_list(0)
        , rparen_token(0)
    {}

    virtual RecordTypeDefinitionAST *asRecordTypeDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RefCursorTypeDefinitionAST: public TypeDefinitionAST
{
public:
    unsigned type_token;
    PlSqlTypeNameAST *typeName; // PlSqlTypeNameAST::RefCursor
    unsigned is_token;
    unsigned ref_token;
    unsigned cursor_token;
    unsigned return_token;
    RowTypeAST *returnType;

public:
    RefCursorTypeDefinitionAST()
        : TypeDefinitionAST()
        , type_token(0)
        , typeName(0)
        , is_token(0)
        , ref_token(0)
        , cursor_token(0)
        , return_token(0)
        , returnType(0)
    {}

    virtual RefCursorTypeDefinitionAST *asRefCursorTypeDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubtypeDefinitionAST: public TypeDefinitionAST
{
public:
    unsigned subtype_token;
    PlSqlTypeNameAST *subtypeName;  // PlSqlTypeNameAST::Subtype
    unsigned is_token;
    PlSqlTypeNameAST *typeName;
    unsigned lparen_token;
    TableConstraintListAST *constraintList;
    unsigned rparen_token;
    unsigned not_token;
    unsigned null_token;

public:
    SubtypeDefinitionAST()
        : TypeDefinitionAST()
        , subtype_token(0)
        , subtypeName(0)
        , is_token(0)
        , typeName(0)
        , lparen_token(0)
        , constraintList(0)
        , rparen_token(0)
        , not_token(0)
        , null_token(0)
    {}

    virtual SubtypeDefinitionAST *asSubtypeDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollectionTypeDefinitionAST: public TypeDefinitionAST
{
public:
    unsigned type_token;
    PlSqlTypeNameAST *typeName;
    unsigned is_token;
    CollectionTypeAST *collectionType;

public:
    CollectionTypeDefinitionAST()
        : TypeDefinitionAST()
        , type_token(0)
        , typeName(0)
        , is_token(0)
        , collectionType(0)
    {}

    virtual CollectionTypeDefinitionAST *asCollectionTypeDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollectionTypeAST: public AST
{
public:
    CollectionTypeAST() {}
    virtual CollectionTypeAST *asCollectionType() { return this; }
};

class TableCollectionTypeAST: public CollectionTypeAST
{
public:
    unsigned table_token;
    unsigned of_token;
    PlSqlTypeNameAST *elementType;  // PlSqlTypeNameAST::Element
    unsigned not_token;
    unsigned null_token;

public:
    TableCollectionTypeAST()
        : CollectionTypeAST()
        , table_token(0)
        , of_token(0)
        , elementType(0)
        , not_token(0)
        , null_token(0)
    {}

    virtual TableCollectionTypeAST *asTableCollectionType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrayCollectionTypeAST: public CollectionTypeAST
{
public:
    unsigned arr0_token;
    unsigned arr1_token;
    unsigned lparen_token;
    NumericExpressionAST *size;
    unsigned rparen_token;
    unsigned of_token;
    PlSqlTypeNameAST *elementType;
    unsigned not_token;
    unsigned null_token;

public:
    ArrayCollectionTypeAST()
        : CollectionTypeAST()
        , arr0_token(0)
        , arr1_token(0)
        , lparen_token(0)
        , size(0)
        , rparen_token(0)
        , of_token(0)
        , elementType(0)
        , not_token(0)
        , null_token(0)
    {}

    virtual ArrayCollectionTypeAST *asArrayCollectionType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CursorSpecAST: public AST
{
public:
    unsigned cursor_token;
    PlSqlNameAST *cursorName;
    unsigned lparen_token;
    CursorParameterDeclarationListAST *params_list;
    unsigned rparen_token;
    unsigned return_token;
    RowTypeAST *rowType;

public:
    CursorSpecAST()
        : cursor_token(0)
        , cursorName(0)
        , lparen_token(0)
        , params_list(0)
        , rparen_token(0)
        , return_token(0)
        , rowType(0)
    {}

    virtual CursorSpecAST *asCursorSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RowTypeAST: public AST
{
public:
    enum Spec {
        Rowtype,
        Type,
        Name
    };
    Spec spec;

public:
    PlSqlBaseNameAST *typeName;
    unsigned percent_token;
    unsigned row_or_rowtype_token;

public:
    RowTypeAST( Spec spec_ = Name )
        : spec(spec_)
        , typeName(0)
        , percent_token(0)
        , row_or_rowtype_token(0)
    {}

    virtual RowTypeAST *asRowType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeAST: public AST
{
public:
    DataTypeAST() {}
    virtual DataTypeAST *asDataType() { return this; }
};

class DataTypeByTypeNameAST: public DataTypeAST
{
public:
    unsigned ref_token;
    PlSqlTypeNameAST *typeName;

public:
    DataTypeByTypeNameAST()
        : DataTypeAST()
        , ref_token(0)
        , typeName(0)
    {}

    virtual DataTypeByTypeNameAST *asDataTypeByTypeName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeByNameAST: public DataTypeAST
{
public:
    PlSqlNameAST *variableName; // PlSqlNameAST::Variable
    unsigned percent_token;
    unsigned type_token;

public:
    DataTypeByNameAST()
        : DataTypeAST()
        , variableName(0)
        , percent_token(0)
        , type_token(0)
    {}

    virtual DataTypeByNameAST *asDataTypeByName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeByTableRowAST: public DataTypeAST
{
public:
    NameAST *tableName;
    unsigned percent_token;
    unsigned rowtype_token;

public:
    DataTypeByTableRowAST()
        : DataTypeAST()
        , tableName(0)
        , percent_token(0)
        , rowtype_token(0)
    {}

    virtual DataTypeByTableRowAST *asDataTypeByTableRow() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeByTableColumnAST: public DataTypeAST
{
public:
    NameAST *tableName;
    unsigned dot_token;
    NameAST *columnName;
    unsigned percent_token;
    unsigned type_token;

public:
    DataTypeByTableColumnAST()
        : DataTypeAST()
        , tableName(0)
        , dot_token(0)
        , columnName(0)
        , percent_token(0)
        , type_token(0)
    {}

    virtual DataTypeByTableColumnAST *asDataTypeByTableColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ScalarDataTypeAST: public DataTypeAST
{
public:
    BaseTypeAST *type;

public:
    ScalarDataTypeAST()
        : DataTypeAST()
        , type(0)
    {}

    virtual ScalarDataTypeAST *asScalarDataType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CursorParameterDeclarationAST: public AST
{
public:
    PlSqlNameAST *paramName;    // PlSqlNameAST::CursorParameter
    unsigned in_token;
    DataTypeAST *type;
    unsigned assign_token;
    ValueExpressionAST *defaultExpr;

public:
    CursorParameterDeclarationAST()
        : paramName(0)
        , in_token(0)
        , type(0)
        , assign_token(0)
        , defaultExpr(0)
    {}

    virtual CursorParameterDeclarationAST *asCursorParameterDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RecordFieldDeclarationAST: public AST
{
public:
    PlSqlNameAST *fieldName;    // PlSqlNameAST::RecordField
    DataTypeAST *type;
    unsigned not_token;
    unsigned null_token;
    unsigned assign_token;
    ValueExpressionAST *defaultExpr;

public:
    RecordFieldDeclarationAST()
        : fieldName(0)
        , type(0)
        , not_token(0)
        , null_token(0)
        , assign_token(0)
        , defaultExpr(0)
    {}

    virtual RecordFieldDeclarationAST *asRecordFieldDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineArgumentDeclarationAST: public AST
{
public:
    PlSqlNameAST *argName;  // PlSqlNameAST::RoutineArgument
    unsigned in_token;
    unsigned out_token;
    unsigned nocopy_token;
    DataTypeAST *type;
    unsigned assign_token;
    ValueExpressionAST *defaultExpr;

public:
    RoutineArgumentDeclarationAST()
        : argName(0)
        , in_token(0)
        , out_token(0)
        , nocopy_token(0)
        , type(0)
        , assign_token(0)
        , defaultExpr(0)
    {}

    virtual RoutineArgumentDeclarationAST *asRoutineArgumentDeclaration() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionHeadingAST: public AST
{
public:
    unsigned function_token;
    PlSqlNameAST *functionName; // PlSqlNameAST::Function
    unsigned lparen_token;
    RoutineArgumentDeclarationListAST *arg_list;
    unsigned rparen_token;
    unsigned return_token;
    DataTypeAST *type;

public:
    FunctionHeadingAST()
        : function_token(0)
        , functionName(0)
        , lparen_token(0)
        , arg_list(0)
        , rparen_token(0)
        , return_token(0)
        , type(0)
    {}

    virtual FunctionHeadingAST *asFunctionHeading() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionSpecAST: public AST
{
public:
    FunctionSpecAST() {}
    virtual FunctionSpecAST *asFunctionSpec() { return this; }
};

class FunctionDeterministicOrPipelinedSpecAST: public FunctionSpecAST
{
public:
    unsigned spec_token;

public:
    FunctionDeterministicOrPipelinedSpecAST()
        : FunctionSpecAST()
        , spec_token(0)
    {}

    virtual FunctionDeterministicOrPipelinedSpecAST *asFunctionDeterministicOrPipelinedSpec() { return this; }

    virtual int firstToken() const { return spec_token; }
    virtual int lastToken() const { return spec_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionCasheSpecAST: public FunctionSpecAST
{
public:
    unsigned result_cashe_token;
    unsigned relies_on_token;
    unsigned lparen_token;
    NameListAST *source_list;
    unsigned rparen_token;

public:
    FunctionCasheSpecAST()
        : FunctionSpecAST()
        , result_cashe_token(0)
        , relies_on_token(0)
        , lparen_token(0)
        , source_list(0)
        , rparen_token(0)
    {}

    virtual FunctionCasheSpecAST *asFunctionCasheSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ProcedureHeadingAST: public AST
{
public:
    unsigned procedure_token;
    PlSqlNameAST *procedureName; // PlSqlNameAST::Procedure
    unsigned lparen_token;
    RoutineArgumentDeclarationListAST *arg_list;
    unsigned rparen_token;

public:
    ProcedureHeadingAST()
        : procedure_token(0)
        , procedureName(0)
        , lparen_token(0)
        , arg_list(0)
        , rparen_token(0)
    {}

    virtual ProcedureHeadingAST *asProcedureHeading() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExceptionHandlerAST: public AST
{
public:
    ExceptionHandlerAST() {}
    virtual ExceptionHandlerAST *asExceptionHandler() { return this; }
};

class NamedExceptionHandlerAST: public ExceptionHandlerAST
{
public:
    unsigned when_token;
    PlSqlNameListAST *name_list;
    unsigned then_token;
    PlSqlStatementListAST *statements;

public:
    NamedExceptionHandlerAST()
        : ExceptionHandlerAST()
        , when_token(0)
        , name_list(0)
        , then_token(0)
        , statements(0)
    {}

    virtual NamedExceptionHandlerAST *asNamedExceptionHandler() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OthersExceptionHandlerAST: public ExceptionHandlerAST
{
public:
    unsigned when_token;
    unsigned others_token;
    unsigned then_token;
    PlSqlStatementListAST *statements;

public:
    OthersExceptionHandlerAST()
        : ExceptionHandlerAST()
        , when_token(0)
        , others_token(0)
        , then_token(0)
        , statements(0)
    {}

    virtual OthersExceptionHandlerAST *asOthersExceptionHandler() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseIntoClauseAST: public AST
{
public:
    BaseIntoClauseAST() {}
    virtual BaseIntoClauseAST *asBaseIntoClause() { return this; }
};

class IntoClauseAST: public BaseIntoClauseAST
{
public:
    unsigned into_token;
    LValueExpressionListAST *names;

public:
    IntoClauseAST()
        : BaseIntoClauseAST()
        , into_token(0)
        , names(0)
    {}

    virtual IntoClauseAST *asIntoClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BulkIntoClauseAST: public BaseIntoClauseAST
{
public:
    unsigned bulk_token;
    unsigned collect_token;
    unsigned into_token;
    LValueExpressionListAST *names;

public:
    BulkIntoClauseAST()
        : BaseIntoClauseAST()
        , bulk_token(0)
        , collect_token(0)
        , into_token(0)
        , names(0)
    {}

    virtual BulkIntoClauseAST *asBulkIntoClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingClauseAST: public AST
{
public:
    unsigned using_token;
    UsingBindListAST *bind_list;

public:
    UsingClauseAST()
        : using_token(0)
        , bind_list(0)
    {}

    virtual UsingClauseAST *asUsingClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingBindAST: public AST
{
public:
    unsigned in_token;
    unsigned out_token;
    ValueExpressionAST *bindExpr;

public:
    UsingBindAST()
        : in_token(0)
        , out_token(0)
        , bindExpr(0)
    {}

    virtual UsingBindAST *asUsingBind() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DynamicReturningClauseAST: public AST
{
public:
    unsigned return_token;
    BaseIntoClauseAST *intoClause;

public:
    DynamicReturningClauseAST()
        : return_token(0)
        , intoClause(0)
    {}

    virtual DynamicReturningClauseAST *asDynamicReturningClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrayIndexExpressionAST: public AST
{
public:
    unsigned lparen_token;
    ValueExpressionAST *indexExpr;
    unsigned rparen_token;

public:
    ArrayIndexExpressionAST()
        : lparen_token(0)
        , indexExpr(0)
        , rparen_token(0)
    {}

    virtual ArrayIndexExpressionAST *asArrayIndexExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnIdentifierAST: public SimpleValueExpressionAST
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
        : SimpleValueExpressionAST()
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
class SqlCheckNamesWalker: public Sql::BaseWalker
{
public:
    SqlCheckNamesWalker( AST *ast, Sql::TranslationUnit *translationUnit )
        : BaseWalker()
        , m_ast(ast)
        , m_translationUnit(translationUnit)
    {}

    virtual void walk( Sql::BaseWalkReflector *reflector );

private:
    AST *m_ast;
    Sql::TranslationUnit *m_translationUnit;
};

} // namespace QLalrOracle

#endif // QLALR_ORACLE_AST_H
