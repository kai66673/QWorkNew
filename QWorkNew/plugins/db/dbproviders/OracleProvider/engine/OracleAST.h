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

#ifndef ORACLE_AST_H
#define ORACLE_AST_H

#include <QString>

#include "OracleASTfwd.h"
#include "MemoryPool.h"
#include "TranslationUnit.h"
#include "SqlToken.h"
#include "BaseWalker.h"
#include "sql_constants.h"

namespace Oracle {

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
        : value(value), next(0)
    { }

    unsigned firstToken() const
    {
        if (value)
            return value->firstToken();

        // ### assert(0);
        return 0;
    }

    unsigned lastToken() const
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

    virtual unsigned firstToken() const = 0;
    virtual unsigned lastToken() const = 0;

//    virtual AST *clone(Utils::MemoryPool *pool) const = 0;

    virtual TranslationUnitAST *asTranslationUnit() { return 0; }
    virtual StatementAST *asStatement() { return 0; }
    virtual InvalidStatementAST *asInvalidStatement() { return 0; }
    virtual UnknownStatementAST *asUnknownStatement() { return 0; }
    virtual DmlStatementAST *asDmlStatement() { return 0; }
    virtual SelectStatementAST *asSelectStatement() { return 0; }
    virtual SelectCoreAST *asSelectCore() { return 0; }
    virtual SelectCompoundAST *asSelectCompound() { return 0; }
    virtual SelectCompoundUnionAST *asSelectCompoundUnion() { return 0; }
    virtual SelectCompoundIntersectAST *asSelectCompoundIntersect() { return 0; }
    virtual SelectCompoundExceptAST *asSelectCompoundExcept() { return 0; }
    virtual SelectCompoundMinusAST *asSelectCompoundMinus() { return 0; }
    virtual OrderByBlockAST *asOrderByBlock() { return 0; }
    virtual LimitToBlockAST *asLimitToBlock() { return 0; }
    virtual OrderingTermAST *asOrderingTerm() { return 0; }
    virtual LabelAST *asLabel() { return 0; }
    virtual NameAST *asName() { return 0; }
    virtual SchemaNameAST *asSchemaName() { return 0; }
    virtual TableNameAST *asTableName() { return 0; }
    virtual ColumnAliasAST *asColumnAlias() { return 0; }
    virtual TableAliasAST *asTableAlias() { return 0; }
    virtual ColumnNameAST *asColumnName() { return 0; }
    virtual FunctionNameAST *asFunctionName() { return 0; }
    virtual BindingNameAST *asParamName() { return 0; }
    virtual LabelNameAST *asLabelName() { return 0; }
    virtual ViewNameAST *asViewName() { return 0; }
    virtual IndexNameAST *asIndexName() { return 0; }
    virtual TriggerNameAST *asTriggerName() { return 0; }
    virtual ConstraintNameAST *asConstraintName() { return 0; }
    virtual PlSqlBaseNameAST *asPlSqlBaseName() { return 0; }
    virtual PlSqlNameAST *asPlSqlName() { return 0; }
    virtual PlSqlTypeNameAST *asPlSqlTypeName() { return 0; }
    virtual SchemaTableNameAST *asSchemaTableName() { return 0; }
    virtual SchemaIndexNameAST *asSchemaIndexName() { return 0; }
    virtual SchemaTriggerNameAST *asSchemaTriggerName() { return 0; }
    virtual ResultColumnAST *asResultColumn() { return 0; }
    virtual StarResultColumnAST *asStarResultColumn() { return 0; }
    virtual ExprResultColumnAST *asExprResultColumn() { return 0; }
    virtual SubQueryAST *asSubQuery() { return 0; }
    virtual SingleSourceAST *asSingleSource() { return 0; }
    virtual DirectSingleSourceAST *asDirectSingleSource() { return 0; }
    virtual SubQuerySingleSourceAST *asSubQuerySingleSource() { return 0; }
    virtual JoinOpAST *asJoinOp() { return 0; }
    virtual JoinConstraintAST *asJoinConstraint() { return 0; }
    virtual JoinItemAST *asJoinItem() { return 0; }
    virtual JoinSourceAST *asJoinSource() { return 0; }
    virtual GroupByBlockAST *asGroupByBlock() { return 0; }

    virtual DeleteStatementAST *asDeleteStatement() { return 0; }
    virtual InsertStatementAST *asInsertStatement() { return 0; }
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
    virtual ColumnDefinitionAST *asColumnDefinition() { return 0; }
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

    virtual DdlStatementAST *asDdlStatement() { return 0; }
    virtual CreateStatementAST *asCreateStatement() { return 0; }
    virtual CreateTableStatementAST *asCreateTableStatement() { return 0; }
    virtual CreateViewStatementAST *asCreateViewStatement() { return 0; }
    virtual CreateIndexStatementAST *asCreateIndexStatement() { return 0; }
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
    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return 0; }
    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return 0; }
    virtual SimpleValueExpressionAST *asSimpleValueExpression() { return 0; }
    virtual ConstantExpressionAST *asConstantExpression() { return 0; }
    virtual ColumnExpressionAST *asColumnExpression() { return 0; }
    virtual LiteralExpressionAST *asLiteralExpression() { return 0; }
    virtual NumericExpressionAST *asNumericExpression() { return 0; }
    virtual FunctionExpressionAST *asFunctionExpression() { return 0; }
    virtual CastFunctionAST *asCastFunction() { return 0; }
    virtual SubstringFunctionAST *asSubstringFunction() { return 0; }
    virtual CountFunctionAST *asCountFunction() { return 0; }
    virtual ParameterExpressionAST *asParameterExpression() { return 0; }
    virtual NullValueExpressionAST *asNullValueExpression() { return 0; }
    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return 0; }
    virtual CompoundRValueExpressionAST *asCompoundRValueExpression() { return 0; }
    virtual ValueListExpressionAST *asValueListExpression() { return 0; }
    virtual SubQueryExpressionAST *asSubQueryExpression() { return 0; }
    virtual ConditionExpressionAST *asConditionExpression() { return 0; }
    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return 0; }
    virtual InConditionExpressionAST *asInConditionExpression() { return 0; }
    virtual LikeConditionExpressionAST *asLikeConditionExpression() { return 0; }
    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return 0; }
    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return 0; }
    virtual UnaryConditionExpressionAST *asUnaryConditionExpression() { return 0; }
    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return 0; }
    virtual AssignmentExpressionAST *asAssignmentExpression() { return 0; }
    virtual CaseExpressionAST *asCaseExpression() { return 0; }
    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return 0; }
    virtual CaseConditionAST *asCaseCondition() { return 0; }
    virtual CaseConditionClauseAST *asCaseConditionClause() { return 0; }

    // PL-SQL
    virtual PlSqlBlockAST *asPlSqlBlock() { return 0; }
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

    // Additional for SelectStatement and InvalidSelectStatement
    virtual bool isSelectStatement() const { return false; }

protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
};

class TranslationUnitAST: public AST
{
public:
    StatementListAST *statement_list;

public:
    TranslationUnitAST()
        : statement_list(0)
    {}

    virtual TranslationUnitAST *asTranslationUnit() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual void skip( Parser *p );

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

    virtual unsigned firstToken() const { return first_token; }
    virtual unsigned lastToken() const { return first_token; }

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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
    SelectCoreListAST *select_core_list;
    SelectCompoundListAST *select_compound_list;
    OrderByBlockAST *order_by_block;
    LimitToBlockAST *limit_to_block;
    unsigned last_token;

public:
    SelectStatementAST()
        : DmlStatementAST()
        , select_core_list(0)
        , select_compound_list(0)
        , order_by_block(0)
        , limit_to_block(0)
        , last_token(0)
    {}

    virtual SelectStatementAST *asSelectStatement() { return this; }
    virtual bool isSelectStatement() const { return true; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCoreAST: public AST
{
public:
    unsigned select_token;
    unsigned select_spec_token;         // ALL or DISTINCT
    ResultColumnListAST *columnList;
    unsigned from_token;
    JoinSourceAST *joinSource;
    unsigned where_token;
    ConditionExpressionAST *whereExpr;
    GroupByBlockAST *groupBy;

public:
    SelectCoreAST()
        : select_token(0)
        , select_spec_token(0)
        , columnList(0)
        , from_token(0)
        , joinSource(0)
        , where_token(0)
        , whereExpr(0)
        , groupBy(0)
    {}

    virtual SelectCoreAST *asSelectCore() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCompoundAST: public AST
{
public:
    SelectCompoundAST() {}
    virtual SelectCompoundAST *asSelectCompound() { return this; }
};

class SelectCompoundUnionAST: public SelectCompoundAST
{
public:
    unsigned union_token;
    unsigned union_spec_token; // ALL or DISTINCT

public:
    SelectCompoundUnionAST()
        : union_token(0)
        , union_spec_token(0)
    {}

    virtual SelectCompoundUnionAST *asSelectCompoundUnion() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCompoundIntersectAST: public SelectCompoundAST
{
public:
    unsigned intersect_token;

public:
    SelectCompoundIntersectAST()
        :intersect_token(0)
    {}

    virtual SelectCompoundIntersectAST *asSelectCompoundIntersect() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCompoundExceptAST: public SelectCompoundAST
{
public:
    unsigned except_token;

public:
    SelectCompoundExceptAST()
        : except_token(0)
    {}

    virtual SelectCompoundExceptAST *asSelectCompoundExcept() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SelectCompoundMinusAST: public SelectCompoundAST
{
public:
    unsigned minus_token;

public:
    SelectCompoundMinusAST()
        : minus_token(0)
    {}

    virtual SelectCompoundMinusAST *asSelectCompoundMinus() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OrderByBlockAST: public AST
{
public:
    unsigned order_token;
    unsigned by_token;
    OrderingTermListAST *term_list;

public:
    OrderByBlockAST()
        : order_token(0)
        , by_token(0)
        , term_list(0)
    {}

    virtual OrderByBlockAST *asOrderByBlock() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class OrderingTermAST: public AST
{
public:
    ValueExpressionAST *expr;
    unsigned asc_or_desc_token;

public:
    OrderingTermAST()
        : expr(0)
        , asc_or_desc_token(0)
    {}

    virtual OrderingTermAST *asOrderingTerm() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const { return less0_token; }
    virtual unsigned lastToken() const { return greater1_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NameAST: public AST
{
public:
    unsigned name_token;

public: // Annotations
    const Sql::Identifier *name;

public:
    NameAST()
        : name_token(0)
        , name(0)
    {}

    virtual NameAST *asName() { return this; }

    virtual unsigned firstToken() const { return name_token; }
    virtual unsigned lastToken() const { return name_token + 1; }
};

class SchemaNameAST: public NameAST
{
public:
    SchemaNameAST()
        : NameAST()
    {}

    virtual SchemaNameAST *asSchemaName() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableNameAST: public NameAST
{
public:
    TableNameAST()
        : NameAST()
    {}

    virtual TableNameAST *asTableName() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ViewNameAST: public NameAST
{
public:
    ViewNameAST() : NameAST() {}

    virtual ViewNameAST *asViewName() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IndexNameAST: public NameAST
{
public:
    IndexNameAST() : NameAST() {}

    virtual IndexNameAST *asIndexName() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TriggerNameAST: public NameAST
{
public:
    TriggerNameAST() : NameAST() {}

    virtual TriggerNameAST *asTriggerName() { return this; }

    TriggerNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintNameAST: public NameAST
{
public:
    ConstraintNameAST() : NameAST() {}

    virtual ConstraintNameAST *asConstraintName() { return this; }

    ConstraintNameAST *clone(Utils::MemoryPool *pool) const;

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

class SchemaTableNameAST: public AST
{
public:
    SchemaNameAST *schemaName;
    unsigned dot_token;
    TableNameAST *tableName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    SchemaTableNameAST()
        : schemaName(0)
        , dot_token(0)
        , tableName(0)
    {}

    virtual SchemaTableNameAST *asSchemaTableName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    SchemaTableNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SchemaIndexNameAST: public AST
{
public:
    SchemaNameAST *schemaName;
    unsigned dot_token;
    IndexNameAST *indexName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    SchemaIndexNameAST()
        : schemaName(0)
        , dot_token(0)
        , indexName(0)
    {}

    virtual SchemaIndexNameAST *asSchemaIndexName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    SchemaIndexNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SchemaTriggerNameAST: public AST
{
public:
    SchemaNameAST *schemaName;
    unsigned dot_token;
    TriggerNameAST *triggerName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    SchemaTriggerNameAST()
        : schemaName(0)
        , dot_token(0)
        , triggerName(0)
    {}

    virtual SchemaTriggerNameAST *asSchemaTriggerName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    SchemaTriggerNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TableAliasAST: public NameAST
{
public: // Annotations
    SubQueryAST  *subqueryAliasType;
    SchemaTableNameAST *schemaTableName;

public:
    TableAliasAST()
        : NameAST()
        , subqueryAliasType(0)
        , schemaTableName(0)
    {}

    virtual TableAliasAST *asTableAlias() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnNameAST: public NameAST
{
public: // Annotations
    SchemaNameAST *schemaName;
    NameAST *tableOfColumn;    //    columnOwner->asTableName()
                               // OR columnOwner->asTableAlias()->subqueryAliasType
                               // OR columnOwner->asTableAlias()->tableAliasType
public:
    ColumnNameAST()
        : NameAST()
        , schemaName(0)
        , tableOfColumn(0)
    {}

    virtual ColumnNameAST *asColumnName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BindingNameAST: public NameAST
{
public:
    unsigned param_prefix_token;

public:
    BindingNameAST()
        : NameAST()
        , param_prefix_token(0)
    {}

    virtual BindingNameAST *asParamName() { return this; }

    virtual unsigned firstToken() const { return param_prefix_token; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LabelNameAST: public NameAST
{
public:
    LabelNameAST()
        : NameAST()
    {}

    virtual LabelNameAST *asLabelName() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnAliasAST: public NameAST
{
public:
    ColumnAliasAST()
        : NameAST()
    {}

    virtual ColumnAliasAST *asColumnAlias() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionNameAST: public NameAST
{
public:
    FunctionNameAST()
        : NameAST()
    {}

    virtual FunctionNameAST *asFunctionName() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ResultColumnAST: public AST
{
public:
    ResultColumnAST()
    {}

    virtual ResultColumnAST *asResultColumn() { return this; }
};

class StarResultColumnAST: public ResultColumnAST
{
public:
    SchemaTableNameAST *schemaTableName;
    unsigned dot_token;
    unsigned star_token;

public:
    StarResultColumnAST()
        : schemaTableName(0)
        , dot_token(0)
        , star_token(0)
    {}

    virtual StarResultColumnAST *asStarResultColumn() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExprResultColumnAST: public ResultColumnAST
{
public:
    ValueExpressionAST *expr;
    unsigned as_token;
    ColumnAliasAST *columnAlias;

public:
    ExprResultColumnAST()
        : expr(0)
        , as_token(0)
        , columnAlias(0)
    {}

    virtual ExprResultColumnAST *asExprResultColumn() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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
    SchemaTableNameAST *schemaTableName;
    unsigned as_token;
    TableAliasAST *tableAlias;

public:
    DirectSingleSourceAST()
        : schemaTableName(0)
        , as_token(0)
        , tableAlias(0)
    {}

    virtual DirectSingleSourceAST *asDirectSingleSource() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubQuerySingleSourceAST: public SingleSourceAST
{
public:
    SubQueryAST *subQuery;
    unsigned as_token;
    TableAliasAST *tableAlias;

public:
    SubQuerySingleSourceAST()
        : subQuery(0)
        , as_token(0)
        , tableAlias(0)
    {}

    virtual SubQuerySingleSourceAST *asSubQuerySingleSource() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const { return first_token; }
    virtual unsigned lastToken() const { return last_token; }

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JoinSourceAST: public AST
{
public:
    SingleSourceAST *firstSource;
    JoinItemListAST *source_list;

public:
    JoinSourceAST()
        : firstSource(0)
        , source_list(0)
    {}

    JoinSourceAST *asJoinSource() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class GroupByBlockAST: public AST
{
public:
    unsigned group_token;
    unsigned by_token;
    ValueExpressionListAST *group_items_list;
    unsigned having_token;
    ConditionExpressionAST *havingExpr;

public:
    GroupByBlockAST()
        : group_token(0)
        , by_token(0)
        , group_items_list(0)
        , having_token(0)
        , havingExpr(0)
    {}

    virtual GroupByBlockAST *asGroupByBlock() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DeleteStatementAST: public DmlStatementAST
{
public:
    unsigned delete_token;
    unsigned from_token;
    DirectSingleSourceAST *source;
    unsigned where_token;
    ConditionExpressionAST *whereExpr;

public:
    DeleteStatementAST()
        : DmlStatementAST()
        , delete_token(0)
        , from_token(0)
        , source(0)
        , where_token(0)
        , whereExpr(0)
    {}

    virtual DeleteStatementAST *asDeleteStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertStatementAST: public DmlStatementAST
{
public:
    unsigned insert_token;
    unsigned into_token;
    DirectSingleSourceAST *source;
    unsigned lparen_columns_token;
    ColumnNameListAST *columns_list;
    unsigned rparen_columns_token;
    InsertClauseAST *insClause;

public:
    InsertStatementAST()
        : DmlStatementAST()
        , insert_token(0)
        , into_token(0)
        , source(0)
        , lparen_columns_token(0)
        , columns_list(0)
        , rparen_columns_token(0)
        , insClause(0)
    {}

    virtual InsertStatementAST *asInsertStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UpdateStatementAST: public DmlStatementAST
{
public:
    unsigned update_token;
    DirectSingleSourceAST *source;
    unsigned set_token;
    AssignmentExpressionListAST *upd_list;
    unsigned where_token;
    ConditionExpressionAST *whereExpr;

public:
    UpdateStatementAST()
        : DmlStatementAST()
        , update_token(0)
        , source(0)
        , set_token(0)
        , upd_list(0)
        , where_token(0)
        , whereExpr(0)
    {}

    virtual UpdateStatementAST *asUpdateStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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
    unsigned lparen_token;
    ColumnDefinitionListAST *column_def_list;
    TableConstraintListAST *constraintList;
    unsigned rparen_token;

public:
    CreateTableDirectClauseAST()
        : CreateTableClauseAST()
        , lparen_token(0)
        , column_def_list(0)
        , constraintList(0)
        , rparen_token(0)
    {}

    virtual CreateTableDirectClauseAST *asCreateTableDirectClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTableAsSelectClauseAST: public CreateTableClauseAST
{
public:
    unsigned as_token;
    StatementAST *selectStatement;

public:
    CreateTableAsSelectClauseAST()
        : CreateTableClauseAST()
        , as_token(0)
        , selectStatement(0)
    {}

    CreateTableAsSelectClauseAST *asCreateTableAsSelectClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTriggerRefClauseAST: public AST
{
public: // Annotations
    SchemaTableNameAST *schemaTableName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    unsigned referencing_token;
    unsigned new_token;
    unsigned asn_token;
    TableAliasAST *newAlias;
    unsigned old_token;
    unsigned aso_token;
    TableAliasAST *oldAlias;

public:
    CreateTriggerRefClauseAST( SchemaTableNameAST *schemaTableName_ )
        : schemaTableName(schemaTableName_)
        , referencing_token(0)
        , new_token(0)
        , asn_token(0)
        , newAlias(0)
        , old_token(0)
        , aso_token(0)
        , oldAlias(0)
    {}

    virtual CreateTriggerRefClauseAST *asCreateTriggerRefClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    CreateRdbTriggerBodyAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateRdbTriggerBodyItemAST: public AST
{
public:
    unsigned when_token;
    unsigned lparen_when_token;
    ConditionExpressionAST *whenExpr;
    unsigned rparen_when_token;
    unsigned lparen_token;
    StatementListAST *stat_list;
    unsigned rparen_token;
    unsigned for_token;
    unsigned each_token;
    unsigned row_or_statement_token;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ColumnDefinitionAST: public AST
{
public:
    ColumnNameAST *columnName;
    BaseTypeAST *columnType;
    unsigned default_token;
    ConstantExpressionAST *defaultExpr;
    ColumnConstraintListAST *constraintList;

public:
    ColumnDefinitionAST()
        : columnName(0)
        , columnType(0)
        , default_token(0)
        , defaultExpr(0)
        , constraintList(0)
    {}

    virtual ColumnDefinitionAST *asColumnDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstraintAST: public AST
{
public:
    unsigned constraint_token;
    ConstraintNameAST *constraintName;

public:
    ConstraintAST()
        : constraint_token(0)
        , constraintName(0)
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

    TableConstraintAST( TableConstraintType constraintType_ )
        : ConstraintAST()
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
    ColumnConstraintAST()
        : ConstraintAST()
    {}

    virtual ColumnConstraintAST *asColumnConstraint() { return this; }
};

class UniqueOrPkTableConstraintAST: public TableConstraintAST
{
public:
    unsigned unique_or_primary_token;
    unsigned key_token;
    unsigned lparen_token;
    SortedColumnNameListAST *column_list;
    unsigned rparen_token;

    bool parse( Parser *p );

public:
    UniqueOrPkTableConstraintAST()
        : TableConstraintAST(TableConstraintAST::UniqueOrPrimaryKey)
        , unique_or_primary_token(0)
        , key_token(0)
        , lparen_token(0)
        , column_list(0)
        , rparen_token(0)
    {}

    virtual UniqueOrPkTableConstraintAST *asUniqueOrPkTableConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckTableConstraintAST: public TableConstraintAST
{
public:
    unsigned check_token;
    unsigned lparen_token;
    ConditionExpressionAST *checkExpr;
    unsigned rparen_token;

    bool parse( Parser *p );

public:
    CheckTableConstraintAST()
        : TableConstraintAST(TableConstraintAST::Check)
        , check_token(0)
        , lparen_token(0)
        , checkExpr(0)
        , rparen_token(0)
    {}

    virtual CheckTableConstraintAST *asCheckTableConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FkTableConstraintAST: public TableConstraintAST
{
public:
    unsigned foreign_token;
    unsigned key_token;
    unsigned lparen_token;
    ColumnNameListAST *col_list;
    unsigned rparen_token;
    FkReferencesClauseAST *refClause;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    FkTableConstraintAST()
        : TableConstraintAST(TableConstraintAST::ForeignKey)
        , foreign_token(0)
        , key_token(0)
        , lparen_token(0)
        , col_list(0)
        , rparen_token(0)
        , refClause(0)
    {}

    virtual FkTableConstraintAST *asFkTableConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FkReferencesClauseAST: public AST
{
public:
    unsigned references_token;
    SchemaTableNameAST *refSchemaTableName;
    unsigned ref_lparen_token;
    ColumnNameListAST *ref_col_list;
    unsigned ref_rparen_token;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    FkReferencesClauseAST()
        : references_token(0)
        , refSchemaTableName(0)
        , ref_lparen_token(0)
        , ref_col_list(0)
        , ref_rparen_token(0)
    {}

    virtual FkReferencesClauseAST * asForeignKeyClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PkColumnConstraintAST: public ColumnConstraintAST
{
public:
    unsigned primary_token;
    unsigned key_token;
    unsigned asc_or_desc_token;

    bool parse( Parser *p );

public:
    PkColumnConstraintAST()
        : ColumnConstraintAST()
        , primary_token(0)
        , key_token(0)
        , asc_or_desc_token(0)
    {}

    virtual PkColumnConstraintAST *asPkColumnConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NotNullColumnConstraintAST: public ColumnConstraintAST
{
public:
    unsigned not_token;
    unsigned null_token;

    bool parse( Parser *p );

public:
    NotNullColumnConstraintAST()
        : ColumnConstraintAST()
        , not_token(0)
        , null_token(0)
    {}

    virtual NotNullColumnConstraintAST *asNotNullColumnConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UniqueColumnConstraintAST: public ColumnConstraintAST
{
public:
    unsigned unique_token;

public:
    UniqueColumnConstraintAST()
        : ColumnConstraintAST()
        , unique_token(0)
    {}

    virtual UniqueColumnConstraintAST *asUniqueColumnConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;


protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CheckColumnConstraintAST: public ColumnConstraintAST
{
public:
    unsigned check_token;
    unsigned lparen_token;
    ConditionExpressionAST *checkExpr;
    unsigned rparen_token;

    bool parse( Parser *p );

public:
    CheckColumnConstraintAST()
        : ColumnConstraintAST()
        , check_token(0)
        , lparen_token(0)
        , checkExpr(0)
        , rparen_token(0)
    {}

    virtual CheckColumnConstraintAST *asCheckColumnConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ReferencesColumnConstraintAST: public ColumnConstraintAST
{
public:
    FkReferencesClauseAST *refClause;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    ReferencesColumnConstraintAST()
        : ColumnConstraintAST()
        , refClause(0)
    {}

    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SortedColumnNameAST: public AST
{
public:
    ColumnNameAST *columnName;
    unsigned asc_or_desc_token;

public:
    SortedColumnNameAST()
        : columnName(0)
        , asc_or_desc_token(0)
    {}

    virtual SortedColumnNameAST *asSortedColumnName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertClauseAsValuesAST: public InsertClauseAST
{
public:
    unsigned values_token;
    unsigned lparen_token;
    ValueExpressionListAST *val_list;
    unsigned rparen_token;

public:
    InsertClauseAsValuesAST()
        : InsertClauseAST()
        , values_token(0)
        , lparen_token(0)
        , val_list(0)
        , rparen_token(0)
    {}

    virtual InsertClauseAsValuesAST *asInsertClauseAsValues() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InsertClauseAsSelectAST: public InsertClauseAST
{
public:
    StatementAST *selectStatement;

public:
    InsertClauseAsSelectAST()
        : InsertClauseAST()
        , selectStatement(0)
    {}

    virtual InsertClauseAsSelectAST *asInsertClauseAsSelect() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DdlStatementAST: public StatementAST
{
public:
    DdlStatementAST() : StatementAST() {}
    virtual DdlStatementAST *asDdlStatement() { return this; }

    virtual unsigned targetKind() const = 0;
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const = 0;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const {
        *tableName = "";
        *tableObjectType = 0;
        return true;
    }
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
    unsigned create_token;
    unsigned table_token;
    SchemaTableNameAST *schemaTableName;
    CreateTableClauseAST *tableClause;

public:
    CreateTableStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , table_token(0)
        , schemaTableName(0)
        , tableClause(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_TABLE; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;

    virtual CreateTableStatementAST *asCreateTableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateViewStatementAST: public CreateStatementAST
{
public:
    unsigned create_token;
    unsigned view_token;
    SchemaTableNameAST *schemaViewName;
    unsigned as_token;
    StatementAST *selectStatement;

public:
    CreateViewStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , view_token(0)
        , schemaViewName(0)
        , as_token(0)
        , selectStatement(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_VIEW; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;

    virtual CreateViewStatementAST *asCreateViewStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateIndexStatementAST: public CreateStatementAST
{
public:
    unsigned create_token;
    unsigned unique_token;
    unsigned index_token;
    SchemaIndexNameAST *schemaIndexName;
    unsigned on_token;
    SchemaTableNameAST *schemaTableName;
    unsigned lparen_token;
    SortedColumnNameListAST *columns_list;
    unsigned rparen_token;

public:
    CreateIndexStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , unique_token(0)
        , index_token(0)
        , schemaIndexName(0)
        , on_token(0)
        , schemaTableName(0)
        , lparen_token(0)
        , columns_list(0)
        , rparen_token(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_INDEX; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const;

    virtual CreateIndexStatementAST *asCreateIndexStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CreateTriggerStatementAST: public CreateStatementAST
{
public:
    unsigned create_token;
    unsigned trigger_token;
    SchemaTriggerNameAST *schemaTriggerName;
    unsigned before_or_after_token;
    unsigned action_token;
    unsigned of_token;
    ColumnNameListAST *col_list;
    unsigned on_token;
    SchemaTableNameAST *schemaTableName;
    CreateTriggerRefClauseAST *triggerRefClause;
    CreateTriggerBodyAST *triggerBody;

public:
    CreateTriggerStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , trigger_token(0)
        , schemaTriggerName(0)
        , before_or_after_token(0)
        , action_token(0)
        , of_token(0)
        , col_list(0)
        , on_token(0)
        , schemaTableName(0)
        , triggerRefClause(0)
        , triggerBody(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_TRIGGER; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const;

    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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
    unsigned drop_token;
    unsigned table_token;
    SchemaTableNameAST *schemaTableName;

public:
    DropTableStatementAST()
        : DropStatementAST()
        , drop_token(0)
        , table_token(0)
        , schemaTableName(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_TABLE; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;

    virtual DropTableStatementAST *asDropTableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropViewStatementAST: public DropStatementAST
{
public:
    unsigned drop_token;
    unsigned view_token;
    SchemaTableNameAST *schemaViewName;

public:
    DropViewStatementAST()
        : DropStatementAST()
        , drop_token(0)
        , view_token(0)
        , schemaViewName(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_VIEW; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;

    virtual DropViewStatementAST *asDropViewStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropIndexStatementAST: public DropStatementAST
{
public:
    unsigned drop_token;
    unsigned index_token;
    SchemaIndexNameAST *schemaIndexName;

public:
    DropIndexStatementAST()
        : DropStatementAST()
        , drop_token(0)
        , index_token(0)
        , schemaIndexName(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_INDEX; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const {
        *tableName = "";
        *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_INDEX;
        return true;
    }

    virtual DropIndexStatementAST *asDropIndexStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DropTriggerStatementAST: public DropStatementAST
{
public:
    unsigned drop_token;
    unsigned trigger_token;
    SchemaTriggerNameAST *schemaTriggerName;

public:
    DropTriggerStatementAST()
        : DropStatementAST()
        , drop_token(0)
        , trigger_token(0)
        , schemaTriggerName(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_TRIGGER; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const {
        *tableName = "";
        *tableObjectType = Sql::Constants::DbSchemaObjectSubtype::C_TRIGGER;
        return true;
    }

    virtual DropTriggerStatementAST *asDropTriggerStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TruncateStatementAST: public DdlStatementAST
{
public:
    unsigned truncate_token;
    unsigned table_token;
    SchemaTableNameAST *schemaTableName;

public:
    TruncateStatementAST()
        : DdlStatementAST()
        , truncate_token(0)
        , table_token(0)
        , schemaTableName(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_TABLE; }
    virtual bool targetSchemaObjectName( QString * /*schemaName*/, QString * /*objectName*/ ) const { return false; }

    virtual TruncateStatementAST *asTruncateStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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
    unsigned alter_token;
    unsigned table_token;
    SchemaTableNameAST *schemaTableName;
    AlterTableClauseAST *alterTableClause;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableStatementAST()
        : AlterStatementAST()
        , alter_token(0)
        , table_token(0)
        , schemaTableName(0)
        , alterTableClause(0)
    {}

    virtual unsigned targetKind() const;
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const;

    virtual AlterTableStatementAST *asAlterTableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentStatementAST: public StatementAST
{
public:
    CommentStatementAST() : StatementAST() {}
    CommentStatementAST *asCommentStatement() { return this; }
};

class CommentOnTableStatementAST: public CommentStatementAST
{
public:
    unsigned comment_token;
    unsigned on_token;
    unsigned table_token;
    SchemaTableNameAST *schemaTableName;
    unsigned is_token;
    LiteralExpressionAST *comment;

public:
    CommentOnTableStatementAST()
        : CommentStatementAST()
        , comment_token(0)
        , on_token(0)
        , table_token(0)
        , schemaTableName(0)
        , is_token(0)
        , comment(0)
    {}

    virtual CommentOnTableStatementAST *asCommentOnTableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentOnColumnStatementAST: public CommentStatementAST
{
public:
    unsigned comment_token;
    unsigned on_token;
    unsigned column_token;
    SchemaTableNameAST *schemaTableName;
    unsigned dot_token;
    ColumnNameAST *column;
    unsigned is_token;
    LiteralExpressionAST *comment;

public:
    CommentOnColumnStatementAST()
        : CommentStatementAST()
        , comment_token(0)
        , on_token(0)
        , column_token(0)
        , schemaTableName(0)
        , dot_token(0)
        , column(0)
        , is_token(0)
        , comment(0)
    {}

    virtual CommentOnColumnStatementAST *asCommentOnColumnStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CallStatementAST: public StatementAST
{
public:
    unsigned call_token;
    FunctionNameAST *funcName;
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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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
    unsigned type_token;
    unsigned lparen_token;
    unsigned size_token;
    unsigned rparen_token;

public:
    CharacterTypeAST()
        : BaseTypeAST()
        , type_token(0)
        , lparen_token(0)
        , size_token(0)
        , rparen_token(0)
    {}

    virtual CharacterTypeAST *asCharacterType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NumberTypeAST: public BaseTypeAST
{
public:
    unsigned type_token;
    unsigned lparen_token;
    unsigned size_token;
    unsigned comma_token;
    unsigned prec_token;
    unsigned rparen_token;

public:
    NumberTypeAST()
        : BaseTypeAST()
        , type_token(0)
        , lparen_token(0)
        , size_token(0)
        , comma_token(0)
        , prec_token(0)
        , rparen_token(0)
    {}

    virtual NumberTypeAST *asNumericType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleTypeAST: public BaseTypeAST
{
public:
    unsigned type_token;

public:
    SimpleTypeAST()
        : BaseTypeAST()
        , type_token(0)
    {}

    virtual SimpleTypeAST *asSimpleType() { return this; }

    virtual unsigned firstToken() const { return type_token; }
    virtual unsigned lastToken() const { return type_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

/// Expressions
class BaseExpressionAST: public AST
{
public:
    unsigned first_token;
    unsigned last_token;
    bool valid;

public:
    BaseExpressionAST()
        : first_token(0)
        , last_token(0)
        , valid(true)
    {}

    virtual BaseExpressionAST *asBaseExpression() { return this; }

    virtual unsigned firstToken() const { return first_token; }
    virtual unsigned lastToken() const { return last_token; }
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
    unsigned lparen_token;
    ValueExpressionAST *valExpr;
    unsigned rparen_token;

public:
    CompoundValueExpressionAST ()
        : ValueExpressionAST()
        , lparen_token(0)
        , valExpr(0)
        , rparen_token(0)
    {}

    virtual CompoundValueExpressionAST *asCompoundValueExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UnaryValueExpressionAST: public ValueExpressionAST
{
public:
    unsigned op_token;
    ValueExpressionAST *valExpr;

public:
    UnaryValueExpressionAST()
        : ValueExpressionAST()
        , op_token(0)
        , valExpr(0)
    {}

    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BinaryValueExpressionAST: public ValueExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned op_token;
    ValueExpressionAST *rvExpr;

public:
    BinaryValueExpressionAST()
        : ValueExpressionAST()
        , lvExpr(0)
        , op_token(0)
        , rvExpr(0)
    {}

    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return this; }

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

class ColumnExpressionAST: public SimpleValueExpressionAST
{
public:
    ColumnNameAST *column;

public:
    ColumnExpressionAST()
        : SimpleValueExpressionAST()
        , column(0)
    {}

    virtual ColumnExpressionAST *asColumnExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LiteralExpressionAST: public ConstantExpressionAST
{
public:
    unsigned liter_token;

public:
    LiteralExpressionAST()
        : ConstantExpressionAST()
        , liter_token(0)
    {}

    virtual LiteralExpressionAST *asLiteralExpression() { return this; }

    virtual unsigned firstToken() const { return liter_token; }
    virtual unsigned lastToken() const { return liter_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NumericExpressionAST: public ConstantExpressionAST
{
public:
    unsigned numeric_token;

public:
    NumericExpressionAST()
        : ConstantExpressionAST()
        , numeric_token(0)
    {}

    virtual NumericExpressionAST *asNumericExpression() { return this; }

    virtual unsigned firstToken() const { return numeric_token; }
    virtual unsigned lastToken() const { return numeric_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionExpressionAST: public SimpleValueExpressionAST
{
public:
    FunctionNameAST *functionName;
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

class CountFunctionAST: public SimpleValueExpressionAST
{
public:
    unsigned count_token;
    unsigned lparen_token;
    unsigned distinct_token;
    ValueExpressionAST *countExpr;
    unsigned rparen_token;

public:
    CountFunctionAST()
        : SimpleValueExpressionAST()
        , count_token(0)
        , lparen_token(0)
        , distinct_token(0)
        , countExpr(0)
        , rparen_token(0)
    {}

    virtual CountFunctionAST *asCountFunction() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ParameterExpressionAST: public SimpleValueExpressionAST
{
public:
    BindingNameAST *binding;

public:
    ParameterExpressionAST()
        : SimpleValueExpressionAST()
        , binding(0)
    {}

    virtual ParameterExpressionAST *asParameterExpression() { return this; }

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

    virtual unsigned firstToken() const { if ( not_token ) return not_token; return null_token; }
    virtual unsigned lastToken() const { return null_token + 1; }

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
    CompoundRValueExpressionAST()
        : BaseExpressionAST()
        , lparen_token(0)
        , rparen_roken(0)
    {}

    virtual CompoundRValueExpressionAST *asCompoundRValueExpression() { return this; }
};

class ValueListExpressionAST: public CompoundRValueExpressionAST
{
public:
    ValueExpressionListAST *val_list;

public:
    ValueListExpressionAST()
        : CompoundRValueExpressionAST()
        , val_list(0)
    {}

    virtual ValueListExpressionAST *asValueListExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SubQueryExpressionAST: public CompoundRValueExpressionAST
{
public:
    StatementAST *query;

public:
    SubQueryExpressionAST()
        : CompoundRValueExpressionAST()
        , query(0)
    {}

    virtual SubQueryExpressionAST *asSubQueryExpression() { return this; }

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

class CompoundConditionExpressionAST: public ConditionExpressionAST
{
public:
    unsigned lparen_token;
    ConditionExpressionAST *condExpr;
    unsigned rparen_token;

public:
    CompoundConditionExpressionAST()
        : ConditionExpressionAST()
        , lparen_token(0)
        , condExpr(0)
        , rparen_token(0)
    {}

    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned not_token;
    unsigned in_token;
    CompoundRValueExpressionAST *rvExpr;

public:
    InConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(0)
        , not_token(0)
        , in_token(0)
        , rvExpr(0)
    {}

    virtual InConditionExpressionAST *asInConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LikeConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned not_token;
    unsigned like_token;
    ValueExpressionAST *rvExpr;

public:
    LikeConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(0)
        , not_token(0)
        , like_token(0)
        , rvExpr(0)
    {}

    virtual LikeConditionExpressionAST *asLikeConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BetweenConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned not_token;
    unsigned between_token;
    ValueExpressionAST *rv1Expr;
    unsigned and_token;
    ValueExpressionAST *rv2Expr;

public:
    BetweenConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(0)
        , not_token(0)
        , between_token(0)
        , rv1Expr(0)
        , and_token(0)
        , rv2Expr(0)
    {}

    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ExistsConditionExpressionAST: public ConditionExpressionAST
{
public:
    unsigned not_token;
    unsigned exists_token;
    SubQueryExpressionAST *rvExpr;

public:
    ExistsConditionExpressionAST()
        : ConditionExpressionAST()
        , not_token(0)
        , exists_token(0)
    {}

    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UnaryConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned last_op_token;

public:
    UnaryConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(0)
        , last_op_token(0)
    {}

    virtual UnaryConditionExpressionAST *asUnaryConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BinaryConditionExpressionAST: public ConditionExpressionAST
{
public:
    ConditionExpressionAST *lvCondExpr;
    unsigned cond_op_token;
    ConditionExpressionAST *rvCondExpr;

public:
    BinaryConditionExpressionAST()
        : ConditionExpressionAST()
        , lvCondExpr(0)
        , cond_op_token(0)
        , rvCondExpr(0)
    {}

    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class AssignmentExpressionAST: public BaseExpressionAST
{
public:
    ColumnNameAST *columnName;
    unsigned equal_token;
    ValueExpressionAST *rvExpr;

public:
    AssignmentExpressionAST()
        : BaseExpressionAST()
        , columnName(0)
        , equal_token(0)
        , rvExpr(0)
    {}

    virtual AssignmentExpressionAST *asAssignmentExpression() { return this; }

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const { return commit_token; }
    virtual unsigned lastToken() const { return commit_token + 1; }

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BlockLockTableStatementAST: public BlockSqlStatementAST
{
public:
    unsigned lock_token;
    unsigned table_token;
    SchemaTableNameAST *tableName;

public:
    BlockLockTableStatementAST()
        : BlockSqlStatementAST()
        , lock_token(0)
        , table_token(0)
        , tableName(0)
    {}

    virtual BlockLockTableStatementAST *asBlockLockTableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const { return null_token; }
    virtual unsigned lastToken() const { return null_token + 1; }

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeByTableRowAST: public DataTypeAST
{
public:
    TableNameAST *tableName;
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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeByTableColumnAST: public DataTypeAST
{
public:
    TableNameAST *tableName;
    unsigned dot_token;
    ColumnNameAST *columnName;
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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const { return spec_token; }
    virtual unsigned lastToken() const { return spec_token + 1; }

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FunctionCasheSpecAST: public FunctionSpecAST
{
public:
    unsigned result_cashe_token;
    unsigned relies_on_token;
    unsigned lparen_token;
    TableNameListAST *source_list;
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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

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

} // namespace Oracle

#endif // ORACLE_AST_H
