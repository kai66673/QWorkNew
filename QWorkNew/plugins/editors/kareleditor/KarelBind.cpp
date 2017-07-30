#include <QPair>
#include <QDebug>

#include "KarelBind.h"
#include "KarelAST.h"

namespace Karel {

Bind::Bind(TranslationUnit *unit)
    : ASTVisitor(unit)
    , _scope(0)
    , _control(translationUnit()->control())
    , _type(0)
{ }

void Bind::operator()(ModuleAST *ast, Scope *globalScope)
{
    Scope *previousScope = switchScope(globalScope);
    (void) switchScope(previousScope);
    accept(ast->declarations);
    accept(ast->innerRoutines);
    if ( ast->t_begin ) {
        Scope *mainScope = _control->scope(ast->t_begin, Scope::StatementsArea, _scope);
        _scope->appendScope(mainScope);
        _scope = mainScope;
        accept(ast->statements);
        _scope = _scope->parentScope();
    }
    accept(ast->outerRoutines);
}

Scope *Bind::switchScope(Scope *scope)
{
    if (! scope)
        return _scope;

    std::swap(_scope, scope);
    return scope;
}

bool Bind::visit(RoutineDeclarationAST *ast)
{
    if ( ast->definition && ast->definition->routineName )
        _scope->appendRoutine(new Symbol(translationUnit()->tokenAt(ast->definition->routineName->t_identifier).identifier,
                                         ast->definition->routineName->t_identifier));
    Scope *newScope = _control->scope(ast->firstToken(), Scope::DeclarationsArea, _scope);
    newScope->setAst(ast);
    ast->scope = newScope;
    _scope->appendScope(newScope);
    (void) switchScope(newScope);
    return true;
}

void Bind::endVisit(RoutineDeclarationAST *)
{
    (void) switchScope(_scope->parentScope());
}

bool Bind::visit(DirectRoutineImplemenationAST *ast)
{
    accept(ast->declarations);

    if ( ast->t_begin ) {
        Scope *routineImplementationScope = _control->scope(ast->t_begin, Scope::StatementsArea, _scope);
        _scope->appendScope(routineImplementationScope);
        _scope = routineImplementationScope;
        accept(ast->statements);
        _scope = _scope->parentScope();
        accept(ast->routineName);
    }

    return false;
}

bool Bind::visit(ConstDeclarationItemAST *ast)
{
    if ( ast->constantName )
        _scope->appendConstant(new Symbol(translationUnit()->tokenAt(ast->constantName->t_identifier).identifier,
                                          ast->constantName->t_identifier));
    return false;
}

bool Bind::visit(TypeDeclarationItemAST *ast)
{
    if ( ast->typeName && ast->userType ) {
        if ( StructureUserTypeAST *structureType = ast->userType->asStructureUserType() ) {
            StructureType *type = _control->structureType();
            for (FieldDefinitionListAST *iter = structureType->fields; iter; iter = iter->next) {
                if ( FieldDefinitionAST *field = iter->value ) {
                    if ( field->fieldName && field->type ) {
                        _type = 0;
                        accept(field->type);
                        if ( _type )
                            type->addField(new Declaration(translationUnit()->tokenAt(field->fieldName->t_identifier).identifier,
                                                           field->fieldName->t_identifier, _type));
                    }
                }
            }
            const Identifier *id = translationUnit()->tokenAt(ast->typeName->t_identifier).identifier;
            NamedType *namedTy = _control->namedType(id);
            namedTy->bind(type);
            ast->typeName->type = namedTy;
            _scope->appendTypeDeclaration(new Declaration(translationUnit()->tokenAt(ast->typeName->t_identifier).identifier,
                                                          ast->typeName->t_identifier, namedTy));
        } else if ( DataUserTypeAST *dataUserType = ast->userType->asDataUserType() ) {
            if ( DataTypeAST *dataType = dataUserType->dataType ) {
                _type = 0;
                accept(dataType);
                if ( _type )
                    _scope->appendTypeDeclaration(new Declaration(translationUnit()->tokenAt(ast->typeName->t_identifier).identifier,
                                                                  ast->typeName->t_identifier, _type));
                ast->typeName->type = _type;
            }
        }
    }

    return false;
}

bool Bind::visit(VarDeclarationItemAST *ast)
{
    proceedVarDeclarations(ast->variables, ast->type);
    return false;
}

bool Bind::visit(RoutineParameterAST *ast)
{
    proceedVarDeclarations(ast->vars, ast->type);
    return false;
}

bool Bind::visit(ByteStructureArrayTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingByte);
    return true;
}

bool Bind::visit(ShortStructureArrayTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingShort);
    return true;
}

bool Bind::visit(CamSetupDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingCamSetup);
    return true;
}

bool Bind::visit(ConfigDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingConfig);
    return true;
}

bool Bind::visit(FileDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingFile);
    return true;
}

bool Bind::visit(ModelDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingModel);
    return true;
}

bool Bind::visit(PathDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingPath);
    return true;
}

bool Bind::visit(SimpleDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingInteger);
    return true;
}

bool Bind::visit(StringDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingString);
    return true;
}

bool Bind::visit(UserDataTypeAST *ast)
{
    if ( ast->typeName ) {
        const Identifier *id = translationUnit()->tokenAt(ast->typeName->t_identifier).identifier;
        NamedType *namedTy = _control->namedType(id);
        unsigned sourceLocation;
        if ( const Type *bindTy = _scope->lookupNamedType(id, sourceLocation) ) {
            namedTy->bind(bindTy);
            ast->typeName->type = bindTy;
        }
        _type = namedTy;
    }

    return false;
}

bool Bind::visit(VectorDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingVector);
    return true;
}

bool Bind::visit(VisProcessDataTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingVisProcess);
    return true;
}

bool Bind::visit(JointposPositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingJointPos);
    return true;
}

bool Bind::visit(PositionPositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingPosition);
    return true;
}

bool Bind::visit(XYZWPRPositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingXYZWPR);
    return true;
}

bool Bind::visit(XYZWPREXTPositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingXYZWPREXT);
    return true;
}

bool Bind::visit(Jointpos1PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos2PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos3PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos4PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos5PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos6PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos7PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos8PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(Jointpos9PositionTypeAST *)
{
    _type = _control->builtingType(Type::BuiltingReal);
    return true;
}

bool Bind::visit(ArrayDataTypeAST *ast)
{
    _type = 0;

    if ( ast->itemType ) {
        accept(ast->itemType);
        if ( _type )
            _type = _control->arrayType(_type);
    }

    return false;
}

bool Bind::visit(FakeVariableStatementAST *)
{
    return false;
}

bool Bind::visit(UsingStatementAST *ast)
{
    if ( ast->structVars ) {
        Scope *usingScope = _control->scope(ast->t_using, Scope::StatementsArea, _scope);
        if ( ast->t_endusing )
            usingScope->setEndSourceLocation(ast->t_endusing);
        _scope->appendScope(usingScope);
        _scope = usingScope;
        ast->scope = _scope;
        for (VariableAccessListAST *it = ast->structVars; it; it = it->next) {
            if ( VariableAccessAST *var = it->value ) {
                _usingType = 0;
                accept(var);
                if ( _usingType && _usingType->hasFields() )
                    _scope->appendUsingType(_usingType);
            }
        }
        accept(ast->statements);
        _scope = _scope->parentScope();
    }

    return false;
}

bool Bind::visit(VariableAccessAST *ast)
{
    accept(ast->id);
    if ( _usingType )
        accept(ast->field);
    return false;
}

bool Bind::visit(PrimaryIdAST *ast)
{
    const Token &tk = tokenAt(ast->variable->t_identifier);

    if ( _usingType ) {
        if ( const Declaration *decl = _usingType->fieldDeclaration(tk.identifier) )
            _usingType = decl->type();
        else
            _usingType = 0;
    } else {
        if ( const Declaration *decl = _scope->lookupVariable(tk.identifier) )
            _usingType = decl->type();
        else
            _usingType = _scope->parentScope()->lookupUsingFieldType(tk.identifier);
    }

    return false;
}

bool Bind::visit(ArrayIdAST *ast)
{
    accept(ast->id);
    if ( _usingType )
        _usingType = _usingType->arrayItemtype();
    return false;
}

void Bind::proceedVarDeclarations(VariableNameListAST *variables, DataTypeAST *typeAst)
{
    if ( variables && typeAst ) {
        QList<QPair<const Identifier *, unsigned> > varIdents;
        for (VariableNameListAST *iter = variables; iter; iter = iter->next) {
            if ( iter->value )
                varIdents.append(qMakePair(translationUnit()->tokenAt(iter->value->t_identifier).identifier, iter->value->t_identifier));
        }
        if ( !varIdents.isEmpty() ) {
            _type = 0;
            accept(typeAst);
            if ( _type ) {
                QList<QPair<const Identifier *, unsigned> >::const_iterator iter = varIdents.constBegin();
                while ( iter != varIdents.constEnd() ) {
                    _scope->appendVarDeclaration(new Declaration(iter->first, iter->second, _type));
                    ++iter;
                }
            }
        }
    }
}

}   // namespace Karel
