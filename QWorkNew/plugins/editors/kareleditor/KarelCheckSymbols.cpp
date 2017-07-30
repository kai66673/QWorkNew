#include <QStack>

#include <algorithm.h>
#include <qtcassert.h>

#include "KarelCheckSymbols.h"
#include "KarelAST.h"
#include "KarelKeywords.h"

namespace Karel {

static bool sortByLinePredicate(const CheckSymbols::Result &lhs, const CheckSymbols::Result &rhs)
{
    if (lhs.line == rhs.line)
        return lhs.column < rhs.column;
    else
        return lhs.line < rhs.line;
}

CheckSymbols::~CheckSymbols()
{ }

void CheckSymbols::run()
{
    if ( !isCanceled() ) {
        if ( TranslationUnit *unit = _doc->translationUnit() ) {
            accept(unit->ast());
            flush();
        }
    }

    reportFinished();
}

CheckSymbols::Future CheckSymbols::go(Document::Ptr doc)
{
    QTC_ASSERT(doc, return Future());
    QTC_ASSERT(doc->translationUnit()->ast(), return Future());

    return (new CheckSymbols(doc))->start();
}

CheckSymbols::CheckSymbols(Document::Ptr doc)
    : ASTVisitor(doc->translationUnit())
    , _doc(doc)
    , _scope(doc->globalScope())
{
    _nulltype = control()->builtingType(Type::Null);

    unsigned line = 0;
    getTokenEndPosition(translationUnit()->ast()->lastToken(), &line, 0);
    _chunkSize = qMax(50U, line / 200);
    _usages.reserve(_chunkSize);
}

bool CheckSymbols::preVisit(AST *)
{ return !isCanceled(); }

bool CheckSymbols::visit(ModuleAST *ast)
{
    accept(ast->programName);
    accept(ast->directives);
    flush();
    accept(ast->declarations);
    flush();
    for (RoutineDeclarationListAST *it1 = ast->innerRoutines; it1; it1 = it1->next) {
        accept(it1->value);
        flush();
    }
    accept(ast->statements);
    accept(ast->programName1);
    flush();
    for (RoutineDeclarationListAST *it2 = ast->outerRoutines; it2; it2 = it2->next) {
        accept(it2->value);
        flush();
    }

    return false;
}

bool CheckSymbols::visit(RoutineDeclarationAST *ast)
{
    const Scope *oldScope = _scope;
    _scope = ast->scope;
    accept(ast->definition);
    accept(ast->implementation);
    _scope = oldScope;
    return false;
}

bool CheckSymbols::visit(ConstDeclarationItemAST *ast)
{
    if ( ast->constantName )
        addUse(ast->constantName->t_identifier, Result::Constant);

    accept(ast->value);
    return false;
}

bool CheckSymbols::visit(TypeDeclarationItemAST *ast)
{
    if ( ast->typeName )
        addUse(ast->typeName->t_identifier, Result::UserType);

    accept(ast->fromProgram);
    accept(ast->userType);
    return false;
}

bool CheckSymbols::visit(FieldDefinitionAST *ast)
{
    if ( ast->fieldName )
        addUse(ast->fieldName->t_identifier, Result::TypeField);

    accept(ast->type);
    return false;
}

bool CheckSymbols::visit(ConstantNameAST *ast)
{
    if ( const Symbol *constantSymbol = _scope->lookupConstant(translationUnit()->tokenAt(ast->t_identifier).identifier) )
        if ( ast->t_identifier > constantSymbol->tokenIndex() )
            addUse(ast->t_identifier, Result::Constant);

    return false;
}

bool CheckSymbols::visit(TypeNameAST *ast)
{
    if ( ast->type )
        addUse(ast->t_identifier, Result::UserType);

    return false;
}

bool CheckSymbols::visit(VariableNameAST *ast)
{
    addUse(ast->t_identifier, Result::Variable);
    return false;
}

bool CheckSymbols::visit(ProgramNameAST *ast)
{
    addUse(ast->t_identifier, Result::ProgramName);
    return false;
}

bool CheckSymbols::visit(RoutineNameAST *ast)
{
    addUse(ast->t_identifier, Result::Routine);
    return false;
}

bool CheckSymbols::visit(FakeVariableStatementAST *ast)
{
    _type = 0;
    accept(ast->var);
    return false;
}

bool CheckSymbols::visit(UsingStatementAST *ast)
{
    for (VariableAccessListAST *it = ast->structVars; it; it = it->next) {
        _type = 0;
        accept(it->value);
    }
    const Scope *oldScope = _scope;
    _scope = ast->scope;
    accept(ast->statements);
    _scope = oldScope;
    return false;
}

bool CheckSymbols::visit(PrimaryIdAST *ast)
{
    unsigned tokenIndex = ast->variable->t_identifier;
    const Token &tk = tokenAt(tokenIndex);

    if ( _type ) {
        if ( const Declaration *decl = _type->fieldDeclaration(tk.identifier) ) {
            _type = decl->type();
            addUse(tokenIndex, Result::TypeField);
        } else {
            _type = 0;
        }
    } else {
        if ( _scope->lookupConstant(tk.identifier) ) {
            addUse(tokenIndex, Result::Constant);
        } else if ( const Declaration *decl = _scope->lookupVariable(tk.identifier) ) {
            _type = decl->type();
            addUse(tokenIndex, Result::Variable);
        } else if ( const Type *fieldType = _scope->lookupUsingFieldType(tk.identifier) ) {
            _type = fieldType;
            addUse(tokenIndex, Result::TypeField);
        } else if ( const Symbol *routineSymbol = _scope->lookupRoutine(tk.identifier) ) {
            if ( tokenIndex > routineSymbol->tokenIndex() )
                addUse(tokenIndex, Result::Routine);
        }
    }

    if ( !_type )
        _type = _nulltype;

    return false;
}

bool CheckSymbols::visit(ArrayIdAST *ast)
{
    accept(ast->id);
    if ( _type ) {
        _type = _type->arrayItemtype();
        if ( !_type )
            _type = _nulltype;
    }

    const Type *oldType = _type;
    _type = 0;
    accept(ast->vars);
    _type = oldType;

    return false;
}

void CheckSymbols::proceedStatementList(unsigned beginLocation)
{   /// FIXME: proceed real statements...
    if ( !beginLocation )
        return;

    QStack<const Type *> typeForBracketDepth;
    const Type *currentType = 0;

    unsigned prevTokenKind = T_SEMICOLON;
    unsigned tokenIndex = beginLocation + 1;
    while (prevTokenKind && !isCanceled()) {
        const Token &tk = translationUnit()->tokenAt(tokenIndex);

        if ( tk.keyid() == T_END )
            break;

        unsigned tokenKind = tk.f.kind;

        switch (tokenKind) {
            case T_LBRACKET:
                typeForBracketDepth.push(currentType);
                currentType = 0;
                break;
            case T_RBRACKET:
                if ( !typeForBracketDepth.isEmpty() ) {
                    currentType = typeForBracketDepth.pop();
                    if ( currentType )
                        currentType = currentType->arrayItemtype();
                } else {
                    currentType = 0;
                }
                break;
            case T_IDENTIFIER:
                if ( prevTokenKind == T_DOT && currentType ) {
                    currentType = currentType->fieldType(tk.identifier);
                    if ( currentType )
                        addUse(tokenIndex, Result::TypeField);
                } else if ( prevTokenKind != T_DOT ) {
                    currentType = 0;
                    if ( _scope->lookupConstant(tk.identifier) )
                        addUse(tokenIndex, Result::Constant);
                    else if ( const Declaration *varDecl = _scope->lookupVariable(tk.identifier) ) {
                        currentType = varDecl->type();
                        if ( currentType )
                            addUse(tokenIndex, Result::Variable);
                    }
                    else if ( const Symbol *routineSymbol = _scope->lookupRoutine(tk.identifier) ) {
                        if ( tokenIndex > routineSymbol->tokenIndex() )
                            addUse(tokenIndex, Result::Routine);
                    }
                }
                break;
            case T_DOT:
                break;
            default:
                currentType = 0;
                break;
        }

        prevTokenKind = tokenKind;
        tokenIndex++;
    }
}

void CheckSymbols::flush()
{
    if (_usages.isEmpty())
        return;

    Utils::sort(_usages, sortByLinePredicate);
    reportResults(_usages);
    int cap = _usages.capacity();
    _usages.clear();
    _usages.reserve(cap);
}

void CheckSymbols::addUse(unsigned tokenIndex, Kind kind)
{
    if (! tokenIndex)
        return;

    const Token &tok = tokenAt(tokenIndex);
    if (tok.generated())
        return;

    unsigned line, column;
    getTokenStartPosition(tokenIndex, &line, &column);
    const unsigned length = tok.length();

    const Result use(line, column, length, kind);
    _usages.append(use);
}


}   // namespace Karel
