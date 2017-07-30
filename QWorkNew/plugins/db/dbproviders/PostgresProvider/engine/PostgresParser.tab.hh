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
   OptRoleListAST               *optRoleList;
   RoleIdListAST                *roleIdList;
   VarValueListAST              *varValueList;
   TransactionModeListAST       *transModeList;
   NameListAST                  *nameList;
   AlterTableCmdListAST         *alterCmdList;
   ArrBoundListAST              *arrBoundsList;
   GenericOptionListAST         *optionList;
   ExpressionListAST            *exprList;
   TypeListAST                  *typeList;
   IndirectionListAST           *indirectList;
   OrderExprListAST             *orderList;
   ColumnExpressionListAST      *colExprList;
   CommonTableExprListAST       *tabExprList;
   LockingExprListAST           *lockSpecList;
   TableIdentifierListAST       *tableIdentList;
   IndexIdentifierListAST       *indexIdentList;
   SequenceIdentifierListAST    *seqIdentList;
   SelectTargetListAST          *selTargetList;
   TableRefListAST              *tableRefList;
   FunctionArgExprListAST       *funcArgList;
   XmlAttributeListAST          *xmlAttrList;
   TableFuncElementListAST      *funcElemList;
   WindowDefinitionListAST      *winDefList;
   CaseWhenClauseListAST        *caseWhenList;
   ArrayExprListAST             *arrExprList;
   ColumnConstraintListAST      *colConstrList;
   DefinitionElemListAST        *defList;
   RelOptionListAST             *relOptList;
   ConstraintAttributeListAST   *constrAttrList;
   ExclusionConstraintListAST   *exclConstrList;
   AlterGenericOptionListAST    *altGenOptList;
   ColumnRefListAST             *columnRefList;
   SetClauseListAST             *updSetList;
   EventTriggerWhenListAST      *evtTriggerList;
   EventTriggerValueListAST     *evtTrgValueList;
   AlterDbOptionListAST         *alterDbOptList;
   DefACLOptionListAST          *defACLOptList;
   GranteeListAST               *granteeList;
   CommonPrivilegeListAST       *privsList;
   UpdateExtensionCmdListAST    *updExtList;
   FunctionArgListAST           *fncArgList;
   FdwOptionListAST             *fdwOptList;
   AlterFunctionOptionListAST   *altFuncOptList;
   SequenceOptionListAST        *seqOptList;
   AlterTypeCommandListAST      *altTypeCmdList;
   AnyIdentifierListAST         *anyIdentList;
   ConstraintIdentifierListAST  *constrIdentList;
   CopyOptionListAST            *copyOptList;
   GenericCopyOptionListAST     *genCopyOptList;
   GenericCopyOptionCmpArgItemListAST  *genCopyArgList;
   CreateExtOptionListAST       *createExtOptList;
   TableElementListAST          *tableElemList;
   TableLikeOptionListAST       *tabLikeOptList;
   CreateFuncOptionListAST      *creFuncOptList;
   FuncArgWithDefaultListAST    *funcArgDefList;
   TableFuncColumnListAST       *tabFuncColList;
   OpClassListAST               *opClassList;
   OpClassDropListAST           *opClassDropList;
   StatementListAST             *stmtList;
   TypedTableElementListAST     *typElemList;
   TriggerEventListAST          *trgEventList;
   TriggerFuncArgListAST        *trgFuncArgList;
   CreateDbOptionListAST        *creDbOptList;
   EnumValueListAST             *enumValList;
   AggrDefinitionListAST        *aggrDefList;
   DoStmtItemListAST            *doStmtItemList;
   ExplainOptionListAST         *explOptList;
   FunctionWithArgTypesListAST  *funcArgTypeList;
   NumericListAST               *numList;
   IndexElementListAST          *indElemList;
   RelationExprListAST          *relExprList;
   VacuumOptionListAST          *vacuumOptList;


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
        T_NULLS_FIRST = 258,
        T_NULLS_LAST = 259,
        T_WITH_TIME = 260,
        T_PARAM = 261,
        T_IDENTIFIER = 262,
        T_DIGIT = 263,
        T_CHARSTR = 264,
        T_STRSTR = 265,
        T_AMPER = 266,
        T_CARET = 267,
        T_COLON = 268,
        T_TYPECAST = 269,
        T_ASSIGN = 270,
        T_COMMA = 271,
        T_SLASH = 272,
        T_DOT = 273,
        T_DOTDOT = 274,
        T_EQUAL = 275,
        T_EXCLAIM = 276,
        T_EXCLAIM_EQUAL = 277,
        T_GREATER = 278,
        T_GREATER_EQUAL = 279,
        T_LEFT_BRACE = 280,
        T_LEFT_BRACKET = 281,
        T_LESS = 282,
        T_LESS_EQUAL = 283,
        T_LESS_GREATER = 284,
        T_LEFT_PAREN = 285,
        T_DASH = 286,
        T_PERCENT = 287,
        T_PIPE = 288,
        T_PIPE_PIPE = 289,
        T_PLUS = 290,
        T_RIGHT_BRACE = 291,
        T_RIGHT_BRACKET = 292,
        T_RIGHT_PAREN = 293,
        T_SEMICOLON = 294,
        T_STAR = 295,
        T_TILDA = 296,
        T_ABORT = 297,
        T_ABSOLUTE = 298,
        T_ACCESS = 299,
        T_ACTION = 300,
        T_ADD = 301,
        T_ADMIN = 302,
        T_AFTER = 303,
        T_AGGREGATE = 304,
        T_ALL = 305,
        T_ALSO = 306,
        T_ALTER = 307,
        T_ALWAYS = 308,
        T_ANALYSE = 309,
        T_ANALYZE = 310,
        T_AND = 311,
        T_ANY = 312,
        T_ARRAY = 313,
        T_AS = 314,
        T_ASC = 315,
        T_ASSERTION = 316,
        T_ASSIGNMENT = 317,
        T_ASYMMETRIC = 318,
        T_AT = 319,
        T_ATTRIBUTE = 320,
        T_AUTHORIZATION = 321,
        T_BACKWARD = 322,
        T_BEFORE = 323,
        T_BEGIN = 324,
        T_BETWEEN = 325,
        T_BIGINT = 326,
        T_BIGSERIAL = 327,
        T_BINARY = 328,
        T_BIT = 329,
        T_BOOL = 330,
        T_BOOLEAN = 331,
        T_BOTH = 332,
        T_BOX = 333,
        T_BY = 334,
        T_BYTEA = 335,
        T_CACHE = 336,
        T_CALLED = 337,
        T_CASCADE = 338,
        T_CASCADED = 339,
        T_CASE = 340,
        T_CAST = 341,
        T_CATALOG = 342,
        T_CHAIN = 343,
        T_CHAR = 344,
        T_CHARACTER = 345,
        T_CHARACTERISTICS = 346,
        T_CHECK = 347,
        T_CHECKPOINT = 348,
        T_CIDR = 349,
        T_CIRCLE = 350,
        T_CLASS = 351,
        T_CLOSE = 352,
        T_CLUSTER = 353,
        T_COALESCE = 354,
        T_COLLATE = 355,
        T_COLLATION = 356,
        T_COLUMN = 357,
        T_COMMENT = 358,
        T_COMMENTS = 359,
        T_COMMIT = 360,
        T_COMMITTED = 361,
        T_CONCURRENTLY = 362,
        T_CONFIGURATION = 363,
        T_CONNECTION = 364,
        T_CONSTRAINT = 365,
        T_CONSTRAINTS = 366,
        T_CONTENT = 367,
        T_CONTINUE = 368,
        T_CONVERSION = 369,
        T_COPY = 370,
        T_COST = 371,
        T_CREATE = 372,
        T_COUNT = 373,
        T_CROSS = 374,
        T_CSV = 375,
        T_CURRENT = 376,
        T_CURRENT_CATALOG = 377,
        T_CURRENT_DATE = 378,
        T_CURRENT_ROLE = 379,
        T_CURRENT_SCHEMA = 380,
        T_CURRENT_TIME = 381,
        T_CURRENT_TIMESTAMP = 382,
        T_CURRENT_USER = 383,
        T_CURSOR = 384,
        T_CYCLE = 385,
        T_DATA = 386,
        T_DATABASE = 387,
        T_DATE = 388,
        T_DAY = 389,
        T_DEALLOCATE = 390,
        T_DEC = 391,
        T_DECIMAL = 392,
        T_DECLARE = 393,
        T_DEFAULT = 394,
        T_DEFAULTS = 395,
        T_DEFERRABLE = 396,
        T_DEFERRED = 397,
        T_DEFINER = 398,
        T_DELETE = 399,
        T_DELIMITER = 400,
        T_DELIMITERS = 401,
        T_DESC = 402,
        T_DICTIONARY = 403,
        T_DISABLE = 404,
        T_DISCARD = 405,
        T_DISTINCT = 406,
        T_DO = 407,
        T_DOCUMENT = 408,
        T_DOMAIN = 409,
        T_DOUBLE = 410,
        T_DROP = 411,
        T_EACH = 412,
        T_ELSE = 413,
        T_ENABLE = 414,
        T_ENCODING = 415,
        T_ENCRYPTED = 416,
        T_END = 417,
        T_ENUM = 418,
        T_ESCAPE = 419,
        T_EVENT = 420,
        T_EXCEPT = 421,
        T_EXCLUDE = 422,
        T_EXCLUDING = 423,
        T_EXCLUSIVE = 424,
        T_EXECUTE = 425,
        T_EXISTS = 426,
        T_EXPLAIN = 427,
        T_EXTENSION = 428,
        T_EXTERNAL = 429,
        T_EXTRACT = 430,
        T_FALSE = 431,
        T_FAMILY = 432,
        T_FETCH = 433,
        T_FIRST = 434,
        T_FLOAT = 435,
        T_FLOAT4 = 436,
        T_FLOAT8 = 437,
        T_FOLLOWING = 438,
        T_FOR = 439,
        T_FORCE = 440,
        T_FOREIGN = 441,
        T_FORWARD = 442,
        T_FREEZE = 443,
        T_FROM = 444,
        T_FULL = 445,
        T_FUNCTION = 446,
        T_FUNCTIONS = 447,
        T_GLOBAL = 448,
        T_GRANT = 449,
        T_GRANTED = 450,
        T_GREATEST = 451,
        T_GROUP = 452,
        T_HANDLER = 453,
        T_HAVING = 454,
        T_HEADER = 455,
        T_HOLD = 456,
        T_HOUR = 457,
        T_IDENTITY = 458,
        T_IF = 459,
        T_ILIKE = 460,
        T_IMMEDIATE = 461,
        T_IMMUTABLE = 462,
        T_IMPLICIT = 463,
        T_IN = 464,
        T_INCLUDING = 465,
        T_INCREMENT = 466,
        T_INDEX = 467,
        T_INDEXES = 468,
        T_INET = 469,
        T_INHERIT = 470,
        T_INHERITS = 471,
        T_INITIALLY = 472,
        T_INLINE = 473,
        T_INNER = 474,
        T_INOUT = 475,
        T_INPUT = 476,
        T_INSENSITIVE = 477,
        T_INSERT = 478,
        T_INSTEAD = 479,
        T_INT = 480,
        T_INT2 = 481,
        T_INT4 = 482,
        T_INT8 = 483,
        T_INTEGER = 484,
        T_INTERSECT = 485,
        T_INTERVAL = 486,
        T_INTO = 487,
        T_INVOKER = 488,
        T_IS = 489,
        T_ISNULL = 490,
        T_ISOLATION = 491,
        T_JOIN = 492,
        T_JSON = 493,
        T_KEY = 494,
        T_LABEL = 495,
        T_LANGUAGE = 496,
        T_LARGE = 497,
        T_LAST = 498,
        T_LATERAL = 499,
        T_LC_COLLATE = 500,
        T_LC_CTYPE = 501,
        T_LEADING = 502,
        T_LEAKPROOF = 503,
        T_LEAST = 504,
        T_LEFT = 505,
        T_LEVEL = 506,
        T_LIKE = 507,
        T_LINE = 508,
        T_LIMIT = 509,
        T_LISTEN = 510,
        T_LOAD = 511,
        T_LOCAL = 512,
        T_LOCALTIME = 513,
        T_LOCALTIMESTAMP = 514,
        T_LOCATION = 515,
        T_LOCK = 516,
        T_LSEG = 517,
        T_MACADDR = 518,
        T_MAPPING = 519,
        T_MATCH = 520,
        T_MATERIALIZED = 521,
        T_MAX = 522,
        T_MAXVALUE = 523,
        T_MIN = 524,
        T_MINUTE = 525,
        T_MINVALUE = 526,
        T_MODE = 527,
        T_MONEY = 528,
        T_MONTH = 529,
        T_MOVE = 530,
        T_NAME = 531,
        T_NAMES = 532,
        T_NATIONAL = 533,
        T_NATURAL = 534,
        T_NCHAR = 535,
        T_NEXT = 536,
        T_NO = 537,
        T_NONE = 538,
        T_NOT = 539,
        T_NOTHING = 540,
        T_NOTIFY = 541,
        T_NOTNULL = 542,
        T_NOWAIT = 543,
        T_NULL = 544,
        T_NULLIF = 545,
        T_NULLS = 546,
        T_NUMERIC = 547,
        T_OBJECT = 548,
        T_OF = 549,
        T_OFF = 550,
        T_OFFSET = 551,
        T_OIDS = 552,
        T_ON = 553,
        T_ONLY = 554,
        T_OPERATOR = 555,
        T_OPTION = 556,
        T_OPTIONS = 557,
        T_OR = 558,
        T_ORDER = 559,
        T_OUT = 560,
        T_OUTER = 561,
        T_OVER = 562,
        T_OVERLAPS = 563,
        T_OVERLAY = 564,
        T_OWNED = 565,
        T_OWNER = 566,
        T_PARSER = 567,
        T_PARTIAL = 568,
        T_PARTITION = 569,
        T_PASSING = 570,
        T_PASSWORD = 571,
        T_PATH = 572,
        T_PLACING = 573,
        T_PLANS = 574,
        T_POINT = 575,
        T_POLYGON = 576,
        T_POSITION = 577,
        T_PRECEDING = 578,
        T_PRECISION = 579,
        T_PRESERVE = 580,
        T_PREPARE = 581,
        T_PREPARED = 582,
        T_PRIMARY = 583,
        T_PRIOR = 584,
        T_PRIVILEGES = 585,
        T_PROCEDURAL = 586,
        T_PROCEDURE = 587,
        T_PROGRAM = 588,
        T_QUOTE = 589,
        T_RANGE = 590,
        T_READ = 591,
        T_REAL = 592,
        T_REASSIGN = 593,
        T_RECHECK = 594,
        T_RECURSIVE = 595,
        T_REF = 596,
        T_REFERENCES = 597,
        T_REFRESH = 598,
        T_REINDEX = 599,
        T_RELATIVE = 600,
        T_RELEASE = 601,
        T_RENAME = 602,
        T_REPEATABLE = 603,
        T_REPLACE = 604,
        T_REPLICA = 605,
        T_RESET = 606,
        T_RESTART = 607,
        T_RESTRICT = 608,
        T_RETURNING = 609,
        T_RETURNS = 610,
        T_REVOKE = 611,
        T_RIGHT = 612,
        T_ROLE = 613,
        T_ROLLBACK = 614,
        T_ROW = 615,
        T_ROWS = 616,
        T_RULE = 617,
        T_SAVEPOINT = 618,
        T_SCHEMA = 619,
        T_SCROLL = 620,
        T_SEARCH = 621,
        T_SECOND = 622,
        T_SECURITY = 623,
        T_SELECT = 624,
        T_SEQUENCE = 625,
        T_SEQUENCES = 626,
        T_SERIAL = 627,
        T_SERIAL2 = 628,
        T_SERIAL4 = 629,
        T_SERIAL8 = 630,
        T_SERIALIZABLE = 631,
        T_SERVER = 632,
        T_SESSION = 633,
        T_SESSION_USER = 634,
        T_SET = 635,
        T_SETOF = 636,
        T_SHARE = 637,
        T_SHOW = 638,
        T_SIMILAR = 639,
        T_SIMPLE = 640,
        T_SMALLINT = 641,
        T_SMALLSERIAL = 642,
        T_SNAPSHOT = 643,
        T_SOME = 644,
        T_STABLE = 645,
        T_STANDALONE = 646,
        T_START = 647,
        T_STATEMENT = 648,
        T_STATISTICS = 649,
        T_STDIN = 650,
        T_STDOUT = 651,
        T_STORAGE = 652,
        T_STRICT = 653,
        T_STRIP = 654,
        T_SUBSTRING = 655,
        T_SUM = 656,
        T_SYMMETRIC = 657,
        T_SYSID = 658,
        T_SYSTEM = 659,
        T_TABLE = 660,
        T_TABLES = 661,
        T_TABLESPACE = 662,
        T_TEMP = 663,
        T_TEMPLATE = 664,
        T_TEMPORARY = 665,
        T_TEXT = 666,
        T_THEN = 667,
        T_TIME = 668,
        T_TIMESTAMP = 669,
        T_TIMESTAMPTZ = 670,
        T_TIMETZ = 671,
        T_TO = 672,
        T_TRAILING = 673,
        T_TRANSACTION = 674,
        T_TREAT = 675,
        T_TRIGGER = 676,
        T_TRIM = 677,
        T_TRUE = 678,
        T_TRUNCATE = 679,
        T_TRUSTED = 680,
        T_TSQUERY = 681,
        T_TSVECTOR = 682,
        T_TYPE = 683,
        T_TYPES = 684,
        T_TXID_SNAPSHOT = 685,
        T_UNBOUNDED = 686,
        T_UNCOMMITTED = 687,
        T_UNENCRYPTED = 688,
        T_UNION = 689,
        T_UNIQUE = 690,
        T_UNKNOWN = 691,
        T_UNLISTEN = 692,
        T_UNLOGGED = 693,
        T_UNTIL = 694,
        T_UPDATE = 695,
        T_USER = 696,
        T_USING = 697,
        T_UUID = 698,
        T_VACUUM = 699,
        T_VALID = 700,
        T_VALIDATE = 701,
        T_VALIDATOR = 702,
        T_VALUE = 703,
        T_VALUES = 704,
        T_VARBIT = 705,
        T_VARCHAR = 706,
        T_VARIADIC = 707,
        T_VARYING = 708,
        T_VERBOSE = 709,
        T_VERSION = 710,
        T_VIEW = 711,
        T_VOLATILE = 712,
        T_WHEN = 713,
        T_WHERE = 714,
        T_WHITESPACE = 715,
        T_WINDOW = 716,
        T_WITH = 717,
        T_WITHOUT = 718,
        T_WORK = 719,
        T_WRAPPER = 720,
        T_WRITE = 721,
        T_XML = 722,
        T_XMLATTRIBUTES = 723,
        T_XMLCONCAT = 724,
        T_XMLELEMENT = 725,
        T_XMLEXISTS = 726,
        T_XMLFOREST = 727,
        T_XMLPARSE = 728,
        T_XMLPI = 729,
        T_XMLROOT = 730,
        T_XMLSERIALIZE = 731,
        T_YEAR = 732,
        T_YES = 733,
        T_ZONE = 734,
        POSTFIXOP = 735,
        Op = 736,
        UMINUS = 737
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
  static const int yypact_[];

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
      yylast_ = 88202,           //< Last index in yytable_.
      yynnts_ = 571,  //< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 675, //< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 483    //< Number of tokens.
    };


    // User arguments.
    PostgresScanner  &scanner;
    PostgresDriver  &driver;
  };



} // Postgres





#endif // !YY_YY_POSTGRESPARSER_TAB_HH_INCLUDED
