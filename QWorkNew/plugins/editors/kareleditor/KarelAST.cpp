#include "KarelAST.h"
#include "KarelASTVisitor.h"

namespace Karel {

AST::AST()
{ }

AST::~AST()
{ }

void AST::accept( ASTVisitor *visitor )
{
    if ( visitor->preVisit(this) )
        accept0(visitor);
    visitor->postVisit(this);
}

unsigned ModuleAST::firstToken() const
{
    if ( t_program )
        return t_program;
    if ( programName )
        return programName->firstToken();
    if ( directives )
        return directives->firstToken();
    if ( declarations )
        return declarations->firstToken();
    if ( innerRoutines )
        return innerRoutines->firstToken();
    if ( t_begin )
        return t_begin;
    if ( statements )
        return statements->firstToken();
    if ( t_end )
        return t_end;
    if ( programName1 )
        return programName1->firstToken();
    if ( outerRoutines )
        return outerRoutines->firstToken();
    return 0;
}

unsigned ModuleAST::lastToken() const
{
    if ( outerRoutines )
        return outerRoutines->lastToken();
    if ( programName1 )
        return programName1->lastToken();
    if ( t_end )
        return t_end + 1;
    if ( statements )
        return statements->lastToken();
    if ( t_begin )
        return t_begin + 1;
    if ( innerRoutines )
        return innerRoutines->lastToken();
    if ( declarations )
        return declarations->lastToken();
    if ( directives )
        return directives->lastToken();
    if ( programName )
        return programName->lastToken();
    if ( t_program )
        return t_program + 1;
    return 1;
}

void ModuleAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(programName, visitor);
        accept(directives, visitor);
        accept(declarations, visitor);
        accept(innerRoutines, visitor);
        accept(statements, visitor);
        accept(programName1, visitor);
        accept(outerRoutines, visitor);
    }
    visitor->endVisit(this);
}

unsigned IntegerLiteralAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

unsigned IntegerLiteralAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

void IntegerLiteralAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned IntegerLiteralOrStarAST::firstToken() const
{
    if ( t_value_or_star )
        return t_value_or_star;
    return 0;
}

unsigned IntegerLiteralOrStarAST::lastToken() const
{
    if ( t_value_or_star )
        return t_value_or_star + 1;
    return 1;
}

void IntegerLiteralOrStarAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ConstantArraySizeAST::firstToken() const
{
    if ( constantName )
        return constantName->firstToken();
    return 0;
}

unsigned ConstantArraySizeAST::lastToken() const
{
    if ( constantName )
        return constantName->lastToken();
    return 1;
}

void ConstantArraySizeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constantName, visitor);
    }
    visitor->endVisit(this);
}

unsigned StringLiteralAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

unsigned StringLiteralAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

void StringLiteralAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned BooleanLiteralAST::firstToken() const
{
    if ( t_value )
        return t_value;
    return 0;
}

unsigned BooleanLiteralAST::lastToken() const
{
    if ( t_value )
        return t_value + 1;
    return 1;
}

void BooleanLiteralAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ProgramNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned ProgramNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void ProgramNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned RoutineNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned RoutineNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void RoutineNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned EnvironmentNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned EnvironmentNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void EnvironmentNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ConstantNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned ConstantNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void ConstantNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned VariableNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned VariableNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void VariableNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned FieldNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned FieldNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void FieldNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned TypeNameAST::firstToken() const
{
    if ( t_identifier )
        return t_identifier;
    return 0;
}

unsigned TypeNameAST::lastToken() const
{
    if ( t_identifier )
        return t_identifier + 1;
    return 1;
}

void TypeNameAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned AlphabetizeDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned AlphabetizeDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void AlphabetizeDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned CommentDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( t_comment )
        return t_comment;
    return 0;
}

unsigned CommentDirectiveAST::lastToken() const
{
    if ( t_comment )
        return t_comment + 1;
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void CommentDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned CmosVarsDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned CmosVarsDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void CmosVarsDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned CrtDeviceDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned CrtDeviceDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void CrtDeviceDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned DefGroupDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( t_group )
        return t_group;
    return 0;
}

unsigned DefGroupDirectiveAST::lastToken() const
{
    if ( t_group )
        return t_group + 1;
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void DefGroupDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned DelayDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( t_delay )
        return t_delay;
    return 0;
}

unsigned DelayDirectiveAST::lastToken() const
{
    if ( t_delay )
        return t_delay + 1;
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void DelayDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned InveronmentDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( environmentName )
        return environmentName->firstToken();
    return 0;
}

unsigned InveronmentDirectiveAST::lastToken() const
{
    if ( environmentName )
        return environmentName->lastToken();
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void InveronmentDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(environmentName, visitor);
    }
    visitor->endVisit(this);
}

unsigned IncludeDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( programName )
        return programName->firstToken();
    return 0;
}

unsigned IncludeDirectiveAST::lastToken() const
{
    if ( programName )
        return programName->lastToken();
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void IncludeDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(programName, visitor);
    }
    visitor->endVisit(this);
}

unsigned LockGroupDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( lockGroups )
        return lockGroups->firstToken();
    return 0;
}

unsigned LockGroupDirectiveAST::lastToken() const
{
    if ( lockGroups )
        return lockGroups->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void LockGroupDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(lockGroups, visitor);
    }
    visitor->endVisit(this);
}

unsigned NoAbortDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( t_d1 )
        return t_d1;
    if ( t_plus )
        return t_plus;
    if ( t_d2 )
        return t_d2;
    return 0;
}

unsigned NoAbortDirectiveAST::lastToken() const
{
    if ( t_d2 )
        return t_d2 + 1;
    if ( t_plus )
        return t_plus + 1;
    if ( t_d1 )
        return t_d1 + 1;
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void NoAbortDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned NoPauseDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( t_d1 )
        return t_d1;
    if ( t_plus )
        return t_plus;
    if ( t_d2 )
        return t_d2;
    if ( t_plus1 )
        return t_plus1;
    if ( t_d3 )
        return t_d3;
    return 0;
}

unsigned NoPauseDirectiveAST::lastToken() const
{
    if ( t_d3 )
        return t_d3 + 1;
    if ( t_plus1 )
        return t_plus1 + 1;
    if ( t_d2 )
        return t_d2 + 1;
    if ( t_plus )
        return t_plus + 1;
    if ( t_d1 )
        return t_d1 + 1;
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void NoPauseDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned NoBusyLampDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned NoBusyLampDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void NoBusyLampDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned NoLockGroupDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned NoLockGroupDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void NoLockGroupDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned NoPauseShiftDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned NoPauseShiftDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void NoPauseShiftDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned PriorityDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( value )
        return value->firstToken();
    return 0;
}

unsigned PriorityDirectiveAST::lastToken() const
{
    if ( value )
        return value->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void PriorityDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

unsigned StackSizeDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( value )
        return value->firstToken();
    return 0;
}

unsigned StackSizeDirectiveAST::lastToken() const
{
    if ( value )
        return value->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void StackSizeDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

unsigned TimeSliceDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    if ( t_assign )
        return t_assign;
    if ( value )
        return value->firstToken();
    return 0;
}

unsigned TimeSliceDirectiveAST::lastToken() const
{
    if ( value )
        return value->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void TimeSliceDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

unsigned TpMotionDirectiveAST::firstToken() const
{
    if ( t_keyword )
        return t_keyword;
    return 0;
}

unsigned TpMotionDirectiveAST::lastToken() const
{
    if ( t_keyword )
        return t_keyword + 1;
    return 1;
}

void TpMotionDirectiveAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ConstDeclarationAST::firstToken() const
{
    if ( t_const )
        return t_const;
    if ( declarations )
        return declarations->firstToken();
    return 0;
}

unsigned ConstDeclarationAST::lastToken() const
{
    if ( declarations )
        return declarations->lastToken();
    if ( t_const )
        return t_const + 1;
    return 1;
}

void ConstDeclarationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(declarations, visitor);
    }
    visitor->endVisit(this);
}

unsigned TypeDeclarationAST::firstToken() const
{
    if ( t_type )
        return t_type;
    if ( declarations )
        return declarations->firstToken();
    return 0;
}

unsigned TypeDeclarationAST::lastToken() const
{
    if ( declarations )
        return declarations->lastToken();
    if ( t_type )
        return t_type + 1;
    return 1;
}

void TypeDeclarationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(declarations, visitor);
    }
    visitor->endVisit(this);
}

unsigned VarDeclarationAST::firstToken() const
{
    if ( t_var )
        return t_var;
    if ( declarations )
        return declarations->firstToken();
    return 0;
}

unsigned VarDeclarationAST::lastToken() const
{
    if ( declarations )
        return declarations->lastToken();
    if ( t_var )
        return t_var + 1;
    return 1;
}

void VarDeclarationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(declarations, visitor);
    }
    visitor->endVisit(this);
}

unsigned ConstDeclarationItemAST::firstToken() const
{
    if ( constantName )
        return constantName->firstToken();
    if ( t_assign )
        return t_assign;
    if ( value )
        return value->firstToken();
    return 0;
}

unsigned ConstDeclarationItemAST::lastToken() const
{
    if ( value )
        return value->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( constantName )
        return constantName->lastToken();
    return 1;
}

void ConstDeclarationItemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(constantName, visitor);
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

unsigned LiteralConstantValueAST::firstToken() const
{
    if ( value )
        return value->firstToken();
    return 0;
}

unsigned LiteralConstantValueAST::lastToken() const
{
    if ( value )
        return value->lastToken();
    return 1;
}

void LiteralConstantValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

unsigned ConstantNameConstantValueAST::firstToken() const
{
    if ( value )
        return value->firstToken();
    return 0;
}

unsigned ConstantNameConstantValueAST::lastToken() const
{
    if ( value )
        return value->lastToken();
    return 1;
}

void ConstantNameConstantValueAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(value, visitor);
    }
    visitor->endVisit(this);
}

unsigned TypeDeclarationItemAST::firstToken() const
{
    if ( typeName )
        return typeName->firstToken();
    if ( fromProgram )
        return fromProgram->firstToken();
    if ( t_assign )
        return t_assign;
    if ( userType )
        return userType->firstToken();
    return 0;
}

unsigned TypeDeclarationItemAST::lastToken() const
{
    if ( userType )
        return userType->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( fromProgram )
        return fromProgram->lastToken();
    if ( typeName )
        return typeName->lastToken();
    return 1;
}

void TypeDeclarationItemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
        accept(fromProgram, visitor);
        accept(userType, visitor);
    }
    visitor->endVisit(this);
}

unsigned VarDeclarationItemAST::firstToken() const
{
    if ( variables )
        return variables->firstToken();
    if ( t_cmos_or_dram )
        return t_cmos_or_dram;
    if ( fromProgram )
        return fromProgram->firstToken();
    if ( t_colon )
        return t_colon;
    if ( type )
        return type->firstToken();
    return 0;
}

unsigned VarDeclarationItemAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    if ( t_colon )
        return t_colon + 1;
    if ( fromProgram )
        return fromProgram->lastToken();
    if ( t_cmos_or_dram )
        return t_cmos_or_dram + 1;
    if ( variables )
        return variables->lastToken();
    return 1;
}

void VarDeclarationItemAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(variables, visitor);
        accept(fromProgram, visitor);
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

unsigned FromProgramClauseAST::firstToken() const
{
    if ( t_from )
        return t_from;
    if ( programName )
        return programName->firstToken();
    return 0;
}

unsigned FromProgramClauseAST::lastToken() const
{
    if ( programName )
        return programName->lastToken();
    if ( t_from )
        return t_from + 1;
    return 1;
}

void FromProgramClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(programName, visitor);
    }
    visitor->endVisit(this);
}

unsigned DataUserTypeAST::firstToken() const
{
    if ( dataType )
        return dataType->firstToken();
    return 0;
}

unsigned DataUserTypeAST::lastToken() const
{
    if ( dataType )
        return dataType->lastToken();
    return 1;
}

void DataUserTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(dataType, visitor);
    }
    visitor->endVisit(this);
}

unsigned StructureUserTypeAST::firstToken() const
{
    if ( t_structure )
        return t_structure;
    if ( fields )
        return fields->firstToken();
    if ( t_endstructure )
        return t_endstructure;
    return 0;
}

unsigned StructureUserTypeAST::lastToken() const
{
    if ( t_endstructure )
        return t_endstructure + 1;
    if ( fields )
        return fields->lastToken();
    if ( t_structure )
        return t_structure + 1;
    return 1;
}

void StructureUserTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fields, visitor);
    }
    visitor->endVisit(this);
}

unsigned FieldDefinitionAST::firstToken() const
{
    if ( fieldName )
        return fieldName->firstToken();
    if ( t_colon )
        return t_colon;
    if ( type )
        return type->firstToken();
    return 0;
}

unsigned FieldDefinitionAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    if ( t_colon )
        return t_colon + 1;
    if ( fieldName )
        return fieldName->lastToken();
    return 1;
}

void FieldDefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(fieldName, visitor);
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

unsigned DataStructureArrayTypeAST::firstToken() const
{
    if ( type )
        return type->firstToken();
    return 0;
}

unsigned DataStructureArrayTypeAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    return 1;
}

void DataStructureArrayTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

unsigned ByteStructureArrayTypeAST::firstToken() const
{
    if ( t_byte )
        return t_byte;
    return 0;
}

unsigned ByteStructureArrayTypeAST::lastToken() const
{
    if ( t_byte )
        return t_byte + 1;
    return 1;
}

void ByteStructureArrayTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ShortStructureArrayTypeAST::firstToken() const
{
    if ( t_short )
        return t_short;
    return 0;
}

unsigned ShortStructureArrayTypeAST::lastToken() const
{
    if ( t_short )
        return t_short + 1;
    return 1;
}

void ShortStructureArrayTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned UserDataTypeAST::firstToken() const
{
    if ( typeName )
        return typeName->firstToken();
    return 0;
}

unsigned UserDataTypeAST::lastToken() const
{
    if ( typeName )
        return typeName->lastToken();
    return 1;
}

void UserDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
    }
    visitor->endVisit(this);
}

unsigned SimpleDataTypeAST::firstToken() const
{
    if ( t_int_real_bool )
        return t_int_real_bool;
    return 0;
}

unsigned SimpleDataTypeAST::lastToken() const
{
    if ( t_int_real_bool )
        return t_int_real_bool + 1;
    return 1;
}

void SimpleDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned VectorDataTypeAST::firstToken() const
{
    if ( t_vector )
        return t_vector;
    return 0;
}

unsigned VectorDataTypeAST::lastToken() const
{
    if ( t_vector )
        return t_vector + 1;
    return 1;
}

void VectorDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned VisProcessDataTypeAST::firstToken() const
{
    if ( t_vis_process )
        return t_vis_process;
    return 0;
}

unsigned VisProcessDataTypeAST::lastToken() const
{
    if ( t_vis_process )
        return t_vis_process + 1;
    return 1;
}

void VisProcessDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ModelDataTypeAST::firstToken() const
{
    if ( t_model )
        return t_model;
    return 0;
}

unsigned ModelDataTypeAST::lastToken() const
{
    if ( t_model )
        return t_model + 1;
    return 1;
}

void ModelDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned CamSetupDataTypeAST::firstToken() const
{
    if ( t_can_setup )
        return t_can_setup;
    return 0;
}

unsigned CamSetupDataTypeAST::lastToken() const
{
    if ( t_can_setup )
        return t_can_setup + 1;
    return 1;
}

void CamSetupDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned FileDataTypeAST::firstToken() const
{
    if ( t_file )
        return t_file;
    return 0;
}

unsigned FileDataTypeAST::lastToken() const
{
    if ( t_file )
        return t_file + 1;
    return 1;
}

void FileDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned ConfigDataTypeAST::firstToken() const
{
    if ( t_config )
        return t_config;
    return 0;
}

unsigned ConfigDataTypeAST::lastToken() const
{
    if ( t_config )
        return t_config + 1;
    return 1;
}

void ConfigDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned PathDataTypeAST::firstToken() const
{
    if ( t_path )
        return t_path;
    if ( pathHeader )
        return pathHeader->firstToken();
    if ( nodeData )
        return nodeData->firstToken();
    return 0;
}

unsigned PathDataTypeAST::lastToken() const
{
    if ( nodeData )
        return nodeData->lastToken();
    if ( pathHeader )
        return pathHeader->lastToken();
    if ( t_path )
        return t_path + 1;
    return 1;
}

void PathDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(pathHeader, visitor);
        accept(nodeData, visitor);
    }
    visitor->endVisit(this);
}

unsigned PathHeaderClauseAST::firstToken() const
{
    if ( t_path )
        return t_path;
    if ( t_header )
        return t_header;
    if ( t_assign )
        return t_assign;
    if ( typeName )
        return typeName->firstToken();
    return 0;
}

unsigned PathHeaderClauseAST::lastToken() const
{
    if ( typeName )
        return typeName->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( t_header )
        return t_header + 1;
    if ( t_path )
        return t_path + 1;
    return 1;
}

void PathHeaderClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
    }
    visitor->endVisit(this);
}

unsigned NodeDataClauseAST::firstToken() const
{
    if ( t_nodedata )
        return t_nodedata;
    if ( t_assign )
        return t_assign;
    if ( typeName )
        return typeName->firstToken();
    return 0;
}

unsigned NodeDataClauseAST::lastToken() const
{
    if ( typeName )
        return typeName->lastToken();
    if ( t_assign )
        return t_assign + 1;
    if ( t_nodedata )
        return t_nodedata + 1;
    return 1;
}

void NodeDataClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(typeName, visitor);
    }
    visitor->endVisit(this);
}

unsigned StringDataTypeAST::firstToken() const
{
    if ( t_string )
        return t_string;
    if ( t_lbracket )
        return t_lbracket;
    if ( stringSize )
        return stringSize->firstToken();
    if ( t_rbracket )
        return t_rbracket;
    return 0;
}

unsigned StringDataTypeAST::lastToken() const
{
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( stringSize )
        return stringSize->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    if ( t_string )
        return t_string + 1;
    return 1;
}

void StringDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(stringSize, visitor);
    }
    visitor->endVisit(this);
}

unsigned ArrayDataTypeAST::firstToken() const
{
    if ( t_array )
        return t_array;
    if ( t_lbracket )
        return t_lbracket;
    if ( sizes )
        return sizes->firstToken();
    if ( t_rbracket )
        return t_rbracket;
    if ( t_of )
        return t_of;
    if ( itemType )
        return itemType->firstToken();
    return 0;
}

unsigned ArrayDataTypeAST::lastToken() const
{
    if ( itemType )
        return itemType->lastToken();
    if ( t_of )
        return t_of + 1;
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( sizes )
        return sizes->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    if ( t_array )
        return t_array + 1;
    return 1;
}

void ArrayDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(sizes, visitor);
        accept(itemType, visitor);
    }
    visitor->endVisit(this);
}

unsigned PositionDataTypeAST::firstToken() const
{
    if ( positionType )
        return positionType->firstToken();
    if ( inGroupClause )
        return inGroupClause->firstToken();
    return 0;
}

unsigned PositionDataTypeAST::lastToken() const
{
    if ( inGroupClause )
        return inGroupClause->lastToken();
    if ( positionType )
        return positionType->lastToken();
    return 1;
}

void PositionDataTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(positionType, visitor);
        accept(inGroupClause, visitor);
    }
    visitor->endVisit(this);
}

unsigned InGroupClauseAST::firstToken() const
{
    if ( t_in )
        return t_in;
    if ( t_group )
        return t_group;
    if ( t_lbracket )
        return t_lbracket;
    if ( groupNum )
        return groupNum->firstToken();
    if ( t_rbracket )
        return t_rbracket;
    return 0;
}

unsigned InGroupClauseAST::lastToken() const
{
    if ( t_rbracket )
        return t_rbracket + 1;
    if ( groupNum )
        return groupNum->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    if ( t_group )
        return t_group + 1;
    if ( t_in )
        return t_in + 1;
    return 1;
}

void InGroupClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(groupNum, visitor);
    }
    visitor->endVisit(this);
}

unsigned PositionPositionTypeAST::firstToken() const
{
    if ( t_position )
        return t_position;
    return 0;
}

unsigned PositionPositionTypeAST::lastToken() const
{
    if ( t_position )
        return t_position + 1;
    return 1;
}

void PositionPositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned XYZWPRPositionTypeAST::firstToken() const
{
    if ( t_xyzwpr )
        return t_xyzwpr;
    return 0;
}

unsigned XYZWPRPositionTypeAST::lastToken() const
{
    if ( t_xyzwpr )
        return t_xyzwpr + 1;
    return 1;
}

void XYZWPRPositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned XYZWPREXTPositionTypeAST::firstToken() const
{
    if ( t_xyzwprext )
        return t_xyzwprext;
    return 0;
}

unsigned XYZWPREXTPositionTypeAST::lastToken() const
{
    if ( t_xyzwprext )
        return t_xyzwprext + 1;
    return 1;
}

void XYZWPREXTPositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned JointposPositionTypeAST::firstToken() const
{
    if ( t_jointpos )
        return t_jointpos;
    return 0;
}

unsigned JointposPositionTypeAST::lastToken() const
{
    if ( t_jointpos )
        return t_jointpos + 1;
    return 1;
}

void JointposPositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos1PositionTypeAST::firstToken() const
{
    if ( t_jointpos1 )
        return t_jointpos1;
    return 0;
}

unsigned Jointpos1PositionTypeAST::lastToken() const
{
    if ( t_jointpos1 )
        return t_jointpos1 + 1;
    return 1;
}

void Jointpos1PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos2PositionTypeAST::firstToken() const
{
    if ( t_jointpos2 )
        return t_jointpos2;
    return 0;
}

unsigned Jointpos2PositionTypeAST::lastToken() const
{
    if ( t_jointpos2 )
        return t_jointpos2 + 1;
    return 1;
}

void Jointpos2PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos3PositionTypeAST::firstToken() const
{
    if ( t_jointpos3 )
        return t_jointpos3;
    return 0;
}

unsigned Jointpos3PositionTypeAST::lastToken() const
{
    if ( t_jointpos3 )
        return t_jointpos3 + 1;
    return 1;
}

void Jointpos3PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos4PositionTypeAST::firstToken() const
{
    if ( t_jointpos4 )
        return t_jointpos4;
    return 0;
}

unsigned Jointpos4PositionTypeAST::lastToken() const
{
    if ( t_jointpos4 )
        return t_jointpos4 + 1;
    return 1;
}

void Jointpos4PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos5PositionTypeAST::firstToken() const
{
    if ( t_jointpos5 )
        return t_jointpos5;
    return 0;
}

unsigned Jointpos5PositionTypeAST::lastToken() const
{
    if ( t_jointpos5 )
        return t_jointpos5 + 1;
    return 1;
}

void Jointpos5PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos6PositionTypeAST::firstToken() const
{
    if ( t_jointpos6 )
        return t_jointpos6;
    return 0;
}

unsigned Jointpos6PositionTypeAST::lastToken() const
{
    if ( t_jointpos6 )
        return t_jointpos6 + 1;
    return 1;
}

void Jointpos6PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos7PositionTypeAST::firstToken() const
{
    if ( t_jointpos7 )
        return t_jointpos7;
    return 0;
}

unsigned Jointpos7PositionTypeAST::lastToken() const
{
    if ( t_jointpos7 )
        return t_jointpos7 + 1;
    return 1;
}

void Jointpos7PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos8PositionTypeAST::firstToken() const
{
    if ( t_jointpos8 )
        return t_jointpos8;
    return 0;
}

unsigned Jointpos8PositionTypeAST::lastToken() const
{
    if ( t_jointpos8 )
        return t_jointpos8 + 1;
    return 1;
}

void Jointpos8PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned Jointpos9PositionTypeAST::firstToken() const
{
    if ( t_jointpos9 )
        return t_jointpos9;
    return 0;
}

unsigned Jointpos9PositionTypeAST::lastToken() const
{
    if ( t_jointpos9 )
        return t_jointpos9 + 1;
    return 1;
}

void Jointpos9PositionTypeAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
    }
    visitor->endVisit(this);
}

unsigned RoutineDeclarationAST::firstToken() const
{
    if ( definition )
        return definition->firstToken();
    if ( implementation )
        return implementation->firstToken();
    return 0;
}

unsigned RoutineDeclarationAST::lastToken() const
{
    if ( implementation )
        return implementation->lastToken();
    if ( definition )
        return definition->lastToken();
    return 1;
}

void RoutineDeclarationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(definition, visitor);
        accept(implementation, visitor);
    }
    visitor->endVisit(this);
}

unsigned FromPrgRoutineImplemenationAST::firstToken() const
{
    if ( t_from )
        return t_from;
    if ( programName )
        return programName->firstToken();
    return 0;
}

unsigned FromPrgRoutineImplemenationAST::lastToken() const
{
    if ( programName )
        return programName->lastToken();
    if ( t_from )
        return t_from + 1;
    return 1;
}

void FromPrgRoutineImplemenationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(programName, visitor);
    }
    visitor->endVisit(this);
}

unsigned DirectRoutineImplemenationAST::firstToken() const
{
    if ( declarations )
        return declarations->firstToken();
    if ( t_begin )
        return t_begin;
    if ( statements )
        return statements->firstToken();
    if ( t_end )
        return t_end;
    if ( routineName )
        return routineName->firstToken();
    return 0;
}

unsigned DirectRoutineImplemenationAST::lastToken() const
{
    if ( routineName )
        return routineName->lastToken();
    if ( t_end )
        return t_end + 1;
    if ( statements )
        return statements->lastToken();
    if ( t_begin )
        return t_begin + 1;
    if ( declarations )
        return declarations->lastToken();
    return 1;
}

void DirectRoutineImplemenationAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(declarations, visitor);
        accept(statements, visitor);
        accept(routineName, visitor);
    }
    visitor->endVisit(this);
}

unsigned RoutineDefinitionAST::firstToken() const
{
    if ( t_routine )
        return t_routine;
    if ( routineName )
        return routineName->firstToken();
    if ( parametersClause )
        return parametersClause->firstToken();
    if ( returnType )
        return returnType->firstToken();
    return 0;
}

unsigned RoutineDefinitionAST::lastToken() const
{
    if ( returnType )
        return returnType->lastToken();
    if ( parametersClause )
        return parametersClause->lastToken();
    if ( routineName )
        return routineName->lastToken();
    if ( t_routine )
        return t_routine + 1;
    return 1;
}

void RoutineDefinitionAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(routineName, visitor);
        accept(parametersClause, visitor);
        accept(returnType, visitor);
    }
    visitor->endVisit(this);
}

unsigned RoutineParametersClauseAST::firstToken() const
{
    if ( t_lparen )
        return t_lparen;
    if ( parameters )
        return parameters->firstToken();
    if ( t_rparen )
        return t_rparen;
    return 0;
}

unsigned RoutineParametersClauseAST::lastToken() const
{
    if ( t_rparen )
        return t_rparen + 1;
    if ( parameters )
        return parameters->lastToken();
    if ( t_lparen )
        return t_lparen + 1;
    return 1;
}

void RoutineParametersClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(parameters, visitor);
    }
    visitor->endVisit(this);
}

unsigned RoutineReturnTypeClauseAST::firstToken() const
{
    if ( t_colon )
        return t_colon;
    if ( returnType )
        return returnType->firstToken();
    return 0;
}

unsigned RoutineReturnTypeClauseAST::lastToken() const
{
    if ( returnType )
        return returnType->lastToken();
    if ( t_colon )
        return t_colon + 1;
    return 1;
}

void RoutineReturnTypeClauseAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(returnType, visitor);
    }
    visitor->endVisit(this);
}

unsigned RoutineParameterAST::firstToken() const
{
    if ( vars )
        return vars->firstToken();
    if ( t_colon )
        return t_colon;
    if ( type )
        return type->firstToken();
    return 0;
}

unsigned RoutineParameterAST::lastToken() const
{
    if ( type )
        return type->lastToken();
    if ( t_colon )
        return t_colon + 1;
    if ( vars )
        return vars->lastToken();
    return 1;
}

void RoutineParameterAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(vars, visitor);
        accept(type, visitor);
    }
    visitor->endVisit(this);
}

unsigned PrimaryIdAST::firstToken() const
{
    if ( variable )
        return variable->firstToken();
    return 0;
}

unsigned PrimaryIdAST::lastToken() const
{
    if ( variable )
        return variable->lastToken();
    return 1;
}

void PrimaryIdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(variable, visitor);
    }
    visitor->endVisit(this);
}

unsigned ArrayIdAST::firstToken() const
{
    if ( id )
        return id->firstToken();
    if ( t_lbracket )
        return t_lbracket;
    if ( vars )
        return vars->firstToken();
    if ( t_rbaracket )
        return t_rbaracket;
    return 0;
}

unsigned ArrayIdAST::lastToken() const
{
    if ( t_rbaracket )
        return t_rbaracket + 1;
    if ( vars )
        return vars->lastToken();
    if ( t_lbracket )
        return t_lbracket + 1;
    if ( id )
        return id->lastToken();
    return 1;
}

void ArrayIdAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(id, visitor);
        accept(vars, visitor);
    }
    visitor->endVisit(this);
}

unsigned VariableFieldAST::firstToken() const
{
    if ( t_dot )
        return t_dot;
    if ( field )
        return field->firstToken();
    return 0;
}

unsigned VariableFieldAST::lastToken() const
{
    if ( field )
        return field->lastToken();
    if ( t_dot )
        return t_dot + 1;
    return 1;
}

void VariableFieldAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(field, visitor);
    }
    visitor->endVisit(this);
}

unsigned VariableAccessAST::firstToken() const
{
    if ( id )
        return id->firstToken();
    if ( field )
        return field->firstToken();
    return 0;
}

unsigned VariableAccessAST::lastToken() const
{
    if ( field )
        return field->lastToken();
    if ( id )
        return id->lastToken();
    return 1;
}

void VariableAccessAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(id, visitor);
        accept(field, visitor);
    }
    visitor->endVisit(this);
}

unsigned FakeVariableStatementAST::firstToken() const
{
    if ( var )
        return var->firstToken();
    return 0;
}

unsigned FakeVariableStatementAST::lastToken() const
{
    if ( var )
        return var->lastToken();
    return 1;
}

void FakeVariableStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(var, visitor);
    }
    visitor->endVisit(this);
}

unsigned UsingStatementAST::firstToken() const
{
    if ( t_using )
        return t_using;
    if ( structVars )
        return structVars->firstToken();
    if ( t_do )
        return t_do;
    if ( statements )
        return statements->firstToken();
    if ( t_endusing )
        return t_endusing;
    return 0;
}

unsigned UsingStatementAST::lastToken() const
{
    if ( t_endusing )
        return t_endusing + 1;
    if ( statements )
        return statements->lastToken();
    if ( t_do )
        return t_do + 1;
    if ( structVars )
        return structVars->lastToken();
    if ( t_using )
        return t_using + 1;
    return 1;
}

void UsingStatementAST::accept0( ASTVisitor *visitor )
{
    if ( visitor->visit(this) ) {
        accept(structVars, visitor);
        accept(statements, visitor);
    }
    visitor->endVisit(this);
}

}   // namespace Karel
