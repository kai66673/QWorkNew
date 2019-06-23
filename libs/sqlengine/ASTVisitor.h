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

#ifndef SQL_ASTVISITOR_H
#define SQL_ASTVISITOR_H

// #include "CPlusPlusForwardDeclarations.h"
#include "ASTfwd.h"
#include "TranslationUnit.h"

namespace Sql {

class SQL_EXPORT ASTVisitor
{
    ASTVisitor(const ASTVisitor &other);
    void operator =(const ASTVisitor &other);

public:
    ASTVisitor(TranslationUnit *unit);
    virtual ~ASTVisitor();

    TranslationUnit *translationUnit() const;
    void setTranslationUnit(TranslationUnit *translationUnit);

//    Control *control() const;
    unsigned tokenCount() const;
    const Token &tokenAt(unsigned index) const;
    int tokenKind(unsigned index) const;
    const char *spell(unsigned index) const;
    const Identifier *identifier(unsigned index) const;
    const Literal *literal(unsigned index) const;
    const NumericLiteral *numericLiteral(unsigned index) const;
    const StringLiteral *stringLiteral(unsigned index) const;

    void getPosition(unsigned offset,
                     unsigned *line,
                     unsigned *column = 0,
                     const StringLiteral **fileName = 0) const;

    void getTokenPosition(unsigned index,
                          unsigned *line,
                          unsigned *column = 0,
                          const StringLiteral **fileName = 0) const;

    void getTokenStartPosition(unsigned index, unsigned *line, unsigned *column) const;
    void getTokenEndPosition(unsigned index, unsigned *line, unsigned *column) const;

    void accept(AST *ast);

    template <typename _Tp>
    void accept(List<_Tp> *it)
    {
        for (; it; it = it->next)
            accept(it->value);
    }

    virtual bool preVisit(AST *) { return true; }
    virtual void postVisit(AST *) {}

    virtual bool visit(TranslationUnitAST *) { return true; }
    virtual bool visit(StatementAST *) { return true; }
    virtual bool visit(InvalidStatementAST *) { return true; }
    virtual bool visit(UnknownStatementAST *) { return true; }
    virtual bool visit(SelectStatementAST *) { return true; }
    virtual bool visit(SelectCoreCompoundAST *) { return true; }
    virtual bool visit(SelectCompoundAST *) { return true; }
    virtual bool visit(SelectCompoundUnionAST *) { return true; }
    virtual bool visit(SelectCompoundIntersectAST *) { return true; }
    virtual bool visit(SelectCompoundExceptAST *) { return true; }
    virtual bool visit(SelectCompoundMinusAST *) { return true; }
    virtual bool visit(SelectCoreAST *) { return true; }
    virtual bool visit(OrderByBlockAST *) { return true; }
    virtual bool visit(LimitToBlockAST *) { return true; }
    virtual bool visit(OrderingTermAST *) { return true; }
    virtual bool visit(SchemaNameAST *) { return true; }
    virtual bool visit(TableNameAST *) { return true; }
    virtual bool visit(TableAliasAST *) { return true; }
    virtual bool visit(ColumnNameAST *) { return true; }
    virtual bool visit(ParamNameAST *) { return true; }
    virtual bool visit(LabelNameAST *) { return true; }
    virtual bool visit(ColumnAliasAST *) { return true; }
    virtual bool visit(FunctionNameAST *) { return true; }
    virtual bool visit(ViewNameAST *) { return true; }
    virtual bool visit(IndexNameAST *) { return true; }
    virtual bool visit(TriggerNameAST *) { return true; }
    virtual bool visit(ConstraintNameAST *) { return true; }
    virtual bool visit(SchemaTableNameAST *) { return true; }
    virtual bool visit(SchemaIndexNameAST *) { return true; }
    virtual bool visit(SchemaTriggerNameAST *) { return true; }
    virtual bool visit(StarResultColumnAST *) { return true; }
    virtual bool visit(ExprResultColumnAST *) { return true; }
    virtual bool visit(SubQueryAST *) { return true; }
    virtual bool visit(DirectSingleSourceAST *) { return true; }
    virtual bool visit(SubQuerySingleSourceAST *) { return true; }
    virtual bool visit(JoinOpAST *) { return true; }
    virtual bool visit(JoinConstraintAST *) { return true; }
    virtual bool visit(JoinItemAST *) { return true; }
    virtual bool visit(JoinSourceAST *) { return true; }
    virtual bool visit(GroupByBlockAST *) { return true; }
    virtual bool visit(DeleteStatementAST *) { return true; }
    virtual bool visit(InsertStatementAST *) { return true; }
    virtual bool visit(CreateTableDirectClauseAST *) { return true; }
    virtual bool visit(CreateTableAsSelectClauseAST *) { return true; }
    virtual bool visit(CreateTriggerRefClauseAST *) { return true; }
    virtual bool visit(CreateRdbTriggerBodyAST *) { return true; }
    virtual bool visit(CreateRdbTriggerBodyItemAST *) { return true; }
    virtual bool visit(ColumnDefinitionAST *) { return true; }
    virtual bool visit(UniqueOrPkTableConstraintAST *) { return true; }
    virtual bool visit(CheckTableConstraintAST *) { return true; }
    virtual bool visit(FkTableConstraintAST *) { return true; }
    virtual bool visit(FkReferencesClauseAST *) { return true; }
    virtual bool visit(PkColumnConstraintAST *) { return true; }
    virtual bool visit(NotNullColumnConstraintAST *) { return true; }
    virtual bool visit(UniqueColumnConstraintAST *) { return true; }
    virtual bool visit(CheckColumnConstraintAST *) { return true; }
    virtual bool visit(ReferencesColumnConstraintAST *) { return true; }
    virtual bool visit(SortedColumnNameAST *) { return true; }
    virtual bool visit(InsertClauseAsValuesAST *) { return true; }
    virtual bool visit(InsertClauseAsSelectAST *) { return true; }
    virtual bool visit(UpdateStatementAST *) { return true; }
    virtual bool visit(CharacterTypeAST *) { return true; }
    virtual bool visit(NumberTypeAST *) { return true; }
    virtual bool visit(SimpleTypeAST *) { return true; }
    virtual bool visit(CreateTableStatementAST *) { return true; }
    virtual bool visit(CreateViewStatementAST *) { return true; }
    virtual bool visit(CreateIndexStatementAST *) { return true; }
    virtual bool visit(CreateTriggerStatementAST *) { return true; }
    virtual bool visit(DropTableStatementAST *) { return true; }
    virtual bool visit(DropViewStatementAST *) { return true; }
    virtual bool visit(DropIndexStatementAST *) { return true; }
    virtual bool visit(DropTriggerStatementAST *) { return true; }
    virtual bool visit(TruncateStatementAST *) { return true; }
    virtual bool visit(AlterTableStatementAST *) { return true; }
    virtual bool visit(AlterTableDropClauseAST *) { return true; }
    virtual bool visit(AlterTableAddConstraintClauseAST *) { return true; }
    virtual bool visit(AlterTableAddColumnClauseAST *) { return true; }
    virtual bool visit(AlterTableModifyColumnClauseAST *) { return true; }
    virtual bool visit(CommentOnTableStatementAST *) { return true; }
    virtual bool visit(CommentOnColumnStatementAST *) { return true; }
    virtual bool visit(CommitStatementAST *) { return true; }
    virtual bool visit(RollbackStatementAST *) { return true; }
    virtual bool visit(CallStatementAST *) { return true; }

    // expressions
    virtual bool visit(CompoundValueExpressionAST *) { return true; }
    virtual bool visit(UnaryValueExpressionAST *) { return true; }
    virtual bool visit(BinaryValueExpressionAST *) { return true; }
    virtual bool visit(ColumnExpressionAST *) { return true; }
    virtual bool visit(LiteralExpressionAST *) { return true; }
    virtual bool visit(NumericExpressionAST *) { return true; }
    virtual bool visit(FunctionExpressionAST *) { return true; }
    virtual bool visit(CastFunctionAST *) { return true; }
    virtual bool visit(SubstringFunctionAST *) { return true; }
    virtual bool visit(CountFunctionAST *) { return true; }
    virtual bool visit(ParameterExpressionAST *) { return true; }
    virtual bool visit(NullValueExpressionAST *) { return true; }
    virtual bool visit(SelectCompoundValueExpressionAST *) { return true; }
    virtual bool visit(ValueListExpressionAST *) { return true; }
    virtual bool visit(SubQueryExpressionAST *) { return true; }
    virtual bool visit(CompoundConditionExpressionAST *) { return true; }
    virtual bool visit(InConditionExpressionAST *) { return true; }
    virtual bool visit(LikeConditionExpressionAST *) { return true; }
    virtual bool visit(BetweenConditionExpressionAST *) { return true; }
    virtual bool visit(ExistsConditionExpressionAST *) { return true; }
    virtual bool visit(UnaryConditionExpressionAST *) { return true; }
    virtual bool visit(BinaryConditionExpressionAST *) { return true; }
    virtual bool visit(AssignmentExpressionAST *) { return true; }
    virtual bool visit(CaseExpressionAST *) { return true; }
    virtual bool visit(CaseExpressionClauseAST *) { return true; }
    virtual bool visit(CaseConditionAST *) { return true; }
    virtual bool visit(CaseConditionClauseAST *) { return true; }

    virtual void endVisit(TranslationUnitAST *) { }
    virtual void endVisit(StatementAST *) { }
    virtual void endVisit(InvalidStatementAST *) { }
    virtual void endVisit(UnknownStatementAST *) { }
    virtual void endVisit(SelectStatementAST *) { }
    virtual void endVisit(SelectCoreCompoundAST *) { }
    virtual void endVisit(SelectCompoundAST *) { }
    virtual void endVisit(SelectCompoundUnionAST *) { }
    virtual void endVisit(SelectCompoundIntersectAST *) { }
    virtual void endVisit(SelectCompoundExceptAST *) { }
    virtual void endVisit(SelectCompoundMinusAST *) { }
    virtual void endVisit(SelectCoreAST *) { }
    virtual void endVisit(OrderByBlockAST *) { }
    virtual void endVisit(LimitToBlockAST *) { }
    virtual void endVisit(OrderingTermAST *) { }
    virtual void endVisit(SchemaNameAST *) { }
    virtual void endVisit(TableNameAST *) { }
    virtual void endVisit(TableAliasAST *) { }
    virtual void endVisit(ColumnNameAST *) { }
    virtual void endVisit(ParamNameAST *) { }
    virtual void endVisit(LabelNameAST *) { }
    virtual void endVisit(ColumnAliasAST *) { }
    virtual void endVisit(FunctionNameAST *) { }
    virtual void endVisit(ViewNameAST *) { }
    virtual void endVisit(IndexNameAST *) { }
    virtual void endVisit(TriggerNameAST *) { }
    virtual void endVisit(ConstraintNameAST *) { }
    virtual void endVisit(SchemaTableNameAST *) { }
    virtual void endVisit(SchemaIndexNameAST *) { }
    virtual void endVisit(SchemaTriggerNameAST *) { }
    virtual void endVisit(StarResultColumnAST *) { }
    virtual void endVisit(ExprResultColumnAST *) { }
    virtual void endVisit(SubQueryAST *) { }
    virtual void endVisit(DirectSingleSourceAST *) { }
    virtual void endVisit(SubQuerySingleSourceAST *) { }
    virtual void endVisit(JoinOpAST *) { }
    virtual void endVisit(JoinConstraintAST *) { }
    virtual void endVisit(JoinItemAST *) { }
    virtual void endVisit(JoinSourceAST *) { }
    virtual void endVisit(GroupByBlockAST *) { }
    virtual void endVisit(DeleteStatementAST *) { }
    virtual void endVisit(InsertStatementAST *) { }
    virtual void endVisit(CreateTableDirectClauseAST *) { }
    virtual void endVisit(CreateTableAsSelectClauseAST *) { }
    virtual void endVisit(CreateTriggerRefClauseAST *) { }
    virtual void endVisit(CreateRdbTriggerBodyAST *) { }
    virtual void endVisit(CreateRdbTriggerBodyItemAST *) { }
    virtual void endVisit(ColumnDefinitionAST *) { }
    virtual void endVisit(UniqueOrPkTableConstraintAST *) { }
    virtual void endVisit(CheckTableConstraintAST *) { }
    virtual void endVisit(FkTableConstraintAST *) { }
    virtual void endVisit(FkReferencesClauseAST *) { }
    virtual void endVisit(PkColumnConstraintAST *) { }
    virtual void endVisit(NotNullColumnConstraintAST *) { }
    virtual void endVisit(UniqueColumnConstraintAST *) { }
    virtual void endVisit(CheckColumnConstraintAST *) { }
    virtual void endVisit(ReferencesColumnConstraintAST *) { }
    virtual void endVisit(SortedColumnNameAST *) { }
    virtual void endVisit(InsertClauseAsValuesAST *) { }
    virtual void endVisit(InsertClauseAsSelectAST *) { }
    virtual void endVisit(UpdateStatementAST *) { }
    virtual void endVisit(CharacterTypeAST *) { }
    virtual void endVisit(NumberTypeAST *) { }
    virtual void endVisit(SimpleTypeAST *) { }
    virtual void endVisit(CreateTableStatementAST *) { }
    virtual void endVisit(CreateViewStatementAST *) { }
    virtual void endVisit(CreateIndexStatementAST *) { }
    virtual void endVisit(CreateTriggerStatementAST *) { }
    virtual void endVisit(DropTableStatementAST *) { }
    virtual void endVisit(DropViewStatementAST *) { }
    virtual void endVisit(DropIndexStatementAST *) { }
    virtual void endVisit(DropTriggerStatementAST *) { }
    virtual void endVisit(TruncateStatementAST *) { }
    virtual void endVisit(AlterTableStatementAST *) { }
    virtual void endVisit(AlterTableDropClauseAST *) { }
    virtual void endVisit(AlterTableAddConstraintClauseAST *) { }
    virtual void endVisit(AlterTableAddColumnClauseAST *) { }
    virtual void endVisit(AlterTableModifyColumnClauseAST *) { }
    virtual void endVisit(CommentOnTableStatementAST *) { }
    virtual void endVisit(CommentOnColumnStatementAST *) { }
    virtual void endVisit(CommitStatementAST *) { }
    virtual void endVisit(RollbackStatementAST *) { }
    virtual void endVisit(CallStatementAST *) { }

    // Expressions
    virtual void endVisit(CompoundValueExpressionAST *) { }
    virtual void endVisit(UnaryValueExpressionAST *) { }
    virtual void endVisit(BinaryValueExpressionAST *) { }
    virtual void endVisit(ColumnExpressionAST *) { }
    virtual void endVisit(LiteralExpressionAST *) { }
    virtual void endVisit(NumericExpressionAST *) { }
    virtual void endVisit(FunctionExpressionAST *) { }
    virtual void endVisit(CastFunctionAST *) { }
    virtual void endVisit(SubstringFunctionAST *) { }
    virtual void endVisit(CountFunctionAST *) { }
    virtual void endVisit(ParameterExpressionAST *) { }
    virtual void endVisit(NullValueExpressionAST *) { }
    virtual void endVisit(SelectCompoundValueExpressionAST *) { }
    virtual void endVisit(ValueListExpressionAST *) { }
    virtual void endVisit(SubQueryExpressionAST *) { }
    virtual void endVisit(CompoundConditionExpressionAST *) { }
    virtual void endVisit(InConditionExpressionAST *) { }
    virtual void endVisit(LikeConditionExpressionAST *) { }
    virtual void endVisit(BetweenConditionExpressionAST *) { }
    virtual void endVisit(ExistsConditionExpressionAST *) { }
    virtual void endVisit(UnaryConditionExpressionAST *) { }
    virtual void endVisit(BinaryConditionExpressionAST *) { }
    virtual void endVisit(AssignmentExpressionAST *) { }
    virtual void endVisit(CaseExpressionAST *) { }
    virtual void endVisit(CaseExpressionClauseAST *) { }
    virtual void endVisit(CaseConditionAST *) { }
    virtual void endVisit(CaseConditionClauseAST *) { }

private:
    TranslationUnit *_translationUnit;
};

} // namespace Sql


#endif // SQL_ASTVISITOR_H
