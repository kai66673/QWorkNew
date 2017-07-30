// A Bison parser, made by GNU Bison 3.0.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

/**
 ** \file PostgresParser.tab.hh
 ** Define the Postgres::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_POSTGRESPARSER_TAB_HH_INCLUDED
# define YY_YY_POSTGRESPARSER_TAB_HH_INCLUDED
// //                    "%code requires" blocks.


  #include "PostgresASTfwd.h"

   namespace Postgres {
      class PostgresDriver;
      class PostgresScanner;
   }




# include <vector>
# include <iostream>
# include <stdexcept>
# include <string>
# include "stack.hh"



/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace Postgres {






  /// A Bison parser.
  class PostgresParser
  {
  public:
#ifndef YYSTYPE
    /// Symbol semantic values.
    union semantic_type
    {
    

   int location;
   AST *astNode;
   ValueExpressionListAST       *exprList;
   BaseSelectColumnListAST      *colList;
   TableReferenceListAST        *tableRefList;
   OrderingTermListAST          *orderTermList;
   ColumnIdentifierListAST      *colIdentList;
   CaseExpressionClauseListAST  *caseExprList;
   CaseConditionClauseListAST   *caseCondList;
   AssignmentExpressionListAST  *updateSetList;
   ColumnConstraintListAST      *colConstrList;
   NameListAST                  *nameList;


    };
#else
    typedef YYSTYPE semantic_type;
#endif

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m);
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        T_EOF = 0,
        T_IDENTIFIER = 258,
        T_DIGIT = 259,
        T_CHARSTR = 260,
        T_STRSTR = 261,
        T_AMPER = 262,
        T_CARET = 263,
        T_COLON = 264,
        T_ASSIGN = 265,
        T_COMMA = 266,
        T_SLASH = 267,
        T_DOT = 268,
        T_DOTDOT = 269,
        T_EQUAL = 270,
        T_EXCLAIM = 271,
        T_EXCLAIM_EQUAL = 272,
        T_GREATER = 273,
        T_GREATER_EQUAL = 274,
        T_LEFT_BRACE = 275,
        T_LEFT_BRACKET = 276,
        T_LESS = 277,
        T_LESS_EQUAL = 278,
        T_LESS_GREATER = 279,
        T_LEFT_PAREN = 280,
        T_DASH = 281,
        T_PERCENT = 282,
        T_PIPE = 283,
        T_PIPE_PIPE = 284,
        T_PLUS = 285,
        T_RIGHT_BRACE = 286,
        T_RIGHT_BRACKET = 287,
        T_RIGHT_PAREN = 288,
        T_SEMICOLON = 289,
        T_STAR = 290,
        T_AS = 291,
        T_AT = 292,
        T_BY = 293,
        T_DO = 294,
        T_GO = 295,
        T_IF = 296,
        T_IN = 297,
        T_IS = 298,
        T_NO = 299,
        T_OF = 300,
        T_ON = 301,
        T_OR = 302,
        T_TO = 303,
        T_ADD = 304,
        T_ALL = 305,
        T_AND = 306,
        T_ANY = 307,
        T_ARE = 308,
        T_ASC = 309,
        T_DEC = 310,
        T_END = 311,
        T_FOR = 312,
        T_GET = 313,
        T_KEY = 314,
        T_MAP = 315,
        T_NEW = 316,
        T_NOT = 317,
        T_OLD = 318,
        T_OUT = 319,
        T_REF = 320,
        T_ROW = 321,
        T_SET = 322,
        T_SQL = 323,
        T_BOTH = 324,
        T_CALL = 325,
        T_CASE = 326,
        T_CUBE = 327,
        T_DATA = 328,
        T_DESC = 329,
        T_DROP = 330,
        T_EACH = 331,
        T_ELSE = 332,
        T_EXEC = 333,
        T_EXIT = 334,
        T_FREE = 335,
        T_FROM = 336,
        T_FULL = 337,
        T_GOTO = 338,
        T_HOLD = 339,
        T_INTO = 340,
        T_JOIN = 341,
        T_LEFT = 342,
        T_LIKE = 343,
        T_LOOP = 344,
        T_NEXT = 345,
        T_NONE = 346,
        T_NULL = 347,
        T_ONLY = 348,
        T_OPEN = 349,
        T_OVER = 350,
        T_READ = 351,
        T_ROLE = 352,
        T_ROWS = 353,
        T_SETS = 354,
        T_SOME = 355,
        T_THEN = 356,
        T_TRUE = 357,
        T_UNDO = 358,
        T_USER = 359,
        T_VIEW = 360,
        T_WHEN = 361,
        T_WITH = 362,
        T_WORK = 363,
        T_AFTER = 364,
        T_ALTER = 365,
        T_ARRAY = 366,
        T_BEGIN = 367,
        T_CHECK = 368,
        T_CLOSE = 369,
        T_CROSS = 370,
        T_CYCLE = 371,
        T_DEPTH = 372,
        T_DEREF = 373,
        T_FALSE = 374,
        T_FETCH = 375,
        T_GRANT = 376,
        T_GROUP = 377,
        T_INDEX = 378,
        T_INNER = 379,
        T_INOUT = 380,
        T_INPUT = 381,
        T_LARGE = 382,
        T_LEAVE = 383,
        T_LEVEL = 384,
        T_LIMIT = 385,
        T_LOCAL = 386,
        T_MATCH = 387,
        T_MINUS = 388,
        T_NAMES = 389,
        T_ORDER = 390,
        T_OUTER = 391,
        T_PRIOR = 392,
        T_RANGE = 393,
        T_READS = 394,
        T_RIGHT = 395,
        T_SCOPE = 396,
        T_SPACE = 397,
        T_START = 398,
        T_STATE = 399,
        T_TABLE = 400,
        T_TREAT = 401,
        T_UNDER = 402,
        T_UNION = 403,
        T_UNTIL = 404,
        T_USAGE = 405,
        T_USING = 406,
        T_VALUE = 407,
        T_WHERE = 408,
        T_WHILE = 409,
        T_WRITE = 410,
        T_ACTION = 411,
        T_ATOMIC = 412,
        T_BEFORE = 413,
        T_COLUMN = 414,
        T_COMMIT = 415,
        T_CREATE = 416,
        T_CURSOR = 417,
        T_DELETE = 418,
        T_DOMAIN = 419,
        T_ELSEIF = 420,
        T_ENABLE = 421,
        T_EQUALS = 422,
        T_ESCAPE = 423,
        T_EXCEPT = 424,
        T_EXISTS = 425,
        T_FILTER = 426,
        T_GLOBAL = 427,
        T_HAVING = 428,
        T_INSERT = 429,
        T_METHOD = 430,
        T_MODULE = 431,
        T_NOWAIT = 432,
        T_NUMBER = 433,
        T_OPTION = 434,
        T_OUTPUT = 435,
        T_RENAME = 436,
        T_REPEAT = 437,
        T_RESULT = 438,
        T_RETURN = 439,
        T_REVOKE = 440,
        T_ROLLUP = 441,
        T_SCHEMA = 442,
        T_SCROLL = 443,
        T_SEARCH = 444,
        T_SELECT = 445,
        T_SIGNAL = 446,
        T_SIMPLE = 447,
        T_STATIC = 448,
        T_SYSTEM = 449,
        T_UNIQUE = 450,
        T_UNNEST = 451,
        T_UPDATE = 452,
        T_VALUES = 453,
        T_WINDOW = 454,
        T_WITHIN = 455,
        T_BETWEEN = 456,
        T_BREADTH = 457,
        T_CASCADE = 458,
        T_CATALOG = 459,
        T_COLLATE = 460,
        T_COMMENT = 461,
        T_CONNECT = 462,
        T_CURRENT = 463,
        T_DECLARE = 464,
        T_DEFAULT = 465,
        T_DISABLE = 466,
        T_DYNAMIC = 467,
        T_EXECUTE = 468,
        T_FOREIGN = 469,
        T_GENERAL = 470,
        T_HANDLER = 471,
        T_ITERATE = 472,
        T_LATERAL = 473,
        T_LEADING = 474,
        T_LOCATOR = 475,
        T_NATURAL = 476,
        T_PARTIAL = 477,
        T_PREPARE = 478,
        T_PRIMARY = 479,
        T_RELEASE = 480,
        T_REPLACE = 481,
        T_RETURNS = 482,
        T_ROUTINE = 483,
        T_SECTION = 484,
        T_SESSION = 485,
        T_SIMILAR = 486,
        T_TRIGGER = 487,
        T_UNKNOWN = 488,
        T_WITHOUT = 489,
        T_ABSOLUTE = 490,
        T_ALLOCATE = 491,
        T_CASCADED = 492,
        T_CONTINUE = 493,
        T_DEFERRED = 494,
        T_DESCRIBE = 495,
        T_DISTINCT = 496,
        T_EXTERNAL = 497,
        T_FUNCTION = 498,
        T_GROUPING = 499,
        T_IDENTITY = 500,
        T_INTERVAL = 501,
        T_LANGUAGE = 502,
        T_MODIFIES = 503,
        T_OVERLAPS = 504,
        T_PRESERVE = 505,
        T_RELATIVE = 506,
        T_RESIGNAL = 507,
        T_RESTRICT = 508,
        T_ROLLBACK = 509,
        T_SPECIFIC = 510,
        T_SQLSTATE = 511,
        T_TRAILING = 512,
        T_TRUNCATE = 513,
        T_WHENEVER = 514,
        T_ASSERTION = 515,
        T_COLLATION = 516,
        T_CONDITION = 517,
        T_EXCEPTION = 518,
        T_IMMEDIATE = 519,
        T_INDICATOR = 520,
        T_INITIALLY = 521,
        T_INTERSECT = 522,
        T_ISOLATION = 523,
        T_LOCALTIME = 524,
        T_PARAMETER = 525,
        T_PARTITION = 526,
        T_PRECISION = 527,
        T_PROCEDURE = 528,
        T_RECURSIVE = 529,
        T_SAVEPOINT = 530,
        T_SENSITIVE = 531,
        T_STATEMENT = 532,
        T_SYMMETRIC = 533,
        T_TEMPORARY = 534,
        T_ASENSITIVE = 535,
        T_ASYMMETRIC = 536,
        T_CONNECTION = 537,
        T_CONSTRAINT = 538,
        T_DEALLOCATE = 539,
        T_DEFERRABLE = 540,
        T_DESCRIPTOR = 541,
        T_DISCONNECT = 542,
        T_ORDINALITY = 543,
        T_PRIVILEGES = 544,
        T_REFERENCES = 545,
        T_SQLWARNING = 546,
        T_CONSTRAINTS = 547,
        T_CONSTRUCTOR = 548,
        T_DIAGNOSTICS = 549,
        T_INSENSITIVE = 550,
        T_REFERENCING = 551,
        T_SYSTEM_USER = 552,
        T_TRANSACTION = 553,
        T_TRANSLATION = 554,
        T_CURRENT_DATE = 555,
        T_CURRENT_PATH = 556,
        T_CURRENT_ROLE = 557,
        T_CURRENT_TIME = 558,
        T_CURRENT_USER = 559,
        T_SESSION_USER = 560,
        T_SPECIFICTYPE = 561,
        T_SQLEXCEPTION = 562,
        T_AUTHORIZATION = 563,
        T_CORRESPONDING = 564,
        T_DETERMINISTIC = 565,
        T_TIMEZONE_HOUR = 566,
        T_LOCALTIMESTAMP = 567,
        T_TIMEZONE_MINUTE = 568,
        T_CURRENT_TIMESTAMP = 569,
        T_DAY = 570,
        T_MAX = 571,
        T_MIN = 572,
        T_PAD = 573,
        T_SUM = 574,
        T_CAST = 575,
        T_HOUR = 576,
        T_LAST = 577,
        T_SIZE = 578,
        T_COUNT = 579,
        T_FIRST = 580,
        T_FOUND = 581,
        T_MONTH = 582,
        T_MINUTE = 583,
        T_SECOND = 584,
        T_SUBSTRING = 585,
        T_BIGINT = 586,
        T_BIGSERIAL = 587,
        T_BIT = 588,
        T_VARBIT = 589,
        T_BOX = 590,
        T_BYTEA = 591,
        T_CIDR = 592,
        T_CIRCLE = 593,
        T_INET = 594,
        T_JSON = 595,
        T_LINE = 596,
        T_LSEG = 597,
        T_MACADDR = 598,
        T_MONEY = 599,
        T_PATH = 600,
        T_POINT = 601,
        T_POLYGON = 602,
        T_REAL = 603,
        T_SMALLSERIAL = 604,
        T_SERIAL = 605,
        T_TEXT = 606,
        T_TIME = 607,
        T_TIMESTAMP = 608,
        T_TSQUERY = 609,
        T_TSVECTOR = 610,
        T_TXID_SNAPSHOT = 611,
        T_UUID = 612,
        T_XML = 613,
        T_INT2 = 614,
        T_INT4 = 615,
        T_INT8 = 616,
        T_SERIAL2 = 617,
        T_SERIAL4 = 618,
        T_SERIAL8 = 619,
        T_BOOL = 620,
        T_FLOAT8 = 621,
        T_FLOAT4 = 622,
        T_TIMETZ = 623,
        T_TIMESTAMPTZ = 624,
        T_INT = 625,
        T_CHAR = 626,
        T_DATE = 627,
        T_YEAR = 628,
        T_ZONE = 629,
        T_FLOAT = 630,
        T_DOUBLE = 631,
        T_OBJECT = 632,
        T_BOOLEAN = 633,
        T_DECIMAL = 634,
        T_INTEGER = 635,
        T_NUMERIC = 636,
        T_VARCHAR = 637,
        T_VARYING = 638,
        T_SMALLINT = 639,
        T_CHARACTER = 640
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Internal symbol number.
    typedef int symbol_number_type;

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned short int token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v);

      ~basic_symbol ();

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// -1 when this symbol is empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      enum { empty = 0 };

      /// The symbol type.
      /// -1 when this symbol is empty.
      token_number_type type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;


    /// Build a parser object.
    PostgresParser (PostgresScanner  &scanner_yyarg, PostgresDriver  &driver_yyarg);
    virtual ~PostgresParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    PostgresParser (const PostgresParser&);
    PostgresParser& operator= (const PostgresParser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token type, or yyempty_.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         symbol_number_type yytoken) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yylhs     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yylhs);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short int yypact_ninf_;
    static const short int yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (int t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned short int yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short int yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned short int yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned short int yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param s         The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// "empty" when empty.
      symbol_number_type type_get () const;

      enum { empty = 0 };

      /// The state.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    // Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 1055,           //< Last index in yytable_.
      yynnts_ = 97,  //< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 48, //< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 386    //< Number of tokens.
    };


    // User arguments.
    PostgresScanner  &scanner;
    PostgresDriver  &driver;
  };



} // Postgres





#endif // !YY_YY_POSTGRESPARSER_TAB_HH_INCLUDED
