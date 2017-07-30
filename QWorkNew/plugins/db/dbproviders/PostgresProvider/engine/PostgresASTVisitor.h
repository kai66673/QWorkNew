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
    virtual void endVisit(EmptyAST *) {}
    virtual bool visit(NameAST *) { return true; }
    virtual void endVisit(NameAST *) {}
    virtual bool visit(TranslationUnitAST *) { return true; }
    virtual void endVisit(TranslationUnitAST *) {}
    virtual bool visit(DdlStatementAST *) { return true; }
    virtual void endVisit(DdlStatementAST *) {}
    virtual bool visit(DmlStatementAST *) { return true; }
    virtual void endVisit(DmlStatementAST *) {}
    virtual bool visit(UnknownStatementAST *) { return true; }
    virtual void endVisit(UnknownStatementAST *) {}
    virtual bool visit(CreateStatementAST *) { return true; }
    virtual void endVisit(CreateStatementAST *) {}
    virtual bool visit(CreateViewStmtAST *) { return true; }
    virtual void endVisit(CreateViewStmtAST *) {}
    virtual bool visit(WithCheckOptionClauseAST *) { return true; }
    virtual void endVisit(WithCheckOptionClauseAST *) {}
    virtual bool visit(VacuumStmtAST *) { return true; }
    virtual void endVisit(VacuumStmtAST *) {}
    virtual bool visit(CommonVacuumStmtAST *) { return true; }
    virtual void endVisit(CommonVacuumStmtAST *) {}
    virtual bool visit(VacuumOptionAST *) { return true; }
    virtual void endVisit(VacuumOptionAST *) {}
    virtual bool visit(VacuumTableStmtAST *) { return true; }
    virtual void endVisit(VacuumTableStmtAST *) {}
    virtual bool visit(VacuumAnalyzeStmtAST *) { return true; }
    virtual void endVisit(VacuumAnalyzeStmtAST *) {}
    virtual bool visit(TruncateStmtAST *) { return true; }
    virtual void endVisit(TruncateStmtAST *) {}
    virtual bool visit(TruncateRestartClauseAST *) { return true; }
    virtual void endVisit(TruncateRestartClauseAST *) {}
    virtual bool visit(TransactionStmtAST *) { return true; }
    virtual void endVisit(TransactionStmtAST *) {}
    virtual bool visit(BeginTransactionStmtAST *) { return true; }
    virtual void endVisit(BeginTransactionStmtAST *) {}
    virtual bool visit(SavepointTransactionStmtAST *) { return true; }
    virtual void endVisit(SavepointTransactionStmtAST *) {}
    virtual bool visit(ReleaseTransactionStmtAST *) { return true; }
    virtual void endVisit(ReleaseTransactionStmtAST *) {}
    virtual bool visit(CommitTransactionStmtAST *) { return true; }
    virtual void endVisit(CommitTransactionStmtAST *) {}
    virtual bool visit(RollbackTransactionStmtAST *) { return true; }
    virtual void endVisit(RollbackTransactionStmtAST *) {}
    virtual bool visit(PrepareTransactionStmtAST *) { return true; }
    virtual void endVisit(PrepareTransactionStmtAST *) {}
    virtual bool visit(CommitPrepTransactionStmtAST *) { return true; }
    virtual void endVisit(CommitPrepTransactionStmtAST *) {}
    virtual bool visit(RollPrepTransactionStmtAST *) { return true; }
    virtual void endVisit(RollPrepTransactionStmtAST *) {}
    virtual bool visit(SecLabelStmtAST *) { return true; }
    virtual void endVisit(SecLabelStmtAST *) {}
    virtual bool visit(ColSecLabelStmtAST *) { return true; }
    virtual void endVisit(ColSecLabelStmtAST *) {}
    virtual bool visit(SequenceSecLabelStmtAST *) { return true; }
    virtual void endVisit(SequenceSecLabelStmtAST *) {}
    virtual bool visit(TableSecLabelStmtAST *) { return true; }
    virtual void endVisit(TableSecLabelStmtAST *) {}
    virtual bool visit(AggrSecLabelStmtAST *) { return true; }
    virtual void endVisit(AggrSecLabelStmtAST *) {}
    virtual bool visit(FuncSecLabelStmtAST *) { return true; }
    virtual void endVisit(FuncSecLabelStmtAST *) {}
    virtual bool visit(LargeObjSecLabelStmtAST *) { return true; }
    virtual void endVisit(LargeObjSecLabelStmtAST *) {}
    virtual bool visit(LangSecLabelStmtAST *) { return true; }
    virtual void endVisit(LangSecLabelStmtAST *) {}
    virtual bool visit(SchemaSecLabelStmtAST *) { return true; }
    virtual void endVisit(SchemaSecLabelStmtAST *) {}
    virtual bool visit(RoleSecLabelStmtAST *) { return true; }
    virtual void endVisit(RoleSecLabelStmtAST *) {}
    virtual bool visit(CommonSecLabelStmtAST *) { return true; }
    virtual void endVisit(CommonSecLabelStmtAST *) {}
    virtual bool visit(SecurityProviderAST *) { return true; }
    virtual void endVisit(SecurityProviderAST *) {}
    virtual bool visit(CommonSecurityTypeAST *) { return true; }
    virtual void endVisit(CommonSecurityTypeAST *) {}
    virtual bool visit(RenameStmtAST *) { return true; }
    virtual void endVisit(RenameStmtAST *) {}
    virtual bool visit(RenameTypeStmtAST *) { return true; }
    virtual void endVisit(RenameTypeStmtAST *) {}
    virtual bool visit(RenameTypeAttrStmtAST *) { return true; }
    virtual void endVisit(RenameTypeAttrStmtAST *) {}
    virtual bool visit(RenameRuleStmtAST *) { return true; }
    virtual void endVisit(RenameRuleStmtAST *) {}
    virtual bool visit(RenameTriggerStmtAST *) { return true; }
    virtual void endVisit(RenameTriggerStmtAST *) {}
    virtual bool visit(RenameSchemaStmtAST *) { return true; }
    virtual void endVisit(RenameSchemaStmtAST *) {}
    virtual bool visit(RenameServerStmtAST *) { return true; }
    virtual void endVisit(RenameServerStmtAST *) {}
    virtual bool visit(RenameEvtTrgStmtAST *) { return true; }
    virtual void endVisit(RenameEvtTrgStmtAST *) {}
    virtual bool visit(RenameRoleStmtAST *) { return true; }
    virtual void endVisit(RenameRoleStmtAST *) {}
    virtual bool visit(RenameTablespaceStmtAST *) { return true; }
    virtual void endVisit(RenameTablespaceStmtAST *) {}
    virtual bool visit(ChgReloptsTablespaceStmtAST *) { return true; }
    virtual void endVisit(ChgReloptsTablespaceStmtAST *) {}
    virtual bool visit(RenameSearchStmtAST *) { return true; }
    virtual void endVisit(RenameSearchStmtAST *) {}
    virtual bool visit(RenameTableStmtAST *) { return true; }
    virtual void endVisit(RenameTableStmtAST *) {}
    virtual bool visit(RenameTableColStmtAST *) { return true; }
    virtual void endVisit(RenameTableColStmtAST *) {}
    virtual bool visit(RenameTableConstrStmtAST *) { return true; }
    virtual void endVisit(RenameTableConstrStmtAST *) {}
    virtual bool visit(RenameMatViewColStmtAST *) { return true; }
    virtual void endVisit(RenameMatViewColStmtAST *) {}
    virtual bool visit(RenameSequenceStmtAST *) { return true; }
    virtual void endVisit(RenameSequenceStmtAST *) {}
    virtual bool visit(RenameViewStmtAST *) { return true; }
    virtual void endVisit(RenameViewStmtAST *) {}
    virtual bool visit(RenameIndexStmtAST *) { return true; }
    virtual void endVisit(RenameIndexStmtAST *) {}
    virtual bool visit(RenameCollationStmtAST *) { return true; }
    virtual void endVisit(RenameCollationStmtAST *) {}
    virtual bool visit(RenameConversionStmtAST *) { return true; }
    virtual void endVisit(RenameConversionStmtAST *) {}
    virtual bool visit(RenameDomainStmtAST *) { return true; }
    virtual void endVisit(RenameDomainStmtAST *) {}
    virtual bool visit(RenameDomainConstrStmtAST *) { return true; }
    virtual void endVisit(RenameDomainConstrStmtAST *) {}
    virtual bool visit(RenameDatabaseStmtAST *) { return true; }
    virtual void endVisit(RenameDatabaseStmtAST *) {}
    virtual bool visit(RenameFdwStmtAST *) { return true; }
    virtual void endVisit(RenameFdwStmtAST *) {}
    virtual bool visit(RenameFuncStmtAST *) { return true; }
    virtual void endVisit(RenameFuncStmtAST *) {}
    virtual bool visit(RenameOpUsingStmtAST *) { return true; }
    virtual void endVisit(RenameOpUsingStmtAST *) {}
    virtual bool visit(RenameGroupRoleStmtAST *) { return true; }
    virtual void endVisit(RenameGroupRoleStmtAST *) {}
    virtual bool visit(RenameLangStmtAST *) { return true; }
    virtual void endVisit(RenameLangStmtAST *) {}
    virtual bool visit(RenameAggrStmtAST *) { return true; }
    virtual void endVisit(RenameAggrStmtAST *) {}
    virtual bool visit(ReindexStmtAST *) { return true; }
    virtual void endVisit(ReindexStmtAST *) {}
    virtual bool visit(ReindexTableStmtAST *) { return true; }
    virtual void endVisit(ReindexTableStmtAST *) {}
    virtual bool visit(ReindexIndexStmtAST *) { return true; }
    virtual void endVisit(ReindexIndexStmtAST *) {}
    virtual bool visit(ReindexSystemStmtAST *) { return true; }
    virtual void endVisit(ReindexSystemStmtAST *) {}
    virtual bool visit(ReindexDatabaseStmtAST *) { return true; }
    virtual void endVisit(ReindexDatabaseStmtAST *) {}
    virtual bool visit(PrepareStmtAST *) { return true; }
    virtual void endVisit(PrepareStmtAST *) {}
    virtual bool visit(PrepareTypeClauseAST *) { return true; }
    virtual void endVisit(PrepareTypeClauseAST *) {}
    virtual bool visit(LockStmtAST *) { return true; }
    virtual void endVisit(LockStmtAST *) {}
    virtual bool visit(LockModeClauseAST *) { return true; }
    virtual void endVisit(LockModeClauseAST *) {}
    virtual bool visit(LockTypeAST *) { return true; }
    virtual void endVisit(LockTypeAST *) {}
    virtual bool visit(DropStmtAST *) { return true; }
    virtual void endVisit(DropStmtAST *) {}
    virtual bool visit(DropAggrStmtAST *) { return true; }
    virtual void endVisit(DropAggrStmtAST *) {}
    virtual bool visit(DropFuncStmtAST *) { return true; }
    virtual void endVisit(DropFuncStmtAST *) {}
    virtual bool visit(DropOperStmtAST *) { return true; }
    virtual void endVisit(DropOperStmtAST *) {}
    virtual bool visit(LoadStmtAST *) { return true; }
    virtual void endVisit(LoadStmtAST *) {}
    virtual bool visit(NotifyStmtAST *) { return true; }
    virtual void endVisit(NotifyStmtAST *) {}
    virtual bool visit(NotifyPayloadAST *) { return true; }
    virtual void endVisit(NotifyPayloadAST *) {}
    virtual bool visit(ListenStmtAST *) { return true; }
    virtual void endVisit(ListenStmtAST *) {}
    virtual bool visit(UnlistenStmtAST *) { return true; }
    virtual void endVisit(UnlistenStmtAST *) {}
    virtual bool visit(CreateIndexStatementAST *) { return true; }
    virtual void endVisit(CreateIndexStatementAST *) {}
    virtual bool visit(GrantRoleStmtAST *) { return true; }
    virtual void endVisit(GrantRoleStmtAST *) {}
    virtual bool visit(RevokeRoleStmtAST *) { return true; }
    virtual void endVisit(RevokeRoleStmtAST *) {}
    virtual bool visit(WithAdminOptionAST *) { return true; }
    virtual void endVisit(WithAdminOptionAST *) {}
    virtual bool visit(GrantedByClauseAST *) { return true; }
    virtual void endVisit(GrantedByClauseAST *) {}
    virtual bool visit(GrantStmtAST *) { return true; }
    virtual void endVisit(GrantStmtAST *) {}
    virtual bool visit(RevokeStmtAST *) { return true; }
    virtual void endVisit(RevokeStmtAST *) {}
    virtual bool visit(PrivilegeTargetAST *) { return true; }
    virtual void endVisit(PrivilegeTargetAST *) {}
    virtual bool visit(TablePrivilegeTargetAST *) { return true; }
    virtual void endVisit(TablePrivilegeTargetAST *) {}
    virtual bool visit(SequencePrivilegeTargetAST *) { return true; }
    virtual void endVisit(SequencePrivilegeTargetAST *) {}
    virtual bool visit(FdwPrivilegeTargetAST *) { return true; }
    virtual void endVisit(FdwPrivilegeTargetAST *) {}
    virtual bool visit(ForeignSrvPrivilegeTargetAST *) { return true; }
    virtual void endVisit(ForeignSrvPrivilegeTargetAST *) {}
    virtual bool visit(FuncPrivilegeTargetAST *) { return true; }
    virtual void endVisit(FuncPrivilegeTargetAST *) {}
    virtual bool visit(DbPrivilegeTargetAST *) { return true; }
    virtual void endVisit(DbPrivilegeTargetAST *) {}
    virtual bool visit(DomainPrivilegeTargetAST *) { return true; }
    virtual void endVisit(DomainPrivilegeTargetAST *) {}
    virtual bool visit(LangPrivilegeTargetAST *) { return true; }
    virtual void endVisit(LangPrivilegeTargetAST *) {}
    virtual bool visit(LargeObjPrivilegeTargetAST *) { return true; }
    virtual void endVisit(LargeObjPrivilegeTargetAST *) {}
    virtual bool visit(SchemaPrivilegeTargetAST *) { return true; }
    virtual void endVisit(SchemaPrivilegeTargetAST *) {}
    virtual bool visit(TblspacePrivilegeTargetAST *) { return true; }
    virtual void endVisit(TblspacePrivilegeTargetAST *) {}
    virtual bool visit(TypePrivilegeTargetAST *) { return true; }
    virtual void endVisit(TypePrivilegeTargetAST *) {}
    virtual bool visit(AllInSchPrivilegeTargetAST *) { return true; }
    virtual void endVisit(AllInSchPrivilegeTargetAST *) {}
    virtual bool visit(NumericAST *) { return true; }
    virtual void endVisit(NumericAST *) {}
    virtual bool visit(FetchStmtAST *) { return true; }
    virtual void endVisit(FetchStmtAST *) {}
    virtual bool visit(FetchClauseAST *) { return true; }
    virtual void endVisit(FetchClauseAST *) {}
    virtual bool visit(BaseExplainStmtAST *) { return true; }
    virtual void endVisit(BaseExplainStmtAST *) {}
    virtual bool visit(CommonExplainStmtAST *) { return true; }
    virtual void endVisit(CommonExplainStmtAST *) {}
    virtual bool visit(OptExplainStmtAST *) { return true; }
    virtual void endVisit(OptExplainStmtAST *) {}
    virtual bool visit(ExplainOptionAST *) { return true; }
    virtual void endVisit(ExplainOptionAST *) {}
    virtual bool visit(RefreshMatViewStmtAST *) { return true; }
    virtual void endVisit(RefreshMatViewStmtAST *) {}
    virtual bool visit(DropDbStmtAST *) { return true; }
    virtual void endVisit(DropDbStmtAST *) {}
    virtual bool visit(DropUserMappingStmtAST *) { return true; }
    virtual void endVisit(DropUserMappingStmtAST *) {}
    virtual bool visit(DropTableSpaceStmtAST *) { return true; }
    virtual void endVisit(DropTableSpaceStmtAST *) {}
    virtual bool visit(CommonDropStmtAST *) { return true; }
    virtual void endVisit(CommonDropStmtAST *) {}
    virtual bool visit(CommonDropTypeAST *) { return true; }
    virtual void endVisit(CommonDropTypeAST *) {}
    virtual bool visit(DropTableStmtAST *) { return true; }
    virtual void endVisit(DropTableStmtAST *) {}
    virtual bool visit(DropViewStmtAST *) { return true; }
    virtual void endVisit(DropViewStmtAST *) {}
    virtual bool visit(DropIndexStmtAST *) { return true; }
    virtual void endVisit(DropIndexStmtAST *) {}
    virtual bool visit(DropSchemaStmtAST *) { return true; }
    virtual void endVisit(DropSchemaStmtAST *) {}
    virtual bool visit(DropSequenceStmtAST *) { return true; }
    virtual void endVisit(DropSequenceStmtAST *) {}
    virtual bool visit(RuleStmtAST *) { return true; }
    virtual void endVisit(RuleStmtAST *) {}
    virtual bool visit(RuleActionsAST *) { return true; }
    virtual void endVisit(RuleActionsAST *) {}
    virtual bool visit(NothingRuleActionsAST *) { return true; }
    virtual void endVisit(NothingRuleActionsAST *) {}
    virtual bool visit(SingleRuleActionsAST *) { return true; }
    virtual void endVisit(SingleRuleActionsAST *) {}
    virtual bool visit(MultiRuleActionsAST *) { return true; }
    virtual void endVisit(MultiRuleActionsAST *) {}
    virtual bool visit(DropRuleStmtAST *) { return true; }
    virtual void endVisit(DropRuleStmtAST *) {}
    virtual bool visit(DropOwnedStmtAST *) { return true; }
    virtual void endVisit(DropOwnedStmtAST *) {}
    virtual bool visit(ReassignOwnedStmtAST *) { return true; }
    virtual void endVisit(ReassignOwnedStmtAST *) {}
    virtual bool visit(DropGroupStmtAST *) { return true; }
    virtual void endVisit(DropGroupStmtAST *) {}
    virtual bool visit(DropForeignServerStmtAST *) { return true; }
    virtual void endVisit(DropForeignServerStmtAST *) {}
    virtual bool visit(DropFdwStmtAST *) { return true; }
    virtual void endVisit(DropFdwStmtAST *) {}
    virtual bool visit(DoStmtAST *) { return true; }
    virtual void endVisit(DoStmtAST *) {}
    virtual bool visit(DoStmtItemAST *) { return true; }
    virtual void endVisit(DoStmtItemAST *) {}
    virtual bool visit(DoStmtCmdItemAST *) { return true; }
    virtual void endVisit(DoStmtCmdItemAST *) {}
    virtual bool visit(DoStmtLangItemAST *) { return true; }
    virtual void endVisit(DoStmtLangItemAST *) {}
    virtual bool visit(CreateOldAggregateStmtAST *) { return true; }
    virtual void endVisit(CreateOldAggregateStmtAST *) {}
    virtual bool visit(AggregateDefinitionAST *) { return true; }
    virtual void endVisit(AggregateDefinitionAST *) {}
    virtual bool visit(AggrDefinitionAST *) { return true; }
    virtual void endVisit(AggrDefinitionAST *) {}
    virtual bool visit(CreateAggregateStmtAST *) { return true; }
    virtual void endVisit(CreateAggregateStmtAST *) {}
    virtual bool visit(CreateOpStmtAST *) { return true; }
    virtual void endVisit(CreateOpStmtAST *) {}
    virtual bool visit(CreateTypeStmtAST *) { return true; }
    virtual void endVisit(CreateTypeStmtAST *) {}
    virtual bool visit(CreateTypeAsRecordStmtAST *) { return true; }
    virtual void endVisit(CreateTypeAsRecordStmtAST *) {}
    virtual bool visit(CreateTypeAsEnumStmtAST *) { return true; }
    virtual void endVisit(CreateTypeAsEnumStmtAST *) {}
    virtual bool visit(EnumValueAST *) { return true; }
    virtual void endVisit(EnumValueAST *) {}
    virtual bool visit(CreateTypeAsDefStmtAST *) { return true; }
    virtual void endVisit(CreateTypeAsDefStmtAST *) {}
    virtual bool visit(CreateTypeAsRangeStmtAST *) { return true; }
    virtual void endVisit(CreateTypeAsRangeStmtAST *) {}
    virtual bool visit(CreateSearchObjStmtAST *) { return true; }
    virtual void endVisit(CreateSearchObjStmtAST *) {}
    virtual bool visit(CreateCollationStmtAST *) { return true; }
    virtual void endVisit(CreateCollationStmtAST *) {}
    virtual bool visit(CreateCollationDefStmtAST *) { return true; }
    virtual void endVisit(CreateCollationDefStmtAST *) {}
    virtual bool visit(CreateCollationFromStmtAST *) { return true; }
    virtual void endVisit(CreateCollationFromStmtAST *) {}
    virtual bool visit(DeclareCursorStmtAST *) { return true; }
    virtual void endVisit(DeclareCursorStmtAST *) {}
    virtual bool visit(CursorHoldOptionAST *) { return true; }
    virtual void endVisit(CursorHoldOptionAST *) {}
    virtual bool visit(DeallocateStmtAST *) { return true; }
    virtual void endVisit(DeallocateStmtAST *) {}
    virtual bool visit(CreateDbStmtAST *) { return true; }
    virtual void endVisit(CreateDbStmtAST *) {}
    virtual bool visit(CreateDbOptionAST *) { return true; }
    virtual void endVisit(CreateDbOptionAST *) {}
    virtual bool visit(NamedCreateDbOptionAST *) { return true; }
    virtual void endVisit(NamedCreateDbOptionAST *) {}
    virtual bool visit(ValDefCreateDbOptionAST *) { return true; }
    virtual void endVisit(ValDefCreateDbOptionAST *) {}
    virtual bool visit(CreateUserMappingStmtAST *) { return true; }
    virtual void endVisit(CreateUserMappingStmtAST *) {}
    virtual bool visit(DropTrigStmtAST *) { return true; }
    virtual void endVisit(DropTrigStmtAST *) {}
    virtual bool visit(CreateConstrTrigStmtAST *) { return true; }
    virtual void endVisit(CreateConstrTrigStmtAST *) {}
    virtual bool visit(ConstrFromTableAST *) { return true; }
    virtual void endVisit(ConstrFromTableAST *) {}
    virtual bool visit(CreateTrigStmtAST *) { return true; }
    virtual void endVisit(CreateTrigStmtAST *) {}
    virtual bool visit(TriggerExecuteClauseAST *) { return true; }
    virtual void endVisit(TriggerExecuteClauseAST *) {}
    virtual bool visit(TriggerFuncArgAST *) { return true; }
    virtual void endVisit(TriggerFuncArgAST *) {}
    virtual bool visit(TriggerWhenAST *) { return true; }
    virtual void endVisit(TriggerWhenAST *) {}
    virtual bool visit(TriggerForSpecAST *) { return true; }
    virtual void endVisit(TriggerForSpecAST *) {}
    virtual bool visit(TriggerEventAST *) { return true; }
    virtual void endVisit(TriggerEventAST *) {}
    virtual bool visit(TriggerActionTimeAST *) { return true; }
    virtual void endVisit(TriggerActionTimeAST *) {}
    virtual bool visit(CreateTableSpaceStmtAST *) { return true; }
    virtual void endVisit(CreateTableSpaceStmtAST *) {}
    virtual bool visit(TableSpaceOwnerClauseAST *) { return true; }
    virtual void endVisit(TableSpaceOwnerClauseAST *) {}
    virtual bool visit(CreateTableAsExecStmtAST *) { return true; }
    virtual void endVisit(CreateTableAsExecStmtAST *) {}
    virtual bool visit(ExecuteStmtAST *) { return true; }
    virtual void endVisit(ExecuteStmtAST *) {}
    virtual bool visit(ExecuteParamClauseAST *) { return true; }
    virtual void endVisit(ExecuteParamClauseAST *) {}
    virtual bool visit(CreateTableStmtAST *) { return true; }
    virtual void endVisit(CreateTableStmtAST *) {}
    virtual bool visit(CreateTableOptionsAST *) { return true; }
    virtual void endVisit(CreateTableOptionsAST *) {}
    virtual bool visit(CreateTableClauseAST *) { return true; }
    virtual void endVisit(CreateTableClauseAST *) {}
    virtual bool visit(CreateTableDirecClauseAST *) { return true; }
    virtual void endVisit(CreateTableDirecClauseAST *) {}
    virtual bool visit(CreateTableOfClauseAST *) { return true; }
    virtual void endVisit(CreateTableOfClauseAST *) {}
    virtual bool visit(TypedTableElementsAST *) { return true; }
    virtual void endVisit(TypedTableElementsAST *) {}
    virtual bool visit(TypedTableElementAST *) { return true; }
    virtual void endVisit(TypedTableElementAST *) {}
    virtual bool visit(TypedTableConstrElementAST *) { return true; }
    virtual void endVisit(TypedTableConstrElementAST *) {}
    virtual bool visit(TypedTableColElementAST *) { return true; }
    virtual void endVisit(TypedTableColElementAST *) {}
    virtual bool visit(TableInheritClauseAST *) { return true; }
    virtual void endVisit(TableInheritClauseAST *) {}
    virtual bool visit(CreateSchemaStmtAST *) { return true; }
    virtual void endVisit(CreateSchemaStmtAST *) {}
    virtual bool visit(CreateSchemaClauseAST *) { return true; }
    virtual void endVisit(CreateSchemaClauseAST *) {}
    virtual bool visit(DropPLangStmtAST *) { return true; }
    virtual void endVisit(DropPLangStmtAST *) {}
    virtual bool visit(CreatePLangStmtAST *) { return true; }
    virtual void endVisit(CreatePLangStmtAST *) {}
    virtual bool visit(InlineHandlerClauseAST *) { return true; }
    virtual void endVisit(InlineHandlerClauseAST *) {}
    virtual bool visit(ValidatorHandlerClauseAST *) { return true; }
    virtual void endVisit(ValidatorHandlerClauseAST *) {}
    virtual bool visit(DropOpClassStmtAST *) { return true; }
    virtual void endVisit(DropOpClassStmtAST *) {}
    virtual bool visit(DropOpFamilyStmtAST *) { return true; }
    virtual void endVisit(DropOpFamilyStmtAST *) {}
    virtual bool visit(AlterOpFamilyAddStmtAST *) { return true; }
    virtual void endVisit(AlterOpFamilyAddStmtAST *) {}
    virtual bool visit(AlterOpFamilyDropStmtAST *) { return true; }
    virtual void endVisit(AlterOpFamilyDropStmtAST *) {}
    virtual bool visit(OpClassDropAST *) { return true; }
    virtual void endVisit(OpClassDropAST *) {}
    virtual bool visit(CreateOpFamilyStmtAST *) { return true; }
    virtual void endVisit(CreateOpFamilyStmtAST *) {}
    virtual bool visit(CreateOpClassStmtAST *) { return true; }
    virtual void endVisit(CreateOpClassStmtAST *) {}
    virtual bool visit(OpClassAST *) { return true; }
    virtual void endVisit(OpClassAST *) {}
    virtual bool visit(StorageOpClassAST *) { return true; }
    virtual void endVisit(StorageOpClassAST *) {}
    virtual bool visit(OperatorOpClassAST *) { return true; }
    virtual void endVisit(OperatorOpClassAST *) {}
    virtual bool visit(OpClassPurposeAST *) { return true; }
    virtual void endVisit(OpClassPurposeAST *) {}
    virtual bool visit(OpClassSearchPurposeAST *) { return true; }
    virtual void endVisit(OpClassSearchPurposeAST *) {}
    virtual bool visit(OpClassOrderPurposeAST *) { return true; }
    virtual void endVisit(OpClassOrderPurposeAST *) {}
    virtual bool visit(FunctionOpClassAST *) { return true; }
    virtual void endVisit(FunctionOpClassAST *) {}
    virtual bool visit(OpFamilyAST *) { return true; }
    virtual void endVisit(OpFamilyAST *) {}
    virtual bool visit(CreateMatViewStmtAST *) { return true; }
    virtual void endVisit(CreateMatViewStmtAST *) {}
    virtual bool visit(CreateMvTargetAST *) { return true; }
    virtual void endVisit(CreateMvTargetAST *) {}
    virtual bool visit(WithReloptionsAST *) { return true; }
    virtual void endVisit(WithReloptionsAST *) {}
    virtual bool visit(CreateFunctionStmtAST *) { return true; }
    virtual void endVisit(CreateFunctionStmtAST *) {}
    virtual bool visit(BaseFuncReturnAST *) { return true; }
    virtual void endVisit(BaseFuncReturnAST *) {}
    virtual bool visit(FuncTypeFuncReturnAST *) { return true; }
    virtual void endVisit(FuncTypeFuncReturnAST *) {}
    virtual bool visit(TableFuncReturnAST *) { return true; }
    virtual void endVisit(TableFuncReturnAST *) {}
    virtual bool visit(TableFuncColumnAST *) { return true; }
    virtual void endVisit(TableFuncColumnAST *) {}
    virtual bool visit(CreateFuncOptionAST *) { return true; }
    virtual void endVisit(CreateFuncOptionAST *) {}
    virtual bool visit(CreateFuncAsOptionAST *) { return true; }
    virtual void endVisit(CreateFuncAsOptionAST *) {}
    virtual bool visit(CreateFuncLangOptionAST *) { return true; }
    virtual void endVisit(CreateFuncLangOptionAST *) {}
    virtual bool visit(CreateFuncWindowOptionAST *) { return true; }
    virtual void endVisit(CreateFuncWindowOptionAST *) {}
    virtual bool visit(CreateFuncCommonOptionAST *) { return true; }
    virtual void endVisit(CreateFuncCommonOptionAST *) {}
    virtual bool visit(FunctionAsClauseAST *) { return true; }
    virtual void endVisit(FunctionAsClauseAST *) {}
    virtual bool visit(FuncArgsWithDefaultsAST *) { return true; }
    virtual void endVisit(FuncArgsWithDefaultsAST *) {}
    virtual bool visit(FuncArgWithDefaultAST *) { return true; }
    virtual void endVisit(FuncArgWithDefaultAST *) {}
    virtual bool visit(OrReplaceClauseAST *) { return true; }
    virtual void endVisit(OrReplaceClauseAST *) {}
    virtual bool visit(CreateForeignTableStmtAST *) { return true; }
    virtual void endVisit(CreateForeignTableStmtAST *) {}
    virtual bool visit(TableElementAST *) { return true; }
    virtual void endVisit(TableElementAST *) {}
    virtual bool visit(ColumnDefTableElementAST *) { return true; }
    virtual void endVisit(ColumnDefTableElementAST *) {}
    virtual bool visit(LikeTableElementAST *) { return true; }
    virtual void endVisit(LikeTableElementAST *) {}
    virtual bool visit(TableLikeClauseAST *) { return true; }
    virtual void endVisit(TableLikeClauseAST *) {}
    virtual bool visit(TableLikeOptionAST *) { return true; }
    virtual void endVisit(TableLikeOptionAST *) {}
    virtual bool visit(ConstraintTableElementAST *) { return true; }
    virtual void endVisit(ConstraintTableElementAST *) {}
    virtual bool visit(CreateForeignServerStmtAST *) { return true; }
    virtual void endVisit(CreateForeignServerStmtAST *) {}
    virtual bool visit(ServerTypeClauseAST *) { return true; }
    virtual void endVisit(ServerTypeClauseAST *) {}
    virtual bool visit(CreateFdwStmtAST *) { return true; }
    virtual void endVisit(CreateFdwStmtAST *) {}
    virtual bool visit(CreateExtensionStmtAST *) { return true; }
    virtual void endVisit(CreateExtensionStmtAST *) {}
    virtual bool visit(CreateExtOptionAST *) { return true; }
    virtual void endVisit(CreateExtOptionAST *) {}
    virtual bool visit(CreateExtSchemaOptionAST *) { return true; }
    virtual void endVisit(CreateExtSchemaOptionAST *) {}
    virtual bool visit(CreateExtVerFromOptionAST *) { return true; }
    virtual void endVisit(CreateExtVerFromOptionAST *) {}
    virtual bool visit(CreateConversionStmtAST *) { return true; }
    virtual void endVisit(CreateConversionStmtAST *) {}
    virtual bool visit(CreateCastStmtAST *) { return true; }
    virtual void endVisit(CreateCastStmtAST *) {}
    virtual bool visit(CastContextAST *) { return true; }
    virtual void endVisit(CastContextAST *) {}
    virtual bool visit(DropCastStmtAST *) { return true; }
    virtual void endVisit(DropCastStmtAST *) {}
    virtual bool visit(IfExistsAST *) { return true; }
    virtual void endVisit(IfExistsAST *) {}
    virtual bool visit(DropAssertStmtAST *) { return true; }
    virtual void endVisit(DropAssertStmtAST *) {}
    virtual bool visit(CreateAssertStmtAST *) { return true; }
    virtual void endVisit(CreateAssertStmtAST *) {}
    virtual bool visit(CreateTableAsStmtAST *) { return true; }
    virtual void endVisit(CreateTableAsStmtAST *) {}
    virtual bool visit(CreateAsTargetAST *) { return true; }
    virtual void endVisit(CreateAsTargetAST *) {}
    virtual bool visit(TableSpaceClauseAST *) { return true; }
    virtual void endVisit(TableSpaceClauseAST *) {}
    virtual bool visit(OnCommitOptionAST *) { return true; }
    virtual void endVisit(OnCommitOptionAST *) {}
    virtual bool visit(CreateWithOptionsAST *) { return true; }
    virtual void endVisit(CreateWithOptionsAST *) {}
    virtual bool visit(CreateWithRelOptionsAST *) { return true; }
    virtual void endVisit(CreateWithRelOptionsAST *) {}
    virtual bool visit(CreateWithOidsOptionsAST *) { return true; }
    virtual void endVisit(CreateWithOidsOptionsAST *) {}
    virtual bool visit(WithDataClauseAST *) { return true; }
    virtual void endVisit(WithDataClauseAST *) {}
    virtual bool visit(CopyStmtAST *) { return true; }
    virtual void endVisit(CopyStmtAST *) {}
    virtual bool visit(TableCopyStmtAST *) { return true; }
    virtual void endVisit(TableCopyStmtAST *) {}
    virtual bool visit(WithOidsClauseAST *) { return true; }
    virtual void endVisit(WithOidsClauseAST *) {}
    virtual bool visit(CopyDelimiterAST *) { return true; }
    virtual void endVisit(CopyDelimiterAST *) {}
    virtual bool visit(SelectCopyStmtAST *) { return true; }
    virtual void endVisit(SelectCopyStmtAST *) {}
    virtual bool visit(CopyOptionsAST *) { return true; }
    virtual void endVisit(CopyOptionsAST *) {}
    virtual bool visit(NewCopyOptionsAST *) { return true; }
    virtual void endVisit(NewCopyOptionsAST *) {}
    virtual bool visit(GenericCopyOptionAST *) { return true; }
    virtual void endVisit(GenericCopyOptionAST *) {}
    virtual bool visit(GenericCopyOptionArgAST *) { return true; }
    virtual void endVisit(GenericCopyOptionArgAST *) {}
    virtual bool visit(GenericCopyOptionValArgAST *) { return true; }
    virtual void endVisit(GenericCopyOptionValArgAST *) {}
    virtual bool visit(GenericCopyOptionCmpArgAST *) { return true; }
    virtual void endVisit(GenericCopyOptionCmpArgAST *) {}
    virtual bool visit(GenericCopyOptionCmpArgItemAST *) { return true; }
    virtual void endVisit(GenericCopyOptionCmpArgItemAST *) {}
    virtual bool visit(OldCopyOptionsAST *) { return true; }
    virtual void endVisit(OldCopyOptionsAST *) {}
    virtual bool visit(CopyOptionAST *) { return true; }
    virtual void endVisit(CopyOptionAST *) {}
    virtual bool visit(ConstraintsSetStmtAST *) { return true; }
    virtual void endVisit(ConstraintsSetStmtAST *) {}
    virtual bool visit(ConstraintsSetListAST *) { return true; }
    virtual void endVisit(ConstraintsSetListAST *) {}
    virtual bool visit(AllConstraintsSetListAST *) { return true; }
    virtual void endVisit(AllConstraintsSetListAST *) {}
    virtual bool visit(CommonConstraintsSetListAST *) { return true; }
    virtual void endVisit(CommonConstraintsSetListAST *) {}
    virtual bool visit(CommentStmtAST *) { return true; }
    virtual void endVisit(CommentStmtAST *) {}
    virtual bool visit(LangCommentStmtAST *) { return true; }
    virtual void endVisit(LangCommentStmtAST *) {}
    virtual bool visit(CastCommentStmtAST *) { return true; }
    virtual void endVisit(CastCommentStmtAST *) {}
    virtual bool visit(LargeObjCommentStmtAST *) { return true; }
    virtual void endVisit(LargeObjCommentStmtAST *) {}
    virtual bool visit(OpUsingCommentStmtAST *) { return true; }
    virtual void endVisit(OpUsingCommentStmtAST *) {}
    virtual bool visit(ConstraintCommentStmtAST *) { return true; }
    virtual void endVisit(ConstraintCommentStmtAST *) {}
    virtual bool visit(RuleCommentStmtAST *) { return true; }
    virtual void endVisit(RuleCommentStmtAST *) {}
    virtual bool visit(TriggerCommentStmtAST *) { return true; }
    virtual void endVisit(TriggerCommentStmtAST *) {}
    virtual bool visit(CommentOnNameAST *) { return true; }
    virtual void endVisit(CommentOnNameAST *) {}
    virtual bool visit(OpCommentStmtAST *) { return true; }
    virtual void endVisit(OpCommentStmtAST *) {}
    virtual bool visit(FunctionCommentStmtAST *) { return true; }
    virtual void endVisit(FunctionCommentStmtAST *) {}
    virtual bool visit(AggrCommentStmtAST *) { return true; }
    virtual void endVisit(AggrCommentStmtAST *) {}
    virtual bool visit(CommonCommentStmtAST *) { return true; }
    virtual void endVisit(CommonCommentStmtAST *) {}
    virtual bool visit(CommentObjectAST *) { return true; }
    virtual void endVisit(CommentObjectAST *) {}
    virtual bool visit(SearchCommentObjectAST *) { return true; }
    virtual void endVisit(SearchCommentObjectAST *) {}
    virtual bool visit(EvtTriggerCommentObjectAST *) { return true; }
    virtual void endVisit(EvtTriggerCommentObjectAST *) {}
    virtual bool visit(FdwCommentObjectAST *) { return true; }
    virtual void endVisit(FdwCommentObjectAST *) {}
    virtual bool visit(RoleCommentObjectAST *) { return true; }
    virtual void endVisit(RoleCommentObjectAST *) {}
    virtual bool visit(TableCommentObjectAST *) { return true; }
    virtual void endVisit(TableCommentObjectAST *) {}
    virtual bool visit(SequenceCommentObjectAST *) { return true; }
    virtual void endVisit(SequenceCommentObjectAST *) {}
    virtual bool visit(IndexCommentObjectAST *) { return true; }
    virtual void endVisit(IndexCommentObjectAST *) {}
    virtual bool visit(SchemaCommentObjectAST *) { return true; }
    virtual void endVisit(SchemaCommentObjectAST *) {}
    virtual bool visit(ColumnCommentObjectAST *) { return true; }
    virtual void endVisit(ColumnCommentObjectAST *) {}
    virtual bool visit(NamedCommentObjectAST *) { return true; }
    virtual void endVisit(NamedCommentObjectAST *) {}
    virtual bool visit(AnyCommentObjectAST *) { return true; }
    virtual void endVisit(AnyCommentObjectAST *) {}
    virtual bool visit(CommentTextClauseAST *) { return true; }
    virtual void endVisit(CommentTextClauseAST *) {}
    virtual bool visit(ClusterStmtAST *) { return true; }
    virtual void endVisit(ClusterStmtAST *) {}
    virtual bool visit(ClusterOnStmtAST *) { return true; }
    virtual void endVisit(ClusterOnStmtAST *) {}
    virtual bool visit(ClusterUsingStmtAST *) { return true; }
    virtual void endVisit(ClusterUsingStmtAST *) {}
    virtual bool visit(UsingIndexClauseAST *) { return true; }
    virtual void endVisit(UsingIndexClauseAST *) {}
    virtual bool visit(ClosePortalStmtAST *) { return true; }
    virtual void endVisit(ClosePortalStmtAST *) {}
    virtual bool visit(CloseCursorStmtAST *) { return true; }
    virtual void endVisit(CloseCursorStmtAST *) {}
    virtual bool visit(CloseAllStmtAST *) { return true; }
    virtual void endVisit(CloseAllStmtAST *) {}
    virtual bool visit(CheckPointStmtAST *) { return true; }
    virtual void endVisit(CheckPointStmtAST *) {}
    virtual bool visit(AnalyzeStmtAST *) { return true; }
    virtual void endVisit(AnalyzeStmtAST *) {}
    virtual bool visit(AlterUserMappingStmtAST *) { return true; }
    virtual void endVisit(AlterUserMappingStmtAST *) {}
    virtual bool visit(AuthIdentifierAST *) { return true; }
    virtual void endVisit(AuthIdentifierAST *) {}
    virtual bool visit(UserAuthIdentifierAST *) { return true; }
    virtual void endVisit(UserAuthIdentifierAST *) {}
    virtual bool visit(RoleAuthIdentifierAST *) { return true; }
    virtual void endVisit(RoleAuthIdentifierAST *) {}
    virtual bool visit(AlterTSConfigurationStmtAST *) { return true; }
    virtual void endVisit(AlterTSConfigurationStmtAST *) {}
    virtual bool visit(AlterTSConfCmdAST *) { return true; }
    virtual void endVisit(AlterTSConfCmdAST *) {}
    virtual bool visit(AlterTSConfAddAlterCmdAST *) { return true; }
    virtual void endVisit(AlterTSConfAddAlterCmdAST *) {}
    virtual bool visit(AlterTSConfDropCmdAST *) { return true; }
    virtual void endVisit(AlterTSConfDropCmdAST *) {}
    virtual bool visit(AlterTSDictionaryStmtAST *) { return true; }
    virtual void endVisit(AlterTSDictionaryStmtAST *) {}
    virtual bool visit(AlterCompositeTypeStmtAST *) { return true; }
    virtual void endVisit(AlterCompositeTypeStmtAST *) {}
    virtual bool visit(AlterTypeCommandAST *) { return true; }
    virtual void endVisit(AlterTypeCommandAST *) {}
    virtual bool visit(AlterTypeAddCommandAST *) { return true; }
    virtual void endVisit(AlterTypeAddCommandAST *) {}
    virtual bool visit(AlterTypeDropCommandAST *) { return true; }
    virtual void endVisit(AlterTypeDropCommandAST *) {}
    virtual bool visit(AlterTypeAlterCommandAST *) { return true; }
    virtual void endVisit(AlterTypeAlterCommandAST *) {}
    virtual bool visit(AlterSeqStmtAST *) { return true; }
    virtual void endVisit(AlterSeqStmtAST *) {}
    virtual bool visit(CreateSeqStmtAST *) { return true; }
    virtual void endVisit(CreateSeqStmtAST *) {}
    virtual bool visit(SequenceOptionAST *) { return true; }
    virtual void endVisit(SequenceOptionAST *) {}
    virtual bool visit(SimpleSequenceOptionAST *) { return true; }
    virtual void endVisit(SimpleSequenceOptionAST *) {}
    virtual bool visit(OwnedSequenceOptionAST *) { return true; }
    virtual void endVisit(OwnedSequenceOptionAST *) {}
    virtual bool visit(TempClauseAST *) { return true; }
    virtual void endVisit(TempClauseAST *) {}
    virtual bool visit(AlterOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterOwnerStmtAST *) {}
    virtual bool visit(AlterEvtTriggerOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterEvtTriggerOwnerStmtAST *) {}
    virtual bool visit(AlterServerOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterServerOwnerStmtAST *) {}
    virtual bool visit(AlterFdwOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterFdwOwnerStmtAST *) {}
    virtual bool visit(AlterSearchOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterSearchOwnerStmtAST *) {}
    virtual bool visit(AlterTablespaceOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterTablespaceOwnerStmtAST *) {}
    virtual bool visit(AlterTypeOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterTypeOwnerStmtAST *) {}
    virtual bool visit(AlterSchemaOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterSchemaOwnerStmtAST *) {}
    virtual bool visit(AlterOpUsingOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterOpUsingOwnerStmtAST *) {}
    virtual bool visit(AlterOpOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterOpOwnerStmtAST *) {}
    virtual bool visit(AlterLargeObjOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterLargeObjOwnerStmtAST *) {}
    virtual bool visit(AlterLangOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterLangOwnerStmtAST *) {}
    virtual bool visit(AlterFunctionOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterFunctionOwnerStmtAST *) {}
    virtual bool visit(AlterDbOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterDbOwnerStmtAST *) {}
    virtual bool visit(AlterCollationOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterCollationOwnerStmtAST *) {}
    virtual bool visit(AlterConvOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterConvOwnerStmtAST *) {}
    virtual bool visit(AlterDomainOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterDomainOwnerStmtAST *) {}
    virtual bool visit(AlterAggrOwnerStmtAST *) { return true; }
    virtual void endVisit(AlterAggrOwnerStmtAST *) {}
    virtual bool visit(OwnerToClauseAST *) { return true; }
    virtual void endVisit(OwnerToClauseAST *) {}
    virtual bool visit(AlterObjectSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterObjectSchemaStmtAST *) {}
    virtual bool visit(AlterTypeSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterTypeSchemaStmtAST *) {}
    virtual bool visit(AlterViewSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterViewSchemaStmtAST *) {}
    virtual bool visit(AlterSeqSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterSeqSchemaStmtAST *) {}
    virtual bool visit(AlterSearchSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterSearchSchemaStmtAST *) {}
    virtual bool visit(AlterTableSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterTableSchemaStmtAST *) {}
    virtual bool visit(AlterOpUsingSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterOpUsingSchemaStmtAST *) {}
    virtual bool visit(AlterOpSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterOpSchemaStmtAST *) {}
    virtual bool visit(AlterFunctionSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterFunctionSchemaStmtAST *) {}
    virtual bool visit(AlterCollationSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterCollationSchemaStmtAST *) {}
    virtual bool visit(AlterConvSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterConvSchemaStmtAST *) {}
    virtual bool visit(AlterDomainSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterDomainSchemaStmtAST *) {}
    virtual bool visit(AlterExtSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterExtSchemaStmtAST *) {}
    virtual bool visit(AlterAggrFuncSchemaStmtAST *) { return true; }
    virtual void endVisit(AlterAggrFuncSchemaStmtAST *) {}
    virtual bool visit(SetSchemaClauseAST *) { return true; }
    virtual void endVisit(SetSchemaClauseAST *) {}
    virtual bool visit(AlterFunctionStmtAST *) { return true; }
    virtual void endVisit(AlterFunctionStmtAST *) {}
    virtual bool visit(AlterFunctionOptionAST *) { return true; }
    virtual void endVisit(AlterFunctionOptionAST *) {}
    virtual bool visit(CommonAlterFunctionOptAST *) { return true; }
    virtual void endVisit(CommonAlterFunctionOptAST *) {}
    virtual bool visit(RestAlterFunctionOptAST *) { return true; }
    virtual void endVisit(RestAlterFunctionOptAST *) {}
    virtual bool visit(AlterForeignServerStmtAST *) { return true; }
    virtual void endVisit(AlterForeignServerStmtAST *) {}
    virtual bool visit(ForeignServerVersionAST *) { return true; }
    virtual void endVisit(ForeignServerVersionAST *) {}
    virtual bool visit(AlterFdwStmtAST *) { return true; }
    virtual void endVisit(AlterFdwStmtAST *) {}
    virtual bool visit(FdwOptionAST *) { return true; }
    virtual void endVisit(FdwOptionAST *) {}
    virtual bool visit(SetFdwOptionAST *) { return true; }
    virtual void endVisit(SetFdwOptionAST *) {}
    virtual bool visit(ResetFdwOptionAST *) { return true; }
    virtual void endVisit(ResetFdwOptionAST *) {}
    virtual bool visit(HandlerIdentifierAST *) { return true; }
    virtual void endVisit(HandlerIdentifierAST *) {}
    virtual bool visit(AlterExtensionCtxStmtAST *) { return true; }
    virtual void endVisit(AlterExtensionCtxStmtAST *) {}
    virtual bool visit(ExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(ExtensionCtxCmdAST *) {}
    virtual bool visit(OpExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(OpExtensionCtxCmdAST *) {}
    virtual bool visit(OperArgTypesAST *) { return true; }
    virtual void endVisit(OperArgTypesAST *) {}
    virtual bool visit(LangExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(LangExtensionCtxCmdAST *) {}
    virtual bool visit(FunctionExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(FunctionExtensionCtxCmdAST *) {}
    virtual bool visit(FunctionWithArgTypesAST *) { return true; }
    virtual void endVisit(FunctionWithArgTypesAST *) {}
    virtual bool visit(FunctionArgTypesAST *) { return true; }
    virtual void endVisit(FunctionArgTypesAST *) {}
    virtual bool visit(FunctionArgAST *) { return true; }
    virtual void endVisit(FunctionArgAST *) {}
    virtual bool visit(SimpleExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(SimpleExtensionCtxCmdAST *) {}
    virtual bool visit(CastExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(CastExtensionCtxCmdAST *) {}
    virtual bool visit(AggrExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(AggrExtensionCtxCmdAST *) {}
    virtual bool visit(AggrArgumentsAST *) { return true; }
    virtual void endVisit(AggrArgumentsAST *) {}
    virtual bool visit(ListAggrArgumentsAST *) { return true; }
    virtual void endVisit(ListAggrArgumentsAST *) {}
    virtual bool visit(StarAggrArgumentsAST *) { return true; }
    virtual void endVisit(StarAggrArgumentsAST *) {}
    virtual bool visit(OpUsingExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(OpUsingExtensionCtxCmdAST *) {}
    virtual bool visit(SchemaExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(SchemaExtensionCtxCmdAST *) {}
    virtual bool visit(TrgEventExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(TrgEventExtensionCtxCmdAST *) {}
    virtual bool visit(TxtSearchExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(TxtSearchExtensionCtxCmdAST *) {}
    virtual bool visit(SequenceExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(SequenceExtensionCtxCmdAST *) {}
    virtual bool visit(TableExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(TableExtensionCtxCmdAST *) {}
    virtual bool visit(ViewExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(ViewExtensionCtxCmdAST *) {}
    virtual bool visit(FdwExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(FdwExtensionCtxCmdAST *) {}
    virtual bool visit(TypeExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(TypeExtensionCtxCmdAST *) {}
    virtual bool visit(ServerExtensionCtxCmdAST *) { return true; }
    virtual void endVisit(ServerExtensionCtxCmdAST *) {}
    virtual bool visit(AlterExtensionStmtAST *) { return true; }
    virtual void endVisit(AlterExtensionStmtAST *) {}
    virtual bool visit(UpdateExtensionCmdAST *) { return true; }
    virtual void endVisit(UpdateExtensionCmdAST *) {}
    virtual bool visit(UpdateToExtensionCmdAST *) { return true; }
    virtual void endVisit(UpdateToExtensionCmdAST *) {}
    virtual bool visit(AlterEnumStmtAST *) { return true; }
    virtual void endVisit(AlterEnumStmtAST *) {}
    virtual bool visit(IfNotExistsAST *) { return true; }
    virtual void endVisit(IfNotExistsAST *) {}
    virtual bool visit(AlterDomainStmtAST *) { return true; }
    virtual void endVisit(AlterDomainStmtAST *) {}
    virtual bool visit(AlterDomainAltDefStmtAST *) { return true; }
    virtual void endVisit(AlterDomainAltDefStmtAST *) {}
    virtual bool visit(AlterDomainAddConstrStmtAST *) { return true; }
    virtual void endVisit(AlterDomainAddConstrStmtAST *) {}
    virtual bool visit(AlterDomainDropConstrStmtAST *) { return true; }
    virtual void endVisit(AlterDomainDropConstrStmtAST *) {}
    virtual bool visit(AlterDomainValConstrStmtAST *) { return true; }
    virtual void endVisit(AlterDomainValConstrStmtAST *) {}
    virtual bool visit(AlterDomainChgNotNullStmtAST *) { return true; }
    virtual void endVisit(AlterDomainChgNotNullStmtAST *) {}
    virtual bool visit(CreateDomainStmtAST *) { return true; }
    virtual void endVisit(CreateDomainStmtAST *) {}
    virtual bool visit(AlterDefaultPrivilegesStmtAST *) { return true; }
    virtual void endVisit(AlterDefaultPrivilegesStmtAST *) {}
    virtual bool visit(DefACLOptionAST *) { return true; }
    virtual void endVisit(DefACLOptionAST *) {}
    virtual bool visit(InSchemaDefACLOptionAST *) { return true; }
    virtual void endVisit(InSchemaDefACLOptionAST *) {}
    virtual bool visit(ForRoleUserDefACLOptionAST *) { return true; }
    virtual void endVisit(ForRoleUserDefACLOptionAST *) {}
    virtual bool visit(DefACLActionAST *) { return true; }
    virtual void endVisit(DefACLActionAST *) {}
    virtual bool visit(RevokeDefACLActionAST *) { return true; }
    virtual void endVisit(RevokeDefACLActionAST *) {}
    virtual bool visit(GrantDefACLActionAST *) { return true; }
    virtual void endVisit(GrantDefACLActionAST *) {}
    virtual bool visit(PrivilegesAST *) { return true; }
    virtual void endVisit(PrivilegesAST *) {}
    virtual bool visit(AllPrivilegesAST *) { return true; }
    virtual void endVisit(AllPrivilegesAST *) {}
    virtual bool visit(PrivilegesAsListAST *) { return true; }
    virtual void endVisit(PrivilegesAsListAST *) {}
    virtual bool visit(CommonPrivilegeAST *) { return true; }
    virtual void endVisit(CommonPrivilegeAST *) {}
    virtual bool visit(GranteeAST *) { return true; }
    virtual void endVisit(GranteeAST *) {}
    virtual bool visit(WithGrantOptionAST *) { return true; }
    virtual void endVisit(WithGrantOptionAST *) {}
    virtual bool visit(AlterDatabaseStmtAST *) { return true; }
    virtual void endVisit(AlterDatabaseStmtAST *) {}
    virtual bool visit(AlterDatabaseSetStmtAST *) { return true; }
    virtual void endVisit(AlterDatabaseSetStmtAST *) {}
    virtual bool visit(AlterDatabaseChgOptStmtAST *) { return true; }
    virtual void endVisit(AlterDatabaseChgOptStmtAST *) {}
    virtual bool visit(AlterDbOptionAST *) { return true; }
    virtual void endVisit(AlterDbOptionAST *) {}
    virtual bool visit(AlterDbConnLimitOptionAST *) { return true; }
    virtual void endVisit(AlterDbConnLimitOptionAST *) {}
    virtual bool visit(AlterDatabaseSetTblspStmtAST *) { return true; }
    virtual void endVisit(AlterDatabaseSetTblspStmtAST *) {}
    virtual bool visit(CreateEventTrigStmtAST *) { return true; }
    virtual void endVisit(CreateEventTrigStmtAST *) {}
    virtual bool visit(EventTriggerWhenAST *) { return true; }
    virtual void endVisit(EventTriggerWhenAST *) {}
    virtual bool visit(EventTriggerValueAST *) { return true; }
    virtual void endVisit(EventTriggerValueAST *) {}
    virtual bool visit(AlterEventTrigStmtAST *) { return true; }
    virtual void endVisit(AlterEventTrigStmtAST *) {}
    virtual bool visit(EventTriggerCmdAST *) { return true; }
    virtual void endVisit(EventTriggerCmdAST *) {}
    virtual bool visit(CreateRoleStmtAST *) { return true; }
    virtual void endVisit(CreateRoleStmtAST *) {}
    virtual bool visit(AlterRoleStmtAST *) { return true; }
    virtual void endVisit(AlterRoleStmtAST *) {}
    virtual bool visit(OptRoleAST *) { return true; }
    virtual void endVisit(OptRoleAST *) {}
    virtual bool visit(OptRoleWithRoleListAST *) { return true; }
    virtual void endVisit(OptRoleWithRoleListAST *) {}
    virtual bool visit(OptRoleStaticAST *) { return true; }
    virtual void endVisit(OptRoleStaticAST *) {}
    virtual bool visit(RoleIdAST *) { return true; }
    virtual void endVisit(RoleIdAST *) {}
    virtual bool visit(DatabaseNameAST *) { return true; }
    virtual void endVisit(DatabaseNameAST *) {}
    virtual bool visit(InDatabaseClauseAST *) { return true; }
    virtual void endVisit(InDatabaseClauseAST *) {}
    virtual bool visit(AlterRoleSetStmtAST *) { return true; }
    virtual void endVisit(AlterRoleSetStmtAST *) {}
    virtual bool visit(AlterAnyRoleSetStmtAST *) { return true; }
    virtual void endVisit(AlterAnyRoleSetStmtAST *) {}
    virtual bool visit(AlterAllRolesSetStmtAST *) { return true; }
    virtual void endVisit(AlterAllRolesSetStmtAST *) {}
    virtual bool visit(SetResetClauseAST *) { return true; }
    virtual void endVisit(SetResetClauseAST *) {}
    virtual bool visit(SetResetClauseAsSetAST *) { return true; }
    virtual void endVisit(SetResetClauseAsSetAST *) {}
    virtual bool visit(SetResetClauseAsVarResetAST *) { return true; }
    virtual void endVisit(SetResetClauseAsVarResetAST *) {}
    virtual bool visit(SetRestAST *) { return true; }
    virtual void endVisit(SetRestAST *) {}
    virtual bool visit(SetRestTransactionAST *) { return true; }
    virtual void endVisit(SetRestTransactionAST *) {}
    virtual bool visit(SetRestVarNameToAST *) { return true; }
    virtual void endVisit(SetRestVarNameToAST *) {}
    virtual bool visit(SetRestVarNameToValuesAST *) { return true; }
    virtual void endVisit(SetRestVarNameToValuesAST *) {}
    virtual bool visit(SetRestZoneValueAST *) { return true; }
    virtual void endVisit(SetRestZoneValueAST *) {}
    virtual bool visit(SetRestCatalogAST *) { return true; }
    virtual void endVisit(SetRestCatalogAST *) {}
    virtual bool visit(SetRestSchemaAST *) { return true; }
    virtual void endVisit(SetRestSchemaAST *) {}
    virtual bool visit(SetRestEncodingAST *) { return true; }
    virtual void endVisit(SetRestEncodingAST *) {}
    virtual bool visit(SetRestRoleAST *) { return true; }
    virtual void endVisit(SetRestRoleAST *) {}
    virtual bool visit(SetRestSessionAuthAST *) { return true; }
    virtual void endVisit(SetRestSessionAuthAST *) {}
    virtual bool visit(SetRestXmlOptionAST *) { return true; }
    virtual void endVisit(SetRestXmlOptionAST *) {}
    virtual bool visit(SetRestTransactioSnapshotAST *) { return true; }
    virtual void endVisit(SetRestTransactioSnapshotAST *) {}
    virtual bool visit(VariableShowStmtAST *) { return true; }
    virtual void endVisit(VariableShowStmtAST *) {}
    virtual bool visit(VarShowStmtAST *) { return true; }
    virtual void endVisit(VarShowStmtAST *) {}
    virtual bool visit(TimeZoneShowStmtAST *) { return true; }
    virtual void endVisit(TimeZoneShowStmtAST *) {}
    virtual bool visit(IsoLevelShowStmtAST *) { return true; }
    virtual void endVisit(IsoLevelShowStmtAST *) {}
    virtual bool visit(SessionAuthShowStmtAST *) { return true; }
    virtual void endVisit(SessionAuthShowStmtAST *) {}
    virtual bool visit(AllShowStmtAST *) { return true; }
    virtual void endVisit(AllShowStmtAST *) {}
    virtual bool visit(VariableSetStmtAST *) { return true; }
    virtual void endVisit(VariableSetStmtAST *) {}
    virtual bool visit(VariableResetStmtAST *) { return true; }
    virtual void endVisit(VariableResetStmtAST *) {}
    virtual bool visit(SetRestResetVarNameAST *) { return true; }
    virtual void endVisit(SetRestResetVarNameAST *) {}
    virtual bool visit(SetRestResetTimeZoneAST *) { return true; }
    virtual void endVisit(SetRestResetTimeZoneAST *) {}
    virtual bool visit(SetRestResetTransLevelAST *) { return true; }
    virtual void endVisit(SetRestResetTransLevelAST *) {}
    virtual bool visit(SetRestResetAuthorizationAST *) { return true; }
    virtual void endVisit(SetRestResetAuthorizationAST *) {}
    virtual bool visit(SetRestResetAllAST *) { return true; }
    virtual void endVisit(SetRestResetAllAST *) {}
    virtual bool visit(IsoLevelAST *) { return true; }
    virtual void endVisit(IsoLevelAST *) {}
    virtual bool visit(TransactionModeAST *) { return true; }
    virtual void endVisit(TransactionModeAST *) {}
    virtual bool visit(TransactionModeIsoLevelAST *) { return true; }
    virtual void endVisit(TransactionModeIsoLevelAST *) {}
    virtual bool visit(TransactionModeReadWriteAST *) { return true; }
    virtual void endVisit(TransactionModeReadWriteAST *) {}
    virtual bool visit(TransactionModeDeferAST *) { return true; }
    virtual void endVisit(TransactionModeDeferAST *) {}
    virtual bool visit(IntervalSecondAST *) { return true; }
    virtual void endVisit(IntervalSecondAST *) {}
    virtual bool visit(IntervalAST *) { return true; }
    virtual void endVisit(IntervalAST *) {}
    virtual bool visit(IntervalSimpleAST *) { return true; }
    virtual void endVisit(IntervalSimpleAST *) {}
    virtual bool visit(IntervalSimpleToAST *) { return true; }
    virtual void endVisit(IntervalSimpleToAST *) {}
    virtual bool visit(IntervalSimpleToSecondAST *) { return true; }
    virtual void endVisit(IntervalSimpleToSecondAST *) {}
    virtual bool visit(VarValueAST *) { return true; }
    virtual void endVisit(VarValueAST *) {}
    virtual bool visit(ZoneValueAST *) { return true; }
    virtual void endVisit(ZoneValueAST *) {}
    virtual bool visit(ZoneValueSimpleAST *) { return true; }
    virtual void endVisit(ZoneValueSimpleAST *) {}
    virtual bool visit(ZoneValueIntervalAST *) { return true; }
    virtual void endVisit(ZoneValueIntervalAST *) {}
    virtual bool visit(DropRoleStmtAST *) { return true; }
    virtual void endVisit(DropRoleStmtAST *) {}
    virtual bool visit(CreateGroupStmtAST *) { return true; }
    virtual void endVisit(CreateGroupStmtAST *) {}
    virtual bool visit(AlterGroupStmtAST *) { return true; }
    virtual void endVisit(AlterGroupStmtAST *) {}
    virtual bool visit(DiscardStmtAST *) { return true; }
    virtual void endVisit(DiscardStmtAST *) {}
    virtual bool visit(DropStatementAST *) { return true; }
    virtual void endVisit(DropStatementAST *) {}
    virtual bool visit(TruncateStatementAST *) { return true; }
    virtual void endVisit(TruncateStatementAST *) {}
    virtual bool visit(AlterStmtAST *) { return true; }
    virtual void endVisit(AlterStmtAST *) {}
    virtual bool visit(AlterIndexStmtAST *) { return true; }
    virtual void endVisit(AlterIndexStmtAST *) {}
    virtual bool visit(AlterSequenceStmtAST *) { return true; }
    virtual void endVisit(AlterSequenceStmtAST *) {}
    virtual bool visit(AlterTableStmtAST *) { return true; }
    virtual void endVisit(AlterTableStmtAST *) {}
    virtual bool visit(AlterForeignTableStmtAST *) { return true; }
    virtual void endVisit(AlterForeignTableStmtAST *) {}
    virtual bool visit(AlterViewStmtAST *) { return true; }
    virtual void endVisit(AlterViewStmtAST *) {}
    virtual bool visit(UpdateStmtAST *) { return true; }
    virtual void endVisit(UpdateStmtAST *) {}
    virtual bool visit(SetClauseAST *) { return true; }
    virtual void endVisit(SetClauseAST *) {}
    virtual bool visit(SingleSetClauseAST *) { return true; }
    virtual void endVisit(SingleSetClauseAST *) {}
    virtual bool visit(MultipleSetClauseAST *) { return true; }
    virtual void endVisit(MultipleSetClauseAST *) {}
    virtual bool visit(InsertStmtAST *) { return true; }
    virtual void endVisit(InsertStmtAST *) {}
    virtual bool visit(InsertRestAST *) { return true; }
    virtual void endVisit(InsertRestAST *) {}
    virtual bool visit(CommonInsertRestAST *) { return true; }
    virtual void endVisit(CommonInsertRestAST *) {}
    virtual bool visit(DefaultValuesInsertRestAST *) { return true; }
    virtual void endVisit(DefaultValuesInsertRestAST *) {}
    virtual bool visit(DeleteStmtAST *) { return true; }
    virtual void endVisit(DeleteStmtAST *) {}
    virtual bool visit(ReturningClauseAST *) { return true; }
    virtual void endVisit(ReturningClauseAST *) {}
    virtual bool visit(WhereOrCurrentClauseAST *) { return true; }
    virtual void endVisit(WhereOrCurrentClauseAST *) {}
    virtual bool visit(WhereNoCurrentClauseAST *) { return true; }
    virtual void endVisit(WhereNoCurrentClauseAST *) {}
    virtual bool visit(CurrenWhereClauseAST *) { return true; }
    virtual void endVisit(CurrenWhereClauseAST *) {}
    virtual bool visit(DelUsingClauseAST *) { return true; }
    virtual void endVisit(DelUsingClauseAST *) {}
    virtual bool visit(RelationAliasExprAST *) { return true; }
    virtual void endVisit(RelationAliasExprAST *) {}
    virtual bool visit(SelectStmtAST *) { return true; }
    virtual void endVisit(SelectStmtAST *) {}
    virtual bool visit(SelectWithParensAST *) { return true; }
    virtual void endVisit(SelectWithParensAST *) {}
    virtual bool visit(SelectNoParensAST *) { return true; }
    virtual void endVisit(SelectNoParensAST *) {}
    virtual bool visit(WithClauseAST *) { return true; }
    virtual void endVisit(WithClauseAST *) {}
    virtual bool visit(CommonTableExprAST *) { return true; }
    virtual void endVisit(CommonTableExprAST *) {}
    virtual bool visit(CompoundColumnNamesAST *) { return true; }
    virtual void endVisit(CompoundColumnNamesAST *) {}
    virtual bool visit(SortClauseAST *) { return true; }
    virtual void endVisit(SortClauseAST *) {}
    virtual bool visit(OrderExprAST *) { return true; }
    virtual void endVisit(OrderExprAST *) {}
    virtual bool visit(UsingOrderExprAST *) { return true; }
    virtual void endVisit(UsingOrderExprAST *) {}
    virtual bool visit(CommonOrderExprAST *) { return true; }
    virtual void endVisit(CommonOrderExprAST *) {}
    virtual bool visit(LockingLimitClauseAST *) { return true; }
    virtual void endVisit(LockingLimitClauseAST *) {}
    virtual bool visit(LockingFirstLimitClauseAST *) { return true; }
    virtual void endVisit(LockingFirstLimitClauseAST *) {}
    virtual bool visit(LockingLastLimitClauseAST *) { return true; }
    virtual void endVisit(LockingLastLimitClauseAST *) {}
    virtual bool visit(LimitClauseAST *) { return true; }
    virtual void endVisit(LimitClauseAST *) {}
    virtual bool visit(LimitOffsetClauseAST *) { return true; }
    virtual void endVisit(LimitOffsetClauseAST *) {}
    virtual bool visit(OffsetLimitClauseAST *) { return true; }
    virtual void endVisit(OffsetLimitClauseAST *) {}
    virtual bool visit(OffsetAST *) { return true; }
    virtual void endVisit(OffsetAST *) {}
    virtual bool visit(LimitAST *) { return true; }
    virtual void endVisit(LimitAST *) {}
    virtual bool visit(CommonLimitAST *) { return true; }
    virtual void endVisit(CommonLimitAST *) {}
    virtual bool visit(FetchLimitAST *) { return true; }
    virtual void endVisit(FetchLimitAST *) {}
    virtual bool visit(FetchFirstValueAST *) { return true; }
    virtual void endVisit(FetchFirstValueAST *) {}
    virtual bool visit(ConstFetchFirstValueAST *) { return true; }
    virtual void endVisit(ConstFetchFirstValueAST *) {}
    virtual bool visit(CompoundFetchFirstValueAST *) { return true; }
    virtual void endVisit(CompoundFetchFirstValueAST *) {}
    virtual bool visit(LimitValueAST *) { return true; }
    virtual void endVisit(LimitValueAST *) {}
    virtual bool visit(AllLimitValueAST *) { return true; }
    virtual void endVisit(AllLimitValueAST *) {}
    virtual bool visit(ExprLimitValueAST *) { return true; }
    virtual void endVisit(ExprLimitValueAST *) {}
    virtual bool visit(ForLockingClauseAST *) { return true; }
    virtual void endVisit(ForLockingClauseAST *) {}
    virtual bool visit(ReadOnlyForLockingClauseAST *) { return true; }
    virtual void endVisit(ReadOnlyForLockingClauseAST *) {}
    virtual bool visit(CommonForLockingClauseAST *) { return true; }
    virtual void endVisit(CommonForLockingClauseAST *) {}
    virtual bool visit(LockingExprAST *) { return true; }
    virtual void endVisit(LockingExprAST *) {}
    virtual bool visit(LockingStrengthAST *) { return true; }
    virtual void endVisit(LockingStrengthAST *) {}
    virtual bool visit(LockingRelsAST *) { return true; }
    virtual void endVisit(LockingRelsAST *) {}
    virtual bool visit(SelectClauseAST *) { return true; }
    virtual void endVisit(SelectClauseAST *) {}
    virtual bool visit(CompoundSelectClauseAST *) { return true; }
    virtual void endVisit(CompoundSelectClauseAST *) {}
    virtual bool visit(CoreSimpleSelectAST *) { return true; }
    virtual void endVisit(CoreSimpleSelectAST *) {}
    virtual bool visit(SimpleSelectAST *) { return true; }
    virtual void endVisit(SimpleSelectAST *) {}
    virtual bool visit(ValuesSimpleSelectAST *) { return true; }
    virtual void endVisit(ValuesSimpleSelectAST *) {}
    virtual bool visit(TableSimpleSelectAST *) { return true; }
    virtual void endVisit(TableSimpleSelectAST *) {}
    virtual bool visit(CompositeSimpleSelectAST *) { return true; }
    virtual void endVisit(CompositeSimpleSelectAST *) {}
    virtual bool visit(SelectCoreAST *) { return true; }
    virtual void endVisit(SelectCoreAST *) {}
    virtual bool visit(WindowClauseAST *) { return true; }
    virtual void endVisit(WindowClauseAST *) {}
    virtual bool visit(WindowDefinitionAST *) { return true; }
    virtual void endVisit(WindowDefinitionAST *) {}
    virtual bool visit(HavingClauseAST *) { return true; }
    virtual void endVisit(HavingClauseAST *) {}
    virtual bool visit(GroupByClauseAST *) { return true; }
    virtual void endVisit(GroupByClauseAST *) {}
    virtual bool visit(WhereClauseAST *) { return true; }
    virtual void endVisit(WhereClauseAST *) {}
    virtual bool visit(FromClauseAST *) { return true; }
    virtual void endVisit(FromClauseAST *) {}
    virtual bool visit(TableRefAST *) { return true; }
    virtual void endVisit(TableRefAST *) {}
    virtual bool visit(JoinedTableRefAST *) { return true; }
    virtual void endVisit(JoinedTableRefAST *) {}
    virtual bool visit(JoinedTableAST *) { return true; }
    virtual void endVisit(JoinedTableAST *) {}
    virtual bool visit(CommonJoinedTableAST *) { return true; }
    virtual void endVisit(CommonJoinedTableAST *) {}
    virtual bool visit(JoinTypeAST *) { return true; }
    virtual void endVisit(JoinTypeAST *) {}
    virtual bool visit(JoinConditionAST *) { return true; }
    virtual void endVisit(JoinConditionAST *) {}
    virtual bool visit(OnJoinConditionAST *) { return true; }
    virtual void endVisit(OnJoinConditionAST *) {}
    virtual bool visit(UsingJoinConditionAST *) { return true; }
    virtual void endVisit(UsingJoinConditionAST *) {}
    virtual bool visit(CrossJoinedTableAST *) { return true; }
    virtual void endVisit(CrossJoinedTableAST *) {}
    virtual bool visit(CompoundJoinedTableAST *) { return true; }
    virtual void endVisit(CompoundJoinedTableAST *) {}
    virtual bool visit(CompoundTableRefAST *) { return true; }
    virtual void endVisit(CompoundTableRefAST *) {}
    virtual bool visit(FuncionTableRefAST *) { return true; }
    virtual void endVisit(FuncionTableRefAST *) {}
    virtual bool visit(FuncAliasClauseAST *) { return true; }
    virtual void endVisit(FuncAliasClauseAST *) {}
    virtual bool visit(FuncFuncAliasClauseAST *) { return true; }
    virtual void endVisit(FuncFuncAliasClauseAST *) {}
    virtual bool visit(TableFuncAliasClauseAST *) { return true; }
    virtual void endVisit(TableFuncAliasClauseAST *) {}
    virtual bool visit(TableFuncElementAST *) { return true; }
    virtual void endVisit(TableFuncElementAST *) {}
    virtual bool visit(CollateClauseAST *) { return true; }
    virtual void endVisit(CollateClauseAST *) {}
    virtual bool visit(FunctionExpressionAST *) { return true; }
    virtual void endVisit(FunctionExpressionAST *) {}
    virtual bool visit(XmlserializeFunctionExprAST *) { return true; }
    virtual void endVisit(XmlserializeFunctionExprAST *) {}
    virtual bool visit(XmlrootFunctionExprAST *) { return true; }
    virtual void endVisit(XmlrootFunctionExprAST *) {}
    virtual bool visit(XmlRootVersionAST *) { return true; }
    virtual void endVisit(XmlRootVersionAST *) {}
    virtual bool visit(ExprXmlRootVersionAST *) { return true; }
    virtual void endVisit(ExprXmlRootVersionAST *) {}
    virtual bool visit(NoValueXmlRootVersionAST *) { return true; }
    virtual void endVisit(NoValueXmlRootVersionAST *) {}
    virtual bool visit(XmlRootStandaloneAST *) { return true; }
    virtual void endVisit(XmlRootStandaloneAST *) {}
    virtual bool visit(XmlpiFunctionExprAST *) { return true; }
    virtual void endVisit(XmlpiFunctionExprAST *) {}
    virtual bool visit(XmlparseFunctionExprAST *) { return true; }
    virtual void endVisit(XmlparseFunctionExprAST *) {}
    virtual bool visit(XmlWhitespaceOptionAST *) { return true; }
    virtual void endVisit(XmlWhitespaceOptionAST *) {}
    virtual bool visit(XmlforestFunctionExprAST *) { return true; }
    virtual void endVisit(XmlforestFunctionExprAST *) {}
    virtual bool visit(XmlexistsFunctionExprAST *) { return true; }
    virtual void endVisit(XmlexistsFunctionExprAST *) {}
    virtual bool visit(XmlExistsArgumentAST *) { return true; }
    virtual void endVisit(XmlExistsArgumentAST *) {}
    virtual bool visit(XmlelementFunctionExprAST *) { return true; }
    virtual void endVisit(XmlelementFunctionExprAST *) {}
    virtual bool visit(XmlAttributesAST *) { return true; }
    virtual void endVisit(XmlAttributesAST *) {}
    virtual bool visit(XmlAttributeAST *) { return true; }
    virtual void endVisit(XmlAttributeAST *) {}
    virtual bool visit(CoalesceFunctionExprAST *) { return true; }
    virtual void endVisit(CoalesceFunctionExprAST *) {}
    virtual bool visit(GreatestFunctionExprAST *) { return true; }
    virtual void endVisit(GreatestFunctionExprAST *) {}
    virtual bool visit(LeastFunctionExprAST *) { return true; }
    virtual void endVisit(LeastFunctionExprAST *) {}
    virtual bool visit(XmlconcatFunctionExprAST *) { return true; }
    virtual void endVisit(XmlconcatFunctionExprAST *) {}
    virtual bool visit(NullifFunctionExprAST *) { return true; }
    virtual void endVisit(NullifFunctionExprAST *) {}
    virtual bool visit(TrimFunctionExprAST *) { return true; }
    virtual void endVisit(TrimFunctionExprAST *) {}
    virtual bool visit(TrimExprAST *) { return true; }
    virtual void endVisit(TrimExprAST *) {}
    virtual bool visit(TraetFunctionExprAST *) { return true; }
    virtual void endVisit(TraetFunctionExprAST *) {}
    virtual bool visit(SubstringFunctionExprAST *) { return true; }
    virtual void endVisit(SubstringFunctionExprAST *) {}
    virtual bool visit(SubstringExprAST *) { return true; }
    virtual void endVisit(SubstringExprAST *) {}
    virtual bool visit(StandardSubstringExprAST *) { return true; }
    virtual void endVisit(StandardSubstringExprAST *) {}
    virtual bool visit(ListSubstringExprAST *) { return true; }
    virtual void endVisit(ListSubstringExprAST *) {}
    virtual bool visit(PositionFunctionExprAST *) { return true; }
    virtual void endVisit(PositionFunctionExprAST *) {}
    virtual bool visit(OverlayFunctionExprAST *) { return true; }
    virtual void endVisit(OverlayFunctionExprAST *) {}
    virtual bool visit(PositionExprAST *) { return true; }
    virtual void endVisit(PositionExprAST *) {}
    virtual bool visit(OverlayExprAST *) { return true; }
    virtual void endVisit(OverlayExprAST *) {}
    virtual bool visit(OverlayPlacingAST *) { return true; }
    virtual void endVisit(OverlayPlacingAST *) {}
    virtual bool visit(ExtractFunctionExprAST *) { return true; }
    virtual void endVisit(ExtractFunctionExprAST *) {}
    virtual bool visit(SubstringFromAST *) { return true; }
    virtual void endVisit(SubstringFromAST *) {}
    virtual bool visit(SubstringForAST *) { return true; }
    virtual void endVisit(SubstringForAST *) {}
    virtual bool visit(ExtractExprAST *) { return true; }
    virtual void endVisit(ExtractExprAST *) {}
    virtual bool visit(CastFunctionExprAST *) { return true; }
    virtual void endVisit(CastFunctionExprAST *) {}
    virtual bool visit(CollationFunctionExprAST *) { return true; }
    virtual void endVisit(CollationFunctionExprAST *) {}
    virtual bool visit(SimpleFunctionExprAST *) { return true; }
    virtual void endVisit(SimpleFunctionExprAST *) {}
    virtual bool visit(CommonFunctionExprAST *) { return true; }
    virtual void endVisit(CommonFunctionExprAST *) {}
    virtual bool visit(FunctionArgExprAST *) { return true; }
    virtual void endVisit(FunctionArgExprAST *) {}
    virtual bool visit(OverClauseAST *) { return true; }
    virtual void endVisit(OverClauseAST *) {}
    virtual bool visit(SimpleOverClauseAST *) { return true; }
    virtual void endVisit(SimpleOverClauseAST *) {}
    virtual bool visit(WindowOverClauseAST *) { return true; }
    virtual void endVisit(WindowOverClauseAST *) {}
    virtual bool visit(WindowSpecificationAST *) { return true; }
    virtual void endVisit(WindowSpecificationAST *) {}
    virtual bool visit(PartitionClauseAST *) { return true; }
    virtual void endVisit(PartitionClauseAST *) {}
    virtual bool visit(FrameClauseAST *) { return true; }
    virtual void endVisit(FrameClauseAST *) {}
    virtual bool visit(FrameExtentAST *) { return true; }
    virtual void endVisit(FrameExtentAST *) {}
    virtual bool visit(BoundFrameExtentAST *) { return true; }
    virtual void endVisit(BoundFrameExtentAST *) {}
    virtual bool visit(BetweenFrameExtentAST *) { return true; }
    virtual void endVisit(BetweenFrameExtentAST *) {}
    virtual bool visit(FrameBoundAST *) { return true; }
    virtual void endVisit(FrameBoundAST *) {}
    virtual bool visit(SimpleFrameBoundAST *) { return true; }
    virtual void endVisit(SimpleFrameBoundAST *) {}
    virtual bool visit(ExprFrameBoundAST *) { return true; }
    virtual void endVisit(ExprFrameBoundAST *) {}
    virtual bool visit(DirectTableRefAST *) { return true; }
    virtual void endVisit(DirectTableRefAST *) {}
    virtual bool visit(TableAliasClauseAST *) { return true; }
    virtual void endVisit(TableAliasClauseAST *) {}
    virtual bool visit(DistinctClauseAST *) { return true; }
    virtual void endVisit(DistinctClauseAST *) {}
    virtual bool visit(SimpleDistinctClauseAST *) { return true; }
    virtual void endVisit(SimpleDistinctClauseAST *) {}
    virtual bool visit(ExprListDistinctClauseAST *) { return true; }
    virtual void endVisit(ExprListDistinctClauseAST *) {}
    virtual bool visit(SelectTargetAST *) { return true; }
    virtual void endVisit(SelectTargetAST *) {}
    virtual bool visit(StarSelectTargetAST *) { return true; }
    virtual void endVisit(StarSelectTargetAST *) {}
    virtual bool visit(ExprSelectTargetAST *) { return true; }
    virtual void endVisit(ExprSelectTargetAST *) {}
    virtual bool visit(IntoClauseAST *) { return true; }
    virtual void endVisit(IntoClauseAST *) {}
    virtual bool visit(OptTempTableNameAST *) { return true; }
    virtual void endVisit(OptTempTableNameAST *) {}
    virtual bool visit(ValuesClauseAST *) { return true; }
    virtual void endVisit(ValuesClauseAST *) {}
    virtual bool visit(ColumnExprRowAST *) { return true; }
    virtual void endVisit(ColumnExprRowAST *) {}
    virtual bool visit(ColumnExpressionAST *) { return true; }
    virtual void endVisit(ColumnExpressionAST *) {}
    virtual bool visit(DefaultColumnExpressionAST *) { return true; }
    virtual void endVisit(DefaultColumnExpressionAST *) {}
    virtual bool visit(ExprColumnExpressionAST *) { return true; }
    virtual void endVisit(ExprColumnExpressionAST *) {}
    virtual bool visit(AlterTableCmdAST *) { return true; }
    virtual void endVisit(AlterTableCmdAST *) {}
    virtual bool visit(AlterTableGenericCmdAST *) { return true; }
    virtual void endVisit(AlterTableGenericCmdAST *) {}
    virtual bool visit(AlterGenericOptionAST *) { return true; }
    virtual void endVisit(AlterGenericOptionAST *) {}
    virtual bool visit(SetAddAlterGenericOptionAST *) { return true; }
    virtual void endVisit(SetAddAlterGenericOptionAST *) {}
    virtual bool visit(DropAlterGenericOptionAST *) { return true; }
    virtual void endVisit(DropAlterGenericOptionAST *) {}
    virtual bool visit(AlterTableSetReloptCmdAST *) { return true; }
    virtual void endVisit(AlterTableSetReloptCmdAST *) {}
    virtual bool visit(AlterTableTablespaceCmdAST *) { return true; }
    virtual void endVisit(AlterTableTablespaceCmdAST *) {}
    virtual bool visit(AlterTableOwnerCmdAST *) { return true; }
    virtual void endVisit(AlterTableOwnerCmdAST *) {}
    virtual bool visit(AlterTableNotOfCmdAST *) { return true; }
    virtual void endVisit(AlterTableNotOfCmdAST *) {}
    virtual bool visit(AlterTableOfCmdAST *) { return true; }
    virtual void endVisit(AlterTableOfCmdAST *) {}
    virtual bool visit(AlterTableInheritCmdAST *) { return true; }
    virtual void endVisit(AlterTableInheritCmdAST *) {}
    virtual bool visit(AlterTableTriggerCmdAST *) { return true; }
    virtual void endVisit(AlterTableTriggerCmdAST *) {}
    virtual bool visit(AlterTableRuleCmdAST *) { return true; }
    virtual void endVisit(AlterTableRuleCmdAST *) {}
    virtual bool visit(WildcardAST *) { return true; }
    virtual void endVisit(WildcardAST *) {}
    virtual bool visit(NameWildcardAST *) { return true; }
    virtual void endVisit(NameWildcardAST *) {}
    virtual bool visit(AllOrUserWildcardAST *) { return true; }
    virtual void endVisit(AllOrUserWildcardAST *) {}
    virtual bool visit(AlterTableClusterOffCmdAST *) { return true; }
    virtual void endVisit(AlterTableClusterOffCmdAST *) {}
    virtual bool visit(AlterTableClusterOnCmdAST *) { return true; }
    virtual void endVisit(AlterTableClusterOnCmdAST *) {}
    virtual bool visit(AlterTableSetOidsCmdAST *) { return true; }
    virtual void endVisit(AlterTableSetOidsCmdAST *) {}
    virtual bool visit(AlterTableDropConstrCmdAST *) { return true; }
    virtual void endVisit(AlterTableDropConstrCmdAST *) {}
    virtual bool visit(AlterTableValConstrCmdAST *) { return true; }
    virtual void endVisit(AlterTableValConstrCmdAST *) {}
    virtual bool visit(AlterTableAddConstrCmdAST *) { return true; }
    virtual void endVisit(AlterTableAddConstrCmdAST *) {}
    virtual bool visit(AlterTableSetTypeCmdAST *) { return true; }
    virtual void endVisit(AlterTableSetTypeCmdAST *) {}
    virtual bool visit(SetDataAST *) { return true; }
    virtual void endVisit(SetDataAST *) {}
    virtual bool visit(UsingClauseAST *) { return true; }
    virtual void endVisit(UsingClauseAST *) {}
    virtual bool visit(AlterTableSetStorageCmdAST *) { return true; }
    virtual void endVisit(AlterTableSetStorageCmdAST *) {}
    virtual bool visit(AlterTableChgReloptCmdAST *) { return true; }
    virtual void endVisit(AlterTableChgReloptCmdAST *) {}
    virtual bool visit(RelOptionsAST *) { return true; }
    virtual void endVisit(RelOptionsAST *) {}
    virtual bool visit(RelOptionAST *) { return true; }
    virtual void endVisit(RelOptionAST *) {}
    virtual bool visit(AlterTableSetStatCmdAST *) { return true; }
    virtual void endVisit(AlterTableSetStatCmdAST *) {}
    virtual bool visit(AlterTableChgNotNullCmdAST *) { return true; }
    virtual void endVisit(AlterTableChgNotNullCmdAST *) {}
    virtual bool visit(AlterTableChgDefColCmdAST *) { return true; }
    virtual void endVisit(AlterTableChgDefColCmdAST *) {}
    virtual bool visit(AlterDefaultColumnAST *) { return true; }
    virtual void endVisit(AlterDefaultColumnAST *) {}
    virtual bool visit(SetAlterDefaultColumnAST *) { return true; }
    virtual void endVisit(SetAlterDefaultColumnAST *) {}
    virtual bool visit(DropAlterDefaultColumnAST *) { return true; }
    virtual void endVisit(DropAlterDefaultColumnAST *) {}
    virtual bool visit(AlterTableDropColCmdAST *) { return true; }
    virtual void endVisit(AlterTableDropColCmdAST *) {}
    virtual bool visit(AlterTableAddColCmdAST *) { return true; }
    virtual void endVisit(AlterTableAddColCmdAST *) {}
    virtual bool visit(ColumnDefAST *) { return true; }
    virtual void endVisit(ColumnDefAST *) {}
    virtual bool visit(ConstraintAST *) { return true; }
    virtual void endVisit(ConstraintAST *) {}
    virtual bool visit(TableConstraintAST *) { return true; }
    virtual void endVisit(TableConstraintAST *) {}
    virtual bool visit(TableConstraintBodyAST *) { return true; }
    virtual void endVisit(TableConstraintBodyAST *) {}
    virtual bool visit(FkTableConstraintAST *) { return true; }
    virtual void endVisit(FkTableConstraintAST *) {}
    virtual bool visit(ExcludeTableConstraintAST *) { return true; }
    virtual void endVisit(ExcludeTableConstraintAST *) {}
    virtual bool visit(AccessMethodClauseAST *) { return true; }
    virtual void endVisit(AccessMethodClauseAST *) {}
    virtual bool visit(ExclusionWhereClauseAST *) { return true; }
    virtual void endVisit(ExclusionWhereClauseAST *) {}
    virtual bool visit(ExclusionConstraintAST *) { return true; }
    virtual void endVisit(ExclusionConstraintAST *) {}
    virtual bool visit(IndexElementAST *) { return true; }
    virtual void endVisit(IndexElementAST *) {}
    virtual bool visit(ColIndexElementAST *) { return true; }
    virtual void endVisit(ColIndexElementAST *) {}
    virtual bool visit(FuncExprIndexElementAST *) { return true; }
    virtual void endVisit(FuncExprIndexElementAST *) {}
    virtual bool visit(ExprIndexElementAST *) { return true; }
    virtual void endVisit(ExprIndexElementAST *) {}
    virtual bool visit(ClassClauseAST *) { return true; }
    virtual void endVisit(ClassClauseAST *) {}
    virtual bool visit(IndPrimKeyTableConstraintAST *) { return true; }
    virtual void endVisit(IndPrimKeyTableConstraintAST *) {}
    virtual bool visit(PrimKeyTableConstraintAST *) { return true; }
    virtual void endVisit(PrimKeyTableConstraintAST *) {}
    virtual bool visit(IndUniqueTableConstraintAST *) { return true; }
    virtual void endVisit(IndUniqueTableConstraintAST *) {}
    virtual bool visit(ExistingIndexAST *) { return true; }
    virtual void endVisit(ExistingIndexAST *) {}
    virtual bool visit(UniqueTableConstraintAST *) { return true; }
    virtual void endVisit(UniqueTableConstraintAST *) {}
    virtual bool visit(CheckTableConstraintAST *) { return true; }
    virtual void endVisit(CheckTableConstraintAST *) {}
    virtual bool visit(ConstraintAttributeAST *) { return true; }
    virtual void endVisit(ConstraintAttributeAST *) {}
    virtual bool visit(DeferConstraintAttrAST *) { return true; }
    virtual void endVisit(DeferConstraintAttrAST *) {}
    virtual bool visit(InitiallyConstraintAttrAST *) { return true; }
    virtual void endVisit(InitiallyConstraintAttrAST *) {}
    virtual bool visit(NotValidConstraintAttrAST *) { return true; }
    virtual void endVisit(NotValidConstraintAttrAST *) {}
    virtual bool visit(NoInheritConstraintAttrAST *) { return true; }
    virtual void endVisit(NoInheritConstraintAttrAST *) {}
    virtual bool visit(ColumnConstraintAST *) { return true; }
    virtual void endVisit(ColumnConstraintAST *) {}
    virtual bool visit(CommonColConstraintAST *) { return true; }
    virtual void endVisit(CommonColConstraintAST *) {}
    virtual bool visit(ConstraintBodyAST *) { return true; }
    virtual void endVisit(ConstraintBodyAST *) {}
    virtual bool visit(RefConstraintBodyAST *) { return true; }
    virtual void endVisit(RefConstraintBodyAST *) {}
    virtual bool visit(RefColumnsAST *) { return true; }
    virtual void endVisit(RefColumnsAST *) {}
    virtual bool visit(KeyMatchAST *) { return true; }
    virtual void endVisit(KeyMatchAST *) {}
    virtual bool visit(KeyActionsAST *) { return true; }
    virtual void endVisit(KeyActionsAST *) {}
    virtual bool visit(UpdDelKeyActionsAST *) { return true; }
    virtual void endVisit(UpdDelKeyActionsAST *) {}
    virtual bool visit(DelUpdKeyActionsAST *) { return true; }
    virtual void endVisit(DelUpdKeyActionsAST *) {}
    virtual bool visit(KeyUpdateAST *) { return true; }
    virtual void endVisit(KeyUpdateAST *) {}
    virtual bool visit(KeyDeleteAST *) { return true; }
    virtual void endVisit(KeyDeleteAST *) {}
    virtual bool visit(KeyActionAST *) { return true; }
    virtual void endVisit(KeyActionAST *) {}
    virtual bool visit(DefaultConstraintBodyAST *) { return true; }
    virtual void endVisit(DefaultConstraintBodyAST *) {}
    virtual bool visit(CheckConstraintBodyAST *) { return true; }
    virtual void endVisit(CheckConstraintBodyAST *) {}
    virtual bool visit(NoInheritAST *) { return true; }
    virtual void endVisit(NoInheritAST *) {}
    virtual bool visit(UniqueConstraintBodyAST *) { return true; }
    virtual void endVisit(UniqueConstraintBodyAST *) {}
    virtual bool visit(PrimaryKeyConstraintBodyAST *) { return true; }
    virtual void endVisit(PrimaryKeyConstraintBodyAST *) {}
    virtual bool visit(ConstraintTablespaceAST *) { return true; }
    virtual void endVisit(ConstraintTablespaceAST *) {}
    virtual bool visit(ConstraintDefinitionsAST *) { return true; }
    virtual void endVisit(ConstraintDefinitionsAST *) {}
    virtual bool visit(DefinitionAST *) { return true; }
    virtual void endVisit(DefinitionAST *) {}
    virtual bool visit(DefinitionElemAST *) { return true; }
    virtual void endVisit(DefinitionElemAST *) {}
    virtual bool visit(DefinitionArgumentAST *) { return true; }
    virtual void endVisit(DefinitionArgumentAST *) {}
    virtual bool visit(FuncTyDefinitionArgumentAST *) { return true; }
    virtual void endVisit(FuncTyDefinitionArgumentAST *) {}
    virtual bool visit(FunctionTypeAST *) { return true; }
    virtual void endVisit(FunctionTypeAST *) {}
    virtual bool visit(PercentFunctionTypeAST *) { return true; }
    virtual void endVisit(PercentFunctionTypeAST *) {}
    virtual bool visit(SimpleFunctionTypeAST *) { return true; }
    virtual void endVisit(SimpleFunctionTypeAST *) {}
    virtual bool visit(QualOpDefinitionArgumentAST *) { return true; }
    virtual void endVisit(QualOpDefinitionArgumentAST *) {}
    virtual bool visit(SimpleDefinitionArgumentAST *) { return true; }
    virtual void endVisit(SimpleDefinitionArgumentAST *) {}
    virtual bool visit(NotNullConstraintBodyAST *) { return true; }
    virtual void endVisit(NotNullConstraintBodyAST *) {}
    virtual bool visit(AttrColConstraintAST *) { return true; }
    virtual void endVisit(AttrColConstraintAST *) {}
    virtual bool visit(DeferColConstraintAST *) { return true; }
    virtual void endVisit(DeferColConstraintAST *) {}
    virtual bool visit(InitiallyColConstraintAST *) { return true; }
    virtual void endVisit(InitiallyColConstraintAST *) {}
    virtual bool visit(CollateColConstraintAST *) { return true; }
    virtual void endVisit(CollateColConstraintAST *) {}
    virtual bool visit(CreateGeneticOptionsAST *) { return true; }
    virtual void endVisit(CreateGeneticOptionsAST *) {}
    virtual bool visit(GenericOptionAST *) { return true; }
    virtual void endVisit(GenericOptionAST *) {}
    virtual bool visit(TypeAST *) { return true; }
    virtual void endVisit(TypeAST *) {}
    virtual bool visit(SimpleTypenameAST *) { return true; }
    virtual void endVisit(SimpleTypenameAST *) {}
    virtual bool visit(GenericTypenameAST *) { return true; }
    virtual void endVisit(GenericTypenameAST *) {}
    virtual bool visit(IntervalTypenameAST *) { return true; }
    virtual void endVisit(IntervalTypenameAST *) {}
    virtual bool visit(NumericTypenameAST *) { return true; }
    virtual void endVisit(NumericTypenameAST *) {}
    virtual bool visit(FloatTypenameAST *) { return true; }
    virtual void endVisit(FloatTypenameAST *) {}
    virtual bool visit(DoubleTypenameAST *) { return true; }
    virtual void endVisit(DoubleTypenameAST *) {}
    virtual bool visit(DecimalTypenameAST *) { return true; }
    virtual void endVisit(DecimalTypenameAST *) {}
    virtual bool visit(FloatPrecisionAST *) { return true; }
    virtual void endVisit(FloatPrecisionAST *) {}
    virtual bool visit(BitTypenameAST *) { return true; }
    virtual void endVisit(BitTypenameAST *) {}
    virtual bool visit(CharacterTypenameAST *) { return true; }
    virtual void endVisit(CharacterTypenameAST *) {}
    virtual bool visit(DatetimeTypenameAST *) { return true; }
    virtual void endVisit(DatetimeTypenameAST *) {}
    virtual bool visit(TimeZoneSpecAST *) { return true; }
    virtual void endVisit(TimeZoneSpecAST *) {}
    virtual bool visit(CharacterTypeSpecAST *) { return true; }
    virtual void endVisit(CharacterTypeSpecAST *) {}
    virtual bool visit(CharSetAST *) { return true; }
    virtual void endVisit(CharSetAST *) {}
    virtual bool visit(TypeModifiersAST *) { return true; }
    virtual void endVisit(TypeModifiersAST *) {}
    virtual bool visit(ArrBoundAST *) { return true; }
    virtual void endVisit(ArrBoundAST *) {}
    virtual bool visit(ExpressionAST *) { return true; }
    virtual void endVisit(ExpressionAST *) {}
    virtual bool visit(PrimaryExpressionAST *) { return true; }
    virtual void endVisit(PrimaryExpressionAST *) {}
    virtual bool visit(RowPrimaryExpressionAST *) { return true; }
    virtual void endVisit(RowPrimaryExpressionAST *) {}
    virtual bool visit(ArrayPrimaryExpressionAST *) { return true; }
    virtual void endVisit(ArrayPrimaryExpressionAST *) {}
    virtual bool visit(ArrayExprAST *) { return true; }
    virtual void endVisit(ArrayExprAST *) {}
    virtual bool visit(ExprListArrayExprAST *) { return true; }
    virtual void endVisit(ExprListArrayExprAST *) {}
    virtual bool visit(ArrListArrayExprAST *) { return true; }
    virtual void endVisit(ArrListArrayExprAST *) {}
    virtual bool visit(EmptyArrayExprAST *) { return true; }
    virtual void endVisit(EmptyArrayExprAST *) {}
    virtual bool visit(SubqueryPrimaryExpressionAST *) { return true; }
    virtual void endVisit(SubqueryPrimaryExpressionAST *) {}
    virtual bool visit(FunctionPrimaryExpressionAST *) { return true; }
    virtual void endVisit(FunctionPrimaryExpressionAST *) {}
    virtual bool visit(ColRefPrimaryExpressionAST *) { return true; }
    virtual void endVisit(ColRefPrimaryExpressionAST *) {}
    virtual bool visit(ConstPrimaryExpressionAST *) { return true; }
    virtual void endVisit(ConstPrimaryExpressionAST *) {}
    virtual bool visit(ParamPrimaryExpressionAST *) { return true; }
    virtual void endVisit(ParamPrimaryExpressionAST *) {}
    virtual bool visit(CompoundPrimaryExpressionAST *) { return true; }
    virtual void endVisit(CompoundPrimaryExpressionAST *) {}
    virtual bool visit(CasePrimaryExpressionAST *) { return true; }
    virtual void endVisit(CasePrimaryExpressionAST *) {}
    virtual bool visit(CaseExprAST *) { return true; }
    virtual void endVisit(CaseExprAST *) {}
    virtual bool visit(CaseArgAST *) { return true; }
    virtual void endVisit(CaseArgAST *) {}
    virtual bool visit(CaseWhenClauseAST *) { return true; }
    virtual void endVisit(CaseWhenClauseAST *) {}
    virtual bool visit(CaseDefaultClauseAST *) { return true; }
    virtual void endVisit(CaseDefaultClauseAST *) {}
    virtual bool visit(ConstantAST *) { return true; }
    virtual void endVisit(ConstantAST *) {}
    virtual bool visit(SimpleConstantAST *) { return true; }
    virtual void endVisit(SimpleConstantAST *) {}
    virtual bool visit(FuncNameConstantAST *) { return true; }
    virtual void endVisit(FuncNameConstantAST *) {}
    virtual bool visit(IntervalConstantAST *) { return true; }
    virtual void endVisit(IntervalConstantAST *) {}
    virtual bool visit(TypenameConstantAST *) { return true; }
    virtual void endVisit(TypenameConstantAST *) {}
    virtual bool visit(TypecastExpressionAST *) { return true; }
    virtual void endVisit(TypecastExpressionAST *) {}
    virtual bool visit(CollateExpressionAST *) { return true; }
    virtual void endVisit(CollateExpressionAST *) {}
    virtual bool visit(AtTimeZoneExpressionAST *) { return true; }
    virtual void endVisit(AtTimeZoneExpressionAST *) {}
    virtual bool visit(UnaryExpressionAST *) { return true; }
    virtual void endVisit(UnaryExpressionAST *) {}
    virtual bool visit(BinaryExpressionAST *) { return true; }
    virtual void endVisit(BinaryExpressionAST *) {}
    virtual bool visit(CommonBinaryExpressionAST *) { return true; }
    virtual void endVisit(CommonBinaryExpressionAST *) {}
    virtual bool visit(CommonUnaryExpresionAST *) { return true; }
    virtual void endVisit(CommonUnaryExpresionAST *) {}
    virtual bool visit(CommonPostfixExpressionAST *) { return true; }
    virtual void endVisit(CommonPostfixExpressionAST *) {}
    virtual bool visit(LikeExpressionAST *) { return true; }
    virtual void endVisit(LikeExpressionAST *) {}
    virtual bool visit(SimilarToExpressionAST *) { return true; }
    virtual void endVisit(SimilarToExpressionAST *) {}
    virtual bool visit(IsExpressionAST *) { return true; }
    virtual void endVisit(IsExpressionAST *) {}
    virtual bool visit(IsDdistinctExpressionAST *) { return true; }
    virtual void endVisit(IsDdistinctExpressionAST *) {}
    virtual bool visit(IsTypeofExpressionAST *) { return true; }
    virtual void endVisit(IsTypeofExpressionAST *) {}
    virtual bool visit(BetweenExpressionAST *) { return true; }
    virtual void endVisit(BetweenExpressionAST *) {}
    virtual bool visit(InExpressionAST *) { return true; }
    virtual void endVisit(InExpressionAST *) {}
    virtual bool visit(InValuesAST *) { return true; }
    virtual void endVisit(InValuesAST *) {}
    virtual bool visit(InValuesAsQueryAST *) { return true; }
    virtual void endVisit(InValuesAsQueryAST *) {}
    virtual bool visit(InValuesAsExprListAST *) { return true; }
    virtual void endVisit(InValuesAsExprListAST *) {}
    virtual bool visit(RelationExprSimpleAST *) { return true; }
    virtual void endVisit(RelationExprSimpleAST *) {}
    virtual bool visit(SubqueryExpressionAST *) { return true; }
    virtual void endVisit(SubqueryExpressionAST *) {}
    virtual bool visit(SubExpressionExpressionAST *) { return true; }
    virtual void endVisit(SubExpressionExpressionAST *) {}
    virtual bool visit(RowOverlapsExpressionAST *) { return true; }
    virtual void endVisit(RowOverlapsExpressionAST *) {}
    virtual bool visit(UniqueExpressionAST *) { return true; }
    virtual void endVisit(UniqueExpressionAST *) {}
    virtual bool visit(RowAST *) { return true; }
    virtual void endVisit(RowAST *) {}
    virtual bool visit(RelationExprOnlyAST *) { return true; }
    virtual void endVisit(RelationExprOnlyAST *) {}
    virtual bool visit(SubqueryOperatorAST *) { return true; }
    virtual void endVisit(SubqueryOperatorAST *) {}
    virtual bool visit(QualSubqueryOperatorAST *) { return true; }
    virtual void endVisit(QualSubqueryOperatorAST *) {}
    virtual bool visit(LikeSubqueryOperatorAST *) { return true; }
    virtual void endVisit(LikeSubqueryOperatorAST *) {}
    virtual bool visit(QualifiedOperatorAST *) { return true; }
    virtual void endVisit(QualifiedOperatorAST *) {}
    virtual bool visit(SimpleQualifiedOperatorAST *) { return true; }
    virtual void endVisit(SimpleQualifiedOperatorAST *) {}
    virtual bool visit(CompoundQualifiedOperatorAST *) { return true; }
    virtual void endVisit(CompoundQualifiedOperatorAST *) {}
    virtual bool visit(OperandAST *) { return true; }
    virtual void endVisit(OperandAST *) {}
    virtual bool visit(AnyIdentifierAST *) { return true; }
    virtual void endVisit(AnyIdentifierAST *) {}
    virtual bool visit(TableIdentifierAST *) { return true; }
    virtual void endVisit(TableIdentifierAST *) {}
    virtual bool visit(TriggerIdentifierAST *) { return true; }
    virtual void endVisit(TriggerIdentifierAST *) {}
    virtual bool visit(IndexIdentifierAST *) { return true; }
    virtual void endVisit(IndexIdentifierAST *) {}
    virtual bool visit(ConstraintIdentifierAST *) { return true; }
    virtual void endVisit(ConstraintIdentifierAST *) {}
    virtual bool visit(SequenceIdentifierAST *) { return true; }
    virtual void endVisit(SequenceIdentifierAST *) {}
    virtual bool visit(ColumnIdentifierAST *) { return true; }
    virtual void endVisit(ColumnIdentifierAST *) {}
    virtual bool visit(ColumnRefAST *) { return true; }
    virtual void endVisit(ColumnRefAST *) {}
    virtual bool visit(FunctionRefAST *) { return true; }
    virtual void endVisit(FunctionRefAST *) {}
    virtual bool visit(IndirectionAST *) { return true; }
    virtual void endVisit(IndirectionAST *) {}
    virtual bool visit(AttirbuteIndirectionAST *) { return true; }
    virtual void endVisit(AttirbuteIndirectionAST *) {}
    virtual bool visit(StarIndirectionAST *) { return true; }
    virtual void endVisit(StarIndirectionAST *) {}
    virtual bool visit(IndexingIndirectionAST *) { return true; }
    virtual void endVisit(IndexingIndirectionAST *) {}

private:
    Sql::TranslationUnit *m_translationUnit;
};

}   // namespace Postgres

#endif // POSTGRESASTVISITOR_H
