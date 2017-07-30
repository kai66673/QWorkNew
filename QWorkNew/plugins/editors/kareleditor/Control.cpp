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

#include <QList>
#include <QString>
#include <QDebug>

#include "Control.h"
#include "KarelIdentifierConstants.h"
#include "KarelAST.h"

namespace Karel {

Scope::Scope(unsigned sourceLocation, Scope::Kind kind, Scope *parentScope)
    : m_sourceLocation(sourceLocation)
    , m_endSourceLocation(0)
    , m_kind(kind)
    , m_parentScope(parentScope)
    , m_ast(0)
{ }

Scope *Scope::scopeAt(const QList<Scope *> &subScopes, TranslationUnit *unit, unsigned line, unsigned column)
{
    if ( subScopes.isEmpty() )
        return 0;

    Scope *result = 0;
    foreach (Scope *scope, subScopes) {
        unsigned startLine, startColumn;
        unit->getPosition(unit->tokenAt(scope->sourceLocation()).begin(), &startLine, &startColumn);
        if ( line > startLine || (line == startLine && column >= startColumn) ) {
            unsigned endSourceLocation = scope->endSourceLocation();
            if ( endSourceLocation ) {
                unsigned endLine, endColumn;
                unit->getPosition(unit->tokenAt(endSourceLocation).begin(), &endLine, &endColumn);
                if ( line > endLine || (line == endLine && column >= endColumn) )
                    continue;
            }
            result = scope;
        } else {
            break;
        }
    }

    if ( result ) {
        if ( Scope *subscopeCandidate = scopeAt(result->m_subScopes, unit, line, column) )
            result = subscopeCandidate;
    }

    return result;
}

void Scope::fillRoutines(QStringList &result) const
{
    foreach (Symbol *routine, m_routineDeclarations)
        result << QString(routine->name()->charsOrig());

    if ( m_parentScope )
        m_parentScope->fillRoutines(result);
}

void Scope::fillVariables(QStringList &result) const
{
    foreach (Declaration *varDeclaration, m_varDeclarations)
        result << QString(varDeclaration->name()->charsOrig());

    if ( m_parentScope )
        m_parentScope->fillVariables(result);
}

void Scope::fillConstants(QStringList &result) const
{
    foreach (Symbol *constant, m_constantDeclarations)
        result << QString(constant->name()->charsOrig());

    if ( m_parentScope )
        m_parentScope->fillConstants(result);
}

void Scope::fillUserTypes(QStringList &result) const
{
    foreach (Declaration *typeDeclaration, m_typeDeclarations)
        result << typeDeclaration->name()->charsOrig();

    if ( m_parentScope )
        m_parentScope->fillUserTypes(result);
}

void Scope::fillUsingFields(QStringList &result) const
{
    foreach (const Type *type, m_usingTypes)
        result << type->fields();

    if ( m_parentScope )
        m_parentScope->fillUsingFields(result);
}

unsigned Scope::endSourceLocation() const
{
    return m_endSourceLocation;
}

void Scope::setEndSourceLocation(const unsigned &endSourceLocation)
{
    m_endSourceLocation = endSourceLocation;
}

AST *Scope::ast() const
{ return m_ast; }

void Scope::setAst(AST *ast)
{ m_ast = ast; }

unsigned Scope::sourceLocation() const
{ return m_sourceLocation; }

Scope *Scope::scopeAt(TranslationUnit *unit, bool &isStatementsArea, unsigned line, unsigned column)
{
    Scope *result = this;
    unsigned startLine, startColumn;
    unit->getPosition(unit->tokenAt(m_sourceLocation).begin(), &startLine, &startColumn);

    if ( line > startLine || (line == startLine && column >= startColumn) ) {
        if ( Scope *subscopeCandidate = scopeAt(m_subScopes, unit, line, column) )
            result = subscopeCandidate;
    }

    if ( result ) {
        isStatementsArea = result->m_kind == StatementsArea;
        return result;
    }

    isStatementsArea = true;
    return 0;
}

QStringList Scope::routines() const
{
    QStringList result;
    fillRoutines(result);
    return result;
}

QStringList Scope::variables() const
{
    QStringList result;
    fillVariables(result);
    return result;
}

QStringList Scope::constants() const
{
    QStringList result;
    fillConstants(result);
    return result;
}

QStringList Scope::userTypes() const
{
    QStringList result;
    fillUserTypes(result);
    return result;
}

QStringList Scope::usingFields() const
{
    QStringList result;
    fillUsingFields(result);
    return result;
}

Scope *Scope::parentScope() const
{ return m_parentScope; }

void Scope::dump() const
{
    qDebug() << "========================================================";
    qDebug() << "Address = " << this << "(" << m_sourceLocation << ")";
    qDebug() << "Childrens count = " << m_subScopes.size();
    qDebug() << "Parent = " << m_parentScope;
    qDebug() << "sourceLocation = " <<  m_sourceLocation;
    qDebug() << "kind = " << m_kind;
    qDebug() << "Constants count = " << m_constantDeclarations.size();
    qDebug() << "Routines count = " << m_routineDeclarations.size();
    qDebug() << "Types count = " << m_typeDeclarations.size();
    qDebug() << "Variables count = " << m_varDeclarations.size();
    foreach (Scope *scope, m_subScopes)
        scope->dump();
}

const Type *Scope::lookupNamedType(const Identifier *typeName, unsigned &sourceLocation) const
{
    foreach (Declaration *typeDecl, m_typeDeclarations)
        if ( typeDecl->name()->equalTo(typeName) ) {
            sourceLocation = typeDecl->tokenIndex();
            return typeDecl->type();
        }

    return m_parentScope ? m_parentScope->lookupNamedType(typeName, sourceLocation) : 0;
}

const Declaration *Scope::lookupNamedTypeDeclaration(const Identifier *typeName) const
{
    foreach (Declaration *typeDecl, m_typeDeclarations)
        if ( typeDecl->name()->equalTo(typeName) )
            return typeDecl;

    return m_parentScope ? m_parentScope->lookupNamedTypeDeclaration(typeName) : 0;
}

const Declaration *Scope::strictLookupNamedTypeDeclaration(const Identifier *typeName) const
{
    foreach (Declaration *typeDecl, m_typeDeclarations)
        if ( typeDecl->name()->equalTo(typeName) )
            return typeDecl;

    return 0;
}

const Declaration *Scope::lookupNamedTypeDeclaration(const Identifier *typeName, const Scope *&owner) const
{
    foreach (Declaration *typeDecl, m_typeDeclarations)
        if ( typeDecl->name()->equalTo(typeName) ) {
            owner = this;
            return typeDecl;
        }

    return m_parentScope ? m_parentScope->lookupNamedTypeDeclaration(typeName, owner) : 0;
}

const Symbol *Scope::lookupRoutine(const Identifier *routineName) const
{
    foreach (Symbol *routine, m_routineDeclarations)
        if ( routine->name()->equalTo(routineName) )
            return routine;

    return m_parentScope ? m_parentScope->lookupRoutine(routineName) : 0;
}

const Symbol *Scope::strictLookupRoutine(const Identifier *routineName) const
{
    foreach (Symbol *routine, m_routineDeclarations)
        if ( routine->name()->equalTo(routineName) )
            return routine;

    return 0;
}

const Symbol *Scope::lookupRoutine(const Identifier *routineName, const Scope *&owner) const
{
    foreach (Symbol *routine, m_routineDeclarations)
        if ( routine->name()->equalTo(routineName) ) {
            owner = this;
            return routine;
        }

    return m_parentScope ? m_parentScope->lookupRoutine(routineName, owner) : 0;
}

const Symbol *Scope::lookupConstant(const Identifier *constantName) const
{
    foreach (Symbol *constant, m_constantDeclarations)
        if ( constant->name()->equalTo(constantName) )
            return constant;

    return m_parentScope ? m_parentScope->lookupConstant(constantName) : 0;
}

const Symbol *Scope::strictLookupConstant(const Identifier *constantName) const
{
    foreach (Symbol *constant, m_constantDeclarations)
        if ( constant->name()->equalTo(constantName) )
            return constant;

    return 0;
}

const Symbol *Scope::lookupConstant(const Identifier *constantName, const Scope *&owner) const
{
    foreach (Symbol *constant, m_constantDeclarations)
        if ( constant->name()->equalTo(constantName) ) {
            owner = this;
            return constant;
        }

    return m_parentScope ? m_parentScope->lookupConstant(constantName, owner) : 0;
}

const Declaration *Scope::lookupVariable(const Identifier *variableName) const
{
    foreach (Declaration *variable, m_varDeclarations)
        if ( variable->name()->equalTo(variableName) )
            return variable;

    return m_parentScope ? m_parentScope->lookupVariable(variableName) : 0;
}

const Declaration *Scope::strictLookupVariable(const Identifier *variableName) const
{
    foreach (Declaration *variable, m_varDeclarations)
        if ( variable->name()->equalTo(variableName) )
            return variable;

    return 0;
}

const Declaration *Scope::lookupVariable(const Identifier *variableName, const Scope *&owner) const
{
    foreach (Declaration *variable, m_varDeclarations)
        if ( variable->name()->equalTo(variableName) ) {
            owner = this;
            return variable;
        }

    return m_parentScope ? m_parentScope->lookupVariable(variableName, owner) : 0;
}

const Symbol *Scope::lookupSymbolInDeclarationsArea(const Identifier *id)
{
    if ( const Symbol *constantSymbol = lookupConstant(id) )
        return constantSymbol;

    if ( const Declaration *typeDeclaration = lookupNamedTypeDeclaration(id) )
        return typeDeclaration;

    if ( const Symbol *routineSymbol = lookupRoutine(id) )
        return routineSymbol;

    return 0;
}

const Symbol *Scope::lookupSymbolInStatementsArea(const Identifier *id)
{
    if ( const Symbol *constantSymbol = lookupConstant(id) )
        return constantSymbol;

    if ( const Declaration *varDeclaration = lookupVariable(id) )
        return varDeclaration;

    if ( const Symbol *routineSymbol = lookupRoutine(id) )
        return routineSymbol;

    return 0;
}

const Type *Scope::lookupUsingFieldType(const Identifier *id) const
{
    foreach ( const Type *type, m_usingTypes ) {
        if ( const Type *fieldType = type->fieldType(id) )
            return fieldType;
    }

    return m_parentScope ? m_parentScope->lookupUsingFieldType(id) : 0;
}

const Type *Scope::lookupUsingTypeForField(const Identifier *id) const
{
    foreach ( const Type *type, m_usingTypes ) {
        if ( type->fieldType(id) )
            return type;
    }

    return m_parentScope ? m_parentScope->lookupUsingTypeForField(id) : 0;
}

Scope::~Scope()
{
    qDeleteAll(m_constantDeclarations);
    qDeleteAll(m_routineDeclarations);
    qDeleteAll(m_typeDeclarations);
    qDeleteAll(m_varDeclarations);
}

void Scope::appendScope(Scope *child)
{ m_subScopes.append(child); }

void Scope::appendConstant(Symbol *constantSymbol)
{ m_constantDeclarations.append(constantSymbol); }

void Scope::appendRoutine(Symbol *routineSymbol)
{ m_routineDeclarations.append(routineSymbol); }

void Scope::appendTypeDeclaration(Declaration *typeDeclaration)
{ m_typeDeclarations.append(typeDeclaration); }

void Scope::appendVarDeclaration(Declaration *typeDeclaration)
{ m_varDeclarations.append(typeDeclaration); }

void Scope::appendUsingType(const Type *type)
{ m_usingTypes.append(type); }

class Control::Data
{
public:
    Data(Control *control)
        : control(control)
        , translationUnit(0)
        , diagnosticClient(0)
        , nullType(new NullType)
        , byteType(new ByteType)
        , shortType(new ShortType)
        , integerType(new IntegerType)
        , realType(new RealType)
        , booleanType(new BooleanType)
        , stringType(new StringType)
        , fileType(new FileType)
        , camSetupType(new CamSetupType)
        , modelType(new ModelType)
        , pathType(new PathType)
        , visProcessType(new VisProcessType)
        , positionType(new PositionType)
        , configType(new ConfigType)
        , vectorType(new VectorType)
        , xyzwprType(new XYZWPRType)
        , xyzwprextType(new XYZWPREXTType)
        , jointPosType(new JointPosType)
    {
        vectorType->addField(new Declaration(&Constants::ID_X, 0, realType));
        vectorType->addField(new Declaration(&Constants::ID_Y, 0, realType));
        vectorType->addField(new Declaration(&Constants::ID_Z, 0, realType));

        configType->addField(new Declaration(&Constants::ID_CFG_TURN_NO1, 0, integerType));
        configType->addField(new Declaration(&Constants::ID_CFG_TURN_NO2, 0, integerType));
        configType->addField(new Declaration(&Constants::ID_CFG_TURN_NO3, 0, integerType));
        configType->addField(new Declaration(&Constants::ID_CFG_FLIP, 0, booleanType));
        configType->addField(new Declaration(&Constants::ID_CFG_LEFT, 0, booleanType));
        configType->addField(new Declaration(&Constants::ID_CFG_UP, 0, booleanType));
        configType->addField(new Declaration(&Constants::ID_CFG_FRONT, 0, booleanType));

        positionType->addField(new Declaration(&Constants::ID_NORMAL, 0, vectorType));
        positionType->addField(new Declaration(&Constants::ID_ORIENT, 0, vectorType));
        positionType->addField(new Declaration(&Constants::ID_APPROACH, 0, vectorType));
        positionType->addField(new Declaration(&Constants::ID_LOCATION, 0, vectorType));
        positionType->addField(new Declaration(&Constants::ID_CONFIG_DATA, 0, configType));

        xyzwprType->addField(new Declaration(&Constants::ID_X, 0, realType));
        xyzwprType->addField(new Declaration(&Constants::ID_Y, 0, realType));
        xyzwprType->addField(new Declaration(&Constants::ID_Z, 0, realType));
        xyzwprType->addField(new Declaration(&Constants::ID_W, 0, realType));
        xyzwprType->addField(new Declaration(&Constants::ID_P, 0, realType));
        xyzwprType->addField(new Declaration(&Constants::ID_R, 0, realType));
        xyzwprType->addField(new Declaration(&Constants::ID_CONFIG_DATA, 0, configType));

        xyzwprextType->addField(new Declaration(&Constants::ID_X, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_Y, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_Z, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_W, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_P, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_R, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_CONFIG_DATA, 0, configType));
        xyzwprextType->addField(new Declaration(&Constants::ID_EXT1, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_EXT2, 0, realType));
        xyzwprextType->addField(new Declaration(&Constants::ID_EXT3, 0, realType));
    }

    ~Data()
    {
        qDeleteAll(identifiers);
        qDeleteAll(scopes);
        qDeleteAll(types);

        delete nullType;
        delete byteType;
        delete shortType;
        delete integerType;
        delete realType;
        delete booleanType;
        delete stringType;
        delete fileType;
        delete camSetupType;
        delete modelType;
        delete pathType;
        delete visProcessType;
        delete positionType;
        delete vectorType;
        delete configType;
        delete xyzwprType;
        delete xyzwprextType;
        delete jointPosType;
    }

    Control *control;
    TranslationUnit *translationUnit;
    DiagnosticClient *diagnosticClient;
    QList<Identifier *> identifiers;
    QList<Scope *> scopes;
    QList<Type *> types;

    NullType *nullType;
    ByteType *byteType;
    ShortType *shortType;
    IntegerType *integerType;
    RealType *realType;
    BooleanType *booleanType;
    StringType *stringType;
    FileType *fileType;
    CamSetupType *camSetupType;
    ModelType *modelType;
    PathType *pathType;
    VisProcessType *visProcessType;
    PositionType *positionType;
    VectorType *vectorType;
    ConfigType *configType;
    XYZWPRType *xyzwprType;
    XYZWPREXTType *xyzwprextType;
    JointPosType *jointPosType;
};

Control::Control()
{
    d = new Data(this);
}

Control::~Control()
{ delete d; }

TranslationUnit *Control::translationUnit() const
{ return d->translationUnit; }

TranslationUnit *Control::switchTranslationUnit(TranslationUnit *unit)
{
    TranslationUnit *previousTranslationUnit = d->translationUnit;
    d->translationUnit = unit;
    return previousTranslationUnit;
}

DiagnosticClient *Control::diagnosticClient() const
{ return d->diagnosticClient; }

void Control::setDiagnosticClient(DiagnosticClient *diagnosticClient)
{ d->diagnosticClient = diagnosticClient; }

Identifier *Control::identifier(const char *id, int length)
{
    if ( length <= 0 )
        return 0;
    Identifier *ident = new Identifier(id, length);
    d->identifiers.append(ident);
    return ident;
}

Scope *Control::scope(unsigned sourceLocation, Scope::Kind kind, Scope *parentScope)
{
    Scope *scope = new Scope(sourceLocation, kind, parentScope);
    d->scopes.append(scope);
    return scope;
}

StructureType *Control::structureType()
{
    StructureType *type = new StructureType;
    d->types.append(type);
    return type;
}

ArrayType *Control::arrayType(const Type *itemType)
{
    ArrayType *type = new ArrayType(itemType);
    d->types.append(type);
    return type;
}

NamedType *Control::namedType(const Identifier *name)
{
    NamedType *type = new NamedType(name);
    d->types.append(type);
    return type;
}

Type *Control::builtingType(Type::Kind kind)
{
    switch ( kind ) {
        case Type::Null:                return d->nullType;
        case Type::BuiltingByte:        return d->byteType;
        case Type::BuiltingShort:       return d->shortType;
        case Type::BuiltingInteger:     return d->integerType;
        case Type::BuiltingReal:        return d->realType;
        case Type::BuiltingBoolean:     return d->booleanType;
        case Type::BuiltingString:      return d->stringType;
        case Type::BuiltingFile:        return d->fileType;
        case Type::BuiltingCamSetup:    return d->camSetupType;
        case Type::BuiltingModel:       return d->modelType;
        case Type::BuiltingPath:        return d->pathType;
        case Type::BuiltingVisProcess:  return d->visProcessType;
        case Type::BuiltingJointPos:    return d->jointPosType;

        case Type::BuiltingPosition:    return d->positionType;
        case Type::BuiltingVector:      return d->vectorType;
        case Type::BuiltingConfig:      return d->configType;
        case Type::BuiltingXYZWPR:      return d->xyzwprType;
        case Type::BuiltingXYZWPREXT:   return d->xyzwprextType;
    }

    return 0;
}

}   // namespace Karel
