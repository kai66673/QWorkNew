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

#ifndef KAREL_CONTROL_H
#define KAREL_CONTROL_H

#include "TranslationUnit.h"
#include "KarelType.h"
#include "KarelASTfwd.h"

namespace Karel {

class Scope
{
public:
    enum Kind {
        DeclarationsArea,
        StatementsArea
    };

    virtual ~Scope();

    void appendScope(Scope *child);
    void appendConstant(Symbol *constantSymbol);
    void appendRoutine(Symbol *routineSymbol);
    void appendTypeDeclaration(Declaration *typeDeclaration);
    void appendVarDeclaration(Declaration *typeDeclaration);
    void appendUsingType(const Type *type);

    Scope *parentScope() const;

    void dump() const;

    const Type *lookupNamedType(const Identifier *typeName, unsigned &sourceLocation) const;
    const Declaration *lookupNamedTypeDeclaration(const Identifier *typeName) const;
    const Declaration *strictLookupNamedTypeDeclaration(const Identifier *typeName) const;
    const Declaration *lookupNamedTypeDeclaration(const Identifier *typeName, const Scope *&owner) const;

    const Symbol *lookupRoutine(const Identifier *routineName) const;
    const Symbol *strictLookupRoutine(const Identifier *routineName) const;
    const Symbol *lookupRoutine(const Identifier *routineName, const Scope *&owner) const;

    const Symbol *lookupConstant(const Identifier *constantName) const;
    const Symbol *strictLookupConstant(const Identifier *constantName) const;
    const Symbol *lookupConstant(const Identifier *constantName, const Scope *&owner) const;

    const Declaration *lookupVariable(const Identifier *variableName) const;
    const Declaration *strictLookupVariable(const Identifier *variableName) const;
    const Declaration *lookupVariable(const Identifier *variableName, const Scope *&owner) const;

    const Symbol *lookupSymbolInDeclarationsArea(const Identifier *id);
    const Symbol *lookupSymbolInStatementsArea(const Identifier *id);

    const Type *lookupUsingFieldType(const Identifier *id) const;
    const Type *lookupUsingTypeForField(const Identifier *id) const;

    unsigned sourceLocation() const;

    Scope *scopeAt(TranslationUnit *unit, bool &isStatementsArea, unsigned line, unsigned column = 0);
    QStringList routines() const;
    QStringList variables() const;
    QStringList constants() const;
    QStringList userTypes() const;
    QStringList usingFields() const;

    AST *ast() const;
    void setAst(AST *ast);

    unsigned endSourceLocation() const;
    void setEndSourceLocation(const unsigned &endSourceLocation);

private:
    Scope(unsigned sourceLocation, Kind kind = DeclarationsArea, Scope *parentScope = 0);
    Scope *scopeAt(const QList<Scope *> &subScopes, TranslationUnit *unit, unsigned line, unsigned column = 0);
    void fillRoutines(QStringList &result) const;
    void fillVariables(QStringList &result) const;
    void fillConstants(QStringList &result) const;
    void fillUserTypes(QStringList &result) const;
    void fillUsingFields(QStringList &result) const;

    unsigned m_sourceLocation;
    unsigned m_endSourceLocation;
    Kind m_kind;
    Scope *m_parentScope;
    QList<Scope *> m_subScopes;

    QList<Symbol *> m_constantDeclarations;
    QList<Symbol *> m_routineDeclarations;
    QList<Declaration *> m_typeDeclarations;
    QList<Declaration *> m_varDeclarations;
    QList<const Type *> m_usingTypes;

    AST *m_ast;

    friend class Control;
};

class Control
{
public:
    Control();
    ~Control();

    TranslationUnit *translationUnit() const;
    TranslationUnit *switchTranslationUnit(TranslationUnit *unit);

    DiagnosticClient *diagnosticClient() const;
    void setDiagnosticClient(DiagnosticClient *diagnosticClient);

    Identifier *identifier(const char *id, int length);
    Scope *scope(unsigned sourceLocation, Scope::Kind kind = Scope::DeclarationsArea, Scope *parentScope = 0);

    StructureType *structureType();
    ArrayType *arrayType(const Type *itemType);
    NamedType *namedType(const Identifier *name);
    Type *builtingType(Type::Kind kind);

private:
    class Data;
    friend class Data;
    Data *d;
};

} // namespace Karel


#endif // KAREL_CONTROL_H
