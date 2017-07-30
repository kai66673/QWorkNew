#ifndef KARELASTVISITOR_H
#define KARELASTVISITOR_H

#include "KarelASTfwd.h"
#include "TranslationUnit.h"

namespace Karel {

class ASTVisitor
{
    ASTVisitor(const ASTVisitor &other);
    void operator =(const ASTVisitor &other);

public:
    ASTVisitor(TranslationUnit *unit);
    virtual ~ASTVisitor();

    TranslationUnit *translationUnit() const;
    void setTranslationUnit(TranslationUnit *translationUnit);

    Control *control() const;
    unsigned tokenCount() const;
    const Token &tokenAt(unsigned index) const;
    int tokenKind(unsigned index) const;
    const char *spell(unsigned index) const;
    const Identifier *identifier(unsigned index) const;
    const Literal *literal(unsigned index) const;
    const NumericLiteral *numericLiteral(unsigned index) const;
    const StringLiteral *stringLiteral(unsigned index) const;

    void getPosition(unsigned offset,
                     unsigned *line,
                     unsigned *column = 0,
                     const StringLiteral **fileName = 0) const;

    void getTokenPosition(unsigned index,
                          unsigned *line,
                          unsigned *column = 0,
                          const StringLiteral **fileName = 0) const;

    void getTokenStartPosition(unsigned index, unsigned *line, unsigned *column) const;
    void getTokenEndPosition(unsigned index, unsigned *line, unsigned *column) const;

    void accept(AST *ast);

    template <typename _Tp>
    void accept(List<_Tp> *it)
    {
        for (; it; it = it->next)
            accept(it->value);
    }

    virtual bool preVisit(AST *) { return true; }
    virtual void postVisit(AST *) {}

    virtual bool visit(ModuleAST *) { return true; }
    virtual void endVisit(ModuleAST *) {}
    virtual bool visit(LiteralAST *) { return true; }
    virtual void endVisit(LiteralAST *) {}
    virtual bool visit(IntegerLiteralAST *) { return true; }
    virtual void endVisit(IntegerLiteralAST *) {}
    virtual bool visit(ArraySizeAST *) { return true; }
    virtual void endVisit(ArraySizeAST *) {}
    virtual bool visit(IntegerLiteralOrStarAST *) { return true; }
    virtual void endVisit(IntegerLiteralOrStarAST *) {}
    virtual bool visit(ConstantArraySizeAST *) { return true; }
    virtual void endVisit(ConstantArraySizeAST *) {}
    virtual bool visit(StringLiteralAST *) { return true; }
    virtual void endVisit(StringLiteralAST *) {}
    virtual bool visit(BooleanLiteralAST *) { return true; }
    virtual void endVisit(BooleanLiteralAST *) {}
    virtual bool visit(NameAST *) { return true; }
    virtual void endVisit(NameAST *) {}
    virtual bool visit(ProgramNameAST *) { return true; }
    virtual void endVisit(ProgramNameAST *) {}
    virtual bool visit(RoutineNameAST *) { return true; }
    virtual void endVisit(RoutineNameAST *) {}
    virtual bool visit(EnvironmentNameAST *) { return true; }
    virtual void endVisit(EnvironmentNameAST *) {}
    virtual bool visit(ConstantNameAST *) { return true; }
    virtual void endVisit(ConstantNameAST *) {}
    virtual bool visit(VariableNameAST *) { return true; }
    virtual void endVisit(VariableNameAST *) {}
    virtual bool visit(FieldNameAST *) { return true; }
    virtual void endVisit(FieldNameAST *) {}
    virtual bool visit(TypeNameAST *) { return true; }
    virtual void endVisit(TypeNameAST *) {}
    virtual bool visit(DirectiveAST *) { return true; }
    virtual void endVisit(DirectiveAST *) {}
    virtual bool visit(AlphabetizeDirectiveAST *) { return true; }
    virtual void endVisit(AlphabetizeDirectiveAST *) {}
    virtual bool visit(CommentDirectiveAST *) { return true; }
    virtual void endVisit(CommentDirectiveAST *) {}
    virtual bool visit(CmosVarsDirectiveAST *) { return true; }
    virtual void endVisit(CmosVarsDirectiveAST *) {}
    virtual bool visit(CrtDeviceDirectiveAST *) { return true; }
    virtual void endVisit(CrtDeviceDirectiveAST *) {}
    virtual bool visit(DefGroupDirectiveAST *) { return true; }
    virtual void endVisit(DefGroupDirectiveAST *) {}
    virtual bool visit(DelayDirectiveAST *) { return true; }
    virtual void endVisit(DelayDirectiveAST *) {}
    virtual bool visit(InveronmentDirectiveAST *) { return true; }
    virtual void endVisit(InveronmentDirectiveAST *) {}
    virtual bool visit(IncludeDirectiveAST *) { return true; }
    virtual void endVisit(IncludeDirectiveAST *) {}
    virtual bool visit(LockGroupDirectiveAST *) { return true; }
    virtual void endVisit(LockGroupDirectiveAST *) {}
    virtual bool visit(NoAbortDirectiveAST *) { return true; }
    virtual void endVisit(NoAbortDirectiveAST *) {}
    virtual bool visit(NoPauseDirectiveAST *) { return true; }
    virtual void endVisit(NoPauseDirectiveAST *) {}
    virtual bool visit(NoBusyLampDirectiveAST *) { return true; }
    virtual void endVisit(NoBusyLampDirectiveAST *) {}
    virtual bool visit(NoLockGroupDirectiveAST *) { return true; }
    virtual void endVisit(NoLockGroupDirectiveAST *) {}
    virtual bool visit(NoPauseShiftDirectiveAST *) { return true; }
    virtual void endVisit(NoPauseShiftDirectiveAST *) {}
    virtual bool visit(PriorityDirectiveAST *) { return true; }
    virtual void endVisit(PriorityDirectiveAST *) {}
    virtual bool visit(StackSizeDirectiveAST *) { return true; }
    virtual void endVisit(StackSizeDirectiveAST *) {}
    virtual bool visit(TimeSliceDirectiveAST *) { return true; }
    virtual void endVisit(TimeSliceDirectiveAST *) {}
    virtual bool visit(TpMotionDirectiveAST *) { return true; }
    virtual void endVisit(TpMotionDirectiveAST *) {}
    virtual bool visit(VarTypeConstDeclarationAST *) { return true; }
    virtual void endVisit(VarTypeConstDeclarationAST *) {}
    virtual bool visit(ConstDeclarationAST *) { return true; }
    virtual void endVisit(ConstDeclarationAST *) {}
    virtual bool visit(TypeDeclarationAST *) { return true; }
    virtual void endVisit(TypeDeclarationAST *) {}
    virtual bool visit(VarDeclarationAST *) { return true; }
    virtual void endVisit(VarDeclarationAST *) {}
    virtual bool visit(ConstDeclarationItemAST *) { return true; }
    virtual void endVisit(ConstDeclarationItemAST *) {}
    virtual bool visit(ConstantValueAST *) { return true; }
    virtual void endVisit(ConstantValueAST *) {}
    virtual bool visit(LiteralConstantValueAST *) { return true; }
    virtual void endVisit(LiteralConstantValueAST *) {}
    virtual bool visit(ConstantNameConstantValueAST *) { return true; }
    virtual void endVisit(ConstantNameConstantValueAST *) {}
    virtual bool visit(TypeDeclarationItemAST *) { return true; }
    virtual void endVisit(TypeDeclarationItemAST *) {}
    virtual bool visit(VarDeclarationItemAST *) { return true; }
    virtual void endVisit(VarDeclarationItemAST *) {}
    virtual bool visit(FromProgramClauseAST *) { return true; }
    virtual void endVisit(FromProgramClauseAST *) {}
    virtual bool visit(UserTypeAST *) { return true; }
    virtual void endVisit(UserTypeAST *) {}
    virtual bool visit(DataUserTypeAST *) { return true; }
    virtual void endVisit(DataUserTypeAST *) {}
    virtual bool visit(StructureUserTypeAST *) { return true; }
    virtual void endVisit(StructureUserTypeAST *) {}
    virtual bool visit(FieldDefinitionAST *) { return true; }
    virtual void endVisit(FieldDefinitionAST *) {}
    virtual bool visit(StructureArrayTypeAST *) { return true; }
    virtual void endVisit(StructureArrayTypeAST *) {}
    virtual bool visit(DataStructureArrayTypeAST *) { return true; }
    virtual void endVisit(DataStructureArrayTypeAST *) {}
    virtual bool visit(ByteStructureArrayTypeAST *) { return true; }
    virtual void endVisit(ByteStructureArrayTypeAST *) {}
    virtual bool visit(ShortStructureArrayTypeAST *) { return true; }
    virtual void endVisit(ShortStructureArrayTypeAST *) {}
    virtual bool visit(DataTypeAST *) { return true; }
    virtual void endVisit(DataTypeAST *) {}
    virtual bool visit(UserDataTypeAST *) { return true; }
    virtual void endVisit(UserDataTypeAST *) {}
    virtual bool visit(SimpleDataTypeAST *) { return true; }
    virtual void endVisit(SimpleDataTypeAST *) {}
    virtual bool visit(VectorDataTypeAST *) { return true; }
    virtual void endVisit(VectorDataTypeAST *) {}
    virtual bool visit(VisProcessDataTypeAST *) { return true; }
    virtual void endVisit(VisProcessDataTypeAST *) {}
    virtual bool visit(ModelDataTypeAST *) { return true; }
    virtual void endVisit(ModelDataTypeAST *) {}
    virtual bool visit(CamSetupDataTypeAST *) { return true; }
    virtual void endVisit(CamSetupDataTypeAST *) {}
    virtual bool visit(FileDataTypeAST *) { return true; }
    virtual void endVisit(FileDataTypeAST *) {}
    virtual bool visit(ConfigDataTypeAST *) { return true; }
    virtual void endVisit(ConfigDataTypeAST *) {}
    virtual bool visit(PathDataTypeAST *) { return true; }
    virtual void endVisit(PathDataTypeAST *) {}
    virtual bool visit(PathHeaderClauseAST *) { return true; }
    virtual void endVisit(PathHeaderClauseAST *) {}
    virtual bool visit(NodeDataClauseAST *) { return true; }
    virtual void endVisit(NodeDataClauseAST *) {}
    virtual bool visit(StringDataTypeAST *) { return true; }
    virtual void endVisit(StringDataTypeAST *) {}
    virtual bool visit(ArrayDataTypeAST *) { return true; }
    virtual void endVisit(ArrayDataTypeAST *) {}
    virtual bool visit(PositionDataTypeAST *) { return true; }
    virtual void endVisit(PositionDataTypeAST *) {}
    virtual bool visit(InGroupClauseAST *) { return true; }
    virtual void endVisit(InGroupClauseAST *) {}
    virtual bool visit(PositionTypeAST *) { return true; }
    virtual void endVisit(PositionTypeAST *) {}
    virtual bool visit(PositionPositionTypeAST *) { return true; }
    virtual void endVisit(PositionPositionTypeAST *) {}
    virtual bool visit(XYZWPRPositionTypeAST *) { return true; }
    virtual void endVisit(XYZWPRPositionTypeAST *) {}
    virtual bool visit(XYZWPREXTPositionTypeAST *) { return true; }
    virtual void endVisit(XYZWPREXTPositionTypeAST *) {}
    virtual bool visit(JointposPositionTypeAST *) { return true; }
    virtual void endVisit(JointposPositionTypeAST *) {}
    virtual bool visit(Jointpos1PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos1PositionTypeAST *) {}
    virtual bool visit(Jointpos2PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos2PositionTypeAST *) {}
    virtual bool visit(Jointpos3PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos3PositionTypeAST *) {}
    virtual bool visit(Jointpos4PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos4PositionTypeAST *) {}
    virtual bool visit(Jointpos5PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos5PositionTypeAST *) {}
    virtual bool visit(Jointpos6PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos6PositionTypeAST *) {}
    virtual bool visit(Jointpos7PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos7PositionTypeAST *) {}
    virtual bool visit(Jointpos8PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos8PositionTypeAST *) {}
    virtual bool visit(Jointpos9PositionTypeAST *) { return true; }
    virtual void endVisit(Jointpos9PositionTypeAST *) {}
    virtual bool visit(RoutineDeclarationAST *) { return true; }
    virtual void endVisit(RoutineDeclarationAST *) {}
    virtual bool visit(RoutineImplemenationAST *) { return true; }
    virtual void endVisit(RoutineImplemenationAST *) {}
    virtual bool visit(FromPrgRoutineImplemenationAST *) { return true; }
    virtual void endVisit(FromPrgRoutineImplemenationAST *) {}
    virtual bool visit(DirectRoutineImplemenationAST *) { return true; }
    virtual void endVisit(DirectRoutineImplemenationAST *) {}
    virtual bool visit(RoutineDefinitionAST *) { return true; }
    virtual void endVisit(RoutineDefinitionAST *) {}
    virtual bool visit(RoutineParametersClauseAST *) { return true; }
    virtual void endVisit(RoutineParametersClauseAST *) {}
    virtual bool visit(RoutineReturnTypeClauseAST *) { return true; }
    virtual void endVisit(RoutineReturnTypeClauseAST *) {}
    virtual bool visit(RoutineParameterAST *) { return true; }
    virtual void endVisit(RoutineParameterAST *) {}
    virtual bool visit(IdAST *) { return true; }
    virtual void endVisit(IdAST *) {}
    virtual bool visit(PrimaryIdAST *) { return true; }
    virtual void endVisit(PrimaryIdAST *) {}
    virtual bool visit(ArrayIdAST *) { return true; }
    virtual void endVisit(ArrayIdAST *) {}
    virtual bool visit(VariableFieldAST *) { return true; }
    virtual void endVisit(VariableFieldAST *) {}
    virtual bool visit(VariableAccessAST *) { return true; }
    virtual void endVisit(VariableAccessAST *) {}
    virtual bool visit(StatementAST *) { return true; }
    virtual void endVisit(StatementAST *) {}
    virtual bool visit(FakeVariableStatementAST *) { return true; }
    virtual void endVisit(FakeVariableStatementAST *) {}
    virtual bool visit(UsingStatementAST *) { return true; }
    virtual void endVisit(UsingStatementAST *) {}

private:
    TranslationUnit *_translationUnit;
};

}   // namespace Karel

#endif // KARELASTVISITOR_H
