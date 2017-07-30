import os
import shutil
from collections import defaultdict

namespace = 'Postgres'
keywords = [
    ("abort",               1),
    ("absolute",            1),
    ("access",              1),
    ("action",              1),
    ("add",                 1),
    ("admin",               1),
    ("after",               1),
    ("aggregate",           1),
    ("all",                 1),
    ("also",                1),
    ("alter",               1),
    ("always",              1),
    ("analyse",             1),
    ("analyze",             1),
    ("and",                 1),
    ("any",                 1),
    ("array",               1),
    ("as",                  1),
    ("asc",                 1),
    ("assertion",           1),
    ("assignment",          1),
    ("asymmetric",          1),
    ("at",                  1),
    ("attribute",           1),
    ("authorization",       1),
    ("backward",            1),
    ("before",              1),
    ("begin",               1),
    ("between",             1),
    ("bigint",              3),
    ("bigserial",           3),
    ("binary",              3),
    ("bit",                 3),
    ("bool",                3),
    ("boolean",             3),
    ("both",                1),
    ("box",                 1),
    ("by",                  3),
    ("bytea",               3),
    ("cache",               1),
    ("called",              1),
    ("cascade",             1),
    ("cascaded",            1),
    ("case",                1),
    ("cast",                2),
    ("catalog",             1),
    ("chain",               1),
    ("char",                3),
    ("character",           3),
    ("characteristics",     1),
    ("check",               1),
    ("checkpoint",          1),
    ("cidr",                3),
    ("circle",              3),
    ("class",               1),
    ("close",               1),
    ("cluster",             1),
    ("coalesce",            1),
    ("collate",             1),
    ("collation",           1),
    ("column",              1),
    ("comment",             1),
    ("comments",            1),
    ("commit",              1),
    ("committed",           1),
    ("concurrently",        1),
    ("configuration",       1),
    ("connection",          1),
    ("constraint",          1),
    ("constraints",         1),
    ("content",             1),
    ("continue",            1),
    ("conversion",          1),
    ("copy",                1),
    ("cost",                1),
    ("create",              1),
    ("count",               2),
    ("cross",               1),
    ("csv",                 1),
    ("current",             1),
    ("current_catalog",     1),
    ("current_date",        1),
    ("current_role",        1),
    ("current_schema",      1),
    ("current_time",        1),
    ("current_timestamp",   1),
    ("current_user",        1),
    ("cursor",              1),
    ("cycle",               1),
    ("data",                1),
    ("database",            1),
    ("date",                3),
    ("day",                 2),
    ("deallocate",          1),
    ("dec",                 3),
    ("decimal",             3),
    ("declare",             1),
    ("default",             1),
    ("defaults",            1),
    ("deferrable",          1),
    ("deferred",            1),
    ("definer",             1),
    ("delete",              1),
    ("delimiter",           1),
    ("delimiters",          1),
    ("desc",                1),
    ("dictionary",          1),
    ("disable",             1),
    ("discard",             1),
    ("distinct",            1),
    ("do",                  1),
    ("document",            1),
    ("domain",              1),
    ("double",              3),
    ("drop",                1),
    ("each",                1),
    ("else",                1),
    ("enable",              1),
    ("encoding",            1),
    ("encrypted",           1),
    ("end",                 1),
    ("enum",                1),
    ("escape",              1),
    ("event",               1),
    ("except",              1),
    ("exclude",             1),
    ("excluding",           1),
    ("exclusive",           1),
    ("execute",             1),
    ("exists",              1),
    ("explain",             1),
    ("extension",           1),
    ("external",            1),
    ("extract",             1),
    ("false",               1),
    ("family",              1),
    ("fetch",               1),
    ("first",               2),
    ("float",               3),
    ("float4",              3),
    ("float8",              3),
    ("following",           1),
    ("for",                 1),
    ("force",               1),
    ("foreign",             1),
    ("forward",             1),
    ("freeze",              1),
    ("from",                1),
    ("full",                1),
    ("function",            1),
    ("functions",           1),
    ("global",              1),
    ("grant",               1),
    ("granted",             1),
    ("greatest",            1),
    ("group",               1),
    ("handler",             1),
    ("having",              1),
    ("header",              1),
    ("hold",                1),
    ("hour",                2),
    ("identity",            1),
    ("if",                  1),
    ("ilike",               1),
    ("immediate",           1),
    ("immutable",           1),
    ("implicit",            1),
    ("in",                  1),
    ("including",           1),
    ("increment",           1),
    ("index",               1),
    ("indexes",             1),
    ("inet",                3),
    ("inherit",             1),
    ("inherits",            1),
    ("initially",           1),
    ("inline",              1),
    ("inner",               1),
    ("inout",               1),
    ("input",               1),
    ("insensitive",         1),
    ("insert",              1),
    ("instead",             1),
    ("int",                 3),
    ("int2",                3),
    ("int4",                3),
    ("int8",                3),
    ("integer",             3),
    ("intersect",           1),
    ("interval",            1),
    ("into",                1),
    ("invoker",             1),
    ("is",                  1),
    ("isnull",              1),
    ("isolation",           1),
    ("join",                1),
    ("json",                3),
    ("key",                 1),
    ("label",               1),
    ("language",            1),
    ("large",               1),
    ("last",                2),
    ("lateral",             1),
    ("lc_collate",          1),
    ("lc_ctype",            1),
    ("leading",             1),
    ("leakproof",           1),
    ("least",               1),
    ("left",                1),
    ("level",               1),
    ("like",                1),
    ("line",                1),
    ("limit",               1),
    ("listen",              1),
    ("load",                1),
    ("local",               1),
    ("localtime",           1),
    ("localtimestamp",      1),
    ("location",            1),
    ("lock",                1),
    ("lseg",                3),
    ("macaddr",             3),
    ("mapping",             1),
    ("match",               1),
    ("materialized",        1),
    ("max",                 2),
    ("maxvalue",            1),
    ("min",                 2),
    ("minute",              2),
    ("minvalue",            1),
    ("mode",                1),
    ("money",               3),
    ("month",               2),
    ("move",                1),
    ("name",                1),
    ("names",               1),
    ("national",            1),
    ("natural",             1),
    ("nchar",               1),
    ("next",                1),
    ("no",                  1),
    ("none",                1),
    ("not",                 1),
    ("nothing",             1),
    ("notify",              1),
    ("notnull",             1),
    ("nowait",              1),
    ("null",                1),
    ("nullif",              1),
    ("nulls",               1),
    ("numeric",             3),
    ("object",              1),
    ("of",                  1),
    ("off",                 1),
    ("offset",              1),
    ("oids",                1),
    ("on",                  1),
    ("only",                1),
    ("operator",            1),
    ("option",              1),
    ("options",             1),
    ("or",                  1),
    ("order",               1),
    ("out",                 1),
    ("outer",               1),
    ("over",                1),
    ("overlaps",            1),
    ("overlay",             1),
    ("owned",               1),
    ("owner",               1),
    ("parser",              1),
    ("partial",             1),
    ("partition",           1),
    ("passing",             1),
    ("password",            1),
    ("path",                3),
    ("placing",             1),
    ("plans",               1),
    ("point",               3),
    ("polygon",             3),
    ("position",            1),
    ("preceding",           1),
    ("precision",           1),
    ("preserve",            1),
    ("prepare",             1),
    ("prepared",            1),
    ("primary",             1),
    ("prior",               1),
    ("privileges",          1),
    ("procedural",          1),
    ("procedure",           1),
    ("program",             1),
    ("quote",               1),
    ("range",               1),
    ("read",                1),
    ("real",                3),
    ("reassign",            1),
    ("recheck",             1),
    ("recursive",           1),
    ("ref",                 1),
    ("references",          1),
    ("refresh",             1),
    ("reindex",             1),
    ("relative",            1),
    ("release",             1),
    ("rename",              1),
    ("repeatable",          1),
    ("replace",             1),
    ("replica",             1),
    ("reset",               1),
    ("restart",             1),
    ("restrict",            1),
    ("returning",           1),
    ("returns",             1),
    ("revoke",              1),
    ("right",               1),
    ("role",                1),
    ("rollback",            1),
    ("row",                 1),
    ("rows",                1),
    ("rule",                1),
    ("savepoint",           1),
    ("schema",              1),
    ("scroll",              1),
    ("search",              1),
    ("second",              2),
    ("security",            1),
    ("select",              1),
    ("sequence",            1),
    ("sequences",           1),
    ("serial",              3),
    ("serial2",             3),
    ("serial4",             3),
    ("serial8",             3),
    ("serializable",        1),
    ("server",              1),
    ("session",             1),
    ("session_user",        1),
    ("set",                 1),
    ("setof",               1),
    ("share",               1),
    ("show",                1),
    ("similar",             1),
    ("simple",              1),
    ("smallint",            3),
    ("smallserial",         3),
    ("snapshot",            1),
    ("some",                1),
    ("stable",              1),
    ("standalone",          1),
    ("start",               1),
    ("statement",           1),
    ("statistics",          1),
    ("stdin",               1),
    ("stdout",              1),
    ("storage",             1),
    ("strict",              1),
    ("strip",               1),
    ("substring",           2),
    ("sum",                 2),
    ("symmetric",           1),
    ("sysid",               1),
    ("system",              1),
    ("table",               1),
    ("tables",              1),
    ("tablespace",          1),
    ("temp",                1),
    ("template",            1),
    ("temporary",           1),
    ("text",                3),
    ("then",                1),
    ("time",                3),
    ("timestamp",           3),
    ("timestamptz",         3),
    ("timetz",              3),
    ("to",                  1),
    ("trailing",            1),
    ("transaction",         1),
    ("treat",               1),
    ("trigger",             1),
    ("trim",                1),
    ("true",                1),
    ("truncate",            1),
    ("trusted",             1),
    ("tsquery",             3),
    ("tsvector",            3),
    ("type",                1),
    ("types",               1),
    ("txid_snapshot",       3),
    ("unbounded",           1),
    ("uncommitted",         1),
    ("unencrypted",         1),
    ("union",               1),
    ("unique",              1),
    ("unknown",             1),
    ("unlisten",            1),
    ("unlogged",            1),
    ("until",               1),
    ("update",              1),
    ("user",                1),
    ("using",               1),
    ("uuid",                3),
    ("vacuum",              1),
    ("valid",               1),
    ("validate",            1),
    ("validator",           1),
    ("value",               1),
    ("values",              1),
    ("varbit",              3),
    ("varchar",             3),
    ("variadic",            1),
    ("varying",             3),
    ("verbose",             1),
    ("version",             1),
    ("view",                1),
    ("volatile",            1),
    ("when",                1),
    ("where",               1),
    ("whitespace",          1),
    ("window",              1),
    ("with",                1),
    ("without",             1),
    ("work",                1),
    ("wrapper",             1),
    ("write",               1),
    ("xml",                 3),
    ("xmlattributes",       1),
    ("xmlconcat",           1),
    ("xmlelement",          1),
    ("xmlexists",           1),
    ("xmlforest",           1),
    ("xmlparse",            1),
    ("xmlpi",               1),
    ("xmlroot",             1),
    ("xmlserialize",        1),
    ("year",                1),
    ("yes",                 1),
    ("zone",                3),
]

root_dir = 'C:/Alex/SqlEngineGenerator'
if not os.path.exists(root_dir):
    os.makedirs(root_dir)

work_dir = root_dir + '/' + namespace
if os.path.exists(work_dir):
    shutil.rmtree(work_dir)
os.makedirs(work_dir)

grammar_content_start = """%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {{{0}}}
%define parser_class_name {{{0}Parser}}

%code requires{{
   namespace {0} {{
      class {0}Driver;
      class {0}Scanner;
   }}
}}

%parse-param {{ {0}Scanner  &scanner  }}
%parse-param {{ {0}Driver  &driver  }}

%code {{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for all driver functions */
   #include "{0}Driver.h"
   #include "{0}AST.h"

#undef yylex
#define yylex scanner.yylex
}}

/* token types */
%union {{
   int          location;
   class AST*   astNode;
}}

%token T_EOF    0     "end of file"

/* Common Tokens */
%token <location> T_IDENTIFIER   "identifier"
%token <location> T_NUMBER       "number constant"
%token <location> T_CHARSTR      "quoted string"
%token <location> T_STRSTR       "double quoted string"

/* Punctuation Tokens */
%token <location> T_AMPER "&"
%token <location> T_CARET "^"
%token <location> T_COLON ":"
%token <location> T_COLON_COLON "::"
%token <location> T_ASSIGN ":="
%token <location> T_COMMA ","
%token <location> T_SLASH "/"
%token <location> T_DOT "."
%token <location> T_DOTDOT ".."
%token <location> T_EQUAL "="
%token <location> T_EXCLAIM "!"
%token <location> T_EXCLAIM_EQUAL "!="
%token <location> T_GREATER ">"
%token <location> T_GREATER_EQUAL ">="
%token <location> T_LEFT_BRACE "{{"
%token <location> T_LEFT_BRACKET "["
%token <location> T_LESS "<"
%token <location> T_LESS_EQUAL "<="
%token <location> T_LESS_GREATER "<>"
%token <location> T_LEFT_PAREN "("
%token <location> T_DASH "-"
%token <location> T_PERCENT "%"
%token <location> T_PIPE "|"
%token <location> T_PIPE_PIPE "||"
%token <location> T_PLUS "+"
%token <location> T_RIGHT_BRACE "}}"
%token <location> T_RIGHT_BRACKET "]"
%token <location> T_RIGHT_PAREN ")"
%token <location> T_SEMICOLON ";"
%token <location> T_STAR "*"
%token <location> T_TILDA "~"

/* Keywords */
""".format(namespace)

grammar_content_end = """
%type <astNode> statement_list statement commit_statement rollback_statement

%%

statement_list_option
    : T_EOF
    | statement_list T_EOF
    ;

statement_list
    : statement
    | statement_list T_SEMICOLON statement
    ;

statement
    : /* Empty Statement */
    | commit_statement
    | rollback_statement
    /* And Many Other Statements... */
    ;

commit_statement
    : T_COMMIT
    ;

rollback_statement
    : T_ROLLBACK
    ;

%%


void {0}::{0}Parser::error( const std::string &err_message )
{{
   std::cerr << "Error: " << err_message << "\\n";
}}
""".format(namespace)

engine_header_content = """#ifndef {1}PARSERENGINE_H
#define {1}PARSERENGINE_H

#include "SqlToken.h"
#include "IParserEngine.h"

namespace {0} {{

class {0}ParserEngine : public Sql::IParserEngine
{{
public:
    {0}ParserEngine();

    virtual QList<Sql::Token> parseTextLine( const QString &text, int state );
    virtual const char *name( const Sql::Token &tk ) const;
    virtual Sql::ISemanticParser *createSemanticParser( Sql::TranslationUnit *unit ) const;

    virtual Sql::ClassifyFunc keyworClassifyFunction();
    virtual bool hasFutureNullIdent() const;
}};

}} // {0}

#endif // {1}PARSERENGINE_H
""".format(namespace, namespace.upper())

engine_source_static_content = """{0}ParserEngine::{0}ParserEngine()
    : IParserEngine()
{{
}}

QList<Sql::Token> {0}ParserEngine::parseTextLine( const QString &text, int state )
{{
    QList<Sql::Token> tokens;

    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    Sql::Lexer lex(firstChar, lastChar, sqlClassify);
    lex.setScanKeywords(true);

    if (state != -1)
        lex.setState(state & 0xff);

    unsigned prevKind = Sql::T_EOF_SYMBOL;
    for (;;) {{
        Sql::Token tk;
        lex(&tk, prevKind);
        if (tk.is(Sql::T_EOF_SYMBOL)) {{
            break;
        }}

        tokens.append(tk);
        prevKind = tk.kind();
    }}

    _lastState = lex.state();

    return tokens;
}}

const char *{0}ParserEngine::name( const Sql::Token &tk ) const
{{
    return tk.spell();
}}

Sql::ISemanticParser *{0}ParserEngine::createSemanticParser( Sql::TranslationUnit *unit ) const
{{
    return new {0}SemanticParser(unit);
}}

Sql::ClassifyFunc {0}ParserEngine::keyworClassifyFunction()
{{
    return sqlClassify;
}}

bool {0}ParserEngine::hasFutureNullIdent() const
{{
    return false;
}}

""".format(namespace)

def writeGrammar():
    with open(work_dir + '/' + namespace + 'Parser.yy', 'w') as fg:
        fg.write(grammar_content_start)
        for k in keywords:
            fg.write('%token <location> T_' + k[0].upper() + ' "' + k[0] + '"\n')
        fg.write(grammar_content_end)

def writeEngineSource():
    keywords.sort(key=lambda k: k[0])
    keywords.sort(key=lambda k: len(k[0]))

    keywords_map = defaultdict(list)
    for k in keywords:
        keywords_map[len(k[0])].append(k)

    # Write Parser-Engine Header
    with open(work_dir + '/' + namespace + 'ParserEngine.h', 'w') as feh:
        feh.write(engine_header_content)

    # Write Parser-Engine Source
    with open(work_dir + '/' + namespace + 'ParserEngine.cpp', 'w') as fes:
      fes.write('#include "{0}ParserEngine.h"\n#include "SqlLexer.h"\n#include "{0}SemanticParser.h"\n#include "{0}Parser.tab.hh"\n\nnamespace {0} {{\n\n'.format(namespace))

      for l in keywords_map.keys():
          fes.write('static inline void sqlClassify' + str(l) + '( const char *s, Sql::Token &tk )\n')
          fes.write('{\n')

          s = keywords_map[l][0][0]
          level = 1
          lines = []
          spaces = ''
          for c in s:
              spaces = ' ' * (level * 2)
              lines.append(spaces + "if ( s[{0}] == '{1}' || s[{0}] == '{2}' ) {{\n".format(level - 1, c.upper(), c))
              level += 1
          spaces = ' ' * (level * 2)
          strTypeId = str(keywords_map[l][0][1])
          lines.append(spaces + 'tk.kf.keytype = ' + strTypeId + ';\n')
          lines.append(spaces + 'tk.kf.keyword' + strTypeId + ' = 1;\n')
          lines.append(spaces + 'tk.kf.keyid = ' + namespace + '::' + namespace + 'Parser::token::T_' + s.upper() + ';\n')
          while level > 1:
              level -= 1
              lines.append(' ' * (level * 2) + "}\n")

          sprev = s
          wi = 1
          while len(keywords_map[l]) > wi:
              s = keywords_map[l][wi][0]
              sl = 1
              while s[sl - 1] == sprev[sl - 1]:
                  sl += 1
              needElse = True
              ind = len(lines) - sl + 1
              level = sl
              while len(s) >= level:
                  c = s[level - 1]
                  elsestr = ""
                  if needElse:
                      elsestr = "else "
                      needElse = False
                  spaces = ' ' * (level * 2)
                  lines.insert(ind, spaces + elsestr + "if ( s[{0}] == '{1}' || s[{0}] == '{2}' ) {{\n".format(level - 1, c.upper(), c))
                  ind += 1
                  level += 1
              strTypeId = str(keywords_map[l][wi][1])
              spaces = ' ' * (level * 2)
              lines.insert(ind, spaces + 'tk.kf.keytype = ' + strTypeId + ';\n')
              ind += 1
              lines.insert(ind, spaces + 'tk.kf.keyword' + strTypeId + ' = 1;\n')
              ind += 1
              lines.insert(ind, spaces + 'tk.kf.keyid = ' + namespace + '::' + namespace + 'Parser::token::T_' + s.upper() + ';\n')
              ind += 1
              while level > sl:
                  level -= 1
                  lines.insert(ind, ' ' * (level * 2) + "}\n")
                  ind += 1
              wi += 1
              sprev = s

          for line in lines:
              fes.write(line)
          fes.write('}\n\n')

      fes.write('void sqlClassify( const char *s, int l, Sql::Token &tk )\n{\n')
      fes.write('  switch (l) {\n')
      for l in keywords_map.keys():
          fes.write('    case {0}: sqlClassify{0}(s, tk); break;\n'.format(l))
      fes.write('  }\n}\n\n')

      fes.write(engine_source_static_content)

      fes.write('} // ' + namespace)

driver_header_content = """#ifndef __{1}DRIVER_H
#define __{1}DRIVER_H

#include <string>

#include "TranslationUnit.h"
#include "{0}Scanner.h"
#include "{0}Parser.tab.hh"

namespace {0} {{

class {0}Driver {{
public:
   {0}Driver( Sql::TranslationUnit *translationUnit )
        : parser(nullptr)
        , scanner(nullptr)
        , m_translationUnit(translationUnit)
   {{ }};

   virtual ~{0}Driver();

   void parse();

private:
   {0}Parser *parser;
   {0}Scanner *scanner;
   Sql::TranslationUnit *m_translationUnit;
}};

}} /* end namespace {0} */

#endif /* END __{1}DRIVER_H */
""".format(namespace, namespace.upper())

driver_source_content = """#include <cctype>
#include <fstream>
#include <cassert>

#include "{0}Driver.h"

namespace {0} {{

{0}Driver::~{0}Driver()
{{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}}

void {0}Driver::parse()
{{
   delete(scanner);
   scanner = new {0}Scanner(m_translationUnit);

   delete(parser);
   parser = new {0}Parser( (*scanner), (*this) );

   parser->parse();
}}

}}   // namespace {0}
""".format(namespace)

def writeDriverSource():
    with open(work_dir + '/' + namespace + 'Driver.h', 'w') as fdh:
        fdh.write(driver_header_content)
    with open(work_dir + '/' + namespace + 'Driver.cpp', 'w') as fds:
        fds.write(driver_source_content)

scanner_header_content = """#ifndef __{1}SCANNER_H
#define __{1}SCANNER_H

#include <vector>

#include "{0}Parser.tab.hh"
#include "TranslationUnit.h"

namespace {0} {{

class {0}Scanner
{{
public:
   {0}Scanner( Sql::TranslationUnit *translationUnit );

   int yylex( {0}Parser::semantic_type *lval )
   {{
      yylval = lval;
      return( yylex() );
   }}

private:
   int yylex();
   Sql::TranslationUnit *m_translationUnit;
   int m_index;
   {0}Parser::semantic_type *yylval;
   std::vector<Sql::Token> *m_tokens;
}};

}} /* end namespace {0} */

#endif /* END __{1}SCANNER_H */
""".format(namespace, namespace.upper())

scanner_source_content = """#include "{0}Scanner.h"

namespace {0} {{

{0}Scanner::{0}Scanner( Sql::TranslationUnit *translationUnit )
   : m_translationUnit(translationUnit)
   , m_index(0)
   , yylval( nullptr )
   , m_tokens(translationUnit->tokens())
{{ }}

int {0}Scanner::yylex()
{{
   m_index++;

   const Sql::Token &tk = m_tokens->at(m_index);
   yylval->location = m_index;

   switch ( tk.kind() ) {{
      case Sql::T_EOF_SYMBOL:       return {0}Parser::token::T_EOF;
      case Sql::T_IDENTIFIER:       return {0}Parser::token::T_IDENTIFIER;

      case Sql::T_NUMERIC_LITERAL:  return {0}Parser::token::T_NUMBER;
      case Sql::T_CHAR_LITERAL:     return {0}Parser::token::T_CHARSTR;
      case Sql::T_STRING_LITERAL:   return {0}Parser::token::T_STRSTR;

      case Sql::T_AMPER:            return {0}Parser::token::T_AMPER;
      case Sql::T_CARET:            return {0}Parser::token::T_CARET;
      case Sql::T_COLON:            return {0}Parser::token::T_COLON;
      case Sql::T_ASSIGN:           return {0}Parser::token::T_ASSIGN;
      case Sql::T_COMMA:            return {0}Parser::token::T_COMMA;
      case Sql::T_SLASH:            return {0}Parser::token::T_SLASH;
      case Sql::T_DOT:              return {0}Parser::token::T_DOT;
      case Sql::T_DOTDOT:           return {0}Parser::token::T_DOTDOT;
      case Sql::T_EQUAL:            return {0}Parser::token::T_EQUAL;
      case Sql::T_EXCLAIM:          return {0}Parser::token::T_EXCLAIM;
      case Sql::T_EXCLAIM_EQUAL:    return {0}Parser::token::T_EXCLAIM_EQUAL;
      case Sql::T_GREATER:          return {0}Parser::token::T_GREATER;
      case Sql::T_GREATER_EQUAL:    return {0}Parser::token::T_GREATER_EQUAL;
      case Sql::T_LBRACE:           return {0}Parser::token::T_LEFT_BRACE;
      case Sql::T_LBRACKET:         return {0}Parser::token::T_LEFT_BRACKET;
      case Sql::T_LESS:             return {0}Parser::token::T_LESS;
      case Sql::T_LESS_EQUAL:       return {0}Parser::token::T_LESS_EQUAL;
      case Sql::T_LESS_GREATER:     return {0}Parser::token::T_LESS_GREATER;
      case Sql::T_LPAREN:           return {0}Parser::token::T_LEFT_PAREN;
      case Sql::T_MINUS:            return {0}Parser::token::T_DASH;
      case Sql::T_PERCENT:          return {0}Parser::token::T_PERCENT;
      case Sql::T_PIPE:             return {0}Parser::token::T_PIPE;
      case Sql::T_PIPE_PIPE:        return {0}Parser::token::T_PIPE_PIPE;
      case Sql::T_PLUS:             return {0}Parser::token::T_PLUS;
      case Sql::T_RBRACE:           return {0}Parser::token::T_RIGHT_BRACE;
      case Sql::T_RBRACKET:         return {0}Parser::token::T_RIGHT_BRACKET;
      case Sql::T_RPAREN:           return {0}Parser::token::T_RIGHT_PAREN;
      case Sql::T_SEMICOLON:        return {0}Parser::token::T_SEMICOLON;
      case Sql::T_STAR:             return {0}Parser::token::T_STAR;

      case Sql::T_KEYWORD:
         if ( int id1 = tk.keyid1() ) {{
            return id1;
         }} else if ( int id2 = tk.keyid2() ) {{
            return id2;
         }} else if ( int id3 = tk.keyid3() ) {{
            return id3;
         }}

      default:
         return {0}Parser::token::T_EOF;
   }}

   return {0}Parser::token::T_EOF;
}}

}}  // namespace {0}
""".format(namespace)

def writeScannerSource():
    with open(work_dir + '/' + namespace + 'Scanner.h', 'w') as fsh:
        fsh.write(scanner_header_content)
    with open(work_dir + '/' + namespace + 'Scanner.cpp', 'w') as fss:
        fss.write(scanner_source_content)

writeGrammar()
writeEngineSource()
writeDriverSource()
writeScannerSource()
