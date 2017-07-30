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

#include "Token.h"
#include "Literals.h"

using namespace CPlusPlus;

static const char *token_names[] = {
    (""), ("<error>"),

    ("<C++ comment>"), ("<C++ doxy comment>"),
    ("<comment>"), ("<doxy comment>"),

    ("<identifier>"), ("<numeric literal>"), ("<char literal>"),
    ("<wide char literal>"), ("<string literal>"), ("<wide char literal>"),
    ("<@string literal>"), ("<angle string literal>"),
    ("<exec sql c$include filename>"),

    ("&"), ("&&"), ("&="), ("->"), ("->*"), ("^"), ("^="), (":"), ("::"),
    (","), ("/"), ("/="), ("."), ("..."), (".*"), ("="), ("=="), ("!"),
    ("!="), (">"), (">="), (">>"), (">>="), ("{"), ("["), ("<"), ("<="),
    ("<<"), ("<<="), ("("), ("-"), ("-="), ("--"), ("%"), ("%="), ("%%"), ("|"),
    ("|="), ("||"), ("+"), ("+="), ("++"), ("#"), ("##"), ("?"), ("}"),
    ("]"), (")"), (";"), ("*"), ("*="), ("~"), ("~="),

    ("asm"), ("auto"), ("bool"), ("break"), ("case"), ("catch"), ("char"),
    ("class"), ("const"), ("const_cast"), ("continue"), ("default"),
    ("delete"), ("do"), ("double"), ("dynamic_cast"), ("else"), ("enum"),
    ("explicit"), ("export"), ("extern"), ("false"), ("float"), ("for"),
    ("friend"), ("goto"), ("if"), ("inline"), ("int"), ("long"),
    ("mutable"), ("namespace"), ("new"), ("operator"), ("private"),
    ("protected"), ("public"), ("register"), ("reinterpret_cast"),
    ("return"), ("short"), ("signed"), ("sizeof"), ("static"),
    ("static_cast"), ("struct"), ("switch"), ("template"), ("this"),
    ("throw"), ("true"), ("try"), ("typedef"), ("typeid"), ("typename"),
    ("union"), ("unsigned"), ("using"), ("virtual"), ("void"),
    ("volatile"), ("wchar_t"), ("while"),

    // gnu
    ("__attribute__"), ("__typeof__"),

    // objc @keywords
    ("@catch"), ("@class"), ("@compatibility_alias"), ("@defs"), ("@dynamic"),
    ("@encode"), ("@end"), ("@finally"), ("@implementation"), ("@interface"),
    ("@not_keyword"), ("@optional"), ("@package"), ("@private"), ("@property"),
    ("@protected"), ("@protocol"), ("@public"), ("@required"), ("@selector"),
    ("@synchronized"), ("@synthesize"), ("@throw"), ("@try"),

    // Qt keywords
    ("SIGNAL"), ("SLOT"), ("Q_SIGNAL"), ("Q_SLOT"), ("signals"), ("slots"),
    ("Q_FOREACH"), ("Q_D"), ("Q_Q"),
    ("Q_INVOKABLE"), ("Q_PROPERTY"), ("T_Q_PRIVATE_PROPERTY"),
    ("Q_INTERFACES"), ("Q_ENUMS"), ("Q_FLAGS"),
    ("Q_PRIVATE_SLOT"), ("Q_DECLARE_INTERFACE"), ("Q_OBJECT"), ("Q_GADGET"),
    
    // SQL_Embedded keywords
    ("AS"), ("BY"), ("IN"), ("IS"), ("OF"), ("ON"), ("OR"), ("TO"),
    ("ALL"), ("AND"), ("ASC"), ("FOR"), ("MAX"), ("MIN"), ("NOT"), ("ROW"), ("SET"), ("SQL"), ("SUM"),
    ("DESC"), ("EXEC"), ("FROM"), ("GOTO"), ("HOLD"), ("INTO"), ("JOIN"), ("LEFT"), ("LIKE"), ("LIST"), ("OPEN"), ("ONLY"), ("READ"), ("ROWS"), ("WITH"),
    ("ALIAS"), ("CLOSE"), ("COUNT"), ("CROSS"), ("FETCH"), ("GROUP"), ("INNER"), ("LIMIT"), ("ORDER"), ("RIGHT"), ("TABLE"), ("WHERE"), ("WRITE"),
    ("CURSOR"), ("DELETE"), ("INSERT"), ("SELECT"), ("SHARED"), ("UPDATE"), ("VALUES"),
    ("BETWEEN"), ("CURRENT"), ("DECLARE"), ("INCLUDE"), ("PREPARE"),
    ("C$INCLUDE"), ("DISTINCT"), ("EXTERNAL"), ("FILENAME"), ("PRESERVE"), ("SQLERROR"), ("WHENEVER"),
    ("EXCLUSIVE"), ("RESERVING"),
    ("SQLWARNING"), ("TRANSACTION"),

    ("<<Unknown>>")
};

Token::Token() :
    flags(0), offset(0), ptr(0)
{
}

Token::~Token()
{
}

void Token::reset()
{
    flags = 0;
    offset = 0;
    ptr = 0;
}

const char *Token::name(int kind)
{ return token_names[kind]; }

const char *Token::spell() const
{
    switch (f.kind) {
    case T_IDENTIFIER:
        return identifier->chars();

    case T_NUMERIC_LITERAL:
    case T_CHAR_LITERAL:
    case T_STRING_LITERAL:
    case T_AT_STRING_LITERAL:
    case T_ANGLE_STRING_LITERAL:
    case T_WIDE_CHAR_LITERAL:
    case T_WIDE_STRING_LITERAL:
        return literal->chars();

    default:
        if ( f.kind <= T_LAST_CPP_KIND )
            return token_names[f.kind];
        break;
    } // switch

    return token_names[T_LAST_CPP_KIND + 1];
}


