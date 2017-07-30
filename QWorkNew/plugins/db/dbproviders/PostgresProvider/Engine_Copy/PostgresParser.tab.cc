// A Bison parser, made by GNU Bison 3.0.

// Skeleton implementation for Bison LALR(1) parsers in C++

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


// First part of user declarations.



# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "PostgresParser.tab.hh"

// User implementation prologue.


// Unqualified %code blocks.


   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for all driver functions */
   #include "PostgresDriver.h"
   #include "PostgresAST.h"

#undef yylex
#define yylex scanner.yylex




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace Postgres {


  /// Build a parser object.
  PostgresParser::PostgresParser (PostgresScanner  &scanner_yyarg, PostgresDriver  &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  PostgresParser::~PostgresParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  PostgresParser::syntax_error::syntax_error (const std::string& m)
    : std::runtime_error (m)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  PostgresParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  PostgresParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
  {
    value = other.value;
  }


  template <typename Base>
  inline
  PostgresParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v)
    : Base (t)
    , value (v)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  PostgresParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  inline
  PostgresParser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  PostgresParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
  }

  // by_type.
  inline
  PostgresParser::by_type::by_type ()
     : type (empty)
  {}

  inline
  PostgresParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  PostgresParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  PostgresParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  PostgresParser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  PostgresParser::by_state::by_state ()
    : state (empty)
  {}

  inline
  PostgresParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  PostgresParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  PostgresParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  PostgresParser::symbol_number_type
  PostgresParser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  PostgresParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  PostgresParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  PostgresParser::stack_symbol_type&
  PostgresParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }


  template <typename Base>
  inline
  void
  PostgresParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  PostgresParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  PostgresParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  PostgresParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  PostgresParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  PostgresParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  PostgresParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  PostgresParser::debug_level_type
  PostgresParser::debug_level () const
  {
    return yydebug_;
  }

  void
  PostgresParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline PostgresParser::state_type
  PostgresParser::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
  PostgresParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  PostgresParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  PostgresParser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* If YYLEN is nonzero, implement the default value of the action:
       '$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYLHS.VALUE to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yylhs.value = yystack_[yylen - 1].value;
    else
      yylhs.value = yystack_[0].value;


    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 3:

    { (yylhs.value.location) = (yystack_[0].value.location); }

    break;

  case 4:

    { (yylhs.value.location) = 0; }

    break;

  case 7:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 8:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 9:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 10:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 11:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 12:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 13:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 14:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 15:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 16:

    { driver.addStatement( (yystack_[0].value.astNode)->asStatement() ); }

    break;

  case 17:

    { driver.addUnknownStatement(); }

    break;

  case 19:

    { (yylhs.value.astNode) = driver.makeAstNode<IfExistsAST>( (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 20:

    { (yylhs.value.location) = 0; }

    break;

  case 22:

    { (yylhs.value.location) = 0; }

    break;

  case 25:

    { (yylhs.value.location) = 0; }

    break;

  case 30:

    { (yylhs.value.astNode) = driver.makeAstNode<AllOrUserTriggersAST>( (yystack_[0].value.location) ); }

    break;

  case 31:

    { (yylhs.value.astNode) = driver.makeAstNode<AllOrUserTriggersAST>( (yystack_[0].value.location) ); }

    break;

  case 32:

    { (yylhs.value.astNode) = driver.makeAstNode<ConcreteTriggerAST>( driver.createName(NameAST::Trigger, (yystack_[0].value.location)) ); }

    break;

  case 34:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CollateClauseAST>( (yystack_[1].value.location),
                                                   driver.makeAstNode<LiteralExpressionAST>((yystack_[0].value.location)) );
      }

    break;

  case 35:

    { (yylhs.value.location) = 0; }

    break;

  case 38:

    { (yylhs.value.astNode) = driver.makeAstNode<DeferrableClauseAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 39:

    { (yylhs.value.astNode) = driver.makeAstNode<DeferrableClauseAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 40:

    { (yylhs.value.astNode) = driver.makeAstNode<DeferrableClauseAST>( (yystack_[1].value.location), (yystack_[0].value.location),  0,  0 ); }

    break;

  case 41:

    { (yylhs.value.astNode) = driver.makeAstNode<DeferrableClauseAST>(  0,  0, (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 42:

    { (yylhs.value.astNode) = driver.makeAstNode<DeferrableClauseAST>(  0,  0, (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 44:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ConstraintDeclarationAST>( (yystack_[1].value.location),
                                                           driver.createName(NameAST::Constraint, (yystack_[0].value.location)) );
      }

    break;

  case 45:

    {
        (yylhs.value.astNode)  = driver.makeAstNode<PkColumnConstraintAST>( (yystack_[3].value.astNode)->asConstraintDeclaration(),
                                                         (yystack_[2].value.location),
                                                         (yystack_[1].value.location),
                                                         (yystack_[0].value.astNode)->asDeferrableClause() );
      }

    break;

  case 46:

    {
        (yylhs.value.astNode) = driver.makeAstNode<NotNullColumnConstraintAST>( (yystack_[3].value.astNode)->asConstraintDeclaration(),
                                                             (yystack_[2].value.location),
                                                             (yystack_[1].value.location),
                                                             (yystack_[0].value.astNode)->asDeferrableClause() );
      }

    break;

  case 47:

    {
        (yylhs.value.astNode) = driver.makeAstNode<UniqueColumnConstraintAST>( (yystack_[2].value.astNode)->asConstraintDeclaration(),
                                                            (yystack_[1].value.location),
                                                            (yystack_[0].value.astNode)->asDeferrableClause() );
      }

    break;

  case 48:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CheckColumnConstraintAST>( (yystack_[5].value.astNode)->asConstraintDeclaration(),
                                                           (yystack_[4].value.location),
                                                           (yystack_[3].value.location),
                                                           (yystack_[2].value.astNode)->asConditionExpression(),
                                                           (yystack_[1].value.location),
                                                           (yystack_[0].value.astNode)->asDeferrableClause());
      }

    break;

  case 49:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ReferencesColumnConstraintAST>( (yystack_[2].value.astNode)->asConstraintDeclaration(),
                                                                (yystack_[1].value.astNode)->asFkReferencesClause(),
                                                                (yystack_[0].value.astNode)->asDeferrableClause() );
      }

    break;

  case 53:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesActionAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 54:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesActionAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 55:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesActionAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location) ); }

    break;

  case 56:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesActionAST>( (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location),  0 ); }

    break;

  case 57:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesActionAST>( (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location),  0 ); }

    break;

  case 59:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesMatchAST>( (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 60:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesMatchAST>( (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 61:

    { (yylhs.value.astNode) = driver.makeAstNode<FkReferencesMatchAST>( (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 62:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FkReferencesClauseAST>( (yystack_[6].value.location),
                                                        (yystack_[5].value.astNode)->asTableIdentifier(),
                                                        (yystack_[4].value.location),
                                                        (yystack_[3].value.nameList),
                                                        (yystack_[2].value.location),
                                                        (yystack_[1].value.astNode)->asFkReferencesMatch(),
                                                        (yystack_[0].value.astNode)->asFkReferencesAction() );
      }

    break;

  case 63:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FkReferencesClauseAST>( (yystack_[1].value.location),
                                                        (yystack_[0].value.astNode)->asTableIdentifier(),
                                                        0,
                                                        (NameListAST *) 0,
                                                        0,
                                                        (FkReferencesMatchAST *) 0,
                                                        (FkReferencesActionAST *) 0 );
      }

    break;

  case 64:

    {
        (yylhs.value.nameList) = (yystack_[2].value.nameList);
        if ( (yystack_[0].value.location) )
          (yylhs.value.nameList) = driver.makeAstNode<NameListAST>( (yystack_[2].value.nameList),
                                                driver.createName(NameAST::Column, (yystack_[0].value.location)) );
      }

    break;

  case 65:

    {
        (yylhs.value.nameList) = 0;
        if ( (yystack_[0].value.location) )
          (yylhs.value.nameList) = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Column, (yystack_[0].value.location)) );
      }

    break;

  case 66:

    {
        (yylhs.value.colConstrList) = (yystack_[1].value.colConstrList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.colConstrList) = driver.makeAstNode<ColumnConstraintListAST>( (yystack_[1].value.colConstrList), (yystack_[0].value.astNode)->asColumnConstraint() );
      }

    break;

  case 67:

    { (yylhs.value.colConstrList) = 0; }

    break;

  case 68:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnDefinitionAST>( driver.createName(NameAST::Column, (yystack_[5].value.location)),
                                                      (yystack_[4].value.astNode)->asBaseType(),
                                                      (yystack_[3].value.astNode)->asCollateClause(),
                                                      (yystack_[2].value.location),
                                                      (yystack_[1].value.astNode)->asValueExpression(),
                                                      (yystack_[0].value.colConstrList) );
      }

    break;

  case 69:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnDefinitionAST>( driver.createName(NameAST::Column, (yystack_[3].value.location)),
                                                      (yystack_[2].value.astNode)->asBaseType(),
                                                      (yystack_[1].value.astNode)->asCollateClause(),
                                                      0,
                                                      (ConstantExpressionAST *) 0,
                                                      (yystack_[0].value.colConstrList) );
      }

    break;

  case 70:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnDefinitionAST>( driver.createName(NameAST::Column, (yystack_[0].value.location)),
                                                      (BaseTypeAST *) 0,
                                                      (CollateClauseAST *) 0,
                                                      0,
                                                      (ConstantExpressionAST *) 0,
                                                      (ColumnConstraintListAST *) 0 );
      }

    break;

  case 72:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableAddColumnActionAST>( (yystack_[2].value.location),
                                                               (yystack_[1].value.location),
                                                               (yystack_[0].value.astNode)->asColumnDefinition() );
      }

    break;

  case 73:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableRenameActionAST>( (yystack_[2].value.location),
                                                            (yystack_[1].value.location),
                                                            driver.createName(NameAST::Table, (yystack_[0].value.location)) );
      }

    break;

  case 74:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableRenameColumnActionAST>( (yystack_[4].value.location),
                                                                  (yystack_[3].value.location),
                                                                  driver.createName(NameAST::Column, (yystack_[2].value.location)),
                                                                  (yystack_[1].value.location),
                                                                  driver.createName(NameAST::Column, (yystack_[0].value.location)) );
      }

    break;

  case 75:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableRenameColumnActionAST>( (yystack_[1].value.location),
                                                                  (yystack_[0].value.location),
                                                                  (NameAST *) 0,
                                                                  0,
                                                                  (NameAST *) 0 );
      }

    break;

  case 76:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableRenameConstraintActionAST>( (yystack_[4].value.location),
                                                                      (yystack_[3].value.location),
                                                                      driver.createName(NameAST::Constraint, (yystack_[2].value.location)),
                                                                      (yystack_[1].value.location),
                                                                      driver.createName(NameAST::Constraint, (yystack_[0].value.location)) );
      }

    break;

  case 77:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableRenameConstraintActionAST>( (yystack_[1].value.location),
                                                                      (yystack_[0].value.location),
                                                                      (NameAST *) 0,
                                                                      0,
                                                                      (NameAST *) 0 );
      }

    break;

  case 78:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableAbleTriggerActionAST>( (yystack_[2].value.location),
                                                                 (yystack_[1].value.location),
                                                                 (yystack_[0].value.astNode)->asTriggerWildcard() );
      }

    break;

  case 79:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableDropColumnActionAST>( (yystack_[4].value.location),
                                                                (yystack_[3].value.location),
                                                                (yystack_[2].value.astNode)->asIfExists(),
                                                                driver.createName(NameAST::Column, (yystack_[1].value.location)),
                                                                (yystack_[0].value.location) );
      }

    break;

  case 80:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableDropColumnActionAST>( (yystack_[2].value.location),
                                                                (yystack_[1].value.location),
                                                                (yystack_[0].value.astNode)->asIfExists(),
                                                                (NameAST *) 0,
                                                                0 );
      }

    break;

  case 81:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableDropConstraintActionAST>( (yystack_[4].value.location),
                                                                    (yystack_[3].value.location),
                                                                    (yystack_[2].value.astNode)->asIfExists(),
                                                                    driver.createName(NameAST::Constraint, (yystack_[1].value.location)),
                                                                    (yystack_[0].value.location) );
      }

    break;

  case 82:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableDropConstraintActionAST>( (yystack_[2].value.location),
                                                                    (yystack_[1].value.location),
                                                                    (yystack_[0].value.astNode)->asIfExists(),
                                                                    (NameAST *) 0,
                                                                    0 );
      }

    break;

  case 83:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AlterTableStatementAST>( (yystack_[6].value.location),
                                                         (yystack_[5].value.location),
                                                         (yystack_[4].value.astNode)->asIfExists(),
                                                         (yystack_[3].value.location),
                                                         (yystack_[2].value.astNode)->asTableIdentifier(),
                                                         (yystack_[1].value.astNode)->asAlterTableAction() );
      }

    break;

  case 84:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CommentOnTableStatementAST>( (yystack_[6].value.location),
                                                             (yystack_[5].value.location),
                                                             (yystack_[4].value.location),
                                                             (yystack_[3].value.astNode)->asTableIdentifier(),
                                                             (yystack_[2].value.location),
                                                             driver.makeAstNode<LiteralExpressionAST>((yystack_[1].value.location)) );
      }

    break;

  case 85:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CommentOnTableStatementAST>( (yystack_[4].value.location),
                                                             (yystack_[3].value.location),
                                                             (yystack_[2].value.location),
                                                             (yystack_[1].value.astNode)->asTableIdentifier(),
                                                             0,
                                                             (LiteralExpressionAST *) 0 );
      }

    break;

  case 86:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CommentOnColumnStatementAST>( (yystack_[6].value.location),
                                                              (yystack_[5].value.location),
                                                              (yystack_[4].value.location),
                                                              (yystack_[3].value.astNode)->asColumnIdentifier(),
                                                              (yystack_[2].value.location),
                                                              driver.makeAstNode<LiteralExpressionAST>((yystack_[1].value.location)) );
      }

    break;

  case 87:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CommentOnColumnStatementAST>( (yystack_[4].value.location),
                                                              (yystack_[3].value.location),
                                                              (yystack_[2].value.location),
                                                              (yystack_[1].value.astNode)->asColumnIdentifier(),
                                                              0,
                                                              (LiteralExpressionAST *) 0 );
      }

    break;

  case 88:

    { (yylhs.value.astNode) = driver.makeAstNode<TruncateStatementAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.astNode)->asTableIdentifier() ); }

    break;

  case 89:

    { (yylhs.value.astNode) = driver.makeAstNode<DropTableStatementAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.astNode)->asTableIdentifier() ); }

    break;

  case 90:

    { (yylhs.value.astNode) = driver.makeAstNode<DropViewStatementAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.astNode)->asTableIdentifier() ); }

    break;

  case 91:

    { (yylhs.value.astNode) = driver.makeAstNode<DropIndexStatementAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.astNode)->asIndexIdentifier() ); }

    break;

  case 92:

    { (yylhs.value.astNode) = driver.makeAstNode<DropTriggerStatementAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.astNode)->asTriggerIdentifier() ); }

    break;

  case 94:

    { (yylhs.value.astNode) = driver.makeAstNode<InsertColumnsClauseAST>( (yystack_[2].value.location), (yystack_[1].value.colIdentList), (yystack_[0].value.location) ); }

    break;

  case 95:

    { (yylhs.value.astNode) = driver.makeAstNode<InsertClauseAsValuesAST>( (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.exprList), (yystack_[0].value.location) ); }

    break;

  case 96:

    { (yylhs.value.astNode) = driver.makeAstNode<InsertClauseAsSelectAST>( (yystack_[0].value.astNode)->asSelectStatement() ); }

    break;

  case 97:

    {
        (yylhs.value.astNode) = driver.makeAstNode<InsertStatementAST>( (yystack_[5].value.location),
                                                     (yystack_[4].value.location),
                                                     (yystack_[3].value.astNode)->asDirectSingleSource(),
                                                     (yystack_[2].value.astNode)->asInsertColumnsClause(),
                                                     (yystack_[1].value.astNode)->asInsertClause() );
      }

    break;

  case 98:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AssignmentExpressionAST>( (yystack_[2].value.astNode)->asColumnIdentifier(),
                                                          (yystack_[1].value.location),
                                                          (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 99:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AssignmentExpressionAST>( (yystack_[1].value.astNode)->asColumnIdentifier(),
                                                          (yystack_[0].value.location),
                                                          (ValueExpressionAST *) 0 );
        driver.error((yystack_[0].value.location), "Assignment Value Expected");
      }

    break;

  case 100:

    {
        (yylhs.value.astNode) = driver.makeAstNode<AssignmentExpressionAST>( (yystack_[0].value.astNode)->asColumnIdentifier(),
                                                          0,
                                                          (ValueExpressionAST *) 0 );
        driver.error((yystack_[0].value.astNode), "Assignment Value Expected");
      }

    break;

  case 101:

    {
        (yylhs.value.updateSetList) = (yystack_[2].value.updateSetList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.updateSetList) = driver.makeAstNode<AssignmentExpressionListAST>( (yystack_[2].value.updateSetList), (yystack_[0].value.astNode)->asAssignmentExpression() );
      }

    break;

  case 102:

    {
        (yylhs.value.updateSetList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.updateSetList) = driver.makeAstNode<AssignmentExpressionListAST>( (yystack_[0].value.astNode)->asAssignmentExpression() );
      }

    break;

  case 103:

    { (yylhs.value.astNode) = driver.makeAstNode<UpdateSetClauseAST>( (yystack_[1].value.location), (yystack_[0].value.updateSetList) ); }

    break;

  case 104:

    {
        (yylhs.value.astNode) = driver.makeAstNode<UpdateStatementAST>( (yystack_[4].value.location),
                                                     (yystack_[3].value.astNode)->asDirectSingleSource(),
                                                     (yystack_[2].value.astNode)->asUpdateSetClause(),
                                                     (yystack_[1].value.astNode)->asWhereClause() );
      }

    break;

  case 105:

    {
        (yylhs.value.astNode) = driver.makeAstNode<UpdateStatementAST>( (yystack_[1].value.location),
                                                     (yystack_[0].value.astNode)->asDirectSingleSource(),
                                                     (UpdateSetClauseAST *) 0,
                                                     (WhereClauseAST *) 0 );
      }

    break;

  case 106:

    {
        (yylhs.value.astNode) = driver.makeAstNode<DeleteStatementAST>( (yystack_[4].value.location),
                                                     (yystack_[3].value.location),
                                                     (yystack_[2].value.astNode)->asDirectSingleSource(),
                                                     (yystack_[1].value.astNode)->asWhereClause() );
      }

    break;

  case 108:

    { (yylhs.value.location) = 0; }

    break;

  case 109:

    { (yylhs.value.location) = (yystack_[0].value.location); }

    break;

  case 110:

    { (yylhs.value.location) = (yystack_[0].value.location); }

    break;

  case 111:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SelectCoreAST>( (yystack_[7].value.location),                         /* SELECT token */
                                                (yystack_[6].value.location),                         /* ALL|DISTINCT token */
                                                (yystack_[5].value.colList),                         /* select columns */
                                                (yystack_[4].value.location),                         /* FROM token */
                                                (yystack_[3].value.tableRefList),                         /* select source */
                                                (yystack_[2].value.astNode)->asWhereClause(),        /* WHERE ... */
                                                (yystack_[1].value.astNode)->asGroupByBlock(),       /* GROUP BY ... */
                                                (yystack_[0].value.astNode)->asSetClause()           /* UNION | INTERSECT | MINUS <select_core> */ );
      }

    break;

  case 112:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SelectStatementAST>( (yystack_[2].value.astNode)->asSelectCore(),
                                                     (yystack_[1].value.astNode)->asOrderByBlock(),
                                                     (yystack_[0].value.astNode)->asForUpdateClause() );
      }

    break;

  case 113:

    { (yylhs.value.location) = 0; }

    break;

  case 116:

    { (yylhs.value.astNode) = driver.makeAstNode<OrderingTermAST>( (yystack_[1].value.astNode)->asValueExpression(), (yystack_[0].value.location) ); }

    break;

  case 117:

    {
        (yylhs.value.orderTermList) = (yystack_[2].value.orderTermList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.orderTermList) = driver.makeAstNode<OrderingTermListAST>( (yystack_[2].value.orderTermList), (yystack_[0].value.astNode)->asOrderingTerm() );
      }

    break;

  case 118:

    {
        (yylhs.value.orderTermList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.orderTermList) = driver.makeAstNode<OrderingTermListAST>( (yystack_[0].value.astNode)->asOrderingTerm() );
      }

    break;

  case 120:

    { (yylhs.value.astNode) = driver.makeAstNode<OrderByBlockAST>( (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.orderTermList)); }

    break;

  case 121:

    {
        (yylhs.value.colIdentList) = (yystack_[2].value.colIdentList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.colIdentList) = driver.makeAstNode<ColumnIdentifierListAST>( (yystack_[2].value.colIdentList), (yystack_[0].value.astNode)->asColumnIdentifier() );
      }

    break;

  case 122:

    {
        (yylhs.value.colIdentList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.colIdentList) = driver.makeAstNode<ColumnIdentifierListAST>( (yystack_[0].value.astNode)->asColumnIdentifier() );
      }

    break;

  case 123:

    { (yylhs.value.location) = 0; }

    break;

  case 126:

    { (yylhs.value.astNode) = driver.makeAstNode<ForUpdateClauseAST>( (yystack_[4].value.location), (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.colIdentList), (yystack_[0].value.location) ); }

    break;

  case 128:

    {
        (yylhs.value.astNode) = driver.makeAstNode<GroupByBlockAST>( (yystack_[4].value.location),
                                                  (yystack_[3].value.location),
                                                  (yystack_[2].value.exprList),
                                                  (yystack_[1].value.location),
                                                  (yystack_[0].value.astNode)->asConditionExpression() );
      }

    break;

  case 129:

    {
        (yylhs.value.astNode) = driver.makeAstNode<GroupByBlockAST>( (yystack_[2].value.location),
                                                  (yystack_[1].value.location),
                                                  (yystack_[0].value.exprList),
                                                  0,
                                                  (ConditionExpressionAST *) 0 );
      }

    break;

  case 131:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<UnionSetSpecAST>((yystack_[2].value.location), (yystack_[1].value.location)),
                                               (yystack_[0].value.astNode)->asSelectCore() );
      }

    break;

  case 132:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<UnionSetSpecAST>((yystack_[2].value.location), (yystack_[1].value.location)),
                                               (yystack_[0].value.astNode)->asSelectCore() );
      }

    break;

  case 133:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<UnionSetSpecAST>((yystack_[1].value.location), 0),
                                               (yystack_[0].value.astNode)->asSelectCore() );
      }

    break;

  case 134:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<IntersectSetSpecAST>((yystack_[1].value.location)),
                                               (yystack_[0].value.astNode)->asSelectCore() );
      }

    break;

  case 135:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<MinusSetSpecAST>((yystack_[1].value.location)),
                                               (yystack_[0].value.astNode)->asSelectCore() );
      }

    break;

  case 137:

    {
        (yylhs.value.astNode) = driver.makeAstNode<WhereClauseAST>( (yystack_[1].value.location),
                                                 (yystack_[0].value.astNode)->asConditionExpression() );
      }

    break;

  case 138:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0,  0, (yystack_[0].value.location)); }

    break;

  case 139:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0, (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 140:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0, (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 141:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0, (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 142:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0, (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 143:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0, (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 144:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>( 0, (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 145:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>((yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 146:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>((yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 147:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>((yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 148:

    { (yylhs.value.astNode) = driver.makeAstNode<JoinSpecAST>((yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 150:

    {
        (yylhs.value.astNode) = driver.makeAstNode<JoinConditionAST>( (yystack_[1].value.location),
                                                   (yystack_[0].value.astNode)->asConditionExpression() );
      }

    break;

  case 151:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CompoundJoinedTableAST>( (yystack_[2].value.location),
                                                         (yystack_[1].value.astNode)->asJoinedTable(),
                                                         (yystack_[0].value.location) );
      }

    break;

  case 152:

    {
        (yylhs.value.astNode) = driver.makeAstNode<JoinJoinedTableAST>( (yystack_[3].value.astNode)->asTableReference(),
                                                     (yystack_[2].value.astNode)->asJoinSpec(),
                                                     (yystack_[1].value.astNode)->asTableReference(),
                                                     (yystack_[0].value.astNode)->asJoinCondition() );
      }

    break;

  case 153:

    {
        (yylhs.value.astNode) = driver.makeAstNode<DirectSingleSourceAST>( (yystack_[2].value.astNode)->asTableIdentifier(),
                                                        (yystack_[1].value.location),
                                                        driver.createName(NameAST::TableAlias, (yystack_[0].value.location)) );
      }

    break;

  case 154:

    {
        (yylhs.value.astNode) = driver.makeAstNode<DirectSingleSourceAST>( (yystack_[1].value.astNode)->asTableIdentifier(),
                                                        0,
                                                        driver.createName(NameAST::TableAlias, (yystack_[0].value.location)) );
      }

    break;

  case 155:

    {
        (yylhs.value.astNode) = driver.makeAstNode<DirectSingleSourceAST>( (yystack_[0].value.astNode)->asTableIdentifier(),
                                                        0,
                                                        (NameAST *) 0 );
      }

    break;

  case 156:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SubQuerySingleSourceAST>( (yystack_[4].value.location),
                                                          (yystack_[3].value.astNode)->asSelectStatement(),
                                                          (yystack_[2].value.location),
                                                          (yystack_[1].value.location),
                                                          driver.createName(NameAST::TableAlias, (yystack_[0].value.location)) );
      }

    break;

  case 157:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SubQuerySingleSourceAST>( (yystack_[3].value.location),
                                                          (yystack_[2].value.astNode)->asSelectStatement(),
                                                          (yystack_[1].value.location),
                                                          0,
                                                          driver.createName(NameAST::TableAlias, (yystack_[0].value.location)) );
      }

    break;

  case 158:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SubQuerySingleSourceAST>( (yystack_[2].value.location),
                                                          (yystack_[1].value.astNode)->asSelectStatement(),
                                                          (yystack_[0].value.location),
                                                          0,
                                                          (NameAST *) 0 );
      }

    break;

  case 162:

    {
        (yylhs.value.tableRefList) = (yystack_[2].value.tableRefList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.tableRefList) = driver.makeAstNode<TableReferenceListAST>( (yystack_[2].value.tableRefList), (yystack_[0].value.astNode)->asTableReference() );
      }

    break;

  case 163:

    {
        (yylhs.value.tableRefList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.tableRefList) = driver.makeAstNode<TableReferenceListAST>( (yystack_[0].value.astNode)->asTableReference() );
      }

    break;

  case 164:

    { (yylhs.value.astNode) = driver.makeAstNode<StarSelectColumnAST>( (yystack_[0].value.location) ); }

    break;

  case 165:

    {
        TableIdentifierAST *tabIdent = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[4].value.location)),
                                                                               (yystack_[3].value.location),
                                                                               driver.createName(NameAST::Table, (yystack_[2].value.location)) );
        (yylhs.value.astNode) = driver.makeAstNode<TableSelectColumnAST>( tabIdent, (yystack_[1].value.location), (yystack_[0].value.location) );
      }

    break;

  case 166:

    {
        TableIdentifierAST *tabIdent = driver.makeAstNode<TableIdentifierAST>( (NameAST *) 0,
                                                                               0,
                                                                               driver.createName(NameAST::Table, (yystack_[2].value.location)) );
        (yylhs.value.astNode) = driver.makeAstNode<TableSelectColumnAST>( tabIdent, (yystack_[1].value.location), (yystack_[0].value.location) );
      }

    break;

  case 167:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ExpressionSelectColumnAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                            (yystack_[1].value.location),
                                                            driver.createName(NameAST::ColumnAlias, (yystack_[0].value.location)) );
      }

    break;

  case 168:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ExpressionSelectColumnAST>( (yystack_[1].value.astNode)->asValueExpression(),
                                                            0,
                                                            driver.createName(NameAST::ColumnAlias, (yystack_[0].value.location)) );
      }

    break;

  case 169:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ExpressionSelectColumnAST>( (yystack_[0].value.astNode)->asValueExpression(),
                                                            0,
                                                            (NameAST *) 0 );
      }

    break;

  case 170:

    {
        (yylhs.value.colList) = (yystack_[2].value.colList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.colList) = driver.makeAstNode<BaseSelectColumnListAST>( (yystack_[2].value.colList), (yystack_[0].value.astNode)->asBaseSelectColumn() );
      }

    break;

  case 171:

    {
        (yylhs.value.colList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.colList) = driver.makeAstNode<BaseSelectColumnListAST>( (yystack_[0].value.astNode)->asBaseSelectColumn() );
      }

    break;

  case 172:

    { (yylhs.value.astNode) = driver.makeAstNode<CommitStatementAST>((yystack_[1].value.location)); }

    break;

  case 173:

    { (yylhs.value.astNode) = driver.makeAstNode<RollbackStatementAST>((yystack_[1].value.location)); }

    break;

  case 180:

    {
        (yylhs.value.astNode) = driver.makeAstNode<BinaryValueExpressionAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                           (yystack_[1].value.location),
                                                           (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 184:

    { (yylhs.value.astNode) = driver.makeAstNode<UnaryValueExpressionAST>( (yystack_[1].value.location), (yystack_[0].value.astNode)->asValueExpression() ); }

    break;

  case 199:

    { (yylhs.value.location) = 0; }

    break;

  case 203:

    { (yylhs.value.location) = (yystack_[0].value.location); }

    break;

  case 207:

    { (yylhs.value.location) = (yystack_[0].value.location); }

    break;

  case 240:

    { (yylhs.value.location) = (yystack_[0].value.location); }

    break;

  case 259:

    { (yylhs.value.astNode) = driver.makeAstNode<CharacterTypeAST>((yystack_[0].value.location), 0, 0, 0); }

    break;

  case 260:

    { (yylhs.value.astNode) = driver.makeAstNode<CharacterTypeAST>((yystack_[2].value.location), (yystack_[1].value.location), 0, (yystack_[0].value.location)); }

    break;

  case 261:

    { (yylhs.value.astNode) = driver.makeAstNode<CharacterTypeAST>((yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 262:

    { (yylhs.value.astNode) = driver.makeAstNode<NumberTypeAST>((yystack_[0].value.location), 0, 0, 0, 0, 0); }

    break;

  case 263:

    { (yylhs.value.astNode) = driver.makeAstNode<NumberTypeAST>((yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), 0, 0, (yystack_[0].value.location)); }

    break;

  case 264:

    { (yylhs.value.astNode) = driver.makeAstNode<NumberTypeAST>((yystack_[5].value.location), (yystack_[4].value.location), (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 265:

    { (yylhs.value.astNode) = driver.makeAstNode<SimpleTypeAST>((yystack_[0].value.location)); }

    break;

  case 266:

    { (yylhs.value.astNode) = driver.makeAstNode<UserDefinedTypeAST>( driver.createName(NameAST::Type, (yystack_[0].value.location)) ); }

    break;

  case 267:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[4].value.location)),
                                                      (yystack_[3].value.location),
                                                      driver.createName(NameAST::Table, (yystack_[2].value.location)),
                                                      (yystack_[1].value.location),
                                                      driver.createName(NameAST::Column, (yystack_[0].value.location)) );
      }

    break;

  case 268:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[3].value.location)),
                                                      (yystack_[2].value.location),
                                                      driver.createName(NameAST::Table, (yystack_[1].value.location)),
                                                      (yystack_[0].value.location),
                                                      (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Table Identifier Expected");
      }

    break;

  case 269:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Table, (yystack_[2].value.location)),
                                                      (yystack_[1].value.location),
                                                      driver.createName(NameAST::Column, (yystack_[0].value.location)) );
      }

    break;

  case 270:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Table, (yystack_[1].value.location)),
                                                      (yystack_[0].value.location),
                                                      (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Table Identifier Expected");
      }

    break;

  case 271:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Column, (yystack_[0].value.location)) );
      }

    break;

  case 272:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CaseExpressionClauseAST>( (yystack_[3].value.location),
                                                          (yystack_[2].value.astNode)->asValueExpression(),
                                                          (yystack_[1].value.location),
                                                          (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 273:

    {
        (yylhs.value.caseExprList) = (yystack_[1].value.caseExprList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.caseExprList) = driver.makeAstNode<CaseExpressionClauseListAST>( (yystack_[1].value.caseExprList), (yystack_[0].value.astNode)->asCaseExpressionClause() );
      }

    break;

  case 274:

    {
        (yylhs.value.caseExprList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.caseExprList) = driver.makeAstNode<CaseExpressionClauseListAST>( (yystack_[0].value.astNode)->asCaseExpressionClause() );
      }

    break;

  case 275:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CaseExpressionAST>( (yystack_[5].value.location),
                                                    (yystack_[4].value.astNode)->asValueExpression(),
                                                    (yystack_[3].value.caseExprList),
                                                    (yystack_[2].value.location),
                                                    (yystack_[1].value.astNode)->asValueExpression(),
                                                    (yystack_[0].value.location) );
      }

    break;

  case 276:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CaseExpressionAST>( (yystack_[3].value.location),
                                                    (yystack_[2].value.astNode)->asValueExpression(),
                                                    (yystack_[1].value.caseExprList),
                                                    0,
                                                    (ValueExpressionAST *) 0,
                                                    (yystack_[0].value.location) );
      }

    break;

  case 277:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CaseConditionClauseAST>( (yystack_[3].value.location),
                                                         (yystack_[2].value.astNode)->asConditionExpression(),
                                                         (yystack_[1].value.location),
                                                         (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 278:

    {
        (yylhs.value.caseCondList) = (yystack_[1].value.caseCondList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.caseCondList) = driver.makeAstNode<CaseConditionClauseListAST>( (yystack_[1].value.caseCondList), (yystack_[0].value.astNode)->asCaseConditionClause() );
      }

    break;

  case 279:

    {
        (yylhs.value.caseCondList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.caseCondList) = driver.makeAstNode<CaseConditionClauseListAST>( (yystack_[0].value.astNode)->asCaseConditionClause() );
      }

    break;

  case 280:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CaseConditionAST>( (yystack_[4].value.location),
                                                   (yystack_[3].value.caseCondList),
                                                   (yystack_[2].value.location),
                                                   (yystack_[1].value.astNode)->asValueExpression(),
                                                   (yystack_[0].value.location) );
      }

    break;

  case 281:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CaseConditionAST>( (yystack_[2].value.location),
                                                   (yystack_[1].value.caseCondList),
                                                   0,
                                                   (ValueExpressionAST *) 0,
                                                   (yystack_[0].value.location) );
      }

    break;

  case 282:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CompoundValueExpressionAST>( (yystack_[2].value.location),
                                                             (yystack_[1].value.astNode)->asValueExpression(),
                                                             (yystack_[0].value.location) );
      }

    break;

  case 283:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SelectCompoundValueExpressionAST>( (yystack_[2].value.location),
                                                                   (yystack_[1].value.astNode)->asSelectStatement(),
                                                                   (yystack_[0].value.location) );
      }

    break;

  case 284:

    { (yylhs.value.astNode) = driver.makeAstNode<BuiltinFunctionCallAST>((yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.exprList), (yystack_[0].value.location)); }

    break;

  case 285:

    { (yylhs.value.astNode) = driver.makeAstNode<CountStarCallAST>((yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.location), (yystack_[0].value.location)); }

    break;

  case 286:

    { (yylhs.value.astNode) = driver.makeAstNode<CountExprCallAST>((yystack_[4].value.location), (yystack_[3].value.location), (yystack_[2].value.location), (yystack_[1].value.astNode)->asValueExpression(), (yystack_[0].value.location)); }

    break;

  case 287:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CastFunctionAST>( (yystack_[5].value.location),
                                                  (yystack_[4].value.location),
                                                  (yystack_[3].value.astNode)->asValueExpression(),
                                                  (yystack_[2].value.location),
                                                  (yystack_[1].value.astNode)->asBaseType(),
                                                  (yystack_[0].value.location) );
      }

    break;

  case 288:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CastFunctionAST>( (yystack_[3].value.location),
                                                  (yystack_[2].value.location),
                                                  (yystack_[1].value.astNode)->asValueExpression(),
                                                  0,
                                                  (BaseTypeAST *) 0,
                                                  (yystack_[0].value.location) );
      }

    break;

  case 289:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SubstringFunctionAST>( (yystack_[7].value.location),
                                                       (yystack_[6].value.location),
                                                       (yystack_[5].value.astNode)->asValueExpression(),
                                                       (yystack_[4].value.location),
                                                       (yystack_[3].value.astNode)->asValueExpression(),
                                                       (yystack_[2].value.location),
                                                       (yystack_[1].value.astNode)->asValueExpression(),
                                                       (yystack_[0].value.location) );
      }

    break;

  case 290:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SubstringFunctionAST>( (yystack_[5].value.location),
                                                       (yystack_[4].value.location),
                                                       (yystack_[3].value.astNode)->asValueExpression(),
                                                       (yystack_[2].value.location),
                                                       (yystack_[1].value.astNode)->asValueExpression(),
                                                       0,
                                                       (ValueExpressionAST *) 0,
                                                       (yystack_[0].value.location) );
      }

    break;

  case 291:

    {
        (yylhs.value.astNode) = driver.makeAstNode<SubstringFunctionAST>( (yystack_[3].value.location),
                                                       (yystack_[2].value.location),
                                                       (yystack_[1].value.astNode)->asValueExpression(),
                                                       0,
                                                       (ValueExpressionAST *) 0,
                                                       0,
                                                       (ValueExpressionAST *) 0,
                                                       (yystack_[0].value.location) );
      }

    break;

  case 294:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FunctionCallAST>( (yystack_[3].value.astNode)->asFunctionIdentifier(),
                                                  (yystack_[2].value.location),
                                                  (yystack_[1].value.exprList),
                                                  (yystack_[0].value.location) );
      }

    break;

  case 296:

    { (yylhs.value.astNode) = driver.makeAstNode<NumericExpressionAST>((yystack_[0].value.location)); }

    break;

  case 297:

    { (yylhs.value.astNode) = driver.makeAstNode<LiteralExpressionAST>((yystack_[0].value.location)); }

    break;

  case 298:

    { (yylhs.value.astNode) = driver.makeAstNode<NullValueExpressionAST>((yystack_[0].value.location)); }

    break;

  case 299:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ParameterExpressionAST>( (yystack_[1].value.location),
                                                         driver.createName(NameAST::Binding, (yystack_[0].value.location)) );
      }

    break;

  case 300:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ParameterExpressionAST>( (yystack_[0].value.location),
                                                         (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Parameter Identifier Expected");
      }

    break;

  case 302:

    {
        (yylhs.value.exprList) = (yystack_[2].value.exprList);
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.exprList) = driver.makeAstNode< List<ValueExpressionAST *> >( (yystack_[2].value.exprList), (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 303:

    {
        (yylhs.value.exprList) = 0;
        if ( (yystack_[0].value.astNode) )
          (yylhs.value.exprList) = driver.makeAstNode< List<ValueExpressionAST *> >( (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 304:

    {
        (yylhs.value.astNode) = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[2].value.location)),
                                                     (yystack_[1].value.location),
                                                     driver.createName(NameAST::Table, (yystack_[0].value.location)) );
      }

    break;

  case 305:

    {
        (yylhs.value.astNode) = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[1].value.location)),
                                                     (yystack_[0].value.location),
                                                     (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Table Identifier Expected");
      }

    break;

  case 306:

    {
        (yylhs.value.astNode) = driver.makeAstNode<TableIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Table, (yystack_[0].value.location)) );
      }

    break;

  case 307:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IndexIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[2].value.location)),
                                                     (yystack_[1].value.location),
                                                     driver.createName(NameAST::Index, (yystack_[0].value.location)) );
      }

    break;

  case 308:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IndexIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[1].value.location)),
                                                     (yystack_[0].value.location),
                                                     (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Index Identifier Expected");
      }

    break;

  case 309:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IndexIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Index, (yystack_[0].value.location)) );
      }

    break;

  case 310:

    {
        (yylhs.value.astNode) = driver.makeAstNode<TriggerIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[2].value.location)),
                                                       (yystack_[1].value.location),
                                                       driver.createName(NameAST::Trigger, (yystack_[0].value.location)) );
      }

    break;

  case 311:

    {
        (yylhs.value.astNode) = driver.makeAstNode<TriggerIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[1].value.location)),
                                                       (yystack_[0].value.location),
                                                       (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Trigger Identifier Expected");
      }

    break;

  case 312:

    {
        (yylhs.value.astNode) = driver.makeAstNode<TriggerIdentifierAST>( (NameAST *) 0,
                                                       0,
                                                       driver.createName(NameAST::Trigger, (yystack_[0].value.location)) );
      }

    break;

  case 313:

    {
      (yylhs.value.astNode) = driver.makeAstNode<FunctionIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[4].value.location)),
                                                      (yystack_[3].value.location),
                                                      driver.createName(NameAST::Package, (yystack_[2].value.location)),
                                                      (yystack_[1].value.location),
                                                      driver.createName(NameAST::Routine, (yystack_[2].value.location)) );
    }

    break;

  case 314:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FunctionIdentifierAST>( driver.createName(NameAST::Schema, (yystack_[3].value.location)),
                                                        (yystack_[2].value.location),
                                                        driver.createName(NameAST::Package, (yystack_[1].value.location)),
                                                        (yystack_[0].value.location),
                                                        (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Identifier Expected");
      }

    break;

  case 315:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FunctionIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Package, (yystack_[2].value.location)),
                                                        (yystack_[1].value.location),
                                                        driver.createName(NameAST::Routine, (yystack_[0].value.location)) );
      }

    break;

  case 316:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FunctionIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Package, (yystack_[1].value.location)),
                                                        (yystack_[0].value.location),
                                                        (NameAST *) 0 );
        driver.error((yystack_[0].value.location), "Identifier Expected");
      }

    break;

  case 317:

    {
        (yylhs.value.astNode) = driver.makeAstNode<FunctionIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Routine, (yystack_[0].value.location)) );
      }

    break;

  case 321:

    {
        (yylhs.value.astNode) = driver.makeAstNode<BinaryConditionExpressionAST>( (yystack_[2].value.astNode)->asConditionExpression(),
                                                               (yystack_[1].value.location),
                                                               (yystack_[0].value.astNode)->asConditionExpression() );
      }

    break;

  case 323:

    {
        (yylhs.value.astNode) = driver.makeAstNode<NotConditionExpressionAST>( (yystack_[1].value.location),
                                                            (yystack_[0].value.astNode)->asConditionExpression() );
      }

    break;

  case 331:

    {
        (yylhs.value.astNode) = driver.makeAstNode<BetweenConditionExpressionAST>( (yystack_[5].value.astNode)->asValueExpression(),
                                                                (yystack_[4].value.location),
                                                                (yystack_[3].value.location),
                                                                (yystack_[2].value.astNode)->asValueExpression(),
                                                                (yystack_[1].value.location),
                                                                (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 332:

    {
        (yylhs.value.astNode) = driver.makeAstNode<BetweenConditionExpressionAST>( (yystack_[4].value.astNode)->asValueExpression(),
                                                                (yystack_[3].value.location),
                                                                (yystack_[2].value.location),
                                                                (yystack_[1].value.astNode)->asValueExpression(),
                                                                (yystack_[0].value.location),
                                                                (ValueExpressionAST *) 0 );
      }

    break;

  case 333:

    {
        (yylhs.value.astNode) = driver.makeAstNode<BetweenConditionExpressionAST>( (yystack_[4].value.astNode)->asValueExpression(),
                                                                0,
                                                                (yystack_[3].value.location),
                                                                (yystack_[2].value.astNode)->asValueExpression(),
                                                                (yystack_[1].value.location),
                                                                (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 334:

    {
        (yylhs.value.astNode) = driver.makeAstNode<BetweenConditionExpressionAST>( (yystack_[3].value.astNode)->asValueExpression(),
                                                                0,
                                                                (yystack_[2].value.location),
                                                                (yystack_[1].value.astNode)->asValueExpression(),
                                                                (yystack_[0].value.location),
                                                                (ValueExpressionAST *) 0 );
      }

    break;

  case 335:

    { (yylhs.value.astNode) = driver.makeAstNode<SubQueryExpressionAST>((yystack_[2].value.location), (yystack_[1].value.astNode)->asSelectStatement(), (yystack_[0].value.location)); }

    break;

  case 336:

    { (yylhs.value.astNode) = driver.makeAstNode<ValueListExpressionAST>((yystack_[2].value.location), (yystack_[1].value.exprList), (yystack_[0].value.location)); }

    break;

  case 337:

    {
        (yylhs.value.astNode) = driver.makeAstNode<InConditionExpressionAST>( (yystack_[3].value.astNode)->asValueExpression(),
                                                           (yystack_[2].value.location),
                                                           (yystack_[1].value.location),
                                                           (yystack_[0].value.astNode)->asCompoundRValueExpression() );
      }

    break;

  case 338:

    {
        (yylhs.value.astNode) = driver.makeAstNode<InConditionExpressionAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                           0,
                                                           (yystack_[1].value.location),
                                                           (yystack_[0].value.astNode)->asCompoundRValueExpression() );
      }

    break;

  case 339:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ExistsConditionExpressionAST>( (yystack_[3].value.location),
                                                               driver.makeAstNode<SubQueryExpressionAST>( (yystack_[2].value.location),
                                                                                                          (yystack_[1].value.astNode)->asSelectStatement(),
                                                                                                          (yystack_[0].value.location) ) );
      }

    break;

  case 340:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IsNullConditionExpressionAST>( (yystack_[3].value.astNode)->asValueExpression(),
                                                               (yystack_[2].value.location),
                                                               (yystack_[1].value.location),
                                                               (yystack_[0].value.location) );
      }

    break;

  case 341:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IsNullConditionExpressionAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                               (yystack_[1].value.location),
                                                               (yystack_[0].value.location),
                                                               0 );
      }

    break;

  case 342:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IsNullConditionExpressionAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                               (yystack_[1].value.location),
                                                               0,
                                                               (yystack_[0].value.location) );
      }

    break;

  case 343:

    {
        (yylhs.value.astNode) = driver.makeAstNode<IsNullConditionExpressionAST>( (yystack_[1].value.astNode)->asValueExpression(),
                                                               (yystack_[0].value.location),
                                                               0,
                                                               0 );
      }

    break;

  case 344:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CompoundConditionExpressionAST>( (yystack_[2].value.location),
                                                                 (yystack_[1].value.astNode)->asConditionExpression(),
                                                                 (yystack_[0].value.location) );
      }

    break;

  case 345:

    {
        (yylhs.value.astNode) = driver.makeAstNode<CompareConditionExpressionAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                                (yystack_[1].value.location),
                                                                (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 346:

    {
        (yylhs.value.astNode) = driver.makeAstNode<LikeConditionExpressionAST>( (yystack_[3].value.astNode)->asValueExpression(),
                                                             (yystack_[2].value.location),
                                                             (yystack_[1].value.location),
                                                             (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 347:

    {
        (yylhs.value.astNode) = driver.makeAstNode<LikeConditionExpressionAST>( (yystack_[2].value.astNode)->asValueExpression(),
                                                             0,
                                                             (yystack_[1].value.location),
                                                             (yystack_[0].value.astNode)->asValueExpression() );
      }

    break;

  case 352:

    { (yylhs.value.astNode) = driver.makeAstNode<TrueConditionExpressionAST>((yystack_[0].value.location)); }

    break;

  case 353:

    { (yylhs.value.astNode) = driver.makeAstNode<FalseConditionExpressionAST>((yystack_[0].value.location)); }

    break;

  case 354:

    {
        (yylhs.value.astNode) = driver.makeAstNode<ConditionAsValueExpressionAST>( (yystack_[0].value.astNode)->asValueExpression() );
        driver.error((yystack_[0].value.astNode), "Invalid Conditional Expression");
      }

    break;

  case 355:

    { (yylhs.value.astNode) = driver.makeAstNode<EmptyAST>(); }

    break;



          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  PostgresParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  PostgresParser::yysyntax_error_ (state_type, symbol_number_type) const
  {
    return YY_("syntax error");
  }


  const short int PostgresParser::yypact_ninf_ = -220;

  const short int PostgresParser::yytable_ninf_ = -318;

  const short int
  PostgresParser::yypact_[] =
  {
     580,    51,   -46,  -115,    51,    72,    33,   -41,   121,    -6,
      51,    83,   208,    69,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,   104,    51,  -220,  -220,  -220,  -220,  -220,
    -220,   121,   259,   121,   265,   245,  -220,   121,   121,  -220,
    -220,   347,   279,   242,    30,   -77,  -220,   121,  -220,  -220,
    -220,   276,   272,  -220,  -220,    51,   340,    51,    51,   346,
      51,   191,   269,  -220,   210,   339,   185,  -220,  -220,   363,
     397,  -220,  -220,  -220,   675,  -220,  -220,  -220,  -220,  -220,
    -220,   342,  -220,  -220,  -220,   343,  -220,  -220,  -220,  -220,
    -220,   344,  -220,    42,    59,   228,   179,  -220,   345,  -220,
    -220,  -220,  -220,   349,   372,   375,   210,  -220,   376,   121,
     375,    51,   725,   183,  -220,  -220,  -220,   378,  -220,  -220,
     380,  -220,  -220,  -220,   121,   132,    51,  -220,   375,    63,
    -220,   130,  -220,   292,   351,   354,   132,   283,  -220,    40,
     725,   -24,   725,   347,   136,  -220,   387,  -220,  -220,  -220,
    -220,  -220,   725,  -220,   725,   725,  -220,   379,  -220,   382,
     383,    51,  -220,    57,    64,  -220,  -220,   384,    58,   352,
    -220,  -220,    11,    19,   294,  -220,  -220,   366,   770,   264,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,   236,  -220,   386,
      51,  -220,   303,  -220,   249,  -220,  -220,     7,   725,  -220,
      50,  -220,   725,  -220,   289,   370,  -220,  -220,   725,    38,
    -220,    31,  -220,  -220,  -220,   120,    37,  -220,  -220,  -220,
     254,   256,   401,   375,   725,  -220,   400,  -220,   402,  -220,
     725,  -220,  -220,  -220,   375,   250,  -132,  -220,   -34,  -220,
     176,    51,  -220,   741,   119,  -220,   222,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,   389,    10,   -21,   725,   725,   725,
    -220,  -220,   132,   375,  -220,   725,  -220,   211,   305,   725,
     314,  -220,   725,  -220,   365,  -220,   198,  -220,   391,  -220,
     725,   392,   393,   120,   333,  -220,   350,   355,   -12,     2,
     356,   121,   136,   306,   725,  -220,  -220,   416,  -220,  -220,
      51,    51,  -220,     4,  -220,   427,   245,   245,   428,   429,
     430,    43,  -220,  -220,   404,   397,  -220,   348,  -220,   389,
     725,   725,  -220,   394,  -220,  -220,  -220,   261,   410,  -220,
     270,  -220,   725,   388,  -220,  -220,  -220,  -220,  -220,    55,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,   174,  -220,  -220,  -220,  -220,  -220,  -220,    82,
     438,  -220,   442,   436,  -220,   180,    73,  -220,  -220,  -220,
    -220,  -220,   385,   390,  -220,   395,   396,  -220,   424,   120,
     434,   -68,  -220,  -220,   470,  -220,  -220,  -220,  -220,   198,
    -220,   471,   472,  -220,   431,   432,  -220,  -220,  -220,  -220,
    -220,  -220,   444,   267,  -220,  -220,  -220,   433,   725,  -220,
    -220,  -220,  -220,  -220,  -220,    80,   474,  -220,  -220,   725,
    -220,   482,  -220,  -220,  -220,  -220,   132,  -220,  -220,   725,
     222,   -45,   222,  -220,  -220,  -220,   281,  -148,  -148,   484,
     485,  -220,  -220,   725,  -220,   457,  -220,   274,   460,  -220,
     264,    14,  -220,   222,   222,  -220,  -220,   504,   301,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,   508,  -220,
    -220,   132,  -220,  -220,  -220,   725,    13,   480,   264,  -220,
     511,   -52,  -220,  -220,  -220,    18,  -220,  -220,   490,   -54,
     458,   121,   426,   -54,   132,  -110,   231,  -220,  -220,   -54,
     494,   -54,  -220,   164,  -220,  -220,   255,  -220,   517,  -220,
     -54,   -16,  -220,   280,  -220,  -220,  -220,   519,   443,  -220,
     -44,   477,  -220,  -220,  -220,  -220,  -100,  -220,  -220,  -220,
    -220,   -28,   368,   -60,  -220,  -220,  -220,  -220,  -220
  };

  const unsigned short int
  PostgresParser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     5,    13,    14,    12,    11,    10,
       9,     8,     7,   355,     0,    15,    16,    17,     4,     3,
     301,     0,     0,     0,     0,   355,   172,     0,     0,   109,
     110,     0,   306,   105,   155,     0,   173,     0,     1,     2,
       6,     0,   355,   119,   107,     0,   309,     0,     0,   312,
       0,     0,    20,    18,   355,   355,   271,   296,   297,   300,
       0,   183,   182,   164,     0,   298,   186,   187,   188,   189,
     190,     0,   191,   192,   193,     0,   194,   195,   196,   197,
     198,     0,   171,     0,   169,   174,     0,   181,     0,   295,
     292,   293,   185,     0,   305,     0,   355,   154,     0,     0,
       0,     0,     0,     0,   112,   125,    90,   308,    91,    89,
     311,    92,    19,    21,     0,     0,     0,   136,     0,     0,
      93,   270,   299,   271,     0,     0,     0,     0,   279,     0,
       0,   199,     0,     0,     0,   168,     0,   179,   176,   177,
     175,   178,     0,   184,     0,     0,   304,   271,   102,   103,
     100,     0,   153,     0,     0,    88,   118,   120,   113,     0,
     307,   310,   355,     0,     0,   352,   353,     0,   354,   137,
     320,   348,   349,   350,   351,   322,   106,     0,   122,     0,
       0,    96,   269,   166,   270,   283,   282,     0,     0,   274,
       0,   281,     0,   278,     0,     0,   200,   201,     0,     0,
     170,     0,   161,   159,   160,   163,   355,   167,   180,   303,
       0,     0,   270,     0,    99,   104,     0,    85,     0,    87,
       0,   115,   114,   116,     0,    25,    25,    27,    25,    28,
       0,     0,    71,   354,     0,   323,     0,   324,   327,   326,
     330,   325,   329,   328,     0,   343,     0,     0,     0,     0,
     319,   318,     0,     0,    94,     0,    97,   268,   269,     0,
       0,   276,     0,   273,     0,   288,     0,   285,     0,   291,
       0,     0,   161,     0,     0,   138,     0,     0,     0,     0,
       0,     0,     0,   355,     0,   284,   294,   269,   101,    98,
       0,     0,   117,   123,    26,     0,   355,   355,     0,    77,
      75,   355,    83,   344,     0,     0,   338,   341,   342,     0,
       0,     0,   347,     0,   345,   321,   121,     0,   267,   165,
     268,   277,     0,     0,   280,   266,   244,   209,   210,   202,
     204,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   247,   248,   249,   250,   251,
     252,   246,   254,   253,   255,   256,   235,   205,   236,   237,
     238,   239,   241,   242,   258,   243,   257,   208,   245,   206,
     259,   265,   262,     0,   286,     0,   158,   151,   143,   141,
     144,   139,     0,     0,   140,     0,     0,   142,   355,   162,
       0,   355,   127,   302,   268,    84,    86,   124,   126,    70,
      72,    82,    80,    73,     0,     0,    32,    30,    31,    78,
      29,   339,     0,     0,   340,   337,   346,     0,   334,    95,
     272,   275,   203,   240,   207,     0,     0,   287,   290,     0,
     157,     0,   145,   146,   147,   148,     0,   152,   149,     0,
       0,     0,     0,   111,   130,   267,   355,    22,    22,     0,
       0,   335,   336,   332,   333,     0,   260,     0,     0,   156,
     150,   129,   135,     0,     0,   133,   134,     0,    67,    33,
      24,    23,    81,    79,    76,    74,   331,   261,     0,   263,
     289,     0,   131,   132,    34,     0,   355,     0,   128,    67,
       0,    35,    66,    43,   264,   355,    44,    36,     0,   355,
       0,     0,     0,   355,     0,     0,     0,    47,    37,   355,
      63,   355,    49,     0,    41,    42,    40,    45,     0,    46,
     355,     0,    65,     0,    48,    38,    39,     0,   355,    64,
       0,   355,    58,    59,    61,    60,     0,    62,    52,    50,
      51,     0,     0,     0,    57,    56,    53,    54,    55
  };

  const short int
  PostgresParser::yypgoto_[] =
  {
    -220,  -220,   116,  -220,   513,  -139,  -220,   117,   122,  -220,
    -220,  -220,    16,  -219,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,    75,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,   367,  -220,  -220,  -220,  -220,  -220,  -220,  -129,   -51,
    -220,   358,  -220,  -220,   357,  -220,  -220,  -220,  -220,   -86,
    -220,  -220,   381,    -1,  -220,  -161,  -220,   446,  -220,  -220,
    -220,   -39,  -220,  -220,  -220,   441,  -220,  -220,  -220,  -220,
    -220,   167,   -18,   398,  -220,  -220,   455,  -220,  -220,   499,
    -220,  -151,   -30,  -220,  -220,  -220,  -220,  -130,   334,  -220,
    -220,   278,  -220,  -220,  -220,   425,   -23
  };

  const short int
  PostgresParser::yydefgoto_[] =
  {
      -1,    12,    30,    13,    14,    62,   124,   492,   305,   240,
     429,   488,   526,   527,   511,   512,   561,   557,   551,   523,
     543,   506,   420,   241,    15,    16,    17,    18,   129,   190,
      19,   158,   159,   106,    20,    21,    22,    41,    23,    24,
     233,   166,   167,    52,   187,   418,   114,   411,   463,   126,
     291,   457,   212,   213,   214,   215,   216,    92,    93,    25,
      26,   178,   152,    95,    96,    97,    98,   208,   390,   391,
     392,   393,    99,   199,   200,   100,   138,   139,   101,   102,
      27,   220,    44,    57,    60,   103,   262,   179,   180,   259,
     181,   316,   182,   183,   184,   185,   528
  };

  const short int
  PostgresParser::yytable_[] =
  {
      53,    55,    94,    58,   221,   483,   197,    43,   517,    39,
     517,   205,    63,   -69,   308,   263,   562,   111,   -68,   134,
     161,   319,   133,    67,    68,   294,   206,   304,    69,   115,
      35,   135,   567,   107,    42,   137,    64,    65,   553,   563,
      45,   127,   130,   244,   173,    71,   426,   -69,   292,    72,
     283,    28,   -68,   143,   260,   490,   211,    28,   261,    31,
     235,   518,   145,   559,    28,   460,   108,   320,   109,    49,
       1,   279,   317,   168,   401,   402,   450,    32,   191,   163,
     461,   174,   110,   127,   475,    29,   236,   160,   404,   405,
      74,    29,   164,   427,   172,   146,   201,   560,    29,    33,
     226,   204,   318,   209,    94,   491,   271,   228,   269,   451,
     188,    75,   231,   476,   327,   219,   219,   202,    38,   280,
      36,   175,   134,   144,    42,   304,    46,   272,   403,   534,
     293,   409,   232,   192,   243,   133,    67,    68,   176,    42,
      54,    69,   406,   519,     2,     7,   136,   428,   554,   242,
     568,   306,   313,    37,   535,  -316,   198,   173,    71,   270,
     281,   211,    72,   274,   433,   193,   260,   421,   422,   278,
     261,   116,   520,   118,   119,   564,   121,   237,   555,     3,
     321,   417,   133,    67,    68,   299,    34,   501,    69,   177,
     125,   168,   238,   127,   174,   314,   484,   540,   131,   462,
      40,   335,   284,    74,    70,   160,   285,   286,    48,     7,
    -317,   260,   525,   448,   328,   261,   188,   207,   322,   323,
     324,     7,   239,   545,    75,   565,   219,   165,    47,     4,
     331,   -35,     5,   333,   175,   287,  -314,   449,   521,    51,
     147,   395,   186,     6,   288,   326,   329,   263,   546,   309,
      74,   176,   268,     7,   148,   413,   289,   149,   150,     7,
     290,   189,    56,   151,   432,   294,     8,   294,    59,   264,
     412,    75,   294,   328,  -316,     9,   219,   225,   294,   227,
     229,   436,   437,    63,    63,   498,    61,   295,   430,   296,
     408,   547,   104,   440,   439,  -314,   510,   133,    67,    68,
     472,   510,   177,    69,   532,   194,   266,   499,   481,   105,
     537,   260,   539,   548,   112,   261,   267,  -317,   330,   173,
      71,   544,   275,    10,    72,   276,   480,    11,  -315,   113,
    -315,   482,   485,   486,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      66,    67,    68,   117,   502,   503,    69,   312,   307,   120,
     310,   122,   123,   125,   128,    74,   132,   140,   141,   142,
     154,   508,    70,    71,   155,   156,   336,    72,   157,   162,
     169,   170,    73,   171,   195,   458,    75,   196,   464,   198,
     217,   246,   222,   223,   533,   230,   175,   234,   224,   474,
     133,    67,    68,   277,   297,   300,    69,   301,   311,   304,
     478,   265,     7,   176,   315,   332,   415,   416,    74,   398,
     219,   334,    70,    71,   394,   396,   397,    72,   410,   414,
     419,   423,   424,   425,   496,  -313,   399,   431,   442,    75,
     434,   400,   407,   489,   441,   438,   443,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   445,   177,   444,   509,   446,    74,   447,
     456,   452,   459,   465,   467,   468,   453,   471,   477,   469,
     470,   454,   455,   513,   473,   479,   487,   494,   495,    75,
     497,   530,   513,   500,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     504,   505,   507,   514,   516,   524,   536,   529,   531,   538,
     542,   541,   549,   556,   566,   552,    50,   522,   558,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   550,   383,   384,   385,   386,
     387,     1,   388,   389,   515,   493,   466,     7,   302,   210,
     298,   303,   282,   218,   203,   153,   325,   435,   273,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     0,     0,     0,     0,
       0,     0,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   133,    67,
      68,     0,     0,     0,    69,     0,     0,     0,     0,     0,
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      70,    71,     0,     0,     0,    72,     0,     0,     0,     0,
       0,     0,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   133,    67,
      68,     0,     0,     0,    69,     0,     0,     0,     0,     0,
       4,     0,     0,     5,     0,     0,    74,     0,     0,     0,
      70,    71,     0,     0,     6,    72,   247,     0,   248,   249,
     250,     0,     0,   251,   252,   253,     0,    75,     0,     0,
       7,     0,     0,     0,   196,     0,     0,     8,     0,     0,
       0,   136,     0,   254,   255,   247,     9,   248,   249,   250,
       0,     0,   251,   252,   253,     0,    74,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,     0,     0,     0,
       0,     0,   254,   255,     0,     0,     0,    75,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
       0,     0,   256,     0,    10,     0,     0,     0,    11,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   257,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91
  };

  const short int
  PostgresParser::yycheck_[] =
  {
      23,    31,    41,    33,   155,    50,   136,     8,    62,    50,
      62,    35,    35,     0,    48,    11,    44,    47,     0,    70,
     106,    42,     3,     4,     5,    11,    50,   159,     9,    52,
     145,    70,    92,     3,     3,    74,    37,    38,    82,    67,
      46,    64,    65,   173,    25,    26,     3,    34,    11,    30,
     211,     0,    34,    11,    47,   203,    25,     0,    51,   105,
      49,   113,     3,   163,     0,   133,    36,    88,   145,     0,
       1,    33,    62,   112,    86,    87,     3,   123,   129,   109,
     148,    62,   159,   106,     4,    34,    75,   105,    86,    87,
      71,    34,   110,    50,   124,    36,    56,   197,    34,   145,
      43,   140,    92,   142,   143,   253,    56,    43,   101,    36,
     128,    92,    54,    33,   265,   154,   155,    77,    85,    81,
       4,   102,   173,    81,     3,   159,    10,    77,   140,   239,
     216,   292,    74,     3,   173,     3,     4,     5,   119,     3,
      24,     9,   140,   195,    75,   190,   106,   104,   192,   172,
     210,   283,    33,    81,   264,    25,   106,    25,    26,   198,
     211,    25,    30,   202,   315,    35,    47,   306,   307,   208,
      51,    55,   224,    57,    58,   203,    60,   166,   222,   110,
     201,   177,     3,     4,     5,   224,   232,   173,     9,   170,
     153,   230,   181,   216,    62,   246,   241,    33,    13,   267,
     241,     3,    82,    71,    25,   223,    86,    87,     0,   190,
      25,    47,   266,    33,     3,    51,   234,   241,   257,   258,
     259,   190,   211,   239,    92,   253,   265,   111,   145,   160,
     269,   285,   163,   272,   102,   115,    25,    57,   290,   135,
      12,   280,   126,   174,   124,   263,    35,    11,   264,   283,
      71,   119,     3,   190,    26,   294,   136,    29,    30,   190,
     140,   198,     3,    35,   315,    11,   197,    11,     3,    33,
     293,    92,    11,     3,    25,   206,   315,   161,    11,   163,
     164,   320,   321,   306,   307,    11,    41,    33,   311,    33,
     291,    11,    13,   332,    33,    25,   283,     3,     4,     5,
      33,   283,   170,     9,   523,    13,   190,    33,   459,    67,
     529,    47,   531,    33,    38,    51,    13,    25,    13,    25,
      26,   540,    33,   254,    30,    36,   456,   258,    25,    57,
      25,   460,   461,   462,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       3,     4,     5,    13,   483,   484,     9,   241,   236,    13,
     238,   170,    93,   153,    25,    71,     3,    25,    25,    25,
      25,   501,    25,    26,    25,     3,   178,    30,     3,     3,
     197,     3,    35,     3,    33,   408,    92,    33,   411,   106,
       3,    25,    13,    11,   524,    11,   102,    45,    15,   438,
       3,     4,     5,    33,     3,     5,     9,     5,   232,   159,
     449,    25,   190,   119,    25,   101,   300,   301,    71,    86,
     459,    56,    25,    26,    33,    33,    33,    30,   122,    13,
       3,     3,     3,     3,   473,    25,    86,    33,   383,    92,
      92,    86,    86,   466,    56,    51,   272,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,    25,   170,   383,   505,    25,    71,    33,
      46,    86,    38,     3,     3,     3,    86,    33,     4,    48,
      48,    86,    86,   506,    51,     3,   205,     3,     3,    92,
      33,   521,   515,    33,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       6,   210,     4,    33,     3,    25,   285,    59,    92,    25,
       3,   266,     3,    46,   156,   548,    13,   511,   551,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   132,   378,   379,   380,   381,
     382,     1,   384,   385,   509,   468,   419,   190,   230,   143,
     223,   234,   211,   152,   139,    96,   262,   319,   200,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     3,     4,
       5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     3,     4,
       5,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,    -1,    -1,    71,    -1,    -1,    -1,
      25,    26,    -1,    -1,   174,    30,    15,    -1,    17,    18,
      19,    -1,    -1,    22,    23,    24,    -1,    92,    -1,    -1,
     190,    -1,    -1,    -1,    33,    -1,    -1,   197,    -1,    -1,
      -1,   106,    -1,    42,    43,    15,   206,    17,    18,    19,
      -1,    -1,    22,    23,    24,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    62,    -1,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330
  };

  const unsigned short int
  PostgresParser::yystos_[] =
  {
       0,     1,    75,   110,   160,   163,   174,   190,   197,   206,
     254,   258,   387,   389,   390,   410,   411,   412,   413,   416,
     420,   421,   422,   424,   425,   445,   446,   466,     0,    34,
     388,   105,   123,   145,   232,   145,   388,    81,    85,    50,
     241,   423,     3,   439,   468,    46,   388,   145,     0,     0,
     390,   135,   429,   482,   388,   468,     3,   469,   468,     3,
     470,    41,   391,   482,   439,   439,     3,     4,     5,     9,
      25,    26,    30,    35,    71,    92,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   443,   444,   447,   449,   450,   451,   452,   458,
     461,   464,   465,   471,    13,    67,   419,     3,    36,   145,
     159,   468,    38,    57,   432,   482,   388,    13,   388,   388,
      13,   388,   170,    93,   392,   153,   435,   482,    25,   414,
     482,    13,     3,     3,   425,   447,   106,   447,   462,   463,
      25,    25,    25,    11,    81,     3,    36,    12,    26,    29,
      30,    35,   448,   465,    25,    25,     3,     3,   417,   418,
     458,   435,     3,   468,   458,   388,   427,   428,   447,   197,
       3,     3,   468,    25,    62,   102,   119,   170,   447,   473,
     474,   476,   478,   479,   480,   481,   388,   430,   458,   198,
     415,   425,     3,    35,    13,    33,    33,   473,   106,   459,
     460,    56,    77,   462,   447,    35,    50,   241,   453,   447,
     443,    25,   438,   439,   440,   441,   442,     3,   451,   447,
     467,   467,    13,    11,    15,   388,    43,   388,    43,   388,
      11,    54,    74,   426,    45,    49,    75,   166,   181,   211,
     395,   409,   482,   447,   473,   481,    25,    15,    17,    18,
      19,    22,    23,    24,    42,    43,    62,    88,   201,   475,
      47,    51,   472,    11,    33,    25,   388,    13,     3,   101,
     447,    56,    77,   459,   447,    33,    36,    33,   447,    33,
      81,   425,   438,   441,    82,    86,    87,   115,   124,   136,
     140,   436,    11,   435,    11,    33,    33,     3,   417,   447,
       5,     5,   427,   430,   159,   394,   283,   394,    48,   283,
     394,   232,   388,    33,   425,    25,   477,    62,    92,    42,
      88,   201,   447,   447,   447,   474,   458,   467,     3,    35,
      13,   447,   101,   447,    56,     3,   178,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   378,   379,   380,   381,   382,   384,   385,
     454,   455,   456,   457,    33,   447,    33,    33,    86,    86,
      86,    86,    87,   140,    86,    87,   140,    86,   439,   441,
     122,   433,   482,   447,    13,   388,   388,   177,   431,     3,
     408,   391,   391,     3,     3,     3,     3,    50,   104,   396,
     482,    33,   425,   467,    92,   477,   447,   447,    51,    33,
     447,    56,   383,   272,   383,    25,    25,    33,    33,    57,
       3,    36,    86,    86,    86,    86,    46,   437,   482,    38,
     133,   148,   267,   434,   482,     3,   457,     3,     3,    48,
      48,    33,    33,    51,   447,     4,    33,     4,   447,     3,
     473,   467,   424,    50,   241,   424,   424,   205,   397,   482,
     203,   253,   393,   393,     3,     3,   447,    33,    11,    33,
      33,   173,   424,   424,     6,   210,   407,     4,   473,   447,
     283,   400,   401,   482,    33,   407,     3,    62,   113,   195,
     224,   290,   398,   405,    25,   266,   398,   399,   482,    59,
     468,    92,   399,   473,   239,   264,   285,   399,    25,   399,
      33,   266,     3,   406,   399,   239,   264,    11,    33,     3,
     132,   404,   482,    82,   192,   222,    46,   403,   482,   163,
     197,   402,    44,    67,   203,   253,   156,    92,   210
  };

  const unsigned short int
  PostgresParser::yyr1_[] =
  {
       0,   386,   387,   388,   388,   389,   389,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   391,   391,
     392,   392,   393,   393,   393,   394,   394,   395,   395,   396,
     396,   396,   396,   397,   397,   398,   398,   399,   399,   399,
     399,   399,   399,   400,   400,   401,   401,   401,   401,   401,
     402,   402,   403,   403,   403,   403,   403,   403,   404,   404,
     404,   404,   405,   405,   406,   406,   407,   407,   408,   408,
     408,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   410,   411,   411,   411,   411,   412,   413,
     413,   413,   413,   414,   414,   415,   415,   416,   417,   417,
     417,   418,   418,   419,   420,   420,   421,   422,   423,   423,
     423,   424,   425,   426,   426,   426,   427,   428,   428,   429,
     429,   430,   430,   431,   431,   432,   432,   433,   433,   433,
     434,   434,   434,   434,   434,   434,   435,   435,   436,   436,
     436,   436,   436,   436,   436,   436,   436,   436,   436,   437,
     437,   438,   438,   439,   439,   439,   440,   440,   440,   441,
     441,   441,   442,   442,   443,   443,   443,   443,   443,   443,
     444,   444,   445,   446,   447,   448,   448,   448,   448,   448,
     449,   449,   450,   450,   451,   451,   452,   452,   452,   452,
     452,   452,   452,   452,   452,   452,   452,   452,   452,   453,
     453,   453,   454,   454,   454,   454,   454,   454,   454,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   456,   456,   457,
     457,   457,   457,   457,   457,   457,   457,   458,   458,   458,
     458,   458,   459,   460,   460,   461,   461,   462,   463,   463,
     464,   464,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   466,   467,   467,   468,   468,   468,   469,   469,   469,
     470,   470,   470,   471,   471,   471,   471,   471,   472,   472,
     473,   473,   474,   474,   475,   475,   475,   475,   475,   475,
     475,   476,   476,   476,   476,   477,   477,   478,   478,   479,
     480,   480,   480,   480,   481,   481,   481,   481,   481,   481,
     481,   481,   481,   481,   481,   482
  };

  const unsigned char
  PostgresParser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       0,     1,     0,     1,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     1,     4,     4,
       2,     2,     2,     1,     2,     4,     4,     3,     6,     3,
       1,     1,     1,     4,     4,     4,     3,     3,     1,     2,
       2,     2,     7,     2,     3,     1,     2,     0,     6,     4,
       1,     1,     3,     3,     5,     2,     5,     2,     3,     5,
       3,     5,     3,     7,     7,     5,     7,     5,     4,     4,
       4,     4,     4,     1,     3,     4,     1,     6,     3,     2,
       1,     3,     1,     2,     5,     2,     5,     2,     0,     1,
       1,     8,     3,     0,     1,     1,     2,     3,     1,     1,
       3,     3,     1,     0,     1,     1,     5,     1,     5,     3,
       1,     3,     3,     2,     2,     2,     1,     2,     1,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     1,
       2,     3,     4,     3,     2,     1,     5,     4,     3,     1,
       1,     1,     3,     1,     1,     5,     3,     3,     2,     1,
       3,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     2,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     1,     4,     6,     1,     1,     5,     4,     3,
       2,     1,     4,     2,     1,     6,     4,     4,     2,     1,
       5,     3,     3,     3,     4,     4,     5,     6,     4,     8,
       6,     4,     1,     1,     4,     1,     1,     1,     1,     2,
       1,     2,     3,     1,     3,     2,     1,     3,     2,     1,
       3,     2,     1,     5,     4,     3,     2,     1,     1,     1,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     6,     5,     5,     4,     3,     3,     4,     3,     4,
       4,     3,     3,     2,     3,     3,     4,     3,     1,     1,
       1,     1,     1,     1,     1,     0
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const PostgresParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"identifier\"",
  "\"number constant\"", "\"quoted string\"", "\"double quoted string\"",
  "\"&\"", "\"^\"", "\":\"", "\":=\"", "\",\"", "\"/\"", "\".\"", "\"..\"",
  "\"=\"", "\"!\"", "\"!=\"", "\">\"", "\">=\"", "\"{\"", "\"[\"", "\"<\"",
  "\"<=\"", "\"<>\"", "\"(\"", "\"-\"", "\"%\"", "\"|\"", "\"||\"",
  "\"+\"", "\"}\"", "\"]\"", "\")\"", "\";\"", "\"*\"", "\"as\"", "\"at\"",
  "\"by\"", "\"do\"", "\"go\"", "\"if\"", "\"in\"", "\"is\"", "\"no\"",
  "\"of\"", "\"on\"", "\"or\"", "\"to\"", "\"add\"", "\"all\"", "\"and\"",
  "\"any\"", "\"are\"", "\"asc\"", "\"dec\"", "\"end\"", "\"for\"",
  "\"get\"", "\"key\"", "\"map\"", "\"new\"", "\"not\"", "\"old\"",
  "\"out\"", "\"ref\"", "\"row\"", "\"set\"", "\"sql\"", "\"both\"",
  "\"call\"", "\"case\"", "\"cube\"", "\"data\"", "\"desc\"", "\"drop\"",
  "\"each\"", "\"else\"", "\"exec\"", "\"exit\"", "\"free\"", "\"from\"",
  "\"full\"", "\"goto\"", "\"hold\"", "\"into\"", "\"join\"", "\"left\"",
  "\"like\"", "\"loop\"", "\"next\"", "\"none\"", "\"null\"", "\"only\"",
  "\"open\"", "\"over\"", "\"read\"", "\"role\"", "\"rows\"", "\"sets\"",
  "\"some\"", "\"then\"", "\"true\"", "\"undo\"", "\"user\"", "\"view\"",
  "\"when\"", "\"with\"", "\"work\"", "\"after\"", "\"alter\"",
  "\"array\"", "\"begin\"", "\"check\"", "\"close\"", "\"cross\"",
  "\"cycle\"", "\"depth\"", "\"deref\"", "\"false\"", "\"fetch\"",
  "\"grant\"", "\"group\"", "\"index\"", "\"inner\"", "\"inout\"",
  "\"input\"", "\"large\"", "\"leave\"", "\"level\"", "\"limit\"",
  "\"local\"", "\"match\"", "\"minus\"", "\"names\"", "\"order\"",
  "\"outer\"", "\"prior\"", "\"range\"", "\"reads\"", "\"right\"",
  "\"scope\"", "\"space\"", "\"start\"", "\"state\"", "\"table\"",
  "\"treat\"", "\"under\"", "\"union\"", "\"until\"", "\"usage\"",
  "\"using\"", "\"value\"", "\"where\"", "\"while\"", "\"write\"",
  "\"action\"", "\"atomic\"", "\"before\"", "\"column\"", "\"commit\"",
  "\"create\"", "\"cursor\"", "\"delete\"", "\"domain\"", "\"elseif\"",
  "\"enable\"", "\"equals\"", "\"escape\"", "\"except\"", "\"exists\"",
  "\"filter\"", "\"global\"", "\"having\"", "\"insert\"", "\"method\"",
  "\"module\"", "\"nowait\"", "\"number\"", "\"option\"", "\"output\"",
  "\"rename\"", "\"repeat\"", "\"result\"", "\"return\"", "\"revoke\"",
  "\"rollup\"", "\"schema\"", "\"scroll\"", "\"search\"", "\"select\"",
  "\"signal\"", "\"simple\"", "\"static\"", "\"system\"", "\"unique\"",
  "\"unnest\"", "\"update\"", "\"values\"", "\"window\"", "\"within\"",
  "\"between\"", "\"breadth\"", "\"cascade\"", "\"catalog\"",
  "\"collate\"", "\"comment\"", "\"connect\"", "\"current\"",
  "\"declare\"", "\"default\"", "\"disable\"", "\"dynamic\"",
  "\"execute\"", "\"foreign\"", "\"general\"", "\"handler\"",
  "\"iterate\"", "\"lateral\"", "\"leading\"", "\"locator\"",
  "\"natural\"", "\"partial\"", "\"prepare\"", "\"primary\"",
  "\"release\"", "\"replace\"", "\"returns\"", "\"routine\"",
  "\"section\"", "\"session\"", "\"similar\"", "\"trigger\"",
  "\"unknown\"", "\"without\"", "\"absolute\"", "\"allocate\"",
  "\"cascaded\"", "\"continue\"", "\"deferred\"", "\"describe\"",
  "\"distinct\"", "\"external\"", "\"function\"", "\"grouping\"",
  "\"identity\"", "\"interval\"", "\"language\"", "\"modifies\"",
  "\"overlaps\"", "\"preserve\"", "\"relative\"", "\"resignal\"",
  "\"restrict\"", "\"rollback\"", "\"specific\"", "\"sqlstate\"",
  "\"trailing\"", "\"truncate\"", "\"whenever\"", "\"assertion\"",
  "\"collation\"", "\"condition\"", "\"exception\"", "\"immediate\"",
  "\"indicator\"", "\"initially\"", "\"intersect\"", "\"isolation\"",
  "\"localtime\"", "\"parameter\"", "\"partition\"", "\"precision\"",
  "\"procedure\"", "\"recursive\"", "\"savepoint\"", "\"sensitive\"",
  "\"statement\"", "\"symmetric\"", "\"temporary\"", "\"asensitive\"",
  "\"asymmetric\"", "\"connection\"", "\"constraint\"", "\"deallocate\"",
  "\"deferrable\"", "\"descriptor\"", "\"disconnect\"", "\"ordinality\"",
  "\"privileges\"", "\"references\"", "\"sqlwarning\"", "\"constraints\"",
  "\"constructor\"", "\"diagnostics\"", "\"insensitive\"",
  "\"referencing\"", "\"system_user\"", "\"transaction\"",
  "\"translation\"", "\"current_date\"", "\"current_path\"",
  "\"current_role\"", "\"current_time\"", "\"current_user\"",
  "\"session_user\"", "\"specifictype\"", "\"sqlexception\"",
  "\"authorization\"", "\"corresponding\"", "\"deterministic\"",
  "\"timezone_hour\"", "\"localtimestamp\"", "\"timezone_minute\"",
  "\"current_timestamp\"", "\"day\"", "\"max\"", "\"min\"", "\"pad\"",
  "\"sum\"", "\"cast\"", "\"hour\"", "\"last\"", "\"size\"", "\"count\"",
  "\"first\"", "\"found\"", "\"month\"", "\"minute\"", "\"second\"",
  "\"substring\"", "\"bigint\"", "\"bigserial\"", "\"bit\"", "\"varbit\"",
  "\"box\"", "\"bytea\"", "\"cidr\"", "\"circle\"", "\"inet\"", "\"json\"",
  "\"line\"", "\"lseg\"", "\"macaddr\"", "\"money\"", "\"path\"",
  "\"point\"", "\"polygon\"", "\"real\"", "\"smallserial\"", "\"serial\"",
  "\"text\"", "\"time\"", "\"timestamp\"", "\"tsquery\"", "\"tsvector\"",
  "\"txid_snapshot\"", "\"uuid\"", "\"xml\"", "\"int2\"", "\"int4\"",
  "\"int8\"", "\"serial2\"", "\"serial4\"", "\"serial8\"", "\"bool\"",
  "\"float8\"", "\"float4\"", "\"timetz\"", "\"timestamptz\"", "\"int\"",
  "\"char\"", "\"date\"", "\"year\"", "\"zone\"", "\"float\"",
  "\"double\"", "\"object\"", "\"boolean\"", "\"decimal\"", "\"integer\"",
  "\"numeric\"", "\"varchar\"", "\"varying\"", "\"smallint\"",
  "\"character\"", "$accept", "start", "semicolon_or_eof",
  "statement_list", "statement", "if_exists_clause", "only_optional",
  "restrict_or_cascade_optional", "column_optional", "enable_or_disable",
  "trigger_wildcard", "collate_clause", "not_optional",
  "deferrable_cluase", "constraint_decl_optional",
  "column_constraint_item", "delete_or_update", "references_action_clause",
  "references_match_clause", "referrences_clause", "comma_name_list",
  "column_constraint_list", "column_definition", "alter_table_action",
  "alter_table_statement", "comment_statement", "truncate_statement",
  "drop_statement", "insert_columns_clause", "insert_values_clause",
  "insert_statement", "update_set_item", "update_set_list",
  "update_set_clause", "update_statement", "delete_statement",
  "top_select_statement", "select_spec", "select_core_item",
  "sel_statement", "order_definition", "ordered_expression",
  "comma_ordered_expression_list", "order_clause",
  "comma_column_identifier_list", "nowait_clause", "update_clause",
  "group_clause", "set_clause", "where_clause", "join_spec",
  "join_condition", "joined_table", "direct_table_reference",
  "subquery_table_reference", "table_reference", "table_reference_list",
  "select_columns_item", "select_columns", "commit_statement",
  "rollback_statement", "value_expression", "binary_op",
  "binary_expression", "unary_op", "unary_expression",
  "builtin_function_name", "count_spec", "char_type_name",
  "simple_type_name", "numeric_or_decimal", "type_spec",
  "column_identifier", "case_expression_item", "case_expression_list",
  "case_expression", "case_condition_item", "case_condition_list",
  "case_condition", "primary_expression", "unknown_statement",
  "comma_expression_list", "table_identifier", "index_identifier",
  "trigger_identifier", "func_ident", "binary_cond_op",
  "conditional_expression", "conditional_term", "compare_op",
  "between_expression", "value_expression_set", "in_expression",
  "exists_expression", "null_comparison_expression", "conditional_primary",
  "empty_aka_optional", YY_NULL
  };


  const unsigned short int
  PostgresParser::yyrline_[] =
  {
       0,   480,   480,   484,   485,   489,   490,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   508,   512,   513,
     518,   519,   523,   524,   525,   529,   530,   534,   535,   539,
     540,   541,   542,   546,   547,   555,   556,   560,   561,   562,
     563,   564,   565,   569,   570,   578,   585,   592,   598,   607,
     616,   617,   621,   622,   623,   624,   625,   626,   630,   631,
     632,   633,   637,   647,   660,   667,   676,   688,   692,   701,
     710,   722,   723,   729,   735,   743,   751,   759,   767,   773,
     781,   789,   797,   808,   820,   829,   838,   847,   859,   864,
     866,   868,   870,   875,   876,   881,   883,   888,   899,   905,
     912,   922,   928,   937,   942,   949,   959,   969,   973,   974,
     975,   979,   993,  1002,  1003,  1004,  1008,  1013,  1019,  1028,
    1029,  1034,  1040,  1049,  1050,  1054,  1055,  1060,  1061,  1069,
    1080,  1081,  1086,  1091,  1096,  1101,  1109,  1110,  1118,  1119,
    1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1132,
    1133,  1141,  1147,  1157,  1163,  1169,  1178,  1186,  1194,  1205,
    1206,  1207,  1211,  1217,  1226,  1228,  1235,  1242,  1248,  1254,
    1263,  1269,  1278,  1282,  1286,  1290,  1291,  1292,  1293,  1294,
    1298,  1304,  1308,  1309,  1313,  1314,  1318,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,  1330,  1334,
    1335,  1336,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1350,
    1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,
    1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,
    1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1380,
    1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,
    1391,  1392,  1393,  1394,  1395,  1396,  1397,  1401,  1402,  1406,
    1408,  1410,  1412,  1414,  1416,  1418,  1420,  1425,  1433,  1442,
    1450,  1459,  1470,  1480,  1486,  1495,  1504,  1516,  1526,  1532,
    1541,  1549,  1560,  1566,  1572,  1574,  1576,  1578,  1587,  1596,
    1607,  1618,  1629,  1630,  1631,  1638,  1639,  1640,  1641,  1642,
    1647,  1656,  1660,  1666,  1675,  1681,  1688,  1697,  1703,  1710,
    1719,  1725,  1732,  1741,  1749,  1758,  1766,  1775,  1786,  1787,
    1791,  1792,  1801,  1802,  1810,  1811,  1812,  1813,  1814,  1815,
    1816,  1820,  1829,  1838,  1847,  1859,  1861,  1867,  1874,  1884,
    1894,  1901,  1908,  1915,  1925,  1931,  1937,  1944,  1951,  1952,
    1953,  1954,  1955,  1956,  1957,  1965
  };

  // Print the state stack on the debug stream.
  void
  PostgresParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  PostgresParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  PostgresParser::token_number_type
  PostgresParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385
    };
    const unsigned int user_token_number_max_ = 640;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // Postgres





void Postgres::PostgresParser::error( const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}
