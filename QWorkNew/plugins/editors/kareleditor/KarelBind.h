#ifndef KARELBIND_H
#define KARELBIND_H

#include "KarelASTVisitor.h"
#include "Control.h"

namespace Karel {

class Bind: protected ASTVisitor
{
public:
    Bind(TranslationUnit *unit);

    void operator()(ModuleAST *ast, Scope *globalScope);

protected:
    Scope *switchScope(Scope *scope);

    bool visit(RoutineDeclarationAST *ast);
    void endVisit(RoutineDeclarationAST *);

    bool visit(DirectRoutineImplemenationAST *ast);
    bool visit(ConstDeclarationItemAST *ast);

    bool visit(TypeDeclarationItemAST *ast);
    bool visit(VarDeclarationItemAST *ast);
    bool visit(RoutineParameterAST *ast);

    bool visit(ByteStructureArrayTypeAST *);
    bool visit(ShortStructureArrayTypeAST *);
    bool visit(CamSetupDataTypeAST *);
    bool visit(ConfigDataTypeAST *);
    bool visit(FileDataTypeAST *);
    bool visit(ModelDataTypeAST *);
    bool visit(PathDataTypeAST *);
    bool visit(SimpleDataTypeAST *);
    bool visit(StringDataTypeAST *);
    bool visit(UserDataTypeAST *ast);
    bool visit(VectorDataTypeAST *);
    bool visit(VisProcessDataTypeAST *);
    bool visit(JointposPositionTypeAST *);
    bool visit(PositionPositionTypeAST *);
    bool visit(XYZWPRPositionTypeAST *);
    bool visit(XYZWPREXTPositionTypeAST *);
    bool visit(Jointpos1PositionTypeAST *);
    bool visit(Jointpos2PositionTypeAST *);
    bool visit(Jointpos3PositionTypeAST *);
    bool visit(Jointpos4PositionTypeAST *);
    bool visit(Jointpos5PositionTypeAST *);
    bool visit(Jointpos6PositionTypeAST *);
    bool visit(Jointpos7PositionTypeAST *);
    bool visit(Jointpos8PositionTypeAST *);
    bool visit(Jointpos9PositionTypeAST *);

    bool visit(ArrayDataTypeAST *ast);

    bool visit(FakeVariableStatementAST *);
    bool visit(UsingStatementAST *ast);

    bool visit(VariableAccessAST *ast);
    bool visit(PrimaryIdAST *ast);
    bool visit(ArrayIdAST *ast);

private:
    void proceedVarDeclarations(VariableNameListAST *variables, DataTypeAST *typeAst);

private:
    Scope *_scope;
    Control *_control;
    Type *_type;
    const Type *_usingType;
};

}   //namespace Karel

#endif // KARELBIND_H
