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

#include <QDebug>

#include "RdbParser.h"
#include "SqlToken.h"
#include "SqlLexer.h"
#include "RdbAST.h"
#include "RdbASTVisitor.h"
#include "RdbKeywords.h"
#include "sqlchecknamesvisitor.h"
#include "RdbSqlStatementsBindVisitor.h"
#include "RdbSqlCompletionAssistVisitor.h"
#include "RdbSqlLinkFinder.h"

#include <string>
#include <cstdio> // for putchar

#ifdef _MSC_VER
#    define va_copy(dst, src) ((dst) = (src))
#elif defined(__INTEL_COMPILER) && !defined(va_copy)
#    define va_copy __va_copy
#endif

#define P_RET_TRUE { node = ast; \
                     return true; }

#define P_RET_TRUE_W(w) { warning(_tokenIndex, (w)); \
                          node = ast; \
                          return true; }

#define P_RET_TRUE_E(e) { error(_tokenIndex, (e)); \
                          node = ast; \
                          return true; }

#define P_RET_FALSE(e)  { error(_tokenIndex, (e)); \
                          return false; }
using namespace RDB;

Parser::Parser( Sql::TranslationUnit *unit )
    : Sql::ISemanticParser()
    , _translationUnit(unit)
    , _pool(_translationUnit->memoryPool())
    , _tokenIndex(1)
    , _expressionDepth(0)
    , _blockMode(false)
{ }

Parser::~Parser()
{ }

Sql::BaseWalker *Parser::parseDocument( Sql::ParseTarget target )
{
    switch ( target ) {
        case Sql::ParseSqlNames:
            TranslationUnitAST *node = 0;
            parseTranslationUnit(node);
            Sql::BaseWalker *walker = new SqlCheckNamesWalker(node, _translationUnit);
            return walker;
    }

    return 0;
}

void Parser::fillSqlStatements( QList<Sql::SqlStatement *> *statements,
                                Database::ExecuteType executeType, ITextDocument *textDocument )
{
    TranslationUnitAST *node = 0;
    parseTranslationUnit(node);
    SqlStatementsBindVisitor visitor(_translationUnit);
    visitor.fillStatementsList(statements, node, executeType, textDocument);
}

void Parser::fillStatementFromText( QList<Sql::SqlStatement *> *statements, const QString &statementText )
{
    TranslationUnitAST *node = 0;
    parseTranslationUnit(node);
    SqlStatementsBindVisitor visitor(_translationUnit);
    visitor.fillStatementFromText(statements, node, statementText);
}

int Parser::fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, unsigned position,
                             int startPosition, unsigned completionOperator )
{
    TranslationUnitAST *node = 0;
    parseTranslationUnit(node);
    SqlCompletionAssistVisitor visitor(_translationUnit);
    return visitor.fillCompletions(completions, node, position, startPosition, completionOperator);
    return -1;
}

Core::ILink *Parser::findLinkAt( const QTextCursor &tc )
{
    TranslationUnitAST *node = 0;
    parseTranslationUnit(node);
    SqlLinkFinder finder(_translationUnit);
    return finder.findLinkAt(node, tc.position());
}

bool Parser::blockErrors(bool block)
{ return _translationUnit->blockErrors(block); }

bool Parser::skipUntil(int token)
{
    while (int tk = LA()) {
        if (tk == token)
            return true;

        consumeToken();
    }

    return false;
}

bool Parser::skip(int l, int r)
{
    int count = 0;

    while (int tk = LA()) {
        if (tk == l)
            ++count;
        else if (tk == r)
            --count;
        else if (l != Sql::T_LBRACE && (tk == Sql::T_LBRACE ||
                                        tk == Sql::T_RBRACE ||
                                        tk == Sql::T_SEMICOLON))
            return false;

        if (count == 0)
            return true;

        consumeToken();
    }

    return false;
}

bool Parser::match(int kind, unsigned *token)
{
    if (LA() == kind) {
        *token = consumeToken();
        return true;
    }
    *token = 0;
    error(_tokenIndex, "expected token `%s' got `%s'", Sql::Token::name(kind), tok().spell());
    return false;
}

bool Parser::match1(int kind, unsigned *token)
{
    if ( LA1() == kind ) {
        *token = consumeToken();
        return true;
    }
    *token = 0;
    error(_tokenIndex, "expected token `%s' got `%s'", Sql::Token::name1(kind), tok().spell());
    return false;
}

bool Parser::parseTranslationUnit( TranslationUnitAST *&node )
{
    TranslationUnitAST *ast = new (_pool) TranslationUnitAST;
    StatementListAST **statement_list = &ast->statement_list;

    while ( LA() ) {
        // unsigned start_declaration = cursor();
        StatementAST *statement = 0;
        unsigned line;
        unsigned column;
        _translationUnit->getTokenStartPosition(_tokenIndex, &line, &column);
        parseStatement(statement);
        if ( statement ) {
            *statement_list = new (_pool) StatementListAST;
            (*statement_list)->value = statement;
            statement->start_line = line;
            statement->start_pos = column;
            statement_list = &(*statement_list)->next;
        }
        skipUntilStatement(statement);
    }

    node = ast;
    return true;
}

bool Parser::parseStatement( StatementAST *&node )
{
    bool result;
    unsigned startStatementTokenIndex = _tokenIndex;
    int la1 = LA1();
    _blockMode = false;

    switch ( la1 ) {
        case T_6_SELECT:
            result = parseSelectStatement(node, true);
            break;
        case T_6_DELETE:
            result = parseDeleteStatement(node);
            break;
        case T_6_INSERT:
            result = parseInsertStatement(node);
            break;
        case T_6_UPDATE:
            result = parseUpdateStatement(node);
            break;
        case T_8_TRUNCATE:
            result = parseTruncateStatement(node);
            break;
        case T_4_DROP:
            result = parseDropStatement(node);
            break;
        case T_7_COMMENT:
            result = parseCommentStatement(node);
            break;
        case T_6_CREATE:
            result = parseCreateStatement(node);
            break;
        case T_5_ALTER:
            result = parseAlterStatement(node);
            break;
        case T_6_COMMIT:
            result = parseCommitStatement(node);
            break;
        case T_8_ROLLBACK:
            result = parseRollbackStatement(node);
            break;
        case T_4_CALL:
            result = parseCallStatement(node);
            break;
        case T_5_BEGIN:
            _blockMode = true;
            result = parseBlockStatement(node);
            break;
        default:
            result = false;
            error(_tokenIndex, "Unknown SQL-Statement");
            break;
    }

    if ( !node )
        node = new (_pool) InvalidStatementAST(startStatementTokenIndex, la1 == T_6_SELECT);

    return result;
}

bool Parser::skipUntilStatement( StatementAST *node )
{
    if ( node ) {
        unsigned line;
        unsigned pos;
        node->skip(this);
        _translationUnit->getTokenEndPosition(_tokenIndex - 1, &line, &pos);
        node->end_line = line;
        if ( !line )
            pos++;
        node->end_pos = pos;
        return true;
    }

    if ( skipUntil(Sql::T_SEMICOLON) )
        consumeToken();

    return true;
}

bool Parser::parseDmlStatement( StatementAST *&node )
{
    bool result;

    switch ( LA1() ) {
        case T_6_SELECT:
            result = parseSelectStatement(node, true);
            break;
        case T_6_DELETE:
            result = parseDeleteStatement(node);
            break;
        case T_6_INSERT:
            result = parseInsertStatement(node);
            break;
        case T_6_UPDATE:
            result = parseUpdateStatement(node);
            break;
        default:
            result = false;
            error(_tokenIndex, "Unknown SQL-Statement");
            break;
    }

    if ( skipUntil(Sql::T_SEMICOLON) )
        consumeToken();

    return result;
}

bool Parser::parseSelectStatement( StatementAST *&node , bool topLevel )
{
    SelectStatementAST *ast = new (_pool) SelectStatementAST();
    SelectCoreListAST  **sel_core_list = &ast->select_core_list;
    SelectCompoundListAST **sel_comp_list = &ast->select_compound_list;

    int counter = 0;
    while ( LA() && counter < 20 ) {
        SelectCoreAST *sel_core = 0;
        if ( parseSelectCore(sel_core) ) {
            *sel_core_list = new (_pool) SelectCoreListAST;
            (*sel_core_list)->value = sel_core;
            sel_core_list = &(*sel_core_list)->next;
            SelectCompoundAST *sel_comp = 0;
            switch ( LA1() ) {
                case T_5_UNION: {
                    SelectCompoundUnionAST *comp = new (_pool) SelectCompoundUnionAST;
                    comp->union_token = consumeToken();
                    if ( LA1() == T_3_ALL || LA1() == T_8_DISTINCT )
                        comp->union_spec_token = consumeToken();
                    sel_comp = comp;
                    break;
                }
                case T_6_EXCEPT: {
                    SelectCompoundExceptAST *comp = new (_pool) SelectCompoundExceptAST;
                    comp->except_token = consumeToken();
                    sel_comp = comp;
                    break;
                }
                case T_9_INTERSECT: {
                    SelectCompoundIntersectAST *comp = new (_pool) SelectCompoundIntersectAST;
                    comp->intersect_token = consumeToken();
                    sel_comp = comp;
                    break;
                }
            }
            if ( sel_comp ) {
                *sel_comp_list = new (_pool) SelectCompoundListAST;
                (*sel_comp_list)->value = sel_comp;
                sel_comp_list = &(*sel_comp_list)->next;
            }
            else break;
        }
        else {
            return false;
        }
    }

    if ( LA1() == T_5_ORDER )
        parseOrderByBlock(ast->order_by_block);

    if ( LA1() == T_5_LIMIT )
        parseLimitToBlock(ast->limit_to_block);

    ast->last_token = cursor();

    node = ast;
    node->setTopLevelStatement(topLevel);
    return true;
}

bool Parser::parseSelectCore( SelectCoreAST *&node )
{
    if ( LA1() != T_6_SELECT )
        return false;

    SelectCoreAST *ast = new (_pool) SelectCoreAST;
    ast->select_token = consumeToken();
    if ( LA1() == T_3_ALL || LA1() == T_8_DISTINCT )
        ast->select_spec_token = consumeToken();
    if ( !parseResultColumnList(ast->columnList) )
        return false;
    if ( !match1(T_4_FROM, &ast->from_token) )
        return false;
    parseJoinSource(ast->joinSource);
    if ( LA1() == T_5_WHERE ) {
        ast->where_token = consumeToken();
        parseConditionExpression(ast->whereExpr);
    }
    if ( LA1() == T_5_GROUP ) {
        parseGroupByBlock(ast->groupBy);
    }
    node = ast;
    return true;
}

bool Parser::parseOrderByBlock( OrderByBlockAST *&node )
{
    OrderByBlockAST *ast = new (_pool) OrderByBlockAST;
    ast->order_token = consumeToken();
    match1(T_2_BY, &ast->by_token);
    parseOrderTermList(ast->term_list);
    node = ast;
    return true;
}

bool Parser::parseOrderTermList( OrderingTermListAST *&node )
{
    OrderingTermListAST **term_list_ptr = &node;

    while (1) {
        unsigned old_cursor = cursor();

        if ( LA() == Sql::T_RPAREN ) {
            error(_tokenIndex, "Unexpected token `)'");
            consumeToken();
            continue;
        }
        if ( LA1() || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
            return false;
        if ( LA() == Sql::T_COMMA ) {
            warning(_tokenIndex, "Order term expected");
            consumeToken();
            continue;
        }

        ValueExpressionAST *orderTermExpr;
        if ( !parseValueExpresion(orderTermExpr) )
            return false;
        OrderingTermAST *term = new (_pool) OrderingTermAST;
        term->expr = orderTermExpr;
        if ( !orderTermExpr )
            warning(_tokenIndex, "Order term expected");
        if ( LA1() == T_3_ASC || LA1() == T_4_DESC )
            term->asc_or_desc_token = consumeToken();
        *term_list_ptr = new (_pool) OrderingTermListAST;
        (*term_list_ptr)->value = term;
        term_list_ptr = &(*term_list_ptr)->next;

        if ( LA() == Sql::T_COMMA ) {
            consumeToken();
            continue;
        }

        if ( old_cursor == cursor() ) {
            error(_tokenIndex, "WTF! (parseOrderTermList)");
            return false;
        }
    }

    return true;
}

bool Parser::parseLimitToBlock( LimitToBlockAST *&node )
{
    LimitToBlockAST *ast = new (_pool) LimitToBlockAST;
    ast->limit_token = consumeToken();
    match1(T_2_TO, &ast->to_token);
    parseValueExpresion(ast->expr);
    node = ast;
    return true;
}

bool Parser::parseGroupByBlock( GroupByBlockAST *&node )
{
    GroupByBlockAST *ast = new (_pool) GroupByBlockAST;
    ast->group_token = consumeToken();
    match1(T_2_BY, &ast->by_token);
    parseValueExpresionList(ast->group_items_list);
//    parseOrderTermList(ast->group_items_list);
    if ( LA1() == T_6_HAVING ) {
        ast->having_token = consumeToken();
        parseConditionExpression(ast->havingExpr);
    }
    node = ast;
    return true;
}

bool Parser::parseSubQuery( SubQueryAST *&node )
{
    SubQueryAST *ast = new (_pool) SubQueryAST;
    ast->lparen_token = consumeToken();
    parseSelectStatement(ast->selectStatement);
    match(Sql::T_RPAREN, &ast->rparen_token);
    node = ast;
    return (ast->rparen_token != 0);
}

bool Parser::parseResultColumnList( ResultColumnListAST *&node )
{
    ResultColumnListAST **res_col_list = &node;

    unsigned first_token = cursor();

    while (1) {
        unsigned old_cursor = cursor();

        if ( (isKeywordOnly() && LA1() != T_4_NULL) || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL ) {
            if ( first_token == cursor() )
                error(_tokenIndex, "Expected select column expression");
            return true;
        }

        if ( LA() ==Sql:: T_COMMA ) {
            warning(_tokenIndex, "Expected select column expression");
            consumeToken();
            continue;
        }

        if ( LA() == Sql::T_STAR ) {
            StarResultColumnAST *star = new (_pool) StarResultColumnAST;
            star->star_token = consumeToken();
            *res_col_list = new (_pool) ResultColumnListAST;
            (*res_col_list)->value = star;
            res_col_list = &(*res_col_list)->next;
        }
        else if ( isPatentialIdentifier() && LA(2) == Sql::T_DOT && LA(3) == Sql::T_STAR ) {
            StarResultColumnAST *tableStar = new (_pool) StarResultColumnAST;
            tableStar->schemaTableName = new (_pool) SchemaTableNameAST;
            tableStar->schemaTableName->tableName = new (_pool) TableNameAST;
            const Sql::Token &tk = tok();
            tableStar->schemaTableName->tableName->name = tk.identifier;
            tableStar->schemaTableName->tableName->name_token = consumeToken();
            tableStar->dot_token = consumeToken();
            tableStar->star_token = consumeToken();
            *res_col_list = new (_pool) ResultColumnListAST;
            (*res_col_list)->value = tableStar;
            res_col_list = &(*res_col_list)->next;
        }
        else if ( isPatentialIdentifier() && LA(2) == Sql::T_DOT && isPatentialIdentifier(3) &&
                  LA(4) == Sql::T_DOT && LA(5) == Sql::T_STAR ) {
            StarResultColumnAST *tableStar = new (_pool) StarResultColumnAST;
            tableStar->schemaTableName = new (_pool) SchemaTableNameAST;
            tableStar->schemaTableName->parse(this, _pool);
            tableStar->dot_token = consumeToken();
            tableStar->star_token = consumeToken();
            *res_col_list = new (_pool) ResultColumnListAST;
            (*res_col_list)->value = tableStar;
            res_col_list = &(*res_col_list)->next;
        }

        else {
            ValueExpressionAST *expr;
            if ( !parseValueExpresion(expr) )
                return false;
            ExprResultColumnAST *exprRes = new (_pool) ExprResultColumnAST;
            exprRes->expr = expr;
            if ( !expr )
                warning(_tokenIndex, "Expected select column expression");
            bool needAlias = false;
            if ( LA1() == T_2_AS ) {
                exprRes->as_token = consumeToken();
                needAlias = true;
            }
            if ( isPatentialIdentifier() ) {
                const Sql::Token &tk = tok();
                exprRes->columnAlias = new (_pool) ColumnAliasAST;
                exprRes->columnAlias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                exprRes->columnAlias->name_token = consumeToken();
            }
            else if ( needAlias )
                warning(_tokenIndex, "Expected column alias");
            *res_col_list = new (_pool) ResultColumnListAST;
            (*res_col_list)->value = exprRes;
            res_col_list = &(*res_col_list)->next;
        }

        if ( LA() == Sql::T_COMMA ) {
            consumeToken();
            first_token = cursor();
        }
        else return true;

        if ( old_cursor == cursor() ) {
            error(_tokenIndex, "WTF! (parseResultColumnList)");
            return false;
        }
    }

    return true;
}

bool Parser::parseJoinSource(JoinSourceAST *&node)
{
    bool result = true;
    JoinSourceAST *ast = new (_pool) JoinSourceAST;

    if ( isPatentialIdentifier() ) {
        DirectSingleSourceAST *source = new (_pool) DirectSingleSourceAST;
        source->schemaTableName = new (_pool) SchemaTableNameAST;
        source->schemaTableName->parse(this, _pool);
        bool needAlias = false;
        if ( LA1() == T_2_AS ) {
            source->as_token = consumeToken();
            needAlias = true;
        }
        if ( isPatentialIdentifier() ) {
            const Sql::Token &tk1 = tok();
            source->tableAlias = new (_pool) TableAliasAST;
            source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
            source->tableAlias->name_token = consumeToken();
            source->tableAlias->schemaTableName = source->schemaTableName /*->clone(_pool)*/;
        }
        else if ( needAlias )
            error(_tokenIndex, "Expected table alias");
        ast->firstSource = source;
    }
    else if ( LA() == Sql::T_LPAREN && LA1(2) == T_6_SELECT ) {
        SubQuerySingleSourceAST *source = new (_pool) SubQuerySingleSourceAST;
        parseSubQuery(source->subQuery);
        bool needAlias = false;
        if ( LA1() == T_2_AS ) {
            source->as_token = consumeToken();
            needAlias = true;
        }
        if ( isPatentialIdentifier() ) {
            const Sql::Token &tk1 = tok();
            source->tableAlias = new (_pool) TableAliasAST;
            source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
            source->tableAlias->name_token = consumeToken();
            source->tableAlias->subqueryAliasType = source->subQuery /*->clone(_pool)*/;
        }
        else if ( needAlias )
            error(_tokenIndex, "Expected subquery alias");
        ast->firstSource = source;
    }
    else {
        error(_tokenIndex, "Expected table source");
        result = false;
    }

    if ( result ) {
        result = parseJoinItemList(ast->source_list);
    }

    node = ast;
    return result;
}

bool Parser::checkIsJoinOp( JoinOpAST *&node, bool &isComma )
{
    isComma = false;

    if ( LA() == Sql::T_COMMA || LA1() == T_4_JOIN ) {
        if ( LA() == Sql::T_COMMA )
            isComma = true;
        JoinOpAST *ast = new (_pool) JoinOpAST;
        ast->first_token = ast->last_token = consumeToken();
        node = ast;
        return true;
    }

    if ( ((LA1() == T_5_INNER || LA1() == T_5_OUTER || LA1() == T_4_FULL) && LA1(2) == T_4_JOIN) ||
         ((LA1() == T_4_LEFT || LA1() == T_5_RIGHT || LA1() == T_5_CROSS) && LA1(2) == T_4_JOIN) ) {
        JoinOpAST *ast = new (_pool) JoinOpAST;
        ast->first_token = consumeToken();
        ast->last_token = consumeToken();
        node = ast;
        return true;
    }
    if ( (LA1() == T_5_INNER || LA1() == T_5_OUTER || LA1() == T_4_FULL) && (LA1(2) == T_4_LEFT || LA1(2) == T_5_RIGHT || LA1(2) == T_5_CROSS) && LA1(3) == T_4_JOIN ) {
        JoinOpAST *ast = new (_pool) JoinOpAST;
        ast->first_token = consumeToken();
        consumeToken();
        ast->last_token = consumeToken();
        node = ast;
        return true;
    }

    return false;
}

bool Parser::parseDeleteStatement( DeleteStatementAST *&node )
{
    DeleteStatementAST *ast = new (_pool) DeleteStatementAST;
    ast->delete_token = consumeToken();
    if ( !match1(T_4_FROM, &ast->from_token) )
        P_RET_TRUE;

    if ( !isPatentialIdentifier() )
        P_RET_TRUE_E("Expected table name");

    DirectSingleSourceAST *source = new (_pool) DirectSingleSourceAST;
    source->schemaTableName = new (_pool) SchemaTableNameAST;
    source->schemaTableName->parse(this, _pool);
    bool needAlias = false;
    if ( LA1() == T_2_AS ) {
        source->as_token = consumeToken();
        needAlias = true;
    }
    if ( isPatentialIdentifier() ) {
        const Sql::Token &tk1 = tok();
        source->tableAlias = new (_pool) TableAliasAST;
        source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
        source->tableAlias->name_token = consumeToken();
        source->tableAlias->schemaTableName = source->schemaTableName;
    }
    else if ( needAlias )
        error(_tokenIndex, "Expected table source alias");
    ast->source = source;
    if ( LA1() == T_5_WHERE ) {
        ast->where_token = consumeToken();
        parseConditionExpression(ast->whereExpr);
    }

    P_RET_TRUE;
}

bool Parser::parseDeleteStatement( StatementAST *&node )
{
    DeleteStatementAST *ast = 0;
    bool result = parseDeleteStatement(ast);
    node = ast;
    return result;
}

bool Parser::parseInsertStatement( InsertStatementAST *&node )
{
    InsertStatementAST *ast = new (_pool) InsertStatementAST;
    ast->insert_token = consumeToken();
    if ( !match1(T_4_INTO, &ast->into_token) )
        P_RET_TRUE;
    if ( !isPatentialIdentifier() )
        P_RET_TRUE_E("Expected table name");
    DirectSingleSourceAST *source = new (_pool) DirectSingleSourceAST;
    source->schemaTableName = new (_pool) SchemaTableNameAST;
    source->schemaTableName->parse(this, _pool);
    bool needAlias = false;
    if ( LA1() == T_2_AS ) {
        source->as_token = consumeToken();
        needAlias = true;
    }
    if ( isPatentialIdentifier() ) {
        const Sql::Token &tk1 = tok();
        source->tableAlias = new (_pool) TableAliasAST;
        source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
        source->tableAlias->name_token = consumeToken();
        source->tableAlias->schemaTableName = source->schemaTableName;
    }
    else if ( needAlias )
        error(_tokenIndex, "Expected table source alias");
    ast->source = source;
    if ( LA() == Sql::T_LPAREN ) {
        ast->lparen_columns_token = consumeToken();
        if ( !parseColumnList(ast->columns_list) )
            P_RET_TRUE;
        match(Sql::T_RPAREN, &ast->rparen_columns_token);
    }
    if ( LA1() == T_6_VALUES ) {
        InsertClauseAsValuesAST *valClause = new (_pool) InsertClauseAsValuesAST;
        valClause->values_token = consumeToken();
        if ( !match(Sql::T_LPAREN, &valClause->lparen_token) )
            P_RET_TRUE;
        if ( !parseValueExpresionList(valClause->val_list, true) )
            P_RET_TRUE;
        match(Sql::T_RPAREN, &valClause->rparen_token);
        ast->insClause = valClause;
    }
    else if ( LA1() == T_6_SELECT ) {
        InsertClauseAsSelectAST *selClause = new (_pool) InsertClauseAsSelectAST;
        if ( !parseSelectStatement(selClause->selectStatement) )
            return false;
        ast->insClause = selClause;
    }
    else
        P_RET_TRUE_E("Expected `values' or `select' token");

    P_RET_TRUE;
}

bool Parser::parseInsertStatement( StatementAST *&node )
{
    InsertStatementAST *ast = 0;
    bool result = parseInsertStatement(ast);
    node = ast;
    return result;
}

bool Parser::parseUpdateStatement( UpdateStatementAST *&node )
{
    UpdateStatementAST *ast = new (_pool) UpdateStatementAST;
    ast->update_token = consumeToken();
    if ( !isPatentialIdentifier() )
        P_RET_TRUE_E("Expected table name");

    DirectSingleSourceAST *source = new (_pool) DirectSingleSourceAST;
    source->schemaTableName = new (_pool) SchemaTableNameAST;
    source->schemaTableName->parse(this, _pool);
    bool needAlias = false;
    if ( LA1() == T_2_AS ) {
        source->as_token = consumeToken();
        needAlias = true;
    }
    if ( isPatentialIdentifier() ) {
        const Sql::Token &tk1 = tok();
        source->tableAlias = new (_pool) TableAliasAST;
        source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
        source->tableAlias->name_token = consumeToken();
        source->tableAlias->schemaTableName = source->schemaTableName;
    }
    else if ( needAlias )
        error(_tokenIndex, "Expected table source alias");
    ast->source = source;

    if ( !match1(T_3_SET, &ast->set_token))
        P_RET_TRUE;
    if ( !parseAssignmentExpressionList(ast->upd_list) )
        P_RET_TRUE;

    if ( LA1() == T_5_WHERE ) {
        ast->where_token = consumeToken();
        parseConditionExpression(ast->whereExpr);
    }

    P_RET_TRUE;
}

bool Parser::parseUpdateStatement( StatementAST *&node )
{
    UpdateStatementAST *ast = 0;
    bool result = parseUpdateStatement(ast);
    node = ast;
    return result;
}

bool Parser::parseColumnList(ColumnNameListAST *&node)
{
    ColumnNameListAST **col_list = &node;

    while (1) {
        unsigned first_token = cursor();

        if ( LA() == Sql::T_RPAREN )
            return true;
        if ( LA() == Sql::T_COMMA ) {
            warning(_tokenIndex, "Expected column name");
            consumeToken();
            continue;
        }
        if ( LA1() || LA() == Sql::T_EOF_SYMBOL || LA() == Sql::T_SEMICOLON ) {
            return true;
        }

        if ( isPatentialIdentifier() ) {
            ColumnNameAST *column_name = new (_pool) ColumnNameAST;
            if ( LA(2) == Sql::T_DOT && isPatentialIdentifier(3) && LA(4) == Sql::T_DOT ) {
                SchemaNameAST *schemaName = new (_pool) SchemaNameAST;
                const Sql::Token &stk = tok();
                schemaName->name = stk.identifier;
                schemaName->name_token = consumeToken();
                consumeToken();
                TableNameAST *tableName = new (_pool) TableNameAST;
                const Sql::Token &ttk = tok();
                tableName->name = ttk.identifier;
                tableName->name_token = consumeToken();
                consumeToken();
                column_name->schemaName = schemaName;
                column_name->tableOfColumn = tableName;
            }
            else if ( LA(2) == Sql::T_DOT ) {
                TableAliasAST *alias = new (_pool) TableAliasAST;
                const Sql::Token &tk = tok();
                alias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                alias->name_token = consumeToken();
                consumeToken();
                column_name->tableOfColumn = alias;
            }
            if ( isPatentialIdentifier() ) {
                const Sql::Token &tk = tok();
                column_name->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                column_name->name_token = consumeToken();
                *col_list = new (_pool) ColumnNameListAST;
                (*col_list)->value = column_name;
                col_list = &(*col_list)->next;
            }
            else {
                warning(_tokenIndex, "Expected identifier got `%s'", tok().spell());
            }
        }
        else {
            warning(_tokenIndex, "Expected identifier got `%s'", tok().spell());
        }


        if ( LA() == Sql::T_COMMA ) {
            consumeToken();
            continue;
        }

        if ( first_token == cursor() ) {
            error(_tokenIndex, "WTF! (parseColumnList)");
            return false;
        }
    }

    return true;
}

bool Parser::parseAssignmentExpressionList(AssignmentExpressionListAST *&node)
{
    AssignmentExpressionListAST **upd_list = &node;

    while (1) {
        unsigned first_token = cursor();

        if ( LA1() || LA() == Sql::T_EOF_SYMBOL || LA() == Sql::T_SEMICOLON ) {
            return true;
        }
        if ( LA() == Sql::T_COMMA ) {
            warning(_tokenIndex, "Expected assignment expression");
            consumeToken();
            continue;
        }

        if ( isPatentialIdentifier() ) {
            ColumnNameAST *column_name = new (_pool) ColumnNameAST;
            AssignmentExpressionAST *assignExpr = new (_pool) AssignmentExpressionAST;
            assignExpr->first_token = cursor();
            if ( LA(2) == Sql::T_DOT && isPatentialIdentifier(3) && LA(4) == Sql::T_DOT ) {
                SchemaNameAST *schemaName = new (_pool) SchemaNameAST;
                const Sql::Token &stk = tok();
                schemaName->name = stk.identifier;
                schemaName->name_token = consumeToken();
                consumeToken();
                TableNameAST *tableName = new (_pool) TableNameAST;
                const Sql::Token &ttk = tok();
                tableName->name = ttk.identifier;
                tableName->name_token = consumeToken();
                consumeToken();
                column_name->schemaName = schemaName;
                column_name->tableOfColumn = tableName;
            }
            else if ( LA(2) == Sql::T_DOT ) {
                TableAliasAST *alias = new (_pool) TableAliasAST;
                const Sql::Token &tk = tok();
                alias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                alias->name_token = consumeToken();
                consumeToken();
                column_name->tableOfColumn = alias;
            }
            if ( isPatentialIdentifier() ) {
                const Sql::Token &tk = tok();
                column_name->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                column_name->name_token = consumeToken();
                assignExpr->columnName = column_name;
                if ( LA() != Sql::T_EQUAL ) {
                    error(_tokenIndex, "Expected token `='");
                    return false;
                }
                assignExpr->equal_token = consumeToken();
                if ( !parseValueExpresion(assignExpr->rvExpr) )
                    return false;
                assignExpr->last_token = cursor();
                *upd_list = new (_pool) AssignmentExpressionListAST;
                (*upd_list)->value = assignExpr;
                upd_list = &(*upd_list)->next;
            }
            else {
                warning(_tokenIndex, "Expected identifier got `%s'", tok().spell());
            }
        }
        else {
            warning(_tokenIndex, "Expected identifier got `%s'", tok().spell());
        }


        if ( LA() == Sql::T_COMMA ) {
            consumeToken();
            continue;
        }

        if ( first_token == cursor() ) {
            error(_tokenIndex, "WTF! (parseAssignmentExpressionList)");
            return false;
        }
    }
}

bool Parser::parseDropStatement(StatementAST *&node)
{
    unsigned drop_token = consumeToken();

    switch ( LA1() ) {
        case T_5_TABLE: {
            DropTableStatementAST *ast = new (_pool) DropTableStatementAST;
            ast->drop_token = drop_token;
            ast->table_token = consumeToken();
            if ( !isPatentialIdentifier() ) {
                error(_tokenIndex, "Expected table name");
                return false;
            }
            ast->schemaTableName = new (_pool) SchemaTableNameAST;
            ast->schemaTableName->parse(this, _pool);
            node = ast;
            return true;
        }
        case T_4_VIEW: {
            DropViewStatementAST *ast = new (_pool) DropViewStatementAST;
            ast->drop_token = drop_token;
            ast->view_token = consumeToken();
            if ( !isPatentialIdentifier() ) {
                error(_tokenIndex, "Expected view name");
                return false;
            }
            ast->schemaViewName = new (_pool) SchemaTableNameAST;
            ast->schemaViewName->parse(this, _pool);
            node = ast;
            return true;
        }
        case T_5_INDEX: {
            DropIndexStatementAST *ast = new (_pool) DropIndexStatementAST;
            ast->drop_token = drop_token;
            ast->index_token = consumeToken();
            if ( !isPatentialIdentifier() ) {
                error(_tokenIndex, "Expected index name");
                return false;
            }
            ast->schemaIndexName = new (_pool) SchemaIndexNameAST;
            ast->schemaIndexName->parse(this, _pool);
            node = ast;
            return true;
        }
        case T_7_TRIGGER: {
            DropTriggerStatementAST *ast = new (_pool) DropTriggerStatementAST;
            ast->drop_token = drop_token;
            ast->trigger_token = consumeToken();
            if ( !isPatentialIdentifier() ) {
                error(_tokenIndex, "Expected trigger name");
                return false;
            }
            ast->schemaTriggerName = new (_pool) SchemaTriggerNameAST;
            ast->schemaTriggerName->parse(this, _pool);
            node = ast;
            return true;
        }
        case T_6_MODULE: {
            DropModuleStatementAST *ast = new (_pool) DropModuleStatementAST;
            ast->drop_token = drop_token;
            ast->module_token = consumeToken();
            if ( LA() == Sql::T_IDENTIFIER ) {
                const Sql::Token &tk = tok();
                ast->moduleName = new (_pool) ModuleNameAST;
                ast->moduleName->name = tk.identifier;
                ast->moduleName->name_token = consumeToken();
            }
            else
                error(_tokenIndex, "Module Name Expected");
            node = ast;
            return true;
        }
        default:
            error(_tokenIndex, "Expected object type (TABLE, VIEW, INDEX, TRIGGER)");
            return false;
    }

    return false;
}

bool Parser::parseTruncateStatement( StatementAST *&node )
{
    TruncateStatementAST *ast = new (_pool) TruncateStatementAST;
    ast->truncate_token = consumeToken();
    if ( !match1(T_5_TABLE, &ast->table_token) )
        return false;
    if ( !isPatentialIdentifier() ) {
        error(_tokenIndex, "Table name expected");
        return false;
    }
    ast->schemaTableName = new (_pool) SchemaTableNameAST;
    ast->schemaTableName->parse(this, _pool);
    node = ast;
    return true;
}

bool Parser::parseCommentStatement( StatementAST *&node )
{
    unsigned comment_token = consumeToken();

    unsigned on_token;
    if ( !match1(T_2_ON, &on_token) )
        return false;

    switch ( LA1() ) {
        case T_5_TABLE: {
            CommentOnTableStatementAST *ast = new (_pool) CommentOnTableStatementAST;
            ast->comment_token = comment_token;
            ast->on_token = on_token;
            ast->table_token = consumeToken();
            if ( !isPatentialIdentifier() ) {
                error(_tokenIndex, "Expected Table Name");
                node = ast;
                return true;
            }
            ast->schemaTableName = new (_pool) SchemaTableNameAST;
            ast->schemaTableName->parse(this, _pool);
            if ( !match1(T_2_IS, &ast->is_token) ) {
                node = ast;
                return true;
            }
            if ( LA() != Sql::T_CHAR_LITERAL ) {
                error(_tokenIndex, "Expected comment literal token");
                node = ast;
                return true;
            }
            ast->comment = new (_pool) LiteralExpressionAST;
            ast->comment->first_token = cursor();
            ast->comment->liter_token = consumeToken();
            ast->comment->last_token = ast->comment->liter_token;
            node = ast;
            return true;
        }
        case T_6_COLUMN: {
            CommentOnColumnStatementAST *ast = new (_pool) CommentOnColumnStatementAST;
            ast->comment_token = comment_token;
            ast->on_token = on_token;
            ast->column_token = consumeToken();

            bool schemaPresent = false;
            if ( isPatentialIdentifier() ) {
                if ( LA(2) == Sql::T_DOT ) {
                    if ( isPatentialIdentifier(3) && LA(4) == Sql::T_DOT)  {
                        schemaPresent = true;
                    }
                }
                else {
                    error(_tokenIndex + 1, "Expected token `.'");
                    node = ast;
                    return true;
                }
            }
            else {
                error(_tokenIndex, "Expected table name");
                node = ast;
                return true;
            }

            SchemaTableNameAST *schemaTableName = new (_pool) SchemaTableNameAST;
            if ( schemaPresent ) {
                const Sql::Token &stk = tok();
                schemaTableName->schemaName = new (_pool) SchemaNameAST;
                schemaTableName->schemaName->name = stk.identifier;
                schemaTableName->schemaName->name_token = consumeToken();
                schemaTableName->dot_token = consumeToken();
            }
            const Sql::Token &ttk = tok();
            schemaTableName->tableName = new (_pool) TableNameAST;
            schemaTableName->tableName->name = ttk.identifier;
            schemaTableName->tableName->name_token = consumeToken();
            ast->dot_token = consumeToken();
            ast->schemaTableName = schemaTableName;


            if ( !isPatentialIdentifier() ) {
                error(_tokenIndex, "Expected column name");
                node = ast;
                return true;
            }
            ColumnNameAST *colAst = new (_pool) ColumnNameAST;
            const Sql::Token &tk1 = tok();
            colAst->name = tk1.identifier;
            colAst->name_token = consumeToken();
            ast->column = colAst;
            if ( !match1(T_2_IS, &ast->is_token) ) {
                node = ast;
                return true;
            }
            if ( LA() != Sql::T_CHAR_LITERAL ) {
                error(_tokenIndex, "Expected comment literal token");
                node = ast;
                return true;
            }
            ast->comment = new (_pool) LiteralExpressionAST;
            ast->comment->first_token = cursor();
            ast->comment->liter_token = consumeToken();
            ast->comment->last_token = ast->comment->liter_token;
            node = ast;
            return true;
        }
        default:
            error(_tokenIndex, "Expected TABLE|COLUMN token");
            return false;
    }

    return false;
}

bool Parser::parseCreateStatement( StatementAST *&node )
{
    unsigned create_token = consumeToken();

    if ( LA1() == T_5_TABLE ) {
        CreateTableStatementAST *ast = new (_pool) CreateTableStatementAST;
        ast->create_token = create_token;
        ast->table_token = consumeToken();
        if ( !isPatentialIdentifier() ) {
            P_RET_FALSE("Expected table name");
        }
        ast->schemaTableName = new (_pool) SchemaTableNameAST;
        ast->schemaTableName->parse(this, _pool);
        if ( LA1() == T_2_AS ) {
            CreateTableAsSelectClauseAST *asClause = new (_pool) CreateTableAsSelectClauseAST;
            asClause->as_token = consumeToken();
            if ( LA1() == T_6_SELECT )
                parseSelectStatement(asClause->selectStatement);
            else
                warningCursor("Expected SELECT token");
            ast->tableClause = asClause;
            P_RET_TRUE;
        }
        else if ( LA() == Sql::T_LPAREN ) {
            CreateTableDirectClauseAST *directClause = new (_pool) CreateTableDirectClauseAST;
            directClause->lparen_token = consumeToken();
            parseColumnDefinitionList(directClause->column_def_list);
            parseTableConstraintList(directClause->constraintList);
            match(Sql::T_RPAREN, &directClause->rparen_token);
            ast->tableClause = directClause;
            P_RET_TRUE;
        }
        P_RET_TRUE_E("Expected `(' or AS token");
    }

    if ( (LA1() == T_2_OR && LA1(2) == T_7_REPLACE && LA1(3) == T_4_VIEW) || (LA1() == T_4_VIEW) ) {
        CreateViewStatementAST *ast = new (_pool) CreateViewStatementAST;
        ast->create_token = create_token;
        if ( LA1() == T_2_OR ) {
            consumeToken();
            consumeToken();
        }
        ast->view_token = consumeToken();
        if ( !isPatentialIdentifier() ) {
            error(_tokenIndex, "Expected view name");
            return false;
        }
        ast->schemaViewName = new (_pool) SchemaTableNameAST;
        ast->schemaViewName->parse(this, _pool);
        if ( !match1(T_2_AS, &ast->as_token) ) {
            node = ast;
            return true;
        }
        if ( LA1() == T_6_SELECT )
            parseSelectStatement(ast->selectStatement);
        else
            warningCursor("Expected SELECT token");
        node = ast;
        return true;
    }

    if ( (LA1() == T_6_UNIQUE && LA1(2) == T_5_INDEX) || LA1() == T_5_INDEX ) {
        CreateIndexStatementAST *ast = new (_pool) CreateIndexStatementAST;
        ast->create_token = create_token;
        if ( LA1() == T_6_UNIQUE )
            ast->unique_token = consumeToken();
        ast->index_token = consumeToken();
        if ( !isPatentialIdentifier() ) {
            error(_tokenIndex, "Expected table name");
            return false;
        }
        ast->schemaIndexName = new (_pool) SchemaIndexNameAST;
        ast->schemaIndexName->parse(this, _pool);
        if ( LA1() != T_2_ON ) {
            error(_tokenIndex, "Expected ON token");
            node = ast;
            return true;
        }
        ast->on_token = consumeToken();
        if ( !isPatentialIdentifier() ) {
            error(_tokenIndex, "Expected table name");
            node = ast;
            return true;
        }
        ast->schemaTableName = new (_pool) SchemaTableNameAST;
        ast->schemaTableName->parse(this, _pool);
        if ( !match(Sql::T_LPAREN, &ast->lparen_token) ) {
            error(_tokenIndex, "Expected token `('");
            node = ast;
            return true;
        }
        parseSortedColumnNameList(ast->columns_list);
        if ( match(Sql::T_RPAREN, &ast->rparen_token) ) {
            if ( LA() != Sql::T_SEMICOLON && LA() != Sql::T_EOF_SYMBOL ) {
                ast->options = new (_pool) IndexOptionsAST;
                ast->options->parse(this, _pool);
            }
        }
        node = ast;
        return true;
    }

    if ( LA1() == T_7_TRIGGER ) {
        CreateTriggerStatementAST *ast = new (_pool) CreateTriggerStatementAST;
        ast->create_token = create_token;
        ast->trigger_token = consumeToken();
        if ( !isPatentialIdentifier() ) {
            error(_tokenIndex, "Expected Trigger Name");
            return false;
        }
        ast->schemaTriggerName = new (_pool) SchemaTriggerNameAST;
        ast->schemaTriggerName->parse(this, _pool);
        if ( LA1() == T_6_BEFORE || LA1() == T_5_AFTER )
            ast->before_or_after_token = consumeToken();
        else {
            error(_tokenIndex, "Expected BEFORE|AFTER token");
            node = ast;
            return true;
        }
        if ( LA1() == T_6_DELETE || LA1() == T_6_INSERT || LA1() == T_6_UPDATE )
            ast->action_token = consumeToken();
        else {
            error(_tokenIndex, "Expected DELETE|INSERT|UPDATE token");
            node = ast;
            return true;
        }
        if ( LA1() == T_2_OF ) {
            ast->of_token = consumeToken();
            parseColumnList(ast->col_list);
        }
        if ( LA1() != T_2_ON ) {
            error(_tokenIndex, "Expected ON token");
            node = ast;
            return true;
        }
        ast->on_token = consumeToken();
        if ( !isPatentialIdentifier() ) {
            error(_tokenIndex, "Expected Table name");
            node = ast;
            return true;
        }
        ast->schemaTableName = new (_pool) SchemaTableNameAST;
        ast->schemaTableName->parse(this, _pool);
        if ( LA1() == T_11_REFERENCING ) {
            ast->triggerRefClause = new (_pool) CreateTriggerRefClauseAST(ast->schemaTableName);
            ast->triggerRefClause->parse(this, _pool);
        }
        if ( LA() == Sql::T_LPAREN || LA1() == T_4_WHEN ) {
            CreateRdbTriggerBodyAST *triggerBody = new (_pool) CreateRdbTriggerBodyAST;
            parseRdbTriggerBodyList(triggerBody->item_list);
            ast->triggerBody = triggerBody;
        }
        /// TODO: Oracle/SQLite (BEGIN ... END;) or Db2 (BEGIN ATOMIC ... END;) dialects
        // else { ... }
        node = ast;
        return true;
    }

    if ( LA1() == T_7_STORAGE ) {
        CreateStorageMapStatementAST *ast = new (_pool) CreateStorageMapStatementAST;
        ast->create_token = create_token;
        ast->storage_token  = consumeToken();
        if ( LA1() == T_3_MAP ) {
            ast->map_token = consumeToken();
            if ( LA() == Sql::T_IDENTIFIER ) {
                ast->mapName = new (_pool) StorageMapNameAST;
                const Sql::Token &tk = tok();
                ast->mapName->name = tk.identifier;
                ast->mapName->name_token = consumeToken();
                if ( LA1() == T_3_FOR ) {
                    ast->for_token = consumeToken();
                    if ( LA() == Sql::T_IDENTIFIER ) {
                        ast->tableName = new (_pool) TableNameAST;
                        const Sql::Token &tk = tok();
                        ast->tableName->name = tk.identifier;
                        ast->tableName->name_token = consumeToken();
                        if ( LA1() == T_5_STORE ) {
                            ast->store_token = consumeToken();
                            if ( LA1() == T_2_IN ) {
                                ast->in_token = consumeToken();
                                if ( LA() == Sql::T_IDENTIFIER ) {
                                    ast->areaName = new (_pool) AreaNameAST;
                                    const Sql::Token &tk = tok();
                                    ast->areaName->name = tk.identifier;
                                    ast->areaName->name_token = consumeToken();
                                }
                                else
                                    error(_tokenIndex, "Area Name Expected");
                            }
                            else
                                error(_tokenIndex, "Expected Token `IN'");
                        }
                        else
                            error(_tokenIndex, "Expected Token `STORE'");
                    }
                    else
                        error(_tokenIndex, "Table Name Expected");
                }
                else
                    error(_tokenIndex, "Expected Token `FOR'");
            }
            else
                error(_tokenIndex, "Map Name Expected");
        }
        else
            error(_tokenIndex, "Expected Token `MAP'");
        node = ast;
        return true;
    }

    if ( LA1() == T_6_MODULE ) {
        _blockMode = true;
        CreateModuleStatementAST *ast =  new (_pool) CreateModuleStatementAST;
        ast->create_token = create_token;
        ast->module_token = consumeToken();
        if ( LA() == Sql::T_IDENTIFIER ) {
            const Sql::Token &tk = tok();
            ast->moduleName = new (_pool) ModuleNameAST;
            ast->moduleName->name = tk.identifier;
            ast->moduleName->name_token = consumeToken();
            if ( LA1() == T_8_LANGUAGE && LA1(2) == T_3_SQL ) {
                ast->language_token = consumeToken();
                ast->sql_token = consumeToken();
            }
            if ( LA1() == T_7_COMMENT && LA1(2) == T_2_IS ) {
                ast->comment_token = consumeToken();
                ast->is_token = consumeToken();
                if ( LA() == Sql::T_CHAR_LITERAL ) {
                    ast->commentExpr = new (_pool) LiteralExpressionAST;
                    ast->commentExpr->liter_token = consumeToken();
                }
                else
                    error(_tokenIndex, "Expected Comment Literal Entry");
            }
            parseModuleRoutineList(ast->routine_list);
        }
        else
            error(_tokenIndex, "Module Name Expected");
        node = ast;
        return true;
    }

    if ( LA1() == T_9_PROCEDURE || LA1() == T_8_FUNCTION ) {
        _blockMode = true;
        CreateRoutineStatementAST *ast = new (_pool) CreateRoutineStatementAST;
        bool isProcMode = ( LA1() == T_9_PROCEDURE );
        ast->create_token = create_token;
        ast->setProcedureType(isProcMode);
        parseRoutine(ast->routine);
        node = ast;
        return true;
    }

    error(_tokenIndex, "Expected TABLE|VIEW|INDEX|TRIGGER|STORAGE|FUNCTION|PROCEDURE token");
    return false;
}

bool Parser::parseColumnDefinitionList(ColumnDefinitionListAST *&node)
{
    if ( LA() == Sql::T_RPAREN || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
        return true;

    if ( LA() == Sql::T_COMMA ) {
        warningCursor("Expected column definition");
        consumeToken();
        return parseColumnDefinitionList(node);
    }

    ColumnDefinitionListAST **col_ptr = &node;

    if ( isPatentialIdentifier() ) {
        ColumnDefinitionAST *columnDef = new (_pool) ColumnDefinitionAST;
        columnDef->columnName = new (_pool) ColumnNameAST;
        const Sql::Token &tk = tok();
        columnDef->columnName->name = tk.identifier;
        columnDef->columnName->name_token = consumeToken();
        if ( LA3() ) {
            parseTypeSpecifier(columnDef->columnType);
        }
        else {
            errorCursor("Expected type specifier");
        }

        if ( LA1() == T_7_DEFAULT ) {
            columnDef->default_token = consumeToken();
            if ( LA1() == T_4_NULL ) {
                NullValueExpressionAST *nullExpr = new (_pool) NullValueExpressionAST;
                nullExpr->null_token = consumeToken();
                columnDef->defaultExpr = nullExpr;
            }
            else if ( LA() == Sql::T_NUMERIC_LITERAL ) {
                NumericExpressionAST *numericAst = new (_pool) NumericExpressionAST;
                numericAst->numeric_token = consumeToken();
                columnDef->defaultExpr = numericAst;
            }
            else if ( LA() == Sql::T_CHAR_LITERAL ) {
                LiteralExpressionAST *literAst = new (_pool) LiteralExpressionAST;
                literAst->liter_token = consumeToken();
                columnDef->defaultExpr = literAst;
            }
            else
                warningCursor("Expected default value");
        }

        *col_ptr = new (_pool) ColumnDefinitionListAST;
        (*col_ptr)->value = columnDef;
        col_ptr = &(*col_ptr)->next;

        parseColumnConstraintList(columnDef->constraintList);
    }
    else {
        switch ( LA1() ) {
            case T_10_CONSTRAINT:
            case T_7_PRIMARY:
            case T_6_UNIQUE:
            case T_5_CHECK:
            case T_7_FOREIGN:
                return true;
        }
        errorCursor("Column name expected");
    }

    while ( 1 ) {
        switch ( LA() ) {
            case Sql::T_COMMA:
                consumeToken();
                return parseColumnDefinitionList(*col_ptr);
            case Sql::T_RPAREN:
            case Sql::T_SEMICOLON:
            case Sql::T_EOF_SYMBOL:
                return false;
            default:
                consumeToken();
        }
    }

    return false;
}

bool Parser::parseColumnConstraintList( ColumnConstraintListAST *&node )
{
    ColumnConstraintListAST **constr_list_ptr = &node;

    if ( LA() == Sql::T_COMMA || LA() == Sql::T_RPAREN || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
        return true;

    unsigned constraint_token = 0;
    ConstraintNameAST *constraintName = 0;
    if ( LA1() == T_10_CONSTRAINT ) {
        constraint_token = consumeToken();
        if ( isPatentialIdentifier() ) {
            constraintName = new (_pool) ConstraintNameAST;
            const Sql::Token &tk = tok();
            constraintName->name = tk.identifier;
            constraintName->name_token = consumeToken();
        }
        else {
            errorCursor("Constraint name expected");
        }
        if ( LA() == Sql::T_COMMA || LA() == Sql::T_RPAREN || LA() == Sql::T_SEMICOLON )
            return true;
    }

    switch ( LA1() ) {
        case T_7_PRIMARY: {
            PkColumnConstraintAST *constraint = new (_pool) PkColumnConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            constraint->parse(this);
            *constr_list_ptr = new (_pool) ColumnConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            return parseColumnConstraintList(*constr_list_ptr);
        }
        case T_3_NOT: {
            NotNullColumnConstraintAST *constraint = new (_pool) NotNullColumnConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            constraint->parse(this);
            *constr_list_ptr = new (_pool) ColumnConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            return parseColumnConstraintList(*constr_list_ptr);
        }
        case T_6_UNIQUE: {
            UniqueColumnConstraintAST *constraint = new (_pool) UniqueColumnConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            constraint->unique_token = consumeToken();
            *constr_list_ptr = new (_pool) ColumnConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            return parseColumnConstraintList(*constr_list_ptr);
        }
        case T_5_CHECK: {
            CheckColumnConstraintAST *constraint = new (_pool) CheckColumnConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            constraint->parse(this);
            *constr_list_ptr = new (_pool) ColumnConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            return parseColumnConstraintList(*constr_list_ptr);
        }
        case T_10_REFERENCES: {
            ReferencesColumnConstraintAST *constraint = new (_pool) ReferencesColumnConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            constraint->parse(this, _pool);
            *constr_list_ptr = new (_pool) ColumnConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            return parseColumnConstraintList(*constr_list_ptr);
        }
    }

    return false;
}

bool Parser::parseTableConstraintList(TableConstraintListAST *&node)
{
    if ( LA() == Sql::T_RPAREN || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
        return true;

    if ( LA() == Sql::T_COMMA ) {
        warningCursor("Expected table constraint definition");
        consumeToken();
        return parseTableConstraintList(node);
    }

    TableConstraintListAST **constr_list_ptr = &node;

    unsigned constraint_token = 0;
    ConstraintNameAST *constraintName = 0;
    if ( LA1() == T_10_CONSTRAINT ) {
        constraint_token = consumeToken();
        if ( isPatentialIdentifier() ) {
            constraintName = new (_pool) ConstraintNameAST;
            const Sql::Token &tk = tok();
            constraintName->name = tk.identifier;
            constraintName->name_token = consumeToken();
        }
        else {
            errorCursor("Constraint name expected");
        }
    }

    bool notCheckEndCorrect = true;
    switch ( LA1() ) {
        case T_6_UNIQUE:
        case T_7_PRIMARY: {
            UniqueOrPkTableConstraintAST *constraint = new (_pool) UniqueOrPkTableConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            notCheckEndCorrect = constraint->parse(this);
            *constr_list_ptr = new (_pool) TableConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            break;
        }
        case T_5_CHECK: {
            CheckTableConstraintAST *constraint = new (_pool) CheckTableConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            notCheckEndCorrect = constraint->parse(this);
            *constr_list_ptr = new (_pool) TableConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            break;
        }
        case T_7_FOREIGN: {
            FkTableConstraintAST *constraint = new (_pool) FkTableConstraintAST;
            constraint->constraint_token = constraint_token;
            constraint->constraintName = constraintName;
            notCheckEndCorrect = constraint->parse(this, _pool);
            *constr_list_ptr = new (_pool) TableConstraintListAST;
            (*constr_list_ptr)->value = constraint;
            constr_list_ptr = &(*constr_list_ptr)->next;
            break;
        }
    }

    if ( !notCheckEndCorrect ) {
        if ( LA() != Sql::T_COMMA && LA() != Sql::T_RPAREN )
            errorCursor("Unexpected token");
    }

    while ( 1 ) {
        switch ( LA() ) {
            case Sql::T_COMMA:
                consumeToken();
                return parseTableConstraintList(*constr_list_ptr);
            case Sql::T_RPAREN:
                return true;
            case Sql::T_SEMICOLON:
            case Sql::T_EOF_SYMBOL:
                return false;
            default:
                consumeToken();
        }
    }

    return false;
}

bool Parser::parseSortedColumnNameList( SortedColumnNameListAST *&node )
{
    if ( LA() == Sql::T_RPAREN || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
        return true;

    if ( LA() == Sql::T_COMMA ) {
        warning(_tokenIndex, "Expected column name");
        consumeToken();
        return parseSortedColumnNameList(node);
    }

    SortedColumnNameListAST **col_list_ptr = &node;

    if ( isPatentialIdentifier() ) {
        SortedColumnNameAST *columnName = new (_pool) SortedColumnNameAST;
        columnName->columnName = new (_pool) ColumnNameAST;
        const Sql::Token &tk = tok();
        columnName->columnName->name = tk.identifier;
        columnName->columnName->name_token = consumeToken();

        if ( LA1() == T_3_ASC || LA1() == T_4_DESC )
            columnName->asc_or_desc_token = consumeToken();

        *col_list_ptr = new (_pool) SortedColumnNameListAST;
        (*col_list_ptr)->value = columnName;
        col_list_ptr = &(*col_list_ptr)->next;

        if ( LA() != Sql::T_COMMA && LA() != Sql::T_RPAREN) {
            errorCursor("Unexpected token");
        }
    }
    else
        errorCursor("Expected column name");

    while ( 1 ) {
        switch ( LA() ) {
            case Sql::T_COMMA:
                consumeToken();
                return parseSortedColumnNameList(*col_list_ptr);
            case Sql::T_RPAREN:
                return true;
            case Sql::T_SEMICOLON:
            case Sql::T_EOF_SYMBOL:
                return false;
            default:
                consumeToken();
        }
    }

    return false;
}

bool Parser::parseRdbTriggerBodyList( CreateRdbTriggerBodyListAST *&node )
{
    if ( LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
        return true;

    if ( LA1() != T_4_WHEN && LA() != Sql::T_LPAREN )
        return true;

    CreateRdbTriggerBodyItemAST *bodyItem = new (_pool) CreateRdbTriggerBodyItemAST;

    bool ok = true;
    if ( LA1() == T_4_WHEN ) {
        bodyItem->when_token = consumeToken();
        if ( LA() == Sql::T_LPAREN ) {
            bodyItem->lparen_when_token = consumeToken();
            parseConditionExpression(bodyItem->whenExpr);
            if ( LA() == Sql::T_RPAREN )
                bodyItem->rparen_when_token = consumeToken();
            else {
                ok = false;
                errorCursor("Expected `)' token");
            }
        }
        else {
            ok = false;
            errorCursor("Expected `(' token");
        }
    }

    if ( ok ) {
        if ( LA() == Sql::T_LPAREN ) {
            bodyItem->lparen_token = consumeToken();
            if ( ok = parseTriggerStatementList(bodyItem->stat_list) ) {
                if ( LA() == Sql::T_RPAREN ) {
                    bodyItem->rparen_token = consumeToken();
                    if ( LA1() == T_3_FOR ) {
                        bodyItem->for_token = consumeToken();
                        if ( LA1() == T_4_EACH ) {
                            bodyItem->each_token = consumeToken();
                            if ( LA1() == T_3_ROW || LA1() == T_9_STATEMENT )
                                bodyItem->row_or_statement_token = consumeToken();
                            else
                                warningCursor("Expected ROW|STATEMENT token");
                        }
                        else
                            warningCursor("Expected EACH token");
                    }
                }
                else {
                    ok = false;
                    errorCursor("Expected `)' token");
                }
            }
        }
        else {
            ok = false;
            errorCursor("Expected `(' token");
        }
    }

    if ( bodyItem->when_token || bodyItem->lparen_token ) {
        CreateRdbTriggerBodyListAST **list_ptr = &node;
        *list_ptr = new (_pool) CreateRdbTriggerBodyListAST;
        (*list_ptr)->value = bodyItem;
        list_ptr = &(*list_ptr)->next;
        if ( ok ) {
            return parseRdbTriggerBodyList(*list_ptr);
        }
    }

    return false;
}

bool Parser::parseTriggerStatementList( StatementListAST *&node )
{
    if ( LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL || LA() == Sql::T_RPAREN )
        return true;

    if ( LA() == Sql::T_COMMA ) {
        warningCursor("Unexpected token `,'");
        consumeToken();
        return parseTriggerStatementList(node);
    }

    int la = LA1();
    StatementAST *statement = 0;

    if ( la == T_6_DELETE ) {
        parseDeleteStatement(statement);
    }
    else if ( la == T_6_INSERT ) {
        parseInsertStatement(statement);
    }
    else if ( la == T_6_UPDATE ) {
        parseUpdateStatement(statement);
    }
    else if ( la == T_4_CALL ) {
        parseCallStatement(statement);
    }
    else if ( LA() == Sql::T_IDENTIFIER ) {
        const Sql::Token &tk = tok();
        QString errStr = tk.identifier->chars();
        if ( !errStr.compare("ERROR", Qt::CaseInsensitive) )
            consumeToken();
        else {
            errorCursor("Expected CALL|DELETE|INSERT|UPDATE|ERROR token");
            consumeToken();
            return true;
        }
    }
    else {
        errorCursor("Expected CALL|DELETE|INSERT|UPDATE|ERROR token");
        consumeToken();
        return true;
    }

    StatementListAST **stat_list_ptr = &node;
    if ( statement ) {
        *stat_list_ptr = new (_pool) StatementListAST;
        (*stat_list_ptr)->value = statement;
        stat_list_ptr = &(*stat_list_ptr)->next;
    }

    if ( LA() == Sql::T_COMMA ) {
        consumeToken();
    }

    return parseTriggerStatementList(*stat_list_ptr);
}

bool Parser::parseAlterStatement( StatementAST *&node )
{
    if ( LA1(2) != T_5_TABLE ) {
        error(_tokenIndex + 1, "Expected token `TABLE'");
        return false;
    }

    AlterTableStatementAST *ast = new (_pool) AlterTableStatementAST;
    node = ast;
    return ast->parse(this, _pool);
}

bool Parser::parseCommitStatement( StatementAST *&node )
{
    CommitStatementAST *ast = new (_pool) CommitStatementAST;
    ast->commit_token = consumeToken();
    node = ast;
    return true;
}

bool Parser::parseRollbackStatement( StatementAST *&node )
{
    RollbackStatementAST *ast = new (_pool) RollbackStatementAST;
    ast->rollback_token = consumeToken();
    node = ast;
    return true;
}

bool Parser::parseCallStatement( CallStatementAST *&node )
{
    CallStatementAST *ast = new (_pool) CallStatementAST;
    ast->call_token = consumeToken();
    if ( LA() == Sql::T_IDENTIFIER ) {
        const Sql::Token &tk = tok();
        ast->funcName = new (_pool) RoutineNameAST;
        ast->funcName->name = tk.identifier;
        ast->funcName->name_token = consumeToken();
        if ( LA() == Sql::T_LPAREN ) {
            ast->lparen_token = consumeToken();
            if ( parseValueExpresionList(ast->arg_list, true) ) {
                if ( LA() == Sql::T_RPAREN )
                    ast->rparen_token = consumeToken();
                else
                    error(_tokenIndex, "Expected Token `)'");
            }
        }
        else
            error(_tokenIndex, "Expected Token `('");
    }
    else
        error(_tokenIndex, "Function/Procedure Name Expected");
    node = ast;
    return true;
}

bool Parser::parseCallStatement( StatementAST *&node )
{
    CallStatementAST *ast = 0;
    bool result = parseCallStatement(ast);
    node = ast;
    return result;
}

bool Parser::parseBlockStatement( BlockStatementAST *&node , bool isFuncMode )
{
    BlockStatementAST *ast = new (_pool) BlockStatementAST;
    ast->begin_token = consumeToken();
    if ( parseBlockDeclareList(ast->decl_list) ) {
        if ( parseBlockEntryList(ast->entry_list, isFuncMode) ) {
            if ( LA1() == T_3_END ) {
                ast->end_token = cursor();
                if ( LA(2) != Sql::T_SEMICOLON )
                    error(_tokenIndex + 1, "Expected `;' got `%s'", tok(2).spell());
            }
            else {
                error(_tokenIndex, "expected token `END' got `%s'", tok().spell());
            }
        }
    }
    node = ast;
    return true;
}

bool Parser::parseBlockStatement( StatementAST *&node )
{
    BlockStatementAST *ast = 0;
    bool result = parseBlockStatement(ast);
    node = ast;
    return result;
}

bool Parser::parseBlockDeclareList( DeclareListAST *&node )
{
    if ( LA1() != T_7_DECLARE )
        return true;

    DeclareAST *declItem = new (_pool) DeclareAST;
    declItem->declare_token = consumeToken();

    if ( LA() == Sql::T_COLON && LA(2) == Sql::T_IDENTIFIER ) {
        declItem->identName = new (_pool) IdentifierNameAST;
        declItem->identName->colon_token = consumeToken();
        const Sql::Token &tk = tok();
        declItem->identName->name = tk.identifier;
        declItem->identName->name_token = consumeToken();
        if ( parseTypeSpecifier(declItem->identType) ) {
            if ( LA1() == T_7_DEFAULT ) {
                declItem->default_token = consumeToken();
                parseValueExpresion(declItem->defaulExpr);
            }
        }
        else
            error(_tokenIndex, "Identifier Type Expected");
    }
    else
        error(_tokenIndex, "Expected Token [:IDENT_NAME]");

    if ( skipUntil(Sql::T_SEMICOLON) )
        consumeToken();

    DeclareListAST **list_ptr = &node;
    *list_ptr = new (_pool) DeclareListAST;
    (*list_ptr)->value = declItem;
    list_ptr = &(*list_ptr)->next;

    return parseBlockDeclareList(*list_ptr);
}

bool Parser::parseBlockEntryList( BlockStatementEntryListAST *&node, bool isFuncMode )
{
    if ( LA() == Sql::T_EOF_SYMBOL || LA1() == T_3_END || LA1() == T_4_ELSE )
        return true;

    BlockStatementEntryAST *entry = 0;
    bool isNotLabel = true;

    switch ( LA1() ) {

        case T_5_BEGIN: {
            BlockBlockStatementAST *innerBlock = new (_pool) BlockBlockStatementAST;
            parseBlockStatement(innerBlock->block, isFuncMode);
            entry = innerBlock;
            break;
        }

        case T_6_SELECT:    parseBlockSelectStatement(entry);       break;
        case T_6_INSERT:    parseBlockInsertStatement(entry);       break;
        case T_6_UPDATE:    parseBlockUpdateStatement(entry);       break;
        case T_6_DELETE:    parseBlockDeleteStatement(entry);       break;
        case T_3_SET:       parseBlockSetStatemnt(entry);           break;
        case T_6_COMMIT:    parseBlockCommitStatemnt(entry);        break;
        case T_8_ROLLBACK:  parseBlockRollbackStatemnt(entry);      break;
        case T_4_CALL:      parseBlockCallStatement(entry);         break;
        case T_2_IF:        parseIfStatemnt(entry, isFuncMode);     break;
        case T_5_WHILE:     // Break Down!!!
        case T_4_LOOP:      parseLoopStatement(entry, isFuncMode);  break;
        case T_5_LEAVE:     parseLeaveStatemnt(entry);              break;
        case T_3_FOR:       parseForStatement(entry, isFuncMode);   break;

        case T_6_RETURN:
            if ( isFuncMode ) {
                BlockReturnStatementAST *retAst = new (_pool) BlockReturnStatementAST;
                retAst->return_token = consumeToken();
                parseValueExpresion(retAst->returnExpr);
                entry = retAst;
                break;
            }
            //!!! Break Down !!!

        default:
            if ( LA() == Sql::T_IDENTIFIER && LA(2) == Sql::T_COLON ) {
                parseLabelStatemnt(entry);
                isNotLabel = false;
            }
            else
                error(_tokenIndex, "Unknown Block Entry Statement");
            break;
    }

    if ( isNotLabel )
        if ( skipUntil(Sql::T_SEMICOLON) )
            consumeToken();

    BlockStatementEntryListAST **list_ptr = &node;
    if ( entry ) {
        *list_ptr = new (_pool) BlockStatementEntryListAST;
        (*list_ptr)->value = entry;
        list_ptr = &(*list_ptr)->next;
    }

    return parseBlockEntryList(*list_ptr, isFuncMode);
}

bool Parser::parseBlockSelectStatement( BlockStatementEntryAST *&node )
{
    BlockSelectStatementAST *ast = new (_pool) BlockSelectStatementAST;
    ast->select_token = consumeToken();

    if ( LA1() == T_3_ALL || LA1() == T_8_DISTINCT )
        ast->select_spec_token = consumeToken();

    if ( parseResultColumnList(ast->columnList) ) {
        if ( LA1() == T_4_INTO ) {
            ast->into_token = consumeToken();
            if ( parseIntoIdentifierList(ast->into_list) ) {
                if ( LA1() == T_4_FROM ) {
                    ast->from_token = consumeToken();
                    parseJoinSource(ast->joinSource);
                    if ( LA1() == T_5_WHERE ) {
                        ast->where_token = consumeToken();
                        parseConditionExpression(ast->whereExpr);
                    }
                    if ( LA1() == T_5_GROUP ) {
                        parseGroupByBlock(ast->groupBy);
                    }
                }
                else {
                    error(_tokenIndex, "Expected Token `FROM'");
                }
            }
        }
        else
            error(_tokenIndex, "Expected Token `INTO'");
    }

    node = ast;
    return true;
}

bool Parser::parseBlockInsertStatement( BlockStatementEntryAST *&node )
{
    BlockInsertStatementAST *ast = new (_pool) BlockInsertStatementAST;
    parseInsertStatement(ast->insertStatement);
    ast->insertStatement->setBlockMode();
    node = ast;
    return true;
}

bool Parser::parseBlockUpdateStatement( BlockStatementEntryAST *&node )
{
    BlockUpdateStatementAST *ast = new (_pool) BlockUpdateStatementAST;
    parseUpdateStatement(ast->updateStatement);
    ast->updateStatement->setBlockMode();
    node = ast;
    return true;
}

bool Parser::parseBlockDeleteStatement (BlockStatementEntryAST *&node )
{
    BlockDeleteStatementAST *ast = new (_pool) BlockDeleteStatementAST;
    parseDeleteStatement(ast->deleteStatement);
    ast->deleteStatement->setBlockMode();
    node = ast;
    return true;
}

bool Parser::parseIntoIdentifierList( IdentifierNameListAST *&node )
{
    if ( LA1() || LA() == Sql::T_EOF_SYMBOL || LA() == Sql::T_SEMICOLON )
        return true;

    if ( LA() == Sql::T_COMMA ) {
        warning(_tokenIndex, "Empty Identifier Specified");
        consumeToken();
        return parseIntoIdentifierList(node);
    }

    IdentifierNameAST *ident = 0;
    if ( LA() == Sql::T_COLON && LA(2) == Sql::T_IDENTIFIER ) {
        ident = new (_pool) IdentifierNameAST;
        ident->colon_token = consumeToken();
        const Sql::Token &tk = tok();
        ident->name = tk.identifier;
        ident->name_token = consumeToken();
        if ( LA() == Sql::T_COMMA )
            consumeToken();
        else if ( LA1() != T_4_FROM )
            error(_tokenIndex, "Expected Token `FROM'");
    }
    else {
        error(_tokenIndex, "Identifier Not Specified");
        return true;
    }

    IdentifierNameListAST **list_ptr = &node;
    if ( ident ) {
        *list_ptr = new (_pool) IdentifierNameListAST;
        (*list_ptr)->value = ident;
        list_ptr = &(*list_ptr)->next;
    }

    return parseIntoIdentifierList(*list_ptr);
}

bool Parser::parseBlockSetStatemnt( BlockStatementEntryAST *&node )
{
    BlockSetStatementAST *ast = new (_pool) BlockSetStatementAST;
    ast->set_token = consumeToken();

    if ( LA() == Sql::T_COLON && LA(2) == Sql::T_IDENTIFIER ) {
        ast->ident = new (_pool) IdentifierNameAST;
        ast->ident->colon_token = consumeToken();
        const Sql::Token &tk = tok();
        ast->ident->name = tk.identifier;
        ast->ident->name_token = consumeToken();
        if ( LA() == Sql::T_EQUAL ) {
            ast->equal_token = consumeToken();
            parseValueExpresion(ast->assignExpr);
        }
        else
            error(_tokenIndex, "Expected token `='");
    }
    else
        error(_tokenIndex, "Identifier Not Specified");

    node = ast;
    return true;
}

bool Parser::parseBlockCommitStatemnt( BlockStatementEntryAST *&node )
{
    BlockCommitStatmentAST *ast = new (_pool) BlockCommitStatmentAST;
    ast->commit_token = consumeToken();
    node = ast;
    return true;
}

bool Parser::parseBlockRollbackStatemnt(BlockStatementEntryAST *&node)
{
    BlockRollbackStatmentAST *ast = new (_pool) BlockRollbackStatmentAST;
    ast->rollback_token = consumeToken();
    node = ast;
    return true;
}

bool Parser::parseBlockCallStatement( BlockStatementEntryAST *&node )
{
    BlockCallStatementAST *ast = new (_pool) BlockCallStatementAST;
    parseCallStatement(ast->callExpr);
    node = ast;
    return true;
}

bool Parser::parseIfStatemnt( BlockStatementEntryAST *&node , bool isFuncMode )
{
    IfStatementAST *ast = new (_pool) IfStatementAST;
    bool endif_found = false;
    ast->if_token = consumeToken();
    if ( parseConditionExpression(ast->conditionExpr) ) {
        if ( LA1() == T_4_THEN ) {
            ast->then_token = consumeToken();
            if ( parseBlockEntryList(ast->then_entry_list, isFuncMode) ) {
                if ( LA1() == T_4_ELSE ) {
                    ast->else_token = consumeToken();
                    parseBlockEntryList(ast->else_entry_list, isFuncMode);
                }
                if ( LA1() == T_3_END && LA1(2) == T_2_IF ) {
                    ast->end_token = consumeToken();
                    ast->end_if_token = consumeToken();
                    endif_found = true;
                    if ( LA() != Sql::T_SEMICOLON )
                        warning(_tokenIndex, "Expected Token `;'");
                }
                else
                    error(_tokenIndex, "Expected Tokens `END' `IF'");
            }
        }
        else {
            error(_tokenIndex, "Expected Token `THEN'");
        }
    }

    if ( !endif_found ) {
        while ( LA() ) {
            if ( LA1() == T_3_END && LA1(2) == T_2_IF ) {
                ast->end_token = consumeToken();
                ast->end_if_token = consumeToken();
                break;
            }
            consumeToken();
        }
    }

    node = ast;
    return true;
}

bool Parser::parseLoopStatement( BlockStatementEntryAST *&node , bool isFuncMode )
{
    LoopStatementAST *ast = new (_pool) LoopStatementAST;
    bool loopend_found = false;

    if ( LA1() == T_5_WHILE ) {
        ast->while_token = consumeToken();
        parseConditionExpression(ast->whileExpr);
    }

    if ( LA1() == T_4_LOOP ) {
        ast->loop_token = consumeToken();
        parseBlockEntryList(ast->entry_list, isFuncMode);
        if ( LA1() == T_3_END && LA1(2) == T_4_LOOP ) {
            ast->end_token = consumeToken();
            ast->end_loop_token = consumeToken();
            loopend_found = true;
            if ( LA() != Sql::T_SEMICOLON )
                warning(_tokenIndex, "Expected Token `;'");
        }
        else {
            error(_tokenIndex, "Expected Tokens `END' `LOOP'");
            while ( LA() ) {
                if ( LA1() == T_3_END && LA1(2) == T_4_LOOP ) {
                    ast->end_token = consumeToken();
                    ast->end_loop_token = consumeToken();
                    break;
                }
                consumeToken();
            }
        }
    }
    else
        error(_tokenIndex, "Expectes Token `LOOP'");

    node = ast;
    return true;
}

bool Parser::parseDeclareCursor( DeclareCursorClauseAST *&node )
{
    DeclareCursorClauseAST *ast = new (_pool) DeclareCursorClauseAST;
    if ( (LA1() == T_4_READ && LA1(2) == T_4_ONLY) ||
         (LA1() == T_6_UPDATE && LA1(2) == T_4_ONLY) ) {
        ast->read_or_update_token = consumeToken();
        ast->only_token = consumeToken();
    }
    if ( LA1() == T_5_TABLE )
        ast->table_token = consumeToken();
    if ( LA1() == T_6_CURSOR ) {
        ast->cursor_token = consumeToken();
        if ( LA() == Sql::T_IDENTIFIER ) {
            const Sql::Token &tk = tok();
            ast->cursorName = new (_pool) CursorNameAST;
            ast->cursorName->name = tk.identifier;
            ast->cursorName->name_token = consumeToken();
            if ( LA1() == T_3_FOR )
                ast->for_token = consumeToken();
            else
                error(_tokenIndex, "Expected Token `FOR'");
        }
        else
            error(_tokenIndex, "Expected Cursor Name");
    }
    else
        error(_tokenIndex, "Expected Token `CURSOR'");

    node = ast;
    return true;
}

bool Parser::parseForStatement(BlockStatementEntryAST *&node , bool isFuncMode )
{
    ForStatementAST *ast = new (_pool) ForStatementAST;
    bool endfor_found = false;

    ast->for_token = consumeToken();
    if ( LA() == Sql::T_COLON ) {
        consumeToken();
        if ( LA() == Sql::T_IDENTIFIER ) {
            const Sql::Token &tk = tok();
            ast->forName = new (_pool) IdentifierNameAST;
            ast->forName->name = tk.identifier;
            ast->forName->name_token = consumeToken();
            if ( LA1() == T_2_AS ) {
                ast->as_token = consumeToken();
                if ( LA1() == T_4_EACH && LA1(2) == T_3_ROW && LA1(3) == T_2_OF ) {
                    ast->each_token = consumeToken();
                    ast->row_token = consumeToken();
                    ast->of_token = consumeToken();
                }
                if ( (LA1() == T_4_READ && LA1(2) == T_4_ONLY) ||
                     (LA1() == T_6_UPDATE && LA1(2) == T_4_ONLY) ||
                     LA1() == T_5_TABLE || LA1() == T_6_CURSOR )
                    parseDeclareCursor(ast->cursor);
                if ( LA1() == T_6_SELECT ) {
                    if ( parseSelectStatement(ast->selectStatement) ) {
                        if ( LA1() == T_2_DO ) {
                            ast->do_token = consumeToken();
                            parseBlockEntryList(ast->entry_list, isFuncMode);
                            if ( LA1() == T_3_END && LA1(2) == T_3_FOR ) {
                                ast->end_token = consumeToken();
                                ast->end_for_token = consumeToken();
                                endfor_found = true;
                                if ( LA() != Sql::T_SEMICOLON )
                                    warning(_tokenIndex, "Expected Token `;'");
                            }
                        }
                        else
                            error(_tokenIndex, "Expected Token `DO'");
                    }
                }
                else
                    error(_tokenIndex, "Expected Token `SELECT'");
            }
            else
                error(_tokenIndex, "Expected Token `AS'");
        }
        else
            error(_tokenIndex, "Ecpected FOR-Variable Name");
    }
    else
        error(_tokenIndex, "Expected Token `:'");

    if ( !endfor_found ) {
        while ( LA() ) {
            if ( LA1() == T_3_END && LA1(2) == T_3_FOR ) {
                ast->end_token = consumeToken();
                ast->end_for_token = consumeToken();
                break;
            }
            consumeToken();
        }
    }

    node = ast;
    return true;
}

bool Parser::parseLabelStatemnt( BlockStatementEntryAST *&node )
{
    BlockLabelAST *ast = new (_pool) BlockLabelAST;
    ast->labelName = new (_pool) LabelNameAST;
    const Sql::Token &tk = tok();
    ast->labelName->name = tk.identifier;
    ast->labelName->name_token = consumeToken();
    ast->colon_token = consumeToken();
    node = ast;
    return true;
}

bool Parser::parseLeaveStatemnt( BlockStatementEntryAST *&node )
{
    LeaveStatementAST *ast = new (_pool) LeaveStatementAST;
    ast->leave_token = consumeToken();
    if ( LA() == Sql::T_IDENTIFIER ) {
        const Sql::Token &tk = tok();
        ast->labelName = new (_pool) LabelNameAST;
        ast->labelName->name = tk.identifier;
        ast->labelName->name_token = consumeToken();
        if ( LA() != Sql::T_SEMICOLON )
            warning(_tokenIndex, "Expected Token `;'");
    }
    else
        error(_tokenIndex, "Expected Label Name");

    node = ast;
    return true;
}

bool Parser::parseRoutine( RoutineAST *&node )
{
    RoutineAST *ast = new (_pool) RoutineAST;
    bool isProcMode = ( LA1() == T_9_PROCEDURE );
    ast->routine_token = consumeToken();
    ast->setProcedureType(isProcMode);
    if ( LA() == Sql::T_IDENTIFIER ) {
        const Sql::Token &tk = tok();
        ast->routineName = new (_pool) RoutineNameAST;
        ast->routineName->name = tk.identifier;
        ast->routineName->name_token = consumeToken();
        if ( LA() == Sql::T_LPAREN ) {
            ast->lparen_token = consumeToken();
            if ( parseArgumentList(ast->arg_list) ) {
                if ( LA() == Sql::T_RPAREN ) {
                    ast->rparen_token = consumeToken();
                    if ( !isProcMode ) {
                        // Function Returns
                        if ( LA1() == T_6_RETURN || LA1() == T_7_RETURNS ) {
                            ast->return_token = consumeToken();
                            if ( LA3() )
                                parseTypeSpecifier(ast->returnType);
                            else
                                errorCursor("Expected type specifier");
                        }
                        else
                            error(_tokenIndex, "Expected RETURN|RETURNS Token");
                    }
                    if ( LA() == Sql::T_SEMICOLON ) {
                        consumeToken();
                        if ( LA1() == T_5_BEGIN ) {
                            parseBlockStatement(ast->block, !isProcMode);
                        }
                        else
                            error(_tokenIndex, "Expected Token `BEGIN'");
                    }
                    else
                        error(_tokenIndex, "Expected Token `;'");
                }
                else
                    error(_tokenIndex, "Expected Token `)'");
            }
        }
        else
            error(_tokenIndex, "Expected Token `'(");
    }
    else
        error(_tokenIndex, "Routine Name Expeceted");
    node = ast;
    return true;
}

bool Parser::parseModuleRoutineList( RoutineListAST *&node )
{
    if ( LA1() == T_3_END && LA1(2) == T_6_MODULE )
        return true;

    if ( LA1() == T_8_FUNCTION || LA1() == T_9_PROCEDURE ) {
        RoutineAST *route = 0;
        parseRoutine(route);
        if ( route ) {
            route->skip(this);
            RoutineListAST **routes_ptr = &node;
            *routes_ptr = new (_pool) RoutineListAST;
            (*routes_ptr)->value = route;
            routes_ptr = &(*routes_ptr)->next;
            return parseModuleRoutineList(*routes_ptr);
        }
        return false;
    }

    error(_tokenIndex, "Invalid Module Syntax: Expected FUNCTION|PROCEDURE|END MODULE Tokens");
    return false;
}

bool Parser::parseArgumentList( ArgumentListAST *&node )
{
    if ( LA() == Sql::T_RPAREN || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
        return true;

    if ( LA() == Sql::T_COMMA ) {
        warningCursor("Expected Argument Definition");
        consumeToken();
        return parseArgumentList(node);
    }

    ArgumentListAST **arg_ptr = &node;

    ArgumentAST *arg = new (_pool) ArgumentAST;
    switch ( LA1() ) {
        case T_2_IN:
        case T_3_OUT:
        case T_5_INOUT:
            arg->inout_spec_token = consumeToken();
            break;
    }

    if ( LA() == Sql::T_COLON ) {
        if ( LA(2) == Sql::T_IDENTIFIER ) {
            arg->ident = new (_pool) IdentifierNameAST;
            arg->ident->colon_token = consumeToken();
            const Sql::Token &tk = tok();
            arg->ident->name = tk.identifier;
            arg->ident->name_token = consumeToken();
            if ( LA3() )
                parseTypeSpecifier(arg->identType);
            else
                error(_tokenIndex, "Expected Argument Type");
            *arg_ptr = new (_pool) ArgumentListAST;
            (*arg_ptr)->value = arg;
            arg_ptr = &(*arg_ptr)->next;
            if ( LA() == Sql::T_COMMA )
                consumeToken();
            return parseArgumentList(*arg_ptr);
        }
        else
            error(_tokenIndex, "Argument Name Expected");
    }
    else
        error(_tokenIndex, "Expected Token `:'");

    return false;
}

bool Parser::parseValueExpresion( ValueExpressionAST *&node, bool conditionMode )
{
    ValueExpressionAST *ast = 0;
    QList<ValueExpressionAST *>items;
    bool result = parseValueExpresionInternal(ast, items, conditionMode);
    if ( ast ) {
        if ( items.size() == 0 )
            warning(_tokenIndex, "Expected value-expression");
        ast->last_token = cursor();
    }
    node = ast;
    return result;
}

bool Parser::parseValueExpresionInternal( ValueExpressionAST *&node, QList<ValueExpressionAST *> &items, bool conditionMode )
{
    if ( LA1() == T_4_NULL || (LA1() == T_3_NOT && LA1(2) == T_4_NULL) ) {
        if ( items.size() == 0 ) {
            NullValueExpressionAST *ast = new (_pool) NullValueExpressionAST;
            if ( LA1() == T_3_NOT )
                ast->not_token = consumeToken();
            ast->null_token = consumeToken();
            node = ast;
            items.append(ast);
            return parseValueExpresionInternal(node, items, conditionMode);
        }
        error(_tokenIndex, "Invalid value-expression (NULL)");
        return false;
    }

    if ( isKeywordOnly() || LA() == Sql::T_SEMICOLON || LA() == Sql::T_COMMA ||
         LA() == Sql::T_RPAREN || LA() == Sql::T_EOF_SYMBOL )
        return true;

    if ( items.size() == 0 ) {

        if ( LA1() == T_4_CASE ) {
            if ( LA1(2) == T_4_WHEN ) {
                CaseConditionAST *ast;
                if ( parseCaseCondition(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            }
            else {
                CaseExpressionAST *ast;
                if ( parseCaseExpression(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            }
        }

        if ( LA2() /** TODO: Specially parsing builtin SQL-functions*/
             && LA(2) == Sql::T_LPAREN ) {
            if ( LA2() == T_4_CAST ) {
                CastFunctionAST *ast;
                if ( parseCastExpression(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            } else if ( LA2() == T_9_SUBSTRING ) {
                SubstringFunctionAST *ast;
                if ( parseSubstringExpression(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            } else if ( LA2() == T_5_COUNT ) {
                CountFunctionAST *ast;
                if ( parseCountExpression(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            } else {
                FunctionExpressionAST *ast;
                if ( parseFunctionExpresion(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            }
        }

        else if ( isPatentialIdentifier() ) {
            if ( LA(2) == Sql::T_LPAREN ) {
                FunctionExpressionAST *ast;
                if ( parseFunctionExpresion(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            }
            else {
                ColumnExpressionAST *ast;
                if ( parseColumnExpression(ast) ) {
                    node = ast;
                    items.append(ast);
                }
            }
        }

        else {
            switch ( LA() ) {
                case Sql::T_LPAREN: {
                    CompoundValueExpressionAST *ast;
                    if ( parseCompoundValueExpresion(ast) ) {
                        node = ast;
                        items.append(ast);
                    }
                    break;
                }
                case Sql::T_PLUS:
                case Sql::T_MINUS: {
                    UnaryValueExpressionAST *ast;
                    if ( parseUnaryValueExpression(ast) ) {
                        node = ast;
                        items.append(ast);
                    }
                    break;
                }
                case Sql::T_COLON: {
                    if ( _blockMode ) {
                        if ( LA(2) == Sql::T_IDENTIFIER && LA(3) == Sql::T_DOT && LA(4) == Sql::T_IDENTIFIER ) {
                            IdentDotColumnExpressionAST *ast;
                            parseIdentDotColumnExpression(ast);
                            node = ast;
                            items.append(ast);
                        }
                        else {
                            IdentifierExpressionAST *ast;
                            if ( parseIdentifierExpression(ast) ) {
                                node = ast;
                                items.append(ast);
                            }
                        }
                    }
                    else {
                        ParameterExpressionAST *ast;
                        if ( parseParameterExpression(ast) ) {
                            node = ast;
                            items.append(ast);
                        }
                    }
                    break;
                }
                case Sql::T_NUMERIC_LITERAL: {
                    NumericExpressionAST *ast = new (_pool) NumericExpressionAST;
                    ast->first_token = cursor();
                    ast->numeric_token = consumeToken();
                    ast->last_token = ast->numeric_token;
                    node = ast;
                    items.append(ast);
                    break;
                }
                case Sql::T_CHAR_LITERAL: {
                    LiteralExpressionAST *ast = new (_pool) LiteralExpressionAST;
                    ast->first_token = cursor();
                    ast->liter_token = consumeToken();
                    ast->last_token = ast->liter_token;
                    node = ast;
                    items.append(ast);
                    break;
                }
            }
        }
    }

    else if ( items.size() == 1 ) {
        // Wait binary op
        switch ( LA() ) {
            case Sql::T_MINUS:
            case Sql::T_PLUS:
            case Sql::T_SLASH:
            case Sql::T_STAR:
            case Sql::T_PIPE_PIPE: {
                unsigned op_token = consumeToken();
                ValueExpressionAST *rvExpr;
                if ( parseValueExpresion(rvExpr) ) {
                    if ( !rvExpr )
                        warning(_tokenIndex, "Expected value-expression");
                    BinaryValueExpressionAST *ast = new (_pool) BinaryValueExpressionAST;
                    ast->lvExpr = items[0];
                    ast->op_token = op_token;
                    ast->rvExpr = rvExpr;
                    node = ast;
                    items.clear();
                    items.append(ast);
                }
                else
                    return false;
                break;
            }
            case Sql::T_LESS:
            case Sql::T_LESS_EQUAL:
            case Sql::T_LESS_GREATER:
            case Sql::T_GREATER:
            case Sql::T_GREATER_EQUAL:
            case Sql::T_EXCLAIM_EQUAL:
            case Sql::T_EQUAL:
                if ( conditionMode ) {
                    node = items[0];
                    return true;
                }
                // VVV break down
            default:
                warning(_tokenIndex, "Invalid value-expression");
                return false;
        }
    }

    if ( node )
        return parseValueExpresionInternal(node, items, conditionMode);

    return false;
}

bool Parser::parseValueExpresionList( ValueExpressionListAST *&node, bool eatRParen )
{
    ValueExpressionListAST **expr_list = &node;

    while (1) {
        unsigned old_cursor = cursor();

        if ( LA() == Sql::T_RPAREN ) {
            if ( eatRParen )
                return true;
            error(_tokenIndex, "Unexpected token `)'");
            consumeToken();
            continue;
        }
        if ( (LA1() && LA1() != T_4_NULL) || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
            return false;
        if ( LA() == Sql::T_COMMA ) {
            warning(_tokenIndex, "Expected value expression");
            consumeToken();
            continue;
        }

        ValueExpressionAST *expr;
        if ( !parseValueExpresion(expr) )
            return false;
        if ( !expr )
            warning(_tokenIndex, "Expected value expression");
        else {
            *expr_list = new (_pool) ValueExpressionListAST;
            (*expr_list)->value = expr;
            expr_list = &(*expr_list)->next;
        }

        if ( LA() == Sql::T_COMMA ) {
            consumeToken();
            continue;
        }

        if ( old_cursor == cursor() ) {
            error(_tokenIndex, "WTF! (parseValueExpresionList)");
            return false;
        }
    }

    return true;
}

bool Parser::parseCompoundValueExpresion( CompoundValueExpressionAST *&node )
{
    CompoundValueExpressionAST *ast = new (_pool) CompoundValueExpressionAST;
    ast->lparen_token = consumeToken();
    ast->first_token = ast->lparen_token;
    {
        ValueExpressionAST *subExpr = 0;
        if ( !parseValueExpresion(subExpr) )
            return false;
        if ( !subExpr )
            warning(_tokenIndex, "Expected value-expression");
        ast->valExpr = subExpr;
    }
    ast->last_token = cursor();
    match(Sql::T_RPAREN, &ast->rparen_token);
    node = ast;
    return true;
}

bool Parser::parseUnaryValueExpression( UnaryValueExpressionAST *&node )
{
    UnaryValueExpressionAST *ast = new (_pool) UnaryValueExpressionAST;
    ast->op_token = consumeToken();
    ast->first_token = ast->op_token;
    {
        ValueExpressionAST *rvExpr;
        if ( !parseValueExpresion(rvExpr) )
            return false;
        if ( rvExpr ) {
            ast->valExpr = rvExpr;
            ast->last_token = rvExpr->last_token;
        }
        else {
            ast->last_token = ast->op_token;
            warning(_tokenIndex, "Expected value-expression");
        }
    }
    node = ast;
    return true;
}

bool Parser::parseParameterExpression( ParameterExpressionAST *&node )
{
    ParameterExpressionAST *ast = new (_pool) ParameterExpressionAST;
    ast->first_token = consumeToken();
    if ( isPatentialIdentifier() ) {
        ParamNameAST *param = new (_pool) ParamNameAST;
        const Sql::Token &tk = tok();
        param->param_prefix_token = ast->first_token;
        param->name = tk.identifier;
        ast->last_token = consumeToken();
        param->name_token = ast->last_token;
        ast->binding = param;
    }
    else {
        warning(_tokenIndex, "Expected parameter identifier");
        ast->last_token = ast->first_token;
    }
    node = ast;
    return true;
}

bool Parser::parseIdentifierExpression( IdentifierExpressionAST *&node )
{
    IdentifierExpressionAST *ast = new (_pool) IdentifierExpressionAST;
    ast->first_token = consumeToken();
    if ( isPatentialIdentifier() ) {
        IdentifierNameAST *param = new (_pool) IdentifierNameAST;
        const Sql::Token &tk = tok();
        param->colon_token = ast->first_token;
        param->name = tk.identifier;
        ast->last_token = consumeToken();
        param->name_token = ast->last_token;
        ast->ident = param;
    }
    else {
        warning(_tokenIndex, "Identifier Not Specified");
        ast->last_token = ast->first_token;
    }
    node = ast;
    return true;
}

bool Parser::parseIdentDotColumnExpression( IdentDotColumnExpressionAST *&node )
{
    IdentDotColumnExpressionAST *ast = new (_pool) IdentDotColumnExpressionAST;
    ast->first_token = consumeToken();
    ast->ident = new (_pool) IdentifierNameAST;
    ast->ident->colon_token = ast->first_token;
    const Sql::Token &tk = tok();
    ast->ident->name = tk.identifier;
    ast->ident->name_token = consumeToken();
    ast->dot_token = consumeToken();
    const Sql::Token &tk1 = tok();
    ast->columnName = new (_pool) ColumnNameAST;
    ast->columnName->name = tk1.identifier;
    ast->last_token = ast->columnName->name_token = consumeToken();
    node = ast;
    return true;
}

bool Parser::parseFunctionExpresion( FunctionExpressionAST *&node )
{
    FunctionExpressionAST *ast = new (_pool) FunctionExpressionAST;
    ValueExpressionListAST **arg_list = &ast->arg_list;

    const Sql::Token &tk = tok();
    RoutineNameAST *funcName = new (_pool) RoutineNameAST;
    funcName->name = tk.identifier;
    ast->first_token = consumeToken();
    funcName->name_token = ast->first_token;
    ast->functionName = funcName;
    ast->lparen_token = consumeToken();

    // Parameters list
    if ( LA() == Sql::T_STAR ) {
        consumeToken();
    }
    else {
        while (1) {
            unsigned old_cursor = cursor();

            if ( LA() == Sql::T_RPAREN )
                break;
            if ( (LA1() && LA1() != T_4_NULL) || LA() == Sql::T_SEMICOLON || LA() == Sql::T_EOF_SYMBOL )
                return false;
            if ( LA() == Sql::T_COMMA ) {
                warning(_tokenIndex, "Function argument expected");
                consumeToken();
                continue;
            }

            ValueExpressionAST *arg;
            if ( !parseValueExpresion(arg) )
                return false;
            if ( arg) {
                *arg_list = new (_pool) ValueExpressionListAST;
                (*arg_list)->value = arg;
                arg_list = &(*arg_list)->next;
            }

            if ( LA() == Sql::T_COMMA ) {
                if ( !arg )
                    warning(_tokenIndex, "Function argument expected");
                consumeToken();
                continue;
            }

            if ( old_cursor == cursor() ) {
                error(_tokenIndex, "WTF! (parseFunctionExpresion)");
                return false;
            }
        }
    }

    ast->last_token = cursor();
    match(Sql::T_RPAREN, &ast->rparen_token);

    node = ast;
    return true;
}

bool Parser::parseCastExpression( CastFunctionAST *&node )
{
    CastFunctionAST *ast = new (_pool) CastFunctionAST;
    ast->first_token = cursor();

    ast->cast_token = consumeToken();
    if ( !match(Sql::T_LPAREN, &ast->lparen_token) )
        return false;

    ValueExpressionAST *expr;
    if ( !parseValueExpresion(expr) )
        return false;
    ast->castExpr = expr;

    if ( !match1(T_2_AS, &ast->as_token) )
        return false;

    if ( !LA3() ) {
        error(_tokenIndex, "Expected type specifier");
        return false;
    }
    if ( !parseTypeSpecifier(ast->typeSpec) )
        return false;

    if ( !match(Sql::T_RPAREN, &ast->rparen_token) )
        return false;

    ast->last_token = ast->rparen_token;
    node = ast;
    return true;
}

bool Parser::parseSubstringExpression( SubstringFunctionAST *&node )
{
    SubstringFunctionAST *ast = new (_pool) SubstringFunctionAST;
    ast->first_token = cursor();

    ast->substring_token = consumeToken();
    if ( !match(Sql::T_LPAREN, &ast->lparen_token) )
        return false;

    ValueExpressionAST *strExpr;
    if ( !parseValueExpresion(strExpr) )
        return false;
    ast->strExpr = strExpr;
    if ( LA() == Sql::T_RPAREN ) {
        ast->rparen_token = ast->last_token = consumeToken();
        node = ast;
        return true;
    }

    if ( !match1(T_4_FROM, &ast->from_token) )
        return false;

    ValueExpressionAST *startExpr;
    if ( !parseValueExpresion(startExpr) )
        return false;
    ast->startExpr = startExpr;
    if ( LA() == Sql::T_RPAREN ) {
        ast->rparen_token = ast->last_token = consumeToken();
        node = ast;
        return true;
    }

    if ( !match1(T_3_FOR, &ast->for_token) )
        return false;

    ValueExpressionAST *lengthExpr;
    if ( !parseValueExpresion(lengthExpr) )
        return false;
    ast->lengthExpr = lengthExpr;

    if ( !match(Sql::T_RPAREN, &ast->rparen_token) )
        return false;

    ast->last_token = ast->rparen_token;
    node = ast;
    return true;
}

bool Parser::parseCountExpression( CountFunctionAST *&node )
{
    CountFunctionAST *ast = new (_pool) CountFunctionAST;
    ast->first_token = cursor();

    ast->count_token = consumeToken();
    if ( !match(Sql::T_LPAREN, &ast->lparen_token) )
        return false;

    if ( LA1() == T_8_DISTINCT )
        ast->distinct_token = consumeToken();

    if ( LA() == Sql::T_STAR ) {
        consumeToken();
        if ( !match(Sql::T_RPAREN, &ast->rparen_token) )
            return false;
        node = ast;
        return true;
    }

    ValueExpressionAST *countExpr;
    if ( !parseValueExpresion(countExpr) )
        return false;
    ast->countExpr = countExpr;

    if ( !match(Sql::T_RPAREN, &ast->rparen_token) )
        return false;

    ast->last_token = ast->rparen_token;
    node = ast;
    return true;
}

bool Parser::parseTypeSpecifier( BaseTypeAST *&node )
{
    switch ( LA3() ) {
        case 0: return false;
        case T_4_CHAR:
        case T_5_NCHAR:
        case T_7_VARCHAR:
        case T_9_CHARACTER: {
            CharacterTypeAST *ast = new (_pool) CharacterTypeAST;
            ast->type_token = consumeToken();
            if ( LA() == Sql::T_LPAREN ) {
                ast->lparen_token = consumeToken();
                if ( LA() != Sql::T_NUMERIC_LITERAL ) {
                    error(_tokenIndex, "Expected NUMERIC token");
                    return false;
                }
                ast->size_token = consumeToken();
                if ( !match(Sql::T_RPAREN, &ast->rparen_token) )
                    return false;
            }
            node = ast;
            return true;
        }
        default: {
            SimpleTypeAST *ast = new (_pool) SimpleTypeAST;
            ast->type_token = consumeToken();
            node = ast;
            return true;
        }
    }

    return false;
}

bool Parser::parseColumnExpression( ColumnExpressionAST *&node )
{
    ColumnExpressionAST *ast = new (_pool) ColumnExpressionAST;
    ast->first_token = cursor();

    ColumnNameAST *column_name = new (_pool) ColumnNameAST;
    if ( LA(2) == Sql::T_DOT && isPatentialIdentifier(3) && LA(4) == Sql::T_DOT ) {
        SchemaNameAST *schemaName = new (_pool) SchemaNameAST;
        const Sql::Token &stk = tok();
        schemaName->name = stk.identifier;
        schemaName->name_token = consumeToken();
        consumeToken();
        TableNameAST *tableName = new (_pool) TableNameAST;
        const Sql::Token &ttk = tok();
        tableName->name = ttk.identifier;
        tableName->name_token = consumeToken();
        consumeToken();
        column_name->schemaName = schemaName;
        column_name->tableOfColumn = tableName;
    }
    else if ( LA(2) == Sql::T_DOT ) {
        TableAliasAST *alias = new (_pool) TableAliasAST;
        const Sql::Token &tk = tok();
        alias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
        alias->name_token = consumeToken();
        ast->last_token = consumeToken();
        column_name->tableOfColumn = alias;
    }
    if ( isPatentialIdentifier() ) {
        const Sql::Token &tk = tok();
        column_name->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
        ast->last_token = consumeToken();
        column_name->name_token = ast->last_token;
    }
    else {
        warning(_tokenIndex, "Expected identifier got `%s'", tok().spell());
    }
    ast->column = column_name;

    node = ast;
    return true;
}

bool Parser::parseCaseExpression( CaseExpressionAST *&node )
{
    CaseExpressionAST *ast = new (_pool) CaseExpressionAST;

    ast->first_token = ast->case_token = consumeToken();

    if ( !parseValueExpresion(ast->caseExpr) )
        return false;

    if ( LA1() != T_4_WHEN ) {
        errorCursor("Expected WHEN token");
        return false;
    }

    if ( !parseCaseExpressionClauseList(ast->var_list) )
        return false;

    if ( LA1() == T_4_ELSE ) {
        ast->else_token = consumeToken();
        if ( !parseValueExpresion(ast->elseExpr) )
            return false;
    }

    if ( LA1() != T_3_END ) {
        errorCursor("Expected END token");
        return false;
    }

    ast->last_token = ast->end_token = consumeToken();

    node = ast;
    return true;
}

bool Parser::parseCaseExpressionClauseList( CaseExpressionClauseListAST *&node )
{
    CaseExpressionClauseListAST **var_list = &node;

    while (1) {
        unsigned first_token = cursor();

        if ( LA1() == T_4_ELSE || LA1() == T_3_END ) {
            return true;
        }

        if ( LA() == Sql::T_EOF_SYMBOL || LA() == Sql::T_SEMICOLON ) {
            return true;
        }

        if ( LA1() == T_4_WHEN ) {
            CaseExpressionClauseAST *caseClauseItem = new (_pool) CaseExpressionClauseAST;
            caseClauseItem->when_token = consumeToken();
            if ( !parseValueExpresion(caseClauseItem->whenExpr) )
                return false;
            if ( LA1() != T_4_THEN ) {
                errorCursor("Expected THEN token");
                return false;
            }
            caseClauseItem->then_token = consumeToken();
            if ( !parseValueExpresion(caseClauseItem->thenExpr) )
                return false;
            *var_list = new (_pool) CaseExpressionClauseListAST;
            (*var_list)->value = caseClauseItem;
            var_list = &(*var_list)->next;
        }
        else {
            errorCursor("Unexpected token");
        }

        if ( first_token == cursor() ) {
            error(_tokenIndex, "WTF! (parseCaseExpressionClauseList)");
            return false;
        }
    }

    return true;
}

bool Parser::parseCaseCondition( CaseConditionAST *&node )
{
    CaseConditionAST *ast = new (_pool) CaseConditionAST;

    ast->first_token = ast->case_token = consumeToken();

    if ( LA1() != T_4_WHEN ) {
        errorCursor("Expected WHEN token");
        return false;
    }

    if ( !parseCaseConditionClauseList(ast->var_list) )
        return false;

    if ( LA1() == T_4_ELSE ) {
        ast->else_token = consumeToken();
        if ( !parseValueExpresion(ast->elseExpr) )
            return false;
    }

    if ( LA1() != T_3_END ) {
        errorCursor("Expected END token");
        return false;
    }

    ast->last_token = ast->end_token = consumeToken();

    node = ast;
    return true;
}

bool Parser::parseCaseConditionClauseList( CaseConditionClauseListAST *&node )
{
    CaseConditionClauseListAST **var_list = &node;

    while (1) {
        unsigned first_cursor = cursor();

        if ( LA1() == T_4_ELSE || LA1() == T_3_END ) {
            return true;
        }

        if ( LA() == Sql::T_EOF_SYMBOL || LA() == Sql::T_SEMICOLON ) {
            return true;
        }

        if ( LA1() == T_4_WHEN ) {
            CaseConditionClauseAST *caseClauseItem = new (_pool) CaseConditionClauseAST;
            caseClauseItem->when_token = consumeToken();
            if ( !parseConditionExpression(caseClauseItem->whenExpr) )
                return false;
            if ( LA1() != T_4_THEN ) {
                errorCursor("Expected THEN token");
                return false;
            }
            caseClauseItem->then_token = consumeToken();
            if ( !parseValueExpresion(caseClauseItem->thenExpr) )
                return false;
            *var_list = new (_pool) CaseConditionClauseListAST;
            (*var_list)->value = caseClauseItem;
            var_list = &(*var_list)->next;
        }
        else {
            errorCursor("Expected WHEN token");
            return false;
        }

        if ( first_cursor == cursor() ) {
            error(_tokenIndex, "WTF! (parseCaseConditionClauseList)");
            return false;
        }
    }

    return true;
}

bool Parser::parseConditionExpression(ConditionExpressionAST *&node)
{
    ConditionExpressionAST *ast = 0;
    QList<ConditionExpressionAST *>items;
    bool result = parseConditionExpressionInternal(ast, items);
    if ( ast ) {
        if ( items.size() == 0 )
            warning(_tokenIndex, "Expected condition expression");
        ast->last_token = cursor();
    }
    node = ast;
    return result;
}

bool Parser::parseConditionExpressionInternal( ConditionExpressionAST *&node, QList<ConditionExpressionAST *> &items )
{
    if ( LA() == Sql::T_SEMICOLON || LA() == Sql::T_COMMA || LA() == Sql::T_RPAREN || LA() == Sql::T_EOF_SYMBOL )
        return true;

    switch ( LA1() ) {
        case 0:
        case T_3_NOT:
        case T_2_IN:
        case T_3_AND:
        case T_3_ANY:
        case T_2_OR:
        case T_4_LIKE:
        case T_6_EXISTS:
            break;
        default:
            return true;
    }

    if ( items.size() == 0 ) {
        BaseExpressionAST *expr = 0;
        if ( !parseSimpleExpression(expr, true, true) )
            return false;
        ConditionExpressionAST *ast = (expr ? expr->asConditionExpression() : 0);
        if ( !ast )
            return false;
        node = ast;
        items.append(ast);
    }

    else if ( items.size() == 1 ) {\
        if ( LA1() == T_2_OR || LA1() == T_3_AND ) {
            unsigned op_token = consumeToken();
            if ( LA1() == T_3_NOT )
                consumeToken();
            ConditionExpressionAST *rvExpr;
            if ( parseConditionExpression(rvExpr) ) {
                if ( !rvExpr ) {
                    warning(_tokenIndex, "Expected condition-expression");
                    return false;
                }
                BinaryConditionExpressionAST *ast = new (_pool) BinaryConditionExpressionAST;
                ast->lvCondExpr = items[0];
                ast->cond_op_token = op_token;
                ast->rvCondExpr = rvExpr;
                node = ast;
                items.clear();
                items.append(ast);
            }
            else
                return false;
        }
        else
            return false;
    }

    if ( node )
        return parseConditionExpressionInternal(node, items);

    return false;
}

bool Parser::parseCompoundConditionExpresion( CompoundConditionExpressionAST *&node )
{
    CompoundConditionExpressionAST *ast = new (_pool) CompoundConditionExpressionAST;
    ast->lparen_token = consumeToken();
    ast->first_token = ast->lparen_token;
    {
        ConditionExpressionAST *subExpr = 0;
        if ( !parseConditionExpression(subExpr) )
            return false;
        if ( !subExpr )
            warning(_tokenIndex, "Expected condition-expression");
        ast->condExpr = subExpr;
    }
    ast->last_token = cursor();
    match(Sql::T_RPAREN, &ast->rparen_token);
    node = ast;
    return true;
}

bool Parser::parseExistsConditionExpression( ExistsConditionExpressionAST *&node )
{
    ExistsConditionExpressionAST *ast = new (_pool) ExistsConditionExpressionAST;
    ast->first_token = cursor();
    if ( LA1() == T_3_NOT )
        ast->not_token = consumeToken();
    ast->exists_token = consumeToken();
    ast->rvExpr = new (_pool) SubQueryExpressionAST;
    if ( !match(Sql::T_LPAREN, &ast->rvExpr->lparen_token) )
        return false;
    if ( !parseSelectStatement(ast->rvExpr->query) )
        return false;
    ast->last_token = cursor();
    if ( !match(Sql::T_RPAREN, &ast->rvExpr->rparen_roken) )
        return false;
    node = ast;
    return true;
}

bool Parser::parseInConditionExpression( InConditionExpressionAST *&node, ValueExpressionAST *lvExpr )
{
    InConditionExpressionAST *ast = new (_pool) InConditionExpressionAST;
    ast->first_token = cursor();
    if ( LA1() == T_3_NOT )
        ast->not_token = consumeToken();
    ast->in_token = consumeToken();
    ast->lvExpr = lvExpr;

    if ( LA() == Sql::T_LPAREN && LA1(2) != T_6_SELECT ) {
        ValueListExpressionAST *valList = new (_pool) ValueListExpressionAST;
        valList->lparen_token = consumeToken();
        if ( !parseValueExpresionList(valList->val_list, true) )
            return false;
        ast->last_token = cursor();
        if ( !match(Sql::T_RPAREN, &valList->rparen_roken) )
            return false;
        ast->rvExpr =  valList;
    }

    else {
        BaseExpressionAST *rExpr;
        if ( !parseSimpleExpression(rExpr) )
            return false;
        if ( !rExpr ) {
            error(_tokenIndex, "Expected expression");
            return false;
        }
        ValueExpressionAST *rvExpr = rExpr->asValueExpression();
        if ( !rvExpr )
            return false;
        ast->last_token = cursor();
        ValueListExpressionAST *valExpr = new (_pool) ValueListExpressionAST;
        ValueExpressionListAST **val_list = &valExpr->val_list;
        *val_list = new (_pool) ValueExpressionListAST;
        (*val_list)->value = rvExpr;
        val_list = &(*val_list)->next;
        ast->rvExpr = valExpr;
        node = ast;
    }

    node = ast;
    return true;
}

bool Parser::parseCommonConditionExpression( InConditionExpressionAST *&node, ValueExpressionAST *lvExpr )
{
    InConditionExpressionAST *ast = new (_pool) InConditionExpressionAST;
    ast->first_token = cursor();
    if ( LA1() == T_3_NOT )
        ast->not_token = consumeToken();
    ast->in_token = consumeToken();
    ast->lvExpr = lvExpr;

    BaseExpressionAST *rExpr;
    if ( !parseSimpleExpression(rExpr) )
        return false;
    if ( !rExpr ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    ValueExpressionAST *rvExpr = rExpr->asValueExpression();
    if ( !rvExpr )
        return false;
    ast->last_token = cursor();
    ValueListExpressionAST *valExpr = new (_pool) ValueListExpressionAST;
    ValueExpressionListAST **val_list = &valExpr->val_list;
    *val_list = new (_pool) ValueExpressionListAST;
    (*val_list)->value = rvExpr;
    val_list = &(*val_list)->next;
    ast->rvExpr = valExpr;

    node = ast;
    return true;
}

bool Parser::parseBetweenConditionExpression(BetweenConditionExpressionAST *&node, ValueExpressionAST *lvExpr)
{
    BetweenConditionExpressionAST *ast = new (_pool) BetweenConditionExpressionAST;
    ast->first_token = cursor();

    if ( LA1() == T_3_NOT )
        ast->not_token = consumeToken();
    ast->between_token = consumeToken();
    ast->lvExpr = lvExpr;

    BaseExpressionAST *r1Expr;
    if ( !parseSimpleExpression(r1Expr) ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    if ( !r1Expr ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    ast->rv1Expr = r1Expr->asValueExpression();
    if ( !ast->rv1Expr )
        return false;

    if ( !match1(T_3_AND, &ast->and_token) ) {
        error(_tokenIndex, "Expected token `AND'");
        return false;
    }

    BaseExpressionAST *r2Expr;
    if ( !parseSimpleExpression(r2Expr) ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    if ( !r2Expr ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    ast->rv2Expr = r2Expr->asValueExpression();
    if ( !ast->rv2Expr )
        return false;

    ast->last_token = cursor();
    node = ast;
    return true;
}

bool Parser::parseLikeConditionExpression(LikeConditionExpressionAST *&node, ValueExpressionAST *lvExpr)
{
    LikeConditionExpressionAST *ast = new (_pool) LikeConditionExpressionAST;
    ast->first_token = cursor();

    if ( LA1() == T_3_NOT )
        ast->not_token = consumeToken();
    ast->like_token = consumeToken();
    ast->lvExpr = lvExpr;

    BaseExpressionAST *rExpr;
    if ( !parseSimpleExpression(rExpr) ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    if ( !rExpr ) {
        error(_tokenIndex, "Expected expression");
        return false;
    }
    ast->rvExpr = rExpr->asValueExpression();
    if ( !ast->rvExpr )
        return false;

    node = ast;
    return true;
}

bool Parser::parseCompoundExpresion( BaseExpressionAST *&node, bool enableCondition )
{
    unsigned rparen_token;
    unsigned lparen_token = consumeToken();

    if ( LA1() == T_6_SELECT ) {
        SelectCompoundValueExpressionAST *ast = new (_pool) SelectCompoundValueExpressionAST;
        ast->lparen_token = lparen_token;
        if ( !parseSelectStatement(ast->query) )
            return false;
        if ( !match(Sql::T_RPAREN, &ast->rparen_token) )
            return false;
        node = ast;
        return true;
    }

    BaseExpressionAST *sExpr = 0;
    if ( !parseSimpleExpression(sExpr, enableCondition) )
        return false;
    if ( !sExpr ) {
        error(_tokenIndex, "Expected expresion (parseCompoundExpresion->parseSimpleExpression)");
        return false;
    }

    if ( LA() == Sql::T_RPAREN ) {
        rparen_token = consumeToken();
        if ( ConditionExpressionAST *scExpr = sExpr->asConditionExpression() ) {
            CompoundConditionExpressionAST *ast = new (_pool) CompoundConditionExpressionAST;
            ast->lparen_token = lparen_token;
            ast->condExpr = scExpr;
            ast->rparen_token = rparen_token;
            node = ast;
            return true;
        }
        if ( ValueExpressionAST *svExpr = sExpr->asValueExpression() ) {
            CompoundValueExpressionAST *ast = new (_pool) CompoundValueExpressionAST;
            ast->lparen_token = lparen_token;
            ast->valExpr = svExpr;
            ast->rparen_token = rparen_token;
            node = ast;
            return true;
        }
        error(_tokenIndex, "WTF !!!!!!!!!!!!!! parseCompoundExpresion");
        return false;
    }

    if ( !enableCondition ) {
        error(_tokenIndex, "Expected token `)'");
        return false;
    }

    ConditionExpressionAST *lvExpr = sExpr->asConditionExpression();
    if ( lvExpr && (LA1() == T_3_AND || LA1() == T_2_OR) ) {
        QList <ConditionExpressionAST *>items;
        items.append(lvExpr);
        if ( !parseConditionExpressionInternal(lvExpr, items) )
            return false;
        if ( !match(Sql::T_RPAREN, &rparen_token) )
            return false;
        CompoundConditionExpressionAST *ast = new (_pool) CompoundConditionExpressionAST;
        ast->lparen_token = lparen_token;
        ast->condExpr = lvExpr;
        ast->rparen_token = rparen_token;
        node = ast;
        return true;
    }

    error(_tokenIndex, "Invalid compound expression");
    return false;
}

bool Parser::parseSimpleExpression( BaseExpressionAST *&node, bool enableCondition, bool needCondition )
{
    if ( enableCondition ) {
        if ( (LA1() == T_6_EXISTS) || (LA1() == T_3_NOT && LA1(2) == T_6_EXISTS) ) {
            ExistsConditionExpressionAST *existsExpr;
            if ( !parseExistsConditionExpression(existsExpr) )
                return false;
            if ( LA1() == T_3_NOT )
                existsExpr->not_token = consumeToken();
            node = existsExpr;
            return true;
        }
    }

    ValueExpressionAST *lvExpr = 0;

    if ( LA() == Sql::T_LPAREN ) {
        BaseExpressionAST *cExpr = 0;
        if ( !parseCompoundExpresion(cExpr , enableCondition) )
            return false;
        if ( !cExpr ) {
            error(_tokenIndex, "Expected expression (parseSimpleExpression->parseCompoundExpresion)");
            return false;
        }
        if ( !enableCondition ) {
            ValueExpressionAST *cvExpr = cExpr->asValueExpression();
            if ( cvExpr ) {
                node = cvExpr;
                return true;
            }
            return false;
        }
        ConditionExpressionAST *ccExpr = cExpr->asConditionExpression();
        if ( ccExpr ) {
            node = ccExpr;
            return true;
        }
        lvExpr = cExpr->asValueExpression();
        if ( !lvExpr )
            return false;
    }

    if ( !lvExpr )
        if ( !parseValueExpresion(lvExpr, enableCondition) )
            return false;

    if ( !enableCondition ) {
        node = lvExpr;
        return true;
    }

    if ( (LA1() == T_3_NOT && LA1(2) == T_2_IN) || LA1() == T_2_IN ) {
        InConditionExpressionAST *inExpr;
        if ( !parseInConditionExpression(inExpr, lvExpr) )
            return false;
        node = inExpr;
        return true;
    }

    if ( (LA1() == T_3_NOT && LA1(2) == T_4_LIKE) || LA1() == T_4_LIKE ) {
        LikeConditionExpressionAST *likeExpr;
        if ( !parseLikeConditionExpression(likeExpr, lvExpr) )
            return false;
        node = likeExpr;
        return true;
    }

    if ( (LA1() == T_3_NOT && LA1(2) == T_7_BETWEEN) || LA1() == T_7_BETWEEN ) {
        BetweenConditionExpressionAST *betweenExpr;
        if ( !parseBetweenConditionExpression(betweenExpr, lvExpr) )
            return false;
        node = betweenExpr;
        return true;
    }

    if ( (LA1() == T_3_NOT && LA1(2) == T_3_ANY) || LA1() == T_3_ANY ||
         LA() == Sql::T_LESS || LA() == Sql::T_LESS_EQUAL || LA() == Sql::T_LESS_GREATER || LA() == Sql::T_EQUAL ||
         LA() == Sql::T_GREATER || LA() == Sql::T_GREATER_EQUAL || LA() == Sql::T_EXCLAIM_EQUAL ) {
        InConditionExpressionAST *inExpr;
        if ( !parseCommonConditionExpression(inExpr, lvExpr) )
            return false;
        node = inExpr;
        return true;
    }

    if ( LA1() == T_2_IS ) {
        UnaryConditionExpressionAST *ast = new (_pool) UnaryConditionExpressionAST;
        ast->lvExpr = lvExpr;
        ast->last_op_token = consumeToken();
        if ( LA1() == T_3_NOT )
            ast->last_op_token = consumeToken();
        if ( LA1() == T_4_NULL )
            ast->last_op_token = consumeToken();
        else
            warning(_tokenIndex, "Expected `NULL'");
        ast->last_token = ast->last_op_token;
        node = ast;
        return true;
    }

    if ( needCondition ) {
        error(_tokenIndex, "Expected condition");
        return false;
    }

    node = lvExpr;
    return true;
}

bool Parser::parseJoinItemList( JoinItemListAST *&node )
{
    bool result = true;
    JoinItemListAST **join_list = &node;

    bool isComma;
    JoinOpAST *join_op;
    while ( checkIsJoinOp(join_op, isComma) ) {
        JoinItemAST *join_source;

        if ( isPatentialIdentifier() ) {
            join_source = new (_pool) JoinItemAST;
            join_source->joinOp = join_op;
            DirectSingleSourceAST *source = new (_pool) DirectSingleSourceAST;
            source->schemaTableName = new (_pool) SchemaTableNameAST;
            source->schemaTableName->parse(this, _pool);
            bool needAlias = false;
            if ( LA1() == T_2_AS ) {
                source->as_token = consumeToken();
                needAlias = true;
            }
            if ( isPatentialIdentifier() ) {
                const Sql::Token &tk1 = tok();
                source->tableAlias = new (_pool) TableAliasAST;
                source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
                source->tableAlias->name_token = consumeToken();
                source->tableAlias->schemaTableName = source->schemaTableName;
            }
            else if ( needAlias )
                error(_tokenIndex, "Expected table source alias");
            join_source->singleSource = source;
        }
        else if ( LA() == Sql::T_LPAREN && LA1(2) == T_6_SELECT ) {
            join_source = new (_pool) JoinItemAST;
            join_source->joinOp = join_op;
            SubQuerySingleSourceAST *source = new (_pool) SubQuerySingleSourceAST;
            parseSubQuery(source->subQuery);
            bool needAlias = false;
            if ( LA1() == T_2_AS ) {
                source->as_token = consumeToken();
                needAlias = true;
            }
            if ( isPatentialIdentifier() ) {
                const Sql::Token &tk1 = tok();
                source->tableAlias = new (_pool) TableAliasAST;
                source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
                source->tableAlias->name_token = consumeToken();
                source->tableAlias->subqueryAliasType = source->subQuery /*->clone(_pool)*/;
            }
            else if ( needAlias )
                error(_tokenIndex, "Expected subquery alias");
            join_source->singleSource = source;
        }
        else {
            result = false;
            error(_tokenIndex, "Expected table source join item");
            break;
        }

        if ( !isComma ) {
            // Join Constraint
            if ( LA1() == T_2_ON ) {
                join_source->joinConstraint = new (_pool) JoinConstraintAST;
                join_source->joinConstraint->on_token = consumeToken();
                parseConditionExpression(join_source->joinConstraint->expr);
            }
        }
        *join_list = new (_pool) JoinItemListAST;
        (*join_list)->value = join_source;
        join_list = &(*join_list)->next;
    }

    return result;
}

void Parser::rewind(unsigned cursor)
{
    if (cursor < _translationUnit->tokenCount())
        _tokenIndex = cursor;
    else
        _tokenIndex = _translationUnit->tokenCount() - 1;
}

bool Parser::isPatentialIdentifier( int n ) const
{
    if ( LA(n) == Sql::T_IDENTIFIER )
        return true;

    switch ( LA1(n) ) {
        case T_8_INTERVAL:
        case T_5_VALUE:
        case T_4_DATA:
        case T_4_DATE:
        case T_4_TIME:
            return true;
    }

    return false;
}

bool Parser::isKeywordOnly(int n) const
{
    switch ( LA1(n) ) {
        case 0:
        case T_8_INTERVAL:
        case T_5_VALUE:
        case T_4_DATA:
        case T_4_DATE:
        case T_4_TIME:
        case T_4_CASE:
            return false;
        default:
            return true;
    }

    return false;
}

void Parser::warning(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(Sql::DiagnosticClient::Warning, index, format, ap);
    va_end(ap);
    va_end(args);
}

void Parser::error(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(Sql::DiagnosticClient::Error, index, format, ap);
    va_end(ap);
    va_end(args);
}

void Parser::fatal(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(Sql::DiagnosticClient::Fatal, index, format, ap);
    va_end(ap);
    va_end(args);
}

void Parser::errorCursor( const char *message )
{
    error(_tokenIndex, message);
}

void Parser::warningCursor( const char *message )
{
    warning(_tokenIndex, message);
}

