#ifndef POSTGRESAST_H
#define POSTGRESAST_H

#include "TranslationUnit.h"
#include "BaseWalker.h"
#include "PostgresASTfwd.h"
#include "MemoryPool.h"

namespace Postgres {

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
    virtual bool isNull() const = 0;

    virtual StatementAST *asStatement() { return 0; }
    virtual RelationExprAST *asRelationExpr() { return 0; }
    virtual NameAST *asName() { return 0; }

    virtual TranslationUnitAST *asTranslationUnit() { return 0; }
    virtual DdlStatementAST *asDdlStatement() { return 0; }
    virtual DmlStatementAST *asDmlStatement() { return 0; }
    virtual UnknownStatementAST *asUnknownStatement() { return 0; }
    virtual CreateStatementAST *asCreateStatement() { return 0; }
    virtual CreateViewStmtAST *asCreateViewStmt() { return 0; }
    virtual WithCheckOptionClauseAST *asWithCheckOptionClause() { return 0; }
    virtual VacuumStmtAST *asVacuumStmt() { return 0; }
    virtual CommonVacuumStmtAST *asCommonVacuumStmt() { return 0; }
    virtual VacuumOptionAST *asVacuumOption() { return 0; }
    virtual VacuumTableStmtAST *asVacuumTableStmt() { return 0; }
    virtual VacuumAnalyzeStmtAST *asVacuumAnalyzeStmt() { return 0; }
    virtual TruncateStmtAST *asTruncateStmt() { return 0; }
    virtual TruncateRestartClauseAST *asTruncateRestartClause() { return 0; }
    virtual TransactionStmtAST *asTransactionStmt() { return 0; }
    virtual BeginTransactionStmtAST *asBeginTransactionStmt() { return 0; }
    virtual SavepointTransactionStmtAST *asSavepointTransactionStmt() { return 0; }
    virtual ReleaseTransactionStmtAST *asReleaseTransactionStmt() { return 0; }
    virtual CommitTransactionStmtAST *asCommitTransactionStmt() { return 0; }
    virtual RollbackTransactionStmtAST *asRollbackTransactionStmt() { return 0; }
    virtual PrepareTransactionStmtAST *asPrepareTransactionStmt() { return 0; }
    virtual CommitPrepTransactionStmtAST *asCommitPrepTransactionStmt() { return 0; }
    virtual RollPrepTransactionStmtAST *asRollPrepTransactionStmt() { return 0; }
    virtual SecLabelStmtAST *asSecLabelStmt() { return 0; }
    virtual ColSecLabelStmtAST *asColSecLabelStmt() { return 0; }
    virtual SequenceSecLabelStmtAST *asSequenceSecLabelStmt() { return 0; }
    virtual TableSecLabelStmtAST *asTableSecLabelStmt() { return 0; }
    virtual AggrSecLabelStmtAST *asAggrSecLabelStmt() { return 0; }
    virtual FuncSecLabelStmtAST *asFuncSecLabelStmt() { return 0; }
    virtual LargeObjSecLabelStmtAST *asLargeObjSecLabelStmt() { return 0; }
    virtual LangSecLabelStmtAST *asLangSecLabelStmt() { return 0; }
    virtual SchemaSecLabelStmtAST *asSchemaSecLabelStmt() { return 0; }
    virtual RoleSecLabelStmtAST *asRoleSecLabelStmt() { return 0; }
    virtual CommonSecLabelStmtAST *asCommonSecLabelStmt() { return 0; }
    virtual SecurityProviderAST *asSecurityProvider() { return 0; }
    virtual CommonSecurityTypeAST *asCommonSecurityType() { return 0; }
    virtual RenameStmtAST *asRenameStmt() { return 0; }
    virtual RenameTypeStmtAST *asRenameTypeStmt() { return 0; }
    virtual RenameTypeAttrStmtAST *asRenameTypeAttrStmt() { return 0; }
    virtual RenameRuleStmtAST *asRenameRuleStmt() { return 0; }
    virtual RenameTriggerStmtAST *asRenameTriggerStmt() { return 0; }
    virtual RenameSchemaStmtAST *asRenameSchemaStmt() { return 0; }
    virtual RenameServerStmtAST *asRenameServerStmt() { return 0; }
    virtual RenameEvtTrgStmtAST *asRenameEvtTrgStmt() { return 0; }
    virtual RenameRoleStmtAST *asRenameRoleStmt() { return 0; }
    virtual RenameTablespaceStmtAST *asRenameTablespaceStmt() { return 0; }
    virtual ChgReloptsTablespaceStmtAST *asChgReloptsTablespaceStmt() { return 0; }
    virtual RenameSearchStmtAST *asRenameSearchStmt() { return 0; }
    virtual RenameTableStmtAST *asRenameTableStmt() { return 0; }
    virtual RenameTableColStmtAST *asRenameTableColStmt() { return 0; }
    virtual RenameTableConstrStmtAST *asRenameTableConstrStmt() { return 0; }
    virtual RenameMatViewColStmtAST *asRenameMatViewColStmt() { return 0; }
    virtual RenameSequenceStmtAST *asRenameSequenceStmt() { return 0; }
    virtual RenameViewStmtAST *asRenameViewStmt() { return 0; }
    virtual RenameIndexStmtAST *asRenameIndexStmt() { return 0; }
    virtual RenameCollationStmtAST *asRenameCollationStmt() { return 0; }
    virtual RenameConversionStmtAST *asRenameConversionStmt() { return 0; }
    virtual RenameDomainStmtAST *asRenameDomainStmt() { return 0; }
    virtual RenameDomainConstrStmtAST *asRenameDomainConstrStmt() { return 0; }
    virtual RenameDatabaseStmtAST *asRenameDatabaseStmt() { return 0; }
    virtual RenameFdwStmtAST *asRenameFdwStmt() { return 0; }
    virtual RenameFuncStmtAST *asRenameFuncStmt() { return 0; }
    virtual RenameOpUsingStmtAST *asRenameOpUsingStmt() { return 0; }
    virtual RenameGroupRoleStmtAST *asRenameGroupRoleStmt() { return 0; }
    virtual RenameLangStmtAST *asRenameLangStmt() { return 0; }
    virtual RenameAggrStmtAST *asRenameAggrStmt() { return 0; }
    virtual ReindexStmtAST *asReindexStmt() { return 0; }
    virtual ReindexTableStmtAST *asReindexTableStmt() { return 0; }
    virtual ReindexIndexStmtAST *asReindexIndexStmt() { return 0; }
    virtual ReindexSystemStmtAST *asReindexSystemStmt() { return 0; }
    virtual ReindexDatabaseStmtAST *asReindexDatabaseStmt() { return 0; }
    virtual PrepareStmtAST *asPrepareStmt() { return 0; }
    virtual PrepareTypeClauseAST *asPrepareTypeClause() { return 0; }
    virtual LockStmtAST *asLockStmt() { return 0; }
    virtual LockModeClauseAST *asLockModeClause() { return 0; }
    virtual LockTypeAST *asLockType() { return 0; }
    virtual DropStmtAST *asDropStmt() { return 0; }
    virtual DropAggrStmtAST *asDropAggrStmt() { return 0; }
    virtual DropFuncStmtAST *asDropFuncStmt() { return 0; }
    virtual DropOperStmtAST *asDropOperStmt() { return 0; }
    virtual LoadStmtAST *asLoadStmt() { return 0; }
    virtual NotifyStmtAST *asNotifyStmt() { return 0; }
    virtual NotifyPayloadAST *asNotifyPayload() { return 0; }
    virtual ListenStmtAST *asListenStmt() { return 0; }
    virtual UnlistenStmtAST *asUnlistenStmt() { return 0; }
    virtual CreateIndexStatementAST *asCreateIndexStatement() { return 0; }
    virtual GrantRoleStmtAST *asGrantRoleStmt() { return 0; }
    virtual RevokeRoleStmtAST *asRevokeRoleStmt() { return 0; }
    virtual WithAdminOptionAST *asWithAdminOption() { return 0; }
    virtual GrantedByClauseAST *asGrantedByClause() { return 0; }
    virtual GrantStmtAST *asGrantStmt() { return 0; }
    virtual RevokeStmtAST *asRevokeStmt() { return 0; }
    virtual PrivilegeTargetAST *asPrivilegeTarget() { return 0; }
    virtual TablePrivilegeTargetAST *asTablePrivilegeTarget() { return 0; }
    virtual SequencePrivilegeTargetAST *asSequencePrivilegeTarget() { return 0; }
    virtual FdwPrivilegeTargetAST *asFdwPrivilegeTarget() { return 0; }
    virtual ForeignSrvPrivilegeTargetAST *asForeignSrvPrivilegeTarget() { return 0; }
    virtual FuncPrivilegeTargetAST *asFuncPrivilegeTarget() { return 0; }
    virtual DbPrivilegeTargetAST *asDbPrivilegeTarget() { return 0; }
    virtual DomainPrivilegeTargetAST *asDomainPrivilegeTarget() { return 0; }
    virtual LangPrivilegeTargetAST *asLangPrivilegeTarget() { return 0; }
    virtual LargeObjPrivilegeTargetAST *asLargeObjPrivilegeTarget() { return 0; }
    virtual SchemaPrivilegeTargetAST *asSchemaPrivilegeTarget() { return 0; }
    virtual TblspacePrivilegeTargetAST *asTblspacePrivilegeTarget() { return 0; }
    virtual TypePrivilegeTargetAST *asTypePrivilegeTarget() { return 0; }
    virtual AllInSchPrivilegeTargetAST *asAllInSchPrivilegeTarget() { return 0; }
    virtual NumericAST *asNumeric() { return 0; }
    virtual FetchStmtAST *asFetchStmt() { return 0; }
    virtual FetchClauseAST *asFetchClause() { return 0; }
    virtual BaseExplainStmtAST *asBaseExplainStmt() { return 0; }
    virtual CommonExplainStmtAST *asCommonExplainStmt() { return 0; }
    virtual OptExplainStmtAST *asOptExplainStmt() { return 0; }
    virtual ExplainOptionAST *asExplainOption() { return 0; }
    virtual RefreshMatViewStmtAST *asRefreshMatViewStmt() { return 0; }
    virtual DropDbStmtAST *asDropDbStmt() { return 0; }
    virtual DropUserMappingStmtAST *asDropUserMappingStmt() { return 0; }
    virtual DropTableSpaceStmtAST *asDropTableSpaceStmt() { return 0; }
    virtual CommonDropStmtAST *asCommonDropStmt() { return 0; }
    virtual CommonDropTypeAST *asCommonDropType() { return 0; }
    virtual DropTableStmtAST *asDropTableStmt() { return 0; }
    virtual DropViewStmtAST *asDropViewStmt() { return 0; }
    virtual DropIndexStmtAST *asDropIndexStmt() { return 0; }
    virtual DropSchemaStmtAST *asDropSchemaStmt() { return 0; }
    virtual DropSequenceStmtAST *asDropSequenceStmt() { return 0; }
    virtual RuleStmtAST *asRuleStmt() { return 0; }
    virtual RuleActionsAST *asRuleActions() { return 0; }
    virtual NothingRuleActionsAST *asNothingRuleActions() { return 0; }
    virtual SingleRuleActionsAST *asSingleRuleActions() { return 0; }
    virtual MultiRuleActionsAST *asMultiRuleActions() { return 0; }
    virtual DropRuleStmtAST *asDropRuleStmt() { return 0; }
    virtual DropOwnedStmtAST *asDropOwnedStmt() { return 0; }
    virtual ReassignOwnedStmtAST *asReassignOwnedStmt() { return 0; }
    virtual DropGroupStmtAST *asDropGroupStmt() { return 0; }
    virtual DropForeignServerStmtAST *asDropForeignServerStmt() { return 0; }
    virtual DropFdwStmtAST *asDropFdwStmt() { return 0; }
    virtual DoStmtAST *asDoStmt() { return 0; }
    virtual DoStmtItemAST *asDoStmtItem() { return 0; }
    virtual DoStmtCmdItemAST *asDoStmtCmdItem() { return 0; }
    virtual DoStmtLangItemAST *asDoStmtLangItem() { return 0; }
    virtual CreateOldAggregateStmtAST *asCreateOldAggregateStmt() { return 0; }
    virtual AggregateDefinitionAST *asAggregateDefinition() { return 0; }
    virtual AggrDefinitionAST *asAggrDefinition() { return 0; }
    virtual CreateAggregateStmtAST *asCreateAggregateStmt() { return 0; }
    virtual CreateOpStmtAST *asCreateOpStmt() { return 0; }
    virtual CreateTypeStmtAST *asCreateTypeStmt() { return 0; }
    virtual CreateTypeAsRecordStmtAST *asCreateTypeAsRecordStmt() { return 0; }
    virtual CreateTypeAsEnumStmtAST *asCreateTypeAsEnumStmt() { return 0; }
    virtual EnumValueAST *asEnumValue() { return 0; }
    virtual CreateTypeAsDefStmtAST *asCreateTypeAsDefStmt() { return 0; }
    virtual CreateTypeAsRangeStmtAST *asCreateTypeAsRangeStmt() { return 0; }
    virtual CreateSearchObjStmtAST *asCreateSearchObjStmt() { return 0; }
    virtual CreateCollationStmtAST *asCreateCollationStmt() { return 0; }
    virtual CreateCollationDefStmtAST *asCreateCollationDefStmt() { return 0; }
    virtual CreateCollationFromStmtAST *asCreateCollationFromStmt() { return 0; }
    virtual DeclareCursorStmtAST *asDeclareCursorStmt() { return 0; }
    virtual CursorHoldOptionAST *asCursorHoldOption() { return 0; }
    virtual DeallocateStmtAST *asDeallocateStmt() { return 0; }
    virtual CreateDbStmtAST *asCreateDbStmt() { return 0; }
    virtual CreateDbOptionAST *asCreateDbOption() { return 0; }
    virtual NamedCreateDbOptionAST *asNamedCreateDbOption() { return 0; }
    virtual ValDefCreateDbOptionAST *asValDefCreateDbOption() { return 0; }
    virtual CreateUserMappingStmtAST *asCreateUserMappingStmt() { return 0; }
    virtual DropTrigStmtAST *asDropTrigStmt() { return 0; }
    virtual CreateConstrTrigStmtAST *asCreateConstrTrigStmt() { return 0; }
    virtual ConstrFromTableAST *asConstrFromTable() { return 0; }
    virtual CreateTrigStmtAST *asCreateTrigStmt() { return 0; }
    virtual TriggerExecuteClauseAST *asTriggerExecuteClause() { return 0; }
    virtual TriggerFuncArgAST *asTriggerFuncArg() { return 0; }
    virtual TriggerWhenAST *asTriggerWhen() { return 0; }
    virtual TriggerForSpecAST *asTriggerForSpec() { return 0; }
    virtual TriggerEventAST *asTriggerEvent() { return 0; }
    virtual TriggerActionTimeAST *asTriggerActionTime() { return 0; }
    virtual CreateTableSpaceStmtAST *asCreateTableSpaceStmt() { return 0; }
    virtual TableSpaceOwnerClauseAST *asTableSpaceOwnerClause() { return 0; }
    virtual CreateTableAsExecStmtAST *asCreateTableAsExecStmt() { return 0; }
    virtual ExecuteStmtAST *asExecuteStmt() { return 0; }
    virtual ExecuteParamClauseAST *asExecuteParamClause() { return 0; }
    virtual CreateTableStmtAST *asCreateTableStmt() { return 0; }
    virtual CreateTableOptionsAST *asCreateTableOptions() { return 0; }
    virtual CreateTableClauseAST *asCreateTableClause() { return 0; }
    virtual CreateTableDirecClauseAST *asCreateTableDirecClause() { return 0; }
    virtual CreateTableOfClauseAST *asCreateTableOfClause() { return 0; }
    virtual TypedTableElementsAST *asTypedTableElements() { return 0; }
    virtual TypedTableElementAST *asTypedTableElement() { return 0; }
    virtual TypedTableConstrElementAST *asTypedTableConstrElement() { return 0; }
    virtual TypedTableColElementAST *asTypedTableColElement() { return 0; }
    virtual TableInheritClauseAST *asTableInheritClause() { return 0; }
    virtual CreateSchemaStmtAST *asCreateSchemaStmt() { return 0; }
    virtual CreateSchemaClauseAST *asCreateSchemaClause() { return 0; }
    virtual DropPLangStmtAST *asDropPLangStmt() { return 0; }
    virtual CreatePLangStmtAST *asCreatePLangStmt() { return 0; }
    virtual InlineHandlerClauseAST *asInlineHandlerClause() { return 0; }
    virtual ValidatorHandlerClauseAST *asValidatorHandlerClause() { return 0; }
    virtual DropOpClassStmtAST *asDropOpClassStmt() { return 0; }
    virtual DropOpFamilyStmtAST *asDropOpFamilyStmt() { return 0; }
    virtual AlterOpFamilyAddStmtAST *asAlterOpFamilyAddStmt() { return 0; }
    virtual AlterOpFamilyDropStmtAST *asAlterOpFamilyDropStmt() { return 0; }
    virtual OpClassDropAST *asOpClassDrop() { return 0; }
    virtual CreateOpFamilyStmtAST *asCreateOpFamilyStmt() { return 0; }
    virtual CreateOpClassStmtAST *asCreateOpClassStmt() { return 0; }
    virtual OpClassAST *asOpClass() { return 0; }
    virtual StorageOpClassAST *asStorageOpClass() { return 0; }
    virtual OperatorOpClassAST *asOperatorOpClass() { return 0; }
    virtual OpClassPurposeAST *asOpClassPurpose() { return 0; }
    virtual OpClassSearchPurposeAST *asOpClassSearchPurpose() { return 0; }
    virtual OpClassOrderPurposeAST *asOpClassOrderPurpose() { return 0; }
    virtual FunctionOpClassAST *asFunctionOpClass() { return 0; }
    virtual OpFamilyAST *asOpFamily() { return 0; }
    virtual CreateMatViewStmtAST *asCreateMatViewStmt() { return 0; }
    virtual CreateMvTargetAST *asCreateMvTarget() { return 0; }
    virtual WithReloptionsAST *asWithReloptions() { return 0; }
    virtual CreateFunctionStmtAST *asCreateFunctionStmt() { return 0; }
    virtual BaseFuncReturnAST *asBaseFuncReturn() { return 0; }
    virtual FuncTypeFuncReturnAST *asFuncTypeFuncReturn() { return 0; }
    virtual TableFuncReturnAST *asTableFuncReturn() { return 0; }
    virtual TableFuncColumnAST *asTableFuncColumn() { return 0; }
    virtual CreateFuncOptionAST *asCreateFuncOption() { return 0; }
    virtual CreateFuncAsOptionAST *asCreateFuncAsOption() { return 0; }
    virtual CreateFuncLangOptionAST *asCreateFuncLangOption() { return 0; }
    virtual CreateFuncWindowOptionAST *asCreateFuncWindowOption() { return 0; }
    virtual CreateFuncCommonOptionAST *asCreateFuncCommonOption() { return 0; }
    virtual FunctionAsClauseAST *asFunctionAsClause() { return 0; }
    virtual FuncArgsWithDefaultsAST *asFuncArgsWithDefaults() { return 0; }
    virtual FuncArgWithDefaultAST *asFuncArgWithDefault() { return 0; }
    virtual OrReplaceClauseAST *asOrReplaceClause() { return 0; }
    virtual CreateForeignTableStmtAST *asCreateForeignTableStmt() { return 0; }
    virtual TableElementAST *asTableElement() { return 0; }
    virtual ColumnDefTableElementAST *asColumnDefTableElement() { return 0; }
    virtual LikeTableElementAST *asLikeTableElement() { return 0; }
    virtual TableLikeClauseAST *asTableLikeClause() { return 0; }
    virtual TableLikeOptionAST *asTableLikeOption() { return 0; }
    virtual ConstraintTableElementAST *asConstraintTableElement() { return 0; }
    virtual CreateForeignServerStmtAST *asCreateForeignServerStmt() { return 0; }
    virtual ServerTypeClauseAST *asServerTypeClause() { return 0; }
    virtual CreateFdwStmtAST *asCreateFdwStmt() { return 0; }
    virtual CreateExtensionStmtAST *asCreateExtensionStmt() { return 0; }
    virtual CreateExtOptionAST *asCreateExtOption() { return 0; }
    virtual CreateExtSchemaOptionAST *asCreateExtSchemaOption() { return 0; }
    virtual CreateExtVerFromOptionAST *asCreateExtVerFromOption() { return 0; }
    virtual CreateConversionStmtAST *asCreateConversionStmt() { return 0; }
    virtual CreateCastStmtAST *asCreateCastStmt() { return 0; }
    virtual CastContextAST *asCastContext() { return 0; }
    virtual DropCastStmtAST *asDropCastStmt() { return 0; }
    virtual IfExistsAST *asIfExists() { return 0; }
    virtual DropAssertStmtAST *asDropAssertStmt() { return 0; }
    virtual CreateAssertStmtAST *asCreateAssertStmt() { return 0; }
    virtual CreateTableAsStmtAST *asCreateTableAsStmt() { return 0; }
    virtual CreateAsTargetAST *asCreateAsTarget() { return 0; }
    virtual TableSpaceClauseAST *asTableSpaceClause() { return 0; }
    virtual OnCommitOptionAST *asOnCommitOption() { return 0; }
    virtual CreateWithOptionsAST *asCreateWithOptions() { return 0; }
    virtual CreateWithRelOptionsAST *asCreateWithRelOptions() { return 0; }
    virtual CreateWithOidsOptionsAST *asCreateWithOidsOptions() { return 0; }
    virtual WithDataClauseAST *asWithDataClause() { return 0; }
    virtual CopyStmtAST *asCopyStmt() { return 0; }
    virtual TableCopyStmtAST *asTableCopyStmt() { return 0; }
    virtual WithOidsClauseAST *asWithOidsClause() { return 0; }
    virtual CopyDelimiterAST *asCopyDelimiter() { return 0; }
    virtual SelectCopyStmtAST *asSelectCopyStmt() { return 0; }
    virtual CopyOptionsAST *asCopyOptions() { return 0; }
    virtual NewCopyOptionsAST *asNewCopyOptions() { return 0; }
    virtual GenericCopyOptionAST *asGenericCopyOption() { return 0; }
    virtual GenericCopyOptionArgAST *asGenericCopyOptionArg() { return 0; }
    virtual GenericCopyOptionValArgAST *asGenericCopyOptionValArg() { return 0; }
    virtual GenericCopyOptionCmpArgAST *asGenericCopyOptionCmpArg() { return 0; }
    virtual GenericCopyOptionCmpArgItemAST *asGenericCopyOptionCmpArgItem() { return 0; }
    virtual OldCopyOptionsAST *asOldCopyOptions() { return 0; }
    virtual CopyOptionAST *asCopyOption() { return 0; }
    virtual ConstraintsSetStmtAST *asConstraintsSetStmt() { return 0; }
    virtual ConstraintsSetListAST *asConstraintsSetList() { return 0; }
    virtual AllConstraintsSetListAST *asAllConstraintsSetList() { return 0; }
    virtual CommonConstraintsSetListAST *asCommonConstraintsSetList() { return 0; }
    virtual CommentStmtAST *asCommentStmt() { return 0; }
    virtual LangCommentStmtAST *asLangCommentStmt() { return 0; }
    virtual CastCommentStmtAST *asCastCommentStmt() { return 0; }
    virtual LargeObjCommentStmtAST *asLargeObjCommentStmt() { return 0; }
    virtual OpUsingCommentStmtAST *asOpUsingCommentStmt() { return 0; }
    virtual ConstraintCommentStmtAST *asConstraintCommentStmt() { return 0; }
    virtual RuleCommentStmtAST *asRuleCommentStmt() { return 0; }
    virtual TriggerCommentStmtAST *asTriggerCommentStmt() { return 0; }
    virtual CommentOnNameAST *asCommentOnName() { return 0; }
    virtual OpCommentStmtAST *asOpCommentStmt() { return 0; }
    virtual FunctionCommentStmtAST *asFunctionCommentStmt() { return 0; }
    virtual AggrCommentStmtAST *asAggrCommentStmt() { return 0; }
    virtual CommonCommentStmtAST *asCommonCommentStmt() { return 0; }
    virtual CommentObjectAST *asCommentObject() { return 0; }
    virtual SearchCommentObjectAST *asSearchCommentObject() { return 0; }
    virtual EvtTriggerCommentObjectAST *asEvtTriggerCommentObject() { return 0; }
    virtual FdwCommentObjectAST *asFdwCommentObject() { return 0; }
    virtual RoleCommentObjectAST *asRoleCommentObject() { return 0; }
    virtual TableCommentObjectAST *asTableCommentObject() { return 0; }
    virtual SequenceCommentObjectAST *asSequenceCommentObject() { return 0; }
    virtual IndexCommentObjectAST *asIndexCommentObject() { return 0; }
    virtual SchemaCommentObjectAST *asSchemaCommentObject() { return 0; }
    virtual ColumnCommentObjectAST *asColumnCommentObject() { return 0; }
    virtual NamedCommentObjectAST *asNamedCommentObject() { return 0; }
    virtual AnyCommentObjectAST *asAnyCommentObject() { return 0; }
    virtual CommentTextClauseAST *asCommentTextClause() { return 0; }
    virtual ClusterStmtAST *asClusterStmt() { return 0; }
    virtual ClusterOnStmtAST *asClusterOnStmt() { return 0; }
    virtual ClusterUsingStmtAST *asClusterUsingStmt() { return 0; }
    virtual UsingIndexClauseAST *asUsingIndexClause() { return 0; }
    virtual ClosePortalStmtAST *asClosePortalStmt() { return 0; }
    virtual CloseCursorStmtAST *asCloseCursorStmt() { return 0; }
    virtual CloseAllStmtAST *asCloseAllStmt() { return 0; }
    virtual CheckPointStmtAST *asCheckPointStmt() { return 0; }
    virtual AnalyzeStmtAST *asAnalyzeStmt() { return 0; }
    virtual AlterUserMappingStmtAST *asAlterUserMappingStmt() { return 0; }
    virtual AuthIdentifierAST *asAuthIdentifier() { return 0; }
    virtual UserAuthIdentifierAST *asUserAuthIdentifier() { return 0; }
    virtual RoleAuthIdentifierAST *asRoleAuthIdentifier() { return 0; }
    virtual AlterTSConfigurationStmtAST *asAlterTSConfigurationStmt() { return 0; }
    virtual AlterTSConfCmdAST *asAlterTSConfCmd() { return 0; }
    virtual AlterTSConfAddAlterCmdAST *asAlterTSConfAddAlterCmd() { return 0; }
    virtual AlterTSConfDropCmdAST *asAlterTSConfDropCmd() { return 0; }
    virtual AlterTSDictionaryStmtAST *asAlterTSDictionaryStmt() { return 0; }
    virtual AlterCompositeTypeStmtAST *asAlterCompositeTypeStmt() { return 0; }
    virtual AlterTypeCommandAST *asAlterTypeCommand() { return 0; }
    virtual AlterTypeAddCommandAST *asAlterTypeAddCommand() { return 0; }
    virtual AlterTypeDropCommandAST *asAlterTypeDropCommand() { return 0; }
    virtual AlterTypeAlterCommandAST *asAlterTypeAlterCommand() { return 0; }
    virtual AlterSeqStmtAST *asAlterSeqStmt() { return 0; }
    virtual CreateSeqStmtAST *asCreateSeqStmt() { return 0; }
    virtual SequenceOptionAST *asSequenceOption() { return 0; }
    virtual SimpleSequenceOptionAST *asSimpleSequenceOption() { return 0; }
    virtual OwnedSequenceOptionAST *asOwnedSequenceOption() { return 0; }
    virtual TempClauseAST *asTempClause() { return 0; }
    virtual AlterOwnerStmtAST *asAlterOwnerStmt() { return 0; }
    virtual AlterEvtTriggerOwnerStmtAST *asAlterEvtTriggerOwnerStmt() { return 0; }
    virtual AlterServerOwnerStmtAST *asAlterServerOwnerStmt() { return 0; }
    virtual AlterFdwOwnerStmtAST *asAlterFdwOwnerStmt() { return 0; }
    virtual AlterSearchOwnerStmtAST *asAlterSearchOwnerStmt() { return 0; }
    virtual AlterTablespaceOwnerStmtAST *asAlterTablespaceOwnerStmt() { return 0; }
    virtual AlterTypeOwnerStmtAST *asAlterTypeOwnerStmt() { return 0; }
    virtual AlterSchemaOwnerStmtAST *asAlterSchemaOwnerStmt() { return 0; }
    virtual AlterOpUsingOwnerStmtAST *asAlterOpUsingOwnerStmt() { return 0; }
    virtual AlterOpOwnerStmtAST *asAlterOpOwnerStmt() { return 0; }
    virtual AlterLargeObjOwnerStmtAST *asAlterLargeObjOwnerStmt() { return 0; }
    virtual AlterLangOwnerStmtAST *asAlterLangOwnerStmt() { return 0; }
    virtual AlterFunctionOwnerStmtAST *asAlterFunctionOwnerStmt() { return 0; }
    virtual AlterDbOwnerStmtAST *asAlterDbOwnerStmt() { return 0; }
    virtual AlterCollationOwnerStmtAST *asAlterCollationOwnerStmt() { return 0; }
    virtual AlterConvOwnerStmtAST *asAlterConvOwnerStmt() { return 0; }
    virtual AlterDomainOwnerStmtAST *asAlterDomainOwnerStmt() { return 0; }
    virtual AlterAggrOwnerStmtAST *asAlterAggrOwnerStmt() { return 0; }
    virtual OwnerToClauseAST *asOwnerToClause() { return 0; }
    virtual AlterObjectSchemaStmtAST *asAlterObjectSchemaStmt() { return 0; }
    virtual AlterTypeSchemaStmtAST *asAlterTypeSchemaStmt() { return 0; }
    virtual AlterViewSchemaStmtAST *asAlterViewSchemaStmt() { return 0; }
    virtual AlterSeqSchemaStmtAST *asAlterSeqSchemaStmt() { return 0; }
    virtual AlterSearchSchemaStmtAST *asAlterSearchSchemaStmt() { return 0; }
    virtual AlterTableSchemaStmtAST *asAlterTableSchemaStmt() { return 0; }
    virtual AlterOpUsingSchemaStmtAST *asAlterOpUsingSchemaStmt() { return 0; }
    virtual AlterOpSchemaStmtAST *asAlterOpSchemaStmt() { return 0; }
    virtual AlterFunctionSchemaStmtAST *asAlterFunctionSchemaStmt() { return 0; }
    virtual AlterCollationSchemaStmtAST *asAlterCollationSchemaStmt() { return 0; }
    virtual AlterConvSchemaStmtAST *asAlterConvSchemaStmt() { return 0; }
    virtual AlterDomainSchemaStmtAST *asAlterDomainSchemaStmt() { return 0; }
    virtual AlterExtSchemaStmtAST *asAlterExtSchemaStmt() { return 0; }
    virtual AlterAggrFuncSchemaStmtAST *asAlterAggrFuncSchemaStmt() { return 0; }
    virtual SetSchemaClauseAST *asSetSchemaClause() { return 0; }
    virtual AlterFunctionStmtAST *asAlterFunctionStmt() { return 0; }
    virtual AlterFunctionOptionAST *asAlterFunctionOption() { return 0; }
    virtual CommonAlterFunctionOptAST *asCommonAlterFunctionOpt() { return 0; }
    virtual RestAlterFunctionOptAST *asRestAlterFunctionOpt() { return 0; }
    virtual AlterForeignServerStmtAST *asAlterForeignServerStmt() { return 0; }
    virtual ForeignServerVersionAST *asForeignServerVersion() { return 0; }
    virtual AlterFdwStmtAST *asAlterFdwStmt() { return 0; }
    virtual FdwOptionAST *asFdwOption() { return 0; }
    virtual SetFdwOptionAST *asSetFdwOption() { return 0; }
    virtual ResetFdwOptionAST *asResetFdwOption() { return 0; }
    virtual HandlerIdentifierAST *asHandlerIdentifier() { return 0; }
    virtual AlterExtensionCtxStmtAST *asAlterExtensionCtxStmt() { return 0; }
    virtual ExtensionCtxCmdAST *asExtensionCtxCmd() { return 0; }
    virtual OpExtensionCtxCmdAST *asOpExtensionCtxCmd() { return 0; }
    virtual OperArgTypesAST *asOperArgTypes() { return 0; }
    virtual LangExtensionCtxCmdAST *asLangExtensionCtxCmd() { return 0; }
    virtual FunctionExtensionCtxCmdAST *asFunctionExtensionCtxCmd() { return 0; }
    virtual FunctionWithArgTypesAST *asFunctionWithArgTypes() { return 0; }
    virtual FunctionArgTypesAST *asFunctionArgTypes() { return 0; }
    virtual FunctionArgAST *asFunctionArg() { return 0; }
    virtual SimpleExtensionCtxCmdAST *asSimpleExtensionCtxCmd() { return 0; }
    virtual CastExtensionCtxCmdAST *asCastExtensionCtxCmd() { return 0; }
    virtual AggrExtensionCtxCmdAST *asAggrExtensionCtxCmd() { return 0; }
    virtual AggrArgumentsAST *asAggrArguments() { return 0; }
    virtual ListAggrArgumentsAST *asListAggrArguments() { return 0; }
    virtual StarAggrArgumentsAST *asStarAggrArguments() { return 0; }
    virtual OpUsingExtensionCtxCmdAST *asOpUsingExtensionCtxCmd() { return 0; }
    virtual SchemaExtensionCtxCmdAST *asSchemaExtensionCtxCmd() { return 0; }
    virtual TrgEventExtensionCtxCmdAST *asTrgEventExtensionCtxCmd() { return 0; }
    virtual TxtSearchExtensionCtxCmdAST *asTxtSearchExtensionCtxCmd() { return 0; }
    virtual SequenceExtensionCtxCmdAST *asSequenceExtensionCtxCmd() { return 0; }
    virtual TableExtensionCtxCmdAST *asTableExtensionCtxCmd() { return 0; }
    virtual ViewExtensionCtxCmdAST *asViewExtensionCtxCmd() { return 0; }
    virtual FdwExtensionCtxCmdAST *asFdwExtensionCtxCmd() { return 0; }
    virtual TypeExtensionCtxCmdAST *asTypeExtensionCtxCmd() { return 0; }
    virtual ServerExtensionCtxCmdAST *asServerExtensionCtxCmd() { return 0; }
    virtual AlterExtensionStmtAST *asAlterExtensionStmt() { return 0; }
    virtual UpdateExtensionCmdAST *asUpdateExtensionCmd() { return 0; }
    virtual UpdateToExtensionCmdAST *asUpdateToExtensionCmd() { return 0; }
    virtual AlterEnumStmtAST *asAlterEnumStmt() { return 0; }
    virtual IfNotExistsAST *asIfNotExists() { return 0; }
    virtual AlterDomainStmtAST *asAlterDomainStmt() { return 0; }
    virtual AlterDomainAltDefStmtAST *asAlterDomainAltDefStmt() { return 0; }
    virtual AlterDomainAddConstrStmtAST *asAlterDomainAddConstrStmt() { return 0; }
    virtual AlterDomainDropConstrStmtAST *asAlterDomainDropConstrStmt() { return 0; }
    virtual AlterDomainValConstrStmtAST *asAlterDomainValConstrStmt() { return 0; }
    virtual AlterDomainChgNotNullStmtAST *asAlterDomainChgNotNullStmt() { return 0; }
    virtual CreateDomainStmtAST *asCreateDomainStmt() { return 0; }
    virtual AlterDefaultPrivilegesStmtAST *asAlterDefaultPrivilegesStmt() { return 0; }
    virtual DefACLOptionAST *asDefACLOption() { return 0; }
    virtual InSchemaDefACLOptionAST *asInSchemaDefACLOption() { return 0; }
    virtual ForRoleUserDefACLOptionAST *asForRoleUserDefACLOption() { return 0; }
    virtual DefACLActionAST *asDefACLAction() { return 0; }
    virtual RevokeDefACLActionAST *asRevokeDefACLAction() { return 0; }
    virtual GrantDefACLActionAST *asGrantDefACLAction() { return 0; }
    virtual PrivilegesAST *asPrivileges() { return 0; }
    virtual AllPrivilegesAST *asAllPrivileges() { return 0; }
    virtual PrivilegesAsListAST *asPrivilegesAsList() { return 0; }
    virtual CommonPrivilegeAST *asCommonPrivilege() { return 0; }
    virtual GranteeAST *asGrantee() { return 0; }
    virtual WithGrantOptionAST *asWithGrantOption() { return 0; }
    virtual AlterDatabaseStmtAST *asAlterDatabaseStmt() { return 0; }
    virtual AlterDatabaseSetStmtAST *asAlterDatabaseSetStmt() { return 0; }
    virtual AlterDatabaseChgOptStmtAST *asAlterDatabaseChgOptStmt() { return 0; }
    virtual AlterDbOptionAST *asAlterDbOption() { return 0; }
    virtual AlterDbConnLimitOptionAST *asAlterDbConnLimitOption() { return 0; }
    virtual AlterDatabaseSetTblspStmtAST *asAlterDatabaseSetTblspStmt() { return 0; }
    virtual CreateEventTrigStmtAST *asCreateEventTrigStmt() { return 0; }
    virtual EventTriggerWhenAST *asEventTriggerWhen() { return 0; }
    virtual EventTriggerValueAST *asEventTriggerValue() { return 0; }
    virtual AlterEventTrigStmtAST *asAlterEventTrigStmt() { return 0; }
    virtual EventTriggerCmdAST *asEventTriggerCmd() { return 0; }
    virtual CreateRoleStmtAST *asCreateRoleStmt() { return 0; }
    virtual AlterRoleStmtAST *asAlterRoleStmt() { return 0; }
    virtual OptRoleAST *asOptRole() { return 0; }
    virtual OptRoleWithRoleListAST *asOptRoleWithRoleList() { return 0; }
    virtual OptRoleStaticAST *asOptRoleStatic() { return 0; }
    virtual RoleIdAST *asRoleId() { return 0; }
    virtual DatabaseNameAST *asDatabaseName() { return 0; }
    virtual InDatabaseClauseAST *asInDatabaseClause() { return 0; }
    virtual AlterRoleSetStmtAST *asAlterRoleSetStmt() { return 0; }
    virtual AlterAnyRoleSetStmtAST *asAlterAnyRoleSetStmt() { return 0; }
    virtual AlterAllRolesSetStmtAST *asAlterAllRolesSetStmt() { return 0; }
    virtual SetResetClauseAST *asSetResetClause() { return 0; }
    virtual SetResetClauseAsSetAST *asSetResetClauseAsSet() { return 0; }
    virtual SetResetClauseAsVarResetAST *asSetResetClauseAsVarReset() { return 0; }
    virtual SetRestAST *asSetRest() { return 0; }
    virtual SetRestTransactionAST *asSetRestTransaction() { return 0; }
    virtual SetRestVarNameToAST *asSetRestVarNameTo() { return 0; }
    virtual SetRestVarNameToValuesAST *asSetRestVarNameToValues() { return 0; }
    virtual SetRestZoneValueAST *asSetRestZoneValue() { return 0; }
    virtual SetRestCatalogAST *asSetRestCatalog() { return 0; }
    virtual SetRestSchemaAST *asSetRestSchema() { return 0; }
    virtual SetRestEncodingAST *asSetRestEncoding() { return 0; }
    virtual SetRestRoleAST *asSetRestRole() { return 0; }
    virtual SetRestSessionAuthAST *asSetRestSessionAuth() { return 0; }
    virtual SetRestXmlOptionAST *asSetRestXmlOption() { return 0; }
    virtual SetRestTransactioSnapshotAST *asSetRestTransactioSnapshot() { return 0; }
    virtual VariableShowStmtAST *asVariableShowStmt() { return 0; }
    virtual VarShowStmtAST *asVarShowStmt() { return 0; }
    virtual TimeZoneShowStmtAST *asTimeZoneShowStmt() { return 0; }
    virtual IsoLevelShowStmtAST *asIsoLevelShowStmt() { return 0; }
    virtual SessionAuthShowStmtAST *asSessionAuthShowStmt() { return 0; }
    virtual AllShowStmtAST *asAllShowStmt() { return 0; }
    virtual VariableSetStmtAST *asVariableSetStmt() { return 0; }
    virtual VariableResetStmtAST *asVariableResetStmt() { return 0; }
    virtual SetRestResetVarNameAST *asSetRestResetVarName() { return 0; }
    virtual SetRestResetTimeZoneAST *asSetRestResetTimeZone() { return 0; }
    virtual SetRestResetTransLevelAST *asSetRestResetTransLevel() { return 0; }
    virtual SetRestResetAuthorizationAST *asSetRestResetAuthorization() { return 0; }
    virtual SetRestResetAllAST *asSetRestResetAll() { return 0; }
    virtual IsoLevelAST *asIsoLevel() { return 0; }
    virtual TransactionModeAST *asTransactionMode() { return 0; }
    virtual TransactionModeIsoLevelAST *asTransactionModeIsoLevel() { return 0; }
    virtual TransactionModeReadWriteAST *asTransactionModeReadWrite() { return 0; }
    virtual TransactionModeDeferAST *asTransactionModeDefer() { return 0; }
    virtual IntervalSecondAST *asIntervalSecond() { return 0; }
    virtual IntervalAST *asInterval() { return 0; }
    virtual IntervalSimpleAST *asIntervalSimple() { return 0; }
    virtual IntervalSimpleToAST *asIntervalSimpleTo() { return 0; }
    virtual IntervalSimpleToSecondAST *asIntervalSimpleToSecond() { return 0; }
    virtual VarValueAST *asVarValue() { return 0; }
    virtual ZoneValueAST *asZoneValue() { return 0; }
    virtual ZoneValueSimpleAST *asZoneValueSimple() { return 0; }
    virtual ZoneValueIntervalAST *asZoneValueInterval() { return 0; }
    virtual DropRoleStmtAST *asDropRoleStmt() { return 0; }
    virtual CreateGroupStmtAST *asCreateGroupStmt() { return 0; }
    virtual AlterGroupStmtAST *asAlterGroupStmt() { return 0; }
    virtual DiscardStmtAST *asDiscardStmt() { return 0; }
    virtual DropStatementAST *asDropStatement() { return 0; }
    virtual TruncateStatementAST *asTruncateStatement() { return 0; }
    virtual AlterStmtAST *asAlterStmt() { return 0; }
    virtual AlterIndexStmtAST *asAlterIndexStmt() { return 0; }
    virtual AlterSequenceStmtAST *asAlterSequenceStmt() { return 0; }
    virtual AlterTableStmtAST *asAlterTableStmt() { return 0; }
    virtual AlterForeignTableStmtAST *asAlterForeignTableStmt() { return 0; }
    virtual AlterViewStmtAST *asAlterViewStmt() { return 0; }
    virtual UpdateStmtAST *asUpdateStmt() { return 0; }
    virtual SetClauseAST *asSetClause() { return 0; }
    virtual SingleSetClauseAST *asSingleSetClause() { return 0; }
    virtual MultipleSetClauseAST *asMultipleSetClause() { return 0; }
    virtual InsertStmtAST *asInsertStmt() { return 0; }
    virtual InsertRestAST *asInsertRest() { return 0; }
    virtual CommonInsertRestAST *asCommonInsertRest() { return 0; }
    virtual DefaultValuesInsertRestAST *asDefaultValuesInsertRest() { return 0; }
    virtual DeleteStmtAST *asDeleteStmt() { return 0; }
    virtual ReturningClauseAST *asReturningClause() { return 0; }
    virtual WhereOrCurrentClauseAST *asWhereOrCurrentClause() { return 0; }
    virtual WhereNoCurrentClauseAST *asWhereNoCurrentClause() { return 0; }
    virtual CurrenWhereClauseAST *asCurrenWhereClause() { return 0; }
    virtual DelUsingClauseAST *asDelUsingClause() { return 0; }
    virtual RelationAliasExprAST *asRelationAliasExpr() { return 0; }
    virtual SelectStmtAST *asSelectStmt() { return 0; }
    virtual SelectWithParensAST *asSelectWithParens() { return 0; }
    virtual SelectNoParensAST *asSelectNoParens() { return 0; }
    virtual WithClauseAST *asWithClause() { return 0; }
    virtual CommonTableExprAST *asCommonTableExpr() { return 0; }
    virtual CompoundColumnNamesAST *asCompoundColumnNames() { return 0; }
    virtual SortClauseAST *asSortClause() { return 0; }
    virtual OrderExprAST *asOrderExpr() { return 0; }
    virtual UsingOrderExprAST *asUsingOrderExpr() { return 0; }
    virtual CommonOrderExprAST *asCommonOrderExpr() { return 0; }
    virtual LockingLimitClauseAST *asLockingLimitClause() { return 0; }
    virtual LockingFirstLimitClauseAST *asLockingFirstLimitClause() { return 0; }
    virtual LockingLastLimitClauseAST *asLockingLastLimitClause() { return 0; }
    virtual LimitClauseAST *asLimitClause() { return 0; }
    virtual LimitOffsetClauseAST *asLimitOffsetClause() { return 0; }
    virtual OffsetLimitClauseAST *asOffsetLimitClause() { return 0; }
    virtual OffsetAST *asOffset() { return 0; }
    virtual LimitAST *asLimit() { return 0; }
    virtual CommonLimitAST *asCommonLimit() { return 0; }
    virtual FetchLimitAST *asFetchLimit() { return 0; }
    virtual FetchFirstValueAST *asFetchFirstValue() { return 0; }
    virtual ConstFetchFirstValueAST *asConstFetchFirstValue() { return 0; }
    virtual CompoundFetchFirstValueAST *asCompoundFetchFirstValue() { return 0; }
    virtual LimitValueAST *asLimitValue() { return 0; }
    virtual AllLimitValueAST *asAllLimitValue() { return 0; }
    virtual ExprLimitValueAST *asExprLimitValue() { return 0; }
    virtual ForLockingClauseAST *asForLockingClause() { return 0; }
    virtual ReadOnlyForLockingClauseAST *asReadOnlyForLockingClause() { return 0; }
    virtual CommonForLockingClauseAST *asCommonForLockingClause() { return 0; }
    virtual LockingExprAST *asLockingExpr() { return 0; }
    virtual LockingStrengthAST *asLockingStrength() { return 0; }
    virtual LockingRelsAST *asLockingRels() { return 0; }
    virtual SelectClauseAST *asSelectClause() { return 0; }
    virtual CompoundSelectClauseAST *asCompoundSelectClause() { return 0; }
    virtual CoreSimpleSelectAST *asCoreSimpleSelect() { return 0; }
    virtual SimpleSelectAST *asSimpleSelect() { return 0; }
    virtual ValuesSimpleSelectAST *asValuesSimpleSelect() { return 0; }
    virtual TableSimpleSelectAST *asTableSimpleSelect() { return 0; }
    virtual CompositeSimpleSelectAST *asCompositeSimpleSelect() { return 0; }
    virtual SelectCoreAST *asSelectCore() { return 0; }
    virtual WindowClauseAST *asWindowClause() { return 0; }
    virtual WindowDefinitionAST *asWindowDefinition() { return 0; }
    virtual HavingClauseAST *asHavingClause() { return 0; }
    virtual GroupByClauseAST *asGroupByClause() { return 0; }
    virtual WhereClauseAST *asWhereClause() { return 0; }
    virtual FromClauseAST *asFromClause() { return 0; }
    virtual TableRefAST *asTableRef() { return 0; }
    virtual JoinedTableRefAST *asJoinedTableRef() { return 0; }
    virtual JoinedTableAST *asJoinedTable() { return 0; }
    virtual CommonJoinedTableAST *asCommonJoinedTable() { return 0; }
    virtual JoinTypeAST *asJoinType() { return 0; }
    virtual JoinConditionAST *asJoinCondition() { return 0; }
    virtual OnJoinConditionAST *asOnJoinCondition() { return 0; }
    virtual UsingJoinConditionAST *asUsingJoinCondition() { return 0; }
    virtual CrossJoinedTableAST *asCrossJoinedTable() { return 0; }
    virtual CompoundJoinedTableAST *asCompoundJoinedTable() { return 0; }
    virtual CompoundTableRefAST *asCompoundTableRef() { return 0; }
    virtual FuncionTableRefAST *asFuncionTableRef() { return 0; }
    virtual FuncAliasClauseAST *asFuncAliasClause() { return 0; }
    virtual FuncFuncAliasClauseAST *asFuncFuncAliasClause() { return 0; }
    virtual TableFuncAliasClauseAST *asTableFuncAliasClause() { return 0; }
    virtual TableFuncElementAST *asTableFuncElement() { return 0; }
    virtual CollateClauseAST *asCollateClause() { return 0; }
    virtual FunctionExpressionAST *asFunctionExpression() { return 0; }
    virtual XmlserializeFunctionExprAST *asXmlserializeFunctionExpr() { return 0; }
    virtual XmlrootFunctionExprAST *asXmlrootFunctionExpr() { return 0; }
    virtual XmlRootVersionAST *asXmlRootVersion() { return 0; }
    virtual ExprXmlRootVersionAST *asExprXmlRootVersion() { return 0; }
    virtual NoValueXmlRootVersionAST *asNoValueXmlRootVersion() { return 0; }
    virtual XmlRootStandaloneAST *asXmlRootStandalone() { return 0; }
    virtual XmlpiFunctionExprAST *asXmlpiFunctionExpr() { return 0; }
    virtual XmlparseFunctionExprAST *asXmlparseFunctionExpr() { return 0; }
    virtual XmlWhitespaceOptionAST *asXmlWhitespaceOption() { return 0; }
    virtual XmlforestFunctionExprAST *asXmlforestFunctionExpr() { return 0; }
    virtual XmlexistsFunctionExprAST *asXmlexistsFunctionExpr() { return 0; }
    virtual XmlExistsArgumentAST *asXmlExistsArgument() { return 0; }
    virtual XmlelementFunctionExprAST *asXmlelementFunctionExpr() { return 0; }
    virtual XmlAttributesAST *asXmlAttributes() { return 0; }
    virtual XmlAttributeAST *asXmlAttribute() { return 0; }
    virtual CoalesceFunctionExprAST *asCoalesceFunctionExpr() { return 0; }
    virtual GreatestFunctionExprAST *asGreatestFunctionExpr() { return 0; }
    virtual LeastFunctionExprAST *asLeastFunctionExpr() { return 0; }
    virtual XmlconcatFunctionExprAST *asXmlconcatFunctionExpr() { return 0; }
    virtual NullifFunctionExprAST *asNullifFunctionExpr() { return 0; }
    virtual TrimFunctionExprAST *asTrimFunctionExpr() { return 0; }
    virtual TrimExprAST *asTrimExpr() { return 0; }
    virtual TraetFunctionExprAST *asTraetFunctionExpr() { return 0; }
    virtual SubstringFunctionExprAST *asSubstringFunctionExpr() { return 0; }
    virtual SubstringExprAST *asSubstringExpr() { return 0; }
    virtual StandardSubstringExprAST *asStandardSubstringExpr() { return 0; }
    virtual ListSubstringExprAST *asListSubstringExpr() { return 0; }
    virtual PositionFunctionExprAST *asPositionFunctionExpr() { return 0; }
    virtual OverlayFunctionExprAST *asOverlayFunctionExpr() { return 0; }
    virtual PositionExprAST *asPositionExpr() { return 0; }
    virtual OverlayExprAST *asOverlayExpr() { return 0; }
    virtual OverlayPlacingAST *asOverlayPlacing() { return 0; }
    virtual ExtractFunctionExprAST *asExtractFunctionExpr() { return 0; }
    virtual SubstringFromAST *asSubstringFrom() { return 0; }
    virtual SubstringForAST *asSubstringFor() { return 0; }
    virtual ExtractExprAST *asExtractExpr() { return 0; }
    virtual CastFunctionExprAST *asCastFunctionExpr() { return 0; }
    virtual CollationFunctionExprAST *asCollationFunctionExpr() { return 0; }
    virtual SimpleFunctionExprAST *asSimpleFunctionExpr() { return 0; }
    virtual CommonFunctionExprAST *asCommonFunctionExpr() { return 0; }
    virtual FunctionArgExprAST *asFunctionArgExpr() { return 0; }
    virtual OverClauseAST *asOverClause() { return 0; }
    virtual SimpleOverClauseAST *asSimpleOverClause() { return 0; }
    virtual WindowOverClauseAST *asWindowOverClause() { return 0; }
    virtual WindowSpecificationAST *asWindowSpecification() { return 0; }
    virtual PartitionClauseAST *asPartitionClause() { return 0; }
    virtual FrameClauseAST *asFrameClause() { return 0; }
    virtual FrameExtentAST *asFrameExtent() { return 0; }
    virtual BoundFrameExtentAST *asBoundFrameExtent() { return 0; }
    virtual BetweenFrameExtentAST *asBetweenFrameExtent() { return 0; }
    virtual FrameBoundAST *asFrameBound() { return 0; }
    virtual SimpleFrameBoundAST *asSimpleFrameBound() { return 0; }
    virtual ExprFrameBoundAST *asExprFrameBound() { return 0; }
    virtual DirectTableRefAST *asDirectTableRef() { return 0; }
    virtual TableAliasClauseAST *asTableAliasClause() { return 0; }
    virtual DistinctClauseAST *asDistinctClause() { return 0; }
    virtual SimpleDistinctClauseAST *asSimpleDistinctClause() { return 0; }
    virtual ExprListDistinctClauseAST *asExprListDistinctClause() { return 0; }
    virtual SelectTargetAST *asSelectTarget() { return 0; }
    virtual StarSelectTargetAST *asStarSelectTarget() { return 0; }
    virtual ExprSelectTargetAST *asExprSelectTarget() { return 0; }
    virtual IntoClauseAST *asIntoClause() { return 0; }
    virtual OptTempTableNameAST *asOptTempTableName() { return 0; }
    virtual ValuesClauseAST *asValuesClause() { return 0; }
    virtual ColumnExprRowAST *asColumnExprRow() { return 0; }
    virtual ColumnExpressionAST *asColumnExpression() { return 0; }
    virtual DefaultColumnExpressionAST *asDefaultColumnExpression() { return 0; }
    virtual ExprColumnExpressionAST *asExprColumnExpression() { return 0; }
    virtual AlterTableCmdAST *asAlterTableCmd() { return 0; }
    virtual AlterTableGenericCmdAST *asAlterTableGenericCmd() { return 0; }
    virtual AlterGenericOptionAST *asAlterGenericOption() { return 0; }
    virtual SetAddAlterGenericOptionAST *asSetAddAlterGenericOption() { return 0; }
    virtual DropAlterGenericOptionAST *asDropAlterGenericOption() { return 0; }
    virtual AlterTableSetReloptCmdAST *asAlterTableSetReloptCmd() { return 0; }
    virtual AlterTableTablespaceCmdAST *asAlterTableTablespaceCmd() { return 0; }
    virtual AlterTableOwnerCmdAST *asAlterTableOwnerCmd() { return 0; }
    virtual AlterTableNotOfCmdAST *asAlterTableNotOfCmd() { return 0; }
    virtual AlterTableOfCmdAST *asAlterTableOfCmd() { return 0; }
    virtual AlterTableInheritCmdAST *asAlterTableInheritCmd() { return 0; }
    virtual AlterTableTriggerCmdAST *asAlterTableTriggerCmd() { return 0; }
    virtual AlterTableRuleCmdAST *asAlterTableRuleCmd() { return 0; }
    virtual WildcardAST *asWildcard() { return 0; }
    virtual NameWildcardAST *asNameWildcard() { return 0; }
    virtual AllOrUserWildcardAST *asAllOrUserWildcard() { return 0; }
    virtual AlterTableClusterOffCmdAST *asAlterTableClusterOffCmd() { return 0; }
    virtual AlterTableClusterOnCmdAST *asAlterTableClusterOnCmd() { return 0; }
    virtual AlterTableSetOidsCmdAST *asAlterTableSetOidsCmd() { return 0; }
    virtual AlterTableDropConstrCmdAST *asAlterTableDropConstrCmd() { return 0; }
    virtual AlterTableValConstrCmdAST *asAlterTableValConstrCmd() { return 0; }
    virtual AlterTableAddConstrCmdAST *asAlterTableAddConstrCmd() { return 0; }
    virtual AlterTableSetTypeCmdAST *asAlterTableSetTypeCmd() { return 0; }
    virtual SetDataAST *asSetData() { return 0; }
    virtual UsingClauseAST *asUsingClause() { return 0; }
    virtual AlterTableSetStorageCmdAST *asAlterTableSetStorageCmd() { return 0; }
    virtual AlterTableChgReloptCmdAST *asAlterTableChgReloptCmd() { return 0; }
    virtual RelOptionsAST *asRelOptions() { return 0; }
    virtual RelOptionAST *asRelOption() { return 0; }
    virtual AlterTableSetStatCmdAST *asAlterTableSetStatCmd() { return 0; }
    virtual AlterTableChgNotNullCmdAST *asAlterTableChgNotNullCmd() { return 0; }
    virtual AlterTableChgDefColCmdAST *asAlterTableChgDefColCmd() { return 0; }
    virtual AlterDefaultColumnAST *asAlterDefaultColumn() { return 0; }
    virtual SetAlterDefaultColumnAST *asSetAlterDefaultColumn() { return 0; }
    virtual DropAlterDefaultColumnAST *asDropAlterDefaultColumn() { return 0; }
    virtual AlterTableDropColCmdAST *asAlterTableDropColCmd() { return 0; }
    virtual AlterTableAddColCmdAST *asAlterTableAddColCmd() { return 0; }
    virtual ColumnDefAST *asColumnDef() { return 0; }
    virtual ConstraintAST *asConstraint() { return 0; }
    virtual TableConstraintAST *asTableConstraint() { return 0; }
    virtual TableConstraintBodyAST *asTableConstraintBody() { return 0; }
    virtual FkTableConstraintAST *asFkTableConstraint() { return 0; }
    virtual ExcludeTableConstraintAST *asExcludeTableConstraint() { return 0; }
    virtual AccessMethodClauseAST *asAccessMethodClause() { return 0; }
    virtual ExclusionWhereClauseAST *asExclusionWhereClause() { return 0; }
    virtual ExclusionConstraintAST *asExclusionConstraint() { return 0; }
    virtual IndexElementAST *asIndexElement() { return 0; }
    virtual ColIndexElementAST *asColIndexElement() { return 0; }
    virtual FuncExprIndexElementAST *asFuncExprIndexElement() { return 0; }
    virtual ExprIndexElementAST *asExprIndexElement() { return 0; }
    virtual ClassClauseAST *asClassClause() { return 0; }
    virtual IndPrimKeyTableConstraintAST *asIndPrimKeyTableConstraint() { return 0; }
    virtual PrimKeyTableConstraintAST *asPrimKeyTableConstraint() { return 0; }
    virtual IndUniqueTableConstraintAST *asIndUniqueTableConstraint() { return 0; }
    virtual ExistingIndexAST *asExistingIndex() { return 0; }
    virtual UniqueTableConstraintAST *asUniqueTableConstraint() { return 0; }
    virtual CheckTableConstraintAST *asCheckTableConstraint() { return 0; }
    virtual ConstraintAttributeAST *asConstraintAttribute() { return 0; }
    virtual DeferConstraintAttrAST *asDeferConstraintAttr() { return 0; }
    virtual InitiallyConstraintAttrAST *asInitiallyConstraintAttr() { return 0; }
    virtual NotValidConstraintAttrAST *asNotValidConstraintAttr() { return 0; }
    virtual NoInheritConstraintAttrAST *asNoInheritConstraintAttr() { return 0; }
    virtual ColumnConstraintAST *asColumnConstraint() { return 0; }
    virtual CommonColConstraintAST *asCommonColConstraint() { return 0; }
    virtual ConstraintBodyAST *asConstraintBody() { return 0; }
    virtual RefConstraintBodyAST *asRefConstraintBody() { return 0; }
    virtual RefColumnsAST *asRefColumns() { return 0; }
    virtual KeyMatchAST *asKeyMatch() { return 0; }
    virtual KeyActionsAST *asKeyActions() { return 0; }
    virtual UpdDelKeyActionsAST *asUpdDelKeyActions() { return 0; }
    virtual DelUpdKeyActionsAST *asDelUpdKeyActions() { return 0; }
    virtual KeyUpdateAST *asKeyUpdate() { return 0; }
    virtual KeyDeleteAST *asKeyDelete() { return 0; }
    virtual KeyActionAST *asKeyAction() { return 0; }
    virtual DefaultConstraintBodyAST *asDefaultConstraintBody() { return 0; }
    virtual CheckConstraintBodyAST *asCheckConstraintBody() { return 0; }
    virtual NoInheritAST *asNoInherit() { return 0; }
    virtual UniqueConstraintBodyAST *asUniqueConstraintBody() { return 0; }
    virtual PrimaryKeyConstraintBodyAST *asPrimaryKeyConstraintBody() { return 0; }
    virtual ConstraintTablespaceAST *asConstraintTablespace() { return 0; }
    virtual ConstraintDefinitionsAST *asConstraintDefinitions() { return 0; }
    virtual DefinitionAST *asDefinition() { return 0; }
    virtual DefinitionElemAST *asDefinitionElem() { return 0; }
    virtual DefinitionArgumentAST *asDefinitionArgument() { return 0; }
    virtual FuncTyDefinitionArgumentAST *asFuncTyDefinitionArgument() { return 0; }
    virtual FunctionTypeAST *asFunctionType() { return 0; }
    virtual PercentFunctionTypeAST *asPercentFunctionType() { return 0; }
    virtual SimpleFunctionTypeAST *asSimpleFunctionType() { return 0; }
    virtual QualOpDefinitionArgumentAST *asQualOpDefinitionArgument() { return 0; }
    virtual SimpleDefinitionArgumentAST *asSimpleDefinitionArgument() { return 0; }
    virtual NotNullConstraintBodyAST *asNotNullConstraintBody() { return 0; }
    virtual AttrColConstraintAST *asAttrColConstraint() { return 0; }
    virtual DeferColConstraintAST *asDeferColConstraint() { return 0; }
    virtual InitiallyColConstraintAST *asInitiallyColConstraint() { return 0; }
    virtual CollateColConstraintAST *asCollateColConstraint() { return 0; }
    virtual CreateGeneticOptionsAST *asCreateGeneticOptions() { return 0; }
    virtual GenericOptionAST *asGenericOption() { return 0; }
    virtual TypeAST *asType() { return 0; }
    virtual SimpleTypenameAST *asSimpleTypename() { return 0; }
    virtual GenericTypenameAST *asGenericTypename() { return 0; }
    virtual IntervalTypenameAST *asIntervalTypename() { return 0; }
    virtual NumericTypenameAST *asNumericTypename() { return 0; }
    virtual FloatTypenameAST *asFloatTypename() { return 0; }
    virtual DoubleTypenameAST *asDoubleTypename() { return 0; }
    virtual DecimalTypenameAST *asDecimalTypename() { return 0; }
    virtual FloatPrecisionAST *asFloatPrecision() { return 0; }
    virtual BitTypenameAST *asBitTypename() { return 0; }
    virtual CharacterTypenameAST *asCharacterTypename() { return 0; }
    virtual DatetimeTypenameAST *asDatetimeTypename() { return 0; }
    virtual TimeZoneSpecAST *asTimeZoneSpec() { return 0; }
    virtual CharacterTypeSpecAST *asCharacterTypeSpec() { return 0; }
    virtual CharSetAST *asCharSet() { return 0; }
    virtual TypeModifiersAST *asTypeModifiers() { return 0; }
    virtual ArrBoundAST *asArrBound() { return 0; }
    virtual ExpressionAST *asExpression() { return 0; }
    virtual PrimaryExpressionAST *asPrimaryExpression() { return 0; }
    virtual RowPrimaryExpressionAST *asRowPrimaryExpression() { return 0; }
    virtual ArrayPrimaryExpressionAST *asArrayPrimaryExpression() { return 0; }
    virtual ArrayExprAST *asArrayExpr() { return 0; }
    virtual ExprListArrayExprAST *asExprListArrayExpr() { return 0; }
    virtual ArrListArrayExprAST *asArrListArrayExpr() { return 0; }
    virtual EmptyArrayExprAST *asEmptyArrayExpr() { return 0; }
    virtual SubqueryPrimaryExpressionAST *asSubqueryPrimaryExpression() { return 0; }
    virtual FunctionPrimaryExpressionAST *asFunctionPrimaryExpression() { return 0; }
    virtual ColRefPrimaryExpressionAST *asColRefPrimaryExpression() { return 0; }
    virtual ConstPrimaryExpressionAST *asConstPrimaryExpression() { return 0; }
    virtual ParamPrimaryExpressionAST *asParamPrimaryExpression() { return 0; }
    virtual CompoundPrimaryExpressionAST *asCompoundPrimaryExpression() { return 0; }
    virtual CasePrimaryExpressionAST *asCasePrimaryExpression() { return 0; }
    virtual CaseExprAST *asCaseExpr() { return 0; }
    virtual CaseArgAST *asCaseArg() { return 0; }
    virtual CaseWhenClauseAST *asCaseWhenClause() { return 0; }
    virtual CaseDefaultClauseAST *asCaseDefaultClause() { return 0; }
    virtual ConstantAST *asConstant() { return 0; }
    virtual SimpleConstantAST *asSimpleConstant() { return 0; }
    virtual FuncNameConstantAST *asFuncNameConstant() { return 0; }
    virtual IntervalConstantAST *asIntervalConstant() { return 0; }
    virtual TypenameConstantAST *asTypenameConstant() { return 0; }
    virtual TypecastExpressionAST *asTypecastExpression() { return 0; }
    virtual CollateExpressionAST *asCollateExpression() { return 0; }
    virtual AtTimeZoneExpressionAST *asAtTimeZoneExpression() { return 0; }
    virtual UnaryExpressionAST *asUnaryExpression() { return 0; }
    virtual BinaryExpressionAST *asBinaryExpression() { return 0; }
    virtual CommonBinaryExpressionAST *asCommonBinaryExpression() { return 0; }
    virtual CommonUnaryExpresionAST *asCommonUnaryExpresion() { return 0; }
    virtual CommonPostfixExpressionAST *asCommonPostfixExpression() { return 0; }
    virtual LikeExpressionAST *asLikeExpression() { return 0; }
    virtual SimilarToExpressionAST *asSimilarToExpression() { return 0; }
    virtual IsExpressionAST *asIsExpression() { return 0; }
    virtual IsDdistinctExpressionAST *asIsDdistinctExpression() { return 0; }
    virtual IsTypeofExpressionAST *asIsTypeofExpression() { return 0; }
    virtual BetweenExpressionAST *asBetweenExpression() { return 0; }
    virtual InExpressionAST *asInExpression() { return 0; }
    virtual InValuesAST *asInValues() { return 0; }
    virtual InValuesAsQueryAST *asInValuesAsQuery() { return 0; }
    virtual InValuesAsExprListAST *asInValuesAsExprList() { return 0; }
    virtual RelationExprSimpleAST *asRelationExprSimple() { return 0; }
    virtual SubqueryExpressionAST *asSubqueryExpression() { return 0; }
    virtual SubExpressionExpressionAST *asSubExpressionExpression() { return 0; }
    virtual RowOverlapsExpressionAST *asRowOverlapsExpression() { return 0; }
    virtual UniqueExpressionAST *asUniqueExpression() { return 0; }
    virtual RowAST *asRow() { return 0; }
    virtual RelationExprOnlyAST *asRelationExprOnly() { return 0; }
    virtual SubqueryOperatorAST *asSubqueryOperator() { return 0; }
    virtual QualSubqueryOperatorAST *asQualSubqueryOperator() { return 0; }
    virtual LikeSubqueryOperatorAST *asLikeSubqueryOperator() { return 0; }
    virtual QualifiedOperatorAST *asQualifiedOperator() { return 0; }
    virtual SimpleQualifiedOperatorAST *asSimpleQualifiedOperator() { return 0; }
    virtual CompoundQualifiedOperatorAST *asCompoundQualifiedOperator() { return 0; }
    virtual OperandAST *asOperand() { return 0; }
    virtual AnyIdentifierAST *asAnyIdentifier() { return 0; }
    virtual TableIdentifierAST *asTableIdentifier() { return 0; }
    virtual TriggerIdentifierAST *asTriggerIdentifier() { return 0; }
    virtual IndexIdentifierAST *asIndexIdentifier() { return 0; }
    virtual ConstraintIdentifierAST *asConstraintIdentifier() { return 0; }
    virtual SequenceIdentifierAST *asSequenceIdentifier() { return 0; }
    virtual ColumnIdentifierAST *asColumnIdentifier() { return 0; }
    virtual ColumnRefAST *asColumnRef() { return 0; }
    virtual FunctionRefAST *asFunctionRef() { return 0; }
    virtual IndirectionAST *asIndirection() { return 0; }
    virtual AttirbuteIndirectionAST *asAttirbuteIndirection() { return 0; }
    virtual StarIndirectionAST *asStarIndirection() { return 0; }
    virtual IndexingIndirectionAST *asIndexingIndirection() { return 0; }

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
    virtual bool isNull() const { return true; }

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

    QList<RelationExprAST *> *relations;

    class Info {
    public:
        enum Type {
            UnknownType = 0,
            Select,
            Insert,
            Update,
            Delete,
            Truncate,
            Comment,
            Alter,
            Create,
            Drop
        };

        enum Target {
            UnknownTarget = 0,
            Table,
            Column,
            View,
            Index,
            Trigger,
            Constraint,
            Sequence,
            Role,
            User,
            Schema,
            TypeTarget,
            Aggregate,
            Function,
            Operator
        };

        Info( Type type_ = UnknownType, Target target_ = UnknownTarget )
            : type(type_)
            , target(target_)
            , schemaName(0)
            , objectName(0)
        { }

        Type type;
        Target target;
        NameAST *schemaName;
        NameAST *objectName;
    };

    Info *info;

    struct CompletionHelper
    {
        CompletionHelper()
            : start_token(0)
            , end_token(0)
            , schemaName(0)
            , objectName(0)
            , type(UnknownHelper)
        { }

        int start_token;
        int end_token;
        NameAST *schemaName;
        NameAST *objectName;
        QString keywords;
        enum HelperType {
            UnknownHelper = 0,
            SchemaName,
            TableIdentifier,
            ViewIdentifier,
            IndexIdentifier,
            TriggerIdentifier,
            SequenceIdentifier,
            TableColumnIdentifier,
            TableConstraintIdentifier,
            TriggerTableIdentifier,
            ColumnName,
            IndexName,
            TriggerName,
            ConstraintName,
            Keywords
        };
        HelperType type;
    };

    QList<CompletionHelper *> *helpers;

public:
    StatementAST( bool topLevel_ = true, bool blockMode_ = false )
        : start_line(0)
        , start_pos(0)
        , end_line(0)
        , end_pos(0)
        , relations(0)
        , info(0)
        , topLevel(topLevel_)
        , blockMode(blockMode_)
    { }

    virtual ~StatementAST() {
        if ( relations )
            delete relations;
        if ( info )
            delete info;
        if ( helpers ) {
            qDeleteAll(*helpers);
            delete helpers;
        }
    }

    virtual StatementAST *asStatement() { return this; }

    inline bool isTopLevelStatement() const { return topLevel; }
    inline void setTopLevelStatement( bool topLevel_ ) { topLevel = topLevel_; }
    inline bool isBlockMode() const { return blockMode; }
    inline void setBlockMode( bool blockMode_ = true ) { blockMode = blockMode_; }

private:
    bool topLevel;
    bool blockMode;
};

class RelationExprAST: public AST
{
public:
    TableAliasClauseAST *alias;

public:
    RelationExprAST()
        : alias(0)
    { }

    virtual RelationExprAST *asRelationExpr() { return this; }
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
        Sequence,
        Binding,
        Label,
        Package,
        Routine,
        Type,
        Role,
        Database,
        Window,
        Variable
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
    virtual bool isNull() const { return name == 0; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TranslationUnitAST: public AST
{
public:
    StatementListAST *statements;

public:
    TranslationUnitAST( StatementListAST *statements_ )
        : AST()
        , statements(finish(statements_))
    {}

    virtual TranslationUnitAST *asTranslationUnit() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DdlStatementAST: public StatementAST
{
public:
    DdlStatementAST() : StatementAST() {}
    virtual DdlStatementAST *asDdlStatement() { return this; }
};

class DmlStatementAST: public StatementAST
{
public:
    DmlStatementAST() : StatementAST() {}
    virtual DmlStatementAST *asDmlStatement() { return this; }
};

class UnknownStatementAST: public StatementAST
{
public:
    int t_first;
    int t_last;

public:
    UnknownStatementAST( int t_first_, int t_last_ )
        : StatementAST()
        , t_first(t_first_)
        , t_last(t_last_)
    {}

    virtual UnknownStatementAST *asUnknownStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateStatementAST: public DdlStatementAST
{
public:
    CreateStatementAST() : DdlStatementAST() {}
    virtual CreateStatementAST *asCreateStatement() { return this; }
};

class CreateViewStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_or;
    int t_replace;
    TempClauseAST *tempClause;
    int t_recursive;
    int t_view;
    TableIdentifierAST *viewIdent;
    RefColumnsAST *columns;
    WithReloptionsAST *relOpts;
    int t_as;
    SelectStmtAST *query;
    WithCheckOptionClauseAST *checkOpt;

public:
    CreateViewStmtAST( int t_create_, int t_or_, int t_replace_, TempClauseAST *tempClause_, int t_recursive_, int t_view_, TableIdentifierAST *viewIdent_, RefColumnsAST *columns_, WithReloptionsAST *relOpts_, int t_as_, SelectStmtAST *query_, WithCheckOptionClauseAST *checkOpt_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_or(t_or_)
        , t_replace(t_replace_)
        , tempClause(tempClause_)
        , t_recursive(t_recursive_)
        , t_view(t_view_)
        , viewIdent(viewIdent_)
        , columns(columns_)
        , relOpts(relOpts_)
        , t_as(t_as_)
        , query(query_)
        , checkOpt(checkOpt_)
    {}

    virtual CreateViewStmtAST *asCreateViewStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithCheckOptionClauseAST: public AST
{
public:
    int t_with;
    int t_local_or_cascaded;
    int t_check;
    int t_option;

public:
    WithCheckOptionClauseAST( int t_with_, int t_local_or_cascaded_, int t_check_, int t_option_ )
        : AST()
        , t_with(t_with_)
        , t_local_or_cascaded(t_local_or_cascaded_)
        , t_check(t_check_)
        , t_option(t_option_)
    {}

    virtual WithCheckOptionClauseAST *asWithCheckOptionClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VacuumStmtAST: public DdlStatementAST
{
public:
    VacuumStmtAST() : DdlStatementAST() {}
    virtual VacuumStmtAST *asVacuumStmt() { return this; }
};

class CommonVacuumStmtAST: public VacuumStmtAST
{
public:
    int t_vacuum;
    int t_lparen;
    VacuumOptionListAST *options;
    int t_rparen;
    TableIdentifierAST *tableIdent;
    CompoundColumnNamesAST *columns;

public:
    CommonVacuumStmtAST( int t_vacuum_, int t_lparen_, VacuumOptionListAST *options_, int t_rparen_, TableIdentifierAST *tableIdent_, CompoundColumnNamesAST *columns_ )
        : VacuumStmtAST()
        , t_vacuum(t_vacuum_)
        , t_lparen(t_lparen_)
        , options(finish(options_))
        , t_rparen(t_rparen_)
        , tableIdent(tableIdent_)
        , columns(columns_)
    {}

    virtual CommonVacuumStmtAST *asCommonVacuumStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VacuumOptionAST: public AST
{
public:
    int t_analyze_verbose_freeze_full;

public:
    VacuumOptionAST( int t_analyze_verbose_freeze_full_ )
        : AST()
        , t_analyze_verbose_freeze_full(t_analyze_verbose_freeze_full_)
    {}

    virtual VacuumOptionAST *asVacuumOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VacuumTableStmtAST: public VacuumStmtAST
{
public:
    int t_vacuum;
    int t_full;
    int t_freeze;
    int t_verbose;
    TableIdentifierAST *tableIdent;

public:
    VacuumTableStmtAST( int t_vacuum_, int t_full_, int t_freeze_, int t_verbose_, TableIdentifierAST *tableIdent_ )
        : VacuumStmtAST()
        , t_vacuum(t_vacuum_)
        , t_full(t_full_)
        , t_freeze(t_freeze_)
        , t_verbose(t_verbose_)
        , tableIdent(tableIdent_)
    {}

    virtual VacuumTableStmtAST *asVacuumTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VacuumAnalyzeStmtAST: public VacuumStmtAST
{
public:
    int t_vacuum;
    int t_full;
    int t_freeze;
    int t_verbose;
    AnalyzeStmtAST *analyzeStmt;

public:
    VacuumAnalyzeStmtAST( int t_vacuum_, int t_full_, int t_freeze_, int t_verbose_, AnalyzeStmtAST *analyzeStmt_ )
        : VacuumStmtAST()
        , t_vacuum(t_vacuum_)
        , t_full(t_full_)
        , t_freeze(t_freeze_)
        , t_verbose(t_verbose_)
        , analyzeStmt(analyzeStmt_)
    {}

    virtual VacuumAnalyzeStmtAST *asVacuumAnalyzeStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TruncateStmtAST: public DmlStatementAST
{
public:
    int t_truncate;
    int t_table;
    RelationExprListAST *tables;
    TruncateRestartClauseAST *restartClause;
    int t_behavior;

public:
    TruncateStmtAST( int t_truncate_, int t_table_, RelationExprListAST *tables_, TruncateRestartClauseAST *restartClause_, int t_behavior_ )
        : DmlStatementAST()
        , t_truncate(t_truncate_)
        , t_table(t_table_)
        , tables(finish(tables_))
        , restartClause(restartClause_)
        , t_behavior(t_behavior_)
    {}

    virtual TruncateStmtAST *asTruncateStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TruncateRestartClauseAST: public AST
{
public:
    int t_continue_or_restart;
    int t_identity;

public:
    TruncateRestartClauseAST( int t_continue_or_restart_, int t_identity_ )
        : AST()
        , t_continue_or_restart(t_continue_or_restart_)
        , t_identity(t_identity_)
    {}

    virtual TruncateRestartClauseAST *asTruncateRestartClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TransactionStmtAST: public StatementAST
{
public:
    TransactionStmtAST() : StatementAST() {}
    virtual TransactionStmtAST *asTransactionStmt() { return this; }
};

class BeginTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_begin_or_start;
    int t_transaction;
    TransactionModeListAST *transOptions;

public:
    BeginTransactionStmtAST( int t_begin_or_start_, int t_transaction_, TransactionModeListAST *transOptions_ )
        : TransactionStmtAST()
        , t_begin_or_start(t_begin_or_start_)
        , t_transaction(t_transaction_)
        , transOptions(finish(transOptions_))
    {}

    virtual BeginTransactionStmtAST *asBeginTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SavepointTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_savepoint;
    int t_savepoint_label;

public:
    SavepointTransactionStmtAST( int t_savepoint_, int t_savepoint_label_ )
        : TransactionStmtAST()
        , t_savepoint(t_savepoint_)
        , t_savepoint_label(t_savepoint_label_)
    {}

    virtual SavepointTransactionStmtAST *asSavepointTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReleaseTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_release;
    int t_transaction;
    int t_savepoint_label;

public:
    ReleaseTransactionStmtAST( int t_release_, int t_transaction_, int t_savepoint_label_ )
        : TransactionStmtAST()
        , t_release(t_release_)
        , t_transaction(t_transaction_)
        , t_savepoint_label(t_savepoint_label_)
    {}

    virtual ReleaseTransactionStmtAST *asReleaseTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommitTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_commit_or_end;
    int t_transaction;

public:
    CommitTransactionStmtAST( int t_commit_or_end_, int t_transaction_ )
        : TransactionStmtAST()
        , t_commit_or_end(t_commit_or_end_)
        , t_transaction(t_transaction_)
    {}

    virtual CommitTransactionStmtAST *asCommitTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RollbackTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_rollback_or_abort;
    int t_transaction;
    int t_to;
    int t_savepoint;
    int t_savepoint_label;

public:
    RollbackTransactionStmtAST( int t_rollback_or_abort_, int t_transaction_, int t_to_, int t_savepoint_, int t_savepoint_label_ )
        : TransactionStmtAST()
        , t_rollback_or_abort(t_rollback_or_abort_)
        , t_transaction(t_transaction_)
        , t_to(t_to_)
        , t_savepoint(t_savepoint_)
        , t_savepoint_label(t_savepoint_label_)
    {}

    virtual RollbackTransactionStmtAST *asRollbackTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrepareTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_prepare;
    int t_transaction;
    int t_pre_trans_label;

public:
    PrepareTransactionStmtAST( int t_prepare_, int t_transaction_, int t_pre_trans_label_ )
        : TransactionStmtAST()
        , t_prepare(t_prepare_)
        , t_transaction(t_transaction_)
        , t_pre_trans_label(t_pre_trans_label_)
    {}

    virtual PrepareTransactionStmtAST *asPrepareTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommitPrepTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_commit;
    int t_prepared;
    int t_pre_trans_label;

public:
    CommitPrepTransactionStmtAST( int t_commit_, int t_prepared_, int t_pre_trans_label_ )
        : TransactionStmtAST()
        , t_commit(t_commit_)
        , t_prepared(t_prepared_)
        , t_pre_trans_label(t_pre_trans_label_)
    {}

    virtual CommitPrepTransactionStmtAST *asCommitPrepTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RollPrepTransactionStmtAST: public TransactionStmtAST
{
public:
    int t_rollback;
    int t_prepared;
    int t_pre_trans_label;

public:
    RollPrepTransactionStmtAST( int t_rollback_, int t_prepared_, int t_pre_trans_label_ )
        : TransactionStmtAST()
        , t_rollback(t_rollback_)
        , t_prepared(t_prepared_)
        , t_pre_trans_label(t_pre_trans_label_)
    {}

    virtual RollPrepTransactionStmtAST *asRollPrepTransactionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SecLabelStmtAST: public DdlStatementAST
{
public:
    SecLabelStmtAST() : DdlStatementAST() {}
    virtual SecLabelStmtAST *asSecLabelStmt() { return this; }
};

class ColSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_column;
    ColumnIdentifierAST *column;
    int t_is;
    int t_seq_label;

public:
    ColSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_column_, ColumnIdentifierAST *column_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_column(t_column_)
        , column(column_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual ColSecLabelStmtAST *asColSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SequenceSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_sequence;
    SequenceIdentifierAST *seqIdent;
    int t_is;
    int t_seq_label;

public:
    SequenceSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_sequence_, SequenceIdentifierAST *seqIdent_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_sequence(t_sequence_)
        , seqIdent(seqIdent_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual SequenceSecLabelStmtAST *asSequenceSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_foreign_or_materialized;
    int t_table_or_view;
    TableIdentifierAST *ident;
    int t_is;
    int t_seq_label;

public:
    TableSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_foreign_or_materialized_, int t_table_or_view_, TableIdentifierAST *ident_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_foreign_or_materialized(t_foreign_or_materialized_)
        , t_table_or_view(t_table_or_view_)
        , ident(ident_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual TableSecLabelStmtAST *asTableSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AggrSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_aggregate;
    FunctionRefAST *funcRef;
    AggrArgumentsAST *args;
    int t_is;
    int t_seq_label;

public:
    AggrSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_aggregate_, FunctionRefAST *funcRef_, AggrArgumentsAST *args_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_aggregate(t_aggregate_)
        , funcRef(funcRef_)
        , args(args_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual AggrSecLabelStmtAST *asAggrSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_function;
    FunctionRefAST *funcRef;
    FunctionArgTypesAST *argTypes;
    int t_is;
    int t_seq_label;

public:
    FuncSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_function_, FunctionRefAST *funcRef_, FunctionArgTypesAST *argTypes_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_function(t_function_)
        , funcRef(funcRef_)
        , argTypes(argTypes_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual FuncSecLabelStmtAST *asFuncSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LargeObjSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_large;
    int t_object;
    int t_sp_num;
    int t_is;
    int t_seq_label;

public:
    LargeObjSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_large_, int t_object_, int t_sp_num_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_large(t_large_)
        , t_object(t_object_)
        , t_sp_num(t_sp_num_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual LargeObjSecLabelStmtAST *asLargeObjSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LangSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_procedural;
    int t_language;
    AnyIdentifierAST *ident;
    int t_is;
    int t_seq_label;

public:
    LangSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_procedural_, int t_language_, AnyIdentifierAST *ident_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , ident(ident_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual LangSecLabelStmtAST *asLangSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SchemaSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_schema;
    NameAST *schemaName;
    int t_is;
    int t_seq_label;

public:
    SchemaSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_schema_, NameAST *schemaName_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_schema(t_schema_)
        , schemaName(schemaName_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual SchemaSecLabelStmtAST *asSchemaSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoleSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    int t_role;
    RoleIdAST *roleId;
    int t_is;
    int t_seq_label;

public:
    RoleSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, int t_role_, RoleIdAST *roleId_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , t_role(t_role_)
        , roleId(roleId_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual RoleSecLabelStmtAST *asRoleSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonSecLabelStmtAST: public SecLabelStmtAST
{
public:
    int t_security;
    int t_label;
    SecurityProviderAST *provider;
    int t_on;
    CommonSecurityTypeAST *secType;
    AnyIdentifierAST *ident;
    int t_is;
    int t_seq_label;

public:
    CommonSecLabelStmtAST( int t_security_, int t_label_, SecurityProviderAST *provider_, int t_on_, CommonSecurityTypeAST *secType_, AnyIdentifierAST *ident_, int t_is_, int t_seq_label_ )
        : SecLabelStmtAST()
        , t_security(t_security_)
        , t_label(t_label_)
        , provider(provider_)
        , t_on(t_on_)
        , secType(secType_)
        , ident(ident_)
        , t_is(t_is_)
        , t_seq_label(t_seq_label_)
    {}

    virtual CommonSecLabelStmtAST *asCommonSecLabelStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SecurityProviderAST: public AST
{
public:
    int t_for;
    int t_prov_id;

public:
    SecurityProviderAST( int t_for_, int t_prov_id_ )
        : AST()
        , t_for(t_for_)
        , t_prov_id(t_prov_id_)
    {}

    virtual SecurityProviderAST *asSecurityProvider() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonSecurityTypeAST: public AST
{
public:
    int t_sp1;
    int t_sp2;

public:
    CommonSecurityTypeAST( int t_sp1_, int t_sp2_ )
        : AST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
    {}

    virtual CommonSecurityTypeAST *asCommonSecurityType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameStmtAST: public DdlStatementAST
{
public:
    RenameStmtAST() : DdlStatementAST() {}
    virtual RenameStmtAST *asRenameStmt() { return this; }
};

class RenameTypeStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_type;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameTypeStmtAST( int t_alter_, int t_type_, AnyIdentifierAST *ident_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_type(t_type_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameTypeStmtAST *asRenameTypeStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameTypeAttrStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_type;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_attribute;
    NameAST *oldName;
    int t_to;
    NameAST *newName;

public:
    RenameTypeAttrStmtAST( int t_alter_, int t_type_, AnyIdentifierAST *ident_, int t_rename_, int t_attribute_, NameAST *oldName_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_type(t_type_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_attribute(t_attribute_)
        , oldName(oldName_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameTypeAttrStmtAST *asRenameTypeAttrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameRuleStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_rule;
    NameAST *ruleName;
    int t_on;
    TableIdentifierAST *tableIdent;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameRuleStmtAST( int t_alter_, int t_rule_, NameAST *ruleName_, int t_on_, TableIdentifierAST *tableIdent_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_rule(t_rule_)
        , ruleName(ruleName_)
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameRuleStmtAST *asRenameRuleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameTriggerStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_trigger;
    NameAST *triggerName;
    int t_on;
    TableIdentifierAST *tableIdent;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameTriggerStmtAST( int t_alter_, int t_trigger_, NameAST *triggerName_, int t_on_, TableIdentifierAST *tableIdent_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameTriggerStmtAST *asRenameTriggerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameSchemaStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_schema;
    NameAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameSchemaStmtAST( int t_alter_, int t_schema_, NameAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_schema(t_schema_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameSchemaStmtAST *asRenameSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameServerStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_server;
    NameAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameServerStmtAST( int t_alter_, int t_server_, NameAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_server(t_server_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameServerStmtAST *asRenameServerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameEvtTrgStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_event;
    int t_trigger;
    NameAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameEvtTrgStmtAST( int t_alter_, int t_event_, int t_trigger_, NameAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_event(t_event_)
        , t_trigger(t_trigger_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameEvtTrgStmtAST *asRenameEvtTrgStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameRoleStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_role_or_user;
    RoleIdAST *oldName;
    int t_rename;
    int t_to;
    RoleIdAST *newName;

public:
    RenameRoleStmtAST( int t_alter_, int t_role_or_user_, RoleIdAST *oldName_, int t_rename_, int t_to_, RoleIdAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_role_or_user(t_role_or_user_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameRoleStmtAST *asRenameRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameTablespaceStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_tablespace;
    NameAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameTablespaceStmtAST( int t_alter_, int t_tablespace_, NameAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_tablespace(t_tablespace_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameTablespaceStmtAST *asRenameTablespaceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ChgReloptsTablespaceStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_tablespace;
    NameAST *tablespaceName;
    int t_set_or_reset;
    RelOptionsAST *relOptions;

public:
    ChgReloptsTablespaceStmtAST( int t_alter_, int t_tablespace_, NameAST *tablespaceName_, int t_set_or_reset_, RelOptionsAST *relOptions_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
        , t_set_or_reset(t_set_or_reset_)
        , relOptions(relOptions_)
    {}

    virtual ChgReloptsTablespaceStmtAST *asChgReloptsTablespaceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameSearchStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_text;
    int t_search;
    int t_sp_obj;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameSearchStmtAST( int t_alter_, int t_text_, int t_search_, int t_sp_obj_, AnyIdentifierAST *ident_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_text(t_text_)
        , t_search(t_search_)
        , t_sp_obj(t_sp_obj_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameSearchStmtAST *asRenameSearchStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameTableStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_foreign;
    int t_table;
    int t_if;
    int t_exists;
    RelationExprAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameTableStmtAST( int t_alter_, int t_foreign_, int t_table_, int t_if_, int t_exists_, RelationExprAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameTableStmtAST *asRenameTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameTableColStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_foreign;
    int t_table;
    int t_if;
    int t_exists;
    RelationExprAST *rel;
    int t_rename;
    int t_column;
    NameAST *oldColName;
    int t_to;
    NameAST *newColName;

public:
    RenameTableColStmtAST( int t_alter_, int t_foreign_, int t_table_, int t_if_, int t_exists_, RelationExprAST *rel_, int t_rename_, int t_column_, NameAST *oldColName_, int t_to_, NameAST *newColName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , rel(rel_)
        , t_rename(t_rename_)
        , t_column(t_column_)
        , oldColName(oldColName_)
        , t_to(t_to_)
        , newColName(newColName_)
    {}

    virtual RenameTableColStmtAST *asRenameTableColStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameTableConstrStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_table;
    RelationExprAST *rel;
    int t_rename;
    int t_constraint;
    NameAST *oldConstrName;
    int t_to;
    NameAST *newConstrName;

public:
    RenameTableConstrStmtAST( int t_alter_, int t_table_, RelationExprAST *rel_, int t_rename_, int t_constraint_, NameAST *oldConstrName_, int t_to_, NameAST *newConstrName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_table(t_table_)
        , rel(rel_)
        , t_rename(t_rename_)
        , t_constraint(t_constraint_)
        , oldConstrName(oldConstrName_)
        , t_to(t_to_)
        , newConstrName(newConstrName_)
    {}

    virtual RenameTableConstrStmtAST *asRenameTableConstrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameMatViewColStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_materialized;
    int t_view;
    int t_if;
    int t_exists;
    TableIdentifierAST *viewIdent;
    int t_rename;
    int t_column;
    NameAST *oldColName;
    int t_to;
    NameAST *newColName;

public:
    RenameMatViewColStmtAST( int t_alter_, int t_materialized_, int t_view_, int t_if_, int t_exists_, TableIdentifierAST *viewIdent_, int t_rename_, int t_column_, NameAST *oldColName_, int t_to_, NameAST *newColName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , viewIdent(viewIdent_)
        , t_rename(t_rename_)
        , t_column(t_column_)
        , oldColName(oldColName_)
        , t_to(t_to_)
        , newColName(newColName_)
    {}

    virtual RenameMatViewColStmtAST *asRenameMatViewColStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameSequenceStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_sequence;
    int t_if;
    int t_exists;
    SequenceIdentifierAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameSequenceStmtAST( int t_alter_, int t_sequence_, int t_if_, int t_exists_, SequenceIdentifierAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_sequence(t_sequence_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameSequenceStmtAST *asRenameSequenceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameViewStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_matirialized;
    int t_view;
    int t_if;
    int t_exists;
    TableIdentifierAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameViewStmtAST( int t_alter_, int t_matirialized_, int t_view_, int t_if_, int t_exists_, TableIdentifierAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_matirialized(t_matirialized_)
        , t_view(t_view_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameViewStmtAST *asRenameViewStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameIndexStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_index;
    int t_if;
    int t_exists;
    IndexIdentifierAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameIndexStmtAST( int t_alter_, int t_index_, int t_if_, int t_exists_, IndexIdentifierAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_index(t_index_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameIndexStmtAST *asRenameIndexStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameCollationStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_collation;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameCollationStmtAST( int t_alter_, int t_collation_, AnyIdentifierAST *ident_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_collation(t_collation_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameCollationStmtAST *asRenameCollationStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameConversionStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_conversion;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameConversionStmtAST( int t_alter_, int t_conversion_, AnyIdentifierAST *ident_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_conversion(t_conversion_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameConversionStmtAST *asRenameConversionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameDomainStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameDomainStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *ident_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameDomainStmtAST *asRenameDomainStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameDomainConstrStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *ident;
    int t_rename;
    int t_constraint;
    NameAST *constraintName;
    int t_to;
    NameAST *newName;

public:
    RenameDomainConstrStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *ident_, int t_rename_, int t_constraint_, NameAST *constraintName_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , ident(ident_)
        , t_rename(t_rename_)
        , t_constraint(t_constraint_)
        , constraintName(constraintName_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameDomainConstrStmtAST *asRenameDomainConstrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameDatabaseStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_database;
    DatabaseNameAST *oldName;
    int t_rename;
    int t_to;
    DatabaseNameAST *newName;

public:
    RenameDatabaseStmtAST( int t_alter_, int t_database_, DatabaseNameAST *oldName_, int t_rename_, int t_to_, DatabaseNameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_database(t_database_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameDatabaseStmtAST *asRenameDatabaseStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameFdwStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameFdwStmtAST( int t_alter_, int t_foreign_, int t_data_, int t_wrapper_, NameAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameFdwStmtAST *asRenameFdwStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameFuncStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_function;
    FunctionWithArgTypesAST *args;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameFuncStmtAST( int t_alter_, int t_function_, FunctionWithArgTypesAST *args_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_function(t_function_)
        , args(args_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameFuncStmtAST *asRenameFuncStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameOpUsingStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_operator;
    int t_class_or_family;
    AnyIdentifierAST *ident;
    int t_using;
    int t_acc_method;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameOpUsingStmtAST( int t_alter_, int t_operator_, int t_class_or_family_, AnyIdentifierAST *ident_, int t_using_, int t_acc_method_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , t_class_or_family(t_class_or_family_)
        , ident(ident_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameOpUsingStmtAST *asRenameOpUsingStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameGroupRoleStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_group;
    RoleIdAST *oldName;
    int t_rename;
    int t_to;
    RoleIdAST *newName;

public:
    RenameGroupRoleStmtAST( int t_alter_, int t_group_, RoleIdAST *oldName_, int t_rename_, int t_to_, RoleIdAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_group(t_group_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameGroupRoleStmtAST *asRenameGroupRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameLangStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_procedural;
    int t_language;
    NameAST *oldName;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameLangStmtAST( int t_alter_, int t_procedural_, int t_language_, NameAST *oldName_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , oldName(oldName_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameLangStmtAST *asRenameLangStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RenameAggrStmtAST: public RenameStmtAST
{
public:
    int t_alter;
    int t_aggregate;
    FunctionRefAST *funcRef;
    AggrArgumentsAST *args;
    int t_rename;
    int t_to;
    NameAST *newName;

public:
    RenameAggrStmtAST( int t_alter_, int t_aggregate_, FunctionRefAST *funcRef_, AggrArgumentsAST *args_, int t_rename_, int t_to_, NameAST *newName_ )
        : RenameStmtAST()
        , t_alter(t_alter_)
        , t_aggregate(t_aggregate_)
        , funcRef(funcRef_)
        , args(args_)
        , t_rename(t_rename_)
        , t_to(t_to_)
        , newName(newName_)
    {}

    virtual RenameAggrStmtAST *asRenameAggrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReindexStmtAST: public StatementAST
{
public:
    ReindexStmtAST() : StatementAST() {}
    virtual ReindexStmtAST *asReindexStmt() { return this; }
};

class ReindexTableStmtAST: public ReindexStmtAST
{
public:
    int t_reindex;
    int t_table;
    TableIdentifierAST *tableIdent;
    int t_force;

public:
    ReindexTableStmtAST( int t_reindex_, int t_table_, TableIdentifierAST *tableIdent_, int t_force_ )
        : ReindexStmtAST()
        , t_reindex(t_reindex_)
        , t_table(t_table_)
        , tableIdent(tableIdent_)
        , t_force(t_force_)
    {}

    virtual ReindexTableStmtAST *asReindexTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReindexIndexStmtAST: public ReindexStmtAST
{
public:
    int t_reindex;
    int t_index;
    IndexIdentifierAST *tableIdent;
    int t_force;

public:
    ReindexIndexStmtAST( int t_reindex_, int t_index_, IndexIdentifierAST *tableIdent_, int t_force_ )
        : ReindexStmtAST()
        , t_reindex(t_reindex_)
        , t_index(t_index_)
        , tableIdent(tableIdent_)
        , t_force(t_force_)
    {}

    virtual ReindexIndexStmtAST *asReindexIndexStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReindexSystemStmtAST: public ReindexStmtAST
{
public:
    int t_reindex;
    int t_system;
    NameAST *systemName;
    int t_force;

public:
    ReindexSystemStmtAST( int t_reindex_, int t_system_, NameAST *systemName_, int t_force_ )
        : ReindexStmtAST()
        , t_reindex(t_reindex_)
        , t_system(t_system_)
        , systemName(systemName_)
        , t_force(t_force_)
    {}

    virtual ReindexSystemStmtAST *asReindexSystemStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReindexDatabaseStmtAST: public ReindexStmtAST
{
public:
    int t_reindex;
    int t_database;
    NameAST *dbName;
    int t_force;

public:
    ReindexDatabaseStmtAST( int t_reindex_, int t_database_, NameAST *dbName_, int t_force_ )
        : ReindexStmtAST()
        , t_reindex(t_reindex_)
        , t_database(t_database_)
        , dbName(dbName_)
        , t_force(t_force_)
    {}

    virtual ReindexDatabaseStmtAST *asReindexDatabaseStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrepareStmtAST: public StatementAST
{
public:
    int t_prepare;
    NameAST *prepName;
    PrepareTypeClauseAST *prepareTypeClause;
    int t_as;
    StatementAST *stmt;

public:
    PrepareStmtAST( int t_prepare_, NameAST *prepName_, PrepareTypeClauseAST *prepareTypeClause_, int t_as_, StatementAST *stmt_ )
        : StatementAST()
        , t_prepare(t_prepare_)
        , prepName(prepName_)
        , prepareTypeClause(prepareTypeClause_)
        , t_as(t_as_)
        , stmt(stmt_)
    {}

    virtual PrepareStmtAST *asPrepareStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrepareTypeClauseAST: public AST
{
public:
    int t_lparen;
    TypeListAST *types;
    int t_rparen;

public:
    PrepareTypeClauseAST( int t_lparen_, TypeListAST *types_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , types(finish(types_))
        , t_rparen(t_rparen_)
    {}

    virtual PrepareTypeClauseAST *asPrepareTypeClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockStmtAST: public StatementAST
{
public:
    int t_lock;
    int t_table;
    RelationExprListAST *reations;
    LockModeClauseAST *lockMode;
    int t_nowait;

public:
    LockStmtAST( int t_lock_, int t_table_, RelationExprListAST *reations_, LockModeClauseAST *lockMode_, int t_nowait_ )
        : StatementAST()
        , t_lock(t_lock_)
        , t_table(t_table_)
        , reations(finish(reations_))
        , lockMode(lockMode_)
        , t_nowait(t_nowait_)
    {}

    virtual LockStmtAST *asLockStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockModeClauseAST: public AST
{
public:
    int t_in;
    LockTypeAST *lockType;
    int t_mode;

public:
    LockModeClauseAST( int t_in_, LockTypeAST *lockType_, int t_mode_ )
        : AST()
        , t_in(t_in_)
        , lockType(lockType_)
        , t_mode(t_mode_)
    {}

    virtual LockModeClauseAST *asLockModeClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockTypeAST: public AST
{
public:
    int t_sp1;
    int t_sp2;
    int t_sp3;

public:
    LockTypeAST( int t_sp1_, int t_sp2_, int t_sp3_ )
        : AST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
        , t_sp3(t_sp3_)
    {}

    virtual LockTypeAST *asLockType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropStmtAST: public DdlStatementAST
{
public:
    DropStmtAST() : DdlStatementAST() {}
    virtual DropStmtAST *asDropStmt() { return this; }
};

class DropAggrStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_aggregate;
    int t_if;
    int t_exists;
    FunctionRefAST *funcRef;
    AggrArgumentsAST *args;
    int t_behavior;

public:
    DropAggrStmtAST( int t_drop_, int t_aggregate_, int t_if_, int t_exists_, FunctionRefAST *funcRef_, AggrArgumentsAST *args_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_aggregate(t_aggregate_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , funcRef(funcRef_)
        , args(args_)
        , t_behavior(t_behavior_)
    {}

    virtual DropAggrStmtAST *asDropAggrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropFuncStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_function;
    int t_if;
    int t_exists;
    FunctionRefAST *funcRef;
    FunctionArgTypesAST *argTypes;
    int t_behavior;

public:
    DropFuncStmtAST( int t_drop_, int t_function_, int t_if_, int t_exists_, FunctionRefAST *funcRef_, FunctionArgTypesAST *argTypes_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_function(t_function_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , funcRef(funcRef_)
        , argTypes(argTypes_)
        , t_behavior(t_behavior_)
    {}

    virtual DropFuncStmtAST *asDropFuncStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropOperStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_operator;
    int t_if;
    int t_exists;
    OperandAST *operand;
    OperArgTypesAST *argTypes;
    int t_behavior;

public:
    DropOperStmtAST( int t_drop_, int t_operator_, int t_if_, int t_exists_, OperandAST *operand_, OperArgTypesAST *argTypes_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_operator(t_operator_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , operand(operand_)
        , argTypes(argTypes_)
        , t_behavior(t_behavior_)
    {}

    virtual DropOperStmtAST *asDropOperStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LoadStmtAST: public StatementAST
{
public:
    int t_load;
    int t_file_name;

public:
    LoadStmtAST( int t_load_, int t_file_name_ )
        : StatementAST()
        , t_load(t_load_)
        , t_file_name(t_file_name_)
    {}

    virtual LoadStmtAST *asLoadStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotifyStmtAST: public StatementAST
{
public:
    int t_notify;
    int t_notify_label;
    NotifyPayloadAST *payload;

public:
    NotifyStmtAST( int t_notify_, int t_notify_label_, NotifyPayloadAST *payload_ )
        : StatementAST()
        , t_notify(t_notify_)
        , t_notify_label(t_notify_label_)
        , payload(payload_)
    {}

    virtual NotifyStmtAST *asNotifyStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotifyPayloadAST: public AST
{
public:
    int t_comma;
    int t_sp_str;

public:
    NotifyPayloadAST( int t_comma_, int t_sp_str_ )
        : AST()
        , t_comma(t_comma_)
        , t_sp_str(t_sp_str_)
    {}

    virtual NotifyPayloadAST *asNotifyPayload() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ListenStmtAST: public StatementAST
{
public:
    int t_listen;
    int t_listen_label;

public:
    ListenStmtAST( int t_listen_, int t_listen_label_ )
        : StatementAST()
        , t_listen(t_listen_)
        , t_listen_label(t_listen_label_)
    {}

    virtual ListenStmtAST *asListenStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UnlistenStmtAST: public StatementAST
{
public:
    int t_unlisten;
    int t_star_or_ident;

public:
    UnlistenStmtAST( int t_unlisten_, int t_star_or_ident_ )
        : StatementAST()
        , t_unlisten(t_unlisten_)
        , t_star_or_ident(t_star_or_ident_)
    {}

    virtual UnlistenStmtAST *asUnlistenStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateIndexStatementAST: public CreateStatementAST
{
public:
    int t_create;
    int t_unique;
    int t_index;
    int t_concurrently;
    NameAST *indexName;
    int t_on;
    TableIdentifierAST *tableIdent;
    AccessMethodClauseAST *accMethod;
    int t_lparen;
    IndexElementListAST *elements;
    int t_rparen;
    WithReloptionsAST *reopts;
    TableSpaceClauseAST *tablespaceClause;
    WhereClauseAST *whereClause;

public:
    CreateIndexStatementAST( int t_create_, int t_unique_, int t_index_, int t_concurrently_, NameAST *indexName_, int t_on_, TableIdentifierAST *tableIdent_, AccessMethodClauseAST *accMethod_, int t_lparen_, IndexElementListAST *elements_, int t_rparen_, WithReloptionsAST *reopts_, TableSpaceClauseAST *tablespaceClause_, WhereClauseAST *whereClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_unique(t_unique_)
        , t_index(t_index_)
        , t_concurrently(t_concurrently_)
        , indexName(indexName_)
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , accMethod(accMethod_)
        , t_lparen(t_lparen_)
        , elements(finish(elements_))
        , t_rparen(t_rparen_)
        , reopts(reopts_)
        , tablespaceClause(tablespaceClause_)
        , whereClause(whereClause_)
    {}

    virtual CreateIndexStatementAST *asCreateIndexStatement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GrantRoleStmtAST: public StatementAST
{
public:
    int t_grant;
    CommonPrivilegeListAST *privs;
    int t_to;
    RoleIdListAST *roles;
    WithAdminOptionAST *withAdmin;
    GrantedByClauseAST *grantedBy;

public:
    GrantRoleStmtAST( int t_grant_, CommonPrivilegeListAST *privs_, int t_to_, RoleIdListAST *roles_, WithAdminOptionAST *withAdmin_, GrantedByClauseAST *grantedBy_ )
        : StatementAST()
        , t_grant(t_grant_)
        , privs(finish(privs_))
        , t_to(t_to_)
        , roles(finish(roles_))
        , withAdmin(withAdmin_)
        , grantedBy(grantedBy_)
    {}

    virtual GrantRoleStmtAST *asGrantRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RevokeRoleStmtAST: public StatementAST
{
public:
    int t_revoke;
    int t_admin;
    int t_option;
    int t_for;
    CommonPrivilegeListAST *privs;
    int t_from;
    RoleIdListAST *roles;
    GrantedByClauseAST *grantedBy;
    int t_behavior;

public:
    RevokeRoleStmtAST( int t_revoke_, int t_admin_, int t_option_, int t_for_, CommonPrivilegeListAST *privs_, int t_from_, RoleIdListAST *roles_, GrantedByClauseAST *grantedBy_, int t_behavior_ )
        : StatementAST()
        , t_revoke(t_revoke_)
        , t_admin(t_admin_)
        , t_option(t_option_)
        , t_for(t_for_)
        , privs(finish(privs_))
        , t_from(t_from_)
        , roles(finish(roles_))
        , grantedBy(grantedBy_)
        , t_behavior(t_behavior_)
    {}

    virtual RevokeRoleStmtAST *asRevokeRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithAdminOptionAST: public AST
{
public:
    int t_with;
    int t_admin;
    int t_option;

public:
    WithAdminOptionAST( int t_with_, int t_admin_, int t_option_ )
        : AST()
        , t_with(t_with_)
        , t_admin(t_admin_)
        , t_option(t_option_)
    {}

    virtual WithAdminOptionAST *asWithAdminOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GrantedByClauseAST: public AST
{
public:
    int t_granted;
    int t_by;
    RoleIdAST *granter;

public:
    GrantedByClauseAST( int t_granted_, int t_by_, RoleIdAST *granter_ )
        : AST()
        , t_granted(t_granted_)
        , t_by(t_by_)
        , granter(granter_)
    {}

    virtual GrantedByClauseAST *asGrantedByClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GrantStmtAST: public StatementAST
{
public:
    int t_grant;
    PrivilegesAST *privs;
    int t_on;
    PrivilegeTargetAST *target;
    int t_to;
    GranteeListAST *grantees;
    WithGrantOptionAST *withGrantOption;

public:
    GrantStmtAST( int t_grant_, PrivilegesAST *privs_, int t_on_, PrivilegeTargetAST *target_, int t_to_, GranteeListAST *grantees_, WithGrantOptionAST *withGrantOption_ )
        : StatementAST()
        , t_grant(t_grant_)
        , privs(privs_)
        , t_on(t_on_)
        , target(target_)
        , t_to(t_to_)
        , grantees(finish(grantees_))
        , withGrantOption(withGrantOption_)
    {}

    virtual GrantStmtAST *asGrantStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RevokeStmtAST: public StatementAST
{
public:
    int t_revoke;
    int t_grant;
    int t_option;
    int t_for;
    PrivilegesAST *privs;
    int t_on;
    PrivilegeTargetAST *target;
    int t_from;
    GranteeListAST *grantees;
    int t_behavior;

public:
    RevokeStmtAST( int t_revoke_, int t_grant_, int t_option_, int t_for_, PrivilegesAST *privs_, int t_on_, PrivilegeTargetAST *target_, int t_from_, GranteeListAST *grantees_, int t_behavior_ )
        : StatementAST()
        , t_revoke(t_revoke_)
        , t_grant(t_grant_)
        , t_option(t_option_)
        , t_for(t_for_)
        , privs(privs_)
        , t_on(t_on_)
        , target(target_)
        , t_from(t_from_)
        , grantees(finish(grantees_))
        , t_behavior(t_behavior_)
    {}

    virtual RevokeStmtAST *asRevokeStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrivilegeTargetAST: public AST
{
public:
    PrivilegeTargetAST() : AST() {}
    virtual PrivilegeTargetAST *asPrivilegeTarget() { return this; }
};

class TablePrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_table;
    TableIdentifierListAST *tables;

public:
    TablePrivilegeTargetAST( int t_table_, TableIdentifierListAST *tables_ )
        : PrivilegeTargetAST()
        , t_table(t_table_)
        , tables(finish(tables_))
    {}

    virtual TablePrivilegeTargetAST *asTablePrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SequencePrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_sequence;
    SequenceIdentifierListAST *seqs;

public:
    SequencePrivilegeTargetAST( int t_sequence_, SequenceIdentifierListAST *seqs_ )
        : PrivilegeTargetAST()
        , t_sequence(t_sequence_)
        , seqs(finish(seqs_))
    {}

    virtual SequencePrivilegeTargetAST *asSequencePrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FdwPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameListAST *wrappers;

public:
    FdwPrivilegeTargetAST( int t_foreign_, int t_data_, int t_wrapper_, NameListAST *wrappers_ )
        : PrivilegeTargetAST()
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , wrappers(finish(wrappers_))
    {}

    virtual FdwPrivilegeTargetAST *asFdwPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForeignSrvPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_foreign;
    int t_server;
    NameListAST *servers;

public:
    ForeignSrvPrivilegeTargetAST( int t_foreign_, int t_server_, NameListAST *servers_ )
        : PrivilegeTargetAST()
        , t_foreign(t_foreign_)
        , t_server(t_server_)
        , servers(finish(servers_))
    {}

    virtual ForeignSrvPrivilegeTargetAST *asForeignSrvPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_function;
    FunctionWithArgTypesListAST *functions;

public:
    FuncPrivilegeTargetAST( int t_function_, FunctionWithArgTypesListAST *functions_ )
        : PrivilegeTargetAST()
        , t_function(t_function_)
        , functions(finish(functions_))
    {}

    virtual FuncPrivilegeTargetAST *asFuncPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DbPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_database;
    NameListAST *databases;

public:
    DbPrivilegeTargetAST( int t_database_, NameListAST *databases_ )
        : PrivilegeTargetAST()
        , t_database(t_database_)
        , databases(finish(databases_))
    {}

    virtual DbPrivilegeTargetAST *asDbPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DomainPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_domain;
    AnyIdentifierListAST *domains;

public:
    DomainPrivilegeTargetAST( int t_domain_, AnyIdentifierListAST *domains_ )
        : PrivilegeTargetAST()
        , t_domain(t_domain_)
        , domains(finish(domains_))
    {}

    virtual DomainPrivilegeTargetAST *asDomainPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LangPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_language;
    NameListAST *langs;

public:
    LangPrivilegeTargetAST( int t_language_, NameListAST *langs_ )
        : PrivilegeTargetAST()
        , t_language(t_language_)
        , langs(finish(langs_))
    {}

    virtual LangPrivilegeTargetAST *asLangPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LargeObjPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_large;
    int t_object;
    NumericListAST *nums;

public:
    LargeObjPrivilegeTargetAST( int t_large_, int t_object_, NumericListAST *nums_ )
        : PrivilegeTargetAST()
        , t_large(t_large_)
        , t_object(t_object_)
        , nums(finish(nums_))
    {}

    virtual LargeObjPrivilegeTargetAST *asLargeObjPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SchemaPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_schema;
    NameListAST *schemas;

public:
    SchemaPrivilegeTargetAST( int t_schema_, NameListAST *schemas_ )
        : PrivilegeTargetAST()
        , t_schema(t_schema_)
        , schemas(finish(schemas_))
    {}

    virtual SchemaPrivilegeTargetAST *asSchemaPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TblspacePrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_tablespace;
    NameListAST *tablespaces;

public:
    TblspacePrivilegeTargetAST( int t_tablespace_, NameListAST *tablespaces_ )
        : PrivilegeTargetAST()
        , t_tablespace(t_tablespace_)
        , tablespaces(finish(tablespaces_))
    {}

    virtual TblspacePrivilegeTargetAST *asTblspacePrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypePrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_type;
    AnyIdentifierListAST *types;

public:
    TypePrivilegeTargetAST( int t_type_, AnyIdentifierListAST *types_ )
        : PrivilegeTargetAST()
        , t_type(t_type_)
        , types(finish(types_))
    {}

    virtual TypePrivilegeTargetAST *asTypePrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AllInSchPrivilegeTargetAST: public PrivilegeTargetAST
{
public:
    int t_all;
    int t_sp_obj;
    int t_in;
    int t_schema;
    NameListAST *schemas;

public:
    AllInSchPrivilegeTargetAST( int t_all_, int t_sp_obj_, int t_in_, int t_schema_, NameListAST *schemas_ )
        : PrivilegeTargetAST()
        , t_all(t_all_)
        , t_sp_obj(t_sp_obj_)
        , t_in(t_in_)
        , t_schema(t_schema_)
        , schemas(finish(schemas_))
    {}

    virtual AllInSchPrivilegeTargetAST *asAllInSchPrivilegeTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NumericAST: public AST
{
public:
    int t_sp_num;

public:
    NumericAST( int t_sp_num_ )
        : AST()
        , t_sp_num(t_sp_num_)
    {}

    virtual NumericAST *asNumeric() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FetchStmtAST: public StatementAST
{
public:
    int t_fetch_or_move;
    FetchClauseAST *fetchClause;

public:
    FetchStmtAST( int t_fetch_or_move_, FetchClauseAST *fetchClause_ )
        : StatementAST()
        , t_fetch_or_move(t_fetch_or_move_)
        , fetchClause(fetchClause_)
    {}

    virtual FetchStmtAST *asFetchStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FetchClauseAST: public AST
{
public:
    int t_sp_step;
    int t_sp_step_spec;
    int t_from_or_in;
    NameAST *cursorName;

public:
    FetchClauseAST( int t_sp_step_, int t_sp_step_spec_, int t_from_or_in_, NameAST *cursorName_ )
        : AST()
        , t_sp_step(t_sp_step_)
        , t_sp_step_spec(t_sp_step_spec_)
        , t_from_or_in(t_from_or_in_)
        , cursorName(cursorName_)
    {}

    virtual FetchClauseAST *asFetchClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseExplainStmtAST: public StatementAST
{
public:
    BaseExplainStmtAST() : StatementAST() {}
    virtual BaseExplainStmtAST *asBaseExplainStmt() { return this; }
};

class CommonExplainStmtAST: public BaseExplainStmtAST
{
public:
    int t_explain;
    int t_analize;
    int t_verbose;
    StatementAST *explStmt;

public:
    CommonExplainStmtAST( int t_explain_, int t_analize_, int t_verbose_, StatementAST *explStmt_ )
        : BaseExplainStmtAST()
        , t_explain(t_explain_)
        , t_analize(t_analize_)
        , t_verbose(t_verbose_)
        , explStmt(explStmt_)
    {}

    virtual CommonExplainStmtAST *asCommonExplainStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OptExplainStmtAST: public BaseExplainStmtAST
{
public:
    int t_explain;
    int t_lparen;
    ExplainOptionListAST *options;
    int t_rparen;
    StatementAST *explStmt;

public:
    OptExplainStmtAST( int t_explain_, int t_lparen_, ExplainOptionListAST *options_, int t_rparen_, StatementAST *explStmt_ )
        : BaseExplainStmtAST()
        , t_explain(t_explain_)
        , t_lparen(t_lparen_)
        , options(finish(options_))
        , t_rparen(t_rparen_)
        , explStmt(explStmt_)
    {}

    virtual OptExplainStmtAST *asOptExplainStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExplainOptionAST: public AST
{
public:
    int t_sp_name;
    int t_sp_arg;

public:
    ExplainOptionAST( int t_sp_name_, int t_sp_arg_ )
        : AST()
        , t_sp_name(t_sp_name_)
        , t_sp_arg(t_sp_arg_)
    {}

    virtual ExplainOptionAST *asExplainOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RefreshMatViewStmtAST: public StatementAST
{
public:
    int t_refresh;
    int t_materialized;
    int t_view;
    TableIdentifierAST *viewIdent;
    WithDataClauseAST *withData;

public:
    RefreshMatViewStmtAST( int t_refresh_, int t_materialized_, int t_view_, TableIdentifierAST *viewIdent_, WithDataClauseAST *withData_ )
        : StatementAST()
        , t_refresh(t_refresh_)
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , viewIdent(viewIdent_)
        , withData(withData_)
    {}

    virtual RefreshMatViewStmtAST *asRefreshMatViewStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropDbStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_database;
    int t_if;
    int t_exists;
    DatabaseNameAST *dbName;

public:
    DropDbStmtAST( int t_drop_, int t_database_, int t_if_, int t_exists_, DatabaseNameAST *dbName_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_database(t_database_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , dbName(dbName_)
    {}

    virtual DropDbStmtAST *asDropDbStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropUserMappingStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_user;
    int t_mapping;
    int t_if;
    int t_exists;
    int t_for;
    AuthIdentifierAST *authIdent;
    int t_server;
    NameAST *serverName;

public:
    DropUserMappingStmtAST( int t_drop_, int t_user_, int t_mapping_, int t_if_, int t_exists_, int t_for_, AuthIdentifierAST *authIdent_, int t_server_, NameAST *serverName_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_user(t_user_)
        , t_mapping(t_mapping_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , t_for(t_for_)
        , authIdent(authIdent_)
        , t_server(t_server_)
        , serverName(serverName_)
    {}

    virtual DropUserMappingStmtAST *asDropUserMappingStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropTableSpaceStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_tablespace;
    int t_if;
    int t_exists;
    NameAST *tablespaceName;

public:
    DropTableSpaceStmtAST( int t_drop_, int t_tablespace_, int t_if_, int t_exists_, NameAST *tablespaceName_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_tablespace(t_tablespace_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , tablespaceName(tablespaceName_)
    {}

    virtual DropTableSpaceStmtAST *asDropTableSpaceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonDropStmtAST: public AST
{
public:
    int t_drop;
    CommonDropTypeAST *dropType;
    int t_if;
    int t_exists;
    AnyIdentifierListAST *idents;
    int t_behavior;

public:
    CommonDropStmtAST( int t_drop_, CommonDropTypeAST *dropType_, int t_if_, int t_exists_, AnyIdentifierListAST *idents_, int t_behavior_ )
        : AST()
        , t_drop(t_drop_)
        , dropType(dropType_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , idents(finish(idents_))
        , t_behavior(t_behavior_)
    {}

    virtual CommonDropStmtAST *asCommonDropStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonDropTypeAST: public AST
{
public:
    int t_sp1;
    int t_sp2;
    int t_sp3;

public:
    CommonDropTypeAST( int t_sp1_, int t_sp2_, int t_sp3_ )
        : AST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
        , t_sp3(t_sp3_)
    {}

    virtual CommonDropTypeAST *asCommonDropType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropTableStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_foreign;
    int t_table;
    int t_if;
    int t_exists;
    TableIdentifierListAST *tables;
    int t_behavior;

public:
    DropTableStmtAST( int t_drop_, int t_foreign_, int t_table_, int t_if_, int t_exists_, TableIdentifierListAST *tables_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , tables(finish(tables_))
        , t_behavior(t_behavior_)
    {}

    virtual DropTableStmtAST *asDropTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropViewStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_materialized;
    int t_view;
    int t_if;
    int t_exists;
    TableIdentifierListAST *tables;
    int t_behavior;

public:
    DropViewStmtAST( int t_drop_, int t_materialized_, int t_view_, int t_if_, int t_exists_, TableIdentifierListAST *tables_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , tables(finish(tables_))
        , t_behavior(t_behavior_)
    {}

    virtual DropViewStmtAST *asDropViewStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropIndexStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_index;
    int t_concurrently;
    int t_if;
    int t_exists;
    IndexIdentifierListAST *indexes;
    int t_behavior;

public:
    DropIndexStmtAST( int t_drop_, int t_index_, int t_concurrently_, int t_if_, int t_exists_, IndexIdentifierListAST *indexes_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_index(t_index_)
        , t_concurrently(t_concurrently_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , indexes(finish(indexes_))
        , t_behavior(t_behavior_)
    {}

    virtual DropIndexStmtAST *asDropIndexStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropSchemaStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_schema;
    int t_if;
    int t_exists;
    NameListAST *schemas;
    int t_behavior;

public:
    DropSchemaStmtAST( int t_drop_, int t_schema_, int t_if_, int t_exists_, NameListAST *schemas_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_schema(t_schema_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , schemas(finish(schemas_))
        , t_behavior(t_behavior_)
    {}

    virtual DropSchemaStmtAST *asDropSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropSequenceStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_sequence;
    int t_if;
    int t_exists;
    SequenceIdentifierListAST *seqs;
    int t_behavior;

public:
    DropSequenceStmtAST( int t_drop_, int t_sequence_, int t_if_, int t_exists_, SequenceIdentifierListAST *seqs_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_sequence(t_sequence_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , seqs(finish(seqs_))
        , t_behavior(t_behavior_)
    {}

    virtual DropSequenceStmtAST *asDropSequenceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RuleStmtAST: public CreateStatementAST
{
public:
    int t_create;
    OrReplaceClauseAST *orReplace;
    int t_rule;
    NameAST *ruleName;
    int t_as;
    int t_on;
    int t_sp_event;
    int t_to;
    TableIdentifierAST *tableIdent;
    WhereClauseAST *whereClause;
    int t_do;
    int t_instaed;
    RuleActionsAST *actions;

public:
    RuleStmtAST( int t_create_, OrReplaceClauseAST *orReplace_, int t_rule_, NameAST *ruleName_, int t_as_, int t_on_, int t_sp_event_, int t_to_, TableIdentifierAST *tableIdent_, WhereClauseAST *whereClause_, int t_do_, int t_instaed_, RuleActionsAST *actions_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , orReplace(orReplace_)
        , t_rule(t_rule_)
        , ruleName(ruleName_)
        , t_as(t_as_)
        , t_on(t_on_)
        , t_sp_event(t_sp_event_)
        , t_to(t_to_)
        , tableIdent(tableIdent_)
        , whereClause(whereClause_)
        , t_do(t_do_)
        , t_instaed(t_instaed_)
        , actions(actions_)
    {}

    virtual RuleStmtAST *asRuleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RuleActionsAST: public AST
{
public:
    RuleActionsAST() : AST() {}
    virtual RuleActionsAST *asRuleActions() { return this; }
};

class NothingRuleActionsAST: public RuleActionsAST
{
public:
    int t_nothing;

public:
    NothingRuleActionsAST( int t_nothing_ )
        : RuleActionsAST()
        , t_nothing(t_nothing_)
    {}

    virtual NothingRuleActionsAST *asNothingRuleActions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SingleRuleActionsAST: public RuleActionsAST
{
public:
    StatementAST *actStatement;

public:
    SingleRuleActionsAST( StatementAST *actStatement_ )
        : RuleActionsAST()
        , actStatement(actStatement_)
    {}

    virtual SingleRuleActionsAST *asSingleRuleActions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class MultiRuleActionsAST: public RuleActionsAST
{
public:
    int t_lparen;
    StatementListAST *actions;
    int t_rparen;

public:
    MultiRuleActionsAST( int t_lparen_, StatementListAST *actions_, int t_rparen_ )
        : RuleActionsAST()
        , t_lparen(t_lparen_)
        , actions(finish(actions_))
        , t_rparen(t_rparen_)
    {}

    virtual MultiRuleActionsAST *asMultiRuleActions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropRuleStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_rule;
    int t_if;
    int t_exists;
    NameAST *ruleName;
    int t_on;
    TableIdentifierAST *tableIdent;
    int t_behavior;

public:
    DropRuleStmtAST( int t_drop_, int t_rule_, int t_if_, int t_exists_, NameAST *ruleName_, int t_on_, TableIdentifierAST *tableIdent_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_rule(t_rule_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , ruleName(ruleName_)
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , t_behavior(t_behavior_)
    {}

    virtual DropRuleStmtAST *asDropRuleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropOwnedStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_owned;
    int t_by;
    RoleIdListAST *roles;
    int t_behavior;

public:
    DropOwnedStmtAST( int t_drop_, int t_owned_, int t_by_, RoleIdListAST *roles_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_owned(t_owned_)
        , t_by(t_by_)
        , roles(finish(roles_))
        , t_behavior(t_behavior_)
    {}

    virtual DropOwnedStmtAST *asDropOwnedStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReassignOwnedStmtAST: public DdlStatementAST
{
public:
    int t_reassign;
    int t_owned;
    int t_by;
    RoleIdListAST *roles;
    int t_to;
    NameAST *newRole;

public:
    ReassignOwnedStmtAST( int t_reassign_, int t_owned_, int t_by_, RoleIdListAST *roles_, int t_to_, NameAST *newRole_ )
        : DdlStatementAST()
        , t_reassign(t_reassign_)
        , t_owned(t_owned_)
        , t_by(t_by_)
        , roles(finish(roles_))
        , t_to(t_to_)
        , newRole(newRole_)
    {}

    virtual ReassignOwnedStmtAST *asReassignOwnedStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropGroupStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_group;
    int t_if;
    int t_exists;
    RoleIdListAST *roles;

public:
    DropGroupStmtAST( int t_drop_, int t_group_, int t_if_, int t_exists_, RoleIdListAST *roles_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_group(t_group_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , roles(finish(roles_))
    {}

    virtual DropGroupStmtAST *asDropGroupStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropForeignServerStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_server;
    int t_if;
    int t_exists;
    NameAST *serverName;
    int t_behavior;

public:
    DropForeignServerStmtAST( int t_drop_, int t_server_, int t_if_, int t_exists_, NameAST *serverName_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_server(t_server_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , serverName(serverName_)
        , t_behavior(t_behavior_)
    {}

    virtual DropForeignServerStmtAST *asDropForeignServerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropFdwStmtAST: public DropStmtAST
{
public:
    int t_drop;
    int t_foreign;
    int t_data;
    int t_wrapper;
    int t_if;
    int t_exists;
    NameAST *fdwName;
    int t_behavior;

public:
    DropFdwStmtAST( int t_drop_, int t_foreign_, int t_data_, int t_wrapper_, int t_if_, int t_exists_, NameAST *fdwName_, int t_behavior_ )
        : DropStmtAST()
        , t_drop(t_drop_)
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , fdwName(fdwName_)
        , t_behavior(t_behavior_)
    {}

    virtual DropFdwStmtAST *asDropFdwStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DoStmtAST: public StatementAST
{
public:
    int t_do;
    DoStmtItemListAST *items;

public:
    DoStmtAST( int t_do_, DoStmtItemListAST *items_ )
        : StatementAST()
        , t_do(t_do_)
        , items(finish(items_))
    {}

    virtual DoStmtAST *asDoStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DoStmtItemAST: public AST
{
public:
    DoStmtItemAST() : AST() {}
    virtual DoStmtItemAST *asDoStmtItem() { return this; }
};

class DoStmtCmdItemAST: public DoStmtItemAST
{
public:
    int t_str_cmd;

public:
    DoStmtCmdItemAST( int t_str_cmd_ )
        : DoStmtItemAST()
        , t_str_cmd(t_str_cmd_)
    {}

    virtual DoStmtCmdItemAST *asDoStmtCmdItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DoStmtLangItemAST: public DoStmtItemAST
{
public:
    int t_language;
    int t_lang_label;

public:
    DoStmtLangItemAST( int t_language_, int t_lang_label_ )
        : DoStmtItemAST()
        , t_language(t_language_)
        , t_lang_label(t_lang_label_)
    {}

    virtual DoStmtLangItemAST *asDoStmtLangItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateOldAggregateStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_aggregate;
    FunctionRefAST *functionRef;
    AggregateDefinitionAST *aggrDefinition;

public:
    CreateOldAggregateStmtAST( int t_create_, int t_aggregate_, FunctionRefAST *functionRef_, AggregateDefinitionAST *aggrDefinition_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_aggregate(t_aggregate_)
        , functionRef(functionRef_)
        , aggrDefinition(aggrDefinition_)
    {}

    virtual CreateOldAggregateStmtAST *asCreateOldAggregateStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AggregateDefinitionAST: public AST
{
public:
    int t_lparen;
    AggrDefinitionListAST *defs;
    int t_rparen;

public:
    AggregateDefinitionAST( int t_lparen_, AggrDefinitionListAST *defs_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , defs(finish(defs_))
        , t_rparen(t_rparen_)
    {}

    virtual AggregateDefinitionAST *asAggregateDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AggrDefinitionAST: public AST
{
public:
    int t_identifier;
    int t_equal;
    DefinitionArgumentAST *arg;

public:
    AggrDefinitionAST( int t_identifier_, int t_equal_, DefinitionArgumentAST *arg_ )
        : AST()
        , t_identifier(t_identifier_)
        , t_equal(t_equal_)
        , arg(arg_)
    {}

    virtual AggrDefinitionAST *asAggrDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateAggregateStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_aggregate;
    FunctionRefAST *functionRef;
    AggrArgumentsAST *args;
    DefinitionAST *defintion;

public:
    CreateAggregateStmtAST( int t_create_, int t_aggregate_, FunctionRefAST *functionRef_, AggrArgumentsAST *args_, DefinitionAST *defintion_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_aggregate(t_aggregate_)
        , functionRef(functionRef_)
        , args(args_)
        , defintion(defintion_)
    {}

    virtual CreateAggregateStmtAST *asCreateAggregateStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateOpStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_operator;
    OperandAST *operand;
    DefinitionAST *defintion;

public:
    CreateOpStmtAST( int t_create_, int t_operator_, OperandAST *operand_, DefinitionAST *defintion_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_operator(t_operator_)
        , operand(operand_)
        , defintion(defintion_)
    {}

    virtual CreateOpStmtAST *asCreateOpStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTypeStmtAST: public CreateStatementAST
{
public:
    CreateTypeStmtAST() : CreateStatementAST() {}
    virtual CreateTypeStmtAST *asCreateTypeStmt() { return this; }
};

class CreateTypeAsRecordStmtAST: public CreateTypeStmtAST
{
public:
    int t_create;
    int t_type;
    AnyIdentifierAST *typeIdent;
    int t_as;
    int t_lparen;
    TableFuncElementListAST *members;
    int t_rparen;

public:
    CreateTypeAsRecordStmtAST( int t_create_, int t_type_, AnyIdentifierAST *typeIdent_, int t_as_, int t_lparen_, TableFuncElementListAST *members_, int t_rparen_ )
        : CreateTypeStmtAST()
        , t_create(t_create_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , t_as(t_as_)
        , t_lparen(t_lparen_)
        , members(finish(members_))
        , t_rparen(t_rparen_)
    {}

    virtual CreateTypeAsRecordStmtAST *asCreateTypeAsRecordStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTypeAsEnumStmtAST: public CreateTypeStmtAST
{
public:
    int t_create;
    int t_type;
    AnyIdentifierAST *typeIdent;
    int t_as;
    int t_enum;
    int t_lparen;
    EnumValueListAST *values;
    int t_rparen;

public:
    CreateTypeAsEnumStmtAST( int t_create_, int t_type_, AnyIdentifierAST *typeIdent_, int t_as_, int t_enum_, int t_lparen_, EnumValueListAST *values_, int t_rparen_ )
        : CreateTypeStmtAST()
        , t_create(t_create_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , t_as(t_as_)
        , t_enum(t_enum_)
        , t_lparen(t_lparen_)
        , values(finish(values_))
        , t_rparen(t_rparen_)
    {}

    virtual CreateTypeAsEnumStmtAST *asCreateTypeAsEnumStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EnumValueAST: public AST
{
public:
    int t_str_value;

public:
    EnumValueAST( int t_str_value_ )
        : AST()
        , t_str_value(t_str_value_)
    {}

    virtual EnumValueAST *asEnumValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTypeAsDefStmtAST: public CreateTypeStmtAST
{
public:
    int t_create;
    int t_type;
    AnyIdentifierAST *typeIdent;
    DefinitionAST *defintion;

public:
    CreateTypeAsDefStmtAST( int t_create_, int t_type_, AnyIdentifierAST *typeIdent_, DefinitionAST *defintion_ )
        : CreateTypeStmtAST()
        , t_create(t_create_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , defintion(defintion_)
    {}

    virtual CreateTypeAsDefStmtAST *asCreateTypeAsDefStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTypeAsRangeStmtAST: public CreateTypeStmtAST
{
public:
    int t_create;
    int t_type;
    AnyIdentifierAST *typeIdent;
    int t_as;
    int t_range;
    DefinitionAST *defintion;

public:
    CreateTypeAsRangeStmtAST( int t_create_, int t_type_, AnyIdentifierAST *typeIdent_, int t_as_, int t_range_, DefinitionAST *defintion_ )
        : CreateTypeStmtAST()
        , t_create(t_create_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , t_as(t_as_)
        , t_range(t_range_)
        , defintion(defintion_)
    {}

    virtual CreateTypeAsRangeStmtAST *asCreateTypeAsRangeStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateSearchObjStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_text;
    int t_search;
    int t_sp_obj;
    AnyIdentifierAST *ident;
    DefinitionAST *defintion;

public:
    CreateSearchObjStmtAST( int t_create_, int t_text_, int t_search_, int t_sp_obj_, AnyIdentifierAST *ident_, DefinitionAST *defintion_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_text(t_text_)
        , t_search(t_search_)
        , t_sp_obj(t_sp_obj_)
        , ident(ident_)
        , defintion(defintion_)
    {}

    virtual CreateSearchObjStmtAST *asCreateSearchObjStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateCollationStmtAST: public CreateStatementAST
{
public:
    CreateCollationStmtAST() : CreateStatementAST() {}
    virtual CreateCollationStmtAST *asCreateCollationStmt() { return this; }
};

class CreateCollationDefStmtAST: public CreateCollationStmtAST
{
public:
    int t_create;
    int t_collation;
    AnyIdentifierAST *collationIdent;
    DefinitionAST *defintion;

public:
    CreateCollationDefStmtAST( int t_create_, int t_collation_, AnyIdentifierAST *collationIdent_, DefinitionAST *defintion_ )
        : CreateCollationStmtAST()
        , t_create(t_create_)
        , t_collation(t_collation_)
        , collationIdent(collationIdent_)
        , defintion(defintion_)
    {}

    virtual CreateCollationDefStmtAST *asCreateCollationDefStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateCollationFromStmtAST: public CreateCollationStmtAST
{
public:
    int t_create;
    int t_collation;
    AnyIdentifierAST *collationIdent;
    int t_from;
    AnyIdentifierAST *fromCollationIdent;

public:
    CreateCollationFromStmtAST( int t_create_, int t_collation_, AnyIdentifierAST *collationIdent_, int t_from_, AnyIdentifierAST *fromCollationIdent_ )
        : CreateCollationStmtAST()
        , t_create(t_create_)
        , t_collation(t_collation_)
        , collationIdent(collationIdent_)
        , t_from(t_from_)
        , fromCollationIdent(fromCollationIdent_)
    {}

    virtual CreateCollationFromStmtAST *asCreateCollationFromStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeclareCursorStmtAST: public StatementAST
{
public:
    int t_declare;
    NameAST *cursorName;
    int t_cursor;
    CursorHoldOptionAST *holdOption;
    int t_for;
    SelectStmtAST *query;

public:
    DeclareCursorStmtAST( int t_declare_, NameAST *cursorName_, int t_cursor_, CursorHoldOptionAST *holdOption_, int t_for_, SelectStmtAST *query_ )
        : StatementAST()
        , t_declare(t_declare_)
        , cursorName(cursorName_)
        , t_cursor(t_cursor_)
        , holdOption(holdOption_)
        , t_for(t_for_)
        , query(query_)
    {}

    virtual DeclareCursorStmtAST *asDeclareCursorStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CursorHoldOptionAST: public AST
{
public:
    int t_with_or_without;
    int t_hold;

public:
    CursorHoldOptionAST( int t_with_or_without_, int t_hold_ )
        : AST()
        , t_with_or_without(t_with_or_without_)
        , t_hold(t_hold_)
    {}

    virtual CursorHoldOptionAST *asCursorHoldOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeallocateStmtAST: public StatementAST
{
public:
    int t_deallocate;
    int t_prepare;
    int t_all_or_plan_name;

public:
    DeallocateStmtAST( int t_deallocate_, int t_prepare_, int t_all_or_plan_name_ )
        : StatementAST()
        , t_deallocate(t_deallocate_)
        , t_prepare(t_prepare_)
        , t_all_or_plan_name(t_all_or_plan_name_)
    {}

    virtual DeallocateStmtAST *asDeallocateStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateDbStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_database;
    DatabaseNameAST *dbName;
    int t_with;
    CreateDbOptionListAST *options;

public:
    CreateDbStmtAST( int t_create_, int t_database_, DatabaseNameAST *dbName_, int t_with_, CreateDbOptionListAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_database(t_database_)
        , dbName(dbName_)
        , t_with(t_with_)
        , options(finish(options_))
    {}

    virtual CreateDbStmtAST *asCreateDbStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateDbOptionAST: public AST
{
public:
    CreateDbOptionAST() : AST() {}
    virtual CreateDbOptionAST *asCreateDbOption() { return this; }
};

class NamedCreateDbOptionAST: public CreateDbOptionAST
{
public:
    int t_sp_opt;
    int t_equal;
    NameAST *optName;

public:
    NamedCreateDbOptionAST( int t_sp_opt_, int t_equal_, NameAST *optName_ )
        : CreateDbOptionAST()
        , t_sp_opt(t_sp_opt_)
        , t_equal(t_equal_)
        , optName(optName_)
    {}

    virtual NamedCreateDbOptionAST *asNamedCreateDbOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ValDefCreateDbOptionAST: public CreateDbOptionAST
{
public:
    int t_sp1;
    int t_sp2;
    int t_equal;
    int t_default_or_value;

public:
    ValDefCreateDbOptionAST( int t_sp1_, int t_sp2_, int t_equal_, int t_default_or_value_ )
        : CreateDbOptionAST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
        , t_equal(t_equal_)
        , t_default_or_value(t_default_or_value_)
    {}

    virtual ValDefCreateDbOptionAST *asValDefCreateDbOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateUserMappingStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_user;
    int t_mapping;
    int t_for;
    AuthIdentifierAST *authIdent;
    int t_server;
    NameAST *serverName;
    CreateGeneticOptionsAST *options;

public:
    CreateUserMappingStmtAST( int t_create_, int t_user_, int t_mapping_, int t_for_, AuthIdentifierAST *authIdent_, int t_server_, NameAST *serverName_, CreateGeneticOptionsAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_user(t_user_)
        , t_mapping(t_mapping_)
        , t_for(t_for_)
        , authIdent(authIdent_)
        , t_server(t_server_)
        , serverName(serverName_)
        , options(options_)
    {}

    virtual CreateUserMappingStmtAST *asCreateUserMappingStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropTrigStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_trigger;
    int t_if;
    int t_exists;
    NameAST *triggerName;
    int t_on;
    TableIdentifierAST *tableIdent;
    int t_behavior;

public:
    DropTrigStmtAST( int t_drop_, int t_trigger_, int t_if_, int t_exists_, NameAST *triggerName_, int t_on_, TableIdentifierAST *tableIdent_, int t_behavior_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_trigger(t_trigger_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , triggerName(triggerName_)
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , t_behavior(t_behavior_)
    {}

    virtual DropTrigStmtAST *asDropTrigStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateConstrTrigStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_constraint;
    int t_trigger;
    NameAST *triggerName;
    int t_after;
    TriggerEventListAST *events;
    int t_on;
    TableIdentifierAST *tableIdent;
    ConstrFromTableAST *fromTable;
    ConstraintAttributeListAST *constraints;
    TriggerForSpecAST *forSpec;
    TriggerWhenAST *trgWhen;
    TriggerExecuteClauseAST *execClause;

public:
    CreateConstrTrigStmtAST( int t_create_, int t_constraint_, int t_trigger_, NameAST *triggerName_, int t_after_, TriggerEventListAST *events_, int t_on_, TableIdentifierAST *tableIdent_, ConstrFromTableAST *fromTable_, ConstraintAttributeListAST *constraints_, TriggerForSpecAST *forSpec_, TriggerWhenAST *trgWhen_, TriggerExecuteClauseAST *execClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_constraint(t_constraint_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , t_after(t_after_)
        , events(finish(events_))
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , fromTable(fromTable_)
        , constraints(finish(constraints_))
        , forSpec(forSpec_)
        , trgWhen(trgWhen_)
        , execClause(execClause_)
    {}

    virtual CreateConstrTrigStmtAST *asCreateConstrTrigStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstrFromTableAST: public AST
{
public:
    int t_from;
    TableIdentifierAST *tableIdent;

public:
    ConstrFromTableAST( int t_from_, TableIdentifierAST *tableIdent_ )
        : AST()
        , t_from(t_from_)
        , tableIdent(tableIdent_)
    {}

    virtual ConstrFromTableAST *asConstrFromTable() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTrigStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_trigger;
    NameAST *triggerName;
    TriggerActionTimeAST *actTime;
    TriggerEventListAST *events;
    int t_on;
    TableIdentifierAST *tableIdent;
    TriggerForSpecAST *forSpec;
    TriggerWhenAST *trgWhen;
    TriggerExecuteClauseAST *execClause;

public:
    CreateTrigStmtAST( int t_create_, int t_trigger_, NameAST *triggerName_, TriggerActionTimeAST *actTime_, TriggerEventListAST *events_, int t_on_, TableIdentifierAST *tableIdent_, TriggerForSpecAST *forSpec_, TriggerWhenAST *trgWhen_, TriggerExecuteClauseAST *execClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , actTime(actTime_)
        , events(finish(events_))
        , t_on(t_on_)
        , tableIdent(tableIdent_)
        , forSpec(forSpec_)
        , trgWhen(trgWhen_)
        , execClause(execClause_)
    {}

    virtual CreateTrigStmtAST *asCreateTrigStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerExecuteClauseAST: public AST
{
public:
    int t_execute;
    int t_procedure;
    FunctionRefAST *functionRef;
    int t_lparen;
    TriggerFuncArgListAST *args;
    int t_rparen;

public:
    TriggerExecuteClauseAST( int t_execute_, int t_procedure_, FunctionRefAST *functionRef_, int t_lparen_, TriggerFuncArgListAST *args_, int t_rparen_ )
        : AST()
        , t_execute(t_execute_)
        , t_procedure(t_procedure_)
        , functionRef(functionRef_)
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual TriggerExecuteClauseAST *asTriggerExecuteClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerFuncArgAST: public AST
{
public:
    int t_arg_value;

public:
    TriggerFuncArgAST( int t_arg_value_ )
        : AST()
        , t_arg_value(t_arg_value_)
    {}

    virtual TriggerFuncArgAST *asTriggerFuncArg() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerWhenAST: public AST
{
public:
    int t_when;
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;

public:
    TriggerWhenAST( int t_when_, int t_lparen_, ExpressionAST *expr_, int t_rparen_ )
        : AST()
        , t_when(t_when_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
    {}

    virtual TriggerWhenAST *asTriggerWhen() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerForSpecAST: public AST
{
public:
    int t_for;
    int t_each;
    int t_row_or_statement;

public:
    TriggerForSpecAST( int t_for_, int t_each_, int t_row_or_statement_ )
        : AST()
        , t_for(t_for_)
        , t_each(t_each_)
        , t_row_or_statement(t_row_or_statement_)
    {}

    virtual TriggerForSpecAST *asTriggerForSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerEventAST: public AST
{
public:
    int t_sp_event;
    int t_of;
    NameListAST *columns;

public:
    TriggerEventAST( int t_sp_event_, int t_of_, NameListAST *columns_ )
        : AST()
        , t_sp_event(t_sp_event_)
        , t_of(t_of_)
        , columns(finish(columns_))
    {}

    virtual TriggerEventAST *asTriggerEvent() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerActionTimeAST: public AST
{
public:
    int t_before_after_instaed;
    int t_of;

public:
    TriggerActionTimeAST( int t_before_after_instaed_, int t_of_ )
        : AST()
        , t_before_after_instaed(t_before_after_instaed_)
        , t_of(t_of_)
    {}

    virtual TriggerActionTimeAST *asTriggerActionTime() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableSpaceStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_tablespace;
    NameAST *tablespaceName;
    TableSpaceOwnerClauseAST *tablespaceOwner;
    int t_location;
    int t_path_value;

public:
    CreateTableSpaceStmtAST( int t_create_, int t_tablespace_, NameAST *tablespaceName_, TableSpaceOwnerClauseAST *tablespaceOwner_, int t_location_, int t_path_value_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
        , tablespaceOwner(tablespaceOwner_)
        , t_location(t_location_)
        , t_path_value(t_path_value_)
    {}

    virtual CreateTableSpaceStmtAST *asCreateTableSpaceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableSpaceOwnerClauseAST: public AST
{
public:
    int t_owner;
    NameAST *ownerName;

public:
    TableSpaceOwnerClauseAST( int t_owner_, NameAST *ownerName_ )
        : AST()
        , t_owner(t_owner_)
        , ownerName(ownerName_)
    {}

    virtual TableSpaceOwnerClauseAST *asTableSpaceOwnerClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableAsExecStmtAST: public CreateStatementAST
{
public:
    int t_create;
    TempClauseAST *tempClause;
    int t_table;
    CreateAsTargetAST *target;
    int t_as;
    ExecuteStmtAST *execStmt;
    WithDataClauseAST *withData;

public:
    CreateTableAsExecStmtAST( int t_create_, TempClauseAST *tempClause_, int t_table_, CreateAsTargetAST *target_, int t_as_, ExecuteStmtAST *execStmt_, WithDataClauseAST *withData_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , tempClause(tempClause_)
        , t_table(t_table_)
        , target(target_)
        , t_as(t_as_)
        , execStmt(execStmt_)
        , withData(withData_)
    {}

    virtual CreateTableAsExecStmtAST *asCreateTableAsExecStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExecuteStmtAST: public StatementAST
{
public:
    int t_execute;
    NameAST *planName;
    ExecuteParamClauseAST *parClause;

public:
    ExecuteStmtAST( int t_execute_, NameAST *planName_, ExecuteParamClauseAST *parClause_ )
        : StatementAST()
        , t_execute(t_execute_)
        , planName(planName_)
        , parClause(parClause_)
    {}

    virtual ExecuteStmtAST *asExecuteStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExecuteParamClauseAST: public AST
{
public:
    int t_lparen;
    ExpressionListAST *params;
    int t_rparen;

public:
    ExecuteParamClauseAST( int t_lparen_, ExpressionListAST *params_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , params(finish(params_))
        , t_rparen(t_rparen_)
    {}

    virtual ExecuteParamClauseAST *asExecuteParamClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableStmtAST: public CreateStatementAST
{
public:
    int t_create;
    TempClauseAST *tempClause;
    int t_table;
    int t_if;
    int t_not;
    int t_exists;
    TableIdentifierAST *tableIdent;
    CreateTableClauseAST *createClause;
    CreateTableOptionsAST *options;

public:
    CreateTableStmtAST( int t_create_, TempClauseAST *tempClause_, int t_table_, int t_if_, int t_not_, int t_exists_, TableIdentifierAST *tableIdent_, CreateTableClauseAST *createClause_, CreateTableOptionsAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , tempClause(tempClause_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_not(t_not_)
        , t_exists(t_exists_)
        , tableIdent(tableIdent_)
        , createClause(createClause_)
        , options(options_)
    {}

    virtual CreateTableStmtAST *asCreateTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableOptionsAST: public AST
{
public:
    CreateWithOptionsAST *withOptions;
    OnCommitOptionAST *commitOption;
    TableSpaceClauseAST *tablespaceClause;

public:
    CreateTableOptionsAST( CreateWithOptionsAST *withOptions_, OnCommitOptionAST *commitOption_, TableSpaceClauseAST *tablespaceClause_ )
        : AST()
        , withOptions(withOptions_)
        , commitOption(commitOption_)
        , tablespaceClause(tablespaceClause_)
    {}

    virtual CreateTableOptionsAST *asCreateTableOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableClauseAST: public AST
{
public:
    CreateTableClauseAST() : AST() {}
    virtual CreateTableClauseAST *asCreateTableClause() { return this; }
};

class CreateTableDirecClauseAST: public CreateTableClauseAST
{
public:
    int t_lparen;
    TableElementListAST *columns;
    int t_rparen;
    TableInheritClauseAST *inherit;

public:
    CreateTableDirecClauseAST( int t_lparen_, TableElementListAST *columns_, int t_rparen_, TableInheritClauseAST *inherit_ )
        : CreateTableClauseAST()
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , inherit(inherit_)
    {}

    virtual CreateTableDirecClauseAST *asCreateTableDirecClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableOfClauseAST: public CreateTableClauseAST
{
public:
    int t_of;
    AnyIdentifierAST *ident;
    TypedTableElementsAST *elements;

public:
    CreateTableOfClauseAST( int t_of_, AnyIdentifierAST *ident_, TypedTableElementsAST *elements_ )
        : CreateTableClauseAST()
        , t_of(t_of_)
        , ident(ident_)
        , elements(elements_)
    {}

    virtual CreateTableOfClauseAST *asCreateTableOfClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypedTableElementsAST: public AST
{
public:
    int t_lparen;
    TypedTableElementListAST *members;
    int t_rparen;

public:
    TypedTableElementsAST( int t_lparen_, TypedTableElementListAST *members_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , members(finish(members_))
        , t_rparen(t_rparen_)
    {}

    virtual TypedTableElementsAST *asTypedTableElements() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypedTableElementAST: public AST
{
public:
    TypedTableElementAST() : AST() {}
    virtual TypedTableElementAST *asTypedTableElement() { return this; }
};

class TypedTableConstrElementAST: public TypedTableElementAST
{
public:
    TableConstraintAST *constraint;

public:
    TypedTableConstrElementAST( TableConstraintAST *constraint_ )
        : TypedTableElementAST()
        , constraint(constraint_)
    {}

    virtual TypedTableConstrElementAST *asTypedTableConstrElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypedTableColElementAST: public TypedTableElementAST
{
public:
    NameAST *columnName;
    int t_with;
    int t_options;
    ColumnConstraintListAST *constraints;

public:
    TypedTableColElementAST( NameAST *columnName_, int t_with_, int t_options_, ColumnConstraintListAST *constraints_ )
        : TypedTableElementAST()
        , columnName(columnName_)
        , t_with(t_with_)
        , t_options(t_options_)
        , constraints(finish(constraints_))
    {}

    virtual TypedTableColElementAST *asTypedTableColElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableInheritClauseAST: public AST
{
public:
    int t_inherits;
    int t_lparen;
    TableIdentifierListAST *tables;
    int t_rparen;

public:
    TableInheritClauseAST( int t_inherits_, int t_lparen_, TableIdentifierListAST *tables_, int t_rparen_ )
        : AST()
        , t_inherits(t_inherits_)
        , t_lparen(t_lparen_)
        , tables(finish(tables_))
        , t_rparen(t_rparen_)
    {}

    virtual TableInheritClauseAST *asTableInheritClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateSchemaStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_schema;
    int t_if;
    int t_not;
    int t_exists;
    CreateSchemaClauseAST *schemaClause;

public:
    CreateSchemaStmtAST( int t_create_, int t_schema_, int t_if_, int t_not_, int t_exists_, CreateSchemaClauseAST *schemaClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_schema(t_schema_)
        , t_if(t_if_)
        , t_not(t_not_)
        , t_exists(t_exists_)
        , schemaClause(schemaClause_)
    {}

    virtual CreateSchemaStmtAST *asCreateSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateSchemaClauseAST: public AST
{
public:
    NameAST *schemaName;
    int t_authorization;
    RoleIdAST *roleId;
    StatementListAST *schemaStatements;

public:
    CreateSchemaClauseAST( NameAST *schemaName_, int t_authorization_, RoleIdAST *roleId_, StatementListAST *schemaStatements_ )
        : AST()
        , schemaName(schemaName_)
        , t_authorization(t_authorization_)
        , roleId(roleId_)
        , schemaStatements(finish(schemaStatements_))
    {}

    virtual CreateSchemaClauseAST *asCreateSchemaClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropPLangStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_procedural;
    int t_language;
    int t_if;
    int t_exists;
    int t_lang_alias;
    int t_behavior;

public:
    DropPLangStmtAST( int t_drop_, int t_procedural_, int t_language_, int t_if_, int t_exists_, int t_lang_alias_, int t_behavior_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , t_lang_alias(t_lang_alias_)
        , t_behavior(t_behavior_)
    {}

    virtual DropPLangStmtAST *asDropPLangStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreatePLangStmtAST: public CreateStatementAST
{
public:
    int t_create;
    OrReplaceClauseAST *orReplaceClause;
    int t_trusted;
    int t_procedural;
    int t_language;
    int t_lang_alias;
    int t_handler;
    HandlerIdentifierAST *handlerIdent;
    InlineHandlerClauseAST *inlineClause;
    ValidatorHandlerClauseAST *validatorClause;

public:
    CreatePLangStmtAST( int t_create_, OrReplaceClauseAST *orReplaceClause_, int t_trusted_, int t_procedural_, int t_language_, int t_lang_alias_, int t_handler_, HandlerIdentifierAST *handlerIdent_, InlineHandlerClauseAST *inlineClause_, ValidatorHandlerClauseAST *validatorClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , orReplaceClause(orReplaceClause_)
        , t_trusted(t_trusted_)
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , t_lang_alias(t_lang_alias_)
        , t_handler(t_handler_)
        , handlerIdent(handlerIdent_)
        , inlineClause(inlineClause_)
        , validatorClause(validatorClause_)
    {}

    virtual CreatePLangStmtAST *asCreatePLangStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InlineHandlerClauseAST: public AST
{
public:
    int t_inline;
    HandlerIdentifierAST *handlerIdent;

public:
    InlineHandlerClauseAST( int t_inline_, HandlerIdentifierAST *handlerIdent_ )
        : AST()
        , t_inline(t_inline_)
        , handlerIdent(handlerIdent_)
    {}

    virtual InlineHandlerClauseAST *asInlineHandlerClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ValidatorHandlerClauseAST: public AST
{
public:
    int t_validator;
    HandlerIdentifierAST *handlerIdent;

public:
    ValidatorHandlerClauseAST( int t_validator_, HandlerIdentifierAST *handlerIdent_ )
        : AST()
        , t_validator(t_validator_)
        , handlerIdent(handlerIdent_)
    {}

    virtual ValidatorHandlerClauseAST *asValidatorHandlerClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropOpClassStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_operator;
    int t_class;
    int t_if;
    int t_exists;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;
    int t_ehavior;

public:
    DropOpClassStmtAST( int t_drop_, int t_operator_, int t_class_, int t_if_, int t_exists_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_, int t_ehavior_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_operator(t_operator_)
        , t_class(t_class_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , t_ehavior(t_ehavior_)
    {}

    virtual DropOpClassStmtAST *asDropOpClassStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropOpFamilyStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_operator;
    int t_family;
    int t_if;
    int t_exists;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;
    int t_ehavior;

public:
    DropOpFamilyStmtAST( int t_drop_, int t_operator_, int t_family_, int t_if_, int t_exists_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_, int t_ehavior_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_operator(t_operator_)
        , t_family(t_family_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , t_ehavior(t_ehavior_)
    {}

    virtual DropOpFamilyStmtAST *asDropOpFamilyStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOpFamilyAddStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_operator;
    int t_family;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;
    int t_add;
    OpClassListAST *opClasses;

public:
    AlterOpFamilyAddStmtAST( int t_alter_, int t_operator_, int t_family_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_, int t_add_, OpClassListAST *opClasses_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , t_family(t_family_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , t_add(t_add_)
        , opClasses(finish(opClasses_))
    {}

    virtual AlterOpFamilyAddStmtAST *asAlterOpFamilyAddStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOpFamilyDropStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_operator;
    int t_family;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;
    int t_drop;
    OpClassDropListAST *opClasses;

public:
    AlterOpFamilyDropStmtAST( int t_alter_, int t_operator_, int t_family_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_, int t_drop_, OpClassDropListAST *opClasses_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , t_family(t_family_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , t_drop(t_drop_)
        , opClasses(finish(opClasses_))
    {}

    virtual AlterOpFamilyDropStmtAST *asAlterOpFamilyDropStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpClassDropAST: public AST
{
public:
    int t_function_or_operator;
    int t_sp_value;
    int t_lparen;
    TypeListAST *types;
    int t_rparen;

public:
    OpClassDropAST( int t_function_or_operator_, int t_sp_value_, int t_lparen_, TypeListAST *types_, int t_rparen_ )
        : AST()
        , t_function_or_operator(t_function_or_operator_)
        , t_sp_value(t_sp_value_)
        , t_lparen(t_lparen_)
        , types(finish(types_))
        , t_rparen(t_rparen_)
    {}

    virtual OpClassDropAST *asOpClassDrop() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateOpFamilyStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_operator;
    int t_family;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;

public:
    CreateOpFamilyStmtAST( int t_create_, int t_operator_, int t_family_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_operator(t_operator_)
        , t_family(t_family_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
    {}

    virtual CreateOpFamilyStmtAST *asCreateOpFamilyStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateOpClassStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_operator;
    int t_class;
    AnyIdentifierAST *opIdent;
    int t_default;
    int t_for;
    int t_type;
    TypeAST *opType;
    int t_using;
    int t_acc_method;
    OpFamilyAST *opFamily;
    int t_as;
    OpClassListAST *opClass;

public:
    CreateOpClassStmtAST( int t_create_, int t_operator_, int t_class_, AnyIdentifierAST *opIdent_, int t_default_, int t_for_, int t_type_, TypeAST *opType_, int t_using_, int t_acc_method_, OpFamilyAST *opFamily_, int t_as_, OpClassListAST *opClass_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_operator(t_operator_)
        , t_class(t_class_)
        , opIdent(opIdent_)
        , t_default(t_default_)
        , t_for(t_for_)
        , t_type(t_type_)
        , opType(opType_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , opFamily(opFamily_)
        , t_as(t_as_)
        , opClass(finish(opClass_))
    {}

    virtual CreateOpClassStmtAST *asCreateOpClassStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpClassAST: public AST
{
public:
    OpClassAST() : AST() {}
    virtual OpClassAST *asOpClass() { return this; }
};

class StorageOpClassAST: public OpClassAST
{
public:
    int t_storage;
    TypeAST *storageType;

public:
    StorageOpClassAST( int t_storage_, TypeAST *storageType_ )
        : OpClassAST()
        , t_storage(t_storage_)
        , storageType(storageType_)
    {}

    virtual StorageOpClassAST *asStorageOpClass() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OperatorOpClassAST: public OpClassAST
{
public:
    int t_operator;
    int t_sp_value;
    OperandAST *operand;
    OperArgTypesAST *argTypes;
    OpClassPurposeAST *purpose;
    int t_recheck;

public:
    OperatorOpClassAST( int t_operator_, int t_sp_value_, OperandAST *operand_, OperArgTypesAST *argTypes_, OpClassPurposeAST *purpose_, int t_recheck_ )
        : OpClassAST()
        , t_operator(t_operator_)
        , t_sp_value(t_sp_value_)
        , operand(operand_)
        , argTypes(argTypes_)
        , purpose(purpose_)
        , t_recheck(t_recheck_)
    {}

    virtual OperatorOpClassAST *asOperatorOpClass() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpClassPurposeAST: public AST
{
public:
    OpClassPurposeAST() : AST() {}
    virtual OpClassPurposeAST *asOpClassPurpose() { return this; }
};

class OpClassSearchPurposeAST: public OpClassPurposeAST
{
public:
    int t_for;
    int t_search;

public:
    OpClassSearchPurposeAST( int t_for_, int t_search_ )
        : OpClassPurposeAST()
        , t_for(t_for_)
        , t_search(t_search_)
    {}

    virtual OpClassSearchPurposeAST *asOpClassSearchPurpose() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpClassOrderPurposeAST: public OpClassPurposeAST
{
public:
    int t_for;
    int t_order;
    int t_by;
    AnyIdentifierAST *orderIdent;

public:
    OpClassOrderPurposeAST( int t_for_, int t_order_, int t_by_, AnyIdentifierAST *orderIdent_ )
        : OpClassPurposeAST()
        , t_for(t_for_)
        , t_order(t_order_)
        , t_by(t_by_)
        , orderIdent(orderIdent_)
    {}

    virtual OpClassOrderPurposeAST *asOpClassOrderPurpose() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionOpClassAST: public OpClassAST
{
public:
    int t_function;
    int t_sp_value;
    int t_lparen;
    TypeListAST *types;
    int t_rparen;
    FunctionRefAST *functionRef;
    FunctionArgTypesAST *argTypes;

public:
    FunctionOpClassAST( int t_function_, int t_sp_value_, int t_lparen_, TypeListAST *types_, int t_rparen_, FunctionRefAST *functionRef_, FunctionArgTypesAST *argTypes_ )
        : OpClassAST()
        , t_function(t_function_)
        , t_sp_value(t_sp_value_)
        , t_lparen(t_lparen_)
        , types(finish(types_))
        , t_rparen(t_rparen_)
        , functionRef(functionRef_)
        , argTypes(argTypes_)
    {}

    virtual FunctionOpClassAST *asFunctionOpClass() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpFamilyAST: public AST
{
public:
    int t_family;
    AnyIdentifierAST *ident;

public:
    OpFamilyAST( int t_family_, AnyIdentifierAST *ident_ )
        : AST()
        , t_family(t_family_)
        , ident(ident_)
    {}

    virtual OpFamilyAST *asOpFamily() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateMatViewStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_unlogged;
    int t_materialized;
    int t_view;
    CreateMvTargetAST *target;
    int t_as;
    SelectStmtAST *query;
    WithDataClauseAST *withDataClause;

public:
    CreateMatViewStmtAST( int t_create_, int t_unlogged_, int t_materialized_, int t_view_, CreateMvTargetAST *target_, int t_as_, SelectStmtAST *query_, WithDataClauseAST *withDataClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_unlogged(t_unlogged_)
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , target(target_)
        , t_as(t_as_)
        , query(query_)
        , withDataClause(withDataClause_)
    {}

    virtual CreateMatViewStmtAST *asCreateMatViewStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateMvTargetAST: public AST
{
public:
    TableIdentifierAST *viewIdent;
    RefColumnsAST *columns;
    WithReloptionsAST *reloptions;
    TableSpaceClauseAST *tablespaceClause;

public:
    CreateMvTargetAST( TableIdentifierAST *viewIdent_, RefColumnsAST *columns_, WithReloptionsAST *reloptions_, TableSpaceClauseAST *tablespaceClause_ )
        : AST()
        , viewIdent(viewIdent_)
        , columns(columns_)
        , reloptions(reloptions_)
        , tablespaceClause(tablespaceClause_)
    {}

    virtual CreateMvTargetAST *asCreateMvTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithReloptionsAST: public AST
{
public:
    int t_with;
    RelOptionsAST *relOptions;

public:
    WithReloptionsAST( int t_with_, RelOptionsAST *relOptions_ )
        : AST()
        , t_with(t_with_)
        , relOptions(relOptions_)
    {}

    virtual WithReloptionsAST *asWithReloptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateFunctionStmtAST: public CreateStatementAST
{
public:
    int t_create;
    OrReplaceClauseAST *orReplace;
    int t_function;
    FunctionRefAST *functionRef;
    FuncArgsWithDefaultsAST *args;
    int t_returns;
    BaseFuncReturnAST *funcReturn;
    CreateFuncOptionListAST *options;
    ConstraintDefinitionsAST *defs;

public:
    CreateFunctionStmtAST( int t_create_, OrReplaceClauseAST *orReplace_, int t_function_, FunctionRefAST *functionRef_, FuncArgsWithDefaultsAST *args_, int t_returns_, BaseFuncReturnAST *funcReturn_, CreateFuncOptionListAST *options_, ConstraintDefinitionsAST *defs_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , orReplace(orReplace_)
        , t_function(t_function_)
        , functionRef(functionRef_)
        , args(args_)
        , t_returns(t_returns_)
        , funcReturn(funcReturn_)
        , options(finish(options_))
        , defs(defs_)
    {}

    virtual CreateFunctionStmtAST *asCreateFunctionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BaseFuncReturnAST: public AST
{
public:
    BaseFuncReturnAST() : AST() {}
    virtual BaseFuncReturnAST *asBaseFuncReturn() { return this; }
};

class FuncTypeFuncReturnAST: public BaseFuncReturnAST
{
public:
    FunctionTypeAST *functionType;

public:
    FuncTypeFuncReturnAST( FunctionTypeAST *functionType_ )
        : BaseFuncReturnAST()
        , functionType(functionType_)
    {}

    virtual FuncTypeFuncReturnAST *asFuncTypeFuncReturn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableFuncReturnAST: public BaseFuncReturnAST
{
public:
    int t_table;
    int t_lparen;
    TableFuncColumnListAST *columns;
    int t_rparen;

public:
    TableFuncReturnAST( int t_table_, int t_lparen_, TableFuncColumnListAST *columns_, int t_rparen_ )
        : BaseFuncReturnAST()
        , t_table(t_table_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
    {}

    virtual TableFuncReturnAST *asTableFuncReturn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableFuncColumnAST: public AST
{
public:
    NameAST *paramName;
    FunctionTypeAST *functionType;

public:
    TableFuncColumnAST( NameAST *paramName_, FunctionTypeAST *functionType_ )
        : AST()
        , paramName(paramName_)
        , functionType(functionType_)
    {}

    virtual TableFuncColumnAST *asTableFuncColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateFuncOptionAST: public AST
{
public:
    CreateFuncOptionAST() : AST() {}
    virtual CreateFuncOptionAST *asCreateFuncOption() { return this; }
};

class CreateFuncAsOptionAST: public CreateFuncOptionAST
{
public:
    int t_as;
    FunctionAsClauseAST *asClause;

public:
    CreateFuncAsOptionAST( int t_as_, FunctionAsClauseAST *asClause_ )
        : CreateFuncOptionAST()
        , t_as(t_as_)
        , asClause(asClause_)
    {}

    virtual CreateFuncAsOptionAST *asCreateFuncAsOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateFuncLangOptionAST: public CreateFuncOptionAST
{
public:
    int t_language;
    int t_lang_value;

public:
    CreateFuncLangOptionAST( int t_language_, int t_lang_value_ )
        : CreateFuncOptionAST()
        , t_language(t_language_)
        , t_lang_value(t_lang_value_)
    {}

    virtual CreateFuncLangOptionAST *asCreateFuncLangOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateFuncWindowOptionAST: public CreateFuncOptionAST
{
public:
    int t_window;

public:
    CreateFuncWindowOptionAST( int t_window_ )
        : CreateFuncOptionAST()
        , t_window(t_window_)
    {}

    virtual CreateFuncWindowOptionAST *asCreateFuncWindowOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateFuncCommonOptionAST: public CreateFuncOptionAST
{
public:
    CommonAlterFunctionOptAST *commonOpt;

public:
    CreateFuncCommonOptionAST( CommonAlterFunctionOptAST *commonOpt_ )
        : CreateFuncOptionAST()
        , commonOpt(commonOpt_)
    {}

    virtual CreateFuncCommonOptionAST *asCreateFuncCommonOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionAsClauseAST: public AST
{
public:
    int t_first_value;
    int t_comma;
    int t_second_value;

public:
    FunctionAsClauseAST( int t_first_value_, int t_comma_, int t_second_value_ )
        : AST()
        , t_first_value(t_first_value_)
        , t_comma(t_comma_)
        , t_second_value(t_second_value_)
    {}

    virtual FunctionAsClauseAST *asFunctionAsClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncArgsWithDefaultsAST: public AST
{
public:
    int t_lparen;
    FuncArgWithDefaultListAST *args;
    int t_rparen;

public:
    FuncArgsWithDefaultsAST( int t_lparen_, FuncArgWithDefaultListAST *args_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual FuncArgsWithDefaultsAST *asFuncArgsWithDefaults() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncArgWithDefaultAST: public AST
{
public:
    FunctionArgAST *arg;
    int t_default_or_equal;
    ExpressionAST *defExpr;

public:
    FuncArgWithDefaultAST( FunctionArgAST *arg_, int t_default_or_equal_, ExpressionAST *defExpr_ )
        : AST()
        , arg(arg_)
        , t_default_or_equal(t_default_or_equal_)
        , defExpr(defExpr_)
    {}

    virtual FuncArgWithDefaultAST *asFuncArgWithDefault() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OrReplaceClauseAST: public AST
{
public:
    int t_or;
    int t_replace;

public:
    OrReplaceClauseAST( int t_or_, int t_replace_ )
        : AST()
        , t_or(t_or_)
        , t_replace(t_replace_)
    {}

    virtual OrReplaceClauseAST *asOrReplaceClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateForeignTableStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_foreign;
    int t_table;
    int t_if;
    int t_not;
    int t_exists;
    TableIdentifierAST *tableIdent;
    int t_lparen;
    TableElementListAST *columns;
    int t_rparen;
    int t_server;
    NameAST *serverName;
    CreateGeneticOptionsAST *genericOptions;

public:
    CreateForeignTableStmtAST( int t_create_, int t_foreign_, int t_table_, int t_if_, int t_not_, int t_exists_, TableIdentifierAST *tableIdent_, int t_lparen_, TableElementListAST *columns_, int t_rparen_, int t_server_, NameAST *serverName_, CreateGeneticOptionsAST *genericOptions_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_not(t_not_)
        , t_exists(t_exists_)
        , tableIdent(tableIdent_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , t_server(t_server_)
        , serverName(serverName_)
        , genericOptions(genericOptions_)
    {}

    virtual CreateForeignTableStmtAST *asCreateForeignTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableElementAST: public AST
{
public:
    TableElementAST() : AST() {}
    virtual TableElementAST *asTableElement() { return this; }
};

class ColumnDefTableElementAST: public TableElementAST
{
public:
    ColumnDefAST *columnDef;

public:
    ColumnDefTableElementAST( ColumnDefAST *columnDef_ )
        : TableElementAST()
        , columnDef(columnDef_)
    {}

    virtual ColumnDefTableElementAST *asColumnDefTableElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LikeTableElementAST: public TableElementAST
{
public:
    TableLikeClauseAST *tableLikeClause;

public:
    LikeTableElementAST( TableLikeClauseAST *tableLikeClause_ )
        : TableElementAST()
        , tableLikeClause(tableLikeClause_)
    {}

    virtual LikeTableElementAST *asLikeTableElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableLikeClauseAST: public AST
{
public:
    int t_like;
    TableIdentifierAST *tableIdent;
    TableLikeOptionListAST *options;

public:
    TableLikeClauseAST( int t_like_, TableIdentifierAST *tableIdent_, TableLikeOptionListAST *options_ )
        : AST()
        , t_like(t_like_)
        , tableIdent(tableIdent_)
        , options(finish(options_))
    {}

    virtual TableLikeClauseAST *asTableLikeClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableLikeOptionAST: public AST
{
public:
    int t_sp_kwd;

public:
    TableLikeOptionAST( int t_sp_kwd_ )
        : AST()
        , t_sp_kwd(t_sp_kwd_)
    {}

    virtual TableLikeOptionAST *asTableLikeOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintTableElementAST: public TableElementAST
{
public:
    TableConstraintAST *constraint;

public:
    ConstraintTableElementAST( TableConstraintAST *constraint_ )
        : TableElementAST()
        , constraint(constraint_)
    {}

    virtual ConstraintTableElementAST *asConstraintTableElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateForeignServerStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_server;
    NameAST *serverName;
    ServerTypeClauseAST *serverType;
    ForeignServerVersionAST *serverVersion;
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameAST *wrapperName;
    CreateGeneticOptionsAST *genericOptions;

public:
    CreateForeignServerStmtAST( int t_create_, int t_server_, NameAST *serverName_, ServerTypeClauseAST *serverType_, ForeignServerVersionAST *serverVersion_, int t_foreign_, int t_data_, int t_wrapper_, NameAST *wrapperName_, CreateGeneticOptionsAST *genericOptions_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_server(t_server_)
        , serverName(serverName_)
        , serverType(serverType_)
        , serverVersion(serverVersion_)
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , wrapperName(wrapperName_)
        , genericOptions(genericOptions_)
    {}

    virtual CreateForeignServerStmtAST *asCreateForeignServerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ServerTypeClauseAST: public AST
{
public:
    int t_type;
    int t_type_value;

public:
    ServerTypeClauseAST( int t_type_, int t_type_value_ )
        : AST()
        , t_type(t_type_)
        , t_type_value(t_type_value_)
    {}

    virtual ServerTypeClauseAST *asServerTypeClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateFdwStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameAST *wrapperName;
    FdwOptionListAST *options;
    CreateGeneticOptionsAST *genericOptions;

public:
    CreateFdwStmtAST( int t_create_, int t_foreign_, int t_data_, int t_wrapper_, NameAST *wrapperName_, FdwOptionListAST *options_, CreateGeneticOptionsAST *genericOptions_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , wrapperName(wrapperName_)
        , options(finish(options_))
        , genericOptions(genericOptions_)
    {}

    virtual CreateFdwStmtAST *asCreateFdwStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateExtensionStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_extension;
    int t_if;
    int t_not;
    int t_exists;
    NameAST *extensionName;
    int t_with;
    CreateExtOptionListAST *options;

public:
    CreateExtensionStmtAST( int t_create_, int t_extension_, int t_if_, int t_not_, int t_exists_, NameAST *extensionName_, int t_with_, CreateExtOptionListAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_extension(t_extension_)
        , t_if(t_if_)
        , t_not(t_not_)
        , t_exists(t_exists_)
        , extensionName(extensionName_)
        , t_with(t_with_)
        , options(finish(options_))
    {}

    virtual CreateExtensionStmtAST *asCreateExtensionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateExtOptionAST: public AST
{
public:
    CreateExtOptionAST() : AST() {}
    virtual CreateExtOptionAST *asCreateExtOption() { return this; }
};

class CreateExtSchemaOptionAST: public CreateExtOptionAST
{
public:
    int t_schema;
    NameAST *schemaName;

public:
    CreateExtSchemaOptionAST( int t_schema_, NameAST *schemaName_ )
        : CreateExtOptionAST()
        , t_schema(t_schema_)
        , schemaName(schemaName_)
    {}

    virtual CreateExtSchemaOptionAST *asCreateExtSchemaOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateExtVerFromOptionAST: public CreateExtOptionAST
{
public:
    int t_version_or_from;
    int t_sp_value;

public:
    CreateExtVerFromOptionAST( int t_version_or_from_, int t_sp_value_ )
        : CreateExtOptionAST()
        , t_version_or_from(t_version_or_from_)
        , t_sp_value(t_sp_value_)
    {}

    virtual CreateExtVerFromOptionAST *asCreateExtVerFromOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateConversionStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_default;
    int t_conversion;
    AnyIdentifierAST *ident;
    int t_for;
    int t_for_value;
    int t_to;
    int t_to_value;
    int t_from;
    AnyIdentifierAST *fromIdent;

public:
    CreateConversionStmtAST( int t_create_, int t_default_, int t_conversion_, AnyIdentifierAST *ident_, int t_for_, int t_for_value_, int t_to_, int t_to_value_, int t_from_, AnyIdentifierAST *fromIdent_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_default(t_default_)
        , t_conversion(t_conversion_)
        , ident(ident_)
        , t_for(t_for_)
        , t_for_value(t_for_value_)
        , t_to(t_to_)
        , t_to_value(t_to_value_)
        , t_from(t_from_)
        , fromIdent(fromIdent_)
    {}

    virtual CreateConversionStmtAST *asCreateConversionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateCastStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_cast;
    int t_lparen;
    TypeAST *fromType;
    int t_as;
    TypeAST *toType;
    int t_rparen;
    int t_with_or_without;
    int t_function_or_inout;
    FunctionWithArgTypesAST *functionWithArgTypes;
    CastContextAST *context;

public:
    CreateCastStmtAST( int t_create_, int t_cast_, int t_lparen_, TypeAST *fromType_, int t_as_, TypeAST *toType_, int t_rparen_, int t_with_or_without_, int t_function_or_inout_, FunctionWithArgTypesAST *functionWithArgTypes_, CastContextAST *context_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_cast(t_cast_)
        , t_lparen(t_lparen_)
        , fromType(fromType_)
        , t_as(t_as_)
        , toType(toType_)
        , t_rparen(t_rparen_)
        , t_with_or_without(t_with_or_without_)
        , t_function_or_inout(t_function_or_inout_)
        , functionWithArgTypes(functionWithArgTypes_)
        , context(context_)
    {}

    virtual CreateCastStmtAST *asCreateCastStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CastContextAST: public AST
{
public:
    int t_as;
    int t_implicit_or_assignment;

public:
    CastContextAST( int t_as_, int t_implicit_or_assignment_ )
        : AST()
        , t_as(t_as_)
        , t_implicit_or_assignment(t_implicit_or_assignment_)
    {}

    virtual CastContextAST *asCastContext() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropCastStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_cast;
    IfExistsAST *ifExists;
    int t_lparen;
    TypeAST *fromType;
    int t_as;
    TypeAST *toType;
    int t_rparen;
    int t_behavior;

public:
    DropCastStmtAST( int t_drop_, int t_cast_, IfExistsAST *ifExists_, int t_lparen_, TypeAST *fromType_, int t_as_, TypeAST *toType_, int t_rparen_, int t_behavior_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_cast(t_cast_)
        , ifExists(ifExists_)
        , t_lparen(t_lparen_)
        , fromType(fromType_)
        , t_as(t_as_)
        , toType(toType_)
        , t_rparen(t_rparen_)
        , t_behavior(t_behavior_)
    {}

    virtual DropCastStmtAST *asDropCastStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IfExistsAST: public AST
{
public:
    int t_if;
    int t_exists;

public:
    IfExistsAST( int t_if_, int t_exists_ )
        : AST()
        , t_if(t_if_)
        , t_exists(t_exists_)
    {}

    virtual IfExistsAST *asIfExists() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropAssertStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_assertion;
    NameAST *assertName;
    int t_behavior;

public:
    DropAssertStmtAST( int t_drop_, int t_assertion_, NameAST *assertName_, int t_behavior_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_assertion(t_assertion_)
        , assertName(assertName_)
        , t_behavior(t_behavior_)
    {}

    virtual DropAssertStmtAST *asDropAssertStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateAssertStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_assertion;
    NameAST *assertName;
    int t_check;
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;
    ConstraintAttributeListAST *constrAttrs;

public:
    CreateAssertStmtAST( int t_create_, int t_assertion_, NameAST *assertName_, int t_check_, int t_lparen_, ExpressionAST *expr_, int t_rparen_, ConstraintAttributeListAST *constrAttrs_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_assertion(t_assertion_)
        , assertName(assertName_)
        , t_check(t_check_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
        , constrAttrs(finish(constrAttrs_))
    {}

    virtual CreateAssertStmtAST *asCreateAssertStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableAsStmtAST: public CreateStatementAST
{
public:
    int t_create;
    TempClauseAST *tempClause;
    int t_table;
    CreateAsTargetAST *createTarget;
    int t_as;
    SelectStmtAST *query;
    WithDataClauseAST *withDataClause;

public:
    CreateTableAsStmtAST( int t_create_, TempClauseAST *tempClause_, int t_table_, CreateAsTargetAST *createTarget_, int t_as_, SelectStmtAST *query_, WithDataClauseAST *withDataClause_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , tempClause(tempClause_)
        , t_table(t_table_)
        , createTarget(createTarget_)
        , t_as(t_as_)
        , query(query_)
        , withDataClause(withDataClause_)
    {}

    virtual CreateTableAsStmtAST *asCreateTableAsStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateAsTargetAST: public AST
{
public:
    TableIdentifierAST *tableIdent;
    RefColumnsAST *columns;
    CreateWithOptionsAST *createOpts;
    OnCommitOptionAST *onCommitOption;
    TableSpaceClauseAST *tablespceOpt;

public:
    CreateAsTargetAST( TableIdentifierAST *tableIdent_, RefColumnsAST *columns_, CreateWithOptionsAST *createOpts_, OnCommitOptionAST *onCommitOption_, TableSpaceClauseAST *tablespceOpt_ )
        : AST()
        , tableIdent(tableIdent_)
        , columns(columns_)
        , createOpts(createOpts_)
        , onCommitOption(onCommitOption_)
        , tablespceOpt(tablespceOpt_)
    {}

    virtual CreateAsTargetAST *asCreateAsTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableSpaceClauseAST: public AST
{
public:
    int t_tablespace;
    NameAST *tablespaceName;

public:
    TableSpaceClauseAST( int t_tablespace_, NameAST *tablespaceName_ )
        : AST()
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
    {}

    virtual TableSpaceClauseAST *asTableSpaceClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OnCommitOptionAST: public AST
{
public:
    int t_on;
    int t_commit;
    int t_sp_act;
    int t_rows;

public:
    OnCommitOptionAST( int t_on_, int t_commit_, int t_sp_act_, int t_rows_ )
        : AST()
        , t_on(t_on_)
        , t_commit(t_commit_)
        , t_sp_act(t_sp_act_)
        , t_rows(t_rows_)
    {}

    virtual OnCommitOptionAST *asOnCommitOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateWithOptionsAST: public AST
{
public:
    CreateWithOptionsAST() : AST() {}
    virtual CreateWithOptionsAST *asCreateWithOptions() { return this; }
};

class CreateWithRelOptionsAST: public CreateWithOptionsAST
{
public:
    int t_with;
    RelOptionsAST *opts;

public:
    CreateWithRelOptionsAST( int t_with_, RelOptionsAST *opts_ )
        : CreateWithOptionsAST()
        , t_with(t_with_)
        , opts(opts_)
    {}

    virtual CreateWithRelOptionsAST *asCreateWithRelOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateWithOidsOptionsAST: public CreateWithOptionsAST
{
public:
    int t_with_or_without;
    int t_oids;

public:
    CreateWithOidsOptionsAST( int t_with_or_without_, int t_oids_ )
        : CreateWithOptionsAST()
        , t_with_or_without(t_with_or_without_)
        , t_oids(t_oids_)
    {}

    virtual CreateWithOidsOptionsAST *asCreateWithOidsOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithDataClauseAST: public AST
{
public:
    int t_with;
    int t_no;
    int t_data;

public:
    WithDataClauseAST( int t_with_, int t_no_, int t_data_ )
        : AST()
        , t_with(t_with_)
        , t_no(t_no_)
        , t_data(t_data_)
    {}

    virtual WithDataClauseAST *asWithDataClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CopyStmtAST: public StatementAST
{
public:
    CopyStmtAST() : StatementAST() {}
    virtual CopyStmtAST *asCopyStmt() { return this; }
};

class TableCopyStmtAST: public CopyStmtAST
{
public:
    int t_copy;
    int t_binary;
    TableIdentifierAST *tableIdent;
    RefColumnsAST *columns;
    WithOidsClauseAST *oids;
    int t_from_or_to;
    int t_program;
    int t_file_name;
    CopyDelimiterAST *delimiter;
    int t_with;
    CopyOptionsAST *options;

public:
    TableCopyStmtAST( int t_copy_, int t_binary_, TableIdentifierAST *tableIdent_, RefColumnsAST *columns_, WithOidsClauseAST *oids_, int t_from_or_to_, int t_program_, int t_file_name_, CopyDelimiterAST *delimiter_, int t_with_, CopyOptionsAST *options_ )
        : CopyStmtAST()
        , t_copy(t_copy_)
        , t_binary(t_binary_)
        , tableIdent(tableIdent_)
        , columns(columns_)
        , oids(oids_)
        , t_from_or_to(t_from_or_to_)
        , t_program(t_program_)
        , t_file_name(t_file_name_)
        , delimiter(delimiter_)
        , t_with(t_with_)
        , options(options_)
    {}

    virtual TableCopyStmtAST *asTableCopyStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithOidsClauseAST: public AST
{
public:
    int t_with;
    int t_oids;

public:
    WithOidsClauseAST( int t_with_, int t_oids_ )
        : AST()
        , t_with(t_with_)
        , t_oids(t_oids_)
    {}

    virtual WithOidsClauseAST *asWithOidsClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CopyDelimiterAST: public AST
{
public:
    int t_using;
    int t_delimiters;
    int t_delim_value;

public:
    CopyDelimiterAST( int t_using_, int t_delimiters_, int t_delim_value_ )
        : AST()
        , t_using(t_using_)
        , t_delimiters(t_delimiters_)
        , t_delim_value(t_delim_value_)
    {}

    virtual CopyDelimiterAST *asCopyDelimiter() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCopyStmtAST: public CopyStmtAST
{
public:
    int t_copy;
    SelectWithParensAST *query;
    int t_to;
    int t_program;
    int t_copy_file_name;
    int t_with;
    CopyOptionsAST *options;

public:
    SelectCopyStmtAST( int t_copy_, SelectWithParensAST *query_, int t_to_, int t_program_, int t_copy_file_name_, int t_with_, CopyOptionsAST *options_ )
        : CopyStmtAST()
        , t_copy(t_copy_)
        , query(query_)
        , t_to(t_to_)
        , t_program(t_program_)
        , t_copy_file_name(t_copy_file_name_)
        , t_with(t_with_)
        , options(options_)
    {}

    virtual SelectCopyStmtAST *asSelectCopyStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CopyOptionsAST: public AST
{
public:
    CopyOptionsAST() : AST() {}
    virtual CopyOptionsAST *asCopyOptions() { return this; }
};

class NewCopyOptionsAST: public CopyOptionsAST
{
public:
    int t_lparen;
    GenericCopyOptionListAST *opts;
    int t_rparen;

public:
    NewCopyOptionsAST( int t_lparen_, GenericCopyOptionListAST *opts_, int t_rparen_ )
        : CopyOptionsAST()
        , t_lparen(t_lparen_)
        , opts(finish(opts_))
        , t_rparen(t_rparen_)
    {}

    virtual NewCopyOptionsAST *asNewCopyOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GenericCopyOptionAST: public AST
{
public:
    int t_label;
    GenericCopyOptionArgAST *arg;

public:
    GenericCopyOptionAST( int t_label_, GenericCopyOptionArgAST *arg_ )
        : AST()
        , t_label(t_label_)
        , arg(arg_)
    {}

    virtual GenericCopyOptionAST *asGenericCopyOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GenericCopyOptionArgAST: public AST
{
public:
    GenericCopyOptionArgAST() : AST() {}
    virtual GenericCopyOptionArgAST *asGenericCopyOptionArg() { return this; }
};

class GenericCopyOptionValArgAST: public GenericCopyOptionArgAST
{
public:
    int t_star_or_value;

public:
    GenericCopyOptionValArgAST( int t_star_or_value_ )
        : GenericCopyOptionArgAST()
        , t_star_or_value(t_star_or_value_)
    {}

    virtual GenericCopyOptionValArgAST *asGenericCopyOptionValArg() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GenericCopyOptionCmpArgAST: public GenericCopyOptionArgAST
{
public:
    int t_lparen;
    GenericCopyOptionCmpArgItemListAST *items;
    int t_rparen;

public:
    GenericCopyOptionCmpArgAST( int t_lparen_, GenericCopyOptionCmpArgItemListAST *items_, int t_rparen_ )
        : GenericCopyOptionArgAST()
        , t_lparen(t_lparen_)
        , items(finish(items_))
        , t_rparen(t_rparen_)
    {}

    virtual GenericCopyOptionCmpArgAST *asGenericCopyOptionCmpArg() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GenericCopyOptionCmpArgItemAST: public AST
{
public:
    int t_bool_or_str_value;

public:
    GenericCopyOptionCmpArgItemAST( int t_bool_or_str_value_ )
        : AST()
        , t_bool_or_str_value(t_bool_or_str_value_)
    {}

    virtual GenericCopyOptionCmpArgItemAST *asGenericCopyOptionCmpArgItem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OldCopyOptionsAST: public CopyOptionsAST
{
public:
    CopyOptionListAST *opts;

public:
    OldCopyOptionsAST( CopyOptionListAST *opts_ )
        : CopyOptionsAST()
        , opts(finish(opts_))
    {}

    virtual OldCopyOptionsAST *asOldCopyOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CopyOptionAST: public AST
{
public:
    int t_sp1;
    int t_sp2;
    int t_sp3;
    NameListAST *columns;

public:
    CopyOptionAST( int t_sp1_, int t_sp2_, int t_sp3_, NameListAST *columns_ )
        : AST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
        , t_sp3(t_sp3_)
        , columns(finish(columns_))
    {}

    virtual CopyOptionAST *asCopyOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintsSetStmtAST: public DdlStatementAST
{
public:
    int t_set;
    int t_constraints;
    ConstraintsSetListAST *constrList;
    int t_set_mode;

public:
    ConstraintsSetStmtAST( int t_set_, int t_constraints_, ConstraintsSetListAST *constrList_, int t_set_mode_ )
        : DdlStatementAST()
        , t_set(t_set_)
        , t_constraints(t_constraints_)
        , constrList(constrList_)
        , t_set_mode(t_set_mode_)
    {}

    virtual ConstraintsSetStmtAST *asConstraintsSetStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintsSetListAST: public AST
{
public:
    ConstraintsSetListAST() : AST() {}
    virtual ConstraintsSetListAST *asConstraintsSetList() { return this; }
};

class AllConstraintsSetListAST: public ConstraintsSetListAST
{
public:
    int t_all;

public:
    AllConstraintsSetListAST( int t_all_ )
        : ConstraintsSetListAST()
        , t_all(t_all_)
    {}

    virtual AllConstraintsSetListAST *asAllConstraintsSetList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonConstraintsSetListAST: public ConstraintsSetListAST
{
public:
    ConstraintIdentifierListAST *idents;

public:
    CommonConstraintsSetListAST( ConstraintIdentifierListAST *idents_ )
        : ConstraintsSetListAST()
        , idents(finish(idents_))
    {}

    virtual CommonConstraintsSetListAST *asCommonConstraintsSetList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentStmtAST: public StatementAST
{
public:
    CommentStmtAST() : StatementAST() {}
    virtual CommentStmtAST *asCommentStmt() { return this; }
};

class LangCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_procedural;
    int t_language;
    AnyIdentifierAST *langIdent;
    CommentTextClauseAST *commentText;

public:
    LangCommentStmtAST( int t_comment_, int t_on_, int t_procedural_, int t_language_, AnyIdentifierAST *langIdent_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , langIdent(langIdent_)
        , commentText(commentText_)
    {}

    virtual LangCommentStmtAST *asLangCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CastCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_cast;
    int t_lparen;
    TypeAST *fromType;
    int t_as;
    TypeAST *toType;
    int t_rparen;
    CommentTextClauseAST *commentText;

public:
    CastCommentStmtAST( int t_comment_, int t_on_, int t_cast_, int t_lparen_, TypeAST *fromType_, int t_as_, TypeAST *toType_, int t_rparen_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_cast(t_cast_)
        , t_lparen(t_lparen_)
        , fromType(fromType_)
        , t_as(t_as_)
        , toType(toType_)
        , t_rparen(t_rparen_)
        , commentText(commentText_)
    {}

    virtual CastCommentStmtAST *asCastCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LargeObjCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_large;
    int t_oject;
    int t_any_digit;
    CommentTextClauseAST *commentText;

public:
    LargeObjCommentStmtAST( int t_comment_, int t_on_, int t_large_, int t_oject_, int t_any_digit_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_large(t_large_)
        , t_oject(t_oject_)
        , t_any_digit(t_any_digit_)
        , commentText(commentText_)
    {}

    virtual LargeObjCommentStmtAST *asLargeObjCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpUsingCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_operator;
    int t_class_or_family;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;
    CommentTextClauseAST *commentText;

public:
    OpUsingCommentStmtAST( int t_comment_, int t_on_, int t_operator_, int t_class_or_family_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_operator(t_operator_)
        , t_class_or_family(t_class_or_family_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , commentText(commentText_)
    {}

    virtual OpUsingCommentStmtAST *asOpUsingCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_constraint;
    NameAST *constraintName;
    CommentOnNameAST *onName;
    CommentTextClauseAST *commentText;

public:
    ConstraintCommentStmtAST( int t_comment_, int t_on_, int t_constraint_, NameAST *constraintName_, CommentOnNameAST *onName_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_constraint(t_constraint_)
        , constraintName(constraintName_)
        , onName(onName_)
        , commentText(commentText_)
    {}

    virtual ConstraintCommentStmtAST *asConstraintCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RuleCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_rule;
    NameAST *ruleName;
    CommentOnNameAST *onName;
    CommentTextClauseAST *commentText;

public:
    RuleCommentStmtAST( int t_comment_, int t_on_, int t_rule_, NameAST *ruleName_, CommentOnNameAST *onName_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_rule(t_rule_)
        , ruleName(ruleName_)
        , onName(onName_)
        , commentText(commentText_)
    {}

    virtual RuleCommentStmtAST *asRuleCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_trigger;
    NameAST *triggerName;
    CommentOnNameAST *onName;
    CommentTextClauseAST *commentText;

public:
    TriggerCommentStmtAST( int t_comment_, int t_on_, int t_trigger_, NameAST *triggerName_, CommentOnNameAST *onName_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , onName(onName_)
        , commentText(commentText_)
    {}

    virtual TriggerCommentStmtAST *asTriggerCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentOnNameAST: public AST
{
public:
    int t_on;
    TableIdentifierAST *tableIdent;

public:
    CommentOnNameAST( int t_on_, TableIdentifierAST *tableIdent_ )
        : AST()
        , t_on(t_on_)
        , tableIdent(tableIdent_)
    {}

    virtual CommentOnNameAST *asCommentOnName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_operator;
    OperandAST *operand;
    OperArgTypesAST *argTypes;
    CommentTextClauseAST *commentText;

public:
    OpCommentStmtAST( int t_comment_, int t_on_, int t_operator_, OperandAST *operand_, OperArgTypesAST *argTypes_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_operator(t_operator_)
        , operand(operand_)
        , argTypes(argTypes_)
        , commentText(commentText_)
    {}

    virtual OpCommentStmtAST *asOpCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_function;
    FunctionRefAST *funcRef;
    FunctionArgTypesAST *argTypes;
    CommentTextClauseAST *commentText;

public:
    FunctionCommentStmtAST( int t_comment_, int t_on_, int t_function_, FunctionRefAST *funcRef_, FunctionArgTypesAST *argTypes_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_function(t_function_)
        , funcRef(funcRef_)
        , argTypes(argTypes_)
        , commentText(commentText_)
    {}

    virtual FunctionCommentStmtAST *asFunctionCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AggrCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    int t_aggregate;
    FunctionRefAST *funcRef;
    AggrArgumentsAST *args;
    CommentTextClauseAST *commentText;

public:
    AggrCommentStmtAST( int t_comment_, int t_on_, int t_aggregate_, FunctionRefAST *funcRef_, AggrArgumentsAST *args_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , t_aggregate(t_aggregate_)
        , funcRef(funcRef_)
        , args(args_)
        , commentText(commentText_)
    {}

    virtual AggrCommentStmtAST *asAggrCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonCommentStmtAST: public CommentStmtAST
{
public:
    int t_comment;
    int t_on;
    CommentObjectAST *obj;
    CommentTextClauseAST *commentText;

public:
    CommonCommentStmtAST( int t_comment_, int t_on_, CommentObjectAST *obj_, CommentTextClauseAST *commentText_ )
        : CommentStmtAST()
        , t_comment(t_comment_)
        , t_on(t_on_)
        , obj(obj_)
        , commentText(commentText_)
    {}

    virtual CommonCommentStmtAST *asCommonCommentStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentObjectAST: public AST
{
public:
    CommentObjectAST() : AST() {}
    virtual CommentObjectAST *asCommentObject() { return this; }
};

class SearchCommentObjectAST: public CommentObjectAST
{
public:
    int t_text;
    int t_search;
    int t_sp_what;
    AnyIdentifierAST *ident;

public:
    SearchCommentObjectAST( int t_text_, int t_search_, int t_sp_what_, AnyIdentifierAST *ident_ )
        : CommentObjectAST()
        , t_text(t_text_)
        , t_search(t_search_)
        , t_sp_what(t_sp_what_)
        , ident(ident_)
    {}

    virtual SearchCommentObjectAST *asSearchCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EvtTriggerCommentObjectAST: public CommentObjectAST
{
public:
    int t_event;
    int t_trigger;
    TriggerIdentifierAST *triggerIdent;

public:
    EvtTriggerCommentObjectAST( int t_event_, int t_trigger_, TriggerIdentifierAST *triggerIdent_ )
        : CommentObjectAST()
        , t_event(t_event_)
        , t_trigger(t_trigger_)
        , triggerIdent(triggerIdent_)
    {}

    virtual EvtTriggerCommentObjectAST *asEvtTriggerCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FdwCommentObjectAST: public CommentObjectAST
{
public:
    int t_foreign;
    int t_data;
    int t_wrapper;
    AnyIdentifierAST *fdwIdent;

public:
    FdwCommentObjectAST( int t_foreign_, int t_data_, int t_wrapper_, AnyIdentifierAST *fdwIdent_ )
        : CommentObjectAST()
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , fdwIdent(fdwIdent_)
    {}

    virtual FdwCommentObjectAST *asFdwCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoleCommentObjectAST: public CommentObjectAST
{
public:
    int t_role;
    RoleIdAST *roleId;

public:
    RoleCommentObjectAST( int t_role_, RoleIdAST *roleId_ )
        : CommentObjectAST()
        , t_role(t_role_)
        , roleId(roleId_)
    {}

    virtual RoleCommentObjectAST *asRoleCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableCommentObjectAST: public CommentObjectAST
{
public:
    int t_foreign_or_materialized;
    int t_table_or_view;
    TableIdentifierAST *ident;

public:
    TableCommentObjectAST( int t_foreign_or_materialized_, int t_table_or_view_, TableIdentifierAST *ident_ )
        : CommentObjectAST()
        , t_foreign_or_materialized(t_foreign_or_materialized_)
        , t_table_or_view(t_table_or_view_)
        , ident(ident_)
    {}

    virtual TableCommentObjectAST *asTableCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SequenceCommentObjectAST: public CommentObjectAST
{
public:
    int t_sequence;
    SequenceIdentifierAST *sequenceIdent;

public:
    SequenceCommentObjectAST( int t_sequence_, SequenceIdentifierAST *sequenceIdent_ )
        : CommentObjectAST()
        , t_sequence(t_sequence_)
        , sequenceIdent(sequenceIdent_)
    {}

    virtual SequenceCommentObjectAST *asSequenceCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndexCommentObjectAST: public CommentObjectAST
{
public:
    int t_index;
    IndexIdentifierAST *indexIdent;

public:
    IndexCommentObjectAST( int t_index_, IndexIdentifierAST *indexIdent_ )
        : CommentObjectAST()
        , t_index(t_index_)
        , indexIdent(indexIdent_)
    {}

    virtual IndexCommentObjectAST *asIndexCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SchemaCommentObjectAST: public CommentObjectAST
{
public:
    int t_schema;
    NameAST *schemaName;

public:
    SchemaCommentObjectAST( int t_schema_, NameAST *schemaName_ )
        : CommentObjectAST()
        , t_schema(t_schema_)
        , schemaName(schemaName_)
    {}

    virtual SchemaCommentObjectAST *asSchemaCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnCommentObjectAST: public CommentObjectAST
{
public:
    int t_columnm;
    ColumnIdentifierAST *columnIdent;

public:
    ColumnCommentObjectAST( int t_columnm_, ColumnIdentifierAST *columnIdent_ )
        : CommentObjectAST()
        , t_columnm(t_columnm_)
        , columnIdent(columnIdent_)
    {}

    virtual ColumnCommentObjectAST *asColumnCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NamedCommentObjectAST: public CommentObjectAST
{
public:
    int t_sp_object;
    NameAST *objName;

public:
    NamedCommentObjectAST( int t_sp_object_, NameAST *objName_ )
        : CommentObjectAST()
        , t_sp_object(t_sp_object_)
        , objName(objName_)
    {}

    virtual NamedCommentObjectAST *asNamedCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AnyCommentObjectAST: public CommentObjectAST
{
public:
    int t_sp_object;
    AnyIdentifierAST *ident;

public:
    AnyCommentObjectAST( int t_sp_object_, AnyIdentifierAST *ident_ )
        : CommentObjectAST()
        , t_sp_object(t_sp_object_)
        , ident(ident_)
    {}

    virtual AnyCommentObjectAST *asAnyCommentObject() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentTextClauseAST: public AST
{
public:
    int t_is;
    int t_comment_text;

public:
    CommentTextClauseAST( int t_is_, int t_comment_text_ )
        : AST()
        , t_is(t_is_)
        , t_comment_text(t_comment_text_)
    {}

    virtual CommentTextClauseAST *asCommentTextClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ClusterStmtAST: public DdlStatementAST
{
public:
    ClusterStmtAST() : DdlStatementAST() {}
    virtual ClusterStmtAST *asClusterStmt() { return this; }
};

class ClusterOnStmtAST: public ClusterStmtAST
{
public:
    int t_cluster;
    int t_verbose;
    NameAST *indexName;
    int t_on;
    TableIdentifierAST *tableIdent;

public:
    ClusterOnStmtAST( int t_cluster_, int t_verbose_, NameAST *indexName_, int t_on_, TableIdentifierAST *tableIdent_ )
        : ClusterStmtAST()
        , t_cluster(t_cluster_)
        , t_verbose(t_verbose_)
        , indexName(indexName_)
        , t_on(t_on_)
        , tableIdent(tableIdent_)
    {}

    virtual ClusterOnStmtAST *asClusterOnStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ClusterUsingStmtAST: public ClusterStmtAST
{
public:
    int t_cluster;
    int t_verbose;
    TableIdentifierAST *tableIdent;
    UsingIndexClauseAST *usingClause;

public:
    ClusterUsingStmtAST( int t_cluster_, int t_verbose_, TableIdentifierAST *tableIdent_, UsingIndexClauseAST *usingClause_ )
        : ClusterStmtAST()
        , t_cluster(t_cluster_)
        , t_verbose(t_verbose_)
        , tableIdent(tableIdent_)
        , usingClause(usingClause_)
    {}

    virtual ClusterUsingStmtAST *asClusterUsingStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingIndexClauseAST: public AST
{
public:
    int t_using;
    NameAST *indexName;

public:
    UsingIndexClauseAST( int t_using_, NameAST *indexName_ )
        : AST()
        , t_using(t_using_)
        , indexName(indexName_)
    {}

    virtual UsingIndexClauseAST *asUsingIndexClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ClosePortalStmtAST: public AST
{
public:
    ClosePortalStmtAST() : AST() {}
    virtual ClosePortalStmtAST *asClosePortalStmt() { return this; }
};

class CloseCursorStmtAST: public AST
{
public:
    int t_close;
    NameAST *cursorName;

public:
    CloseCursorStmtAST( int t_close_, NameAST *cursorName_ )
        : AST()
        , t_close(t_close_)
        , cursorName(cursorName_)
    {}

    virtual CloseCursorStmtAST *asCloseCursorStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CloseAllStmtAST: public ClosePortalStmtAST
{
public:
    int t_close;
    int t_all;

public:
    CloseAllStmtAST( int t_close_, int t_all_ )
        : ClosePortalStmtAST()
        , t_close(t_close_)
        , t_all(t_all_)
    {}

    virtual CloseAllStmtAST *asCloseAllStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckPointStmtAST: public StatementAST
{
public:
    int t_checkpoint;

public:
    CheckPointStmtAST( int t_checkpoint_ )
        : StatementAST()
        , t_checkpoint(t_checkpoint_)
    {}

    virtual CheckPointStmtAST *asCheckPointStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AnalyzeStmtAST: public StatementAST
{
public:
    int t_analize;
    int t_verbose;
    TableIdentifierAST *tabeIdent;
    CompoundColumnNamesAST *columns;

public:
    AnalyzeStmtAST( int t_analize_, int t_verbose_, TableIdentifierAST *tabeIdent_, CompoundColumnNamesAST *columns_ )
        : StatementAST()
        , t_analize(t_analize_)
        , t_verbose(t_verbose_)
        , tabeIdent(tabeIdent_)
        , columns(columns_)
    {}

    virtual AnalyzeStmtAST *asAnalyzeStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterUserMappingStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_user;
    int t_mapping;
    int t_for;
    AuthIdentifierAST *authIdent;
    int t_server;
    NameAST *serverName;
    AlterTableGenericCmdAST *cmds;

public:
    AlterUserMappingStmtAST( int t_alter_, int t_user_, int t_mapping_, int t_for_, AuthIdentifierAST *authIdent_, int t_server_, NameAST *serverName_, AlterTableGenericCmdAST *cmds_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_user(t_user_)
        , t_mapping(t_mapping_)
        , t_for(t_for_)
        , authIdent(authIdent_)
        , t_server(t_server_)
        , serverName(serverName_)
        , cmds(cmds_)
    {}

    virtual AlterUserMappingStmtAST *asAlterUserMappingStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AuthIdentifierAST: public AST
{
public:
    AuthIdentifierAST() : AST() {}
    virtual AuthIdentifierAST *asAuthIdentifier() { return this; }
};

class UserAuthIdentifierAST: public AuthIdentifierAST
{
public:
    int t_user_or_current_user;

public:
    UserAuthIdentifierAST( int t_user_or_current_user_ )
        : AuthIdentifierAST()
        , t_user_or_current_user(t_user_or_current_user_)
    {}

    virtual UserAuthIdentifierAST *asUserAuthIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoleAuthIdentifierAST: public AuthIdentifierAST
{
public:
    RoleIdAST *roleId;

public:
    RoleAuthIdentifierAST( RoleIdAST *roleId_ )
        : AuthIdentifierAST()
        , roleId(roleId_)
    {}

    virtual RoleAuthIdentifierAST *asRoleAuthIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTSConfigurationStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_text;
    int t_search;
    int t_configuration;
    AnyIdentifierAST *confIdent;
    AlterTSConfCmdAST *cmd;

public:
    AlterTSConfigurationStmtAST( int t_alter_, int t_text_, int t_search_, int t_configuration_, AnyIdentifierAST *confIdent_, AlterTSConfCmdAST *cmd_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_text(t_text_)
        , t_search(t_search_)
        , t_configuration(t_configuration_)
        , confIdent(confIdent_)
        , cmd(cmd_)
    {}

    virtual AlterTSConfigurationStmtAST *asAlterTSConfigurationStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTSConfCmdAST: public AST
{
public:
    AlterTSConfCmdAST() : AST() {}
    virtual AlterTSConfCmdAST *asAlterTSConfCmd() { return this; }
};

class AlterTSConfAddAlterCmdAST: public AlterTSConfCmdAST
{
public:
    int t_add_or_alter;
    int t_mapping;
    int t_for;
    NameListAST *forNames;
    int t_replace;
    AnyIdentifierAST *replaceIdent;
    int t_with;
    AnyIdentifierListAST *withIdents;

public:
    AlterTSConfAddAlterCmdAST( int t_add_or_alter_, int t_mapping_, int t_for_, NameListAST *forNames_, int t_replace_, AnyIdentifierAST *replaceIdent_, int t_with_, AnyIdentifierListAST *withIdents_ )
        : AlterTSConfCmdAST()
        , t_add_or_alter(t_add_or_alter_)
        , t_mapping(t_mapping_)
        , t_for(t_for_)
        , forNames(finish(forNames_))
        , t_replace(t_replace_)
        , replaceIdent(replaceIdent_)
        , t_with(t_with_)
        , withIdents(finish(withIdents_))
    {}

    virtual AlterTSConfAddAlterCmdAST *asAlterTSConfAddAlterCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTSConfDropCmdAST: public AlterTSConfCmdAST
{
public:
    int t_drop;
    int t_mapping;
    int t_if;
    int t_exists;
    int t_for;
    NameListAST *names;

public:
    AlterTSConfDropCmdAST( int t_drop_, int t_mapping_, int t_if_, int t_exists_, int t_for_, NameListAST *names_ )
        : AlterTSConfCmdAST()
        , t_drop(t_drop_)
        , t_mapping(t_mapping_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , t_for(t_for_)
        , names(finish(names_))
    {}

    virtual AlterTSConfDropCmdAST *asAlterTSConfDropCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTSDictionaryStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_text;
    int t_search;
    int t_dictionary;
    AnyIdentifierAST *dictIdent;
    DefinitionAST *definition;

public:
    AlterTSDictionaryStmtAST( int t_alter_, int t_text_, int t_search_, int t_dictionary_, AnyIdentifierAST *dictIdent_, DefinitionAST *definition_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_text(t_text_)
        , t_search(t_search_)
        , t_dictionary(t_dictionary_)
        , dictIdent(dictIdent_)
        , definition(definition_)
    {}

    virtual AlterTSDictionaryStmtAST *asAlterTSDictionaryStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterCompositeTypeStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_type;
    AnyIdentifierAST *typeIdent;
    AlterTypeCommandListAST *commands;

public:
    AlterCompositeTypeStmtAST( int t_alter_, int t_type_, AnyIdentifierAST *typeIdent_, AlterTypeCommandListAST *commands_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , commands(finish(commands_))
    {}

    virtual AlterCompositeTypeStmtAST *asAlterCompositeTypeStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTypeCommandAST: public AST
{
public:
    AlterTypeCommandAST() : AST() {}
    virtual AlterTypeCommandAST *asAlterTypeCommand() { return this; }
};

class AlterTypeAddCommandAST: public AlterTypeCommandAST
{
public:
    int t_add;
    int t_atribute;
    TableFuncElementAST *elem;
    int t_behavior;

public:
    AlterTypeAddCommandAST( int t_add_, int t_atribute_, TableFuncElementAST *elem_, int t_behavior_ )
        : AlterTypeCommandAST()
        , t_add(t_add_)
        , t_atribute(t_atribute_)
        , elem(elem_)
        , t_behavior(t_behavior_)
    {}

    virtual AlterTypeAddCommandAST *asAlterTypeAddCommand() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTypeDropCommandAST: public AlterTypeCommandAST
{
public:
    int t_drop;
    int t_attribute;
    int t_if;
    int t_exists;
    NameAST *attrName;
    int t_behavior;

public:
    AlterTypeDropCommandAST( int t_drop_, int t_attribute_, int t_if_, int t_exists_, NameAST *attrName_, int t_behavior_ )
        : AlterTypeCommandAST()
        , t_drop(t_drop_)
        , t_attribute(t_attribute_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , attrName(attrName_)
        , t_behavior(t_behavior_)
    {}

    virtual AlterTypeDropCommandAST *asAlterTypeDropCommand() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTypeAlterCommandAST: public AlterTypeCommandAST
{
public:
    int t_alter;
    int t_attribute;
    NameAST *attrName;
    SetDataAST *setData;
    int t_type;
    TypeAST *newType;
    CollateClauseAST *collate;
    int t_behavior;

public:
    AlterTypeAlterCommandAST( int t_alter_, int t_attribute_, NameAST *attrName_, SetDataAST *setData_, int t_type_, TypeAST *newType_, CollateClauseAST *collate_, int t_behavior_ )
        : AlterTypeCommandAST()
        , t_alter(t_alter_)
        , t_attribute(t_attribute_)
        , attrName(attrName_)
        , setData(setData_)
        , t_type(t_type_)
        , newType(newType_)
        , collate(collate_)
        , t_behavior(t_behavior_)
    {}

    virtual AlterTypeAlterCommandAST *asAlterTypeAlterCommand() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterSeqStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_sequence;
    int t_if;
    int t_exists;
    SequenceIdentifierAST *seqIdent;
    SequenceOptionListAST *options;

public:
    AlterSeqStmtAST( int t_alter_, int t_sequence_, int t_if_, int t_exists_, SequenceIdentifierAST *seqIdent_, SequenceOptionListAST *options_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_sequence(t_sequence_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , seqIdent(seqIdent_)
        , options(finish(options_))
    {}

    virtual AlterSeqStmtAST *asAlterSeqStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateSeqStmtAST: public CreateStatementAST
{
public:
    int t_create;
    TempClauseAST *tempClause;
    int t_sequence;
    SequenceIdentifierAST *seqIdent;
    SequenceOptionListAST *options;

public:
    CreateSeqStmtAST( int t_create_, TempClauseAST *tempClause_, int t_sequence_, SequenceIdentifierAST *seqIdent_, SequenceOptionListAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , tempClause(tempClause_)
        , t_sequence(t_sequence_)
        , seqIdent(seqIdent_)
        , options(finish(options_))
    {}

    virtual CreateSeqStmtAST *asCreateSeqStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SequenceOptionAST: public AST
{
public:
    SequenceOptionAST() : AST() {}
    virtual SequenceOptionAST *asSequenceOption() { return this; }
};

class SimpleSequenceOptionAST: public SequenceOptionAST
{
public:
    int t_sp1;
    int t_sp2;
    int t_sp3;

public:
    SimpleSequenceOptionAST( int t_sp1_, int t_sp2_, int t_sp3_ )
        : SequenceOptionAST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
        , t_sp3(t_sp3_)
    {}

    virtual SimpleSequenceOptionAST *asSimpleSequenceOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OwnedSequenceOptionAST: public SequenceOptionAST
{
public:
    int t_owned;
    int t_by;
    RoleIdAST *ownerRole;

public:
    OwnedSequenceOptionAST( int t_owned_, int t_by_, RoleIdAST *ownerRole_ )
        : SequenceOptionAST()
        , t_owned(t_owned_)
        , t_by(t_by_)
        , ownerRole(ownerRole_)
    {}

    virtual OwnedSequenceOptionAST *asOwnedSequenceOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TempClauseAST: public AST
{
public:
    int t_sp1;
    int t_sp2;

public:
    TempClauseAST( int t_sp1_, int t_sp2_ )
        : AST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
    {}

    virtual TempClauseAST *asTempClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOwnerStmtAST: public DdlStatementAST
{
public:
    AlterOwnerStmtAST() : DdlStatementAST() {}
    virtual AlterOwnerStmtAST *asAlterOwnerStmt() { return this; }
};

class AlterEvtTriggerOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_event;
    int t_trigger;
    NameAST *triggerName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterEvtTriggerOwnerStmtAST( int t_alter_, int t_event_, int t_trigger_, NameAST *triggerName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_event(t_event_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterEvtTriggerOwnerStmtAST *asAlterEvtTriggerOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterServerOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_server;
    NameAST *serverName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterServerOwnerStmtAST( int t_alter_, int t_server_, NameAST *serverName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_server(t_server_)
        , serverName(serverName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterServerOwnerStmtAST *asAlterServerOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterFdwOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameAST *wrapperName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterFdwOwnerStmtAST( int t_alter_, int t_foreign_, int t_data_, int t_wrapper_, NameAST *wrapperName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , wrapperName(wrapperName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterFdwOwnerStmtAST *asAlterFdwOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterSearchOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_text;
    int t_search;
    int t_dict_or_conf;
    AnyIdentifierAST *ident;
    OwnerToClauseAST *ownerToClause;

public:
    AlterSearchOwnerStmtAST( int t_alter_, int t_text_, int t_search_, int t_dict_or_conf_, AnyIdentifierAST *ident_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_text(t_text_)
        , t_search(t_search_)
        , t_dict_or_conf(t_dict_or_conf_)
        , ident(ident_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterSearchOwnerStmtAST *asAlterSearchOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTablespaceOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_tablespace;
    NameAST *tablespaceName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterTablespaceOwnerStmtAST( int t_alter_, int t_tablespace_, NameAST *tablespaceName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterTablespaceOwnerStmtAST *asAlterTablespaceOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTypeOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_type;
    AnyIdentifierAST *typeIdent;
    OwnerToClauseAST *ownerToClause;

public:
    AlterTypeOwnerStmtAST( int t_alter_, int t_type_, AnyIdentifierAST *typeIdent_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterTypeOwnerStmtAST *asAlterTypeOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterSchemaOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_schema;
    NameAST *schemaName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterSchemaOwnerStmtAST( int t_alter_, int t_schema_, NameAST *schemaName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_schema(t_schema_)
        , schemaName(schemaName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterSchemaOwnerStmtAST *asAlterSchemaOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOpUsingOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_operator;
    int t_class_or_family;
    AnyIdentifierAST *ident;
    int t_using;
    int t_acc_method;
    OwnerToClauseAST *ownerToClause;

public:
    AlterOpUsingOwnerStmtAST( int t_alter_, int t_operator_, int t_class_or_family_, AnyIdentifierAST *ident_, int t_using_, int t_acc_method_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , t_class_or_family(t_class_or_family_)
        , ident(ident_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterOpUsingOwnerStmtAST *asAlterOpUsingOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOpOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_operator;
    OperandAST *operand;
    OperArgTypesAST *argTypes;
    OwnerToClauseAST *ownerToClause;

public:
    AlterOpOwnerStmtAST( int t_alter_, int t_operator_, OperandAST *operand_, OperArgTypesAST *argTypes_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , operand(operand_)
        , argTypes(argTypes_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterOpOwnerStmtAST *asAlterOpOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterLargeObjOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_large;
    int t_object;
    int t_value;
    OwnerToClauseAST *ownerToClause;

public:
    AlterLargeObjOwnerStmtAST( int t_alter_, int t_large_, int t_object_, int t_value_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_large(t_large_)
        , t_object(t_object_)
        , t_value(t_value_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterLargeObjOwnerStmtAST *asAlterLargeObjOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterLangOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_procedural;
    int t_language;
    NameAST *langName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterLangOwnerStmtAST( int t_alter_, int t_procedural_, int t_language_, NameAST *langName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , langName(langName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterLangOwnerStmtAST *asAlterLangOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterFunctionOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_function;
    FunctionWithArgTypesAST *funcWithArgTypes;
    OwnerToClauseAST *ownerToClause;

public:
    AlterFunctionOwnerStmtAST( int t_alter_, int t_function_, FunctionWithArgTypesAST *funcWithArgTypes_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_function(t_function_)
        , funcWithArgTypes(funcWithArgTypes_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterFunctionOwnerStmtAST *asAlterFunctionOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDbOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_database;
    DatabaseNameAST *dbName;
    OwnerToClauseAST *ownerToClause;

public:
    AlterDbOwnerStmtAST( int t_alter_, int t_database_, DatabaseNameAST *dbName_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_database(t_database_)
        , dbName(dbName_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterDbOwnerStmtAST *asAlterDbOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterCollationOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_collation;
    AnyIdentifierAST *ident;
    OwnerToClauseAST *ownerToClause;

public:
    AlterCollationOwnerStmtAST( int t_alter_, int t_collation_, AnyIdentifierAST *ident_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_collation(t_collation_)
        , ident(ident_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterCollationOwnerStmtAST *asAlterCollationOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterConvOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_conversion;
    AnyIdentifierAST *ident;
    OwnerToClauseAST *ownerToClause;

public:
    AlterConvOwnerStmtAST( int t_alter_, int t_conversion_, AnyIdentifierAST *ident_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_conversion(t_conversion_)
        , ident(ident_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterConvOwnerStmtAST *asAlterConvOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *ident;
    OwnerToClauseAST *ownerToClause;

public:
    AlterDomainOwnerStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *ident_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , ident(ident_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterDomainOwnerStmtAST *asAlterDomainOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterAggrOwnerStmtAST: public AlterOwnerStmtAST
{
public:
    int t_alter;
    int t_aggregate;
    FunctionRefAST *funcRef;
    AggrArgumentsAST *args;
    OwnerToClauseAST *ownerToClause;

public:
    AlterAggrOwnerStmtAST( int t_alter_, int t_aggregate_, FunctionRefAST *funcRef_, AggrArgumentsAST *args_, OwnerToClauseAST *ownerToClause_ )
        : AlterOwnerStmtAST()
        , t_alter(t_alter_)
        , t_aggregate(t_aggregate_)
        , funcRef(funcRef_)
        , args(args_)
        , ownerToClause(ownerToClause_)
    {}

    virtual AlterAggrOwnerStmtAST *asAlterAggrOwnerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OwnerToClauseAST: public AST
{
public:
    int t_owner;
    int t_to;
    RoleIdAST *roleId;

public:
    OwnerToClauseAST( int t_owner_, int t_to_, RoleIdAST *roleId_ )
        : AST()
        , t_owner(t_owner_)
        , t_to(t_to_)
        , roleId(roleId_)
    {}

    virtual OwnerToClauseAST *asOwnerToClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterObjectSchemaStmtAST: public DdlStatementAST
{
public:
    AlterObjectSchemaStmtAST() : DdlStatementAST() {}
    virtual AlterObjectSchemaStmtAST *asAlterObjectSchemaStmt() { return this; }
};

class AlterTypeSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_type;
    AnyIdentifierAST *typeIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterTypeSchemaStmtAST( int t_alter_, int t_type_, AnyIdentifierAST *typeIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_type(t_type_)
        , typeIdent(typeIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterTypeSchemaStmtAST *asAlterTypeSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterViewSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_materialized;
    int t_view;
    int t_if;
    int t_exists;
    TableIdentifierAST *viewIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterViewSchemaStmtAST( int t_alter_, int t_materialized_, int t_view_, int t_if_, int t_exists_, TableIdentifierAST *viewIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , viewIdent(viewIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterViewSchemaStmtAST *asAlterViewSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterSeqSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_sequence;
    int t_if;
    int t_exists;
    SequenceIdentifierAST *sequenceIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterSeqSchemaStmtAST( int t_alter_, int t_sequence_, int t_if_, int t_exists_, SequenceIdentifierAST *sequenceIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_sequence(t_sequence_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , sequenceIdent(sequenceIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterSeqSchemaStmtAST *asAlterSeqSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterSearchSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_text;
    int t_search;
    int t_sp_what;
    AnyIdentifierAST *ident;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterSearchSchemaStmtAST( int t_alter_, int t_text_, int t_search_, int t_sp_what_, AnyIdentifierAST *ident_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_text(t_text_)
        , t_search(t_search_)
        , t_sp_what(t_sp_what_)
        , ident(ident_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterSearchSchemaStmtAST *asAlterSearchSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_foreign;
    int t_table;
    int t_if;
    int t_exists;
    RelationExprAST *rel;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterTableSchemaStmtAST( int t_alter_, int t_foreign_, int t_table_, int t_if_, int t_exists_, RelationExprAST *rel_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , rel(rel_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterTableSchemaStmtAST *asAlterTableSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOpUsingSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_operator;
    int t_class_orFamily;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterOpUsingSchemaStmtAST( int t_alter_, int t_operator_, int t_class_orFamily_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , t_class_orFamily(t_class_orFamily_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterOpUsingSchemaStmtAST *asAlterOpUsingSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterOpSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_operator;
    OperandAST *operand;
    OperArgTypesAST *argTypes;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterOpSchemaStmtAST( int t_alter_, int t_operator_, OperandAST *operand_, OperArgTypesAST *argTypes_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_operator(t_operator_)
        , operand(operand_)
        , argTypes(argTypes_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterOpSchemaStmtAST *asAlterOpSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterFunctionSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_function;
    FunctionWithArgTypesAST *functionWithArgTypes;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterFunctionSchemaStmtAST( int t_alter_, int t_function_, FunctionWithArgTypesAST *functionWithArgTypes_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_function(t_function_)
        , functionWithArgTypes(functionWithArgTypes_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterFunctionSchemaStmtAST *asAlterFunctionSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterCollationSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_collation;
    AnyIdentifierAST *collationIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterCollationSchemaStmtAST( int t_alter_, int t_collation_, AnyIdentifierAST *collationIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_collation(t_collation_)
        , collationIdent(collationIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterCollationSchemaStmtAST *asAlterCollationSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterConvSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_convertion;
    AnyIdentifierAST *convertionIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterConvSchemaStmtAST( int t_alter_, int t_convertion_, AnyIdentifierAST *convertionIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_convertion(t_convertion_)
        , convertionIdent(convertionIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterConvSchemaStmtAST *asAlterConvSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterDomainSchemaStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *domainIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterDomainSchemaStmtAST *asAlterDomainSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterExtSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_extension;
    AnyIdentifierAST *extensionIdent;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterExtSchemaStmtAST( int t_alter_, int t_extension_, AnyIdentifierAST *extensionIdent_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_extension(t_extension_)
        , extensionIdent(extensionIdent_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterExtSchemaStmtAST *asAlterExtSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterAggrFuncSchemaStmtAST: public AlterObjectSchemaStmtAST
{
public:
    int t_alter;
    int t_aggregate;
    FunctionRefAST *funcRef;
    AggrArgumentsAST *args;
    SetSchemaClauseAST *setSchemaClause;

public:
    AlterAggrFuncSchemaStmtAST( int t_alter_, int t_aggregate_, FunctionRefAST *funcRef_, AggrArgumentsAST *args_, SetSchemaClauseAST *setSchemaClause_ )
        : AlterObjectSchemaStmtAST()
        , t_alter(t_alter_)
        , t_aggregate(t_aggregate_)
        , funcRef(funcRef_)
        , args(args_)
        , setSchemaClause(setSchemaClause_)
    {}

    virtual AlterAggrFuncSchemaStmtAST *asAlterAggrFuncSchemaStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetSchemaClauseAST: public AST
{
public:
    int t_set;
    int t_schema;
    NameAST *schemaName;

public:
    SetSchemaClauseAST( int t_set_, int t_schema_, NameAST *schemaName_ )
        : AST()
        , t_set(t_set_)
        , t_schema(t_schema_)
        , schemaName(schemaName_)
    {}

    virtual SetSchemaClauseAST *asSetSchemaClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterFunctionStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_function;
    FunctionWithArgTypesAST *functionWithArgTypes;
    AlterFunctionOptionListAST *alterOptions;
    int t_restrict;

public:
    AlterFunctionStmtAST( int t_alter_, int t_function_, FunctionWithArgTypesAST *functionWithArgTypes_, AlterFunctionOptionListAST *alterOptions_, int t_restrict_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_function(t_function_)
        , functionWithArgTypes(functionWithArgTypes_)
        , alterOptions(finish(alterOptions_))
        , t_restrict(t_restrict_)
    {}

    virtual AlterFunctionStmtAST *asAlterFunctionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterFunctionOptionAST: public AST
{
public:
    AlterFunctionOptionAST() : AST() {}
    virtual AlterFunctionOptionAST *asAlterFunctionOption() { return this; }
};

class CommonAlterFunctionOptAST: public AlterFunctionOptionAST
{
public:
    int t_sp1;
    int t_sp2;
    int t_sp3;
    int t_sp4;

public:
    CommonAlterFunctionOptAST( int t_sp1_, int t_sp2_, int t_sp3_, int t_sp4_ )
        : AlterFunctionOptionAST()
        , t_sp1(t_sp1_)
        , t_sp2(t_sp2_)
        , t_sp3(t_sp3_)
        , t_sp4(t_sp4_)
    {}

    virtual CommonAlterFunctionOptAST *asCommonAlterFunctionOpt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RestAlterFunctionOptAST: public AlterFunctionOptionAST
{
public:
    SetResetClauseAST *rest;

public:
    RestAlterFunctionOptAST( SetResetClauseAST *rest_ )
        : AlterFunctionOptionAST()
        , rest(rest_)
    {}

    virtual RestAlterFunctionOptAST *asRestAlterFunctionOpt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterForeignServerStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_server;
    NameAST *serverName;
    ForeignServerVersionAST *version;
    AlterTableGenericCmdAST *genericCms;

public:
    AlterForeignServerStmtAST( int t_alter_, int t_server_, NameAST *serverName_, ForeignServerVersionAST *version_, AlterTableGenericCmdAST *genericCms_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_server(t_server_)
        , serverName(serverName_)
        , version(version_)
        , genericCms(genericCms_)
    {}

    virtual AlterForeignServerStmtAST *asAlterForeignServerStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForeignServerVersionAST: public AST
{
public:
    int t_version;
    int t_value;

public:
    ForeignServerVersionAST( int t_version_, int t_value_ )
        : AST()
        , t_version(t_version_)
        , t_value(t_value_)
    {}

    virtual ForeignServerVersionAST *asForeignServerVersion() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterFdwStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameAST *wrapperName;
    FdwOptionListAST *fdwOptions;
    AlterTableGenericCmdAST *genericCms;

public:
    AlterFdwStmtAST( int t_alter_, int t_foreign_, int t_data_, int t_wrapper_, NameAST *wrapperName_, FdwOptionListAST *fdwOptions_, AlterTableGenericCmdAST *genericCms_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , wrapperName(wrapperName_)
        , fdwOptions(finish(fdwOptions_))
        , genericCms(genericCms_)
    {}

    virtual AlterFdwStmtAST *asAlterFdwStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FdwOptionAST: public AST
{
public:
    FdwOptionAST() : AST() {}
    virtual FdwOptionAST *asFdwOption() { return this; }
};

class SetFdwOptionAST: public FdwOptionAST
{
public:
    int t_handler_or_validator;
    HandlerIdentifierAST *handlerIdent;

public:
    SetFdwOptionAST( int t_handler_or_validator_, HandlerIdentifierAST *handlerIdent_ )
        : FdwOptionAST()
        , t_handler_or_validator(t_handler_or_validator_)
        , handlerIdent(handlerIdent_)
    {}

    virtual SetFdwOptionAST *asSetFdwOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ResetFdwOptionAST: public FdwOptionAST
{
public:
    int t_no;
    int t_handler_or_validator;

public:
    ResetFdwOptionAST( int t_no_, int t_handler_or_validator_ )
        : FdwOptionAST()
        , t_no(t_no_)
        , t_handler_or_validator(t_handler_or_validator_)
    {}

    virtual ResetFdwOptionAST *asResetFdwOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class HandlerIdentifierAST: public AST
{
public:
    NameAST *firstName;
    NameListAST *attrs;

public:
    HandlerIdentifierAST( NameAST *firstName_, NameListAST *attrs_ )
        : AST()
        , firstName(firstName_)
        , attrs(finish(attrs_))
    {}

    virtual HandlerIdentifierAST *asHandlerIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterExtensionCtxStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_extension;
    NameAST *extName;
    int t_add_or_drop;
    ExtensionCtxCmdAST *cmd;

public:
    AlterExtensionCtxStmtAST( int t_alter_, int t_extension_, NameAST *extName_, int t_add_or_drop_, ExtensionCtxCmdAST *cmd_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_extension(t_extension_)
        , extName(extName_)
        , t_add_or_drop(t_add_or_drop_)
        , cmd(cmd_)
    {}

    virtual AlterExtensionCtxStmtAST *asAlterExtensionCtxStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExtensionCtxCmdAST: public AST
{
public:
    ExtensionCtxCmdAST() : AST() {}
    virtual ExtensionCtxCmdAST *asExtensionCtxCmd() { return this; }
};

class OpExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_operator;
    OperandAST *operand;
    OperArgTypesAST *opArgTypes;

public:
    OpExtensionCtxCmdAST( int t_operator_, OperandAST *operand_, OperArgTypesAST *opArgTypes_ )
        : ExtensionCtxCmdAST()
        , t_operator(t_operator_)
        , operand(operand_)
        , opArgTypes(opArgTypes_)
    {}

    virtual OpExtensionCtxCmdAST *asOpExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OperArgTypesAST: public AST
{
public:
    int t_lparen;
    TypeAST *type1;
    int t_comma;
    TypeAST *type2;
    int t_rparen;

public:
    OperArgTypesAST( int t_lparen_, TypeAST *type1_, int t_comma_, TypeAST *type2_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , type1(type1_)
        , t_comma(t_comma_)
        , type2(type2_)
        , t_rparen(t_rparen_)
    {}

    virtual OperArgTypesAST *asOperArgTypes() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LangExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_procedural;
    int t_language;
    NameAST *langName;

public:
    LangExtensionCtxCmdAST( int t_procedural_, int t_language_, NameAST *langName_ )
        : ExtensionCtxCmdAST()
        , t_procedural(t_procedural_)
        , t_language(t_language_)
        , langName(langName_)
    {}

    virtual LangExtensionCtxCmdAST *asLangExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_function;
    FunctionWithArgTypesAST *funcWithArgTypes;

public:
    FunctionExtensionCtxCmdAST( int t_function_, FunctionWithArgTypesAST *funcWithArgTypes_ )
        : ExtensionCtxCmdAST()
        , t_function(t_function_)
        , funcWithArgTypes(funcWithArgTypes_)
    {}

    virtual FunctionExtensionCtxCmdAST *asFunctionExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionWithArgTypesAST: public AST
{
public:
    FunctionRefAST *functionRef;
    FunctionArgTypesAST *argTypes;

public:
    FunctionWithArgTypesAST( FunctionRefAST *functionRef_, FunctionArgTypesAST *argTypes_ )
        : AST()
        , functionRef(functionRef_)
        , argTypes(argTypes_)
    {}

    virtual FunctionWithArgTypesAST *asFunctionWithArgTypes() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionArgTypesAST: public AST
{
public:
    int t_lparen;
    FunctionArgListAST *args;
    int t_rparen;

public:
    FunctionArgTypesAST( int t_lparen_, FunctionArgListAST *args_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual FunctionArgTypesAST *asFunctionArgTypes() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionArgAST: public AST
{
public:
    int t_pre_class;
    NameAST *paramName;
    int t_ps_class;
    FunctionTypeAST *funcType;

public:
    FunctionArgAST( int t_pre_class_, NameAST *paramName_, int t_ps_class_, FunctionTypeAST *funcType_ )
        : AST()
        , t_pre_class(t_pre_class_)
        , paramName(paramName_)
        , t_ps_class(t_ps_class_)
        , funcType(funcType_)
    {}

    virtual FunctionArgAST *asFunctionArg() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_sp_what;
    AnyIdentifierAST *ident;

public:
    SimpleExtensionCtxCmdAST( int t_sp_what_, AnyIdentifierAST *ident_ )
        : ExtensionCtxCmdAST()
        , t_sp_what(t_sp_what_)
        , ident(ident_)
    {}

    virtual SimpleExtensionCtxCmdAST *asSimpleExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CastExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_cast;
    int t_lparen;
    TypeAST *fromType;
    int t_as;
    TypeAST *toType;
    int t_rparen;

public:
    CastExtensionCtxCmdAST( int t_cast_, int t_lparen_, TypeAST *fromType_, int t_as_, TypeAST *toType_, int t_rparen_ )
        : ExtensionCtxCmdAST()
        , t_cast(t_cast_)
        , t_lparen(t_lparen_)
        , fromType(fromType_)
        , t_as(t_as_)
        , toType(toType_)
        , t_rparen(t_rparen_)
    {}

    virtual CastExtensionCtxCmdAST *asCastExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AggrExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_aggregate;
    FunctionRefAST *functionRef;
    AggrArgumentsAST *args;

public:
    AggrExtensionCtxCmdAST( int t_aggregate_, FunctionRefAST *functionRef_, AggrArgumentsAST *args_ )
        : ExtensionCtxCmdAST()
        , t_aggregate(t_aggregate_)
        , functionRef(functionRef_)
        , args(args_)
    {}

    virtual AggrExtensionCtxCmdAST *asAggrExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AggrArgumentsAST: public AST
{
public:
    AggrArgumentsAST() : AST() {}
    virtual AggrArgumentsAST *asAggrArguments() { return this; }
};

class ListAggrArgumentsAST: public AggrArgumentsAST
{
public:
    int t_lparen;
    TypeListAST *types;
    int t_rpren;

public:
    ListAggrArgumentsAST( int t_lparen_, TypeListAST *types_, int t_rpren_ )
        : AggrArgumentsAST()
        , t_lparen(t_lparen_)
        , types(finish(types_))
        , t_rpren(t_rpren_)
    {}

    virtual ListAggrArgumentsAST *asListAggrArguments() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StarAggrArgumentsAST: public AggrArgumentsAST
{
public:
    int t_lparen;
    int t_star;
    int t_rparen;

public:
    StarAggrArgumentsAST( int t_lparen_, int t_star_, int t_rparen_ )
        : AggrArgumentsAST()
        , t_lparen(t_lparen_)
        , t_star(t_star_)
        , t_rparen(t_rparen_)
    {}

    virtual StarAggrArgumentsAST *asStarAggrArguments() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OpUsingExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_operator;
    int t_class_or_family;
    AnyIdentifierAST *opIdent;
    int t_using;
    int t_acc_method;

public:
    OpUsingExtensionCtxCmdAST( int t_operator_, int t_class_or_family_, AnyIdentifierAST *opIdent_, int t_using_, int t_acc_method_ )
        : ExtensionCtxCmdAST()
        , t_operator(t_operator_)
        , t_class_or_family(t_class_or_family_)
        , opIdent(opIdent_)
        , t_using(t_using_)
        , t_acc_method(t_acc_method_)
    {}

    virtual OpUsingExtensionCtxCmdAST *asOpUsingExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SchemaExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_schema;
    NameAST *schemaName;

public:
    SchemaExtensionCtxCmdAST( int t_schema_, NameAST *schemaName_ )
        : ExtensionCtxCmdAST()
        , t_schema(t_schema_)
        , schemaName(schemaName_)
    {}

    virtual SchemaExtensionCtxCmdAST *asSchemaExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TrgEventExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_event;
    int t_trigger;
    NameAST *triggerName;

public:
    TrgEventExtensionCtxCmdAST( int t_event_, int t_trigger_, NameAST *triggerName_ )
        : ExtensionCtxCmdAST()
        , t_event(t_event_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
    {}

    virtual TrgEventExtensionCtxCmdAST *asTrgEventExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TxtSearchExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_text;
    int t_search;
    int t_sp_what;
    AnyIdentifierAST *spIdent;

public:
    TxtSearchExtensionCtxCmdAST( int t_text_, int t_search_, int t_sp_what_, AnyIdentifierAST *spIdent_ )
        : ExtensionCtxCmdAST()
        , t_text(t_text_)
        , t_search(t_search_)
        , t_sp_what(t_sp_what_)
        , spIdent(spIdent_)
    {}

    virtual TxtSearchExtensionCtxCmdAST *asTxtSearchExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SequenceExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_sequence;
    NameAST *seqName;

public:
    SequenceExtensionCtxCmdAST( int t_sequence_, NameAST *seqName_ )
        : ExtensionCtxCmdAST()
        , t_sequence(t_sequence_)
        , seqName(seqName_)
    {}

    virtual SequenceExtensionCtxCmdAST *asSequenceExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_foreign;
    int t_table;
    NameAST *tableName;

public:
    TableExtensionCtxCmdAST( int t_foreign_, int t_table_, NameAST *tableName_ )
        : ExtensionCtxCmdAST()
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , tableName(tableName_)
    {}

    virtual TableExtensionCtxCmdAST *asTableExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ViewExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_materialized;
    int t_view;
    NameAST *viewName;

public:
    ViewExtensionCtxCmdAST( int t_materialized_, int t_view_, NameAST *viewName_ )
        : ExtensionCtxCmdAST()
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , viewName(viewName_)
    {}

    virtual ViewExtensionCtxCmdAST *asViewExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FdwExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_foreign;
    int t_data;
    int t_wrapper;
    NameAST *fdwName;

public:
    FdwExtensionCtxCmdAST( int t_foreign_, int t_data_, int t_wrapper_, NameAST *fdwName_ )
        : ExtensionCtxCmdAST()
        , t_foreign(t_foreign_)
        , t_data(t_data_)
        , t_wrapper(t_wrapper_)
        , fdwName(fdwName_)
    {}

    virtual FdwExtensionCtxCmdAST *asFdwExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_type;
    AnyIdentifierAST *typeIdent;

public:
    TypeExtensionCtxCmdAST( int t_type_, AnyIdentifierAST *typeIdent_ )
        : ExtensionCtxCmdAST()
        , t_type(t_type_)
        , typeIdent(typeIdent_)
    {}

    virtual TypeExtensionCtxCmdAST *asTypeExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ServerExtensionCtxCmdAST: public ExtensionCtxCmdAST
{
public:
    int t_server;
    NameAST *serverName;

public:
    ServerExtensionCtxCmdAST( int t_server_, NameAST *serverName_ )
        : ExtensionCtxCmdAST()
        , t_server(t_server_)
        , serverName(serverName_)
    {}

    virtual ServerExtensionCtxCmdAST *asServerExtensionCtxCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterExtensionStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_extension;
    NameAST *extName;
    int t_update;
    UpdateExtensionCmdListAST *updates;

public:
    AlterExtensionStmtAST( int t_alter_, int t_extension_, NameAST *extName_, int t_update_, UpdateExtensionCmdListAST *updates_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_extension(t_extension_)
        , extName(extName_)
        , t_update(t_update_)
        , updates(finish(updates_))
    {}

    virtual AlterExtensionStmtAST *asAlterExtensionStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UpdateExtensionCmdAST: public AST
{
public:
    UpdateExtensionCmdAST() : AST() {}
    virtual UpdateExtensionCmdAST *asUpdateExtensionCmd() { return this; }
};

class UpdateToExtensionCmdAST: public UpdateExtensionCmdAST
{
public:
    int t_to;
    int t_value;

public:
    UpdateToExtensionCmdAST( int t_to_, int t_value_ )
        : UpdateExtensionCmdAST()
        , t_to(t_to_)
        , t_value(t_value_)
    {}

    virtual UpdateToExtensionCmdAST *asUpdateToExtensionCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterEnumStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_type;
    AnyIdentifierAST *enumIdent;
    int t_add;
    int t_value;
    IfNotExistsAST *ifNotExists;
    int t_add_value;
    int t_before_or_after;
    int t_place_value;

public:
    AlterEnumStmtAST( int t_alter_, int t_type_, AnyIdentifierAST *enumIdent_, int t_add_, int t_value_, IfNotExistsAST *ifNotExists_, int t_add_value_, int t_before_or_after_, int t_place_value_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_type(t_type_)
        , enumIdent(enumIdent_)
        , t_add(t_add_)
        , t_value(t_value_)
        , ifNotExists(ifNotExists_)
        , t_add_value(t_add_value_)
        , t_before_or_after(t_before_or_after_)
        , t_place_value(t_place_value_)
    {}

    virtual AlterEnumStmtAST *asAlterEnumStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IfNotExistsAST: public AST
{
public:
    int t_if;
    int t_not;
    int t_exists;

public:
    IfNotExistsAST( int t_if_, int t_not_, int t_exists_ )
        : AST()
        , t_if(t_if_)
        , t_not(t_not_)
        , t_exists(t_exists_)
    {}

    virtual IfNotExistsAST *asIfNotExists() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainStmtAST: public DdlStatementAST
{
public:
    AlterDomainStmtAST() : DdlStatementAST() {}
    virtual AlterDomainStmtAST *asAlterDomainStmt() { return this; }
};

class AlterDomainAltDefStmtAST: public AlterDomainStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    AlterDefaultColumnAST *alterCmd;

public:
    AlterDomainAltDefStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *domainIdent_, AlterDefaultColumnAST *alterCmd_ )
        : AlterDomainStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , alterCmd(alterCmd_)
    {}

    virtual AlterDomainAltDefStmtAST *asAlterDomainAltDefStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainAddConstrStmtAST: public AlterDomainStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    int t_add;
    TableConstraintAST *constraint;

public:
    AlterDomainAddConstrStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *domainIdent_, int t_add_, TableConstraintAST *constraint_ )
        : AlterDomainStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , t_add(t_add_)
        , constraint(constraint_)
    {}

    virtual AlterDomainAddConstrStmtAST *asAlterDomainAddConstrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainDropConstrStmtAST: public AlterDomainStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    int t_drop;
    int t_constraint;
    int t_if;
    int t_exists;
    NameAST *constraintName;
    int t_behavior;

public:
    AlterDomainDropConstrStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *domainIdent_, int t_drop_, int t_constraint_, int t_if_, int t_exists_, NameAST *constraintName_, int t_behavior_ )
        : AlterDomainStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , t_drop(t_drop_)
        , t_constraint(t_constraint_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , constraintName(constraintName_)
        , t_behavior(t_behavior_)
    {}

    virtual AlterDomainDropConstrStmtAST *asAlterDomainDropConstrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainValConstrStmtAST: public AlterDomainStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    int t_validate;
    int t_constraint;
    NameAST *constraintName;

public:
    AlterDomainValConstrStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *domainIdent_, int t_validate_, int t_constraint_, NameAST *constraintName_ )
        : AlterDomainStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , t_validate(t_validate_)
        , t_constraint(t_constraint_)
        , constraintName(constraintName_)
    {}

    virtual AlterDomainValConstrStmtAST *asAlterDomainValConstrStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDomainChgNotNullStmtAST: public AlterDomainStmtAST
{
public:
    int t_alter;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    int t_drop_or_set;
    int t_not;
    int t_null;

public:
    AlterDomainChgNotNullStmtAST( int t_alter_, int t_domain_, AnyIdentifierAST *domainIdent_, int t_drop_or_set_, int t_not_, int t_null_ )
        : AlterDomainStmtAST()
        , t_alter(t_alter_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , t_drop_or_set(t_drop_or_set_)
        , t_not(t_not_)
        , t_null(t_null_)
    {}

    virtual AlterDomainChgNotNullStmtAST *asAlterDomainChgNotNullStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateDomainStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_domain;
    AnyIdentifierAST *domainIdent;
    int t_as;
    TypeAST *domainType;
    ColumnConstraintListAST *constraints;

public:
    CreateDomainStmtAST( int t_create_, int t_domain_, AnyIdentifierAST *domainIdent_, int t_as_, TypeAST *domainType_, ColumnConstraintListAST *constraints_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_domain(t_domain_)
        , domainIdent(domainIdent_)
        , t_as(t_as_)
        , domainType(domainType_)
        , constraints(finish(constraints_))
    {}

    virtual CreateDomainStmtAST *asCreateDomainStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDefaultPrivilegesStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_default;
    int t_privileges;
    DefACLOptionListAST *options;
    DefACLActionAST *action;

public:
    AlterDefaultPrivilegesStmtAST( int t_alter_, int t_default_, int t_privileges_, DefACLOptionListAST *options_, DefACLActionAST *action_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_default(t_default_)
        , t_privileges(t_privileges_)
        , options(finish(options_))
        , action(action_)
    {}

    virtual AlterDefaultPrivilegesStmtAST *asAlterDefaultPrivilegesStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefACLOptionAST: public AST
{
public:
    DefACLOptionAST() : AST() {}
    virtual DefACLOptionAST *asDefACLOption() { return this; }
};

class InSchemaDefACLOptionAST: public DefACLOptionAST
{
public:
    int t_in;
    int t_schema;
    NameListAST *schemas;

public:
    InSchemaDefACLOptionAST( int t_in_, int t_schema_, NameListAST *schemas_ )
        : DefACLOptionAST()
        , t_in(t_in_)
        , t_schema(t_schema_)
        , schemas(finish(schemas_))
    {}

    virtual InSchemaDefACLOptionAST *asInSchemaDefACLOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForRoleUserDefACLOptionAST: public DefACLOptionAST
{
public:
    int t_for;
    int t_role_or_user;
    RoleIdListAST *roles;

public:
    ForRoleUserDefACLOptionAST( int t_for_, int t_role_or_user_, RoleIdListAST *roles_ )
        : DefACLOptionAST()
        , t_for(t_for_)
        , t_role_or_user(t_role_or_user_)
        , roles(finish(roles_))
    {}

    virtual ForRoleUserDefACLOptionAST *asForRoleUserDefACLOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefACLActionAST: public AST
{
public:
    DefACLActionAST() : AST() {}
    virtual DefACLActionAST *asDefACLAction() { return this; }
};

class RevokeDefACLActionAST: public DefACLActionAST
{
public:
    int t_revoke;
    int t_grant;
    int t_option;
    int t_for;
    PrivilegesAST *privs;
    int t_on;
    int t_target;
    int t_from;
    GranteeListAST *grantees;
    int t_behavior;

public:
    RevokeDefACLActionAST( int t_revoke_, int t_grant_, int t_option_, int t_for_, PrivilegesAST *privs_, int t_on_, int t_target_, int t_from_, GranteeListAST *grantees_, int t_behavior_ )
        : DefACLActionAST()
        , t_revoke(t_revoke_)
        , t_grant(t_grant_)
        , t_option(t_option_)
        , t_for(t_for_)
        , privs(privs_)
        , t_on(t_on_)
        , t_target(t_target_)
        , t_from(t_from_)
        , grantees(finish(grantees_))
        , t_behavior(t_behavior_)
    {}

    virtual RevokeDefACLActionAST *asRevokeDefACLAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GrantDefACLActionAST: public DefACLActionAST
{
public:
    int t_grant;
    PrivilegesAST *privs;
    int t_on;
    int t_target;
    int t_to;
    GranteeListAST *grantees;
    WithGrantOptionAST *wgo;

public:
    GrantDefACLActionAST( int t_grant_, PrivilegesAST *privs_, int t_on_, int t_target_, int t_to_, GranteeListAST *grantees_, WithGrantOptionAST *wgo_ )
        : DefACLActionAST()
        , t_grant(t_grant_)
        , privs(privs_)
        , t_on(t_on_)
        , t_target(t_target_)
        , t_to(t_to_)
        , grantees(finish(grantees_))
        , wgo(wgo_)
    {}

    virtual GrantDefACLActionAST *asGrantDefACLAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrivilegesAST: public AST
{
public:
    PrivilegesAST() : AST() {}
    virtual PrivilegesAST *asPrivileges() { return this; }
};

class AllPrivilegesAST: public PrivilegesAST
{
public:
    int t_all;
    int t_privileges;
    int t_lparen;
    NameListAST *columns;
    int t_rparen;

public:
    AllPrivilegesAST( int t_all_, int t_privileges_, int t_lparen_, NameListAST *columns_, int t_rparen_ )
        : PrivilegesAST()
        , t_all(t_all_)
        , t_privileges(t_privileges_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
    {}

    virtual AllPrivilegesAST *asAllPrivileges() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrivilegesAsListAST: public PrivilegesAST
{
public:
    CommonPrivilegeListAST *privs;

public:
    PrivilegesAsListAST( CommonPrivilegeListAST *privs_ )
        : PrivilegesAST()
        , privs(finish(privs_))
    {}

    virtual PrivilegesAsListAST *asPrivilegesAsList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonPrivilegeAST: public AST
{
public:
    int t_priv_entity;
    RefColumnsAST *refColumns;

public:
    CommonPrivilegeAST( int t_priv_entity_, RefColumnsAST *refColumns_ )
        : AST()
        , t_priv_entity(t_priv_entity_)
        , refColumns(refColumns_)
    {}

    virtual CommonPrivilegeAST *asCommonPrivilege() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GranteeAST: public AST
{
public:
    int t_group;
    RoleIdAST *role;

public:
    GranteeAST( int t_group_, RoleIdAST *role_ )
        : AST()
        , t_group(t_group_)
        , role(role_)
    {}

    virtual GranteeAST *asGrantee() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithGrantOptionAST: public AST
{
public:
    int t_with;
    int t_grant;
    int t_option;

public:
    WithGrantOptionAST( int t_with_, int t_grant_, int t_option_ )
        : AST()
        , t_with(t_with_)
        , t_grant(t_grant_)
        , t_option(t_option_)
    {}

    virtual WithGrantOptionAST *asWithGrantOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDatabaseStmtAST: public DdlStatementAST
{
public:
    AlterDatabaseStmtAST() : DdlStatementAST() {}
    virtual AlterDatabaseStmtAST *asAlterDatabaseStmt() { return this; }
};

class AlterDatabaseSetStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_database;
    NameAST *databaseName;
    SetResetClauseAST *setResetClause;

public:
    AlterDatabaseSetStmtAST( int t_alter_, int t_database_, NameAST *databaseName_, SetResetClauseAST *setResetClause_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_database(t_database_)
        , databaseName(databaseName_)
        , setResetClause(setResetClause_)
    {}

    virtual AlterDatabaseSetStmtAST *asAlterDatabaseSetStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDatabaseChgOptStmtAST: public AlterDatabaseStmtAST
{
public:
    int t_alter;
    int t_database;
    NameAST *databaseName;
    int t_with;
    AlterDbOptionListAST *options;

public:
    AlterDatabaseChgOptStmtAST( int t_alter_, int t_database_, NameAST *databaseName_, int t_with_, AlterDbOptionListAST *options_ )
        : AlterDatabaseStmtAST()
        , t_alter(t_alter_)
        , t_database(t_database_)
        , databaseName(databaseName_)
        , t_with(t_with_)
        , options(finish(options_))
    {}

    virtual AlterDatabaseChgOptStmtAST *asAlterDatabaseChgOptStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDbOptionAST: public AST
{
public:
    AlterDbOptionAST() : AST() {}
    virtual AlterDbOptionAST *asAlterDbOption() { return this; }
};

class AlterDbConnLimitOptionAST: public AlterDbOptionAST
{
public:
    int t_connection;
    int t_limit;
    int t_eqaul;
    int t_value;

public:
    AlterDbConnLimitOptionAST( int t_connection_, int t_limit_, int t_eqaul_, int t_value_ )
        : AlterDbOptionAST()
        , t_connection(t_connection_)
        , t_limit(t_limit_)
        , t_eqaul(t_eqaul_)
        , t_value(t_value_)
    {}

    virtual AlterDbConnLimitOptionAST *asAlterDbConnLimitOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDatabaseSetTblspStmtAST: public AlterDatabaseStmtAST
{
public:
    int t_alter;
    int t_database;
    NameAST *databaseName;
    int t_set;
    int t_tablespace;
    NameAST *tablespaceName;

public:
    AlterDatabaseSetTblspStmtAST( int t_alter_, int t_database_, NameAST *databaseName_, int t_set_, int t_tablespace_, NameAST *tablespaceName_ )
        : AlterDatabaseStmtAST()
        , t_alter(t_alter_)
        , t_database(t_database_)
        , databaseName(databaseName_)
        , t_set(t_set_)
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
    {}

    virtual AlterDatabaseSetTblspStmtAST *asAlterDatabaseSetTblspStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateEventTrigStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_event;
    int t_trigger;
    NameAST *triggerName;
    int t_on;
    NameAST *label;
    int t_when;
    EventTriggerWhenListAST *whens;
    int t_execute;
    int t_procedure;
    FunctionRefAST *funcRef;
    int t_lparen;
    int t_rparen;

public:
    CreateEventTrigStmtAST( int t_create_, int t_event_, int t_trigger_, NameAST *triggerName_, int t_on_, NameAST *label_, int t_when_, EventTriggerWhenListAST *whens_, int t_execute_, int t_procedure_, FunctionRefAST *funcRef_, int t_lparen_, int t_rparen_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_event(t_event_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , t_on(t_on_)
        , label(label_)
        , t_when(t_when_)
        , whens(finish(whens_))
        , t_execute(t_execute_)
        , t_procedure(t_procedure_)
        , funcRef(funcRef_)
        , t_lparen(t_lparen_)
        , t_rparen(t_rparen_)
    {}

    virtual CreateEventTrigStmtAST *asCreateEventTrigStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EventTriggerWhenAST: public AST
{
public:
    NameAST *colName;
    int t_in;
    int t_lparen;
    EventTriggerValueListAST *values;
    int t_rparen;

public:
    EventTriggerWhenAST( NameAST *colName_, int t_in_, int t_lparen_, EventTriggerValueListAST *values_, int t_rparen_ )
        : AST()
        , colName(colName_)
        , t_in(t_in_)
        , t_lparen(t_lparen_)
        , values(finish(values_))
        , t_rparen(t_rparen_)
    {}

    virtual EventTriggerWhenAST *asEventTriggerWhen() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EventTriggerValueAST: public AST
{
public:
    int t_value;

public:
    EventTriggerValueAST( int t_value_ )
        : AST()
        , t_value(t_value_)
    {}

    virtual EventTriggerValueAST *asEventTriggerValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterEventTrigStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_event;
    int t_trigger;
    NameAST *triggerName;
    EventTriggerCmdAST *eventCmd;

public:
    AlterEventTrigStmtAST( int t_alter_, int t_event_, int t_trigger_, NameAST *triggerName_, EventTriggerCmdAST *eventCmd_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_event(t_event_)
        , t_trigger(t_trigger_)
        , triggerName(triggerName_)
        , eventCmd(eventCmd_)
    {}

    virtual AlterEventTrigStmtAST *asAlterEventTrigStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EventTriggerCmdAST: public AST
{
public:
    int t_enable_or_disable;
    int t_always_or_replica;

public:
    EventTriggerCmdAST( int t_enable_or_disable_, int t_always_or_replica_ )
        : AST()
        , t_enable_or_disable(t_enable_or_disable_)
        , t_always_or_replica(t_always_or_replica_)
    {}

    virtual EventTriggerCmdAST *asEventTriggerCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateRoleStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_role;
    RoleIdAST *roleIdent;
    int t_with;
    OptRoleListAST *options;

public:
    CreateRoleStmtAST( int t_create_, int t_role_, RoleIdAST *roleIdent_, int t_with_, OptRoleListAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_role(t_role_)
        , roleIdent(roleIdent_)
        , t_with(t_with_)
        , options(finish(options_))
    {}

    virtual CreateRoleStmtAST *asCreateRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterRoleStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_role;
    RoleIdAST *roleIdent;
    int t_with;
    OptRoleListAST *roleOptions;

public:
    AlterRoleStmtAST( int t_alter_, int t_role_, RoleIdAST *roleIdent_, int t_with_, OptRoleListAST *roleOptions_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_role(t_role_)
        , roleIdent(roleIdent_)
        , t_with(t_with_)
        , roleOptions(finish(roleOptions_))
    {}

    virtual AlterRoleStmtAST *asAlterRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OptRoleAST: public AST
{
public:
    OptRoleAST() : AST() {}
    virtual OptRoleAST *asOptRole() { return this; }
};

class OptRoleWithRoleListAST: public OptRoleAST
{
public:
    int t_spec1;
    int t_spec2;
    RoleIdListAST *roles;

public:
    OptRoleWithRoleListAST( int t_spec1_, int t_spec2_, RoleIdListAST *roles_ )
        : OptRoleAST()
        , t_spec1(t_spec1_)
        , t_spec2(t_spec2_)
        , roles(finish(roles_))
    {}

    virtual OptRoleWithRoleListAST *asOptRoleWithRoleList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OptRoleStaticAST: public OptRoleAST
{
public:
    int t_spec1;
    int t_spec2;
    int t_spec3;

public:
    OptRoleStaticAST( int t_spec1_, int t_spec2_, int t_spec3_ )
        : OptRoleAST()
        , t_spec1(t_spec1_)
        , t_spec2(t_spec2_)
        , t_spec3(t_spec3_)
    {}

    virtual OptRoleStaticAST *asOptRoleStatic() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoleIdAST: public AST
{
public:
    NameAST *roleName;

public:
    RoleIdAST( NameAST *roleName_ )
        : AST()
        , roleName(roleName_)
    {}

    virtual RoleIdAST *asRoleId() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DatabaseNameAST: public AST
{
public:
    NameAST *databaseName;

public:
    DatabaseNameAST( NameAST *databaseName_ )
        : AST()
        , databaseName(databaseName_)
    {}

    virtual DatabaseNameAST *asDatabaseName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InDatabaseClauseAST: public AST
{
public:
    int t_in;
    int t_database;
    DatabaseNameAST *databaseName;

public:
    InDatabaseClauseAST( int t_in_, int t_database_, DatabaseNameAST *databaseName_ )
        : AST()
        , t_in(t_in_)
        , t_database(t_database_)
        , databaseName(databaseName_)
    {}

    virtual InDatabaseClauseAST *asInDatabaseClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterRoleSetStmtAST: public DdlStatementAST
{
public:
    AlterRoleSetStmtAST() : DdlStatementAST() {}
    virtual AlterRoleSetStmtAST *asAlterRoleSetStmt() { return this; }
};

class AlterAnyRoleSetStmtAST: public AlterRoleSetStmtAST
{
public:
    int t_alter;
    int t_role;
    RoleIdAST *roleIdent;
    InDatabaseClauseAST *inDbClause;
    SetResetClauseAST *setClause;

public:
    AlterAnyRoleSetStmtAST( int t_alter_, int t_role_, RoleIdAST *roleIdent_, InDatabaseClauseAST *inDbClause_, SetResetClauseAST *setClause_ )
        : AlterRoleSetStmtAST()
        , t_alter(t_alter_)
        , t_role(t_role_)
        , roleIdent(roleIdent_)
        , inDbClause(inDbClause_)
        , setClause(setClause_)
    {}

    virtual AlterAnyRoleSetStmtAST *asAlterAnyRoleSetStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterAllRolesSetStmtAST: public AlterRoleSetStmtAST
{
public:
    int t_alter;
    int t_role;
    int t_all;
    InDatabaseClauseAST *inDbClause;
    SetResetClauseAST *setClause;

public:
    AlterAllRolesSetStmtAST( int t_alter_, int t_role_, int t_all_, InDatabaseClauseAST *inDbClause_, SetResetClauseAST *setClause_ )
        : AlterRoleSetStmtAST()
        , t_alter(t_alter_)
        , t_role(t_role_)
        , t_all(t_all_)
        , inDbClause(inDbClause_)
        , setClause(setClause_)
    {}

    virtual AlterAllRolesSetStmtAST *asAlterAllRolesSetStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetResetClauseAST: public AST
{
public:
    SetResetClauseAST() : AST() {}
    virtual SetResetClauseAST *asSetResetClause() { return this; }
};

class SetResetClauseAsSetAST: public SetResetClauseAST
{
public:
    int t_set;
    SetRestAST *rest;

public:
    SetResetClauseAsSetAST( int t_set_, SetRestAST *rest_ )
        : SetResetClauseAST()
        , t_set(t_set_)
        , rest(rest_)
    {}

    virtual SetResetClauseAsSetAST *asSetResetClauseAsSet() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetResetClauseAsVarResetAST: public SetResetClauseAST
{
public:
    VariableResetStmtAST *varReset;

public:
    SetResetClauseAsVarResetAST( VariableResetStmtAST *varReset_ )
        : SetResetClauseAST()
        , varReset(varReset_)
    {}

    virtual SetResetClauseAsVarResetAST *asSetResetClauseAsVarReset() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestAST: public AST
{
public:
    SetRestAST() : AST() {}
    virtual SetRestAST *asSetRest() { return this; }
};

class SetRestTransactionAST: public SetRestAST
{
public:
    int t_session;
    int t_characterictics;
    int t_as;
    int t_transaction;
    TransactionModeListAST *modes;

public:
    SetRestTransactionAST( int t_session_, int t_characterictics_, int t_as_, int t_transaction_, TransactionModeListAST *modes_ )
        : SetRestAST()
        , t_session(t_session_)
        , t_characterictics(t_characterictics_)
        , t_as(t_as_)
        , t_transaction(t_transaction_)
        , modes(finish(modes_))
    {}

    virtual SetRestTransactionAST *asSetRestTransaction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestVarNameToAST: public SetRestAST
{
public:
    NameListAST *dotted_name;
    int t_to_or_equal_or_from;
    int t_default_or_equal;

public:
    SetRestVarNameToAST( NameListAST *dotted_name_, int t_to_or_equal_or_from_, int t_default_or_equal_ )
        : SetRestAST()
        , dotted_name(finish(dotted_name_))
        , t_to_or_equal_or_from(t_to_or_equal_or_from_)
        , t_default_or_equal(t_default_or_equal_)
    {}

    virtual SetRestVarNameToAST *asSetRestVarNameTo() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestVarNameToValuesAST: public SetRestAST
{
public:
    NameListAST *dotted_name;
    int t_to_or_equal_or_from;
    VarValueListAST *values;

public:
    SetRestVarNameToValuesAST( NameListAST *dotted_name_, int t_to_or_equal_or_from_, VarValueListAST *values_ )
        : SetRestAST()
        , dotted_name(finish(dotted_name_))
        , t_to_or_equal_or_from(t_to_or_equal_or_from_)
        , values(finish(values_))
    {}

    virtual SetRestVarNameToValuesAST *asSetRestVarNameToValues() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestZoneValueAST: public SetRestAST
{
public:
    int t_time;
    int t_zone;
    ZoneValueAST *zoneValue;

public:
    SetRestZoneValueAST( int t_time_, int t_zone_, ZoneValueAST *zoneValue_ )
        : SetRestAST()
        , t_time(t_time_)
        , t_zone(t_zone_)
        , zoneValue(zoneValue_)
    {}

    virtual SetRestZoneValueAST *asSetRestZoneValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestCatalogAST: public SetRestAST
{
public:
    int t_catalog;
    int t_catalog_name;

public:
    SetRestCatalogAST( int t_catalog_, int t_catalog_name_ )
        : SetRestAST()
        , t_catalog(t_catalog_)
        , t_catalog_name(t_catalog_name_)
    {}

    virtual SetRestCatalogAST *asSetRestCatalog() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestSchemaAST: public SetRestAST
{
public:
    int t_schema;
    int t_schema_name;

public:
    SetRestSchemaAST( int t_schema_, int t_schema_name_ )
        : SetRestAST()
        , t_schema(t_schema_)
        , t_schema_name(t_schema_name_)
    {}

    virtual SetRestSchemaAST *asSetRestSchema() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestEncodingAST: public SetRestAST
{
public:
    int t_names;
    int t_encoding;

public:
    SetRestEncodingAST( int t_names_, int t_encoding_ )
        : SetRestAST()
        , t_names(t_names_)
        , t_encoding(t_encoding_)
    {}

    virtual SetRestEncodingAST *asSetRestEncoding() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestRoleAST: public SetRestAST
{
public:
    int t_role;
    int t_role_value;

public:
    SetRestRoleAST( int t_role_, int t_role_value_ )
        : SetRestAST()
        , t_role(t_role_)
        , t_role_value(t_role_value_)
    {}

    virtual SetRestRoleAST *asSetRestRole() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestSessionAuthAST: public SetRestAST
{
public:
    int t_session;
    int t_authorixation;
    int t_authValue;

public:
    SetRestSessionAuthAST( int t_session_, int t_authorixation_, int t_authValue_ )
        : SetRestAST()
        , t_session(t_session_)
        , t_authorixation(t_authorixation_)
        , t_authValue(t_authValue_)
    {}

    virtual SetRestSessionAuthAST *asSetRestSessionAuth() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestXmlOptionAST: public SetRestAST
{
public:
    int t_xml;
    int t_option;
    int t_value;

public:
    SetRestXmlOptionAST( int t_xml_, int t_option_, int t_value_ )
        : SetRestAST()
        , t_xml(t_xml_)
        , t_option(t_option_)
        , t_value(t_value_)
    {}

    virtual SetRestXmlOptionAST *asSetRestXmlOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestTransactioSnapshotAST: public SetRestAST
{
public:
    int t_transaction;
    int t_snapshot;
    int t_value;

public:
    SetRestTransactioSnapshotAST( int t_transaction_, int t_snapshot_, int t_value_ )
        : SetRestAST()
        , t_transaction(t_transaction_)
        , t_snapshot(t_snapshot_)
        , t_value(t_value_)
    {}

    virtual SetRestTransactioSnapshotAST *asSetRestTransactioSnapshot() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableShowStmtAST: public StatementAST
{
public:
    VariableShowStmtAST() : StatementAST() {}
    virtual VariableShowStmtAST *asVariableShowStmt() { return this; }
};

class VarShowStmtAST: public VariableShowStmtAST
{
public:
    int t_show;
    NameListAST *varName;

public:
    VarShowStmtAST( int t_show_, NameListAST *varName_ )
        : VariableShowStmtAST()
        , t_show(t_show_)
        , varName(finish(varName_))
    {}

    virtual VarShowStmtAST *asVarShowStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TimeZoneShowStmtAST: public VariableShowStmtAST
{
public:
    int t_show;
    int t_time;
    int t_zone;

public:
    TimeZoneShowStmtAST( int t_show_, int t_time_, int t_zone_ )
        : VariableShowStmtAST()
        , t_show(t_show_)
        , t_time(t_time_)
        , t_zone(t_zone_)
    {}

    virtual TimeZoneShowStmtAST *asTimeZoneShowStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IsoLevelShowStmtAST: public VariableShowStmtAST
{
public:
    int t_show;
    int t_transaction;
    int t_isolation;
    int t_level;

public:
    IsoLevelShowStmtAST( int t_show_, int t_transaction_, int t_isolation_, int t_level_ )
        : VariableShowStmtAST()
        , t_show(t_show_)
        , t_transaction(t_transaction_)
        , t_isolation(t_isolation_)
        , t_level(t_level_)
    {}

    virtual IsoLevelShowStmtAST *asIsoLevelShowStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SessionAuthShowStmtAST: public VariableShowStmtAST
{
public:
    int t_show;
    int t_session;
    int t_authorization;

public:
    SessionAuthShowStmtAST( int t_show_, int t_session_, int t_authorization_ )
        : VariableShowStmtAST()
        , t_show(t_show_)
        , t_session(t_session_)
        , t_authorization(t_authorization_)
    {}

    virtual SessionAuthShowStmtAST *asSessionAuthShowStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AllShowStmtAST: public VariableShowStmtAST
{
public:
    int t_show;
    int t_all;

public:
    AllShowStmtAST( int t_show_, int t_all_ )
        : VariableShowStmtAST()
        , t_show(t_show_)
        , t_all(t_all_)
    {}

    virtual AllShowStmtAST *asAllShowStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableSetStmtAST: public StatementAST
{
public:
    int t_set;
    int t_local_or_session;
    SetRestAST *sets;

public:
    VariableSetStmtAST( int t_set_, int t_local_or_session_, SetRestAST *sets_ )
        : StatementAST()
        , t_set(t_set_)
        , t_local_or_session(t_local_or_session_)
        , sets(sets_)
    {}

    virtual VariableSetStmtAST *asVariableSetStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableResetStmtAST: public StatementAST
{
public:
    VariableResetStmtAST() : StatementAST() {}
    virtual VariableResetStmtAST *asVariableResetStmt() { return this; }
};

class SetRestResetVarNameAST: public VariableResetStmtAST
{
public:
    int t_reset;
    NameListAST *dotted_name;

public:
    SetRestResetVarNameAST( int t_reset_, NameListAST *dotted_name_ )
        : VariableResetStmtAST()
        , t_reset(t_reset_)
        , dotted_name(finish(dotted_name_))
    {}

    virtual SetRestResetVarNameAST *asSetRestResetVarName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestResetTimeZoneAST: public VariableResetStmtAST
{
public:
    int t_reset;
    int t_time;
    int t_zone;

public:
    SetRestResetTimeZoneAST( int t_reset_, int t_time_, int t_zone_ )
        : VariableResetStmtAST()
        , t_reset(t_reset_)
        , t_time(t_time_)
        , t_zone(t_zone_)
    {}

    virtual SetRestResetTimeZoneAST *asSetRestResetTimeZone() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestResetTransLevelAST: public VariableResetStmtAST
{
public:
    int t_reset;
    int t_transaction;
    int t_isolation;
    int t_level;

public:
    SetRestResetTransLevelAST( int t_reset_, int t_transaction_, int t_isolation_, int t_level_ )
        : VariableResetStmtAST()
        , t_reset(t_reset_)
        , t_transaction(t_transaction_)
        , t_isolation(t_isolation_)
        , t_level(t_level_)
    {}

    virtual SetRestResetTransLevelAST *asSetRestResetTransLevel() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestResetAuthorizationAST: public VariableResetStmtAST
{
public:
    int t_reset;
    int t_session;
    int t_authorization;

public:
    SetRestResetAuthorizationAST( int t_reset_, int t_session_, int t_authorization_ )
        : VariableResetStmtAST()
        , t_reset(t_reset_)
        , t_session(t_session_)
        , t_authorization(t_authorization_)
    {}

    virtual SetRestResetAuthorizationAST *asSetRestResetAuthorization() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetRestResetAllAST: public VariableResetStmtAST
{
public:
    int t_all;

public:
    SetRestResetAllAST( int t_all_ )
        : VariableResetStmtAST()
        , t_all(t_all_)
    {}

    virtual SetRestResetAllAST *asSetRestResetAll() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IsoLevelAST: public AST
{
public:
    int t_spec1;
    int t_spec2;

public:
    IsoLevelAST( int t_spec1_, int t_spec2_ )
        : AST()
        , t_spec1(t_spec1_)
        , t_spec2(t_spec2_)
    {}

    virtual IsoLevelAST *asIsoLevel() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TransactionModeAST: public AST
{
public:
    TransactionModeAST() : AST() {}
    virtual TransactionModeAST *asTransactionMode() { return this; }
};

class TransactionModeIsoLevelAST: public TransactionModeAST
{
public:
    int t_isolation;
    int t_level;
    IsoLevelAST *level;

public:
    TransactionModeIsoLevelAST( int t_isolation_, int t_level_, IsoLevelAST *level_ )
        : TransactionModeAST()
        , t_isolation(t_isolation_)
        , t_level(t_level_)
        , level(level_)
    {}

    virtual TransactionModeIsoLevelAST *asTransactionModeIsoLevel() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TransactionModeReadWriteAST: public TransactionModeAST
{
public:
    int t_read;
    int t_only_or_write;

public:
    TransactionModeReadWriteAST( int t_read_, int t_only_or_write_ )
        : TransactionModeAST()
        , t_read(t_read_)
        , t_only_or_write(t_only_or_write_)
    {}

    virtual TransactionModeReadWriteAST *asTransactionModeReadWrite() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TransactionModeDeferAST: public TransactionModeAST
{
public:
    int t_not;
    int t_deferrable;

public:
    TransactionModeDeferAST( int t_not_, int t_deferrable_ )
        : TransactionModeAST()
        , t_not(t_not_)
        , t_deferrable(t_deferrable_)
    {}

    virtual TransactionModeDeferAST *asTransactionModeDefer() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntervalSecondAST: public AST
{
public:
    int t_second;
    int t_lparen;
    int t_value;
    int t_rparen;

public:
    IntervalSecondAST( int t_second_, int t_lparen_, int t_value_, int t_rparen_ )
        : AST()
        , t_second(t_second_)
        , t_lparen(t_lparen_)
        , t_value(t_value_)
        , t_rparen(t_rparen_)
    {}

    virtual IntervalSecondAST *asIntervalSecond() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntervalAST: public AST
{
public:
    IntervalAST() : AST() {}
    virtual IntervalAST *asInterval() { return this; }
};

class IntervalSimpleAST: public IntervalAST
{
public:
    int t_period;

public:
    IntervalSimpleAST( int t_period_ )
        : IntervalAST()
        , t_period(t_period_)
    {}

    virtual IntervalSimpleAST *asIntervalSimple() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntervalSimpleToAST: public IntervalAST
{
public:
    int t_period;
    int t_to;
    int t_period_to;

public:
    IntervalSimpleToAST( int t_period_, int t_to_, int t_period_to_ )
        : IntervalAST()
        , t_period(t_period_)
        , t_to(t_to_)
        , t_period_to(t_period_to_)
    {}

    virtual IntervalSimpleToAST *asIntervalSimpleTo() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntervalSimpleToSecondAST: public IntervalAST
{
public:
    int t_period;
    int t_to;
    IntervalSecondAST *second;

public:
    IntervalSimpleToSecondAST( int t_period_, int t_to_, IntervalSecondAST *second_ )
        : IntervalAST()
        , t_period(t_period_)
        , t_to(t_to_)
        , second(second_)
    {}

    virtual IntervalSimpleToSecondAST *asIntervalSimpleToSecond() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VarValueAST: public AST
{
public:
    int t_value;

public:
    VarValueAST( int t_value_ )
        : AST()
        , t_value(t_value_)
    {}

    virtual VarValueAST *asVarValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ZoneValueAST: public AST
{
public:
    ZoneValueAST() : AST() {}
    virtual ZoneValueAST *asZoneValue() { return this; }
};

class ZoneValueSimpleAST: public ZoneValueAST
{
public:
    int t_value;

public:
    ZoneValueSimpleAST( int t_value_ )
        : ZoneValueAST()
        , t_value(t_value_)
    {}

    virtual ZoneValueSimpleAST *asZoneValueSimple() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ZoneValueIntervalAST: public ZoneValueAST
{
public:
    int t_interval;
    int t_lparen;
    int t_val;
    int t_rparen;
    int t_sval;
    IntervalAST *interval;

public:
    ZoneValueIntervalAST( int t_interval_, int t_lparen_, int t_val_, int t_rparen_, int t_sval_, IntervalAST *interval_ )
        : ZoneValueAST()
        , t_interval(t_interval_)
        , t_lparen(t_lparen_)
        , t_val(t_val_)
        , t_rparen(t_rparen_)
        , t_sval(t_sval_)
        , interval(interval_)
    {}

    virtual ZoneValueIntervalAST *asZoneValueInterval() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropRoleStmtAST: public DdlStatementAST
{
public:
    int t_drop;
    int t_role;
    int t_if;
    int t_exists;
    RoleIdListAST *roles;

public:
    DropRoleStmtAST( int t_drop_, int t_role_, int t_if_, int t_exists_, RoleIdListAST *roles_ )
        : DdlStatementAST()
        , t_drop(t_drop_)
        , t_role(t_role_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , roles(finish(roles_))
    {}

    virtual DropRoleStmtAST *asDropRoleStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateGroupStmtAST: public CreateStatementAST
{
public:
    int t_create;
    int t_group;
    RoleIdAST *groupIdent;
    int t_with;
    OptRoleListAST *options;

public:
    CreateGroupStmtAST( int t_create_, int t_group_, RoleIdAST *groupIdent_, int t_with_, OptRoleListAST *options_ )
        : CreateStatementAST()
        , t_create(t_create_)
        , t_group(t_group_)
        , groupIdent(groupIdent_)
        , t_with(t_with_)
        , options(finish(options_))
    {}

    virtual CreateGroupStmtAST *asCreateGroupStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterGroupStmtAST: public DdlStatementAST
{
public:
    int t_alter;
    int t_group;
    RoleIdAST *groupIdent;
    int t_add_or_drop;
    int t_user;
    RoleIdListAST *roles;

public:
    AlterGroupStmtAST( int t_alter_, int t_group_, RoleIdAST *groupIdent_, int t_add_or_drop_, int t_user_, RoleIdListAST *roles_ )
        : DdlStatementAST()
        , t_alter(t_alter_)
        , t_group(t_group_)
        , groupIdent(groupIdent_)
        , t_add_or_drop(t_add_or_drop_)
        , t_user(t_user_)
        , roles(finish(roles_))
    {}

    virtual AlterGroupStmtAST *asAlterGroupStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DiscardStmtAST: public DdlStatementAST
{
public:
    int t_discard;
    int t_all_or_tem_or_plans;

public:
    DiscardStmtAST( int t_discard_, int t_all_or_tem_or_plans_ )
        : DdlStatementAST()
        , t_discard(t_discard_)
        , t_all_or_tem_or_plans(t_all_or_tem_or_plans_)
    {}

    virtual DiscardStmtAST *asDiscardStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropStatementAST: public DdlStatementAST
{
public:
    DropStatementAST() : DdlStatementAST() {}
    virtual DropStatementAST *asDropStatement() { return this; }
};

class TruncateStatementAST: public DdlStatementAST
{
public:
    TruncateStatementAST() : DdlStatementAST() {}
    virtual TruncateStatementAST *asTruncateStatement() { return this; }
};

class AlterStmtAST: public DdlStatementAST
{
public:
    AlterStmtAST() : DdlStatementAST() {}
    virtual AlterStmtAST *asAlterStmt() { return this; }
};

class AlterIndexStmtAST: public AlterStmtAST
{
public:
    int t_alter;
    int t_index;
    int t_if;
    int t_exists;
    IndexIdentifierAST *indexIdent;
    AlterTableCmdListAST *alterCommands;

public:
    AlterIndexStmtAST( int t_alter_, int t_index_, int t_if_, int t_exists_, IndexIdentifierAST *indexIdent_, AlterTableCmdListAST *alterCommands_ )
        : AlterStmtAST()
        , t_alter(t_alter_)
        , t_index(t_index_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , indexIdent(indexIdent_)
        , alterCommands(finish(alterCommands_))
    {}

    virtual AlterIndexStmtAST *asAlterIndexStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterSequenceStmtAST: public AlterStmtAST
{
public:
    int t_alter;
    int t_sequence;
    int t_if;
    int t_exists;
    SequenceIdentifierAST *indexIdent;
    AlterTableCmdListAST *alterCommands;

public:
    AlterSequenceStmtAST( int t_alter_, int t_sequence_, int t_if_, int t_exists_, SequenceIdentifierAST *indexIdent_, AlterTableCmdListAST *alterCommands_ )
        : AlterStmtAST()
        , t_alter(t_alter_)
        , t_sequence(t_sequence_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , indexIdent(indexIdent_)
        , alterCommands(finish(alterCommands_))
    {}

    virtual AlterSequenceStmtAST *asAlterSequenceStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableStmtAST: public AlterStmtAST
{
public:
    int t_alter;
    int t_table;
    int t_if;
    int t_exists;
    RelationExprAST *relation;
    AlterTableCmdListAST *alterCommands;

public:
    AlterTableStmtAST( int t_alter_, int t_table_, int t_if_, int t_exists_, RelationExprAST *relation_, AlterTableCmdListAST *alterCommands_ )
        : AlterStmtAST()
        , t_alter(t_alter_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , relation(relation_)
        , alterCommands(finish(alterCommands_))
    {}

    virtual AlterTableStmtAST *asAlterTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterForeignTableStmtAST: public AlterStmtAST
{
public:
    int t_alter;
    int t_foreign;
    int t_table;
    int t_if;
    int t_exists;
    RelationExprAST *relation;
    AlterTableCmdListAST *alterCommands;

public:
    AlterForeignTableStmtAST( int t_alter_, int t_foreign_, int t_table_, int t_if_, int t_exists_, RelationExprAST *relation_, AlterTableCmdListAST *alterCommands_ )
        : AlterStmtAST()
        , t_alter(t_alter_)
        , t_foreign(t_foreign_)
        , t_table(t_table_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , relation(relation_)
        , alterCommands(finish(alterCommands_))
    {}

    virtual AlterForeignTableStmtAST *asAlterForeignTableStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterViewStmtAST: public AlterStmtAST
{
public:
    int t_alter;
    int t_materialized;
    int t_view;
    int t_if;
    int t_exists;
    TableIdentifierAST *tableIdent;
    AlterTableCmdListAST *alterCommands;

public:
    AlterViewStmtAST( int t_alter_, int t_materialized_, int t_view_, int t_if_, int t_exists_, TableIdentifierAST *tableIdent_, AlterTableCmdListAST *alterCommands_ )
        : AlterStmtAST()
        , t_alter(t_alter_)
        , t_materialized(t_materialized_)
        , t_view(t_view_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , tableIdent(tableIdent_)
        , alterCommands(finish(alterCommands_))
    {}

    virtual AlterViewStmtAST *asAlterViewStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UpdateStmtAST: public DmlStatementAST
{
public:
    WithClauseAST *withClause;
    int t_update;
    RelationAliasExprAST *relalias;
    int t_set;
    SetClauseListAST *sets;
    FromClauseAST *fromClause;
    WhereOrCurrentClauseAST *whereClause;
    ReturningClauseAST *retClause;

public:
    UpdateStmtAST( WithClauseAST *withClause_, int t_update_, RelationAliasExprAST *relalias_, int t_set_, SetClauseListAST *sets_, FromClauseAST *fromClause_, WhereOrCurrentClauseAST *whereClause_, ReturningClauseAST *retClause_ )
        : DmlStatementAST()
        , withClause(withClause_)
        , t_update(t_update_)
        , relalias(relalias_)
        , t_set(t_set_)
        , sets(finish(sets_))
        , fromClause(fromClause_)
        , whereClause(whereClause_)
        , retClause(retClause_)
    {}

    virtual UpdateStmtAST *asUpdateStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetClauseAST: public AST
{
public:
    SetClauseAST() : AST() {}
    virtual SetClauseAST *asSetClause() { return this; }
};

class SingleSetClauseAST: public SetClauseAST
{
public:
    ColumnRefAST *columnRef;
    int t_equal;
    ColumnExpressionAST *value;

public:
    SingleSetClauseAST( ColumnRefAST *columnRef_, int t_equal_, ColumnExpressionAST *value_ )
        : SetClauseAST()
        , columnRef(columnRef_)
        , t_equal(t_equal_)
        , value(value_)
    {}

    virtual SingleSetClauseAST *asSingleSetClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class MultipleSetClauseAST: public SetClauseAST
{
public:
    int t_lparen;
    ColumnRefListAST *columns;
    int t_rparen;
    int t_equal;
    ColumnExprRowAST *values;

public:
    MultipleSetClauseAST( int t_lparen_, ColumnRefListAST *columns_, int t_rparen_, int t_equal_, ColumnExprRowAST *values_ )
        : SetClauseAST()
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , t_equal(t_equal_)
        , values(values_)
    {}

    virtual MultipleSetClauseAST *asMultipleSetClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertStmtAST: public DmlStatementAST
{
public:
    WithClauseAST *withClause;
    int t_insert;
    int t_into;
    TableIdentifierAST *tableIdent;
    InsertRestAST *rest;
    ReturningClauseAST *retClause;

public:
    InsertStmtAST( WithClauseAST *withClause_, int t_insert_, int t_into_, TableIdentifierAST *tableIdent_, InsertRestAST *rest_, ReturningClauseAST *retClause_ )
        : DmlStatementAST()
        , withClause(withClause_)
        , t_insert(t_insert_)
        , t_into(t_into_)
        , tableIdent(tableIdent_)
        , rest(rest_)
        , retClause(retClause_)
    {}

    virtual InsertStmtAST *asInsertStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertRestAST: public AST
{
public:
    InsertRestAST() : AST() {}
    virtual InsertRestAST *asInsertRest() { return this; }
};

class CommonInsertRestAST: public InsertRestAST
{
public:
    int t_lparen;
    ColumnRefListAST *columns;
    int t_rparen;
    SelectStmtAST *content;

public:
    CommonInsertRestAST( int t_lparen_, ColumnRefListAST *columns_, int t_rparen_, SelectStmtAST *content_ )
        : InsertRestAST()
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , content(content_)
    {}

    virtual CommonInsertRestAST *asCommonInsertRest() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefaultValuesInsertRestAST: public InsertRestAST
{
public:
    int t_default;
    int t_values;

public:
    DefaultValuesInsertRestAST( int t_default_, int t_values_ )
        : InsertRestAST()
        , t_default(t_default_)
        , t_values(t_values_)
    {}

    virtual DefaultValuesInsertRestAST *asDefaultValuesInsertRest() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeleteStmtAST: public DmlStatementAST
{
public:
    WithClauseAST *withClause;
    int t_delete;
    int t_from;
    RelationAliasExprAST *relalias;
    DelUsingClauseAST *usingClause;
    WhereOrCurrentClauseAST *whereClause;
    ReturningClauseAST *retClause;

public:
    DeleteStmtAST( WithClauseAST *withClause_, int t_delete_, int t_from_, RelationAliasExprAST *relalias_, DelUsingClauseAST *usingClause_, WhereOrCurrentClauseAST *whereClause_, ReturningClauseAST *retClause_ )
        : DmlStatementAST()
        , withClause(withClause_)
        , t_delete(t_delete_)
        , t_from(t_from_)
        , relalias(relalias_)
        , usingClause(usingClause_)
        , whereClause(whereClause_)
        , retClause(retClause_)
    {}

    virtual DeleteStmtAST *asDeleteStmt() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReturningClauseAST: public AST
{
public:
    int t_returning;
    SelectTargetListAST *results;

public:
    ReturningClauseAST( int t_returning_, SelectTargetListAST *results_ )
        : AST()
        , t_returning(t_returning_)
        , results(finish(results_))
    {}

    virtual ReturningClauseAST *asReturningClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WhereOrCurrentClauseAST: public AST
{
public:
    WhereOrCurrentClauseAST() : AST() {}
    virtual WhereOrCurrentClauseAST *asWhereOrCurrentClause() { return this; }
};

class WhereNoCurrentClauseAST: public WhereOrCurrentClauseAST
{
public:
    int t_where;
    ExpressionAST *expr;

public:
    WhereNoCurrentClauseAST( int t_where_, ExpressionAST *expr_ )
        : WhereOrCurrentClauseAST()
        , t_where(t_where_)
        , expr(expr_)
    {}

    virtual WhereNoCurrentClauseAST *asWhereNoCurrentClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CurrenWhereClauseAST: public WhereOrCurrentClauseAST
{
public:
    int t_where;
    int t_current;
    int t_of;
    NameAST *cursorName;

public:
    CurrenWhereClauseAST( int t_where_, int t_current_, int t_of_, NameAST *cursorName_ )
        : WhereOrCurrentClauseAST()
        , t_where(t_where_)
        , t_current(t_current_)
        , t_of(t_of_)
        , cursorName(cursorName_)
    {}

    virtual CurrenWhereClauseAST *asCurrenWhereClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DelUsingClauseAST: public AST
{
public:
    int t_using;
    TableRefListAST *refs;

public:
    DelUsingClauseAST( int t_using_, TableRefListAST *refs_ )
        : AST()
        , t_using(t_using_)
        , refs(finish(refs_))
    {}

    virtual DelUsingClauseAST *asDelUsingClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RelationAliasExprAST: public AST
{
public:
    RelationExprAST *rel;
    int t_as;
    NameAST *alias;

public:
    RelationAliasExprAST( RelationExprAST *rel_, int t_as_, NameAST *alias_ )
        : AST()
        , rel(rel_)
        , t_as(t_as_)
        , alias(alias_)
    {}

    virtual RelationAliasExprAST *asRelationAliasExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectStmtAST: public DmlStatementAST
{
public:
    SelectStmtAST() : DmlStatementAST() {}
    virtual SelectStmtAST *asSelectStmt() { return this; }
};

class SelectWithParensAST: public SelectStmtAST
{
public:
    int t_lparen;
    SelectStmtAST *statement;
    int t_rparen;

public:
    SelectWithParensAST( int t_lparen_, SelectStmtAST *statement_, int t_rparen_ )
        : SelectStmtAST()
        , t_lparen(t_lparen_)
        , statement(statement_)
        , t_rparen(t_rparen_)
    {}

    virtual SelectWithParensAST *asSelectWithParens() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectNoParensAST: public SelectStmtAST
{
public:
    WithClauseAST *withClause;
    SelectClauseAST *query;
    SortClauseAST *sortClause;
    LockingLimitClauseAST *llClause;

public:
    SelectNoParensAST( WithClauseAST *withClause_, SelectClauseAST *query_, SortClauseAST *sortClause_, LockingLimitClauseAST *llClause_ )
        : SelectStmtAST()
        , withClause(withClause_)
        , query(query_)
        , sortClause(sortClause_)
        , llClause(llClause_)
    {}

    virtual SelectNoParensAST *asSelectNoParens() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WithClauseAST: public AST
{
public:
    int t_with;
    int t_recursive;
    CommonTableExprListAST *withQueries;

public:
    WithClauseAST( int t_with_, int t_recursive_, CommonTableExprListAST *withQueries_ )
        : AST()
        , t_with(t_with_)
        , t_recursive(t_recursive_)
        , withQueries(finish(withQueries_))
    {}

    virtual WithClauseAST *asWithClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonTableExprAST: public AST
{
public:
    NameAST *relName;
    CompoundColumnNamesAST *relColumns;
    int t_as;
    int t_lparen;
    DmlStatementAST *statement;
    int t_rparen;

public:
    CommonTableExprAST( NameAST *relName_, CompoundColumnNamesAST *relColumns_, int t_as_, int t_lparen_, DmlStatementAST *statement_, int t_rparen_ )
        : AST()
        , relName(relName_)
        , relColumns(relColumns_)
        , t_as(t_as_)
        , t_lparen(t_lparen_)
        , statement(statement_)
        , t_rparen(t_rparen_)
    {}

    virtual CommonTableExprAST *asCommonTableExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundColumnNamesAST: public AST
{
public:
    int t_lparen;
    NameListAST *names;
    int t_rparen;

public:
    CompoundColumnNamesAST( int t_lparen_, NameListAST *names_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , names(finish(names_))
        , t_rparen(t_rparen_)
    {}

    virtual CompoundColumnNamesAST *asCompoundColumnNames() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SortClauseAST: public AST
{
public:
    int t_sort;
    int t_by;
    OrderExprListAST *sortList;

public:
    SortClauseAST( int t_sort_, int t_by_, OrderExprListAST *sortList_ )
        : AST()
        , t_sort(t_sort_)
        , t_by(t_by_)
        , sortList(finish(sortList_))
    {}

    virtual SortClauseAST *asSortClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OrderExprAST: public AST
{
public:
    OrderExprAST() : AST() {}
    virtual OrderExprAST *asOrderExpr() { return this; }
};

class UsingOrderExprAST: public OrderExprAST
{
public:
    ExpressionAST *expr;
    int t_using;
    QualifiedOperatorAST *qualOp;
    int t_nulls_order;

public:
    UsingOrderExprAST( ExpressionAST *expr_, int t_using_, QualifiedOperatorAST *qualOp_, int t_nulls_order_ )
        : OrderExprAST()
        , expr(expr_)
        , t_using(t_using_)
        , qualOp(qualOp_)
        , t_nulls_order(t_nulls_order_)
    {}

    virtual UsingOrderExprAST *asUsingOrderExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonOrderExprAST: public OrderExprAST
{
public:
    ExpressionAST *expr;
    int t_asc_or_desc;
    int t_nulls_order;

public:
    CommonOrderExprAST( ExpressionAST *expr_, int t_asc_or_desc_, int t_nulls_order_ )
        : OrderExprAST()
        , expr(expr_)
        , t_asc_or_desc(t_asc_or_desc_)
        , t_nulls_order(t_nulls_order_)
    {}

    virtual CommonOrderExprAST *asCommonOrderExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockingLimitClauseAST: public AST
{
public:
    LockingLimitClauseAST() : AST() {}
    virtual LockingLimitClauseAST *asLockingLimitClause() { return this; }
};

class LockingFirstLimitClauseAST: public LockingLimitClauseAST
{
public:
    ForLockingClauseAST *locking;
    LimitClauseAST *limit;

public:
    LockingFirstLimitClauseAST( ForLockingClauseAST *locking_, LimitClauseAST *limit_ )
        : LockingLimitClauseAST()
        , locking(locking_)
        , limit(limit_)
    {}

    virtual LockingFirstLimitClauseAST *asLockingFirstLimitClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockingLastLimitClauseAST: public LockingLimitClauseAST
{
public:
    LimitClauseAST *limit;
    ForLockingClauseAST *locking;

public:
    LockingLastLimitClauseAST( LimitClauseAST *limit_, ForLockingClauseAST *locking_ )
        : LockingLimitClauseAST()
        , limit(limit_)
        , locking(locking_)
    {}

    virtual LockingLastLimitClauseAST *asLockingLastLimitClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LimitClauseAST: public AST
{
public:
    LimitClauseAST() : AST() {}
    virtual LimitClauseAST *asLimitClause() { return this; }
};

class LimitOffsetClauseAST: public LimitClauseAST
{
public:
    LimitAST *limit;
    OffsetAST *offset;

public:
    LimitOffsetClauseAST( LimitAST *limit_, OffsetAST *offset_ )
        : LimitClauseAST()
        , limit(limit_)
        , offset(offset_)
    {}

    virtual LimitOffsetClauseAST *asLimitOffsetClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OffsetLimitClauseAST: public LimitClauseAST
{
public:
    OffsetAST *offset;
    LimitAST *limit;

public:
    OffsetLimitClauseAST( OffsetAST *offset_, LimitAST *limit_ )
        : LimitClauseAST()
        , offset(offset_)
        , limit(limit_)
    {}

    virtual OffsetLimitClauseAST *asOffsetLimitClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OffsetAST: public AST
{
public:
    int t_offset;
    ExpressionAST *expr;
    int t_row_or_rows;

public:
    OffsetAST( int t_offset_, ExpressionAST *expr_, int t_row_or_rows_ )
        : AST()
        , t_offset(t_offset_)
        , expr(expr_)
        , t_row_or_rows(t_row_or_rows_)
    {}

    virtual OffsetAST *asOffset() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LimitAST: public AST
{
public:
    LimitAST() : AST() {}
    virtual LimitAST *asLimit() { return this; }
};

class CommonLimitAST: public LimitAST
{
public:
    int t_limit;
    LimitValueAST *value;
    int t_comma;
    ExpressionAST *offset;

public:
    CommonLimitAST( int t_limit_, LimitValueAST *value_, int t_comma_, ExpressionAST *offset_ )
        : LimitAST()
        , t_limit(t_limit_)
        , value(value_)
        , t_comma(t_comma_)
        , offset(offset_)
    {}

    virtual CommonLimitAST *asCommonLimit() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FetchLimitAST: public LimitAST
{
public:
    int t_fetch;
    int t_first_or_next;
    FetchFirstValueAST *first;
    int t_row_or_rows;
    int t_only;

public:
    FetchLimitAST( int t_fetch_, int t_first_or_next_, FetchFirstValueAST *first_, int t_row_or_rows_, int t_only_ )
        : LimitAST()
        , t_fetch(t_fetch_)
        , t_first_or_next(t_first_or_next_)
        , first(first_)
        , t_row_or_rows(t_row_or_rows_)
        , t_only(t_only_)
    {}

    virtual FetchLimitAST *asFetchLimit() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FetchFirstValueAST: public AST
{
public:
    FetchFirstValueAST() : AST() {}
    virtual FetchFirstValueAST *asFetchFirstValue() { return this; }
};

class ConstFetchFirstValueAST: public FetchFirstValueAST
{
public:
    int t_value;

public:
    ConstFetchFirstValueAST( int t_value_ )
        : FetchFirstValueAST()
        , t_value(t_value_)
    {}

    virtual ConstFetchFirstValueAST *asConstFetchFirstValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundFetchFirstValueAST: public FetchFirstValueAST
{
public:
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;

public:
    CompoundFetchFirstValueAST( int t_lparen_, ExpressionAST *expr_, int t_rparen_ )
        : FetchFirstValueAST()
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
    {}

    virtual CompoundFetchFirstValueAST *asCompoundFetchFirstValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LimitValueAST: public AST
{
public:
    LimitValueAST() : AST() {}
    virtual LimitValueAST *asLimitValue() { return this; }
};

class AllLimitValueAST: public LimitValueAST
{
public:
    int t_all;

public:
    AllLimitValueAST( int t_all_ )
        : LimitValueAST()
        , t_all(t_all_)
    {}

    virtual AllLimitValueAST *asAllLimitValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprLimitValueAST: public LimitValueAST
{
public:
    ExpressionAST *expr;

public:
    ExprLimitValueAST( ExpressionAST *expr_ )
        : LimitValueAST()
        , expr(expr_)
    {}

    virtual ExprLimitValueAST *asExprLimitValue() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ForLockingClauseAST: public AST
{
public:
    ForLockingClauseAST() : AST() {}
    virtual ForLockingClauseAST *asForLockingClause() { return this; }
};

class ReadOnlyForLockingClauseAST: public ForLockingClauseAST
{
public:
    int t_for;
    int t_read;
    int t_only;

public:
    ReadOnlyForLockingClauseAST( int t_for_, int t_read_, int t_only_ )
        : ForLockingClauseAST()
        , t_for(t_for_)
        , t_read(t_read_)
        , t_only(t_only_)
    {}

    virtual ReadOnlyForLockingClauseAST *asReadOnlyForLockingClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonForLockingClauseAST: public ForLockingClauseAST
{
public:
    LockingExprListAST *lockings;

public:
    CommonForLockingClauseAST( LockingExprListAST *lockings_ )
        : ForLockingClauseAST()
        , lockings(finish(lockings_))
    {}

    virtual CommonForLockingClauseAST *asCommonForLockingClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockingExprAST: public AST
{
public:
    LockingStrengthAST *strentgh;
    LockingRelsAST *relations;
    int t_nowait;

public:
    LockingExprAST( LockingStrengthAST *strentgh_, LockingRelsAST *relations_, int t_nowait_ )
        : AST()
        , strentgh(strentgh_)
        , relations(relations_)
        , t_nowait(t_nowait_)
    {}

    virtual LockingExprAST *asLockingExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockingStrengthAST: public AST
{
public:
    int t_for;
    int t_spec1;
    int t_spec2;
    int t_spec3;

public:
    LockingStrengthAST( int t_for_, int t_spec1_, int t_spec2_, int t_spec3_ )
        : AST()
        , t_for(t_for_)
        , t_spec1(t_spec1_)
        , t_spec2(t_spec2_)
        , t_spec3(t_spec3_)
    {}

    virtual LockingStrengthAST *asLockingStrength() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockingRelsAST: public AST
{
public:
    int t_of;
    TableIdentifierListAST *lockRels;

public:
    LockingRelsAST( int t_of_, TableIdentifierListAST *lockRels_ )
        : AST()
        , t_of(t_of_)
        , lockRels(finish(lockRels_))
    {}

    virtual LockingRelsAST *asLockingRels() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectClauseAST: public AST
{
public:
    SelectClauseAST() : AST() {}
    virtual SelectClauseAST *asSelectClause() { return this; }
};

class CompoundSelectClauseAST: public SelectClauseAST
{
public:
    SelectWithParensAST *queryWithParens;

public:
    CompoundSelectClauseAST( SelectWithParensAST *queryWithParens_ )
        : SelectClauseAST()
        , queryWithParens(queryWithParens_)
    {}

    virtual CompoundSelectClauseAST *asCompoundSelectClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CoreSimpleSelectAST: public SelectClauseAST
{
public:
    SimpleSelectAST *queryCore;

public:
    CoreSimpleSelectAST( SimpleSelectAST *queryCore_ )
        : SelectClauseAST()
        , queryCore(queryCore_)
    {}

    virtual CoreSimpleSelectAST *asCoreSimpleSelect() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleSelectAST: public AST
{
public:
    SimpleSelectAST() : AST() {}
    virtual SimpleSelectAST *asSimpleSelect() { return this; }
};

class ValuesSimpleSelectAST: public SimpleSelectAST
{
public:
    ValuesClauseAST *values;

public:
    ValuesSimpleSelectAST( ValuesClauseAST *values_ )
        : SimpleSelectAST()
        , values(values_)
    {}

    virtual ValuesSimpleSelectAST *asValuesSimpleSelect() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableSimpleSelectAST: public SimpleSelectAST
{
public:
    int t_table;
    RelationExprAST *relation;

public:
    TableSimpleSelectAST( int t_table_, RelationExprAST *relation_ )
        : SimpleSelectAST()
        , t_table(t_table_)
        , relation(relation_)
    {}

    virtual TableSimpleSelectAST *asTableSimpleSelect() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompositeSimpleSelectAST: public SimpleSelectAST
{
public:
    SelectClauseAST *lQuery;
    int t_compose_op;
    int t_compose_spec;
    SelectClauseAST *rQuery;

public:
    CompositeSimpleSelectAST( SelectClauseAST *lQuery_, int t_compose_op_, int t_compose_spec_, SelectClauseAST *rQuery_ )
        : SimpleSelectAST()
        , lQuery(lQuery_)
        , t_compose_op(t_compose_op_)
        , t_compose_spec(t_compose_spec_)
        , rQuery(rQuery_)
    {}

    virtual CompositeSimpleSelectAST *asCompositeSimpleSelect() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCoreAST: public SimpleSelectAST
{
public:
    int t_select;
    DistinctClauseAST *distinctSpec;
    SelectTargetListAST *targets;
    IntoClauseAST *intoClause;
    FromClauseAST *fromClause;
    WhereClauseAST *whereClause;
    GroupByClauseAST *groupByClause;
    HavingClauseAST *havingClause;
    WindowClauseAST *windowClause;

public:
    SelectCoreAST( int t_select_, DistinctClauseAST *distinctSpec_, SelectTargetListAST *targets_, IntoClauseAST *intoClause_, FromClauseAST *fromClause_, WhereClauseAST *whereClause_, GroupByClauseAST *groupByClause_, HavingClauseAST *havingClause_, WindowClauseAST *windowClause_ )
        : SimpleSelectAST()
        , t_select(t_select_)
        , distinctSpec(distinctSpec_)
        , targets(finish(targets_))
        , intoClause(intoClause_)
        , fromClause(fromClause_)
        , whereClause(whereClause_)
        , groupByClause(groupByClause_)
        , havingClause(havingClause_)
        , windowClause(windowClause_)
    {}

    virtual SelectCoreAST *asSelectCore() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WindowClauseAST: public AST
{
public:
    int t_window;
    WindowDefinitionListAST *definitions;

public:
    WindowClauseAST( int t_window_, WindowDefinitionListAST *definitions_ )
        : AST()
        , t_window(t_window_)
        , definitions(finish(definitions_))
    {}

    virtual WindowClauseAST *asWindowClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WindowDefinitionAST: public AST
{
public:
    NameAST *windowName;
    int t_as;
    WindowSpecificationAST *specification;

public:
    WindowDefinitionAST( NameAST *windowName_, int t_as_, WindowSpecificationAST *specification_ )
        : AST()
        , windowName(windowName_)
        , t_as(t_as_)
        , specification(specification_)
    {}

    virtual WindowDefinitionAST *asWindowDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class HavingClauseAST: public AST
{
public:
    int t_having;
    ExpressionAST *expr;

public:
    HavingClauseAST( int t_having_, ExpressionAST *expr_ )
        : AST()
        , t_having(t_having_)
        , expr(expr_)
    {}

    virtual HavingClauseAST *asHavingClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GroupByClauseAST: public AST
{
public:
    int t_group;
    int t_by;
    ExpressionListAST *groupings;

public:
    GroupByClauseAST( int t_group_, int t_by_, ExpressionListAST *groupings_ )
        : AST()
        , t_group(t_group_)
        , t_by(t_by_)
        , groupings(finish(groupings_))
    {}

    virtual GroupByClauseAST *asGroupByClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WhereClauseAST: public AST
{
public:
    int t_where;
    ExpressionAST *expr;

public:
    WhereClauseAST( int t_where_, ExpressionAST *expr_ )
        : AST()
        , t_where(t_where_)
        , expr(expr_)
    {}

    virtual WhereClauseAST *asWhereClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FromClauseAST: public AST
{
public:
    int t_from;
    TableRefListAST *refs;

public:
    FromClauseAST( int t_from_, TableRefListAST *refs_ )
        : AST()
        , t_from(t_from_)
        , refs(finish(refs_))
    {}

    virtual FromClauseAST *asFromClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableRefAST: public AST
{
public:
    TableRefAST() : AST() {}
    virtual TableRefAST *asTableRef() { return this; }
};

class JoinedTableRefAST: public TableRefAST
{
public:
    int t_lparen;
    JoinedTableAST *joined;
    int t_rparen;

public:
    JoinedTableRefAST( int t_lparen_, JoinedTableAST *joined_, int t_rparen_ )
        : TableRefAST()
        , t_lparen(t_lparen_)
        , joined(joined_)
        , t_rparen(t_rparen_)
    {}

    virtual JoinedTableRefAST *asJoinedTableRef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinedTableAST: public AST
{
public:
    JoinedTableAST() : AST() {}
    virtual JoinedTableAST *asJoinedTable() { return this; }
};

class CommonJoinedTableAST: public JoinedTableAST
{
public:
    TableRefAST *lTableRef;
    int t_natural;
    JoinTypeAST *joinType;
    int t_join;
    TableRefAST *rTableRef;
    JoinConditionAST *joinCondition;

public:
    CommonJoinedTableAST( TableRefAST *lTableRef_, int t_natural_, JoinTypeAST *joinType_, int t_join_, TableRefAST *rTableRef_, JoinConditionAST *joinCondition_ )
        : JoinedTableAST()
        , lTableRef(lTableRef_)
        , t_natural(t_natural_)
        , joinType(joinType_)
        , t_join(t_join_)
        , rTableRef(rTableRef_)
        , joinCondition(joinCondition_)
    {}

    virtual CommonJoinedTableAST *asCommonJoinedTable() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinTypeAST: public AST
{
public:
    int t_full_or_left_or_right;
    int t_inner_or_outer;

public:
    JoinTypeAST( int t_full_or_left_or_right_, int t_inner_or_outer_ )
        : AST()
        , t_full_or_left_or_right(t_full_or_left_or_right_)
        , t_inner_or_outer(t_inner_or_outer_)
    {}

    virtual JoinTypeAST *asJoinType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinConditionAST: public AST
{
public:
    JoinConditionAST() : AST() {}
    virtual JoinConditionAST *asJoinCondition() { return this; }
};

class OnJoinConditionAST: public JoinConditionAST
{
public:
    int t_on;
    ExpressionAST *expr;

public:
    OnJoinConditionAST( int t_on_, ExpressionAST *expr_ )
        : JoinConditionAST()
        , t_on(t_on_)
        , expr(expr_)
    {}

    virtual OnJoinConditionAST *asOnJoinCondition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingJoinConditionAST: public JoinConditionAST
{
public:
    int t_using;
    int t_lparen;
    NameListAST *columns;
    int t_rparen;

public:
    UsingJoinConditionAST( int t_using_, int t_lparen_, NameListAST *columns_, int t_rparen_ )
        : JoinConditionAST()
        , t_using(t_using_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
    {}

    virtual UsingJoinConditionAST *asUsingJoinCondition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CrossJoinedTableAST: public JoinedTableAST
{
public:
    TableRefAST *lTableRef;
    int t_cross;
    int t_join;
    TableRefAST *rTableRef;

public:
    CrossJoinedTableAST( TableRefAST *lTableRef_, int t_cross_, int t_join_, TableRefAST *rTableRef_ )
        : JoinedTableAST()
        , lTableRef(lTableRef_)
        , t_cross(t_cross_)
        , t_join(t_join_)
        , rTableRef(rTableRef_)
    {}

    virtual CrossJoinedTableAST *asCrossJoinedTable() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundJoinedTableAST: public JoinedTableAST
{
public:
    int t_lparen;
    JoinedTableAST *joinedTable;
    int t_rparen;

public:
    CompoundJoinedTableAST( int t_lparen_, JoinedTableAST *joinedTable_, int t_rparen_ )
        : JoinedTableAST()
        , t_lparen(t_lparen_)
        , joinedTable(joinedTable_)
        , t_rparen(t_rparen_)
    {}

    virtual CompoundJoinedTableAST *asCompoundJoinedTable() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundTableRefAST: public TableRefAST
{
public:
    int t_lateral;
    SelectWithParensAST *query;
    TableAliasClauseAST *tableAlias;

public:
    CompoundTableRefAST( int t_lateral_, SelectWithParensAST *query_, TableAliasClauseAST *tableAlias_ )
        : TableRefAST()
        , t_lateral(t_lateral_)
        , query(query_)
        , tableAlias(tableAlias_)
    {}

    virtual CompoundTableRefAST *asCompoundTableRef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncionTableRefAST: public TableRefAST
{
public:
    int t_lateral;
    FunctionExpressionAST *func;
    FuncAliasClauseAST *alias;

public:
    FuncionTableRefAST( int t_lateral_, FunctionExpressionAST *func_, FuncAliasClauseAST *alias_ )
        : TableRefAST()
        , t_lateral(t_lateral_)
        , func(func_)
        , alias(alias_)
    {}

    virtual FuncionTableRefAST *asFuncionTableRef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncAliasClauseAST: public AST
{
public:
    FuncAliasClauseAST() : AST() {}
    virtual FuncAliasClauseAST *asFuncAliasClause() { return this; }
};

class FuncFuncAliasClauseAST: public FuncAliasClauseAST
{
public:
    int t_as;
    NameAST *aliasName;
    int t_lparen;
    TableFuncElementListAST *elements;
    int t_rparen;

public:
    FuncFuncAliasClauseAST( int t_as_, NameAST *aliasName_, int t_lparen_, TableFuncElementListAST *elements_, int t_rparen_ )
        : FuncAliasClauseAST()
        , t_as(t_as_)
        , aliasName(aliasName_)
        , t_lparen(t_lparen_)
        , elements(finish(elements_))
        , t_rparen(t_rparen_)
    {}

    virtual FuncFuncAliasClauseAST *asFuncFuncAliasClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableFuncAliasClauseAST: public FuncAliasClauseAST
{
public:
    TableAliasClauseAST *tableAlias;

public:
    TableFuncAliasClauseAST( TableAliasClauseAST *tableAlias_ )
        : FuncAliasClauseAST()
        , tableAlias(tableAlias_)
    {}

    virtual TableFuncAliasClauseAST *asTableFuncAliasClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableFuncElementAST: public AST
{
public:
    NameAST *columnName;
    TypeAST *columnType;
    CollateClauseAST *collate;

public:
    TableFuncElementAST( NameAST *columnName_, TypeAST *columnType_, CollateClauseAST *collate_ )
        : AST()
        , columnName(columnName_)
        , columnType(columnType_)
        , collate(collate_)
    {}

    virtual TableFuncElementAST *asTableFuncElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollateClauseAST: public AST
{
public:
    int t_collate;
    NameAST *collateName;

public:
    CollateClauseAST( int t_collate_, NameAST *collateName_ )
        : AST()
        , t_collate(t_collate_)
        , collateName(collateName_)
    {}

    virtual CollateClauseAST *asCollateClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionExpressionAST: public AST
{
public:
    FunctionExpressionAST() : AST() {}
    virtual FunctionExpressionAST *asFunctionExpression() { return this; }
};

class XmlserializeFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlserialize;
    int t_lparen;
    int t_document_or_content;
    ExpressionAST *expr;
    int t_as;
    SimpleTypenameAST *type;
    int t_rparen;

public:
    XmlserializeFunctionExprAST( int t_xmlserialize_, int t_lparen_, int t_document_or_content_, ExpressionAST *expr_, int t_as_, SimpleTypenameAST *type_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlserialize(t_xmlserialize_)
        , t_lparen(t_lparen_)
        , t_document_or_content(t_document_or_content_)
        , expr(expr_)
        , t_as(t_as_)
        , type(type_)
        , t_rparen(t_rparen_)
    {}

    virtual XmlserializeFunctionExprAST *asXmlserializeFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlrootFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlroot;
    int t_lparen;
    ExpressionAST *expr;
    int t_comma;
    XmlRootVersionAST *version;
    XmlRootStandaloneAST *standalone;
    int t_rparen;

public:
    XmlrootFunctionExprAST( int t_xmlroot_, int t_lparen_, ExpressionAST *expr_, int t_comma_, XmlRootVersionAST *version_, XmlRootStandaloneAST *standalone_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlroot(t_xmlroot_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_comma(t_comma_)
        , version(version_)
        , standalone(standalone_)
        , t_rparen(t_rparen_)
    {}

    virtual XmlrootFunctionExprAST *asXmlrootFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlRootVersionAST: public AST
{
public:
    XmlRootVersionAST() : AST() {}
    virtual XmlRootVersionAST *asXmlRootVersion() { return this; }
};

class ExprXmlRootVersionAST: public XmlRootVersionAST
{
public:
    int t_version;
    ExpressionAST *expr;

public:
    ExprXmlRootVersionAST( int t_version_, ExpressionAST *expr_ )
        : XmlRootVersionAST()
        , t_version(t_version_)
        , expr(expr_)
    {}

    virtual ExprXmlRootVersionAST *asExprXmlRootVersion() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoValueXmlRootVersionAST: public XmlRootVersionAST
{
public:
    int t_version;
    int t_no;
    int t_value;

public:
    NoValueXmlRootVersionAST( int t_version_, int t_no_, int t_value_ )
        : XmlRootVersionAST()
        , t_version(t_version_)
        , t_no(t_no_)
        , t_value(t_value_)
    {}

    virtual NoValueXmlRootVersionAST *asNoValueXmlRootVersion() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlRootStandaloneAST: public AST
{
public:
    int t_comma;
    int t_standalone;
    int t_yes_or_no;
    int t_value;

public:
    XmlRootStandaloneAST( int t_comma_, int t_standalone_, int t_yes_or_no_, int t_value_ )
        : AST()
        , t_comma(t_comma_)
        , t_standalone(t_standalone_)
        , t_yes_or_no(t_yes_or_no_)
        , t_value(t_value_)
    {}

    virtual XmlRootStandaloneAST *asXmlRootStandalone() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlpiFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlpi;
    int t_lparen;
    int t_name;
    NameAST *label;
    int t_comma;
    ExpressionAST *expr;
    int t_rparen;

public:
    XmlpiFunctionExprAST( int t_xmlpi_, int t_lparen_, int t_name_, NameAST *label_, int t_comma_, ExpressionAST *expr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlpi(t_xmlpi_)
        , t_lparen(t_lparen_)
        , t_name(t_name_)
        , label(label_)
        , t_comma(t_comma_)
        , expr(expr_)
        , t_rparen(t_rparen_)
    {}

    virtual XmlpiFunctionExprAST *asXmlpiFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlparseFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlparse;
    int t_lparen;
    int t_document_or_content;
    ExpressionAST *expr;
    XmlWhitespaceOptionAST *wsOpt;
    int t_rparen;

public:
    XmlparseFunctionExprAST( int t_xmlparse_, int t_lparen_, int t_document_or_content_, ExpressionAST *expr_, XmlWhitespaceOptionAST *wsOpt_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlparse(t_xmlparse_)
        , t_lparen(t_lparen_)
        , t_document_or_content(t_document_or_content_)
        , expr(expr_)
        , wsOpt(wsOpt_)
        , t_rparen(t_rparen_)
    {}

    virtual XmlparseFunctionExprAST *asXmlparseFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlWhitespaceOptionAST: public AST
{
public:
    int t_preserve_or_strip;
    int t_whitespace;

public:
    XmlWhitespaceOptionAST( int t_preserve_or_strip_, int t_whitespace_ )
        : AST()
        , t_preserve_or_strip(t_preserve_or_strip_)
        , t_whitespace(t_whitespace_)
    {}

    virtual XmlWhitespaceOptionAST *asXmlWhitespaceOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlforestFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlforest;
    int t_lparen;
    XmlAttributeListAST *attrs;
    int t_rparen;

public:
    XmlforestFunctionExprAST( int t_xmlforest_, int t_lparen_, XmlAttributeListAST *attrs_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlforest(t_xmlforest_)
        , t_lparen(t_lparen_)
        , attrs(finish(attrs_))
        , t_rparen(t_rparen_)
    {}

    virtual XmlforestFunctionExprAST *asXmlforestFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlexistsFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlexists;
    int t_lparen;
    ExpressionAST *expr;
    XmlExistsArgumentAST *arg;
    int t_rparen;

public:
    XmlexistsFunctionExprAST( int t_xmlexists_, int t_lparen_, ExpressionAST *expr_, XmlExistsArgumentAST *arg_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlexists(t_xmlexists_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , arg(arg_)
        , t_rparen(t_rparen_)
    {}

    virtual XmlexistsFunctionExprAST *asXmlexistsFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlExistsArgumentAST: public AST
{
public:
    int t_passing;
    int t_by1;
    int t_ref1;
    ExpressionAST *expr;
    int t_by2;
    int t_ref2;

public:
    XmlExistsArgumentAST( int t_passing_, int t_by1_, int t_ref1_, ExpressionAST *expr_, int t_by2_, int t_ref2_ )
        : AST()
        , t_passing(t_passing_)
        , t_by1(t_by1_)
        , t_ref1(t_ref1_)
        , expr(expr_)
        , t_by2(t_by2_)
        , t_ref2(t_ref2_)
    {}

    virtual XmlExistsArgumentAST *asXmlExistsArgument() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlelementFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_xmlelement;
    int t_lparen;
    int t_name;
    NameAST *elementName;
    int t_comma_attr;
    XmlAttributesAST *attributes;
    int t_comma;
    ExpressionListAST *values;
    int t_rparen;

public:
    XmlelementFunctionExprAST( int t_xmlelement_, int t_lparen_, int t_name_, NameAST *elementName_, int t_comma_attr_, XmlAttributesAST *attributes_, int t_comma_, ExpressionListAST *values_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_xmlelement(t_xmlelement_)
        , t_lparen(t_lparen_)
        , t_name(t_name_)
        , elementName(elementName_)
        , t_comma_attr(t_comma_attr_)
        , attributes(attributes_)
        , t_comma(t_comma_)
        , values(finish(values_))
        , t_rparen(t_rparen_)
    {}

    virtual XmlelementFunctionExprAST *asXmlelementFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlAttributesAST: public AST
{
public:
    int t_xmlattributes;
    int t_lparen;
    XmlAttributeListAST *attrs;
    int t_rparen;

public:
    XmlAttributesAST( int t_xmlattributes_, int t_lparen_, XmlAttributeListAST *attrs_, int t_rparen_ )
        : AST()
        , t_xmlattributes(t_xmlattributes_)
        , t_lparen(t_lparen_)
        , attrs(finish(attrs_))
        , t_rparen(t_rparen_)
    {}

    virtual XmlAttributesAST *asXmlAttributes() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlAttributeAST: public AST
{
public:
    ExpressionAST *expr;
    int t_as;
    NameAST *alias;

public:
    XmlAttributeAST( ExpressionAST *expr_, int t_as_, NameAST *alias_ )
        : AST()
        , expr(expr_)
        , t_as(t_as_)
        , alias(alias_)
    {}

    virtual XmlAttributeAST *asXmlAttribute() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CoalesceFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_coalesce;
    int t_lparen;
    ExpressionListAST *args;
    int t_rparen;

public:
    CoalesceFunctionExprAST( int t_coalesce_, int t_lparen_, ExpressionListAST *args_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_coalesce(t_coalesce_)
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual CoalesceFunctionExprAST *asCoalesceFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GreatestFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_coalesce;
    int t_lparen;
    ExpressionListAST *args;
    int t_rparen;

public:
    GreatestFunctionExprAST( int t_coalesce_, int t_lparen_, ExpressionListAST *args_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_coalesce(t_coalesce_)
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual GreatestFunctionExprAST *asGreatestFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LeastFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_coalesce;
    int t_lparen;
    ExpressionListAST *args;
    int t_rparen;

public:
    LeastFunctionExprAST( int t_coalesce_, int t_lparen_, ExpressionListAST *args_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_coalesce(t_coalesce_)
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual LeastFunctionExprAST *asLeastFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XmlconcatFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_coalesce;
    int t_lparen;
    ExpressionListAST *args;
    int t_rparen;

public:
    XmlconcatFunctionExprAST( int t_coalesce_, int t_lparen_, ExpressionListAST *args_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_coalesce(t_coalesce_)
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
    {}

    virtual XmlconcatFunctionExprAST *asXmlconcatFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NullifFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_nullif;
    int t_lparen;
    ExpressionAST *expr1;
    int t_comma;
    ExpressionAST *expr2;
    int t_rparen;

public:
    NullifFunctionExprAST( int t_nullif_, int t_lparen_, ExpressionAST *expr1_, int t_comma_, ExpressionAST *expr2_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_nullif(t_nullif_)
        , t_lparen(t_lparen_)
        , expr1(expr1_)
        , t_comma(t_comma_)
        , expr2(expr2_)
        , t_rparen(t_rparen_)
    {}

    virtual NullifFunctionExprAST *asNullifFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TrimFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_trim;
    int t_lparen;
    int t_trim_spec;
    TrimExprAST *trimExpr;
    int t_rparen;

public:
    TrimFunctionExprAST( int t_trim_, int t_lparen_, int t_trim_spec_, TrimExprAST *trimExpr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_trim(t_trim_)
        , t_lparen(t_lparen_)
        , t_trim_spec(t_trim_spec_)
        , trimExpr(trimExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual TrimFunctionExprAST *asTrimFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TrimExprAST: public AST
{
public:
    ExpressionAST *expr;
    int t_from;
    ExpressionListAST *fromExpressions;

public:
    TrimExprAST( ExpressionAST *expr_, int t_from_, ExpressionListAST *fromExpressions_ )
        : AST()
        , expr(expr_)
        , t_from(t_from_)
        , fromExpressions(finish(fromExpressions_))
    {}

    virtual TrimExprAST *asTrimExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TraetFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_traet;
    int t_lparen;
    ExpressionAST *expr;
    int t_as;
    TypeAST *traetType;
    int t_rparen;

public:
    TraetFunctionExprAST( int t_traet_, int t_lparen_, ExpressionAST *expr_, int t_as_, TypeAST *traetType_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_traet(t_traet_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_as(t_as_)
        , traetType(traetType_)
        , t_rparen(t_rparen_)
    {}

    virtual TraetFunctionExprAST *asTraetFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubstringFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_substring;
    int t_lparen;
    SubstringExprAST *substringExpr;
    int t_rparen;

public:
    SubstringFunctionExprAST( int t_substring_, int t_lparen_, SubstringExprAST *substringExpr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_substring(t_substring_)
        , t_lparen(t_lparen_)
        , substringExpr(substringExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual SubstringFunctionExprAST *asSubstringFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubstringExprAST: public AST
{
public:
    SubstringExprAST() : AST() {}
    virtual SubstringExprAST *asSubstringExpr() { return this; }
};

class StandardSubstringExprAST: public SubstringExprAST
{
public:
    ExpressionAST *expr;
    SubstringFromAST *substrFrom;
    SubstringForAST *substrFor;

public:
    StandardSubstringExprAST( ExpressionAST *expr_, SubstringFromAST *substrFrom_, SubstringForAST *substrFor_ )
        : SubstringExprAST()
        , expr(expr_)
        , substrFrom(substrFrom_)
        , substrFor(substrFor_)
    {}

    virtual StandardSubstringExprAST *asStandardSubstringExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ListSubstringExprAST: public SubstringExprAST
{
public:
    ExpressionListAST *args;

public:
    ListSubstringExprAST( ExpressionListAST *args_ )
        : SubstringExprAST()
        , args(finish(args_))
    {}

    virtual ListSubstringExprAST *asListSubstringExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PositionFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_position;
    int t_lparen;
    PositionExprAST *positionExpr;
    int t_rparen;

public:
    PositionFunctionExprAST( int t_position_, int t_lparen_, PositionExprAST *positionExpr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_position(t_position_)
        , t_lparen(t_lparen_)
        , positionExpr(positionExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual PositionFunctionExprAST *asPositionFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OverlayFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_overlay;
    int t_lparen;
    OverlayExprAST *overlayExpr;
    int t_rparen;

public:
    OverlayFunctionExprAST( int t_overlay_, int t_lparen_, OverlayExprAST *overlayExpr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_overlay(t_overlay_)
        , t_lparen(t_lparen_)
        , overlayExpr(overlayExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual OverlayFunctionExprAST *asOverlayFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PositionExprAST: public AST
{
public:
    ExpressionAST *expr;
    int t_in;
    ExpressionAST *inExpr;

public:
    PositionExprAST( ExpressionAST *expr_, int t_in_, ExpressionAST *inExpr_ )
        : AST()
        , expr(expr_)
        , t_in(t_in_)
        , inExpr(inExpr_)
    {}

    virtual PositionExprAST *asPositionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OverlayExprAST: public AST
{
public:
    ExpressionAST *expr;
    OverlayPlacingAST *placing;
    SubstringFromAST *substrFrom;
    SubstringForAST *substrFor;

public:
    OverlayExprAST( ExpressionAST *expr_, OverlayPlacingAST *placing_, SubstringFromAST *substrFrom_, SubstringForAST *substrFor_ )
        : AST()
        , expr(expr_)
        , placing(placing_)
        , substrFrom(substrFrom_)
        , substrFor(substrFor_)
    {}

    virtual OverlayExprAST *asOverlayExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OverlayPlacingAST: public AST
{
public:
    int t_placing;
    ExpressionAST *expr;

public:
    OverlayPlacingAST( int t_placing_, ExpressionAST *expr_ )
        : AST()
        , t_placing(t_placing_)
        , expr(expr_)
    {}

    virtual OverlayPlacingAST *asOverlayPlacing() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExtractFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_extract;
    int t_lparen;
    ExtractExprAST *extractExpr;
    int t_rparen;

public:
    ExtractFunctionExprAST( int t_extract_, int t_lparen_, ExtractExprAST *extractExpr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_extract(t_extract_)
        , t_lparen(t_lparen_)
        , extractExpr(extractExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual ExtractFunctionExprAST *asExtractFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubstringFromAST: public AST
{
public:
    int t_from;
    ExpressionAST *expr;

public:
    SubstringFromAST( int t_from_, ExpressionAST *expr_ )
        : AST()
        , t_from(t_from_)
        , expr(expr_)
    {}

    virtual SubstringFromAST *asSubstringFrom() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubstringForAST: public AST
{
public:
    int t_for;
    ExpressionAST *expr;

public:
    SubstringForAST( int t_for_, ExpressionAST *expr_ )
        : AST()
        , t_for(t_for_)
        , expr(expr_)
    {}

    virtual SubstringForAST *asSubstringFor() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExtractExprAST: public AST
{
public:
    int t_value;
    int t_from;
    ExpressionAST *expr;

public:
    ExtractExprAST( int t_value_, int t_from_, ExpressionAST *expr_ )
        : AST()
        , t_value(t_value_)
        , t_from(t_from_)
        , expr(expr_)
    {}

    virtual ExtractExprAST *asExtractExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CastFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_cast;
    int t_lparen;
    ExpressionAST *castExpr;
    int t_as;
    TypeAST *castType;
    int t_rparen;

public:
    CastFunctionExprAST( int t_cast_, int t_lparen_, ExpressionAST *castExpr_, int t_as_, TypeAST *castType_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_cast(t_cast_)
        , t_lparen(t_lparen_)
        , castExpr(castExpr_)
        , t_as(t_as_)
        , castType(castType_)
        , t_rparen(t_rparen_)
    {}

    virtual CastFunctionExprAST *asCastFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollationFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_collation;
    int t_for;
    int t_lparen;
    ExpressionAST *collationExpr;
    int t_rparen;

public:
    CollationFunctionExprAST( int t_collation_, int t_for_, int t_lparen_, ExpressionAST *collationExpr_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_collation(t_collation_)
        , t_for(t_for_)
        , t_lparen(t_lparen_)
        , collationExpr(collationExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual CollationFunctionExprAST *asCollationFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleFunctionExprAST: public FunctionExpressionAST
{
public:
    int t_builtin_funcname;
    int t_lparen;
    int t_value;
    int t_rparen;

public:
    SimpleFunctionExprAST( int t_builtin_funcname_, int t_lparen_, int t_value_, int t_rparen_ )
        : FunctionExpressionAST()
        , t_builtin_funcname(t_builtin_funcname_)
        , t_lparen(t_lparen_)
        , t_value(t_value_)
        , t_rparen(t_rparen_)
    {}

    virtual SimpleFunctionExprAST *asSimpleFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonFunctionExprAST: public FunctionExpressionAST
{
public:
    FunctionRefAST *funcName;
    int t_lparen;
    int t_all_or_distinct_or_star;
    FunctionArgExprListAST *arguments;
    SortClauseAST *sortClause;
    int t_comma;
    int t_variadic;
    FunctionArgExprAST *variadicArg;
    int t_rparen;
    OverClauseAST *overClause;

public:
    CommonFunctionExprAST( FunctionRefAST *funcName_, int t_lparen_, int t_all_or_distinct_or_star_, FunctionArgExprListAST *arguments_, SortClauseAST *sortClause_, int t_comma_, int t_variadic_, FunctionArgExprAST *variadicArg_, int t_rparen_, OverClauseAST *overClause_ )
        : FunctionExpressionAST()
        , funcName(funcName_)
        , t_lparen(t_lparen_)
        , t_all_or_distinct_or_star(t_all_or_distinct_or_star_)
        , arguments(finish(arguments_))
        , sortClause(sortClause_)
        , t_comma(t_comma_)
        , t_variadic(t_variadic_)
        , variadicArg(variadicArg_)
        , t_rparen(t_rparen_)
        , overClause(overClause_)
    {}

    virtual CommonFunctionExprAST *asCommonFunctionExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionArgExprAST: public AST
{
public:
    NameAST *paramName;
    int t_assign;
    ExpressionAST *argExpr;

public:
    FunctionArgExprAST( NameAST *paramName_, int t_assign_, ExpressionAST *argExpr_ )
        : AST()
        , paramName(paramName_)
        , t_assign(t_assign_)
        , argExpr(argExpr_)
    {}

    virtual FunctionArgExprAST *asFunctionArgExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OverClauseAST: public AST
{
public:
    OverClauseAST() : AST() {}
    virtual OverClauseAST *asOverClause() { return this; }
};

class SimpleOverClauseAST: public OverClauseAST
{
public:
    int t_over;
    NameAST *column;

public:
    SimpleOverClauseAST( int t_over_, NameAST *column_ )
        : OverClauseAST()
        , t_over(t_over_)
        , column(column_)
    {}

    virtual SimpleOverClauseAST *asSimpleOverClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WindowOverClauseAST: public OverClauseAST
{
public:
    int t_over;
    WindowSpecificationAST *windowSpec;

public:
    WindowOverClauseAST( int t_over_, WindowSpecificationAST *windowSpec_ )
        : OverClauseAST()
        , t_over(t_over_)
        , windowSpec(windowSpec_)
    {}

    virtual WindowOverClauseAST *asWindowOverClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WindowSpecificationAST: public AST
{
public:
    int t_lparen;
    NameAST *windowName;
    PartitionClauseAST *partitionClause;
    SortClauseAST *sortClause;
    FrameClauseAST *frameClause;
    int t_rparen;

public:
    WindowSpecificationAST( int t_lparen_, NameAST *windowName_, PartitionClauseAST *partitionClause_, SortClauseAST *sortClause_, FrameClauseAST *frameClause_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , windowName(windowName_)
        , partitionClause(partitionClause_)
        , sortClause(sortClause_)
        , frameClause(frameClause_)
        , t_rparen(t_rparen_)
    {}

    virtual WindowSpecificationAST *asWindowSpecification() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PartitionClauseAST: public AST
{
public:
    int t_partition;
    int t_by;
    ExpressionListAST *params;

public:
    PartitionClauseAST( int t_partition_, int t_by_, ExpressionListAST *params_ )
        : AST()
        , t_partition(t_partition_)
        , t_by(t_by_)
        , params(finish(params_))
    {}

    virtual PartitionClauseAST *asPartitionClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FrameClauseAST: public AST
{
public:
    int t_range_or_rows;
    FrameExtentAST *extent;

public:
    FrameClauseAST( int t_range_or_rows_, FrameExtentAST *extent_ )
        : AST()
        , t_range_or_rows(t_range_or_rows_)
        , extent(extent_)
    {}

    virtual FrameClauseAST *asFrameClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FrameExtentAST: public AST
{
public:
    FrameExtentAST() : AST() {}
    virtual FrameExtentAST *asFrameExtent() { return this; }
};

class BoundFrameExtentAST: public FrameExtentAST
{
public:
    FrameBoundAST *bound;

public:
    BoundFrameExtentAST( FrameBoundAST *bound_ )
        : FrameExtentAST()
        , bound(bound_)
    {}

    virtual BoundFrameExtentAST *asBoundFrameExtent() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BetweenFrameExtentAST: public AST
{
public:
    int t_between;
    FrameBoundAST *lBound;
    int t_and;
    FrameBoundAST *rBound;

public:
    BetweenFrameExtentAST( int t_between_, FrameBoundAST *lBound_, int t_and_, FrameBoundAST *rBound_ )
        : AST()
        , t_between(t_between_)
        , lBound(lBound_)
        , t_and(t_and_)
        , rBound(rBound_)
    {}

    virtual BetweenFrameExtentAST *asBetweenFrameExtent() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FrameBoundAST: public AST
{
public:
    FrameBoundAST() : AST() {}
    virtual FrameBoundAST *asFrameBound() { return this; }
};

class SimpleFrameBoundAST: public AST
{
public:
    int t_unbounded_or_current;
    int t_prec_or_foll_or_row;

public:
    SimpleFrameBoundAST( int t_unbounded_or_current_, int t_prec_or_foll_or_row_ )
        : AST()
        , t_unbounded_or_current(t_unbounded_or_current_)
        , t_prec_or_foll_or_row(t_prec_or_foll_or_row_)
    {}

    virtual SimpleFrameBoundAST *asSimpleFrameBound() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprFrameBoundAST: public FrameBoundAST
{
public:
    ExpressionAST *boundExpr;
    int t_prec_or_foll;

public:
    ExprFrameBoundAST( ExpressionAST *boundExpr_, int t_prec_or_foll_ )
        : FrameBoundAST()
        , boundExpr(boundExpr_)
        , t_prec_or_foll(t_prec_or_foll_)
    {}

    virtual ExprFrameBoundAST *asExprFrameBound() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DirectTableRefAST: public TableRefAST
{
public:
    RelationExprAST *relation;
    TableAliasClauseAST *alias;

public:
    DirectTableRefAST( RelationExprAST *relation_, TableAliasClauseAST *alias_ )
        : TableRefAST()
        , relation(relation_)
        , alias(alias_)
    {}

    virtual DirectTableRefAST *asDirectTableRef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableAliasClauseAST: public AST
{
public:
    int t_as;
    NameAST *aliasName;
    int t_lparen;
    NameListAST *columns;
    int t_rparen;

public:
    TableAliasClauseAST( int t_as_, NameAST *aliasName_, int t_lparen_, NameListAST *columns_, int t_rparen_ )
        : AST()
        , t_as(t_as_)
        , aliasName(aliasName_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
    {}

    virtual TableAliasClauseAST *asTableAliasClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DistinctClauseAST: public AST
{
public:
    DistinctClauseAST() : AST() {}
    virtual DistinctClauseAST *asDistinctClause() { return this; }
};

class SimpleDistinctClauseAST: public AST
{
public:
    int t_all_or_distinct;

public:
    SimpleDistinctClauseAST( int t_all_or_distinct_ )
        : AST()
        , t_all_or_distinct(t_all_or_distinct_)
    {}

    virtual SimpleDistinctClauseAST *asSimpleDistinctClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprListDistinctClauseAST: public DistinctClauseAST
{
public:
    int t_distinct;
    int t_on;
    int t_lparen;
    ExpressionListAST *distinctExpressions;
    int t_rparen;

public:
    ExprListDistinctClauseAST( int t_distinct_, int t_on_, int t_lparen_, ExpressionListAST *distinctExpressions_, int t_rparen_ )
        : DistinctClauseAST()
        , t_distinct(t_distinct_)
        , t_on(t_on_)
        , t_lparen(t_lparen_)
        , distinctExpressions(finish(distinctExpressions_))
        , t_rparen(t_rparen_)
    {}

    virtual ExprListDistinctClauseAST *asExprListDistinctClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectTargetAST: public AST
{
public:
    SelectTargetAST() : AST() {}
    virtual SelectTargetAST *asSelectTarget() { return this; }
};

class StarSelectTargetAST: public SelectTargetAST
{
public:
    int t_star;

public:
    StarSelectTargetAST( int t_star_ )
        : SelectTargetAST()
        , t_star(t_star_)
    {}

    virtual StarSelectTargetAST *asStarSelectTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprSelectTargetAST: public SelectTargetAST
{
public:
    ExpressionAST *selTargetExpr;
    int t_as;
    NameAST *columnAlias;

public:
    ExprSelectTargetAST( ExpressionAST *selTargetExpr_, int t_as_, NameAST *columnAlias_ )
        : SelectTargetAST()
        , selTargetExpr(selTargetExpr_)
        , t_as(t_as_)
        , columnAlias(columnAlias_)
    {}

    virtual ExprSelectTargetAST *asExprSelectTarget() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntoClauseAST: public AST
{
public:
    int t_into;
    OptTempTableNameAST *tempTable;

public:
    IntoClauseAST( int t_into_, OptTempTableNameAST *tempTable_ )
        : AST()
        , t_into(t_into_)
        , tempTable(tempTable_)
    {}

    virtual IntoClauseAST *asIntoClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OptTempTableNameAST: public AST
{
public:
    int t_local_or_global_or_unlogged;
    int t_temp;
    int t_table;
    TableIdentifierAST *intoTableIdent;

public:
    OptTempTableNameAST( int t_local_or_global_or_unlogged_, int t_temp_, int t_table_, TableIdentifierAST *intoTableIdent_ )
        : AST()
        , t_local_or_global_or_unlogged(t_local_or_global_or_unlogged_)
        , t_temp(t_temp_)
        , t_table(t_table_)
        , intoTableIdent(intoTableIdent_)
    {}

    virtual OptTempTableNameAST *asOptTempTableName() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ValuesClauseAST: public AST
{
public:
    int t_values;
    ColumnExprRowAST *row;

public:
    ValuesClauseAST( int t_values_, ColumnExprRowAST *row_ )
        : AST()
        , t_values(t_values_)
        , row(row_)
    {}

    virtual ValuesClauseAST *asValuesClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnExprRowAST: public AST
{
public:
    int t_lparen;
    ColumnExpressionListAST *columns;
    int t_rparen;

public:
    ColumnExprRowAST( int t_lparen_, ColumnExpressionListAST *columns_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
    {}

    virtual ColumnExprRowAST *asColumnExprRow() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnExpressionAST: public AST
{
public:
    ColumnExpressionAST() : AST() {}
    virtual ColumnExpressionAST *asColumnExpression() { return this; }
};

class DefaultColumnExpressionAST: public ColumnExpressionAST
{
public:
    int t_default;

public:
    DefaultColumnExpressionAST( int t_default_ )
        : ColumnExpressionAST()
        , t_default(t_default_)
    {}

    virtual DefaultColumnExpressionAST *asDefaultColumnExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprColumnExpressionAST: public ColumnExpressionAST
{
public:
    ExpressionAST *expr;

public:
    ExprColumnExpressionAST( ExpressionAST *expr_ )
        : ColumnExpressionAST()
        , expr(expr_)
    {}

    virtual ExprColumnExpressionAST *asExprColumnExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableCmdAST: public AST
{
public:
    AlterTableCmdAST() : AST() {}
    virtual AlterTableCmdAST *asAlterTableCmd() { return this; }
};

class AlterTableGenericCmdAST: public AlterTableCmdAST
{
public:
    int t_options;
    int t_lparen;
    AlterGenericOptionListAST *options;
    int t_rparen;

public:
    AlterTableGenericCmdAST( int t_options_, int t_lparen_, AlterGenericOptionListAST *options_, int t_rparen_ )
        : AlterTableCmdAST()
        , t_options(t_options_)
        , t_lparen(t_lparen_)
        , options(finish(options_))
        , t_rparen(t_rparen_)
    {}

    virtual AlterTableGenericCmdAST *asAlterTableGenericCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterGenericOptionAST: public AST
{
public:
    AlterGenericOptionAST() : AST() {}
    virtual AlterGenericOptionAST *asAlterGenericOption() { return this; }
};

class SetAddAlterGenericOptionAST: public AlterGenericOptionAST
{
public:
    int t_add_or_set;
    GenericOptionAST *option;

public:
    SetAddAlterGenericOptionAST( int t_add_or_set_, GenericOptionAST *option_ )
        : AlterGenericOptionAST()
        , t_add_or_set(t_add_or_set_)
        , option(option_)
    {}

    virtual SetAddAlterGenericOptionAST *asSetAddAlterGenericOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropAlterGenericOptionAST: public AlterGenericOptionAST
{
public:
    int t_drop;
    int t_opt_name;

public:
    DropAlterGenericOptionAST( int t_drop_, int t_opt_name_ )
        : AlterGenericOptionAST()
        , t_drop(t_drop_)
        , t_opt_name(t_opt_name_)
    {}

    virtual DropAlterGenericOptionAST *asDropAlterGenericOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableSetReloptCmdAST: public AlterTableCmdAST
{
public:
    int t_set_or_reset;
    RelOptionsAST *relOptions;

public:
    AlterTableSetReloptCmdAST( int t_set_or_reset_, RelOptionsAST *relOptions_ )
        : AlterTableCmdAST()
        , t_set_or_reset(t_set_or_reset_)
        , relOptions(relOptions_)
    {}

    virtual AlterTableSetReloptCmdAST *asAlterTableSetReloptCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableTablespaceCmdAST: public AlterTableCmdAST
{
public:
    int t_set;
    int t_tablespace;
    NameAST *tablespaceName;

public:
    AlterTableTablespaceCmdAST( int t_set_, int t_tablespace_, NameAST *tablespaceName_ )
        : AlterTableCmdAST()
        , t_set(t_set_)
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
    {}

    virtual AlterTableTablespaceCmdAST *asAlterTableTablespaceCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableOwnerCmdAST: public AlterTableCmdAST
{
public:
    int t_owner;
    int t_to;
    RoleIdAST *roleId;

public:
    AlterTableOwnerCmdAST( int t_owner_, int t_to_, RoleIdAST *roleId_ )
        : AlterTableCmdAST()
        , t_owner(t_owner_)
        , t_to(t_to_)
        , roleId(roleId_)
    {}

    virtual AlterTableOwnerCmdAST *asAlterTableOwnerCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableNotOfCmdAST: public AlterTableCmdAST
{
public:
    int t_not;
    int t_of;

public:
    AlterTableNotOfCmdAST( int t_not_, int t_of_ )
        : AlterTableCmdAST()
        , t_not(t_not_)
        , t_of(t_of_)
    {}

    virtual AlterTableNotOfCmdAST *asAlterTableNotOfCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableOfCmdAST: public AlterTableCmdAST
{
public:
    int t_of;
    AnyIdentifierAST *ident;

public:
    AlterTableOfCmdAST( int t_of_, AnyIdentifierAST *ident_ )
        : AlterTableCmdAST()
        , t_of(t_of_)
        , ident(ident_)
    {}

    virtual AlterTableOfCmdAST *asAlterTableOfCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableInheritCmdAST: public AlterTableCmdAST
{
public:
    int t_no;
    int t_inherit;
    TableIdentifierAST *tableIdent;

public:
    AlterTableInheritCmdAST( int t_no_, int t_inherit_, TableIdentifierAST *tableIdent_ )
        : AlterTableCmdAST()
        , t_no(t_no_)
        , t_inherit(t_inherit_)
        , tableIdent(tableIdent_)
    {}

    virtual AlterTableInheritCmdAST *asAlterTableInheritCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableTriggerCmdAST: public AlterTableCmdAST
{
public:
    int t_enable_or_disable;
    int t_always_or_replica;
    int t_trigger;
    WildcardAST *wildcard;

public:
    AlterTableTriggerCmdAST( int t_enable_or_disable_, int t_always_or_replica_, int t_trigger_, WildcardAST *wildcard_ )
        : AlterTableCmdAST()
        , t_enable_or_disable(t_enable_or_disable_)
        , t_always_or_replica(t_always_or_replica_)
        , t_trigger(t_trigger_)
        , wildcard(wildcard_)
    {}

    virtual AlterTableTriggerCmdAST *asAlterTableTriggerCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableRuleCmdAST: public AlterTableCmdAST
{
public:
    int t_enable_or_disable;
    int t_always_or_replica;
    int t_rule;
    NameAST *ruleName;

public:
    AlterTableRuleCmdAST( int t_enable_or_disable_, int t_always_or_replica_, int t_rule_, NameAST *ruleName_ )
        : AlterTableCmdAST()
        , t_enable_or_disable(t_enable_or_disable_)
        , t_always_or_replica(t_always_or_replica_)
        , t_rule(t_rule_)
        , ruleName(ruleName_)
    {}

    virtual AlterTableRuleCmdAST *asAlterTableRuleCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class WildcardAST: public AST
{
public:
    WildcardAST() : AST() {}
    virtual WildcardAST *asWildcard() { return this; }
};

class NameWildcardAST: public WildcardAST
{
public:
    NameAST *name;

public:
    NameWildcardAST( NameAST *name_ )
        : WildcardAST()
        , name(name_)
    {}

    virtual NameWildcardAST *asNameWildcard() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AllOrUserWildcardAST: public WildcardAST
{
public:
    int t_all_or_user;

public:
    AllOrUserWildcardAST( int t_all_or_user_ )
        : WildcardAST()
        , t_all_or_user(t_all_or_user_)
    {}

    virtual AllOrUserWildcardAST *asAllOrUserWildcard() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableClusterOffCmdAST: public AlterTableCmdAST
{
public:
    int t_set;
    int t_without;
    int t_cluster;

public:
    AlterTableClusterOffCmdAST( int t_set_, int t_without_, int t_cluster_ )
        : AlterTableCmdAST()
        , t_set(t_set_)
        , t_without(t_without_)
        , t_cluster(t_cluster_)
    {}

    virtual AlterTableClusterOffCmdAST *asAlterTableClusterOffCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableClusterOnCmdAST: public AlterTableCmdAST
{
public:
    int t_cluster;
    int t_on;
    NameAST *clusterName;

public:
    AlterTableClusterOnCmdAST( int t_cluster_, int t_on_, NameAST *clusterName_ )
        : AlterTableCmdAST()
        , t_cluster(t_cluster_)
        , t_on(t_on_)
        , clusterName(clusterName_)
    {}

    virtual AlterTableClusterOnCmdAST *asAlterTableClusterOnCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableSetOidsCmdAST: public AlterTableCmdAST
{
public:
    int t_set;
    int t_with_or_without;
    int t_oids;

public:
    AlterTableSetOidsCmdAST( int t_set_, int t_with_or_without_, int t_oids_ )
        : AlterTableCmdAST()
        , t_set(t_set_)
        , t_with_or_without(t_with_or_without_)
        , t_oids(t_oids_)
    {}

    virtual AlterTableSetOidsCmdAST *asAlterTableSetOidsCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableDropConstrCmdAST: public AlterTableCmdAST
{
public:
    int t_drop;
    int t_constrint;
    int t_if;
    int t_exists;
    NameAST *constrintName;
    int t_behavior;

public:
    AlterTableDropConstrCmdAST( int t_drop_, int t_constrint_, int t_if_, int t_exists_, NameAST *constrintName_, int t_behavior_ )
        : AlterTableCmdAST()
        , t_drop(t_drop_)
        , t_constrint(t_constrint_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , constrintName(constrintName_)
        , t_behavior(t_behavior_)
    {}

    virtual AlterTableDropConstrCmdAST *asAlterTableDropConstrCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableValConstrCmdAST: public AlterTableCmdAST
{
public:
    int t_validate;
    int t_constraint;
    NameAST *constraintName;

public:
    AlterTableValConstrCmdAST( int t_validate_, int t_constraint_, NameAST *constraintName_ )
        : AlterTableCmdAST()
        , t_validate(t_validate_)
        , t_constraint(t_constraint_)
        , constraintName(constraintName_)
    {}

    virtual AlterTableValConstrCmdAST *asAlterTableValConstrCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAddConstrCmdAST: public AlterTableCmdAST
{
public:
    int t_add;
    TableConstraintAST *constraint;

public:
    AlterTableAddConstrCmdAST( int t_add_, TableConstraintAST *constraint_ )
        : AlterTableCmdAST()
        , t_add(t_add_)
        , constraint(constraint_)
    {}

    virtual AlterTableAddConstrCmdAST *asAlterTableAddConstrCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableSetTypeCmdAST: public AlterTableCmdAST
{
public:
    int t_alter;
    int t_column;
    NameAST *columnName;
    SetDataAST *setData;
    int t_type;
    TypeAST *columnType;
    CollateClauseAST *collate;
    UsingClauseAST *usingClause;

public:
    AlterTableSetTypeCmdAST( int t_alter_, int t_column_, NameAST *columnName_, SetDataAST *setData_, int t_type_, TypeAST *columnType_, CollateClauseAST *collate_, UsingClauseAST *usingClause_ )
        : AlterTableCmdAST()
        , t_alter(t_alter_)
        , t_column(t_column_)
        , columnName(columnName_)
        , setData(setData_)
        , t_type(t_type_)
        , columnType(columnType_)
        , collate(collate_)
        , usingClause(usingClause_)
    {}

    virtual AlterTableSetTypeCmdAST *asAlterTableSetTypeCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SetDataAST: public AST
{
public:
    int t_set;
    int t_data;

public:
    SetDataAST( int t_set_, int t_data_ )
        : AST()
        , t_set(t_set_)
        , t_data(t_data_)
    {}

    virtual SetDataAST *asSetData() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingClauseAST: public AST
{
public:
    int t_using;
    ExpressionAST *expr;

public:
    UsingClauseAST( int t_using_, ExpressionAST *expr_ )
        : AST()
        , t_using(t_using_)
        , expr(expr_)
    {}

    virtual UsingClauseAST *asUsingClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableSetStorageCmdAST: public AlterTableCmdAST
{
public:
    int t_alter;
    int t_column;
    NameAST *columnName;
    int t_set;
    int t_storage;
    NameAST *storageName;

public:
    AlterTableSetStorageCmdAST( int t_alter_, int t_column_, NameAST *columnName_, int t_set_, int t_storage_, NameAST *storageName_ )
        : AlterTableCmdAST()
        , t_alter(t_alter_)
        , t_column(t_column_)
        , columnName(columnName_)
        , t_set(t_set_)
        , t_storage(t_storage_)
        , storageName(storageName_)
    {}

    virtual AlterTableSetStorageCmdAST *asAlterTableSetStorageCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableChgReloptCmdAST: public AlterTableCmdAST
{
public:
    int t_alter;
    int t_column;
    NameAST *columnName;
    int t_set_or_reset;
    RelOptionsAST *relOptions;

public:
    AlterTableChgReloptCmdAST( int t_alter_, int t_column_, NameAST *columnName_, int t_set_or_reset_, RelOptionsAST *relOptions_ )
        : AlterTableCmdAST()
        , t_alter(t_alter_)
        , t_column(t_column_)
        , columnName(columnName_)
        , t_set_or_reset(t_set_or_reset_)
        , relOptions(relOptions_)
    {}

    virtual AlterTableChgReloptCmdAST *asAlterTableChgReloptCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RelOptionsAST: public AST
{
public:
    int t_lparen;
    RelOptionListAST *options;
    int t_rparen;

public:
    RelOptionsAST( int t_lparen_, RelOptionListAST *options_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , options(finish(options_))
        , t_rparen(t_rparen_)
    {}

    virtual RelOptionsAST *asRelOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RelOptionAST: public AST
{
public:
    NameAST *rel1;
    int t_dot;
    NameAST *rel2;
    int t_equal;
    DefinitionArgumentAST *arg;

public:
    RelOptionAST( NameAST *rel1_, int t_dot_, NameAST *rel2_, int t_equal_, DefinitionArgumentAST *arg_ )
        : AST()
        , rel1(rel1_)
        , t_dot(t_dot_)
        , rel2(rel2_)
        , t_equal(t_equal_)
        , arg(arg_)
    {}

    virtual RelOptionAST *asRelOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableSetStatCmdAST: public AlterTableCmdAST
{
public:
    int t_alter;
    int t_column;
    NameAST *columnName;
    int t_set;
    int t_statistics;
    int t_value;

public:
    AlterTableSetStatCmdAST( int t_alter_, int t_column_, NameAST *columnName_, int t_set_, int t_statistics_, int t_value_ )
        : AlterTableCmdAST()
        , t_alter(t_alter_)
        , t_column(t_column_)
        , columnName(columnName_)
        , t_set(t_set_)
        , t_statistics(t_statistics_)
        , t_value(t_value_)
    {}

    virtual AlterTableSetStatCmdAST *asAlterTableSetStatCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableChgNotNullCmdAST: public AlterTableCmdAST
{
public:
    int t_alter;
    int t_column;
    NameAST *columnName;
    int t_set_or_drop;
    int t_not;
    int t_null;

public:
    AlterTableChgNotNullCmdAST( int t_alter_, int t_column_, NameAST *columnName_, int t_set_or_drop_, int t_not_, int t_null_ )
        : AlterTableCmdAST()
        , t_alter(t_alter_)
        , t_column(t_column_)
        , columnName(columnName_)
        , t_set_or_drop(t_set_or_drop_)
        , t_not(t_not_)
        , t_null(t_null_)
    {}

    virtual AlterTableChgNotNullCmdAST *asAlterTableChgNotNullCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableChgDefColCmdAST: public AlterTableCmdAST
{
public:
    int t_alter;
    int t_column;
    NameAST *columnName;
    AlterDefaultColumnAST *cmd;

public:
    AlterTableChgDefColCmdAST( int t_alter_, int t_column_, NameAST *columnName_, AlterDefaultColumnAST *cmd_ )
        : AlterTableCmdAST()
        , t_alter(t_alter_)
        , t_column(t_column_)
        , columnName(columnName_)
        , cmd(cmd_)
    {}

    virtual AlterTableChgDefColCmdAST *asAlterTableChgDefColCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterDefaultColumnAST: public AST
{
public:
    AlterDefaultColumnAST() : AST() {}
    virtual AlterDefaultColumnAST *asAlterDefaultColumn() { return this; }
};

class SetAlterDefaultColumnAST: public AlterDefaultColumnAST
{
public:
    int t_set;
    int t_default;
    ExpressionAST *expr;

public:
    SetAlterDefaultColumnAST( int t_set_, int t_default_, ExpressionAST *expr_ )
        : AlterDefaultColumnAST()
        , t_set(t_set_)
        , t_default(t_default_)
        , expr(expr_)
    {}

    virtual SetAlterDefaultColumnAST *asSetAlterDefaultColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropAlterDefaultColumnAST: public AlterDefaultColumnAST
{
public:
    int t_drop;
    int t_default;

public:
    DropAlterDefaultColumnAST( int t_drop_, int t_default_ )
        : AlterDefaultColumnAST()
        , t_drop(t_drop_)
        , t_default(t_default_)
    {}

    virtual DropAlterDefaultColumnAST *asDropAlterDefaultColumn() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableDropColCmdAST: public AlterTableCmdAST
{
public:
    int t_drop;
    int t_column;
    int t_if;
    int t_exists;
    NameAST *columnName;
    int t_drop_behavior;

public:
    AlterTableDropColCmdAST( int t_drop_, int t_column_, int t_if_, int t_exists_, NameAST *columnName_, int t_drop_behavior_ )
        : AlterTableCmdAST()
        , t_drop(t_drop_)
        , t_column(t_column_)
        , t_if(t_if_)
        , t_exists(t_exists_)
        , columnName(columnName_)
        , t_drop_behavior(t_drop_behavior_)
    {}

    virtual AlterTableDropColCmdAST *asAlterTableDropColCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AlterTableAddColCmdAST: public AlterTableCmdAST
{
public:
    int t_add;
    int t_column;
    ColumnDefAST *colDef;

public:
    AlterTableAddColCmdAST( int t_add_, int t_column_, ColumnDefAST *colDef_ )
        : AlterTableCmdAST()
        , t_add(t_add_)
        , t_column(t_column_)
        , colDef(colDef_)
    {}

    virtual AlterTableAddColCmdAST *asAlterTableAddColCmd() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnDefAST: public AST
{
public:
    NameAST *columnName;
    TypeAST *columnType;
    CreateGeneticOptionsAST *options;
    ColumnConstraintListAST *constraints;

public:
    ColumnDefAST( NameAST *columnName_, TypeAST *columnType_, CreateGeneticOptionsAST *options_, ColumnConstraintListAST *constraints_ )
        : AST()
        , columnName(columnName_)
        , columnType(columnType_)
        , options(options_)
        , constraints(finish(constraints_))
    {}

    virtual ColumnDefAST *asColumnDef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintAST: public AST
{
public:
    ConstraintAST() : AST() {}
    virtual ConstraintAST *asConstraint() { return this; }
};

class TableConstraintAST: public ConstraintAST
{
public:
    int t_constraint;
    NameAST *constraintName;
    TableConstraintBodyAST *body;

public:
    TableConstraintAST( int t_constraint_, NameAST *constraintName_, TableConstraintBodyAST *body_ )
        : ConstraintAST()
        , t_constraint(t_constraint_)
        , constraintName(constraintName_)
        , body(body_)
    {}

    virtual TableConstraintAST *asTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableConstraintBodyAST: public AST
{
public:
    TableConstraintBodyAST() : AST() {}
    virtual TableConstraintBodyAST *asTableConstraintBody() { return this; }
};

class FkTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_foreign;
    int t_key;
    int t_lparen;
    NameListAST *columns;
    int t_rparen;
    int t_references;
    TableIdentifierAST *tableIdent;
    RefColumnsAST *refColumns;
    KeyMatchAST *match;
    KeyActionsAST *actions;
    ConstraintAttributeListAST *attrs;

public:
    FkTableConstraintAST( int t_foreign_, int t_key_, int t_lparen_, NameListAST *columns_, int t_rparen_, int t_references_, TableIdentifierAST *tableIdent_, RefColumnsAST *refColumns_, KeyMatchAST *match_, KeyActionsAST *actions_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_foreign(t_foreign_)
        , t_key(t_key_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , t_references(t_references_)
        , tableIdent(tableIdent_)
        , refColumns(refColumns_)
        , match(match_)
        , actions(actions_)
        , attrs(finish(attrs_))
    {}

    virtual FkTableConstraintAST *asFkTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExcludeTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_exclude;
    AccessMethodClauseAST *accessClause;
    int t_lparen;
    ExclusionConstraintListAST *items;
    int t_rparen;
    ConstraintDefinitionsAST *definitions;
    ConstraintTablespaceAST *constrTablespace;
    ExclusionWhereClauseAST *whereClause;
    ConstraintAttributeListAST *attrs;

public:
    ExcludeTableConstraintAST( int t_exclude_, AccessMethodClauseAST *accessClause_, int t_lparen_, ExclusionConstraintListAST *items_, int t_rparen_, ConstraintDefinitionsAST *definitions_, ConstraintTablespaceAST *constrTablespace_, ExclusionWhereClauseAST *whereClause_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_exclude(t_exclude_)
        , accessClause(accessClause_)
        , t_lparen(t_lparen_)
        , items(finish(items_))
        , t_rparen(t_rparen_)
        , definitions(definitions_)
        , constrTablespace(constrTablespace_)
        , whereClause(whereClause_)
        , attrs(finish(attrs_))
    {}

    virtual ExcludeTableConstraintAST *asExcludeTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AccessMethodClauseAST: public AST
{
public:
    int t_using;
    NameAST *methodName;

public:
    AccessMethodClauseAST( int t_using_, NameAST *methodName_ )
        : AST()
        , t_using(t_using_)
        , methodName(methodName_)
    {}

    virtual AccessMethodClauseAST *asAccessMethodClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExclusionWhereClauseAST: public AST
{
public:
    int t_where;
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;

public:
    ExclusionWhereClauseAST( int t_where_, int t_lparen_, ExpressionAST *expr_, int t_rparen_ )
        : AST()
        , t_where(t_where_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
    {}

    virtual ExclusionWhereClauseAST *asExclusionWhereClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExclusionConstraintAST: public AST
{
public:
    IndexElementAST *indexElement;
    int t_with;
    int t_operator;
    int t_lparen;
    OperandAST *operand;
    int t_rparen;

public:
    ExclusionConstraintAST( IndexElementAST *indexElement_, int t_with_, int t_operator_, int t_lparen_, OperandAST *operand_, int t_rparen_ )
        : AST()
        , indexElement(indexElement_)
        , t_with(t_with_)
        , t_operator(t_operator_)
        , t_lparen(t_lparen_)
        , operand(operand_)
        , t_rparen(t_rparen_)
    {}

    virtual ExclusionConstraintAST *asExclusionConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndexElementAST: public AST
{
public:
    IndexElementAST() : AST() {}
    virtual IndexElementAST *asIndexElement() { return this; }
};

class ColIndexElementAST: public IndexElementAST
{
public:
    NameAST *columnName;
    CollateClauseAST *collate;
    ClassClauseAST *classClause;
    int t_asc_or_desc;
    int t_nulls_order;

public:
    ColIndexElementAST( NameAST *columnName_, CollateClauseAST *collate_, ClassClauseAST *classClause_, int t_asc_or_desc_, int t_nulls_order_ )
        : IndexElementAST()
        , columnName(columnName_)
        , collate(collate_)
        , classClause(classClause_)
        , t_asc_or_desc(t_asc_or_desc_)
        , t_nulls_order(t_nulls_order_)
    {}

    virtual ColIndexElementAST *asColIndexElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncExprIndexElementAST: public IndexElementAST
{
public:
    FunctionExpressionAST *funcExpr;
    CollateClauseAST *collate;
    ClassClauseAST *classClause;
    int t_asc_or_desc;
    int t_nulls_order;

public:
    FuncExprIndexElementAST( FunctionExpressionAST *funcExpr_, CollateClauseAST *collate_, ClassClauseAST *classClause_, int t_asc_or_desc_, int t_nulls_order_ )
        : IndexElementAST()
        , funcExpr(funcExpr_)
        , collate(collate_)
        , classClause(classClause_)
        , t_asc_or_desc(t_asc_or_desc_)
        , t_nulls_order(t_nulls_order_)
    {}

    virtual FuncExprIndexElementAST *asFuncExprIndexElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprIndexElementAST: public IndexElementAST
{
public:
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;
    CollateClauseAST *collate;
    ClassClauseAST *classClause;
    int t_asc_or_desc;
    int t_nulls_order;

public:
    ExprIndexElementAST( int t_lparen_, ExpressionAST *expr_, int t_rparen_, CollateClauseAST *collate_, ClassClauseAST *classClause_, int t_asc_or_desc_, int t_nulls_order_ )
        : IndexElementAST()
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
        , collate(collate_)
        , classClause(classClause_)
        , t_asc_or_desc(t_asc_or_desc_)
        , t_nulls_order(t_nulls_order_)
    {}

    virtual ExprIndexElementAST *asExprIndexElement() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ClassClauseAST: public AST
{
public:
    int t_using;
    NameAST *name;

public:
    ClassClauseAST( int t_using_, NameAST *name_ )
        : AST()
        , t_using(t_using_)
        , name(name_)
    {}

    virtual ClassClauseAST *asClassClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndPrimKeyTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_primary;
    int t_key;
    ExistingIndexAST *existingIndex;
    ConstraintAttributeListAST *attrs;

public:
    IndPrimKeyTableConstraintAST( int t_primary_, int t_key_, ExistingIndexAST *existingIndex_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_primary(t_primary_)
        , t_key(t_key_)
        , existingIndex(existingIndex_)
        , attrs(finish(attrs_))
    {}

    virtual IndPrimKeyTableConstraintAST *asIndPrimKeyTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrimKeyTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_primary;
    int t_key;
    int t_lparen;
    NameListAST *columns;
    int t_rparen;
    ConstraintDefinitionsAST *definitions;
    ConstraintTablespaceAST *constrTablespace;
    ConstraintAttributeListAST *attrs;

public:
    PrimKeyTableConstraintAST( int t_primary_, int t_key_, int t_lparen_, NameListAST *columns_, int t_rparen_, ConstraintDefinitionsAST *definitions_, ConstraintTablespaceAST *constrTablespace_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_primary(t_primary_)
        , t_key(t_key_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , definitions(definitions_)
        , constrTablespace(constrTablespace_)
        , attrs(finish(attrs_))
    {}

    virtual PrimKeyTableConstraintAST *asPrimKeyTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndUniqueTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_unique;
    ExistingIndexAST *existingIndex;
    ConstraintAttributeListAST *attrs;

public:
    IndUniqueTableConstraintAST( int t_unique_, ExistingIndexAST *existingIndex_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_unique(t_unique_)
        , existingIndex(existingIndex_)
        , attrs(finish(attrs_))
    {}

    virtual IndUniqueTableConstraintAST *asIndUniqueTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExistingIndexAST: public AST
{
public:
    int t_using;
    int t_index;
    NameAST *indexName;

public:
    ExistingIndexAST( int t_using_, int t_index_, NameAST *indexName_ )
        : AST()
        , t_using(t_using_)
        , t_index(t_index_)
        , indexName(indexName_)
    {}

    virtual ExistingIndexAST *asExistingIndex() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_unique;
    int t_lparen;
    NameListAST *columns;
    int t_rparen;
    ConstraintDefinitionsAST *definitions;
    ConstraintTablespaceAST *constrTablespace;
    ConstraintAttributeListAST *attrs;

public:
    UniqueTableConstraintAST( int t_unique_, int t_lparen_, NameListAST *columns_, int t_rparen_, ConstraintDefinitionsAST *definitions_, ConstraintTablespaceAST *constrTablespace_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_unique(t_unique_)
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
        , definitions(definitions_)
        , constrTablespace(constrTablespace_)
        , attrs(finish(attrs_))
    {}

    virtual UniqueTableConstraintAST *asUniqueTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckTableConstraintAST: public TableConstraintBodyAST
{
public:
    int t_check;
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;
    ConstraintAttributeListAST *attrs;

public:
    CheckTableConstraintAST( int t_check_, int t_lparen_, ExpressionAST *expr_, int t_rparen_, ConstraintAttributeListAST *attrs_ )
        : TableConstraintBodyAST()
        , t_check(t_check_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
        , attrs(finish(attrs_))
    {}

    virtual CheckTableConstraintAST *asCheckTableConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintAttributeAST: public AST
{
public:
    ConstraintAttributeAST() : AST() {}
    virtual ConstraintAttributeAST *asConstraintAttribute() { return this; }
};

class DeferConstraintAttrAST: public ConstraintAttributeAST
{
public:
    int t_not;
    int t_deferrable;

public:
    DeferConstraintAttrAST( int t_not_, int t_deferrable_ )
        : ConstraintAttributeAST()
        , t_not(t_not_)
        , t_deferrable(t_deferrable_)
    {}

    virtual DeferConstraintAttrAST *asDeferConstraintAttr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InitiallyConstraintAttrAST: public ConstraintAttributeAST
{
public:
    int t_initally;
    int t_deferred_or_immediate;

public:
    InitiallyConstraintAttrAST( int t_initally_, int t_deferred_or_immediate_ )
        : ConstraintAttributeAST()
        , t_initally(t_initally_)
        , t_deferred_or_immediate(t_deferred_or_immediate_)
    {}

    virtual InitiallyConstraintAttrAST *asInitiallyConstraintAttr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotValidConstraintAttrAST: public ConstraintAttributeAST
{
public:
    int t_not;
    int t_valid;

public:
    NotValidConstraintAttrAST( int t_not_, int t_valid_ )
        : ConstraintAttributeAST()
        , t_not(t_not_)
        , t_valid(t_valid_)
    {}

    virtual NotValidConstraintAttrAST *asNotValidConstraintAttr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoInheritConstraintAttrAST: public ConstraintAttributeAST
{
public:
    int t_no;
    int t_inherit;

public:
    NoInheritConstraintAttrAST( int t_no_, int t_inherit_ )
        : ConstraintAttributeAST()
        , t_no(t_no_)
        , t_inherit(t_inherit_)
    {}

    virtual NoInheritConstraintAttrAST *asNoInheritConstraintAttr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnConstraintAST: public ConstraintAST
{
public:
    ColumnConstraintAST() : ConstraintAST() {}
    virtual ColumnConstraintAST *asColumnConstraint() { return this; }
};

class CommonColConstraintAST: public ColumnConstraintAST
{
public:
    int t_constraint;
    NameAST *constraintName;
    ConstraintBodyAST *body;

public:
    CommonColConstraintAST( int t_constraint_, NameAST *constraintName_, ConstraintBodyAST *body_ )
        : ColumnConstraintAST()
        , t_constraint(t_constraint_)
        , constraintName(constraintName_)
        , body(body_)
    {}

    virtual CommonColConstraintAST *asCommonColConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintBodyAST: public AST
{
public:
    ConstraintBodyAST() : AST() {}
    virtual ConstraintBodyAST *asConstraintBody() { return this; }
};

class RefConstraintBodyAST: public ConstraintBodyAST
{
public:
    int t_references;
    TableIdentifierAST *tableIdent;
    RefColumnsAST *refColumns;
    KeyMatchAST *match;
    KeyActionsAST *actions;

public:
    RefConstraintBodyAST( int t_references_, TableIdentifierAST *tableIdent_, RefColumnsAST *refColumns_, KeyMatchAST *match_, KeyActionsAST *actions_ )
        : ConstraintBodyAST()
        , t_references(t_references_)
        , tableIdent(tableIdent_)
        , refColumns(refColumns_)
        , match(match_)
        , actions(actions_)
    {}

    virtual RefConstraintBodyAST *asRefConstraintBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RefColumnsAST: public AST
{
public:
    int t_lparen;
    NameListAST *columns;
    int t_rparen;

public:
    RefColumnsAST( int t_lparen_, NameListAST *columns_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , columns(finish(columns_))
        , t_rparen(t_rparen_)
    {}

    virtual RefColumnsAST *asRefColumns() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class KeyMatchAST: public AST
{
public:
    int t_match;
    int t_full_or_partial_or_simple;

public:
    KeyMatchAST( int t_match_, int t_full_or_partial_or_simple_ )
        : AST()
        , t_match(t_match_)
        , t_full_or_partial_or_simple(t_full_or_partial_or_simple_)
    {}

    virtual KeyMatchAST *asKeyMatch() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class KeyActionsAST: public AST
{
public:
    KeyActionsAST() : AST() {}
    virtual KeyActionsAST *asKeyActions() { return this; }
};

class UpdDelKeyActionsAST: public KeyActionsAST
{
public:
    KeyUpdateAST *upd;
    KeyDeleteAST *del;

public:
    UpdDelKeyActionsAST( KeyUpdateAST *upd_, KeyDeleteAST *del_ )
        : KeyActionsAST()
        , upd(upd_)
        , del(del_)
    {}

    virtual UpdDelKeyActionsAST *asUpdDelKeyActions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DelUpdKeyActionsAST: public KeyActionsAST
{
public:
    KeyDeleteAST *del;
    KeyUpdateAST *upd;

public:
    DelUpdKeyActionsAST( KeyDeleteAST *del_, KeyUpdateAST *upd_ )
        : KeyActionsAST()
        , del(del_)
        , upd(upd_)
    {}

    virtual DelUpdKeyActionsAST *asDelUpdKeyActions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class KeyUpdateAST: public AST
{
public:
    int t_on;
    int t_update;
    KeyActionAST *action;

public:
    KeyUpdateAST( int t_on_, int t_update_, KeyActionAST *action_ )
        : AST()
        , t_on(t_on_)
        , t_update(t_update_)
        , action(action_)
    {}

    virtual KeyUpdateAST *asKeyUpdate() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class KeyDeleteAST: public AST
{
public:
    int t_on;
    int t_delete;
    KeyActionAST *action;

public:
    KeyDeleteAST( int t_on_, int t_delete_, KeyActionAST *action_ )
        : AST()
        , t_on(t_on_)
        , t_delete(t_delete_)
        , action(action_)
    {}

    virtual KeyDeleteAST *asKeyDelete() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class KeyActionAST: public AST
{
public:
    int t_spec1;
    int t_spec2;

public:
    KeyActionAST( int t_spec1_, int t_spec2_ )
        : AST()
        , t_spec1(t_spec1_)
        , t_spec2(t_spec2_)
    {}

    virtual KeyActionAST *asKeyAction() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefaultConstraintBodyAST: public ConstraintBodyAST
{
public:
    int t_default;
    ExpressionAST *expr;

public:
    DefaultConstraintBodyAST( int t_default_, ExpressionAST *expr_ )
        : ConstraintBodyAST()
        , t_default(t_default_)
        , expr(expr_)
    {}

    virtual DefaultConstraintBodyAST *asDefaultConstraintBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckConstraintBodyAST: public ConstraintBodyAST
{
public:
    int t_check;
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;
    NoInheritAST *noInherit;

public:
    CheckConstraintBodyAST( int t_check_, int t_lparen_, ExpressionAST *expr_, int t_rparen_, NoInheritAST *noInherit_ )
        : ConstraintBodyAST()
        , t_check(t_check_)
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
        , noInherit(noInherit_)
    {}

    virtual CheckConstraintBodyAST *asCheckConstraintBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoInheritAST: public AST
{
public:
    int t_no;
    int t_inherit;

public:
    NoInheritAST( int t_no_, int t_inherit_ )
        : AST()
        , t_no(t_no_)
        , t_inherit(t_inherit_)
    {}

    virtual NoInheritAST *asNoInherit() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueConstraintBodyAST: public ConstraintBodyAST
{
public:
    int t_unique;
    ConstraintDefinitionsAST *definitions;
    ConstraintTablespaceAST *tablespace;

public:
    UniqueConstraintBodyAST( int t_unique_, ConstraintDefinitionsAST *definitions_, ConstraintTablespaceAST *tablespace_ )
        : ConstraintBodyAST()
        , t_unique(t_unique_)
        , definitions(definitions_)
        , tablespace(tablespace_)
    {}

    virtual UniqueConstraintBodyAST *asUniqueConstraintBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PrimaryKeyConstraintBodyAST: public ConstraintBodyAST
{
public:
    int t_primary;
    int t_key;
    ConstraintDefinitionsAST *definitions;
    ConstraintTablespaceAST *tablespace;

public:
    PrimaryKeyConstraintBodyAST( int t_primary_, int t_key_, ConstraintDefinitionsAST *definitions_, ConstraintTablespaceAST *tablespace_ )
        : ConstraintBodyAST()
        , t_primary(t_primary_)
        , t_key(t_key_)
        , definitions(definitions_)
        , tablespace(tablespace_)
    {}

    virtual PrimaryKeyConstraintBodyAST *asPrimaryKeyConstraintBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintTablespaceAST: public AST
{
public:
    int t_using;
    int t_index;
    int t_tablespace;
    NameAST *tablespaceName;

public:
    ConstraintTablespaceAST( int t_using_, int t_index_, int t_tablespace_, NameAST *tablespaceName_ )
        : AST()
        , t_using(t_using_)
        , t_index(t_index_)
        , t_tablespace(t_tablespace_)
        , tablespaceName(tablespaceName_)
    {}

    virtual ConstraintTablespaceAST *asConstraintTablespace() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintDefinitionsAST: public AST
{
public:
    int t_with;
    DefinitionAST *def;

public:
    ConstraintDefinitionsAST( int t_with_, DefinitionAST *def_ )
        : AST()
        , t_with(t_with_)
        , def(def_)
    {}

    virtual ConstraintDefinitionsAST *asConstraintDefinitions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefinitionAST: public AST
{
public:
    int t_lparen;
    DefinitionElemListAST *items;
    int t_rparen;

public:
    DefinitionAST( int t_lparen_, DefinitionElemListAST *items_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , items(finish(items_))
        , t_rparen(t_rparen_)
    {}

    virtual DefinitionAST *asDefinition() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefinitionElemAST: public AST
{
public:
    int t_label;
    int t_equal;
    DefinitionArgumentAST *arg;

public:
    DefinitionElemAST( int t_label_, int t_equal_, DefinitionArgumentAST *arg_ )
        : AST()
        , t_label(t_label_)
        , t_equal(t_equal_)
        , arg(arg_)
    {}

    virtual DefinitionElemAST *asDefinitionElem() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefinitionArgumentAST: public AST
{
public:
    DefinitionArgumentAST() : AST() {}
    virtual DefinitionArgumentAST *asDefinitionArgument() { return this; }
};

class FuncTyDefinitionArgumentAST: public DefinitionArgumentAST
{
public:
    FunctionTypeAST *funcTy;

public:
    FuncTyDefinitionArgumentAST( FunctionTypeAST *funcTy_ )
        : DefinitionArgumentAST()
        , funcTy(funcTy_)
    {}

    virtual FuncTyDefinitionArgumentAST *asFuncTyDefinitionArgument() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionTypeAST: public AST
{
public:
    FunctionTypeAST() : AST() {}
    virtual FunctionTypeAST *asFunctionType() { return this; }
};

class PercentFunctionTypeAST: public FunctionTypeAST
{
public:
    int t_setof;
    NameAST *funcName;
    NameListAST *attrs;
    int t_percent;
    int t_type;

public:
    PercentFunctionTypeAST( int t_setof_, NameAST *funcName_, NameListAST *attrs_, int t_percent_, int t_type_ )
        : FunctionTypeAST()
        , t_setof(t_setof_)
        , funcName(funcName_)
        , attrs(finish(attrs_))
        , t_percent(t_percent_)
        , t_type(t_type_)
    {}

    virtual PercentFunctionTypeAST *asPercentFunctionType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleFunctionTypeAST: public FunctionTypeAST
{
public:
    TypeAST *type;

public:
    SimpleFunctionTypeAST( TypeAST *type_ )
        : FunctionTypeAST()
        , type(type_)
    {}

    virtual SimpleFunctionTypeAST *asSimpleFunctionType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class QualOpDefinitionArgumentAST: public DefinitionArgumentAST
{
public:
    QualifiedOperatorAST *qualOp;

public:
    QualOpDefinitionArgumentAST( QualifiedOperatorAST *qualOp_ )
        : DefinitionArgumentAST()
        , qualOp(qualOp_)
    {}

    virtual QualOpDefinitionArgumentAST *asQualOpDefinitionArgument() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleDefinitionArgumentAST: public DefinitionArgumentAST
{
public:
    int t_value;

public:
    SimpleDefinitionArgumentAST( int t_value_ )
        : DefinitionArgumentAST()
        , t_value(t_value_)
    {}

    virtual SimpleDefinitionArgumentAST *asSimpleDefinitionArgument() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotNullConstraintBodyAST: public ConstraintBodyAST
{
public:
    int t_not;
    int t_null;

public:
    NotNullConstraintBodyAST( int t_not_, int t_null_ )
        : ConstraintBodyAST()
        , t_not(t_not_)
        , t_null(t_null_)
    {}

    virtual NotNullConstraintBodyAST *asNotNullConstraintBody() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AttrColConstraintAST: public ColumnConstraintAST
{
public:
    AttrColConstraintAST() : ColumnConstraintAST() {}
    virtual AttrColConstraintAST *asAttrColConstraint() { return this; }
};

class DeferColConstraintAST: public AttrColConstraintAST
{
public:
    int t_not;
    int t_deferrable;

public:
    DeferColConstraintAST( int t_not_, int t_deferrable_ )
        : AttrColConstraintAST()
        , t_not(t_not_)
        , t_deferrable(t_deferrable_)
    {}

    virtual DeferColConstraintAST *asDeferColConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InitiallyColConstraintAST: public AttrColConstraintAST
{
public:
    int t_initially;
    int t_deferred_or_immediate;

public:
    InitiallyColConstraintAST( int t_initially_, int t_deferred_or_immediate_ )
        : AttrColConstraintAST()
        , t_initially(t_initially_)
        , t_deferred_or_immediate(t_deferred_or_immediate_)
    {}

    virtual InitiallyColConstraintAST *asInitiallyColConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollateColConstraintAST: public ColumnConstraintAST
{
public:
    int t_collate;
    AnyIdentifierAST *collateValue;

public:
    CollateColConstraintAST( int t_collate_, AnyIdentifierAST *collateValue_ )
        : ColumnConstraintAST()
        , t_collate(t_collate_)
        , collateValue(collateValue_)
    {}

    virtual CollateColConstraintAST *asCollateColConstraint() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateGeneticOptionsAST: public AST
{
public:
    int t_options;
    int t_lparen;
    GenericOptionListAST *options;
    int t_rparen;

public:
    CreateGeneticOptionsAST( int t_options_, int t_lparen_, GenericOptionListAST *options_, int t_rparen_ )
        : AST()
        , t_options(t_options_)
        , t_lparen(t_lparen_)
        , options(finish(options_))
        , t_rparen(t_rparen_)
    {}

    virtual CreateGeneticOptionsAST *asCreateGeneticOptions() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GenericOptionAST: public AST
{
public:
    int t_option;
    int t_value;

public:
    GenericOptionAST( int t_option_, int t_value_ )
        : AST()
        , t_option(t_option_)
        , t_value(t_value_)
    {}

    virtual GenericOptionAST *asGenericOption() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeAST: public AST
{
public:
    int t_setof;
    SimpleTypenameAST *simpleName;
    int t_array;
    ArrBoundListAST *arrBounds;

public:
    TypeAST( int t_setof_, SimpleTypenameAST *simpleName_, int t_array_, ArrBoundListAST *arrBounds_ )
        : AST()
        , t_setof(t_setof_)
        , simpleName(simpleName_)
        , t_array(t_array_)
        , arrBounds(finish(arrBounds_))
    {}

    virtual TypeAST *asType() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleTypenameAST: public AST
{
public:
    SimpleTypenameAST() : AST() {}
    virtual SimpleTypenameAST *asSimpleTypename() { return this; }
};

class GenericTypenameAST: public SimpleTypenameAST
{
public:
    NameAST *typeName;
    NameListAST *attributes;
    TypeModifiersAST *modifiers;

public:
    GenericTypenameAST( NameAST *typeName_, NameListAST *attributes_, TypeModifiersAST *modifiers_ )
        : SimpleTypenameAST()
        , typeName(typeName_)
        , attributes(finish(attributes_))
        , modifiers(modifiers_)
    {}

    virtual GenericTypenameAST *asGenericTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntervalTypenameAST: public SimpleTypenameAST
{
public:
    int t_interval;
    int t_lparen;
    int t_arrLength;
    int t_rparen;
    IntervalAST *interval;

public:
    IntervalTypenameAST( int t_interval_, int t_lparen_, int t_arrLength_, int t_rparen_, IntervalAST *interval_ )
        : SimpleTypenameAST()
        , t_interval(t_interval_)
        , t_lparen(t_lparen_)
        , t_arrLength(t_arrLength_)
        , t_rparen(t_rparen_)
        , interval(interval_)
    {}

    virtual IntervalTypenameAST *asIntervalTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NumericTypenameAST: public SimpleTypenameAST
{
public:
    int t_type_spec;

public:
    NumericTypenameAST( int t_type_spec_ )
        : SimpleTypenameAST()
        , t_type_spec(t_type_spec_)
    {}

    virtual NumericTypenameAST *asNumericTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FloatTypenameAST: public SimpleTypenameAST
{
public:
    int t_type_spec;
    FloatPrecisionAST *precision;

public:
    FloatTypenameAST( int t_type_spec_, FloatPrecisionAST *precision_ )
        : SimpleTypenameAST()
        , t_type_spec(t_type_spec_)
        , precision(precision_)
    {}

    virtual FloatTypenameAST *asFloatTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DoubleTypenameAST: public SimpleTypenameAST
{
public:
    int t_double;
    int t_precision;

public:
    DoubleTypenameAST( int t_double_, int t_precision_ )
        : SimpleTypenameAST()
        , t_double(t_double_)
        , t_precision(t_precision_)
    {}

    virtual DoubleTypenameAST *asDoubleTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DecimalTypenameAST: public SimpleTypenameAST
{
public:
    int t_type_spec;
    TypeModifiersAST *modifiers;

public:
    DecimalTypenameAST( int t_type_spec_, TypeModifiersAST *modifiers_ )
        : SimpleTypenameAST()
        , t_type_spec(t_type_spec_)
        , modifiers(modifiers_)
    {}

    virtual DecimalTypenameAST *asDecimalTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FloatPrecisionAST: public AST
{
public:
    int t_lparen;
    int t_precision;
    int t_rparen;

public:
    FloatPrecisionAST( int t_lparen_, int t_precision_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , t_precision(t_precision_)
        , t_rparen(t_rparen_)
    {}

    virtual FloatPrecisionAST *asFloatPrecision() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BitTypenameAST: public SimpleTypenameAST
{
public:
    int t_type_spec;
    int t_varying;
    int t_lparen;
    ExpressionListAST *modifiers;
    int t_rparen;

public:
    BitTypenameAST( int t_type_spec_, int t_varying_, int t_lparen_, ExpressionListAST *modifiers_, int t_rparen_ )
        : SimpleTypenameAST()
        , t_type_spec(t_type_spec_)
        , t_varying(t_varying_)
        , t_lparen(t_lparen_)
        , modifiers(finish(modifiers_))
        , t_rparen(t_rparen_)
    {}

    virtual BitTypenameAST *asBitTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CharacterTypenameAST: public SimpleTypenameAST
{
public:
    CharacterTypeSpecAST *typeSpec;
    int t_lparen;
    int t_length;
    int t_rparen;
    CharSetAST *set;

public:
    CharacterTypenameAST( CharacterTypeSpecAST *typeSpec_, int t_lparen_, int t_length_, int t_rparen_, CharSetAST *set_ )
        : SimpleTypenameAST()
        , typeSpec(typeSpec_)
        , t_lparen(t_lparen_)
        , t_length(t_length_)
        , t_rparen(t_rparen_)
        , set(set_)
    {}

    virtual CharacterTypenameAST *asCharacterTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DatetimeTypenameAST: public SimpleTypenameAST
{
public:
    int t_time_or_timestamp;
    int t_lparen;
    int t_value;
    int t_rparen;
    TimeZoneSpecAST *timeZoneSpec;

public:
    DatetimeTypenameAST( int t_time_or_timestamp_, int t_lparen_, int t_value_, int t_rparen_, TimeZoneSpecAST *timeZoneSpec_ )
        : SimpleTypenameAST()
        , t_time_or_timestamp(t_time_or_timestamp_)
        , t_lparen(t_lparen_)
        , t_value(t_value_)
        , t_rparen(t_rparen_)
        , timeZoneSpec(timeZoneSpec_)
    {}

    virtual DatetimeTypenameAST *asDatetimeTypename() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TimeZoneSpecAST: public AST
{
public:
    int t_with_or_without;
    int t_time;
    int t_zone;

public:
    TimeZoneSpecAST( int t_with_or_without_, int t_time_, int t_zone_ )
        : AST()
        , t_with_or_without(t_with_or_without_)
        , t_time(t_time_)
        , t_zone(t_zone_)
    {}

    virtual TimeZoneSpecAST *asTimeZoneSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CharacterTypeSpecAST: public AST
{
public:
    int t_national;
    int t_type_spec;
    int t_varying;

public:
    CharacterTypeSpecAST( int t_national_, int t_type_spec_, int t_varying_ )
        : AST()
        , t_national(t_national_)
        , t_type_spec(t_type_spec_)
        , t_varying(t_varying_)
    {}

    virtual CharacterTypeSpecAST *asCharacterTypeSpec() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CharSetAST: public AST
{
public:
    int t_character;
    int t_set;
    int t_value;

public:
    CharSetAST( int t_character_, int t_set_, int t_value_ )
        : AST()
        , t_character(t_character_)
        , t_set(t_set_)
        , t_value(t_value_)
    {}

    virtual CharSetAST *asCharSet() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeModifiersAST: public AST
{
public:
    int t_lparen;
    ExpressionListAST *modifiers;
    int t_rparen;

public:
    TypeModifiersAST( int t_lparen_, ExpressionListAST *modifiers_, int t_rparen_ )
        : AST()
        , t_lparen(t_lparen_)
        , modifiers(finish(modifiers_))
        , t_rparen(t_rparen_)
    {}

    virtual TypeModifiersAST *asTypeModifiers() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrBoundAST: public AST
{
public:
    int t_lbracket;
    int t_arr_length;
    int t_rbracket;

public:
    ArrBoundAST( int t_lbracket_, int t_arr_length_, int t_rbracket_ )
        : AST()
        , t_lbracket(t_lbracket_)
        , t_arr_length(t_arr_length_)
        , t_rbracket(t_rbracket_)
    {}

    virtual ArrBoundAST *asArrBound() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExpressionAST: public AST
{
public:
    ExpressionAST() : AST() {}
    virtual ExpressionAST *asExpression() { return this; }
};

class PrimaryExpressionAST: public ExpressionAST
{
public:
    PrimaryExpressionAST() : ExpressionAST() {}
    virtual PrimaryExpressionAST *asPrimaryExpression() { return this; }
};

class RowPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    RowAST *row;

public:
    RowPrimaryExpressionAST( RowAST *row_ )
        : PrimaryExpressionAST()
        , row(row_)
    {}

    virtual RowPrimaryExpressionAST *asRowPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrayPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    int t_array;
    ArrayExprAST *arrExpr;

public:
    ArrayPrimaryExpressionAST( int t_array_, ArrayExprAST *arrExpr_ )
        : PrimaryExpressionAST()
        , t_array(t_array_)
        , arrExpr(arrExpr_)
    {}

    virtual ArrayPrimaryExpressionAST *asArrayPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrayExprAST: public AST
{
public:
    ArrayExprAST() : AST() {}
    virtual ArrayExprAST *asArrayExpr() { return this; }
};

class ExprListArrayExprAST: public ArrayExprAST
{
public:
    int t_lbracket;
    ExpressionListAST *values;
    int t_rbracket;

public:
    ExprListArrayExprAST( int t_lbracket_, ExpressionListAST *values_, int t_rbracket_ )
        : ArrayExprAST()
        , t_lbracket(t_lbracket_)
        , values(finish(values_))
        , t_rbracket(t_rbracket_)
    {}

    virtual ExprListArrayExprAST *asExprListArrayExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrListArrayExprAST: public ArrayExprAST
{
public:
    int t_lbracket;
    ArrayExprListAST *arrList;
    int t_rbracket;

public:
    ArrListArrayExprAST( int t_lbracket_, ArrayExprListAST *arrList_, int t_rbracket_ )
        : ArrayExprAST()
        , t_lbracket(t_lbracket_)
        , arrList(finish(arrList_))
        , t_rbracket(t_rbracket_)
    {}

    virtual ArrListArrayExprAST *asArrListArrayExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EmptyArrayExprAST: public ArrayExprAST
{
public:
    int t_lbracket;
    int t_rbracket;

public:
    EmptyArrayExprAST( int t_lbracket_, int t_rbracket_ )
        : ArrayExprAST()
        , t_lbracket(t_lbracket_)
        , t_rbracket(t_rbracket_)
    {}

    virtual EmptyArrayExprAST *asEmptyArrayExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubqueryPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    int t_exists_or_array;
    SelectWithParensAST *subquery;
    IndirectionListAST *qualifiers;

public:
    SubqueryPrimaryExpressionAST( int t_exists_or_array_, SelectWithParensAST *subquery_, IndirectionListAST *qualifiers_ )
        : PrimaryExpressionAST()
        , t_exists_or_array(t_exists_or_array_)
        , subquery(subquery_)
        , qualifiers(finish(qualifiers_))
    {}

    virtual SubqueryPrimaryExpressionAST *asSubqueryPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    FunctionExpressionAST *func;

public:
    FunctionPrimaryExpressionAST( FunctionExpressionAST *func_ )
        : PrimaryExpressionAST()
        , func(func_)
    {}

    virtual FunctionPrimaryExpressionAST *asFunctionPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColRefPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    ColumnRefAST *columnRef;

public:
    ColRefPrimaryExpressionAST( ColumnRefAST *columnRef_ )
        : PrimaryExpressionAST()
        , columnRef(columnRef_)
    {}

    virtual ColRefPrimaryExpressionAST *asColRefPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    ConstantAST *value;

public:
    ConstPrimaryExpressionAST( ConstantAST *value_ )
        : PrimaryExpressionAST()
        , value(value_)
    {}

    virtual ConstPrimaryExpressionAST *asConstPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ParamPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    NameAST *param;
    IndirectionListAST *qualifiers;

public:
    ParamPrimaryExpressionAST( NameAST *param_, IndirectionListAST *qualifiers_ )
        : PrimaryExpressionAST()
        , param(param_)
        , qualifiers(finish(qualifiers_))
    {}

    virtual ParamPrimaryExpressionAST *asParamPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundPrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    int t_lparen;
    ExpressionAST *expr;
    int t_rparen;
    IndirectionListAST *qualifiers;

public:
    CompoundPrimaryExpressionAST( int t_lparen_, ExpressionAST *expr_, int t_rparen_, IndirectionListAST *qualifiers_ )
        : PrimaryExpressionAST()
        , t_lparen(t_lparen_)
        , expr(expr_)
        , t_rparen(t_rparen_)
        , qualifiers(finish(qualifiers_))
    {}

    virtual CompoundPrimaryExpressionAST *asCompoundPrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CasePrimaryExpressionAST: public PrimaryExpressionAST
{
public:
    CaseExprAST *caseExpr;

public:
    CasePrimaryExpressionAST( CaseExprAST *caseExpr_ )
        : PrimaryExpressionAST()
        , caseExpr(caseExpr_)
    {}

    virtual CasePrimaryExpressionAST *asCasePrimaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseExprAST: public AST
{
public:
    int t_case;
    CaseArgAST *arg;
    CaseWhenClauseListAST *whens;
    CaseDefaultClauseAST *deaultClause;
    int t_end;

public:
    CaseExprAST( int t_case_, CaseArgAST *arg_, CaseWhenClauseListAST *whens_, CaseDefaultClauseAST *deaultClause_, int t_end_ )
        : AST()
        , t_case(t_case_)
        , arg(arg_)
        , whens(finish(whens_))
        , deaultClause(deaultClause_)
        , t_end(t_end_)
    {}

    virtual CaseExprAST *asCaseExpr() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseArgAST: public AST
{
public:
    ExpressionAST *expr;

public:
    CaseArgAST( ExpressionAST *expr_ )
        : AST()
        , expr(expr_)
    {}

    virtual CaseArgAST *asCaseArg() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseWhenClauseAST: public AST
{
public:
    int t_when;
    ExpressionAST *whernExpr;
    int t_then;
    ExpressionAST *thenExpr;

public:
    CaseWhenClauseAST( int t_when_, ExpressionAST *whernExpr_, int t_then_, ExpressionAST *thenExpr_ )
        : AST()
        , t_when(t_when_)
        , whernExpr(whernExpr_)
        , t_then(t_then_)
        , thenExpr(thenExpr_)
    {}

    virtual CaseWhenClauseAST *asCaseWhenClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CaseDefaultClauseAST: public AST
{
public:
    int t_else;
    ExpressionAST *expr;

public:
    CaseDefaultClauseAST( int t_else_, ExpressionAST *expr_ )
        : AST()
        , t_else(t_else_)
        , expr(expr_)
    {}

    virtual CaseDefaultClauseAST *asCaseDefaultClause() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstantAST: public AST
{
public:
    ConstantAST() : AST() {}
    virtual ConstantAST *asConstant() { return this; }
};

class SimpleConstantAST: public ConstantAST
{
public:
    int t_value;

public:
    SimpleConstantAST( int t_value_ )
        : ConstantAST()
        , t_value(t_value_)
    {}

    virtual SimpleConstantAST *asSimpleConstant() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FuncNameConstantAST: public ConstantAST
{
public:
    FunctionRefAST *funcRef;
    int t_lparen;
    FunctionArgExprListAST *args;
    int t_rparen;
    int t_value;

public:
    FuncNameConstantAST( FunctionRefAST *funcRef_, int t_lparen_, FunctionArgExprListAST *args_, int t_rparen_, int t_value_ )
        : ConstantAST()
        , funcRef(funcRef_)
        , t_lparen(t_lparen_)
        , args(finish(args_))
        , t_rparen(t_rparen_)
        , t_value(t_value_)
    {}

    virtual FuncNameConstantAST *asFuncNameConstant() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IntervalConstantAST: public ConstantAST
{
public:
    int t_interval;
    int t_lparen;
    int t_val;
    int t_rparen;
    int t_sval;
    IntervalAST *interval;

public:
    IntervalConstantAST( int t_interval_, int t_lparen_, int t_val_, int t_rparen_, int t_sval_, IntervalAST *interval_ )
        : ConstantAST()
        , t_interval(t_interval_)
        , t_lparen(t_lparen_)
        , t_val(t_val_)
        , t_rparen(t_rparen_)
        , t_sval(t_sval_)
        , interval(interval_)
    {}

    virtual IntervalConstantAST *asIntervalConstant() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypenameConstantAST: public AST
{
public:
    SimpleTypenameAST *typeName;
    int t_value;

public:
    TypenameConstantAST( SimpleTypenameAST *typeName_, int t_value_ )
        : AST()
        , typeName(typeName_)
        , t_value(t_value_)
    {}

    virtual TypenameConstantAST *asTypenameConstant() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypecastExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    int t_dotdot;
    TypeAST *type;

public:
    TypecastExpressionAST( ExpressionAST *expr_, int t_dotdot_, TypeAST *type_ )
        : ExpressionAST()
        , expr(expr_)
        , t_dotdot(t_dotdot_)
        , type(type_)
    {}

    virtual TypecastExpressionAST *asTypecastExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CollateExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    int t_collate;
    AnyIdentifierAST *value;

public:
    CollateExpressionAST( ExpressionAST *expr_, int t_collate_, AnyIdentifierAST *value_ )
        : ExpressionAST()
        , expr(expr_)
        , t_collate(t_collate_)
        , value(value_)
    {}

    virtual CollateExpressionAST *asCollateExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AtTimeZoneExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *lExpr;
    int t_at;
    int t_time;
    int t_zone;
    ExpressionAST *rExpr;

public:
    AtTimeZoneExpressionAST( ExpressionAST *lExpr_, int t_at_, int t_time_, int t_zone_, ExpressionAST *rExpr_ )
        : ExpressionAST()
        , lExpr(lExpr_)
        , t_at(t_at_)
        , t_time(t_time_)
        , t_zone(t_zone_)
        , rExpr(rExpr_)
    {}

    virtual AtTimeZoneExpressionAST *asAtTimeZoneExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UnaryExpressionAST: public ExpressionAST
{
public:
    int t_op;
    ExpressionAST *expr;

public:
    UnaryExpressionAST( int t_op_, ExpressionAST *expr_ )
        : ExpressionAST()
        , t_op(t_op_)
        , expr(expr_)
    {}

    virtual UnaryExpressionAST *asUnaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BinaryExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *lExpr;
    int t_op;
    ExpressionAST *rExpr;

public:
    BinaryExpressionAST( ExpressionAST *lExpr_, int t_op_, ExpressionAST *rExpr_ )
        : ExpressionAST()
        , lExpr(lExpr_)
        , t_op(t_op_)
        , rExpr(rExpr_)
    {}

    virtual BinaryExpressionAST *asBinaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonBinaryExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *lExpr;
    QualifiedOperatorAST *commonOp;
    ExpressionAST *rExpr;

public:
    CommonBinaryExpressionAST( ExpressionAST *lExpr_, QualifiedOperatorAST *commonOp_, ExpressionAST *rExpr_ )
        : ExpressionAST()
        , lExpr(lExpr_)
        , commonOp(commonOp_)
        , rExpr(rExpr_)
    {}

    virtual CommonBinaryExpressionAST *asCommonBinaryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonUnaryExpresionAST: public ExpressionAST
{
public:
    QualifiedOperatorAST *commonOp;
    ExpressionAST *expr;

public:
    CommonUnaryExpresionAST( QualifiedOperatorAST *commonOp_, ExpressionAST *expr_ )
        : ExpressionAST()
        , commonOp(commonOp_)
        , expr(expr_)
    {}

    virtual CommonUnaryExpresionAST *asCommonUnaryExpresion() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommonPostfixExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    QualifiedOperatorAST *commonOp;

public:
    CommonPostfixExpressionAST( ExpressionAST *expr_, QualifiedOperatorAST *commonOp_ )
        : ExpressionAST()
        , expr(expr_)
        , commonOp(commonOp_)
    {}

    virtual CommonPostfixExpressionAST *asCommonPostfixExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LikeExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *lExpr;
    int t_not;
    int t_like;
    ExpressionAST *rExpr;
    int t_escape;
    ExpressionAST *escExpr;

public:
    LikeExpressionAST( ExpressionAST *lExpr_, int t_not_, int t_like_, ExpressionAST *rExpr_, int t_escape_, ExpressionAST *escExpr_ )
        : ExpressionAST()
        , lExpr(lExpr_)
        , t_not(t_not_)
        , t_like(t_like_)
        , rExpr(rExpr_)
        , t_escape(t_escape_)
        , escExpr(escExpr_)
    {}

    virtual LikeExpressionAST *asLikeExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimilarToExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *lExpr;
    int t_not;
    int t_similar;
    int t_to;
    ExpressionAST *rExpr;
    int t_excape;
    ExpressionAST *escExpr;

public:
    SimilarToExpressionAST( ExpressionAST *lExpr_, int t_not_, int t_similar_, int t_to_, ExpressionAST *rExpr_, int t_excape_, ExpressionAST *escExpr_ )
        : ExpressionAST()
        , lExpr(lExpr_)
        , t_not(t_not_)
        , t_similar(t_similar_)
        , t_to(t_to_)
        , rExpr(rExpr_)
        , t_excape(t_excape_)
        , escExpr(escExpr_)
    {}

    virtual SimilarToExpressionAST *asSimilarToExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IsExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    int t_is;
    int t_not;
    int t_something;

public:
    IsExpressionAST( ExpressionAST *expr_, int t_is_, int t_not_, int t_something_ )
        : ExpressionAST()
        , expr(expr_)
        , t_is(t_is_)
        , t_not(t_not_)
        , t_something(t_something_)
    {}

    virtual IsExpressionAST *asIsExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IsDdistinctExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *lExpr;
    int t_is;
    int t_not;
    int t_distinct;
    int t_from;
    ExpressionAST *rExpr;

public:
    IsDdistinctExpressionAST( ExpressionAST *lExpr_, int t_is_, int t_not_, int t_distinct_, int t_from_, ExpressionAST *rExpr_ )
        : ExpressionAST()
        , lExpr(lExpr_)
        , t_is(t_is_)
        , t_not(t_not_)
        , t_distinct(t_distinct_)
        , t_from(t_from_)
        , rExpr(rExpr_)
    {}

    virtual IsDdistinctExpressionAST *asIsDdistinctExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IsTypeofExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    int t_is;
    int t_not;
    int t_of;
    int t_lparen;
    TypeListAST *types;
    int t_rparen;

public:
    IsTypeofExpressionAST( ExpressionAST *expr_, int t_is_, int t_not_, int t_of_, int t_lparen_, TypeListAST *types_, int t_rparen_ )
        : ExpressionAST()
        , expr(expr_)
        , t_is(t_is_)
        , t_not(t_not_)
        , t_of(t_of_)
        , t_lparen(t_lparen_)
        , types(finish(types_))
        , t_rparen(t_rparen_)
    {}

    virtual IsTypeofExpressionAST *asIsTypeofExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BetweenExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    int t_not;
    int t_between;
    int t_symmetric_spec;
    ExpressionAST *lExpr;
    int t_and;
    ExpressionAST *rExpr;

public:
    BetweenExpressionAST( ExpressionAST *expr_, int t_not_, int t_between_, int t_symmetric_spec_, ExpressionAST *lExpr_, int t_and_, ExpressionAST *rExpr_ )
        : ExpressionAST()
        , expr(expr_)
        , t_not(t_not_)
        , t_between(t_between_)
        , t_symmetric_spec(t_symmetric_spec_)
        , lExpr(lExpr_)
        , t_and(t_and_)
        , rExpr(rExpr_)
    {}

    virtual BetweenExpressionAST *asBetweenExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    int t_not;
    int t_in;
    InValuesAST *values;

public:
    InExpressionAST( ExpressionAST *expr_, int t_not_, int t_in_, InValuesAST *values_ )
        : ExpressionAST()
        , expr(expr_)
        , t_not(t_not_)
        , t_in(t_in_)
        , values(values_)
    {}

    virtual InExpressionAST *asInExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InValuesAST: public AST
{
public:
    InValuesAST() : AST() {}
    virtual InValuesAST *asInValues() { return this; }
};

class InValuesAsQueryAST: public InValuesAST
{
public:
    SelectWithParensAST *query;

public:
    InValuesAsQueryAST( SelectWithParensAST *query_ )
        : InValuesAST()
        , query(query_)
    {}

    virtual InValuesAsQueryAST *asInValuesAsQuery() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InValuesAsExprListAST: public InValuesAST
{
public:
    int t_lparen;
    ExpressionListAST *values;
    int t_rparen;

public:
    InValuesAsExprListAST( int t_lparen_, ExpressionListAST *values_, int t_rparen_ )
        : InValuesAST()
        , t_lparen(t_lparen_)
        , values(finish(values_))
        , t_rparen(t_rparen_)
    {}

    virtual InValuesAsExprListAST *asInValuesAsExprList() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RelationExprSimpleAST: public RelationExprAST
{
public:
    TableIdentifierAST *tableIdent;
    int t_star;

public:
    RelationExprSimpleAST( TableIdentifierAST *tableIdent_, int t_star_ )
        : RelationExprAST()
        , tableIdent(tableIdent_)
        , t_star(t_star_)
    {}

    virtual RelationExprSimpleAST *asRelationExprSimple() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubqueryExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    SubqueryOperatorAST *subqueryOp;
    int t_sub_type;
    SelectWithParensAST *query;

public:
    SubqueryExpressionAST( ExpressionAST *expr_, SubqueryOperatorAST *subqueryOp_, int t_sub_type_, SelectWithParensAST *query_ )
        : ExpressionAST()
        , expr(expr_)
        , subqueryOp(subqueryOp_)
        , t_sub_type(t_sub_type_)
        , query(query_)
    {}

    virtual SubqueryExpressionAST *asSubqueryExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubExpressionExpressionAST: public ExpressionAST
{
public:
    ExpressionAST *expr;
    SubqueryOperatorAST *subqueryOp;
    int t_sub_type;
    int t_lparen;
    ExpressionAST *subExpr;
    int t_rparen;

public:
    SubExpressionExpressionAST( ExpressionAST *expr_, SubqueryOperatorAST *subqueryOp_, int t_sub_type_, int t_lparen_, ExpressionAST *subExpr_, int t_rparen_ )
        : ExpressionAST()
        , expr(expr_)
        , subqueryOp(subqueryOp_)
        , t_sub_type(t_sub_type_)
        , t_lparen(t_lparen_)
        , subExpr(subExpr_)
        , t_rparen(t_rparen_)
    {}

    virtual SubExpressionExpressionAST *asSubExpressionExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RowOverlapsExpressionAST: public ExpressionAST
{
public:
    RowAST *lRow;
    int t_overlaps;
    RowAST *rRow;

public:
    RowOverlapsExpressionAST( RowAST *lRow_, int t_overlaps_, RowAST *rRow_ )
        : ExpressionAST()
        , lRow(lRow_)
        , t_overlaps(t_overlaps_)
        , rRow(rRow_)
    {}

    virtual RowOverlapsExpressionAST *asRowOverlapsExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueExpressionAST: public ExpressionAST
{
public:
    int t_unique;
    SelectWithParensAST *query;

public:
    UniqueExpressionAST( int t_unique_, SelectWithParensAST *query_ )
        : ExpressionAST()
        , t_unique(t_unique_)
        , query(query_)
    {}

    virtual UniqueExpressionAST *asUniqueExpression() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RowAST: public AST
{
public:
    int t_row;
    int t_lparen;
    ExpressionListAST *values;
    int t_rparen;

public:
    RowAST( int t_row_, int t_lparen_, ExpressionListAST *values_, int t_rparen_ )
        : AST()
        , t_row(t_row_)
        , t_lparen(t_lparen_)
        , values(finish(values_))
        , t_rparen(t_rparen_)
    {}

    virtual RowAST *asRow() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RelationExprOnlyAST: public RelationExprAST
{
public:
    int t_only;
    int t_lparen;
    TableIdentifierAST *tableIdent;
    int t_rparen;

public:
    RelationExprOnlyAST( int t_only_, int t_lparen_, TableIdentifierAST *tableIdent_, int t_rparen_ )
        : RelationExprAST()
        , t_only(t_only_)
        , t_lparen(t_lparen_)
        , tableIdent(tableIdent_)
        , t_rparen(t_rparen_)
    {}

    virtual RelationExprOnlyAST *asRelationExprOnly() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubqueryOperatorAST: public AST
{
public:
    SubqueryOperatorAST() : AST() {}
    virtual SubqueryOperatorAST *asSubqueryOperator() { return this; }
};

class QualSubqueryOperatorAST: public SubqueryOperatorAST
{
public:
    QualifiedOperatorAST *qualOp;

public:
    QualSubqueryOperatorAST( QualifiedOperatorAST *qualOp_ )
        : SubqueryOperatorAST()
        , qualOp(qualOp_)
    {}

    virtual QualSubqueryOperatorAST *asQualSubqueryOperator() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LikeSubqueryOperatorAST: public SubqueryOperatorAST
{
public:
    int t_not;
    int t_like_or_ilike;

public:
    LikeSubqueryOperatorAST( int t_not_, int t_like_or_ilike_ )
        : SubqueryOperatorAST()
        , t_not(t_not_)
        , t_like_or_ilike(t_like_or_ilike_)
    {}

    virtual LikeSubqueryOperatorAST *asLikeSubqueryOperator() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class QualifiedOperatorAST: public AST
{
public:
    QualifiedOperatorAST() : AST() {}
    virtual QualifiedOperatorAST *asQualifiedOperator() { return this; }
};

class SimpleQualifiedOperatorAST: public QualifiedOperatorAST
{
public:
    int t_op;

public:
    SimpleQualifiedOperatorAST( int t_op_ )
        : QualifiedOperatorAST()
        , t_op(t_op_)
    {}

    virtual SimpleQualifiedOperatorAST *asSimpleQualifiedOperator() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CompoundQualifiedOperatorAST: public QualifiedOperatorAST
{
public:
    int t_operator;
    int t_lparen;
    OperandAST *oper;
    int t_rparen;

public:
    CompoundQualifiedOperatorAST( int t_operator_, int t_lparen_, OperandAST *oper_, int t_rparen_ )
        : QualifiedOperatorAST()
        , t_operator(t_operator_)
        , t_lparen(t_lparen_)
        , oper(oper_)
        , t_rparen(t_rparen_)
    {}

    virtual CompoundQualifiedOperatorAST *asCompoundQualifiedOperator() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OperandAST: public AST
{
public:
    ColumnIdentifierAST *columnIdent;
    int t_dot;
    int t_op;

public:
    OperandAST( ColumnIdentifierAST *columnIdent_, int t_dot_, int t_op_ )
        : AST()
        , columnIdent(columnIdent_)
        , t_dot(t_dot_)
        , t_op(t_op_)
    {}

    virtual OperandAST *asOperand() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AnyIdentifierAST: public AST
{
public:
    NameAST *name;
    NameListAST *attributes;

public:
    AnyIdentifierAST( NameAST *name_, NameListAST *attributes_ )
        : AST()
        , name(name_)
        , attributes(finish(attributes_))
    {}

    virtual AnyIdentifierAST *asAnyIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int t_dot;
    NameAST *tableName;

public:
    TableIdentifierAST( NameAST *schemaName_, int t_dot_, NameAST *tableName_ )
        : AST()
        , schemaName(schemaName_)
        , t_dot(t_dot_)
        , tableName(tableName_)
    {}

    virtual TableIdentifierAST *asTableIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int t_dot;
    NameAST *triggerName;

public:
    TriggerIdentifierAST( NameAST *schemaName_, int t_dot_, NameAST *triggerName_ )
        : AST()
        , schemaName(schemaName_)
        , t_dot(t_dot_)
        , triggerName(triggerName_)
    {}

    virtual TriggerIdentifierAST *asTriggerIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndexIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int t_dot;
    NameAST *indexName;

public:
    IndexIdentifierAST( NameAST *schemaName_, int t_dot_, NameAST *indexName_ )
        : AST()
        , schemaName(schemaName_)
        , t_dot(t_dot_)
        , indexName(indexName_)
    {}

    virtual IndexIdentifierAST *asIndexIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int t_dot;
    NameAST *constraintName;

public:
    ConstraintIdentifierAST( NameAST *schemaName_, int t_dot_, NameAST *constraintName_ )
        : AST()
        , schemaName(schemaName_)
        , t_dot(t_dot_)
        , constraintName(constraintName_)
    {}

    virtual ConstraintIdentifierAST *asConstraintIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SequenceIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int t_dot;
    NameAST *sequenceName;

public:
    SequenceIdentifierAST( NameAST *schemaName_, int t_dot_, NameAST *sequenceName_ )
        : AST()
        , schemaName(schemaName_)
        , t_dot(t_dot_)
        , sequenceName(sequenceName_)
    {}

    virtual SequenceIdentifierAST *asSequenceIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnIdentifierAST: public AST
{
public:
    NameAST *schemaName;
    int t_dot1;
    NameAST *tableName;
    int t_dot2;
    NameAST *columnName;

public:
    ColumnIdentifierAST( NameAST *schemaName_, int t_dot1_, NameAST *tableName_, int t_dot2_, NameAST *columnName_ )
        : AST()
        , schemaName(schemaName_)
        , t_dot1(t_dot1_)
        , tableName(tableName_)
        , t_dot2(t_dot2_)
        , columnName(columnName_)
    {}

    virtual ColumnIdentifierAST *asColumnIdentifier() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnRefAST: public AST
{
public:
    NameAST *firstName;
    IndirectionListAST *qualifiers;

public:
    ColumnRefAST( NameAST *firstName_, IndirectionListAST *qualifiers_ )
        : AST()
        , firstName(firstName_)
        , qualifiers(finish(qualifiers_))
    {}

    virtual ColumnRefAST *asColumnRef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionRefAST: public AST
{
public:
    NameAST *firstName;
    IndirectionListAST *qualifiers;

public:
    FunctionRefAST( NameAST *firstName_, IndirectionListAST *qualifiers_ )
        : AST()
        , firstName(firstName_)
        , qualifiers(finish(qualifiers_))
    {}

    virtual FunctionRefAST *asFunctionRef() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndirectionAST: public AST
{
public:
    IndirectionAST() : AST() {}
    virtual IndirectionAST *asIndirection() { return this; }
};

class AttirbuteIndirectionAST: public IndirectionAST
{
public:
    int t_dot;
    NameAST *attrName;

public:
    AttirbuteIndirectionAST( int t_dot_, NameAST *attrName_ )
        : IndirectionAST()
        , t_dot(t_dot_)
        , attrName(attrName_)
    {}

    virtual AttirbuteIndirectionAST *asAttirbuteIndirection() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StarIndirectionAST: public IndirectionAST
{
public:
    int t_dot;
    int t_star;

public:
    StarIndirectionAST( int t_dot_, int t_star_ )
        : IndirectionAST()
        , t_dot(t_dot_)
        , t_star(t_star_)
    {}

    virtual StarIndirectionAST *asStarIndirection() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndexingIndirectionAST: public IndirectionAST
{
public:
    int t_lbracket;
    ExpressionAST *expr1;
    int t_comma;
    ExpressionAST *expr2;
    int t_rpbracket;

public:
    IndexingIndirectionAST( int t_lbracket_, ExpressionAST *expr1_, int t_comma_, ExpressionAST *expr2_, int t_rpbracket_ )
        : IndirectionAST()
        , t_lbracket(t_lbracket_)
        , expr1(expr1_)
        , t_comma(t_comma_)
        , expr2(expr2_)
        , t_rpbracket(t_rpbracket_)
    {}

    virtual IndexingIndirectionAST *asIndexingIndirection() { return this; }

    virtual int firstToken() const;
    virtual int lastToken() const;
    virtual bool isNull() const;

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

}   // namespace Postgres

#endif // POSTGRESAST_H
