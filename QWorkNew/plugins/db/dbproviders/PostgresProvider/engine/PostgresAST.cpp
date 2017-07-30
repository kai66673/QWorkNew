#include <assert.h>

#include "PostgresAST.h"
#include "PostgresASTVisitor.h"
#include "PostgresSqlCheckNamesVisitor.h"

namespace Postgres {

AST::AST()
{ }

AST::~AST()
{ assert(0); }

void AST::accept( ASTVisitor *visitor )
{
    if ( visitor->preVisit(this) )
        accept0(visitor);
    visitor->postVisit(this);
}

void EmptyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

void NameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TranslationUnitAST::firstToken() const
{
    if ( statements )
        return statements->firstToken();
    return 0;
}

int TranslationUnitAST::lastToken() const
{
    if ( statements )
        return statements->lastToken();
    return 1;
}

bool TranslationUnitAST::isNull() const
{
    if ( statements )
        return false;
    return true;
}

void TranslationUnitAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(statements, visitor);
    }
    visitor->endVisit(this);
}

int UnknownStatementAST::firstToken() const
{
    if ( t_first )
        return t_first;
    if ( t_last )
        return t_last;
    return 0;
}

int UnknownStatementAST::lastToken() const
{
    if ( t_last )
        return t_last + 1;
    if ( t_first )
        return t_first + 1;
    return 1;
}

bool UnknownStatementAST::isNull() const
{
    if ( t_first )
        return false;
    if ( t_last )
        return false;
    return true;
}

void UnknownStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateViewStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_or )
        return t_or;
    if ( t_replace )
        return t_replace;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->firstToken();
    if ( t_recursive )
        return t_recursive;
    if ( t_view )
        return t_view;
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->firstToken();
    if ( columns && !columns->isNull() )
        return columns->firstToken();
    if ( relOpts && !relOpts->isNull() )
        return relOpts->firstToken();
    if ( t_as )
        return t_as;
    if ( query && !query->isNull() )
        return query->firstToken();
    if ( checkOpt && !checkOpt->isNull() )
        return checkOpt->firstToken();
    return 0;
}

int CreateViewStmtAST::lastToken() const
{
    if ( checkOpt && !checkOpt->isNull() )
        return checkOpt->lastToken();
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( relOpts && !relOpts->isNull() )
        return relOpts->lastToken();
    if ( columns && !columns->isNull() )
        return columns->lastToken();
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->lastToken();
    if ( t_view )
        return t_view + 1;
    if ( t_recursive )
        return t_recursive + 1;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->lastToken();
    if ( t_replace )
        return t_replace + 1;
    if ( t_or )
        return t_or + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateViewStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_or )
        return false;
    if ( t_replace )
        return false;
    if ( tempClause )
        return false;
    if ( t_recursive )
        return false;
    if ( t_view )
        return false;
    if ( viewIdent )
        return false;
    if ( columns )
        return false;
    if ( relOpts )
        return false;
    if ( t_as )
        return false;
    if ( query )
        return false;
    if ( checkOpt )
        return false;
    return true;
}

void CreateViewStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tempClause, visitor);
        accept(viewIdent, visitor);
        accept(columns, visitor);
        accept(relOpts, visitor);
        accept(query, visitor);
        accept(checkOpt, visitor);
    }
    visitor->endVisit(this);
}

int WithCheckOptionClauseAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( t_local_or_cascaded )
        return t_local_or_cascaded;
    if ( t_check )
        return t_check;
    if ( t_option )
        return t_option;
    return 0;
}

int WithCheckOptionClauseAST::lastToken() const
{
    if ( t_option )
        return t_option + 1;
    if ( t_check )
        return t_check + 1;
    if ( t_local_or_cascaded )
        return t_local_or_cascaded + 1;
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithCheckOptionClauseAST::isNull() const
{
    if ( t_with )
        return false;
    if ( t_local_or_cascaded )
        return false;
    if ( t_check )
        return false;
    if ( t_option )
        return false;
    return true;
}

void WithCheckOptionClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommonVacuumStmtAST::firstToken() const
{
    if ( t_vacuum )
        return t_vacuum;
    if ( t_lparen )
        return t_lparen;
    if ( options )
        return options->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( columns && !columns->isNull() )
        return columns->firstToken();
    return 0;
}

int CommonVacuumStmtAST::lastToken() const
{
    if ( columns && !columns->isNull() )
        return columns->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( options )
        return options->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_vacuum )
        return t_vacuum + 1;
    return 1;
}

bool CommonVacuumStmtAST::isNull() const
{
    if ( t_vacuum )
        return false;
    if ( t_lparen )
        return false;
    if ( options )
        return false;
    if ( t_rparen )
        return false;
    if ( tableIdent )
        return false;
    if ( columns )
        return false;
    return true;
}

void CommonVacuumStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(options, visitor);
        accept(tableIdent, visitor);
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int VacuumOptionAST::firstToken() const
{
    if ( t_analyze_verbose_freeze_full )
        return t_analyze_verbose_freeze_full;
    return 0;
}

int VacuumOptionAST::lastToken() const
{
    if ( t_analyze_verbose_freeze_full )
        return t_analyze_verbose_freeze_full + 1;
    return 1;
}

bool VacuumOptionAST::isNull() const
{
    if ( t_analyze_verbose_freeze_full )
        return false;
    return true;
}

void VacuumOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int VacuumTableStmtAST::firstToken() const
{
    if ( t_vacuum )
        return t_vacuum;
    if ( t_full )
        return t_full;
    if ( t_freeze )
        return t_freeze;
    if ( t_verbose )
        return t_verbose;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    return 0;
}

int VacuumTableStmtAST::lastToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_verbose )
        return t_verbose + 1;
    if ( t_freeze )
        return t_freeze + 1;
    if ( t_full )
        return t_full + 1;
    if ( t_vacuum )
        return t_vacuum + 1;
    return 1;
}

bool VacuumTableStmtAST::isNull() const
{
    if ( t_vacuum )
        return false;
    if ( t_full )
        return false;
    if ( t_freeze )
        return false;
    if ( t_verbose )
        return false;
    if ( tableIdent )
        return false;
    return true;
}

void VacuumTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int VacuumAnalyzeStmtAST::firstToken() const
{
    if ( t_vacuum )
        return t_vacuum;
    if ( t_full )
        return t_full;
    if ( t_freeze )
        return t_freeze;
    if ( t_verbose )
        return t_verbose;
    if ( analyzeStmt && !analyzeStmt->isNull() )
        return analyzeStmt->firstToken();
    return 0;
}

int VacuumAnalyzeStmtAST::lastToken() const
{
    if ( analyzeStmt && !analyzeStmt->isNull() )
        return analyzeStmt->lastToken();
    if ( t_verbose )
        return t_verbose + 1;
    if ( t_freeze )
        return t_freeze + 1;
    if ( t_full )
        return t_full + 1;
    if ( t_vacuum )
        return t_vacuum + 1;
    return 1;
}

bool VacuumAnalyzeStmtAST::isNull() const
{
    if ( t_vacuum )
        return false;
    if ( t_full )
        return false;
    if ( t_freeze )
        return false;
    if ( t_verbose )
        return false;
    if ( analyzeStmt )
        return false;
    return true;
}

void VacuumAnalyzeStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(analyzeStmt, visitor);
    }
    visitor->endVisit(this);
}

int TruncateStmtAST::firstToken() const
{
    if ( t_truncate )
        return t_truncate;
    if ( t_table )
        return t_table;
    if ( tables )
        return tables->firstToken();
    if ( restartClause && !restartClause->isNull() )
        return restartClause->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int TruncateStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( restartClause && !restartClause->isNull() )
        return restartClause->lastToken();
    if ( tables )
        return tables->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( t_truncate )
        return t_truncate + 1;
    return 1;
}

bool TruncateStmtAST::isNull() const
{
    if ( t_truncate )
        return false;
    if ( t_table )
        return false;
    if ( tables )
        return false;
    if ( restartClause )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void TruncateStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tables, visitor);
        accept(restartClause, visitor);
    }
    visitor->endVisit(this);
}

int TruncateRestartClauseAST::firstToken() const
{
    if ( t_continue_or_restart )
        return t_continue_or_restart;
    if ( t_identity )
        return t_identity;
    return 0;
}

int TruncateRestartClauseAST::lastToken() const
{
    if ( t_identity )
        return t_identity + 1;
    if ( t_continue_or_restart )
        return t_continue_or_restart + 1;
    return 1;
}

bool TruncateRestartClauseAST::isNull() const
{
    if ( t_continue_or_restart )
        return false;
    if ( t_identity )
        return false;
    return true;
}

void TruncateRestartClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int BeginTransactionStmtAST::firstToken() const
{
    if ( t_begin_or_start )
        return t_begin_or_start;
    if ( t_transaction )
        return t_transaction;
    if ( transOptions )
        return transOptions->firstToken();
    return 0;
}

int BeginTransactionStmtAST::lastToken() const
{
    if ( transOptions )
        return transOptions->lastToken();
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_begin_or_start )
        return t_begin_or_start + 1;
    return 1;
}

bool BeginTransactionStmtAST::isNull() const
{
    if ( t_begin_or_start )
        return false;
    if ( t_transaction )
        return false;
    if ( transOptions )
        return false;
    return true;
}

void BeginTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(transOptions, visitor);
    }
    visitor->endVisit(this);
}

int SavepointTransactionStmtAST::firstToken() const
{
    if ( t_savepoint )
        return t_savepoint;
    if ( t_savepoint_label )
        return t_savepoint_label;
    return 0;
}

int SavepointTransactionStmtAST::lastToken() const
{
    if ( t_savepoint_label )
        return t_savepoint_label + 1;
    if ( t_savepoint )
        return t_savepoint + 1;
    return 1;
}

bool SavepointTransactionStmtAST::isNull() const
{
    if ( t_savepoint )
        return false;
    if ( t_savepoint_label )
        return false;
    return true;
}

void SavepointTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ReleaseTransactionStmtAST::firstToken() const
{
    if ( t_release )
        return t_release;
    if ( t_transaction )
        return t_transaction;
    if ( t_savepoint_label )
        return t_savepoint_label;
    return 0;
}

int ReleaseTransactionStmtAST::lastToken() const
{
    if ( t_savepoint_label )
        return t_savepoint_label + 1;
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_release )
        return t_release + 1;
    return 1;
}

bool ReleaseTransactionStmtAST::isNull() const
{
    if ( t_release )
        return false;
    if ( t_transaction )
        return false;
    if ( t_savepoint_label )
        return false;
    return true;
}

void ReleaseTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommitTransactionStmtAST::firstToken() const
{
    if ( t_commit_or_end )
        return t_commit_or_end;
    if ( t_transaction )
        return t_transaction;
    return 0;
}

int CommitTransactionStmtAST::lastToken() const
{
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_commit_or_end )
        return t_commit_or_end + 1;
    return 1;
}

bool CommitTransactionStmtAST::isNull() const
{
    if ( t_commit_or_end )
        return false;
    if ( t_transaction )
        return false;
    return true;
}

void CommitTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RollbackTransactionStmtAST::firstToken() const
{
    if ( t_rollback_or_abort )
        return t_rollback_or_abort;
    if ( t_transaction )
        return t_transaction;
    if ( t_to )
        return t_to;
    if ( t_savepoint )
        return t_savepoint;
    if ( t_savepoint_label )
        return t_savepoint_label;
    return 0;
}

int RollbackTransactionStmtAST::lastToken() const
{
    if ( t_savepoint_label )
        return t_savepoint_label + 1;
    if ( t_savepoint )
        return t_savepoint + 1;
    if ( t_to )
        return t_to + 1;
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_rollback_or_abort )
        return t_rollback_or_abort + 1;
    return 1;
}

bool RollbackTransactionStmtAST::isNull() const
{
    if ( t_rollback_or_abort )
        return false;
    if ( t_transaction )
        return false;
    if ( t_to )
        return false;
    if ( t_savepoint )
        return false;
    if ( t_savepoint_label )
        return false;
    return true;
}

void RollbackTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int PrepareTransactionStmtAST::firstToken() const
{
    if ( t_prepare )
        return t_prepare;
    if ( t_transaction )
        return t_transaction;
    if ( t_pre_trans_label )
        return t_pre_trans_label;
    return 0;
}

int PrepareTransactionStmtAST::lastToken() const
{
    if ( t_pre_trans_label )
        return t_pre_trans_label + 1;
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_prepare )
        return t_prepare + 1;
    return 1;
}

bool PrepareTransactionStmtAST::isNull() const
{
    if ( t_prepare )
        return false;
    if ( t_transaction )
        return false;
    if ( t_pre_trans_label )
        return false;
    return true;
}

void PrepareTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommitPrepTransactionStmtAST::firstToken() const
{
    if ( t_commit )
        return t_commit;
    if ( t_prepared )
        return t_prepared;
    if ( t_pre_trans_label )
        return t_pre_trans_label;
    return 0;
}

int CommitPrepTransactionStmtAST::lastToken() const
{
    if ( t_pre_trans_label )
        return t_pre_trans_label + 1;
    if ( t_prepared )
        return t_prepared + 1;
    if ( t_commit )
        return t_commit + 1;
    return 1;
}

bool CommitPrepTransactionStmtAST::isNull() const
{
    if ( t_commit )
        return false;
    if ( t_prepared )
        return false;
    if ( t_pre_trans_label )
        return false;
    return true;
}

void CommitPrepTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RollPrepTransactionStmtAST::firstToken() const
{
    if ( t_rollback )
        return t_rollback;
    if ( t_prepared )
        return t_prepared;
    if ( t_pre_trans_label )
        return t_pre_trans_label;
    return 0;
}

int RollPrepTransactionStmtAST::lastToken() const
{
    if ( t_pre_trans_label )
        return t_pre_trans_label + 1;
    if ( t_prepared )
        return t_prepared + 1;
    if ( t_rollback )
        return t_rollback + 1;
    return 1;
}

bool RollPrepTransactionStmtAST::isNull() const
{
    if ( t_rollback )
        return false;
    if ( t_prepared )
        return false;
    if ( t_pre_trans_label )
        return false;
    return true;
}

void RollPrepTransactionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ColSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_column )
        return t_column;
    if ( column && !column->isNull() )
        return column->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int ColSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( column && !column->isNull() )
        return column->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool ColSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_column )
        return false;
    if ( column )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void ColSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(column, visitor);
    }
    visitor->endVisit(this);
}

int SequenceSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_sequence )
        return t_sequence;
    if ( seqIdent && !seqIdent->isNull() )
        return seqIdent->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int SequenceSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( seqIdent && !seqIdent->isNull() )
        return seqIdent->lastToken();
    if ( t_sequence )
        return t_sequence + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool SequenceSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_sequence )
        return false;
    if ( seqIdent )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void SequenceSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(seqIdent, visitor);
    }
    visitor->endVisit(this);
}

int TableSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_foreign_or_materialized )
        return t_foreign_or_materialized;
    if ( t_table_or_view )
        return t_table_or_view;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int TableSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_table_or_view )
        return t_table_or_view + 1;
    if ( t_foreign_or_materialized )
        return t_foreign_or_materialized + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool TableSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_foreign_or_materialized )
        return false;
    if ( t_table_or_view )
        return false;
    if ( ident )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void TableSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int AggrSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_aggregate )
        return t_aggregate;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int AggrSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool AggrSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_aggregate )
        return false;
    if ( funcRef )
        return false;
    if ( args )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void AggrSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(funcRef, visitor);
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int FuncSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_function )
        return t_function;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int FuncSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool FuncSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_function )
        return false;
    if ( funcRef )
        return false;
    if ( argTypes )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void FuncSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(funcRef, visitor);
        accept(argTypes, visitor);
    }
    visitor->endVisit(this);
}

int LargeObjSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_large )
        return t_large;
    if ( t_object )
        return t_object;
    if ( t_sp_num )
        return t_sp_num;
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int LargeObjSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( t_sp_num )
        return t_sp_num + 1;
    if ( t_object )
        return t_object + 1;
    if ( t_large )
        return t_large + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool LargeObjSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_large )
        return false;
    if ( t_object )
        return false;
    if ( t_sp_num )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void LargeObjSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
    }
    visitor->endVisit(this);
}

int LangSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int LangSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool LangSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( ident )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void LangSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int SchemaSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_schema )
        return t_schema;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int SchemaSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool SchemaSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_schema )
        return false;
    if ( schemaName )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void SchemaSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(schemaName, visitor);
    }
    visitor->endVisit(this);
}

int RoleSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( t_role )
        return t_role;
    if ( roleId && !roleId->isNull() )
        return roleId->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int RoleSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( roleId && !roleId->isNull() )
        return roleId->lastToken();
    if ( t_role )
        return t_role + 1;
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool RoleSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( t_role )
        return false;
    if ( roleId )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void RoleSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(roleId, visitor);
    }
    visitor->endVisit(this);
}

int CommonSecLabelStmtAST::firstToken() const
{
    if ( t_security )
        return t_security;
    if ( t_label )
        return t_label;
    if ( provider && !provider->isNull() )
        return provider->firstToken();
    if ( t_on )
        return t_on;
    if ( secType && !secType->isNull() )
        return secType->firstToken();
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_is )
        return t_is;
    if ( t_seq_label )
        return t_seq_label;
    return 0;
}

int CommonSecLabelStmtAST::lastToken() const
{
    if ( t_seq_label )
        return t_seq_label + 1;
    if ( t_is )
        return t_is + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( secType && !secType->isNull() )
        return secType->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( provider && !provider->isNull() )
        return provider->lastToken();
    if ( t_label )
        return t_label + 1;
    if ( t_security )
        return t_security + 1;
    return 1;
}

bool CommonSecLabelStmtAST::isNull() const
{
    if ( t_security )
        return false;
    if ( t_label )
        return false;
    if ( provider )
        return false;
    if ( t_on )
        return false;
    if ( secType )
        return false;
    if ( ident )
        return false;
    if ( t_is )
        return false;
    if ( t_seq_label )
        return false;
    return true;
}

void CommonSecLabelStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(provider, visitor);
        accept(secType, visitor);
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int SecurityProviderAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_prov_id )
        return t_prov_id;
    return 0;
}

int SecurityProviderAST::lastToken() const
{
    if ( t_prov_id )
        return t_prov_id + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool SecurityProviderAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_prov_id )
        return false;
    return true;
}

void SecurityProviderAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommonSecurityTypeAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    return 0;
}

int CommonSecurityTypeAST::lastToken() const
{
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool CommonSecurityTypeAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    return true;
}

void CommonSecurityTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RenameTypeStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_type )
        return t_type;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameTypeStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTypeStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_type )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameTypeStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameTypeAttrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_type )
        return t_type;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_attribute )
        return t_attribute;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameTypeAttrStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_attribute )
        return t_attribute + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTypeAttrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_type )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_attribute )
        return false;
    if ( oldName )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameTypeAttrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameRuleStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_rule )
        return t_rule;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameRuleStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->lastToken();
    if ( t_rule )
        return t_rule + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameRuleStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_rule )
        return false;
    if ( ruleName )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameRuleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ruleName, visitor);
        accept(tableIdent, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameTriggerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameTriggerStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTriggerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameTriggerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(tableIdent, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_schema )
        return t_schema;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameSchemaStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_schema )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameServerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_server )
        return t_server;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameServerStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameServerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_server )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameServerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameEvtTrgStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_event )
        return t_event;
    if ( t_trigger )
        return t_trigger;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameEvtTrgStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_event )
        return t_event + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameEvtTrgStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_event )
        return false;
    if ( t_trigger )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameEvtTrgStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameRoleStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_role_or_user )
        return t_role_or_user;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameRoleStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_role_or_user )
        return t_role_or_user + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameRoleStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_role_or_user )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameTablespaceStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_tablespace )
        return t_tablespace;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameTablespaceStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTablespaceStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_tablespace )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameTablespaceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int ChgReloptsTablespaceStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    if ( t_set_or_reset )
        return t_set_or_reset;
    if ( relOptions && !relOptions->isNull() )
        return relOptions->firstToken();
    return 0;
}

int ChgReloptsTablespaceStmtAST::lastToken() const
{
    if ( relOptions && !relOptions->isNull() )
        return relOptions->lastToken();
    if ( t_set_or_reset )
        return t_set_or_reset + 1;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool ChgReloptsTablespaceStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    if ( t_set_or_reset )
        return false;
    if ( relOptions )
        return false;
    return true;
}

void ChgReloptsTablespaceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
        accept(relOptions, visitor);
    }
    visitor->endVisit(this);
}

int RenameSearchStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_sp_obj )
        return t_sp_obj;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameSearchStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_sp_obj )
        return t_sp_obj + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameSearchStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_sp_obj )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameSearchStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameTableStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameTableStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTableStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameTableColStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( rel && !rel->isNull() )
        return rel->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_column )
        return t_column;
    if ( oldColName && !oldColName->isNull() )
        return oldColName->firstToken();
    if ( t_to )
        return t_to;
    if ( newColName && !newColName->isNull() )
        return newColName->firstToken();
    return 0;
}

int RenameTableColStmtAST::lastToken() const
{
    if ( newColName && !newColName->isNull() )
        return newColName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( oldColName && !oldColName->isNull() )
        return oldColName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( rel && !rel->isNull() )
        return rel->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTableColStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( rel )
        return false;
    if ( t_rename )
        return false;
    if ( t_column )
        return false;
    if ( oldColName )
        return false;
    if ( t_to )
        return false;
    if ( newColName )
        return false;
    return true;
}

void RenameTableColStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rel, visitor);
        accept(oldColName, visitor);
        accept(newColName, visitor);
    }
    visitor->endVisit(this);
}

int RenameTableConstrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_table )
        return t_table;
    if ( rel && !rel->isNull() )
        return rel->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_constraint )
        return t_constraint;
    if ( oldConstrName && !oldConstrName->isNull() )
        return oldConstrName->firstToken();
    if ( t_to )
        return t_to;
    if ( newConstrName && !newConstrName->isNull() )
        return newConstrName->firstToken();
    return 0;
}

int RenameTableConstrStmtAST::lastToken() const
{
    if ( newConstrName && !newConstrName->isNull() )
        return newConstrName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( oldConstrName && !oldConstrName->isNull() )
        return oldConstrName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( rel && !rel->isNull() )
        return rel->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameTableConstrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_table )
        return false;
    if ( rel )
        return false;
    if ( t_rename )
        return false;
    if ( t_constraint )
        return false;
    if ( oldConstrName )
        return false;
    if ( t_to )
        return false;
    if ( newConstrName )
        return false;
    return true;
}

void RenameTableConstrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rel, visitor);
        accept(oldConstrName, visitor);
        accept(newConstrName, visitor);
    }
    visitor->endVisit(this);
}

int RenameMatViewColStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_column )
        return t_column;
    if ( oldColName && !oldColName->isNull() )
        return oldColName->firstToken();
    if ( t_to )
        return t_to;
    if ( newColName && !newColName->isNull() )
        return newColName->firstToken();
    return 0;
}

int RenameMatViewColStmtAST::lastToken() const
{
    if ( newColName && !newColName->isNull() )
        return newColName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( oldColName && !oldColName->isNull() )
        return oldColName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameMatViewColStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( viewIdent )
        return false;
    if ( t_rename )
        return false;
    if ( t_column )
        return false;
    if ( oldColName )
        return false;
    if ( t_to )
        return false;
    if ( newColName )
        return false;
    return true;
}

void RenameMatViewColStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewIdent, visitor);
        accept(oldColName, visitor);
        accept(newColName, visitor);
    }
    visitor->endVisit(this);
}

int RenameSequenceStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_sequence )
        return t_sequence;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameSequenceStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_sequence )
        return t_sequence + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameSequenceStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_sequence )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameSequenceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameViewStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_matirialized )
        return t_matirialized;
    if ( t_view )
        return t_view;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameViewStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_view )
        return t_view + 1;
    if ( t_matirialized )
        return t_matirialized + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameViewStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_matirialized )
        return false;
    if ( t_view )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameViewStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameIndexStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_index )
        return t_index;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameIndexStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_index )
        return t_index + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameIndexStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_index )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameIndexStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameCollationStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_collation )
        return t_collation;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameCollationStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_collation )
        return t_collation + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameCollationStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_collation )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameCollationStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameConversionStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_conversion )
        return t_conversion;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameConversionStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_conversion )
        return t_conversion + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameConversionStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_conversion )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameConversionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameDomainStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameDomainStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameDomainStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameDomainStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameDomainConstrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_constraint )
        return t_constraint;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameDomainConstrStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameDomainConstrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( ident )
        return false;
    if ( t_rename )
        return false;
    if ( t_constraint )
        return false;
    if ( constraintName )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameDomainConstrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(constraintName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameDatabaseStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_database )
        return t_database;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameDatabaseStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameDatabaseStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_database )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameDatabaseStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameFdwStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameFdwStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameFdwStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameFdwStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameFuncStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_function )
        return t_function;
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameFuncStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameFuncStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_function )
        return false;
    if ( args )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameFuncStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameOpUsingStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( t_class_or_family )
        return t_class_or_family;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameOpUsingStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_class_or_family )
        return t_class_or_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameOpUsingStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( t_class_or_family )
        return false;
    if ( ident )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameOpUsingStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameGroupRoleStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_group )
        return t_group;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameGroupRoleStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_group )
        return t_group + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameGroupRoleStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_group )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameGroupRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameLangStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( oldName && !oldName->isNull() )
        return oldName->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameLangStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( oldName && !oldName->isNull() )
        return oldName->lastToken();
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameLangStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( oldName )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameLangStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oldName, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int RenameAggrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_aggregate )
        return t_aggregate;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( t_rename )
        return t_rename;
    if ( t_to )
        return t_to;
    if ( newName && !newName->isNull() )
        return newName->firstToken();
    return 0;
}

int RenameAggrStmtAST::lastToken() const
{
    if ( newName && !newName->isNull() )
        return newName->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_rename )
        return t_rename + 1;
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool RenameAggrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_aggregate )
        return false;
    if ( funcRef )
        return false;
    if ( args )
        return false;
    if ( t_rename )
        return false;
    if ( t_to )
        return false;
    if ( newName )
        return false;
    return true;
}

void RenameAggrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(args, visitor);
        accept(newName, visitor);
    }
    visitor->endVisit(this);
}

int ReindexTableStmtAST::firstToken() const
{
    if ( t_reindex )
        return t_reindex;
    if ( t_table )
        return t_table;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_force )
        return t_force;
    return 0;
}

int ReindexTableStmtAST::lastToken() const
{
    if ( t_force )
        return t_force + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( t_reindex )
        return t_reindex + 1;
    return 1;
}

bool ReindexTableStmtAST::isNull() const
{
    if ( t_reindex )
        return false;
    if ( t_table )
        return false;
    if ( tableIdent )
        return false;
    if ( t_force )
        return false;
    return true;
}

void ReindexTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int ReindexIndexStmtAST::firstToken() const
{
    if ( t_reindex )
        return t_reindex;
    if ( t_index )
        return t_index;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_force )
        return t_force;
    return 0;
}

int ReindexIndexStmtAST::lastToken() const
{
    if ( t_force )
        return t_force + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_index )
        return t_index + 1;
    if ( t_reindex )
        return t_reindex + 1;
    return 1;
}

bool ReindexIndexStmtAST::isNull() const
{
    if ( t_reindex )
        return false;
    if ( t_index )
        return false;
    if ( tableIdent )
        return false;
    if ( t_force )
        return false;
    return true;
}

void ReindexIndexStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int ReindexSystemStmtAST::firstToken() const
{
    if ( t_reindex )
        return t_reindex;
    if ( t_system )
        return t_system;
    if ( systemName && !systemName->isNull() )
        return systemName->firstToken();
    if ( t_force )
        return t_force;
    return 0;
}

int ReindexSystemStmtAST::lastToken() const
{
    if ( t_force )
        return t_force + 1;
    if ( systemName && !systemName->isNull() )
        return systemName->lastToken();
    if ( t_system )
        return t_system + 1;
    if ( t_reindex )
        return t_reindex + 1;
    return 1;
}

bool ReindexSystemStmtAST::isNull() const
{
    if ( t_reindex )
        return false;
    if ( t_system )
        return false;
    if ( systemName )
        return false;
    if ( t_force )
        return false;
    return true;
}

void ReindexSystemStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(systemName, visitor);
    }
    visitor->endVisit(this);
}

int ReindexDatabaseStmtAST::firstToken() const
{
    if ( t_reindex )
        return t_reindex;
    if ( t_database )
        return t_database;
    if ( dbName && !dbName->isNull() )
        return dbName->firstToken();
    if ( t_force )
        return t_force;
    return 0;
}

int ReindexDatabaseStmtAST::lastToken() const
{
    if ( t_force )
        return t_force + 1;
    if ( dbName && !dbName->isNull() )
        return dbName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_reindex )
        return t_reindex + 1;
    return 1;
}

bool ReindexDatabaseStmtAST::isNull() const
{
    if ( t_reindex )
        return false;
    if ( t_database )
        return false;
    if ( dbName )
        return false;
    if ( t_force )
        return false;
    return true;
}

void ReindexDatabaseStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dbName, visitor);
    }
    visitor->endVisit(this);
}

int PrepareStmtAST::firstToken() const
{
    if ( t_prepare )
        return t_prepare;
    if ( prepName && !prepName->isNull() )
        return prepName->firstToken();
    if ( prepareTypeClause && !prepareTypeClause->isNull() )
        return prepareTypeClause->firstToken();
    if ( t_as )
        return t_as;
    if ( stmt && !stmt->isNull() )
        return stmt->firstToken();
    return 0;
}

int PrepareStmtAST::lastToken() const
{
    if ( stmt && !stmt->isNull() )
        return stmt->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( prepareTypeClause && !prepareTypeClause->isNull() )
        return prepareTypeClause->lastToken();
    if ( prepName && !prepName->isNull() )
        return prepName->lastToken();
    if ( t_prepare )
        return t_prepare + 1;
    return 1;
}

bool PrepareStmtAST::isNull() const
{
    if ( t_prepare )
        return false;
    if ( prepName )
        return false;
    if ( prepareTypeClause )
        return false;
    if ( t_as )
        return false;
    if ( stmt )
        return false;
    return true;
}

void PrepareStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(prepName, visitor);
        accept(prepareTypeClause, visitor);
        accept(stmt, visitor);
    }
    visitor->endVisit(this);
}

int PrepareTypeClauseAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( types )
        return types->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int PrepareTypeClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( types )
        return types->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool PrepareTypeClauseAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( types )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void PrepareTypeClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(types, visitor);
    }
    visitor->endVisit(this);
}

int LockStmtAST::firstToken() const
{
    if ( t_lock )
        return t_lock;
    if ( t_table )
        return t_table;
    if ( reations )
        return reations->firstToken();
    if ( lockMode && !lockMode->isNull() )
        return lockMode->firstToken();
    if ( t_nowait )
        return t_nowait;
    return 0;
}

int LockStmtAST::lastToken() const
{
    if ( t_nowait )
        return t_nowait + 1;
    if ( lockMode && !lockMode->isNull() )
        return lockMode->lastToken();
    if ( reations )
        return reations->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( t_lock )
        return t_lock + 1;
    return 1;
}

bool LockStmtAST::isNull() const
{
    if ( t_lock )
        return false;
    if ( t_table )
        return false;
    if ( reations )
        return false;
    if ( lockMode )
        return false;
    if ( t_nowait )
        return false;
    return true;
}

void LockStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(reations, visitor);
        accept(lockMode, visitor);
    }
    visitor->endVisit(this);
}

int LockModeClauseAST::firstToken() const
{
    if ( t_in )
        return t_in;
    if ( lockType && !lockType->isNull() )
        return lockType->firstToken();
    if ( t_mode )
        return t_mode;
    return 0;
}

int LockModeClauseAST::lastToken() const
{
    if ( t_mode )
        return t_mode + 1;
    if ( lockType && !lockType->isNull() )
        return lockType->lastToken();
    if ( t_in )
        return t_in + 1;
    return 1;
}

bool LockModeClauseAST::isNull() const
{
    if ( t_in )
        return false;
    if ( lockType )
        return false;
    if ( t_mode )
        return false;
    return true;
}

void LockModeClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lockType, visitor);
    }
    visitor->endVisit(this);
}

int LockTypeAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    if ( t_sp3 )
        return t_sp3;
    return 0;
}

int LockTypeAST::lastToken() const
{
    if ( t_sp3 )
        return t_sp3 + 1;
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool LockTypeAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    if ( t_sp3 )
        return false;
    return true;
}

void LockTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DropAggrStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_aggregate )
        return t_aggregate;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropAggrStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropAggrStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_aggregate )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( funcRef )
        return false;
    if ( args )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropAggrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int DropFuncStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_function )
        return t_function;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropFuncStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_function )
        return t_function + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropFuncStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_function )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( funcRef )
        return false;
    if ( argTypes )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropFuncStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(argTypes, visitor);
    }
    visitor->endVisit(this);
}

int DropOperStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_operator )
        return t_operator;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropOperStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropOperStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_operator )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( operand )
        return false;
    if ( argTypes )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropOperStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(argTypes, visitor);
    }
    visitor->endVisit(this);
}

int LoadStmtAST::firstToken() const
{
    if ( t_load )
        return t_load;
    if ( t_file_name )
        return t_file_name;
    return 0;
}

int LoadStmtAST::lastToken() const
{
    if ( t_file_name )
        return t_file_name + 1;
    if ( t_load )
        return t_load + 1;
    return 1;
}

bool LoadStmtAST::isNull() const
{
    if ( t_load )
        return false;
    if ( t_file_name )
        return false;
    return true;
}

void LoadStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int NotifyStmtAST::firstToken() const
{
    if ( t_notify )
        return t_notify;
    if ( t_notify_label )
        return t_notify_label;
    if ( payload && !payload->isNull() )
        return payload->firstToken();
    return 0;
}

int NotifyStmtAST::lastToken() const
{
    if ( payload && !payload->isNull() )
        return payload->lastToken();
    if ( t_notify_label )
        return t_notify_label + 1;
    if ( t_notify )
        return t_notify + 1;
    return 1;
}

bool NotifyStmtAST::isNull() const
{
    if ( t_notify )
        return false;
    if ( t_notify_label )
        return false;
    if ( payload )
        return false;
    return true;
}

void NotifyStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(payload, visitor);
    }
    visitor->endVisit(this);
}

int NotifyPayloadAST::firstToken() const
{
    if ( t_comma )
        return t_comma;
    if ( t_sp_str )
        return t_sp_str;
    return 0;
}

int NotifyPayloadAST::lastToken() const
{
    if ( t_sp_str )
        return t_sp_str + 1;
    if ( t_comma )
        return t_comma + 1;
    return 1;
}

bool NotifyPayloadAST::isNull() const
{
    if ( t_comma )
        return false;
    if ( t_sp_str )
        return false;
    return true;
}

void NotifyPayloadAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ListenStmtAST::firstToken() const
{
    if ( t_listen )
        return t_listen;
    if ( t_listen_label )
        return t_listen_label;
    return 0;
}

int ListenStmtAST::lastToken() const
{
    if ( t_listen_label )
        return t_listen_label + 1;
    if ( t_listen )
        return t_listen + 1;
    return 1;
}

bool ListenStmtAST::isNull() const
{
    if ( t_listen )
        return false;
    if ( t_listen_label )
        return false;
    return true;
}

void ListenStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UnlistenStmtAST::firstToken() const
{
    if ( t_unlisten )
        return t_unlisten;
    if ( t_star_or_ident )
        return t_star_or_ident;
    return 0;
}

int UnlistenStmtAST::lastToken() const
{
    if ( t_star_or_ident )
        return t_star_or_ident + 1;
    if ( t_unlisten )
        return t_unlisten + 1;
    return 1;
}

bool UnlistenStmtAST::isNull() const
{
    if ( t_unlisten )
        return false;
    if ( t_star_or_ident )
        return false;
    return true;
}

void UnlistenStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateIndexStatementAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_unique )
        return t_unique;
    if ( t_index )
        return t_index;
    if ( t_concurrently )
        return t_concurrently;
    if ( indexName && !indexName->isNull() )
        return indexName->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( accMethod && !accMethod->isNull() )
        return accMethod->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( elements )
        return elements->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( reopts && !reopts->isNull() )
        return reopts->firstToken();
    if ( tablespaceClause && !tablespaceClause->isNull() )
        return tablespaceClause->firstToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->firstToken();
    return 0;
}

int CreateIndexStatementAST::lastToken() const
{
    if ( whereClause && !whereClause->isNull() )
        return whereClause->lastToken();
    if ( tablespaceClause && !tablespaceClause->isNull() )
        return tablespaceClause->lastToken();
    if ( reopts && !reopts->isNull() )
        return reopts->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( elements )
        return elements->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( accMethod && !accMethod->isNull() )
        return accMethod->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( indexName && !indexName->isNull() )
        return indexName->lastToken();
    if ( t_concurrently )
        return t_concurrently + 1;
    if ( t_index )
        return t_index + 1;
    if ( t_unique )
        return t_unique + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateIndexStatementAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_unique )
        return false;
    if ( t_index )
        return false;
    if ( t_concurrently )
        return false;
    if ( indexName )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( accMethod )
        return false;
    if ( t_lparen )
        return false;
    if ( elements )
        return false;
    if ( t_rparen )
        return false;
    if ( reopts )
        return false;
    if ( tablespaceClause )
        return false;
    if ( whereClause )
        return false;
    return true;
}

void CreateIndexStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexName, visitor);
        accept(tableIdent, visitor);
        accept(accMethod, visitor);
        accept(elements, visitor);
        accept(reopts, visitor);
        accept(tablespaceClause, visitor);
        accept(whereClause, visitor);
    }
    visitor->endVisit(this);
}

int GrantRoleStmtAST::firstToken() const
{
    if ( t_grant )
        return t_grant;
    if ( privs )
        return privs->firstToken();
    if ( t_to )
        return t_to;
    if ( roles )
        return roles->firstToken();
    if ( withAdmin && !withAdmin->isNull() )
        return withAdmin->firstToken();
    if ( grantedBy && !grantedBy->isNull() )
        return grantedBy->firstToken();
    return 0;
}

int GrantRoleStmtAST::lastToken() const
{
    if ( grantedBy && !grantedBy->isNull() )
        return grantedBy->lastToken();
    if ( withAdmin && !withAdmin->isNull() )
        return withAdmin->lastToken();
    if ( roles )
        return roles->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( privs )
        return privs->lastToken();
    if ( t_grant )
        return t_grant + 1;
    return 1;
}

bool GrantRoleStmtAST::isNull() const
{
    if ( t_grant )
        return false;
    if ( privs )
        return false;
    if ( t_to )
        return false;
    if ( roles )
        return false;
    if ( withAdmin )
        return false;
    if ( grantedBy )
        return false;
    return true;
}

void GrantRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
        accept(roles, visitor);
        accept(withAdmin, visitor);
        accept(grantedBy, visitor);
    }
    visitor->endVisit(this);
}

int RevokeRoleStmtAST::firstToken() const
{
    if ( t_revoke )
        return t_revoke;
    if ( t_admin )
        return t_admin;
    if ( t_option )
        return t_option;
    if ( t_for )
        return t_for;
    if ( privs )
        return privs->firstToken();
    if ( t_from )
        return t_from;
    if ( roles )
        return roles->firstToken();
    if ( grantedBy && !grantedBy->isNull() )
        return grantedBy->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int RevokeRoleStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( grantedBy && !grantedBy->isNull() )
        return grantedBy->lastToken();
    if ( roles )
        return roles->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( privs )
        return privs->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_option )
        return t_option + 1;
    if ( t_admin )
        return t_admin + 1;
    if ( t_revoke )
        return t_revoke + 1;
    return 1;
}

bool RevokeRoleStmtAST::isNull() const
{
    if ( t_revoke )
        return false;
    if ( t_admin )
        return false;
    if ( t_option )
        return false;
    if ( t_for )
        return false;
    if ( privs )
        return false;
    if ( t_from )
        return false;
    if ( roles )
        return false;
    if ( grantedBy )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void RevokeRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
        accept(roles, visitor);
        accept(grantedBy, visitor);
    }
    visitor->endVisit(this);
}

int WithAdminOptionAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( t_admin )
        return t_admin;
    if ( t_option )
        return t_option;
    return 0;
}

int WithAdminOptionAST::lastToken() const
{
    if ( t_option )
        return t_option + 1;
    if ( t_admin )
        return t_admin + 1;
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithAdminOptionAST::isNull() const
{
    if ( t_with )
        return false;
    if ( t_admin )
        return false;
    if ( t_option )
        return false;
    return true;
}

void WithAdminOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int GrantedByClauseAST::firstToken() const
{
    if ( t_granted )
        return t_granted;
    if ( t_by )
        return t_by;
    if ( granter && !granter->isNull() )
        return granter->firstToken();
    return 0;
}

int GrantedByClauseAST::lastToken() const
{
    if ( granter && !granter->isNull() )
        return granter->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_granted )
        return t_granted + 1;
    return 1;
}

bool GrantedByClauseAST::isNull() const
{
    if ( t_granted )
        return false;
    if ( t_by )
        return false;
    if ( granter )
        return false;
    return true;
}

void GrantedByClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(granter, visitor);
    }
    visitor->endVisit(this);
}

int GrantStmtAST::firstToken() const
{
    if ( t_grant )
        return t_grant;
    if ( privs && !privs->isNull() )
        return privs->firstToken();
    if ( t_on )
        return t_on;
    if ( target && !target->isNull() )
        return target->firstToken();
    if ( t_to )
        return t_to;
    if ( grantees )
        return grantees->firstToken();
    if ( withGrantOption && !withGrantOption->isNull() )
        return withGrantOption->firstToken();
    return 0;
}

int GrantStmtAST::lastToken() const
{
    if ( withGrantOption && !withGrantOption->isNull() )
        return withGrantOption->lastToken();
    if ( grantees )
        return grantees->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( target && !target->isNull() )
        return target->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( privs && !privs->isNull() )
        return privs->lastToken();
    if ( t_grant )
        return t_grant + 1;
    return 1;
}

bool GrantStmtAST::isNull() const
{
    if ( t_grant )
        return false;
    if ( privs )
        return false;
    if ( t_on )
        return false;
    if ( target )
        return false;
    if ( t_to )
        return false;
    if ( grantees )
        return false;
    if ( withGrantOption )
        return false;
    return true;
}

void GrantStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
        accept(target, visitor);
        accept(grantees, visitor);
        accept(withGrantOption, visitor);
    }
    visitor->endVisit(this);
}

int RevokeStmtAST::firstToken() const
{
    if ( t_revoke )
        return t_revoke;
    if ( t_grant )
        return t_grant;
    if ( t_option )
        return t_option;
    if ( t_for )
        return t_for;
    if ( privs && !privs->isNull() )
        return privs->firstToken();
    if ( t_on )
        return t_on;
    if ( target && !target->isNull() )
        return target->firstToken();
    if ( t_from )
        return t_from;
    if ( grantees )
        return grantees->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int RevokeStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( grantees )
        return grantees->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( target && !target->isNull() )
        return target->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( privs && !privs->isNull() )
        return privs->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_option )
        return t_option + 1;
    if ( t_grant )
        return t_grant + 1;
    if ( t_revoke )
        return t_revoke + 1;
    return 1;
}

bool RevokeStmtAST::isNull() const
{
    if ( t_revoke )
        return false;
    if ( t_grant )
        return false;
    if ( t_option )
        return false;
    if ( t_for )
        return false;
    if ( privs )
        return false;
    if ( t_on )
        return false;
    if ( target )
        return false;
    if ( t_from )
        return false;
    if ( grantees )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void RevokeStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
        accept(target, visitor);
        accept(grantees, visitor);
    }
    visitor->endVisit(this);
}

int TablePrivilegeTargetAST::firstToken() const
{
    if ( t_table )
        return t_table;
    if ( tables )
        return tables->firstToken();
    return 0;
}

int TablePrivilegeTargetAST::lastToken() const
{
    if ( tables )
        return tables->lastToken();
    if ( t_table )
        return t_table + 1;
    return 1;
}

bool TablePrivilegeTargetAST::isNull() const
{
    if ( t_table )
        return false;
    if ( tables )
        return false;
    return true;
}

void TablePrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tables, visitor);
    }
    visitor->endVisit(this);
}

int SequencePrivilegeTargetAST::firstToken() const
{
    if ( t_sequence )
        return t_sequence;
    if ( seqs )
        return seqs->firstToken();
    return 0;
}

int SequencePrivilegeTargetAST::lastToken() const
{
    if ( seqs )
        return seqs->lastToken();
    if ( t_sequence )
        return t_sequence + 1;
    return 1;
}

bool SequencePrivilegeTargetAST::isNull() const
{
    if ( t_sequence )
        return false;
    if ( seqs )
        return false;
    return true;
}

void SequencePrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(seqs, visitor);
    }
    visitor->endVisit(this);
}

int FdwPrivilegeTargetAST::firstToken() const
{
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( wrappers )
        return wrappers->firstToken();
    return 0;
}

int FdwPrivilegeTargetAST::lastToken() const
{
    if ( wrappers )
        return wrappers->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    return 1;
}

bool FdwPrivilegeTargetAST::isNull() const
{
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( wrappers )
        return false;
    return true;
}

void FdwPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(wrappers, visitor);
    }
    visitor->endVisit(this);
}

int ForeignSrvPrivilegeTargetAST::firstToken() const
{
    if ( t_foreign )
        return t_foreign;
    if ( t_server )
        return t_server;
    if ( servers )
        return servers->firstToken();
    return 0;
}

int ForeignSrvPrivilegeTargetAST::lastToken() const
{
    if ( servers )
        return servers->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( t_foreign )
        return t_foreign + 1;
    return 1;
}

bool ForeignSrvPrivilegeTargetAST::isNull() const
{
    if ( t_foreign )
        return false;
    if ( t_server )
        return false;
    if ( servers )
        return false;
    return true;
}

void ForeignSrvPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(servers, visitor);
    }
    visitor->endVisit(this);
}

int FuncPrivilegeTargetAST::firstToken() const
{
    if ( t_function )
        return t_function;
    if ( functions )
        return functions->firstToken();
    return 0;
}

int FuncPrivilegeTargetAST::lastToken() const
{
    if ( functions )
        return functions->lastToken();
    if ( t_function )
        return t_function + 1;
    return 1;
}

bool FuncPrivilegeTargetAST::isNull() const
{
    if ( t_function )
        return false;
    if ( functions )
        return false;
    return true;
}

void FuncPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functions, visitor);
    }
    visitor->endVisit(this);
}

int DbPrivilegeTargetAST::firstToken() const
{
    if ( t_database )
        return t_database;
    if ( databases )
        return databases->firstToken();
    return 0;
}

int DbPrivilegeTargetAST::lastToken() const
{
    if ( databases )
        return databases->lastToken();
    if ( t_database )
        return t_database + 1;
    return 1;
}

bool DbPrivilegeTargetAST::isNull() const
{
    if ( t_database )
        return false;
    if ( databases )
        return false;
    return true;
}

void DbPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(databases, visitor);
    }
    visitor->endVisit(this);
}

int DomainPrivilegeTargetAST::firstToken() const
{
    if ( t_domain )
        return t_domain;
    if ( domains )
        return domains->firstToken();
    return 0;
}

int DomainPrivilegeTargetAST::lastToken() const
{
    if ( domains )
        return domains->lastToken();
    if ( t_domain )
        return t_domain + 1;
    return 1;
}

bool DomainPrivilegeTargetAST::isNull() const
{
    if ( t_domain )
        return false;
    if ( domains )
        return false;
    return true;
}

void DomainPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domains, visitor);
    }
    visitor->endVisit(this);
}

int LangPrivilegeTargetAST::firstToken() const
{
    if ( t_language )
        return t_language;
    if ( langs )
        return langs->firstToken();
    return 0;
}

int LangPrivilegeTargetAST::lastToken() const
{
    if ( langs )
        return langs->lastToken();
    if ( t_language )
        return t_language + 1;
    return 1;
}

bool LangPrivilegeTargetAST::isNull() const
{
    if ( t_language )
        return false;
    if ( langs )
        return false;
    return true;
}

void LangPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(langs, visitor);
    }
    visitor->endVisit(this);
}

int LargeObjPrivilegeTargetAST::firstToken() const
{
    if ( t_large )
        return t_large;
    if ( t_object )
        return t_object;
    if ( nums )
        return nums->firstToken();
    return 0;
}

int LargeObjPrivilegeTargetAST::lastToken() const
{
    if ( nums )
        return nums->lastToken();
    if ( t_object )
        return t_object + 1;
    if ( t_large )
        return t_large + 1;
    return 1;
}

bool LargeObjPrivilegeTargetAST::isNull() const
{
    if ( t_large )
        return false;
    if ( t_object )
        return false;
    if ( nums )
        return false;
    return true;
}

void LargeObjPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(nums, visitor);
    }
    visitor->endVisit(this);
}

int SchemaPrivilegeTargetAST::firstToken() const
{
    if ( t_schema )
        return t_schema;
    if ( schemas )
        return schemas->firstToken();
    return 0;
}

int SchemaPrivilegeTargetAST::lastToken() const
{
    if ( schemas )
        return schemas->lastToken();
    if ( t_schema )
        return t_schema + 1;
    return 1;
}

bool SchemaPrivilegeTargetAST::isNull() const
{
    if ( t_schema )
        return false;
    if ( schemas )
        return false;
    return true;
}

void SchemaPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemas, visitor);
    }
    visitor->endVisit(this);
}

int TblspacePrivilegeTargetAST::firstToken() const
{
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaces )
        return tablespaces->firstToken();
    return 0;
}

int TblspacePrivilegeTargetAST::lastToken() const
{
    if ( tablespaces )
        return tablespaces->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    return 1;
}

bool TblspacePrivilegeTargetAST::isNull() const
{
    if ( t_tablespace )
        return false;
    if ( tablespaces )
        return false;
    return true;
}

void TblspacePrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaces, visitor);
    }
    visitor->endVisit(this);
}

int TypePrivilegeTargetAST::firstToken() const
{
    if ( t_type )
        return t_type;
    if ( types )
        return types->firstToken();
    return 0;
}

int TypePrivilegeTargetAST::lastToken() const
{
    if ( types )
        return types->lastToken();
    if ( t_type )
        return t_type + 1;
    return 1;
}

bool TypePrivilegeTargetAST::isNull() const
{
    if ( t_type )
        return false;
    if ( types )
        return false;
    return true;
}

void TypePrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(types, visitor);
    }
    visitor->endVisit(this);
}

int AllInSchPrivilegeTargetAST::firstToken() const
{
    if ( t_all )
        return t_all;
    if ( t_sp_obj )
        return t_sp_obj;
    if ( t_in )
        return t_in;
    if ( t_schema )
        return t_schema;
    if ( schemas )
        return schemas->firstToken();
    return 0;
}

int AllInSchPrivilegeTargetAST::lastToken() const
{
    if ( schemas )
        return schemas->lastToken();
    if ( t_schema )
        return t_schema + 1;
    if ( t_in )
        return t_in + 1;
    if ( t_sp_obj )
        return t_sp_obj + 1;
    if ( t_all )
        return t_all + 1;
    return 1;
}

bool AllInSchPrivilegeTargetAST::isNull() const
{
    if ( t_all )
        return false;
    if ( t_sp_obj )
        return false;
    if ( t_in )
        return false;
    if ( t_schema )
        return false;
    if ( schemas )
        return false;
    return true;
}

void AllInSchPrivilegeTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemas, visitor);
    }
    visitor->endVisit(this);
}

int NumericAST::firstToken() const
{
    if ( t_sp_num )
        return t_sp_num;
    return 0;
}

int NumericAST::lastToken() const
{
    if ( t_sp_num )
        return t_sp_num + 1;
    return 1;
}

bool NumericAST::isNull() const
{
    if ( t_sp_num )
        return false;
    return true;
}

void NumericAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int FetchStmtAST::firstToken() const
{
    if ( t_fetch_or_move )
        return t_fetch_or_move;
    if ( fetchClause && !fetchClause->isNull() )
        return fetchClause->firstToken();
    return 0;
}

int FetchStmtAST::lastToken() const
{
    if ( fetchClause && !fetchClause->isNull() )
        return fetchClause->lastToken();
    if ( t_fetch_or_move )
        return t_fetch_or_move + 1;
    return 1;
}

bool FetchStmtAST::isNull() const
{
    if ( t_fetch_or_move )
        return false;
    if ( fetchClause )
        return false;
    return true;
}

void FetchStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fetchClause, visitor);
    }
    visitor->endVisit(this);
}

int FetchClauseAST::firstToken() const
{
    if ( t_sp_step )
        return t_sp_step;
    if ( t_sp_step_spec )
        return t_sp_step_spec;
    if ( t_from_or_in )
        return t_from_or_in;
    if ( cursorName && !cursorName->isNull() )
        return cursorName->firstToken();
    return 0;
}

int FetchClauseAST::lastToken() const
{
    if ( cursorName && !cursorName->isNull() )
        return cursorName->lastToken();
    if ( t_from_or_in )
        return t_from_or_in + 1;
    if ( t_sp_step_spec )
        return t_sp_step_spec + 1;
    if ( t_sp_step )
        return t_sp_step + 1;
    return 1;
}

bool FetchClauseAST::isNull() const
{
    if ( t_sp_step )
        return false;
    if ( t_sp_step_spec )
        return false;
    if ( t_from_or_in )
        return false;
    if ( cursorName )
        return false;
    return true;
}

void FetchClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(cursorName, visitor);
    }
    visitor->endVisit(this);
}

int CommonExplainStmtAST::firstToken() const
{
    if ( t_explain )
        return t_explain;
    if ( t_analize )
        return t_analize;
    if ( t_verbose )
        return t_verbose;
    if ( explStmt && !explStmt->isNull() )
        return explStmt->firstToken();
    return 0;
}

int CommonExplainStmtAST::lastToken() const
{
    if ( explStmt && !explStmt->isNull() )
        return explStmt->lastToken();
    if ( t_verbose )
        return t_verbose + 1;
    if ( t_analize )
        return t_analize + 1;
    if ( t_explain )
        return t_explain + 1;
    return 1;
}

bool CommonExplainStmtAST::isNull() const
{
    if ( t_explain )
        return false;
    if ( t_analize )
        return false;
    if ( t_verbose )
        return false;
    if ( explStmt )
        return false;
    return true;
}

void CommonExplainStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(explStmt, visitor);
    }
    visitor->endVisit(this);
}

int OptExplainStmtAST::firstToken() const
{
    if ( t_explain )
        return t_explain;
    if ( t_lparen )
        return t_lparen;
    if ( options )
        return options->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( explStmt && !explStmt->isNull() )
        return explStmt->firstToken();
    return 0;
}

int OptExplainStmtAST::lastToken() const
{
    if ( explStmt && !explStmt->isNull() )
        return explStmt->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( options )
        return options->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_explain )
        return t_explain + 1;
    return 1;
}

bool OptExplainStmtAST::isNull() const
{
    if ( t_explain )
        return false;
    if ( t_lparen )
        return false;
    if ( options )
        return false;
    if ( t_rparen )
        return false;
    if ( explStmt )
        return false;
    return true;
}

void OptExplainStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(options, visitor);
        accept(explStmt, visitor);
    }
    visitor->endVisit(this);
}

int ExplainOptionAST::firstToken() const
{
    if ( t_sp_name )
        return t_sp_name;
    if ( t_sp_arg )
        return t_sp_arg;
    return 0;
}

int ExplainOptionAST::lastToken() const
{
    if ( t_sp_arg )
        return t_sp_arg + 1;
    if ( t_sp_name )
        return t_sp_name + 1;
    return 1;
}

bool ExplainOptionAST::isNull() const
{
    if ( t_sp_name )
        return false;
    if ( t_sp_arg )
        return false;
    return true;
}

void ExplainOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RefreshMatViewStmtAST::firstToken() const
{
    if ( t_refresh )
        return t_refresh;
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->firstToken();
    if ( withData && !withData->isNull() )
        return withData->firstToken();
    return 0;
}

int RefreshMatViewStmtAST::lastToken() const
{
    if ( withData && !withData->isNull() )
        return withData->lastToken();
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->lastToken();
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    if ( t_refresh )
        return t_refresh + 1;
    return 1;
}

bool RefreshMatViewStmtAST::isNull() const
{
    if ( t_refresh )
        return false;
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( viewIdent )
        return false;
    if ( withData )
        return false;
    return true;
}

void RefreshMatViewStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewIdent, visitor);
        accept(withData, visitor);
    }
    visitor->endVisit(this);
}

int DropDbStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_database )
        return t_database;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( dbName && !dbName->isNull() )
        return dbName->firstToken();
    return 0;
}

int DropDbStmtAST::lastToken() const
{
    if ( dbName && !dbName->isNull() )
        return dbName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_database )
        return t_database + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropDbStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_database )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( dbName )
        return false;
    return true;
}

void DropDbStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dbName, visitor);
    }
    visitor->endVisit(this);
}

int DropUserMappingStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_user )
        return t_user;
    if ( t_mapping )
        return t_mapping;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( t_for )
        return t_for;
    if ( authIdent && !authIdent->isNull() )
        return authIdent->firstToken();
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    return 0;
}

int DropUserMappingStmtAST::lastToken() const
{
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( authIdent && !authIdent->isNull() )
        return authIdent->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_mapping )
        return t_mapping + 1;
    if ( t_user )
        return t_user + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropUserMappingStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_user )
        return false;
    if ( t_mapping )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( t_for )
        return false;
    if ( authIdent )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    return true;
}

void DropUserMappingStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(authIdent, visitor);
        accept(serverName, visitor);
    }
    visitor->endVisit(this);
}

int DropTableSpaceStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_tablespace )
        return t_tablespace;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    return 0;
}

int DropTableSpaceStmtAST::lastToken() const
{
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropTableSpaceStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_tablespace )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( tablespaceName )
        return false;
    return true;
}

void DropTableSpaceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
    }
    visitor->endVisit(this);
}

int CommonDropStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( dropType && !dropType->isNull() )
        return dropType->firstToken();
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( idents )
        return idents->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int CommonDropStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( idents )
        return idents->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( dropType && !dropType->isNull() )
        return dropType->lastToken();
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool CommonDropStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( dropType )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( idents )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void CommonDropStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dropType, visitor);
        accept(idents, visitor);
    }
    visitor->endVisit(this);
}

int CommonDropTypeAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    if ( t_sp3 )
        return t_sp3;
    return 0;
}

int CommonDropTypeAST::lastToken() const
{
    if ( t_sp3 )
        return t_sp3 + 1;
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool CommonDropTypeAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    if ( t_sp3 )
        return false;
    return true;
}

void CommonDropTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DropTableStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( tables )
        return tables->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropTableStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( tables )
        return tables->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropTableStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( tables )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tables, visitor);
    }
    visitor->endVisit(this);
}

int DropViewStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( tables )
        return tables->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropViewStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( tables )
        return tables->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropViewStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( tables )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropViewStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tables, visitor);
    }
    visitor->endVisit(this);
}

int DropIndexStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_index )
        return t_index;
    if ( t_concurrently )
        return t_concurrently;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( indexes )
        return indexes->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropIndexStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( indexes )
        return indexes->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_concurrently )
        return t_concurrently + 1;
    if ( t_index )
        return t_index + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropIndexStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_index )
        return false;
    if ( t_concurrently )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( indexes )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropIndexStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexes, visitor);
    }
    visitor->endVisit(this);
}

int DropSchemaStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_schema )
        return t_schema;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( schemas )
        return schemas->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropSchemaStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( schemas )
        return schemas->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_schema )
        return t_schema + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropSchemaStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_schema )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( schemas )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemas, visitor);
    }
    visitor->endVisit(this);
}

int DropSequenceStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_sequence )
        return t_sequence;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( seqs )
        return seqs->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropSequenceStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( seqs )
        return seqs->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_sequence )
        return t_sequence + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropSequenceStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_sequence )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( seqs )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropSequenceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(seqs, visitor);
    }
    visitor->endVisit(this);
}

int RuleStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( orReplace && !orReplace->isNull() )
        return orReplace->firstToken();
    if ( t_rule )
        return t_rule;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->firstToken();
    if ( t_as )
        return t_as;
    if ( t_on )
        return t_on;
    if ( t_sp_event )
        return t_sp_event;
    if ( t_to )
        return t_to;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->firstToken();
    if ( t_do )
        return t_do;
    if ( t_instaed )
        return t_instaed;
    if ( actions && !actions->isNull() )
        return actions->firstToken();
    return 0;
}

int RuleStmtAST::lastToken() const
{
    if ( actions && !actions->isNull() )
        return actions->lastToken();
    if ( t_instaed )
        return t_instaed + 1;
    if ( t_do )
        return t_do + 1;
    if ( whereClause && !whereClause->isNull() )
        return whereClause->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_sp_event )
        return t_sp_event + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_as )
        return t_as + 1;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->lastToken();
    if ( t_rule )
        return t_rule + 1;
    if ( orReplace && !orReplace->isNull() )
        return orReplace->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool RuleStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( orReplace )
        return false;
    if ( t_rule )
        return false;
    if ( ruleName )
        return false;
    if ( t_as )
        return false;
    if ( t_on )
        return false;
    if ( t_sp_event )
        return false;
    if ( t_to )
        return false;
    if ( tableIdent )
        return false;
    if ( whereClause )
        return false;
    if ( t_do )
        return false;
    if ( t_instaed )
        return false;
    if ( actions )
        return false;
    return true;
}

void RuleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(orReplace, visitor);
        accept(ruleName, visitor);
        accept(tableIdent, visitor);
        accept(whereClause, visitor);
        accept(actions, visitor);
    }
    visitor->endVisit(this);
}

int NothingRuleActionsAST::firstToken() const
{
    if ( t_nothing )
        return t_nothing;
    return 0;
}

int NothingRuleActionsAST::lastToken() const
{
    if ( t_nothing )
        return t_nothing + 1;
    return 1;
}

bool NothingRuleActionsAST::isNull() const
{
    if ( t_nothing )
        return false;
    return true;
}

void NothingRuleActionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SingleRuleActionsAST::firstToken() const
{
    if ( actStatement && !actStatement->isNull() )
        return actStatement->firstToken();
    return 0;
}

int SingleRuleActionsAST::lastToken() const
{
    if ( actStatement && !actStatement->isNull() )
        return actStatement->lastToken();
    return 1;
}

bool SingleRuleActionsAST::isNull() const
{
    if ( actStatement )
        return false;
    return true;
}

void SingleRuleActionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(actStatement, visitor);
    }
    visitor->endVisit(this);
}

int MultiRuleActionsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( actions )
        return actions->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int MultiRuleActionsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( actions )
        return actions->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool MultiRuleActionsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( actions )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void MultiRuleActionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(actions, visitor);
    }
    visitor->endVisit(this);
}

int DropRuleStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_rule )
        return t_rule;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropRuleStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_rule )
        return t_rule + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropRuleStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_rule )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( ruleName )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropRuleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ruleName, visitor);
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int DropOwnedStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_owned )
        return t_owned;
    if ( t_by )
        return t_by;
    if ( roles )
        return roles->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropOwnedStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( roles )
        return roles->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_owned )
        return t_owned + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropOwnedStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_owned )
        return false;
    if ( t_by )
        return false;
    if ( roles )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropOwnedStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roles, visitor);
    }
    visitor->endVisit(this);
}

int ReassignOwnedStmtAST::firstToken() const
{
    if ( t_reassign )
        return t_reassign;
    if ( t_owned )
        return t_owned;
    if ( t_by )
        return t_by;
    if ( roles )
        return roles->firstToken();
    if ( t_to )
        return t_to;
    if ( newRole && !newRole->isNull() )
        return newRole->firstToken();
    return 0;
}

int ReassignOwnedStmtAST::lastToken() const
{
    if ( newRole && !newRole->isNull() )
        return newRole->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( roles )
        return roles->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_owned )
        return t_owned + 1;
    if ( t_reassign )
        return t_reassign + 1;
    return 1;
}

bool ReassignOwnedStmtAST::isNull() const
{
    if ( t_reassign )
        return false;
    if ( t_owned )
        return false;
    if ( t_by )
        return false;
    if ( roles )
        return false;
    if ( t_to )
        return false;
    if ( newRole )
        return false;
    return true;
}

void ReassignOwnedStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roles, visitor);
        accept(newRole, visitor);
    }
    visitor->endVisit(this);
}

int DropGroupStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_group )
        return t_group;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( roles )
        return roles->firstToken();
    return 0;
}

int DropGroupStmtAST::lastToken() const
{
    if ( roles )
        return roles->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_group )
        return t_group + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropGroupStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_group )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( roles )
        return false;
    return true;
}

void DropGroupStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roles, visitor);
    }
    visitor->endVisit(this);
}

int DropForeignServerStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_server )
        return t_server;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropForeignServerStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_server )
        return t_server + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropForeignServerStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_server )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( serverName )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropForeignServerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(serverName, visitor);
    }
    visitor->endVisit(this);
}

int DropFdwStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( fdwName && !fdwName->isNull() )
        return fdwName->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropFdwStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( fdwName && !fdwName->isNull() )
        return fdwName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropFdwStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( fdwName )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropFdwStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fdwName, visitor);
    }
    visitor->endVisit(this);
}

int DoStmtAST::firstToken() const
{
    if ( t_do )
        return t_do;
    if ( items )
        return items->firstToken();
    return 0;
}

int DoStmtAST::lastToken() const
{
    if ( items )
        return items->lastToken();
    if ( t_do )
        return t_do + 1;
    return 1;
}

bool DoStmtAST::isNull() const
{
    if ( t_do )
        return false;
    if ( items )
        return false;
    return true;
}

void DoStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(items, visitor);
    }
    visitor->endVisit(this);
}

int DoStmtCmdItemAST::firstToken() const
{
    if ( t_str_cmd )
        return t_str_cmd;
    return 0;
}

int DoStmtCmdItemAST::lastToken() const
{
    if ( t_str_cmd )
        return t_str_cmd + 1;
    return 1;
}

bool DoStmtCmdItemAST::isNull() const
{
    if ( t_str_cmd )
        return false;
    return true;
}

void DoStmtCmdItemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DoStmtLangItemAST::firstToken() const
{
    if ( t_language )
        return t_language;
    if ( t_lang_label )
        return t_lang_label;
    return 0;
}

int DoStmtLangItemAST::lastToken() const
{
    if ( t_lang_label )
        return t_lang_label + 1;
    if ( t_language )
        return t_language + 1;
    return 1;
}

bool DoStmtLangItemAST::isNull() const
{
    if ( t_language )
        return false;
    if ( t_lang_label )
        return false;
    return true;
}

void DoStmtLangItemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateOldAggregateStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_aggregate )
        return t_aggregate;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( aggrDefinition && !aggrDefinition->isNull() )
        return aggrDefinition->firstToken();
    return 0;
}

int CreateOldAggregateStmtAST::lastToken() const
{
    if ( aggrDefinition && !aggrDefinition->isNull() )
        return aggrDefinition->lastToken();
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateOldAggregateStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_aggregate )
        return false;
    if ( functionRef )
        return false;
    if ( aggrDefinition )
        return false;
    return true;
}

void CreateOldAggregateStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionRef, visitor);
        accept(aggrDefinition, visitor);
    }
    visitor->endVisit(this);
}

int AggregateDefinitionAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( defs )
        return defs->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int AggregateDefinitionAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( defs )
        return defs->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool AggregateDefinitionAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( defs )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void AggregateDefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(defs, visitor);
    }
    visitor->endVisit(this);
}

int AggrDefinitionAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    if ( t_equal )
        return t_equal;
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    return 0;
}

int AggrDefinitionAST::lastToken() const
{
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    if ( t_equal )
        return t_equal + 1;
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

bool AggrDefinitionAST::isNull() const
{
    if ( t_identifier )
        return false;
    if ( t_equal )
        return false;
    if ( arg )
        return false;
    return true;
}

void AggrDefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arg, visitor);
    }
    visitor->endVisit(this);
}

int CreateAggregateStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_aggregate )
        return t_aggregate;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( defintion && !defintion->isNull() )
        return defintion->firstToken();
    return 0;
}

int CreateAggregateStmtAST::lastToken() const
{
    if ( defintion && !defintion->isNull() )
        return defintion->lastToken();
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateAggregateStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_aggregate )
        return false;
    if ( functionRef )
        return false;
    if ( args )
        return false;
    if ( defintion )
        return false;
    return true;
}

void CreateAggregateStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionRef, visitor);
        accept(args, visitor);
        accept(defintion, visitor);
    }
    visitor->endVisit(this);
}

int CreateOpStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_operator )
        return t_operator;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( defintion && !defintion->isNull() )
        return defintion->firstToken();
    return 0;
}

int CreateOpStmtAST::lastToken() const
{
    if ( defintion && !defintion->isNull() )
        return defintion->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_operator )
        return t_operator + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateOpStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_operator )
        return false;
    if ( operand )
        return false;
    if ( defintion )
        return false;
    return true;
}

void CreateOpStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(defintion, visitor);
    }
    visitor->endVisit(this);
}

int CreateTypeAsRecordStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( t_as )
        return t_as;
    if ( t_lparen )
        return t_lparen;
    if ( members )
        return members->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CreateTypeAsRecordStmtAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( members )
        return members->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_as )
        return t_as + 1;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTypeAsRecordStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( t_as )
        return false;
    if ( t_lparen )
        return false;
    if ( members )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CreateTypeAsRecordStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(members, visitor);
    }
    visitor->endVisit(this);
}

int CreateTypeAsEnumStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( t_as )
        return t_as;
    if ( t_enum )
        return t_enum;
    if ( t_lparen )
        return t_lparen;
    if ( values )
        return values->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CreateTypeAsEnumStmtAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( values )
        return values->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_enum )
        return t_enum + 1;
    if ( t_as )
        return t_as + 1;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTypeAsEnumStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( t_as )
        return false;
    if ( t_enum )
        return false;
    if ( t_lparen )
        return false;
    if ( values )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CreateTypeAsEnumStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int EnumValueAST::firstToken() const
{
    if ( t_str_value )
        return t_str_value;
    return 0;
}

int EnumValueAST::lastToken() const
{
    if ( t_str_value )
        return t_str_value + 1;
    return 1;
}

bool EnumValueAST::isNull() const
{
    if ( t_str_value )
        return false;
    return true;
}

void EnumValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateTypeAsDefStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( defintion && !defintion->isNull() )
        return defintion->firstToken();
    return 0;
}

int CreateTypeAsDefStmtAST::lastToken() const
{
    if ( defintion && !defintion->isNull() )
        return defintion->lastToken();
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTypeAsDefStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( defintion )
        return false;
    return true;
}

void CreateTypeAsDefStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(defintion, visitor);
    }
    visitor->endVisit(this);
}

int CreateTypeAsRangeStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( t_as )
        return t_as;
    if ( t_range )
        return t_range;
    if ( defintion && !defintion->isNull() )
        return defintion->firstToken();
    return 0;
}

int CreateTypeAsRangeStmtAST::lastToken() const
{
    if ( defintion && !defintion->isNull() )
        return defintion->lastToken();
    if ( t_range )
        return t_range + 1;
    if ( t_as )
        return t_as + 1;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTypeAsRangeStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( t_as )
        return false;
    if ( t_range )
        return false;
    if ( defintion )
        return false;
    return true;
}

void CreateTypeAsRangeStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(defintion, visitor);
    }
    visitor->endVisit(this);
}

int CreateSearchObjStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_sp_obj )
        return t_sp_obj;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( defintion && !defintion->isNull() )
        return defintion->firstToken();
    return 0;
}

int CreateSearchObjStmtAST::lastToken() const
{
    if ( defintion && !defintion->isNull() )
        return defintion->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_sp_obj )
        return t_sp_obj + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateSearchObjStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_sp_obj )
        return false;
    if ( ident )
        return false;
    if ( defintion )
        return false;
    return true;
}

void CreateSearchObjStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(defintion, visitor);
    }
    visitor->endVisit(this);
}

int CreateCollationDefStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_collation )
        return t_collation;
    if ( collationIdent && !collationIdent->isNull() )
        return collationIdent->firstToken();
    if ( defintion && !defintion->isNull() )
        return defintion->firstToken();
    return 0;
}

int CreateCollationDefStmtAST::lastToken() const
{
    if ( defintion && !defintion->isNull() )
        return defintion->lastToken();
    if ( collationIdent && !collationIdent->isNull() )
        return collationIdent->lastToken();
    if ( t_collation )
        return t_collation + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateCollationDefStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_collation )
        return false;
    if ( collationIdent )
        return false;
    if ( defintion )
        return false;
    return true;
}

void CreateCollationDefStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collationIdent, visitor);
        accept(defintion, visitor);
    }
    visitor->endVisit(this);
}

int CreateCollationFromStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_collation )
        return t_collation;
    if ( collationIdent && !collationIdent->isNull() )
        return collationIdent->firstToken();
    if ( t_from )
        return t_from;
    if ( fromCollationIdent && !fromCollationIdent->isNull() )
        return fromCollationIdent->firstToken();
    return 0;
}

int CreateCollationFromStmtAST::lastToken() const
{
    if ( fromCollationIdent && !fromCollationIdent->isNull() )
        return fromCollationIdent->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( collationIdent && !collationIdent->isNull() )
        return collationIdent->lastToken();
    if ( t_collation )
        return t_collation + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateCollationFromStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_collation )
        return false;
    if ( collationIdent )
        return false;
    if ( t_from )
        return false;
    if ( fromCollationIdent )
        return false;
    return true;
}

void CreateCollationFromStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collationIdent, visitor);
        accept(fromCollationIdent, visitor);
    }
    visitor->endVisit(this);
}

int DeclareCursorStmtAST::firstToken() const
{
    if ( t_declare )
        return t_declare;
    if ( cursorName && !cursorName->isNull() )
        return cursorName->firstToken();
    if ( t_cursor )
        return t_cursor;
    if ( holdOption && !holdOption->isNull() )
        return holdOption->firstToken();
    if ( t_for )
        return t_for;
    if ( query && !query->isNull() )
        return query->firstToken();
    return 0;
}

int DeclareCursorStmtAST::lastToken() const
{
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( holdOption && !holdOption->isNull() )
        return holdOption->lastToken();
    if ( t_cursor )
        return t_cursor + 1;
    if ( cursorName && !cursorName->isNull() )
        return cursorName->lastToken();
    if ( t_declare )
        return t_declare + 1;
    return 1;
}

bool DeclareCursorStmtAST::isNull() const
{
    if ( t_declare )
        return false;
    if ( cursorName )
        return false;
    if ( t_cursor )
        return false;
    if ( holdOption )
        return false;
    if ( t_for )
        return false;
    if ( query )
        return false;
    return true;
}

void DeclareCursorStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(cursorName, visitor);
        accept(holdOption, visitor);
        accept(query, visitor);
    }
    visitor->endVisit(this);
}

int CursorHoldOptionAST::firstToken() const
{
    if ( t_with_or_without )
        return t_with_or_without;
    if ( t_hold )
        return t_hold;
    return 0;
}

int CursorHoldOptionAST::lastToken() const
{
    if ( t_hold )
        return t_hold + 1;
    if ( t_with_or_without )
        return t_with_or_without + 1;
    return 1;
}

bool CursorHoldOptionAST::isNull() const
{
    if ( t_with_or_without )
        return false;
    if ( t_hold )
        return false;
    return true;
}

void CursorHoldOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DeallocateStmtAST::firstToken() const
{
    if ( t_deallocate )
        return t_deallocate;
    if ( t_prepare )
        return t_prepare;
    if ( t_all_or_plan_name )
        return t_all_or_plan_name;
    return 0;
}

int DeallocateStmtAST::lastToken() const
{
    if ( t_all_or_plan_name )
        return t_all_or_plan_name + 1;
    if ( t_prepare )
        return t_prepare + 1;
    if ( t_deallocate )
        return t_deallocate + 1;
    return 1;
}

bool DeallocateStmtAST::isNull() const
{
    if ( t_deallocate )
        return false;
    if ( t_prepare )
        return false;
    if ( t_all_or_plan_name )
        return false;
    return true;
}

void DeallocateStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateDbStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_database )
        return t_database;
    if ( dbName && !dbName->isNull() )
        return dbName->firstToken();
    if ( t_with )
        return t_with;
    if ( options )
        return options->firstToken();
    return 0;
}

int CreateDbStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( dbName && !dbName->isNull() )
        return dbName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateDbStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_database )
        return false;
    if ( dbName )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateDbStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dbName, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int NamedCreateDbOptionAST::firstToken() const
{
    if ( t_sp_opt )
        return t_sp_opt;
    if ( t_equal )
        return t_equal;
    if ( optName && !optName->isNull() )
        return optName->firstToken();
    return 0;
}

int NamedCreateDbOptionAST::lastToken() const
{
    if ( optName && !optName->isNull() )
        return optName->lastToken();
    if ( t_equal )
        return t_equal + 1;
    if ( t_sp_opt )
        return t_sp_opt + 1;
    return 1;
}

bool NamedCreateDbOptionAST::isNull() const
{
    if ( t_sp_opt )
        return false;
    if ( t_equal )
        return false;
    if ( optName )
        return false;
    return true;
}

void NamedCreateDbOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(optName, visitor);
    }
    visitor->endVisit(this);
}

int ValDefCreateDbOptionAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    if ( t_equal )
        return t_equal;
    if ( t_default_or_value )
        return t_default_or_value;
    return 0;
}

int ValDefCreateDbOptionAST::lastToken() const
{
    if ( t_default_or_value )
        return t_default_or_value + 1;
    if ( t_equal )
        return t_equal + 1;
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool ValDefCreateDbOptionAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    if ( t_equal )
        return false;
    if ( t_default_or_value )
        return false;
    return true;
}

void ValDefCreateDbOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateUserMappingStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_user )
        return t_user;
    if ( t_mapping )
        return t_mapping;
    if ( t_for )
        return t_for;
    if ( authIdent && !authIdent->isNull() )
        return authIdent->firstToken();
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( options && !options->isNull() )
        return options->firstToken();
    return 0;
}

int CreateUserMappingStmtAST::lastToken() const
{
    if ( options && !options->isNull() )
        return options->lastToken();
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( authIdent && !authIdent->isNull() )
        return authIdent->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_mapping )
        return t_mapping + 1;
    if ( t_user )
        return t_user + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateUserMappingStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_user )
        return false;
    if ( t_mapping )
        return false;
    if ( t_for )
        return false;
    if ( authIdent )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateUserMappingStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(authIdent, visitor);
        accept(serverName, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int DropTrigStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_trigger )
        return t_trigger;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropTrigStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropTrigStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_trigger )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( triggerName )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropTrigStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int CreateConstrTrigStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_constraint )
        return t_constraint;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( t_after )
        return t_after;
    if ( events )
        return events->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( fromTable && !fromTable->isNull() )
        return fromTable->firstToken();
    if ( constraints )
        return constraints->firstToken();
    if ( forSpec && !forSpec->isNull() )
        return forSpec->firstToken();
    if ( trgWhen && !trgWhen->isNull() )
        return trgWhen->firstToken();
    if ( execClause && !execClause->isNull() )
        return execClause->firstToken();
    return 0;
}

int CreateConstrTrigStmtAST::lastToken() const
{
    if ( execClause && !execClause->isNull() )
        return execClause->lastToken();
    if ( trgWhen && !trgWhen->isNull() )
        return trgWhen->lastToken();
    if ( forSpec && !forSpec->isNull() )
        return forSpec->lastToken();
    if ( constraints )
        return constraints->lastToken();
    if ( fromTable && !fromTable->isNull() )
        return fromTable->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( events )
        return events->lastToken();
    if ( t_after )
        return t_after + 1;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateConstrTrigStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_constraint )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( t_after )
        return false;
    if ( events )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( fromTable )
        return false;
    if ( constraints )
        return false;
    if ( forSpec )
        return false;
    if ( trgWhen )
        return false;
    if ( execClause )
        return false;
    return true;
}

void CreateConstrTrigStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(events, visitor);
        accept(tableIdent, visitor);
        accept(fromTable, visitor);
        accept(constraints, visitor);
        accept(forSpec, visitor);
        accept(trgWhen, visitor);
        accept(execClause, visitor);
    }
    visitor->endVisit(this);
}

int ConstrFromTableAST::firstToken() const
{
    if ( t_from )
        return t_from;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    return 0;
}

int ConstrFromTableAST::lastToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_from )
        return t_from + 1;
    return 1;
}

bool ConstrFromTableAST::isNull() const
{
    if ( t_from )
        return false;
    if ( tableIdent )
        return false;
    return true;
}

void ConstrFromTableAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int CreateTrigStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( actTime && !actTime->isNull() )
        return actTime->firstToken();
    if ( events )
        return events->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( forSpec && !forSpec->isNull() )
        return forSpec->firstToken();
    if ( trgWhen && !trgWhen->isNull() )
        return trgWhen->firstToken();
    if ( execClause && !execClause->isNull() )
        return execClause->firstToken();
    return 0;
}

int CreateTrigStmtAST::lastToken() const
{
    if ( execClause && !execClause->isNull() )
        return execClause->lastToken();
    if ( trgWhen && !trgWhen->isNull() )
        return trgWhen->lastToken();
    if ( forSpec && !forSpec->isNull() )
        return forSpec->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( events )
        return events->lastToken();
    if ( actTime && !actTime->isNull() )
        return actTime->lastToken();
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTrigStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( actTime )
        return false;
    if ( events )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    if ( forSpec )
        return false;
    if ( trgWhen )
        return false;
    if ( execClause )
        return false;
    return true;
}

void CreateTrigStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(actTime, visitor);
        accept(events, visitor);
        accept(tableIdent, visitor);
        accept(forSpec, visitor);
        accept(trgWhen, visitor);
        accept(execClause, visitor);
    }
    visitor->endVisit(this);
}

int TriggerExecuteClauseAST::firstToken() const
{
    if ( t_execute )
        return t_execute;
    if ( t_procedure )
        return t_procedure;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TriggerExecuteClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    if ( t_procedure )
        return t_procedure + 1;
    if ( t_execute )
        return t_execute + 1;
    return 1;
}

bool TriggerExecuteClauseAST::isNull() const
{
    if ( t_execute )
        return false;
    if ( t_procedure )
        return false;
    if ( functionRef )
        return false;
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TriggerExecuteClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionRef, visitor);
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int TriggerFuncArgAST::firstToken() const
{
    if ( t_arg_value )
        return t_arg_value;
    return 0;
}

int TriggerFuncArgAST::lastToken() const
{
    if ( t_arg_value )
        return t_arg_value + 1;
    return 1;
}

bool TriggerFuncArgAST::isNull() const
{
    if ( t_arg_value )
        return false;
    return true;
}

void TriggerFuncArgAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TriggerWhenAST::firstToken() const
{
    if ( t_when )
        return t_when;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TriggerWhenAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_when )
        return t_when + 1;
    return 1;
}

bool TriggerWhenAST::isNull() const
{
    if ( t_when )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TriggerWhenAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int TriggerForSpecAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_each )
        return t_each;
    if ( t_row_or_statement )
        return t_row_or_statement;
    return 0;
}

int TriggerForSpecAST::lastToken() const
{
    if ( t_row_or_statement )
        return t_row_or_statement + 1;
    if ( t_each )
        return t_each + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool TriggerForSpecAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_each )
        return false;
    if ( t_row_or_statement )
        return false;
    return true;
}

void TriggerForSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TriggerEventAST::firstToken() const
{
    if ( t_sp_event )
        return t_sp_event;
    if ( t_of )
        return t_of;
    if ( columns )
        return columns->firstToken();
    return 0;
}

int TriggerEventAST::lastToken() const
{
    if ( columns )
        return columns->lastToken();
    if ( t_of )
        return t_of + 1;
    if ( t_sp_event )
        return t_sp_event + 1;
    return 1;
}

bool TriggerEventAST::isNull() const
{
    if ( t_sp_event )
        return false;
    if ( t_of )
        return false;
    if ( columns )
        return false;
    return true;
}

void TriggerEventAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int TriggerActionTimeAST::firstToken() const
{
    if ( t_before_after_instaed )
        return t_before_after_instaed;
    if ( t_of )
        return t_of;
    return 0;
}

int TriggerActionTimeAST::lastToken() const
{
    if ( t_of )
        return t_of + 1;
    if ( t_before_after_instaed )
        return t_before_after_instaed + 1;
    return 1;
}

bool TriggerActionTimeAST::isNull() const
{
    if ( t_before_after_instaed )
        return false;
    if ( t_of )
        return false;
    return true;
}

void TriggerActionTimeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateTableSpaceStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    if ( tablespaceOwner && !tablespaceOwner->isNull() )
        return tablespaceOwner->firstToken();
    if ( t_location )
        return t_location;
    if ( t_path_value )
        return t_path_value;
    return 0;
}

int CreateTableSpaceStmtAST::lastToken() const
{
    if ( t_path_value )
        return t_path_value + 1;
    if ( t_location )
        return t_location + 1;
    if ( tablespaceOwner && !tablespaceOwner->isNull() )
        return tablespaceOwner->lastToken();
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTableSpaceStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    if ( tablespaceOwner )
        return false;
    if ( t_location )
        return false;
    if ( t_path_value )
        return false;
    return true;
}

void CreateTableSpaceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
        accept(tablespaceOwner, visitor);
    }
    visitor->endVisit(this);
}

int TableSpaceOwnerClauseAST::firstToken() const
{
    if ( t_owner )
        return t_owner;
    if ( ownerName && !ownerName->isNull() )
        return ownerName->firstToken();
    return 0;
}

int TableSpaceOwnerClauseAST::lastToken() const
{
    if ( ownerName && !ownerName->isNull() )
        return ownerName->lastToken();
    if ( t_owner )
        return t_owner + 1;
    return 1;
}

bool TableSpaceOwnerClauseAST::isNull() const
{
    if ( t_owner )
        return false;
    if ( ownerName )
        return false;
    return true;
}

void TableSpaceOwnerClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ownerName, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableAsExecStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->firstToken();
    if ( t_table )
        return t_table;
    if ( target && !target->isNull() )
        return target->firstToken();
    if ( t_as )
        return t_as;
    if ( execStmt && !execStmt->isNull() )
        return execStmt->firstToken();
    if ( withData && !withData->isNull() )
        return withData->firstToken();
    return 0;
}

int CreateTableAsExecStmtAST::lastToken() const
{
    if ( withData && !withData->isNull() )
        return withData->lastToken();
    if ( execStmt && !execStmt->isNull() )
        return execStmt->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( target && !target->isNull() )
        return target->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTableAsExecStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( tempClause )
        return false;
    if ( t_table )
        return false;
    if ( target )
        return false;
    if ( t_as )
        return false;
    if ( execStmt )
        return false;
    if ( withData )
        return false;
    return true;
}

void CreateTableAsExecStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tempClause, visitor);
        accept(target, visitor);
        accept(execStmt, visitor);
        accept(withData, visitor);
    }
    visitor->endVisit(this);
}

int ExecuteStmtAST::firstToken() const
{
    if ( t_execute )
        return t_execute;
    if ( planName && !planName->isNull() )
        return planName->firstToken();
    if ( parClause && !parClause->isNull() )
        return parClause->firstToken();
    return 0;
}

int ExecuteStmtAST::lastToken() const
{
    if ( parClause && !parClause->isNull() )
        return parClause->lastToken();
    if ( planName && !planName->isNull() )
        return planName->lastToken();
    if ( t_execute )
        return t_execute + 1;
    return 1;
}

bool ExecuteStmtAST::isNull() const
{
    if ( t_execute )
        return false;
    if ( planName )
        return false;
    if ( parClause )
        return false;
    return true;
}

void ExecuteStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(planName, visitor);
        accept(parClause, visitor);
    }
    visitor->endVisit(this);
}

int ExecuteParamClauseAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( params )
        return params->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int ExecuteParamClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( params )
        return params->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool ExecuteParamClauseAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( params )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void ExecuteParamClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(params, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->firstToken();
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_not )
        return t_not;
    if ( t_exists )
        return t_exists;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( createClause && !createClause->isNull() )
        return createClause->firstToken();
    if ( options && !options->isNull() )
        return options->firstToken();
    return 0;
}

int CreateTableStmtAST::lastToken() const
{
    if ( options && !options->isNull() )
        return options->lastToken();
    if ( createClause && !createClause->isNull() )
        return createClause->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTableStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( tempClause )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_not )
        return false;
    if ( t_exists )
        return false;
    if ( tableIdent )
        return false;
    if ( createClause )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tempClause, visitor);
        accept(tableIdent, visitor);
        accept(createClause, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableOptionsAST::firstToken() const
{
    if ( withOptions && !withOptions->isNull() )
        return withOptions->firstToken();
    if ( commitOption && !commitOption->isNull() )
        return commitOption->firstToken();
    if ( tablespaceClause && !tablespaceClause->isNull() )
        return tablespaceClause->firstToken();
    return 0;
}

int CreateTableOptionsAST::lastToken() const
{
    if ( tablespaceClause && !tablespaceClause->isNull() )
        return tablespaceClause->lastToken();
    if ( commitOption && !commitOption->isNull() )
        return commitOption->lastToken();
    if ( withOptions && !withOptions->isNull() )
        return withOptions->lastToken();
    return 1;
}

bool CreateTableOptionsAST::isNull() const
{
    if ( withOptions )
        return false;
    if ( commitOption )
        return false;
    if ( tablespaceClause )
        return false;
    return true;
}

void CreateTableOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(withOptions, visitor);
        accept(commitOption, visitor);
        accept(tablespaceClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableDirecClauseAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( inherit && !inherit->isNull() )
        return inherit->firstToken();
    return 0;
}

int CreateTableDirecClauseAST::lastToken() const
{
    if ( inherit && !inherit->isNull() )
        return inherit->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool CreateTableDirecClauseAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( inherit )
        return false;
    return true;
}

void CreateTableDirecClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
        accept(inherit, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableOfClauseAST::firstToken() const
{
    if ( t_of )
        return t_of;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( elements && !elements->isNull() )
        return elements->firstToken();
    return 0;
}

int CreateTableOfClauseAST::lastToken() const
{
    if ( elements && !elements->isNull() )
        return elements->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_of )
        return t_of + 1;
    return 1;
}

bool CreateTableOfClauseAST::isNull() const
{
    if ( t_of )
        return false;
    if ( ident )
        return false;
    if ( elements )
        return false;
    return true;
}

void CreateTableOfClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(elements, visitor);
    }
    visitor->endVisit(this);
}

int TypedTableElementsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( members )
        return members->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TypedTableElementsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( members )
        return members->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool TypedTableElementsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( members )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TypedTableElementsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(members, visitor);
    }
    visitor->endVisit(this);
}

int TypedTableConstrElementAST::firstToken() const
{
    if ( constraint && !constraint->isNull() )
        return constraint->firstToken();
    return 0;
}

int TypedTableConstrElementAST::lastToken() const
{
    if ( constraint && !constraint->isNull() )
        return constraint->lastToken();
    return 1;
}

bool TypedTableConstrElementAST::isNull() const
{
    if ( constraint )
        return false;
    return true;
}

void TypedTableConstrElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraint, visitor);
    }
    visitor->endVisit(this);
}

int TypedTableColElementAST::firstToken() const
{
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( t_with )
        return t_with;
    if ( t_options )
        return t_options;
    if ( constraints )
        return constraints->firstToken();
    return 0;
}

int TypedTableColElementAST::lastToken() const
{
    if ( constraints )
        return constraints->lastToken();
    if ( t_options )
        return t_options + 1;
    if ( t_with )
        return t_with + 1;
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    return 1;
}

bool TypedTableColElementAST::isNull() const
{
    if ( columnName )
        return false;
    if ( t_with )
        return false;
    if ( t_options )
        return false;
    if ( constraints )
        return false;
    return true;
}

void TypedTableColElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(constraints, visitor);
    }
    visitor->endVisit(this);
}

int TableInheritClauseAST::firstToken() const
{
    if ( t_inherits )
        return t_inherits;
    if ( t_lparen )
        return t_lparen;
    if ( tables )
        return tables->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TableInheritClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( tables )
        return tables->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_inherits )
        return t_inherits + 1;
    return 1;
}

bool TableInheritClauseAST::isNull() const
{
    if ( t_inherits )
        return false;
    if ( t_lparen )
        return false;
    if ( tables )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TableInheritClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tables, visitor);
    }
    visitor->endVisit(this);
}

int CreateSchemaStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_schema )
        return t_schema;
    if ( t_if )
        return t_if;
    if ( t_not )
        return t_not;
    if ( t_exists )
        return t_exists;
    if ( schemaClause && !schemaClause->isNull() )
        return schemaClause->firstToken();
    return 0;
}

int CreateSchemaStmtAST::lastToken() const
{
    if ( schemaClause && !schemaClause->isNull() )
        return schemaClause->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_schema )
        return t_schema + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateSchemaStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_schema )
        return false;
    if ( t_if )
        return false;
    if ( t_not )
        return false;
    if ( t_exists )
        return false;
    if ( schemaClause )
        return false;
    return true;
}

void CreateSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateSchemaClauseAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_authorization )
        return t_authorization;
    if ( roleId && !roleId->isNull() )
        return roleId->firstToken();
    if ( schemaStatements )
        return schemaStatements->firstToken();
    return 0;
}

int CreateSchemaClauseAST::lastToken() const
{
    if ( schemaStatements )
        return schemaStatements->lastToken();
    if ( roleId && !roleId->isNull() )
        return roleId->lastToken();
    if ( t_authorization )
        return t_authorization + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool CreateSchemaClauseAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_authorization )
        return false;
    if ( roleId )
        return false;
    if ( schemaStatements )
        return false;
    return true;
}

void CreateSchemaClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(roleId, visitor);
        accept(schemaStatements, visitor);
    }
    visitor->endVisit(this);
}

int DropPLangStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( t_lang_alias )
        return t_lang_alias;
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropPLangStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( t_lang_alias )
        return t_lang_alias + 1;
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropPLangStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( t_lang_alias )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropPLangStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreatePLangStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( orReplaceClause && !orReplaceClause->isNull() )
        return orReplaceClause->firstToken();
    if ( t_trusted )
        return t_trusted;
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( t_lang_alias )
        return t_lang_alias;
    if ( t_handler )
        return t_handler;
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->firstToken();
    if ( inlineClause && !inlineClause->isNull() )
        return inlineClause->firstToken();
    if ( validatorClause && !validatorClause->isNull() )
        return validatorClause->firstToken();
    return 0;
}

int CreatePLangStmtAST::lastToken() const
{
    if ( validatorClause && !validatorClause->isNull() )
        return validatorClause->lastToken();
    if ( inlineClause && !inlineClause->isNull() )
        return inlineClause->lastToken();
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->lastToken();
    if ( t_handler )
        return t_handler + 1;
    if ( t_lang_alias )
        return t_lang_alias + 1;
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    if ( t_trusted )
        return t_trusted + 1;
    if ( orReplaceClause && !orReplaceClause->isNull() )
        return orReplaceClause->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreatePLangStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( orReplaceClause )
        return false;
    if ( t_trusted )
        return false;
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( t_lang_alias )
        return false;
    if ( t_handler )
        return false;
    if ( handlerIdent )
        return false;
    if ( inlineClause )
        return false;
    if ( validatorClause )
        return false;
    return true;
}

void CreatePLangStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(orReplaceClause, visitor);
        accept(handlerIdent, visitor);
        accept(inlineClause, visitor);
        accept(validatorClause, visitor);
    }
    visitor->endVisit(this);
}

int InlineHandlerClauseAST::firstToken() const
{
    if ( t_inline )
        return t_inline;
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->firstToken();
    return 0;
}

int InlineHandlerClauseAST::lastToken() const
{
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->lastToken();
    if ( t_inline )
        return t_inline + 1;
    return 1;
}

bool InlineHandlerClauseAST::isNull() const
{
    if ( t_inline )
        return false;
    if ( handlerIdent )
        return false;
    return true;
}

void InlineHandlerClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(handlerIdent, visitor);
    }
    visitor->endVisit(this);
}

int ValidatorHandlerClauseAST::firstToken() const
{
    if ( t_validator )
        return t_validator;
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->firstToken();
    return 0;
}

int ValidatorHandlerClauseAST::lastToken() const
{
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->lastToken();
    if ( t_validator )
        return t_validator + 1;
    return 1;
}

bool ValidatorHandlerClauseAST::isNull() const
{
    if ( t_validator )
        return false;
    if ( handlerIdent )
        return false;
    return true;
}

void ValidatorHandlerClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(handlerIdent, visitor);
    }
    visitor->endVisit(this);
}

int DropOpClassStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_operator )
        return t_operator;
    if ( t_class )
        return t_class;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( t_ehavior )
        return t_ehavior;
    return 0;
}

int DropOpClassStmtAST::lastToken() const
{
    if ( t_ehavior )
        return t_ehavior + 1;
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_class )
        return t_class + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropOpClassStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_operator )
        return false;
    if ( t_class )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( t_ehavior )
        return false;
    return true;
}

void DropOpClassStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
    }
    visitor->endVisit(this);
}

int DropOpFamilyStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_operator )
        return t_operator;
    if ( t_family )
        return t_family;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( t_ehavior )
        return t_ehavior;
    return 0;
}

int DropOpFamilyStmtAST::lastToken() const
{
    if ( t_ehavior )
        return t_ehavior + 1;
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_family )
        return t_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropOpFamilyStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_operator )
        return false;
    if ( t_family )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( t_ehavior )
        return false;
    return true;
}

void DropOpFamilyStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
    }
    visitor->endVisit(this);
}

int AlterOpFamilyAddStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( t_family )
        return t_family;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( t_add )
        return t_add;
    if ( opClasses )
        return opClasses->firstToken();
    return 0;
}

int AlterOpFamilyAddStmtAST::lastToken() const
{
    if ( opClasses )
        return opClasses->lastToken();
    if ( t_add )
        return t_add + 1;
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_family )
        return t_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterOpFamilyAddStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( t_family )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( t_add )
        return false;
    if ( opClasses )
        return false;
    return true;
}

void AlterOpFamilyAddStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
        accept(opClasses, visitor);
    }
    visitor->endVisit(this);
}

int AlterOpFamilyDropStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( t_family )
        return t_family;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( t_drop )
        return t_drop;
    if ( opClasses )
        return opClasses->firstToken();
    return 0;
}

int AlterOpFamilyDropStmtAST::lastToken() const
{
    if ( opClasses )
        return opClasses->lastToken();
    if ( t_drop )
        return t_drop + 1;
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_family )
        return t_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterOpFamilyDropStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( t_family )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( t_drop )
        return false;
    if ( opClasses )
        return false;
    return true;
}

void AlterOpFamilyDropStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
        accept(opClasses, visitor);
    }
    visitor->endVisit(this);
}

int OpClassDropAST::firstToken() const
{
    if ( t_function_or_operator )
        return t_function_or_operator;
    if ( t_sp_value )
        return t_sp_value;
    if ( t_lparen )
        return t_lparen;
    if ( types )
        return types->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int OpClassDropAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( types )
        return types->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_sp_value )
        return t_sp_value + 1;
    if ( t_function_or_operator )
        return t_function_or_operator + 1;
    return 1;
}

bool OpClassDropAST::isNull() const
{
    if ( t_function_or_operator )
        return false;
    if ( t_sp_value )
        return false;
    if ( t_lparen )
        return false;
    if ( types )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void OpClassDropAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(types, visitor);
    }
    visitor->endVisit(this);
}

int CreateOpFamilyStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_operator )
        return t_operator;
    if ( t_family )
        return t_family;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    return 0;
}

int CreateOpFamilyStmtAST::lastToken() const
{
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_family )
        return t_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateOpFamilyStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_operator )
        return false;
    if ( t_family )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    return true;
}

void CreateOpFamilyStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
    }
    visitor->endVisit(this);
}

int CreateOpClassStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_operator )
        return t_operator;
    if ( t_class )
        return t_class;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_default )
        return t_default;
    if ( t_for )
        return t_for;
    if ( t_type )
        return t_type;
    if ( opType && !opType->isNull() )
        return opType->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( opFamily && !opFamily->isNull() )
        return opFamily->firstToken();
    if ( t_as )
        return t_as;
    if ( opClass )
        return opClass->firstToken();
    return 0;
}

int CreateOpClassStmtAST::lastToken() const
{
    if ( opClass )
        return opClass->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( opFamily && !opFamily->isNull() )
        return opFamily->lastToken();
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opType && !opType->isNull() )
        return opType->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_for )
        return t_for + 1;
    if ( t_default )
        return t_default + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_class )
        return t_class + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateOpClassStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_operator )
        return false;
    if ( t_class )
        return false;
    if ( opIdent )
        return false;
    if ( t_default )
        return false;
    if ( t_for )
        return false;
    if ( t_type )
        return false;
    if ( opType )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( opFamily )
        return false;
    if ( t_as )
        return false;
    if ( opClass )
        return false;
    return true;
}

void CreateOpClassStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
        accept(opType, visitor);
        accept(opFamily, visitor);
        accept(opClass, visitor);
    }
    visitor->endVisit(this);
}

int StorageOpClassAST::firstToken() const
{
    if ( t_storage )
        return t_storage;
    if ( storageType && !storageType->isNull() )
        return storageType->firstToken();
    return 0;
}

int StorageOpClassAST::lastToken() const
{
    if ( storageType && !storageType->isNull() )
        return storageType->lastToken();
    if ( t_storage )
        return t_storage + 1;
    return 1;
}

bool StorageOpClassAST::isNull() const
{
    if ( t_storage )
        return false;
    if ( storageType )
        return false;
    return true;
}

void StorageOpClassAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(storageType, visitor);
    }
    visitor->endVisit(this);
}

int OperatorOpClassAST::firstToken() const
{
    if ( t_operator )
        return t_operator;
    if ( t_sp_value )
        return t_sp_value;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( purpose && !purpose->isNull() )
        return purpose->firstToken();
    if ( t_recheck )
        return t_recheck;
    return 0;
}

int OperatorOpClassAST::lastToken() const
{
    if ( t_recheck )
        return t_recheck + 1;
    if ( purpose && !purpose->isNull() )
        return purpose->lastToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_sp_value )
        return t_sp_value + 1;
    if ( t_operator )
        return t_operator + 1;
    return 1;
}

bool OperatorOpClassAST::isNull() const
{
    if ( t_operator )
        return false;
    if ( t_sp_value )
        return false;
    if ( operand )
        return false;
    if ( argTypes )
        return false;
    if ( purpose )
        return false;
    if ( t_recheck )
        return false;
    return true;
}

void OperatorOpClassAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(argTypes, visitor);
        accept(purpose, visitor);
    }
    visitor->endVisit(this);
}

int OpClassSearchPurposeAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_search )
        return t_search;
    return 0;
}

int OpClassSearchPurposeAST::lastToken() const
{
    if ( t_search )
        return t_search + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool OpClassSearchPurposeAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_search )
        return false;
    return true;
}

void OpClassSearchPurposeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int OpClassOrderPurposeAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_order )
        return t_order;
    if ( t_by )
        return t_by;
    if ( orderIdent && !orderIdent->isNull() )
        return orderIdent->firstToken();
    return 0;
}

int OpClassOrderPurposeAST::lastToken() const
{
    if ( orderIdent && !orderIdent->isNull() )
        return orderIdent->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_order )
        return t_order + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool OpClassOrderPurposeAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_order )
        return false;
    if ( t_by )
        return false;
    if ( orderIdent )
        return false;
    return true;
}

void OpClassOrderPurposeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(orderIdent, visitor);
    }
    visitor->endVisit(this);
}

int FunctionOpClassAST::firstToken() const
{
    if ( t_function )
        return t_function;
    if ( t_sp_value )
        return t_sp_value;
    if ( t_lparen )
        return t_lparen;
    if ( types )
        return types->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    return 0;
}

int FunctionOpClassAST::lastToken() const
{
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( types )
        return types->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_sp_value )
        return t_sp_value + 1;
    if ( t_function )
        return t_function + 1;
    return 1;
}

bool FunctionOpClassAST::isNull() const
{
    if ( t_function )
        return false;
    if ( t_sp_value )
        return false;
    if ( t_lparen )
        return false;
    if ( types )
        return false;
    if ( t_rparen )
        return false;
    if ( functionRef )
        return false;
    if ( argTypes )
        return false;
    return true;
}

void FunctionOpClassAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(types, visitor);
        accept(functionRef, visitor);
        accept(argTypes, visitor);
    }
    visitor->endVisit(this);
}

int OpFamilyAST::firstToken() const
{
    if ( t_family )
        return t_family;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    return 0;
}

int OpFamilyAST::lastToken() const
{
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_family )
        return t_family + 1;
    return 1;
}

bool OpFamilyAST::isNull() const
{
    if ( t_family )
        return false;
    if ( ident )
        return false;
    return true;
}

void OpFamilyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int CreateMatViewStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_unlogged )
        return t_unlogged;
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( target && !target->isNull() )
        return target->firstToken();
    if ( t_as )
        return t_as;
    if ( query && !query->isNull() )
        return query->firstToken();
    if ( withDataClause && !withDataClause->isNull() )
        return withDataClause->firstToken();
    return 0;
}

int CreateMatViewStmtAST::lastToken() const
{
    if ( withDataClause && !withDataClause->isNull() )
        return withDataClause->lastToken();
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( target && !target->isNull() )
        return target->lastToken();
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    if ( t_unlogged )
        return t_unlogged + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateMatViewStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_unlogged )
        return false;
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( target )
        return false;
    if ( t_as )
        return false;
    if ( query )
        return false;
    if ( withDataClause )
        return false;
    return true;
}

void CreateMatViewStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(target, visitor);
        accept(query, visitor);
        accept(withDataClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateMvTargetAST::firstToken() const
{
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->firstToken();
    if ( columns && !columns->isNull() )
        return columns->firstToken();
    if ( reloptions && !reloptions->isNull() )
        return reloptions->firstToken();
    if ( tablespaceClause && !tablespaceClause->isNull() )
        return tablespaceClause->firstToken();
    return 0;
}

int CreateMvTargetAST::lastToken() const
{
    if ( tablespaceClause && !tablespaceClause->isNull() )
        return tablespaceClause->lastToken();
    if ( reloptions && !reloptions->isNull() )
        return reloptions->lastToken();
    if ( columns && !columns->isNull() )
        return columns->lastToken();
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->lastToken();
    return 1;
}

bool CreateMvTargetAST::isNull() const
{
    if ( viewIdent )
        return false;
    if ( columns )
        return false;
    if ( reloptions )
        return false;
    if ( tablespaceClause )
        return false;
    return true;
}

void CreateMvTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewIdent, visitor);
        accept(columns, visitor);
        accept(reloptions, visitor);
        accept(tablespaceClause, visitor);
    }
    visitor->endVisit(this);
}

int WithReloptionsAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( relOptions && !relOptions->isNull() )
        return relOptions->firstToken();
    return 0;
}

int WithReloptionsAST::lastToken() const
{
    if ( relOptions && !relOptions->isNull() )
        return relOptions->lastToken();
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithReloptionsAST::isNull() const
{
    if ( t_with )
        return false;
    if ( relOptions )
        return false;
    return true;
}

void WithReloptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relOptions, visitor);
    }
    visitor->endVisit(this);
}

int CreateFunctionStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( orReplace && !orReplace->isNull() )
        return orReplace->firstToken();
    if ( t_function )
        return t_function;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( t_returns )
        return t_returns;
    if ( funcReturn && !funcReturn->isNull() )
        return funcReturn->firstToken();
    if ( options )
        return options->firstToken();
    if ( defs && !defs->isNull() )
        return defs->firstToken();
    return 0;
}

int CreateFunctionStmtAST::lastToken() const
{
    if ( defs && !defs->isNull() )
        return defs->lastToken();
    if ( options )
        return options->lastToken();
    if ( funcReturn && !funcReturn->isNull() )
        return funcReturn->lastToken();
    if ( t_returns )
        return t_returns + 1;
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( orReplace && !orReplace->isNull() )
        return orReplace->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateFunctionStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( orReplace )
        return false;
    if ( t_function )
        return false;
    if ( functionRef )
        return false;
    if ( args )
        return false;
    if ( t_returns )
        return false;
    if ( funcReturn )
        return false;
    if ( options )
        return false;
    if ( defs )
        return false;
    return true;
}

void CreateFunctionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(orReplace, visitor);
        accept(functionRef, visitor);
        accept(args, visitor);
        accept(funcReturn, visitor);
        accept(options, visitor);
        accept(defs, visitor);
    }
    visitor->endVisit(this);
}

int FuncTypeFuncReturnAST::firstToken() const
{
    if ( functionType && !functionType->isNull() )
        return functionType->firstToken();
    return 0;
}

int FuncTypeFuncReturnAST::lastToken() const
{
    if ( functionType && !functionType->isNull() )
        return functionType->lastToken();
    return 1;
}

bool FuncTypeFuncReturnAST::isNull() const
{
    if ( functionType )
        return false;
    return true;
}

void FuncTypeFuncReturnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionType, visitor);
    }
    visitor->endVisit(this);
}

int TableFuncReturnAST::firstToken() const
{
    if ( t_table )
        return t_table;
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TableFuncReturnAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_table )
        return t_table + 1;
    return 1;
}

bool TableFuncReturnAST::isNull() const
{
    if ( t_table )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TableFuncReturnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int TableFuncColumnAST::firstToken() const
{
    if ( paramName && !paramName->isNull() )
        return paramName->firstToken();
    if ( functionType && !functionType->isNull() )
        return functionType->firstToken();
    return 0;
}

int TableFuncColumnAST::lastToken() const
{
    if ( functionType && !functionType->isNull() )
        return functionType->lastToken();
    if ( paramName && !paramName->isNull() )
        return paramName->lastToken();
    return 1;
}

bool TableFuncColumnAST::isNull() const
{
    if ( paramName )
        return false;
    if ( functionType )
        return false;
    return true;
}

void TableFuncColumnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(paramName, visitor);
        accept(functionType, visitor);
    }
    visitor->endVisit(this);
}

int CreateFuncAsOptionAST::firstToken() const
{
    if ( t_as )
        return t_as;
    if ( asClause && !asClause->isNull() )
        return asClause->firstToken();
    return 0;
}

int CreateFuncAsOptionAST::lastToken() const
{
    if ( asClause && !asClause->isNull() )
        return asClause->lastToken();
    if ( t_as )
        return t_as + 1;
    return 1;
}

bool CreateFuncAsOptionAST::isNull() const
{
    if ( t_as )
        return false;
    if ( asClause )
        return false;
    return true;
}

void CreateFuncAsOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(asClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateFuncLangOptionAST::firstToken() const
{
    if ( t_language )
        return t_language;
    if ( t_lang_value )
        return t_lang_value;
    return 0;
}

int CreateFuncLangOptionAST::lastToken() const
{
    if ( t_lang_value )
        return t_lang_value + 1;
    if ( t_language )
        return t_language + 1;
    return 1;
}

bool CreateFuncLangOptionAST::isNull() const
{
    if ( t_language )
        return false;
    if ( t_lang_value )
        return false;
    return true;
}

void CreateFuncLangOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateFuncWindowOptionAST::firstToken() const
{
    if ( t_window )
        return t_window;
    return 0;
}

int CreateFuncWindowOptionAST::lastToken() const
{
    if ( t_window )
        return t_window + 1;
    return 1;
}

bool CreateFuncWindowOptionAST::isNull() const
{
    if ( t_window )
        return false;
    return true;
}

void CreateFuncWindowOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateFuncCommonOptionAST::firstToken() const
{
    if ( commonOpt && !commonOpt->isNull() )
        return commonOpt->firstToken();
    return 0;
}

int CreateFuncCommonOptionAST::lastToken() const
{
    if ( commonOpt && !commonOpt->isNull() )
        return commonOpt->lastToken();
    return 1;
}

bool CreateFuncCommonOptionAST::isNull() const
{
    if ( commonOpt )
        return false;
    return true;
}

void CreateFuncCommonOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(commonOpt, visitor);
    }
    visitor->endVisit(this);
}

int FunctionAsClauseAST::firstToken() const
{
    if ( t_first_value )
        return t_first_value;
    if ( t_comma )
        return t_comma;
    if ( t_second_value )
        return t_second_value;
    return 0;
}

int FunctionAsClauseAST::lastToken() const
{
    if ( t_second_value )
        return t_second_value + 1;
    if ( t_comma )
        return t_comma + 1;
    if ( t_first_value )
        return t_first_value + 1;
    return 1;
}

bool FunctionAsClauseAST::isNull() const
{
    if ( t_first_value )
        return false;
    if ( t_comma )
        return false;
    if ( t_second_value )
        return false;
    return true;
}

void FunctionAsClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int FuncArgsWithDefaultsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int FuncArgsWithDefaultsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool FuncArgsWithDefaultsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void FuncArgsWithDefaultsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int FuncArgWithDefaultAST::firstToken() const
{
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    if ( t_default_or_equal )
        return t_default_or_equal;
    if ( defExpr && !defExpr->isNull() )
        return defExpr->firstToken();
    return 0;
}

int FuncArgWithDefaultAST::lastToken() const
{
    if ( defExpr && !defExpr->isNull() )
        return defExpr->lastToken();
    if ( t_default_or_equal )
        return t_default_or_equal + 1;
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    return 1;
}

bool FuncArgWithDefaultAST::isNull() const
{
    if ( arg )
        return false;
    if ( t_default_or_equal )
        return false;
    if ( defExpr )
        return false;
    return true;
}

void FuncArgWithDefaultAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arg, visitor);
        accept(defExpr, visitor);
    }
    visitor->endVisit(this);
}

int OrReplaceClauseAST::firstToken() const
{
    if ( t_or )
        return t_or;
    if ( t_replace )
        return t_replace;
    return 0;
}

int OrReplaceClauseAST::lastToken() const
{
    if ( t_replace )
        return t_replace + 1;
    if ( t_or )
        return t_or + 1;
    return 1;
}

bool OrReplaceClauseAST::isNull() const
{
    if ( t_or )
        return false;
    if ( t_replace )
        return false;
    return true;
}

void OrReplaceClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateForeignTableStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_not )
        return t_not;
    if ( t_exists )
        return t_exists;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( genericOptions && !genericOptions->isNull() )
        return genericOptions->firstToken();
    return 0;
}

int CreateForeignTableStmtAST::lastToken() const
{
    if ( genericOptions && !genericOptions->isNull() )
        return genericOptions->lastToken();
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateForeignTableStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_not )
        return false;
    if ( t_exists )
        return false;
    if ( tableIdent )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    if ( genericOptions )
        return false;
    return true;
}

void CreateForeignTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(columns, visitor);
        accept(serverName, visitor);
        accept(genericOptions, visitor);
    }
    visitor->endVisit(this);
}

int ColumnDefTableElementAST::firstToken() const
{
    if ( columnDef && !columnDef->isNull() )
        return columnDef->firstToken();
    return 0;
}

int ColumnDefTableElementAST::lastToken() const
{
    if ( columnDef && !columnDef->isNull() )
        return columnDef->lastToken();
    return 1;
}

bool ColumnDefTableElementAST::isNull() const
{
    if ( columnDef )
        return false;
    return true;
}

void ColumnDefTableElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnDef, visitor);
    }
    visitor->endVisit(this);
}

int LikeTableElementAST::firstToken() const
{
    if ( tableLikeClause && !tableLikeClause->isNull() )
        return tableLikeClause->firstToken();
    return 0;
}

int LikeTableElementAST::lastToken() const
{
    if ( tableLikeClause && !tableLikeClause->isNull() )
        return tableLikeClause->lastToken();
    return 1;
}

bool LikeTableElementAST::isNull() const
{
    if ( tableLikeClause )
        return false;
    return true;
}

void LikeTableElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableLikeClause, visitor);
    }
    visitor->endVisit(this);
}

int TableLikeClauseAST::firstToken() const
{
    if ( t_like )
        return t_like;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( options )
        return options->firstToken();
    return 0;
}

int TableLikeClauseAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_like )
        return t_like + 1;
    return 1;
}

bool TableLikeClauseAST::isNull() const
{
    if ( t_like )
        return false;
    if ( tableIdent )
        return false;
    if ( options )
        return false;
    return true;
}

void TableLikeClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int TableLikeOptionAST::firstToken() const
{
    if ( t_sp_kwd )
        return t_sp_kwd;
    return 0;
}

int TableLikeOptionAST::lastToken() const
{
    if ( t_sp_kwd )
        return t_sp_kwd + 1;
    return 1;
}

bool TableLikeOptionAST::isNull() const
{
    if ( t_sp_kwd )
        return false;
    return true;
}

void TableLikeOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ConstraintTableElementAST::firstToken() const
{
    if ( constraint && !constraint->isNull() )
        return constraint->firstToken();
    return 0;
}

int ConstraintTableElementAST::lastToken() const
{
    if ( constraint && !constraint->isNull() )
        return constraint->lastToken();
    return 1;
}

bool ConstraintTableElementAST::isNull() const
{
    if ( constraint )
        return false;
    return true;
}

void ConstraintTableElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraint, visitor);
    }
    visitor->endVisit(this);
}

int CreateForeignServerStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( serverType && !serverType->isNull() )
        return serverType->firstToken();
    if ( serverVersion && !serverVersion->isNull() )
        return serverVersion->firstToken();
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->firstToken();
    if ( genericOptions && !genericOptions->isNull() )
        return genericOptions->firstToken();
    return 0;
}

int CreateForeignServerStmtAST::lastToken() const
{
    if ( genericOptions && !genericOptions->isNull() )
        return genericOptions->lastToken();
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( serverVersion && !serverVersion->isNull() )
        return serverVersion->lastToken();
    if ( serverType && !serverType->isNull() )
        return serverType->lastToken();
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateForeignServerStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    if ( serverType )
        return false;
    if ( serverVersion )
        return false;
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( wrapperName )
        return false;
    if ( genericOptions )
        return false;
    return true;
}

void CreateForeignServerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(serverName, visitor);
        accept(serverType, visitor);
        accept(serverVersion, visitor);
        accept(wrapperName, visitor);
        accept(genericOptions, visitor);
    }
    visitor->endVisit(this);
}

int ServerTypeClauseAST::firstToken() const
{
    if ( t_type )
        return t_type;
    if ( t_type_value )
        return t_type_value;
    return 0;
}

int ServerTypeClauseAST::lastToken() const
{
    if ( t_type_value )
        return t_type_value + 1;
    if ( t_type )
        return t_type + 1;
    return 1;
}

bool ServerTypeClauseAST::isNull() const
{
    if ( t_type )
        return false;
    if ( t_type_value )
        return false;
    return true;
}

void ServerTypeClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateFdwStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->firstToken();
    if ( options )
        return options->firstToken();
    if ( genericOptions && !genericOptions->isNull() )
        return genericOptions->firstToken();
    return 0;
}

int CreateFdwStmtAST::lastToken() const
{
    if ( genericOptions && !genericOptions->isNull() )
        return genericOptions->lastToken();
    if ( options )
        return options->lastToken();
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateFdwStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( wrapperName )
        return false;
    if ( options )
        return false;
    if ( genericOptions )
        return false;
    return true;
}

void CreateFdwStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(wrapperName, visitor);
        accept(options, visitor);
        accept(genericOptions, visitor);
    }
    visitor->endVisit(this);
}

int CreateExtensionStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_extension )
        return t_extension;
    if ( t_if )
        return t_if;
    if ( t_not )
        return t_not;
    if ( t_exists )
        return t_exists;
    if ( extensionName && !extensionName->isNull() )
        return extensionName->firstToken();
    if ( t_with )
        return t_with;
    if ( options )
        return options->firstToken();
    return 0;
}

int CreateExtensionStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( extensionName && !extensionName->isNull() )
        return extensionName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_extension )
        return t_extension + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateExtensionStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_extension )
        return false;
    if ( t_if )
        return false;
    if ( t_not )
        return false;
    if ( t_exists )
        return false;
    if ( extensionName )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateExtensionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(extensionName, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int CreateExtSchemaOptionAST::firstToken() const
{
    if ( t_schema )
        return t_schema;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    return 0;
}

int CreateExtSchemaOptionAST::lastToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    return 1;
}

bool CreateExtSchemaOptionAST::isNull() const
{
    if ( t_schema )
        return false;
    if ( schemaName )
        return false;
    return true;
}

void CreateExtSchemaOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
    }
    visitor->endVisit(this);
}

int CreateExtVerFromOptionAST::firstToken() const
{
    if ( t_version_or_from )
        return t_version_or_from;
    if ( t_sp_value )
        return t_sp_value;
    return 0;
}

int CreateExtVerFromOptionAST::lastToken() const
{
    if ( t_sp_value )
        return t_sp_value + 1;
    if ( t_version_or_from )
        return t_version_or_from + 1;
    return 1;
}

bool CreateExtVerFromOptionAST::isNull() const
{
    if ( t_version_or_from )
        return false;
    if ( t_sp_value )
        return false;
    return true;
}

void CreateExtVerFromOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateConversionStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_default )
        return t_default;
    if ( t_conversion )
        return t_conversion;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_for )
        return t_for;
    if ( t_for_value )
        return t_for_value;
    if ( t_to )
        return t_to;
    if ( t_to_value )
        return t_to_value;
    if ( t_from )
        return t_from;
    if ( fromIdent && !fromIdent->isNull() )
        return fromIdent->firstToken();
    return 0;
}

int CreateConversionStmtAST::lastToken() const
{
    if ( fromIdent && !fromIdent->isNull() )
        return fromIdent->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( t_to_value )
        return t_to_value + 1;
    if ( t_to )
        return t_to + 1;
    if ( t_for_value )
        return t_for_value + 1;
    if ( t_for )
        return t_for + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_conversion )
        return t_conversion + 1;
    if ( t_default )
        return t_default + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateConversionStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_default )
        return false;
    if ( t_conversion )
        return false;
    if ( ident )
        return false;
    if ( t_for )
        return false;
    if ( t_for_value )
        return false;
    if ( t_to )
        return false;
    if ( t_to_value )
        return false;
    if ( t_from )
        return false;
    if ( fromIdent )
        return false;
    return true;
}

void CreateConversionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(fromIdent, visitor);
    }
    visitor->endVisit(this);
}

int CreateCastStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_cast )
        return t_cast;
    if ( t_lparen )
        return t_lparen;
    if ( fromType && !fromType->isNull() )
        return fromType->firstToken();
    if ( t_as )
        return t_as;
    if ( toType && !toType->isNull() )
        return toType->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( t_with_or_without )
        return t_with_or_without;
    if ( t_function_or_inout )
        return t_function_or_inout;
    if ( functionWithArgTypes && !functionWithArgTypes->isNull() )
        return functionWithArgTypes->firstToken();
    if ( context && !context->isNull() )
        return context->firstToken();
    return 0;
}

int CreateCastStmtAST::lastToken() const
{
    if ( context && !context->isNull() )
        return context->lastToken();
    if ( functionWithArgTypes && !functionWithArgTypes->isNull() )
        return functionWithArgTypes->lastToken();
    if ( t_function_or_inout )
        return t_function_or_inout + 1;
    if ( t_with_or_without )
        return t_with_or_without + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( toType && !toType->isNull() )
        return toType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( fromType && !fromType->isNull() )
        return fromType->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_cast )
        return t_cast + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateCastStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_cast )
        return false;
    if ( t_lparen )
        return false;
    if ( fromType )
        return false;
    if ( t_as )
        return false;
    if ( toType )
        return false;
    if ( t_rparen )
        return false;
    if ( t_with_or_without )
        return false;
    if ( t_function_or_inout )
        return false;
    if ( functionWithArgTypes )
        return false;
    if ( context )
        return false;
    return true;
}

void CreateCastStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fromType, visitor);
        accept(toType, visitor);
        accept(functionWithArgTypes, visitor);
        accept(context, visitor);
    }
    visitor->endVisit(this);
}

int CastContextAST::firstToken() const
{
    if ( t_as )
        return t_as;
    if ( t_implicit_or_assignment )
        return t_implicit_or_assignment;
    return 0;
}

int CastContextAST::lastToken() const
{
    if ( t_implicit_or_assignment )
        return t_implicit_or_assignment + 1;
    if ( t_as )
        return t_as + 1;
    return 1;
}

bool CastContextAST::isNull() const
{
    if ( t_as )
        return false;
    if ( t_implicit_or_assignment )
        return false;
    return true;
}

void CastContextAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DropCastStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_cast )
        return t_cast;
    if ( ifExists && !ifExists->isNull() )
        return ifExists->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( fromType && !fromType->isNull() )
        return fromType->firstToken();
    if ( t_as )
        return t_as;
    if ( toType && !toType->isNull() )
        return toType->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropCastStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( toType && !toType->isNull() )
        return toType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( fromType && !fromType->isNull() )
        return fromType->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( ifExists && !ifExists->isNull() )
        return ifExists->lastToken();
    if ( t_cast )
        return t_cast + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropCastStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_cast )
        return false;
    if ( ifExists )
        return false;
    if ( t_lparen )
        return false;
    if ( fromType )
        return false;
    if ( t_as )
        return false;
    if ( toType )
        return false;
    if ( t_rparen )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropCastStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ifExists, visitor);
        accept(fromType, visitor);
        accept(toType, visitor);
    }
    visitor->endVisit(this);
}

int IfExistsAST::firstToken() const
{
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    return 0;
}

int IfExistsAST::lastToken() const
{
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    return 1;
}

bool IfExistsAST::isNull() const
{
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    return true;
}

void IfExistsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DropAssertStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_assertion )
        return t_assertion;
    if ( assertName && !assertName->isNull() )
        return assertName->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int DropAssertStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( assertName && !assertName->isNull() )
        return assertName->lastToken();
    if ( t_assertion )
        return t_assertion + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropAssertStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_assertion )
        return false;
    if ( assertName )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void DropAssertStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(assertName, visitor);
    }
    visitor->endVisit(this);
}

int CreateAssertStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_assertion )
        return t_assertion;
    if ( assertName && !assertName->isNull() )
        return assertName->firstToken();
    if ( t_check )
        return t_check;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( constrAttrs )
        return constrAttrs->firstToken();
    return 0;
}

int CreateAssertStmtAST::lastToken() const
{
    if ( constrAttrs )
        return constrAttrs->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_check )
        return t_check + 1;
    if ( assertName && !assertName->isNull() )
        return assertName->lastToken();
    if ( t_assertion )
        return t_assertion + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateAssertStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_assertion )
        return false;
    if ( assertName )
        return false;
    if ( t_check )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    if ( constrAttrs )
        return false;
    return true;
}

void CreateAssertStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(assertName, visitor);
        accept(expr, visitor);
        accept(constrAttrs, visitor);
    }
    visitor->endVisit(this);
}

int CreateTableAsStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->firstToken();
    if ( t_table )
        return t_table;
    if ( createTarget && !createTarget->isNull() )
        return createTarget->firstToken();
    if ( t_as )
        return t_as;
    if ( query && !query->isNull() )
        return query->firstToken();
    if ( withDataClause && !withDataClause->isNull() )
        return withDataClause->firstToken();
    return 0;
}

int CreateTableAsStmtAST::lastToken() const
{
    if ( withDataClause && !withDataClause->isNull() )
        return withDataClause->lastToken();
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( createTarget && !createTarget->isNull() )
        return createTarget->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateTableAsStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( tempClause )
        return false;
    if ( t_table )
        return false;
    if ( createTarget )
        return false;
    if ( t_as )
        return false;
    if ( query )
        return false;
    if ( withDataClause )
        return false;
    return true;
}

void CreateTableAsStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tempClause, visitor);
        accept(createTarget, visitor);
        accept(query, visitor);
        accept(withDataClause, visitor);
    }
    visitor->endVisit(this);
}

int CreateAsTargetAST::firstToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( columns && !columns->isNull() )
        return columns->firstToken();
    if ( createOpts && !createOpts->isNull() )
        return createOpts->firstToken();
    if ( onCommitOption && !onCommitOption->isNull() )
        return onCommitOption->firstToken();
    if ( tablespceOpt && !tablespceOpt->isNull() )
        return tablespceOpt->firstToken();
    return 0;
}

int CreateAsTargetAST::lastToken() const
{
    if ( tablespceOpt && !tablespceOpt->isNull() )
        return tablespceOpt->lastToken();
    if ( onCommitOption && !onCommitOption->isNull() )
        return onCommitOption->lastToken();
    if ( createOpts && !createOpts->isNull() )
        return createOpts->lastToken();
    if ( columns && !columns->isNull() )
        return columns->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    return 1;
}

bool CreateAsTargetAST::isNull() const
{
    if ( tableIdent )
        return false;
    if ( columns )
        return false;
    if ( createOpts )
        return false;
    if ( onCommitOption )
        return false;
    if ( tablespceOpt )
        return false;
    return true;
}

void CreateAsTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(columns, visitor);
        accept(createOpts, visitor);
        accept(onCommitOption, visitor);
        accept(tablespceOpt, visitor);
    }
    visitor->endVisit(this);
}

int TableSpaceClauseAST::firstToken() const
{
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    return 0;
}

int TableSpaceClauseAST::lastToken() const
{
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    return 1;
}

bool TableSpaceClauseAST::isNull() const
{
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    return true;
}

void TableSpaceClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
    }
    visitor->endVisit(this);
}

int OnCommitOptionAST::firstToken() const
{
    if ( t_on )
        return t_on;
    if ( t_commit )
        return t_commit;
    if ( t_sp_act )
        return t_sp_act;
    if ( t_rows )
        return t_rows;
    return 0;
}

int OnCommitOptionAST::lastToken() const
{
    if ( t_rows )
        return t_rows + 1;
    if ( t_sp_act )
        return t_sp_act + 1;
    if ( t_commit )
        return t_commit + 1;
    if ( t_on )
        return t_on + 1;
    return 1;
}

bool OnCommitOptionAST::isNull() const
{
    if ( t_on )
        return false;
    if ( t_commit )
        return false;
    if ( t_sp_act )
        return false;
    if ( t_rows )
        return false;
    return true;
}

void OnCommitOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateWithRelOptionsAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( opts && !opts->isNull() )
        return opts->firstToken();
    return 0;
}

int CreateWithRelOptionsAST::lastToken() const
{
    if ( opts && !opts->isNull() )
        return opts->lastToken();
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool CreateWithRelOptionsAST::isNull() const
{
    if ( t_with )
        return false;
    if ( opts )
        return false;
    return true;
}

void CreateWithRelOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opts, visitor);
    }
    visitor->endVisit(this);
}

int CreateWithOidsOptionsAST::firstToken() const
{
    if ( t_with_or_without )
        return t_with_or_without;
    if ( t_oids )
        return t_oids;
    return 0;
}

int CreateWithOidsOptionsAST::lastToken() const
{
    if ( t_oids )
        return t_oids + 1;
    if ( t_with_or_without )
        return t_with_or_without + 1;
    return 1;
}

bool CreateWithOidsOptionsAST::isNull() const
{
    if ( t_with_or_without )
        return false;
    if ( t_oids )
        return false;
    return true;
}

void CreateWithOidsOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int WithDataClauseAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( t_no )
        return t_no;
    if ( t_data )
        return t_data;
    return 0;
}

int WithDataClauseAST::lastToken() const
{
    if ( t_data )
        return t_data + 1;
    if ( t_no )
        return t_no + 1;
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithDataClauseAST::isNull() const
{
    if ( t_with )
        return false;
    if ( t_no )
        return false;
    if ( t_data )
        return false;
    return true;
}

void WithDataClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TableCopyStmtAST::firstToken() const
{
    if ( t_copy )
        return t_copy;
    if ( t_binary )
        return t_binary;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( columns && !columns->isNull() )
        return columns->firstToken();
    if ( oids && !oids->isNull() )
        return oids->firstToken();
    if ( t_from_or_to )
        return t_from_or_to;
    if ( t_program )
        return t_program;
    if ( t_file_name )
        return t_file_name;
    if ( delimiter && !delimiter->isNull() )
        return delimiter->firstToken();
    if ( t_with )
        return t_with;
    if ( options && !options->isNull() )
        return options->firstToken();
    return 0;
}

int TableCopyStmtAST::lastToken() const
{
    if ( options && !options->isNull() )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( delimiter && !delimiter->isNull() )
        return delimiter->lastToken();
    if ( t_file_name )
        return t_file_name + 1;
    if ( t_program )
        return t_program + 1;
    if ( t_from_or_to )
        return t_from_or_to + 1;
    if ( oids && !oids->isNull() )
        return oids->lastToken();
    if ( columns && !columns->isNull() )
        return columns->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_binary )
        return t_binary + 1;
    if ( t_copy )
        return t_copy + 1;
    return 1;
}

bool TableCopyStmtAST::isNull() const
{
    if ( t_copy )
        return false;
    if ( t_binary )
        return false;
    if ( tableIdent )
        return false;
    if ( columns )
        return false;
    if ( oids )
        return false;
    if ( t_from_or_to )
        return false;
    if ( t_program )
        return false;
    if ( t_file_name )
        return false;
    if ( delimiter )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void TableCopyStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(columns, visitor);
        accept(oids, visitor);
        accept(delimiter, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int WithOidsClauseAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( t_oids )
        return t_oids;
    return 0;
}

int WithOidsClauseAST::lastToken() const
{
    if ( t_oids )
        return t_oids + 1;
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithOidsClauseAST::isNull() const
{
    if ( t_with )
        return false;
    if ( t_oids )
        return false;
    return true;
}

void WithOidsClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CopyDelimiterAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( t_delimiters )
        return t_delimiters;
    if ( t_delim_value )
        return t_delim_value;
    return 0;
}

int CopyDelimiterAST::lastToken() const
{
    if ( t_delim_value )
        return t_delim_value + 1;
    if ( t_delimiters )
        return t_delimiters + 1;
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool CopyDelimiterAST::isNull() const
{
    if ( t_using )
        return false;
    if ( t_delimiters )
        return false;
    if ( t_delim_value )
        return false;
    return true;
}

void CopyDelimiterAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SelectCopyStmtAST::firstToken() const
{
    if ( t_copy )
        return t_copy;
    if ( query && !query->isNull() )
        return query->firstToken();
    if ( t_to )
        return t_to;
    if ( t_program )
        return t_program;
    if ( t_copy_file_name )
        return t_copy_file_name;
    if ( t_with )
        return t_with;
    if ( options && !options->isNull() )
        return options->firstToken();
    return 0;
}

int SelectCopyStmtAST::lastToken() const
{
    if ( options && !options->isNull() )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( t_copy_file_name )
        return t_copy_file_name + 1;
    if ( t_program )
        return t_program + 1;
    if ( t_to )
        return t_to + 1;
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_copy )
        return t_copy + 1;
    return 1;
}

bool SelectCopyStmtAST::isNull() const
{
    if ( t_copy )
        return false;
    if ( query )
        return false;
    if ( t_to )
        return false;
    if ( t_program )
        return false;
    if ( t_copy_file_name )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void SelectCopyStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(query, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int NewCopyOptionsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( opts )
        return opts->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int NewCopyOptionsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( opts )
        return opts->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool NewCopyOptionsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( opts )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void NewCopyOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opts, visitor);
    }
    visitor->endVisit(this);
}

int GenericCopyOptionAST::firstToken() const
{
    if ( t_label )
        return t_label;
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    return 0;
}

int GenericCopyOptionAST::lastToken() const
{
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    if ( t_label )
        return t_label + 1;
    return 1;
}

bool GenericCopyOptionAST::isNull() const
{
    if ( t_label )
        return false;
    if ( arg )
        return false;
    return true;
}

void GenericCopyOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arg, visitor);
    }
    visitor->endVisit(this);
}

int GenericCopyOptionValArgAST::firstToken() const
{
    if ( t_star_or_value )
        return t_star_or_value;
    return 0;
}

int GenericCopyOptionValArgAST::lastToken() const
{
    if ( t_star_or_value )
        return t_star_or_value + 1;
    return 1;
}

bool GenericCopyOptionValArgAST::isNull() const
{
    if ( t_star_or_value )
        return false;
    return true;
}

void GenericCopyOptionValArgAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int GenericCopyOptionCmpArgAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( items )
        return items->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int GenericCopyOptionCmpArgAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( items )
        return items->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool GenericCopyOptionCmpArgAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( items )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void GenericCopyOptionCmpArgAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(items, visitor);
    }
    visitor->endVisit(this);
}

int GenericCopyOptionCmpArgItemAST::firstToken() const
{
    if ( t_bool_or_str_value )
        return t_bool_or_str_value;
    return 0;
}

int GenericCopyOptionCmpArgItemAST::lastToken() const
{
    if ( t_bool_or_str_value )
        return t_bool_or_str_value + 1;
    return 1;
}

bool GenericCopyOptionCmpArgItemAST::isNull() const
{
    if ( t_bool_or_str_value )
        return false;
    return true;
}

void GenericCopyOptionCmpArgItemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int OldCopyOptionsAST::firstToken() const
{
    if ( opts )
        return opts->firstToken();
    return 0;
}

int OldCopyOptionsAST::lastToken() const
{
    if ( opts )
        return opts->lastToken();
    return 1;
}

bool OldCopyOptionsAST::isNull() const
{
    if ( opts )
        return false;
    return true;
}

void OldCopyOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opts, visitor);
    }
    visitor->endVisit(this);
}

int CopyOptionAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    if ( t_sp3 )
        return t_sp3;
    if ( columns )
        return columns->firstToken();
    return 0;
}

int CopyOptionAST::lastToken() const
{
    if ( columns )
        return columns->lastToken();
    if ( t_sp3 )
        return t_sp3 + 1;
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool CopyOptionAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    if ( t_sp3 )
        return false;
    if ( columns )
        return false;
    return true;
}

void CopyOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintsSetStmtAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_constraints )
        return t_constraints;
    if ( constrList && !constrList->isNull() )
        return constrList->firstToken();
    if ( t_set_mode )
        return t_set_mode;
    return 0;
}

int ConstraintsSetStmtAST::lastToken() const
{
    if ( t_set_mode )
        return t_set_mode + 1;
    if ( constrList && !constrList->isNull() )
        return constrList->lastToken();
    if ( t_constraints )
        return t_constraints + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool ConstraintsSetStmtAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_constraints )
        return false;
    if ( constrList )
        return false;
    if ( t_set_mode )
        return false;
    return true;
}

void ConstraintsSetStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constrList, visitor);
    }
    visitor->endVisit(this);
}

int AllConstraintsSetListAST::firstToken() const
{
    if ( t_all )
        return t_all;
    return 0;
}

int AllConstraintsSetListAST::lastToken() const
{
    if ( t_all )
        return t_all + 1;
    return 1;
}

bool AllConstraintsSetListAST::isNull() const
{
    if ( t_all )
        return false;
    return true;
}

void AllConstraintsSetListAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommonConstraintsSetListAST::firstToken() const
{
    if ( idents )
        return idents->firstToken();
    return 0;
}

int CommonConstraintsSetListAST::lastToken() const
{
    if ( idents )
        return idents->lastToken();
    return 1;
}

bool CommonConstraintsSetListAST::isNull() const
{
    if ( idents )
        return false;
    return true;
}

void CommonConstraintsSetListAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(idents, visitor);
    }
    visitor->endVisit(this);
}

int LangCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( langIdent && !langIdent->isNull() )
        return langIdent->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int LangCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( langIdent && !langIdent->isNull() )
        return langIdent->lastToken();
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool LangCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( langIdent )
        return false;
    if ( commentText )
        return false;
    return true;
}

void LangCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(langIdent, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int CastCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_cast )
        return t_cast;
    if ( t_lparen )
        return t_lparen;
    if ( fromType && !fromType->isNull() )
        return fromType->firstToken();
    if ( t_as )
        return t_as;
    if ( toType && !toType->isNull() )
        return toType->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int CastCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( toType && !toType->isNull() )
        return toType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( fromType && !fromType->isNull() )
        return fromType->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_cast )
        return t_cast + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool CastCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_cast )
        return false;
    if ( t_lparen )
        return false;
    if ( fromType )
        return false;
    if ( t_as )
        return false;
    if ( toType )
        return false;
    if ( t_rparen )
        return false;
    if ( commentText )
        return false;
    return true;
}

void CastCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fromType, visitor);
        accept(toType, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int LargeObjCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_large )
        return t_large;
    if ( t_oject )
        return t_oject;
    if ( t_any_digit )
        return t_any_digit;
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int LargeObjCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( t_any_digit )
        return t_any_digit + 1;
    if ( t_oject )
        return t_oject + 1;
    if ( t_large )
        return t_large + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool LargeObjCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_large )
        return false;
    if ( t_oject )
        return false;
    if ( t_any_digit )
        return false;
    if ( commentText )
        return false;
    return true;
}

void LargeObjCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int OpUsingCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_operator )
        return t_operator;
    if ( t_class_or_family )
        return t_class_or_family;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int OpUsingCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_class_or_family )
        return t_class_or_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool OpUsingCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_operator )
        return false;
    if ( t_class_or_family )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( commentText )
        return false;
    return true;
}

void OpUsingCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_constraint )
        return t_constraint;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    if ( onName && !onName->isNull() )
        return onName->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int ConstraintCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( onName && !onName->isNull() )
        return onName->lastToken();
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool ConstraintCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_constraint )
        return false;
    if ( constraintName )
        return false;
    if ( onName )
        return false;
    if ( commentText )
        return false;
    return true;
}

void ConstraintCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintName, visitor);
        accept(onName, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int RuleCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_rule )
        return t_rule;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->firstToken();
    if ( onName && !onName->isNull() )
        return onName->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int RuleCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( onName && !onName->isNull() )
        return onName->lastToken();
    if ( ruleName && !ruleName->isNull() )
        return ruleName->lastToken();
    if ( t_rule )
        return t_rule + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool RuleCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_rule )
        return false;
    if ( ruleName )
        return false;
    if ( onName )
        return false;
    if ( commentText )
        return false;
    return true;
}

void RuleCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ruleName, visitor);
        accept(onName, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int TriggerCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( onName && !onName->isNull() )
        return onName->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int TriggerCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( onName && !onName->isNull() )
        return onName->lastToken();
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool TriggerCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( onName )
        return false;
    if ( commentText )
        return false;
    return true;
}

void TriggerCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(onName, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int CommentOnNameAST::firstToken() const
{
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    return 0;
}

int CommentOnNameAST::lastToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    return 1;
}

bool CommentOnNameAST::isNull() const
{
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    return true;
}

void CommentOnNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int OpCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_operator )
        return t_operator;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int OpCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_operator )
        return t_operator + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool OpCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_operator )
        return false;
    if ( operand )
        return false;
    if ( argTypes )
        return false;
    if ( commentText )
        return false;
    return true;
}

void OpCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(argTypes, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int FunctionCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_function )
        return t_function;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int FunctionCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool FunctionCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_function )
        return false;
    if ( funcRef )
        return false;
    if ( argTypes )
        return false;
    if ( commentText )
        return false;
    return true;
}

void FunctionCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(argTypes, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int AggrCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( t_aggregate )
        return t_aggregate;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int AggrCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool AggrCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( t_aggregate )
        return false;
    if ( funcRef )
        return false;
    if ( args )
        return false;
    if ( commentText )
        return false;
    return true;
}

void AggrCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(args, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int CommonCommentStmtAST::firstToken() const
{
    if ( t_comment )
        return t_comment;
    if ( t_on )
        return t_on;
    if ( obj && !obj->isNull() )
        return obj->firstToken();
    if ( commentText && !commentText->isNull() )
        return commentText->firstToken();
    return 0;
}

int CommonCommentStmtAST::lastToken() const
{
    if ( commentText && !commentText->isNull() )
        return commentText->lastToken();
    if ( obj && !obj->isNull() )
        return obj->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( t_comment )
        return t_comment + 1;
    return 1;
}

bool CommonCommentStmtAST::isNull() const
{
    if ( t_comment )
        return false;
    if ( t_on )
        return false;
    if ( obj )
        return false;
    if ( commentText )
        return false;
    return true;
}

void CommonCommentStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(obj, visitor);
        accept(commentText, visitor);
    }
    visitor->endVisit(this);
}

int SearchCommentObjectAST::firstToken() const
{
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_sp_what )
        return t_sp_what;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    return 0;
}

int SearchCommentObjectAST::lastToken() const
{
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_sp_what )
        return t_sp_what + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    return 1;
}

bool SearchCommentObjectAST::isNull() const
{
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_sp_what )
        return false;
    if ( ident )
        return false;
    return true;
}

void SearchCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int EvtTriggerCommentObjectAST::firstToken() const
{
    if ( t_event )
        return t_event;
    if ( t_trigger )
        return t_trigger;
    if ( triggerIdent && !triggerIdent->isNull() )
        return triggerIdent->firstToken();
    return 0;
}

int EvtTriggerCommentObjectAST::lastToken() const
{
    if ( triggerIdent && !triggerIdent->isNull() )
        return triggerIdent->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_event )
        return t_event + 1;
    return 1;
}

bool EvtTriggerCommentObjectAST::isNull() const
{
    if ( t_event )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerIdent )
        return false;
    return true;
}

void EvtTriggerCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerIdent, visitor);
    }
    visitor->endVisit(this);
}

int FdwCommentObjectAST::firstToken() const
{
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( fdwIdent && !fdwIdent->isNull() )
        return fdwIdent->firstToken();
    return 0;
}

int FdwCommentObjectAST::lastToken() const
{
    if ( fdwIdent && !fdwIdent->isNull() )
        return fdwIdent->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    return 1;
}

bool FdwCommentObjectAST::isNull() const
{
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( fdwIdent )
        return false;
    return true;
}

void FdwCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fdwIdent, visitor);
    }
    visitor->endVisit(this);
}

int RoleCommentObjectAST::firstToken() const
{
    if ( t_role )
        return t_role;
    if ( roleId && !roleId->isNull() )
        return roleId->firstToken();
    return 0;
}

int RoleCommentObjectAST::lastToken() const
{
    if ( roleId && !roleId->isNull() )
        return roleId->lastToken();
    if ( t_role )
        return t_role + 1;
    return 1;
}

bool RoleCommentObjectAST::isNull() const
{
    if ( t_role )
        return false;
    if ( roleId )
        return false;
    return true;
}

void RoleCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleId, visitor);
    }
    visitor->endVisit(this);
}

int TableCommentObjectAST::firstToken() const
{
    if ( t_foreign_or_materialized )
        return t_foreign_or_materialized;
    if ( t_table_or_view )
        return t_table_or_view;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    return 0;
}

int TableCommentObjectAST::lastToken() const
{
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_table_or_view )
        return t_table_or_view + 1;
    if ( t_foreign_or_materialized )
        return t_foreign_or_materialized + 1;
    return 1;
}

bool TableCommentObjectAST::isNull() const
{
    if ( t_foreign_or_materialized )
        return false;
    if ( t_table_or_view )
        return false;
    if ( ident )
        return false;
    return true;
}

void TableCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int SequenceCommentObjectAST::firstToken() const
{
    if ( t_sequence )
        return t_sequence;
    if ( sequenceIdent && !sequenceIdent->isNull() )
        return sequenceIdent->firstToken();
    return 0;
}

int SequenceCommentObjectAST::lastToken() const
{
    if ( sequenceIdent && !sequenceIdent->isNull() )
        return sequenceIdent->lastToken();
    if ( t_sequence )
        return t_sequence + 1;
    return 1;
}

bool SequenceCommentObjectAST::isNull() const
{
    if ( t_sequence )
        return false;
    if ( sequenceIdent )
        return false;
    return true;
}

void SequenceCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(sequenceIdent, visitor);
    }
    visitor->endVisit(this);
}

int IndexCommentObjectAST::firstToken() const
{
    if ( t_index )
        return t_index;
    if ( indexIdent && !indexIdent->isNull() )
        return indexIdent->firstToken();
    return 0;
}

int IndexCommentObjectAST::lastToken() const
{
    if ( indexIdent && !indexIdent->isNull() )
        return indexIdent->lastToken();
    if ( t_index )
        return t_index + 1;
    return 1;
}

bool IndexCommentObjectAST::isNull() const
{
    if ( t_index )
        return false;
    if ( indexIdent )
        return false;
    return true;
}

void IndexCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexIdent, visitor);
    }
    visitor->endVisit(this);
}

int SchemaCommentObjectAST::firstToken() const
{
    if ( t_schema )
        return t_schema;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    return 0;
}

int SchemaCommentObjectAST::lastToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    return 1;
}

bool SchemaCommentObjectAST::isNull() const
{
    if ( t_schema )
        return false;
    if ( schemaName )
        return false;
    return true;
}

void SchemaCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
    }
    visitor->endVisit(this);
}

int ColumnCommentObjectAST::firstToken() const
{
    if ( t_columnm )
        return t_columnm;
    if ( columnIdent && !columnIdent->isNull() )
        return columnIdent->firstToken();
    return 0;
}

int ColumnCommentObjectAST::lastToken() const
{
    if ( columnIdent && !columnIdent->isNull() )
        return columnIdent->lastToken();
    if ( t_columnm )
        return t_columnm + 1;
    return 1;
}

bool ColumnCommentObjectAST::isNull() const
{
    if ( t_columnm )
        return false;
    if ( columnIdent )
        return false;
    return true;
}

void ColumnCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnIdent, visitor);
    }
    visitor->endVisit(this);
}

int NamedCommentObjectAST::firstToken() const
{
    if ( t_sp_object )
        return t_sp_object;
    if ( objName && !objName->isNull() )
        return objName->firstToken();
    return 0;
}

int NamedCommentObjectAST::lastToken() const
{
    if ( objName && !objName->isNull() )
        return objName->lastToken();
    if ( t_sp_object )
        return t_sp_object + 1;
    return 1;
}

bool NamedCommentObjectAST::isNull() const
{
    if ( t_sp_object )
        return false;
    if ( objName )
        return false;
    return true;
}

void NamedCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(objName, visitor);
    }
    visitor->endVisit(this);
}

int AnyCommentObjectAST::firstToken() const
{
    if ( t_sp_object )
        return t_sp_object;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    return 0;
}

int AnyCommentObjectAST::lastToken() const
{
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_sp_object )
        return t_sp_object + 1;
    return 1;
}

bool AnyCommentObjectAST::isNull() const
{
    if ( t_sp_object )
        return false;
    if ( ident )
        return false;
    return true;
}

void AnyCommentObjectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int CommentTextClauseAST::firstToken() const
{
    if ( t_is )
        return t_is;
    if ( t_comment_text )
        return t_comment_text;
    return 0;
}

int CommentTextClauseAST::lastToken() const
{
    if ( t_comment_text )
        return t_comment_text + 1;
    if ( t_is )
        return t_is + 1;
    return 1;
}

bool CommentTextClauseAST::isNull() const
{
    if ( t_is )
        return false;
    if ( t_comment_text )
        return false;
    return true;
}

void CommentTextClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ClusterOnStmtAST::firstToken() const
{
    if ( t_cluster )
        return t_cluster;
    if ( t_verbose )
        return t_verbose;
    if ( indexName && !indexName->isNull() )
        return indexName->firstToken();
    if ( t_on )
        return t_on;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    return 0;
}

int ClusterOnStmtAST::lastToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( indexName && !indexName->isNull() )
        return indexName->lastToken();
    if ( t_verbose )
        return t_verbose + 1;
    if ( t_cluster )
        return t_cluster + 1;
    return 1;
}

bool ClusterOnStmtAST::isNull() const
{
    if ( t_cluster )
        return false;
    if ( t_verbose )
        return false;
    if ( indexName )
        return false;
    if ( t_on )
        return false;
    if ( tableIdent )
        return false;
    return true;
}

void ClusterOnStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexName, visitor);
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int ClusterUsingStmtAST::firstToken() const
{
    if ( t_cluster )
        return t_cluster;
    if ( t_verbose )
        return t_verbose;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( usingClause && !usingClause->isNull() )
        return usingClause->firstToken();
    return 0;
}

int ClusterUsingStmtAST::lastToken() const
{
    if ( usingClause && !usingClause->isNull() )
        return usingClause->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_verbose )
        return t_verbose + 1;
    if ( t_cluster )
        return t_cluster + 1;
    return 1;
}

bool ClusterUsingStmtAST::isNull() const
{
    if ( t_cluster )
        return false;
    if ( t_verbose )
        return false;
    if ( tableIdent )
        return false;
    if ( usingClause )
        return false;
    return true;
}

void ClusterUsingStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(usingClause, visitor);
    }
    visitor->endVisit(this);
}

int UsingIndexClauseAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( indexName && !indexName->isNull() )
        return indexName->firstToken();
    return 0;
}

int UsingIndexClauseAST::lastToken() const
{
    if ( indexName && !indexName->isNull() )
        return indexName->lastToken();
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool UsingIndexClauseAST::isNull() const
{
    if ( t_using )
        return false;
    if ( indexName )
        return false;
    return true;
}

void UsingIndexClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexName, visitor);
    }
    visitor->endVisit(this);
}

int CloseCursorStmtAST::firstToken() const
{
    if ( t_close )
        return t_close;
    if ( cursorName && !cursorName->isNull() )
        return cursorName->firstToken();
    return 0;
}

int CloseCursorStmtAST::lastToken() const
{
    if ( cursorName && !cursorName->isNull() )
        return cursorName->lastToken();
    if ( t_close )
        return t_close + 1;
    return 1;
}

bool CloseCursorStmtAST::isNull() const
{
    if ( t_close )
        return false;
    if ( cursorName )
        return false;
    return true;
}

void CloseCursorStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(cursorName, visitor);
    }
    visitor->endVisit(this);
}

int CloseAllStmtAST::firstToken() const
{
    if ( t_close )
        return t_close;
    if ( t_all )
        return t_all;
    return 0;
}

int CloseAllStmtAST::lastToken() const
{
    if ( t_all )
        return t_all + 1;
    if ( t_close )
        return t_close + 1;
    return 1;
}

bool CloseAllStmtAST::isNull() const
{
    if ( t_close )
        return false;
    if ( t_all )
        return false;
    return true;
}

void CloseAllStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CheckPointStmtAST::firstToken() const
{
    if ( t_checkpoint )
        return t_checkpoint;
    return 0;
}

int CheckPointStmtAST::lastToken() const
{
    if ( t_checkpoint )
        return t_checkpoint + 1;
    return 1;
}

bool CheckPointStmtAST::isNull() const
{
    if ( t_checkpoint )
        return false;
    return true;
}

void CheckPointStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AnalyzeStmtAST::firstToken() const
{
    if ( t_analize )
        return t_analize;
    if ( t_verbose )
        return t_verbose;
    if ( tabeIdent && !tabeIdent->isNull() )
        return tabeIdent->firstToken();
    if ( columns && !columns->isNull() )
        return columns->firstToken();
    return 0;
}

int AnalyzeStmtAST::lastToken() const
{
    if ( columns && !columns->isNull() )
        return columns->lastToken();
    if ( tabeIdent && !tabeIdent->isNull() )
        return tabeIdent->lastToken();
    if ( t_verbose )
        return t_verbose + 1;
    if ( t_analize )
        return t_analize + 1;
    return 1;
}

bool AnalyzeStmtAST::isNull() const
{
    if ( t_analize )
        return false;
    if ( t_verbose )
        return false;
    if ( tabeIdent )
        return false;
    if ( columns )
        return false;
    return true;
}

void AnalyzeStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tabeIdent, visitor);
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int AlterUserMappingStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_user )
        return t_user;
    if ( t_mapping )
        return t_mapping;
    if ( t_for )
        return t_for;
    if ( authIdent && !authIdent->isNull() )
        return authIdent->firstToken();
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( cmds && !cmds->isNull() )
        return cmds->firstToken();
    return 0;
}

int AlterUserMappingStmtAST::lastToken() const
{
    if ( cmds && !cmds->isNull() )
        return cmds->lastToken();
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( authIdent && !authIdent->isNull() )
        return authIdent->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_mapping )
        return t_mapping + 1;
    if ( t_user )
        return t_user + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterUserMappingStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_user )
        return false;
    if ( t_mapping )
        return false;
    if ( t_for )
        return false;
    if ( authIdent )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    if ( cmds )
        return false;
    return true;
}

void AlterUserMappingStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(authIdent, visitor);
        accept(serverName, visitor);
        accept(cmds, visitor);
    }
    visitor->endVisit(this);
}

int UserAuthIdentifierAST::firstToken() const
{
    if ( t_user_or_current_user )
        return t_user_or_current_user;
    return 0;
}

int UserAuthIdentifierAST::lastToken() const
{
    if ( t_user_or_current_user )
        return t_user_or_current_user + 1;
    return 1;
}

bool UserAuthIdentifierAST::isNull() const
{
    if ( t_user_or_current_user )
        return false;
    return true;
}

void UserAuthIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RoleAuthIdentifierAST::firstToken() const
{
    if ( roleId && !roleId->isNull() )
        return roleId->firstToken();
    return 0;
}

int RoleAuthIdentifierAST::lastToken() const
{
    if ( roleId && !roleId->isNull() )
        return roleId->lastToken();
    return 1;
}

bool RoleAuthIdentifierAST::isNull() const
{
    if ( roleId )
        return false;
    return true;
}

void RoleAuthIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleId, visitor);
    }
    visitor->endVisit(this);
}

int AlterTSConfigurationStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_configuration )
        return t_configuration;
    if ( confIdent && !confIdent->isNull() )
        return confIdent->firstToken();
    if ( cmd && !cmd->isNull() )
        return cmd->firstToken();
    return 0;
}

int AlterTSConfigurationStmtAST::lastToken() const
{
    if ( cmd && !cmd->isNull() )
        return cmd->lastToken();
    if ( confIdent && !confIdent->isNull() )
        return confIdent->lastToken();
    if ( t_configuration )
        return t_configuration + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTSConfigurationStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_configuration )
        return false;
    if ( confIdent )
        return false;
    if ( cmd )
        return false;
    return true;
}

void AlterTSConfigurationStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(confIdent, visitor);
        accept(cmd, visitor);
    }
    visitor->endVisit(this);
}

int AlterTSConfAddAlterCmdAST::firstToken() const
{
    if ( t_add_or_alter )
        return t_add_or_alter;
    if ( t_mapping )
        return t_mapping;
    if ( t_for )
        return t_for;
    if ( forNames )
        return forNames->firstToken();
    if ( t_replace )
        return t_replace;
    if ( replaceIdent && !replaceIdent->isNull() )
        return replaceIdent->firstToken();
    if ( t_with )
        return t_with;
    if ( withIdents )
        return withIdents->firstToken();
    return 0;
}

int AlterTSConfAddAlterCmdAST::lastToken() const
{
    if ( withIdents )
        return withIdents->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( replaceIdent && !replaceIdent->isNull() )
        return replaceIdent->lastToken();
    if ( t_replace )
        return t_replace + 1;
    if ( forNames )
        return forNames->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_mapping )
        return t_mapping + 1;
    if ( t_add_or_alter )
        return t_add_or_alter + 1;
    return 1;
}

bool AlterTSConfAddAlterCmdAST::isNull() const
{
    if ( t_add_or_alter )
        return false;
    if ( t_mapping )
        return false;
    if ( t_for )
        return false;
    if ( forNames )
        return false;
    if ( t_replace )
        return false;
    if ( replaceIdent )
        return false;
    if ( t_with )
        return false;
    if ( withIdents )
        return false;
    return true;
}

void AlterTSConfAddAlterCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(forNames, visitor);
        accept(replaceIdent, visitor);
        accept(withIdents, visitor);
    }
    visitor->endVisit(this);
}

int AlterTSConfDropCmdAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_mapping )
        return t_mapping;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( t_for )
        return t_for;
    if ( names )
        return names->firstToken();
    return 0;
}

int AlterTSConfDropCmdAST::lastToken() const
{
    if ( names )
        return names->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_mapping )
        return t_mapping + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool AlterTSConfDropCmdAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_mapping )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( t_for )
        return false;
    if ( names )
        return false;
    return true;
}

void AlterTSConfDropCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(names, visitor);
    }
    visitor->endVisit(this);
}

int AlterTSDictionaryStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_dictionary )
        return t_dictionary;
    if ( dictIdent && !dictIdent->isNull() )
        return dictIdent->firstToken();
    if ( definition && !definition->isNull() )
        return definition->firstToken();
    return 0;
}

int AlterTSDictionaryStmtAST::lastToken() const
{
    if ( definition && !definition->isNull() )
        return definition->lastToken();
    if ( dictIdent && !dictIdent->isNull() )
        return dictIdent->lastToken();
    if ( t_dictionary )
        return t_dictionary + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTSDictionaryStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_dictionary )
        return false;
    if ( dictIdent )
        return false;
    if ( definition )
        return false;
    return true;
}

void AlterTSDictionaryStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dictIdent, visitor);
        accept(definition, visitor);
    }
    visitor->endVisit(this);
}

int AlterCompositeTypeStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( commands )
        return commands->firstToken();
    return 0;
}

int AlterCompositeTypeStmtAST::lastToken() const
{
    if ( commands )
        return commands->lastToken();
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterCompositeTypeStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( commands )
        return false;
    return true;
}

void AlterCompositeTypeStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(commands, visitor);
    }
    visitor->endVisit(this);
}

int AlterTypeAddCommandAST::firstToken() const
{
    if ( t_add )
        return t_add;
    if ( t_atribute )
        return t_atribute;
    if ( elem && !elem->isNull() )
        return elem->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int AlterTypeAddCommandAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( elem && !elem->isNull() )
        return elem->lastToken();
    if ( t_atribute )
        return t_atribute + 1;
    if ( t_add )
        return t_add + 1;
    return 1;
}

bool AlterTypeAddCommandAST::isNull() const
{
    if ( t_add )
        return false;
    if ( t_atribute )
        return false;
    if ( elem )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void AlterTypeAddCommandAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(elem, visitor);
    }
    visitor->endVisit(this);
}

int AlterTypeDropCommandAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_attribute )
        return t_attribute;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( attrName && !attrName->isNull() )
        return attrName->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int AlterTypeDropCommandAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( attrName && !attrName->isNull() )
        return attrName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_attribute )
        return t_attribute + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool AlterTypeDropCommandAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_attribute )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( attrName )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void AlterTypeDropCommandAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(attrName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTypeAlterCommandAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_attribute )
        return t_attribute;
    if ( attrName && !attrName->isNull() )
        return attrName->firstToken();
    if ( setData && !setData->isNull() )
        return setData->firstToken();
    if ( t_type )
        return t_type;
    if ( newType && !newType->isNull() )
        return newType->firstToken();
    if ( collate && !collate->isNull() )
        return collate->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int AlterTypeAlterCommandAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( collate && !collate->isNull() )
        return collate->lastToken();
    if ( newType && !newType->isNull() )
        return newType->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( setData && !setData->isNull() )
        return setData->lastToken();
    if ( attrName && !attrName->isNull() )
        return attrName->lastToken();
    if ( t_attribute )
        return t_attribute + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTypeAlterCommandAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_attribute )
        return false;
    if ( attrName )
        return false;
    if ( setData )
        return false;
    if ( t_type )
        return false;
    if ( newType )
        return false;
    if ( collate )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void AlterTypeAlterCommandAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(attrName, visitor);
        accept(setData, visitor);
        accept(newType, visitor);
        accept(collate, visitor);
    }
    visitor->endVisit(this);
}

int AlterSeqStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_sequence )
        return t_sequence;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( seqIdent && !seqIdent->isNull() )
        return seqIdent->firstToken();
    if ( options )
        return options->firstToken();
    return 0;
}

int AlterSeqStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( seqIdent && !seqIdent->isNull() )
        return seqIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_sequence )
        return t_sequence + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterSeqStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_sequence )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( seqIdent )
        return false;
    if ( options )
        return false;
    return true;
}

void AlterSeqStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(seqIdent, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int CreateSeqStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->firstToken();
    if ( t_sequence )
        return t_sequence;
    if ( seqIdent && !seqIdent->isNull() )
        return seqIdent->firstToken();
    if ( options )
        return options->firstToken();
    return 0;
}

int CreateSeqStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( seqIdent && !seqIdent->isNull() )
        return seqIdent->lastToken();
    if ( t_sequence )
        return t_sequence + 1;
    if ( tempClause && !tempClause->isNull() )
        return tempClause->lastToken();
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateSeqStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( tempClause )
        return false;
    if ( t_sequence )
        return false;
    if ( seqIdent )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateSeqStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tempClause, visitor);
        accept(seqIdent, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int SimpleSequenceOptionAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    if ( t_sp3 )
        return t_sp3;
    return 0;
}

int SimpleSequenceOptionAST::lastToken() const
{
    if ( t_sp3 )
        return t_sp3 + 1;
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool SimpleSequenceOptionAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    if ( t_sp3 )
        return false;
    return true;
}

void SimpleSequenceOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int OwnedSequenceOptionAST::firstToken() const
{
    if ( t_owned )
        return t_owned;
    if ( t_by )
        return t_by;
    if ( ownerRole && !ownerRole->isNull() )
        return ownerRole->firstToken();
    return 0;
}

int OwnedSequenceOptionAST::lastToken() const
{
    if ( ownerRole && !ownerRole->isNull() )
        return ownerRole->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_owned )
        return t_owned + 1;
    return 1;
}

bool OwnedSequenceOptionAST::isNull() const
{
    if ( t_owned )
        return false;
    if ( t_by )
        return false;
    if ( ownerRole )
        return false;
    return true;
}

void OwnedSequenceOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ownerRole, visitor);
    }
    visitor->endVisit(this);
}

int TempClauseAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    return 0;
}

int TempClauseAST::lastToken() const
{
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool TempClauseAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    return true;
}

void TempClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterEvtTriggerOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_event )
        return t_event;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterEvtTriggerOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_event )
        return t_event + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterEvtTriggerOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_event )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterEvtTriggerOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterServerOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterServerOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterServerOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterServerOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(serverName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterFdwOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterFdwOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterFdwOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( wrapperName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterFdwOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(wrapperName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterSearchOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_dict_or_conf )
        return t_dict_or_conf;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterSearchOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_dict_or_conf )
        return t_dict_or_conf + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterSearchOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_dict_or_conf )
        return false;
    if ( ident )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterSearchOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterTablespaceOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterTablespaceOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTablespaceOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterTablespaceOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterTypeOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterTypeOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTypeOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterTypeOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterSchemaOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_schema )
        return t_schema;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterSchemaOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterSchemaOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_schema )
        return false;
    if ( schemaName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterSchemaOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterOpUsingOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( t_class_or_family )
        return t_class_or_family;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterOpUsingOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_class_or_family )
        return t_class_or_family + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterOpUsingOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( t_class_or_family )
        return false;
    if ( ident )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterOpUsingOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterOpOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterOpOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterOpOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( operand )
        return false;
    if ( argTypes )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterOpOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(argTypes, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterLargeObjOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_large )
        return t_large;
    if ( t_object )
        return t_object;
    if ( t_value )
        return t_value;
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterLargeObjOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( t_value )
        return t_value + 1;
    if ( t_object )
        return t_object + 1;
    if ( t_large )
        return t_large + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterLargeObjOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_large )
        return false;
    if ( t_object )
        return false;
    if ( t_value )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterLargeObjOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterLangOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( langName && !langName->isNull() )
        return langName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterLangOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( langName && !langName->isNull() )
        return langName->lastToken();
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterLangOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( langName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterLangOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(langName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterFunctionOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_function )
        return t_function;
    if ( funcWithArgTypes && !funcWithArgTypes->isNull() )
        return funcWithArgTypes->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterFunctionOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( funcWithArgTypes && !funcWithArgTypes->isNull() )
        return funcWithArgTypes->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterFunctionOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_function )
        return false;
    if ( funcWithArgTypes )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterFunctionOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcWithArgTypes, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterDbOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_database )
        return t_database;
    if ( dbName && !dbName->isNull() )
        return dbName->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterDbOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( dbName && !dbName->isNull() )
        return dbName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDbOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_database )
        return false;
    if ( dbName )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterDbOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dbName, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterCollationOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_collation )
        return t_collation;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterCollationOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_collation )
        return t_collation + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterCollationOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_collation )
        return false;
    if ( ident )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterCollationOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterConvOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_conversion )
        return t_conversion;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterConvOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_conversion )
        return t_conversion + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterConvOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_conversion )
        return false;
    if ( ident )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterConvOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterDomainOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterDomainOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( ident )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterDomainOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterAggrOwnerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_aggregate )
        return t_aggregate;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->firstToken();
    return 0;
}

int AlterAggrOwnerStmtAST::lastToken() const
{
    if ( ownerToClause && !ownerToClause->isNull() )
        return ownerToClause->lastToken();
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterAggrOwnerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_aggregate )
        return false;
    if ( funcRef )
        return false;
    if ( args )
        return false;
    if ( ownerToClause )
        return false;
    return true;
}

void AlterAggrOwnerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(args, visitor);
        accept(ownerToClause, visitor);
    }
    visitor->endVisit(this);
}

int OwnerToClauseAST::firstToken() const
{
    if ( t_owner )
        return t_owner;
    if ( t_to )
        return t_to;
    if ( roleId && !roleId->isNull() )
        return roleId->firstToken();
    return 0;
}

int OwnerToClauseAST::lastToken() const
{
    if ( roleId && !roleId->isNull() )
        return roleId->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_owner )
        return t_owner + 1;
    return 1;
}

bool OwnerToClauseAST::isNull() const
{
    if ( t_owner )
        return false;
    if ( t_to )
        return false;
    if ( roleId )
        return false;
    return true;
}

void OwnerToClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleId, visitor);
    }
    visitor->endVisit(this);
}

int AlterTypeSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterTypeSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTypeSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterTypeSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterViewSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterViewSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( viewIdent && !viewIdent->isNull() )
        return viewIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterViewSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( viewIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterViewSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterSeqSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_sequence )
        return t_sequence;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( sequenceIdent && !sequenceIdent->isNull() )
        return sequenceIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterSeqSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( sequenceIdent && !sequenceIdent->isNull() )
        return sequenceIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_sequence )
        return t_sequence + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterSeqSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_sequence )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( sequenceIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterSeqSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(sequenceIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterSearchSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_sp_what )
        return t_sp_what;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterSearchSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_sp_what )
        return t_sp_what + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterSearchSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_sp_what )
        return false;
    if ( ident )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterSearchSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( rel && !rel->isNull() )
        return rel->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterTableSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( rel && !rel->isNull() )
        return rel->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( rel )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterTableSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rel, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterOpUsingSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( t_class_orFamily )
        return t_class_orFamily;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterOpUsingSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_class_orFamily )
        return t_class_orFamily + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterOpUsingSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( t_class_orFamily )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterOpUsingSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterOpSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_operator )
        return t_operator;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterOpSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_operator )
        return t_operator + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterOpSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_operator )
        return false;
    if ( operand )
        return false;
    if ( argTypes )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterOpSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(argTypes, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterFunctionSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_function )
        return t_function;
    if ( functionWithArgTypes && !functionWithArgTypes->isNull() )
        return functionWithArgTypes->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterFunctionSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( functionWithArgTypes && !functionWithArgTypes->isNull() )
        return functionWithArgTypes->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterFunctionSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_function )
        return false;
    if ( functionWithArgTypes )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterFunctionSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionWithArgTypes, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterCollationSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_collation )
        return t_collation;
    if ( collationIdent && !collationIdent->isNull() )
        return collationIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterCollationSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( collationIdent && !collationIdent->isNull() )
        return collationIdent->lastToken();
    if ( t_collation )
        return t_collation + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterCollationSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_collation )
        return false;
    if ( collationIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterCollationSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collationIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterConvSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_convertion )
        return t_convertion;
    if ( convertionIdent && !convertionIdent->isNull() )
        return convertionIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterConvSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( convertionIdent && !convertionIdent->isNull() )
        return convertionIdent->lastToken();
    if ( t_convertion )
        return t_convertion + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterConvSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_convertion )
        return false;
    if ( convertionIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterConvSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(convertionIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterDomainSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterDomainSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterDomainSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterExtSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_extension )
        return t_extension;
    if ( extensionIdent && !extensionIdent->isNull() )
        return extensionIdent->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterExtSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( extensionIdent && !extensionIdent->isNull() )
        return extensionIdent->lastToken();
    if ( t_extension )
        return t_extension + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterExtSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_extension )
        return false;
    if ( extensionIdent )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterExtSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(extensionIdent, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterAggrFuncSchemaStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_aggregate )
        return t_aggregate;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->firstToken();
    return 0;
}

int AlterAggrFuncSchemaStmtAST::lastToken() const
{
    if ( setSchemaClause && !setSchemaClause->isNull() )
        return setSchemaClause->lastToken();
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterAggrFuncSchemaStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_aggregate )
        return false;
    if ( funcRef )
        return false;
    if ( args )
        return false;
    if ( setSchemaClause )
        return false;
    return true;
}

void AlterAggrFuncSchemaStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(args, visitor);
        accept(setSchemaClause, visitor);
    }
    visitor->endVisit(this);
}

int SetSchemaClauseAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_schema )
        return t_schema;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    return 0;
}

int SetSchemaClauseAST::lastToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool SetSchemaClauseAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_schema )
        return false;
    if ( schemaName )
        return false;
    return true;
}

void SetSchemaClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
    }
    visitor->endVisit(this);
}

int AlterFunctionStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_function )
        return t_function;
    if ( functionWithArgTypes && !functionWithArgTypes->isNull() )
        return functionWithArgTypes->firstToken();
    if ( alterOptions )
        return alterOptions->firstToken();
    if ( t_restrict )
        return t_restrict;
    return 0;
}

int AlterFunctionStmtAST::lastToken() const
{
    if ( t_restrict )
        return t_restrict + 1;
    if ( alterOptions )
        return alterOptions->lastToken();
    if ( functionWithArgTypes && !functionWithArgTypes->isNull() )
        return functionWithArgTypes->lastToken();
    if ( t_function )
        return t_function + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterFunctionStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_function )
        return false;
    if ( functionWithArgTypes )
        return false;
    if ( alterOptions )
        return false;
    if ( t_restrict )
        return false;
    return true;
}

void AlterFunctionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionWithArgTypes, visitor);
        accept(alterOptions, visitor);
    }
    visitor->endVisit(this);
}

int CommonAlterFunctionOptAST::firstToken() const
{
    if ( t_sp1 )
        return t_sp1;
    if ( t_sp2 )
        return t_sp2;
    if ( t_sp3 )
        return t_sp3;
    if ( t_sp4 )
        return t_sp4;
    return 0;
}

int CommonAlterFunctionOptAST::lastToken() const
{
    if ( t_sp4 )
        return t_sp4 + 1;
    if ( t_sp3 )
        return t_sp3 + 1;
    if ( t_sp2 )
        return t_sp2 + 1;
    if ( t_sp1 )
        return t_sp1 + 1;
    return 1;
}

bool CommonAlterFunctionOptAST::isNull() const
{
    if ( t_sp1 )
        return false;
    if ( t_sp2 )
        return false;
    if ( t_sp3 )
        return false;
    if ( t_sp4 )
        return false;
    return true;
}

void CommonAlterFunctionOptAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RestAlterFunctionOptAST::firstToken() const
{
    if ( rest && !rest->isNull() )
        return rest->firstToken();
    return 0;
}

int RestAlterFunctionOptAST::lastToken() const
{
    if ( rest && !rest->isNull() )
        return rest->lastToken();
    return 1;
}

bool RestAlterFunctionOptAST::isNull() const
{
    if ( rest )
        return false;
    return true;
}

void RestAlterFunctionOptAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rest, visitor);
    }
    visitor->endVisit(this);
}

int AlterForeignServerStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    if ( version && !version->isNull() )
        return version->firstToken();
    if ( genericCms && !genericCms->isNull() )
        return genericCms->firstToken();
    return 0;
}

int AlterForeignServerStmtAST::lastToken() const
{
    if ( genericCms && !genericCms->isNull() )
        return genericCms->lastToken();
    if ( version && !version->isNull() )
        return version->lastToken();
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterForeignServerStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    if ( version )
        return false;
    if ( genericCms )
        return false;
    return true;
}

void AlterForeignServerStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(serverName, visitor);
        accept(version, visitor);
        accept(genericCms, visitor);
    }
    visitor->endVisit(this);
}

int ForeignServerVersionAST::firstToken() const
{
    if ( t_version )
        return t_version;
    if ( t_value )
        return t_value;
    return 0;
}

int ForeignServerVersionAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_version )
        return t_version + 1;
    return 1;
}

bool ForeignServerVersionAST::isNull() const
{
    if ( t_version )
        return false;
    if ( t_value )
        return false;
    return true;
}

void ForeignServerVersionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterFdwStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->firstToken();
    if ( fdwOptions )
        return fdwOptions->firstToken();
    if ( genericCms && !genericCms->isNull() )
        return genericCms->firstToken();
    return 0;
}

int AlterFdwStmtAST::lastToken() const
{
    if ( genericCms && !genericCms->isNull() )
        return genericCms->lastToken();
    if ( fdwOptions )
        return fdwOptions->lastToken();
    if ( wrapperName && !wrapperName->isNull() )
        return wrapperName->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterFdwStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( wrapperName )
        return false;
    if ( fdwOptions )
        return false;
    if ( genericCms )
        return false;
    return true;
}

void AlterFdwStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(wrapperName, visitor);
        accept(fdwOptions, visitor);
        accept(genericCms, visitor);
    }
    visitor->endVisit(this);
}

int SetFdwOptionAST::firstToken() const
{
    if ( t_handler_or_validator )
        return t_handler_or_validator;
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->firstToken();
    return 0;
}

int SetFdwOptionAST::lastToken() const
{
    if ( handlerIdent && !handlerIdent->isNull() )
        return handlerIdent->lastToken();
    if ( t_handler_or_validator )
        return t_handler_or_validator + 1;
    return 1;
}

bool SetFdwOptionAST::isNull() const
{
    if ( t_handler_or_validator )
        return false;
    if ( handlerIdent )
        return false;
    return true;
}

void SetFdwOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(handlerIdent, visitor);
    }
    visitor->endVisit(this);
}

int ResetFdwOptionAST::firstToken() const
{
    if ( t_no )
        return t_no;
    if ( t_handler_or_validator )
        return t_handler_or_validator;
    return 0;
}

int ResetFdwOptionAST::lastToken() const
{
    if ( t_handler_or_validator )
        return t_handler_or_validator + 1;
    if ( t_no )
        return t_no + 1;
    return 1;
}

bool ResetFdwOptionAST::isNull() const
{
    if ( t_no )
        return false;
    if ( t_handler_or_validator )
        return false;
    return true;
}

void ResetFdwOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int HandlerIdentifierAST::firstToken() const
{
    if ( firstName && !firstName->isNull() )
        return firstName->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int HandlerIdentifierAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( firstName && !firstName->isNull() )
        return firstName->lastToken();
    return 1;
}

bool HandlerIdentifierAST::isNull() const
{
    if ( firstName )
        return false;
    if ( attrs )
        return false;
    return true;
}

void HandlerIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(firstName, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int AlterExtensionCtxStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_extension )
        return t_extension;
    if ( extName && !extName->isNull() )
        return extName->firstToken();
    if ( t_add_or_drop )
        return t_add_or_drop;
    if ( cmd && !cmd->isNull() )
        return cmd->firstToken();
    return 0;
}

int AlterExtensionCtxStmtAST::lastToken() const
{
    if ( cmd && !cmd->isNull() )
        return cmd->lastToken();
    if ( t_add_or_drop )
        return t_add_or_drop + 1;
    if ( extName && !extName->isNull() )
        return extName->lastToken();
    if ( t_extension )
        return t_extension + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterExtensionCtxStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_extension )
        return false;
    if ( extName )
        return false;
    if ( t_add_or_drop )
        return false;
    if ( cmd )
        return false;
    return true;
}

void AlterExtensionCtxStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(extName, visitor);
        accept(cmd, visitor);
    }
    visitor->endVisit(this);
}

int OpExtensionCtxCmdAST::firstToken() const
{
    if ( t_operator )
        return t_operator;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( opArgTypes && !opArgTypes->isNull() )
        return opArgTypes->firstToken();
    return 0;
}

int OpExtensionCtxCmdAST::lastToken() const
{
    if ( opArgTypes && !opArgTypes->isNull() )
        return opArgTypes->lastToken();
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_operator )
        return t_operator + 1;
    return 1;
}

bool OpExtensionCtxCmdAST::isNull() const
{
    if ( t_operator )
        return false;
    if ( operand )
        return false;
    if ( opArgTypes )
        return false;
    return true;
}

void OpExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(operand, visitor);
        accept(opArgTypes, visitor);
    }
    visitor->endVisit(this);
}

int OperArgTypesAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( type1 && !type1->isNull() )
        return type1->firstToken();
    if ( t_comma )
        return t_comma;
    if ( type2 && !type2->isNull() )
        return type2->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int OperArgTypesAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( type2 && !type2->isNull() )
        return type2->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( type1 && !type1->isNull() )
        return type1->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool OperArgTypesAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( type1 )
        return false;
    if ( t_comma )
        return false;
    if ( type2 )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void OperArgTypesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(type1, visitor);
        accept(type2, visitor);
    }
    visitor->endVisit(this);
}

int LangExtensionCtxCmdAST::firstToken() const
{
    if ( t_procedural )
        return t_procedural;
    if ( t_language )
        return t_language;
    if ( langName && !langName->isNull() )
        return langName->firstToken();
    return 0;
}

int LangExtensionCtxCmdAST::lastToken() const
{
    if ( langName && !langName->isNull() )
        return langName->lastToken();
    if ( t_language )
        return t_language + 1;
    if ( t_procedural )
        return t_procedural + 1;
    return 1;
}

bool LangExtensionCtxCmdAST::isNull() const
{
    if ( t_procedural )
        return false;
    if ( t_language )
        return false;
    if ( langName )
        return false;
    return true;
}

void LangExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(langName, visitor);
    }
    visitor->endVisit(this);
}

int FunctionExtensionCtxCmdAST::firstToken() const
{
    if ( t_function )
        return t_function;
    if ( funcWithArgTypes && !funcWithArgTypes->isNull() )
        return funcWithArgTypes->firstToken();
    return 0;
}

int FunctionExtensionCtxCmdAST::lastToken() const
{
    if ( funcWithArgTypes && !funcWithArgTypes->isNull() )
        return funcWithArgTypes->lastToken();
    if ( t_function )
        return t_function + 1;
    return 1;
}

bool FunctionExtensionCtxCmdAST::isNull() const
{
    if ( t_function )
        return false;
    if ( funcWithArgTypes )
        return false;
    return true;
}

void FunctionExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcWithArgTypes, visitor);
    }
    visitor->endVisit(this);
}

int FunctionWithArgTypesAST::firstToken() const
{
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( argTypes && !argTypes->isNull() )
        return argTypes->firstToken();
    return 0;
}

int FunctionWithArgTypesAST::lastToken() const
{
    if ( argTypes && !argTypes->isNull() )
        return argTypes->lastToken();
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    return 1;
}

bool FunctionWithArgTypesAST::isNull() const
{
    if ( functionRef )
        return false;
    if ( argTypes )
        return false;
    return true;
}

void FunctionWithArgTypesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionRef, visitor);
        accept(argTypes, visitor);
    }
    visitor->endVisit(this);
}

int FunctionArgTypesAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int FunctionArgTypesAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool FunctionArgTypesAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void FunctionArgTypesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int FunctionArgAST::firstToken() const
{
    if ( t_pre_class )
        return t_pre_class;
    if ( paramName && !paramName->isNull() )
        return paramName->firstToken();
    if ( t_ps_class )
        return t_ps_class;
    if ( funcType && !funcType->isNull() )
        return funcType->firstToken();
    return 0;
}

int FunctionArgAST::lastToken() const
{
    if ( funcType && !funcType->isNull() )
        return funcType->lastToken();
    if ( t_ps_class )
        return t_ps_class + 1;
    if ( paramName && !paramName->isNull() )
        return paramName->lastToken();
    if ( t_pre_class )
        return t_pre_class + 1;
    return 1;
}

bool FunctionArgAST::isNull() const
{
    if ( t_pre_class )
        return false;
    if ( paramName )
        return false;
    if ( t_ps_class )
        return false;
    if ( funcType )
        return false;
    return true;
}

void FunctionArgAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(paramName, visitor);
        accept(funcType, visitor);
    }
    visitor->endVisit(this);
}

int SimpleExtensionCtxCmdAST::firstToken() const
{
    if ( t_sp_what )
        return t_sp_what;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    return 0;
}

int SimpleExtensionCtxCmdAST::lastToken() const
{
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_sp_what )
        return t_sp_what + 1;
    return 1;
}

bool SimpleExtensionCtxCmdAST::isNull() const
{
    if ( t_sp_what )
        return false;
    if ( ident )
        return false;
    return true;
}

void SimpleExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int CastExtensionCtxCmdAST::firstToken() const
{
    if ( t_cast )
        return t_cast;
    if ( t_lparen )
        return t_lparen;
    if ( fromType && !fromType->isNull() )
        return fromType->firstToken();
    if ( t_as )
        return t_as;
    if ( toType && !toType->isNull() )
        return toType->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CastExtensionCtxCmdAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( toType && !toType->isNull() )
        return toType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( fromType && !fromType->isNull() )
        return fromType->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_cast )
        return t_cast + 1;
    return 1;
}

bool CastExtensionCtxCmdAST::isNull() const
{
    if ( t_cast )
        return false;
    if ( t_lparen )
        return false;
    if ( fromType )
        return false;
    if ( t_as )
        return false;
    if ( toType )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CastExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fromType, visitor);
        accept(toType, visitor);
    }
    visitor->endVisit(this);
}

int AggrExtensionCtxCmdAST::firstToken() const
{
    if ( t_aggregate )
        return t_aggregate;
    if ( functionRef && !functionRef->isNull() )
        return functionRef->firstToken();
    if ( args && !args->isNull() )
        return args->firstToken();
    return 0;
}

int AggrExtensionCtxCmdAST::lastToken() const
{
    if ( args && !args->isNull() )
        return args->lastToken();
    if ( functionRef && !functionRef->isNull() )
        return functionRef->lastToken();
    if ( t_aggregate )
        return t_aggregate + 1;
    return 1;
}

bool AggrExtensionCtxCmdAST::isNull() const
{
    if ( t_aggregate )
        return false;
    if ( functionRef )
        return false;
    if ( args )
        return false;
    return true;
}

void AggrExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(functionRef, visitor);
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int ListAggrArgumentsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( types )
        return types->firstToken();
    if ( t_rpren )
        return t_rpren;
    return 0;
}

int ListAggrArgumentsAST::lastToken() const
{
    if ( t_rpren )
        return t_rpren + 1;
    if ( types )
        return types->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool ListAggrArgumentsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( types )
        return false;
    if ( t_rpren )
        return false;
    return true;
}

void ListAggrArgumentsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(types, visitor);
    }
    visitor->endVisit(this);
}

int StarAggrArgumentsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( t_star )
        return t_star;
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int StarAggrArgumentsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_star )
        return t_star + 1;
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool StarAggrArgumentsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( t_star )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void StarAggrArgumentsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int OpUsingExtensionCtxCmdAST::firstToken() const
{
    if ( t_operator )
        return t_operator;
    if ( t_class_or_family )
        return t_class_or_family;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->firstToken();
    if ( t_using )
        return t_using;
    if ( t_acc_method )
        return t_acc_method;
    return 0;
}

int OpUsingExtensionCtxCmdAST::lastToken() const
{
    if ( t_acc_method )
        return t_acc_method + 1;
    if ( t_using )
        return t_using + 1;
    if ( opIdent && !opIdent->isNull() )
        return opIdent->lastToken();
    if ( t_class_or_family )
        return t_class_or_family + 1;
    if ( t_operator )
        return t_operator + 1;
    return 1;
}

bool OpUsingExtensionCtxCmdAST::isNull() const
{
    if ( t_operator )
        return false;
    if ( t_class_or_family )
        return false;
    if ( opIdent )
        return false;
    if ( t_using )
        return false;
    if ( t_acc_method )
        return false;
    return true;
}

void OpUsingExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(opIdent, visitor);
    }
    visitor->endVisit(this);
}

int SchemaExtensionCtxCmdAST::firstToken() const
{
    if ( t_schema )
        return t_schema;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    return 0;
}

int SchemaExtensionCtxCmdAST::lastToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    if ( t_schema )
        return t_schema + 1;
    return 1;
}

bool SchemaExtensionCtxCmdAST::isNull() const
{
    if ( t_schema )
        return false;
    if ( schemaName )
        return false;
    return true;
}

void SchemaExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
    }
    visitor->endVisit(this);
}

int TrgEventExtensionCtxCmdAST::firstToken() const
{
    if ( t_event )
        return t_event;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    return 0;
}

int TrgEventExtensionCtxCmdAST::lastToken() const
{
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_event )
        return t_event + 1;
    return 1;
}

bool TrgEventExtensionCtxCmdAST::isNull() const
{
    if ( t_event )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    return true;
}

void TrgEventExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
    }
    visitor->endVisit(this);
}

int TxtSearchExtensionCtxCmdAST::firstToken() const
{
    if ( t_text )
        return t_text;
    if ( t_search )
        return t_search;
    if ( t_sp_what )
        return t_sp_what;
    if ( spIdent && !spIdent->isNull() )
        return spIdent->firstToken();
    return 0;
}

int TxtSearchExtensionCtxCmdAST::lastToken() const
{
    if ( spIdent && !spIdent->isNull() )
        return spIdent->lastToken();
    if ( t_sp_what )
        return t_sp_what + 1;
    if ( t_search )
        return t_search + 1;
    if ( t_text )
        return t_text + 1;
    return 1;
}

bool TxtSearchExtensionCtxCmdAST::isNull() const
{
    if ( t_text )
        return false;
    if ( t_search )
        return false;
    if ( t_sp_what )
        return false;
    if ( spIdent )
        return false;
    return true;
}

void TxtSearchExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(spIdent, visitor);
    }
    visitor->endVisit(this);
}

int SequenceExtensionCtxCmdAST::firstToken() const
{
    if ( t_sequence )
        return t_sequence;
    if ( seqName && !seqName->isNull() )
        return seqName->firstToken();
    return 0;
}

int SequenceExtensionCtxCmdAST::lastToken() const
{
    if ( seqName && !seqName->isNull() )
        return seqName->lastToken();
    if ( t_sequence )
        return t_sequence + 1;
    return 1;
}

bool SequenceExtensionCtxCmdAST::isNull() const
{
    if ( t_sequence )
        return false;
    if ( seqName )
        return false;
    return true;
}

void SequenceExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(seqName, visitor);
    }
    visitor->endVisit(this);
}

int TableExtensionCtxCmdAST::firstToken() const
{
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( tableName && !tableName->isNull() )
        return tableName->firstToken();
    return 0;
}

int TableExtensionCtxCmdAST::lastToken() const
{
    if ( tableName && !tableName->isNull() )
        return tableName->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    return 1;
}

bool TableExtensionCtxCmdAST::isNull() const
{
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( tableName )
        return false;
    return true;
}

void TableExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableName, visitor);
    }
    visitor->endVisit(this);
}

int ViewExtensionCtxCmdAST::firstToken() const
{
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( viewName && !viewName->isNull() )
        return viewName->firstToken();
    return 0;
}

int ViewExtensionCtxCmdAST::lastToken() const
{
    if ( viewName && !viewName->isNull() )
        return viewName->lastToken();
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    return 1;
}

bool ViewExtensionCtxCmdAST::isNull() const
{
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( viewName )
        return false;
    return true;
}

void ViewExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(viewName, visitor);
    }
    visitor->endVisit(this);
}

int FdwExtensionCtxCmdAST::firstToken() const
{
    if ( t_foreign )
        return t_foreign;
    if ( t_data )
        return t_data;
    if ( t_wrapper )
        return t_wrapper;
    if ( fdwName && !fdwName->isNull() )
        return fdwName->firstToken();
    return 0;
}

int FdwExtensionCtxCmdAST::lastToken() const
{
    if ( fdwName && !fdwName->isNull() )
        return fdwName->lastToken();
    if ( t_wrapper )
        return t_wrapper + 1;
    if ( t_data )
        return t_data + 1;
    if ( t_foreign )
        return t_foreign + 1;
    return 1;
}

bool FdwExtensionCtxCmdAST::isNull() const
{
    if ( t_foreign )
        return false;
    if ( t_data )
        return false;
    if ( t_wrapper )
        return false;
    if ( fdwName )
        return false;
    return true;
}

void FdwExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fdwName, visitor);
    }
    visitor->endVisit(this);
}

int TypeExtensionCtxCmdAST::firstToken() const
{
    if ( t_type )
        return t_type;
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->firstToken();
    return 0;
}

int TypeExtensionCtxCmdAST::lastToken() const
{
    if ( typeIdent && !typeIdent->isNull() )
        return typeIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    return 1;
}

bool TypeExtensionCtxCmdAST::isNull() const
{
    if ( t_type )
        return false;
    if ( typeIdent )
        return false;
    return true;
}

void TypeExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeIdent, visitor);
    }
    visitor->endVisit(this);
}

int ServerExtensionCtxCmdAST::firstToken() const
{
    if ( t_server )
        return t_server;
    if ( serverName && !serverName->isNull() )
        return serverName->firstToken();
    return 0;
}

int ServerExtensionCtxCmdAST::lastToken() const
{
    if ( serverName && !serverName->isNull() )
        return serverName->lastToken();
    if ( t_server )
        return t_server + 1;
    return 1;
}

bool ServerExtensionCtxCmdAST::isNull() const
{
    if ( t_server )
        return false;
    if ( serverName )
        return false;
    return true;
}

void ServerExtensionCtxCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(serverName, visitor);
    }
    visitor->endVisit(this);
}

int AlterExtensionStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_extension )
        return t_extension;
    if ( extName && !extName->isNull() )
        return extName->firstToken();
    if ( t_update )
        return t_update;
    if ( updates )
        return updates->firstToken();
    return 0;
}

int AlterExtensionStmtAST::lastToken() const
{
    if ( updates )
        return updates->lastToken();
    if ( t_update )
        return t_update + 1;
    if ( extName && !extName->isNull() )
        return extName->lastToken();
    if ( t_extension )
        return t_extension + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterExtensionStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_extension )
        return false;
    if ( extName )
        return false;
    if ( t_update )
        return false;
    if ( updates )
        return false;
    return true;
}

void AlterExtensionStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(extName, visitor);
        accept(updates, visitor);
    }
    visitor->endVisit(this);
}

int UpdateToExtensionCmdAST::firstToken() const
{
    if ( t_to )
        return t_to;
    if ( t_value )
        return t_value;
    return 0;
}

int UpdateToExtensionCmdAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_to )
        return t_to + 1;
    return 1;
}

bool UpdateToExtensionCmdAST::isNull() const
{
    if ( t_to )
        return false;
    if ( t_value )
        return false;
    return true;
}

void UpdateToExtensionCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterEnumStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_type )
        return t_type;
    if ( enumIdent && !enumIdent->isNull() )
        return enumIdent->firstToken();
    if ( t_add )
        return t_add;
    if ( t_value )
        return t_value;
    if ( ifNotExists && !ifNotExists->isNull() )
        return ifNotExists->firstToken();
    if ( t_add_value )
        return t_add_value;
    if ( t_before_or_after )
        return t_before_or_after;
    if ( t_place_value )
        return t_place_value;
    return 0;
}

int AlterEnumStmtAST::lastToken() const
{
    if ( t_place_value )
        return t_place_value + 1;
    if ( t_before_or_after )
        return t_before_or_after + 1;
    if ( t_add_value )
        return t_add_value + 1;
    if ( ifNotExists && !ifNotExists->isNull() )
        return ifNotExists->lastToken();
    if ( t_value )
        return t_value + 1;
    if ( t_add )
        return t_add + 1;
    if ( enumIdent && !enumIdent->isNull() )
        return enumIdent->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterEnumStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_type )
        return false;
    if ( enumIdent )
        return false;
    if ( t_add )
        return false;
    if ( t_value )
        return false;
    if ( ifNotExists )
        return false;
    if ( t_add_value )
        return false;
    if ( t_before_or_after )
        return false;
    if ( t_place_value )
        return false;
    return true;
}

void AlterEnumStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(enumIdent, visitor);
        accept(ifNotExists, visitor);
    }
    visitor->endVisit(this);
}

int IfNotExistsAST::firstToken() const
{
    if ( t_if )
        return t_if;
    if ( t_not )
        return t_not;
    if ( t_exists )
        return t_exists;
    return 0;
}

int IfNotExistsAST::lastToken() const
{
    if ( t_exists )
        return t_exists + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_if )
        return t_if + 1;
    return 1;
}

bool IfNotExistsAST::isNull() const
{
    if ( t_if )
        return false;
    if ( t_not )
        return false;
    if ( t_exists )
        return false;
    return true;
}

void IfNotExistsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterDomainAltDefStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( alterCmd && !alterCmd->isNull() )
        return alterCmd->firstToken();
    return 0;
}

int AlterDomainAltDefStmtAST::lastToken() const
{
    if ( alterCmd && !alterCmd->isNull() )
        return alterCmd->lastToken();
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainAltDefStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( alterCmd )
        return false;
    return true;
}

void AlterDomainAltDefStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
        accept(alterCmd, visitor);
    }
    visitor->endVisit(this);
}

int AlterDomainAddConstrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( t_add )
        return t_add;
    if ( constraint && !constraint->isNull() )
        return constraint->firstToken();
    return 0;
}

int AlterDomainAddConstrStmtAST::lastToken() const
{
    if ( constraint && !constraint->isNull() )
        return constraint->lastToken();
    if ( t_add )
        return t_add + 1;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainAddConstrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( t_add )
        return false;
    if ( constraint )
        return false;
    return true;
}

void AlterDomainAddConstrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
        accept(constraint, visitor);
    }
    visitor->endVisit(this);
}

int AlterDomainDropConstrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( t_drop )
        return t_drop;
    if ( t_constraint )
        return t_constraint;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int AlterDomainDropConstrStmtAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_drop )
        return t_drop + 1;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainDropConstrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( t_drop )
        return false;
    if ( t_constraint )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( constraintName )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void AlterDomainDropConstrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
        accept(constraintName, visitor);
    }
    visitor->endVisit(this);
}

int AlterDomainValConstrStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( t_validate )
        return t_validate;
    if ( t_constraint )
        return t_constraint;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    return 0;
}

int AlterDomainValConstrStmtAST::lastToken() const
{
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_validate )
        return t_validate + 1;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainValConstrStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( t_validate )
        return false;
    if ( t_constraint )
        return false;
    if ( constraintName )
        return false;
    return true;
}

void AlterDomainValConstrStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
        accept(constraintName, visitor);
    }
    visitor->endVisit(this);
}

int AlterDomainChgNotNullStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( t_drop_or_set )
        return t_drop_or_set;
    if ( t_not )
        return t_not;
    if ( t_null )
        return t_null;
    return 0;
}

int AlterDomainChgNotNullStmtAST::lastToken() const
{
    if ( t_null )
        return t_null + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_drop_or_set )
        return t_drop_or_set + 1;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDomainChgNotNullStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( t_drop_or_set )
        return false;
    if ( t_not )
        return false;
    if ( t_null )
        return false;
    return true;
}

void AlterDomainChgNotNullStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
    }
    visitor->endVisit(this);
}

int CreateDomainStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_domain )
        return t_domain;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->firstToken();
    if ( t_as )
        return t_as;
    if ( domainType && !domainType->isNull() )
        return domainType->firstToken();
    if ( constraints )
        return constraints->firstToken();
    return 0;
}

int CreateDomainStmtAST::lastToken() const
{
    if ( constraints )
        return constraints->lastToken();
    if ( domainType && !domainType->isNull() )
        return domainType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( domainIdent && !domainIdent->isNull() )
        return domainIdent->lastToken();
    if ( t_domain )
        return t_domain + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateDomainStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_domain )
        return false;
    if ( domainIdent )
        return false;
    if ( t_as )
        return false;
    if ( domainType )
        return false;
    if ( constraints )
        return false;
    return true;
}

void CreateDomainStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(domainIdent, visitor);
        accept(domainType, visitor);
        accept(constraints, visitor);
    }
    visitor->endVisit(this);
}

int AlterDefaultPrivilegesStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_default )
        return t_default;
    if ( t_privileges )
        return t_privileges;
    if ( options )
        return options->firstToken();
    if ( action && !action->isNull() )
        return action->firstToken();
    return 0;
}

int AlterDefaultPrivilegesStmtAST::lastToken() const
{
    if ( action && !action->isNull() )
        return action->lastToken();
    if ( options )
        return options->lastToken();
    if ( t_privileges )
        return t_privileges + 1;
    if ( t_default )
        return t_default + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDefaultPrivilegesStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_default )
        return false;
    if ( t_privileges )
        return false;
    if ( options )
        return false;
    if ( action )
        return false;
    return true;
}

void AlterDefaultPrivilegesStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(options, visitor);
        accept(action, visitor);
    }
    visitor->endVisit(this);
}

int InSchemaDefACLOptionAST::firstToken() const
{
    if ( t_in )
        return t_in;
    if ( t_schema )
        return t_schema;
    if ( schemas )
        return schemas->firstToken();
    return 0;
}

int InSchemaDefACLOptionAST::lastToken() const
{
    if ( schemas )
        return schemas->lastToken();
    if ( t_schema )
        return t_schema + 1;
    if ( t_in )
        return t_in + 1;
    return 1;
}

bool InSchemaDefACLOptionAST::isNull() const
{
    if ( t_in )
        return false;
    if ( t_schema )
        return false;
    if ( schemas )
        return false;
    return true;
}

void InSchemaDefACLOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemas, visitor);
    }
    visitor->endVisit(this);
}

int ForRoleUserDefACLOptionAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_role_or_user )
        return t_role_or_user;
    if ( roles )
        return roles->firstToken();
    return 0;
}

int ForRoleUserDefACLOptionAST::lastToken() const
{
    if ( roles )
        return roles->lastToken();
    if ( t_role_or_user )
        return t_role_or_user + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool ForRoleUserDefACLOptionAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_role_or_user )
        return false;
    if ( roles )
        return false;
    return true;
}

void ForRoleUserDefACLOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roles, visitor);
    }
    visitor->endVisit(this);
}

int RevokeDefACLActionAST::firstToken() const
{
    if ( t_revoke )
        return t_revoke;
    if ( t_grant )
        return t_grant;
    if ( t_option )
        return t_option;
    if ( t_for )
        return t_for;
    if ( privs && !privs->isNull() )
        return privs->firstToken();
    if ( t_on )
        return t_on;
    if ( t_target )
        return t_target;
    if ( t_from )
        return t_from;
    if ( grantees )
        return grantees->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int RevokeDefACLActionAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( grantees )
        return grantees->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( t_target )
        return t_target + 1;
    if ( t_on )
        return t_on + 1;
    if ( privs && !privs->isNull() )
        return privs->lastToken();
    if ( t_for )
        return t_for + 1;
    if ( t_option )
        return t_option + 1;
    if ( t_grant )
        return t_grant + 1;
    if ( t_revoke )
        return t_revoke + 1;
    return 1;
}

bool RevokeDefACLActionAST::isNull() const
{
    if ( t_revoke )
        return false;
    if ( t_grant )
        return false;
    if ( t_option )
        return false;
    if ( t_for )
        return false;
    if ( privs )
        return false;
    if ( t_on )
        return false;
    if ( t_target )
        return false;
    if ( t_from )
        return false;
    if ( grantees )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void RevokeDefACLActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
        accept(grantees, visitor);
    }
    visitor->endVisit(this);
}

int GrantDefACLActionAST::firstToken() const
{
    if ( t_grant )
        return t_grant;
    if ( privs && !privs->isNull() )
        return privs->firstToken();
    if ( t_on )
        return t_on;
    if ( t_target )
        return t_target;
    if ( t_to )
        return t_to;
    if ( grantees )
        return grantees->firstToken();
    if ( wgo && !wgo->isNull() )
        return wgo->firstToken();
    return 0;
}

int GrantDefACLActionAST::lastToken() const
{
    if ( wgo && !wgo->isNull() )
        return wgo->lastToken();
    if ( grantees )
        return grantees->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_target )
        return t_target + 1;
    if ( t_on )
        return t_on + 1;
    if ( privs && !privs->isNull() )
        return privs->lastToken();
    if ( t_grant )
        return t_grant + 1;
    return 1;
}

bool GrantDefACLActionAST::isNull() const
{
    if ( t_grant )
        return false;
    if ( privs )
        return false;
    if ( t_on )
        return false;
    if ( t_target )
        return false;
    if ( t_to )
        return false;
    if ( grantees )
        return false;
    if ( wgo )
        return false;
    return true;
}

void GrantDefACLActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
        accept(grantees, visitor);
        accept(wgo, visitor);
    }
    visitor->endVisit(this);
}

int AllPrivilegesAST::firstToken() const
{
    if ( t_all )
        return t_all;
    if ( t_privileges )
        return t_privileges;
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int AllPrivilegesAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_privileges )
        return t_privileges + 1;
    if ( t_all )
        return t_all + 1;
    return 1;
}

bool AllPrivilegesAST::isNull() const
{
    if ( t_all )
        return false;
    if ( t_privileges )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void AllPrivilegesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int PrivilegesAsListAST::firstToken() const
{
    if ( privs )
        return privs->firstToken();
    return 0;
}

int PrivilegesAsListAST::lastToken() const
{
    if ( privs )
        return privs->lastToken();
    return 1;
}

bool PrivilegesAsListAST::isNull() const
{
    if ( privs )
        return false;
    return true;
}

void PrivilegesAsListAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(privs, visitor);
    }
    visitor->endVisit(this);
}

int CommonPrivilegeAST::firstToken() const
{
    if ( t_priv_entity )
        return t_priv_entity;
    if ( refColumns && !refColumns->isNull() )
        return refColumns->firstToken();
    return 0;
}

int CommonPrivilegeAST::lastToken() const
{
    if ( refColumns && !refColumns->isNull() )
        return refColumns->lastToken();
    if ( t_priv_entity )
        return t_priv_entity + 1;
    return 1;
}

bool CommonPrivilegeAST::isNull() const
{
    if ( t_priv_entity )
        return false;
    if ( refColumns )
        return false;
    return true;
}

void CommonPrivilegeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(refColumns, visitor);
    }
    visitor->endVisit(this);
}

int GranteeAST::firstToken() const
{
    if ( t_group )
        return t_group;
    if ( role && !role->isNull() )
        return role->firstToken();
    return 0;
}

int GranteeAST::lastToken() const
{
    if ( role && !role->isNull() )
        return role->lastToken();
    if ( t_group )
        return t_group + 1;
    return 1;
}

bool GranteeAST::isNull() const
{
    if ( t_group )
        return false;
    if ( role )
        return false;
    return true;
}

void GranteeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(role, visitor);
    }
    visitor->endVisit(this);
}

int WithGrantOptionAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( t_grant )
        return t_grant;
    if ( t_option )
        return t_option;
    return 0;
}

int WithGrantOptionAST::lastToken() const
{
    if ( t_option )
        return t_option + 1;
    if ( t_grant )
        return t_grant + 1;
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithGrantOptionAST::isNull() const
{
    if ( t_with )
        return false;
    if ( t_grant )
        return false;
    if ( t_option )
        return false;
    return true;
}

void WithGrantOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterDatabaseSetStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_database )
        return t_database;
    if ( databaseName && !databaseName->isNull() )
        return databaseName->firstToken();
    if ( setResetClause && !setResetClause->isNull() )
        return setResetClause->firstToken();
    return 0;
}

int AlterDatabaseSetStmtAST::lastToken() const
{
    if ( setResetClause && !setResetClause->isNull() )
        return setResetClause->lastToken();
    if ( databaseName && !databaseName->isNull() )
        return databaseName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDatabaseSetStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_database )
        return false;
    if ( databaseName )
        return false;
    if ( setResetClause )
        return false;
    return true;
}

void AlterDatabaseSetStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(databaseName, visitor);
        accept(setResetClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterDatabaseChgOptStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_database )
        return t_database;
    if ( databaseName && !databaseName->isNull() )
        return databaseName->firstToken();
    if ( t_with )
        return t_with;
    if ( options )
        return options->firstToken();
    return 0;
}

int AlterDatabaseChgOptStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( databaseName && !databaseName->isNull() )
        return databaseName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDatabaseChgOptStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_database )
        return false;
    if ( databaseName )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void AlterDatabaseChgOptStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(databaseName, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int AlterDbConnLimitOptionAST::firstToken() const
{
    if ( t_connection )
        return t_connection;
    if ( t_limit )
        return t_limit;
    if ( t_eqaul )
        return t_eqaul;
    if ( t_value )
        return t_value;
    return 0;
}

int AlterDbConnLimitOptionAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_eqaul )
        return t_eqaul + 1;
    if ( t_limit )
        return t_limit + 1;
    if ( t_connection )
        return t_connection + 1;
    return 1;
}

bool AlterDbConnLimitOptionAST::isNull() const
{
    if ( t_connection )
        return false;
    if ( t_limit )
        return false;
    if ( t_eqaul )
        return false;
    if ( t_value )
        return false;
    return true;
}

void AlterDbConnLimitOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterDatabaseSetTblspStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_database )
        return t_database;
    if ( databaseName && !databaseName->isNull() )
        return databaseName->firstToken();
    if ( t_set )
        return t_set;
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    return 0;
}

int AlterDatabaseSetTblspStmtAST::lastToken() const
{
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_set )
        return t_set + 1;
    if ( databaseName && !databaseName->isNull() )
        return databaseName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterDatabaseSetTblspStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_database )
        return false;
    if ( databaseName )
        return false;
    if ( t_set )
        return false;
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    return true;
}

void AlterDatabaseSetTblspStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(databaseName, visitor);
        accept(tablespaceName, visitor);
    }
    visitor->endVisit(this);
}

int CreateEventTrigStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_event )
        return t_event;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( t_on )
        return t_on;
    if ( label && !label->isNull() )
        return label->firstToken();
    if ( t_when )
        return t_when;
    if ( whens )
        return whens->firstToken();
    if ( t_execute )
        return t_execute;
    if ( t_procedure )
        return t_procedure;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CreateEventTrigStmtAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    if ( t_procedure )
        return t_procedure + 1;
    if ( t_execute )
        return t_execute + 1;
    if ( whens )
        return whens->lastToken();
    if ( t_when )
        return t_when + 1;
    if ( label && !label->isNull() )
        return label->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_event )
        return t_event + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateEventTrigStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_event )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( t_on )
        return false;
    if ( label )
        return false;
    if ( t_when )
        return false;
    if ( whens )
        return false;
    if ( t_execute )
        return false;
    if ( t_procedure )
        return false;
    if ( funcRef )
        return false;
    if ( t_lparen )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CreateEventTrigStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(label, visitor);
        accept(whens, visitor);
        accept(funcRef, visitor);
    }
    visitor->endVisit(this);
}

int EventTriggerWhenAST::firstToken() const
{
    if ( colName && !colName->isNull() )
        return colName->firstToken();
    if ( t_in )
        return t_in;
    if ( t_lparen )
        return t_lparen;
    if ( values )
        return values->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int EventTriggerWhenAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( values )
        return values->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_in )
        return t_in + 1;
    if ( colName && !colName->isNull() )
        return colName->lastToken();
    return 1;
}

bool EventTriggerWhenAST::isNull() const
{
    if ( colName )
        return false;
    if ( t_in )
        return false;
    if ( t_lparen )
        return false;
    if ( values )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void EventTriggerWhenAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(colName, visitor);
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int EventTriggerValueAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

int EventTriggerValueAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool EventTriggerValueAST::isNull() const
{
    if ( t_value )
        return false;
    return true;
}

void EventTriggerValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterEventTrigStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_event )
        return t_event;
    if ( t_trigger )
        return t_trigger;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    if ( eventCmd && !eventCmd->isNull() )
        return eventCmd->firstToken();
    return 0;
}

int AlterEventTrigStmtAST::lastToken() const
{
    if ( eventCmd && !eventCmd->isNull() )
        return eventCmd->lastToken();
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_event )
        return t_event + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterEventTrigStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_event )
        return false;
    if ( t_trigger )
        return false;
    if ( triggerName )
        return false;
    if ( eventCmd )
        return false;
    return true;
}

void AlterEventTrigStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(triggerName, visitor);
        accept(eventCmd, visitor);
    }
    visitor->endVisit(this);
}

int EventTriggerCmdAST::firstToken() const
{
    if ( t_enable_or_disable )
        return t_enable_or_disable;
    if ( t_always_or_replica )
        return t_always_or_replica;
    return 0;
}

int EventTriggerCmdAST::lastToken() const
{
    if ( t_always_or_replica )
        return t_always_or_replica + 1;
    if ( t_enable_or_disable )
        return t_enable_or_disable + 1;
    return 1;
}

bool EventTriggerCmdAST::isNull() const
{
    if ( t_enable_or_disable )
        return false;
    if ( t_always_or_replica )
        return false;
    return true;
}

void EventTriggerCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CreateRoleStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_role )
        return t_role;
    if ( roleIdent && !roleIdent->isNull() )
        return roleIdent->firstToken();
    if ( t_with )
        return t_with;
    if ( options )
        return options->firstToken();
    return 0;
}

int CreateRoleStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( roleIdent && !roleIdent->isNull() )
        return roleIdent->lastToken();
    if ( t_role )
        return t_role + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateRoleStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_role )
        return false;
    if ( roleIdent )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleIdent, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int AlterRoleStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_role )
        return t_role;
    if ( roleIdent && !roleIdent->isNull() )
        return roleIdent->firstToken();
    if ( t_with )
        return t_with;
    if ( roleOptions )
        return roleOptions->firstToken();
    return 0;
}

int AlterRoleStmtAST::lastToken() const
{
    if ( roleOptions )
        return roleOptions->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( roleIdent && !roleIdent->isNull() )
        return roleIdent->lastToken();
    if ( t_role )
        return t_role + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterRoleStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_role )
        return false;
    if ( roleIdent )
        return false;
    if ( t_with )
        return false;
    if ( roleOptions )
        return false;
    return true;
}

void AlterRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleIdent, visitor);
        accept(roleOptions, visitor);
    }
    visitor->endVisit(this);
}

int OptRoleWithRoleListAST::firstToken() const
{
    if ( t_spec1 )
        return t_spec1;
    if ( t_spec2 )
        return t_spec2;
    if ( roles )
        return roles->firstToken();
    return 0;
}

int OptRoleWithRoleListAST::lastToken() const
{
    if ( roles )
        return roles->lastToken();
    if ( t_spec2 )
        return t_spec2 + 1;
    if ( t_spec1 )
        return t_spec1 + 1;
    return 1;
}

bool OptRoleWithRoleListAST::isNull() const
{
    if ( t_spec1 )
        return false;
    if ( t_spec2 )
        return false;
    if ( roles )
        return false;
    return true;
}

void OptRoleWithRoleListAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roles, visitor);
    }
    visitor->endVisit(this);
}

int OptRoleStaticAST::firstToken() const
{
    if ( t_spec1 )
        return t_spec1;
    if ( t_spec2 )
        return t_spec2;
    if ( t_spec3 )
        return t_spec3;
    return 0;
}

int OptRoleStaticAST::lastToken() const
{
    if ( t_spec3 )
        return t_spec3 + 1;
    if ( t_spec2 )
        return t_spec2 + 1;
    if ( t_spec1 )
        return t_spec1 + 1;
    return 1;
}

bool OptRoleStaticAST::isNull() const
{
    if ( t_spec1 )
        return false;
    if ( t_spec2 )
        return false;
    if ( t_spec3 )
        return false;
    return true;
}

void OptRoleStaticAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RoleIdAST::firstToken() const
{
    if ( roleName && !roleName->isNull() )
        return roleName->firstToken();
    return 0;
}

int RoleIdAST::lastToken() const
{
    if ( roleName && !roleName->isNull() )
        return roleName->lastToken();
    return 1;
}

bool RoleIdAST::isNull() const
{
    if ( roleName )
        return false;
    return true;
}

void RoleIdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleName, visitor);
    }
    visitor->endVisit(this);
}

int DatabaseNameAST::firstToken() const
{
    if ( databaseName && !databaseName->isNull() )
        return databaseName->firstToken();
    return 0;
}

int DatabaseNameAST::lastToken() const
{
    if ( databaseName && !databaseName->isNull() )
        return databaseName->lastToken();
    return 1;
}

bool DatabaseNameAST::isNull() const
{
    if ( databaseName )
        return false;
    return true;
}

void DatabaseNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(databaseName, visitor);
    }
    visitor->endVisit(this);
}

int InDatabaseClauseAST::firstToken() const
{
    if ( t_in )
        return t_in;
    if ( t_database )
        return t_database;
    if ( databaseName && !databaseName->isNull() )
        return databaseName->firstToken();
    return 0;
}

int InDatabaseClauseAST::lastToken() const
{
    if ( databaseName && !databaseName->isNull() )
        return databaseName->lastToken();
    if ( t_database )
        return t_database + 1;
    if ( t_in )
        return t_in + 1;
    return 1;
}

bool InDatabaseClauseAST::isNull() const
{
    if ( t_in )
        return false;
    if ( t_database )
        return false;
    if ( databaseName )
        return false;
    return true;
}

void InDatabaseClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(databaseName, visitor);
    }
    visitor->endVisit(this);
}

int AlterAnyRoleSetStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_role )
        return t_role;
    if ( roleIdent && !roleIdent->isNull() )
        return roleIdent->firstToken();
    if ( inDbClause && !inDbClause->isNull() )
        return inDbClause->firstToken();
    if ( setClause && !setClause->isNull() )
        return setClause->firstToken();
    return 0;
}

int AlterAnyRoleSetStmtAST::lastToken() const
{
    if ( setClause && !setClause->isNull() )
        return setClause->lastToken();
    if ( inDbClause && !inDbClause->isNull() )
        return inDbClause->lastToken();
    if ( roleIdent && !roleIdent->isNull() )
        return roleIdent->lastToken();
    if ( t_role )
        return t_role + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterAnyRoleSetStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_role )
        return false;
    if ( roleIdent )
        return false;
    if ( inDbClause )
        return false;
    if ( setClause )
        return false;
    return true;
}

void AlterAnyRoleSetStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleIdent, visitor);
        accept(inDbClause, visitor);
        accept(setClause, visitor);
    }
    visitor->endVisit(this);
}

int AlterAllRolesSetStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_role )
        return t_role;
    if ( t_all )
        return t_all;
    if ( inDbClause && !inDbClause->isNull() )
        return inDbClause->firstToken();
    if ( setClause && !setClause->isNull() )
        return setClause->firstToken();
    return 0;
}

int AlterAllRolesSetStmtAST::lastToken() const
{
    if ( setClause && !setClause->isNull() )
        return setClause->lastToken();
    if ( inDbClause && !inDbClause->isNull() )
        return inDbClause->lastToken();
    if ( t_all )
        return t_all + 1;
    if ( t_role )
        return t_role + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterAllRolesSetStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_role )
        return false;
    if ( t_all )
        return false;
    if ( inDbClause )
        return false;
    if ( setClause )
        return false;
    return true;
}

void AlterAllRolesSetStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(inDbClause, visitor);
        accept(setClause, visitor);
    }
    visitor->endVisit(this);
}

int SetResetClauseAsSetAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( rest && !rest->isNull() )
        return rest->firstToken();
    return 0;
}

int SetResetClauseAsSetAST::lastToken() const
{
    if ( rest && !rest->isNull() )
        return rest->lastToken();
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool SetResetClauseAsSetAST::isNull() const
{
    if ( t_set )
        return false;
    if ( rest )
        return false;
    return true;
}

void SetResetClauseAsSetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rest, visitor);
    }
    visitor->endVisit(this);
}

int SetResetClauseAsVarResetAST::firstToken() const
{
    if ( varReset && !varReset->isNull() )
        return varReset->firstToken();
    return 0;
}

int SetResetClauseAsVarResetAST::lastToken() const
{
    if ( varReset && !varReset->isNull() )
        return varReset->lastToken();
    return 1;
}

bool SetResetClauseAsVarResetAST::isNull() const
{
    if ( varReset )
        return false;
    return true;
}

void SetResetClauseAsVarResetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(varReset, visitor);
    }
    visitor->endVisit(this);
}

int SetRestTransactionAST::firstToken() const
{
    if ( t_session )
        return t_session;
    if ( t_characterictics )
        return t_characterictics;
    if ( t_as )
        return t_as;
    if ( t_transaction )
        return t_transaction;
    if ( modes )
        return modes->firstToken();
    return 0;
}

int SetRestTransactionAST::lastToken() const
{
    if ( modes )
        return modes->lastToken();
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_as )
        return t_as + 1;
    if ( t_characterictics )
        return t_characterictics + 1;
    if ( t_session )
        return t_session + 1;
    return 1;
}

bool SetRestTransactionAST::isNull() const
{
    if ( t_session )
        return false;
    if ( t_characterictics )
        return false;
    if ( t_as )
        return false;
    if ( t_transaction )
        return false;
    if ( modes )
        return false;
    return true;
}

void SetRestTransactionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(modes, visitor);
    }
    visitor->endVisit(this);
}

int SetRestVarNameToAST::firstToken() const
{
    if ( dotted_name )
        return dotted_name->firstToken();
    if ( t_to_or_equal_or_from )
        return t_to_or_equal_or_from;
    if ( t_default_or_equal )
        return t_default_or_equal;
    return 0;
}

int SetRestVarNameToAST::lastToken() const
{
    if ( t_default_or_equal )
        return t_default_or_equal + 1;
    if ( t_to_or_equal_or_from )
        return t_to_or_equal_or_from + 1;
    if ( dotted_name )
        return dotted_name->lastToken();
    return 1;
}

bool SetRestVarNameToAST::isNull() const
{
    if ( dotted_name )
        return false;
    if ( t_to_or_equal_or_from )
        return false;
    if ( t_default_or_equal )
        return false;
    return true;
}

void SetRestVarNameToAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dotted_name, visitor);
    }
    visitor->endVisit(this);
}

int SetRestVarNameToValuesAST::firstToken() const
{
    if ( dotted_name )
        return dotted_name->firstToken();
    if ( t_to_or_equal_or_from )
        return t_to_or_equal_or_from;
    if ( values )
        return values->firstToken();
    return 0;
}

int SetRestVarNameToValuesAST::lastToken() const
{
    if ( values )
        return values->lastToken();
    if ( t_to_or_equal_or_from )
        return t_to_or_equal_or_from + 1;
    if ( dotted_name )
        return dotted_name->lastToken();
    return 1;
}

bool SetRestVarNameToValuesAST::isNull() const
{
    if ( dotted_name )
        return false;
    if ( t_to_or_equal_or_from )
        return false;
    if ( values )
        return false;
    return true;
}

void SetRestVarNameToValuesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dotted_name, visitor);
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int SetRestZoneValueAST::firstToken() const
{
    if ( t_time )
        return t_time;
    if ( t_zone )
        return t_zone;
    if ( zoneValue && !zoneValue->isNull() )
        return zoneValue->firstToken();
    return 0;
}

int SetRestZoneValueAST::lastToken() const
{
    if ( zoneValue && !zoneValue->isNull() )
        return zoneValue->lastToken();
    if ( t_zone )
        return t_zone + 1;
    if ( t_time )
        return t_time + 1;
    return 1;
}

bool SetRestZoneValueAST::isNull() const
{
    if ( t_time )
        return false;
    if ( t_zone )
        return false;
    if ( zoneValue )
        return false;
    return true;
}

void SetRestZoneValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(zoneValue, visitor);
    }
    visitor->endVisit(this);
}

int SetRestCatalogAST::firstToken() const
{
    if ( t_catalog )
        return t_catalog;
    if ( t_catalog_name )
        return t_catalog_name;
    return 0;
}

int SetRestCatalogAST::lastToken() const
{
    if ( t_catalog_name )
        return t_catalog_name + 1;
    if ( t_catalog )
        return t_catalog + 1;
    return 1;
}

bool SetRestCatalogAST::isNull() const
{
    if ( t_catalog )
        return false;
    if ( t_catalog_name )
        return false;
    return true;
}

void SetRestCatalogAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestSchemaAST::firstToken() const
{
    if ( t_schema )
        return t_schema;
    if ( t_schema_name )
        return t_schema_name;
    return 0;
}

int SetRestSchemaAST::lastToken() const
{
    if ( t_schema_name )
        return t_schema_name + 1;
    if ( t_schema )
        return t_schema + 1;
    return 1;
}

bool SetRestSchemaAST::isNull() const
{
    if ( t_schema )
        return false;
    if ( t_schema_name )
        return false;
    return true;
}

void SetRestSchemaAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestEncodingAST::firstToken() const
{
    if ( t_names )
        return t_names;
    if ( t_encoding )
        return t_encoding;
    return 0;
}

int SetRestEncodingAST::lastToken() const
{
    if ( t_encoding )
        return t_encoding + 1;
    if ( t_names )
        return t_names + 1;
    return 1;
}

bool SetRestEncodingAST::isNull() const
{
    if ( t_names )
        return false;
    if ( t_encoding )
        return false;
    return true;
}

void SetRestEncodingAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestRoleAST::firstToken() const
{
    if ( t_role )
        return t_role;
    if ( t_role_value )
        return t_role_value;
    return 0;
}

int SetRestRoleAST::lastToken() const
{
    if ( t_role_value )
        return t_role_value + 1;
    if ( t_role )
        return t_role + 1;
    return 1;
}

bool SetRestRoleAST::isNull() const
{
    if ( t_role )
        return false;
    if ( t_role_value )
        return false;
    return true;
}

void SetRestRoleAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestSessionAuthAST::firstToken() const
{
    if ( t_session )
        return t_session;
    if ( t_authorixation )
        return t_authorixation;
    if ( t_authValue )
        return t_authValue;
    return 0;
}

int SetRestSessionAuthAST::lastToken() const
{
    if ( t_authValue )
        return t_authValue + 1;
    if ( t_authorixation )
        return t_authorixation + 1;
    if ( t_session )
        return t_session + 1;
    return 1;
}

bool SetRestSessionAuthAST::isNull() const
{
    if ( t_session )
        return false;
    if ( t_authorixation )
        return false;
    if ( t_authValue )
        return false;
    return true;
}

void SetRestSessionAuthAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestXmlOptionAST::firstToken() const
{
    if ( t_xml )
        return t_xml;
    if ( t_option )
        return t_option;
    if ( t_value )
        return t_value;
    return 0;
}

int SetRestXmlOptionAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_option )
        return t_option + 1;
    if ( t_xml )
        return t_xml + 1;
    return 1;
}

bool SetRestXmlOptionAST::isNull() const
{
    if ( t_xml )
        return false;
    if ( t_option )
        return false;
    if ( t_value )
        return false;
    return true;
}

void SetRestXmlOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestTransactioSnapshotAST::firstToken() const
{
    if ( t_transaction )
        return t_transaction;
    if ( t_snapshot )
        return t_snapshot;
    if ( t_value )
        return t_value;
    return 0;
}

int SetRestTransactioSnapshotAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_snapshot )
        return t_snapshot + 1;
    if ( t_transaction )
        return t_transaction + 1;
    return 1;
}

bool SetRestTransactioSnapshotAST::isNull() const
{
    if ( t_transaction )
        return false;
    if ( t_snapshot )
        return false;
    if ( t_value )
        return false;
    return true;
}

void SetRestTransactioSnapshotAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int VarShowStmtAST::firstToken() const
{
    if ( t_show )
        return t_show;
    if ( varName )
        return varName->firstToken();
    return 0;
}

int VarShowStmtAST::lastToken() const
{
    if ( varName )
        return varName->lastToken();
    if ( t_show )
        return t_show + 1;
    return 1;
}

bool VarShowStmtAST::isNull() const
{
    if ( t_show )
        return false;
    if ( varName )
        return false;
    return true;
}

void VarShowStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(varName, visitor);
    }
    visitor->endVisit(this);
}

int TimeZoneShowStmtAST::firstToken() const
{
    if ( t_show )
        return t_show;
    if ( t_time )
        return t_time;
    if ( t_zone )
        return t_zone;
    return 0;
}

int TimeZoneShowStmtAST::lastToken() const
{
    if ( t_zone )
        return t_zone + 1;
    if ( t_time )
        return t_time + 1;
    if ( t_show )
        return t_show + 1;
    return 1;
}

bool TimeZoneShowStmtAST::isNull() const
{
    if ( t_show )
        return false;
    if ( t_time )
        return false;
    if ( t_zone )
        return false;
    return true;
}

void TimeZoneShowStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IsoLevelShowStmtAST::firstToken() const
{
    if ( t_show )
        return t_show;
    if ( t_transaction )
        return t_transaction;
    if ( t_isolation )
        return t_isolation;
    if ( t_level )
        return t_level;
    return 0;
}

int IsoLevelShowStmtAST::lastToken() const
{
    if ( t_level )
        return t_level + 1;
    if ( t_isolation )
        return t_isolation + 1;
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_show )
        return t_show + 1;
    return 1;
}

bool IsoLevelShowStmtAST::isNull() const
{
    if ( t_show )
        return false;
    if ( t_transaction )
        return false;
    if ( t_isolation )
        return false;
    if ( t_level )
        return false;
    return true;
}

void IsoLevelShowStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SessionAuthShowStmtAST::firstToken() const
{
    if ( t_show )
        return t_show;
    if ( t_session )
        return t_session;
    if ( t_authorization )
        return t_authorization;
    return 0;
}

int SessionAuthShowStmtAST::lastToken() const
{
    if ( t_authorization )
        return t_authorization + 1;
    if ( t_session )
        return t_session + 1;
    if ( t_show )
        return t_show + 1;
    return 1;
}

bool SessionAuthShowStmtAST::isNull() const
{
    if ( t_show )
        return false;
    if ( t_session )
        return false;
    if ( t_authorization )
        return false;
    return true;
}

void SessionAuthShowStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AllShowStmtAST::firstToken() const
{
    if ( t_show )
        return t_show;
    if ( t_all )
        return t_all;
    return 0;
}

int AllShowStmtAST::lastToken() const
{
    if ( t_all )
        return t_all + 1;
    if ( t_show )
        return t_show + 1;
    return 1;
}

bool AllShowStmtAST::isNull() const
{
    if ( t_show )
        return false;
    if ( t_all )
        return false;
    return true;
}

void AllShowStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int VariableSetStmtAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_local_or_session )
        return t_local_or_session;
    if ( sets && !sets->isNull() )
        return sets->firstToken();
    return 0;
}

int VariableSetStmtAST::lastToken() const
{
    if ( sets && !sets->isNull() )
        return sets->lastToken();
    if ( t_local_or_session )
        return t_local_or_session + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool VariableSetStmtAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_local_or_session )
        return false;
    if ( sets )
        return false;
    return true;
}

void VariableSetStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(sets, visitor);
    }
    visitor->endVisit(this);
}

int SetRestResetVarNameAST::firstToken() const
{
    if ( t_reset )
        return t_reset;
    if ( dotted_name )
        return dotted_name->firstToken();
    return 0;
}

int SetRestResetVarNameAST::lastToken() const
{
    if ( dotted_name )
        return dotted_name->lastToken();
    if ( t_reset )
        return t_reset + 1;
    return 1;
}

bool SetRestResetVarNameAST::isNull() const
{
    if ( t_reset )
        return false;
    if ( dotted_name )
        return false;
    return true;
}

void SetRestResetVarNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dotted_name, visitor);
    }
    visitor->endVisit(this);
}

int SetRestResetTimeZoneAST::firstToken() const
{
    if ( t_reset )
        return t_reset;
    if ( t_time )
        return t_time;
    if ( t_zone )
        return t_zone;
    return 0;
}

int SetRestResetTimeZoneAST::lastToken() const
{
    if ( t_zone )
        return t_zone + 1;
    if ( t_time )
        return t_time + 1;
    if ( t_reset )
        return t_reset + 1;
    return 1;
}

bool SetRestResetTimeZoneAST::isNull() const
{
    if ( t_reset )
        return false;
    if ( t_time )
        return false;
    if ( t_zone )
        return false;
    return true;
}

void SetRestResetTimeZoneAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestResetTransLevelAST::firstToken() const
{
    if ( t_reset )
        return t_reset;
    if ( t_transaction )
        return t_transaction;
    if ( t_isolation )
        return t_isolation;
    if ( t_level )
        return t_level;
    return 0;
}

int SetRestResetTransLevelAST::lastToken() const
{
    if ( t_level )
        return t_level + 1;
    if ( t_isolation )
        return t_isolation + 1;
    if ( t_transaction )
        return t_transaction + 1;
    if ( t_reset )
        return t_reset + 1;
    return 1;
}

bool SetRestResetTransLevelAST::isNull() const
{
    if ( t_reset )
        return false;
    if ( t_transaction )
        return false;
    if ( t_isolation )
        return false;
    if ( t_level )
        return false;
    return true;
}

void SetRestResetTransLevelAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestResetAuthorizationAST::firstToken() const
{
    if ( t_reset )
        return t_reset;
    if ( t_session )
        return t_session;
    if ( t_authorization )
        return t_authorization;
    return 0;
}

int SetRestResetAuthorizationAST::lastToken() const
{
    if ( t_authorization )
        return t_authorization + 1;
    if ( t_session )
        return t_session + 1;
    if ( t_reset )
        return t_reset + 1;
    return 1;
}

bool SetRestResetAuthorizationAST::isNull() const
{
    if ( t_reset )
        return false;
    if ( t_session )
        return false;
    if ( t_authorization )
        return false;
    return true;
}

void SetRestResetAuthorizationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SetRestResetAllAST::firstToken() const
{
    if ( t_all )
        return t_all;
    return 0;
}

int SetRestResetAllAST::lastToken() const
{
    if ( t_all )
        return t_all + 1;
    return 1;
}

bool SetRestResetAllAST::isNull() const
{
    if ( t_all )
        return false;
    return true;
}

void SetRestResetAllAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IsoLevelAST::firstToken() const
{
    if ( t_spec1 )
        return t_spec1;
    if ( t_spec2 )
        return t_spec2;
    return 0;
}

int IsoLevelAST::lastToken() const
{
    if ( t_spec2 )
        return t_spec2 + 1;
    if ( t_spec1 )
        return t_spec1 + 1;
    return 1;
}

bool IsoLevelAST::isNull() const
{
    if ( t_spec1 )
        return false;
    if ( t_spec2 )
        return false;
    return true;
}

void IsoLevelAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TransactionModeIsoLevelAST::firstToken() const
{
    if ( t_isolation )
        return t_isolation;
    if ( t_level )
        return t_level;
    if ( level && !level->isNull() )
        return level->firstToken();
    return 0;
}

int TransactionModeIsoLevelAST::lastToken() const
{
    if ( level && !level->isNull() )
        return level->lastToken();
    if ( t_level )
        return t_level + 1;
    if ( t_isolation )
        return t_isolation + 1;
    return 1;
}

bool TransactionModeIsoLevelAST::isNull() const
{
    if ( t_isolation )
        return false;
    if ( t_level )
        return false;
    if ( level )
        return false;
    return true;
}

void TransactionModeIsoLevelAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(level, visitor);
    }
    visitor->endVisit(this);
}

int TransactionModeReadWriteAST::firstToken() const
{
    if ( t_read )
        return t_read;
    if ( t_only_or_write )
        return t_only_or_write;
    return 0;
}

int TransactionModeReadWriteAST::lastToken() const
{
    if ( t_only_or_write )
        return t_only_or_write + 1;
    if ( t_read )
        return t_read + 1;
    return 1;
}

bool TransactionModeReadWriteAST::isNull() const
{
    if ( t_read )
        return false;
    if ( t_only_or_write )
        return false;
    return true;
}

void TransactionModeReadWriteAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TransactionModeDeferAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_deferrable )
        return t_deferrable;
    return 0;
}

int TransactionModeDeferAST::lastToken() const
{
    if ( t_deferrable )
        return t_deferrable + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool TransactionModeDeferAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_deferrable )
        return false;
    return true;
}

void TransactionModeDeferAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IntervalSecondAST::firstToken() const
{
    if ( t_second )
        return t_second;
    if ( t_lparen )
        return t_lparen;
    if ( t_value )
        return t_value;
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int IntervalSecondAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_value )
        return t_value + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_second )
        return t_second + 1;
    return 1;
}

bool IntervalSecondAST::isNull() const
{
    if ( t_second )
        return false;
    if ( t_lparen )
        return false;
    if ( t_value )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void IntervalSecondAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IntervalSimpleAST::firstToken() const
{
    if ( t_period )
        return t_period;
    return 0;
}

int IntervalSimpleAST::lastToken() const
{
    if ( t_period )
        return t_period + 1;
    return 1;
}

bool IntervalSimpleAST::isNull() const
{
    if ( t_period )
        return false;
    return true;
}

void IntervalSimpleAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IntervalSimpleToAST::firstToken() const
{
    if ( t_period )
        return t_period;
    if ( t_to )
        return t_to;
    if ( t_period_to )
        return t_period_to;
    return 0;
}

int IntervalSimpleToAST::lastToken() const
{
    if ( t_period_to )
        return t_period_to + 1;
    if ( t_to )
        return t_to + 1;
    if ( t_period )
        return t_period + 1;
    return 1;
}

bool IntervalSimpleToAST::isNull() const
{
    if ( t_period )
        return false;
    if ( t_to )
        return false;
    if ( t_period_to )
        return false;
    return true;
}

void IntervalSimpleToAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IntervalSimpleToSecondAST::firstToken() const
{
    if ( t_period )
        return t_period;
    if ( t_to )
        return t_to;
    if ( second && !second->isNull() )
        return second->firstToken();
    return 0;
}

int IntervalSimpleToSecondAST::lastToken() const
{
    if ( second && !second->isNull() )
        return second->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_period )
        return t_period + 1;
    return 1;
}

bool IntervalSimpleToSecondAST::isNull() const
{
    if ( t_period )
        return false;
    if ( t_to )
        return false;
    if ( second )
        return false;
    return true;
}

void IntervalSimpleToSecondAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(second, visitor);
    }
    visitor->endVisit(this);
}

int VarValueAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

int VarValueAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool VarValueAST::isNull() const
{
    if ( t_value )
        return false;
    return true;
}

void VarValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ZoneValueSimpleAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

int ZoneValueSimpleAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool ZoneValueSimpleAST::isNull() const
{
    if ( t_value )
        return false;
    return true;
}

void ZoneValueSimpleAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ZoneValueIntervalAST::firstToken() const
{
    if ( t_interval )
        return t_interval;
    if ( t_lparen )
        return t_lparen;
    if ( t_val )
        return t_val;
    if ( t_rparen )
        return t_rparen;
    if ( t_sval )
        return t_sval;
    if ( interval && !interval->isNull() )
        return interval->firstToken();
    return 0;
}

int ZoneValueIntervalAST::lastToken() const
{
    if ( interval && !interval->isNull() )
        return interval->lastToken();
    if ( t_sval )
        return t_sval + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_val )
        return t_val + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_interval )
        return t_interval + 1;
    return 1;
}

bool ZoneValueIntervalAST::isNull() const
{
    if ( t_interval )
        return false;
    if ( t_lparen )
        return false;
    if ( t_val )
        return false;
    if ( t_rparen )
        return false;
    if ( t_sval )
        return false;
    if ( interval )
        return false;
    return true;
}

void ZoneValueIntervalAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(interval, visitor);
    }
    visitor->endVisit(this);
}

int DropRoleStmtAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_role )
        return t_role;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( roles )
        return roles->firstToken();
    return 0;
}

int DropRoleStmtAST::lastToken() const
{
    if ( roles )
        return roles->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_role )
        return t_role + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropRoleStmtAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_role )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( roles )
        return false;
    return true;
}

void DropRoleStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roles, visitor);
    }
    visitor->endVisit(this);
}

int CreateGroupStmtAST::firstToken() const
{
    if ( t_create )
        return t_create;
    if ( t_group )
        return t_group;
    if ( groupIdent && !groupIdent->isNull() )
        return groupIdent->firstToken();
    if ( t_with )
        return t_with;
    if ( options )
        return options->firstToken();
    return 0;
}

int CreateGroupStmtAST::lastToken() const
{
    if ( options )
        return options->lastToken();
    if ( t_with )
        return t_with + 1;
    if ( groupIdent && !groupIdent->isNull() )
        return groupIdent->lastToken();
    if ( t_group )
        return t_group + 1;
    if ( t_create )
        return t_create + 1;
    return 1;
}

bool CreateGroupStmtAST::isNull() const
{
    if ( t_create )
        return false;
    if ( t_group )
        return false;
    if ( groupIdent )
        return false;
    if ( t_with )
        return false;
    if ( options )
        return false;
    return true;
}

void CreateGroupStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(groupIdent, visitor);
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int AlterGroupStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_group )
        return t_group;
    if ( groupIdent && !groupIdent->isNull() )
        return groupIdent->firstToken();
    if ( t_add_or_drop )
        return t_add_or_drop;
    if ( t_user )
        return t_user;
    if ( roles )
        return roles->firstToken();
    return 0;
}

int AlterGroupStmtAST::lastToken() const
{
    if ( roles )
        return roles->lastToken();
    if ( t_user )
        return t_user + 1;
    if ( t_add_or_drop )
        return t_add_or_drop + 1;
    if ( groupIdent && !groupIdent->isNull() )
        return groupIdent->lastToken();
    if ( t_group )
        return t_group + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterGroupStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_group )
        return false;
    if ( groupIdent )
        return false;
    if ( t_add_or_drop )
        return false;
    if ( t_user )
        return false;
    if ( roles )
        return false;
    return true;
}

void AlterGroupStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(groupIdent, visitor);
        accept(roles, visitor);
    }
    visitor->endVisit(this);
}

int DiscardStmtAST::firstToken() const
{
    if ( t_discard )
        return t_discard;
    if ( t_all_or_tem_or_plans )
        return t_all_or_tem_or_plans;
    return 0;
}

int DiscardStmtAST::lastToken() const
{
    if ( t_all_or_tem_or_plans )
        return t_all_or_tem_or_plans + 1;
    if ( t_discard )
        return t_discard + 1;
    return 1;
}

bool DiscardStmtAST::isNull() const
{
    if ( t_discard )
        return false;
    if ( t_all_or_tem_or_plans )
        return false;
    return true;
}

void DiscardStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterIndexStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_index )
        return t_index;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( indexIdent && !indexIdent->isNull() )
        return indexIdent->firstToken();
    if ( alterCommands )
        return alterCommands->firstToken();
    return 0;
}

int AlterIndexStmtAST::lastToken() const
{
    if ( alterCommands )
        return alterCommands->lastToken();
    if ( indexIdent && !indexIdent->isNull() )
        return indexIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_index )
        return t_index + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterIndexStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_index )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( indexIdent )
        return false;
    if ( alterCommands )
        return false;
    return true;
}

void AlterIndexStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexIdent, visitor);
        accept(alterCommands, visitor);
    }
    visitor->endVisit(this);
}

int AlterSequenceStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_sequence )
        return t_sequence;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( indexIdent && !indexIdent->isNull() )
        return indexIdent->firstToken();
    if ( alterCommands )
        return alterCommands->firstToken();
    return 0;
}

int AlterSequenceStmtAST::lastToken() const
{
    if ( alterCommands )
        return alterCommands->lastToken();
    if ( indexIdent && !indexIdent->isNull() )
        return indexIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_sequence )
        return t_sequence + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterSequenceStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_sequence )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( indexIdent )
        return false;
    if ( alterCommands )
        return false;
    return true;
}

void AlterSequenceStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexIdent, visitor);
        accept(alterCommands, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( relation && !relation->isNull() )
        return relation->firstToken();
    if ( alterCommands )
        return alterCommands->firstToken();
    return 0;
}

int AlterTableStmtAST::lastToken() const
{
    if ( alterCommands )
        return alterCommands->lastToken();
    if ( relation && !relation->isNull() )
        return relation->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( relation )
        return false;
    if ( alterCommands )
        return false;
    return true;
}

void AlterTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relation, visitor);
        accept(alterCommands, visitor);
    }
    visitor->endVisit(this);
}

int AlterForeignTableStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_foreign )
        return t_foreign;
    if ( t_table )
        return t_table;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( relation && !relation->isNull() )
        return relation->firstToken();
    if ( alterCommands )
        return alterCommands->firstToken();
    return 0;
}

int AlterForeignTableStmtAST::lastToken() const
{
    if ( alterCommands )
        return alterCommands->lastToken();
    if ( relation && !relation->isNull() )
        return relation->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_table )
        return t_table + 1;
    if ( t_foreign )
        return t_foreign + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterForeignTableStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_foreign )
        return false;
    if ( t_table )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( relation )
        return false;
    if ( alterCommands )
        return false;
    return true;
}

void AlterForeignTableStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relation, visitor);
        accept(alterCommands, visitor);
    }
    visitor->endVisit(this);
}

int AlterViewStmtAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_materialized )
        return t_materialized;
    if ( t_view )
        return t_view;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( alterCommands )
        return alterCommands->firstToken();
    return 0;
}

int AlterViewStmtAST::lastToken() const
{
    if ( alterCommands )
        return alterCommands->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_view )
        return t_view + 1;
    if ( t_materialized )
        return t_materialized + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterViewStmtAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_materialized )
        return false;
    if ( t_view )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( tableIdent )
        return false;
    if ( alterCommands )
        return false;
    return true;
}

void AlterViewStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(alterCommands, visitor);
    }
    visitor->endVisit(this);
}

int UpdateStmtAST::firstToken() const
{
    if ( withClause && !withClause->isNull() )
        return withClause->firstToken();
    if ( t_update )
        return t_update;
    if ( relalias && !relalias->isNull() )
        return relalias->firstToken();
    if ( t_set )
        return t_set;
    if ( sets )
        return sets->firstToken();
    if ( fromClause && !fromClause->isNull() )
        return fromClause->firstToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->firstToken();
    if ( retClause && !retClause->isNull() )
        return retClause->firstToken();
    return 0;
}

int UpdateStmtAST::lastToken() const
{
    if ( retClause && !retClause->isNull() )
        return retClause->lastToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->lastToken();
    if ( fromClause && !fromClause->isNull() )
        return fromClause->lastToken();
    if ( sets )
        return sets->lastToken();
    if ( t_set )
        return t_set + 1;
    if ( relalias && !relalias->isNull() )
        return relalias->lastToken();
    if ( t_update )
        return t_update + 1;
    if ( withClause && !withClause->isNull() )
        return withClause->lastToken();
    return 1;
}

bool UpdateStmtAST::isNull() const
{
    if ( withClause )
        return false;
    if ( t_update )
        return false;
    if ( relalias )
        return false;
    if ( t_set )
        return false;
    if ( sets )
        return false;
    if ( fromClause )
        return false;
    if ( whereClause )
        return false;
    if ( retClause )
        return false;
    return true;
}

void UpdateStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(withClause, visitor);
        accept(relalias, visitor);
        accept(sets, visitor);
        accept(fromClause, visitor);
        accept(whereClause, visitor);
        accept(retClause, visitor);
    }
    visitor->endVisit(this);
}

int SingleSetClauseAST::firstToken() const
{
    if ( columnRef && !columnRef->isNull() )
        return columnRef->firstToken();
    if ( t_equal )
        return t_equal;
    if ( value && !value->isNull() )
        return value->firstToken();
    return 0;
}

int SingleSetClauseAST::lastToken() const
{
    if ( value && !value->isNull() )
        return value->lastToken();
    if ( t_equal )
        return t_equal + 1;
    if ( columnRef && !columnRef->isNull() )
        return columnRef->lastToken();
    return 1;
}

bool SingleSetClauseAST::isNull() const
{
    if ( columnRef )
        return false;
    if ( t_equal )
        return false;
    if ( value )
        return false;
    return true;
}

void SingleSetClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnRef, visitor);
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

int MultipleSetClauseAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( t_equal )
        return t_equal;
    if ( values && !values->isNull() )
        return values->firstToken();
    return 0;
}

int MultipleSetClauseAST::lastToken() const
{
    if ( values && !values->isNull() )
        return values->lastToken();
    if ( t_equal )
        return t_equal + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool MultipleSetClauseAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( t_equal )
        return false;
    if ( values )
        return false;
    return true;
}

void MultipleSetClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int InsertStmtAST::firstToken() const
{
    if ( withClause && !withClause->isNull() )
        return withClause->firstToken();
    if ( t_insert )
        return t_insert;
    if ( t_into )
        return t_into;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( rest && !rest->isNull() )
        return rest->firstToken();
    if ( retClause && !retClause->isNull() )
        return retClause->firstToken();
    return 0;
}

int InsertStmtAST::lastToken() const
{
    if ( retClause && !retClause->isNull() )
        return retClause->lastToken();
    if ( rest && !rest->isNull() )
        return rest->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_into )
        return t_into + 1;
    if ( t_insert )
        return t_insert + 1;
    if ( withClause && !withClause->isNull() )
        return withClause->lastToken();
    return 1;
}

bool InsertStmtAST::isNull() const
{
    if ( withClause )
        return false;
    if ( t_insert )
        return false;
    if ( t_into )
        return false;
    if ( tableIdent )
        return false;
    if ( rest )
        return false;
    if ( retClause )
        return false;
    return true;
}

void InsertStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(withClause, visitor);
        accept(tableIdent, visitor);
        accept(rest, visitor);
        accept(retClause, visitor);
    }
    visitor->endVisit(this);
}

int CommonInsertRestAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( content && !content->isNull() )
        return content->firstToken();
    return 0;
}

int CommonInsertRestAST::lastToken() const
{
    if ( content && !content->isNull() )
        return content->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool CommonInsertRestAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( content )
        return false;
    return true;
}

void CommonInsertRestAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
        accept(content, visitor);
    }
    visitor->endVisit(this);
}

int DefaultValuesInsertRestAST::firstToken() const
{
    if ( t_default )
        return t_default;
    if ( t_values )
        return t_values;
    return 0;
}

int DefaultValuesInsertRestAST::lastToken() const
{
    if ( t_values )
        return t_values + 1;
    if ( t_default )
        return t_default + 1;
    return 1;
}

bool DefaultValuesInsertRestAST::isNull() const
{
    if ( t_default )
        return false;
    if ( t_values )
        return false;
    return true;
}

void DefaultValuesInsertRestAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DeleteStmtAST::firstToken() const
{
    if ( withClause && !withClause->isNull() )
        return withClause->firstToken();
    if ( t_delete )
        return t_delete;
    if ( t_from )
        return t_from;
    if ( relalias && !relalias->isNull() )
        return relalias->firstToken();
    if ( usingClause && !usingClause->isNull() )
        return usingClause->firstToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->firstToken();
    if ( retClause && !retClause->isNull() )
        return retClause->firstToken();
    return 0;
}

int DeleteStmtAST::lastToken() const
{
    if ( retClause && !retClause->isNull() )
        return retClause->lastToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->lastToken();
    if ( usingClause && !usingClause->isNull() )
        return usingClause->lastToken();
    if ( relalias && !relalias->isNull() )
        return relalias->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( t_delete )
        return t_delete + 1;
    if ( withClause && !withClause->isNull() )
        return withClause->lastToken();
    return 1;
}

bool DeleteStmtAST::isNull() const
{
    if ( withClause )
        return false;
    if ( t_delete )
        return false;
    if ( t_from )
        return false;
    if ( relalias )
        return false;
    if ( usingClause )
        return false;
    if ( whereClause )
        return false;
    if ( retClause )
        return false;
    return true;
}

void DeleteStmtAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(withClause, visitor);
        accept(relalias, visitor);
        accept(usingClause, visitor);
        accept(whereClause, visitor);
        accept(retClause, visitor);
    }
    visitor->endVisit(this);
}

int ReturningClauseAST::firstToken() const
{
    if ( t_returning )
        return t_returning;
    if ( results )
        return results->firstToken();
    return 0;
}

int ReturningClauseAST::lastToken() const
{
    if ( results )
        return results->lastToken();
    if ( t_returning )
        return t_returning + 1;
    return 1;
}

bool ReturningClauseAST::isNull() const
{
    if ( t_returning )
        return false;
    if ( results )
        return false;
    return true;
}

void ReturningClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(results, visitor);
    }
    visitor->endVisit(this);
}

int WhereNoCurrentClauseAST::firstToken() const
{
    if ( t_where )
        return t_where;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int WhereNoCurrentClauseAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_where )
        return t_where + 1;
    return 1;
}

bool WhereNoCurrentClauseAST::isNull() const
{
    if ( t_where )
        return false;
    if ( expr )
        return false;
    return true;
}

void WhereNoCurrentClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CurrenWhereClauseAST::firstToken() const
{
    if ( t_where )
        return t_where;
    if ( t_current )
        return t_current;
    if ( t_of )
        return t_of;
    if ( cursorName && !cursorName->isNull() )
        return cursorName->firstToken();
    return 0;
}

int CurrenWhereClauseAST::lastToken() const
{
    if ( cursorName && !cursorName->isNull() )
        return cursorName->lastToken();
    if ( t_of )
        return t_of + 1;
    if ( t_current )
        return t_current + 1;
    if ( t_where )
        return t_where + 1;
    return 1;
}

bool CurrenWhereClauseAST::isNull() const
{
    if ( t_where )
        return false;
    if ( t_current )
        return false;
    if ( t_of )
        return false;
    if ( cursorName )
        return false;
    return true;
}

void CurrenWhereClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(cursorName, visitor);
    }
    visitor->endVisit(this);
}

int DelUsingClauseAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( refs )
        return refs->firstToken();
    return 0;
}

int DelUsingClauseAST::lastToken() const
{
    if ( refs )
        return refs->lastToken();
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool DelUsingClauseAST::isNull() const
{
    if ( t_using )
        return false;
    if ( refs )
        return false;
    return true;
}

void DelUsingClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(refs, visitor);
    }
    visitor->endVisit(this);
}

int RelationAliasExprAST::firstToken() const
{
    if ( rel && !rel->isNull() )
        return rel->firstToken();
    if ( t_as )
        return t_as;
    if ( alias && !alias->isNull() )
        return alias->firstToken();
    return 0;
}

int RelationAliasExprAST::lastToken() const
{
    if ( alias && !alias->isNull() )
        return alias->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( rel && !rel->isNull() )
        return rel->lastToken();
    return 1;
}

bool RelationAliasExprAST::isNull() const
{
    if ( rel )
        return false;
    if ( t_as )
        return false;
    if ( alias )
        return false;
    return true;
}

void RelationAliasExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rel, visitor);
        accept(alias, visitor);
    }
    visitor->endVisit(this);
}

int SelectWithParensAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( statement && !statement->isNull() )
        return statement->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int SelectWithParensAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( statement && !statement->isNull() )
        return statement->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool SelectWithParensAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( statement )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void SelectWithParensAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(statement, visitor);
    }
    visitor->endVisit(this);
}

int SelectNoParensAST::firstToken() const
{
    if ( withClause && !withClause->isNull() )
        return withClause->firstToken();
    if ( query && !query->isNull() )
        return query->firstToken();
    if ( sortClause && !sortClause->isNull() )
        return sortClause->firstToken();
    if ( llClause && !llClause->isNull() )
        return llClause->firstToken();
    return 0;
}

int SelectNoParensAST::lastToken() const
{
    if ( llClause && !llClause->isNull() )
        return llClause->lastToken();
    if ( sortClause && !sortClause->isNull() )
        return sortClause->lastToken();
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( withClause && !withClause->isNull() )
        return withClause->lastToken();
    return 1;
}

bool SelectNoParensAST::isNull() const
{
    if ( withClause )
        return false;
    if ( query )
        return false;
    if ( sortClause )
        return false;
    if ( llClause )
        return false;
    return true;
}

void SelectNoParensAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(withClause, visitor);
        accept(query, visitor);
        accept(sortClause, visitor);
        accept(llClause, visitor);
    }
    visitor->endVisit(this);
}

int WithClauseAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( t_recursive )
        return t_recursive;
    if ( withQueries )
        return withQueries->firstToken();
    return 0;
}

int WithClauseAST::lastToken() const
{
    if ( withQueries )
        return withQueries->lastToken();
    if ( t_recursive )
        return t_recursive + 1;
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool WithClauseAST::isNull() const
{
    if ( t_with )
        return false;
    if ( t_recursive )
        return false;
    if ( withQueries )
        return false;
    return true;
}

void WithClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(withQueries, visitor);
    }
    visitor->endVisit(this);
}

int CommonTableExprAST::firstToken() const
{
    if ( relName && !relName->isNull() )
        return relName->firstToken();
    if ( relColumns && !relColumns->isNull() )
        return relColumns->firstToken();
    if ( t_as )
        return t_as;
    if ( t_lparen )
        return t_lparen;
    if ( statement && !statement->isNull() )
        return statement->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CommonTableExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( statement && !statement->isNull() )
        return statement->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_as )
        return t_as + 1;
    if ( relColumns && !relColumns->isNull() )
        return relColumns->lastToken();
    if ( relName && !relName->isNull() )
        return relName->lastToken();
    return 1;
}

bool CommonTableExprAST::isNull() const
{
    if ( relName )
        return false;
    if ( relColumns )
        return false;
    if ( t_as )
        return false;
    if ( t_lparen )
        return false;
    if ( statement )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CommonTableExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relName, visitor);
        accept(relColumns, visitor);
        accept(statement, visitor);
    }
    visitor->endVisit(this);
}

int CompoundColumnNamesAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( names )
        return names->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CompoundColumnNamesAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( names )
        return names->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool CompoundColumnNamesAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( names )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CompoundColumnNamesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(names, visitor);
    }
    visitor->endVisit(this);
}

int SortClauseAST::firstToken() const
{
    if ( t_sort )
        return t_sort;
    if ( t_by )
        return t_by;
    if ( sortList )
        return sortList->firstToken();
    return 0;
}

int SortClauseAST::lastToken() const
{
    if ( sortList )
        return sortList->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_sort )
        return t_sort + 1;
    return 1;
}

bool SortClauseAST::isNull() const
{
    if ( t_sort )
        return false;
    if ( t_by )
        return false;
    if ( sortList )
        return false;
    return true;
}

void SortClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(sortList, visitor);
    }
    visitor->endVisit(this);
}

int UsingOrderExprAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_using )
        return t_using;
    if ( qualOp && !qualOp->isNull() )
        return qualOp->firstToken();
    if ( t_nulls_order )
        return t_nulls_order;
    return 0;
}

int UsingOrderExprAST::lastToken() const
{
    if ( t_nulls_order )
        return t_nulls_order + 1;
    if ( qualOp && !qualOp->isNull() )
        return qualOp->lastToken();
    if ( t_using )
        return t_using + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool UsingOrderExprAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_using )
        return false;
    if ( qualOp )
        return false;
    if ( t_nulls_order )
        return false;
    return true;
}

void UsingOrderExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(qualOp, visitor);
    }
    visitor->endVisit(this);
}

int CommonOrderExprAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_asc_or_desc )
        return t_asc_or_desc;
    if ( t_nulls_order )
        return t_nulls_order;
    return 0;
}

int CommonOrderExprAST::lastToken() const
{
    if ( t_nulls_order )
        return t_nulls_order + 1;
    if ( t_asc_or_desc )
        return t_asc_or_desc + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool CommonOrderExprAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_asc_or_desc )
        return false;
    if ( t_nulls_order )
        return false;
    return true;
}

void CommonOrderExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int LockingFirstLimitClauseAST::firstToken() const
{
    if ( locking && !locking->isNull() )
        return locking->firstToken();
    if ( limit && !limit->isNull() )
        return limit->firstToken();
    return 0;
}

int LockingFirstLimitClauseAST::lastToken() const
{
    if ( limit && !limit->isNull() )
        return limit->lastToken();
    if ( locking && !locking->isNull() )
        return locking->lastToken();
    return 1;
}

bool LockingFirstLimitClauseAST::isNull() const
{
    if ( locking )
        return false;
    if ( limit )
        return false;
    return true;
}

void LockingFirstLimitClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(locking, visitor);
        accept(limit, visitor);
    }
    visitor->endVisit(this);
}

int LockingLastLimitClauseAST::firstToken() const
{
    if ( limit && !limit->isNull() )
        return limit->firstToken();
    if ( locking && !locking->isNull() )
        return locking->firstToken();
    return 0;
}

int LockingLastLimitClauseAST::lastToken() const
{
    if ( locking && !locking->isNull() )
        return locking->lastToken();
    if ( limit && !limit->isNull() )
        return limit->lastToken();
    return 1;
}

bool LockingLastLimitClauseAST::isNull() const
{
    if ( limit )
        return false;
    if ( locking )
        return false;
    return true;
}

void LockingLastLimitClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(limit, visitor);
        accept(locking, visitor);
    }
    visitor->endVisit(this);
}

int LimitOffsetClauseAST::firstToken() const
{
    if ( limit && !limit->isNull() )
        return limit->firstToken();
    if ( offset && !offset->isNull() )
        return offset->firstToken();
    return 0;
}

int LimitOffsetClauseAST::lastToken() const
{
    if ( offset && !offset->isNull() )
        return offset->lastToken();
    if ( limit && !limit->isNull() )
        return limit->lastToken();
    return 1;
}

bool LimitOffsetClauseAST::isNull() const
{
    if ( limit )
        return false;
    if ( offset )
        return false;
    return true;
}

void LimitOffsetClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(limit, visitor);
        accept(offset, visitor);
    }
    visitor->endVisit(this);
}

int OffsetLimitClauseAST::firstToken() const
{
    if ( offset && !offset->isNull() )
        return offset->firstToken();
    if ( limit && !limit->isNull() )
        return limit->firstToken();
    return 0;
}

int OffsetLimitClauseAST::lastToken() const
{
    if ( limit && !limit->isNull() )
        return limit->lastToken();
    if ( offset && !offset->isNull() )
        return offset->lastToken();
    return 1;
}

bool OffsetLimitClauseAST::isNull() const
{
    if ( offset )
        return false;
    if ( limit )
        return false;
    return true;
}

void OffsetLimitClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(offset, visitor);
        accept(limit, visitor);
    }
    visitor->endVisit(this);
}

int OffsetAST::firstToken() const
{
    if ( t_offset )
        return t_offset;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_row_or_rows )
        return t_row_or_rows;
    return 0;
}

int OffsetAST::lastToken() const
{
    if ( t_row_or_rows )
        return t_row_or_rows + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_offset )
        return t_offset + 1;
    return 1;
}

bool OffsetAST::isNull() const
{
    if ( t_offset )
        return false;
    if ( expr )
        return false;
    if ( t_row_or_rows )
        return false;
    return true;
}

void OffsetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CommonLimitAST::firstToken() const
{
    if ( t_limit )
        return t_limit;
    if ( value && !value->isNull() )
        return value->firstToken();
    if ( t_comma )
        return t_comma;
    if ( offset && !offset->isNull() )
        return offset->firstToken();
    return 0;
}

int CommonLimitAST::lastToken() const
{
    if ( offset && !offset->isNull() )
        return offset->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( value && !value->isNull() )
        return value->lastToken();
    if ( t_limit )
        return t_limit + 1;
    return 1;
}

bool CommonLimitAST::isNull() const
{
    if ( t_limit )
        return false;
    if ( value )
        return false;
    if ( t_comma )
        return false;
    if ( offset )
        return false;
    return true;
}

void CommonLimitAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
        accept(offset, visitor);
    }
    visitor->endVisit(this);
}

int FetchLimitAST::firstToken() const
{
    if ( t_fetch )
        return t_fetch;
    if ( t_first_or_next )
        return t_first_or_next;
    if ( first && !first->isNull() )
        return first->firstToken();
    if ( t_row_or_rows )
        return t_row_or_rows;
    if ( t_only )
        return t_only;
    return 0;
}

int FetchLimitAST::lastToken() const
{
    if ( t_only )
        return t_only + 1;
    if ( t_row_or_rows )
        return t_row_or_rows + 1;
    if ( first && !first->isNull() )
        return first->lastToken();
    if ( t_first_or_next )
        return t_first_or_next + 1;
    if ( t_fetch )
        return t_fetch + 1;
    return 1;
}

bool FetchLimitAST::isNull() const
{
    if ( t_fetch )
        return false;
    if ( t_first_or_next )
        return false;
    if ( first )
        return false;
    if ( t_row_or_rows )
        return false;
    if ( t_only )
        return false;
    return true;
}

void FetchLimitAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(first, visitor);
    }
    visitor->endVisit(this);
}

int ConstFetchFirstValueAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

int ConstFetchFirstValueAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool ConstFetchFirstValueAST::isNull() const
{
    if ( t_value )
        return false;
    return true;
}

void ConstFetchFirstValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CompoundFetchFirstValueAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CompoundFetchFirstValueAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool CompoundFetchFirstValueAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CompoundFetchFirstValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int AllLimitValueAST::firstToken() const
{
    if ( t_all )
        return t_all;
    return 0;
}

int AllLimitValueAST::lastToken() const
{
    if ( t_all )
        return t_all + 1;
    return 1;
}

bool AllLimitValueAST::isNull() const
{
    if ( t_all )
        return false;
    return true;
}

void AllLimitValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ExprLimitValueAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int ExprLimitValueAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool ExprLimitValueAST::isNull() const
{
    if ( expr )
        return false;
    return true;
}

void ExprLimitValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int ReadOnlyForLockingClauseAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_read )
        return t_read;
    if ( t_only )
        return t_only;
    return 0;
}

int ReadOnlyForLockingClauseAST::lastToken() const
{
    if ( t_only )
        return t_only + 1;
    if ( t_read )
        return t_read + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool ReadOnlyForLockingClauseAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_read )
        return false;
    if ( t_only )
        return false;
    return true;
}

void ReadOnlyForLockingClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommonForLockingClauseAST::firstToken() const
{
    if ( lockings )
        return lockings->firstToken();
    return 0;
}

int CommonForLockingClauseAST::lastToken() const
{
    if ( lockings )
        return lockings->lastToken();
    return 1;
}

bool CommonForLockingClauseAST::isNull() const
{
    if ( lockings )
        return false;
    return true;
}

void CommonForLockingClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lockings, visitor);
    }
    visitor->endVisit(this);
}

int LockingExprAST::firstToken() const
{
    if ( strentgh && !strentgh->isNull() )
        return strentgh->firstToken();
    if ( relations && !relations->isNull() )
        return relations->firstToken();
    if ( t_nowait )
        return t_nowait;
    return 0;
}

int LockingExprAST::lastToken() const
{
    if ( t_nowait )
        return t_nowait + 1;
    if ( relations && !relations->isNull() )
        return relations->lastToken();
    if ( strentgh && !strentgh->isNull() )
        return strentgh->lastToken();
    return 1;
}

bool LockingExprAST::isNull() const
{
    if ( strentgh )
        return false;
    if ( relations )
        return false;
    if ( t_nowait )
        return false;
    return true;
}

void LockingExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(strentgh, visitor);
        accept(relations, visitor);
    }
    visitor->endVisit(this);
}

int LockingStrengthAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( t_spec1 )
        return t_spec1;
    if ( t_spec2 )
        return t_spec2;
    if ( t_spec3 )
        return t_spec3;
    return 0;
}

int LockingStrengthAST::lastToken() const
{
    if ( t_spec3 )
        return t_spec3 + 1;
    if ( t_spec2 )
        return t_spec2 + 1;
    if ( t_spec1 )
        return t_spec1 + 1;
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool LockingStrengthAST::isNull() const
{
    if ( t_for )
        return false;
    if ( t_spec1 )
        return false;
    if ( t_spec2 )
        return false;
    if ( t_spec3 )
        return false;
    return true;
}

void LockingStrengthAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int LockingRelsAST::firstToken() const
{
    if ( t_of )
        return t_of;
    if ( lockRels )
        return lockRels->firstToken();
    return 0;
}

int LockingRelsAST::lastToken() const
{
    if ( lockRels )
        return lockRels->lastToken();
    if ( t_of )
        return t_of + 1;
    return 1;
}

bool LockingRelsAST::isNull() const
{
    if ( t_of )
        return false;
    if ( lockRels )
        return false;
    return true;
}

void LockingRelsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lockRels, visitor);
    }
    visitor->endVisit(this);
}

int CompoundSelectClauseAST::firstToken() const
{
    if ( queryWithParens && !queryWithParens->isNull() )
        return queryWithParens->firstToken();
    return 0;
}

int CompoundSelectClauseAST::lastToken() const
{
    if ( queryWithParens && !queryWithParens->isNull() )
        return queryWithParens->lastToken();
    return 1;
}

bool CompoundSelectClauseAST::isNull() const
{
    if ( queryWithParens )
        return false;
    return true;
}

void CompoundSelectClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(queryWithParens, visitor);
    }
    visitor->endVisit(this);
}

int CoreSimpleSelectAST::firstToken() const
{
    if ( queryCore && !queryCore->isNull() )
        return queryCore->firstToken();
    return 0;
}

int CoreSimpleSelectAST::lastToken() const
{
    if ( queryCore && !queryCore->isNull() )
        return queryCore->lastToken();
    return 1;
}

bool CoreSimpleSelectAST::isNull() const
{
    if ( queryCore )
        return false;
    return true;
}

void CoreSimpleSelectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(queryCore, visitor);
    }
    visitor->endVisit(this);
}

int ValuesSimpleSelectAST::firstToken() const
{
    if ( values && !values->isNull() )
        return values->firstToken();
    return 0;
}

int ValuesSimpleSelectAST::lastToken() const
{
    if ( values && !values->isNull() )
        return values->lastToken();
    return 1;
}

bool ValuesSimpleSelectAST::isNull() const
{
    if ( values )
        return false;
    return true;
}

void ValuesSimpleSelectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int TableSimpleSelectAST::firstToken() const
{
    if ( t_table )
        return t_table;
    if ( relation && !relation->isNull() )
        return relation->firstToken();
    return 0;
}

int TableSimpleSelectAST::lastToken() const
{
    if ( relation && !relation->isNull() )
        return relation->lastToken();
    if ( t_table )
        return t_table + 1;
    return 1;
}

bool TableSimpleSelectAST::isNull() const
{
    if ( t_table )
        return false;
    if ( relation )
        return false;
    return true;
}

void TableSimpleSelectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relation, visitor);
    }
    visitor->endVisit(this);
}

int CompositeSimpleSelectAST::firstToken() const
{
    if ( lQuery && !lQuery->isNull() )
        return lQuery->firstToken();
    if ( t_compose_op )
        return t_compose_op;
    if ( t_compose_spec )
        return t_compose_spec;
    if ( rQuery && !rQuery->isNull() )
        return rQuery->firstToken();
    return 0;
}

int CompositeSimpleSelectAST::lastToken() const
{
    if ( rQuery && !rQuery->isNull() )
        return rQuery->lastToken();
    if ( t_compose_spec )
        return t_compose_spec + 1;
    if ( t_compose_op )
        return t_compose_op + 1;
    if ( lQuery && !lQuery->isNull() )
        return lQuery->lastToken();
    return 1;
}

bool CompositeSimpleSelectAST::isNull() const
{
    if ( lQuery )
        return false;
    if ( t_compose_op )
        return false;
    if ( t_compose_spec )
        return false;
    if ( rQuery )
        return false;
    return true;
}

void CompositeSimpleSelectAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lQuery, visitor);
        accept(rQuery, visitor);
    }
    visitor->endVisit(this);
}

int SelectCoreAST::firstToken() const
{
    if ( t_select )
        return t_select;
    if ( distinctSpec && !distinctSpec->isNull() )
        return distinctSpec->firstToken();
    if ( targets )
        return targets->firstToken();
    if ( intoClause && !intoClause->isNull() )
        return intoClause->firstToken();
    if ( fromClause && !fromClause->isNull() )
        return fromClause->firstToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->firstToken();
    if ( groupByClause && !groupByClause->isNull() )
        return groupByClause->firstToken();
    if ( havingClause && !havingClause->isNull() )
        return havingClause->firstToken();
    if ( windowClause && !windowClause->isNull() )
        return windowClause->firstToken();
    return 0;
}

int SelectCoreAST::lastToken() const
{
    if ( windowClause && !windowClause->isNull() )
        return windowClause->lastToken();
    if ( havingClause && !havingClause->isNull() )
        return havingClause->lastToken();
    if ( groupByClause && !groupByClause->isNull() )
        return groupByClause->lastToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->lastToken();
    if ( fromClause && !fromClause->isNull() )
        return fromClause->lastToken();
    if ( intoClause && !intoClause->isNull() )
        return intoClause->lastToken();
    if ( targets )
        return targets->lastToken();
    if ( distinctSpec && !distinctSpec->isNull() )
        return distinctSpec->lastToken();
    if ( t_select )
        return t_select + 1;
    return 1;
}

bool SelectCoreAST::isNull() const
{
    if ( t_select )
        return false;
    if ( distinctSpec )
        return false;
    if ( targets )
        return false;
    if ( intoClause )
        return false;
    if ( fromClause )
        return false;
    if ( whereClause )
        return false;
    if ( groupByClause )
        return false;
    if ( havingClause )
        return false;
    if ( windowClause )
        return false;
    return true;
}

void SelectCoreAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(distinctSpec, visitor);
        accept(targets, visitor);
        accept(intoClause, visitor);
        accept(fromClause, visitor);
        accept(whereClause, visitor);
        accept(groupByClause, visitor);
        accept(havingClause, visitor);
        accept(windowClause, visitor);
    }
    visitor->endVisit(this);
}

int WindowClauseAST::firstToken() const
{
    if ( t_window )
        return t_window;
    if ( definitions )
        return definitions->firstToken();
    return 0;
}

int WindowClauseAST::lastToken() const
{
    if ( definitions )
        return definitions->lastToken();
    if ( t_window )
        return t_window + 1;
    return 1;
}

bool WindowClauseAST::isNull() const
{
    if ( t_window )
        return false;
    if ( definitions )
        return false;
    return true;
}

void WindowClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(definitions, visitor);
    }
    visitor->endVisit(this);
}

int WindowDefinitionAST::firstToken() const
{
    if ( windowName && !windowName->isNull() )
        return windowName->firstToken();
    if ( t_as )
        return t_as;
    if ( specification && !specification->isNull() )
        return specification->firstToken();
    return 0;
}

int WindowDefinitionAST::lastToken() const
{
    if ( specification && !specification->isNull() )
        return specification->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( windowName && !windowName->isNull() )
        return windowName->lastToken();
    return 1;
}

bool WindowDefinitionAST::isNull() const
{
    if ( windowName )
        return false;
    if ( t_as )
        return false;
    if ( specification )
        return false;
    return true;
}

void WindowDefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(windowName, visitor);
        accept(specification, visitor);
    }
    visitor->endVisit(this);
}

int HavingClauseAST::firstToken() const
{
    if ( t_having )
        return t_having;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int HavingClauseAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_having )
        return t_having + 1;
    return 1;
}

bool HavingClauseAST::isNull() const
{
    if ( t_having )
        return false;
    if ( expr )
        return false;
    return true;
}

void HavingClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int GroupByClauseAST::firstToken() const
{
    if ( t_group )
        return t_group;
    if ( t_by )
        return t_by;
    if ( groupings )
        return groupings->firstToken();
    return 0;
}

int GroupByClauseAST::lastToken() const
{
    if ( groupings )
        return groupings->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_group )
        return t_group + 1;
    return 1;
}

bool GroupByClauseAST::isNull() const
{
    if ( t_group )
        return false;
    if ( t_by )
        return false;
    if ( groupings )
        return false;
    return true;
}

void GroupByClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(groupings, visitor);
    }
    visitor->endVisit(this);
}

int WhereClauseAST::firstToken() const
{
    if ( t_where )
        return t_where;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int WhereClauseAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_where )
        return t_where + 1;
    return 1;
}

bool WhereClauseAST::isNull() const
{
    if ( t_where )
        return false;
    if ( expr )
        return false;
    return true;
}

void WhereClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int FromClauseAST::firstToken() const
{
    if ( t_from )
        return t_from;
    if ( refs )
        return refs->firstToken();
    return 0;
}

int FromClauseAST::lastToken() const
{
    if ( refs )
        return refs->lastToken();
    if ( t_from )
        return t_from + 1;
    return 1;
}

bool FromClauseAST::isNull() const
{
    if ( t_from )
        return false;
    if ( refs )
        return false;
    return true;
}

void FromClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(refs, visitor);
    }
    visitor->endVisit(this);
}

int JoinedTableRefAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( joined && !joined->isNull() )
        return joined->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int JoinedTableRefAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( joined && !joined->isNull() )
        return joined->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool JoinedTableRefAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( joined )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void JoinedTableRefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(joined, visitor);
    }
    visitor->endVisit(this);
}

int CommonJoinedTableAST::firstToken() const
{
    if ( lTableRef && !lTableRef->isNull() )
        return lTableRef->firstToken();
    if ( t_natural )
        return t_natural;
    if ( joinType && !joinType->isNull() )
        return joinType->firstToken();
    if ( t_join )
        return t_join;
    if ( rTableRef && !rTableRef->isNull() )
        return rTableRef->firstToken();
    if ( joinCondition && !joinCondition->isNull() )
        return joinCondition->firstToken();
    return 0;
}

int CommonJoinedTableAST::lastToken() const
{
    if ( joinCondition && !joinCondition->isNull() )
        return joinCondition->lastToken();
    if ( rTableRef && !rTableRef->isNull() )
        return rTableRef->lastToken();
    if ( t_join )
        return t_join + 1;
    if ( joinType && !joinType->isNull() )
        return joinType->lastToken();
    if ( t_natural )
        return t_natural + 1;
    if ( lTableRef && !lTableRef->isNull() )
        return lTableRef->lastToken();
    return 1;
}

bool CommonJoinedTableAST::isNull() const
{
    if ( lTableRef )
        return false;
    if ( t_natural )
        return false;
    if ( joinType )
        return false;
    if ( t_join )
        return false;
    if ( rTableRef )
        return false;
    if ( joinCondition )
        return false;
    return true;
}

void CommonJoinedTableAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lTableRef, visitor);
        accept(joinType, visitor);
        accept(rTableRef, visitor);
        accept(joinCondition, visitor);
    }
    visitor->endVisit(this);
}

int JoinTypeAST::firstToken() const
{
    if ( t_full_or_left_or_right )
        return t_full_or_left_or_right;
    if ( t_inner_or_outer )
        return t_inner_or_outer;
    return 0;
}

int JoinTypeAST::lastToken() const
{
    if ( t_inner_or_outer )
        return t_inner_or_outer + 1;
    if ( t_full_or_left_or_right )
        return t_full_or_left_or_right + 1;
    return 1;
}

bool JoinTypeAST::isNull() const
{
    if ( t_full_or_left_or_right )
        return false;
    if ( t_inner_or_outer )
        return false;
    return true;
}

void JoinTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int OnJoinConditionAST::firstToken() const
{
    if ( t_on )
        return t_on;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int OnJoinConditionAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_on )
        return t_on + 1;
    return 1;
}

bool OnJoinConditionAST::isNull() const
{
    if ( t_on )
        return false;
    if ( expr )
        return false;
    return true;
}

void OnJoinConditionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int UsingJoinConditionAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int UsingJoinConditionAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool UsingJoinConditionAST::isNull() const
{
    if ( t_using )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void UsingJoinConditionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int CrossJoinedTableAST::firstToken() const
{
    if ( lTableRef && !lTableRef->isNull() )
        return lTableRef->firstToken();
    if ( t_cross )
        return t_cross;
    if ( t_join )
        return t_join;
    if ( rTableRef && !rTableRef->isNull() )
        return rTableRef->firstToken();
    return 0;
}

int CrossJoinedTableAST::lastToken() const
{
    if ( rTableRef && !rTableRef->isNull() )
        return rTableRef->lastToken();
    if ( t_join )
        return t_join + 1;
    if ( t_cross )
        return t_cross + 1;
    if ( lTableRef && !lTableRef->isNull() )
        return lTableRef->lastToken();
    return 1;
}

bool CrossJoinedTableAST::isNull() const
{
    if ( lTableRef )
        return false;
    if ( t_cross )
        return false;
    if ( t_join )
        return false;
    if ( rTableRef )
        return false;
    return true;
}

void CrossJoinedTableAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lTableRef, visitor);
        accept(rTableRef, visitor);
    }
    visitor->endVisit(this);
}

int CompoundJoinedTableAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( joinedTable && !joinedTable->isNull() )
        return joinedTable->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CompoundJoinedTableAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( joinedTable && !joinedTable->isNull() )
        return joinedTable->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool CompoundJoinedTableAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( joinedTable )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CompoundJoinedTableAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(joinedTable, visitor);
    }
    visitor->endVisit(this);
}

int CompoundTableRefAST::firstToken() const
{
    if ( t_lateral )
        return t_lateral;
    if ( query && !query->isNull() )
        return query->firstToken();
    if ( tableAlias && !tableAlias->isNull() )
        return tableAlias->firstToken();
    return 0;
}

int CompoundTableRefAST::lastToken() const
{
    if ( tableAlias && !tableAlias->isNull() )
        return tableAlias->lastToken();
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_lateral )
        return t_lateral + 1;
    return 1;
}

bool CompoundTableRefAST::isNull() const
{
    if ( t_lateral )
        return false;
    if ( query )
        return false;
    if ( tableAlias )
        return false;
    return true;
}

void CompoundTableRefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(query, visitor);
        accept(tableAlias, visitor);
    }
    visitor->endVisit(this);
}

int FuncionTableRefAST::firstToken() const
{
    if ( t_lateral )
        return t_lateral;
    if ( func && !func->isNull() )
        return func->firstToken();
    if ( alias && !alias->isNull() )
        return alias->firstToken();
    return 0;
}

int FuncionTableRefAST::lastToken() const
{
    if ( alias && !alias->isNull() )
        return alias->lastToken();
    if ( func && !func->isNull() )
        return func->lastToken();
    if ( t_lateral )
        return t_lateral + 1;
    return 1;
}

bool FuncionTableRefAST::isNull() const
{
    if ( t_lateral )
        return false;
    if ( func )
        return false;
    if ( alias )
        return false;
    return true;
}

void FuncionTableRefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(func, visitor);
        accept(alias, visitor);
    }
    visitor->endVisit(this);
}

int FuncFuncAliasClauseAST::firstToken() const
{
    if ( t_as )
        return t_as;
    if ( aliasName && !aliasName->isNull() )
        return aliasName->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( elements )
        return elements->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int FuncFuncAliasClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( elements )
        return elements->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( aliasName && !aliasName->isNull() )
        return aliasName->lastToken();
    if ( t_as )
        return t_as + 1;
    return 1;
}

bool FuncFuncAliasClauseAST::isNull() const
{
    if ( t_as )
        return false;
    if ( aliasName )
        return false;
    if ( t_lparen )
        return false;
    if ( elements )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void FuncFuncAliasClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(aliasName, visitor);
        accept(elements, visitor);
    }
    visitor->endVisit(this);
}

int TableFuncAliasClauseAST::firstToken() const
{
    if ( tableAlias && !tableAlias->isNull() )
        return tableAlias->firstToken();
    return 0;
}

int TableFuncAliasClauseAST::lastToken() const
{
    if ( tableAlias && !tableAlias->isNull() )
        return tableAlias->lastToken();
    return 1;
}

bool TableFuncAliasClauseAST::isNull() const
{
    if ( tableAlias )
        return false;
    return true;
}

void TableFuncAliasClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableAlias, visitor);
    }
    visitor->endVisit(this);
}

int TableFuncElementAST::firstToken() const
{
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( columnType && !columnType->isNull() )
        return columnType->firstToken();
    if ( collate && !collate->isNull() )
        return collate->firstToken();
    return 0;
}

int TableFuncElementAST::lastToken() const
{
    if ( collate && !collate->isNull() )
        return collate->lastToken();
    if ( columnType && !columnType->isNull() )
        return columnType->lastToken();
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    return 1;
}

bool TableFuncElementAST::isNull() const
{
    if ( columnName )
        return false;
    if ( columnType )
        return false;
    if ( collate )
        return false;
    return true;
}

void TableFuncElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(columnType, visitor);
        accept(collate, visitor);
    }
    visitor->endVisit(this);
}

int CollateClauseAST::firstToken() const
{
    if ( t_collate )
        return t_collate;
    if ( collateName && !collateName->isNull() )
        return collateName->firstToken();
    return 0;
}

int CollateClauseAST::lastToken() const
{
    if ( collateName && !collateName->isNull() )
        return collateName->lastToken();
    if ( t_collate )
        return t_collate + 1;
    return 1;
}

bool CollateClauseAST::isNull() const
{
    if ( t_collate )
        return false;
    if ( collateName )
        return false;
    return true;
}

void CollateClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collateName, visitor);
    }
    visitor->endVisit(this);
}

int XmlserializeFunctionExprAST::firstToken() const
{
    if ( t_xmlserialize )
        return t_xmlserialize;
    if ( t_lparen )
        return t_lparen;
    if ( t_document_or_content )
        return t_document_or_content;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_as )
        return t_as;
    if ( type && !type->isNull() )
        return type->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlserializeFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( type && !type->isNull() )
        return type->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_document_or_content )
        return t_document_or_content + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlserialize )
        return t_xmlserialize + 1;
    return 1;
}

bool XmlserializeFunctionExprAST::isNull() const
{
    if ( t_xmlserialize )
        return false;
    if ( t_lparen )
        return false;
    if ( t_document_or_content )
        return false;
    if ( expr )
        return false;
    if ( t_as )
        return false;
    if ( type )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlserializeFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

int XmlrootFunctionExprAST::firstToken() const
{
    if ( t_xmlroot )
        return t_xmlroot;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_comma )
        return t_comma;
    if ( version && !version->isNull() )
        return version->firstToken();
    if ( standalone && !standalone->isNull() )
        return standalone->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlrootFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( standalone && !standalone->isNull() )
        return standalone->lastToken();
    if ( version && !version->isNull() )
        return version->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlroot )
        return t_xmlroot + 1;
    return 1;
}

bool XmlrootFunctionExprAST::isNull() const
{
    if ( t_xmlroot )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_comma )
        return false;
    if ( version )
        return false;
    if ( standalone )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlrootFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(version, visitor);
        accept(standalone, visitor);
    }
    visitor->endVisit(this);
}

int ExprXmlRootVersionAST::firstToken() const
{
    if ( t_version )
        return t_version;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int ExprXmlRootVersionAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_version )
        return t_version + 1;
    return 1;
}

bool ExprXmlRootVersionAST::isNull() const
{
    if ( t_version )
        return false;
    if ( expr )
        return false;
    return true;
}

void ExprXmlRootVersionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int NoValueXmlRootVersionAST::firstToken() const
{
    if ( t_version )
        return t_version;
    if ( t_no )
        return t_no;
    if ( t_value )
        return t_value;
    return 0;
}

int NoValueXmlRootVersionAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_no )
        return t_no + 1;
    if ( t_version )
        return t_version + 1;
    return 1;
}

bool NoValueXmlRootVersionAST::isNull() const
{
    if ( t_version )
        return false;
    if ( t_no )
        return false;
    if ( t_value )
        return false;
    return true;
}

void NoValueXmlRootVersionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int XmlRootStandaloneAST::firstToken() const
{
    if ( t_comma )
        return t_comma;
    if ( t_standalone )
        return t_standalone;
    if ( t_yes_or_no )
        return t_yes_or_no;
    if ( t_value )
        return t_value;
    return 0;
}

int XmlRootStandaloneAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_yes_or_no )
        return t_yes_or_no + 1;
    if ( t_standalone )
        return t_standalone + 1;
    if ( t_comma )
        return t_comma + 1;
    return 1;
}

bool XmlRootStandaloneAST::isNull() const
{
    if ( t_comma )
        return false;
    if ( t_standalone )
        return false;
    if ( t_yes_or_no )
        return false;
    if ( t_value )
        return false;
    return true;
}

void XmlRootStandaloneAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int XmlpiFunctionExprAST::firstToken() const
{
    if ( t_xmlpi )
        return t_xmlpi;
    if ( t_lparen )
        return t_lparen;
    if ( t_name )
        return t_name;
    if ( label && !label->isNull() )
        return label->firstToken();
    if ( t_comma )
        return t_comma;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlpiFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( label && !label->isNull() )
        return label->lastToken();
    if ( t_name )
        return t_name + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlpi )
        return t_xmlpi + 1;
    return 1;
}

bool XmlpiFunctionExprAST::isNull() const
{
    if ( t_xmlpi )
        return false;
    if ( t_lparen )
        return false;
    if ( t_name )
        return false;
    if ( label )
        return false;
    if ( t_comma )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlpiFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(label, visitor);
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int XmlparseFunctionExprAST::firstToken() const
{
    if ( t_xmlparse )
        return t_xmlparse;
    if ( t_lparen )
        return t_lparen;
    if ( t_document_or_content )
        return t_document_or_content;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( wsOpt && !wsOpt->isNull() )
        return wsOpt->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlparseFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( wsOpt && !wsOpt->isNull() )
        return wsOpt->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_document_or_content )
        return t_document_or_content + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlparse )
        return t_xmlparse + 1;
    return 1;
}

bool XmlparseFunctionExprAST::isNull() const
{
    if ( t_xmlparse )
        return false;
    if ( t_lparen )
        return false;
    if ( t_document_or_content )
        return false;
    if ( expr )
        return false;
    if ( wsOpt )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlparseFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(wsOpt, visitor);
    }
    visitor->endVisit(this);
}

int XmlWhitespaceOptionAST::firstToken() const
{
    if ( t_preserve_or_strip )
        return t_preserve_or_strip;
    if ( t_whitespace )
        return t_whitespace;
    return 0;
}

int XmlWhitespaceOptionAST::lastToken() const
{
    if ( t_whitespace )
        return t_whitespace + 1;
    if ( t_preserve_or_strip )
        return t_preserve_or_strip + 1;
    return 1;
}

bool XmlWhitespaceOptionAST::isNull() const
{
    if ( t_preserve_or_strip )
        return false;
    if ( t_whitespace )
        return false;
    return true;
}

void XmlWhitespaceOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int XmlforestFunctionExprAST::firstToken() const
{
    if ( t_xmlforest )
        return t_xmlforest;
    if ( t_lparen )
        return t_lparen;
    if ( attrs )
        return attrs->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlforestFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( attrs )
        return attrs->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlforest )
        return t_xmlforest + 1;
    return 1;
}

bool XmlforestFunctionExprAST::isNull() const
{
    if ( t_xmlforest )
        return false;
    if ( t_lparen )
        return false;
    if ( attrs )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlforestFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int XmlexistsFunctionExprAST::firstToken() const
{
    if ( t_xmlexists )
        return t_xmlexists;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlexistsFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlexists )
        return t_xmlexists + 1;
    return 1;
}

bool XmlexistsFunctionExprAST::isNull() const
{
    if ( t_xmlexists )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( arg )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlexistsFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(arg, visitor);
    }
    visitor->endVisit(this);
}

int XmlExistsArgumentAST::firstToken() const
{
    if ( t_passing )
        return t_passing;
    if ( t_by1 )
        return t_by1;
    if ( t_ref1 )
        return t_ref1;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_by2 )
        return t_by2;
    if ( t_ref2 )
        return t_ref2;
    return 0;
}

int XmlExistsArgumentAST::lastToken() const
{
    if ( t_ref2 )
        return t_ref2 + 1;
    if ( t_by2 )
        return t_by2 + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_ref1 )
        return t_ref1 + 1;
    if ( t_by1 )
        return t_by1 + 1;
    if ( t_passing )
        return t_passing + 1;
    return 1;
}

bool XmlExistsArgumentAST::isNull() const
{
    if ( t_passing )
        return false;
    if ( t_by1 )
        return false;
    if ( t_ref1 )
        return false;
    if ( expr )
        return false;
    if ( t_by2 )
        return false;
    if ( t_ref2 )
        return false;
    return true;
}

void XmlExistsArgumentAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int XmlelementFunctionExprAST::firstToken() const
{
    if ( t_xmlelement )
        return t_xmlelement;
    if ( t_lparen )
        return t_lparen;
    if ( t_name )
        return t_name;
    if ( elementName && !elementName->isNull() )
        return elementName->firstToken();
    if ( t_comma_attr )
        return t_comma_attr;
    if ( attributes && !attributes->isNull() )
        return attributes->firstToken();
    if ( t_comma )
        return t_comma;
    if ( values )
        return values->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlelementFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( values )
        return values->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( attributes && !attributes->isNull() )
        return attributes->lastToken();
    if ( t_comma_attr )
        return t_comma_attr + 1;
    if ( elementName && !elementName->isNull() )
        return elementName->lastToken();
    if ( t_name )
        return t_name + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlelement )
        return t_xmlelement + 1;
    return 1;
}

bool XmlelementFunctionExprAST::isNull() const
{
    if ( t_xmlelement )
        return false;
    if ( t_lparen )
        return false;
    if ( t_name )
        return false;
    if ( elementName )
        return false;
    if ( t_comma_attr )
        return false;
    if ( attributes )
        return false;
    if ( t_comma )
        return false;
    if ( values )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlelementFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(elementName, visitor);
        accept(attributes, visitor);
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int XmlAttributesAST::firstToken() const
{
    if ( t_xmlattributes )
        return t_xmlattributes;
    if ( t_lparen )
        return t_lparen;
    if ( attrs )
        return attrs->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlAttributesAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( attrs )
        return attrs->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_xmlattributes )
        return t_xmlattributes + 1;
    return 1;
}

bool XmlAttributesAST::isNull() const
{
    if ( t_xmlattributes )
        return false;
    if ( t_lparen )
        return false;
    if ( attrs )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlAttributesAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int XmlAttributeAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_as )
        return t_as;
    if ( alias && !alias->isNull() )
        return alias->firstToken();
    return 0;
}

int XmlAttributeAST::lastToken() const
{
    if ( alias && !alias->isNull() )
        return alias->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool XmlAttributeAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_as )
        return false;
    if ( alias )
        return false;
    return true;
}

void XmlAttributeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(alias, visitor);
    }
    visitor->endVisit(this);
}

int CoalesceFunctionExprAST::firstToken() const
{
    if ( t_coalesce )
        return t_coalesce;
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CoalesceFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_coalesce )
        return t_coalesce + 1;
    return 1;
}

bool CoalesceFunctionExprAST::isNull() const
{
    if ( t_coalesce )
        return false;
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CoalesceFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int GreatestFunctionExprAST::firstToken() const
{
    if ( t_coalesce )
        return t_coalesce;
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int GreatestFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_coalesce )
        return t_coalesce + 1;
    return 1;
}

bool GreatestFunctionExprAST::isNull() const
{
    if ( t_coalesce )
        return false;
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void GreatestFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int LeastFunctionExprAST::firstToken() const
{
    if ( t_coalesce )
        return t_coalesce;
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int LeastFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_coalesce )
        return t_coalesce + 1;
    return 1;
}

bool LeastFunctionExprAST::isNull() const
{
    if ( t_coalesce )
        return false;
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void LeastFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int XmlconcatFunctionExprAST::firstToken() const
{
    if ( t_coalesce )
        return t_coalesce;
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int XmlconcatFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_coalesce )
        return t_coalesce + 1;
    return 1;
}

bool XmlconcatFunctionExprAST::isNull() const
{
    if ( t_coalesce )
        return false;
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void XmlconcatFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int NullifFunctionExprAST::firstToken() const
{
    if ( t_nullif )
        return t_nullif;
    if ( t_lparen )
        return t_lparen;
    if ( expr1 && !expr1->isNull() )
        return expr1->firstToken();
    if ( t_comma )
        return t_comma;
    if ( expr2 && !expr2->isNull() )
        return expr2->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int NullifFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr2 && !expr2->isNull() )
        return expr2->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( expr1 && !expr1->isNull() )
        return expr1->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_nullif )
        return t_nullif + 1;
    return 1;
}

bool NullifFunctionExprAST::isNull() const
{
    if ( t_nullif )
        return false;
    if ( t_lparen )
        return false;
    if ( expr1 )
        return false;
    if ( t_comma )
        return false;
    if ( expr2 )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void NullifFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr1, visitor);
        accept(expr2, visitor);
    }
    visitor->endVisit(this);
}

int TrimFunctionExprAST::firstToken() const
{
    if ( t_trim )
        return t_trim;
    if ( t_lparen )
        return t_lparen;
    if ( t_trim_spec )
        return t_trim_spec;
    if ( trimExpr && !trimExpr->isNull() )
        return trimExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TrimFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( trimExpr && !trimExpr->isNull() )
        return trimExpr->lastToken();
    if ( t_trim_spec )
        return t_trim_spec + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_trim )
        return t_trim + 1;
    return 1;
}

bool TrimFunctionExprAST::isNull() const
{
    if ( t_trim )
        return false;
    if ( t_lparen )
        return false;
    if ( t_trim_spec )
        return false;
    if ( trimExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TrimFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(trimExpr, visitor);
    }
    visitor->endVisit(this);
}

int TrimExprAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_from )
        return t_from;
    if ( fromExpressions )
        return fromExpressions->firstToken();
    return 0;
}

int TrimExprAST::lastToken() const
{
    if ( fromExpressions )
        return fromExpressions->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool TrimExprAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_from )
        return false;
    if ( fromExpressions )
        return false;
    return true;
}

void TrimExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(fromExpressions, visitor);
    }
    visitor->endVisit(this);
}

int TraetFunctionExprAST::firstToken() const
{
    if ( t_traet )
        return t_traet;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_as )
        return t_as;
    if ( traetType && !traetType->isNull() )
        return traetType->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TraetFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( traetType && !traetType->isNull() )
        return traetType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_traet )
        return t_traet + 1;
    return 1;
}

bool TraetFunctionExprAST::isNull() const
{
    if ( t_traet )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_as )
        return false;
    if ( traetType )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TraetFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(traetType, visitor);
    }
    visitor->endVisit(this);
}

int SubstringFunctionExprAST::firstToken() const
{
    if ( t_substring )
        return t_substring;
    if ( t_lparen )
        return t_lparen;
    if ( substringExpr && !substringExpr->isNull() )
        return substringExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int SubstringFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( substringExpr && !substringExpr->isNull() )
        return substringExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_substring )
        return t_substring + 1;
    return 1;
}

bool SubstringFunctionExprAST::isNull() const
{
    if ( t_substring )
        return false;
    if ( t_lparen )
        return false;
    if ( substringExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void SubstringFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(substringExpr, visitor);
    }
    visitor->endVisit(this);
}

int StandardSubstringExprAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( substrFrom && !substrFrom->isNull() )
        return substrFrom->firstToken();
    if ( substrFor && !substrFor->isNull() )
        return substrFor->firstToken();
    return 0;
}

int StandardSubstringExprAST::lastToken() const
{
    if ( substrFor && !substrFor->isNull() )
        return substrFor->lastToken();
    if ( substrFrom && !substrFrom->isNull() )
        return substrFrom->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool StandardSubstringExprAST::isNull() const
{
    if ( expr )
        return false;
    if ( substrFrom )
        return false;
    if ( substrFor )
        return false;
    return true;
}

void StandardSubstringExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(substrFrom, visitor);
        accept(substrFor, visitor);
    }
    visitor->endVisit(this);
}

int ListSubstringExprAST::firstToken() const
{
    if ( args )
        return args->firstToken();
    return 0;
}

int ListSubstringExprAST::lastToken() const
{
    if ( args )
        return args->lastToken();
    return 1;
}

bool ListSubstringExprAST::isNull() const
{
    if ( args )
        return false;
    return true;
}

void ListSubstringExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int PositionFunctionExprAST::firstToken() const
{
    if ( t_position )
        return t_position;
    if ( t_lparen )
        return t_lparen;
    if ( positionExpr && !positionExpr->isNull() )
        return positionExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int PositionFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( positionExpr && !positionExpr->isNull() )
        return positionExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_position )
        return t_position + 1;
    return 1;
}

bool PositionFunctionExprAST::isNull() const
{
    if ( t_position )
        return false;
    if ( t_lparen )
        return false;
    if ( positionExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void PositionFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(positionExpr, visitor);
    }
    visitor->endVisit(this);
}

int OverlayFunctionExprAST::firstToken() const
{
    if ( t_overlay )
        return t_overlay;
    if ( t_lparen )
        return t_lparen;
    if ( overlayExpr && !overlayExpr->isNull() )
        return overlayExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int OverlayFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( overlayExpr && !overlayExpr->isNull() )
        return overlayExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_overlay )
        return t_overlay + 1;
    return 1;
}

bool OverlayFunctionExprAST::isNull() const
{
    if ( t_overlay )
        return false;
    if ( t_lparen )
        return false;
    if ( overlayExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void OverlayFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(overlayExpr, visitor);
    }
    visitor->endVisit(this);
}

int PositionExprAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_in )
        return t_in;
    if ( inExpr && !inExpr->isNull() )
        return inExpr->firstToken();
    return 0;
}

int PositionExprAST::lastToken() const
{
    if ( inExpr && !inExpr->isNull() )
        return inExpr->lastToken();
    if ( t_in )
        return t_in + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool PositionExprAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_in )
        return false;
    if ( inExpr )
        return false;
    return true;
}

void PositionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(inExpr, visitor);
    }
    visitor->endVisit(this);
}

int OverlayExprAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( placing && !placing->isNull() )
        return placing->firstToken();
    if ( substrFrom && !substrFrom->isNull() )
        return substrFrom->firstToken();
    if ( substrFor && !substrFor->isNull() )
        return substrFor->firstToken();
    return 0;
}

int OverlayExprAST::lastToken() const
{
    if ( substrFor && !substrFor->isNull() )
        return substrFor->lastToken();
    if ( substrFrom && !substrFrom->isNull() )
        return substrFrom->lastToken();
    if ( placing && !placing->isNull() )
        return placing->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool OverlayExprAST::isNull() const
{
    if ( expr )
        return false;
    if ( placing )
        return false;
    if ( substrFrom )
        return false;
    if ( substrFor )
        return false;
    return true;
}

void OverlayExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(placing, visitor);
        accept(substrFrom, visitor);
        accept(substrFor, visitor);
    }
    visitor->endVisit(this);
}

int OverlayPlacingAST::firstToken() const
{
    if ( t_placing )
        return t_placing;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int OverlayPlacingAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_placing )
        return t_placing + 1;
    return 1;
}

bool OverlayPlacingAST::isNull() const
{
    if ( t_placing )
        return false;
    if ( expr )
        return false;
    return true;
}

void OverlayPlacingAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int ExtractFunctionExprAST::firstToken() const
{
    if ( t_extract )
        return t_extract;
    if ( t_lparen )
        return t_lparen;
    if ( extractExpr && !extractExpr->isNull() )
        return extractExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int ExtractFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( extractExpr && !extractExpr->isNull() )
        return extractExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_extract )
        return t_extract + 1;
    return 1;
}

bool ExtractFunctionExprAST::isNull() const
{
    if ( t_extract )
        return false;
    if ( t_lparen )
        return false;
    if ( extractExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void ExtractFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(extractExpr, visitor);
    }
    visitor->endVisit(this);
}

int SubstringFromAST::firstToken() const
{
    if ( t_from )
        return t_from;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int SubstringFromAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_from )
        return t_from + 1;
    return 1;
}

bool SubstringFromAST::isNull() const
{
    if ( t_from )
        return false;
    if ( expr )
        return false;
    return true;
}

void SubstringFromAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int SubstringForAST::firstToken() const
{
    if ( t_for )
        return t_for;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int SubstringForAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_for )
        return t_for + 1;
    return 1;
}

bool SubstringForAST::isNull() const
{
    if ( t_for )
        return false;
    if ( expr )
        return false;
    return true;
}

void SubstringForAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int ExtractExprAST::firstToken() const
{
    if ( t_value )
        return t_value;
    if ( t_from )
        return t_from;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int ExtractExprAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool ExtractExprAST::isNull() const
{
    if ( t_value )
        return false;
    if ( t_from )
        return false;
    if ( expr )
        return false;
    return true;
}

void ExtractExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CastFunctionExprAST::firstToken() const
{
    if ( t_cast )
        return t_cast;
    if ( t_lparen )
        return t_lparen;
    if ( castExpr && !castExpr->isNull() )
        return castExpr->firstToken();
    if ( t_as )
        return t_as;
    if ( castType && !castType->isNull() )
        return castType->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CastFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( castType && !castType->isNull() )
        return castType->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( castExpr && !castExpr->isNull() )
        return castExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_cast )
        return t_cast + 1;
    return 1;
}

bool CastFunctionExprAST::isNull() const
{
    if ( t_cast )
        return false;
    if ( t_lparen )
        return false;
    if ( castExpr )
        return false;
    if ( t_as )
        return false;
    if ( castType )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CastFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(castExpr, visitor);
        accept(castType, visitor);
    }
    visitor->endVisit(this);
}

int CollationFunctionExprAST::firstToken() const
{
    if ( t_collation )
        return t_collation;
    if ( t_for )
        return t_for;
    if ( t_lparen )
        return t_lparen;
    if ( collationExpr && !collationExpr->isNull() )
        return collationExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CollationFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( collationExpr && !collationExpr->isNull() )
        return collationExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_for )
        return t_for + 1;
    if ( t_collation )
        return t_collation + 1;
    return 1;
}

bool CollationFunctionExprAST::isNull() const
{
    if ( t_collation )
        return false;
    if ( t_for )
        return false;
    if ( t_lparen )
        return false;
    if ( collationExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CollationFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collationExpr, visitor);
    }
    visitor->endVisit(this);
}

int SimpleFunctionExprAST::firstToken() const
{
    if ( t_builtin_funcname )
        return t_builtin_funcname;
    if ( t_lparen )
        return t_lparen;
    if ( t_value )
        return t_value;
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int SimpleFunctionExprAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_value )
        return t_value + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_builtin_funcname )
        return t_builtin_funcname + 1;
    return 1;
}

bool SimpleFunctionExprAST::isNull() const
{
    if ( t_builtin_funcname )
        return false;
    if ( t_lparen )
        return false;
    if ( t_value )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void SimpleFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommonFunctionExprAST::firstToken() const
{
    if ( funcName && !funcName->isNull() )
        return funcName->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( t_all_or_distinct_or_star )
        return t_all_or_distinct_or_star;
    if ( arguments )
        return arguments->firstToken();
    if ( sortClause && !sortClause->isNull() )
        return sortClause->firstToken();
    if ( t_comma )
        return t_comma;
    if ( t_variadic )
        return t_variadic;
    if ( variadicArg && !variadicArg->isNull() )
        return variadicArg->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( overClause && !overClause->isNull() )
        return overClause->firstToken();
    return 0;
}

int CommonFunctionExprAST::lastToken() const
{
    if ( overClause && !overClause->isNull() )
        return overClause->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( variadicArg && !variadicArg->isNull() )
        return variadicArg->lastToken();
    if ( t_variadic )
        return t_variadic + 1;
    if ( t_comma )
        return t_comma + 1;
    if ( sortClause && !sortClause->isNull() )
        return sortClause->lastToken();
    if ( arguments )
        return arguments->lastToken();
    if ( t_all_or_distinct_or_star )
        return t_all_or_distinct_or_star + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( funcName && !funcName->isNull() )
        return funcName->lastToken();
    return 1;
}

bool CommonFunctionExprAST::isNull() const
{
    if ( funcName )
        return false;
    if ( t_lparen )
        return false;
    if ( t_all_or_distinct_or_star )
        return false;
    if ( arguments )
        return false;
    if ( sortClause )
        return false;
    if ( t_comma )
        return false;
    if ( t_variadic )
        return false;
    if ( variadicArg )
        return false;
    if ( t_rparen )
        return false;
    if ( overClause )
        return false;
    return true;
}

void CommonFunctionExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcName, visitor);
        accept(arguments, visitor);
        accept(sortClause, visitor);
        accept(variadicArg, visitor);
        accept(overClause, visitor);
    }
    visitor->endVisit(this);
}

int FunctionArgExprAST::firstToken() const
{
    if ( paramName && !paramName->isNull() )
        return paramName->firstToken();
    if ( t_assign )
        return t_assign;
    if ( argExpr && !argExpr->isNull() )
        return argExpr->firstToken();
    return 0;
}

int FunctionArgExprAST::lastToken() const
{
    if ( argExpr && !argExpr->isNull() )
        return argExpr->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( paramName && !paramName->isNull() )
        return paramName->lastToken();
    return 1;
}

bool FunctionArgExprAST::isNull() const
{
    if ( paramName )
        return false;
    if ( t_assign )
        return false;
    if ( argExpr )
        return false;
    return true;
}

void FunctionArgExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(paramName, visitor);
        accept(argExpr, visitor);
    }
    visitor->endVisit(this);
}

int SimpleOverClauseAST::firstToken() const
{
    if ( t_over )
        return t_over;
    if ( column && !column->isNull() )
        return column->firstToken();
    return 0;
}

int SimpleOverClauseAST::lastToken() const
{
    if ( column && !column->isNull() )
        return column->lastToken();
    if ( t_over )
        return t_over + 1;
    return 1;
}

bool SimpleOverClauseAST::isNull() const
{
    if ( t_over )
        return false;
    if ( column )
        return false;
    return true;
}

void SimpleOverClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(column, visitor);
    }
    visitor->endVisit(this);
}

int WindowOverClauseAST::firstToken() const
{
    if ( t_over )
        return t_over;
    if ( windowSpec && !windowSpec->isNull() )
        return windowSpec->firstToken();
    return 0;
}

int WindowOverClauseAST::lastToken() const
{
    if ( windowSpec && !windowSpec->isNull() )
        return windowSpec->lastToken();
    if ( t_over )
        return t_over + 1;
    return 1;
}

bool WindowOverClauseAST::isNull() const
{
    if ( t_over )
        return false;
    if ( windowSpec )
        return false;
    return true;
}

void WindowOverClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(windowSpec, visitor);
    }
    visitor->endVisit(this);
}

int WindowSpecificationAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( windowName && !windowName->isNull() )
        return windowName->firstToken();
    if ( partitionClause && !partitionClause->isNull() )
        return partitionClause->firstToken();
    if ( sortClause && !sortClause->isNull() )
        return sortClause->firstToken();
    if ( frameClause && !frameClause->isNull() )
        return frameClause->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int WindowSpecificationAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( frameClause && !frameClause->isNull() )
        return frameClause->lastToken();
    if ( sortClause && !sortClause->isNull() )
        return sortClause->lastToken();
    if ( partitionClause && !partitionClause->isNull() )
        return partitionClause->lastToken();
    if ( windowName && !windowName->isNull() )
        return windowName->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool WindowSpecificationAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( windowName )
        return false;
    if ( partitionClause )
        return false;
    if ( sortClause )
        return false;
    if ( frameClause )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void WindowSpecificationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(windowName, visitor);
        accept(partitionClause, visitor);
        accept(sortClause, visitor);
        accept(frameClause, visitor);
    }
    visitor->endVisit(this);
}

int PartitionClauseAST::firstToken() const
{
    if ( t_partition )
        return t_partition;
    if ( t_by )
        return t_by;
    if ( params )
        return params->firstToken();
    return 0;
}

int PartitionClauseAST::lastToken() const
{
    if ( params )
        return params->lastToken();
    if ( t_by )
        return t_by + 1;
    if ( t_partition )
        return t_partition + 1;
    return 1;
}

bool PartitionClauseAST::isNull() const
{
    if ( t_partition )
        return false;
    if ( t_by )
        return false;
    if ( params )
        return false;
    return true;
}

void PartitionClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(params, visitor);
    }
    visitor->endVisit(this);
}

int FrameClauseAST::firstToken() const
{
    if ( t_range_or_rows )
        return t_range_or_rows;
    if ( extent && !extent->isNull() )
        return extent->firstToken();
    return 0;
}

int FrameClauseAST::lastToken() const
{
    if ( extent && !extent->isNull() )
        return extent->lastToken();
    if ( t_range_or_rows )
        return t_range_or_rows + 1;
    return 1;
}

bool FrameClauseAST::isNull() const
{
    if ( t_range_or_rows )
        return false;
    if ( extent )
        return false;
    return true;
}

void FrameClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(extent, visitor);
    }
    visitor->endVisit(this);
}

int BoundFrameExtentAST::firstToken() const
{
    if ( bound && !bound->isNull() )
        return bound->firstToken();
    return 0;
}

int BoundFrameExtentAST::lastToken() const
{
    if ( bound && !bound->isNull() )
        return bound->lastToken();
    return 1;
}

bool BoundFrameExtentAST::isNull() const
{
    if ( bound )
        return false;
    return true;
}

void BoundFrameExtentAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(bound, visitor);
    }
    visitor->endVisit(this);
}

int BetweenFrameExtentAST::firstToken() const
{
    if ( t_between )
        return t_between;
    if ( lBound && !lBound->isNull() )
        return lBound->firstToken();
    if ( t_and )
        return t_and;
    if ( rBound && !rBound->isNull() )
        return rBound->firstToken();
    return 0;
}

int BetweenFrameExtentAST::lastToken() const
{
    if ( rBound && !rBound->isNull() )
        return rBound->lastToken();
    if ( t_and )
        return t_and + 1;
    if ( lBound && !lBound->isNull() )
        return lBound->lastToken();
    if ( t_between )
        return t_between + 1;
    return 1;
}

bool BetweenFrameExtentAST::isNull() const
{
    if ( t_between )
        return false;
    if ( lBound )
        return false;
    if ( t_and )
        return false;
    if ( rBound )
        return false;
    return true;
}

void BetweenFrameExtentAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lBound, visitor);
        accept(rBound, visitor);
    }
    visitor->endVisit(this);
}

int SimpleFrameBoundAST::firstToken() const
{
    if ( t_unbounded_or_current )
        return t_unbounded_or_current;
    if ( t_prec_or_foll_or_row )
        return t_prec_or_foll_or_row;
    return 0;
}

int SimpleFrameBoundAST::lastToken() const
{
    if ( t_prec_or_foll_or_row )
        return t_prec_or_foll_or_row + 1;
    if ( t_unbounded_or_current )
        return t_unbounded_or_current + 1;
    return 1;
}

bool SimpleFrameBoundAST::isNull() const
{
    if ( t_unbounded_or_current )
        return false;
    if ( t_prec_or_foll_or_row )
        return false;
    return true;
}

void SimpleFrameBoundAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ExprFrameBoundAST::firstToken() const
{
    if ( boundExpr && !boundExpr->isNull() )
        return boundExpr->firstToken();
    if ( t_prec_or_foll )
        return t_prec_or_foll;
    return 0;
}

int ExprFrameBoundAST::lastToken() const
{
    if ( t_prec_or_foll )
        return t_prec_or_foll + 1;
    if ( boundExpr && !boundExpr->isNull() )
        return boundExpr->lastToken();
    return 1;
}

bool ExprFrameBoundAST::isNull() const
{
    if ( boundExpr )
        return false;
    if ( t_prec_or_foll )
        return false;
    return true;
}

void ExprFrameBoundAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(boundExpr, visitor);
    }
    visitor->endVisit(this);
}

int DirectTableRefAST::firstToken() const
{
    if ( relation && !relation->isNull() )
        return relation->firstToken();
    if ( alias && !alias->isNull() )
        return alias->firstToken();
    return 0;
}

int DirectTableRefAST::lastToken() const
{
    if ( alias && !alias->isNull() )
        return alias->lastToken();
    if ( relation && !relation->isNull() )
        return relation->lastToken();
    return 1;
}

bool DirectTableRefAST::isNull() const
{
    if ( relation )
        return false;
    if ( alias )
        return false;
    return true;
}

void DirectTableRefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relation, visitor);
        accept(alias, visitor);
    }
    visitor->endVisit(this);
}

int TableAliasClauseAST::firstToken() const
{
    if ( t_as )
        return t_as;
    if ( aliasName && !aliasName->isNull() )
        return aliasName->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TableAliasClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( aliasName && !aliasName->isNull() )
        return aliasName->lastToken();
    if ( t_as )
        return t_as + 1;
    return 1;
}

bool TableAliasClauseAST::isNull() const
{
    if ( t_as )
        return false;
    if ( aliasName )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TableAliasClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(aliasName, visitor);
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int SimpleDistinctClauseAST::firstToken() const
{
    if ( t_all_or_distinct )
        return t_all_or_distinct;
    return 0;
}

int SimpleDistinctClauseAST::lastToken() const
{
    if ( t_all_or_distinct )
        return t_all_or_distinct + 1;
    return 1;
}

bool SimpleDistinctClauseAST::isNull() const
{
    if ( t_all_or_distinct )
        return false;
    return true;
}

void SimpleDistinctClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ExprListDistinctClauseAST::firstToken() const
{
    if ( t_distinct )
        return t_distinct;
    if ( t_on )
        return t_on;
    if ( t_lparen )
        return t_lparen;
    if ( distinctExpressions )
        return distinctExpressions->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int ExprListDistinctClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( distinctExpressions )
        return distinctExpressions->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_on )
        return t_on + 1;
    if ( t_distinct )
        return t_distinct + 1;
    return 1;
}

bool ExprListDistinctClauseAST::isNull() const
{
    if ( t_distinct )
        return false;
    if ( t_on )
        return false;
    if ( t_lparen )
        return false;
    if ( distinctExpressions )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void ExprListDistinctClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(distinctExpressions, visitor);
    }
    visitor->endVisit(this);
}

int StarSelectTargetAST::firstToken() const
{
    if ( t_star )
        return t_star;
    return 0;
}

int StarSelectTargetAST::lastToken() const
{
    if ( t_star )
        return t_star + 1;
    return 1;
}

bool StarSelectTargetAST::isNull() const
{
    if ( t_star )
        return false;
    return true;
}

void StarSelectTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ExprSelectTargetAST::firstToken() const
{
    if ( selTargetExpr && !selTargetExpr->isNull() )
        return selTargetExpr->firstToken();
    if ( t_as )
        return t_as;
    if ( columnAlias && !columnAlias->isNull() )
        return columnAlias->firstToken();
    return 0;
}

int ExprSelectTargetAST::lastToken() const
{
    if ( columnAlias && !columnAlias->isNull() )
        return columnAlias->lastToken();
    if ( t_as )
        return t_as + 1;
    if ( selTargetExpr && !selTargetExpr->isNull() )
        return selTargetExpr->lastToken();
    return 1;
}

bool ExprSelectTargetAST::isNull() const
{
    if ( selTargetExpr )
        return false;
    if ( t_as )
        return false;
    if ( columnAlias )
        return false;
    return true;
}

void ExprSelectTargetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(selTargetExpr, visitor);
        accept(columnAlias, visitor);
    }
    visitor->endVisit(this);
}

int IntoClauseAST::firstToken() const
{
    if ( t_into )
        return t_into;
    if ( tempTable && !tempTable->isNull() )
        return tempTable->firstToken();
    return 0;
}

int IntoClauseAST::lastToken() const
{
    if ( tempTable && !tempTable->isNull() )
        return tempTable->lastToken();
    if ( t_into )
        return t_into + 1;
    return 1;
}

bool IntoClauseAST::isNull() const
{
    if ( t_into )
        return false;
    if ( tempTable )
        return false;
    return true;
}

void IntoClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tempTable, visitor);
    }
    visitor->endVisit(this);
}

int OptTempTableNameAST::firstToken() const
{
    if ( t_local_or_global_or_unlogged )
        return t_local_or_global_or_unlogged;
    if ( t_temp )
        return t_temp;
    if ( t_table )
        return t_table;
    if ( intoTableIdent && !intoTableIdent->isNull() )
        return intoTableIdent->firstToken();
    return 0;
}

int OptTempTableNameAST::lastToken() const
{
    if ( intoTableIdent && !intoTableIdent->isNull() )
        return intoTableIdent->lastToken();
    if ( t_table )
        return t_table + 1;
    if ( t_temp )
        return t_temp + 1;
    if ( t_local_or_global_or_unlogged )
        return t_local_or_global_or_unlogged + 1;
    return 1;
}

bool OptTempTableNameAST::isNull() const
{
    if ( t_local_or_global_or_unlogged )
        return false;
    if ( t_temp )
        return false;
    if ( t_table )
        return false;
    if ( intoTableIdent )
        return false;
    return true;
}

void OptTempTableNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(intoTableIdent, visitor);
    }
    visitor->endVisit(this);
}

int ValuesClauseAST::firstToken() const
{
    if ( t_values )
        return t_values;
    if ( row && !row->isNull() )
        return row->firstToken();
    return 0;
}

int ValuesClauseAST::lastToken() const
{
    if ( row && !row->isNull() )
        return row->lastToken();
    if ( t_values )
        return t_values + 1;
    return 1;
}

bool ValuesClauseAST::isNull() const
{
    if ( t_values )
        return false;
    if ( row )
        return false;
    return true;
}

void ValuesClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(row, visitor);
    }
    visitor->endVisit(this);
}

int ColumnExprRowAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int ColumnExprRowAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool ColumnExprRowAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void ColumnExprRowAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int DefaultColumnExpressionAST::firstToken() const
{
    if ( t_default )
        return t_default;
    return 0;
}

int DefaultColumnExpressionAST::lastToken() const
{
    if ( t_default )
        return t_default + 1;
    return 1;
}

bool DefaultColumnExpressionAST::isNull() const
{
    if ( t_default )
        return false;
    return true;
}

void DefaultColumnExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int ExprColumnExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int ExprColumnExpressionAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool ExprColumnExpressionAST::isNull() const
{
    if ( expr )
        return false;
    return true;
}

void ExprColumnExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableGenericCmdAST::firstToken() const
{
    if ( t_options )
        return t_options;
    if ( t_lparen )
        return t_lparen;
    if ( options )
        return options->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int AlterTableGenericCmdAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( options )
        return options->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_options )
        return t_options + 1;
    return 1;
}

bool AlterTableGenericCmdAST::isNull() const
{
    if ( t_options )
        return false;
    if ( t_lparen )
        return false;
    if ( options )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void AlterTableGenericCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int SetAddAlterGenericOptionAST::firstToken() const
{
    if ( t_add_or_set )
        return t_add_or_set;
    if ( option && !option->isNull() )
        return option->firstToken();
    return 0;
}

int SetAddAlterGenericOptionAST::lastToken() const
{
    if ( option && !option->isNull() )
        return option->lastToken();
    if ( t_add_or_set )
        return t_add_or_set + 1;
    return 1;
}

bool SetAddAlterGenericOptionAST::isNull() const
{
    if ( t_add_or_set )
        return false;
    if ( option )
        return false;
    return true;
}

void SetAddAlterGenericOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(option, visitor);
    }
    visitor->endVisit(this);
}

int DropAlterGenericOptionAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_opt_name )
        return t_opt_name;
    return 0;
}

int DropAlterGenericOptionAST::lastToken() const
{
    if ( t_opt_name )
        return t_opt_name + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropAlterGenericOptionAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_opt_name )
        return false;
    return true;
}

void DropAlterGenericOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableSetReloptCmdAST::firstToken() const
{
    if ( t_set_or_reset )
        return t_set_or_reset;
    if ( relOptions && !relOptions->isNull() )
        return relOptions->firstToken();
    return 0;
}

int AlterTableSetReloptCmdAST::lastToken() const
{
    if ( relOptions && !relOptions->isNull() )
        return relOptions->lastToken();
    if ( t_set_or_reset )
        return t_set_or_reset + 1;
    return 1;
}

bool AlterTableSetReloptCmdAST::isNull() const
{
    if ( t_set_or_reset )
        return false;
    if ( relOptions )
        return false;
    return true;
}

void AlterTableSetReloptCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(relOptions, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableTablespaceCmdAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    return 0;
}

int AlterTableTablespaceCmdAST::lastToken() const
{
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool AlterTableTablespaceCmdAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    return true;
}

void AlterTableTablespaceCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableOwnerCmdAST::firstToken() const
{
    if ( t_owner )
        return t_owner;
    if ( t_to )
        return t_to;
    if ( roleId && !roleId->isNull() )
        return roleId->firstToken();
    return 0;
}

int AlterTableOwnerCmdAST::lastToken() const
{
    if ( roleId && !roleId->isNull() )
        return roleId->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_owner )
        return t_owner + 1;
    return 1;
}

bool AlterTableOwnerCmdAST::isNull() const
{
    if ( t_owner )
        return false;
    if ( t_to )
        return false;
    if ( roleId )
        return false;
    return true;
}

void AlterTableOwnerCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(roleId, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableNotOfCmdAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_of )
        return t_of;
    return 0;
}

int AlterTableNotOfCmdAST::lastToken() const
{
    if ( t_of )
        return t_of + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool AlterTableNotOfCmdAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_of )
        return false;
    return true;
}

void AlterTableNotOfCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableOfCmdAST::firstToken() const
{
    if ( t_of )
        return t_of;
    if ( ident && !ident->isNull() )
        return ident->firstToken();
    return 0;
}

int AlterTableOfCmdAST::lastToken() const
{
    if ( ident && !ident->isNull() )
        return ident->lastToken();
    if ( t_of )
        return t_of + 1;
    return 1;
}

bool AlterTableOfCmdAST::isNull() const
{
    if ( t_of )
        return false;
    if ( ident )
        return false;
    return true;
}

void AlterTableOfCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ident, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableInheritCmdAST::firstToken() const
{
    if ( t_no )
        return t_no;
    if ( t_inherit )
        return t_inherit;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    return 0;
}

int AlterTableInheritCmdAST::lastToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_inherit )
        return t_inherit + 1;
    if ( t_no )
        return t_no + 1;
    return 1;
}

bool AlterTableInheritCmdAST::isNull() const
{
    if ( t_no )
        return false;
    if ( t_inherit )
        return false;
    if ( tableIdent )
        return false;
    return true;
}

void AlterTableInheritCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableTriggerCmdAST::firstToken() const
{
    if ( t_enable_or_disable )
        return t_enable_or_disable;
    if ( t_always_or_replica )
        return t_always_or_replica;
    if ( t_trigger )
        return t_trigger;
    if ( wildcard && !wildcard->isNull() )
        return wildcard->firstToken();
    return 0;
}

int AlterTableTriggerCmdAST::lastToken() const
{
    if ( wildcard && !wildcard->isNull() )
        return wildcard->lastToken();
    if ( t_trigger )
        return t_trigger + 1;
    if ( t_always_or_replica )
        return t_always_or_replica + 1;
    if ( t_enable_or_disable )
        return t_enable_or_disable + 1;
    return 1;
}

bool AlterTableTriggerCmdAST::isNull() const
{
    if ( t_enable_or_disable )
        return false;
    if ( t_always_or_replica )
        return false;
    if ( t_trigger )
        return false;
    if ( wildcard )
        return false;
    return true;
}

void AlterTableTriggerCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(wildcard, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableRuleCmdAST::firstToken() const
{
    if ( t_enable_or_disable )
        return t_enable_or_disable;
    if ( t_always_or_replica )
        return t_always_or_replica;
    if ( t_rule )
        return t_rule;
    if ( ruleName && !ruleName->isNull() )
        return ruleName->firstToken();
    return 0;
}

int AlterTableRuleCmdAST::lastToken() const
{
    if ( ruleName && !ruleName->isNull() )
        return ruleName->lastToken();
    if ( t_rule )
        return t_rule + 1;
    if ( t_always_or_replica )
        return t_always_or_replica + 1;
    if ( t_enable_or_disable )
        return t_enable_or_disable + 1;
    return 1;
}

bool AlterTableRuleCmdAST::isNull() const
{
    if ( t_enable_or_disable )
        return false;
    if ( t_always_or_replica )
        return false;
    if ( t_rule )
        return false;
    if ( ruleName )
        return false;
    return true;
}

void AlterTableRuleCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(ruleName, visitor);
    }
    visitor->endVisit(this);
}

int NameWildcardAST::firstToken() const
{
    if ( name && !name->isNull() )
        return name->firstToken();
    return 0;
}

int NameWildcardAST::lastToken() const
{
    if ( name && !name->isNull() )
        return name->lastToken();
    return 1;
}

bool NameWildcardAST::isNull() const
{
    if ( name )
        return false;
    return true;
}

void NameWildcardAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(name, visitor);
    }
    visitor->endVisit(this);
}

int AllOrUserWildcardAST::firstToken() const
{
    if ( t_all_or_user )
        return t_all_or_user;
    return 0;
}

int AllOrUserWildcardAST::lastToken() const
{
    if ( t_all_or_user )
        return t_all_or_user + 1;
    return 1;
}

bool AllOrUserWildcardAST::isNull() const
{
    if ( t_all_or_user )
        return false;
    return true;
}

void AllOrUserWildcardAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableClusterOffCmdAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_without )
        return t_without;
    if ( t_cluster )
        return t_cluster;
    return 0;
}

int AlterTableClusterOffCmdAST::lastToken() const
{
    if ( t_cluster )
        return t_cluster + 1;
    if ( t_without )
        return t_without + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool AlterTableClusterOffCmdAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_without )
        return false;
    if ( t_cluster )
        return false;
    return true;
}

void AlterTableClusterOffCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableClusterOnCmdAST::firstToken() const
{
    if ( t_cluster )
        return t_cluster;
    if ( t_on )
        return t_on;
    if ( clusterName && !clusterName->isNull() )
        return clusterName->firstToken();
    return 0;
}

int AlterTableClusterOnCmdAST::lastToken() const
{
    if ( clusterName && !clusterName->isNull() )
        return clusterName->lastToken();
    if ( t_on )
        return t_on + 1;
    if ( t_cluster )
        return t_cluster + 1;
    return 1;
}

bool AlterTableClusterOnCmdAST::isNull() const
{
    if ( t_cluster )
        return false;
    if ( t_on )
        return false;
    if ( clusterName )
        return false;
    return true;
}

void AlterTableClusterOnCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(clusterName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableSetOidsCmdAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_with_or_without )
        return t_with_or_without;
    if ( t_oids )
        return t_oids;
    return 0;
}

int AlterTableSetOidsCmdAST::lastToken() const
{
    if ( t_oids )
        return t_oids + 1;
    if ( t_with_or_without )
        return t_with_or_without + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool AlterTableSetOidsCmdAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_with_or_without )
        return false;
    if ( t_oids )
        return false;
    return true;
}

void AlterTableSetOidsCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableDropConstrCmdAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_constrint )
        return t_constrint;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( constrintName && !constrintName->isNull() )
        return constrintName->firstToken();
    if ( t_behavior )
        return t_behavior;
    return 0;
}

int AlterTableDropConstrCmdAST::lastToken() const
{
    if ( t_behavior )
        return t_behavior + 1;
    if ( constrintName && !constrintName->isNull() )
        return constrintName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_constrint )
        return t_constrint + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool AlterTableDropConstrCmdAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_constrint )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( constrintName )
        return false;
    if ( t_behavior )
        return false;
    return true;
}

void AlterTableDropConstrCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constrintName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableValConstrCmdAST::firstToken() const
{
    if ( t_validate )
        return t_validate;
    if ( t_constraint )
        return t_constraint;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    return 0;
}

int AlterTableValConstrCmdAST::lastToken() const
{
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    if ( t_validate )
        return t_validate + 1;
    return 1;
}

bool AlterTableValConstrCmdAST::isNull() const
{
    if ( t_validate )
        return false;
    if ( t_constraint )
        return false;
    if ( constraintName )
        return false;
    return true;
}

void AlterTableValConstrCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableAddConstrCmdAST::firstToken() const
{
    if ( t_add )
        return t_add;
    if ( constraint && !constraint->isNull() )
        return constraint->firstToken();
    return 0;
}

int AlterTableAddConstrCmdAST::lastToken() const
{
    if ( constraint && !constraint->isNull() )
        return constraint->lastToken();
    if ( t_add )
        return t_add + 1;
    return 1;
}

bool AlterTableAddConstrCmdAST::isNull() const
{
    if ( t_add )
        return false;
    if ( constraint )
        return false;
    return true;
}

void AlterTableAddConstrCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraint, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableSetTypeCmdAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_column )
        return t_column;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( setData && !setData->isNull() )
        return setData->firstToken();
    if ( t_type )
        return t_type;
    if ( columnType && !columnType->isNull() )
        return columnType->firstToken();
    if ( collate && !collate->isNull() )
        return collate->firstToken();
    if ( usingClause && !usingClause->isNull() )
        return usingClause->firstToken();
    return 0;
}

int AlterTableSetTypeCmdAST::lastToken() const
{
    if ( usingClause && !usingClause->isNull() )
        return usingClause->lastToken();
    if ( collate && !collate->isNull() )
        return collate->lastToken();
    if ( columnType && !columnType->isNull() )
        return columnType->lastToken();
    if ( t_type )
        return t_type + 1;
    if ( setData && !setData->isNull() )
        return setData->lastToken();
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableSetTypeCmdAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_column )
        return false;
    if ( columnName )
        return false;
    if ( setData )
        return false;
    if ( t_type )
        return false;
    if ( columnType )
        return false;
    if ( collate )
        return false;
    if ( usingClause )
        return false;
    return true;
}

void AlterTableSetTypeCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(setData, visitor);
        accept(columnType, visitor);
        accept(collate, visitor);
        accept(usingClause, visitor);
    }
    visitor->endVisit(this);
}

int SetDataAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_data )
        return t_data;
    return 0;
}

int SetDataAST::lastToken() const
{
    if ( t_data )
        return t_data + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool SetDataAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_data )
        return false;
    return true;
}

void SetDataAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UsingClauseAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int UsingClauseAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool UsingClauseAST::isNull() const
{
    if ( t_using )
        return false;
    if ( expr )
        return false;
    return true;
}

void UsingClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableSetStorageCmdAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_column )
        return t_column;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( t_set )
        return t_set;
    if ( t_storage )
        return t_storage;
    if ( storageName && !storageName->isNull() )
        return storageName->firstToken();
    return 0;
}

int AlterTableSetStorageCmdAST::lastToken() const
{
    if ( storageName && !storageName->isNull() )
        return storageName->lastToken();
    if ( t_storage )
        return t_storage + 1;
    if ( t_set )
        return t_set + 1;
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableSetStorageCmdAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_column )
        return false;
    if ( columnName )
        return false;
    if ( t_set )
        return false;
    if ( t_storage )
        return false;
    if ( storageName )
        return false;
    return true;
}

void AlterTableSetStorageCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(storageName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableChgReloptCmdAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_column )
        return t_column;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( t_set_or_reset )
        return t_set_or_reset;
    if ( relOptions && !relOptions->isNull() )
        return relOptions->firstToken();
    return 0;
}

int AlterTableChgReloptCmdAST::lastToken() const
{
    if ( relOptions && !relOptions->isNull() )
        return relOptions->lastToken();
    if ( t_set_or_reset )
        return t_set_or_reset + 1;
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableChgReloptCmdAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_column )
        return false;
    if ( columnName )
        return false;
    if ( t_set_or_reset )
        return false;
    if ( relOptions )
        return false;
    return true;
}

void AlterTableChgReloptCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(relOptions, visitor);
    }
    visitor->endVisit(this);
}

int RelOptionsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( options )
        return options->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int RelOptionsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( options )
        return options->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool RelOptionsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( options )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void RelOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int RelOptionAST::firstToken() const
{
    if ( rel1 && !rel1->isNull() )
        return rel1->firstToken();
    if ( t_dot )
        return t_dot;
    if ( rel2 && !rel2->isNull() )
        return rel2->firstToken();
    if ( t_equal )
        return t_equal;
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    return 0;
}

int RelOptionAST::lastToken() const
{
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    if ( t_equal )
        return t_equal + 1;
    if ( rel2 && !rel2->isNull() )
        return rel2->lastToken();
    if ( t_dot )
        return t_dot + 1;
    if ( rel1 && !rel1->isNull() )
        return rel1->lastToken();
    return 1;
}

bool RelOptionAST::isNull() const
{
    if ( rel1 )
        return false;
    if ( t_dot )
        return false;
    if ( rel2 )
        return false;
    if ( t_equal )
        return false;
    if ( arg )
        return false;
    return true;
}

void RelOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(rel1, visitor);
        accept(rel2, visitor);
        accept(arg, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableSetStatCmdAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_column )
        return t_column;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( t_set )
        return t_set;
    if ( t_statistics )
        return t_statistics;
    if ( t_value )
        return t_value;
    return 0;
}

int AlterTableSetStatCmdAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_statistics )
        return t_statistics + 1;
    if ( t_set )
        return t_set + 1;
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableSetStatCmdAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_column )
        return false;
    if ( columnName )
        return false;
    if ( t_set )
        return false;
    if ( t_statistics )
        return false;
    if ( t_value )
        return false;
    return true;
}

void AlterTableSetStatCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableChgNotNullCmdAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_column )
        return t_column;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( t_set_or_drop )
        return t_set_or_drop;
    if ( t_not )
        return t_not;
    if ( t_null )
        return t_null;
    return 0;
}

int AlterTableChgNotNullCmdAST::lastToken() const
{
    if ( t_null )
        return t_null + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_set_or_drop )
        return t_set_or_drop + 1;
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableChgNotNullCmdAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_column )
        return false;
    if ( columnName )
        return false;
    if ( t_set_or_drop )
        return false;
    if ( t_not )
        return false;
    if ( t_null )
        return false;
    return true;
}

void AlterTableChgNotNullCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableChgDefColCmdAST::firstToken() const
{
    if ( t_alter )
        return t_alter;
    if ( t_column )
        return t_column;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( cmd && !cmd->isNull() )
        return cmd->firstToken();
    return 0;
}

int AlterTableChgDefColCmdAST::lastToken() const
{
    if ( cmd && !cmd->isNull() )
        return cmd->lastToken();
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_alter )
        return t_alter + 1;
    return 1;
}

bool AlterTableChgDefColCmdAST::isNull() const
{
    if ( t_alter )
        return false;
    if ( t_column )
        return false;
    if ( columnName )
        return false;
    if ( cmd )
        return false;
    return true;
}

void AlterTableChgDefColCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(cmd, visitor);
    }
    visitor->endVisit(this);
}

int SetAlterDefaultColumnAST::firstToken() const
{
    if ( t_set )
        return t_set;
    if ( t_default )
        return t_default;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int SetAlterDefaultColumnAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_default )
        return t_default + 1;
    if ( t_set )
        return t_set + 1;
    return 1;
}

bool SetAlterDefaultColumnAST::isNull() const
{
    if ( t_set )
        return false;
    if ( t_default )
        return false;
    if ( expr )
        return false;
    return true;
}

void SetAlterDefaultColumnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int DropAlterDefaultColumnAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_default )
        return t_default;
    return 0;
}

int DropAlterDefaultColumnAST::lastToken() const
{
    if ( t_default )
        return t_default + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool DropAlterDefaultColumnAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_default )
        return false;
    return true;
}

void DropAlterDefaultColumnAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int AlterTableDropColCmdAST::firstToken() const
{
    if ( t_drop )
        return t_drop;
    if ( t_column )
        return t_column;
    if ( t_if )
        return t_if;
    if ( t_exists )
        return t_exists;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( t_drop_behavior )
        return t_drop_behavior;
    return 0;
}

int AlterTableDropColCmdAST::lastToken() const
{
    if ( t_drop_behavior )
        return t_drop_behavior + 1;
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_exists )
        return t_exists + 1;
    if ( t_if )
        return t_if + 1;
    if ( t_column )
        return t_column + 1;
    if ( t_drop )
        return t_drop + 1;
    return 1;
}

bool AlterTableDropColCmdAST::isNull() const
{
    if ( t_drop )
        return false;
    if ( t_column )
        return false;
    if ( t_if )
        return false;
    if ( t_exists )
        return false;
    if ( columnName )
        return false;
    if ( t_drop_behavior )
        return false;
    return true;
}

void AlterTableDropColCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int AlterTableAddColCmdAST::firstToken() const
{
    if ( t_add )
        return t_add;
    if ( t_column )
        return t_column;
    if ( colDef && !colDef->isNull() )
        return colDef->firstToken();
    return 0;
}

int AlterTableAddColCmdAST::lastToken() const
{
    if ( colDef && !colDef->isNull() )
        return colDef->lastToken();
    if ( t_column )
        return t_column + 1;
    if ( t_add )
        return t_add + 1;
    return 1;
}

bool AlterTableAddColCmdAST::isNull() const
{
    if ( t_add )
        return false;
    if ( t_column )
        return false;
    if ( colDef )
        return false;
    return true;
}

void AlterTableAddColCmdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(colDef, visitor);
    }
    visitor->endVisit(this);
}

int ColumnDefAST::firstToken() const
{
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( columnType && !columnType->isNull() )
        return columnType->firstToken();
    if ( options && !options->isNull() )
        return options->firstToken();
    if ( constraints )
        return constraints->firstToken();
    return 0;
}

int ColumnDefAST::lastToken() const
{
    if ( constraints )
        return constraints->lastToken();
    if ( options && !options->isNull() )
        return options->lastToken();
    if ( columnType && !columnType->isNull() )
        return columnType->lastToken();
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    return 1;
}

bool ColumnDefAST::isNull() const
{
    if ( columnName )
        return false;
    if ( columnType )
        return false;
    if ( options )
        return false;
    if ( constraints )
        return false;
    return true;
}

void ColumnDefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(columnType, visitor);
        accept(options, visitor);
        accept(constraints, visitor);
    }
    visitor->endVisit(this);
}

int TableConstraintAST::firstToken() const
{
    if ( t_constraint )
        return t_constraint;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    if ( body && !body->isNull() )
        return body->firstToken();
    return 0;
}

int TableConstraintAST::lastToken() const
{
    if ( body && !body->isNull() )
        return body->lastToken();
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    return 1;
}

bool TableConstraintAST::isNull() const
{
    if ( t_constraint )
        return false;
    if ( constraintName )
        return false;
    if ( body )
        return false;
    return true;
}

void TableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintName, visitor);
        accept(body, visitor);
    }
    visitor->endVisit(this);
}

int FkTableConstraintAST::firstToken() const
{
    if ( t_foreign )
        return t_foreign;
    if ( t_key )
        return t_key;
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( t_references )
        return t_references;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( refColumns && !refColumns->isNull() )
        return refColumns->firstToken();
    if ( match && !match->isNull() )
        return match->firstToken();
    if ( actions && !actions->isNull() )
        return actions->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int FkTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( actions && !actions->isNull() )
        return actions->lastToken();
    if ( match && !match->isNull() )
        return match->lastToken();
    if ( refColumns && !refColumns->isNull() )
        return refColumns->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_references )
        return t_references + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_key )
        return t_key + 1;
    if ( t_foreign )
        return t_foreign + 1;
    return 1;
}

bool FkTableConstraintAST::isNull() const
{
    if ( t_foreign )
        return false;
    if ( t_key )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( t_references )
        return false;
    if ( tableIdent )
        return false;
    if ( refColumns )
        return false;
    if ( match )
        return false;
    if ( actions )
        return false;
    if ( attrs )
        return false;
    return true;
}

void FkTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
        accept(tableIdent, visitor);
        accept(refColumns, visitor);
        accept(match, visitor);
        accept(actions, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int ExcludeTableConstraintAST::firstToken() const
{
    if ( t_exclude )
        return t_exclude;
    if ( accessClause && !accessClause->isNull() )
        return accessClause->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( items )
        return items->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( definitions && !definitions->isNull() )
        return definitions->firstToken();
    if ( constrTablespace && !constrTablespace->isNull() )
        return constrTablespace->firstToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int ExcludeTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( whereClause && !whereClause->isNull() )
        return whereClause->lastToken();
    if ( constrTablespace && !constrTablespace->isNull() )
        return constrTablespace->lastToken();
    if ( definitions && !definitions->isNull() )
        return definitions->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( items )
        return items->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( accessClause && !accessClause->isNull() )
        return accessClause->lastToken();
    if ( t_exclude )
        return t_exclude + 1;
    return 1;
}

bool ExcludeTableConstraintAST::isNull() const
{
    if ( t_exclude )
        return false;
    if ( accessClause )
        return false;
    if ( t_lparen )
        return false;
    if ( items )
        return false;
    if ( t_rparen )
        return false;
    if ( definitions )
        return false;
    if ( constrTablespace )
        return false;
    if ( whereClause )
        return false;
    if ( attrs )
        return false;
    return true;
}

void ExcludeTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(accessClause, visitor);
        accept(items, visitor);
        accept(definitions, visitor);
        accept(constrTablespace, visitor);
        accept(whereClause, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int AccessMethodClauseAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( methodName && !methodName->isNull() )
        return methodName->firstToken();
    return 0;
}

int AccessMethodClauseAST::lastToken() const
{
    if ( methodName && !methodName->isNull() )
        return methodName->lastToken();
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool AccessMethodClauseAST::isNull() const
{
    if ( t_using )
        return false;
    if ( methodName )
        return false;
    return true;
}

void AccessMethodClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(methodName, visitor);
    }
    visitor->endVisit(this);
}

int ExclusionWhereClauseAST::firstToken() const
{
    if ( t_where )
        return t_where;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int ExclusionWhereClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_where )
        return t_where + 1;
    return 1;
}

bool ExclusionWhereClauseAST::isNull() const
{
    if ( t_where )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void ExclusionWhereClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int ExclusionConstraintAST::firstToken() const
{
    if ( indexElement && !indexElement->isNull() )
        return indexElement->firstToken();
    if ( t_with )
        return t_with;
    if ( t_operator )
        return t_operator;
    if ( t_lparen )
        return t_lparen;
    if ( operand && !operand->isNull() )
        return operand->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int ExclusionConstraintAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( operand && !operand->isNull() )
        return operand->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_operator )
        return t_operator + 1;
    if ( t_with )
        return t_with + 1;
    if ( indexElement && !indexElement->isNull() )
        return indexElement->lastToken();
    return 1;
}

bool ExclusionConstraintAST::isNull() const
{
    if ( indexElement )
        return false;
    if ( t_with )
        return false;
    if ( t_operator )
        return false;
    if ( t_lparen )
        return false;
    if ( operand )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void ExclusionConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexElement, visitor);
        accept(operand, visitor);
    }
    visitor->endVisit(this);
}

int ColIndexElementAST::firstToken() const
{
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    if ( collate && !collate->isNull() )
        return collate->firstToken();
    if ( classClause && !classClause->isNull() )
        return classClause->firstToken();
    if ( t_asc_or_desc )
        return t_asc_or_desc;
    if ( t_nulls_order )
        return t_nulls_order;
    return 0;
}

int ColIndexElementAST::lastToken() const
{
    if ( t_nulls_order )
        return t_nulls_order + 1;
    if ( t_asc_or_desc )
        return t_asc_or_desc + 1;
    if ( classClause && !classClause->isNull() )
        return classClause->lastToken();
    if ( collate && !collate->isNull() )
        return collate->lastToken();
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    return 1;
}

bool ColIndexElementAST::isNull() const
{
    if ( columnName )
        return false;
    if ( collate )
        return false;
    if ( classClause )
        return false;
    if ( t_asc_or_desc )
        return false;
    if ( t_nulls_order )
        return false;
    return true;
}

void ColIndexElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnName, visitor);
        accept(collate, visitor);
        accept(classClause, visitor);
    }
    visitor->endVisit(this);
}

int FuncExprIndexElementAST::firstToken() const
{
    if ( funcExpr && !funcExpr->isNull() )
        return funcExpr->firstToken();
    if ( collate && !collate->isNull() )
        return collate->firstToken();
    if ( classClause && !classClause->isNull() )
        return classClause->firstToken();
    if ( t_asc_or_desc )
        return t_asc_or_desc;
    if ( t_nulls_order )
        return t_nulls_order;
    return 0;
}

int FuncExprIndexElementAST::lastToken() const
{
    if ( t_nulls_order )
        return t_nulls_order + 1;
    if ( t_asc_or_desc )
        return t_asc_or_desc + 1;
    if ( classClause && !classClause->isNull() )
        return classClause->lastToken();
    if ( collate && !collate->isNull() )
        return collate->lastToken();
    if ( funcExpr && !funcExpr->isNull() )
        return funcExpr->lastToken();
    return 1;
}

bool FuncExprIndexElementAST::isNull() const
{
    if ( funcExpr )
        return false;
    if ( collate )
        return false;
    if ( classClause )
        return false;
    if ( t_asc_or_desc )
        return false;
    if ( t_nulls_order )
        return false;
    return true;
}

void FuncExprIndexElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcExpr, visitor);
        accept(collate, visitor);
        accept(classClause, visitor);
    }
    visitor->endVisit(this);
}

int ExprIndexElementAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( collate && !collate->isNull() )
        return collate->firstToken();
    if ( classClause && !classClause->isNull() )
        return classClause->firstToken();
    if ( t_asc_or_desc )
        return t_asc_or_desc;
    if ( t_nulls_order )
        return t_nulls_order;
    return 0;
}

int ExprIndexElementAST::lastToken() const
{
    if ( t_nulls_order )
        return t_nulls_order + 1;
    if ( t_asc_or_desc )
        return t_asc_or_desc + 1;
    if ( classClause && !classClause->isNull() )
        return classClause->lastToken();
    if ( collate && !collate->isNull() )
        return collate->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool ExprIndexElementAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    if ( collate )
        return false;
    if ( classClause )
        return false;
    if ( t_asc_or_desc )
        return false;
    if ( t_nulls_order )
        return false;
    return true;
}

void ExprIndexElementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(collate, visitor);
        accept(classClause, visitor);
    }
    visitor->endVisit(this);
}

int ClassClauseAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( name && !name->isNull() )
        return name->firstToken();
    return 0;
}

int ClassClauseAST::lastToken() const
{
    if ( name && !name->isNull() )
        return name->lastToken();
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool ClassClauseAST::isNull() const
{
    if ( t_using )
        return false;
    if ( name )
        return false;
    return true;
}

void ClassClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(name, visitor);
    }
    visitor->endVisit(this);
}

int IndPrimKeyTableConstraintAST::firstToken() const
{
    if ( t_primary )
        return t_primary;
    if ( t_key )
        return t_key;
    if ( existingIndex && !existingIndex->isNull() )
        return existingIndex->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int IndPrimKeyTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( existingIndex && !existingIndex->isNull() )
        return existingIndex->lastToken();
    if ( t_key )
        return t_key + 1;
    if ( t_primary )
        return t_primary + 1;
    return 1;
}

bool IndPrimKeyTableConstraintAST::isNull() const
{
    if ( t_primary )
        return false;
    if ( t_key )
        return false;
    if ( existingIndex )
        return false;
    if ( attrs )
        return false;
    return true;
}

void IndPrimKeyTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(existingIndex, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int PrimKeyTableConstraintAST::firstToken() const
{
    if ( t_primary )
        return t_primary;
    if ( t_key )
        return t_key;
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( definitions && !definitions->isNull() )
        return definitions->firstToken();
    if ( constrTablespace && !constrTablespace->isNull() )
        return constrTablespace->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int PrimKeyTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( constrTablespace && !constrTablespace->isNull() )
        return constrTablespace->lastToken();
    if ( definitions && !definitions->isNull() )
        return definitions->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_key )
        return t_key + 1;
    if ( t_primary )
        return t_primary + 1;
    return 1;
}

bool PrimKeyTableConstraintAST::isNull() const
{
    if ( t_primary )
        return false;
    if ( t_key )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( definitions )
        return false;
    if ( constrTablespace )
        return false;
    if ( attrs )
        return false;
    return true;
}

void PrimKeyTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
        accept(definitions, visitor);
        accept(constrTablespace, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int IndUniqueTableConstraintAST::firstToken() const
{
    if ( t_unique )
        return t_unique;
    if ( existingIndex && !existingIndex->isNull() )
        return existingIndex->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int IndUniqueTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( existingIndex && !existingIndex->isNull() )
        return existingIndex->lastToken();
    if ( t_unique )
        return t_unique + 1;
    return 1;
}

bool IndUniqueTableConstraintAST::isNull() const
{
    if ( t_unique )
        return false;
    if ( existingIndex )
        return false;
    if ( attrs )
        return false;
    return true;
}

void IndUniqueTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(existingIndex, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int ExistingIndexAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( t_index )
        return t_index;
    if ( indexName && !indexName->isNull() )
        return indexName->firstToken();
    return 0;
}

int ExistingIndexAST::lastToken() const
{
    if ( indexName && !indexName->isNull() )
        return indexName->lastToken();
    if ( t_index )
        return t_index + 1;
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool ExistingIndexAST::isNull() const
{
    if ( t_using )
        return false;
    if ( t_index )
        return false;
    if ( indexName )
        return false;
    return true;
}

void ExistingIndexAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(indexName, visitor);
    }
    visitor->endVisit(this);
}

int UniqueTableConstraintAST::firstToken() const
{
    if ( t_unique )
        return t_unique;
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( definitions && !definitions->isNull() )
        return definitions->firstToken();
    if ( constrTablespace && !constrTablespace->isNull() )
        return constrTablespace->firstToken();
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int UniqueTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( constrTablespace && !constrTablespace->isNull() )
        return constrTablespace->lastToken();
    if ( definitions && !definitions->isNull() )
        return definitions->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_unique )
        return t_unique + 1;
    return 1;
}

bool UniqueTableConstraintAST::isNull() const
{
    if ( t_unique )
        return false;
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    if ( definitions )
        return false;
    if ( constrTablespace )
        return false;
    if ( attrs )
        return false;
    return true;
}

void UniqueTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
        accept(definitions, visitor);
        accept(constrTablespace, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int CheckTableConstraintAST::firstToken() const
{
    if ( t_check )
        return t_check;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( attrs )
        return attrs->firstToken();
    return 0;
}

int CheckTableConstraintAST::lastToken() const
{
    if ( attrs )
        return attrs->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_check )
        return t_check + 1;
    return 1;
}

bool CheckTableConstraintAST::isNull() const
{
    if ( t_check )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    if ( attrs )
        return false;
    return true;
}

void CheckTableConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int DeferConstraintAttrAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_deferrable )
        return t_deferrable;
    return 0;
}

int DeferConstraintAttrAST::lastToken() const
{
    if ( t_deferrable )
        return t_deferrable + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool DeferConstraintAttrAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_deferrable )
        return false;
    return true;
}

void DeferConstraintAttrAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int InitiallyConstraintAttrAST::firstToken() const
{
    if ( t_initally )
        return t_initally;
    if ( t_deferred_or_immediate )
        return t_deferred_or_immediate;
    return 0;
}

int InitiallyConstraintAttrAST::lastToken() const
{
    if ( t_deferred_or_immediate )
        return t_deferred_or_immediate + 1;
    if ( t_initally )
        return t_initally + 1;
    return 1;
}

bool InitiallyConstraintAttrAST::isNull() const
{
    if ( t_initally )
        return false;
    if ( t_deferred_or_immediate )
        return false;
    return true;
}

void InitiallyConstraintAttrAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int NotValidConstraintAttrAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_valid )
        return t_valid;
    return 0;
}

int NotValidConstraintAttrAST::lastToken() const
{
    if ( t_valid )
        return t_valid + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool NotValidConstraintAttrAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_valid )
        return false;
    return true;
}

void NotValidConstraintAttrAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int NoInheritConstraintAttrAST::firstToken() const
{
    if ( t_no )
        return t_no;
    if ( t_inherit )
        return t_inherit;
    return 0;
}

int NoInheritConstraintAttrAST::lastToken() const
{
    if ( t_inherit )
        return t_inherit + 1;
    if ( t_no )
        return t_no + 1;
    return 1;
}

bool NoInheritConstraintAttrAST::isNull() const
{
    if ( t_no )
        return false;
    if ( t_inherit )
        return false;
    return true;
}

void NoInheritConstraintAttrAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CommonColConstraintAST::firstToken() const
{
    if ( t_constraint )
        return t_constraint;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    if ( body && !body->isNull() )
        return body->firstToken();
    return 0;
}

int CommonColConstraintAST::lastToken() const
{
    if ( body && !body->isNull() )
        return body->lastToken();
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_constraint )
        return t_constraint + 1;
    return 1;
}

bool CommonColConstraintAST::isNull() const
{
    if ( t_constraint )
        return false;
    if ( constraintName )
        return false;
    if ( body )
        return false;
    return true;
}

void CommonColConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constraintName, visitor);
        accept(body, visitor);
    }
    visitor->endVisit(this);
}

int RefConstraintBodyAST::firstToken() const
{
    if ( t_references )
        return t_references;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( refColumns && !refColumns->isNull() )
        return refColumns->firstToken();
    if ( match && !match->isNull() )
        return match->firstToken();
    if ( actions && !actions->isNull() )
        return actions->firstToken();
    return 0;
}

int RefConstraintBodyAST::lastToken() const
{
    if ( actions && !actions->isNull() )
        return actions->lastToken();
    if ( match && !match->isNull() )
        return match->lastToken();
    if ( refColumns && !refColumns->isNull() )
        return refColumns->lastToken();
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_references )
        return t_references + 1;
    return 1;
}

bool RefConstraintBodyAST::isNull() const
{
    if ( t_references )
        return false;
    if ( tableIdent )
        return false;
    if ( refColumns )
        return false;
    if ( match )
        return false;
    if ( actions )
        return false;
    return true;
}

void RefConstraintBodyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
        accept(refColumns, visitor);
        accept(match, visitor);
        accept(actions, visitor);
    }
    visitor->endVisit(this);
}

int RefColumnsAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( columns )
        return columns->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int RefColumnsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( columns )
        return columns->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool RefColumnsAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( columns )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void RefColumnsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columns, visitor);
    }
    visitor->endVisit(this);
}

int KeyMatchAST::firstToken() const
{
    if ( t_match )
        return t_match;
    if ( t_full_or_partial_or_simple )
        return t_full_or_partial_or_simple;
    return 0;
}

int KeyMatchAST::lastToken() const
{
    if ( t_full_or_partial_or_simple )
        return t_full_or_partial_or_simple + 1;
    if ( t_match )
        return t_match + 1;
    return 1;
}

bool KeyMatchAST::isNull() const
{
    if ( t_match )
        return false;
    if ( t_full_or_partial_or_simple )
        return false;
    return true;
}

void KeyMatchAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UpdDelKeyActionsAST::firstToken() const
{
    if ( upd && !upd->isNull() )
        return upd->firstToken();
    if ( del && !del->isNull() )
        return del->firstToken();
    return 0;
}

int UpdDelKeyActionsAST::lastToken() const
{
    if ( del && !del->isNull() )
        return del->lastToken();
    if ( upd && !upd->isNull() )
        return upd->lastToken();
    return 1;
}

bool UpdDelKeyActionsAST::isNull() const
{
    if ( upd )
        return false;
    if ( del )
        return false;
    return true;
}

void UpdDelKeyActionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(upd, visitor);
        accept(del, visitor);
    }
    visitor->endVisit(this);
}

int DelUpdKeyActionsAST::firstToken() const
{
    if ( del && !del->isNull() )
        return del->firstToken();
    if ( upd && !upd->isNull() )
        return upd->firstToken();
    return 0;
}

int DelUpdKeyActionsAST::lastToken() const
{
    if ( upd && !upd->isNull() )
        return upd->lastToken();
    if ( del && !del->isNull() )
        return del->lastToken();
    return 1;
}

bool DelUpdKeyActionsAST::isNull() const
{
    if ( del )
        return false;
    if ( upd )
        return false;
    return true;
}

void DelUpdKeyActionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(del, visitor);
        accept(upd, visitor);
    }
    visitor->endVisit(this);
}

int KeyUpdateAST::firstToken() const
{
    if ( t_on )
        return t_on;
    if ( t_update )
        return t_update;
    if ( action && !action->isNull() )
        return action->firstToken();
    return 0;
}

int KeyUpdateAST::lastToken() const
{
    if ( action && !action->isNull() )
        return action->lastToken();
    if ( t_update )
        return t_update + 1;
    if ( t_on )
        return t_on + 1;
    return 1;
}

bool KeyUpdateAST::isNull() const
{
    if ( t_on )
        return false;
    if ( t_update )
        return false;
    if ( action )
        return false;
    return true;
}

void KeyUpdateAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(action, visitor);
    }
    visitor->endVisit(this);
}

int KeyDeleteAST::firstToken() const
{
    if ( t_on )
        return t_on;
    if ( t_delete )
        return t_delete;
    if ( action && !action->isNull() )
        return action->firstToken();
    return 0;
}

int KeyDeleteAST::lastToken() const
{
    if ( action && !action->isNull() )
        return action->lastToken();
    if ( t_delete )
        return t_delete + 1;
    if ( t_on )
        return t_on + 1;
    return 1;
}

bool KeyDeleteAST::isNull() const
{
    if ( t_on )
        return false;
    if ( t_delete )
        return false;
    if ( action )
        return false;
    return true;
}

void KeyDeleteAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(action, visitor);
    }
    visitor->endVisit(this);
}

int KeyActionAST::firstToken() const
{
    if ( t_spec1 )
        return t_spec1;
    if ( t_spec2 )
        return t_spec2;
    return 0;
}

int KeyActionAST::lastToken() const
{
    if ( t_spec2 )
        return t_spec2 + 1;
    if ( t_spec1 )
        return t_spec1 + 1;
    return 1;
}

bool KeyActionAST::isNull() const
{
    if ( t_spec1 )
        return false;
    if ( t_spec2 )
        return false;
    return true;
}

void KeyActionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DefaultConstraintBodyAST::firstToken() const
{
    if ( t_default )
        return t_default;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int DefaultConstraintBodyAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_default )
        return t_default + 1;
    return 1;
}

bool DefaultConstraintBodyAST::isNull() const
{
    if ( t_default )
        return false;
    if ( expr )
        return false;
    return true;
}

void DefaultConstraintBodyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CheckConstraintBodyAST::firstToken() const
{
    if ( t_check )
        return t_check;
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( noInherit && !noInherit->isNull() )
        return noInherit->firstToken();
    return 0;
}

int CheckConstraintBodyAST::lastToken() const
{
    if ( noInherit && !noInherit->isNull() )
        return noInherit->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_check )
        return t_check + 1;
    return 1;
}

bool CheckConstraintBodyAST::isNull() const
{
    if ( t_check )
        return false;
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    if ( noInherit )
        return false;
    return true;
}

void CheckConstraintBodyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(noInherit, visitor);
    }
    visitor->endVisit(this);
}

int NoInheritAST::firstToken() const
{
    if ( t_no )
        return t_no;
    if ( t_inherit )
        return t_inherit;
    return 0;
}

int NoInheritAST::lastToken() const
{
    if ( t_inherit )
        return t_inherit + 1;
    if ( t_no )
        return t_no + 1;
    return 1;
}

bool NoInheritAST::isNull() const
{
    if ( t_no )
        return false;
    if ( t_inherit )
        return false;
    return true;
}

void NoInheritAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int UniqueConstraintBodyAST::firstToken() const
{
    if ( t_unique )
        return t_unique;
    if ( definitions && !definitions->isNull() )
        return definitions->firstToken();
    if ( tablespace && !tablespace->isNull() )
        return tablespace->firstToken();
    return 0;
}

int UniqueConstraintBodyAST::lastToken() const
{
    if ( tablespace && !tablespace->isNull() )
        return tablespace->lastToken();
    if ( definitions && !definitions->isNull() )
        return definitions->lastToken();
    if ( t_unique )
        return t_unique + 1;
    return 1;
}

bool UniqueConstraintBodyAST::isNull() const
{
    if ( t_unique )
        return false;
    if ( definitions )
        return false;
    if ( tablespace )
        return false;
    return true;
}

void UniqueConstraintBodyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(definitions, visitor);
        accept(tablespace, visitor);
    }
    visitor->endVisit(this);
}

int PrimaryKeyConstraintBodyAST::firstToken() const
{
    if ( t_primary )
        return t_primary;
    if ( t_key )
        return t_key;
    if ( definitions && !definitions->isNull() )
        return definitions->firstToken();
    if ( tablespace && !tablespace->isNull() )
        return tablespace->firstToken();
    return 0;
}

int PrimaryKeyConstraintBodyAST::lastToken() const
{
    if ( tablespace && !tablespace->isNull() )
        return tablespace->lastToken();
    if ( definitions && !definitions->isNull() )
        return definitions->lastToken();
    if ( t_key )
        return t_key + 1;
    if ( t_primary )
        return t_primary + 1;
    return 1;
}

bool PrimaryKeyConstraintBodyAST::isNull() const
{
    if ( t_primary )
        return false;
    if ( t_key )
        return false;
    if ( definitions )
        return false;
    if ( tablespace )
        return false;
    return true;
}

void PrimaryKeyConstraintBodyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(definitions, visitor);
        accept(tablespace, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintTablespaceAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( t_index )
        return t_index;
    if ( t_tablespace )
        return t_tablespace;
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->firstToken();
    return 0;
}

int ConstraintTablespaceAST::lastToken() const
{
    if ( tablespaceName && !tablespaceName->isNull() )
        return tablespaceName->lastToken();
    if ( t_tablespace )
        return t_tablespace + 1;
    if ( t_index )
        return t_index + 1;
    if ( t_using )
        return t_using + 1;
    return 1;
}

bool ConstraintTablespaceAST::isNull() const
{
    if ( t_using )
        return false;
    if ( t_index )
        return false;
    if ( t_tablespace )
        return false;
    if ( tablespaceName )
        return false;
    return true;
}

void ConstraintTablespaceAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tablespaceName, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintDefinitionsAST::firstToken() const
{
    if ( t_with )
        return t_with;
    if ( def && !def->isNull() )
        return def->firstToken();
    return 0;
}

int ConstraintDefinitionsAST::lastToken() const
{
    if ( def && !def->isNull() )
        return def->lastToken();
    if ( t_with )
        return t_with + 1;
    return 1;
}

bool ConstraintDefinitionsAST::isNull() const
{
    if ( t_with )
        return false;
    if ( def )
        return false;
    return true;
}

void ConstraintDefinitionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(def, visitor);
    }
    visitor->endVisit(this);
}

int DefinitionAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( items )
        return items->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int DefinitionAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( items )
        return items->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool DefinitionAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( items )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void DefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(items, visitor);
    }
    visitor->endVisit(this);
}

int DefinitionElemAST::firstToken() const
{
    if ( t_label )
        return t_label;
    if ( t_equal )
        return t_equal;
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    return 0;
}

int DefinitionElemAST::lastToken() const
{
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    if ( t_equal )
        return t_equal + 1;
    if ( t_label )
        return t_label + 1;
    return 1;
}

bool DefinitionElemAST::isNull() const
{
    if ( t_label )
        return false;
    if ( t_equal )
        return false;
    if ( arg )
        return false;
    return true;
}

void DefinitionElemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arg, visitor);
    }
    visitor->endVisit(this);
}

int FuncTyDefinitionArgumentAST::firstToken() const
{
    if ( funcTy && !funcTy->isNull() )
        return funcTy->firstToken();
    return 0;
}

int FuncTyDefinitionArgumentAST::lastToken() const
{
    if ( funcTy && !funcTy->isNull() )
        return funcTy->lastToken();
    return 1;
}

bool FuncTyDefinitionArgumentAST::isNull() const
{
    if ( funcTy )
        return false;
    return true;
}

void FuncTyDefinitionArgumentAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcTy, visitor);
    }
    visitor->endVisit(this);
}

int PercentFunctionTypeAST::firstToken() const
{
    if ( t_setof )
        return t_setof;
    if ( funcName && !funcName->isNull() )
        return funcName->firstToken();
    if ( attrs )
        return attrs->firstToken();
    if ( t_percent )
        return t_percent;
    if ( t_type )
        return t_type;
    return 0;
}

int PercentFunctionTypeAST::lastToken() const
{
    if ( t_type )
        return t_type + 1;
    if ( t_percent )
        return t_percent + 1;
    if ( attrs )
        return attrs->lastToken();
    if ( funcName && !funcName->isNull() )
        return funcName->lastToken();
    if ( t_setof )
        return t_setof + 1;
    return 1;
}

bool PercentFunctionTypeAST::isNull() const
{
    if ( t_setof )
        return false;
    if ( funcName )
        return false;
    if ( attrs )
        return false;
    if ( t_percent )
        return false;
    if ( t_type )
        return false;
    return true;
}

void PercentFunctionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcName, visitor);
        accept(attrs, visitor);
    }
    visitor->endVisit(this);
}

int SimpleFunctionTypeAST::firstToken() const
{
    if ( type && !type->isNull() )
        return type->firstToken();
    return 0;
}

int SimpleFunctionTypeAST::lastToken() const
{
    if ( type && !type->isNull() )
        return type->lastToken();
    return 1;
}

bool SimpleFunctionTypeAST::isNull() const
{
    if ( type )
        return false;
    return true;
}

void SimpleFunctionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

int QualOpDefinitionArgumentAST::firstToken() const
{
    if ( qualOp && !qualOp->isNull() )
        return qualOp->firstToken();
    return 0;
}

int QualOpDefinitionArgumentAST::lastToken() const
{
    if ( qualOp && !qualOp->isNull() )
        return qualOp->lastToken();
    return 1;
}

bool QualOpDefinitionArgumentAST::isNull() const
{
    if ( qualOp )
        return false;
    return true;
}

void QualOpDefinitionArgumentAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(qualOp, visitor);
    }
    visitor->endVisit(this);
}

int SimpleDefinitionArgumentAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

int SimpleDefinitionArgumentAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool SimpleDefinitionArgumentAST::isNull() const
{
    if ( t_value )
        return false;
    return true;
}

void SimpleDefinitionArgumentAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int NotNullConstraintBodyAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_null )
        return t_null;
    return 0;
}

int NotNullConstraintBodyAST::lastToken() const
{
    if ( t_null )
        return t_null + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool NotNullConstraintBodyAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_null )
        return false;
    return true;
}

void NotNullConstraintBodyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DeferColConstraintAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_deferrable )
        return t_deferrable;
    return 0;
}

int DeferColConstraintAST::lastToken() const
{
    if ( t_deferrable )
        return t_deferrable + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool DeferColConstraintAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_deferrable )
        return false;
    return true;
}

void DeferColConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int InitiallyColConstraintAST::firstToken() const
{
    if ( t_initially )
        return t_initially;
    if ( t_deferred_or_immediate )
        return t_deferred_or_immediate;
    return 0;
}

int InitiallyColConstraintAST::lastToken() const
{
    if ( t_deferred_or_immediate )
        return t_deferred_or_immediate + 1;
    if ( t_initially )
        return t_initially + 1;
    return 1;
}

bool InitiallyColConstraintAST::isNull() const
{
    if ( t_initially )
        return false;
    if ( t_deferred_or_immediate )
        return false;
    return true;
}

void InitiallyColConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CollateColConstraintAST::firstToken() const
{
    if ( t_collate )
        return t_collate;
    if ( collateValue && !collateValue->isNull() )
        return collateValue->firstToken();
    return 0;
}

int CollateColConstraintAST::lastToken() const
{
    if ( collateValue && !collateValue->isNull() )
        return collateValue->lastToken();
    if ( t_collate )
        return t_collate + 1;
    return 1;
}

bool CollateColConstraintAST::isNull() const
{
    if ( t_collate )
        return false;
    if ( collateValue )
        return false;
    return true;
}

void CollateColConstraintAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(collateValue, visitor);
    }
    visitor->endVisit(this);
}

int CreateGeneticOptionsAST::firstToken() const
{
    if ( t_options )
        return t_options;
    if ( t_lparen )
        return t_lparen;
    if ( options )
        return options->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CreateGeneticOptionsAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( options )
        return options->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_options )
        return t_options + 1;
    return 1;
}

bool CreateGeneticOptionsAST::isNull() const
{
    if ( t_options )
        return false;
    if ( t_lparen )
        return false;
    if ( options )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CreateGeneticOptionsAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(options, visitor);
    }
    visitor->endVisit(this);
}

int GenericOptionAST::firstToken() const
{
    if ( t_option )
        return t_option;
    if ( t_value )
        return t_value;
    return 0;
}

int GenericOptionAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_option )
        return t_option + 1;
    return 1;
}

bool GenericOptionAST::isNull() const
{
    if ( t_option )
        return false;
    if ( t_value )
        return false;
    return true;
}

void GenericOptionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TypeAST::firstToken() const
{
    if ( t_setof )
        return t_setof;
    if ( simpleName && !simpleName->isNull() )
        return simpleName->firstToken();
    if ( t_array )
        return t_array;
    if ( arrBounds )
        return arrBounds->firstToken();
    return 0;
}

int TypeAST::lastToken() const
{
    if ( arrBounds )
        return arrBounds->lastToken();
    if ( t_array )
        return t_array + 1;
    if ( simpleName && !simpleName->isNull() )
        return simpleName->lastToken();
    if ( t_setof )
        return t_setof + 1;
    return 1;
}

bool TypeAST::isNull() const
{
    if ( t_setof )
        return false;
    if ( simpleName )
        return false;
    if ( t_array )
        return false;
    if ( arrBounds )
        return false;
    return true;
}

void TypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(simpleName, visitor);
        accept(arrBounds, visitor);
    }
    visitor->endVisit(this);
}

int GenericTypenameAST::firstToken() const
{
    if ( typeName && !typeName->isNull() )
        return typeName->firstToken();
    if ( attributes )
        return attributes->firstToken();
    if ( modifiers && !modifiers->isNull() )
        return modifiers->firstToken();
    return 0;
}

int GenericTypenameAST::lastToken() const
{
    if ( modifiers && !modifiers->isNull() )
        return modifiers->lastToken();
    if ( attributes )
        return attributes->lastToken();
    if ( typeName && !typeName->isNull() )
        return typeName->lastToken();
    return 1;
}

bool GenericTypenameAST::isNull() const
{
    if ( typeName )
        return false;
    if ( attributes )
        return false;
    if ( modifiers )
        return false;
    return true;
}

void GenericTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
        accept(attributes, visitor);
        accept(modifiers, visitor);
    }
    visitor->endVisit(this);
}

int IntervalTypenameAST::firstToken() const
{
    if ( t_interval )
        return t_interval;
    if ( t_lparen )
        return t_lparen;
    if ( t_arrLength )
        return t_arrLength;
    if ( t_rparen )
        return t_rparen;
    if ( interval && !interval->isNull() )
        return interval->firstToken();
    return 0;
}

int IntervalTypenameAST::lastToken() const
{
    if ( interval && !interval->isNull() )
        return interval->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_arrLength )
        return t_arrLength + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_interval )
        return t_interval + 1;
    return 1;
}

bool IntervalTypenameAST::isNull() const
{
    if ( t_interval )
        return false;
    if ( t_lparen )
        return false;
    if ( t_arrLength )
        return false;
    if ( t_rparen )
        return false;
    if ( interval )
        return false;
    return true;
}

void IntervalTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(interval, visitor);
    }
    visitor->endVisit(this);
}

int NumericTypenameAST::firstToken() const
{
    if ( t_type_spec )
        return t_type_spec;
    return 0;
}

int NumericTypenameAST::lastToken() const
{
    if ( t_type_spec )
        return t_type_spec + 1;
    return 1;
}

bool NumericTypenameAST::isNull() const
{
    if ( t_type_spec )
        return false;
    return true;
}

void NumericTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int FloatTypenameAST::firstToken() const
{
    if ( t_type_spec )
        return t_type_spec;
    if ( precision && !precision->isNull() )
        return precision->firstToken();
    return 0;
}

int FloatTypenameAST::lastToken() const
{
    if ( precision && !precision->isNull() )
        return precision->lastToken();
    if ( t_type_spec )
        return t_type_spec + 1;
    return 1;
}

bool FloatTypenameAST::isNull() const
{
    if ( t_type_spec )
        return false;
    if ( precision )
        return false;
    return true;
}

void FloatTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(precision, visitor);
    }
    visitor->endVisit(this);
}

int DoubleTypenameAST::firstToken() const
{
    if ( t_double )
        return t_double;
    if ( t_precision )
        return t_precision;
    return 0;
}

int DoubleTypenameAST::lastToken() const
{
    if ( t_precision )
        return t_precision + 1;
    if ( t_double )
        return t_double + 1;
    return 1;
}

bool DoubleTypenameAST::isNull() const
{
    if ( t_double )
        return false;
    if ( t_precision )
        return false;
    return true;
}

void DoubleTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int DecimalTypenameAST::firstToken() const
{
    if ( t_type_spec )
        return t_type_spec;
    if ( modifiers && !modifiers->isNull() )
        return modifiers->firstToken();
    return 0;
}

int DecimalTypenameAST::lastToken() const
{
    if ( modifiers && !modifiers->isNull() )
        return modifiers->lastToken();
    if ( t_type_spec )
        return t_type_spec + 1;
    return 1;
}

bool DecimalTypenameAST::isNull() const
{
    if ( t_type_spec )
        return false;
    if ( modifiers )
        return false;
    return true;
}

void DecimalTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(modifiers, visitor);
    }
    visitor->endVisit(this);
}

int FloatPrecisionAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( t_precision )
        return t_precision;
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int FloatPrecisionAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_precision )
        return t_precision + 1;
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool FloatPrecisionAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( t_precision )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void FloatPrecisionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int BitTypenameAST::firstToken() const
{
    if ( t_type_spec )
        return t_type_spec;
    if ( t_varying )
        return t_varying;
    if ( t_lparen )
        return t_lparen;
    if ( modifiers )
        return modifiers->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int BitTypenameAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( modifiers )
        return modifiers->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_varying )
        return t_varying + 1;
    if ( t_type_spec )
        return t_type_spec + 1;
    return 1;
}

bool BitTypenameAST::isNull() const
{
    if ( t_type_spec )
        return false;
    if ( t_varying )
        return false;
    if ( t_lparen )
        return false;
    if ( modifiers )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void BitTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(modifiers, visitor);
    }
    visitor->endVisit(this);
}

int CharacterTypenameAST::firstToken() const
{
    if ( typeSpec && !typeSpec->isNull() )
        return typeSpec->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( t_length )
        return t_length;
    if ( t_rparen )
        return t_rparen;
    if ( set && !set->isNull() )
        return set->firstToken();
    return 0;
}

int CharacterTypenameAST::lastToken() const
{
    if ( set && !set->isNull() )
        return set->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_length )
        return t_length + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( typeSpec && !typeSpec->isNull() )
        return typeSpec->lastToken();
    return 1;
}

bool CharacterTypenameAST::isNull() const
{
    if ( typeSpec )
        return false;
    if ( t_lparen )
        return false;
    if ( t_length )
        return false;
    if ( t_rparen )
        return false;
    if ( set )
        return false;
    return true;
}

void CharacterTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeSpec, visitor);
        accept(set, visitor);
    }
    visitor->endVisit(this);
}

int DatetimeTypenameAST::firstToken() const
{
    if ( t_time_or_timestamp )
        return t_time_or_timestamp;
    if ( t_lparen )
        return t_lparen;
    if ( t_value )
        return t_value;
    if ( t_rparen )
        return t_rparen;
    if ( timeZoneSpec && !timeZoneSpec->isNull() )
        return timeZoneSpec->firstToken();
    return 0;
}

int DatetimeTypenameAST::lastToken() const
{
    if ( timeZoneSpec && !timeZoneSpec->isNull() )
        return timeZoneSpec->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_value )
        return t_value + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_time_or_timestamp )
        return t_time_or_timestamp + 1;
    return 1;
}

bool DatetimeTypenameAST::isNull() const
{
    if ( t_time_or_timestamp )
        return false;
    if ( t_lparen )
        return false;
    if ( t_value )
        return false;
    if ( t_rparen )
        return false;
    if ( timeZoneSpec )
        return false;
    return true;
}

void DatetimeTypenameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(timeZoneSpec, visitor);
    }
    visitor->endVisit(this);
}

int TimeZoneSpecAST::firstToken() const
{
    if ( t_with_or_without )
        return t_with_or_without;
    if ( t_time )
        return t_time;
    if ( t_zone )
        return t_zone;
    return 0;
}

int TimeZoneSpecAST::lastToken() const
{
    if ( t_zone )
        return t_zone + 1;
    if ( t_time )
        return t_time + 1;
    if ( t_with_or_without )
        return t_with_or_without + 1;
    return 1;
}

bool TimeZoneSpecAST::isNull() const
{
    if ( t_with_or_without )
        return false;
    if ( t_time )
        return false;
    if ( t_zone )
        return false;
    return true;
}

void TimeZoneSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CharacterTypeSpecAST::firstToken() const
{
    if ( t_national )
        return t_national;
    if ( t_type_spec )
        return t_type_spec;
    if ( t_varying )
        return t_varying;
    return 0;
}

int CharacterTypeSpecAST::lastToken() const
{
    if ( t_varying )
        return t_varying + 1;
    if ( t_type_spec )
        return t_type_spec + 1;
    if ( t_national )
        return t_national + 1;
    return 1;
}

bool CharacterTypeSpecAST::isNull() const
{
    if ( t_national )
        return false;
    if ( t_type_spec )
        return false;
    if ( t_varying )
        return false;
    return true;
}

void CharacterTypeSpecAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CharSetAST::firstToken() const
{
    if ( t_character )
        return t_character;
    if ( t_set )
        return t_set;
    if ( t_value )
        return t_value;
    return 0;
}

int CharSetAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_set )
        return t_set + 1;
    if ( t_character )
        return t_character + 1;
    return 1;
}

bool CharSetAST::isNull() const
{
    if ( t_character )
        return false;
    if ( t_set )
        return false;
    if ( t_value )
        return false;
    return true;
}

void CharSetAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int TypeModifiersAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( modifiers )
        return modifiers->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int TypeModifiersAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( modifiers )
        return modifiers->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool TypeModifiersAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( modifiers )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void TypeModifiersAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(modifiers, visitor);
    }
    visitor->endVisit(this);
}

int ArrBoundAST::firstToken() const
{
    if ( t_lbracket )
        return t_lbracket;
    if ( t_arr_length )
        return t_arr_length;
    if ( t_rbracket )
        return t_rbracket;
    return 0;
}

int ArrBoundAST::lastToken() const
{
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( t_arr_length )
        return t_arr_length + 1;
    if ( t_lbracket )
        return t_lbracket + 1;
    return 1;
}

bool ArrBoundAST::isNull() const
{
    if ( t_lbracket )
        return false;
    if ( t_arr_length )
        return false;
    if ( t_rbracket )
        return false;
    return true;
}

void ArrBoundAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int RowPrimaryExpressionAST::firstToken() const
{
    if ( row && !row->isNull() )
        return row->firstToken();
    return 0;
}

int RowPrimaryExpressionAST::lastToken() const
{
    if ( row && !row->isNull() )
        return row->lastToken();
    return 1;
}

bool RowPrimaryExpressionAST::isNull() const
{
    if ( row )
        return false;
    return true;
}

void RowPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(row, visitor);
    }
    visitor->endVisit(this);
}

int ArrayPrimaryExpressionAST::firstToken() const
{
    if ( t_array )
        return t_array;
    if ( arrExpr && !arrExpr->isNull() )
        return arrExpr->firstToken();
    return 0;
}

int ArrayPrimaryExpressionAST::lastToken() const
{
    if ( arrExpr && !arrExpr->isNull() )
        return arrExpr->lastToken();
    if ( t_array )
        return t_array + 1;
    return 1;
}

bool ArrayPrimaryExpressionAST::isNull() const
{
    if ( t_array )
        return false;
    if ( arrExpr )
        return false;
    return true;
}

void ArrayPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arrExpr, visitor);
    }
    visitor->endVisit(this);
}

int ExprListArrayExprAST::firstToken() const
{
    if ( t_lbracket )
        return t_lbracket;
    if ( values )
        return values->firstToken();
    if ( t_rbracket )
        return t_rbracket;
    return 0;
}

int ExprListArrayExprAST::lastToken() const
{
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( values )
        return values->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    return 1;
}

bool ExprListArrayExprAST::isNull() const
{
    if ( t_lbracket )
        return false;
    if ( values )
        return false;
    if ( t_rbracket )
        return false;
    return true;
}

void ExprListArrayExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int ArrListArrayExprAST::firstToken() const
{
    if ( t_lbracket )
        return t_lbracket;
    if ( arrList )
        return arrList->firstToken();
    if ( t_rbracket )
        return t_rbracket;
    return 0;
}

int ArrListArrayExprAST::lastToken() const
{
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( arrList )
        return arrList->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    return 1;
}

bool ArrListArrayExprAST::isNull() const
{
    if ( t_lbracket )
        return false;
    if ( arrList )
        return false;
    if ( t_rbracket )
        return false;
    return true;
}

void ArrListArrayExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arrList, visitor);
    }
    visitor->endVisit(this);
}

int EmptyArrayExprAST::firstToken() const
{
    if ( t_lbracket )
        return t_lbracket;
    if ( t_rbracket )
        return t_rbracket;
    return 0;
}

int EmptyArrayExprAST::lastToken() const
{
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( t_lbracket )
        return t_lbracket + 1;
    return 1;
}

bool EmptyArrayExprAST::isNull() const
{
    if ( t_lbracket )
        return false;
    if ( t_rbracket )
        return false;
    return true;
}

void EmptyArrayExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SubqueryPrimaryExpressionAST::firstToken() const
{
    if ( t_exists_or_array )
        return t_exists_or_array;
    if ( subquery && !subquery->isNull() )
        return subquery->firstToken();
    if ( qualifiers )
        return qualifiers->firstToken();
    return 0;
}

int SubqueryPrimaryExpressionAST::lastToken() const
{
    if ( qualifiers )
        return qualifiers->lastToken();
    if ( subquery && !subquery->isNull() )
        return subquery->lastToken();
    if ( t_exists_or_array )
        return t_exists_or_array + 1;
    return 1;
}

bool SubqueryPrimaryExpressionAST::isNull() const
{
    if ( t_exists_or_array )
        return false;
    if ( subquery )
        return false;
    if ( qualifiers )
        return false;
    return true;
}

void SubqueryPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(subquery, visitor);
        accept(qualifiers, visitor);
    }
    visitor->endVisit(this);
}

int FunctionPrimaryExpressionAST::firstToken() const
{
    if ( func && !func->isNull() )
        return func->firstToken();
    return 0;
}

int FunctionPrimaryExpressionAST::lastToken() const
{
    if ( func && !func->isNull() )
        return func->lastToken();
    return 1;
}

bool FunctionPrimaryExpressionAST::isNull() const
{
    if ( func )
        return false;
    return true;
}

void FunctionPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(func, visitor);
    }
    visitor->endVisit(this);
}

int ColRefPrimaryExpressionAST::firstToken() const
{
    if ( columnRef && !columnRef->isNull() )
        return columnRef->firstToken();
    return 0;
}

int ColRefPrimaryExpressionAST::lastToken() const
{
    if ( columnRef && !columnRef->isNull() )
        return columnRef->lastToken();
    return 1;
}

bool ColRefPrimaryExpressionAST::isNull() const
{
    if ( columnRef )
        return false;
    return true;
}

void ColRefPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnRef, visitor);
    }
    visitor->endVisit(this);
}

int ConstPrimaryExpressionAST::firstToken() const
{
    if ( value && !value->isNull() )
        return value->firstToken();
    return 0;
}

int ConstPrimaryExpressionAST::lastToken() const
{
    if ( value && !value->isNull() )
        return value->lastToken();
    return 1;
}

bool ConstPrimaryExpressionAST::isNull() const
{
    if ( value )
        return false;
    return true;
}

void ConstPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

int ParamPrimaryExpressionAST::firstToken() const
{
    if ( param && !param->isNull() )
        return param->firstToken();
    if ( qualifiers )
        return qualifiers->firstToken();
    return 0;
}

int ParamPrimaryExpressionAST::lastToken() const
{
    if ( qualifiers )
        return qualifiers->lastToken();
    if ( param && !param->isNull() )
        return param->lastToken();
    return 1;
}

bool ParamPrimaryExpressionAST::isNull() const
{
    if ( param )
        return false;
    if ( qualifiers )
        return false;
    return true;
}

void ParamPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(param, visitor);
        accept(qualifiers, visitor);
    }
    visitor->endVisit(this);
}

int CompoundPrimaryExpressionAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( qualifiers )
        return qualifiers->firstToken();
    return 0;
}

int CompoundPrimaryExpressionAST::lastToken() const
{
    if ( qualifiers )
        return qualifiers->lastToken();
    if ( t_rparen )
        return t_rparen + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool CompoundPrimaryExpressionAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( expr )
        return false;
    if ( t_rparen )
        return false;
    if ( qualifiers )
        return false;
    return true;
}

void CompoundPrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(qualifiers, visitor);
    }
    visitor->endVisit(this);
}

int CasePrimaryExpressionAST::firstToken() const
{
    if ( caseExpr && !caseExpr->isNull() )
        return caseExpr->firstToken();
    return 0;
}

int CasePrimaryExpressionAST::lastToken() const
{
    if ( caseExpr && !caseExpr->isNull() )
        return caseExpr->lastToken();
    return 1;
}

bool CasePrimaryExpressionAST::isNull() const
{
    if ( caseExpr )
        return false;
    return true;
}

void CasePrimaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(caseExpr, visitor);
    }
    visitor->endVisit(this);
}

int CaseExprAST::firstToken() const
{
    if ( t_case )
        return t_case;
    if ( arg && !arg->isNull() )
        return arg->firstToken();
    if ( whens )
        return whens->firstToken();
    if ( deaultClause && !deaultClause->isNull() )
        return deaultClause->firstToken();
    if ( t_end )
        return t_end;
    return 0;
}

int CaseExprAST::lastToken() const
{
    if ( t_end )
        return t_end + 1;
    if ( deaultClause && !deaultClause->isNull() )
        return deaultClause->lastToken();
    if ( whens )
        return whens->lastToken();
    if ( arg && !arg->isNull() )
        return arg->lastToken();
    if ( t_case )
        return t_case + 1;
    return 1;
}

bool CaseExprAST::isNull() const
{
    if ( t_case )
        return false;
    if ( arg )
        return false;
    if ( whens )
        return false;
    if ( deaultClause )
        return false;
    if ( t_end )
        return false;
    return true;
}

void CaseExprAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(arg, visitor);
        accept(whens, visitor);
        accept(deaultClause, visitor);
    }
    visitor->endVisit(this);
}

int CaseArgAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int CaseArgAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool CaseArgAST::isNull() const
{
    if ( expr )
        return false;
    return true;
}

void CaseArgAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CaseWhenClauseAST::firstToken() const
{
    if ( t_when )
        return t_when;
    if ( whernExpr && !whernExpr->isNull() )
        return whernExpr->firstToken();
    if ( t_then )
        return t_then;
    if ( thenExpr && !thenExpr->isNull() )
        return thenExpr->firstToken();
    return 0;
}

int CaseWhenClauseAST::lastToken() const
{
    if ( thenExpr && !thenExpr->isNull() )
        return thenExpr->lastToken();
    if ( t_then )
        return t_then + 1;
    if ( whernExpr && !whernExpr->isNull() )
        return whernExpr->lastToken();
    if ( t_when )
        return t_when + 1;
    return 1;
}

bool CaseWhenClauseAST::isNull() const
{
    if ( t_when )
        return false;
    if ( whernExpr )
        return false;
    if ( t_then )
        return false;
    if ( thenExpr )
        return false;
    return true;
}

void CaseWhenClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(whernExpr, visitor);
        accept(thenExpr, visitor);
    }
    visitor->endVisit(this);
}

int CaseDefaultClauseAST::firstToken() const
{
    if ( t_else )
        return t_else;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int CaseDefaultClauseAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_else )
        return t_else + 1;
    return 1;
}

bool CaseDefaultClauseAST::isNull() const
{
    if ( t_else )
        return false;
    if ( expr )
        return false;
    return true;
}

void CaseDefaultClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int SimpleConstantAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

int SimpleConstantAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

bool SimpleConstantAST::isNull() const
{
    if ( t_value )
        return false;
    return true;
}

void SimpleConstantAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int FuncNameConstantAST::firstToken() const
{
    if ( funcRef && !funcRef->isNull() )
        return funcRef->firstToken();
    if ( t_lparen )
        return t_lparen;
    if ( args )
        return args->firstToken();
    if ( t_rparen )
        return t_rparen;
    if ( t_value )
        return t_value;
    return 0;
}

int FuncNameConstantAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( args )
        return args->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( funcRef && !funcRef->isNull() )
        return funcRef->lastToken();
    return 1;
}

bool FuncNameConstantAST::isNull() const
{
    if ( funcRef )
        return false;
    if ( t_lparen )
        return false;
    if ( args )
        return false;
    if ( t_rparen )
        return false;
    if ( t_value )
        return false;
    return true;
}

void FuncNameConstantAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(funcRef, visitor);
        accept(args, visitor);
    }
    visitor->endVisit(this);
}

int IntervalConstantAST::firstToken() const
{
    if ( t_interval )
        return t_interval;
    if ( t_lparen )
        return t_lparen;
    if ( t_val )
        return t_val;
    if ( t_rparen )
        return t_rparen;
    if ( t_sval )
        return t_sval;
    if ( interval && !interval->isNull() )
        return interval->firstToken();
    return 0;
}

int IntervalConstantAST::lastToken() const
{
    if ( interval && !interval->isNull() )
        return interval->lastToken();
    if ( t_sval )
        return t_sval + 1;
    if ( t_rparen )
        return t_rparen + 1;
    if ( t_val )
        return t_val + 1;
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_interval )
        return t_interval + 1;
    return 1;
}

bool IntervalConstantAST::isNull() const
{
    if ( t_interval )
        return false;
    if ( t_lparen )
        return false;
    if ( t_val )
        return false;
    if ( t_rparen )
        return false;
    if ( t_sval )
        return false;
    if ( interval )
        return false;
    return true;
}

void IntervalConstantAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(interval, visitor);
    }
    visitor->endVisit(this);
}

int TypenameConstantAST::firstToken() const
{
    if ( typeName && !typeName->isNull() )
        return typeName->firstToken();
    if ( t_value )
        return t_value;
    return 0;
}

int TypenameConstantAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    if ( typeName && !typeName->isNull() )
        return typeName->lastToken();
    return 1;
}

bool TypenameConstantAST::isNull() const
{
    if ( typeName )
        return false;
    if ( t_value )
        return false;
    return true;
}

void TypenameConstantAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
    }
    visitor->endVisit(this);
}

int TypecastExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_dotdot )
        return t_dotdot;
    if ( type && !type->isNull() )
        return type->firstToken();
    return 0;
}

int TypecastExpressionAST::lastToken() const
{
    if ( type && !type->isNull() )
        return type->lastToken();
    if ( t_dotdot )
        return t_dotdot + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool TypecastExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_dotdot )
        return false;
    if ( type )
        return false;
    return true;
}

void TypecastExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

int CollateExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_collate )
        return t_collate;
    if ( value && !value->isNull() )
        return value->firstToken();
    return 0;
}

int CollateExpressionAST::lastToken() const
{
    if ( value && !value->isNull() )
        return value->lastToken();
    if ( t_collate )
        return t_collate + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool CollateExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_collate )
        return false;
    if ( value )
        return false;
    return true;
}

void CollateExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

int AtTimeZoneExpressionAST::firstToken() const
{
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( t_at )
        return t_at;
    if ( t_time )
        return t_time;
    if ( t_zone )
        return t_zone;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    return 0;
}

int AtTimeZoneExpressionAST::lastToken() const
{
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( t_zone )
        return t_zone + 1;
    if ( t_time )
        return t_time + 1;
    if ( t_at )
        return t_at + 1;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    return 1;
}

bool AtTimeZoneExpressionAST::isNull() const
{
    if ( lExpr )
        return false;
    if ( t_at )
        return false;
    if ( t_time )
        return false;
    if ( t_zone )
        return false;
    if ( rExpr )
        return false;
    return true;
}

void AtTimeZoneExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lExpr, visitor);
        accept(rExpr, visitor);
    }
    visitor->endVisit(this);
}

int UnaryExpressionAST::firstToken() const
{
    if ( t_op )
        return t_op;
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int UnaryExpressionAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( t_op )
        return t_op + 1;
    return 1;
}

bool UnaryExpressionAST::isNull() const
{
    if ( t_op )
        return false;
    if ( expr )
        return false;
    return true;
}

void UnaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int BinaryExpressionAST::firstToken() const
{
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( t_op )
        return t_op;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    return 0;
}

int BinaryExpressionAST::lastToken() const
{
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( t_op )
        return t_op + 1;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    return 1;
}

bool BinaryExpressionAST::isNull() const
{
    if ( lExpr )
        return false;
    if ( t_op )
        return false;
    if ( rExpr )
        return false;
    return true;
}

void BinaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lExpr, visitor);
        accept(rExpr, visitor);
    }
    visitor->endVisit(this);
}

int CommonBinaryExpressionAST::firstToken() const
{
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( commonOp && !commonOp->isNull() )
        return commonOp->firstToken();
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    return 0;
}

int CommonBinaryExpressionAST::lastToken() const
{
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( commonOp && !commonOp->isNull() )
        return commonOp->lastToken();
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    return 1;
}

bool CommonBinaryExpressionAST::isNull() const
{
    if ( lExpr )
        return false;
    if ( commonOp )
        return false;
    if ( rExpr )
        return false;
    return true;
}

void CommonBinaryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lExpr, visitor);
        accept(commonOp, visitor);
        accept(rExpr, visitor);
    }
    visitor->endVisit(this);
}

int CommonUnaryExpresionAST::firstToken() const
{
    if ( commonOp && !commonOp->isNull() )
        return commonOp->firstToken();
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    return 0;
}

int CommonUnaryExpresionAST::lastToken() const
{
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    if ( commonOp && !commonOp->isNull() )
        return commonOp->lastToken();
    return 1;
}

bool CommonUnaryExpresionAST::isNull() const
{
    if ( commonOp )
        return false;
    if ( expr )
        return false;
    return true;
}

void CommonUnaryExpresionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(commonOp, visitor);
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int CommonPostfixExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( commonOp && !commonOp->isNull() )
        return commonOp->firstToken();
    return 0;
}

int CommonPostfixExpressionAST::lastToken() const
{
    if ( commonOp && !commonOp->isNull() )
        return commonOp->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool CommonPostfixExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( commonOp )
        return false;
    return true;
}

void CommonPostfixExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(commonOp, visitor);
    }
    visitor->endVisit(this);
}

int LikeExpressionAST::firstToken() const
{
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( t_not )
        return t_not;
    if ( t_like )
        return t_like;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    if ( t_escape )
        return t_escape;
    if ( escExpr && !escExpr->isNull() )
        return escExpr->firstToken();
    return 0;
}

int LikeExpressionAST::lastToken() const
{
    if ( escExpr && !escExpr->isNull() )
        return escExpr->lastToken();
    if ( t_escape )
        return t_escape + 1;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( t_like )
        return t_like + 1;
    if ( t_not )
        return t_not + 1;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    return 1;
}

bool LikeExpressionAST::isNull() const
{
    if ( lExpr )
        return false;
    if ( t_not )
        return false;
    if ( t_like )
        return false;
    if ( rExpr )
        return false;
    if ( t_escape )
        return false;
    if ( escExpr )
        return false;
    return true;
}

void LikeExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lExpr, visitor);
        accept(rExpr, visitor);
        accept(escExpr, visitor);
    }
    visitor->endVisit(this);
}

int SimilarToExpressionAST::firstToken() const
{
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( t_not )
        return t_not;
    if ( t_similar )
        return t_similar;
    if ( t_to )
        return t_to;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    if ( t_excape )
        return t_excape;
    if ( escExpr && !escExpr->isNull() )
        return escExpr->firstToken();
    return 0;
}

int SimilarToExpressionAST::lastToken() const
{
    if ( escExpr && !escExpr->isNull() )
        return escExpr->lastToken();
    if ( t_excape )
        return t_excape + 1;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( t_to )
        return t_to + 1;
    if ( t_similar )
        return t_similar + 1;
    if ( t_not )
        return t_not + 1;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    return 1;
}

bool SimilarToExpressionAST::isNull() const
{
    if ( lExpr )
        return false;
    if ( t_not )
        return false;
    if ( t_similar )
        return false;
    if ( t_to )
        return false;
    if ( rExpr )
        return false;
    if ( t_excape )
        return false;
    if ( escExpr )
        return false;
    return true;
}

void SimilarToExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lExpr, visitor);
        accept(rExpr, visitor);
        accept(escExpr, visitor);
    }
    visitor->endVisit(this);
}

int IsExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_is )
        return t_is;
    if ( t_not )
        return t_not;
    if ( t_something )
        return t_something;
    return 0;
}

int IsExpressionAST::lastToken() const
{
    if ( t_something )
        return t_something + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_is )
        return t_is + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool IsExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_is )
        return false;
    if ( t_not )
        return false;
    if ( t_something )
        return false;
    return true;
}

void IsExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
    }
    visitor->endVisit(this);
}

int IsDdistinctExpressionAST::firstToken() const
{
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( t_is )
        return t_is;
    if ( t_not )
        return t_not;
    if ( t_distinct )
        return t_distinct;
    if ( t_from )
        return t_from;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    return 0;
}

int IsDdistinctExpressionAST::lastToken() const
{
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( t_from )
        return t_from + 1;
    if ( t_distinct )
        return t_distinct + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_is )
        return t_is + 1;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    return 1;
}

bool IsDdistinctExpressionAST::isNull() const
{
    if ( lExpr )
        return false;
    if ( t_is )
        return false;
    if ( t_not )
        return false;
    if ( t_distinct )
        return false;
    if ( t_from )
        return false;
    if ( rExpr )
        return false;
    return true;
}

void IsDdistinctExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lExpr, visitor);
        accept(rExpr, visitor);
    }
    visitor->endVisit(this);
}

int IsTypeofExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_is )
        return t_is;
    if ( t_not )
        return t_not;
    if ( t_of )
        return t_of;
    if ( t_lparen )
        return t_lparen;
    if ( types )
        return types->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int IsTypeofExpressionAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( types )
        return types->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_of )
        return t_of + 1;
    if ( t_not )
        return t_not + 1;
    if ( t_is )
        return t_is + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool IsTypeofExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_is )
        return false;
    if ( t_not )
        return false;
    if ( t_of )
        return false;
    if ( t_lparen )
        return false;
    if ( types )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void IsTypeofExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(types, visitor);
    }
    visitor->endVisit(this);
}

int BetweenExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_not )
        return t_not;
    if ( t_between )
        return t_between;
    if ( t_symmetric_spec )
        return t_symmetric_spec;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->firstToken();
    if ( t_and )
        return t_and;
    if ( rExpr && !rExpr->isNull() )
        return rExpr->firstToken();
    return 0;
}

int BetweenExpressionAST::lastToken() const
{
    if ( rExpr && !rExpr->isNull() )
        return rExpr->lastToken();
    if ( t_and )
        return t_and + 1;
    if ( lExpr && !lExpr->isNull() )
        return lExpr->lastToken();
    if ( t_symmetric_spec )
        return t_symmetric_spec + 1;
    if ( t_between )
        return t_between + 1;
    if ( t_not )
        return t_not + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool BetweenExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_not )
        return false;
    if ( t_between )
        return false;
    if ( t_symmetric_spec )
        return false;
    if ( lExpr )
        return false;
    if ( t_and )
        return false;
    if ( rExpr )
        return false;
    return true;
}

void BetweenExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(lExpr, visitor);
        accept(rExpr, visitor);
    }
    visitor->endVisit(this);
}

int InExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( t_not )
        return t_not;
    if ( t_in )
        return t_in;
    if ( values && !values->isNull() )
        return values->firstToken();
    return 0;
}

int InExpressionAST::lastToken() const
{
    if ( values && !values->isNull() )
        return values->lastToken();
    if ( t_in )
        return t_in + 1;
    if ( t_not )
        return t_not + 1;
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool InExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( t_not )
        return false;
    if ( t_in )
        return false;
    if ( values )
        return false;
    return true;
}

void InExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int InValuesAsQueryAST::firstToken() const
{
    if ( query && !query->isNull() )
        return query->firstToken();
    return 0;
}

int InValuesAsQueryAST::lastToken() const
{
    if ( query && !query->isNull() )
        return query->lastToken();
    return 1;
}

bool InValuesAsQueryAST::isNull() const
{
    if ( query )
        return false;
    return true;
}

void InValuesAsQueryAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(query, visitor);
    }
    visitor->endVisit(this);
}

int InValuesAsExprListAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( values )
        return values->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int InValuesAsExprListAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( values )
        return values->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

bool InValuesAsExprListAST::isNull() const
{
    if ( t_lparen )
        return false;
    if ( values )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void InValuesAsExprListAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int RelationExprSimpleAST::firstToken() const
{
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_star )
        return t_star;
    return 0;
}

int RelationExprSimpleAST::lastToken() const
{
    if ( t_star )
        return t_star + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    return 1;
}

bool RelationExprSimpleAST::isNull() const
{
    if ( tableIdent )
        return false;
    if ( t_star )
        return false;
    return true;
}

void RelationExprSimpleAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int SubqueryExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( subqueryOp && !subqueryOp->isNull() )
        return subqueryOp->firstToken();
    if ( t_sub_type )
        return t_sub_type;
    if ( query && !query->isNull() )
        return query->firstToken();
    return 0;
}

int SubqueryExpressionAST::lastToken() const
{
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_sub_type )
        return t_sub_type + 1;
    if ( subqueryOp && !subqueryOp->isNull() )
        return subqueryOp->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool SubqueryExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( subqueryOp )
        return false;
    if ( t_sub_type )
        return false;
    if ( query )
        return false;
    return true;
}

void SubqueryExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(subqueryOp, visitor);
        accept(query, visitor);
    }
    visitor->endVisit(this);
}

int SubExpressionExpressionAST::firstToken() const
{
    if ( expr && !expr->isNull() )
        return expr->firstToken();
    if ( subqueryOp && !subqueryOp->isNull() )
        return subqueryOp->firstToken();
    if ( t_sub_type )
        return t_sub_type;
    if ( t_lparen )
        return t_lparen;
    if ( subExpr && !subExpr->isNull() )
        return subExpr->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int SubExpressionExpressionAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( subExpr && !subExpr->isNull() )
        return subExpr->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_sub_type )
        return t_sub_type + 1;
    if ( subqueryOp && !subqueryOp->isNull() )
        return subqueryOp->lastToken();
    if ( expr && !expr->isNull() )
        return expr->lastToken();
    return 1;
}

bool SubExpressionExpressionAST::isNull() const
{
    if ( expr )
        return false;
    if ( subqueryOp )
        return false;
    if ( t_sub_type )
        return false;
    if ( t_lparen )
        return false;
    if ( subExpr )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void SubExpressionExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr, visitor);
        accept(subqueryOp, visitor);
        accept(subExpr, visitor);
    }
    visitor->endVisit(this);
}

int RowOverlapsExpressionAST::firstToken() const
{
    if ( lRow && !lRow->isNull() )
        return lRow->firstToken();
    if ( t_overlaps )
        return t_overlaps;
    if ( rRow && !rRow->isNull() )
        return rRow->firstToken();
    return 0;
}

int RowOverlapsExpressionAST::lastToken() const
{
    if ( rRow && !rRow->isNull() )
        return rRow->lastToken();
    if ( t_overlaps )
        return t_overlaps + 1;
    if ( lRow && !lRow->isNull() )
        return lRow->lastToken();
    return 1;
}

bool RowOverlapsExpressionAST::isNull() const
{
    if ( lRow )
        return false;
    if ( t_overlaps )
        return false;
    if ( rRow )
        return false;
    return true;
}

void RowOverlapsExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lRow, visitor);
        accept(rRow, visitor);
    }
    visitor->endVisit(this);
}

int UniqueExpressionAST::firstToken() const
{
    if ( t_unique )
        return t_unique;
    if ( query && !query->isNull() )
        return query->firstToken();
    return 0;
}

int UniqueExpressionAST::lastToken() const
{
    if ( query && !query->isNull() )
        return query->lastToken();
    if ( t_unique )
        return t_unique + 1;
    return 1;
}

bool UniqueExpressionAST::isNull() const
{
    if ( t_unique )
        return false;
    if ( query )
        return false;
    return true;
}

void UniqueExpressionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(query, visitor);
    }
    visitor->endVisit(this);
}

int RowAST::firstToken() const
{
    if ( t_row )
        return t_row;
    if ( t_lparen )
        return t_lparen;
    if ( values )
        return values->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int RowAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( values )
        return values->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_row )
        return t_row + 1;
    return 1;
}

bool RowAST::isNull() const
{
    if ( t_row )
        return false;
    if ( t_lparen )
        return false;
    if ( values )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void RowAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(values, visitor);
    }
    visitor->endVisit(this);
}

int RelationExprOnlyAST::firstToken() const
{
    if ( t_only )
        return t_only;
    if ( t_lparen )
        return t_lparen;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int RelationExprOnlyAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( tableIdent && !tableIdent->isNull() )
        return tableIdent->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_only )
        return t_only + 1;
    return 1;
}

bool RelationExprOnlyAST::isNull() const
{
    if ( t_only )
        return false;
    if ( t_lparen )
        return false;
    if ( tableIdent )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void RelationExprOnlyAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(tableIdent, visitor);
    }
    visitor->endVisit(this);
}

int QualSubqueryOperatorAST::firstToken() const
{
    if ( qualOp && !qualOp->isNull() )
        return qualOp->firstToken();
    return 0;
}

int QualSubqueryOperatorAST::lastToken() const
{
    if ( qualOp && !qualOp->isNull() )
        return qualOp->lastToken();
    return 1;
}

bool QualSubqueryOperatorAST::isNull() const
{
    if ( qualOp )
        return false;
    return true;
}

void QualSubqueryOperatorAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(qualOp, visitor);
    }
    visitor->endVisit(this);
}

int LikeSubqueryOperatorAST::firstToken() const
{
    if ( t_not )
        return t_not;
    if ( t_like_or_ilike )
        return t_like_or_ilike;
    return 0;
}

int LikeSubqueryOperatorAST::lastToken() const
{
    if ( t_like_or_ilike )
        return t_like_or_ilike + 1;
    if ( t_not )
        return t_not + 1;
    return 1;
}

bool LikeSubqueryOperatorAST::isNull() const
{
    if ( t_not )
        return false;
    if ( t_like_or_ilike )
        return false;
    return true;
}

void LikeSubqueryOperatorAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int SimpleQualifiedOperatorAST::firstToken() const
{
    if ( t_op )
        return t_op;
    return 0;
}

int SimpleQualifiedOperatorAST::lastToken() const
{
    if ( t_op )
        return t_op + 1;
    return 1;
}

bool SimpleQualifiedOperatorAST::isNull() const
{
    if ( t_op )
        return false;
    return true;
}

void SimpleQualifiedOperatorAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int CompoundQualifiedOperatorAST::firstToken() const
{
    if ( t_operator )
        return t_operator;
    if ( t_lparen )
        return t_lparen;
    if ( oper && !oper->isNull() )
        return oper->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

int CompoundQualifiedOperatorAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( oper && !oper->isNull() )
        return oper->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    if ( t_operator )
        return t_operator + 1;
    return 1;
}

bool CompoundQualifiedOperatorAST::isNull() const
{
    if ( t_operator )
        return false;
    if ( t_lparen )
        return false;
    if ( oper )
        return false;
    if ( t_rparen )
        return false;
    return true;
}

void CompoundQualifiedOperatorAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(oper, visitor);
    }
    visitor->endVisit(this);
}

int OperandAST::firstToken() const
{
    if ( columnIdent && !columnIdent->isNull() )
        return columnIdent->firstToken();
    if ( t_dot )
        return t_dot;
    if ( t_op )
        return t_op;
    return 0;
}

int OperandAST::lastToken() const
{
    if ( t_op )
        return t_op + 1;
    if ( t_dot )
        return t_dot + 1;
    if ( columnIdent && !columnIdent->isNull() )
        return columnIdent->lastToken();
    return 1;
}

bool OperandAST::isNull() const
{
    if ( columnIdent )
        return false;
    if ( t_dot )
        return false;
    if ( t_op )
        return false;
    return true;
}

void OperandAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(columnIdent, visitor);
    }
    visitor->endVisit(this);
}

int AnyIdentifierAST::firstToken() const
{
    if ( name && !name->isNull() )
        return name->firstToken();
    if ( attributes )
        return attributes->firstToken();
    return 0;
}

int AnyIdentifierAST::lastToken() const
{
    if ( attributes )
        return attributes->lastToken();
    if ( name && !name->isNull() )
        return name->lastToken();
    return 1;
}

bool AnyIdentifierAST::isNull() const
{
    if ( name )
        return false;
    if ( attributes )
        return false;
    return true;
}

void AnyIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(name, visitor);
        accept(attributes, visitor);
    }
    visitor->endVisit(this);
}

int TableIdentifierAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_dot )
        return t_dot;
    if ( tableName && !tableName->isNull() )
        return tableName->firstToken();
    return 0;
}

int TableIdentifierAST::lastToken() const
{
    if ( tableName && !tableName->isNull() )
        return tableName->lastToken();
    if ( t_dot )
        return t_dot + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool TableIdentifierAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_dot )
        return false;
    if ( tableName )
        return false;
    return true;
}

void TableIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(tableName, visitor);
    }
    visitor->endVisit(this);
}

int TriggerIdentifierAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_dot )
        return t_dot;
    if ( triggerName && !triggerName->isNull() )
        return triggerName->firstToken();
    return 0;
}

int TriggerIdentifierAST::lastToken() const
{
    if ( triggerName && !triggerName->isNull() )
        return triggerName->lastToken();
    if ( t_dot )
        return t_dot + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool TriggerIdentifierAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_dot )
        return false;
    if ( triggerName )
        return false;
    return true;
}

void TriggerIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(triggerName, visitor);
    }
    visitor->endVisit(this);
}

int IndexIdentifierAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_dot )
        return t_dot;
    if ( indexName && !indexName->isNull() )
        return indexName->firstToken();
    return 0;
}

int IndexIdentifierAST::lastToken() const
{
    if ( indexName && !indexName->isNull() )
        return indexName->lastToken();
    if ( t_dot )
        return t_dot + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool IndexIdentifierAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_dot )
        return false;
    if ( indexName )
        return false;
    return true;
}

void IndexIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(indexName, visitor);
    }
    visitor->endVisit(this);
}

int ConstraintIdentifierAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_dot )
        return t_dot;
    if ( constraintName && !constraintName->isNull() )
        return constraintName->firstToken();
    return 0;
}

int ConstraintIdentifierAST::lastToken() const
{
    if ( constraintName && !constraintName->isNull() )
        return constraintName->lastToken();
    if ( t_dot )
        return t_dot + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool ConstraintIdentifierAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_dot )
        return false;
    if ( constraintName )
        return false;
    return true;
}

void ConstraintIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(constraintName, visitor);
    }
    visitor->endVisit(this);
}

int SequenceIdentifierAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_dot )
        return t_dot;
    if ( sequenceName && !sequenceName->isNull() )
        return sequenceName->firstToken();
    return 0;
}

int SequenceIdentifierAST::lastToken() const
{
    if ( sequenceName && !sequenceName->isNull() )
        return sequenceName->lastToken();
    if ( t_dot )
        return t_dot + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool SequenceIdentifierAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_dot )
        return false;
    if ( sequenceName )
        return false;
    return true;
}

void SequenceIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(sequenceName, visitor);
    }
    visitor->endVisit(this);
}

int ColumnIdentifierAST::firstToken() const
{
    if ( schemaName && !schemaName->isNull() )
        return schemaName->firstToken();
    if ( t_dot1 )
        return t_dot1;
    if ( tableName && !tableName->isNull() )
        return tableName->firstToken();
    if ( t_dot2 )
        return t_dot2;
    if ( columnName && !columnName->isNull() )
        return columnName->firstToken();
    return 0;
}

int ColumnIdentifierAST::lastToken() const
{
    if ( columnName && !columnName->isNull() )
        return columnName->lastToken();
    if ( t_dot2 )
        return t_dot2 + 1;
    if ( tableName && !tableName->isNull() )
        return tableName->lastToken();
    if ( t_dot1 )
        return t_dot1 + 1;
    if ( schemaName && !schemaName->isNull() )
        return schemaName->lastToken();
    return 1;
}

bool ColumnIdentifierAST::isNull() const
{
    if ( schemaName )
        return false;
    if ( t_dot1 )
        return false;
    if ( tableName )
        return false;
    if ( t_dot2 )
        return false;
    if ( columnName )
        return false;
    return true;
}

void ColumnIdentifierAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(schemaName, visitor);
        accept(tableName, visitor);
        accept(columnName, visitor);
    }
    visitor->endVisit(this);
}

int ColumnRefAST::firstToken() const
{
    if ( firstName && !firstName->isNull() )
        return firstName->firstToken();
    if ( qualifiers )
        return qualifiers->firstToken();
    return 0;
}

int ColumnRefAST::lastToken() const
{
    if ( qualifiers )
        return qualifiers->lastToken();
    if ( firstName && !firstName->isNull() )
        return firstName->lastToken();
    return 1;
}

bool ColumnRefAST::isNull() const
{
    if ( firstName )
        return false;
    if ( qualifiers )
        return false;
    return true;
}

void ColumnRefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(firstName, visitor);
        accept(qualifiers, visitor);
    }
    visitor->endVisit(this);
}

int FunctionRefAST::firstToken() const
{
    if ( firstName && !firstName->isNull() )
        return firstName->firstToken();
    if ( qualifiers )
        return qualifiers->firstToken();
    return 0;
}

int FunctionRefAST::lastToken() const
{
    if ( qualifiers )
        return qualifiers->lastToken();
    if ( firstName && !firstName->isNull() )
        return firstName->lastToken();
    return 1;
}

bool FunctionRefAST::isNull() const
{
    if ( firstName )
        return false;
    if ( qualifiers )
        return false;
    return true;
}

void FunctionRefAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(firstName, visitor);
        accept(qualifiers, visitor);
    }
    visitor->endVisit(this);
}

int AttirbuteIndirectionAST::firstToken() const
{
    if ( t_dot )
        return t_dot;
    if ( attrName && !attrName->isNull() )
        return attrName->firstToken();
    return 0;
}

int AttirbuteIndirectionAST::lastToken() const
{
    if ( attrName && !attrName->isNull() )
        return attrName->lastToken();
    if ( t_dot )
        return t_dot + 1;
    return 1;
}

bool AttirbuteIndirectionAST::isNull() const
{
    if ( t_dot )
        return false;
    if ( attrName )
        return false;
    return true;
}

void AttirbuteIndirectionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(attrName, visitor);
    }
    visitor->endVisit(this);
}

int StarIndirectionAST::firstToken() const
{
    if ( t_dot )
        return t_dot;
    if ( t_star )
        return t_star;
    return 0;
}

int StarIndirectionAST::lastToken() const
{
    if ( t_star )
        return t_star + 1;
    if ( t_dot )
        return t_dot + 1;
    return 1;
}

bool StarIndirectionAST::isNull() const
{
    if ( t_dot )
        return false;
    if ( t_star )
        return false;
    return true;
}

void StarIndirectionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

int IndexingIndirectionAST::firstToken() const
{
    if ( t_lbracket )
        return t_lbracket;
    if ( expr1 && !expr1->isNull() )
        return expr1->firstToken();
    if ( t_comma )
        return t_comma;
    if ( expr2 && !expr2->isNull() )
        return expr2->firstToken();
    if ( t_rpbracket )
        return t_rpbracket;
    return 0;
}

int IndexingIndirectionAST::lastToken() const
{
    if ( t_rpbracket )
        return t_rpbracket + 1;
    if ( expr2 && !expr2->isNull() )
        return expr2->lastToken();
    if ( t_comma )
        return t_comma + 1;
    if ( expr1 && !expr1->isNull() )
        return expr1->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    return 1;
}

bool IndexingIndirectionAST::isNull() const
{
    if ( t_lbracket )
        return false;
    if ( expr1 )
        return false;
    if ( t_comma )
        return false;
    if ( expr2 )
        return false;
    if ( t_rpbracket )
        return false;
    return true;
}

void IndexingIndirectionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(expr1, visitor);
        accept(expr2, visitor);
    }
    visitor->endVisit(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SqlCheckNamesWalker::walk( Sql::BaseWalkReflector *reflector) {
    if ( m_ast ) {
        SqlCheckNamesVisitor visitor(m_translationUnit, reflector);
        m_ast->accept(&visitor);
    }
}

}   // namespace Postgres
