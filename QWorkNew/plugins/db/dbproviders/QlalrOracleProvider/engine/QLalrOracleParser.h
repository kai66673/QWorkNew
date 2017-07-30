
/********************************************************************************
** QLalr Parser Header
********************************************************************************/
#include "ISemanticParser.h"
#include "qlalroracleparsertable_p.h"
#include "QLalrOracleAST.h"
#include "SqlToken.h"
#include "TranslationUnit.h"
#include "MemoryPool.h"
#include <vector>
#include <stack>

#define MAKE_LIST_FIRST(type, li, vl, vi) if ( sym(li).vi ) { \
        sym(li).vl = makeAstNode<type>(sym(1).vi); \
    } else { \
        sym(li).vl = 0; \
    }


#define MAKE_LIST_NEXT(type, ll, li, vl, vi) if ( sym(ll).vl && sym(li).vi ) { \
        sym(ll).vl = makeAstNode<type>(sym(ll).vl, sym(li).vi); \
    } else if ( !sym(ll).vl ) { \
        if ( sym(li).vi ) { \
            sym(ll).vl = makeAstNode<type>(sym(li).vi); \
        } else { \
            sym(ll).vl = 0; \
        } \
    }

namespace QLalrOracle {

class Parser: public QLalrOracleParserTable, public Sql::ISemanticParser
{
public:
    union Value {
        void *ptr;
        AST *ast;
        NameAST *name;
        List<AST *> *ast_list;
        StatementAST *statement;
        List<StatementAST *> *statement_list;
        ValueExpressionAST *expression;
        ConditionExpressionAST *condition;
        OrderingTermAST *ordered_expression;
        List<ValueExpressionAST *> *expression_list;
        List<OrderingTermAST *> *ordered_expression_list;
        ColumnIdentifierAST *column_ident;
        List<ColumnIdentifierAST *> *column_ident_list;
        IndexIdentifierAST *index_ident;
        TriggerIdentifierAST *trigger_ident;
        TableIdentifierAST *table_ident;
        List<TableIdentifierAST *> *table_ident_list;
        AssignmentExpressionAST *update_assign;
        AssignmentExpressionListAST *update_assign_list;
        BaseSelectColumnItemAST *sel_col;
        List<BaseSelectColumnItemAST *> *sel_col_list;
        ColumnDefinitionListAST *col_def_list;
        TableConstraintAST *table_constraint;
        TableConstraintListAST *table_constraint_list;
        SortedColumnNameListAST *ordered_column_list;
        SortedColumnNameAST *ordered_column;
        NameListAST *column_name_list;
        NameAST *column_name;
        ColumnDefinitionAST *column_definition;
        ColumnDefinitionListAST *column_definition_list;
        ColumnConstraintAST *column_constraint;
        ColumnConstraintListAST *column_constraint_list;
        TranslationUnitAST *translation_unit;
        AST::Kind kind;
    };
    
    Parser( Sql::TranslationUnit *translationUnit );
    ~Parser();
    
    virtual Sql::BaseWalker *parseDocument( Sql::ParseTarget target );
    virtual void fillSqlStatements( QList<Sql::SqlStatement *> *statements,
                                    Database::ExecuteType executeType, ITextDocument *textDocument );
    virtual void fillStatementFromText( QList<Sql::SqlStatement *> *statements, const QString &statementText );
    virtual int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, unsigned position,
                                 int startPosition, unsigned completionOperator );

    TranslationUnitAST *parse() {
        if (AST *u = parse(T_QLO_FEED_ORACLE))
            return u->asTranslationUnit();
        return 0;
    }

    BaseExpressionAST *parseExpression() {
        if (AST *u = parse(T_QLO_FEED_EXPRESSION))
            return u->asBaseExpression();
        return 0;
    }

    AST *parse(int startToken);
    
    TranslationUnitAST *parseStatements();
    StatementAST *parseOneStatement();
    
private:
    int &location(int n) { return _locationStack[_tos + n - 1]; }
    Value &sym(int n) { return _symStack[_tos + n - 1]; }
    AST *&ast(int n) { return _symStack[_tos + n - 1].ast; }
    NameAST *&name(int n) { return _symStack[_tos + n - 1].name; }
    ValueExpressionAST *expression(int n) {
        ValueExpressionAST *result = _symStack[_tos + n - 1].expression;
        if ( !result )
            error(location(n - 1) + 1, "Expected Expression");
        return result;
    }
    OrderingTermAST *&ordered_expression(int n) { return _symStack[_tos + n - 1].ordered_expression; }
    ConditionExpressionAST *condition(int n) {
        ConditionExpressionAST *result = _symStack[_tos + n - 1].condition;
        if ( !result )
            error(location(n - 1) + 1, "Expected Conditional Expression");
        return result;
    }
    StatementAST *&statement(int n) { return _symStack[_tos + n - 1].statement; }
    ColumnIdentifierAST *&column_ident(int n) { return _symStack[_tos + n - 1].column_ident; }
    TableIdentifierAST *&table_ident(int n) { return _symStack[_tos + n - 1].table_ident; }
    IndexIdentifierAST *&index_ident(int n) { return _symStack[_tos + n - 1].index_ident; }
    TriggerIdentifierAST *&trigger_ident(int n) { return _symStack[_tos + n - 1].trigger_ident; }
    BaseSelectColumnItemAST *&select_column(int n) { return _symStack[_tos + n - 1].sel_col; }
    
    inline int consumeToken() {
        if (_index < int(_tokens->size()))
            return _index++;
        return _tokens->size() - 1;
    }
    inline const Sql::Token &tokenAt(int index) const {
        if (index == 0)
            return _startToken;
        return _tokens->at(index);
    }
    inline int tokenKind(int index) const {
        if (index == 0)
            return _startToken.m_kindGlobal;
        return _tokens->at(index).m_kindGlobal;
    }
    
    inline int statementConsumeToken() {
        return _index++;
    }
    
    inline int statementTokenKind( int index ) const {
        if ( index == 0 )
            return T_QLO_FEED_STATEMENT;
        if ( index == _statementLength )
            return EOF_SYMBOL;
        return _tokens->at(index + _statementOffset).m_kindGlobal;
    }
    
    void reduce(int ruleno);

    void warning(int index, const QString &message)
    {
        _translationUnit->warningString(index, message);
    }

    void error(int index, const QString &message)
    {
        _translationUnit->errorString(index, message);
    }
    
    void error(int startIndex, int endIndex, const QString &message)
    {
        _translationUnit->errorString(startIndex, endIndex, message);
    }
    
    NameAST *createName( NameAST::Kind kind, int offset ) {
        return new (_pool) NameAST(kind, offset, _tokens->at(offset).identifier);
    }
    
    template <typename T>
    T *makeAstNode()
    {
        T *node = new (_pool) T ();
        return node;
    }
    
    template <typename T, typename A1>
    T *makeAstNode(A1 a1)
    {
        T *node = new (_pool) T (a1);
        return node;
    }

    template <typename T, typename A1, typename A2>
    T *makeAstNode(A1 a1, A2 a2)
    {
        T *node = new (_pool) T (a1, a2);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3>
    T *makeAstNode(A1 a1, A2 a2, A3 a3)
    {
        T *node = new (_pool) T (a1, a2, a3);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4)
    {
        T *node = new (_pool) T (a1, a2, a3, a4);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9);
        return node;
    }

private:
    Sql::TranslationUnit *_translationUnit;
    Utils::MemoryPool *_pool;
    
    int _tos;
    int _index;
    int yyloc;
    int yytoken;
    int yyrecovering;
    bool _recovered;
    Sql::Token _startToken;
    std::vector<int> _stateStack;
    std::vector<int> _locationStack;
    std::vector<Value> _symStack;
    std::vector<Sql::Token> *_tokens;
    
    int _statementOffset;
    int _statementLength;
};

} // namespace QLalrOracle
