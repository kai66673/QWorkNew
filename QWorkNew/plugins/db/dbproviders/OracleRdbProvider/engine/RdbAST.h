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

#ifndef RDB_AST_H
#define RDB_AST_H

#include <QString>
#include <QDebug>

#include "RdbASTfwd.h"
#include "MemoryPool.h"
#include "TranslationUnit.h"
#include "SqlToken.h"
#include "rdb_sql_constants.h"
#include "BaseWalker.h"

namespace RDB {

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

    static bool match(AST *ast, AST *pattern, ASTMatcher *matcher);
    bool match(AST *pattern, ASTMatcher *matcher);

    template <typename _Tp>
    static bool match(List<_Tp> *it, List<_Tp> *patternIt, ASTMatcher *matcher)
    {
        while (it && patternIt) {
            if (! match(it->value, patternIt->value, matcher))
                return false;

            it = it->next;
            patternIt = patternIt->next;
        }

        if (! it && ! patternIt)
            return true;

        return false;
    }

    virtual unsigned firstToken() const = 0;
    virtual unsigned lastToken() const = 0;

    virtual AST *clone(Utils::MemoryPool *pool) const = 0;

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
    virtual NameAST *asName() { return 0; }
    virtual SchemaNameAST *asSchemaName() { return 0; }
    virtual TableNameAST *asTableName() { return 0; }
    virtual ColumnAliasAST *asColumnAlias() { return 0; }
    virtual TableAliasAST *asTableAlias() { return 0; }
    virtual ColumnNameAST *asColumnName() { return 0; }
    virtual ModuleNameAST *asModuleName() { return 0; }
    virtual RoutineNameAST *asFunctionName() { return 0; }
    virtual IdentifierNameAST *asIdentifierName() { return 0; }
    virtual ParamNameAST *asParamName() { return 0; }
    virtual LabelNameAST *asLabelName() { return 0; }
    virtual ViewNameAST *asViewName() { return 0; }
    virtual IndexNameAST *asIndexName() { return 0; }
    virtual TriggerNameAST *asTriggerName() { return 0; }
    virtual ConstraintNameAST *asConstraintName() { return 0; }
    virtual CursorNameAST *asCursorName() { return 0; }
    virtual StorageMapNameAST *asStorageMapName() { return 0; }
    virtual AreaNameAST *asAreaName() { return 0; }
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
    virtual IndexTypeAST *asIndexType() { return 0; }
    virtual IndexNodeSizeAST *asIndexNodeSize() { return 0; }
    virtual IndexStoreInAST *asIndexStoreIn() { return 0; }
    virtual IndexOptionsAST *asIndexOptions() { return 0; }
    virtual CreateIndexStatementAST *asCreateIndexStatement() { return 0; }
    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return 0; }
    virtual CreateStorageMapStatementAST *asCreateStorageMapStatement() { return 0; }
    virtual DropStatementAST *asDropStatement() { return 0; }
    virtual DropTableStatementAST *asDropTableStatement() { return 0; }
    virtual DropViewStatementAST *asDropViewStatement() { return 0; }
    virtual DropIndexStatementAST *asDropIndexStatement() { return 0; }
    virtual DropTriggerStatementAST *asDropTriggerStatement() { return 0; }
    virtual DropModuleStatementAST *asDropModuleStatement() { return 0; }
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

    virtual BlockStatementAST *asBlockStatement() { return 0; }
    virtual CreateModuleStatementAST *asCreateModuleStatement() { return 0; }
    virtual CreateRoutineStatementAST *asCreateRoutineStatement() { return 0; }
    virtual RoutineAST *asRoutine() { return 0; }

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
    virtual IdentifierExpressionAST *asIdentifierExpression() { return 0; }
    virtual IdentDotColumnExpressionAST *asIdentDotColumnExpression() { return 0; }
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
    virtual DeclareAST *asDeclare() { return 0; }
    virtual ArgumentAST *asArgument() { return 0; }
    virtual BlockStatementEntryAST *asBlockStatementEntry() { return 0; }
    virtual BlockInsertStatementAST *asBlockInsertStatement() { return 0; }
    virtual BlockUpdateStatementAST *asBlockUpdateStatement() { return 0; }
    virtual BlockDeleteStatementAST *asBlockDeleteStatement() { return 0; }
    virtual BlockSelectStatementAST *asBlockSelectStatement() { return 0; }
    virtual BlockSetStatementAST *asBlockSetStatement() { return 0; }
    virtual BlockCommitStatmentAST *asBlockCommitStatment() { return 0; }
    virtual BlockRollbackStatmentAST *asBlockRollbackStatment() { return 0; }
    virtual BlockCallStatementAST *asBlockCallStatement() { return 0; }
    virtual IfStatementAST *asIfStatement() { return 0; }
    virtual LoopStatementAST *asLoopStatement() { return 0; }
    virtual DeclareCursorClauseAST *asDeclareCursorClause() { return 0; }
    virtual ForStatementAST *asForStatement() { return 0; }
    virtual BlockLabelAST *asBlockLabel() { return 0; }
    virtual LeaveStatementAST *asLeaveStatement() { return 0; }
    virtual BlockBlockStatementAST *asBlockBlockStatement() { return 0; }
    virtual BlockReturnStatementAST *asBlockReturnStatement() { return 0; }

    // Additional for SelectStatement and InvalidSelectStatement
    virtual bool isSelectStatement() const { return false; }

protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
    virtual bool match0(AST *, ASTMatcher *) = 0;
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

    virtual TranslationUnitAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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
    virtual StatementAST *clone(Utils::MemoryPool *pool) const = 0;

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

    virtual InvalidStatementAST *clone(Utils::MemoryPool *pool) const;
    virtual InvalidStatementAST *asInvalidStatement() { return this; }

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );

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

    virtual UnknownStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class DmlStatementAST: public StatementAST
{
public:
    DmlStatementAST() : StatementAST() {}
    virtual DmlStatementAST *asDmlStatement() { return this; }
    virtual StatementAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual SelectStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SelectCoreAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SelectCompoundAST: public AST
{
public:
    SelectCompoundAST() {}
    virtual SelectCompoundAST *asSelectCompound() { return this; }
    virtual SelectCompoundAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual SelectCompoundUnionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SelectCompoundIntersectAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SelectCompoundExceptAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SelectCompoundMinusAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual OrderByBlockAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual LimitToBlockAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual OrderingTermAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual NameAST *clone(Utils::MemoryPool *pool) const = 0;

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

    virtual SchemaNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class TableNameAST: public NameAST
{
public:
    TableNameAST()
        : NameAST()
    {}

    virtual TableNameAST *asTableName() { return this; }

    virtual TableNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ViewNameAST: public NameAST
{
public:
    ViewNameAST() : NameAST() {}

    virtual ViewNameAST *asViewName() { return this; }

    virtual ViewNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IndexNameAST: public NameAST
{
public:
    IndexNameAST() : NameAST() {}

    virtual IndexNameAST *asIndexName() { return this; }

    virtual IndexNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class TriggerNameAST: public NameAST
{
public:
    TriggerNameAST() : NameAST() {}

    virtual TriggerNameAST *asTriggerName() { return this; }

    TriggerNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ConstraintNameAST: public NameAST
{
public:
    ConstraintNameAST() : NameAST() {}

    virtual ConstraintNameAST *asConstraintName() { return this; }

    ConstraintNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class CursorNameAST: public NameAST
{
public:
    CursorNameAST() : NameAST() {}

    virtual CursorNameAST *asCursorName() { return this; }

    CursorNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class StorageMapNameAST: public NameAST
{
public:
    StorageMapNameAST() : NameAST() {}

    virtual StorageMapNameAST *asStorageMapName() { return this; }

    StorageMapNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class AreaNameAST: public NameAST
{
public:
    AreaNameAST() : NameAST() {}

    virtual AreaNameAST *asAreaName() { return this; }

    AreaNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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
    virtual bool match0( AST *, ASTMatcher * );
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
    virtual bool match0( AST *, ASTMatcher * );
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
    virtual bool match0( AST *, ASTMatcher * );

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

    virtual TableAliasAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual ColumnNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IdentifierNameAST: public NameAST
{
public:
    unsigned colon_token;

public:
    IdentifierNameAST()
        : NameAST()
        , colon_token(0)
    {}

    virtual IdentifierNameAST *asIdentifierName() { return this; }

    virtual IdentifierNameAST *clone(Utils::MemoryPool *pool) const;

    virtual unsigned firstToken() const { return colon_token; }

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ParamNameAST: public NameAST
{
public:
    unsigned param_prefix_token;

public:
    ParamNameAST()
        : NameAST()
        , param_prefix_token(0)
    {}

    virtual ParamNameAST *asParamName() { return this; }

    virtual ParamNameAST *clone(Utils::MemoryPool *pool) const;

    virtual unsigned firstToken() const { return param_prefix_token; }

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class LabelNameAST: public NameAST
{
public:
    LabelNameAST()
        : NameAST()
    {}

    virtual LabelNameAST *asLabelName() { return this; }

    virtual LabelNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ColumnAliasAST: public NameAST
{
public:
    ColumnAliasAST()
        : NameAST()
    {}

    virtual ColumnAliasAST *asColumnAlias() { return this; }

    virtual ColumnAliasAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ModuleNameAST: public NameAST
{
public:
    ModuleNameAST()
        : NameAST()
    {}

    virtual ModuleNameAST *asModuleName() { return this; }

    virtual ModuleNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class RoutineNameAST: public NameAST
{
public:
    RoutineNameAST()
        : NameAST()
    {}

    virtual RoutineNameAST *asFunctionName() { return this; }

    virtual RoutineNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ResultColumnAST: public AST
{
public:
    ResultColumnAST()
    {}

    virtual ResultColumnAST *asResultColumn() { return this; }

    virtual ResultColumnAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual StarResultColumnAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual ExprResultColumnAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SubQueryAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SingleSourceAST: public AST
{
public:
    SingleSourceAST()
    {}

    virtual SingleSourceAST *asSingleSource() { return this; }

    virtual SingleSourceAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual DirectSingleSourceAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SubQuerySingleSourceAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual JoinOpAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual JoinConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual JoinItemAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual JoinSourceAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual GroupByBlockAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual DeleteStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual InsertStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual UpdateStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class InsertClauseAST: public AST
{
public:
    InsertClauseAST() {}
    virtual InsertClauseAST *asInsertClause() { return this; }
    virtual InsertClauseAST *clone(Utils::MemoryPool *pool) const = 0;
};

class CreateTableClauseAST: public AST
{
public:
    CreateTableClauseAST() {}
    virtual CreateTableClauseAST *asCreateTableClause() { return this; }
    virtual CreateTableClauseAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual CreateTableDirectClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CreateTableAsSelectClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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
    virtual bool match0( AST *, ASTMatcher * );
};

class CreateTriggerBodyAST: public AST
{
public:
    CreateTriggerBodyAST() {}
    virtual CreateTriggerBodyAST *asCreateTriggerBody() { return this; }
    virtual CreateTriggerBodyAST *clone(Utils::MemoryPool *pool) const = 0;
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
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CreateRdbTriggerBodyItemAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual ColumnDefinitionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual ConstraintAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual TableConstraintAST *clone(Utils::MemoryPool *pool) const = 0;

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

    virtual ColumnConstraintAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual UniqueOrPkTableConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CheckTableConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual FkTableConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual FkReferencesClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual PkColumnConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual NotNullColumnConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual UniqueColumnConstraintAST *clone(Utils::MemoryPool *pool) const;


protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CheckColumnConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual ReferencesColumnConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SortedColumnNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual InsertClauseAsValuesAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual InsertClauseAsSelectAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class DdlStatementAST: public StatementAST
{
public:
    DdlStatementAST() : StatementAST() {}
    virtual DdlStatementAST *asDdlStatement() { return this; }
    virtual DdlStatementAST *clone(Utils::MemoryPool *pool) const = 0;

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
    virtual CreateStatementAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual CreateTableStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CreateViewStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IndexTypeAST: public AST
{
public:
    unsigned type_token;
    unsigned is_token;
    unsigned sorted_token;
    unsigned ranked_token;

public:
    IndexTypeAST()
        : type_token(0)
        , is_token(0)
        , sorted_token(0)
        , ranked_token(0)
    {}

    virtual IndexTypeAST *asIndexType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IndexTypeAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IndexNodeSizeAST: public AST
{
public:
    unsigned node_token;
    unsigned size_token;
    NumericExpressionAST *sizeExpr;

public:
    IndexNodeSizeAST()
        : node_token(0)
        , size_token(0)
        , sizeExpr(0)
    {}

    virtual IndexNodeSizeAST *asIndexNodeSize() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IndexNodeSizeAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IndexStoreInAST: public AST
{
public:
    unsigned store_token;
    unsigned in_token;
    AreaNameAST *areaName;

public:
    IndexStoreInAST()
        : store_token(0)
        , in_token(0)
        , areaName(0)
    {}

    virtual IndexStoreInAST *asIndexStoreIn() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IndexStoreInAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IndexOptionsAST: public AST
{
public:
    IndexTypeAST *typeAst;
    IndexNodeSizeAST *nodeSizeAst;
    IndexStoreInAST*storeInAst;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    IndexOptionsAST()
        : typeAst(0)
        , nodeSizeAst(0)
        , storeInAst(0)
    {}

    virtual IndexOptionsAST *asIndexOptions() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IndexOptionsAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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
    IndexOptionsAST *options;

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
        , options(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_INDEX; }
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const;

    virtual CreateIndexStatementAST *asCreateIndexStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CreateIndexStatementAST *clone(Utils::MemoryPool *pool) const ;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CreateTriggerStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class CreateStorageMapStatementAST: public CreateStatementAST
{
public:
    unsigned create_token;
    unsigned storage_token;
    unsigned map_token;
    StorageMapNameAST *mapName;
    unsigned for_token;
    TableNameAST *tableName;
    unsigned store_token;
    unsigned in_token;
    AreaNameAST *areaName;

public:
    CreateStorageMapStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , storage_token(0)
        , map_token(0)
        , mapName(0)
        , for_token(0)
        , tableName(0)
        , store_token(0)
        , in_token(0)
        , areaName()
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_TABLE; }
    virtual bool targetSchemaObjectName( QString * /*schemaName*/, QString * /*objectName*/ ) const { return false; }

    virtual CreateStorageMapStatementAST *asCreateStorageMapStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CreateStorageMapStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class CreateModuleStatementAST: public CreateStatementAST
{
public:
    unsigned create_token;
    unsigned module_token;
    ModuleNameAST *moduleName;
    unsigned language_token;
    unsigned sql_token;
    unsigned comment_token;
    unsigned is_token;
    LiteralExpressionAST *commentExpr;
    RoutineListAST *routine_list;
    unsigned end_token;
    unsigned end_module_token;

public:
    CreateModuleStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , module_token(0)
        , moduleName(0)
        , language_token(0)
        , sql_token(0)
        , comment_token(0)
        , is_token(0)
        , commentExpr(0)
        , routine_list(0)
        , end_token(0)
        , end_module_token(0)
    { setBlockMode(); }

    virtual void skip( Parser *p );

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_MODULE; }
    virtual bool targetSchemaObjectName( QString * /*schemaName*/, QString * /*objectName*/ ) const { return false; }

    virtual CreateModuleStatementAST *asCreateModuleStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CreateModuleStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class CreateRoutineStatementAST: public CreateStatementAST
{
public:
    unsigned create_token;
    RoutineAST *routine;

public:
    CreateRoutineStatementAST()
        : CreateStatementAST()
        , create_token(0)
        , routine(0)
    { setBlockMode(); }

    virtual void skip( Parser *p );
    void setProcedureType( bool isProc = true ) { isProcedure = isProc; }

    virtual unsigned targetKind() const { return (isProcedure ? Sql::Constants::DbSchemaObjectType::C_PROCEDURE : Sql::Constants::DbSchemaObjectType::C_FUNCTION); }
    virtual bool targetSchemaObjectName( QString * /*schemaName*/, QString * /*objectName*/ ) const { return false; }

    virtual CreateRoutineStatementAST *asCreateRoutineStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CreateRoutineStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );

private:
    bool isProcedure;
};

class RoutineAST: public AST
{
public:
    unsigned routine_token;
    RoutineNameAST *routineName;
    unsigned lparen_token;
    ArgumentListAST *arg_list;
    unsigned rparen_token;
    unsigned return_token;
    BaseTypeAST *returnType;
    BlockStatementAST *block;

public:
    RoutineAST()
        : routine_token(0)
        , routineName(0)
        , lparen_token(0)
        , arg_list(0)
        , rparen_token(0)
        , return_token(0)
        , returnType(0)
        , block(0)
        , isProcedure(false)
    {}

    virtual void skip( Parser *p );
    void setProcedureType( bool isProc = true ) { isProcedure = isProc; }

    virtual RoutineAST *asRoutine() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual RoutineAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );

private:
    bool isProcedure;
};

class DropStatementAST: public DdlStatementAST
{
public:
    DropStatementAST() : DdlStatementAST() {}
    virtual DropStatementAST *asDropStatement() { return this; }
    virtual DropStatementAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual DropTableStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual DropViewStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual DropIndexStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual DropTriggerStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class DropModuleStatementAST: public DdlStatementAST
{
public:
    unsigned drop_token;
    unsigned module_token;
    ModuleNameAST *moduleName;

public:
    DropModuleStatementAST()
        : DdlStatementAST()
        , drop_token(0)
        , module_token(0)
        , moduleName(0)
    {}

    virtual unsigned targetKind() const { return Sql::Constants::DbSchemaObjectType::C_MODULE; }
    virtual bool targetSchemaObjectName( QString * /*schemaName*/, QString * /*objectName*/ ) const { return false; }

    virtual DropModuleStatementAST *asDropModuleStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DropModuleStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual TruncateStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class AlterStatementAST: public DdlStatementAST
{
public:
    AlterStatementAST()
        : DdlStatementAST()
    {}

    virtual AlterStatementAST *asAlterStatement() { return this; }

    virtual AlterStatementAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual AlterTableStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual AlterTableClauseAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual AlterTableDropClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class AlterTableAddClauseAST: public AlterTableClauseAST
{
public:
    AlterTableAddClauseAST()
        : AlterTableClauseAST()
    {}

    virtual AlterTableAddClauseAST *asAlterTableAddClause() { return this; }

    virtual AlterTableAddClauseAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual AlterTableAddConstraintClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual AlterTableAddColumnClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class CommentStatementAST: public StatementAST
{
public:
    CommentStatementAST() : StatementAST() {}
    CommentStatementAST *asCommentStatement() { return this; }
    CommentStatementAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual CommentOnTableStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CommentOnColumnStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CommitStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual RollbackStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class CallStatementAST: public StatementAST
{
public:
    unsigned call_token;
    RoutineNameAST *funcName;
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

    virtual CallStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockStatementAST: public StatementAST
{
public:
    unsigned begin_token;
    DeclareListAST *decl_list;
    BlockStatementEntryListAST *entry_list;
    unsigned end_token;

public:
    BlockStatementAST()
        : StatementAST()
        , begin_token(0)
        , decl_list(0)
        , entry_list(0)
        , end_token(0)
    {}

    virtual BlockStatementAST *asBlockStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockStatementAST *clone(Utils::MemoryPool *pool) const;

    virtual void skip( Parser *p );

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

/// Type Specifiers
class BaseTypeAST: public AST
{
public:
    BaseTypeAST() {}
    virtual BaseTypeAST *asBaseType() { return this; }
    virtual BaseTypeAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual CharacterTypeAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual NumberTypeAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SimpleTypeAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual BaseExpressionAST *clone(Utils::MemoryPool *pool) const = 0;

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

    virtual ValueExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual CompoundValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual UnaryValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual BinaryValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SimpleValueExpressionAST: public ValueExpressionAST
{
public:
    SimpleValueExpressionAST()
        : ValueExpressionAST()
    {}

    virtual SimpleValueExpressionAST *asSimpleValueExpression() { return this; }

    virtual SimpleValueExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
};

class ConstantExpressionAST: public SimpleValueExpressionAST
{
public:
    ConstantExpressionAST()
        : SimpleValueExpressionAST()
    {}

    virtual ConstantExpressionAST *asConstantExpression() { return this; }

    virtual ConstantExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual ColumnExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual LiteralExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual NumericExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class FunctionExpressionAST: public SimpleValueExpressionAST
{
public:
    RoutineNameAST *functionName;
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

    virtual FunctionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CastFunctionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SubstringFunctionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CountFunctionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ParameterExpressionAST: public SimpleValueExpressionAST
{
public:
    ParamNameAST *binding;

public:
    ParameterExpressionAST()
        : SimpleValueExpressionAST()
        , binding(0)
    {}

    virtual ParameterExpressionAST *asParameterExpression() { return this; }

    virtual ParameterExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IdentifierExpressionAST: public SimpleValueExpressionAST
{
public:
    IdentifierNameAST *ident;

public:
    IdentifierExpressionAST()
        : SimpleValueExpressionAST()
        , ident(0)
    {}

    virtual IdentifierExpressionAST *asIdentifierExpression() { return this; }

    virtual IdentifierExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IdentDotColumnExpressionAST: public SimpleValueExpressionAST
{
public:
    IdentifierNameAST *ident;
    unsigned dot_token;
    ColumnNameAST *columnName;

public:
    IdentDotColumnExpressionAST()
        : SimpleValueExpressionAST()
        , ident(0)
        , dot_token(0)
        , columnName(0)
    {}

    virtual IdentDotColumnExpressionAST *asIdentDotColumnExpression() { return this; }

    virtual IdentDotColumnExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual NullValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SelectCompoundValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CompoundRValueExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual ValueListExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual SubQueryExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ConditionExpressionAST: public BaseExpressionAST
{
public:
    ConditionExpressionAST()
        : BaseExpressionAST()
    {}

    virtual ConditionExpressionAST *asConditionExpression() { return this; }

    virtual ConditionExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
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

    virtual CompoundConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual InConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual LikeConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual BetweenConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual ExistsConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual UnaryConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual BinaryConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual AssignmentExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CaseExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CaseExpressionClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CaseConditionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
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

    virtual CaseConditionClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class DeclareAST: public AST
{
public:
    unsigned declare_token;
    IdentifierNameAST *identName;
    BaseTypeAST *identType;
    unsigned default_token;
    ValueExpressionAST *defaulExpr;

public:
    DeclareAST()
        : declare_token(0)
        , identName(0)
        , identType(0)
        , default_token(0)
        , defaulExpr(0)
    {}

    virtual DeclareAST *asDeclare() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclareAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ArgumentAST: public AST
{
public:
    unsigned inout_spec_token;
    IdentifierNameAST *ident;
    BaseTypeAST *identType;

public:
    ArgumentAST()
        : inout_spec_token(0)
        , ident(0)
        , identType(0)
    {}

    virtual ArgumentAST *asArgument() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArgumentAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockStatementEntryAST: public AST
{
public:
    BlockStatementEntryAST() {}
    virtual BlockStatementEntryAST *asBlockStatementEntry() { return this; }
    virtual BlockStatementEntryAST *clone(Utils::MemoryPool *pool) const = 0;
};

class BlockInsertStatementAST: public BlockStatementEntryAST
{
public:
    InsertStatementAST *insertStatement;

public:
    BlockInsertStatementAST()
        : BlockStatementEntryAST()
        , insertStatement(0)
    {}

    virtual BlockInsertStatementAST *asBlockInsertStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockInsertStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockUpdateStatementAST: public BlockStatementEntryAST
{
public:
    UpdateStatementAST *updateStatement;

public:
    BlockUpdateStatementAST()
        : BlockStatementEntryAST()
        , updateStatement(0)
    {}

    virtual BlockUpdateStatementAST *asBlockUpdateStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockUpdateStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockDeleteStatementAST: public BlockStatementEntryAST
{
public:
    DeleteStatementAST *deleteStatement;

public:
    BlockDeleteStatementAST()
        : BlockStatementEntryAST()
        , deleteStatement(0)
    {}

    virtual BlockDeleteStatementAST *asBlockDeleteStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockDeleteStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockSelectStatementAST: public BlockStatementEntryAST
{
public:
    unsigned select_token;
    unsigned select_spec_token;         // ALL or DISTINCT
    ResultColumnListAST *columnList;
    unsigned into_token;
    IdentifierNameListAST *into_list;
    unsigned from_token;
    JoinSourceAST *joinSource;
    unsigned where_token;
    ConditionExpressionAST *whereExpr;
    GroupByBlockAST *groupBy;
    LimitToBlockAST *limit_to_block;

public:
    BlockSelectStatementAST()
        : BlockStatementEntryAST()
        , select_token(0)
        , select_spec_token(0)
        , columnList(0)
        , into_token(0)
        , into_list(0)
        , from_token(0)
        , joinSource(0)
        , where_token(0)
        , whereExpr(0)
        , groupBy(0)
        , limit_to_block(0)
    {}

    virtual BlockSelectStatementAST *asBlockSelectStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockSelectStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockSetStatementAST: public BlockStatementEntryAST
{
public:
    unsigned set_token;
    IdentifierNameAST *ident;
    unsigned equal_token;
    ValueExpressionAST *assignExpr;

public:
    BlockSetStatementAST()
        : BlockStatementEntryAST()
        , set_token(0)
        , ident(0)
        , equal_token(0)
        , assignExpr(0)
    {}

    virtual BlockSetStatementAST *asBlockSetStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockSetStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockCommitStatmentAST: public BlockStatementEntryAST
{
public:
    unsigned commit_token;

public:
    BlockCommitStatmentAST()
        : BlockStatementEntryAST()
        , commit_token(0)
    {}

    virtual BlockCommitStatmentAST *asBlockCommitStatment() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockCommitStatmentAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockRollbackStatmentAST: public BlockStatementEntryAST
{
public:
    unsigned rollback_token;

public:
    BlockRollbackStatmentAST()
        : BlockStatementEntryAST()
        , rollback_token(0)
    {}

    virtual BlockRollbackStatmentAST *asBlockRollbackStatment() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockRollbackStatmentAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockCallStatementAST: public BlockStatementEntryAST
{
public:
    CallStatementAST *callExpr;

public:
    BlockCallStatementAST()
        : BlockStatementEntryAST()
        , callExpr(0)
    {}

    virtual BlockCallStatementAST *asBlockCallStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockCallStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class IfStatementAST: public BlockStatementEntryAST
{
public:
    unsigned if_token;
    ConditionExpressionAST *conditionExpr;
    unsigned then_token;
    BlockStatementEntryListAST *then_entry_list;
    unsigned else_token;
    BlockStatementEntryListAST *else_entry_list;
    unsigned end_token;
    unsigned end_if_token;

public:
    IfStatementAST()
        : BlockStatementEntryAST()
        , if_token(0)
        , conditionExpr(0)
        , then_token(0)
        , then_entry_list(0)
        , else_token(0)
        , else_entry_list(0)
        , end_token(0)
        , end_if_token(0)
    {}

    virtual IfStatementAST *asIfStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IfStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class LoopStatementAST: public BlockStatementEntryAST
{
public:
    unsigned while_token;
    ConditionExpressionAST *whileExpr;
    unsigned loop_token;
    BlockStatementEntryListAST *entry_list;
    unsigned end_token;
    unsigned end_loop_token;

public:
    LoopStatementAST()
        : BlockStatementEntryAST()
        , while_token(0)
        , whileExpr(0)
        , loop_token(0)
        , entry_list(0)
        , end_token(0)
        , end_loop_token(0)
    {}

    virtual LoopStatementAST *asLoopStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LoopStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class DeclareCursorClauseAST: public AST
{
public:
    unsigned read_or_update_token;
    unsigned only_token;
    unsigned table_token;       // optional
    unsigned cursor_token;
    CursorNameAST *cursorName;
    unsigned for_token;

public:
    DeclareCursorClauseAST()
        : read_or_update_token(0)
        , only_token(0)
        , table_token(0)
        , cursor_token(0)
        , cursorName(0)
        , for_token(0)
    {}

    virtual DeclareCursorClauseAST *asDeclareCursorClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclareCursorClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class ForStatementAST: public BlockStatementEntryAST
{
public:
    unsigned for_token;
    IdentifierNameAST *forName;
    unsigned as_token;
    unsigned each_token;    // f-set optional
    unsigned row_token;     // f-set optional
    unsigned of_token;      // f-set optional
    DeclareCursorClauseAST *cursor; // Cursor optional
    StatementAST *selectStatement;
    unsigned do_token;
    BlockStatementEntryListAST *entry_list;
    unsigned end_token;
    unsigned end_for_token;

public:
    ForStatementAST()
        : BlockStatementEntryAST()
        , for_token(0)
        , forName(0)
        , as_token(0)
        , each_token(0)
        , row_token(0)
        , of_token(0)
        , cursor(0)
        , selectStatement(0)
        , do_token(0)
        , entry_list(0)
        , end_token(0)
        , end_for_token(0)
    {}

    virtual ForStatementAST *asForStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ForStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockLabelAST: public BlockStatementEntryAST
{
public:
    LabelNameAST *labelName;
    unsigned colon_token;

public:
    BlockLabelAST()
        : BlockStatementEntryAST()
        , labelName(0)
        , colon_token(0)
    {}

    virtual BlockLabelAST *asBlockLabel() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockLabelAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class LeaveStatementAST: public BlockStatementEntryAST
{
public:
    unsigned leave_token;
    LabelNameAST *labelName;

public:
    LeaveStatementAST()
        : BlockStatementEntryAST()
        , leave_token(0)
        , labelName(0)
    {}

    virtual LeaveStatementAST *asLeaveStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LeaveStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockBlockStatementAST: public BlockStatementEntryAST
{
public:
    BlockStatementAST *block;

public:
    BlockBlockStatementAST()
        : BlockStatementEntryAST()
        , block(0)
    {}

    virtual BlockBlockStatementAST *asBlockBlockStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockBlockStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class BlockReturnStatementAST: public BlockStatementEntryAST
{
public:
    unsigned return_token;
    ValueExpressionAST *returnExpr;

public:
    BlockReturnStatementAST()
        : BlockStatementEntryAST()
        , return_token(0)
        , returnExpr(0)
    {}

    virtual BlockReturnStatementAST *asBlockReturnStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BlockReturnStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SqlCheckNamesWalker: public Sql::BaseWalker
{
public:
    SqlCheckNamesWalker( AST *ast, Sql::TranslationUnit *translationUnit )
        : Sql::BaseWalker()
        , m_ast(ast)
        , m_translationUnit(translationUnit)
    {}

    virtual void walk( Sql::BaseWalkReflector *reflector );

private:
    AST *m_ast;
    Sql::TranslationUnit *m_translationUnit;
};

} // namespace RDB

#endif // RDB_AST_H
