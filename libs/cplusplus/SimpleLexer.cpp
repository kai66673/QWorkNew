/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#include "SimpleLexer.h"
#include "BackwardsScanner.h"

#include "ObjectiveCTypeQualifiers.h"

#include <Lexer.h>
#include <Token.h>
#include <QtDebug>
#include <QTextBlock>

using namespace CPlusPlus;

SimpleLexer::SimpleLexer()
    : _lastState(0),
      _lastSqlState(0),
      _skipComments(false),
      _qtMocRunEnabled(true),
      _objCEnabled(false),
      _endedJoined(false),
      _singleLineCommentsEnabled(true)
{
}

SimpleLexer::~SimpleLexer()
{ }

bool SimpleLexer::qtMocRunEnabled() const
{
    return _qtMocRunEnabled;
}

void SimpleLexer::setQtMocRunEnabled(bool enabled)
{
    _qtMocRunEnabled = enabled;
}

bool SimpleLexer::objCEnabled() const
{
    return _objCEnabled;
}

void SimpleLexer::setObjCEnabled(bool onoff)
{
    _objCEnabled = onoff;
}

bool SimpleLexer::skipComments() const
{
    return _skipComments;
}

void SimpleLexer::setSkipComments(bool skipComments)
{
    _skipComments = skipComments;
}

bool SimpleLexer::endedJoined() const
{
    return _endedJoined;
}

bool SimpleLexer::singleLineCommentEnabled() const
{
    return _singleLineCommentsEnabled;
}

void SimpleLexer::setSingleLineCommentEnabled( bool onoff )
{
    _singleLineCommentsEnabled = onoff;
}

bool SimpleLexer::extraExecSqlEnabled() const
{
  return _extraExecSqlEnabled;
}

void SimpleLexer::setExtraExecSqlEnabled( bool onoff )
{
  _extraExecSqlEnabled = onoff;
}

/*
static void castExecSqlIdentifier( Token *tok, QStringRef &spell )
{
    switch( spell.size() ) {
    case 2:
        if ( !spell.compare(QLatin1String("as"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_AS;           return; }
        if ( !spell.compare(QLatin1String("by"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_BY;           return; }
        if ( !spell.compare(QLatin1String("is"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_IS;           return; }
        if ( !spell.compare(QLatin1String("in"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_IN;           return; }
        if ( !spell.compare(QLatin1String("on"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_ON;           return; }
        if ( !spell.compare(QLatin1String("or"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_OR;           return; }
        if ( !spell.compare(QLatin1String("to"), Qt::CaseInsensitive) )             { tok->f.kind = T_SQL_TO;           return; }
        break;
    case 3:
        if ( !spell.compare(QLatin1String("and"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_AND;          return; }
        if ( !spell.compare(QLatin1String("asc"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_ASC;          return; }
        if ( !spell.compare(QLatin1String("for"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_FOR;          return; }
        if ( !spell.compare(QLatin1String("max"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_MAX;          return; }
        if ( !spell.compare(QLatin1String("min"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_MIN;          return; }
        if ( !spell.compare(QLatin1String("not"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_NOT;          return; }
        if ( !spell.compare(QLatin1String("row"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_ROW;          return; }
        if ( !spell.compare(QLatin1String("set"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_SET;          return; }
        if ( !spell.compare(QLatin1String("sql"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_SQL;          return; }
        if ( !spell.compare(QLatin1String("sum"), Qt::CaseInsensitive) )            { tok->f.kind = T_SQL_SUM;          return; }
        break;
    case 4:
        if ( !spell.compare(QLatin1String("desc"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_DESC;         return; }
        if ( !spell.compare(QLatin1String("exec"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_EXEC;         return; }
        if ( !spell.compare(QLatin1String("from"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_FROM;         return; }
        if ( !spell.compare(QLatin1String("goto"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_GOTO;         return; }
        if ( !spell.compare(QLatin1String("join"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_JOIN;         return; }
        if ( !spell.compare(QLatin1String("into"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_INTO;         return; }
        if ( !spell.compare(QLatin1String("left"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_LEFT;         return; }
        if ( !spell.compare(QLatin1String("only"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_ONLY;         return; }
        if ( !spell.compare(QLatin1String("open"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_OPEN;         return; }
        if ( !spell.compare(QLatin1String("read"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_READ;         return; }
        if ( !spell.compare(QLatin1String("rows"), Qt::CaseInsensitive) )           { tok->f.kind = T_SQL_ROWS;         return; }
        break;
    case 5:
        if ( !spell.compare(QLatin1String("alias"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_ALIAS;        return; }
        if ( !spell.compare(QLatin1String("close"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_CLOSE;        return; }
        if ( !spell.compare(QLatin1String("count"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_COUNT;        return; }
        if ( !spell.compare(QLatin1String("cross"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_CROSS;        return; }
        if ( !spell.compare(QLatin1String("fetch"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_FETCH;        return; }
        if ( !spell.compare(QLatin1String("group"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_GROUP;        return; }
        if ( !spell.compare(QLatin1String("limit"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_LIMIT;        return; }
        if ( !spell.compare(QLatin1String("order"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_ORDER;        return; }
        if ( !spell.compare(QLatin1String("right"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_RIGHT;        return; }
        if ( !spell.compare(QLatin1String("where"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_WHERE;        return; }
        if ( !spell.compare(QLatin1String("write"), Qt::CaseInsensitive) )          { tok->f.kind = T_SQL_WRITE;        return; }
        break;
    case 6:
        if ( !spell.compare(QLatin1String("cursor"), Qt::CaseInsensitive) )         { tok->f.kind = T_SQL_CURSOR;       return; }
        if ( !spell.compare(QLatin1String("delete"), Qt::CaseInsensitive) )         { tok->f.kind = T_SQL_DELETE;       return; }
        if ( !spell.compare(QLatin1String("insert"), Qt::CaseInsensitive) )         { tok->f.kind = T_SQL_INSERT;       return; }
        if ( !spell.compare(QLatin1String("select"), Qt::CaseInsensitive) )         { tok->f.kind = T_SQL_SELECT;       return; }
        if ( !spell.compare(QLatin1String("update"), Qt::CaseInsensitive) )         { tok->f.kind = T_SQL_UPDATE;       return; }
        if ( !spell.compare(QLatin1String("values"), Qt::CaseInsensitive) )         { tok->f.kind = T_SQL_VALUES;       return; }
        break;
    case 7:
        if ( !spell.compare(QLatin1String("between"), Qt::CaseInsensitive) )        { tok->f.kind = T_SQL_BETWEEN;      return; }
        if ( !spell.compare(QLatin1String("declare"), Qt::CaseInsensitive) )        { tok->f.kind = T_SQL_DECLARE;      return; }
        if ( !spell.compare(QLatin1String("include"), Qt::CaseInsensitive) )        { tok->f.kind = T_SQL_INCLUDE;      return; }
        if ( !spell.compare(QLatin1String("prepare"), Qt::CaseInsensitive) )        { tok->f.kind = T_SQL_PREPARE;      return; }
        break;
    case 8:
        if ( !spell.compare(QLatin1String("distinct"), Qt::CaseInsensitive) )       { tok->f.kind = T_SQL_DISTINCT;     return; }
        if ( !spell.compare(QLatin1String("external"), Qt::CaseInsensitive) )       { tok->f.kind = T_SQL_EXTERNAL;     return; }
        if ( !spell.compare(QLatin1String("filename"), Qt::CaseInsensitive) )       { tok->f.kind = T_SQL_FILENAME;     return; }
        if ( !spell.compare(QLatin1String("sqlerror"), Qt::CaseInsensitive) )       { tok->f.kind = T_SQL_SQLERROR;     return; }
        if ( !spell.compare(QLatin1String("whenever"), Qt::CaseInsensitive) )       { tok->f.kind = T_SQL_WHENEVER;     return; }
        break;
    case 10:
        if ( !spell.compare(QLatin1String("sqlwarning"), Qt::CaseInsensitive) )     { tok->f.kind = T_SQL_SQLWARNING;   return; }
        break;
    case 11:
        if ( !spell.compare(QLatin1String("transaction"), Qt::CaseInsensitive) )    { tok->f.kind = T_SQL_TRANSACTION;  return; }
        break;
    }
}
*/

QList<Token> SimpleLexer::operator()(const QString &text, int state, int sqlState)
{
    QList<Token> tokens;

    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    Lexer lex(firstChar, lastChar);
    lex.setQtMocRunEnabled(_qtMocRunEnabled);
    lex.setObjCEnabled(_objCEnabled);
    lex.setStartWithNewline(true);
    lex.setObjCEnabled(_objCEnabled);
    lex.setSingleLineCommentEnabled(_singleLineCommentsEnabled);
    lex.setExtraExecSqlEnabled(_extraExecSqlEnabled);

    if (! _skipComments)
        lex.setScanCommentTokens(true);

    if (state != -1)
        lex.setState(state & 0xff);
    if ( sqlState != -1 )
        lex.setSqlState(sqlState & 0xff);

    bool inPreproc = false;

    for (;;) {
        Token tk;
        lex(&tk);
        if (tk.is(T_EOF_SYMBOL)) {
            _endedJoined = tk.joined();
            break;
        }

        QStringRef spell = text.midRef(lex.tokenOffset(), lex.tokenLength());
        lex.setScanAngleStringLiteralTokens(false);

        if ( _extraExecSqlEnabled && !state ) {
            if ( tk.is(T_COLON) ) {
                if ( tokens.size() > 0 ) {
                    if ( tokens[tokens.size() - 1].is(T_SQL_CINCLUDE) ) {
                        tokens.append(tk);
                        lex(&tk);
                        if ( tk.is(T_EOF_SYMBOL) ) {
                            _endedJoined = tk.joined();
                            break;
                        }
                        while ( tk.isComment() || tk.f.whitespace ) {
                            tokens.append(tk);
                            lex(&tk);
                        }
                        if ( tk.is(T_EOF_SYMBOL) ) {
                            _endedJoined = tk.joined();
                            break;
                        }
                        Token tkIncludeFile;
                        if ( tk.is(T_EOF_SYMBOL) ) {
                            _endedJoined = tk.joined();
                            break;
                        }
                        tkIncludeFile.offset = tk.begin();
                        tkIncludeFile.f.length = 0;
                        while ( tk.is(T_IDENTIFIER) || tk.isKeyword() || tk.isExecSqlKeyword() || tk.is(T_DOT) ) {
                            tkIncludeFile.f.length += tk.f.length;
                            lex(&tk);
                        }
                        if ( tkIncludeFile.f.length ) {
                            tkIncludeFile.f.kind = T_EXEC_SQL_INCLUDE_FILENAME;
                            tkIncludeFile.lineno = tk.lineno;
                            tokens.append(tkIncludeFile);
                        }
                        else {
                            if ( tk.is(T_EOF_SYMBOL) ) {
                                _endedJoined = tk.joined();
                                break;
                            }
                            tokens.append(tk);
                        }
                        continue;
                    }
                }
            }
        }

        /*
        if ( _extraExecSqlEnabled ) {
            if ( tk.is(T_IDENTIFIER) ) {
                switch ( execSqlState ) {
                case 0:
                    if ( !spell.compare(QLatin1String("exec"), Qt::CaseInsensitive) ) {
                        execSqlState++;
                        tk.f.kind = T_SQL_SQL;
                    }
                    else {
                        execSqlState = -1;
                    }
                    break;
                case 1:
                    if ( !spell.compare(QLatin1String("sql"), Qt::CaseInsensitive) ) {
                        execSqlState++;
                        tk.f.kind = T_SQL_EXEC;
                    }
                    else {
                        execSqlState = -1;
                    }
                    break;
                case 2:
                    castExecSqlIdentifier(&tk, spell);
                    break;
                }
            }
            else if ( tk.is(T_SEMICOLON) || tk.is(T_LBRACE) || tk.is(T_RBRACE) ) {
                execSqlState = 0;
            }
        } */

        if (tk.f.newline && tk.is(T_POUND))
            inPreproc = true;
        else if (inPreproc && tokens.size() == 1 &&
                 (tk.is(T_IDENTIFIER) || tk.is(T_SQL_INCLUDE)) &&
                 spell == QLatin1String("include")) {
            lex.setScanAngleStringLiteralTokens(true);
            tk.f.kind = T_IDENTIFIER;
        }
        else if (_objCEnabled
                 && inPreproc && tokens.size() == 1 && tk.is(T_IDENTIFIER) &&
                 spell == QLatin1String("import"))
            lex.setScanAngleStringLiteralTokens(true);

        tokens.append(tk);
    }

    _lastState = lex.state();
    if ( inPreproc )
        _lastSqlState = Lexer::SqlState_StartDeclaration;
    else
        _lastSqlState = lex.sqlState();
    return tokens;
}

int SimpleLexer::tokenAt(const QList<Token> &tokens, unsigned offset)
{
    for (int index = tokens.size() - 1; index >= 0; --index) {
        const Token &tk = tokens.at(index);
        if (tk.begin() <= offset && tk.end() >= offset)
            return index;
    }

    return -1;
}

Token SimpleLexer::tokenAt(const QString &text,
                           unsigned offset,
                           int state,
                           bool qtMocRunEnabled)
{
    SimpleLexer tokenize;
    tokenize.setQtMocRunEnabled(qtMocRunEnabled);
    const QList<Token> tokens = tokenize(text, state);
    const int tokenIdx = tokenAt(tokens, offset);
    return (tokenIdx == -1) ? Token() : tokens.at(tokenIdx);
}

bool SimpleLexer::inBracesOrIncomment(const QString &text, unsigned offset, int state)
{
    SimpleLexer tokenize;
    tokenize.setQtMocRunEnabled(false);
    const QList<Token> tokens = tokenize(text, state);
    const int tokenIdx = tokenAt(tokens, offset);
    if ( tokenIdx == -1 )
        return false;
    if ( tokens.at(tokenIdx).isComment() )
        return true;

    int braceDepth = state >> 8;
    for (int i = 0; i < tokenIdx; i++) {
        if ( tokens.at(i).is(T_LBRACE) )
            braceDepth++;
        else if ( tokens.at(i).is(T_RBRACE) && braceDepth > 0 )
            braceDepth--;
    }

    return (braceDepth > 0 || ((state & 0xf0) >> 4) > 2);
}

int SimpleLexer::skipFirstComment(QTextDocument *doc)
{
    int line = 0;

    QTextBlock block = doc->findBlockByLineNumber(line);
    if ( !block.isValid() )
        return line;
    QString lineText = block.text().trimmed();
    if ( lineText.isEmpty() ) {
        line++;
        block = doc->findBlockByLineNumber(line);
        if ( !block.isValid() )
            return line - 1;
    }
    if ( lineText.startsWith("//") ) {
        line++;
        while (1) {
            block = doc->findBlockByLineNumber(line);
            if ( !block.isValid() )
                return line;
            lineText = block.text().trimmed();
            if ( !lineText.startsWith("//") )
                return line;
            line++;
        }
    } else if ( lineText.startsWith("/*") ) {
        while (1) {
            line++;
            if ( lineText.indexOf("*/", 2) != -1 )
                return line;
            block = doc->findBlockByLineNumber(line);
            if ( !block.isValid() )
                return line;
            lineText = block.text();
        }
    }

    return line;
}

int SimpleLexer::tokenBefore(const QList<Token> &tokens, unsigned offset)
{
    for (int index = tokens.size() - 1; index >= 0; --index) {
        const Token &tk = tokens.at(index);
        if (tk.begin() <= offset)
            return index;
    }

    return -1;
}
