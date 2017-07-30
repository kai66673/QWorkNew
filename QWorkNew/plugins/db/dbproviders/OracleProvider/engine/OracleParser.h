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

#ifndef ORACLE_PARSER_H
#define ORACLE_PARSER_H

#include "ISemanticParser.h"
#include "OracleASTfwd.h"
#include "SqlToken.h"
#include "sql_constants.h"
#include "TranslationUnit.h"
#include "MemoryPool.h"

#include <map>
#include <QList>

class ITextDocument;

namespace Oracle {

class Parser: public Sql::ISemanticParser
{
public:
    Parser(Sql::TranslationUnit *translationUnit);
    ~Parser();

    virtual Sql::BaseWalker *parseDocument( Sql::ParseTarget target );
    virtual void fillSqlStatements( QList<Sql::SqlStatement *> *statements,
                                    Database::ExecuteType executeType, ITextDocument *textDocument);
    virtual void fillStatementFromText( QList<Sql::SqlStatement *> *statements, const QString &statementText );
    virtual int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, unsigned position,
                                 int startPosition, unsigned completionOperator );


    bool parseTranslationUnit( TranslationUnitAST *&node );

    virtual bool parseStatement( StatementAST *&node );
    virtual bool skipUntilStatement( StatementAST *node );

    bool parseDmlStatement( StatementAST *&node );
    bool parseSelectStatement( StatementAST *&node, bool topLevel = false );
    bool parseSelectCore( SelectCoreAST *&node );
    bool parseOrderByBlock( OrderByBlockAST *&node );
        bool parseOrderTermList( OrderingTermListAST *&node );
    bool parseLimitToBlock( LimitToBlockAST *&node );
    bool parseGroupByBlock(GroupByBlockAST *&node);
    bool parseSubQuery( SubQueryAST *&node);
    bool parseResultColumnList( ResultColumnListAST *&node );
    bool parseJoinSource( JoinSourceAST *&node );
    bool parseJoinItemList( JoinItemListAST *&node );

    bool checkIsJoinOp( JoinOpAST *&node, bool &isComma );

    bool parseDeleteStatement( DeleteStatementAST *&node );
    bool parseDeleteStatement( StatementAST *&node );
    bool parseInsertStatement( InsertStatementAST *&node );
    bool parseInsertStatement( StatementAST *&node );
    bool parseUpdateStatement( UpdateStatementAST *&node );
    bool parseUpdateStatement( StatementAST *&node );

    bool parseColumnList( ColumnNameListAST *&node );
    bool parseAssignmentExpressionList( AssignmentExpressionListAST *&node );

    bool parseDropStatement( StatementAST *&node );
    bool parseTruncateStatement( StatementAST *&node );
    bool parseCommentStatement( StatementAST *&node );

    bool parseCreateStatement( StatementAST *&node );
    bool parseColumnDefinitionList( ColumnDefinitionListAST *&node );
    bool parseColumnConstraintList( ColumnConstraintListAST *&node );
    bool parseTableConstraintList( TableConstraintListAST *&node );
    bool parseSortedColumnNameList( SortedColumnNameListAST *&node );

    bool parseRdbTriggerBodyList( CreateRdbTriggerBodyListAST *&node );
    bool parseTriggerStatementList( StatementListAST *&node );

    bool parseAlterStatement( StatementAST *&node );

    bool parseCommitStatement( StatementAST *&node );
    bool parseRollbackStatement( StatementAST *&node );
    bool parseCallStatement( CallStatementAST *&node );
    bool parseCallStatement( StatementAST *&node );

    // Expressions
    bool parseValueExpresion( ValueExpressionAST *&node, bool conditionMode = false );
    bool parseValueExpresionInternal( ValueExpressionAST *&node, QList<ValueExpressionAST *> &items, bool conditionMode = false );
    bool parseValueExpresionList( ValueExpressionListAST *&node, bool eatRParen = false );

    bool parseCompoundValueExpresion( CompoundValueExpressionAST *&node );
    bool parseUnaryValueExpression( UnaryValueExpressionAST *&node );
    bool parseParameterExpression( ParameterExpressionAST *&node );
    bool parseFunctionExpresion( FunctionExpressionAST *&node );
    bool parseCastExpression( CastFunctionAST *&node );
    bool parseSubstringExpression( SubstringFunctionAST *&node );
    bool parseCountExpression( CountFunctionAST *&node );
    bool parseTypeSpecifier( BaseTypeAST *&node );
    bool parseColumnExpression( ColumnExpressionAST *&node );

    bool parseCaseExpression( CaseExpressionAST *&node );
    bool parseCaseExpressionClauseList( CaseExpressionClauseListAST *&node );
    bool parseCaseCondition( CaseConditionAST *&node );
    bool parseCaseConditionClauseList( CaseConditionClauseListAST *&node );

    bool parseConditionExpression( ConditionExpressionAST *&node );
    bool parseConditionExpressionInternal( ConditionExpressionAST *&node, QList<ConditionExpressionAST *> &items );

    bool parseCompoundConditionExpresion( CompoundConditionExpressionAST *&node );
    bool parseExistsConditionExpression( ExistsConditionExpressionAST *&node );
    bool parseInConditionExpression( InConditionExpressionAST *&node, ValueExpressionAST *lvExpr );
    bool parseCommonConditionExpression( InConditionExpressionAST *&node, ValueExpressionAST *lvExpr );
    bool parseBetweenConditionExpression( BetweenConditionExpressionAST *&node, ValueExpressionAST *lvExpr );
    bool parseLikeConditionExpression( LikeConditionExpressionAST *&node, ValueExpressionAST *lvExpr );

    ///
    bool parseCompoundExpresion( BaseExpressionAST *&node, bool enableCondition );
    bool parseSimpleExpression( BaseExpressionAST *&node, bool enableCondition = false, bool needCondition = false );

    // Pl-Sql
    bool parseDataType( DataTypeAST *&node );
    bool parseRowType( RowTypeAST *&node );
    bool parsePlSqlBlockStatement( PlSqlBlockAST *&node );
    bool parsePlSqlBlockStatement( StatementAST *&node );
    bool parseDeclarationsList( DeclarationListAST *&node );
    bool parseCursorDeclaration( CursorDeclarationAST *&node );
    bool parseCursorSpecification( CursorSpecAST *&node );
    bool parseCursorParameterDeclList( CursorParameterDeclarationListAST *&node );
    bool parseRoutineArgumentList( RoutineArgumentDeclarationListAST *&node );
    bool parseTableList( TableNameListAST *&node );
    bool parseTypeDefintion( TypeDefinitionAST *&node );
    bool parseRecordFieldDeclarationList( RecordFieldDeclarationListAST *&node );
    bool parseFunctionHeading( FunctionHeadingAST *&node );
    bool parseFunctionSpec( FunctionSpecAST *&node );
    bool parseProcedureHeading( ProcedureHeadingAST *&node );

    bool parsePlSqlBlockBody( PlSqlBlockBodyAST *&node );
    bool parsePlSqlStatementsList( PlSqlStatementListAST *&node, bool exceptionMode = false );
    bool parseBlockSelectStatement( BlockSelectIntoStatementAST *&node );
    bool parseArrayIndexList( ArrayIndexExpressionListAST *&node );
    bool parseLValueExpressionItems ( LValueExpressionItemListAST *&node );
    bool parseLValueExpressionList( LValueExpressionListAST *&node );
    bool parseExecImmediateStatement( ExecImmediateStatementAST *&node );
    bool parseBaseIntoClause( BaseIntoClauseAST *&node );
    bool parseUsingBindList( UsingBindListAST *&node );
    bool parsePlSqlBlockEception( PlSqlBlockBodyAST *ast );
    bool parseExceptionNameList( PlSqlNameListAST *&node );

public:
    bool skipUntil(int token);
//    bool skipUntilStatement();
    bool skip(int l, int r);

//    const Identifier *identifier(NameAST *name) const;

    bool match(int kind, unsigned *token);
    bool match1( int kind, unsigned *token );

    bool blockErrors(bool block);
    void warning(unsigned index, const char *format, ...);
    void error(unsigned index, const char *format, ...);
    void fatal(unsigned index, const char *format, ...);

    void errorCursor( const char *message );
    void warningCursor( const char *message );

    inline const Sql::Token &tok(int i = 1) const
    { return _translationUnit->tokenAt(_tokenIndex + i - 1); }

    inline int LA(int n = 1) const
    { return _translationUnit->tokenKind(_tokenIndex + n - 1); }

    inline int LA1(int n = 1) const
    { return _translationUnit->tokenKeyword(_tokenIndex + n - 1); }

    inline int LA2(int n = 1) const
    { return _translationUnit->tokenBuiltinFunction(_tokenIndex + n - 1); }

    inline int LA3(int n = 1) const
    { return _translationUnit->tokenTypeSpecifier(_tokenIndex + n - 1); }

    inline int consumeToken()
    { return _tokenIndex++; }

    inline unsigned cursor() const
    { return _tokenIndex; }

    void rewind(unsigned cursor);

    bool isPatentialIdentifier( int n = 1 ) const;
    bool isKeywordOnly( int n = 1 ) const;

private:
    Sql::TranslationUnit *_translationUnit;
    Utils::MemoryPool *_pool;
    unsigned _tokenIndex;
    int _expressionDepth;

private:
    Parser(const Parser& source);
    void operator =(const Parser& source);
};

} // namespace Oracle


#endif // ORACLE_PARSER_H
