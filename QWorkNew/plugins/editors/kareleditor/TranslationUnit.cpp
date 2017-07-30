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

#include "TranslationUnit.h"
#include "Control.h"
#include "MemoryPool.h"
#include "KarelLexer.h"
#include "KarelParser.h"
#include "KarelAST.h"

#include <stack>
#include <cstdarg>
#include <algorithm>

#ifdef _MSC_VER
#    define va_copy(dst, src) ((dst) = (src))
#elif defined(__INTEL_COMPILER) && !defined(va_copy)
#    define va_copy __va_copy
#endif

namespace Karel {

TranslationUnit::TranslationUnit(Control *control, const StringLiteral *fileId)
    : _control(control)
    , _fileId(fileId)
    , _firstSourceChar(0)
    , _lastSourceChar(0)
    , _pool(0)
    , _ast(0)
    , _flags(0)
{
    _tokens = new std::vector<Token>();
    _previousTranslationUnit = control->switchTranslationUnit(this);
    _pool = new Utils::MemoryPool();
}

TranslationUnit::~TranslationUnit()
{
    (void) _control->switchTranslationUnit(_previousTranslationUnit);
    delete _tokens;
    delete _fileId;
    delete _pool;
}

Control *TranslationUnit::control() const
{ return _control; }

const StringLiteral *TranslationUnit::fileId() const
{ return _fileId; }

const char *TranslationUnit::fileName() const
{ return _fileId->chars(); }

unsigned TranslationUnit::fileNameLength() const
{ return _fileId->size(); }

const char *TranslationUnit::firstSourceChar() const
{ return _firstSourceChar; }

const char *TranslationUnit::lastSourceChar() const
{ return _lastSourceChar; }

unsigned TranslationUnit::sourceLength() const
{ return _lastSourceChar - _firstSourceChar; }

void TranslationUnit::setSource(const char *source, unsigned size)
{
    _firstSourceChar = source;
    _lastSourceChar = source + size;
    f._parsed = false;
    f._tokenized = false;
    _tokens->clear();
}

unsigned TranslationUnit::tokenCount() const
{ return _tokens->size(); }

const Token &TranslationUnit::tokenAt(unsigned index) const
{ return _tokens->at(index); }

int TranslationUnit::tokenKind(unsigned index) const
{ return _tokens->at(index).f.kind; }

int TranslationUnit::tokenKeyword( unsigned index ) const
{ return _tokens->at(index).keyid(); }

const char *TranslationUnit::spell(unsigned index) const
{
    if (! index)
        return 0;

    return _tokens->at(index).spell();
}

const Identifier *TranslationUnit::identifier(unsigned index) const
{ return _tokens->at(index).identifier; }

const Literal *TranslationUnit::literal(unsigned index) const
{ return _tokens->at(index).literal; }

const StringLiteral *TranslationUnit::stringLiteral(unsigned index) const
{ return _tokens->at(index).string; }

const NumericLiteral *TranslationUnit::numericLiteral(unsigned index) const
{ return _tokens->at(index).number; }

unsigned TranslationUnit::matchingBrace(unsigned index) const
{ return _tokens->at(index).close_brace; }

Utils::MemoryPool *TranslationUnit::memoryPool() const
{ return _pool; }

AST *TranslationUnit::ast() const
{ return _ast; }

bool TranslationUnit::isTokenized() const
{ return f._tokenized; }

bool TranslationUnit::isParsed() const
{ return f._parsed; }

bool TranslationUnit::parse(TranslationUnit::ParseMode mode)
{
    if (isParsed())
        return false;

    if (! isTokenized())
        tokenize();

    f._parsed = true;

    Parser parser(this);

    bool parsed = false;

    switch (mode) {
        case TranslationUnit::ParseModule: {
            ModuleAST *node = 0;
            parsed = parser.parseModule(node);
            _ast = node;
            break;
        }
        default:
            break;
    }

    return parsed;
}

void TranslationUnit::tokenize()
{
    if ( isTokenized() )
        return;

    f._tokenized = true;

    Lexer lex(this);

    _tokens->clear();
    _lineOffsets.clear();

    std::stack<unsigned> braces;

    _tokens->push_back(Token()); // the first token needs to be invalid!
    pushLineOffset(0);

    Token tk;

    do {
        lex(&tk);

        if ( !tk.isComment() ) {
            switch (tk.f.kind) {
                case T_LBRACE:
                    braces.push(_tokens->size());
                    break;
                case T_RBRACE:
                    if ( !braces.empty() ) {
                        const unsigned open_brace_index = braces.top();
                        braces.pop();
                        (*_tokens)[open_brace_index].close_brace = _tokens->size();
                    }
                    break;
            }
            _tokens->push_back(tk);
        }
    } while (tk.f.kind);

     _tokens->push_back(Token()); // the last token needs to be invalid!

    for (; ! braces.empty(); braces.pop()) {
        unsigned open_brace_index = braces.top();
        (*_tokens)[open_brace_index].close_brace = _tokens->size();
    }
}

bool TranslationUnit::skipFunctionBody() const
{ return f._skipFunctionBody; }

void TranslationUnit::setSkipFunctionBody(bool skipFunctionBody)
{ f._skipFunctionBody = skipFunctionBody; }


unsigned TranslationUnit::findLineNumber(unsigned offset) const
{
    std::vector<unsigned>::const_iterator it =
        std::lower_bound(_lineOffsets.begin(), _lineOffsets.end(), offset);

    if (it != _lineOffsets.begin())
        --it;

    return it - _lineOffsets.begin();
}

unsigned TranslationUnit::findColumnNumber(unsigned offset, unsigned lineNumber) const
{
    if (! offset)
        return 0;

    return offset - _lineOffsets[lineNumber];
}

void TranslationUnit::getTokenPosition(unsigned index,
                                       unsigned *line,
                                       unsigned *column,
                                       const StringLiteral **fileName) const
{ return getPosition(tokenAt(index).offset, line, column, fileName); }

void TranslationUnit::getTokenStartPosition(unsigned index, unsigned *line,
                                            unsigned *column,
                                            const StringLiteral **fileName) const
{ return getPosition(tokenAt(index).begin(), line, column, fileName); }

void TranslationUnit::getTokenEndPosition(unsigned index, unsigned *line,
                                          unsigned *column,
                                          const StringLiteral **fileName) const
{ return getPosition(tokenAt(index).end(), line, column, fileName); }

void TranslationUnit::getPosition(unsigned tokenOffset,
                                  unsigned *line,
                                  unsigned *column,
                                  const StringLiteral **fileName) const
{
    Q_UNUSED(fileName);

    unsigned lineNumber = findLineNumber(tokenOffset);
    unsigned columnNumber = findColumnNumber(tokenOffset, lineNumber);

    if (line)
        *line = lineNumber;

    if (column)
        *column = columnNumber;
}

bool TranslationUnit::blockErrors(bool block)
{
    bool previous = f._blockErrors;
    f._blockErrors = block;
    return previous;
}

void TranslationUnit::message(DiagnosticClient::Level level, unsigned index, const char *format, va_list args)
{
    if (f._blockErrors)
        return;

    index = std::min(index, tokenCount() - 1);

    unsigned line = 0, column = 0;
    getTokenPosition(index, &line, &column);

    if (DiagnosticClient *client = control()->diagnosticClient()) {
        client->report(level, "", line, column, tokenAt(index).length(), format, args);
    } else {
        fprintf(stderr, "%s:%d: ", "", line);
        const char *l = "error";
        if (level == DiagnosticClient::Warning)
            l = "warning";
        else if (level == DiagnosticClient::Fatal)
            l = "fatal";
        fprintf(stderr, "%s: ", l);

        vfprintf(stderr, format, args);
        fputc('\n', stderr);

        showErrorLine(index, column, stderr);
    }

    if (level == DiagnosticClient::Fatal)
        exit(EXIT_FAILURE);
}

void TranslationUnit::messageString(DiagnosticClient::Level level, unsigned index, const QString &text)
{
    if ( f._blockErrors )
        return;

    unsigned line = 0, column = 0;
    getTokenPosition(index, &line, &column);

    if (DiagnosticClient *client = control()->diagnosticClient()) {
        client->reportString(level, "", line, column, tokenAt(index).length(), text);
    }

    if (level == DiagnosticClient::Fatal)
        exit(EXIT_FAILURE);
}

void TranslationUnit::messageDiaposonString( DiagnosticClient::Level level, unsigned startIndex, unsigned endIndex, const QString &text )
{
    if ( f._blockErrors )
        return;

    unsigned line = 0, column = 0;
    getTokenPosition(startIndex, &line, &column);

    unsigned length = tokenAt(endIndex).offset + tokenAt(endIndex).length() - tokenAt(startIndex).offset;

    if (DiagnosticClient *client = control()->diagnosticClient()) {
        client->reportString(level, "", line, column, length, text);
    }

    if (level == DiagnosticClient::Fatal)
        exit(EXIT_FAILURE);
}

void TranslationUnit::warning(unsigned index, const char *format, ...)
{
    if (f._blockErrors)
        return;

    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    message(DiagnosticClient::Warning, index, format, args);
    va_end(ap);
    va_end(args);
}

void TranslationUnit::warningString( unsigned index, const QString &text )
{
    if ( f._blockErrors )
        return;

    messageString(DiagnosticClient::Warning, index, text);
}

void TranslationUnit::error(unsigned index, const char *format, ...)
{
    if (f._blockErrors)
        return;

    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    message(DiagnosticClient::Error, index, format, args);
    va_end(ap);
    va_end(args);
}

void TranslationUnit::errorString( unsigned index, const QString &text )
{
    if ( f._blockErrors )
        return;

    messageString(DiagnosticClient::Error, index, text);
}

void TranslationUnit::errorString( unsigned startIndex, unsigned endIndex, const QString &text )
{
    if ( f._blockErrors )
        return;

    messageDiaposonString(DiagnosticClient::Error, startIndex, endIndex, text);
}

void TranslationUnit::fatal(unsigned index, const char *format, ...)
{
    if (f._blockErrors)
        return;

    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    message(DiagnosticClient::Fatal, index, format, args);
    va_end(ap);
    va_end(args);
}

unsigned TranslationUnit::findPreviousLineOffset(unsigned tokenIndex) const
{
    unsigned lineOffset = _lineOffsets[findLineNumber(_tokens->at(tokenIndex).offset)];
    return lineOffset;
}

void TranslationUnit::showErrorLine(unsigned index, unsigned column, FILE *out)
{
    unsigned lineOffset = _lineOffsets[findLineNumber(_tokens->at(index).offset)];
    for (const char *cp = _firstSourceChar + lineOffset + 1; *cp && *cp != '\n'; ++cp) {
        fputc(*cp, out);
    }
    fputc('\n', out);

    const char *end = _firstSourceChar + lineOffset + 1 + column - 1;
    for (const char *cp = _firstSourceChar + lineOffset + 1; cp != end; ++cp) {
        if (*cp != '\t')
            fputc(' ', out);
        else
            fputc('\t', out);
    }
    fputc('^', out);
    fputc('\n', out);
}

void TranslationUnit::clear()
{
    delete _pool;
    _pool = 0;
}

void TranslationUnit::release()
{
    clear();
    delete _tokens;
    _tokens = 0;
}

}   // namespace Karel
