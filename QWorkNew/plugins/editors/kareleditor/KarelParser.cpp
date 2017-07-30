#include <QDebug>

#include "KarelParser.h"
#include "KarelKeywords.h"
#include "KarelAST.h"

#ifdef _MSC_VER
#    define va_copy(dst, src) ((dst) = (src))
#elif defined(__INTEL_COMPILER) && !defined(va_copy)
#    define va_copy __va_copy
#endif

namespace Karel {

Parser::Parser(TranslationUnit *translationUnit)
    : _translationUnit(translationUnit)
    , _control(_translationUnit->control())
    , _pool(_translationUnit->memoryPool())
    , _tokenIndex(1)
{ }

Parser::~Parser()
{ }

bool Parser::parseModule(ModuleAST *&node)
{
    unsigned token;
    if ( !matchKeyword(T_PROGRAM, &token) )
        return false;

    ModuleAST *ast = new (_pool) ModuleAST(token);

    if ( LA() == T_IDENTIFIER ) {
        const Token &tk = tok();
        if ( tk.f.newline ) {
            warning(_tokenIndex, "Wrong place for program identifier");
        }
        ast->programName = new (_pool) ProgramNameAST;
        ast->programName->t_identifier = consumeToken();
        QString programNameStr = tk.identifier->chars();

        if ( parseDirectiveList(ast->directives) ) {
            if ( parseModuleDeclarationList(ast->declarations) ) {
                if ( parseRoutineList(ast->innerRoutines) ) {
                    if ( KI() == T_BEGIN ) {
                        ast->t_begin = consumeToken();
                        if ( parseStatementList(ast->statements) ) {
                            if ( KI() == T_END ) {
                                ast->t_end = consumeToken();
                                if ( LA() == T_IDENTIFIER ) {
                                    if ( programNameStr.compare(tok().spell(), Qt::CaseInsensitive) )
                                        warning(_tokenIndex, "Illegal program name specified");
                                    ProgramNameAST *endProgramName = new (_pool) ProgramNameAST;
                                    endProgramName->t_identifier = consumeToken();
                                    ast->programName1 = endProgramName;
                                    if ( parseRoutineList(ast->outerRoutines) ) {
                                        /// EOF should be here
                                    }
                                } else {
                                    error(_tokenIndex, "Expected program name identifier token");
                                }
                            } else {
                                error(_tokenIndex, "Expected token `END'");
                            }
                        }
                    } else {
                        error(_tokenIndex, "Expected token `BEGIN'");
                    }
                }
            }
        }
    } else {
        error(_tokenIndex, "Expected program name identifier");
    }

    node = ast;
    return true;
}

bool Parser::parseDirectiveList(DirectiveListAST *&directives)
{
    if ( !checkNewLine() )
        return false;

    bool isDirectivesArea = true;
    DirectiveListAST **directives_ptr = &directives;

    while ( isDirectivesArea ) {
        bool ok = true;

        const Token &tk = tok();

        if ( tk.is(T_EOF_SYMBOL) )
            return false;

        isDirectivesArea = false;

        if ( tk.isKeyword() ) {
            isDirectivesArea = true;

            switch ( tk.keyid() ) {

            case T__ALPHABETIZE: {
                AlphabetizeDirectiveAST *directive = new (_pool) AlphabetizeDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__COMMENT: {
                CommentDirectiveAST *directive = new (_pool) CommentDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_STRING_LITERAL ) {
                        directive->t_comment = consumeToken();
                    } else  {
                        ok = false;
                        error(_tokenIndex, "Expected comment literal token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__CMOSVARS: {
                CmosVarsDirectiveAST *directive = new (_pool) CmosVarsDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__CRTDEVICE: {
                CrtDeviceDirectiveAST *directive = new (_pool) CrtDeviceDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__DEFGROUP: {
                DefGroupDirectiveAST *directive = new (_pool) DefGroupDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_NUMERIC_LITERAL ) {
                        directive->t_group = consumeToken();
                    } else {
                        ok = false;
                        error(_tokenIndex, "Expected group value integer token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__DELAY: {
                DelayDirectiveAST *directive = new (_pool) DelayDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_NUMERIC_LITERAL ) {
                        directive->t_delay = consumeToken();
                    } else {
                        ok = false;
                        error(_tokenIndex, "Expected delay value integer token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__ENVIRONMENT: {
                InveronmentDirectiveAST *directive = new (_pool) InveronmentDirectiveAST(consumeToken());
                if ( LA() == T_IDENTIFIER ) {
                    directive->environmentName = new (_pool) EnvironmentNameAST;
                    directive->environmentName->t_identifier = consumeToken();
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected environment identifier token");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__INCLUDE: {
                IncludeDirectiveAST *directive = new (_pool) IncludeDirectiveAST(consumeToken());
                if ( LA() == T_IDENTIFIER ) {
                    directive->programName = new (_pool) ProgramNameAST;
                    directive->programName->t_identifier = consumeToken();
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected program name identifier token");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__LOCKGROUP: {
                LockGroupDirectiveAST *directive = new (_pool) LockGroupDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( !parseIntegerList(directive->lockGroups) )
                        return false;
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                break;

            }

            case T__NOABORT: {
                NoAbortDirectiveAST *directive = new (_pool) NoAbortDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_IDENTIFIER && LA(2) == T_PLUS && LA(3) == T_IDENTIFIER ) {
                        directive->t_d1 = consumeToken();
                        directive->t_plus = consumeToken();
                        directive->t_d2 = consumeToken();
                    } else if ( LA() == T_IDENTIFIER ) {
                        directive->t_d1 = consumeToken();
                    } else {
                        ok = false;
                        error(_tokenIndex, "Invalid NOABORT directive syntax");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__NOPAUSE: {
                NoPauseDirectiveAST *directive = new (_pool) NoPauseDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_IDENTIFIER && LA(2) == T_PLUS && LA(3) == T_IDENTIFIER && LA(4) == T_PLUS && LA(5) == T_IDENTIFIER ) {
                        directive->t_d1 = consumeToken();
                        directive->t_plus = consumeToken();
                        directive->t_d2 = consumeToken();
                        directive->t_plus1 = consumeToken();
                        directive->t_d3 = consumeToken();
                    } else if ( LA() == T_IDENTIFIER && LA(2) == T_PLUS && LA(3) == T_IDENTIFIER ) {
                        directive->t_d1 = consumeToken();
                        directive->t_plus = consumeToken();
                        directive->t_d2 = consumeToken();
                    } else if ( LA() == T_IDENTIFIER ) {
                        directive->t_d1 = consumeToken();
                    } else {
                        ok = false;
                        error(_tokenIndex, "Invalid NOABORT directive syntax");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__NOBUSYLAMP: {
                NoBusyLampDirectiveAST *directive = new (_pool) NoBusyLampDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__NOLOCKGROUP: {
                NoLockGroupDirectiveAST *directive = new (_pool) NoLockGroupDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__NOPAUSESHFT: {
                NoPauseShiftDirectiveAST *directive = new (_pool) NoPauseShiftDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__PRIORITY: {
                PriorityDirectiveAST *directive = new (_pool) PriorityDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_NUMERIC_LITERAL ) {
                        directive->value = new (_pool) IntegerLiteralAST(consumeToken());
                    } else {
                        ok = false;
                        error(_tokenIndex, "Expected priority integer value token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__STACKSIZE: {
                StackSizeDirectiveAST *directive = new (_pool) StackSizeDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_NUMERIC_LITERAL ) {
                        directive->value = new (_pool) IntegerLiteralAST(consumeToken());
                    } else {
                        ok = false;
                        error(_tokenIndex, "Expected stack size integer value token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__TIMESLICE: {
                TimeSliceDirectiveAST *directive = new (_pool) TimeSliceDirectiveAST(consumeToken());
                if ( LA() == T_EQUAL ) {
                    directive->t_assign = consumeToken();
                    if ( LA() == T_NUMERIC_LITERAL ) {
                        directive->value = new (_pool) IntegerLiteralAST(consumeToken());
                    } else {
                        ok = false;
                        error(_tokenIndex, "Expected time slice integer value token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `='");
                }
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            case T__TPMOTION: {
                TpMotionDirectiveAST *directive = new (_pool) TpMotionDirectiveAST(consumeToken());
                *directives_ptr = new (_pool) DirectiveListAST(directive);
                directives_ptr = &(*directives_ptr)->next;
                break;
            }

            default:
                isDirectivesArea = false;
            }

        }

        if ( !skipToNewLine(ok) )
            return false;
    }

    return true;
}

bool Parser::parseIntegerList(IntegerLiteralListAST *&integers)
{
    int count = 0;
    IntegerLiteralListAST **integers_ptr = &integers;

    while (true) {
        const Token &tk = tok();

        if ( tk.f.newline )
            return true;

        switch (tk.f.kind) {
            case T_EOF_SYMBOL:
                return false;
            case T_SEMICOLON:
                consumeToken();
                return true;
            case T_COMMA:
                if ( LA(0) != T_NUMERIC_LITERAL )
                    warning(_tokenIndex, "Invalid sequence of integer tokens");
                break;
            case T_NUMERIC_LITERAL:
                if ( count && LA(0) != T_COMMA )
                    warning(_tokenIndex, "Invalid sequence of integer tokens");
                *integers_ptr = new (_pool) IntegerLiteralListAST(new (_pool) IntegerLiteralAST(_tokenIndex));
                integers_ptr = &(*integers_ptr)->next;
                count++;
                break;
            default:
                warning(_tokenIndex, "Invalid sequence of integer tokens");
                skipToNewLine(false);
                return true;
        }

        consumeToken();
    }

    return true;
}

bool Parser::parseArraySizeList(ArraySizeListAST *&sizes)
{
    int count = 0;
    ArraySizeListAST **sizes_ptr = &sizes;

    while (true) {
        const Token &tk = tok();

        switch (tk.f.kind) {
            case T_EOF_SYMBOL:
                return false;
            case T_RBRACKET:
                if ( !count )
                    warning(_tokenIndex, "No array sizes specified");
                return true;
            case T_COMMA:
                if ( LA(0) != T_NUMERIC_LITERAL && LA(0) != T_IDENTIFIER )
                    warning(_tokenIndex, "Invalid sequence of integer or integer constant tokens");
                break;
            case T_NUMERIC_LITERAL:
                if ( count && LA(0) != T_COMMA )
                    warning(_tokenIndex, "Invalid sequence of integer or integer constant tokens");
                *sizes_ptr = new (_pool) ArraySizeListAST(new (_pool) IntegerLiteralOrStarAST(_tokenIndex));
                sizes_ptr = &(*sizes_ptr)->next;
                count++;
                break;
            case T_IDENTIFIER: {
                if ( count && LA(0) != T_COMMA )
                    warning(_tokenIndex, "Invalid sequence of integer or integer constant tokens");
                ConstantNameAST *constantName = new (_pool) ConstantNameAST;
                constantName->t_identifier = _tokenIndex;
                *sizes_ptr = new (_pool) ArraySizeListAST(new (_pool) ConstantArraySizeAST(constantName));
                sizes_ptr = &(*sizes_ptr)->next;
                count++;
                break;
            }
            default:
                warning(_tokenIndex, "Invalid sequence of integer or integer constant tokens");
                skipToNewLineOrPrecKind(false, T_RBRACKET);
                return true;
        }

        consumeToken();
    }
}

bool Parser::parseParameterArraySizeList(ArraySizeListAST *&sizes)
{
    int count = 0;
    ArraySizeListAST **sizes_ptr = &sizes;

    while (true) {
        const Token &tk = tok();

        switch (tk.f.kind) {
            case T_EOF_SYMBOL:
                return false;
            case T_RBRACKET:
                if ( !count )
                    warning(_tokenIndex, "Empty parameter array specifiers");
                return true;
            case T_COMMA:
                if ( LA(0) != T_STAR )
                    warning(_tokenIndex, "Invalid sequence of parameter array specifiers");
                break;
            case T_STAR:
                if ( count && LA(0) != T_COMMA )
                    warning(_tokenIndex, "Invalid sequence of parameter array specifiers");
                *sizes_ptr = new (_pool) ArraySizeListAST(new (_pool) IntegerLiteralOrStarAST(_tokenIndex));
                sizes_ptr = &(*sizes_ptr)->next;
                count++;
                break;
            default:
                warning(_tokenIndex, "Invalid sequence of parameter array specifiers");
                skipToNewLineOrPrecKind(false, T_RBRACKET);
                return true;
        }

        consumeToken();
    }
}

bool Parser::parseModuleDeclarationList(VarTypeConstDeclarationListAST *&declarations)
{
    bool declarationsParsed = false;
    VarTypeConstDeclarationListAST **declarations_ptr = &declarations;

    while ( !declarationsParsed ) {
        switch ( KI() ) {

            case T_CONST: {
                ConstDeclarationAST *constDeclaration = new (_pool) ConstDeclarationAST(consumeToken());
                *declarations_ptr = new (_pool) VarTypeConstDeclarationListAST(constDeclaration);
                declarations_ptr = &(*declarations_ptr)->next;
                if ( !parseConstDeclarationItemList(constDeclaration->declarations) )
                    return false;
                break;
            }

            case T_TYPE: {
                TypeDeclarationAST *typeDeclaration = new (_pool) TypeDeclarationAST(consumeToken());
                *declarations_ptr = new (_pool) VarTypeConstDeclarationListAST(typeDeclaration);
                declarations_ptr = &(*declarations_ptr)->next;
                if ( !parseTypeDeclarationItemList(typeDeclaration->declarations) )
                    return false;
                break;
            }

            case T_VAR: {
                VarDeclarationAST *varDeclaration = new (_pool) VarDeclarationAST(consumeToken());
                *declarations_ptr = new (_pool) VarTypeConstDeclarationListAST(varDeclaration);
                declarations_ptr = &(*declarations_ptr)->next;
                if ( !parseVarDeclarationItemList(varDeclaration->declarations) )
                    return false;
                break;
            }

            default:
                declarationsParsed = true;
        }
    }
    return true;
}

bool Parser::parseRoutineList(RoutineDeclarationListAST *&routines)
{
    RoutineDeclarationListAST **routines_ptr = &routines;

    while (true) {
        if ( KI() == T_ROUTINE ) {
            RoutineDeclarationAST *routine = 0;
            if ( !parseRoutine(routine) )
                return false;
            if ( routine ) {
                *routines_ptr = new (_pool) RoutineDeclarationListAST(routine);
                routines_ptr = &(*routines_ptr)->next;
            }
        } else {
            break;
        }
    }

    return true;
}

bool Parser::parseRoutine(RoutineDeclarationAST *&routine)
{
    bool ok = true;
    RoutineDefinitionAST *routineDefinition = new (_pool) RoutineDefinitionAST(consumeToken());
    routine = new (_pool) RoutineDeclarationAST(routineDefinition);

    if ( LA() == T_IDENTIFIER ) {
        QString routineNameStr = tok().identifier->chars();
        RoutineNameAST *routineName = new (_pool) RoutineNameAST;
        routineName->t_identifier = consumeToken();
        routineDefinition->routineName = routineName;
        if ( LA() == T_LPAREN ) {
            if ( !parseRoutinesParameters(routineDefinition->parametersClause) )
                return false;
        }
        if ( LA() == T_COLON ) {
            routineDefinition->returnType = new (_pool) RoutineReturnTypeClauseAST(consumeToken());
            if ( !parseDataType(routineDefinition->returnType->returnType, true) )
                return false;
        }
        if ( KI() == T_FROM && LA(2) == T_IDENTIFIER ) {
            FromPrgRoutineImplemenationAST *fromProgram = new (_pool) FromPrgRoutineImplemenationAST;
            fromProgram->t_from = consumeToken();
            fromProgram->programName = new (_pool) ProgramNameAST;
            fromProgram->programName->t_identifier = consumeToken();
            routine->implementation = fromProgram;
        } else {
            DirectRoutineImplemenationAST *directRoutineImplementation = new (_pool) DirectRoutineImplemenationAST;
            routine->implementation = directRoutineImplementation;
            if ( !parseModuleDeclarationList(directRoutineImplementation->declarations) )
                return false;
            if ( KI() == T_BEGIN ) {
                directRoutineImplementation->t_begin = consumeToken();
                if ( !parseStatementList(directRoutineImplementation->statements) )
                    return false;
                if ( KI() == T_END ) {
                    directRoutineImplementation->t_end = consumeToken();
                    if ( LA() == T_IDENTIFIER ) {
                        if ( routineNameStr.compare(tok().spell(), Qt::CaseInsensitive) )
                            warning(_tokenIndex, "Illegal routine name specified");
                        RoutineNameAST *endRoutineName = new (_pool) RoutineNameAST;
                        endRoutineName->t_identifier = consumeToken();
                        directRoutineImplementation->routineName = endRoutineName;
                    } else {
                        ok = false;
                        error(_tokenIndex, "Expected routine name identifier token");
                    }
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `end'");
                }
            } else {
                ok = false;
                error(_tokenIndex, "Expected token `begin'");
            }
        }
    } else {
        ok = false;
        error(_tokenIndex, "Expected routine name identifier");
    }

    skipToNewLine(ok);

    return true;
}

bool Parser::parseRoutinesParameters(RoutineParametersClauseAST *&paramsClause)
{
    paramsClause = new (_pool) RoutineParametersClauseAST(consumeToken());

    if ( !parseRoutineParameterList(paramsClause->parameters) )
        return false;

    if ( LA() == T_RPAREN ) {
        paramsClause->t_rparen = consumeToken();
    } else {
        error(_tokenIndex, "Expected token `)'");
    }

    return true;
}

bool Parser::parseConstDeclarationItemList(ConstDeclarationItemListAST *&constants)
{
    ConstDeclarationItemListAST **constants_ptr = &constants;

    unsigned oldIndex = _tokenIndex;

    while ( true ) {
        if ( LA() == T_EOF_SYMBOL )
            return false;

        bool ok = true;

        switch ( KI() ) {
            case T_ROUTINE:
            case T_BEGIN:
            case T_VAR:
            case T_CONST:
            case T_TYPE:
                return true;
        }

        if ( LA() == T_IDENTIFIER ) {
            ConstantNameAST *constantName = new (_pool) ConstantNameAST;
            constantName->t_identifier = consumeToken();
            ConstDeclarationItemAST *constant = new (_pool) ConstDeclarationItemAST(constantName);
            *constants_ptr = new (_pool) ConstDeclarationItemListAST(constant);
            constants_ptr = &(*constants_ptr)->next;
            if ( LA() == T_EQUAL ) {
                constant->t_assign = consumeToken();
                switch ( LA() ) {
                    case T_STRING_LITERAL:
                        constant->value = new (_pool) LiteralConstantValueAST(new (_pool) StringLiteralAST(consumeToken()));
                        break;
                    case T_NUMERIC_LITERAL:
                        constant->value = new (_pool) LiteralConstantValueAST(new (_pool) IntegerLiteralAST(consumeToken()));
                        break;
                    case T_IDENTIFIER: {
                        ConstantNameAST *constantValue = new (_pool) ConstantNameAST;
                        constantValue->t_identifier = consumeToken();
                        constant->value = new (_pool) ConstantNameConstantValueAST(constantValue);
                        break;
                    }
                    default:
                        ok = false;
                        error(_tokenIndex, "Expected constant value token");
                        break;
                }
            } else {
                ok = false;
                error(_tokenIndex, "Expected token `='");
            }
        } else {
            ok = false;
            error(_tokenIndex, "Unexpected token `%s' (waiting constant identifier)", tok().spell());
        }

        skipToNewLine(ok);
        if ( oldIndex == _tokenIndex ) {
            // Something wrong
            consumeToken();
            skipToNewLine(false);
        }

        oldIndex = _tokenIndex;
    }

    return true;
}

bool Parser::parseTypeDeclarationItemList(TypeDeclarationItemListAST *&types)
{
    TypeDeclarationItemListAST **types_ptr = &types;

    unsigned oldIndex = _tokenIndex;

    while ( true ) {
        if ( LA() == T_EOF_SYMBOL )
            return false;

        bool ok = true;

        switch ( KI() ) {
            case T_ROUTINE:
            case T_BEGIN:
            case T_VAR:
            case T_CONST:
            case T_TYPE:
                return true;
        }

        if ( LA() == T_IDENTIFIER ) {
            TypeDeclarationItemAST *declaration = new (_pool) TypeDeclarationItemAST(new (_pool) TypeNameAST(consumeToken()));
            *types_ptr = new (_pool) TypeDeclarationItemListAST(declaration);
            types_ptr = &(*types_ptr)->next;
            if ( KI() == T_FROM && KI(2) == T_PROGRAM ) {
                FromProgramClauseAST *fromProgram = new (_pool) FromProgramClauseAST;
                fromProgram->t_from = consumeToken();
                fromProgram->programName = new (_pool) ProgramNameAST;
                fromProgram->programName->t_identifier = consumeToken();
            }
            if  ( LA() == T_EQUAL ) {
                declaration->t_assign = consumeToken();
                if ( !parseUserType(declaration->userType) )
                    return false;
            } else {
                ok = false;
                error(_tokenIndex, "Expected token `='");
            }
        } else {
            ok = false;
            error(_tokenIndex, "Unexpected token `%s' (waiting type name identifier)", tok().spell());
        }

        skipToNewLine(ok);
        if ( oldIndex == _tokenIndex ) {
            // Something wrong
            consumeToken();
            skipToNewLine(false);
        }

        oldIndex = _tokenIndex;
    }

    return true;
}

bool Parser::parseUserType(UserTypeAST *&userType)
{
    if ( KI() == T_STRUCTURE ) {
        StructureUserTypeAST *structureType = new (_pool) StructureUserTypeAST(consumeToken());
        userType = structureType;

        FieldDefinitionListAST **fields_ptr = &structureType->fields;

        unsigned oldIndex = _tokenIndex;

        while ( true ) {
            bool ok = true;

            if ( LA() == T_EOF_SYMBOL )
                return false;

            if ( KI() == T_ENDSTRUCTURE ) {
                structureType->t_endstructure = consumeToken();
                break;
            }

            if ( LA() == T_IDENTIFIER ) {
                FieldNameAST *fieldName = new (_pool) FieldNameAST;
                fieldName->t_identifier = consumeToken();
                FieldDefinitionAST *field = new (_pool) FieldDefinitionAST(fieldName);
                *fields_ptr = new (_pool) FieldDefinitionListAST(field);
                fields_ptr = &(*fields_ptr)->next;
                if ( LA() == T_COLON ) {
                    field->t_colon = consumeToken();
                    if ( !parseStructureArrayType(field->type, false) )
                        return false;
                } else {
                    ok = false;
                    error(_tokenIndex, "Expected token `:'");
                }
            } else {
                ok = false;
                error(_tokenIndex, "Expected structure field name identifier token");
            }

            skipToNewLine(ok);
            if ( oldIndex == _tokenIndex ) {
                // Something wrong
                consumeToken();
                skipToNewLine(false);
            }

            oldIndex = _tokenIndex;
        }

        return true;
    }

    DataUserTypeAST *dataType = new (_pool) DataUserTypeAST;
    userType = dataType;
    return parseDataType(dataType->dataType);
}

bool Parser::parseDataType(DataTypeAST *&dataType, bool isParamType, bool isArrayAllowed)
{
    if ( LA() == T_EOF_SYMBOL )
        return false;

    switch ( KI() ) {
        case T_INTEGER:
        case T_REAL:
        case T_BOOLEAN:
            dataType = new (_pool) SimpleDataTypeAST(consumeToken());
            return true;
        case T_VECTOR:
            dataType = new (_pool) VectorDataTypeAST(consumeToken());
            return true;
        case T_VIS_PROCESS:
            dataType = new (_pool) VisProcessDataTypeAST(consumeToken());
            return true;
        case T_MODEL:
            dataType = new (_pool) ModelDataTypeAST(consumeToken());
            return true;
        case T_CAM_SETUP:
            dataType = new (_pool) CamSetupDataTypeAST(consumeToken());
            return true;
        case T_FILE:
            dataType = new (_pool) FileDataTypeAST(consumeToken());
            return true;
        case T_CONFIG:
            dataType = new (_pool) ConfigDataTypeAST(consumeToken());
            return true;
        case T_PATH: {
            PathDataTypeAST *pathDataType = new (_pool) PathDataTypeAST(consumeToken());
            dataType = pathDataType;
            QString headerStr = "header";
            QString nodedataStr = "nodedata";
            if ( KI() == T_PATH && LA(2) == T_IDENTIFIER && !headerStr.compare(tok(2).identifier->chars(), Qt::CaseInsensitive) &&
                 LA(3) == T_EQUAL && LA(4) == T_IDENTIFIER ) {
                PathHeaderClauseAST *pathHeader = new (_pool) PathHeaderClauseAST;
                pathHeader->t_path = consumeToken();
                pathHeader->t_header = consumeToken();
                pathHeader->t_assign = consumeToken();
                pathHeader->typeName = new (_pool) TypeNameAST(consumeToken());
                pathDataType->pathHeader = pathHeader;
                if ( LA() == T_COMMA )
                    consumeToken();
            }
            if ( LA() == T_IDENTIFIER && !nodedataStr.compare(tok().identifier->chars(), Qt::CaseInsensitive) &&
                 LA(2) == T_EQUAL && LA(3) == T_IDENTIFIER ) {
                NodeDataClauseAST *nodeData = new (_pool) NodeDataClauseAST;
                nodeData->t_nodedata = consumeToken();
                nodeData->typeName = new (_pool) TypeNameAST(consumeToken());
                pathDataType->nodeData = nodeData;
            }
            return true;
        }
        case T_STRING: {
            StringDataTypeAST *stringType = new (_pool) StringDataTypeAST(consumeToken());
            dataType = stringType;
            if ( LA() == T_LBRACKET ) {
                stringType->t_lbracket = consumeToken();
                if ( isParamType ) {
                    if ( LA() == T_STAR ) {
                        stringType->stringSize = new (_pool) IntegerLiteralOrStarAST(consumeToken());
                    } else {
                        error(_tokenIndex, "Expected token `*'");
                    }
                } else {
                    if ( LA() == T_IDENTIFIER ) {
                        ConstantNameAST *sizeName = new (_pool) ConstantNameAST;
                        sizeName->t_identifier = consumeToken();
                        stringType->stringSize = new (_pool) ConstantArraySizeAST(sizeName);
                    } else if ( LA() == T_NUMERIC_LITERAL ) {
                        stringType->stringSize = new (_pool) IntegerLiteralOrStarAST(consumeToken());
                    } else {
                        error(_tokenIndex, "Expected size value (integer or incteger constant) token");
                    }
                }
                if ( LA() == T_RBRACKET ) {
                    stringType->t_rbracket = consumeToken();
                } else {
                    error(_tokenIndex, "Expected token `]'");
                }
            }
            return true;
        }
        case T_ARRAY: {
            if ( !isArrayAllowed ) {
                error(_tokenIndex, "Unexpected token `ARRAY'");
                return true;
            }
            ArrayDataTypeAST *arrayType = new (_pool) ArrayDataTypeAST(consumeToken());
            dataType = arrayType;
            if ( LA() != T_LBRACKET ) {
                error(_tokenIndex, "Expected token `['");
                return true;
            }
            arrayType->t_lbracket = consumeToken();
            if ( isParamType ) {
                if ( !parseParameterArraySizeList(arrayType->sizes) )
                    return false;
            } else {
                if ( !parseArraySizeList(arrayType->sizes) )
                    return false;
            }
            if ( LA() != T_RBRACKET ) {
                error(_tokenIndex, "Expected token `]'");
                return true;
            }
            arrayType->t_rbracket = consumeToken();
            if ( KI() != T_OF ) {
                error(_tokenIndex, "Expected token `OF'");
                return true;
            }
            arrayType->t_of = consumeToken();
            return parseStructureArrayType(arrayType->itemType, isParamType, false);
        }
        case T_POSITION: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) PositionPositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_XYZWPR: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) XYZWPRPositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS1: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos1PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS2: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos2PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS3: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos3PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS4: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos4PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS5: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos5PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS6: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos6PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS7: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos7PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS8: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos8PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS9: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) Jointpos9PositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_JOINTPOS: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) JointposPositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
        case T_XYZWPREXT: {
            PositionDataTypeAST *posDataType = new (_pool) PositionDataTypeAST(new (_pool) XYZWPREXTPositionTypeAST(consumeToken()));
            dataType = posDataType;
            parseInGroupClause(posDataType->inGroupClause);
            return true;
        }
    }

    if ( LA() == T_IDENTIFIER ) {
        dataType = new (_pool) UserDataTypeAST(new (_pool) TypeNameAST(consumeToken()));
        return true;
    }

    error(_tokenIndex, "Unexpected token `%s'", tok().spell());
    if ( isParamType )
        skipToNewLineOrPrecKind(false, T_RPAREN);
    else
        skipToNewLine(false);
    return true;
}

bool Parser::parseStructureArrayType(StructureArrayTypeAST *&type, bool isParamType, bool isArrayAllowed)
{
    switch ( KI() ) {
        case T_SHORT:
            type = new (_pool) ShortStructureArrayTypeAST(consumeToken());
            return true;
        case T_BYTE:
            type = new (_pool) ByteStructureArrayTypeAST(consumeToken());
            return true;
    }

    DataStructureArrayTypeAST *dataType = new (_pool) DataStructureArrayTypeAST;
    type = dataType;
    return parseDataType(dataType->type, isParamType, isArrayAllowed);
}

bool Parser::parseInGroupClause(InGroupClauseAST *&inGroup)
{
    QString groupStr = "group";
    if ( KI() == T_IN && LA(2) == T_IDENTIFIER && !groupStr.compare(tok(2).identifier->chars(), Qt::CaseInsensitive) ) {
        inGroup = new (_pool) InGroupClauseAST;
        inGroup->t_in = consumeToken();
        inGroup->t_group = consumeToken();
        if ( LA() == T_LBRACKET ) {
            inGroup->t_lbracket = consumeToken();
            if ( LA() == T_NUMERIC_LITERAL ) {
                inGroup->groupNum = new (_pool) IntegerLiteralOrStarAST(consumeToken());
            } else if ( LA() == T_IDENTIFIER ) {
                ConstantNameAST *constantName = new (_pool) ConstantNameAST;
                constantName->t_identifier = consumeToken();
                inGroup->groupNum = new (_pool) ConstantArraySizeAST(constantName);
            }
            if ( LA() == T_RBRACKET ) {
                inGroup->t_rbracket = consumeToken();
            } else {
                error(_tokenIndex, "Expected token `]'");
            }
        } else {
            error(_tokenIndex, "Expected token `['");
        }
    }

    return true;
}

bool Parser::parseVarDeclarationItemList(VarDeclarationItemListAST *&variables)
{
    VarDeclarationItemListAST **variables_ptr = &variables;
    unsigned oldIndex = _tokenIndex;

    while ( true ) {
        if ( LA() == T_EOF_SYMBOL )
            return false;

        bool ok = true;

        switch ( KI() ) {
            case T_ROUTINE:
            case T_BEGIN:
            case T_VAR:
            case T_CONST:
            case T_TYPE:
                return true;
        }

        if ( LA() == T_IDENTIFIER || KI() == T_STATUS ) {
            VarDeclarationItemAST *variable = new (_pool) VarDeclarationItemAST;
            *variables_ptr = new (_pool) VarDeclarationItemListAST(variable);
            variables_ptr = &(*variables_ptr)->next;
            if ( !parseVariableNameList(variable->variables) )
                return false;
            if ( KI() == T_CMOS || KI() == T_DRAM )
                variable->t_cmos_or_dram = consumeToken();
            if ( KI() == T_FROM && LA(2) == T_IDENTIFIER ) {
                FromProgramClauseAST *fromProgram = new (_pool) FromProgramClauseAST;
                fromProgram->t_from = consumeToken();
                fromProgram->programName = new (_pool) ProgramNameAST;
                fromProgram->programName->t_identifier = consumeToken();
            }
            if ( LA() == T_COLON ) {
                variable->t_colon = consumeToken();
                if ( !parseDataType(variable->type) )
                    return false;
            } else {
                ok = false;
                error(_tokenIndex, "Expected token `:'");
            }
        } else {
            ok = false;
            error(_tokenIndex, "Expected variable identifier token");
        }

        skipToNewLine(ok);
        if ( oldIndex == _tokenIndex ) {
            // Something wrong
            consumeToken();
            skipToNewLine(false);
        }

        oldIndex = _tokenIndex;
    }

    return true;
}

bool Parser::parseRoutineParameterList(RoutineParameterListAST *&parameters)
{
    RoutineParameterListAST **parameters_ptr = &parameters;
    unsigned oldIndex = _tokenIndex;

    while ( true ) {
        switch (LA()) {
            case T_EOF_SYMBOL:
                return false;
            case T_RPAREN:
                return true;
        }

        bool ok = true;

        if ( LA() == T_IDENTIFIER || KI() == T_STATUS ) {
            RoutineParameterAST *parameter = new (_pool) RoutineParameterAST;
            *parameters_ptr = new (_pool) RoutineParameterListAST(parameter);
            parameters_ptr = &(*parameters_ptr)->next;
            if ( !parseVariableNameList(parameter->vars) )
                return false;
            if ( LA() == T_COLON ) {
                parameter->t_colon = consumeToken();
                if ( !parseDataType(parameter->type, true) )
                    return false;
                if ( LA() == T_RPAREN )
                    return true;
            } else {
                ok = false;
                error(_tokenIndex, "Expected token `:'");
            }
        } else {
            ok = false;
            error(_tokenIndex, "Expected variable identifier token");
        }

        skipToNewLine(ok);
        if ( oldIndex == _tokenIndex ) {
            // Something wrong
            consumeToken();
            skipToNewLine(false);
        }

        oldIndex = _tokenIndex;
    }

    return true;
}

bool Parser::parseVariableNameList(VariableNameListAST *&variables)
{
    int count = 0;
    VariableNameListAST **variables_ptr = &variables;

    while ( true ) {
        if ( LA() == T_IDENTIFIER || KI() == T_STATUS ) {
            if ( count && LA(0) != T_COMMA )
                warning(_tokenIndex, "Invalid sequence of varaible names");
            VariableNameAST *variableName = new (_pool) VariableNameAST;
            variableName->t_identifier = consumeToken();
            *variables_ptr = new (_pool) VariableNameListAST(variableName);
            variables_ptr = &(*variables_ptr)->next;
            count++;
        } else if ( LA() == T_COMMA ) {
            if ( !count || (LA(0) != T_IDENTIFIER && KI(0) != T_STATUS) )
                warning(_tokenIndex, "Invalid sequence of varaible names");
            consumeToken();
        } else {
            break;
        }
    }

    return true;
}

bool Parser::parseStatementList(StatementListAST *&statements, unsigned usingDepth)
{
    StatementListAST **statements_ptr = &statements;

    while (true) {
        switch ( KI() ) {
            case T_END:
                return true;
            case T_ENDUSING:
                if ( usingDepth > 0 ) {
                    usingDepth--;
                    return true;
                }
                consumeToken();
                break;
            case T_USING: {
                UsingStatementAST *usingAst = 0;
                if ( !parseUsingStatement(usingAst, usingDepth) )
                    return false;
                if ( usingAst ) {
                    *statements_ptr = new (_pool) StatementListAST(usingAst);
                    statements_ptr = &(*statements_ptr)->next;
                } else {
                    consumeToken();
                }
                break;
            }

            default:
                switch ( LA() ) {
                    case T_EOF_SYMBOL:
                        return false;
                    case T_IDENTIFIER: {
                        FakeVariableStatementAST *varAst = new (_pool) FakeVariableStatementAST();
                        parseVariableAccess(varAst->var);
                        *statements_ptr = new (_pool) StatementListAST(varAst);
                        statements_ptr = &(*statements_ptr)->next;
                        break;
                    }
                    default:
                        consumeToken();
                }
        }
    }

    return true;
}

bool Parser::parseUsingStatement(UsingStatementAST *&usingStatement, unsigned usingDepth)
{
    UsingStatementAST *result = new (_pool) UsingStatementAST(consumeToken());
    VariableAccessListAST **vars_ptr = &result->structVars;

    while ( true ) {
        const Token &tk = tok();
        if ( tk.isKeyword() && tk.keywordType() == 2 && tk.kf.keyid != T_DIV && tk.kf.keyid != T_MOD )
            break;

        if ( tk.isIdentifier() ) {
            VariableAccessAST *var;
            parseVariableAccess(var);
            *vars_ptr = new (_pool) VariableAccessListAST(var);
            vars_ptr = &(*vars_ptr)->next;
            if ( KI() == T_DO )
                break;
            if ( LA() == T_COMMA ) {
                consumeToken();
                continue;
            }
        }

        error(_tokenIndex, "Unexpected token `%s'", tk.spell());
        consumeToken();
    }

    if ( KI() != T_DO )
        return true;
    result->t_do = consumeToken();
    usingStatement = result;

    if ( !parseStatementList(result->statements, usingDepth + 1) )
        return false;

    if ( KI() == T_ENDUSING ) {
        result->t_endusing = consumeToken();
    } else {
        error(_tokenIndex, "Expected token `ENDUSING'");
    }

    return true;
}

bool Parser::parseVariableField(VariableFieldAST *&field)
{
    field = new (_pool) VariableFieldAST(consumeToken());

    switch ( LA() ) {
        case T_EOF_SYMBOL:  return false;
        case T_IDENTIFIER:  return parseVariableAccess(field->field);
        default:
            error(_tokenIndex, "Expected structure field identifier");
    }

    return true;
}

bool Parser::parseVariableAccess(VariableAccessAST *&var)
{
    PrimaryIdAST *primaryAst = new (_pool) PrimaryIdAST(new (_pool) VariableNameAST(consumeToken()));

    switch ( LA() ) {
        case T_LBRACKET:
            var = new (_pool) VariableAccessAST(parseArrayAccess(primaryAst));
            if ( LA() == T_DOT )
                return parseVariableField(var->field);
            break;
        case T_DOT:
            var = new (_pool) VariableAccessAST(primaryAst);
            return parseVariableField(var->field);
        default:
            var = new (_pool) VariableAccessAST(primaryAst);
            break;
    }

    return true;
}

ArrayIdAST *Parser::parseArrayAccess(IdAST *lhs)
{
    ArrayIdAST *ast = new (_pool) ArrayIdAST(lhs);
    ast->t_lbracket = consumeToken();

    if ( !parseInBrackets(ast->vars) )
        return ast;

    if ( LA() == T_RBRACKET ) {
        ast->t_rbaracket = consumeToken();
        if ( LA() == T_LBRACKET ) {
            return parseArrayAccess(ast);
        }
    } else {
        error(_tokenIndex, "Expected token `]'");
    }

    return ast;
}

bool Parser::parseInBrackets(VariableAccessListAST *&vars)
{
    VariableAccessListAST **vars_ptr = &vars;

    while ( true ) {
        switch ( LA() ) {
            case T_EOF_SYMBOL:
                return false;
            case T_RBRACKET:
                return true;
            case T_IDENTIFIER: {
                VariableAccessAST *var;
                parseVariableAccess(var);
                *vars_ptr = new (_pool) VariableAccessListAST(var);
                vars_ptr = &(*vars_ptr)->next;
                break;
            }

            default: {
                const Token &tk = tok();
                if ( tk.isKeyword() && tk.keywordType() == 2 && tk.kf.keyid != T_DIV && tk.kf.keyid != T_MOD ) {
                    error(_tokenIndex, "Expected token `]'");
                    return true;
                }
                consumeToken();
                break;
            }
        }
    }

    return true;
}

bool Parser::matchKeyword(int keyid, unsigned *token)
{
    if ( KI() == keyid ) {
        *token = consumeToken();
        return true;
    }

    *token = 0;
    error(_tokenIndex, "expected token `%s' got `%s'", Keywords::name(keyid), tok().spell());
    return false;
}

bool Parser::checkNewLine()
{
    const Token &tk = tok();

    switch ( tk.f.kind ) {
        case T_EOF_SYMBOL:
            return false;
        case T_SEMICOLON:
            consumeToken();
            return true;
    }

    if ( !tk.f.newline )
        warning(_tokenIndex, "Expected new line or `;'");

    return true;
}

bool Parser::skipToNewLine(bool reportWarning)
{
    while (true) {
        const Token &tk = tok();

        switch ( tk.f.kind ) {
            case T_EOF_SYMBOL:
                return false;
            case T_SEMICOLON:
                consumeToken();
                return true;
        }

        if ( tk.f.newline )
            return true;

        if ( reportWarning )
            warning(_tokenIndex, "Expected new line or `;'");

        consumeToken();
    }

    return true;
}

bool Parser::skipToNewLineOrPrecKind(bool reportWarning, unsigned precKind)
{
    while (true) {
        const Token &tk = tok();

        switch ( tk.f.kind ) {
            case T_EOF_SYMBOL:
                return false;
            case T_SEMICOLON:
                consumeToken();
                return true;
        }

        if ( tk.f.newline || tk.f.kind == precKind )
            return true;

        if ( reportWarning )
            warning(_tokenIndex, "Expected new line or `;'");

        consumeToken();
    }

    return true;
}

void Parser::warning(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(DiagnosticClient::Warning, index, format, ap);
    va_end(ap);
    va_end(args);
}

void Parser::error(unsigned index, const char *format, ...)
{
    va_list args, ap;
    va_start(args, format);
    va_copy(ap, args);
    _translationUnit->message(DiagnosticClient::Error, index, format, ap);
    va_end(ap);
    va_end(args);
}

}   // namespace Karel
