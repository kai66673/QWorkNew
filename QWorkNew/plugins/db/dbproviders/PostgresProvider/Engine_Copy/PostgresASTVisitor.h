#ifndef POSTGRESASTVISITOR_H
#define POSTGRESASTVISITOR_H

#include "PostgresASTfwd.h"
#include "TranslationUnit.h"

namespace Postgres {

class ASTVisitor
{
    ASTVisitor(const ASTVisitor &other);
    void operator =(const ASTVisitor &other);

public:
    ASTVisitor( Sql::TranslationUnit *translationUnit );

    Sql::TranslationUnit *translationUnit() { return m_translationUnit; }

    const Sql::Token &tokenAt( unsigned index ) const;

    void accept( AST *ast );

    void getPosition( unsigned offset,
                      unsigned *line,
                      unsigned *column = 0,
                      const Sql::StringLiteral **fileName = 0 ) const;

    void getTokenPosition( unsigned index,
                           unsigned *line,
                           unsigned *column = 0,
                           const Sql::StringLiteral **fileName = 0 ) const;

    void getTokenStartPosition( unsigned index, unsigned *line, unsigned *column ) const;
    void getTokenEndPosition( unsigned index, unsigned *line, unsigned *column ) const;

    virtual bool preVisit(AST *) { return true; }
    virtual void postVisit(AST *) {}

    virtual bool visit(EmptyAST *) { return true; }
    virtual bool visit(TranslationUnitAST *) { return true; }
    virtual bool visit(UnknownStatementAST *) { return true; }
    virtual bool visit(SelectStatementAST *) { return true; }
    virtual bool visit(DeleteStatementAST *) { return true; }
    virtual bool visit(UpdateSetClauseAST *) { return true; }
    virtual bool visit(AssignmentExpressionAST *) { return true; }
    virtual bool visit(UpdateStatementAST *) { return true; }
    virtual bool visit(InsertStatementAST *) { return true; }
    virtual bool visit(InsertColumnsClauseAST *) { return true; }
    virtual bool visit(InsertClauseAsValuesAST *) { return true; }
    virtual bool visit(InsertClauseAsSelectAST *) { return true; }
    virtual bool visit(OrderByBlockAST *) { return true; }
    virtual bool visit(OrderingTermAST *) { return true; }
    virtual bool visit(ForUpdateClauseAST *) { return true; }
    virtual bool visit(SelectCoreAST *) { return true; }
    virtual bool visit(StarSelectColumnAST *) { return true; }
    virtual bool visit(TableSelectColumnAST *) { return true; }
    virtual bool visit(ExpressionSelectColumnAST *) { return true; }
    virtual bool visit(CompoundJoinedTableAST *) { return true; }
    virtual bool visit(JoinJoinedTableAST *) { return true; }
    virtual bool visit(DirectSingleSourceAST *) { return true; }
    virtual bool visit(SubQuerySingleSourceAST *) { return true; }
    virtual bool visit(JoinSpecAST *) { return true; }
    virtual bool visit(JoinConditionAST *) { return true; }
    virtual bool visit(WhereClauseAST *) { return true; }
    virtual bool visit(GroupByBlockAST *) { return true; }
    virtual bool visit(SetClauseAST *) { return true; }
    virtual bool visit(MinusSetSpecAST *) { return true; }
    virtual bool visit(IntersectSetSpecAST *) { return true; }
    virtual bool visit(UnionSetSpecAST *) { return true; }
    virtual bool visit(CreateTableStatementAST *) { return true; }
    virtual bool visit(CreateViewStatementAST *) { return true; }
    virtual bool visit(CreateIndexStatementAST *) { return true; }
    virtual bool visit(CreateTriggerStatementAST *) { return true; }
    virtual bool visit(TriggerWhetherBeforeOrAfterAST *) { return true; }
    virtual bool visit(TriggerWhetherInsteadOfAST *) { return true; }
    virtual bool visit(TriggerEventUpdateAST *) { return true; }
    virtual bool visit(TriggerEventIdtAST *) { return true; }
    virtual bool visit(TriggerFromClauseAST *) { return true; }
    virtual bool visit(TriggerNotDeferrableTimingAST *) { return true; }
    virtual bool visit(TriggerDeferrableTimingAST *) { return true; }
    virtual bool visit(TriggerForClauseAST *) { return true; }
    virtual bool visit(CreateTableAsSelectClauseAST *) { return true; }
    virtual bool visit(CreateTableDirectClauseAST *) { return true; }
    virtual bool visit(ColumnDefinitionAST *) { return true; }
    virtual bool visit(ConstraintStateAST *) { return true; }
    virtual bool visit(ConstraintDeferStateAST *) { return true; }
    virtual bool visit(UsingIndexClauseAST *) { return true; }
    virtual bool visit(ExceptionsClauseAST *) { return true; }
    virtual bool visit(TableCreateIndexClauseAST *) { return true; }
    virtual bool visit(DropTableStatementAST *) { return true; }
    virtual bool visit(DropViewStatementAST *) { return true; }
    virtual bool visit(DropIndexStatementAST *) { return true; }
    virtual bool visit(DropTriggerStatementAST *) { return true; }
    virtual bool visit(TruncateStatementAST *) { return true; }
    virtual bool visit(AlterTableStatementAST *) { return true; }
    virtual bool visit(IfExistsAST *) { return true; }
    virtual bool visit(NotValidClauseAST *) { return true; }
    virtual bool visit(DeferrableClauseAST *) { return true; }
    virtual bool visit(ConcreteTriggerAST *) { return true; }
    virtual bool visit(AllOrUserTriggersAST *) { return true; }
    virtual bool visit(AlterTableRenameActionAST *) { return true; }
    virtual bool visit(AlterTableRenameColumnActionAST *) { return true; }
    virtual bool visit(AlterTableRenameConstraintActionAST *) { return true; }
    virtual bool visit(AlterTableAddConstraintActionAST *) { return true; }
    virtual bool visit(AlterTableAddColumnActionAST *) { return true; }
    virtual bool visit(AlterTableDropColumnActionAST *) { return true; }
    virtual bool visit(AlterTableDropConstraintActionAST *) { return true; }
    virtual bool visit(AlterTableAbleTriggerActionAST *) { return true; }
    virtual bool visit(AlterTableAlterColumnActionAST *) { return true; }
    virtual bool visit(AlterColumnSetTypeAST *) { return true; }
    virtual bool visit(AlterColumnSetDefaultAST *) { return true; }
    virtual bool visit(AlterColumnDropDefaultAST *) { return true; }
    virtual bool visit(AlterColumnSetOrDropNotNullAST *) { return true; }
    virtual bool visit(CommentOnTableStatementAST *) { return true; }
    virtual bool visit(CommentOnColumnStatementAST *) { return true; }
    virtual bool visit(CommitStatementAST *) { return true; }
    virtual bool visit(RollbackStatementAST *) { return true; }
    virtual bool visit(CompoundValueExpressionAST *) { return true; }
    virtual bool visit(CaseExpressionClauseAST *) { return true; }
    virtual bool visit(CaseExpressionAST *) { return true; }
    virtual bool visit(CaseConditionClauseAST *) { return true; }
    virtual bool visit(CaseConditionAST *) { return true; }
    virtual bool visit(TableIdentifierAST *) { return true; }
    virtual bool visit(IndexIdentifierAST *) { return true; }
    virtual bool visit(TriggerIdentifierAST *) { return true; }
    virtual bool visit(ColumnIdentifierAST *) { return true; }
    virtual bool visit(FunctionIdentifierAST *) { return true; }
    virtual bool visit(CollateClauseAST *) { return true; }
    virtual bool visit(FunctionCallAST *) { return true; }
    virtual bool visit(CountStarCallAST *) { return true; }
    virtual bool visit(CountExprCallAST *) { return true; }
    virtual bool visit(CastFunctionAST *) { return true; }
    virtual bool visit(SubstringFunctionAST *) { return true; }
    virtual bool visit(BuiltinFunctionCallAST *) { return true; }
    virtual bool visit(UnaryValueExpressionAST *) { return true; }
    virtual bool visit(BinaryValueExpressionAST *) { return true; }
    virtual bool visit(LiteralExpressionAST *) { return true; }
    virtual bool visit(NumericExpressionAST *) { return true; }
    virtual bool visit(NullValueExpressionAST *) { return true; }
    virtual bool visit(ValueListExpressionAST *) { return true; }
    virtual bool visit(SubQueryExpressionAST *) { return true; }
    virtual bool visit(ParameterExpressionAST *) { return true; }
    virtual bool visit(SelectCompoundValueExpressionAST *) { return true; }
    virtual bool visit(NotConditionExpressionAST *) { return true; }
    virtual bool visit(TrueConditionExpressionAST *) { return true; }
    virtual bool visit(FalseConditionExpressionAST *) { return true; }
    virtual bool visit(BinaryConditionExpressionAST *) { return true; }
    virtual bool visit(CompareConditionExpressionAST *) { return true; }
    virtual bool visit(CompoundConditionExpressionAST *) { return true; }
    virtual bool visit(LikeConditionExpressionAST *) { return true; }
    virtual bool visit(BetweenConditionExpressionAST *) { return true; }
    virtual bool visit(InConditionExpressionAST *) { return true; }
    virtual bool visit(ExistsConditionExpressionAST *) { return true; }
    virtual bool visit(IsNullConditionExpressionAST *) { return true; }
    virtual bool visit(ConditionAsValueExpressionAST *) { return true; }
    virtual bool visit(CharacterTypeAST *) { return true; }
    virtual bool visit(NumberTypeAST *) { return true; }
    virtual bool visit(SimpleTypeAST *) { return true; }
    virtual bool visit(UserDefinedTypeAST *) { return true; }
    virtual bool visit(ConstraintDeclarationAST *) { return true; }
    virtual bool visit(NameAST *) { return true; }
    virtual bool visit(SortedColumnNameAST *) { return true; }
    virtual bool visit(UniqueOrPkTableConstraintAST *) { return true; }
    virtual bool visit(UniqueOrPkTableConstraintUsingIndexAST *) { return true; }
    virtual bool visit(CheckTableConstraintAST *) { return true; }
    virtual bool visit(FkReferencesClauseAST *) { return true; }
    virtual bool visit(FkReferencesMatchAST *) { return true; }
    virtual bool visit(FkReferencesActionAST *) { return true; }
    virtual bool visit(FkTableConstraintAST *) { return true; }
    virtual bool visit(PkColumnConstraintAST *) { return true; }
    virtual bool visit(NotNullColumnConstraintAST *) { return true; }
    virtual bool visit(UniqueColumnConstraintAST *) { return true; }
    virtual bool visit(CheckColumnConstraintAST *) { return true; }
    virtual bool visit(ReferencesColumnConstraintAST *) { return true; }

    virtual void endVisit(EmptyAST *) {}
    virtual void endVisit(TranslationUnitAST *) {}
    virtual void endVisit(UnknownStatementAST *) {}
    virtual void endVisit(SelectStatementAST *) {}
    virtual void endVisit(DeleteStatementAST *) {}
    virtual void endVisit(UpdateSetClauseAST *) {}
    virtual void endVisit(AssignmentExpressionAST *) {}
    virtual void endVisit(InsertStatementAST *) {}
    virtual void endVisit(InsertColumnsClauseAST *) {}
    virtual void endVisit(InsertClauseAsValuesAST *) {}
    virtual void endVisit(InsertClauseAsSelectAST *) {}
    virtual void endVisit(UpdateStatementAST *) {}
    virtual void endVisit(OrderByBlockAST *) {}
    virtual void endVisit(OrderingTermAST *) {}
    virtual void endVisit(ForUpdateClauseAST *) {}
    virtual void endVisit(SelectCoreAST *) {}
    virtual void endVisit(StarSelectColumnAST *) {}
    virtual void endVisit(TableSelectColumnAST *) {}
    virtual void endVisit(ExpressionSelectColumnAST *) {}
    virtual void endVisit(CompoundJoinedTableAST *) {}
    virtual void endVisit(JoinJoinedTableAST *) {}
    virtual void endVisit(SubQuerySingleSourceAST *) {}
    virtual void endVisit(DirectSingleSourceAST *) {}
    virtual void endVisit(JoinSpecAST *) {}
    virtual void endVisit(JoinConditionAST *) {}
    virtual void endVisit(WhereClauseAST *) {}
    virtual void endVisit(GroupByBlockAST *) {}
    virtual void endVisit(SetClauseAST *) {}
    virtual void endVisit(MinusSetSpecAST *) {}
    virtual void endVisit(IntersectSetSpecAST *) {}
    virtual void endVisit(UnionSetSpecAST *) {}
    virtual void endVisit(CreateTableStatementAST *) {}
    virtual void endVisit(CreateViewStatementAST *) {}
    virtual void endVisit(CreateIndexStatementAST *) {}
    virtual void endVisit(CreateTriggerStatementAST *) {}
    virtual void endVisit(TriggerWhetherBeforeOrAfterAST *) {}
    virtual void endVisit(TriggerWhetherInsteadOfAST *) {}
    virtual void endVisit(TriggerEventUpdateAST *) {}
    virtual void endVisit(TriggerEventIdtAST *) {}
    virtual void endVisit(TriggerFromClauseAST *) {}
    virtual void endVisit(TriggerNotDeferrableTimingAST *) {}
    virtual void endVisit(TriggerDeferrableTimingAST *) {}
    virtual void endVisit(TriggerForClauseAST *) {}
    virtual void endVisit(CreateTableAsSelectClauseAST *) {}
    virtual void endVisit(CreateTableDirectClauseAST *) {}
    virtual void endVisit(ColumnDefinitionAST *) {}
    virtual void endVisit(ConstraintStateAST *) {}
    virtual void endVisit(ConstraintDeferStateAST *) {}
    virtual void endVisit(UsingIndexClauseAST *) {}
    virtual void endVisit(ExceptionsClauseAST *) {}
    virtual void endVisit(TableCreateIndexClauseAST *) {}
    virtual void endVisit(DropTableStatementAST *) {}
    virtual void endVisit(DropViewStatementAST *) {}
    virtual void endVisit(DropIndexStatementAST *) {}
    virtual void endVisit(DropTriggerStatementAST *) {}
    virtual void endVisit(TruncateStatementAST *) {}
    virtual void endVisit(AlterTableStatementAST *) {}
    virtual void endVisit(IfExistsAST *) {}
    virtual void endVisit(NotValidClauseAST *) {}
    virtual void endVisit(DeferrableClauseAST *) {}
    virtual void endVisit(ConcreteTriggerAST *) {}
    virtual void endVisit(AllOrUserTriggersAST *) {}
    virtual void endVisit(AlterTableRenameActionAST *) {}
    virtual void endVisit(AlterTableRenameColumnActionAST *) {}
    virtual void endVisit(AlterTableRenameConstraintActionAST *) {}
    virtual void endVisit(AlterTableAddConstraintActionAST *) {}
    virtual void endVisit(AlterTableAddColumnActionAST *) {}
    virtual void endVisit(AlterTableDropColumnActionAST *) {}
    virtual void endVisit(AlterTableDropConstraintActionAST *) {}
    virtual void endVisit(AlterTableAbleTriggerActionAST *) {}
    virtual void endVisit(AlterTableAlterColumnActionAST *) {}
    virtual void endVisit(AlterColumnSetTypeAST *) {}
    virtual void endVisit(AlterColumnSetDefaultAST *) {}
    virtual void endVisit(AlterColumnDropDefaultAST *) {}
    virtual void endVisit(AlterColumnSetOrDropNotNullAST *) {}
    virtual void endVisit(CommentOnTableStatementAST *) {}
    virtual void endVisit(CommentOnColumnStatementAST *) {}
    virtual void endVisit(CommitStatementAST *) {}
    virtual void endVisit(RollbackStatementAST *) {}
    virtual void endVisit(CompoundValueExpressionAST *) {}
    virtual void endVisit(CaseExpressionClauseAST *) {}
    virtual void endVisit(CaseExpressionAST *) {}
    virtual void endVisit(CaseConditionClauseAST *) {}
    virtual void endVisit(CaseConditionAST *) {}
    virtual void endVisit(TableIdentifierAST *) {}
    virtual void endVisit(IndexIdentifierAST *) {}
    virtual void endVisit(TriggerIdentifierAST *) {}
    virtual void endVisit(ColumnIdentifierAST *) {}
    virtual void endVisit(FunctionIdentifierAST *) {}
    virtual void endVisit(CollateClauseAST *) {}
    virtual void endVisit(FunctionCallAST *) {}
    virtual void endVisit(CountStarCallAST *) {}
    virtual void endVisit(CountExprCallAST *) {}
    virtual void endVisit(CastFunctionAST *) {}
    virtual void endVisit(SubstringFunctionAST *) {}
    virtual void endVisit(BuiltinFunctionCallAST *) {}
    virtual void endVisit(UnaryValueExpressionAST *) {}
    virtual void endVisit(BinaryValueExpressionAST *) {}
    virtual void endVisit(LiteralExpressionAST *) {}
    virtual void endVisit(NumericExpressionAST *) {}
    virtual void endVisit(NullValueExpressionAST *) {}
    virtual void endVisit(ValueListExpressionAST *) {}
    virtual void endVisit(SubQueryExpressionAST *) {}
    virtual void endVisit(ParameterExpressionAST *) {}
    virtual void endVisit(SelectCompoundValueExpressionAST *) {}
    virtual void endVisit(NotConditionExpressionAST *) {}
    virtual void endVisit(TrueConditionExpressionAST *) {}
    virtual void endVisit(FalseConditionExpressionAST *) {}
    virtual void endVisit(BinaryConditionExpressionAST *) {}
    virtual void endVisit(CompareConditionExpressionAST *) {}
    virtual void endVisit(CompoundConditionExpressionAST *) {}
    virtual void endVisit(LikeConditionExpressionAST *) {}
    virtual void endVisit(BetweenConditionExpressionAST *) {}
    virtual void endVisit(InConditionExpressionAST *) {}
    virtual void endVisit(ExistsConditionExpressionAST *) {}
    virtual void endVisit(IsNullConditionExpressionAST *) {}
    virtual void endVisit(ConditionAsValueExpressionAST *) {}
    virtual void endVisit(CharacterTypeAST *) {}
    virtual void endVisit(NumberTypeAST *) {}
    virtual void endVisit(SimpleTypeAST *) {}
    virtual void endVisit(UserDefinedTypeAST *) {}
    virtual void endVisit(ConstraintDeclarationAST *) {}
    virtual void endVisit(NameAST *) {}
    virtual void endVisit(SortedColumnNameAST *) {}
    virtual void endVisit(UniqueOrPkTableConstraintAST *) {}
    virtual void endVisit(UniqueOrPkTableConstraintUsingIndexAST *) {}
    virtual void endVisit(CheckTableConstraintAST *) {}
    virtual void endVisit(FkReferencesClauseAST *) {}
    virtual void endVisit(FkReferencesMatchAST *) {}
    virtual void endVisit(FkReferencesActionAST *) {}
    virtual void endVisit(FkTableConstraintAST *) {}
    virtual void endVisit(PkColumnConstraintAST *) {}
    virtual void endVisit(NotNullColumnConstraintAST *) {}
    virtual void endVisit(UniqueColumnConstraintAST *) {}
    virtual void endVisit(CheckColumnConstraintAST *) {}
    virtual void endVisit(ReferencesColumnConstraintAST *) {}

private:
    Sql::TranslationUnit *m_translationUnit;
};

}   // namespace Postgres

#endif // POSTGRESASTVISITOR_H
