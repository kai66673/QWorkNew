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

#include "Parser.h"
#include "SqlToken.h"
#include "SqlLexer.h"
#include "Control.h"
#include "AST.h"
#include "ASTVisitor.h"
#include "sqlchecknamesvisitor.h"
#include "SqlStatementsBindVisitor.h"
#include "SqlCompletionAssistVisitor.h"
#include "SqlLinkFinder.h"

// #include "Literals.h"
// #include "ObjectiveCTypeQualifiers.h"
// #include "QtContextKeywords.h"
#include <string>
#include <cstdio> // for putchar

#ifdef _MSC_VER
#    define va_copy(dst, src) ((dst) = (src))
#elif defined(__INTEL_COMPILER) && !defined(va_copy)
#    define va_copy __va_copy
#endif

#define CPLUSPLUS_NO_DEBUG_RULE
#define MAX_EXPRESSION_DEPTH 100

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
using namespace Sql;

namespace {

class DebugRule {
    const char *name;
    static int depth;

public:
    DebugRule(const char *name, const char *spell, unsigned idx, bool blocked)
        : name(name)
    {
        for (int i = 0; i <= depth; ++i)
          fputc('-', stderr);

        ++depth;
        fprintf(stderr, " %s, ahead: '%s' (%d) - block-errors: %d\n", name, spell, idx, blocked);
    }

    ~DebugRule()
    { --depth; }
};

int DebugRule::depth = 0;

namespace Prec {
enum {
    Unknown         = 0,
    Comma           = 1,
    Assignment      = 2,
    Conditional     = 3,
    LogicalOr       = 4,
    LogicalAnd      = 5,
    InclusiveOr     = 6,
    ExclusiveOr     = 7,
    And             = 8,
    Equality        = 9,
    Relational      = 10,
    Shift           = 11,
    Additive        = 12,
    Multiplicative  = 13,
    PointerToMember = 14
};
} // namespace Precedece

//inline int precedence(int tokenKind, bool templateArguments)
//{
//    // ### this will/might need some tuning for C++0x
//    // (see: [temp.names]p3)
//    if (templateArguments && tokenKind == T_GREATER)
//        return -1;

//    if (lookAtAssignmentOperator(tokenKind))
//        return Prec::Assignment;

//    switch (tokenKind) {
//    case T_COMMA:           return Prec::Comma;
//    case T_QUESTION:        return Prec::Conditional;
//    case T_PIPE_PIPE:       return Prec::LogicalOr;
//    case T_AMPER_AMPER:     return Prec::LogicalAnd;
//    case T_PIPE:            return Prec::InclusiveOr;
//    case T_CARET:           return Prec::ExclusiveOr;
//    case T_AMPER:           return Prec::And;
//    case T_EQUAL_EQUAL:
//    case T_EXCLAIM_EQUAL:   return Prec::Equality;
//    case T_GREATER:
//    case T_LESS:
//    case T_LESS_EQUAL:
//    case T_GREATER_EQUAL:   return Prec::Relational;
//    case T_LESS_LESS:
//    case T_GREATER_GREATER: return Prec::ExclusiveOr;
//    case T_PLUS:
//    case T_MINUS:           return Prec::Additive;
//    case T_STAR:
//    case T_SLASH:
//    case T_PERCENT:         return Prec::Multiplicative;
//    case T_ARROW_STAR:
//    case T_DOT_STAR:        return Prec::PointerToMember;
//    default:                return Prec::Unknown;
//    }
//}

//inline bool isBinaryOperator(int tokenKind)
//{ return precedence(tokenKind, false) != Prec::Unknown; }

//inline bool isRightAssociative(int tokenKind)
//{
//    const int prec = precedence(tokenKind, false);
//    return prec == Prec::Conditional || prec == Prec::Assignment;
//}

} // end of anonymous namespace

#ifndef CPLUSPLUS_NO_DEBUG_RULE
#  define DEBUG_THIS_RULE() DebugRule __debug_rule__(__func__, tok().spell(), cursor(), _translationUnit->blockErrors())
#else
#  define DEBUG_THIS_RULE() do {} while (0)
#endif

#define PARSE_EXPRESSION_WITH_OPERATOR_PRECEDENCE(node, minPrecedence) { \
    if (LA() == T_THROW) { \
        if (!parseThrowExpression(node)) \
            return false; \
    } else if (!parseCastExpression(node)) \
        return false; \
    \
    parseExpressionWithOperatorPrecedence(node, minPrecedence); \
    return true; \
}

Parser::Parser(TranslationUnit *unit)
    : ISemanticParser()
    , _translationUnit(unit)
    , _pool(_translationUnit->memoryPool())
    , _tokenIndex(1)
    , _expressionDepth(0)
{ }

Parser::~Parser()
{ }

BaseWalker *Parser::parseDocument( ParseTarget target )
{
    switch ( target ) {
        case ParseSqlNames:
            TranslationUnitAST *node = 0;
            parseTranslationUnit(node);
            BaseWalker *walker = new SqlCheckNamesWalker(node, _translationUnit);
            return walker;
    }

    return 0;
}

void Parser::fillSqlStatements( QList<SqlStatement *> *statements,
                                Database::ExecuteType executeType, ITextDocument *textDocument )
{
    TranslationUnitAST *node = 0;
    parseTranslationUnit(node);
    SqlStatementsBindVisitor visitor(_translationUnit);
    visitor.fillStatementsList(statements, node, executeType, textDocument);
}

void Parser::fillStatementFromText( QList<SqlStatement *> *statements, const QString &statementText )
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

//bool Parser::skipUntilStatement()
//{
//    while (int tk = LA()) {
//        switch (tk) {
//            case T_SEMICOLON:
//            case T_LBRACE:
//            case T_RBRACE:
//            case T_CONST:
//            case T_VOLATILE:
//            case T_IDENTIFIER:
//            case T_CASE:
//            case T_DEFAULT:
//            case T_IF:
//            case T_SWITCH:
//            case T_WHILE:
//            case T_DO:
//            case T_FOR:
//            case T_BREAK:
//            case T_CONTINUE:
//            case T_RETURN:
//            case T_GOTO:
//            case T_TRY:
//            case T_CATCH:
//            case T_THROW:
//            case T_CHAR:
//            case T_WCHAR_T:
//            case T_BOOL:
//            case T_SHORT:
//            case T_INT:
//            case T_LONG:
//            case T_SIGNED:
//            case T_UNSIGNED:
//            case T_FLOAT:
//            case T_DOUBLE:
//            case T_VOID:
//            case T_CLASS:
//            case T_STRUCT:
//            case T_UNION:
//            case T_ENUM:
//            case T_COLON_COLON:
//            case T_TEMPLATE:
//            case T_USING:
//                return true;

//            default:
//                consumeToken();
//        }
//    }

//    return false;
//}

bool Parser::skip(int l, int r)
{
    int count = 0;

    while (int tk = LA()) {
        if (tk == l)
            ++count;
        else if (tk == r)
            --count;
        else if (l != T_LBRACE && (tk == T_LBRACE ||
                                   tk == T_RBRACE ||
                                   tk == T_SEMICOLON))
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
    error(_tokenIndex, "expected token `%s' got `%s'", Token::name(kind), tok().spell());
    return false;
}

bool Parser::match1(int kind, unsigned *token)
{
    if ( LA1() == kind ) {
        *token = consumeToken();
        return true;
    }
    *token = 0;
    error(_tokenIndex, "expected token `%s' got `%s'", Token::name1(kind), tok().spell());
    return false;
}

bool Parser::parseTranslationUnit( TranslationUnitAST *&node )
{
    DEBUG_THIS_RULE();
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

    if ( skipUntil(T_SEMICOLON) )
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

    if ( skipUntil(T_SEMICOLON) )
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

        if ( LA() == T_RPAREN ) {
            error(_tokenIndex, "Unexpected token `)'");
            consumeToken();
            continue;
        }
        if ( LA1() || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
            return false;
        if ( LA() == T_COMMA ) {
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

        if ( LA() == T_COMMA ) {
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
    match(T_RPAREN, &ast->rparen_token);
    node = ast;
    return (ast->rparen_token != 0);
}

bool Parser::parseResultColumnList( ResultColumnListAST *&node )
{
    ResultColumnListAST **res_col_list = &node;

    unsigned first_token = cursor();

    while (1) {
        unsigned old_cursor = cursor();

        if ( (isKeywordOnly() && LA1() != T_4_NULL) || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL ) {
            if ( first_token == cursor() )
                error(_tokenIndex, "Expected select column expression");
            return true;
        }

        if ( LA() == T_COMMA ) {
            warning(_tokenIndex, "Expected select column expression");
            consumeToken();
            continue;
        }

        if ( LA() == T_STAR ) {
            StarResultColumnAST *star = new (_pool) StarResultColumnAST;
            star->star_token = consumeToken();
            *res_col_list = new (_pool) ResultColumnListAST;
            (*res_col_list)->value = star;
            res_col_list = &(*res_col_list)->next;
        }
        else if ( isPatentialIdentifier() && LA(2) == T_DOT && LA(3) == T_STAR ) {
            StarResultColumnAST *tableStar = new (_pool) StarResultColumnAST;
            tableStar->schemaTableName = new (_pool) SchemaTableNameAST;
            tableStar->schemaTableName->tableName = new (_pool) TableNameAST;
            const Token &tk = tok();
            tableStar->schemaTableName->tableName->name = tk.identifier;
            tableStar->schemaTableName->tableName->name_token = consumeToken();
            tableStar->dot_token = consumeToken();
            tableStar->star_token = consumeToken();
            *res_col_list = new (_pool) ResultColumnListAST;
            (*res_col_list)->value = tableStar;
            res_col_list = &(*res_col_list)->next;
        }
        else if ( isPatentialIdentifier() && LA(2) == T_DOT && isPatentialIdentifier(3) && LA(4) == T_DOT && LA(5) == T_STAR ) {
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
                const Token &tk = tok();
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

        if ( LA() == T_COMMA ) {
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
            const Token &tk1 = tok();
            source->tableAlias = new (_pool) TableAliasAST;
            source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
            source->tableAlias->name_token = consumeToken();
            source->tableAlias->schemaTableName = source->schemaTableName /*->clone(_pool)*/;
        }
        else if ( needAlias )
            error(_tokenIndex, "Expected table alias");
        ast->firstSource = source;
    }
    else if ( LA() == T_LPAREN && LA1(2) == T_6_SELECT ) {
        SubQuerySingleSourceAST *source = new (_pool) SubQuerySingleSourceAST;
        parseSubQuery(source->subQuery);
        bool needAlias = false;
        if ( LA1() == T_2_AS ) {
            source->as_token = consumeToken();
            needAlias = true;
        }
        if ( isPatentialIdentifier() ) {
            const Token &tk1 = tok();
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

    if ( LA() == T_COMMA || LA1() == T_4_JOIN ) {
        if ( LA() == T_COMMA )
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
        const Token &tk1 = tok();
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
        const Token &tk1 = tok();
        source->tableAlias = new (_pool) TableAliasAST;
        source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
        source->tableAlias->name_token = consumeToken();
        source->tableAlias->schemaTableName = source->schemaTableName;
    }
    else if ( needAlias )
        error(_tokenIndex, "Expected table source alias");
    ast->source = source;
    if ( LA() == T_LPAREN ) {
        ast->lparen_columns_token = consumeToken();
        if ( !parseColumnList(ast->columns_list) )
            P_RET_TRUE;
        match(T_RPAREN, &ast->rparen_columns_token);
    }
    if ( LA1() == T_6_VALUES ) {
        InsertClauseAsValuesAST *valClause = new (_pool) InsertClauseAsValuesAST;
        valClause->values_token = consumeToken();
        if ( !match(T_LPAREN, &valClause->lparen_token) )
            P_RET_TRUE;
        if ( !parseValueExpresionList(valClause->val_list, true) )
            P_RET_TRUE;
        match(T_RPAREN, &valClause->rparen_token);
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
        const Token &tk1 = tok();
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

        if ( LA() == T_RPAREN )
            return true;
        if ( LA() == T_COMMA ) {
            warning(_tokenIndex, "Expected column name");
            consumeToken();
            continue;
        }
        if ( LA1() || LA() == T_EOF_SYMBOL || LA() == T_SEMICOLON ) {
            return true;
        }

        if ( isPatentialIdentifier() ) {
            ColumnNameAST *column_name = new (_pool) ColumnNameAST;
            if ( LA(2) == T_DOT && isPatentialIdentifier(3) && LA(4) == T_DOT ) {
                SchemaNameAST *schemaName = new (_pool) SchemaNameAST;
                const Token &stk = tok();
                schemaName->name = stk.identifier;
                schemaName->name_token = consumeToken();
                consumeToken();
                TableNameAST *tableName = new (_pool) TableNameAST;
                const Token &ttk = tok();
                tableName->name = ttk.identifier;
                tableName->name_token = consumeToken();
                consumeToken();
                column_name->schemaName = schemaName;
                column_name->tableOfColumn = tableName;
            }
            else if ( LA(2) == T_DOT ) {
                TableAliasAST *alias = new (_pool) TableAliasAST;
                const Token &tk = tok();
                alias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                alias->name_token = consumeToken();
                consumeToken();
                column_name->tableOfColumn = alias;
            }
            if ( isPatentialIdentifier() ) {
                const Token &tk = tok();
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


        if ( LA() == T_COMMA ) {
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

        if ( LA1() || LA() == T_EOF_SYMBOL || LA() == T_SEMICOLON ) {
            return true;
        }
        if ( LA() == T_COMMA ) {
            warning(_tokenIndex, "Expected assignment expression");
            consumeToken();
            continue;
        }

        if ( isPatentialIdentifier() ) {
            ColumnNameAST *column_name = new (_pool) ColumnNameAST;
            AssignmentExpressionAST *assignExpr = new (_pool) AssignmentExpressionAST;
            assignExpr->first_token = cursor();
            if ( LA(2) == T_DOT && isPatentialIdentifier(3) && LA(4) == T_DOT ) {
                SchemaNameAST *schemaName = new (_pool) SchemaNameAST;
                const Token &stk = tok();
                schemaName->name = stk.identifier;
                schemaName->name_token = consumeToken();
                consumeToken();
                TableNameAST *tableName = new (_pool) TableNameAST;
                const Token &ttk = tok();
                tableName->name = ttk.identifier;
                tableName->name_token = consumeToken();
                consumeToken();
                column_name->schemaName = schemaName;
                column_name->tableOfColumn = tableName;
            }
            else if ( LA(2) == T_DOT ) {
                TableAliasAST *alias = new (_pool) TableAliasAST;
                const Token &tk = tok();
                alias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                alias->name_token = consumeToken();
                consumeToken();
                column_name->tableOfColumn = alias;
            }
            if ( isPatentialIdentifier() ) {
                const Token &tk = tok();
                column_name->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
                column_name->name_token = consumeToken();
                assignExpr->columnName = column_name;
                if ( LA() != T_EQUAL ) {
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


        if ( LA() == T_COMMA ) {
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
            if ( LA() != T_CHAR_LITERAL ) {
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
                if ( LA(2) == T_DOT ) {
                    if ( isPatentialIdentifier(3) && LA(4) == T_DOT)  {
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
                const Token &stk = tok();
                schemaTableName->schemaName = new (_pool) SchemaNameAST;
                schemaTableName->schemaName->name = stk.identifier;
                schemaTableName->schemaName->name_token = consumeToken();
                schemaTableName->dot_token = consumeToken();
            }
            const Token &ttk = tok();
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
            const Token &tk1 = tok();
            colAst->name = tk1.identifier;
            colAst->name_token = consumeToken();
            ast->column = colAst;
            if ( !match1(T_2_IS, &ast->is_token) ) {
                node = ast;
                return true;
            }
            if ( LA() != T_CHAR_LITERAL ) {
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
        else if ( LA() == T_LPAREN ) {
            CreateTableDirectClauseAST *directClause = new (_pool) CreateTableDirectClauseAST;
            directClause->lparen_token = consumeToken();
            parseColumnDefinitionList(directClause->column_def_list);
            parseTableConstraintList(directClause->constraintList);
            match(T_RPAREN, &directClause->rparen_token);
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
        if ( !match(T_LPAREN, &ast->lparen_token) ) {
            error(_tokenIndex, "Expected token `('");
            node = ast;
            return true;
        }
        parseSortedColumnNameList(ast->columns_list);
        match(T_RPAREN, &ast->rparen_token);
        node = ast;
        return true;
    }

    else if ( LA1() == T_7_TRIGGER ) {
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
        if ( LA() == T_LPAREN || LA1() == T_4_WHEN ) {
            CreateRdbTriggerBodyAST *triggerBody = new (_pool) CreateRdbTriggerBodyAST;
            parseRdbTriggerBodyList(triggerBody->item_list);
            ast->triggerBody = triggerBody;
        }
        /// TODO: Oracle/SQLite (BEGIN ... END;) or Db2 (BEGIN ATOMIC ... END;) dialects
        // else { ... }
        node = ast;
        return true;
    }

    error(_tokenIndex, "Expected TABLE|VIEW|INDEX|TRIGGER token");
    return false;
}

bool Parser::parseColumnDefinitionList(ColumnDefinitionListAST *&node)
{
    if ( LA() == T_RPAREN || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
        return true;

    if ( LA() == T_COMMA ) {
        warningCursor("Expected column definition");
        consumeToken();
        return parseColumnDefinitionList(node);
    }

    ColumnDefinitionListAST **col_ptr = &node;

    if ( isPatentialIdentifier() ) {
        ColumnDefinitionAST *columnDef = new (_pool) ColumnDefinitionAST;
        columnDef->columnName = new (_pool) ColumnNameAST;
        const Token &tk = tok();
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
            else if ( LA() == T_NUMERIC_LITERAL ) {
                NumericExpressionAST *numericAst = new (_pool) NumericExpressionAST;
                numericAst->numeric_token = consumeToken();
                columnDef->defaultExpr = numericAst;
            }
            else if ( LA() == T_CHAR_LITERAL ) {
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
            case T_COMMA:
                consumeToken();
                return parseColumnDefinitionList(*col_ptr);
            case T_RPAREN:
            case T_SEMICOLON:
            case T_EOF_SYMBOL:
                return false;
            default:
                consumeToken();
        }
    }

    return false;
}

/*
bool Parser::parseColumnDefinitionList(ColumnDefinitionListAST *&node)
{
    ColumnDefinitionListAST **col_ptr = &node;

    while (1) {
        unsigned first_token = cursor();

        if ( LA() == T_RPAREN )
            return true;
        if ( LA() == T_COMMA ) {
            warning(_tokenIndex, "Expected column definition");
            consumeToken();
            continue;
        }
        if ( LA1() || LA() == T_EOF_SYMBOL || LA() == T_SEMICOLON ) {
            return true;
        }

        if ( LA() == T_IDENTIFIER ) {
            const Token &tk = tok();
            ColumnDefinitionAST *columnDef = new (_pool) ColumnDefinitionAST;
            columnDef->columnName = new (_pool) ColumnNameAST;
            columnDef->columnName->name = tk.identifier;
            columnDef->columnName->name_token = consumeToken();
            if ( !LA3() ) {
                error(_tokenIndex, "Expected type specifier");
                return false;
            }
            if ( !parseTypeSpecifier(columnDef->columnType) )
                return false;
            if ( LA1() == T_7_DEFAULT ) {
                columnDef->default_token = consumeToken();
                if ( LA1() == T_4_NULL ) {
                    NullValueExpressionAST *nullExpr = new (_pool) NullValueExpressionAST;
                    nullExpr->null_token = consumeToken();
                    columnDef->defaultExpr = nullExpr;
                }
                else if ( LA() == T_NUMERIC_LITERAL ) {
                    NumericExpressionAST *numericAst = new (_pool) NumericExpressionAST;
                    numericAst->numeric_token = consumeToken();
                    columnDef->defaultExpr = numericAst;
                }
                else if ( LA() == T_CHAR_LITERAL ) {
                    LiteralExpressionAST *literAst = new (_pool) LiteralExpressionAST;
                    literAst->liter_token = consumeToken();
                    columnDef->defaultExpr = literAst;
                }
                else
                    warning(_tokenIndex, "Expected default value");
            }
            *col_ptr = new (_pool) ColumnDefinitionListAST;
            (*col_ptr)->value = columnDef;
            col_ptr = &(*col_ptr)->next;

            parseColumnConstraintList(columnDef->constraintList);
        }
        else
            warning(_tokenIndex, "Expected column name");

        if ( LA() == T_COMMA ) {
            consumeToken();
            continue;
        }

        if ( first_token == cursor() ) {
            error(_tokenIndex, "WTF! (parseColumnList)");
            return false;
        }
    }

    return true;
} */

bool Parser::parseColumnConstraintList( ColumnConstraintListAST *&node )
{
    ColumnConstraintListAST **constr_list_ptr = &node;

    if ( LA() == T_COMMA || LA() == T_RPAREN || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
        return true;

    unsigned constraint_token = 0;
    ConstraintNameAST *constraintName = 0;
    if ( LA1() == T_10_CONSTRAINT ) {
        constraint_token = consumeToken();
        if ( isPatentialIdentifier() ) {
            constraintName = new (_pool) ConstraintNameAST;
            const Token &tk = tok();
            constraintName->name = tk.identifier;
            constraintName->name_token = consumeToken();
        }
        else {
            errorCursor("Constraint name expected");
        }
        if ( LA() == T_COMMA || LA() == T_RPAREN || LA() == T_SEMICOLON )
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
    if ( LA() == T_RPAREN || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
        return true;

    if ( LA() == T_COMMA ) {
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
            const Token &tk = tok();
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
        if ( LA() != T_COMMA && LA() != T_RPAREN )
            errorCursor("Unexpected token");
    }

    while ( 1 ) {
        switch ( LA() ) {
            case T_COMMA:
                consumeToken();
                return parseTableConstraintList(*constr_list_ptr);
            case T_RPAREN:
                return true;
            case T_SEMICOLON:
            case T_EOF_SYMBOL:
                return false;
            default:
                consumeToken();
        }
    }

    return false;
}

bool Parser::parseSortedColumnNameList( SortedColumnNameListAST *&node )
{
    if ( LA() == T_RPAREN || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
        return true;

    if ( LA() == T_COMMA ) {
        warning(_tokenIndex, "Expected column name");
        consumeToken();
        return parseSortedColumnNameList(node);
    }

    SortedColumnNameListAST **col_list_ptr = &node;

    if ( isPatentialIdentifier() ) {
        SortedColumnNameAST *columnName = new (_pool) SortedColumnNameAST;
        columnName->columnName = new (_pool) ColumnNameAST;
        const Token &tk = tok();
        columnName->columnName->name = tk.identifier;
        columnName->columnName->name_token = consumeToken();

        if ( LA1() == T_3_ASC || LA1() == T_4_DESC )
            columnName->asc_or_desc_token = consumeToken();

        *col_list_ptr = new (_pool) SortedColumnNameListAST;
        (*col_list_ptr)->value = columnName;
        col_list_ptr = &(*col_list_ptr)->next;

        if ( LA() != T_COMMA && LA() != T_RPAREN) {
            errorCursor("Unexpected token");
        }
    }
    else
        errorCursor("Expected column name");

    while ( 1 ) {
        switch ( LA() ) {
            case T_COMMA:
                consumeToken();
                return parseSortedColumnNameList(*col_list_ptr);
            case T_RPAREN:
                return true;
            case T_SEMICOLON:
            case T_EOF_SYMBOL:
                return false;
            default:
                consumeToken();
        }
    }

    return false;
}

bool Parser::parseRdbTriggerBodyList( CreateRdbTriggerBodyListAST *&node )
{
    if ( LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
        return true;

    if ( LA1() != T_4_WHEN && LA() != T_LPAREN )
        return true;

    CreateRdbTriggerBodyItemAST *bodyItem = new (_pool) CreateRdbTriggerBodyItemAST;

    bool ok = true;
    if ( LA1() == T_4_WHEN ) {
        bodyItem->when_token = consumeToken();
        if ( LA() == T_LPAREN ) {
            bodyItem->lparen_when_token = consumeToken();
            parseConditionExpression(bodyItem->whenExpr);
            if ( LA() == T_RPAREN )
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
        if ( LA() == T_LPAREN ) {
            bodyItem->lparen_token = consumeToken();
            if ( ok = parseTriggerStatementList(bodyItem->stat_list) ) {
                if ( LA() == T_RPAREN ) {
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
    if ( LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL || LA() == T_RPAREN )
        return true;

    if ( LA() == T_COMMA ) {
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
    else if ( LA() == T_IDENTIFIER ) {
        const Token &tk = tok();
        QString errStr = tk.identifier->chars();
        if ( !errStr.compare("ERROR", Qt::CaseInsensitive) )
            consumeToken();
        else {
            errorCursor("Expected DELETE|INSERT|UPDATE|ERROR token");
            consumeToken();
            return true;
        }
    }
    else {
        errorCursor("Expected DELETE|INSERT|UPDATE|ERROR token");
        consumeToken();
        return true;
    }

    StatementListAST **stat_list_ptr = &node;
    if ( statement ) {
        *stat_list_ptr = new (_pool) StatementListAST;
        (*stat_list_ptr)->value = statement;
        stat_list_ptr = &(*stat_list_ptr)->next;
    }

    if ( LA() == T_COMMA ) {
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
    if ( LA() == T_IDENTIFIER ) {
        const Token &tk = tok();
        ast->funcName = new (_pool) FunctionNameAST;
        ast->funcName->name = tk.identifier;
        ast->funcName->name_token = consumeToken();
        if ( LA() == T_LPAREN ) {
            ast->lparen_token = consumeToken();
            if ( parseValueExpresionList(ast->arg_list, true) ) {
                if ( LA() == T_RPAREN )
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

    if ( isKeywordOnly() || LA() == T_SEMICOLON || LA() == T_COMMA || LA() == T_RPAREN || LA() == T_EOF_SYMBOL )
        return true;

//    if ( conditionMode ) {
//        switch ( LA() ) {
//            case T_LESS:
//            case T_LESS_EQUAL:
//            case T_LESS_GREATER:
//            case T_GREATER:
//            case T_GREATER_EQUAL:
//            case T_EXCLAIM_EQUAL:
//            case T_EQUAL:
//                return true;
//        }
//    }

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
             && LA(2) == T_LPAREN ) {
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
            if ( LA(2) == T_LPAREN ) {
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
                case T_LPAREN: {
                    CompoundValueExpressionAST *ast;
                    if ( parseCompoundValueExpresion(ast) ) {
                        node = ast;
                        items.append(ast);
                    }
                    break;
                }
                case T_PLUS:
                case T_MINUS: {
                    UnaryValueExpressionAST *ast;
                    if ( parseUnaryValueExpression(ast) ) {
                        node = ast;
                        items.append(ast);
                    }
                    break;
                }
                case T_COLON: {
                    ParameterExpressionAST *ast;
                    if ( parseParameterExpression(ast) ) {
                        node = ast;
                        items.append(ast);
                    }
                    break;
                }
                case T_NUMERIC_LITERAL: {
                    NumericExpressionAST *ast = new (_pool) NumericExpressionAST;
                    ast->first_token = cursor();
                    ast->numeric_token = consumeToken();
                    ast->last_token = ast->numeric_token;
                    node = ast;
                    items.append(ast);
                    break;
                }
                case T_CHAR_LITERAL: {
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
            case T_MINUS:
            case T_PLUS:
            case T_SLASH:
            case T_STAR:
            case T_PIPE_PIPE: {
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
            case T_LESS:
            case T_LESS_EQUAL:
            case T_LESS_GREATER:
            case T_GREATER:
            case T_GREATER_EQUAL:
            case T_EXCLAIM_EQUAL:
            case T_EQUAL:
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

        if ( LA() == T_RPAREN ) {
            if ( eatRParen )
                return true;
            error(_tokenIndex, "Unexpected token `)'");
            consumeToken();
            continue;
        }
        if ( (LA1() && LA1() != T_4_NULL) || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
            return false;
        if ( LA() == T_COMMA ) {
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

        if ( LA() == T_COMMA ) {
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
    match(T_RPAREN, &ast->rparen_token);
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
        const Token &tk = tok();
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

bool Parser::parseFunctionExpresion( FunctionExpressionAST *&node )
{
    FunctionExpressionAST *ast = new (_pool) FunctionExpressionAST;
    ValueExpressionListAST **arg_list = &ast->arg_list;

    const Token &tk = tok();
    FunctionNameAST *funcName = new (_pool) FunctionNameAST;
    funcName->name = tk.identifier;
    ast->first_token = consumeToken();
    funcName->name_token = ast->first_token;
    ast->functionName = funcName;
    ast->lparen_token = consumeToken();

    // Parameters list
    if ( LA() == T_STAR ) {
        consumeToken();
    }
    else {
        while (1) {
            unsigned old_cursor = cursor();

            if ( LA() == T_RPAREN )
                break;
            if ( (LA1() && LA1() != T_4_NULL) || LA() == T_SEMICOLON || LA() == T_EOF_SYMBOL )
                return false;
            if ( LA() == T_COMMA ) {
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

            if ( LA() == T_COMMA ) {
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
    match(T_RPAREN, &ast->rparen_token);

    node = ast;
    return true;
}

bool Parser::parseCastExpression( CastFunctionAST *&node )
{
    CastFunctionAST *ast = new (_pool) CastFunctionAST;
    ast->first_token = cursor();

    ast->cast_token = consumeToken();
    if ( !match(T_LPAREN, &ast->lparen_token) )
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

    if ( !match(T_RPAREN, &ast->rparen_token) )
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
    if ( !match(T_LPAREN, &ast->lparen_token) )
        return false;

    ValueExpressionAST *strExpr;
    if ( !parseValueExpresion(strExpr) )
        return false;
    ast->strExpr = strExpr;
    if ( LA() == T_RPAREN ) {
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
    if ( LA() == T_RPAREN ) {
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

    if ( !match(T_RPAREN, &ast->rparen_token) )
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
    if ( !match(T_LPAREN, &ast->lparen_token) )
        return false;

    if ( LA1() == T_8_DISTINCT )
        ast->distinct_token = consumeToken();

    if ( LA() == T_STAR ) {
        consumeToken();
        if ( !match(T_RPAREN, &ast->rparen_token) )
            return false;
        node = ast;
        return true;
    }

    ValueExpressionAST *countExpr;
    if ( !parseValueExpresion(countExpr) )
        return false;
    ast->countExpr = countExpr;

    if ( !match(T_RPAREN, &ast->rparen_token) )
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
            if ( LA() == T_LPAREN ) {
                ast->lparen_token = consumeToken();
                if ( LA() != T_NUMERIC_LITERAL ) {
                    error(_tokenIndex, "Expected NUMERIC token");
                    return false;
                }
                ast->size_token = consumeToken();
                if ( !match(T_RPAREN, &ast->rparen_token) )
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
    if ( LA(2) == T_DOT && isPatentialIdentifier(3) && LA(4) == T_DOT ) {
        SchemaNameAST *schemaName = new (_pool) SchemaNameAST;
        const Token &stk = tok();
        schemaName->name = stk.identifier;
        schemaName->name_token = consumeToken();
        consumeToken();
        TableNameAST *tableName = new (_pool) TableNameAST;
        const Token &ttk = tok();
        tableName->name = ttk.identifier;
        tableName->name_token = consumeToken();
        consumeToken();
        column_name->schemaName = schemaName;
        column_name->tableOfColumn = tableName;
    }
    else if ( LA(2) == T_DOT ) {
        TableAliasAST *alias = new (_pool) TableAliasAST;
        const Token &tk = tok();
        alias->name = tk.identifier; // new (_pool) QString(tk.identifier->chars());
        alias->name_token = consumeToken();
        ast->last_token = consumeToken();
        column_name->tableOfColumn = alias;
    }
    if ( isPatentialIdentifier() ) {
        const Token &tk = tok();
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

        if ( LA() == T_EOF_SYMBOL || LA() == T_SEMICOLON ) {
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

        if ( LA() == T_EOF_SYMBOL || LA() == T_SEMICOLON ) {
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

bool Parser::parseConditionExpression( ConditionExpressionAST *&node )
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
    if ( LA() == T_SEMICOLON || LA() == T_COMMA || LA() == T_RPAREN || LA() == T_EOF_SYMBOL )
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

    else if ( items.size() == 1 ) {
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
    match(T_RPAREN, &ast->rparen_token);
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
    if ( !match(T_LPAREN, &ast->rvExpr->lparen_token) )
        return false;
    if ( !parseSelectStatement(ast->rvExpr->query) )
        return false;
    ast->last_token = cursor();
    if ( !match(T_RPAREN, &ast->rvExpr->rparen_roken) )
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

    if ( LA() == T_LPAREN && LA1(2) != T_6_SELECT ) {
        ValueListExpressionAST *valList = new (_pool) ValueListExpressionAST;
        valList->lparen_token = consumeToken();
        if ( !parseValueExpresionList(valList->val_list, true) )
            return false;
        ast->last_token = cursor();
        if ( !match(T_RPAREN, &valList->rparen_roken) )
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
        if ( !match(T_RPAREN, &ast->rparen_token) )
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

    if ( LA() == T_RPAREN ) {
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
        if ( !match(T_RPAREN, &rparen_token) )
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

    if ( LA() == T_LPAREN ) {
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
         LA() == T_LESS || LA() == T_LESS_EQUAL || LA() == T_LESS_GREATER || LA() == T_EQUAL ||
         LA() == T_GREATER || LA() == T_GREATER_EQUAL || LA() == T_EXCLAIM_EQUAL ) {
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
                const Token &tk1 = tok();
                source->tableAlias = new (_pool) TableAliasAST;
                source->tableAlias->name = tk1.identifier; // new (_pool) QString(tk1.identifier->chars());
                source->tableAlias->name_token = consumeToken();
                source->tableAlias->schemaTableName = source->schemaTableName;
            }
            else if ( needAlias )
                error(_tokenIndex, "Expected table source alias");
            join_source->singleSource = source;
        }
        else if ( LA() == T_LPAREN && LA1(2) == T_6_SELECT ) {
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
                const Token &tk1 = tok();
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

//const Identifier *Parser::identifier(NameAST *name) const
//{
//  if (! name)
//    return 0;

//  if (QualifiedNameAST *q = name->asQualifiedName())
//    name = q->unqualified_name;

//  if (name) {
//    if (SimpleNameAST *simple = name->asSimpleName())
//      return _translationUnit->identifier(simple->identifier_token);
//    else if (TemplateIdAST *template_id = name->asTemplateId())
//      return _translationUnit->identifier(template_id->identifier_token);
//  }

//  return 0;
//}

//bool Parser::parseStatement(StatementAST *&node)
//{
//    DEBUG_THIS_RULE();
//    switch (LA()) {
//    case T_SQL_EXEC:
//      return parseExecSqlStatement(node);

//    case T_WHILE:
//        return parseWhileStatement(node);

//    case T_DO:
//        return parseDoStatement(node);

//    case T_Q_FOREACH:
//        return parseForeachStatement(node);

//    case T_FOR:
//        return parseForStatement(node);

//    case T_IF:
//        return parseIfStatement(node);

//    case T_SWITCH:
//        return parseSwitchStatement(node);

//    case T_TRY:
//        return parseTryBlockStatement(node);

//    case T_CASE:
//    case T_DEFAULT:
//        return parseLabeledStatement(node);

//    case T_BREAK:
//        return parseBreakStatement(node);

//    case T_CONTINUE:
//        return parseContinueStatement(node);

//    case T_GOTO:
//        return parseGotoStatement(node);

//    case T_RETURN:
//        return parseReturnStatement(node);

//    case T_LBRACE:
//        return parseCompoundStatement(node);

//    case T_ASM:
//    case T_NAMESPACE:
//    case T_USING:
//    case T_TEMPLATE:
//    case T_CLASS: case T_STRUCT: case T_UNION:
//        return parseDeclarationStatement(node);

//    case T_SEMICOLON: {
//        ExpressionStatementAST *ast = new (_pool) ExpressionStatementAST;
//        ast->semicolon_token = consumeToken();
//        node = ast;
//        return true;
//    }

//    case T_AT_SYNCHRONIZED:
//        return objCEnabled() && parseObjCSynchronizedStatement(node);

//    case T_Q_D:
//    case T_Q_Q: {
//        QtMemberDeclarationAST *ast = new (_pool) QtMemberDeclarationAST;
//        ast->q_token = consumeToken();
//        match(T_LPAREN, &ast->lparen_token);
//        parseTypeId(ast->type_id);
//        match(T_RPAREN, &ast->rparen_token);
//        node = ast;
//    } return true;

//    default:
//        if (LA() == T_IDENTIFIER && LA(2) == T_COLON)
//            return parseLabeledStatement(node);

//        return parseExpressionOrDeclarationStatement(node);
//    } // switch
//    return false; //Avoid compiler warning
//}
void Parser::rewind(unsigned cursor)
{
#ifndef CPLUSPLUS_NO_DEBUG_RULE
    if (cursor != _tokenIndex)
        fprintf(stderr, "! rewinding from token %d to token %d\n", _tokenIndex, cursor);
#endif

    if (cursor < _translationUnit->tokenCount())
        _tokenIndex = cursor;
    else
        _tokenIndex = _translationUnit->tokenCount() - 1;
}

bool Parser::isPatentialIdentifier( int n ) const
{
    if ( LA(n) == T_IDENTIFIER )
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
    _translationUnit->message(DiagnosticClient::Warning, index, format, ap);
    va_end(ap);
    va_end(args);
}

void Parser::error(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(DiagnosticClient::Error, index, format, ap);
    va_end(ap);
    va_end(args);
}

void Parser::fatal(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(DiagnosticClient::Fatal, index, format, ap);
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

