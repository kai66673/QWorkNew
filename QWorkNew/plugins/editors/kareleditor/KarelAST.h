#ifndef KARELAST_H
#define KARELAST_H

#include "MemoryPool.h"
#include "KarelASTfwd.h"
#include "KarelType.h"
#include "Control.h"

namespace Karel {

template <typename _Tp>
class List: public Utils::Managed
{
    List(const List &other);
    void operator =(const List &other);

public:
    List()
        : value(_Tp()), next(0)
    { }

    List(const _Tp &value)
        : value(value), next(0)
    { }

    unsigned firstToken() const
    {
        if (value)
            return value->firstToken();

        // ### assert(0);
        return 0;
    }

    unsigned lastToken() const
    {
        _Tp lv = lastValue();

        if (lv)
            return lv->lastToken();

        // ### assert(0);
        return 0;
    }

    _Tp lastValue() const
    {
        _Tp lastValue = 0;

        for (const List *it = this; it; it = it->next) {
            if (it->value)
                lastValue = it->value;
        }

        return lastValue;
    }

    _Tp value;
    List *next;
};

class AST: public Utils::Managed
{
    AST(const AST &other);
    void operator =(const AST &other);

public:
    AST();
    virtual ~AST();

    void accept(ASTVisitor *visitor);

    static void accept(AST *ast, ASTVisitor *visitor)
    { if (ast) ast->accept(visitor); }

    template <typename _Tp>
    static void accept(List<_Tp> *it, ASTVisitor *visitor)
    {
        for (; it; it = it->next)
            accept(it->value, visitor);
    }

    virtual unsigned firstToken() const = 0;
    virtual unsigned lastToken() const = 0;

//    virtual AST *clone(Utils::MemoryPool *pool) const = 0;

    virtual ModuleAST *asModule() { return 0; }
    virtual LiteralAST *asLiteral() { return 0; }
    virtual IntegerLiteralAST *asIntegerLiteral() { return 0; }
    virtual ArraySizeAST *asArraySize() { return 0; }
    virtual IntegerLiteralOrStarAST *asIntegerLiteralOrStar() { return 0; }
    virtual ConstantArraySizeAST *asConstantArraySize() { return 0; }
    virtual StringLiteralAST *asStringLiteral() { return 0; }
    virtual BooleanLiteralAST *asBooleanLiteral() { return 0; }
    virtual NameAST *asName() { return 0; }
    virtual ProgramNameAST *asProgramName() { return 0; }
    virtual RoutineNameAST *asRoutineName() { return 0; }
    virtual EnvironmentNameAST *asEnvironmentName() { return 0; }
    virtual ConstantNameAST *asConstantName() { return 0; }
    virtual VariableNameAST *asVariableName() { return 0; }
    virtual FieldNameAST *asFieldName() { return 0; }
    virtual TypeNameAST *asTypeName() { return 0; }
    virtual DirectiveAST *asDirective() { return 0; }
    virtual AlphabetizeDirectiveAST *asAlphabetizeDirective() { return 0; }
    virtual CommentDirectiveAST *asCommentDirective() { return 0; }
    virtual CmosVarsDirectiveAST *asCmosVarsDirective() { return 0; }
    virtual CrtDeviceDirectiveAST *asCrtDeviceDirective() { return 0; }
    virtual DefGroupDirectiveAST *asDefGroupDirective() { return 0; }
    virtual DelayDirectiveAST *asDelayDirective() { return 0; }
    virtual InveronmentDirectiveAST *asInveronmentDirective() { return 0; }
    virtual IncludeDirectiveAST *asIncludeDirective() { return 0; }
    virtual LockGroupDirectiveAST *asLockGroupDirective() { return 0; }
    virtual NoAbortDirectiveAST *asNoAbortDirective() { return 0; }
    virtual NoPauseDirectiveAST *asNoPauseDirective() { return 0; }
    virtual NoBusyLampDirectiveAST *asNoBusyLampDirective() { return 0; }
    virtual NoLockGroupDirectiveAST *asNoLockGroupDirective() { return 0; }
    virtual NoPauseShiftDirectiveAST *asNoPauseShiftDirective() { return 0; }
    virtual PriorityDirectiveAST *asPriorityDirective() { return 0; }
    virtual StackSizeDirectiveAST *asStackSizeDirective() { return 0; }
    virtual TimeSliceDirectiveAST *asTimeSliceDirective() { return 0; }
    virtual TpMotionDirectiveAST *asTpMotionDirective() { return 0; }
    virtual VarTypeConstDeclarationAST *asVarTypeConstDeclaration() { return 0; }
    virtual ConstDeclarationAST *asConstDeclaration() { return 0; }
    virtual TypeDeclarationAST *asTypeDeclaration() { return 0; }
    virtual VarDeclarationAST *asVarDeclaration() { return 0; }
    virtual ConstDeclarationItemAST *asConstDeclarationItem() { return 0; }
    virtual ConstantValueAST *asConstantValue() { return 0; }
    virtual LiteralConstantValueAST *asLiteralConstantValue() { return 0; }
    virtual ConstantNameConstantValueAST *asConstantNameConstantValue() { return 0; }
    virtual TypeDeclarationItemAST *asTypeDeclarationItem() { return 0; }
    virtual VarDeclarationItemAST *asVarDeclarationItem() { return 0; }
    virtual FromProgramClauseAST *asFromProgramClause() { return 0; }
    virtual UserTypeAST *asUserType() { return 0; }
    virtual DataUserTypeAST *asDataUserType() { return 0; }
    virtual StructureUserTypeAST *asStructureUserType() { return 0; }
    virtual FieldDefinitionAST *asFieldDefinition() { return 0; }
    virtual StructureArrayTypeAST *asStructureArrayType() { return 0; }
    virtual DataStructureArrayTypeAST *asDataStructureArrayType() { return 0; }
    virtual ByteStructureArrayTypeAST *asByteStructureArrayType() { return 0; }
    virtual ShortStructureArrayTypeAST *asShortStructureArrayType() { return 0; }
    virtual DataTypeAST *asDataType() { return 0; }
    virtual UserDataTypeAST *asUserDataType() { return 0; }
    virtual SimpleDataTypeAST *asSimpleDataType() { return 0; }
    virtual VectorDataTypeAST *asVectorDataType() { return 0; }
    virtual VisProcessDataTypeAST *asVisProcessDataType() { return 0; }
    virtual ModelDataTypeAST *asModelDataType() { return 0; }
    virtual CamSetupDataTypeAST *asCamSetupDataType() { return 0; }
    virtual FileDataTypeAST *asFileDataType() { return 0; }
    virtual ConfigDataTypeAST *asConfigDataType() { return 0; }
    virtual PathDataTypeAST *asPathDataType() { return 0; }
    virtual PathHeaderClauseAST *asPathHeaderClause() { return 0; }
    virtual NodeDataClauseAST *asNodeDataClause() { return 0; }
    virtual StringDataTypeAST *asStringDataType() { return 0; }
    virtual ArrayDataTypeAST *asArrayDataType() { return 0; }
    virtual PositionDataTypeAST *asPositionDataType() { return 0; }
    virtual InGroupClauseAST *asInGroupClause() { return 0; }
    virtual PositionTypeAST *asPositionType() { return 0; }
    virtual PositionPositionTypeAST *asPositionPositionType() { return 0; }
    virtual XYZWPRPositionTypeAST *asXYZWPRPositionType() { return 0; }
    virtual XYZWPREXTPositionTypeAST *asXYZWPREXTPositionType() { return 0; }
    virtual JointposPositionTypeAST *asJointposPositionType() { return 0; }
    virtual Jointpos1PositionTypeAST *asJointpos1PositionType() { return 0; }
    virtual Jointpos2PositionTypeAST *asJointpos2PositionType() { return 0; }
    virtual Jointpos3PositionTypeAST *asJointpos3PositionType() { return 0; }
    virtual Jointpos4PositionTypeAST *asJointpos4PositionType() { return 0; }
    virtual Jointpos5PositionTypeAST *asJointpos5PositionType() { return 0; }
    virtual Jointpos6PositionTypeAST *asJointpos6PositionType() { return 0; }
    virtual Jointpos7PositionTypeAST *asJointpos7PositionType() { return 0; }
    virtual Jointpos8PositionTypeAST *asJointpos8PositionType() { return 0; }
    virtual Jointpos9PositionTypeAST *asJointpos9PositionType() { return 0; }
    virtual RoutineDeclarationAST *asRoutineDeclaration() { return 0; }
    virtual RoutineImplemenationAST *asRoutineImplemenation() { return 0; }
    virtual FromPrgRoutineImplemenationAST *asFromPrgRoutineImplemenation() { return 0; }
    virtual DirectRoutineImplemenationAST *asDirectRoutineImplemenation() { return 0; }
    virtual RoutineDefinitionAST *asRoutineDefinition() { return 0; }
    virtual RoutineParametersClauseAST *asRoutineParametersClause() { return 0; }
    virtual RoutineReturnTypeClauseAST *asRoutineReturnTypeClause() { return 0; }
    virtual RoutineParameterAST *asRoutineParameter() { return 0; }
    virtual IdAST *asId() { return 0; }
    virtual PrimaryIdAST *asPrimaryId() { return 0; }
    virtual ArrayIdAST *asArrayId() { return 0; }
    virtual VariableFieldAST *asVariableField() { return 0; }
    virtual VariableAccessAST *asVariableAccess() { return 0; }
    virtual StatementAST *asStatement() { return 0; }
    virtual FakeVariableStatementAST *asFakeVariableStatement() { return 0; }
    virtual UsingStatementAST *asUsingStatement() { return 0; }

protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
};

class ModuleAST: public AST
{
public:
    unsigned t_program;
    ProgramNameAST *programName;
    DirectiveListAST *directives;
    VarTypeConstDeclarationListAST *declarations;
    RoutineDeclarationListAST *innerRoutines;
    unsigned t_begin;
    StatementListAST *statements;
    unsigned t_end;
    ProgramNameAST *programName1;
    RoutineDeclarationListAST *outerRoutines;
    const Scope *scope;

public:
    ModuleAST( unsigned t_program_ = 0, ProgramNameAST *programName_ = 0, DirectiveListAST *directives_ = 0, VarTypeConstDeclarationListAST *declarations_ = 0, RoutineDeclarationListAST *innerRoutines_ = 0, unsigned t_begin_ = 0, StatementListAST *statements_ = 0, unsigned t_end_ = 0, ProgramNameAST *programName1_ = 0, RoutineDeclarationListAST *outerRoutines_ = 0, const Scope *scope_ = 0 )
        : AST()
        , t_program(t_program_)
        , programName(programName_)
        , directives(directives_)
        , declarations(declarations_)
        , innerRoutines(innerRoutines_)
        , t_begin(t_begin_)
        , statements(statements_)
        , t_end(t_end_)
        , programName1(programName1_)
        , outerRoutines(outerRoutines_)
        , scope(scope_)
    {}

    virtual ModuleAST *asModule() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LiteralAST: public AST
{
public:
    LiteralAST() : AST() {}
    virtual LiteralAST *asLiteral() { return this; }
};

class IntegerLiteralAST: public LiteralAST
{
public:
    unsigned t_value;

public:
    IntegerLiteralAST( unsigned t_value_ = 0 )
        : LiteralAST()
        , t_value(t_value_)
    {}

    virtual IntegerLiteralAST *asIntegerLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArraySizeAST: public AST
{
public:
    ArraySizeAST() : AST() {}
    virtual ArraySizeAST *asArraySize() { return this; }
};

class IntegerLiteralOrStarAST: public ArraySizeAST
{
public:
    unsigned t_value_or_star;

public:
    IntegerLiteralOrStarAST( unsigned t_value_or_star_ = 0 )
        : ArraySizeAST()
        , t_value_or_star(t_value_or_star_)
    {}

    virtual IntegerLiteralOrStarAST *asIntegerLiteralOrStar() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstantArraySizeAST: public ArraySizeAST
{
public:
    ConstantNameAST *constantName;

public:
    ConstantArraySizeAST( ConstantNameAST *constantName_ = 0 )
        : ArraySizeAST()
        , constantName(constantName_)
    {}

    virtual ConstantArraySizeAST *asConstantArraySize() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StringLiteralAST: public LiteralAST
{
public:
    unsigned t_value;

public:
    StringLiteralAST( unsigned t_value_ = 0 )
        : LiteralAST()
        , t_value(t_value_)
    {}

    virtual StringLiteralAST *asStringLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class BooleanLiteralAST: public LiteralAST
{
public:
    unsigned t_value;

public:
    BooleanLiteralAST( unsigned t_value_ = 0 )
        : LiteralAST()
        , t_value(t_value_)
    {}

    virtual BooleanLiteralAST *asBooleanLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NameAST: public AST
{
public:
    NameAST() : AST() {}
    virtual NameAST *asName() { return this; }
};

class ProgramNameAST: public NameAST
{
public:
    unsigned t_identifier;

public:
    ProgramNameAST( unsigned t_identifier_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
    {}

    virtual ProgramNameAST *asProgramName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineNameAST: public NameAST
{
public:
    unsigned t_identifier;

public:
    RoutineNameAST( unsigned t_identifier_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
    {}

    virtual RoutineNameAST *asRoutineName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class EnvironmentNameAST: public NameAST
{
public:
    unsigned t_identifier;

public:
    EnvironmentNameAST( unsigned t_identifier_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
    {}

    virtual EnvironmentNameAST *asEnvironmentName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstantNameAST: public NameAST
{
public:
    unsigned t_identifier;

public:
    ConstantNameAST( unsigned t_identifier_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
    {}

    virtual ConstantNameAST *asConstantName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableNameAST: public NameAST
{
public:
    unsigned t_identifier;

public:
    VariableNameAST( unsigned t_identifier_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
    {}

    virtual VariableNameAST *asVariableName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FieldNameAST: public NameAST
{
public:
    unsigned t_identifier;

public:
    FieldNameAST( unsigned t_identifier_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
    {}

    virtual FieldNameAST *asFieldName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeNameAST: public NameAST
{
public:
    unsigned t_identifier;
    const Type *type;

public:
    TypeNameAST( unsigned t_identifier_ = 0, const Type *type_ = 0 )
        : NameAST()
        , t_identifier(t_identifier_)
        , type(type_)
    {}

    virtual TypeNameAST *asTypeName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DirectiveAST: public AST
{
public:
    DirectiveAST() : AST() {}
    virtual DirectiveAST *asDirective() { return this; }
};

class AlphabetizeDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    AlphabetizeDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual AlphabetizeDirectiveAST *asAlphabetizeDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CommentDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    unsigned t_comment;

public:
    CommentDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, unsigned t_comment_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , t_comment(t_comment_)
    {}

    virtual CommentDirectiveAST *asCommentDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CmosVarsDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    CmosVarsDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual CmosVarsDirectiveAST *asCmosVarsDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CrtDeviceDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    CrtDeviceDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual CrtDeviceDirectiveAST *asCrtDeviceDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DefGroupDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    unsigned t_group;

public:
    DefGroupDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, unsigned t_group_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , t_group(t_group_)
    {}

    virtual DefGroupDirectiveAST *asDefGroupDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DelayDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    unsigned t_delay;

public:
    DelayDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, unsigned t_delay_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , t_delay(t_delay_)
    {}

    virtual DelayDirectiveAST *asDelayDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InveronmentDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    EnvironmentNameAST *environmentName;

public:
    InveronmentDirectiveAST( unsigned t_keyword_ = 0, EnvironmentNameAST *environmentName_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , environmentName(environmentName_)
    {}

    virtual InveronmentDirectiveAST *asInveronmentDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IncludeDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    ProgramNameAST *programName;

public:
    IncludeDirectiveAST( unsigned t_keyword_ = 0, ProgramNameAST *programName_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , programName(programName_)
    {}

    virtual IncludeDirectiveAST *asIncludeDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class LockGroupDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    IntegerLiteralListAST *lockGroups;

public:
    LockGroupDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, IntegerLiteralListAST *lockGroups_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , lockGroups(lockGroups_)
    {}

    virtual LockGroupDirectiveAST *asLockGroupDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoAbortDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    unsigned t_d1;
    unsigned t_plus;
    unsigned t_d2;

public:
    NoAbortDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, unsigned t_d1_ = 0, unsigned t_plus_ = 0, unsigned t_d2_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , t_d1(t_d1_)
        , t_plus(t_plus_)
        , t_d2(t_d2_)
    {}

    virtual NoAbortDirectiveAST *asNoAbortDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoPauseDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    unsigned t_d1;
    unsigned t_plus;
    unsigned t_d2;
    unsigned t_plus1;
    unsigned t_d3;

public:
    NoPauseDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, unsigned t_d1_ = 0, unsigned t_plus_ = 0, unsigned t_d2_ = 0, unsigned t_plus1_ = 0, unsigned t_d3_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , t_d1(t_d1_)
        , t_plus(t_plus_)
        , t_d2(t_d2_)
        , t_plus1(t_plus1_)
        , t_d3(t_d3_)
    {}

    virtual NoPauseDirectiveAST *asNoPauseDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoBusyLampDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    NoBusyLampDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual NoBusyLampDirectiveAST *asNoBusyLampDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoLockGroupDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    NoLockGroupDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual NoLockGroupDirectiveAST *asNoLockGroupDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NoPauseShiftDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    NoPauseShiftDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual NoPauseShiftDirectiveAST *asNoPauseShiftDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PriorityDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    IntegerLiteralAST *value;

public:
    PriorityDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, IntegerLiteralAST *value_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , value(value_)
    {}

    virtual PriorityDirectiveAST *asPriorityDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StackSizeDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    IntegerLiteralAST *value;

public:
    StackSizeDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, IntegerLiteralAST *value_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , value(value_)
    {}

    virtual StackSizeDirectiveAST *asStackSizeDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TimeSliceDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;
    unsigned t_assign;
    IntegerLiteralAST *value;

public:
    TimeSliceDirectiveAST( unsigned t_keyword_ = 0, unsigned t_assign_ = 0, IntegerLiteralAST *value_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
        , t_assign(t_assign_)
        , value(value_)
    {}

    virtual TimeSliceDirectiveAST *asTimeSliceDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TpMotionDirectiveAST: public DirectiveAST
{
public:
    unsigned t_keyword;

public:
    TpMotionDirectiveAST( unsigned t_keyword_ = 0 )
        : DirectiveAST()
        , t_keyword(t_keyword_)
    {}

    virtual TpMotionDirectiveAST *asTpMotionDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VarTypeConstDeclarationAST: public AST
{
public:
    VarTypeConstDeclarationAST() : AST() {}
    virtual VarTypeConstDeclarationAST *asVarTypeConstDeclaration() { return this; }
};

class ConstDeclarationAST: public VarTypeConstDeclarationAST
{
public:
    unsigned t_const;
    ConstDeclarationItemListAST *declarations;

public:
    ConstDeclarationAST( unsigned t_const_ = 0, ConstDeclarationItemListAST *declarations_ = 0 )
        : VarTypeConstDeclarationAST()
        , t_const(t_const_)
        , declarations(declarations_)
    {}

    virtual ConstDeclarationAST *asConstDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeDeclarationAST: public VarTypeConstDeclarationAST
{
public:
    unsigned t_type;
    TypeDeclarationItemListAST *declarations;

public:
    TypeDeclarationAST( unsigned t_type_ = 0, TypeDeclarationItemListAST *declarations_ = 0 )
        : VarTypeConstDeclarationAST()
        , t_type(t_type_)
        , declarations(declarations_)
    {}

    virtual TypeDeclarationAST *asTypeDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VarDeclarationAST: public VarTypeConstDeclarationAST
{
public:
    unsigned t_var;
    VarDeclarationItemListAST *declarations;

public:
    VarDeclarationAST( unsigned t_var_ = 0, VarDeclarationItemListAST *declarations_ = 0 )
        : VarTypeConstDeclarationAST()
        , t_var(t_var_)
        , declarations(declarations_)
    {}

    virtual VarDeclarationAST *asVarDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstDeclarationItemAST: public AST
{
public:
    ConstantNameAST *constantName;
    unsigned t_assign;
    ConstantValueAST *value;

public:
    ConstDeclarationItemAST( ConstantNameAST *constantName_ = 0, unsigned t_assign_ = 0, ConstantValueAST *value_ = 0 )
        : AST()
        , constantName(constantName_)
        , t_assign(t_assign_)
        , value(value_)
    {}

    virtual ConstDeclarationItemAST *asConstDeclarationItem() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstantValueAST: public AST
{
public:
    ConstantValueAST() : AST() {}
    virtual ConstantValueAST *asConstantValue() { return this; }
};

class LiteralConstantValueAST: public ConstantValueAST
{
public:
    LiteralAST *value;

public:
    LiteralConstantValueAST( LiteralAST *value_ = 0 )
        : ConstantValueAST()
        , value(value_)
    {}

    virtual LiteralConstantValueAST *asLiteralConstantValue() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConstantNameConstantValueAST: public ConstantValueAST
{
public:
    ConstantNameAST *value;

public:
    ConstantNameConstantValueAST( ConstantNameAST *value_ = 0 )
        : ConstantValueAST()
        , value(value_)
    {}

    virtual ConstantNameConstantValueAST *asConstantNameConstantValue() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class TypeDeclarationItemAST: public AST
{
public:
    TypeNameAST *typeName;
    FromProgramClauseAST *fromProgram;
    unsigned t_assign;
    UserTypeAST *userType;

public:
    TypeDeclarationItemAST( TypeNameAST *typeName_ = 0, FromProgramClauseAST *fromProgram_ = 0, unsigned t_assign_ = 0, UserTypeAST *userType_ = 0 )
        : AST()
        , typeName(typeName_)
        , fromProgram(fromProgram_)
        , t_assign(t_assign_)
        , userType(userType_)
    {}

    virtual TypeDeclarationItemAST *asTypeDeclarationItem() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VarDeclarationItemAST: public AST
{
public:
    VariableNameListAST *variables;
    unsigned t_cmos_or_dram;
    FromProgramClauseAST *fromProgram;
    unsigned t_colon;
    DataTypeAST *type;

public:
    VarDeclarationItemAST( VariableNameListAST *variables_ = 0, unsigned t_cmos_or_dram_ = 0, FromProgramClauseAST *fromProgram_ = 0, unsigned t_colon_ = 0, DataTypeAST *type_ = 0 )
        : AST()
        , variables(variables_)
        , t_cmos_or_dram(t_cmos_or_dram_)
        , fromProgram(fromProgram_)
        , t_colon(t_colon_)
        , type(type_)
    {}

    virtual VarDeclarationItemAST *asVarDeclarationItem() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FromProgramClauseAST: public AST
{
public:
    unsigned t_from;
    ProgramNameAST *programName;

public:
    FromProgramClauseAST( unsigned t_from_ = 0, ProgramNameAST *programName_ = 0 )
        : AST()
        , t_from(t_from_)
        , programName(programName_)
    {}

    virtual FromProgramClauseAST *asFromProgramClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UserTypeAST: public AST
{
public:
    UserTypeAST() : AST() {}
    virtual UserTypeAST *asUserType() { return this; }
};

class DataUserTypeAST: public UserTypeAST
{
public:
    DataTypeAST *dataType;

public:
    DataUserTypeAST( DataTypeAST *dataType_ = 0 )
        : UserTypeAST()
        , dataType(dataType_)
    {}

    virtual DataUserTypeAST *asDataUserType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StructureUserTypeAST: public UserTypeAST
{
public:
    unsigned t_structure;
    FieldDefinitionListAST *fields;
    unsigned t_endstructure;

public:
    StructureUserTypeAST( unsigned t_structure_ = 0, FieldDefinitionListAST *fields_ = 0, unsigned t_endstructure_ = 0 )
        : UserTypeAST()
        , t_structure(t_structure_)
        , fields(fields_)
        , t_endstructure(t_endstructure_)
    {}

    virtual StructureUserTypeAST *asStructureUserType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FieldDefinitionAST: public AST
{
public:
    FieldNameAST *fieldName;
    unsigned t_colon;
    StructureArrayTypeAST *type;

public:
    FieldDefinitionAST( FieldNameAST *fieldName_ = 0, unsigned t_colon_ = 0, StructureArrayTypeAST *type_ = 0 )
        : AST()
        , fieldName(fieldName_)
        , t_colon(t_colon_)
        , type(type_)
    {}

    virtual FieldDefinitionAST *asFieldDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StructureArrayTypeAST: public AST
{
public:
    StructureArrayTypeAST() : AST() {}
    virtual StructureArrayTypeAST *asStructureArrayType() { return this; }
};

class DataStructureArrayTypeAST: public StructureArrayTypeAST
{
public:
    DataTypeAST *type;

public:
    DataStructureArrayTypeAST( DataTypeAST *type_ = 0 )
        : StructureArrayTypeAST()
        , type(type_)
    {}

    virtual DataStructureArrayTypeAST *asDataStructureArrayType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ByteStructureArrayTypeAST: public StructureArrayTypeAST
{
public:
    unsigned t_byte;

public:
    ByteStructureArrayTypeAST( unsigned t_byte_ = 0 )
        : StructureArrayTypeAST()
        , t_byte(t_byte_)
    {}

    virtual ByteStructureArrayTypeAST *asByteStructureArrayType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ShortStructureArrayTypeAST: public StructureArrayTypeAST
{
public:
    unsigned t_short;

public:
    ShortStructureArrayTypeAST( unsigned t_short_ = 0 )
        : StructureArrayTypeAST()
        , t_short(t_short_)
    {}

    virtual ShortStructureArrayTypeAST *asShortStructureArrayType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DataTypeAST: public AST
{
public:
    DataTypeAST() : AST() {}
    virtual DataTypeAST *asDataType() { return this; }
};

class UserDataTypeAST: public DataTypeAST
{
public:
    TypeNameAST *typeName;

public:
    UserDataTypeAST( TypeNameAST *typeName_ = 0 )
        : DataTypeAST()
        , typeName(typeName_)
    {}

    virtual UserDataTypeAST *asUserDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class SimpleDataTypeAST: public DataTypeAST
{
public:
    unsigned t_int_real_bool;

public:
    SimpleDataTypeAST( unsigned t_int_real_bool_ = 0 )
        : DataTypeAST()
        , t_int_real_bool(t_int_real_bool_)
    {}

    virtual SimpleDataTypeAST *asSimpleDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VectorDataTypeAST: public DataTypeAST
{
public:
    unsigned t_vector;

public:
    VectorDataTypeAST( unsigned t_vector_ = 0 )
        : DataTypeAST()
        , t_vector(t_vector_)
    {}

    virtual VectorDataTypeAST *asVectorDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VisProcessDataTypeAST: public DataTypeAST
{
public:
    unsigned t_vis_process;

public:
    VisProcessDataTypeAST( unsigned t_vis_process_ = 0 )
        : DataTypeAST()
        , t_vis_process(t_vis_process_)
    {}

    virtual VisProcessDataTypeAST *asVisProcessDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ModelDataTypeAST: public DataTypeAST
{
public:
    unsigned t_model;

public:
    ModelDataTypeAST( unsigned t_model_ = 0 )
        : DataTypeAST()
        , t_model(t_model_)
    {}

    virtual ModelDataTypeAST *asModelDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class CamSetupDataTypeAST: public DataTypeAST
{
public:
    unsigned t_can_setup;

public:
    CamSetupDataTypeAST( unsigned t_can_setup_ = 0 )
        : DataTypeAST()
        , t_can_setup(t_can_setup_)
    {}

    virtual CamSetupDataTypeAST *asCamSetupDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class FileDataTypeAST: public DataTypeAST
{
public:
    unsigned t_file;

public:
    FileDataTypeAST( unsigned t_file_ = 0 )
        : DataTypeAST()
        , t_file(t_file_)
    {}

    virtual FileDataTypeAST *asFileDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ConfigDataTypeAST: public DataTypeAST
{
public:
    unsigned t_config;

public:
    ConfigDataTypeAST( unsigned t_config_ = 0 )
        : DataTypeAST()
        , t_config(t_config_)
    {}

    virtual ConfigDataTypeAST *asConfigDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PathDataTypeAST: public DataTypeAST
{
public:
    unsigned t_path;
    PathHeaderClauseAST *pathHeader;
    NodeDataClauseAST *nodeData;

public:
    PathDataTypeAST( unsigned t_path_ = 0, PathHeaderClauseAST *pathHeader_ = 0, NodeDataClauseAST *nodeData_ = 0 )
        : DataTypeAST()
        , t_path(t_path_)
        , pathHeader(pathHeader_)
        , nodeData(nodeData_)
    {}

    virtual PathDataTypeAST *asPathDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PathHeaderClauseAST: public AST
{
public:
    unsigned t_path;
    unsigned t_header;
    unsigned t_assign;
    TypeNameAST *typeName;

public:
    PathHeaderClauseAST( unsigned t_path_ = 0, unsigned t_header_ = 0, unsigned t_assign_ = 0, TypeNameAST *typeName_ = 0 )
        : AST()
        , t_path(t_path_)
        , t_header(t_header_)
        , t_assign(t_assign_)
        , typeName(typeName_)
    {}

    virtual PathHeaderClauseAST *asPathHeaderClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class NodeDataClauseAST: public AST
{
public:
    unsigned t_nodedata;
    unsigned t_assign;
    TypeNameAST *typeName;

public:
    NodeDataClauseAST( unsigned t_nodedata_ = 0, unsigned t_assign_ = 0, TypeNameAST *typeName_ = 0 )
        : AST()
        , t_nodedata(t_nodedata_)
        , t_assign(t_assign_)
        , typeName(typeName_)
    {}

    virtual NodeDataClauseAST *asNodeDataClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StringDataTypeAST: public DataTypeAST
{
public:
    unsigned t_string;
    unsigned t_lbracket;
    ArraySizeAST *stringSize;
    unsigned t_rbracket;

public:
    StringDataTypeAST( unsigned t_string_ = 0, unsigned t_lbracket_ = 0, ArraySizeAST *stringSize_ = 0, unsigned t_rbracket_ = 0 )
        : DataTypeAST()
        , t_string(t_string_)
        , t_lbracket(t_lbracket_)
        , stringSize(stringSize_)
        , t_rbracket(t_rbracket_)
    {}

    virtual StringDataTypeAST *asStringDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrayDataTypeAST: public DataTypeAST
{
public:
    unsigned t_array;
    unsigned t_lbracket;
    ArraySizeListAST *sizes;
    unsigned t_rbracket;
    unsigned t_of;
    StructureArrayTypeAST *itemType;

public:
    ArrayDataTypeAST( unsigned t_array_ = 0, unsigned t_lbracket_ = 0, ArraySizeListAST *sizes_ = 0, unsigned t_rbracket_ = 0, unsigned t_of_ = 0, StructureArrayTypeAST *itemType_ = 0 )
        : DataTypeAST()
        , t_array(t_array_)
        , t_lbracket(t_lbracket_)
        , sizes(sizes_)
        , t_rbracket(t_rbracket_)
        , t_of(t_of_)
        , itemType(itemType_)
    {}

    virtual ArrayDataTypeAST *asArrayDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PositionDataTypeAST: public DataTypeAST
{
public:
    PositionTypeAST *positionType;
    InGroupClauseAST *inGroupClause;

public:
    PositionDataTypeAST( PositionTypeAST *positionType_ = 0, InGroupClauseAST *inGroupClause_ = 0 )
        : DataTypeAST()
        , positionType(positionType_)
        , inGroupClause(inGroupClause_)
    {}

    virtual PositionDataTypeAST *asPositionDataType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class InGroupClauseAST: public AST
{
public:
    unsigned t_in;
    unsigned t_group;
    unsigned t_lbracket;
    ArraySizeAST *groupNum;
    unsigned t_rbracket;

public:
    InGroupClauseAST( unsigned t_in_ = 0, unsigned t_group_ = 0, unsigned t_lbracket_ = 0, ArraySizeAST *groupNum_ = 0, unsigned t_rbracket_ = 0 )
        : AST()
        , t_in(t_in_)
        , t_group(t_group_)
        , t_lbracket(t_lbracket_)
        , groupNum(groupNum_)
        , t_rbracket(t_rbracket_)
    {}

    virtual InGroupClauseAST *asInGroupClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class PositionTypeAST: public AST
{
public:
    PositionTypeAST() : AST() {}
    virtual PositionTypeAST *asPositionType() { return this; }
};

class PositionPositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_position;

public:
    PositionPositionTypeAST( unsigned t_position_ = 0 )
        : PositionTypeAST()
        , t_position(t_position_)
    {}

    virtual PositionPositionTypeAST *asPositionPositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XYZWPRPositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_xyzwpr;

public:
    XYZWPRPositionTypeAST( unsigned t_xyzwpr_ = 0 )
        : PositionTypeAST()
        , t_xyzwpr(t_xyzwpr_)
    {}

    virtual XYZWPRPositionTypeAST *asXYZWPRPositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class XYZWPREXTPositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_xyzwprext;

public:
    XYZWPREXTPositionTypeAST( unsigned t_xyzwprext_ = 0 )
        : PositionTypeAST()
        , t_xyzwprext(t_xyzwprext_)
    {}

    virtual XYZWPREXTPositionTypeAST *asXYZWPREXTPositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class JointposPositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos;

public:
    JointposPositionTypeAST( unsigned t_jointpos_ = 0 )
        : PositionTypeAST()
        , t_jointpos(t_jointpos_)
    {}

    virtual JointposPositionTypeAST *asJointposPositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos1PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos1;

public:
    Jointpos1PositionTypeAST( unsigned t_jointpos1_ = 0 )
        : PositionTypeAST()
        , t_jointpos1(t_jointpos1_)
    {}

    virtual Jointpos1PositionTypeAST *asJointpos1PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos2PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos2;

public:
    Jointpos2PositionTypeAST( unsigned t_jointpos2_ = 0 )
        : PositionTypeAST()
        , t_jointpos2(t_jointpos2_)
    {}

    virtual Jointpos2PositionTypeAST *asJointpos2PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos3PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos3;

public:
    Jointpos3PositionTypeAST( unsigned t_jointpos3_ = 0 )
        : PositionTypeAST()
        , t_jointpos3(t_jointpos3_)
    {}

    virtual Jointpos3PositionTypeAST *asJointpos3PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos4PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos4;

public:
    Jointpos4PositionTypeAST( unsigned t_jointpos4_ = 0 )
        : PositionTypeAST()
        , t_jointpos4(t_jointpos4_)
    {}

    virtual Jointpos4PositionTypeAST *asJointpos4PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos5PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos5;

public:
    Jointpos5PositionTypeAST( unsigned t_jointpos5_ = 0 )
        : PositionTypeAST()
        , t_jointpos5(t_jointpos5_)
    {}

    virtual Jointpos5PositionTypeAST *asJointpos5PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos6PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos6;

public:
    Jointpos6PositionTypeAST( unsigned t_jointpos6_ = 0 )
        : PositionTypeAST()
        , t_jointpos6(t_jointpos6_)
    {}

    virtual Jointpos6PositionTypeAST *asJointpos6PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos7PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos7;

public:
    Jointpos7PositionTypeAST( unsigned t_jointpos7_ = 0 )
        : PositionTypeAST()
        , t_jointpos7(t_jointpos7_)
    {}

    virtual Jointpos7PositionTypeAST *asJointpos7PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos8PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos8;

public:
    Jointpos8PositionTypeAST( unsigned t_jointpos8_ = 0 )
        : PositionTypeAST()
        , t_jointpos8(t_jointpos8_)
    {}

    virtual Jointpos8PositionTypeAST *asJointpos8PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class Jointpos9PositionTypeAST: public PositionTypeAST
{
public:
    unsigned t_jointpos9;

public:
    Jointpos9PositionTypeAST( unsigned t_jointpos9_ = 0 )
        : PositionTypeAST()
        , t_jointpos9(t_jointpos9_)
    {}

    virtual Jointpos9PositionTypeAST *asJointpos9PositionType() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineDeclarationAST: public AST
{
public:
    RoutineDefinitionAST *definition;
    RoutineImplemenationAST *implementation;
    const Scope *scope;

public:
    RoutineDeclarationAST( RoutineDefinitionAST *definition_ = 0, RoutineImplemenationAST *implementation_ = 0, const Scope *scope_ = 0 )
        : AST()
        , definition(definition_)
        , implementation(implementation_)
        , scope(scope_)
    {}

    virtual RoutineDeclarationAST *asRoutineDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineImplemenationAST: public AST
{
public:
    RoutineImplemenationAST() : AST() {}
    virtual RoutineImplemenationAST *asRoutineImplemenation() { return this; }
};

class FromPrgRoutineImplemenationAST: public RoutineImplemenationAST
{
public:
    unsigned t_from;
    ProgramNameAST *programName;

public:
    FromPrgRoutineImplemenationAST( unsigned t_from_ = 0, ProgramNameAST *programName_ = 0 )
        : RoutineImplemenationAST()
        , t_from(t_from_)
        , programName(programName_)
    {}

    virtual FromPrgRoutineImplemenationAST *asFromPrgRoutineImplemenation() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class DirectRoutineImplemenationAST: public RoutineImplemenationAST
{
public:
    VarTypeConstDeclarationListAST *declarations;
    unsigned t_begin;
    StatementListAST *statements;
    unsigned t_end;
    RoutineNameAST *routineName;

public:
    DirectRoutineImplemenationAST( VarTypeConstDeclarationListAST *declarations_ = 0, unsigned t_begin_ = 0, StatementListAST *statements_ = 0, unsigned t_end_ = 0, RoutineNameAST *routineName_ = 0 )
        : RoutineImplemenationAST()
        , declarations(declarations_)
        , t_begin(t_begin_)
        , statements(statements_)
        , t_end(t_end_)
        , routineName(routineName_)
    {}

    virtual DirectRoutineImplemenationAST *asDirectRoutineImplemenation() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineDefinitionAST: public AST
{
public:
    unsigned t_routine;
    RoutineNameAST *routineName;
    RoutineParametersClauseAST *parametersClause;
    RoutineReturnTypeClauseAST *returnType;

public:
    RoutineDefinitionAST( unsigned t_routine_ = 0, RoutineNameAST *routineName_ = 0, RoutineParametersClauseAST *parametersClause_ = 0, RoutineReturnTypeClauseAST *returnType_ = 0 )
        : AST()
        , t_routine(t_routine_)
        , routineName(routineName_)
        , parametersClause(parametersClause_)
        , returnType(returnType_)
    {}

    virtual RoutineDefinitionAST *asRoutineDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineParametersClauseAST: public AST
{
public:
    unsigned t_lparen;
    RoutineParameterListAST *parameters;
    unsigned t_rparen;

public:
    RoutineParametersClauseAST( unsigned t_lparen_ = 0, RoutineParameterListAST *parameters_ = 0, unsigned t_rparen_ = 0 )
        : AST()
        , t_lparen(t_lparen_)
        , parameters(parameters_)
        , t_rparen(t_rparen_)
    {}

    virtual RoutineParametersClauseAST *asRoutineParametersClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineReturnTypeClauseAST: public AST
{
public:
    unsigned t_colon;
    DataTypeAST *returnType;

public:
    RoutineReturnTypeClauseAST( unsigned t_colon_ = 0, DataTypeAST *returnType_ = 0 )
        : AST()
        , t_colon(t_colon_)
        , returnType(returnType_)
    {}

    virtual RoutineReturnTypeClauseAST *asRoutineReturnTypeClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class RoutineParameterAST: public AST
{
public:
    VariableNameListAST *vars;
    unsigned t_colon;
    DataTypeAST *type;

public:
    RoutineParameterAST( VariableNameListAST *vars_ = 0, unsigned t_colon_ = 0, DataTypeAST *type_ = 0 )
        : AST()
        , vars(vars_)
        , t_colon(t_colon_)
        , type(type_)
    {}

    virtual RoutineParameterAST *asRoutineParameter() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class IdAST: public AST
{
public:
    IdAST() : AST() {}
    virtual IdAST *asId() { return this; }
};

class PrimaryIdAST: public IdAST
{
public:
    VariableNameAST *variable;

public:
    PrimaryIdAST( VariableNameAST *variable_ = 0 )
        : IdAST()
        , variable(variable_)
    {}

    virtual PrimaryIdAST *asPrimaryId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class ArrayIdAST: public IdAST
{
public:
    IdAST *id;
    unsigned t_lbracket;
    VariableAccessListAST *vars;
    unsigned t_rbaracket;

public:
    ArrayIdAST( IdAST *id_ = 0, unsigned t_lbracket_ = 0, VariableAccessListAST *vars_ = 0, unsigned t_rbaracket_ = 0 )
        : IdAST()
        , id(id_)
        , t_lbracket(t_lbracket_)
        , vars(vars_)
        , t_rbaracket(t_rbaracket_)
    {}

    virtual ArrayIdAST *asArrayId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableFieldAST: public AST
{
public:
    unsigned t_dot;
    VariableAccessAST *field;

public:
    VariableFieldAST( unsigned t_dot_ = 0, VariableAccessAST *field_ = 0 )
        : AST()
        , t_dot(t_dot_)
        , field(field_)
    {}

    virtual VariableFieldAST *asVariableField() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class VariableAccessAST: public AST
{
public:
    IdAST *id;
    VariableFieldAST *field;

public:
    VariableAccessAST( IdAST *id_ = 0, VariableFieldAST *field_ = 0 )
        : AST()
        , id(id_)
        , field(field_)
    {}

    virtual VariableAccessAST *asVariableAccess() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class StatementAST: public AST
{
public:
    StatementAST() : AST() {}
    virtual StatementAST *asStatement() { return this; }
};

class FakeVariableStatementAST: public StatementAST
{
public:
    VariableAccessAST *var;

public:
    FakeVariableStatementAST( VariableAccessAST *var_ = 0 )
        : StatementAST()
        , var(var_)
    {}

    virtual FakeVariableStatementAST *asFakeVariableStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

class UsingStatementAST: public StatementAST
{
public:
    unsigned t_using;
    VariableAccessListAST *structVars;
    unsigned t_do;
    StatementListAST *statements;
    unsigned t_endusing;
    const Scope *scope;

public:
    UsingStatementAST( unsigned t_using_ = 0, VariableAccessListAST *structVars_ = 0, unsigned t_do_ = 0, StatementListAST *statements_ = 0, unsigned t_endusing_ = 0, const Scope *scope_ = 0 )
        : StatementAST()
        , t_using(t_using_)
        , structVars(structVars_)
        , t_do(t_do_)
        , statements(statements_)
        , t_endusing(t_endusing_)
        , scope(scope_)
    {}

    virtual UsingStatementAST *asUsingStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

protected:
    virtual void accept0( ASTVisitor *visitor );
};

}   // namespace Karel

#endif // KARELAST_H
