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

#include "cpplocalsymbols.h"
#include "cppsemanticinfo.h"

#include <CppDocument.h>
#include <ASTVisitor.h>
#include <AST.h>
#include <Scope.h>
#include <Symbols.h>
#include <CoreTypes.h>
#include <Names.h>
#include <Literals.h>
#include <TranslationUnit.h>

using namespace CPlusPlus;
using namespace CppEditor::Internal;

namespace {

class FindLocalSymbols: protected ASTVisitor
{
    Scope *_functionScope;
    Document::Ptr _doc;

public:
    FindLocalSymbols(Document::Ptr doc)
        : ASTVisitor(doc->translationUnit()), _doc(doc), hasD(false), hasQ(false)
    { }

    // local and external uses.
    SemanticInfo::LocalUseMap localUses;
    bool hasD;
    bool hasQ;

    void operator()(DeclarationAST *ast)
    {
        localUses.clear();

        if (!ast)
            return;

        if (FunctionDefinitionAST *def = ast->asFunctionDefinition()) {
            if (def->symbol) {
                _functionScope = def->symbol;
                accept(ast);
            }
        } else if (ObjCMethodDeclarationAST *decl = ast->asObjCMethodDeclaration()) {
            if (decl->method_prototype->symbol) {
                _functionScope = decl->method_prototype->symbol;
                accept(ast);
            }
        }
    }

protected:
    using ASTVisitor::visit;
    using ASTVisitor::endVisit;

    void enterScope(Scope *scope)
    {
        _scopeStack.append(scope);

        for (unsigned i = 0; i < scope->memberCount(); ++i) {
            if (Symbol *member = scope->memberAt(i)) {
                unsigned extraLength = member->extraLength();
                if (member->isTypedef())
                    continue;
                else if ( (!member->isGenerated() || extraLength) && (member->isDeclaration() || member->isArgument()) ) {
                    if (member->name() && member->name()->isNameId()) {
                        const Identifier *id = member->identifier();
                        unsigned line, column;
                        getTokenStartPosition(member->sourceLocation(), &line, &column);
                        localUses[member].append( SemanticInfo::Use( line, column,
                                                                     extraLength ? extraLength : id->size(),
                                                                     extraLength ? SemanticInfo::Use::OnlyOccurance : SemanticInfo::Use::Local ) );
                    }
                }
            }
        }
    }

    virtual bool visit(IdExpressionAST *ast)
    {
        if (SimpleNameAST *simpleName = ast->name->asSimpleName()) {
            const Identifier *id = identifier(simpleName->identifier_token);
            for (int i = _scopeStack.size() - 1; i != -1; --i) {
                if (Symbol *member = _scopeStack.at(i)->find(id)) {
                    if (member->isTypedef())
                        continue;
                    else if (!member->isGenerated() && (member->sourceLocation() < ast->firstToken() || member->enclosingScope()->isFunction())) {
                        unsigned line, column;
                        getTokenStartPosition(simpleName->identifier_token, &line, &column);
                        localUses[member].append(SemanticInfo::Use(line, column, id->size(), member->extraLength() ? SemanticInfo::Use::OnlyOccurance : SemanticInfo::Use::Local));
                        return false;
                    }
                }
            }
        }

        return true;
    }

    virtual bool visit( ExecSqlStatementAST *ast )
    {
        if ( ast->symbol )
            _scopeStack.append(ast->symbol);

        SimpleNameListAST *iter;
        SimpleNameListAST *iter1;

        // bool blocked = _doc->translationUnit()->blockErrors(false);

        for ( iter = ast->var_name_list; iter; iter = iter->next ) {
            const Identifier *id = identifier(iter->value->identifier_token);
            for (int i = _scopeStack.size() - 1; i != -1; --i) {
                if (Symbol *member = _scopeStack.at(i)->find(id)) {
                    if (member->isTypedef())
                        continue;
                    else if (!member->isGenerated() && (member->sourceLocation() < ast->firstToken() || member->enclosingScope()->isFunction())) {
                        unsigned line, column;
                        getTokenStartPosition(iter->value->identifier_token, &line, &column);
                        localUses[member].append(SemanticInfo::Use(line, column, id->size(), SemanticInfo::Use::Local));
                        break;
                    }
                }
            }
        }

        for ( iter = ast->table_name_list, iter1 = ast->alias_name_list;
              iter && iter1;
              iter = iter->next, iter1 = iter1->next ) {

            const Identifier *id = identifier(iter->value->identifier_token);
            for (int i = _scopeStack.size() - 1; i != -1; --i) {
                if (Symbol *member = _scopeStack.at(i)->find(id)) {
                    if (member->isTypedef())
                        continue;
                    else if (!member->isGenerated() && (member->sourceLocation() < ast->firstToken() || member->enclosingScope()->isFunction())) {
                        unsigned line, column;
                        getTokenStartPosition(iter->value->identifier_token, &line, &column);
                        localUses[member].append(SemanticInfo::Use(line, column, id->size(), SemanticInfo::Use::Type));
                        break;
                    }
                }
            }

            const Identifier *id1 = identifier(iter1->value->identifier_token);
            if ( ast->symbol ) {
                if ( Symbol *member = ast->symbol->find(id1) ) {
                    unsigned line, column;
                    getTokenStartPosition(iter1->value->identifier_token, &line, &column);
                    if ( iter->value->identifier_token != iter1->value->identifier_token )
                        localUses[member].append(SemanticInfo::Use(line, column, id1->size(), SemanticInfo::Use::SqlTableAlias));
                    else
                        localUses[member].append(SemanticInfo::Use(line, column, id1->size(), SemanticInfo::Use::SqlTableAliasShadowed));
                }
            }
        }

        for ( iter = ast->alias_uses_list; iter; iter = iter->next ) {
            const Identifier *id = identifier(iter->value->identifier_token);
            if ( ast->symbol ) {
                if ( Symbol *member = ast->symbol->find(id) ) {
                    unsigned line, column;
                    getTokenStartPosition(iter->value->identifier_token, &line, &column);
                    localUses[member].append(SemanticInfo::Use(line, column, id->size(), SemanticInfo::Use::SqlTableAlias));
                }
            }
        }

        // _doc->translationUnit()->blockErrors(blocked);

        return true;
    }

    virtual void endVisit(ExecSqlStatementAST *ast)
    {
        if ( ast->symbol )
            _scopeStack.removeLast();
    }

    virtual bool visit(QtMemberDeclarationAST *ast)
    {
        if (tokenKind(ast->q_token) == T_Q_D)
            hasD = true;
        else
            hasQ = true;

        return true;
    }

    virtual bool visit(FunctionDefinitionAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(FunctionDefinitionAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(CompoundStatementAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(CompoundStatementAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(IfStatementAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(IfStatementAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(WhileStatementAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(WhileStatementAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(ForStatementAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(ForStatementAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(ForeachStatementAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(ForeachStatementAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(SwitchStatementAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(SwitchStatementAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(CatchClauseAST *ast)
    {
        if (ast->symbol)
            enterScope(ast->symbol);
        return true;
    }

    virtual void endVisit(CatchClauseAST *ast)
    {
        if (ast->symbol)
            _scopeStack.removeLast();
    }

    virtual bool visit(ExpressionOrDeclarationStatementAST *ast)
    {
        accept(ast->declaration);
        return false;
    }

private:
    QList<Scope *> _scopeStack;
};

} // end of anonymous namespace


LocalSymbols::LocalSymbols(CPlusPlus::Document::Ptr doc, CPlusPlus::DeclarationAST *ast)
{
    FindLocalSymbols findLocalSymbols(doc);
    findLocalSymbols(ast);
    hasD = findLocalSymbols.hasD;
    hasQ = findLocalSymbols.hasQ;
    uses = findLocalSymbols.localUses;
}
