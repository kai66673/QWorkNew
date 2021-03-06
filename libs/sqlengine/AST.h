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

#ifndef SQL_AST_H
#define SQL_AST_H

#include <QString>

#include "ASTfwd.h"
#include "MemoryPool.h"
#include "TranslationUnit.h"
#include "SqlToken.h"
#include "BaseWalker.h"
#include "sql_constants.h"

namespace Sql {

template <typename _Tp>
class SQL_EXPORT List: public Utils::Managed
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

class SQL_EXPORT AST: public Utils::Managed
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

    virtual TranslationUnitAST *asTranslationUnit() { return nullptr; }
    virtual StatementAST *asStatement() { return nullptr; }
    virtual InvalidStatementAST *asInvalidStatement() { return nullptr; }
    virtual UnknownStatementAST *asUnknownStatement() { return nullptr; }
    virtual DmlStatementAST *asDmlStatement() { return nullptr; }
    virtual SelectStatementAST *asSelectStatement() { return nullptr; }
    virtual SelectCoreAST *asSelectCore() { return nullptr; }
    virtual SelectCompoundAST *asSelectCompound() { return nullptr; }
    virtual SelectCompoundUnionAST *asSelectCompoundUnion() { return nullptr; }
    virtual SelectCompoundIntersectAST *asSelectCompoundIntersect() { return nullptr; }
    virtual SelectCompoundExceptAST *asSelectCompoundExcept() { return nullptr; }
    virtual SelectCompoundMinusAST *asSelectCompoundMinus() { return nullptr; }
    virtual OrderByBlockAST *asOrderByBlock() { return nullptr; }
    virtual LimitToBlockAST *asLimitToBlock() { return nullptr; }
    virtual OrderingTermAST *asOrderingTerm() { return nullptr; }
    virtual NameAST *asName() { return nullptr; }
    virtual SchemaNameAST *asSchemaName() { return nullptr; }
    virtual TableNameAST *asTableName() { return nullptr; }
    virtual ColumnAliasAST *asColumnAlias() { return nullptr; }
    virtual TableAliasAST *asTableAlias() { return nullptr; }
    virtual ColumnNameAST *asColumnName() { return nullptr; }
    virtual FunctionNameAST *asFunctionName() { return nullptr; }
    virtual ParamNameAST *asParamName() { return nullptr; }
    virtual LabelNameAST *asLabelName() { return nullptr; }
    virtual ViewNameAST *asViewName() { return nullptr; }
    virtual IndexNameAST *asIndexName() { return nullptr; }
    virtual TriggerNameAST *asTriggerName() { return nullptr; }
    virtual ConstraintNameAST *asConstraintName() { return nullptr; }
    virtual SchemaTableNameAST *asSchemaTableName() { return nullptr; }
    virtual SchemaIndexNameAST *asSchemaIndexName() { return nullptr; }
    virtual SchemaTriggerNameAST *asSchemaTriggerName() { return nullptr; }
    virtual ResultColumnAST *asResultColumn() { return nullptr; }
    virtual StarResultColumnAST *asStarResultColumn() { return nullptr; }
    virtual ExprResultColumnAST *asExprResultColumn() { return nullptr; }
    virtual SubQueryAST *asSubQuery() { return nullptr; }
    virtual SingleSourceAST *asSingleSource() { return nullptr; }
    virtual DirectSingleSourceAST *asDirectSingleSource() { return nullptr; }
    virtual SubQuerySingleSourceAST *asSubQuerySingleSource() { return nullptr; }
    virtual JoinOpAST *asJoinOp() { return nullptr; }
    virtual JoinConstraintAST *asJoinConstraint() { return nullptr; }
    virtual JoinItemAST *asJoinItem() { return nullptr; }
    virtual JoinSourceAST *asJoinSource() { return nullptr; }
    virtual GroupByBlockAST *asGroupByBlock() { return nullptr; }

    virtual DeleteStatementAST *asDeleteStatement() { return nullptr; }
    virtual InsertStatementAST *asInsertStatement() { return nullptr; }
    virtual InsertClauseAST *asInsertClause() { return nullptr; }
    virtual InsertClauseAsValuesAST *asInsertClauseAsValues() { return nullptr; }
    virtual InsertClauseAsSelectAST *asInsertClauseAsSelect() { return nullptr; }
    virtual CreateTableClauseAST *asCreateTableClause() { return nullptr; }
    virtual CreateTableDirectClauseAST *asCreateTableDirectClause() { return nullptr; }
    virtual CreateTableAsSelectClauseAST *asCreateTableAsSelectClause() { return nullptr; }
    virtual CreateTriggerRefClauseAST *asCreateTriggerRefClause() { return nullptr; }
    virtual CreateRdbTriggerBodyAST *asCreateRdbTriggerBody() { return nullptr; }
    virtual CreateRdbTriggerBodyItemAST *asCreateRdbTriggerBodyItem() { return nullptr; }
    virtual CreateTriggerBodyAST *asCreateTriggerBody() { return nullptr; }
    virtual UpdateStatementAST *asUpdateStatement() { return nullptr; }
    virtual ColumnDefinitionAST *asColumnDefinition() { return nullptr; }
    virtual ConstraintAST *asConstraint() { return nullptr; }
    virtual TableConstraintAST *asTableConstraint() { return nullptr; }
    virtual ColumnConstraintAST *asColumnConstraint() { return nullptr; }
    virtual UniqueOrPkTableConstraintAST *asUniqueOrPkTableConstraint() { return nullptr; }
    virtual CheckTableConstraintAST *asCheckTableConstraint() { return nullptr; }
    virtual FkTableConstraintAST *asFkTableConstraint() { return nullptr; }
    virtual FkReferencesClauseAST *asForeignKeyClause() { return nullptr; }
    virtual PkColumnConstraintAST *asPkColumnConstraint() { return nullptr; }
    virtual NotNullColumnConstraintAST *asNotNullColumnConstraint() { return nullptr; }
    virtual UniqueColumnConstraintAST *asUniqueColumnConstraint() { return nullptr; }
    virtual CheckColumnConstraintAST *asCheckColumnConstraint() { return nullptr; }
    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return nullptr; }
    virtual SortedColumnNameAST *asSortedColumnName() { return nullptr; }

    virtual DdlStatementAST *asDdlStatement() { return nullptr; }
    virtual CreateStatementAST *asCreateStatement() { return nullptr; }
    virtual CreateTableStatementAST *asCreateTableStatement() { return nullptr; }
    virtual CreateViewStatementAST *asCreateViewStatement() { return nullptr; }
    virtual CreateIndexStatementAST *asCreateIndexStatement() { return nullptr; }
    virtual CreateTriggerStatementAST *asCreateTriggerStatement() { return nullptr; }
    virtual DropStatementAST *asDropStatement() { return nullptr; }
    virtual DropTableStatementAST *asDropTableStatement() { return nullptr; }
    virtual DropViewStatementAST *asDropViewStatement() { return nullptr; }
    virtual DropIndexStatementAST *asDropIndexStatement() { return nullptr; }
    virtual DropTriggerStatementAST *asDropTriggerStatement() { return nullptr; }
    virtual TruncateStatementAST *asTruncateStatement() { return nullptr; }
    virtual AlterStatementAST *asAlterStatement() { return nullptr; }
    virtual AlterTableStatementAST *asAlterTableStatement() { return nullptr; }
    virtual AlterTableClauseAST *asAlterTableClause() { return nullptr; }
    virtual AlterTableDropClauseAST *asAlterTableDropClause() { return nullptr; }
    virtual AlterTableAddClauseAST *asAlterTableAddClause() { return nullptr; }
    virtual AlterTableAddConstraintClauseAST *asAlterTableAddConstraintClause() { return nullptr; }
    virtual AlterTableAddColumnClauseAST *asAlterTableAddColumnClause() { return nullptr; }
    virtual AlterTableModifyColumnClauseAST *asAlterTableModifyColumnClause() { return nullptr; }

    virtual CommentStatementAST *asCommentStatement() { return nullptr; }
    virtual CommentOnTableStatementAST *asCommentOnTableStatement() { return nullptr; }
    virtual CommentOnColumnStatementAST *asCommentOnColumnStatement() { return nullptr; }

    virtual CommitStatementAST *asCommitStatement() { return nullptr; }
    virtual RollbackStatementAST *asRollbackStatement() { return nullptr; }
    virtual CallStatementAST *asCallStatement() { return nullptr; }

    virtual BaseTypeAST *asBaseType() { return nullptr; }
    virtual CharacterTypeAST *asCharacterType() { return nullptr; }
    virtual NumberTypeAST *asNumericType() { return nullptr; }
    virtual SimpleTypeAST *asSimpleType() { return nullptr; }

    // Expressions
    virtual BaseExpressionAST *asBaseExpression() { return nullptr; }
    virtual ValueExpressionAST *asValueExpression() { return nullptr; }
    virtual CompoundValueExpressionAST *asCompoundValueExpression() { return nullptr; }
    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return nullptr; }
    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return nullptr; }
    virtual SimpleValueExpressionAST *asSimpleValueExpression() { return nullptr; }
    virtual ConstantExpressionAST *asConstantExpression() { return nullptr; }
    virtual ColumnExpressionAST *asColumnExpression() { return nullptr; }
    virtual LiteralExpressionAST *asLiteralExpression() { return nullptr; }
    virtual NumericExpressionAST *asNumericExpression() { return nullptr; }
    virtual FunctionExpressionAST *asFunctionExpression() { return nullptr; }
    virtual CastFunctionAST *asCastFunction() { return nullptr; }
    virtual SubstringFunctionAST *asSubstringFunction() { return nullptr; }
    virtual CountFunctionAST *asCountFunction() { return nullptr; }
    virtual ParameterExpressionAST *asParameterExpression() { return nullptr; }
    virtual NullValueExpressionAST *asNullValueExpression() { return nullptr; }
    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return nullptr; }
    virtual CompoundRValueExpressionAST *asCompoundRValueExpression() { return nullptr; }
    virtual ValueListExpressionAST *asValueListExpression() { return nullptr; }
    virtual SubQueryExpressionAST *asSubQueryExpression() { return nullptr; }
    virtual ConditionExpressionAST *asConditionExpression() { return nullptr; }
    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return nullptr; }
    virtual InConditionExpressionAST *asInConditionExpression() { return nullptr; }
    virtual LikeConditionExpressionAST *asLikeConditionExpression() { return nullptr; }
    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return nullptr; }
    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return nullptr; }
    virtual UnaryConditionExpressionAST *asUnaryConditionExpression() { return nullptr; }
    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return nullptr; }
    virtual AssignmentExpressionAST *asAssignmentExpression() { return nullptr; }
    virtual CaseExpressionAST *asCaseExpression() { return nullptr; }
    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return nullptr; }
    virtual CaseConditionAST *asCaseCondition() { return nullptr; }
    virtual CaseConditionClauseAST *asCaseConditionClause() { return nullptr; }

    // Additional for SelectStatement and InvalidSelectStatement
    virtual bool isSelectStatement() const { return false; }

protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
    virtual bool match0(AST *, ASTMatcher *) = 0;
};

class SQL_EXPORT TranslationUnitAST: public AST
{
public:
    StatementListAST *statement_list;

public:
    TranslationUnitAST()
        : statement_list(nullptr)
    {}

    virtual TranslationUnitAST *asTranslationUnit() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TranslationUnitAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT StatementAST: public AST
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

class SQL_EXPORT InvalidStatementAST: public StatementAST
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

class SQL_EXPORT UnknownStatementAST: public StatementAST
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

class SQL_EXPORT DmlStatementAST: public StatementAST
{
public:
    DmlStatementAST() : StatementAST() {}
    virtual DmlStatementAST *asDmlStatement();
    virtual StatementAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT SelectStatementAST: public DmlStatementAST
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
        , select_core_list(nullptr)
        , select_compound_list(nullptr)
        , order_by_block(nullptr)
        , limit_to_block(nullptr)
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

class SQL_EXPORT SelectCoreAST: public AST
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
        , columnList(nullptr)
        , from_token(0)
        , joinSource(nullptr)
        , where_token(0)
        , whereExpr(nullptr)
        , groupBy(nullptr)
    {}

    virtual SelectCoreAST *asSelectCore() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SelectCoreAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SelectCompoundAST: public AST
{
public:
    SelectCompoundAST() {}
    virtual SelectCompoundAST *asSelectCompound();
    virtual SelectCompoundAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT SelectCompoundUnionAST: public SelectCompoundAST
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

class SQL_EXPORT SelectCompoundIntersectAST: public SelectCompoundAST
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

class SQL_EXPORT SelectCompoundExceptAST: public SelectCompoundAST
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

class SQL_EXPORT SelectCompoundMinusAST: public SelectCompoundAST
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

class SQL_EXPORT OrderByBlockAST: public AST
{
public:
    unsigned order_token;
    unsigned by_token;
    OrderingTermListAST *term_list;

public:
    OrderByBlockAST()
        : order_token(0)
        , by_token(0)
        , term_list(nullptr)
    {}

    virtual OrderByBlockAST *asOrderByBlock() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual OrderByBlockAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT LimitToBlockAST: public AST
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
        , expr(nullptr)
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

class SQL_EXPORT OrderingTermAST: public AST
{
public:
    ValueExpressionAST *expr;
    unsigned asc_or_desc_token;

public:
    OrderingTermAST()
        : expr(nullptr)
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

class SQL_EXPORT NameAST: public AST
{
public:
    unsigned name_token;

public: // Annotations
    const Identifier *name;

public:
    NameAST()
        : name_token(0)
        , name(nullptr)
    {}

    virtual NameAST *asName() { return this; }

    virtual NameAST *clone(Utils::MemoryPool *pool) const = 0;

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;
};

class SQL_EXPORT SchemaNameAST: public NameAST
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

class SQL_EXPORT TableNameAST: public NameAST
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

class SQL_EXPORT ViewNameAST: public NameAST
{
public:
    ViewNameAST() : NameAST() {}

    virtual ViewNameAST *asViewName() { return this; }

    virtual ViewNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT IndexNameAST: public NameAST
{
public:
    IndexNameAST() : NameAST() {}

    virtual IndexNameAST *asIndexName() { return this; }

    virtual IndexNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT TriggerNameAST: public NameAST
{
public:
    TriggerNameAST() : NameAST() {}

    virtual TriggerNameAST *asTriggerName() { return this; }

    TriggerNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ConstraintNameAST: public NameAST
{
public:
    ConstraintNameAST() : NameAST() {}

    virtual ConstraintNameAST *asConstraintName() { return this; }

    ConstraintNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SchemaTableNameAST: public AST
{
public:
    SchemaNameAST *schemaName;
    unsigned dot_token;
    TableNameAST *tableName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    SchemaTableNameAST()
        : schemaName(nullptr)
        , dot_token(0)
        , tableName(nullptr)
    {}

    virtual SchemaTableNameAST *asSchemaTableName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    SchemaTableNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SchemaIndexNameAST: public AST
{
public:
    SchemaNameAST *schemaName;
    unsigned dot_token;
    IndexNameAST *indexName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    SchemaIndexNameAST()
        : schemaName(nullptr)
        , dot_token(0)
        , indexName(nullptr)
    {}

    virtual SchemaIndexNameAST *asSchemaIndexName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    SchemaIndexNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SchemaTriggerNameAST: public AST
{
public:
    SchemaNameAST *schemaName;
    unsigned dot_token;
    TriggerNameAST *triggerName;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    SchemaTriggerNameAST()
        : schemaName(nullptr)
        , dot_token(0)
        , triggerName(nullptr)
    {}

    virtual SchemaTriggerNameAST *asSchemaTriggerName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    SchemaTriggerNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );

};

class SQL_EXPORT TableAliasAST: public NameAST
{
public: // Annotations
    SubQueryAST  *subqueryAliasType;
    SchemaTableNameAST *schemaTableName;

public:
    TableAliasAST()
        : NameAST()
        , subqueryAliasType(nullptr)
        , schemaTableName(nullptr)
    {}

    virtual TableAliasAST *asTableAlias() { return this; }

    virtual TableAliasAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ColumnNameAST: public NameAST
{
public: // Annotations
    SchemaNameAST *schemaName;
    NameAST *tableOfColumn;    //    columnOwner->asTableName()
                               // OR columnOwner->asTableAlias()->subqueryAliasType
                               // OR columnOwner->asTableAlias()->tableAliasType
public:
    ColumnNameAST()
        : NameAST()
        , schemaName(nullptr)
        , tableOfColumn(nullptr)
    {}

    virtual ColumnNameAST *asColumnName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ColumnNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ParamNameAST: public NameAST
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

class SQL_EXPORT LabelNameAST: public NameAST
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

class SQL_EXPORT ColumnAliasAST: public NameAST
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

class SQL_EXPORT FunctionNameAST: public NameAST
{
public:
    FunctionNameAST()
        : NameAST()
    {}

    virtual FunctionNameAST *asFunctionName() { return this; }

    virtual FunctionNameAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ResultColumnAST: public AST
{
public:
    ResultColumnAST()
    {}

    virtual ResultColumnAST *asResultColumn();

    virtual ResultColumnAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT StarResultColumnAST: public ResultColumnAST
{
public:
    SchemaTableNameAST *schemaTableName;
    unsigned dot_token;
    unsigned star_token;

public:
    StarResultColumnAST()
        : schemaTableName(nullptr)
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

class SQL_EXPORT ExprResultColumnAST: public ResultColumnAST
{
public:
    ValueExpressionAST *expr;
    unsigned as_token;
    ColumnAliasAST *columnAlias;

public:
    ExprResultColumnAST()
        : expr(nullptr)
        , as_token(0)
        , columnAlias(nullptr)
    {}

    virtual ExprResultColumnAST *asExprResultColumn() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExprResultColumnAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SubQueryAST: public AST
{
public:
    unsigned lparen_token;
    StatementAST *selectStatement;
    unsigned rparen_token;

public:
    SubQueryAST()
        : lparen_token(0)
        , selectStatement(nullptr)
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

class SQL_EXPORT SingleSourceAST: public AST
{
public:
    SingleSourceAST()
    {}

    virtual SingleSourceAST *asSingleSource();

    virtual SingleSourceAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT DirectSingleSourceAST: public SingleSourceAST
{
public:
    SchemaTableNameAST *schemaTableName;
    unsigned as_token;
    TableAliasAST *tableAlias;

public:
    DirectSingleSourceAST()
        : schemaTableName(nullptr)
        , as_token(0)
        , tableAlias(nullptr)
    {}

    virtual DirectSingleSourceAST *asDirectSingleSource() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DirectSingleSourceAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SubQuerySingleSourceAST: public SingleSourceAST
{
public:
    SubQueryAST *subQuery;
    unsigned as_token;
    TableAliasAST *tableAlias;

public:
    SubQuerySingleSourceAST()
        : subQuery(nullptr)
        , as_token(0)
        , tableAlias(nullptr)
    {}

    virtual SubQuerySingleSourceAST *asSubQuerySingleSource() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SubQuerySingleSourceAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT JoinOpAST: public AST
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

class SQL_EXPORT JoinConstraintAST: public AST
{
public:
    unsigned on_token;
    ConditionExpressionAST *expr;

public:
    JoinConstraintAST()
        : on_token(0)
        , expr(nullptr)
    {}

    virtual JoinConstraintAST *asJoinConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual JoinConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT JoinItemAST: public AST
{
public:
    JoinOpAST *joinOp;
    SingleSourceAST *singleSource;
    JoinConstraintAST *joinConstraint;

public:
    JoinItemAST()
        : joinOp(nullptr)
        , singleSource(nullptr)
        , joinConstraint(nullptr)
    {}

    virtual JoinItemAST *asJoinItem() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual JoinItemAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT JoinSourceAST: public AST
{
public:
    SingleSourceAST *firstSource;
    JoinItemListAST *source_list;

public:
    JoinSourceAST()
        : firstSource(nullptr)
        , source_list(nullptr)
    {}

    JoinSourceAST *asJoinSource() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual JoinSourceAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT GroupByBlockAST: public AST
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
        , group_items_list(nullptr)
        , having_token(0)
        , havingExpr(nullptr)
    {}

    virtual GroupByBlockAST *asGroupByBlock() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual GroupByBlockAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT DeleteStatementAST: public DmlStatementAST
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
        , source(nullptr)
        , where_token(0)
        , whereExpr(nullptr)
    {}

    virtual DeleteStatementAST *asDeleteStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeleteStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT InsertStatementAST: public DmlStatementAST
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
        , source(nullptr)
        , lparen_columns_token(0)
        , columns_list(nullptr)
        , rparen_columns_token(0)
        , insClause(nullptr)
    {}

    virtual InsertStatementAST *asInsertStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual InsertStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT UpdateStatementAST: public DmlStatementAST
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
        , source(nullptr)
        , set_token(0)
        , upd_list(nullptr)
        , where_token(0)
        , whereExpr(nullptr)
    {}

    virtual UpdateStatementAST *asUpdateStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UpdateStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT InsertClauseAST: public AST
{
public:
    InsertClauseAST() {}
    virtual InsertClauseAST *asInsertClause();
    virtual InsertClauseAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CreateTableClauseAST: public AST
{
public:
    CreateTableClauseAST() {}
    virtual CreateTableClauseAST *asCreateTableClause();
    virtual CreateTableClauseAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CreateTableDirectClauseAST: public CreateTableClauseAST
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
        , column_def_list(nullptr)
        , constraintList(nullptr)
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

class SQL_EXPORT CreateTableAsSelectClauseAST: public CreateTableClauseAST
{
public:
    unsigned as_token;
    StatementAST *selectStatement;

public:
    CreateTableAsSelectClauseAST()
        : CreateTableClauseAST()
        , as_token(0)
        , selectStatement(nullptr)
    {}

    CreateTableAsSelectClauseAST *asCreateTableAsSelectClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CreateTableAsSelectClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CreateTriggerRefClauseAST: public AST
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
        , newAlias(nullptr)
        , old_token(0)
        , aso_token(0)
        , oldAlias(nullptr)
    {}

    virtual CreateTriggerRefClauseAST *asCreateTriggerRefClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    CreateTriggerRefClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CreateTriggerBodyAST: public AST
{
public:
    CreateTriggerBodyAST() {}
    virtual CreateTriggerBodyAST *asCreateTriggerBody();
    virtual CreateTriggerBodyAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CreateRdbTriggerBodyAST: public CreateTriggerBodyAST
{
public:
    CreateRdbTriggerBodyListAST *item_list;

public:
    CreateRdbTriggerBodyAST()
        : CreateTriggerBodyAST()
        , item_list(nullptr)
    {}

    virtual CreateRdbTriggerBodyAST *asCreateRdbTriggerBody() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    CreateRdbTriggerBodyAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CreateRdbTriggerBodyItemAST: public AST
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
        , whenExpr(nullptr)
        , rparen_when_token(0)
        , lparen_token(0)
        , stat_list(nullptr)
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

class SQL_EXPORT ColumnDefinitionAST: public AST
{
public:
    ColumnNameAST *columnName;
    BaseTypeAST *columnType;
    unsigned default_token;
    ConstantExpressionAST *defaultExpr;
    ColumnConstraintListAST *constraintList;

public:
    ColumnDefinitionAST()
        : columnName(nullptr)
        , columnType(nullptr)
        , default_token(0)
        , defaultExpr(nullptr)
        , constraintList(nullptr)
    {}

    virtual ColumnDefinitionAST *asColumnDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ColumnDefinitionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ConstraintAST: public AST
{
public:
    unsigned constraint_token;
    ConstraintNameAST *constraintName;

public:
    ConstraintAST()
        : constraint_token(0)
        , constraintName(nullptr)
    {}

    virtual ConstraintAST *asConstraint();

    virtual ConstraintAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT TableConstraintAST: public ConstraintAST
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

    virtual TableConstraintAST *asTableConstraint();

    virtual TableConstraintAST *clone(Utils::MemoryPool *pool) const = 0;

private:
    TableConstraintType constraintType;
};

class SQL_EXPORT ColumnConstraintAST: public ConstraintAST
{
public:
    ColumnConstraintAST()
        : ConstraintAST()
    {}

    virtual ColumnConstraintAST *asColumnConstraint();

    virtual ColumnConstraintAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT UniqueOrPkTableConstraintAST: public TableConstraintAST
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
        , column_list(nullptr)
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

class SQL_EXPORT CheckTableConstraintAST: public TableConstraintAST
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
        , checkExpr(nullptr)
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

class SQL_EXPORT FkTableConstraintAST: public TableConstraintAST
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
        , col_list(nullptr)
        , rparen_token(0)
        , refClause(nullptr)
    {}

    virtual FkTableConstraintAST *asFkTableConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual FkTableConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT FkReferencesClauseAST: public AST
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
        , refSchemaTableName(nullptr)
        , ref_lparen_token(0)
        , ref_col_list(nullptr)
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

class SQL_EXPORT PkColumnConstraintAST: public ColumnConstraintAST
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

class SQL_EXPORT NotNullColumnConstraintAST: public ColumnConstraintAST
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

class SQL_EXPORT UniqueColumnConstraintAST: public ColumnConstraintAST
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

class SQL_EXPORT CheckColumnConstraintAST: public ColumnConstraintAST
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
        , checkExpr(nullptr)
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

class SQL_EXPORT ReferencesColumnConstraintAST: public ColumnConstraintAST
{
public:
    FkReferencesClauseAST *refClause;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    ReferencesColumnConstraintAST()
        : ColumnConstraintAST()
        , refClause(nullptr)
    {}

    virtual ReferencesColumnConstraintAST *asReferencesColumnConstraint() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ReferencesColumnConstraintAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SortedColumnNameAST: public AST
{
public:
    ColumnNameAST *columnName;
    unsigned asc_or_desc_token;

public:
    SortedColumnNameAST()
        : columnName(nullptr)
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

class SQL_EXPORT InsertClauseAsValuesAST: public InsertClauseAST
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
        , val_list(nullptr)
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

class SQL_EXPORT InsertClauseAsSelectAST: public InsertClauseAST
{
public:
    StatementAST *selectStatement;

public:
    InsertClauseAsSelectAST()
        : InsertClauseAST()
        , selectStatement(nullptr)
    {}

    virtual InsertClauseAsSelectAST *asInsertClauseAsSelect() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual InsertClauseAsSelectAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT DdlStatementAST: public StatementAST
{
public:
    DdlStatementAST() : StatementAST() {}
    virtual DdlStatementAST *asDdlStatement();
    virtual DdlStatementAST *clone(Utils::MemoryPool *pool) const = 0;

    virtual unsigned targetKind() const = 0;
    virtual bool targetSchemaObjectName( QString *schemaName, QString *objectName ) const = 0;
    virtual bool targetTableObjectName( QString *tableName, unsigned *tableObjectType ) const {
        *tableName = "";
        *tableObjectType = 0;
        return true;
    }
};

class SQL_EXPORT CreateStatementAST: public DdlStatementAST
{
public:
    CreateStatementAST() : DdlStatementAST() {}
    virtual CreateStatementAST *asCreateStatement();
    virtual CreateStatementAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CreateTableStatementAST: public CreateStatementAST
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
        , schemaTableName(nullptr)
        , tableClause(nullptr)
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

class SQL_EXPORT CreateViewStatementAST: public CreateStatementAST
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
        , schemaViewName(nullptr)
        , as_token(0)
        , selectStatement(nullptr)
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

class SQL_EXPORT CreateIndexStatementAST: public CreateStatementAST
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
        , schemaIndexName(nullptr)
        , on_token(0)
        , schemaTableName(nullptr)
        , lparen_token(0)
        , columns_list(nullptr)
        , rparen_token(0)
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

class SQL_EXPORT CreateTriggerStatementAST: public CreateStatementAST
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
        , schemaTriggerName(nullptr)
        , before_or_after_token(0)
        , action_token(0)
        , of_token(0)
        , col_list(nullptr)
        , on_token(0)
        , schemaTableName(nullptr)
        , triggerRefClause(nullptr)
        , triggerBody(nullptr)
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

class SQL_EXPORT DropStatementAST: public DdlStatementAST
{
public:
    DropStatementAST() : DdlStatementAST() {}
    virtual DropStatementAST *asDropStatement();
    virtual DropStatementAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT DropTableStatementAST: public DropStatementAST
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
        , schemaTableName(nullptr)
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

class SQL_EXPORT DropViewStatementAST: public DropStatementAST
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
        , schemaViewName(nullptr)
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

class SQL_EXPORT DropIndexStatementAST: public DropStatementAST
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
        , schemaIndexName(nullptr)
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

class SQL_EXPORT DropTriggerStatementAST: public DropStatementAST
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
        , schemaTriggerName(nullptr)
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

class SQL_EXPORT TruncateStatementAST: public DdlStatementAST
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
        , schemaTableName(nullptr)
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

class SQL_EXPORT AlterStatementAST: public DdlStatementAST
{
public:
    AlterStatementAST()
        : DdlStatementAST()
    {}

    virtual AlterStatementAST *asAlterStatement();

    virtual AlterStatementAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT AlterTableStatementAST: public AlterStatementAST
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
        , schemaTableName(nullptr)
        , alterTableClause(nullptr)
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

class SQL_EXPORT AlterTableClauseAST: public AST
{
public:
    enum TableObjectKind {
        Unknown = 0,
        Field,
        Constraint,
        Index
    };

    TableObjectKind objectKind;

public:
    AlterTableClauseAST()
        : objectKind(Unknown)
    {}

    virtual AlterTableClauseAST *asAlterTableClause();

    virtual AlterTableClauseAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT AlterTableDropClauseAST: public AlterTableClauseAST
{
public:
    unsigned drop_token;
    unsigned drop_object_type_token;    // constraint or column or index or foreign
    unsigned drop_object_type_token1;   // constraint or column or index or key
    NameAST *dropObjectName;            // ColumnNameAST or ConstraintNameAST

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableDropClauseAST()
        : AlterTableClauseAST()
        , drop_token(0)
        , drop_object_type_token(0)
        , drop_object_type_token1(0)
        , dropObjectName(nullptr)
    {}

    virtual AlterTableDropClauseAST *asAlterTableDropClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AlterTableDropClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT AlterTableAddClauseAST: public AlterTableClauseAST
{
public:
    AlterTableAddClauseAST()
        : AlterTableClauseAST()
    {}

    virtual AlterTableAddClauseAST *asAlterTableAddClause();

    virtual AlterTableAddClauseAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT AlterTableAddConstraintClauseAST: public AlterTableAddClauseAST
{
public:
    unsigned add_token;
    TableConstraintAST *constraint;

    bool parse( Parser *p, Utils::MemoryPool *pool );

public:
    AlterTableAddConstraintClauseAST()
        : AlterTableAddClauseAST()
        , add_token(0)
        , constraint(nullptr)
    {}

    virtual AlterTableAddConstraintClauseAST *asAlterTableAddConstraintClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AlterTableAddConstraintClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT AlterTableAddColumnClauseAST: public AlterTableAddClauseAST
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
        , column(nullptr)
    {}

    virtual AlterTableAddColumnClauseAST *asAlterTableAddColumnClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AlterTableAddColumnClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT AlterTableModifyColumnClauseAST: public AlterTableClauseAST
{
public:
    unsigned modify_token;
    unsigned column_token;
    ColumnDefinitionAST *column;

    bool parse(Parser *p, Utils::MemoryPool *pool);

public:
    AlterTableModifyColumnClauseAST()
        : AlterTableClauseAST()
        , modify_token(0),
          column_token(0),
          column(nullptr)
    {}

    virtual AlterTableModifyColumnClauseAST *asAlterTableModifyColumnClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AlterTableModifyColumnClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor *visitor);
    virtual bool match0(AST *, ASTMatcher *);
};

class SQL_EXPORT CommentStatementAST: public StatementAST
{
public:
    CommentStatementAST() : StatementAST() {}
    CommentStatementAST *asCommentStatement();
    CommentStatementAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CommentOnTableStatementAST: public CommentStatementAST
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
        , schemaTableName(nullptr)
        , is_token(0)
        , comment(nullptr)
    {}

    virtual CommentOnTableStatementAST *asCommentOnTableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CommentOnTableStatementAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CommentOnColumnStatementAST: public CommentStatementAST
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
        , schemaTableName(nullptr)
        , dot_token(0)
        , column(nullptr)
        , is_token(0)
        , comment(nullptr)
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

class SQL_EXPORT CallStatementAST: public StatementAST
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
        , funcName(nullptr)
        , lparen_token(0)
        , arg_list(nullptr)
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

/// Type Specifiers
class BaseTypeAST: public AST
{
public:
    BaseTypeAST() {}
    virtual BaseTypeAST *asBaseType();
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
class SQL_EXPORT BaseExpressionAST: public AST
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

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;
};

class SQL_EXPORT ValueExpressionAST: public BaseExpressionAST
{
public:
    ValueExpressionAST()
        : BaseExpressionAST()
    {}

    virtual ValueExpressionAST *asValueExpression();

    virtual ValueExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CompoundValueExpressionAST: public ValueExpressionAST
{
public:
    unsigned lparen_token;
    ValueExpressionAST *valExpr;
    unsigned rparen_token;

public:
    CompoundValueExpressionAST ()
        : ValueExpressionAST()
        , lparen_token(0)
        , valExpr(nullptr)
        , rparen_token(0)
    {}

    virtual CompoundValueExpressionAST *asCompoundValueExpression() { return this; }

    virtual CompoundValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT UnaryValueExpressionAST: public ValueExpressionAST
{
public:
    unsigned op_token;
    ValueExpressionAST *valExpr;

public:
    UnaryValueExpressionAST()
        : ValueExpressionAST()
        , op_token(0)
        , valExpr(nullptr)
    {}

    virtual UnaryValueExpressionAST *asUnaryValueExpression() { return this; }

    virtual UnaryValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT BinaryValueExpressionAST: public ValueExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned op_token;
    ValueExpressionAST *rvExpr;

public:
    BinaryValueExpressionAST()
        : ValueExpressionAST()
        , lvExpr(nullptr)
        , op_token(0)
        , rvExpr(nullptr)
    {}

    virtual BinaryValueExpressionAST *asBinaryValueExpression() { return this; }

    virtual BinaryValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SimpleValueExpressionAST: public ValueExpressionAST
{
public:
    SimpleValueExpressionAST()
        : ValueExpressionAST()
    {}

    virtual SimpleValueExpressionAST *asSimpleValueExpression();

    virtual SimpleValueExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT ConstantExpressionAST: public SimpleValueExpressionAST
{
public:
    ConstantExpressionAST()
        : SimpleValueExpressionAST()
    {}

    virtual ConstantExpressionAST *asConstantExpression();

    virtual ConstantExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT ColumnExpressionAST: public SimpleValueExpressionAST
{
public:
    ColumnNameAST *column;

public:
    ColumnExpressionAST()
        : SimpleValueExpressionAST()
        , column(nullptr)
    {}

    virtual ColumnExpressionAST *asColumnExpression() { return this; }

    virtual ColumnExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT LiteralExpressionAST: public ConstantExpressionAST
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

class SQL_EXPORT NumericExpressionAST: public ConstantExpressionAST
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

class SQL_EXPORT FunctionExpressionAST: public SimpleValueExpressionAST
{
public:
    FunctionNameAST *functionName;
    unsigned lparen_token;
    ValueExpressionListAST *arg_list;
    unsigned rparen_token;

public:
    FunctionExpressionAST()
        : SimpleValueExpressionAST()
        , functionName(nullptr)
        , lparen_token(0)
        , arg_list(nullptr)
        , rparen_token(0)
    {}

    virtual FunctionExpressionAST *asFunctionExpression() { return this; }

    virtual FunctionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CastFunctionAST: public SimpleValueExpressionAST
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
        , castExpr(nullptr)
        , as_token(0)
        , typeSpec(nullptr)
        , rparen_token(0)
    {}

    virtual CastFunctionAST *asCastFunction() { return this; }

    virtual CastFunctionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SubstringFunctionAST: public SimpleValueExpressionAST
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
        , strExpr(nullptr)
        , from_token(0)
        , startExpr(nullptr)
        , for_token(0)
        , lengthExpr(nullptr)
        , rparen_token(0)
    {}

    virtual SubstringFunctionAST *asSubstringFunction() { return this; }

    virtual SubstringFunctionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CountFunctionAST: public SimpleValueExpressionAST
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
        , countExpr(nullptr)
        , rparen_token(0)
    {}

    virtual CountFunctionAST *asCountFunction() { return this; }

    virtual CountFunctionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ParameterExpressionAST: public SimpleValueExpressionAST
{
public:
    ParamNameAST *binding;

public:
    ParameterExpressionAST()
        : SimpleValueExpressionAST()
        , binding(nullptr)
    {}

    virtual ParameterExpressionAST *asParameterExpression() { return this; }

    virtual ParameterExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT NullValueExpressionAST: public ConstantExpressionAST
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
        , query(nullptr)
        , rparen_token(0)
    {}

    virtual SelectCompoundValueExpressionAST *asSelectCompoundValueExpression() { return this; }

    virtual SelectCompoundValueExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CompoundRValueExpressionAST: public BaseExpressionAST
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

    virtual CompoundRValueExpressionAST *asCompoundRValueExpression();

    virtual CompoundRValueExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT ValueListExpressionAST: public CompoundRValueExpressionAST
{
public:
    ValueExpressionListAST *val_list;

public:
    ValueListExpressionAST()
        : CompoundRValueExpressionAST()
        , val_list(nullptr)
    {}

    virtual ValueListExpressionAST *asValueListExpression() { return this; }

    virtual ValueListExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SubQueryExpressionAST: public CompoundRValueExpressionAST
{
public:
    StatementAST *query;

public:
    SubQueryExpressionAST()
        : CompoundRValueExpressionAST()
        , query(nullptr)
    {}

    virtual SubQueryExpressionAST *asSubQueryExpression() { return this; }

    virtual SubQueryExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ConditionExpressionAST: public BaseExpressionAST
{
public:
    ConditionExpressionAST()
        : BaseExpressionAST()
    {}

    virtual ConditionExpressionAST *asConditionExpression();

    virtual ConditionExpressionAST *clone(Utils::MemoryPool *pool) const = 0;
};

class SQL_EXPORT CompoundConditionExpressionAST: public ConditionExpressionAST
{
public:
    unsigned lparen_token;
    ConditionExpressionAST *condExpr;
    unsigned rparen_token;

public:
    CompoundConditionExpressionAST()
        : ConditionExpressionAST()
        , lparen_token(0)
        , condExpr(nullptr)
        , rparen_token(0)
    {}

    virtual CompoundConditionExpressionAST *asCompoundConditionExpression() { return this; }

    virtual CompoundConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT InConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned not_token;
    unsigned in_token;
    CompoundRValueExpressionAST *rvExpr;

public:
    InConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(nullptr)
        , not_token(0)
        , in_token(0)
        , rvExpr(nullptr)
    {}

    virtual InConditionExpressionAST *asInConditionExpression() { return this; }

    virtual InConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT LikeConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned not_token;
    unsigned like_token;
    ValueExpressionAST *rvExpr;

public:
    LikeConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(nullptr)
        , not_token(0)
        , like_token(0)
        , rvExpr(nullptr)
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
        , lvExpr(nullptr)
        , not_token(0)
        , between_token(0)
        , rv1Expr(nullptr)
        , and_token(0)
        , rv2Expr(nullptr)
    {}

    virtual BetweenConditionExpressionAST *asBetweenConditionExpression() { return this; }

    virtual BetweenConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT ExistsConditionExpressionAST: public ConditionExpressionAST
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
        , rvExpr(nullptr)
    {}

    virtual ExistsConditionExpressionAST *asExistsConditionExpression() { return this; }

    virtual ExistsConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT UnaryConditionExpressionAST: public ConditionExpressionAST
{
public:
    ValueExpressionAST *lvExpr;
    unsigned last_op_token;

public:
    UnaryConditionExpressionAST()
        : ConditionExpressionAST()
        , lvExpr(nullptr)
        , last_op_token(0)
    {}

    virtual UnaryConditionExpressionAST *asUnaryConditionExpression() { return this; }

    virtual UnaryConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT BinaryConditionExpressionAST: public ConditionExpressionAST
{
public:
    ConditionExpressionAST *lvCondExpr;
    unsigned cond_op_token;
    ConditionExpressionAST *rvCondExpr;

public:
    BinaryConditionExpressionAST()
        : ConditionExpressionAST()
        , lvCondExpr(nullptr)
        , cond_op_token(0)
        , rvCondExpr(nullptr)
    {}

    virtual BinaryConditionExpressionAST *asBinaryConditionExpression() { return this; }

    virtual BinaryConditionExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT AssignmentExpressionAST: public BaseExpressionAST
{
public:
    ColumnNameAST *columnName;
    unsigned equal_token;
    ValueExpressionAST *rvExpr;

public:
    AssignmentExpressionAST()
        : BaseExpressionAST()
        , columnName(nullptr)
        , equal_token(0)
        , rvExpr(nullptr)
    {}

    virtual AssignmentExpressionAST *asAssignmentExpression() { return this; }

    virtual AssignmentExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CaseExpressionAST: public ValueExpressionAST
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
        , caseExpr(nullptr)
        , var_list(nullptr)
        , else_token(0)
        , elseExpr(nullptr)
        , end_token(0)
    {}

    virtual CaseExpressionAST *asCaseExpression() { return this; }

    virtual CaseExpressionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CaseExpressionClauseAST: public AST
{
public:
    unsigned when_token;
    ValueExpressionAST *whenExpr;
    unsigned then_token;
    ValueExpressionAST *thenExpr;

public:
    CaseExpressionClauseAST()
        : when_token(0)
        , whenExpr(nullptr)
        , then_token(0)
        , thenExpr(nullptr)
    {}

    virtual CaseExpressionClauseAST *asCaseExpressionClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CaseExpressionClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CaseConditionAST: public ValueExpressionAST
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
        , var_list(nullptr)
        , else_token(0)
        , elseExpr(nullptr)
        , end_token(0)
    {}

    virtual CaseConditionAST *asCaseCondition() { return this; }

    virtual CaseConditionAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT CaseConditionClauseAST: public AST
{
public:
    unsigned when_token;
    ConditionExpressionAST *whenExpr;
    unsigned then_token;
    ValueExpressionAST *thenExpr;

public:
    CaseConditionClauseAST()
        : when_token(0)
        , whenExpr(nullptr)
        , then_token(0)
        , thenExpr(nullptr)
    {}

    virtual CaseConditionClauseAST *asCaseConditionClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CaseConditionClauseAST *clone(Utils::MemoryPool *pool) const;

protected:
    virtual void accept0( ASTVisitor *visitor );
    virtual bool match0( AST *, ASTMatcher * );
};

class SQL_EXPORT SqlCheckNamesWalker: public BaseWalker
{
public:
    SqlCheckNamesWalker( AST *ast, TranslationUnit *translationUnit )
        : BaseWalker()
        , m_ast(ast)
        , m_translationUnit(translationUnit)
    {}

    virtual void walk( BaseWalkReflector *reflector );

private:
    AST *m_ast;
    TranslationUnit *m_translationUnit;
};

} // namespace Sql

#endif // CPLUSPLUS_AST_H
