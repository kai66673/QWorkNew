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

#ifndef SQL_TRANSLATIONUNIT_H
#define SQL_TRANSLATIONUNIT_H

#include <QTextCursor>

#include "sqlengine_global.h"
#include "ASTfwd.h"
#include "SqlToken.h"
#include "MemoryPool.h"
#include "DiagnosticClient.h"
#include "SqlStatement.h"
#include "IParserEngine.h"
#include "ILink.h"
#include "sql_constants.h"

#include <cstdio>
#include <vector>

class ITextDocument;

namespace Sql {

class Control;


class SQL_EXPORT TranslationUnit
{
    TranslationUnit(const TranslationUnit &other);
    void operator =(const TranslationUnit &other);

public:
    TranslationUnit(Control *control, const StringLiteral *fileId);
    ~TranslationUnit();

    void walk( BaseWalkReflector *reflector );

    Control *control() const;

    const StringLiteral *fileId() const;
    const char *fileName() const;
    unsigned fileNameLength() const;

    const char *firstSourceChar() const;
    const char *lastSourceChar() const;
    unsigned sourceLength() const;

    void setSource(const char *source, unsigned size);

    unsigned tokenCount() const;
    const Token &tokenAt(unsigned index) const;
    int tokenKind(unsigned index) const;
    int tokenKeyword(unsigned index) const;
    int tokenBuiltinFunction(unsigned index) const;
    int tokenTypeSpecifier(unsigned index) const;
    const char *spell(unsigned index) const;

//    unsigned matchingBrace(unsigned index) const;
    const Identifier *identifier(unsigned index) const;
    const Literal *literal(unsigned index) const;
    const StringLiteral *stringLiteral(unsigned index) const;
    const NumericLiteral *numericLiteral(unsigned index) const;

    Utils::MemoryPool *memoryPool() const;

    bool blockErrors() const { return f._blockErrors; }
    bool blockErrors(bool block);

    void warning(unsigned index, const char *fmt, ...);
    void warningString( unsigned index, const QString &text );
    void error(unsigned index, const char *fmt, ...);
    void errorString( unsigned index, const QString &text );
    void errorString( unsigned startIndex, unsigned endIndex, const QString &text );
    void fatal(unsigned index, const char *fmt, ...);

    void message(DiagnosticClient::Level level, unsigned index,
                 const char *format, va_list ap);
    void messageString( DiagnosticClient::Level level, unsigned index, const QString &text );
    void messageDiaposonString( DiagnosticClient::Level level, unsigned startIndex, unsigned endIndex, const QString &text );

    bool isTokenized() const;
    void tokenize( IParserEngine *engine );

    bool skipFunctionBody() const;
    void setSkipFunctionBody(bool skipFunctionBody);

    bool isParsed() const;

    enum ParseMode {
        ParseTranlationUnit,
//        ParseDeclaration,
//        ParseExpression,
//        ParseDeclarator,
        ParseStatement,
        ParseDmlStatement
    };

    void parseDocument( ParseTarget target, IParserEngine *engine );
    void fillSqlStatements( QList<SqlStatement *> *statements, IParserEngine *engine,
                            Database::ExecuteType executeType, ITextDocument *textDocument);
    void fillStatementFromText( QList<SqlStatement *> *statements, IParserEngine *engine,
                                const QString &statementText );
    int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, IParserEngine *engine,
                         unsigned position, int startPosition, unsigned completionOperator );
    Core::ILink *findLinkAt( const QTextCursor &tc, IParserEngine *engine );

    void clear();
    void release();

    void getTokenStartPosition(unsigned index, unsigned *line,
                               unsigned *column = 0,
                               const StringLiteral **fileName = 0) const;

    void getTokenEndPosition(unsigned index, unsigned *line,
                             unsigned *column = 0,
                             const StringLiteral **fileName = 0) const;

    void getPosition(unsigned offset,
                     unsigned *line,
                     unsigned *column = 0,
                     const StringLiteral **fileName = 0) const;

    void getTokenPosition(unsigned index,
                          unsigned *line,
                          unsigned *column = 0,
                          const StringLiteral **fileName = 0) const;

    void pushLineOffset(unsigned offset) { _lineOffsets.push_back(offset); }
    void pushPreprocessorLine(unsigned offset,
                              unsigned line,
                              const StringLiteral *fileName);

    unsigned getTokenOffset( unsigned index ) const { return _tokens->at(index).begin(); }

    unsigned findPreviousLineOffset(unsigned tokenIndex) const;

    std::vector<Token> *tokens() { return _tokens; }

private:
    unsigned findLineNumber(unsigned offset) const;
    unsigned findColumnNumber(unsigned offset, unsigned lineNumber) const;
    void showErrorLine(unsigned index, unsigned column, FILE *out);

    Control *_control;
    const StringLiteral *_fileId;
    const char *_firstSourceChar;
    const char *_lastSourceChar;
    std::vector<Token> *_tokens;
    std::vector<unsigned> _lineOffsets;
    Utils::MemoryPool *_pool;
    BaseWalker *m_walker;
    TranslationUnit *_previousTranslationUnit;
    struct Flags {
        unsigned _tokenized: 1;
        unsigned _parsed: 1;
        unsigned _blockErrors: 1;
        unsigned _skipFunctionBody: 1;
    };
    union {
        unsigned _flags;
        Flags f;
    };
};

} // namespace Sql


#endif // SQL_TRANSLATIONUNIT_H
