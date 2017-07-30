#ifndef PARSER_H
#define PARSER_H

#include "KarelASTfwd.h"
#include "TranslationUnit.h"

namespace Karel {

class Parser
{
public:
    Parser(TranslationUnit *translationUnit);
    ~Parser();

    bool parseModule(ModuleAST *&node);
    bool parseDirectiveList(DirectiveListAST *&directives);
    bool parseIntegerList(IntegerLiteralListAST *&integers);
    bool parseArraySizeList(ArraySizeListAST *&sizes);
    bool parseParameterArraySizeList(ArraySizeListAST *&sizes);
    bool parseModuleDeclarationList(VarTypeConstDeclarationListAST *&declarations);
    bool parseRoutineList(RoutineDeclarationListAST *&routines);
    bool parseRoutine(RoutineDeclarationAST *&routine);
    bool parseRoutinesParameters(RoutineParametersClauseAST *&paramsClause);
    bool parseConstDeclarationItemList(ConstDeclarationItemListAST *&constants);
    bool parseTypeDeclarationItemList(TypeDeclarationItemListAST *&types);
    bool parseUserType(UserTypeAST *&userType);
    bool parseDataType(DataTypeAST *&dataType, bool isParamType = false, bool isArrayAllowed = true);
    bool parseStructureArrayType(StructureArrayTypeAST *&type, bool isParamType, bool isArrayAllowed = true);
    bool parseInGroupClause(InGroupClauseAST *&inGroup);
    bool parseVarDeclarationItemList(VarDeclarationItemListAST *&variables);
    bool parseRoutineParameterList(RoutineParameterListAST *&parameters);
    bool parseVariableNameList(VariableNameListAST *&variables);
    bool parseStatementList(StatementListAST *&statements, unsigned usingDepth = 0);
    bool parseUsingStatement(UsingStatementAST *&usingStatement, unsigned usingDepth);
    bool parseVariableField(VariableFieldAST *&field);
    bool parseVariableAccess(VariableAccessAST *&var);

    ArrayIdAST *parseArrayAccess(IdAST *lhs);
    bool parseInBrackets(VariableAccessListAST *&vars);

    bool matchKeyword(int keyid, unsigned *token);
    bool checkNewLine();
    bool skipToNewLine(bool reportWarning);
    bool skipToNewLineOrPrecKind(bool reportWarning, unsigned precKind);

    void warning(unsigned index, const char *format, ...);
    void error(unsigned index, const char *format, ...);

    inline int consumeToken()
    { return _tokenIndex++; }

    inline const Token &tok(int i = 1) const
    { return _translationUnit->tokenAt(_tokenIndex + i - 1); }

    inline int LA(int n = 1) const
    { return _translationUnit->tokenKind(_tokenIndex + n - 1); }

    inline int KI(int n = 1) const
    { return _translationUnit->tokenKeyword(_tokenIndex + n - 1); }


private:
    TranslationUnit *_translationUnit;
    Control *_control;
    Utils::MemoryPool *_pool;
    unsigned _tokenIndex;
};

}   // namespace Karel

#endif // PARSER_H
