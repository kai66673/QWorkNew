#ifndef POSTGRESASTNOVISITOR_H
#define POSTGRESASTNOVISITOR_H

#include "PostgresASTfwd.h"
#include "TranslationUnit.h"

namespace Postgres {

class ASTNoVisitor: public ASTVisitor
{
    ASTNoVisitor(const ASTNoVisitor &other);
    void operator =(const ASTNoVisitor &other);

public:
    ASTNoVisitor( Sql::TranslationUnit *translationUnit ) : ASTVisitor(translationUnit) {}

    virtual bool visit(EmptyAST *) { return false; }
    virtual void endVisit(EmptyAST *) {}
    virtual bool visit(NameAST *) { return false; }
    virtual void endVisit(NameAST *) {}
    virtual bool visit(TranslationUnitAST *) { return false; }
    virtual void endVisit(TranslationUnitAST *) {}
    virtual bool visit(DdlStatementAST *) { return false; }
    virtual void endVisit(DdlStatementAST *) {}
    virtual bool visit(DmlStatementAST *) { return false; }
    virtual void endVisit(DmlStatementAST *) {}
    virtual bool visit(UnknownStatementAST *) { return false; }
    virtual void endVisit(UnknownStatementAST *) {}
    virtual bool visit(CreateStatementAST *) { return false; }
    virtual void endVisit(CreateStatementAST *) {}
    virtual bool visit(CreateViewStmtAST *) { return false; }
    virtual void endVisit(CreateViewStmtAST *) {}
    virtual bool visit(WithCheckOptionClauseAST *) { return false; }
    virtual void endVisit(WithCheckOptionClauseAST *) {}
    virtual bool visit(VacuumStmtAST *) { return false; }
    virtual void endVisit(VacuumStmtAST *) {}
    virtual bool visit(CommonVacuumStmtAST *) { return false; }
    virtual void endVisit(CommonVacuumStmtAST *) {}
    virtual bool visit(VacuumOptionAST *) { return false; }
    virtual void endVisit(VacuumOptionAST *) {}
    virtual bool visit(VacuumTableStmtAST *) { return false; }
    virtual void endVisit(VacuumTableStmtAST *) {}
    virtual bool visit(VacuumAnalyzeStmtAST *) { return false; }
    virtual void endVisit(VacuumAnalyzeStmtAST *) {}
    virtual bool visit(TruncateStmtAST *) { return false; }
    virtual void endVisit(TruncateStmtAST *) {}
    virtual bool visit(TruncateRestartClauseAST *) { return false; }
    virtual void endVisit(TruncateRestartClauseAST *) {}
    virtual bool visit(TransactionStmtAST *) { return false; }
    virtual void endVisit(TransactionStmtAST *) {}
    virtual bool visit(BeginTransactionStmtAST *) { return false; }
    virtual void endVisit(BeginTransactionStmtAST *) {}
    virtual bool visit(SavepointTransactionStmtAST *) { return false; }
    virtual void endVisit(SavepointTransactionStmtAST *) {}
    virtual bool visit(ReleaseTransactionStmtAST *) { return false; }
    virtual void endVisit(ReleaseTransactionStmtAST *) {}
    virtual bool visit(CommitTransactionStmtAST *) { return false; }
    virtual void endVisit(CommitTransactionStmtAST *) {}
    virtual bool visit(RollbackTransactionStmtAST *) { return false; }
    virtual void endVisit(RollbackTransactionStmtAST *) {}
    virtual bool visit(PrepareTransactionStmtAST *) { return false; }
    virtual void endVisit(PrepareTransactionStmtAST *) {}
    virtual bool visit(CommitPrepTransactionStmtAST *) { return false; }
    virtual void endVisit(CommitPrepTransactionStmtAST *) {}
    virtual bool visit(RollPrepTransactionStmtAST *) { return false; }
    virtual void endVisit(RollPrepTransactionStmtAST *) {}
    virtual bool visit(SecLabelStmtAST *) { return false; }
    virtual void endVisit(SecLabelStmtAST *) {}
    virtual bool visit(ColSecLabelStmtAST *) { return false; }
    virtual void endVisit(ColSecLabelStmtAST *) {}
    virtual bool visit(SequenceSecLabelStmtAST *) { return false; }
    virtual void endVisit(SequenceSecLabelStmtAST *) {}
    virtual bool visit(TableSecLabelStmtAST *) { return false; }
    virtual void endVisit(TableSecLabelStmtAST *) {}
    virtual bool visit(AggrSecLabelStmtAST *) { return false; }
    virtual void endVisit(AggrSecLabelStmtAST *) {}
    virtual bool visit(FuncSecLabelStmtAST *) { return false; }
    virtual void endVisit(FuncSecLabelStmtAST *) {}
    virtual bool visit(LargeObjSecLabelStmtAST *) { return false; }
    virtual void endVisit(LargeObjSecLabelStmtAST *) {}
    virtual bool visit(LangSecLabelStmtAST *) { return false; }
    virtual void endVisit(LangSecLabelStmtAST *) {}
    virtual bool visit(SchemaSecLabelStmtAST *) { return false; }
    virtual void endVisit(SchemaSecLabelStmtAST *) {}
    virtual bool visit(RoleSecLabelStmtAST *) { return false; }
    virtual void endVisit(RoleSecLabelStmtAST *) {}
    virtual bool visit(CommonSecLabelStmtAST *) { return false; }
    virtual void endVisit(CommonSecLabelStmtAST *) {}
    virtual bool visit(SecurityProviderAST *) { return false; }
    virtual void endVisit(SecurityProviderAST *) {}
    virtual bool visit(CommonSecurityTypeAST *) { return false; }
    virtual void endVisit(CommonSecurityTypeAST *) {}
    virtual bool visit(RenameStmtAST *) { return false; }
    virtual void endVisit(RenameStmtAST *) {}
    virtual bool visit(RenameTypeStmtAST *) { return false; }
    virtual void endVisit(RenameTypeStmtAST *) {}
    virtual bool visit(RenameTypeAttrStmtAST *) { return false; }
    virtual void endVisit(RenameTypeAttrStmtAST *) {}
    virtual bool visit(RenameRuleStmtAST *) { return false; }
    virtual void endVisit(RenameRuleStmtAST *) {}
    virtual bool visit(RenameTriggerStmtAST *) { return false; }
    virtual void endVisit(RenameTriggerStmtAST *) {}
    virtual bool visit(RenameSchemaStmtAST *) { return false; }
    virtual void endVisit(RenameSchemaStmtAST *) {}
    virtual bool visit(RenameServerStmtAST *) { return false; }
    virtual void endVisit(RenameServerStmtAST *) {}
    virtual bool visit(RenameEvtTrgStmtAST *) { return false; }
    virtual void endVisit(RenameEvtTrgStmtAST *) {}
    virtual bool visit(RenameRoleStmtAST *) { return false; }
    virtual void endVisit(RenameRoleStmtAST *) {}
    virtual bool visit(RenameTablespaceStmtAST *) { return false; }
    virtual void endVisit(RenameTablespaceStmtAST *) {}
    virtual bool visit(ChgReloptsTablespaceStmtAST *) { return false; }
    virtual void endVisit(ChgReloptsTablespaceStmtAST *) {}
    virtual bool visit(RenameSearchStmtAST *) { return false; }
    virtual void endVisit(RenameSearchStmtAST *) {}
    virtual bool visit(RenameTableStmtAST *) { return false; }
    virtual void endVisit(RenameTableStmtAST *) {}
    virtual bool visit(RenameTableColStmtAST *) { return false; }
    virtual void endVisit(RenameTableColStmtAST *) {}
    virtual bool visit(RenameTableConstrStmtAST *) { return false; }
    virtual void endVisit(RenameTableConstrStmtAST *) {}
    virtual bool visit(RenameMatViewColStmtAST *) { return false; }
    virtual void endVisit(RenameMatViewColStmtAST *) {}
    virtual bool visit(RenameSequenceStmtAST *) { return false; }
    virtual void endVisit(RenameSequenceStmtAST *) {}
    virtual bool visit(RenameViewStmtAST *) { return false; }
    virtual void endVisit(RenameViewStmtAST *) {}
    virtual bool visit(RenameIndexStmtAST *) { return false; }
    virtual void endVisit(RenameIndexStmtAST *) {}
    virtual bool visit(RenameCollationStmtAST *) { return false; }
    virtual void endVisit(RenameCollationStmtAST *) {}
    virtual bool visit(RenameConversionStmtAST *) { return false; }
    virtual void endVisit(RenameConversionStmtAST *) {}
    virtual bool visit(RenameDomainStmtAST *) { return false; }
    virtual void endVisit(RenameDomainStmtAST *) {}
    virtual bool visit(RenameDomainConstrStmtAST *) { return false; }
    virtual void endVisit(RenameDomainConstrStmtAST *) {}
    virtual bool visit(RenameDatabaseStmtAST *) { return false; }
    virtual void endVisit(RenameDatabaseStmtAST *) {}
    virtual bool visit(RenameFdwStmtAST *) { return false; }
    virtual void endVisit(RenameFdwStmtAST *) {}
    virtual bool visit(RenameFuncStmtAST *) { return false; }
    virtual void endVisit(RenameFuncStmtAST *) {}
    virtual bool visit(RenameOpUsingStmtAST *) { return false; }
    virtual void endVisit(RenameOpUsingStmtAST *) {}
    virtual bool visit(RenameGroupRoleStmtAST *) { return false; }
    virtual void endVisit(RenameGroupRoleStmtAST *) {}
    virtual bool visit(RenameLangStmtAST *) { return false; }
    virtual void endVisit(RenameLangStmtAST *) {}
    virtual bool visit(RenameAggrStmtAST *) { return false; }
    virtual void endVisit(RenameAggrStmtAST *) {}
    virtual bool visit(ReindexStmtAST *) { return false; }
    virtual void endVisit(ReindexStmtAST *) {}
    virtual bool visit(ReindexTableStmtAST *) { return false; }
    virtual void endVisit(ReindexTableStmtAST *) {}
    virtual bool visit(ReindexIndexStmtAST *) { return false; }
    virtual void endVisit(ReindexIndexStmtAST *) {}
    virtual bool visit(ReindexSystemStmtAST *) { return false; }
    virtual void endVisit(ReindexSystemStmtAST *) {}
    virtual bool visit(ReindexDatabaseStmtAST *) { return false; }
    virtual void endVisit(ReindexDatabaseStmtAST *) {}
    virtual bool visit(PrepareStmtAST *) { return false; }
    virtual void endVisit(PrepareStmtAST *) {}
    virtual bool visit(PrepareTypeClauseAST *) { return false; }
    virtual void endVisit(PrepareTypeClauseAST *) {}
    virtual bool visit(LockStmtAST *) { return false; }
    virtual void endVisit(LockStmtAST *) {}
    virtual bool visit(LockModeClauseAST *) { return false; }
    virtual void endVisit(LockModeClauseAST *) {}
    virtual bool visit(LockTypeAST *) { return false; }
    virtual void endVisit(LockTypeAST *) {}
    virtual bool visit(DropStmtAST *) { return false; }
    virtual void endVisit(DropStmtAST *) {}
    virtual bool visit(DropAggrStmtAST *) { return false; }
    virtual void endVisit(DropAggrStmtAST *) {}
    virtual bool visit(DropFuncStmtAST *) { return false; }
    virtual void endVisit(DropFuncStmtAST *) {}
    virtual bool visit(DropOperStmtAST *) { return false; }
    virtual void endVisit(DropOperStmtAST *) {}
    virtual bool visit(LoadStmtAST *) { return false; }
    virtual void endVisit(LoadStmtAST *) {}
    virtual bool visit(NotifyStmtAST *) { return false; }
    virtual void endVisit(NotifyStmtAST *) {}
    virtual bool visit(NotifyPayloadAST *) { return false; }
    virtual void endVisit(NotifyPayloadAST *) {}
    virtual bool visit(ListenStmtAST *) { return false; }
    virtual void endVisit(ListenStmtAST *) {}
    virtual bool visit(UnlistenStmtAST *) { return false; }
    virtual void endVisit(UnlistenStmtAST *) {}
    virtual bool visit(CreateIndexStatementAST *) { return false; }
    virtual void endVisit(CreateIndexStatementAST *) {}
    virtual bool visit(GrantRoleStmtAST *) { return false; }
    virtual void endVisit(GrantRoleStmtAST *) {}
    virtual bool visit(RevokeRoleStmtAST *) { return false; }
    virtual void endVisit(RevokeRoleStmtAST *) {}
    virtual bool visit(WithAdminOptionAST *) { return false; }
    virtual void endVisit(WithAdminOptionAST *) {}
    virtual bool visit(GrantedByClauseAST *) { return false; }
    virtual void endVisit(GrantedByClauseAST *) {}
    virtual bool visit(GrantStmtAST *) { return false; }
    virtual void endVisit(GrantStmtAST *) {}
    virtual bool visit(RevokeStmtAST *) { return false; }
    virtual void endVisit(RevokeStmtAST *) {}
    virtual bool visit(PrivilegeTargetAST *) { return false; }
    virtual void endVisit(PrivilegeTargetAST *) {}
    virtual bool visit(TablePrivilegeTargetAST *) { return false; }
    virtual void endVisit(TablePrivilegeTargetAST *) {}
    virtual bool visit(SequencePrivilegeTargetAST *) { return false; }
    virtual void endVisit(SequencePrivilegeTargetAST *) {}
    virtual bool visit(FdwPrivilegeTargetAST *) { return false; }
    virtual void endVisit(FdwPrivilegeTargetAST *) {}
    virtual bool visit(ForeignSrvPrivilegeTargetAST *) { return false; }
    virtual void endVisit(ForeignSrvPrivilegeTargetAST *) {}
    virtual bool visit(FuncPrivilegeTargetAST *) { return false; }
    virtual void endVisit(FuncPrivilegeTargetAST *) {}
    virtual bool visit(DbPrivilegeTargetAST *) { return false; }
    virtual void endVisit(DbPrivilegeTargetAST *) {}
    virtual bool visit(DomainPrivilegeTargetAST *) { return false; }
    virtual void endVisit(DomainPrivilegeTargetAST *) {}
    virtual bool visit(LangPrivilegeTargetAST *) { return false; }
    virtual void endVisit(LangPrivilegeTargetAST *) {}
    virtual bool visit(LargeObjPrivilegeTargetAST *) { return false; }
    virtual void endVisit(LargeObjPrivilegeTargetAST *) {}
    virtual bool visit(SchemaPrivilegeTargetAST *) { return false; }
    virtual void endVisit(SchemaPrivilegeTargetAST *) {}
    virtual bool visit(TblspacePrivilegeTargetAST *) { return false; }
    virtual void endVisit(TblspacePrivilegeTargetAST *) {}
    virtual bool visit(TypePrivilegeTargetAST *) { return false; }
    virtual void endVisit(TypePrivilegeTargetAST *) {}
    virtual bool visit(AllInSchPrivilegeTargetAST *) { return false; }
    virtual void endVisit(AllInSchPrivilegeTargetAST *) {}
    virtual bool visit(NumericAST *) { return false; }
    virtual void endVisit(NumericAST *) {}
    virtual bool visit(FetchStmtAST *) { return false; }
    virtual void endVisit(FetchStmtAST *) {}
    virtual bool visit(FetchClauseAST *) { return false; }
    virtual void endVisit(FetchClauseAST *) {}
    virtual bool visit(BaseExplainStmtAST *) { return false; }
    virtual void endVisit(BaseExplainStmtAST *) {}
    virtual bool visit(CommonExplainStmtAST *) { return false; }
    virtual void endVisit(CommonExplainStmtAST *) {}
    virtual bool visit(OptExplainStmtAST *) { return false; }
    virtual void endVisit(OptExplainStmtAST *) {}
    virtual bool visit(ExplainOptionAST *) { return false; }
    virtual void endVisit(ExplainOptionAST *) {}
    virtual bool visit(RefreshMatViewStmtAST *) { return false; }
    virtual void endVisit(RefreshMatViewStmtAST *) {}
    virtual bool visit(DropDbStmtAST *) { return false; }
    virtual void endVisit(DropDbStmtAST *) {}
    virtual bool visit(DropUserMappingStmtAST *) { return false; }
    virtual void endVisit(DropUserMappingStmtAST *) {}
    virtual bool visit(DropTableSpaceStmtAST *) { return false; }
    virtual void endVisit(DropTableSpaceStmtAST *) {}
    virtual bool visit(CommonDropStmtAST *) { return false; }
    virtual void endVisit(CommonDropStmtAST *) {}
    virtual bool visit(CommonDropTypeAST *) { return false; }
    virtual void endVisit(CommonDropTypeAST *) {}
    virtual bool visit(DropTableStmtAST *) { return false; }
    virtual void endVisit(DropTableStmtAST *) {}
    virtual bool visit(DropViewStmtAST *) { return false; }
    virtual void endVisit(DropViewStmtAST *) {}
    virtual bool visit(DropIndexStmtAST *) { return false; }
    virtual void endVisit(DropIndexStmtAST *) {}
    virtual bool visit(DropSchemaStmtAST *) { return false; }
    virtual void endVisit(DropSchemaStmtAST *) {}
    virtual bool visit(DropSequenceStmtAST *) { return false; }
    virtual void endVisit(DropSequenceStmtAST *) {}
    virtual bool visit(RuleStmtAST *) { return false; }
    virtual void endVisit(RuleStmtAST *) {}
    virtual bool visit(RuleActionsAST *) { return false; }
    virtual void endVisit(RuleActionsAST *) {}
    virtual bool visit(NothingRuleActionsAST *) { return false; }
    virtual void endVisit(NothingRuleActionsAST *) {}
    virtual bool visit(SingleRuleActionsAST *) { return false; }
    virtual void endVisit(SingleRuleActionsAST *) {}
    virtual bool visit(MultiRuleActionsAST *) { return false; }
    virtual void endVisit(MultiRuleActionsAST *) {}
    virtual bool visit(DropRuleStmtAST *) { return false; }
    virtual void endVisit(DropRuleStmtAST *) {}
    virtual bool visit(DropOwnedStmtAST *) { return false; }
    virtual void endVisit(DropOwnedStmtAST *) {}
    virtual bool visit(ReassignOwnedStmtAST *) { return false; }
    virtual void endVisit(ReassignOwnedStmtAST *) {}
    virtual bool visit(DropGroupStmtAST *) { return false; }
    virtual void endVisit(DropGroupStmtAST *) {}
    virtual bool visit(DropForeignServerStmtAST *) { return false; }
    virtual void endVisit(DropForeignServerStmtAST *) {}
    virtual bool visit(DropFdwStmtAST *) { return false; }
    virtual void endVisit(DropFdwStmtAST *) {}
    virtual bool visit(DoStmtAST *) { return false; }
    virtual void endVisit(DoStmtAST *) {}
    virtual bool visit(DoStmtItemAST *) { return false; }
    virtual void endVisit(DoStmtItemAST *) {}
    virtual bool visit(DoStmtCmdItemAST *) { return false; }
    virtual void endVisit(DoStmtCmdItemAST *) {}
    virtual bool visit(DoStmtLangItemAST *) { return false; }
    virtual void endVisit(DoStmtLangItemAST *) {}
    virtual bool visit(CreateOldAggregateStmtAST *) { return false; }
    virtual void endVisit(CreateOldAggregateStmtAST *) {}
    virtual bool visit(AggregateDefinitionAST *) { return false; }
    virtual void endVisit(AggregateDefinitionAST *) {}
    virtual bool visit(AggrDefinitionAST *) { return false; }
    virtual void endVisit(AggrDefinitionAST *) {}
    virtual bool visit(CreateAggregateStmtAST *) { return false; }
    virtual void endVisit(CreateAggregateStmtAST *) {}
    virtual bool visit(CreateOpStmtAST *) { return false; }
    virtual void endVisit(CreateOpStmtAST *) {}
    virtual bool visit(CreateTypeStmtAST *) { return false; }
    virtual void endVisit(CreateTypeStmtAST *) {}
    virtual bool visit(CreateTypeAsRecordStmtAST *) { return false; }
    virtual void endVisit(CreateTypeAsRecordStmtAST *) {}
    virtual bool visit(CreateTypeAsEnumStmtAST *) { return false; }
    virtual void endVisit(CreateTypeAsEnumStmtAST *) {}
    virtual bool visit(EnumValueAST *) { return false; }
    virtual void endVisit(EnumValueAST *) {}
    virtual bool visit(CreateTypeAsDefStmtAST *) { return false; }
    virtual void endVisit(CreateTypeAsDefStmtAST *) {}
    virtual bool visit(CreateTypeAsRangeStmtAST *) { return false; }
    virtual void endVisit(CreateTypeAsRangeStmtAST *) {}
    virtual bool visit(CreateSearchObjStmtAST *) { return false; }
    virtual void endVisit(CreateSearchObjStmtAST *) {}
    virtual bool visit(CreateCollationStmtAST *) { return false; }
    virtual void endVisit(CreateCollationStmtAST *) {}
    virtual bool visit(CreateCollationDefStmtAST *) { return false; }
    virtual void endVisit(CreateCollationDefStmtAST *) {}
    virtual bool visit(CreateCollationFromStmtAST *) { return false; }
    virtual void endVisit(CreateCollationFromStmtAST *) {}
    virtual bool visit(DeclareCursorStmtAST *) { return false; }
    virtual void endVisit(DeclareCursorStmtAST *) {}
    virtual bool visit(CursorHoldOptionAST *) { return false; }
    virtual void endVisit(CursorHoldOptionAST *) {}
    virtual bool visit(DeallocateStmtAST *) { return false; }
    virtual void endVisit(DeallocateStmtAST *) {}
    virtual bool visit(CreateDbStmtAST *) { return false; }
    virtual void endVisit(CreateDbStmtAST *) {}
    virtual bool visit(CreateDbOptionAST *) { return false; }
    virtual void endVisit(CreateDbOptionAST *) {}
    virtual bool visit(NamedCreateDbOptionAST *) { return false; }
    virtual void endVisit(NamedCreateDbOptionAST *) {}
    virtual bool visit(ValDefCreateDbOptionAST *) { return false; }
    virtual void endVisit(ValDefCreateDbOptionAST *) {}
    virtual bool visit(CreateUserMappingStmtAST *) { return false; }
    virtual void endVisit(CreateUserMappingStmtAST *) {}
    virtual bool visit(DropTrigStmtAST *) { return false; }
    virtual void endVisit(DropTrigStmtAST *) {}
    virtual bool visit(CreateConstrTrigStmtAST *) { return false; }
    virtual void endVisit(CreateConstrTrigStmtAST *) {}
    virtual bool visit(ConstrFromTableAST *) { return false; }
    virtual void endVisit(ConstrFromTableAST *) {}
    virtual bool visit(CreateTrigStmtAST *) { return false; }
    virtual void endVisit(CreateTrigStmtAST *) {}
    virtual bool visit(TriggerExecuteClauseAST *) { return false; }
    virtual void endVisit(TriggerExecuteClauseAST *) {}
    virtual bool visit(TriggerFuncArgAST *) { return false; }
    virtual void endVisit(TriggerFuncArgAST *) {}
    virtual bool visit(TriggerWhenAST *) { return false; }
    virtual void endVisit(TriggerWhenAST *) {}
    virtual bool visit(TriggerForSpecAST *) { return false; }
    virtual void endVisit(TriggerForSpecAST *) {}
    virtual bool visit(TriggerEventAST *) { return false; }
    virtual void endVisit(TriggerEventAST *) {}
    virtual bool visit(TriggerActionTimeAST *) { return false; }
    virtual void endVisit(TriggerActionTimeAST *) {}
    virtual bool visit(CreateTableSpaceStmtAST *) { return false; }
    virtual void endVisit(CreateTableSpaceStmtAST *) {}
    virtual bool visit(TableSpaceOwnerClauseAST *) { return false; }
    virtual void endVisit(TableSpaceOwnerClauseAST *) {}
    virtual bool visit(CreateTableAsExecStmtAST *) { return false; }
    virtual void endVisit(CreateTableAsExecStmtAST *) {}
    virtual bool visit(ExecuteStmtAST *) { return false; }
    virtual void endVisit(ExecuteStmtAST *) {}
    virtual bool visit(ExecuteParamClauseAST *) { return false; }
    virtual void endVisit(ExecuteParamClauseAST *) {}
    virtual bool visit(CreateTableStmtAST *) { return false; }
    virtual void endVisit(CreateTableStmtAST *) {}
    virtual bool visit(CreateTableOptionsAST *) { return false; }
    virtual void endVisit(CreateTableOptionsAST *) {}
    virtual bool visit(CreateTableClauseAST *) { return false; }
    virtual void endVisit(CreateTableClauseAST *) {}
    virtual bool visit(CreateTableDirecClauseAST *) { return false; }
    virtual void endVisit(CreateTableDirecClauseAST *) {}
    virtual bool visit(CreateTableOfClauseAST *) { return false; }
    virtual void endVisit(CreateTableOfClauseAST *) {}
    virtual bool visit(TypedTableElementsAST *) { return false; }
    virtual void endVisit(TypedTableElementsAST *) {}
    virtual bool visit(TypedTableElementAST *) { return false; }
    virtual void endVisit(TypedTableElementAST *) {}
    virtual bool visit(TypedTableConstrElementAST *) { return false; }
    virtual void endVisit(TypedTableConstrElementAST *) {}
    virtual bool visit(TypedTableColElementAST *) { return false; }
    virtual void endVisit(TypedTableColElementAST *) {}
    virtual bool visit(TableInheritClauseAST *) { return false; }
    virtual void endVisit(TableInheritClauseAST *) {}
    virtual bool visit(CreateSchemaStmtAST *) { return false; }
    virtual void endVisit(CreateSchemaStmtAST *) {}
    virtual bool visit(CreateSchemaClauseAST *) { return false; }
    virtual void endVisit(CreateSchemaClauseAST *) {}
    virtual bool visit(DropPLangStmtAST *) { return false; }
    virtual void endVisit(DropPLangStmtAST *) {}
    virtual bool visit(CreatePLangStmtAST *) { return false; }
    virtual void endVisit(CreatePLangStmtAST *) {}
    virtual bool visit(InlineHandlerClauseAST *) { return false; }
    virtual void endVisit(InlineHandlerClauseAST *) {}
    virtual bool visit(ValidatorHandlerClauseAST *) { return false; }
    virtual void endVisit(ValidatorHandlerClauseAST *) {}
    virtual bool visit(DropOpClassStmtAST *) { return false; }
    virtual void endVisit(DropOpClassStmtAST *) {}
    virtual bool visit(DropOpFamilyStmtAST *) { return false; }
    virtual void endVisit(DropOpFamilyStmtAST *) {}
    virtual bool visit(AlterOpFamilyAddStmtAST *) { return false; }
    virtual void endVisit(AlterOpFamilyAddStmtAST *) {}
    virtual bool visit(AlterOpFamilyDropStmtAST *) { return false; }
    virtual void endVisit(AlterOpFamilyDropStmtAST *) {}
    virtual bool visit(OpClassDropAST *) { return false; }
    virtual void endVisit(OpClassDropAST *) {}
    virtual bool visit(CreateOpFamilyStmtAST *) { return false; }
    virtual void endVisit(CreateOpFamilyStmtAST *) {}
    virtual bool visit(CreateOpClassStmtAST *) { return false; }
    virtual void endVisit(CreateOpClassStmtAST *) {}
    virtual bool visit(OpClassAST *) { return false; }
    virtual void endVisit(OpClassAST *) {}
    virtual bool visit(StorageOpClassAST *) { return false; }
    virtual void endVisit(StorageOpClassAST *) {}
    virtual bool visit(OperatorOpClassAST *) { return false; }
    virtual void endVisit(OperatorOpClassAST *) {}
    virtual bool visit(OpClassPurposeAST *) { return false; }
    virtual void endVisit(OpClassPurposeAST *) {}
    virtual bool visit(OpClassSearchPurposeAST *) { return false; }
    virtual void endVisit(OpClassSearchPurposeAST *) {}
    virtual bool visit(OpClassOrderPurposeAST *) { return false; }
    virtual void endVisit(OpClassOrderPurposeAST *) {}
    virtual bool visit(FunctionOpClassAST *) { return false; }
    virtual void endVisit(FunctionOpClassAST *) {}
    virtual bool visit(OpFamilyAST *) { return false; }
    virtual void endVisit(OpFamilyAST *) {}
    virtual bool visit(CreateMatViewStmtAST *) { return false; }
    virtual void endVisit(CreateMatViewStmtAST *) {}
    virtual bool visit(CreateMvTargetAST *) { return false; }
    virtual void endVisit(CreateMvTargetAST *) {}
    virtual bool visit(WithReloptionsAST *) { return false; }
    virtual void endVisit(WithReloptionsAST *) {}
    virtual bool visit(CreateFunctionStmtAST *) { return false; }
    virtual void endVisit(CreateFunctionStmtAST *) {}
    virtual bool visit(BaseFuncReturnAST *) { return false; }
    virtual void endVisit(BaseFuncReturnAST *) {}
    virtual bool visit(FuncTypeFuncReturnAST *) { return false; }
    virtual void endVisit(FuncTypeFuncReturnAST *) {}
    virtual bool visit(TableFuncReturnAST *) { return false; }
    virtual void endVisit(TableFuncReturnAST *) {}
    virtual bool visit(TableFuncColumnAST *) { return false; }
    virtual void endVisit(TableFuncColumnAST *) {}
    virtual bool visit(CreateFuncOptionAST *) { return false; }
    virtual void endVisit(CreateFuncOptionAST *) {}
    virtual bool visit(CreateFuncAsOptionAST *) { return false; }
    virtual void endVisit(CreateFuncAsOptionAST *) {}
    virtual bool visit(CreateFuncLangOptionAST *) { return false; }
    virtual void endVisit(CreateFuncLangOptionAST *) {}
    virtual bool visit(CreateFuncWindowOptionAST *) { return false; }
    virtual void endVisit(CreateFuncWindowOptionAST *) {}
    virtual bool visit(CreateFuncCommonOptionAST *) { return false; }
    virtual void endVisit(CreateFuncCommonOptionAST *) {}
    virtual bool visit(FunctionAsClauseAST *) { return false; }
    virtual void endVisit(FunctionAsClauseAST *) {}
    virtual bool visit(FuncArgsWithDefaultsAST *) { return false; }
    virtual void endVisit(FuncArgsWithDefaultsAST *) {}
    virtual bool visit(FuncArgWithDefaultAST *) { return false; }
    virtual void endVisit(FuncArgWithDefaultAST *) {}
    virtual bool visit(OrReplaceClauseAST *) { return false; }
    virtual void endVisit(OrReplaceClauseAST *) {}
    virtual bool visit(CreateForeignTableStmtAST *) { return false; }
    virtual void endVisit(CreateForeignTableStmtAST *) {}
    virtual bool visit(TableElementAST *) { return false; }
    virtual void endVisit(TableElementAST *) {}
    virtual bool visit(ColumnDefTableElementAST *) { return false; }
    virtual void endVisit(ColumnDefTableElementAST *) {}
    virtual bool visit(LikeTableElementAST *) { return false; }
    virtual void endVisit(LikeTableElementAST *) {}
    virtual bool visit(TableLikeClauseAST *) { return false; }
    virtual void endVisit(TableLikeClauseAST *) {}
    virtual bool visit(TableLikeOptionAST *) { return false; }
    virtual void endVisit(TableLikeOptionAST *) {}
    virtual bool visit(ConstraintTableElementAST *) { return false; }
    virtual void endVisit(ConstraintTableElementAST *) {}
    virtual bool visit(CreateForeignServerStmtAST *) { return false; }
    virtual void endVisit(CreateForeignServerStmtAST *) {}
    virtual bool visit(ServerTypeClauseAST *) { return false; }
    virtual void endVisit(ServerTypeClauseAST *) {}
    virtual bool visit(CreateFdwStmtAST *) { return false; }
    virtual void endVisit(CreateFdwStmtAST *) {}
    virtual bool visit(CreateExtensionStmtAST *) { return false; }
    virtual void endVisit(CreateExtensionStmtAST *) {}
    virtual bool visit(CreateExtOptionAST *) { return false; }
    virtual void endVisit(CreateExtOptionAST *) {}
    virtual bool visit(CreateExtSchemaOptionAST *) { return false; }
    virtual void endVisit(CreateExtSchemaOptionAST *) {}
    virtual bool visit(CreateExtVerFromOptionAST *) { return false; }
    virtual void endVisit(CreateExtVerFromOptionAST *) {}
    virtual bool visit(CreateConversionStmtAST *) { return false; }
    virtual void endVisit(CreateConversionStmtAST *) {}
    virtual bool visit(CreateCastStmtAST *) { return false; }
    virtual void endVisit(CreateCastStmtAST *) {}
    virtual bool visit(CastContextAST *) { return false; }
    virtual void endVisit(CastContextAST *) {}
    virtual bool visit(DropCastStmtAST *) { return false; }
    virtual void endVisit(DropCastStmtAST *) {}
    virtual bool visit(IfExistsAST *) { return false; }
    virtual void endVisit(IfExistsAST *) {}
    virtual bool visit(DropAssertStmtAST *) { return false; }
    virtual void endVisit(DropAssertStmtAST *) {}
    virtual bool visit(CreateAssertStmtAST *) { return false; }
    virtual void endVisit(CreateAssertStmtAST *) {}
    virtual bool visit(CreateTableAsStmtAST *) { return false; }
    virtual void endVisit(CreateTableAsStmtAST *) {}
    virtual bool visit(CreateAsTargetAST *) { return false; }
    virtual void endVisit(CreateAsTargetAST *) {}
    virtual bool visit(TableSpaceClauseAST *) { return false; }
    virtual void endVisit(TableSpaceClauseAST *) {}
    virtual bool visit(OnCommitOptionAST *) { return false; }
    virtual void endVisit(OnCommitOptionAST *) {}
    virtual bool visit(CreateWithOptionsAST *) { return false; }
    virtual void endVisit(CreateWithOptionsAST *) {}
    virtual bool visit(CreateWithRelOptionsAST *) { return false; }
    virtual void endVisit(CreateWithRelOptionsAST *) {}
    virtual bool visit(CreateWithOidsOptionsAST *) { return false; }
    virtual void endVisit(CreateWithOidsOptionsAST *) {}
    virtual bool visit(WithDataClauseAST *) { return false; }
    virtual void endVisit(WithDataClauseAST *) {}
    virtual bool visit(CopyStmtAST *) { return false; }
    virtual void endVisit(CopyStmtAST *) {}
    virtual bool visit(TableCopyStmtAST *) { return false; }
    virtual void endVisit(TableCopyStmtAST *) {}
    virtual bool visit(WithOidsClauseAST *) { return false; }
    virtual void endVisit(WithOidsClauseAST *) {}
    virtual bool visit(CopyDelimiterAST *) { return false; }
    virtual void endVisit(CopyDelimiterAST *) {}
    virtual bool visit(SelectCopyStmtAST *) { return false; }
    virtual void endVisit(SelectCopyStmtAST *) {}
    virtual bool visit(CopyOptionsAST *) { return false; }
    virtual void endVisit(CopyOptionsAST *) {}
    virtual bool visit(NewCopyOptionsAST *) { return false; }
    virtual void endVisit(NewCopyOptionsAST *) {}
    virtual bool visit(GenericCopyOptionAST *) { return false; }
    virtual void endVisit(GenericCopyOptionAST *) {}
    virtual bool visit(GenericCopyOptionArgAST *) { return false; }
    virtual void endVisit(GenericCopyOptionArgAST *) {}
    virtual bool visit(GenericCopyOptionValArgAST *) { return false; }
    virtual void endVisit(GenericCopyOptionValArgAST *) {}
    virtual bool visit(GenericCopyOptionCmpArgAST *) { return false; }
    virtual void endVisit(GenericCopyOptionCmpArgAST *) {}
    virtual bool visit(GenericCopyOptionCmpArgItemAST *) { return false; }
    virtual void endVisit(GenericCopyOptionCmpArgItemAST *) {}
    virtual bool visit(OldCopyOptionsAST *) { return false; }
    virtual void endVisit(OldCopyOptionsAST *) {}
    virtual bool visit(CopyOptionAST *) { return false; }
    virtual void endVisit(CopyOptionAST *) {}
    virtual bool visit(ConstraintsSetStmtAST *) { return false; }
    virtual void endVisit(ConstraintsSetStmtAST *) {}
    virtual bool visit(ConstraintsSetListAST *) { return false; }
    virtual void endVisit(ConstraintsSetListAST *) {}
    virtual bool visit(AllConstraintsSetListAST *) { return false; }
    virtual void endVisit(AllConstraintsSetListAST *) {}
    virtual bool visit(CommonConstraintsSetListAST *) { return false; }
    virtual void endVisit(CommonConstraintsSetListAST *) {}
    virtual bool visit(CommentStmtAST *) { return false; }
    virtual void endVisit(CommentStmtAST *) {}
    virtual bool visit(LangCommentStmtAST *) { return false; }
    virtual void endVisit(LangCommentStmtAST *) {}
    virtual bool visit(CastCommentStmtAST *) { return false; }
    virtual void endVisit(CastCommentStmtAST *) {}
    virtual bool visit(LargeObjCommentStmtAST *) { return false; }
    virtual void endVisit(LargeObjCommentStmtAST *) {}
    virtual bool visit(OpUsingCommentStmtAST *) { return false; }
    virtual void endVisit(OpUsingCommentStmtAST *) {}
    virtual bool visit(ConstraintCommentStmtAST *) { return false; }
    virtual void endVisit(ConstraintCommentStmtAST *) {}
    virtual bool visit(RuleCommentStmtAST *) { return false; }
    virtual void endVisit(RuleCommentStmtAST *) {}
    virtual bool visit(TriggerCommentStmtAST *) { return false; }
    virtual void endVisit(TriggerCommentStmtAST *) {}
    virtual bool visit(CommentOnNameAST *) { return false; }
    virtual void endVisit(CommentOnNameAST *) {}
    virtual bool visit(OpCommentStmtAST *) { return false; }
    virtual void endVisit(OpCommentStmtAST *) {}
    virtual bool visit(FunctionCommentStmtAST *) { return false; }
    virtual void endVisit(FunctionCommentStmtAST *) {}
    virtual bool visit(AggrCommentStmtAST *) { return false; }
    virtual void endVisit(AggrCommentStmtAST *) {}
    virtual bool visit(CommonCommentStmtAST *) { return false; }
    virtual void endVisit(CommonCommentStmtAST *) {}
    virtual bool visit(CommentObjectAST *) { return false; }
    virtual void endVisit(CommentObjectAST *) {}
    virtual bool visit(SearchCommentObjectAST *) { return false; }
    virtual void endVisit(SearchCommentObjectAST *) {}
    virtual bool visit(EvtTriggerCommentObjectAST *) { return false; }
    virtual void endVisit(EvtTriggerCommentObjectAST *) {}
    virtual bool visit(FdwCommentObjectAST *) { return false; }
    virtual void endVisit(FdwCommentObjectAST *) {}
    virtual bool visit(RoleCommentObjectAST *) { return false; }
    virtual void endVisit(RoleCommentObjectAST *) {}
    virtual bool visit(TableCommentObjectAST *) { return false; }
    virtual void endVisit(TableCommentObjectAST *) {}
    virtual bool visit(SequenceCommentObjectAST *) { return false; }
    virtual void endVisit(SequenceCommentObjectAST *) {}
    virtual bool visit(IndexCommentObjectAST *) { return false; }
    virtual void endVisit(IndexCommentObjectAST *) {}
    virtual bool visit(SchemaCommentObjectAST *) { return false; }
    virtual void endVisit(SchemaCommentObjectAST *) {}
    virtual bool visit(ColumnCommentObjectAST *) { return false; }
    virtual void endVisit(ColumnCommentObjectAST *) {}
    virtual bool visit(NamedCommentObjectAST *) { return false; }
    virtual void endVisit(NamedCommentObjectAST *) {}
    virtual bool visit(AnyCommentObjectAST *) { return false; }
    virtual void endVisit(AnyCommentObjectAST *) {}
    virtual bool visit(CommentTextClauseAST *) { return false; }
    virtual void endVisit(CommentTextClauseAST *) {}
    virtual bool visit(ClusterStmtAST *) { return false; }
    virtual void endVisit(ClusterStmtAST *) {}
    virtual bool visit(ClusterOnStmtAST *) { return false; }
    virtual void endVisit(ClusterOnStmtAST *) {}
    virtual bool visit(ClusterUsingStmtAST *) { return false; }
    virtual void endVisit(ClusterUsingStmtAST *) {}
    virtual bool visit(UsingIndexClauseAST *) { return false; }
    virtual void endVisit(UsingIndexClauseAST *) {}
    virtual bool visit(ClosePortalStmtAST *) { return false; }
    virtual void endVisit(ClosePortalStmtAST *) {}
    virtual bool visit(CloseCursorStmtAST *) { return false; }
    virtual void endVisit(CloseCursorStmtAST *) {}
    virtual bool visit(CloseAllStmtAST *) { return false; }
    virtual void endVisit(CloseAllStmtAST *) {}
    virtual bool visit(CheckPointStmtAST *) { return false; }
    virtual void endVisit(CheckPointStmtAST *) {}
    virtual bool visit(AnalyzeStmtAST *) { return false; }
    virtual void endVisit(AnalyzeStmtAST *) {}
    virtual bool visit(AlterUserMappingStmtAST *) { return false; }
    virtual void endVisit(AlterUserMappingStmtAST *) {}
    virtual bool visit(AuthIdentifierAST *) { return false; }
    virtual void endVisit(AuthIdentifierAST *) {}
    virtual bool visit(UserAuthIdentifierAST *) { return false; }
    virtual void endVisit(UserAuthIdentifierAST *) {}
    virtual bool visit(RoleAuthIdentifierAST *) { return false; }
    virtual void endVisit(RoleAuthIdentifierAST *) {}
    virtual bool visit(AlterTSConfigurationStmtAST *) { return false; }
    virtual void endVisit(AlterTSConfigurationStmtAST *) {}
    virtual bool visit(AlterTSConfCmdAST *) { return false; }
    virtual void endVisit(AlterTSConfCmdAST *) {}
    virtual bool visit(AlterTSConfAddAlterCmdAST *) { return false; }
    virtual void endVisit(AlterTSConfAddAlterCmdAST *) {}
    virtual bool visit(AlterTSConfDropCmdAST *) { return false; }
    virtual void endVisit(AlterTSConfDropCmdAST *) {}
    virtual bool visit(AlterTSDictionaryStmtAST *) { return false; }
    virtual void endVisit(AlterTSDictionaryStmtAST *) {}
    virtual bool visit(AlterCompositeTypeStmtAST *) { return false; }
    virtual void endVisit(AlterCompositeTypeStmtAST *) {}
    virtual bool visit(AlterTypeCommandAST *) { return false; }
    virtual void endVisit(AlterTypeCommandAST *) {}
    virtual bool visit(AlterTypeAddCommandAST *) { return false; }
    virtual void endVisit(AlterTypeAddCommandAST *) {}
    virtual bool visit(AlterTypeDropCommandAST *) { return false; }
    virtual void endVisit(AlterTypeDropCommandAST *) {}
    virtual bool visit(AlterTypeAlterCommandAST *) { return false; }
    virtual void endVisit(AlterTypeAlterCommandAST *) {}
    virtual bool visit(AlterSeqStmtAST *) { return false; }
    virtual void endVisit(AlterSeqStmtAST *) {}
    virtual bool visit(CreateSeqStmtAST *) { return false; }
    virtual void endVisit(CreateSeqStmtAST *) {}
    virtual bool visit(SequenceOptionAST *) { return false; }
    virtual void endVisit(SequenceOptionAST *) {}
    virtual bool visit(SimpleSequenceOptionAST *) { return false; }
    virtual void endVisit(SimpleSequenceOptionAST *) {}
    virtual bool visit(OwnedSequenceOptionAST *) { return false; }
    virtual void endVisit(OwnedSequenceOptionAST *) {}
    virtual bool visit(TempClauseAST *) { return false; }
    virtual void endVisit(TempClauseAST *) {}
    virtual bool visit(AlterOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterOwnerStmtAST *) {}
    virtual bool visit(AlterEvtTriggerOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterEvtTriggerOwnerStmtAST *) {}
    virtual bool visit(AlterServerOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterServerOwnerStmtAST *) {}
    virtual bool visit(AlterFdwOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterFdwOwnerStmtAST *) {}
    virtual bool visit(AlterSearchOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterSearchOwnerStmtAST *) {}
    virtual bool visit(AlterTablespaceOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterTablespaceOwnerStmtAST *) {}
    virtual bool visit(AlterTypeOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterTypeOwnerStmtAST *) {}
    virtual bool visit(AlterSchemaOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterSchemaOwnerStmtAST *) {}
    virtual bool visit(AlterOpUsingOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterOpUsingOwnerStmtAST *) {}
    virtual bool visit(AlterOpOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterOpOwnerStmtAST *) {}
    virtual bool visit(AlterLargeObjOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterLargeObjOwnerStmtAST *) {}
    virtual bool visit(AlterLangOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterLangOwnerStmtAST *) {}
    virtual bool visit(AlterFunctionOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterFunctionOwnerStmtAST *) {}
    virtual bool visit(AlterDbOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterDbOwnerStmtAST *) {}
    virtual bool visit(AlterCollationOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterCollationOwnerStmtAST *) {}
    virtual bool visit(AlterConvOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterConvOwnerStmtAST *) {}
    virtual bool visit(AlterDomainOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterDomainOwnerStmtAST *) {}
    virtual bool visit(AlterAggrOwnerStmtAST *) { return false; }
    virtual void endVisit(AlterAggrOwnerStmtAST *) {}
    virtual bool visit(OwnerToClauseAST *) { return false; }
    virtual void endVisit(OwnerToClauseAST *) {}
    virtual bool visit(AlterObjectSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterObjectSchemaStmtAST *) {}
    virtual bool visit(AlterTypeSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterTypeSchemaStmtAST *) {}
    virtual bool visit(AlterViewSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterViewSchemaStmtAST *) {}
    virtual bool visit(AlterSeqSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterSeqSchemaStmtAST *) {}
    virtual bool visit(AlterSearchSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterSearchSchemaStmtAST *) {}
    virtual bool visit(AlterTableSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterTableSchemaStmtAST *) {}
    virtual bool visit(AlterOpUsingSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterOpUsingSchemaStmtAST *) {}
    virtual bool visit(AlterOpSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterOpSchemaStmtAST *) {}
    virtual bool visit(AlterFunctionSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterFunctionSchemaStmtAST *) {}
    virtual bool visit(AlterCollationSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterCollationSchemaStmtAST *) {}
    virtual bool visit(AlterConvSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterConvSchemaStmtAST *) {}
    virtual bool visit(AlterDomainSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterDomainSchemaStmtAST *) {}
    virtual bool visit(AlterExtSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterExtSchemaStmtAST *) {}
    virtual bool visit(AlterAggrFuncSchemaStmtAST *) { return false; }
    virtual void endVisit(AlterAggrFuncSchemaStmtAST *) {}
    virtual bool visit(SetSchemaClauseAST *) { return false; }
    virtual void endVisit(SetSchemaClauseAST *) {}
    virtual bool visit(AlterFunctionStmtAST *) { return false; }
    virtual void endVisit(AlterFunctionStmtAST *) {}
    virtual bool visit(AlterFunctionOptionAST *) { return false; }
    virtual void endVisit(AlterFunctionOptionAST *) {}
    virtual bool visit(CommonAlterFunctionOptAST *) { return false; }
    virtual void endVisit(CommonAlterFunctionOptAST *) {}
    virtual bool visit(RestAlterFunctionOptAST *) { return false; }
    virtual void endVisit(RestAlterFunctionOptAST *) {}
    virtual bool visit(AlterForeignServerStmtAST *) { return false; }
    virtual void endVisit(AlterForeignServerStmtAST *) {}
    virtual bool visit(ForeignServerVersionAST *) { return false; }
    virtual void endVisit(ForeignServerVersionAST *) {}
    virtual bool visit(AlterFdwStmtAST *) { return false; }
    virtual void endVisit(AlterFdwStmtAST *) {}
    virtual bool visit(FdwOptionAST *) { return false; }
    virtual void endVisit(FdwOptionAST *) {}
    virtual bool visit(SetFdwOptionAST *) { return false; }
    virtual void endVisit(SetFdwOptionAST *) {}
    virtual bool visit(ResetFdwOptionAST *) { return false; }
    virtual void endVisit(ResetFdwOptionAST *) {}
    virtual bool visit(HandlerIdentifierAST *) { return false; }
    virtual void endVisit(HandlerIdentifierAST *) {}
    virtual bool visit(AlterExtensionCtxStmtAST *) { return false; }
    virtual void endVisit(AlterExtensionCtxStmtAST *) {}
    virtual bool visit(ExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(ExtensionCtxCmdAST *) {}
    virtual bool visit(OpExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(OpExtensionCtxCmdAST *) {}
    virtual bool visit(OperArgTypesAST *) { return false; }
    virtual void endVisit(OperArgTypesAST *) {}
    virtual bool visit(LangExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(LangExtensionCtxCmdAST *) {}
    virtual bool visit(FunctionExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(FunctionExtensionCtxCmdAST *) {}
    virtual bool visit(FunctionWithArgTypesAST *) { return false; }
    virtual void endVisit(FunctionWithArgTypesAST *) {}
    virtual bool visit(FunctionArgTypesAST *) { return false; }
    virtual void endVisit(FunctionArgTypesAST *) {}
    virtual bool visit(FunctionArgAST *) { return false; }
    virtual void endVisit(FunctionArgAST *) {}
    virtual bool visit(SimpleExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(SimpleExtensionCtxCmdAST *) {}
    virtual bool visit(CastExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(CastExtensionCtxCmdAST *) {}
    virtual bool visit(AggrExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(AggrExtensionCtxCmdAST *) {}
    virtual bool visit(AggrArgumentsAST *) { return false; }
    virtual void endVisit(AggrArgumentsAST *) {}
    virtual bool visit(ListAggrArgumentsAST *) { return false; }
    virtual void endVisit(ListAggrArgumentsAST *) {}
    virtual bool visit(StarAggrArgumentsAST *) { return false; }
    virtual void endVisit(StarAggrArgumentsAST *) {}
    virtual bool visit(OpUsingExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(OpUsingExtensionCtxCmdAST *) {}
    virtual bool visit(SchemaExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(SchemaExtensionCtxCmdAST *) {}
    virtual bool visit(TrgEventExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(TrgEventExtensionCtxCmdAST *) {}
    virtual bool visit(TxtSearchExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(TxtSearchExtensionCtxCmdAST *) {}
    virtual bool visit(SequenceExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(SequenceExtensionCtxCmdAST *) {}
    virtual bool visit(TableExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(TableExtensionCtxCmdAST *) {}
    virtual bool visit(ViewExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(ViewExtensionCtxCmdAST *) {}
    virtual bool visit(FdwExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(FdwExtensionCtxCmdAST *) {}
    virtual bool visit(TypeExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(TypeExtensionCtxCmdAST *) {}
    virtual bool visit(ServerExtensionCtxCmdAST *) { return false; }
    virtual void endVisit(ServerExtensionCtxCmdAST *) {}
    virtual bool visit(AlterExtensionStmtAST *) { return false; }
    virtual void endVisit(AlterExtensionStmtAST *) {}
    virtual bool visit(UpdateExtensionCmdAST *) { return false; }
    virtual void endVisit(UpdateExtensionCmdAST *) {}
    virtual bool visit(UpdateToExtensionCmdAST *) { return false; }
    virtual void endVisit(UpdateToExtensionCmdAST *) {}
    virtual bool visit(AlterEnumStmtAST *) { return false; }
    virtual void endVisit(AlterEnumStmtAST *) {}
    virtual bool visit(IfNotExistsAST *) { return false; }
    virtual void endVisit(IfNotExistsAST *) {}
    virtual bool visit(AlterDomainStmtAST *) { return false; }
    virtual void endVisit(AlterDomainStmtAST *) {}
    virtual bool visit(AlterDomainAltDefStmtAST *) { return false; }
    virtual void endVisit(AlterDomainAltDefStmtAST *) {}
    virtual bool visit(AlterDomainAddConstrStmtAST *) { return false; }
    virtual void endVisit(AlterDomainAddConstrStmtAST *) {}
    virtual bool visit(AlterDomainDropConstrStmtAST *) { return false; }
    virtual void endVisit(AlterDomainDropConstrStmtAST *) {}
    virtual bool visit(AlterDomainValConstrStmtAST *) { return false; }
    virtual void endVisit(AlterDomainValConstrStmtAST *) {}
    virtual bool visit(AlterDomainChgNotNullStmtAST *) { return false; }
    virtual void endVisit(AlterDomainChgNotNullStmtAST *) {}
    virtual bool visit(CreateDomainStmtAST *) { return false; }
    virtual void endVisit(CreateDomainStmtAST *) {}
    virtual bool visit(AlterDefaultPrivilegesStmtAST *) { return false; }
    virtual void endVisit(AlterDefaultPrivilegesStmtAST *) {}
    virtual bool visit(DefACLOptionAST *) { return false; }
    virtual void endVisit(DefACLOptionAST *) {}
    virtual bool visit(InSchemaDefACLOptionAST *) { return false; }
    virtual void endVisit(InSchemaDefACLOptionAST *) {}
    virtual bool visit(ForRoleUserDefACLOptionAST *) { return false; }
    virtual void endVisit(ForRoleUserDefACLOptionAST *) {}
    virtual bool visit(DefACLActionAST *) { return false; }
    virtual void endVisit(DefACLActionAST *) {}
    virtual bool visit(RevokeDefACLActionAST *) { return false; }
    virtual void endVisit(RevokeDefACLActionAST *) {}
    virtual bool visit(GrantDefACLActionAST *) { return false; }
    virtual void endVisit(GrantDefACLActionAST *) {}
    virtual bool visit(PrivilegesAST *) { return false; }
    virtual void endVisit(PrivilegesAST *) {}
    virtual bool visit(AllPrivilegesAST *) { return false; }
    virtual void endVisit(AllPrivilegesAST *) {}
    virtual bool visit(PrivilegesAsListAST *) { return false; }
    virtual void endVisit(PrivilegesAsListAST *) {}
    virtual bool visit(CommonPrivilegeAST *) { return false; }
    virtual void endVisit(CommonPrivilegeAST *) {}
    virtual bool visit(GranteeAST *) { return false; }
    virtual void endVisit(GranteeAST *) {}
    virtual bool visit(WithGrantOptionAST *) { return false; }
    virtual void endVisit(WithGrantOptionAST *) {}
    virtual bool visit(AlterDatabaseStmtAST *) { return false; }
    virtual void endVisit(AlterDatabaseStmtAST *) {}
    virtual bool visit(AlterDatabaseSetStmtAST *) { return false; }
    virtual void endVisit(AlterDatabaseSetStmtAST *) {}
    virtual bool visit(AlterDatabaseChgOptStmtAST *) { return false; }
    virtual void endVisit(AlterDatabaseChgOptStmtAST *) {}
    virtual bool visit(AlterDbOptionAST *) { return false; }
    virtual void endVisit(AlterDbOptionAST *) {}
    virtual bool visit(AlterDbConnLimitOptionAST *) { return false; }
    virtual void endVisit(AlterDbConnLimitOptionAST *) {}
    virtual bool visit(AlterDatabaseSetTblspStmtAST *) { return false; }
    virtual void endVisit(AlterDatabaseSetTblspStmtAST *) {}
    virtual bool visit(CreateEventTrigStmtAST *) { return false; }
    virtual void endVisit(CreateEventTrigStmtAST *) {}
    virtual bool visit(EventTriggerWhenAST *) { return false; }
    virtual void endVisit(EventTriggerWhenAST *) {}
    virtual bool visit(EventTriggerValueAST *) { return false; }
    virtual void endVisit(EventTriggerValueAST *) {}
    virtual bool visit(AlterEventTrigStmtAST *) { return false; }
    virtual void endVisit(AlterEventTrigStmtAST *) {}
    virtual bool visit(EventTriggerCmdAST *) { return false; }
    virtual void endVisit(EventTriggerCmdAST *) {}
    virtual bool visit(CreateRoleStmtAST *) { return false; }
    virtual void endVisit(CreateRoleStmtAST *) {}
    virtual bool visit(AlterRoleStmtAST *) { return false; }
    virtual void endVisit(AlterRoleStmtAST *) {}
    virtual bool visit(OptRoleAST *) { return false; }
    virtual void endVisit(OptRoleAST *) {}
    virtual bool visit(OptRoleWithRoleListAST *) { return false; }
    virtual void endVisit(OptRoleWithRoleListAST *) {}
    virtual bool visit(OptRoleStaticAST *) { return false; }
    virtual void endVisit(OptRoleStaticAST *) {}
    virtual bool visit(RoleIdAST *) { return false; }
    virtual void endVisit(RoleIdAST *) {}
    virtual bool visit(DatabaseNameAST *) { return false; }
    virtual void endVisit(DatabaseNameAST *) {}
    virtual bool visit(InDatabaseClauseAST *) { return false; }
    virtual void endVisit(InDatabaseClauseAST *) {}
    virtual bool visit(AlterRoleSetStmtAST *) { return false; }
    virtual void endVisit(AlterRoleSetStmtAST *) {}
    virtual bool visit(AlterAnyRoleSetStmtAST *) { return false; }
    virtual void endVisit(AlterAnyRoleSetStmtAST *) {}
    virtual bool visit(AlterAllRolesSetStmtAST *) { return false; }
    virtual void endVisit(AlterAllRolesSetStmtAST *) {}
    virtual bool visit(SetResetClauseAST *) { return false; }
    virtual void endVisit(SetResetClauseAST *) {}
    virtual bool visit(SetResetClauseAsSetAST *) { return false; }
    virtual void endVisit(SetResetClauseAsSetAST *) {}
    virtual bool visit(SetResetClauseAsVarResetAST *) { return false; }
    virtual void endVisit(SetResetClauseAsVarResetAST *) {}
    virtual bool visit(SetRestAST *) { return false; }
    virtual void endVisit(SetRestAST *) {}
    virtual bool visit(SetRestTransactionAST *) { return false; }
    virtual void endVisit(SetRestTransactionAST *) {}
    virtual bool visit(SetRestVarNameToAST *) { return false; }
    virtual void endVisit(SetRestVarNameToAST *) {}
    virtual bool visit(SetRestVarNameToValuesAST *) { return false; }
    virtual void endVisit(SetRestVarNameToValuesAST *) {}
    virtual bool visit(SetRestZoneValueAST *) { return false; }
    virtual void endVisit(SetRestZoneValueAST *) {}
    virtual bool visit(SetRestCatalogAST *) { return false; }
    virtual void endVisit(SetRestCatalogAST *) {}
    virtual bool visit(SetRestSchemaAST *) { return false; }
    virtual void endVisit(SetRestSchemaAST *) {}
    virtual bool visit(SetRestEncodingAST *) { return false; }
    virtual void endVisit(SetRestEncodingAST *) {}
    virtual bool visit(SetRestRoleAST *) { return false; }
    virtual void endVisit(SetRestRoleAST *) {}
    virtual bool visit(SetRestSessionAuthAST *) { return false; }
    virtual void endVisit(SetRestSessionAuthAST *) {}
    virtual bool visit(SetRestXmlOptionAST *) { return false; }
    virtual void endVisit(SetRestXmlOptionAST *) {}
    virtual bool visit(SetRestTransactioSnapshotAST *) { return false; }
    virtual void endVisit(SetRestTransactioSnapshotAST *) {}
    virtual bool visit(VariableShowStmtAST *) { return false; }
    virtual void endVisit(VariableShowStmtAST *) {}
    virtual bool visit(VarShowStmtAST *) { return false; }
    virtual void endVisit(VarShowStmtAST *) {}
    virtual bool visit(TimeZoneShowStmtAST *) { return false; }
    virtual void endVisit(TimeZoneShowStmtAST *) {}
    virtual bool visit(IsoLevelShowStmtAST *) { return false; }
    virtual void endVisit(IsoLevelShowStmtAST *) {}
    virtual bool visit(SessionAuthShowStmtAST *) { return false; }
    virtual void endVisit(SessionAuthShowStmtAST *) {}
    virtual bool visit(AllShowStmtAST *) { return false; }
    virtual void endVisit(AllShowStmtAST *) {}
    virtual bool visit(VariableSetStmtAST *) { return false; }
    virtual void endVisit(VariableSetStmtAST *) {}
    virtual bool visit(VariableResetStmtAST *) { return false; }
    virtual void endVisit(VariableResetStmtAST *) {}
    virtual bool visit(SetRestResetVarNameAST *) { return false; }
    virtual void endVisit(SetRestResetVarNameAST *) {}
    virtual bool visit(SetRestResetTimeZoneAST *) { return false; }
    virtual void endVisit(SetRestResetTimeZoneAST *) {}
    virtual bool visit(SetRestResetTransLevelAST *) { return false; }
    virtual void endVisit(SetRestResetTransLevelAST *) {}
    virtual bool visit(SetRestResetAuthorizationAST *) { return false; }
    virtual void endVisit(SetRestResetAuthorizationAST *) {}
    virtual bool visit(SetRestResetAllAST *) { return false; }
    virtual void endVisit(SetRestResetAllAST *) {}
    virtual bool visit(IsoLevelAST *) { return false; }
    virtual void endVisit(IsoLevelAST *) {}
    virtual bool visit(TransactionModeAST *) { return false; }
    virtual void endVisit(TransactionModeAST *) {}
    virtual bool visit(TransactionModeIsoLevelAST *) { return false; }
    virtual void endVisit(TransactionModeIsoLevelAST *) {}
    virtual bool visit(TransactionModeReadWriteAST *) { return false; }
    virtual void endVisit(TransactionModeReadWriteAST *) {}
    virtual bool visit(TransactionModeDeferAST *) { return false; }
    virtual void endVisit(TransactionModeDeferAST *) {}
    virtual bool visit(IntervalSecondAST *) { return false; }
    virtual void endVisit(IntervalSecondAST *) {}
    virtual bool visit(IntervalAST *) { return false; }
    virtual void endVisit(IntervalAST *) {}
    virtual bool visit(IntervalSimpleAST *) { return false; }
    virtual void endVisit(IntervalSimpleAST *) {}
    virtual bool visit(IntervalSimpleToAST *) { return false; }
    virtual void endVisit(IntervalSimpleToAST *) {}
    virtual bool visit(IntervalSimpleToSecondAST *) { return false; }
    virtual void endVisit(IntervalSimpleToSecondAST *) {}
    virtual bool visit(VarValueAST *) { return false; }
    virtual void endVisit(VarValueAST *) {}
    virtual bool visit(ZoneValueAST *) { return false; }
    virtual void endVisit(ZoneValueAST *) {}
    virtual bool visit(ZoneValueSimpleAST *) { return false; }
    virtual void endVisit(ZoneValueSimpleAST *) {}
    virtual bool visit(ZoneValueIntervalAST *) { return false; }
    virtual void endVisit(ZoneValueIntervalAST *) {}
    virtual bool visit(DropRoleStmtAST *) { return false; }
    virtual void endVisit(DropRoleStmtAST *) {}
    virtual bool visit(CreateGroupStmtAST *) { return false; }
    virtual void endVisit(CreateGroupStmtAST *) {}
    virtual bool visit(AlterGroupStmtAST *) { return false; }
    virtual void endVisit(AlterGroupStmtAST *) {}
    virtual bool visit(DiscardStmtAST *) { return false; }
    virtual void endVisit(DiscardStmtAST *) {}
    virtual bool visit(DropStatementAST *) { return false; }
    virtual void endVisit(DropStatementAST *) {}
    virtual bool visit(TruncateStatementAST *) { return false; }
    virtual void endVisit(TruncateStatementAST *) {}
    virtual bool visit(AlterStmtAST *) { return false; }
    virtual void endVisit(AlterStmtAST *) {}
    virtual bool visit(AlterIndexStmtAST *) { return false; }
    virtual void endVisit(AlterIndexStmtAST *) {}
    virtual bool visit(AlterSequenceStmtAST *) { return false; }
    virtual void endVisit(AlterSequenceStmtAST *) {}
    virtual bool visit(AlterTableStmtAST *) { return false; }
    virtual void endVisit(AlterTableStmtAST *) {}
    virtual bool visit(AlterForeignTableStmtAST *) { return false; }
    virtual void endVisit(AlterForeignTableStmtAST *) {}
    virtual bool visit(AlterViewStmtAST *) { return false; }
    virtual void endVisit(AlterViewStmtAST *) {}
    virtual bool visit(UpdateStmtAST *) { return false; }
    virtual void endVisit(UpdateStmtAST *) {}
    virtual bool visit(SetClauseAST *) { return false; }
    virtual void endVisit(SetClauseAST *) {}
    virtual bool visit(SingleSetClauseAST *) { return false; }
    virtual void endVisit(SingleSetClauseAST *) {}
    virtual bool visit(MultipleSetClauseAST *) { return false; }
    virtual void endVisit(MultipleSetClauseAST *) {}
    virtual bool visit(InsertStmtAST *) { return false; }
    virtual void endVisit(InsertStmtAST *) {}
    virtual bool visit(InsertRestAST *) { return false; }
    virtual void endVisit(InsertRestAST *) {}
    virtual bool visit(CommonInsertRestAST *) { return false; }
    virtual void endVisit(CommonInsertRestAST *) {}
    virtual bool visit(DefaultValuesInsertRestAST *) { return false; }
    virtual void endVisit(DefaultValuesInsertRestAST *) {}
    virtual bool visit(DeleteStmtAST *) { return false; }
    virtual void endVisit(DeleteStmtAST *) {}
    virtual bool visit(ReturningClauseAST *) { return false; }
    virtual void endVisit(ReturningClauseAST *) {}
    virtual bool visit(WhereOrCurrentClauseAST *) { return false; }
    virtual void endVisit(WhereOrCurrentClauseAST *) {}
    virtual bool visit(WhereNoCurrentClauseAST *) { return false; }
    virtual void endVisit(WhereNoCurrentClauseAST *) {}
    virtual bool visit(CurrenWhereClauseAST *) { return false; }
    virtual void endVisit(CurrenWhereClauseAST *) {}
    virtual bool visit(DelUsingClauseAST *) { return false; }
    virtual void endVisit(DelUsingClauseAST *) {}
    virtual bool visit(RelationAliasExprAST *) { return false; }
    virtual void endVisit(RelationAliasExprAST *) {}
    virtual bool visit(SelectStmtAST *) { return false; }
    virtual void endVisit(SelectStmtAST *) {}
    virtual bool visit(SelectWithParensAST *) { return false; }
    virtual void endVisit(SelectWithParensAST *) {}
    virtual bool visit(SelectNoParensAST *) { return false; }
    virtual void endVisit(SelectNoParensAST *) {}
    virtual bool visit(WithClauseAST *) { return false; }
    virtual void endVisit(WithClauseAST *) {}
    virtual bool visit(CommonTableExprAST *) { return false; }
    virtual void endVisit(CommonTableExprAST *) {}
    virtual bool visit(CompoundColumnNamesAST *) { return false; }
    virtual void endVisit(CompoundColumnNamesAST *) {}
    virtual bool visit(SortClauseAST *) { return false; }
    virtual void endVisit(SortClauseAST *) {}
    virtual bool visit(OrderExprAST *) { return false; }
    virtual void endVisit(OrderExprAST *) {}
    virtual bool visit(UsingOrderExprAST *) { return false; }
    virtual void endVisit(UsingOrderExprAST *) {}
    virtual bool visit(CommonOrderExprAST *) { return false; }
    virtual void endVisit(CommonOrderExprAST *) {}
    virtual bool visit(LockingLimitClauseAST *) { return false; }
    virtual void endVisit(LockingLimitClauseAST *) {}
    virtual bool visit(LockingFirstLimitClauseAST *) { return false; }
    virtual void endVisit(LockingFirstLimitClauseAST *) {}
    virtual bool visit(LockingLastLimitClauseAST *) { return false; }
    virtual void endVisit(LockingLastLimitClauseAST *) {}
    virtual bool visit(LimitClauseAST *) { return false; }
    virtual void endVisit(LimitClauseAST *) {}
    virtual bool visit(LimitOffsetClauseAST *) { return false; }
    virtual void endVisit(LimitOffsetClauseAST *) {}
    virtual bool visit(OffsetLimitClauseAST *) { return false; }
    virtual void endVisit(OffsetLimitClauseAST *) {}
    virtual bool visit(OffsetAST *) { return false; }
    virtual void endVisit(OffsetAST *) {}
    virtual bool visit(LimitAST *) { return false; }
    virtual void endVisit(LimitAST *) {}
    virtual bool visit(CommonLimitAST *) { return false; }
    virtual void endVisit(CommonLimitAST *) {}
    virtual bool visit(FetchLimitAST *) { return false; }
    virtual void endVisit(FetchLimitAST *) {}
    virtual bool visit(FetchFirstValueAST *) { return false; }
    virtual void endVisit(FetchFirstValueAST *) {}
    virtual bool visit(ConstFetchFirstValueAST *) { return false; }
    virtual void endVisit(ConstFetchFirstValueAST *) {}
    virtual bool visit(CompoundFetchFirstValueAST *) { return false; }
    virtual void endVisit(CompoundFetchFirstValueAST *) {}
    virtual bool visit(LimitValueAST *) { return false; }
    virtual void endVisit(LimitValueAST *) {}
    virtual bool visit(AllLimitValueAST *) { return false; }
    virtual void endVisit(AllLimitValueAST *) {}
    virtual bool visit(ExprLimitValueAST *) { return false; }
    virtual void endVisit(ExprLimitValueAST *) {}
    virtual bool visit(ForLockingClauseAST *) { return false; }
    virtual void endVisit(ForLockingClauseAST *) {}
    virtual bool visit(ReadOnlyForLockingClauseAST *) { return false; }
    virtual void endVisit(ReadOnlyForLockingClauseAST *) {}
    virtual bool visit(CommonForLockingClauseAST *) { return false; }
    virtual void endVisit(CommonForLockingClauseAST *) {}
    virtual bool visit(LockingExprAST *) { return false; }
    virtual void endVisit(LockingExprAST *) {}
    virtual bool visit(LockingStrengthAST *) { return false; }
    virtual void endVisit(LockingStrengthAST *) {}
    virtual bool visit(LockingRelsAST *) { return false; }
    virtual void endVisit(LockingRelsAST *) {}
    virtual bool visit(SelectClauseAST *) { return false; }
    virtual void endVisit(SelectClauseAST *) {}
    virtual bool visit(CompoundSelectClauseAST *) { return false; }
    virtual void endVisit(CompoundSelectClauseAST *) {}
    virtual bool visit(CoreSimpleSelectAST *) { return false; }
    virtual void endVisit(CoreSimpleSelectAST *) {}
    virtual bool visit(SimpleSelectAST *) { return false; }
    virtual void endVisit(SimpleSelectAST *) {}
    virtual bool visit(ValuesSimpleSelectAST *) { return false; }
    virtual void endVisit(ValuesSimpleSelectAST *) {}
    virtual bool visit(TableSimpleSelectAST *) { return false; }
    virtual void endVisit(TableSimpleSelectAST *) {}
    virtual bool visit(CompositeSimpleSelectAST *) { return false; }
    virtual void endVisit(CompositeSimpleSelectAST *) {}
    virtual bool visit(SelectCoreAST *) { return false; }
    virtual void endVisit(SelectCoreAST *) {}
    virtual bool visit(WindowClauseAST *) { return false; }
    virtual void endVisit(WindowClauseAST *) {}
    virtual bool visit(WindowDefinitionAST *) { return false; }
    virtual void endVisit(WindowDefinitionAST *) {}
    virtual bool visit(HavingClauseAST *) { return false; }
    virtual void endVisit(HavingClauseAST *) {}
    virtual bool visit(GroupByClauseAST *) { return false; }
    virtual void endVisit(GroupByClauseAST *) {}
    virtual bool visit(WhereClauseAST *) { return false; }
    virtual void endVisit(WhereClauseAST *) {}
    virtual bool visit(FromClauseAST *) { return false; }
    virtual void endVisit(FromClauseAST *) {}
    virtual bool visit(TableRefAST *) { return false; }
    virtual void endVisit(TableRefAST *) {}
    virtual bool visit(JoinedTableRefAST *) { return false; }
    virtual void endVisit(JoinedTableRefAST *) {}
    virtual bool visit(JoinedTableAST *) { return false; }
    virtual void endVisit(JoinedTableAST *) {}
    virtual bool visit(CommonJoinedTableAST *) { return false; }
    virtual void endVisit(CommonJoinedTableAST *) {}
    virtual bool visit(JoinTypeAST *) { return false; }
    virtual void endVisit(JoinTypeAST *) {}
    virtual bool visit(JoinConditionAST *) { return false; }
    virtual void endVisit(JoinConditionAST *) {}
    virtual bool visit(OnJoinConditionAST *) { return false; }
    virtual void endVisit(OnJoinConditionAST *) {}
    virtual bool visit(UsingJoinConditionAST *) { return false; }
    virtual void endVisit(UsingJoinConditionAST *) {}
    virtual bool visit(CrossJoinedTableAST *) { return false; }
    virtual void endVisit(CrossJoinedTableAST *) {}
    virtual bool visit(CompoundJoinedTableAST *) { return false; }
    virtual void endVisit(CompoundJoinedTableAST *) {}
    virtual bool visit(CompoundTableRefAST *) { return false; }
    virtual void endVisit(CompoundTableRefAST *) {}
    virtual bool visit(FuncionTableRefAST *) { return false; }
    virtual void endVisit(FuncionTableRefAST *) {}
    virtual bool visit(FuncAliasClauseAST *) { return false; }
    virtual void endVisit(FuncAliasClauseAST *) {}
    virtual bool visit(FuncFuncAliasClauseAST *) { return false; }
    virtual void endVisit(FuncFuncAliasClauseAST *) {}
    virtual bool visit(TableFuncAliasClauseAST *) { return false; }
    virtual void endVisit(TableFuncAliasClauseAST *) {}
    virtual bool visit(TableFuncElementAST *) { return false; }
    virtual void endVisit(TableFuncElementAST *) {}
    virtual bool visit(CollateClauseAST *) { return false; }
    virtual void endVisit(CollateClauseAST *) {}
    virtual bool visit(FunctionExpressionAST *) { return false; }
    virtual void endVisit(FunctionExpressionAST *) {}
    virtual bool visit(XmlserializeFunctionExprAST *) { return false; }
    virtual void endVisit(XmlserializeFunctionExprAST *) {}
    virtual bool visit(XmlrootFunctionExprAST *) { return false; }
    virtual void endVisit(XmlrootFunctionExprAST *) {}
    virtual bool visit(XmlRootVersionAST *) { return false; }
    virtual void endVisit(XmlRootVersionAST *) {}
    virtual bool visit(ExprXmlRootVersionAST *) { return false; }
    virtual void endVisit(ExprXmlRootVersionAST *) {}
    virtual bool visit(NoValueXmlRootVersionAST *) { return false; }
    virtual void endVisit(NoValueXmlRootVersionAST *) {}
    virtual bool visit(XmlRootStandaloneAST *) { return false; }
    virtual void endVisit(XmlRootStandaloneAST *) {}
    virtual bool visit(XmlpiFunctionExprAST *) { return false; }
    virtual void endVisit(XmlpiFunctionExprAST *) {}
    virtual bool visit(XmlparseFunctionExprAST *) { return false; }
    virtual void endVisit(XmlparseFunctionExprAST *) {}
    virtual bool visit(XmlWhitespaceOptionAST *) { return false; }
    virtual void endVisit(XmlWhitespaceOptionAST *) {}
    virtual bool visit(XmlforestFunctionExprAST *) { return false; }
    virtual void endVisit(XmlforestFunctionExprAST *) {}
    virtual bool visit(XmlexistsFunctionExprAST *) { return false; }
    virtual void endVisit(XmlexistsFunctionExprAST *) {}
    virtual bool visit(XmlExistsArgumentAST *) { return false; }
    virtual void endVisit(XmlExistsArgumentAST *) {}
    virtual bool visit(XmlelementFunctionExprAST *) { return false; }
    virtual void endVisit(XmlelementFunctionExprAST *) {}
    virtual bool visit(XmlAttributesAST *) { return false; }
    virtual void endVisit(XmlAttributesAST *) {}
    virtual bool visit(XmlAttributeAST *) { return false; }
    virtual void endVisit(XmlAttributeAST *) {}
    virtual bool visit(CoalesceFunctionExprAST *) { return false; }
    virtual void endVisit(CoalesceFunctionExprAST *) {}
    virtual bool visit(GreatestFunctionExprAST *) { return false; }
    virtual void endVisit(GreatestFunctionExprAST *) {}
    virtual bool visit(LeastFunctionExprAST *) { return false; }
    virtual void endVisit(LeastFunctionExprAST *) {}
    virtual bool visit(XmlconcatFunctionExprAST *) { return false; }
    virtual void endVisit(XmlconcatFunctionExprAST *) {}
    virtual bool visit(NullifFunctionExprAST *) { return false; }
    virtual void endVisit(NullifFunctionExprAST *) {}
    virtual bool visit(TrimFunctionExprAST *) { return false; }
    virtual void endVisit(TrimFunctionExprAST *) {}
    virtual bool visit(TrimExprAST *) { return false; }
    virtual void endVisit(TrimExprAST *) {}
    virtual bool visit(TraetFunctionExprAST *) { return false; }
    virtual void endVisit(TraetFunctionExprAST *) {}
    virtual bool visit(SubstringFunctionExprAST *) { return false; }
    virtual void endVisit(SubstringFunctionExprAST *) {}
    virtual bool visit(SubstringExprAST *) { return false; }
    virtual void endVisit(SubstringExprAST *) {}
    virtual bool visit(StandardSubstringExprAST *) { return false; }
    virtual void endVisit(StandardSubstringExprAST *) {}
    virtual bool visit(ListSubstringExprAST *) { return false; }
    virtual void endVisit(ListSubstringExprAST *) {}
    virtual bool visit(PositionFunctionExprAST *) { return false; }
    virtual void endVisit(PositionFunctionExprAST *) {}
    virtual bool visit(OverlayFunctionExprAST *) { return false; }
    virtual void endVisit(OverlayFunctionExprAST *) {}
    virtual bool visit(PositionExprAST *) { return false; }
    virtual void endVisit(PositionExprAST *) {}
    virtual bool visit(OverlayExprAST *) { return false; }
    virtual void endVisit(OverlayExprAST *) {}
    virtual bool visit(OverlayPlacingAST *) { return false; }
    virtual void endVisit(OverlayPlacingAST *) {}
    virtual bool visit(ExtractFunctionExprAST *) { return false; }
    virtual void endVisit(ExtractFunctionExprAST *) {}
    virtual bool visit(SubstringFromAST *) { return false; }
    virtual void endVisit(SubstringFromAST *) {}
    virtual bool visit(SubstringForAST *) { return false; }
    virtual void endVisit(SubstringForAST *) {}
    virtual bool visit(ExtractExprAST *) { return false; }
    virtual void endVisit(ExtractExprAST *) {}
    virtual bool visit(CastFunctionExprAST *) { return false; }
    virtual void endVisit(CastFunctionExprAST *) {}
    virtual bool visit(CollationFunctionExprAST *) { return false; }
    virtual void endVisit(CollationFunctionExprAST *) {}
    virtual bool visit(SimpleFunctionExprAST *) { return false; }
    virtual void endVisit(SimpleFunctionExprAST *) {}
    virtual bool visit(CommonFunctionExprAST *) { return false; }
    virtual void endVisit(CommonFunctionExprAST *) {}
    virtual bool visit(FunctionArgExprAST *) { return false; }
    virtual void endVisit(FunctionArgExprAST *) {}
    virtual bool visit(OverClauseAST *) { return false; }
    virtual void endVisit(OverClauseAST *) {}
    virtual bool visit(SimpleOverClauseAST *) { return false; }
    virtual void endVisit(SimpleOverClauseAST *) {}
    virtual bool visit(WindowOverClauseAST *) { return false; }
    virtual void endVisit(WindowOverClauseAST *) {}
    virtual bool visit(WindowSpecificationAST *) { return false; }
    virtual void endVisit(WindowSpecificationAST *) {}
    virtual bool visit(PartitionClauseAST *) { return false; }
    virtual void endVisit(PartitionClauseAST *) {}
    virtual bool visit(FrameClauseAST *) { return false; }
    virtual void endVisit(FrameClauseAST *) {}
    virtual bool visit(FrameExtentAST *) { return false; }
    virtual void endVisit(FrameExtentAST *) {}
    virtual bool visit(BoundFrameExtentAST *) { return false; }
    virtual void endVisit(BoundFrameExtentAST *) {}
    virtual bool visit(BetweenFrameExtentAST *) { return false; }
    virtual void endVisit(BetweenFrameExtentAST *) {}
    virtual bool visit(FrameBoundAST *) { return false; }
    virtual void endVisit(FrameBoundAST *) {}
    virtual bool visit(SimpleFrameBoundAST *) { return false; }
    virtual void endVisit(SimpleFrameBoundAST *) {}
    virtual bool visit(ExprFrameBoundAST *) { return false; }
    virtual void endVisit(ExprFrameBoundAST *) {}
    virtual bool visit(DirectTableRefAST *) { return false; }
    virtual void endVisit(DirectTableRefAST *) {}
    virtual bool visit(TableAliasClauseAST *) { return false; }
    virtual void endVisit(TableAliasClauseAST *) {}
    virtual bool visit(DistinctClauseAST *) { return false; }
    virtual void endVisit(DistinctClauseAST *) {}
    virtual bool visit(SimpleDistinctClauseAST *) { return false; }
    virtual void endVisit(SimpleDistinctClauseAST *) {}
    virtual bool visit(ExprListDistinctClauseAST *) { return false; }
    virtual void endVisit(ExprListDistinctClauseAST *) {}
    virtual bool visit(SelectTargetAST *) { return false; }
    virtual void endVisit(SelectTargetAST *) {}
    virtual bool visit(StarSelectTargetAST *) { return false; }
    virtual void endVisit(StarSelectTargetAST *) {}
    virtual bool visit(ExprSelectTargetAST *) { return false; }
    virtual void endVisit(ExprSelectTargetAST *) {}
    virtual bool visit(IntoClauseAST *) { return false; }
    virtual void endVisit(IntoClauseAST *) {}
    virtual bool visit(OptTempTableNameAST *) { return false; }
    virtual void endVisit(OptTempTableNameAST *) {}
    virtual bool visit(ValuesClauseAST *) { return false; }
    virtual void endVisit(ValuesClauseAST *) {}
    virtual bool visit(ColumnExprRowAST *) { return false; }
    virtual void endVisit(ColumnExprRowAST *) {}
    virtual bool visit(ColumnExpressionAST *) { return false; }
    virtual void endVisit(ColumnExpressionAST *) {}
    virtual bool visit(DefaultColumnExpressionAST *) { return false; }
    virtual void endVisit(DefaultColumnExpressionAST *) {}
    virtual bool visit(ExprColumnExpressionAST *) { return false; }
    virtual void endVisit(ExprColumnExpressionAST *) {}
    virtual bool visit(AlterTableCmdAST *) { return false; }
    virtual void endVisit(AlterTableCmdAST *) {}
    virtual bool visit(AlterTableGenericCmdAST *) { return false; }
    virtual void endVisit(AlterTableGenericCmdAST *) {}
    virtual bool visit(AlterGenericOptionAST *) { return false; }
    virtual void endVisit(AlterGenericOptionAST *) {}
    virtual bool visit(SetAddAlterGenericOptionAST *) { return false; }
    virtual void endVisit(SetAddAlterGenericOptionAST *) {}
    virtual bool visit(DropAlterGenericOptionAST *) { return false; }
    virtual void endVisit(DropAlterGenericOptionAST *) {}
    virtual bool visit(AlterTableSetReloptCmdAST *) { return false; }
    virtual void endVisit(AlterTableSetReloptCmdAST *) {}
    virtual bool visit(AlterTableTablespaceCmdAST *) { return false; }
    virtual void endVisit(AlterTableTablespaceCmdAST *) {}
    virtual bool visit(AlterTableOwnerCmdAST *) { return false; }
    virtual void endVisit(AlterTableOwnerCmdAST *) {}
    virtual bool visit(AlterTableNotOfCmdAST *) { return false; }
    virtual void endVisit(AlterTableNotOfCmdAST *) {}
    virtual bool visit(AlterTableOfCmdAST *) { return false; }
    virtual void endVisit(AlterTableOfCmdAST *) {}
    virtual bool visit(AlterTableInheritCmdAST *) { return false; }
    virtual void endVisit(AlterTableInheritCmdAST *) {}
    virtual bool visit(AlterTableTriggerCmdAST *) { return false; }
    virtual void endVisit(AlterTableTriggerCmdAST *) {}
    virtual bool visit(AlterTableRuleCmdAST *) { return false; }
    virtual void endVisit(AlterTableRuleCmdAST *) {}
    virtual bool visit(WildcardAST *) { return false; }
    virtual void endVisit(WildcardAST *) {}
    virtual bool visit(NameWildcardAST *) { return false; }
    virtual void endVisit(NameWildcardAST *) {}
    virtual bool visit(AllOrUserWildcardAST *) { return false; }
    virtual void endVisit(AllOrUserWildcardAST *) {}
    virtual bool visit(AlterTableClusterOffCmdAST *) { return false; }
    virtual void endVisit(AlterTableClusterOffCmdAST *) {}
    virtual bool visit(AlterTableClusterOnCmdAST *) { return false; }
    virtual void endVisit(AlterTableClusterOnCmdAST *) {}
    virtual bool visit(AlterTableSetOidsCmdAST *) { return false; }
    virtual void endVisit(AlterTableSetOidsCmdAST *) {}
    virtual bool visit(AlterTableDropConstrCmdAST *) { return false; }
    virtual void endVisit(AlterTableDropConstrCmdAST *) {}
    virtual bool visit(AlterTableValConstrCmdAST *) { return false; }
    virtual void endVisit(AlterTableValConstrCmdAST *) {}
    virtual bool visit(AlterTableAddConstrCmdAST *) { return false; }
    virtual void endVisit(AlterTableAddConstrCmdAST *) {}
    virtual bool visit(AlterTableSetTypeCmdAST *) { return false; }
    virtual void endVisit(AlterTableSetTypeCmdAST *) {}
    virtual bool visit(SetDataAST *) { return false; }
    virtual void endVisit(SetDataAST *) {}
    virtual bool visit(UsingClauseAST *) { return false; }
    virtual void endVisit(UsingClauseAST *) {}
    virtual bool visit(AlterTableSetStorageCmdAST *) { return false; }
    virtual void endVisit(AlterTableSetStorageCmdAST *) {}
    virtual bool visit(AlterTableChgReloptCmdAST *) { return false; }
    virtual void endVisit(AlterTableChgReloptCmdAST *) {}
    virtual bool visit(RelOptionsAST *) { return false; }
    virtual void endVisit(RelOptionsAST *) {}
    virtual bool visit(RelOptionAST *) { return false; }
    virtual void endVisit(RelOptionAST *) {}
    virtual bool visit(AlterTableSetStatCmdAST *) { return false; }
    virtual void endVisit(AlterTableSetStatCmdAST *) {}
    virtual bool visit(AlterTableChgNotNullCmdAST *) { return false; }
    virtual void endVisit(AlterTableChgNotNullCmdAST *) {}
    virtual bool visit(AlterTableChgDefColCmdAST *) { return false; }
    virtual void endVisit(AlterTableChgDefColCmdAST *) {}
    virtual bool visit(AlterDefaultColumnAST *) { return false; }
    virtual void endVisit(AlterDefaultColumnAST *) {}
    virtual bool visit(SetAlterDefaultColumnAST *) { return false; }
    virtual void endVisit(SetAlterDefaultColumnAST *) {}
    virtual bool visit(DropAlterDefaultColumnAST *) { return false; }
    virtual void endVisit(DropAlterDefaultColumnAST *) {}
    virtual bool visit(AlterTableDropColCmdAST *) { return false; }
    virtual void endVisit(AlterTableDropColCmdAST *) {}
    virtual bool visit(AlterTableAddColCmdAST *) { return false; }
    virtual void endVisit(AlterTableAddColCmdAST *) {}
    virtual bool visit(ColumnDefAST *) { return false; }
    virtual void endVisit(ColumnDefAST *) {}
    virtual bool visit(ConstraintAST *) { return false; }
    virtual void endVisit(ConstraintAST *) {}
    virtual bool visit(TableConstraintAST *) { return false; }
    virtual void endVisit(TableConstraintAST *) {}
    virtual bool visit(TableConstraintBodyAST *) { return false; }
    virtual void endVisit(TableConstraintBodyAST *) {}
    virtual bool visit(FkTableConstraintAST *) { return false; }
    virtual void endVisit(FkTableConstraintAST *) {}
    virtual bool visit(ExcludeTableConstraintAST *) { return false; }
    virtual void endVisit(ExcludeTableConstraintAST *) {}
    virtual bool visit(AccessMethodClauseAST *) { return false; }
    virtual void endVisit(AccessMethodClauseAST *) {}
    virtual bool visit(ExclusionWhereClauseAST *) { return false; }
    virtual void endVisit(ExclusionWhereClauseAST *) {}
    virtual bool visit(ExclusionConstraintAST *) { return false; }
    virtual void endVisit(ExclusionConstraintAST *) {}
    virtual bool visit(IndexElementAST *) { return false; }
    virtual void endVisit(IndexElementAST *) {}
    virtual bool visit(ColIndexElementAST *) { return false; }
    virtual void endVisit(ColIndexElementAST *) {}
    virtual bool visit(FuncExprIndexElementAST *) { return false; }
    virtual void endVisit(FuncExprIndexElementAST *) {}
    virtual bool visit(ExprIndexElementAST *) { return false; }
    virtual void endVisit(ExprIndexElementAST *) {}
    virtual bool visit(ClassClauseAST *) { return false; }
    virtual void endVisit(ClassClauseAST *) {}
    virtual bool visit(IndPrimKeyTableConstraintAST *) { return false; }
    virtual void endVisit(IndPrimKeyTableConstraintAST *) {}
    virtual bool visit(PrimKeyTableConstraintAST *) { return false; }
    virtual void endVisit(PrimKeyTableConstraintAST *) {}
    virtual bool visit(IndUniqueTableConstraintAST *) { return false; }
    virtual void endVisit(IndUniqueTableConstraintAST *) {}
    virtual bool visit(ExistingIndexAST *) { return false; }
    virtual void endVisit(ExistingIndexAST *) {}
    virtual bool visit(UniqueTableConstraintAST *) { return false; }
    virtual void endVisit(UniqueTableConstraintAST *) {}
    virtual bool visit(CheckTableConstraintAST *) { return false; }
    virtual void endVisit(CheckTableConstraintAST *) {}
    virtual bool visit(ConstraintAttributeAST *) { return false; }
    virtual void endVisit(ConstraintAttributeAST *) {}
    virtual bool visit(DeferConstraintAttrAST *) { return false; }
    virtual void endVisit(DeferConstraintAttrAST *) {}
    virtual bool visit(InitiallyConstraintAttrAST *) { return false; }
    virtual void endVisit(InitiallyConstraintAttrAST *) {}
    virtual bool visit(NotValidConstraintAttrAST *) { return false; }
    virtual void endVisit(NotValidConstraintAttrAST *) {}
    virtual bool visit(NoInheritConstraintAttrAST *) { return false; }
    virtual void endVisit(NoInheritConstraintAttrAST *) {}
    virtual bool visit(ColumnConstraintAST *) { return false; }
    virtual void endVisit(ColumnConstraintAST *) {}
    virtual bool visit(CommonColConstraintAST *) { return false; }
    virtual void endVisit(CommonColConstraintAST *) {}
    virtual bool visit(ConstraintBodyAST *) { return false; }
    virtual void endVisit(ConstraintBodyAST *) {}
    virtual bool visit(RefConstraintBodyAST *) { return false; }
    virtual void endVisit(RefConstraintBodyAST *) {}
    virtual bool visit(RefColumnsAST *) { return false; }
    virtual void endVisit(RefColumnsAST *) {}
    virtual bool visit(KeyMatchAST *) { return false; }
    virtual void endVisit(KeyMatchAST *) {}
    virtual bool visit(KeyActionsAST *) { return false; }
    virtual void endVisit(KeyActionsAST *) {}
    virtual bool visit(UpdDelKeyActionsAST *) { return false; }
    virtual void endVisit(UpdDelKeyActionsAST *) {}
    virtual bool visit(DelUpdKeyActionsAST *) { return false; }
    virtual void endVisit(DelUpdKeyActionsAST *) {}
    virtual bool visit(KeyUpdateAST *) { return false; }
    virtual void endVisit(KeyUpdateAST *) {}
    virtual bool visit(KeyDeleteAST *) { return false; }
    virtual void endVisit(KeyDeleteAST *) {}
    virtual bool visit(KeyActionAST *) { return false; }
    virtual void endVisit(KeyActionAST *) {}
    virtual bool visit(DefaultConstraintBodyAST *) { return false; }
    virtual void endVisit(DefaultConstraintBodyAST *) {}
    virtual bool visit(CheckConstraintBodyAST *) { return false; }
    virtual void endVisit(CheckConstraintBodyAST *) {}
    virtual bool visit(NoInheritAST *) { return false; }
    virtual void endVisit(NoInheritAST *) {}
    virtual bool visit(UniqueConstraintBodyAST *) { return false; }
    virtual void endVisit(UniqueConstraintBodyAST *) {}
    virtual bool visit(PrimaryKeyConstraintBodyAST *) { return false; }
    virtual void endVisit(PrimaryKeyConstraintBodyAST *) {}
    virtual bool visit(ConstraintTablespaceAST *) { return false; }
    virtual void endVisit(ConstraintTablespaceAST *) {}
    virtual bool visit(ConstraintDefinitionsAST *) { return false; }
    virtual void endVisit(ConstraintDefinitionsAST *) {}
    virtual bool visit(DefinitionAST *) { return false; }
    virtual void endVisit(DefinitionAST *) {}
    virtual bool visit(DefinitionElemAST *) { return false; }
    virtual void endVisit(DefinitionElemAST *) {}
    virtual bool visit(DefinitionArgumentAST *) { return false; }
    virtual void endVisit(DefinitionArgumentAST *) {}
    virtual bool visit(FuncTyDefinitionArgumentAST *) { return false; }
    virtual void endVisit(FuncTyDefinitionArgumentAST *) {}
    virtual bool visit(FunctionTypeAST *) { return false; }
    virtual void endVisit(FunctionTypeAST *) {}
    virtual bool visit(PercentFunctionTypeAST *) { return false; }
    virtual void endVisit(PercentFunctionTypeAST *) {}
    virtual bool visit(SimpleFunctionTypeAST *) { return false; }
    virtual void endVisit(SimpleFunctionTypeAST *) {}
    virtual bool visit(QualOpDefinitionArgumentAST *) { return false; }
    virtual void endVisit(QualOpDefinitionArgumentAST *) {}
    virtual bool visit(SimpleDefinitionArgumentAST *) { return false; }
    virtual void endVisit(SimpleDefinitionArgumentAST *) {}
    virtual bool visit(NotNullConstraintBodyAST *) { return false; }
    virtual void endVisit(NotNullConstraintBodyAST *) {}
    virtual bool visit(AttrColConstraintAST *) { return false; }
    virtual void endVisit(AttrColConstraintAST *) {}
    virtual bool visit(DeferColConstraintAST *) { return false; }
    virtual void endVisit(DeferColConstraintAST *) {}
    virtual bool visit(InitiallyColConstraintAST *) { return false; }
    virtual void endVisit(InitiallyColConstraintAST *) {}
    virtual bool visit(CollateColConstraintAST *) { return false; }
    virtual void endVisit(CollateColConstraintAST *) {}
    virtual bool visit(CreateGeneticOptionsAST *) { return false; }
    virtual void endVisit(CreateGeneticOptionsAST *) {}
    virtual bool visit(GenericOptionAST *) { return false; }
    virtual void endVisit(GenericOptionAST *) {}
    virtual bool visit(TypeAST *) { return false; }
    virtual void endVisit(TypeAST *) {}
    virtual bool visit(SimpleTypenameAST *) { return false; }
    virtual void endVisit(SimpleTypenameAST *) {}
    virtual bool visit(GenericTypenameAST *) { return false; }
    virtual void endVisit(GenericTypenameAST *) {}
    virtual bool visit(IntervalTypenameAST *) { return false; }
    virtual void endVisit(IntervalTypenameAST *) {}
    virtual bool visit(NumericTypenameAST *) { return false; }
    virtual void endVisit(NumericTypenameAST *) {}
    virtual bool visit(FloatTypenameAST *) { return false; }
    virtual void endVisit(FloatTypenameAST *) {}
    virtual bool visit(DoubleTypenameAST *) { return false; }
    virtual void endVisit(DoubleTypenameAST *) {}
    virtual bool visit(DecimalTypenameAST *) { return false; }
    virtual void endVisit(DecimalTypenameAST *) {}
    virtual bool visit(FloatPrecisionAST *) { return false; }
    virtual void endVisit(FloatPrecisionAST *) {}
    virtual bool visit(BitTypenameAST *) { return false; }
    virtual void endVisit(BitTypenameAST *) {}
    virtual bool visit(CharacterTypenameAST *) { return false; }
    virtual void endVisit(CharacterTypenameAST *) {}
    virtual bool visit(DatetimeTypenameAST *) { return false; }
    virtual void endVisit(DatetimeTypenameAST *) {}
    virtual bool visit(TimeZoneSpecAST *) { return false; }
    virtual void endVisit(TimeZoneSpecAST *) {}
    virtual bool visit(CharacterTypeSpecAST *) { return false; }
    virtual void endVisit(CharacterTypeSpecAST *) {}
    virtual bool visit(CharSetAST *) { return false; }
    virtual void endVisit(CharSetAST *) {}
    virtual bool visit(TypeModifiersAST *) { return false; }
    virtual void endVisit(TypeModifiersAST *) {}
    virtual bool visit(ArrBoundAST *) { return false; }
    virtual void endVisit(ArrBoundAST *) {}
    virtual bool visit(ExpressionAST *) { return false; }
    virtual void endVisit(ExpressionAST *) {}
    virtual bool visit(PrimaryExpressionAST *) { return false; }
    virtual void endVisit(PrimaryExpressionAST *) {}
    virtual bool visit(RowPrimaryExpressionAST *) { return false; }
    virtual void endVisit(RowPrimaryExpressionAST *) {}
    virtual bool visit(ArrayPrimaryExpressionAST *) { return false; }
    virtual void endVisit(ArrayPrimaryExpressionAST *) {}
    virtual bool visit(ArrayExprAST *) { return false; }
    virtual void endVisit(ArrayExprAST *) {}
    virtual bool visit(ExprListArrayExprAST *) { return false; }
    virtual void endVisit(ExprListArrayExprAST *) {}
    virtual bool visit(ArrListArrayExprAST *) { return false; }
    virtual void endVisit(ArrListArrayExprAST *) {}
    virtual bool visit(EmptyArrayExprAST *) { return false; }
    virtual void endVisit(EmptyArrayExprAST *) {}
    virtual bool visit(SubqueryPrimaryExpressionAST *) { return false; }
    virtual void endVisit(SubqueryPrimaryExpressionAST *) {}
    virtual bool visit(FunctionPrimaryExpressionAST *) { return false; }
    virtual void endVisit(FunctionPrimaryExpressionAST *) {}
    virtual bool visit(ColRefPrimaryExpressionAST *) { return false; }
    virtual void endVisit(ColRefPrimaryExpressionAST *) {}
    virtual bool visit(ConstPrimaryExpressionAST *) { return false; }
    virtual void endVisit(ConstPrimaryExpressionAST *) {}
    virtual bool visit(ParamPrimaryExpressionAST *) { return false; }
    virtual void endVisit(ParamPrimaryExpressionAST *) {}
    virtual bool visit(CompoundPrimaryExpressionAST *) { return false; }
    virtual void endVisit(CompoundPrimaryExpressionAST *) {}
    virtual bool visit(CasePrimaryExpressionAST *) { return false; }
    virtual void endVisit(CasePrimaryExpressionAST *) {}
    virtual bool visit(CaseExprAST *) { return false; }
    virtual void endVisit(CaseExprAST *) {}
    virtual bool visit(CaseArgAST *) { return false; }
    virtual void endVisit(CaseArgAST *) {}
    virtual bool visit(CaseWhenClauseAST *) { return false; }
    virtual void endVisit(CaseWhenClauseAST *) {}
    virtual bool visit(CaseDefaultClauseAST *) { return false; }
    virtual void endVisit(CaseDefaultClauseAST *) {}
    virtual bool visit(ConstantAST *) { return false; }
    virtual void endVisit(ConstantAST *) {}
    virtual bool visit(SimpleConstantAST *) { return false; }
    virtual void endVisit(SimpleConstantAST *) {}
    virtual bool visit(FuncNameConstantAST *) { return false; }
    virtual void endVisit(FuncNameConstantAST *) {}
    virtual bool visit(IntervalConstantAST *) { return false; }
    virtual void endVisit(IntervalConstantAST *) {}
    virtual bool visit(TypenameConstantAST *) { return false; }
    virtual void endVisit(TypenameConstantAST *) {}
    virtual bool visit(TypecastExpressionAST *) { return false; }
    virtual void endVisit(TypecastExpressionAST *) {}
    virtual bool visit(CollateExpressionAST *) { return false; }
    virtual void endVisit(CollateExpressionAST *) {}
    virtual bool visit(AtTimeZoneExpressionAST *) { return false; }
    virtual void endVisit(AtTimeZoneExpressionAST *) {}
    virtual bool visit(UnaryExpressionAST *) { return false; }
    virtual void endVisit(UnaryExpressionAST *) {}
    virtual bool visit(BinaryExpressionAST *) { return false; }
    virtual void endVisit(BinaryExpressionAST *) {}
    virtual bool visit(CommonBinaryExpressionAST *) { return false; }
    virtual void endVisit(CommonBinaryExpressionAST *) {}
    virtual bool visit(CommonUnaryExpresionAST *) { return false; }
    virtual void endVisit(CommonUnaryExpresionAST *) {}
    virtual bool visit(CommonPostfixExpressionAST *) { return false; }
    virtual void endVisit(CommonPostfixExpressionAST *) {}
    virtual bool visit(LikeExpressionAST *) { return false; }
    virtual void endVisit(LikeExpressionAST *) {}
    virtual bool visit(SimilarToExpressionAST *) { return false; }
    virtual void endVisit(SimilarToExpressionAST *) {}
    virtual bool visit(IsExpressionAST *) { return false; }
    virtual void endVisit(IsExpressionAST *) {}
    virtual bool visit(IsDdistinctExpressionAST *) { return false; }
    virtual void endVisit(IsDdistinctExpressionAST *) {}
    virtual bool visit(IsTypeofExpressionAST *) { return false; }
    virtual void endVisit(IsTypeofExpressionAST *) {}
    virtual bool visit(BetweenExpressionAST *) { return false; }
    virtual void endVisit(BetweenExpressionAST *) {}
    virtual bool visit(InExpressionAST *) { return false; }
    virtual void endVisit(InExpressionAST *) {}
    virtual bool visit(InValuesAST *) { return false; }
    virtual void endVisit(InValuesAST *) {}
    virtual bool visit(InValuesAsQueryAST *) { return false; }
    virtual void endVisit(InValuesAsQueryAST *) {}
    virtual bool visit(InValuesAsExprListAST *) { return false; }
    virtual void endVisit(InValuesAsExprListAST *) {}
    virtual bool visit(RelationExprSimpleAST *) { return false; }
    virtual void endVisit(RelationExprSimpleAST *) {}
    virtual bool visit(SubqueryExpressionAST *) { return false; }
    virtual void endVisit(SubqueryExpressionAST *) {}
    virtual bool visit(SubExpressionExpressionAST *) { return false; }
    virtual void endVisit(SubExpressionExpressionAST *) {}
    virtual bool visit(RowOverlapsExpressionAST *) { return false; }
    virtual void endVisit(RowOverlapsExpressionAST *) {}
    virtual bool visit(UniqueExpressionAST *) { return false; }
    virtual void endVisit(UniqueExpressionAST *) {}
    virtual bool visit(RowAST *) { return false; }
    virtual void endVisit(RowAST *) {}
    virtual bool visit(RelationExprOnlyAST *) { return false; }
    virtual void endVisit(RelationExprOnlyAST *) {}
    virtual bool visit(SubqueryOperatorAST *) { return false; }
    virtual void endVisit(SubqueryOperatorAST *) {}
    virtual bool visit(QualSubqueryOperatorAST *) { return false; }
    virtual void endVisit(QualSubqueryOperatorAST *) {}
    virtual bool visit(LikeSubqueryOperatorAST *) { return false; }
    virtual void endVisit(LikeSubqueryOperatorAST *) {}
    virtual bool visit(QualifiedOperatorAST *) { return false; }
    virtual void endVisit(QualifiedOperatorAST *) {}
    virtual bool visit(SimpleQualifiedOperatorAST *) { return false; }
    virtual void endVisit(SimpleQualifiedOperatorAST *) {}
    virtual bool visit(CompoundQualifiedOperatorAST *) { return false; }
    virtual void endVisit(CompoundQualifiedOperatorAST *) {}
    virtual bool visit(OperandAST *) { return false; }
    virtual void endVisit(OperandAST *) {}
    virtual bool visit(AnyIdentifierAST *) { return false; }
    virtual void endVisit(AnyIdentifierAST *) {}
    virtual bool visit(TableIdentifierAST *) { return false; }
    virtual void endVisit(TableIdentifierAST *) {}
    virtual bool visit(TriggerIdentifierAST *) { return false; }
    virtual void endVisit(TriggerIdentifierAST *) {}
    virtual bool visit(IndexIdentifierAST *) { return false; }
    virtual void endVisit(IndexIdentifierAST *) {}
    virtual bool visit(ConstraintIdentifierAST *) { return false; }
    virtual void endVisit(ConstraintIdentifierAST *) {}
    virtual bool visit(SequenceIdentifierAST *) { return false; }
    virtual void endVisit(SequenceIdentifierAST *) {}
    virtual bool visit(ColumnIdentifierAST *) { return false; }
    virtual void endVisit(ColumnIdentifierAST *) {}
    virtual bool visit(ColumnRefAST *) { return false; }
    virtual void endVisit(ColumnRefAST *) {}
    virtual bool visit(FunctionRefAST *) { return false; }
    virtual void endVisit(FunctionRefAST *) {}
    virtual bool visit(IndirectionAST *) { return false; }
    virtual void endVisit(IndirectionAST *) {}
    virtual bool visit(AttirbuteIndirectionAST *) { return false; }
    virtual void endVisit(AttirbuteIndirectionAST *) {}
    virtual bool visit(StarIndirectionAST *) { return false; }
    virtual void endVisit(StarIndirectionAST *) {}
    virtual bool visit(IndexingIndirectionAST *) { return false; }
    virtual void endVisit(IndexingIndirectionAST *) {}
};

}   // namespace Postgres

#endif // POSTGRESASTNOVISITOR_H
