import os

namespace = 'Karel'

work_dir = 'd:/Alex/_____HG/Karel'

classes = [
    [ 'Module',                 '',
        [ ( 0, 'program' ),
          ( 1, 'ProgramName', 'programName' ),
          ( 2, 'Directive', 'directives' ),
          ( 2, 'VarTypeConstDeclaration', 'declarations' ),
          ( 2, 'RoutineDeclaration', 'innerRoutines' ),
          ( 0, 'begin' ),
          ( 2, 'Statement', 'statements' ),
          ( 0, 'end' ),
          ( 1, 'ProgramName', 'programName1' ),
          ( 2, 'RoutineDeclaration', 'outerRoutines' ),
          ( 3, 'Scope', 'scope' ) ] ],
    [ 'Literal',                '',                     [] ],
    [ 'IntegerLiteral',         'Literal',
        [ ( 0, 'value' ) ] ],
    [ 'ArraySize',              '',                     [] ],
    [ 'IntegerLiteralOrStar',   'ArraySize',
        [ ( 0, 'value_or_star' ) ] ],
    [ 'ConstantArraySize',      'ArraySize',
        [ ( 1, 'ConstantName', 'constantName' ) ] ],
    [ 'StringLiteral',          'Literal',
        [ ( 0, 'value' ) ] ],
    [ 'BooleanLiteral',         'Literal',
        [ ( 0, 'value' ) ] ],
    [ 'Name',                   '',                     [] ],
    [ 'ProgramName',            'Name',                 [ (0, 'identifier') ] ],
    [ 'RoutineName',            'Name',                 [ (0, 'identifier') ] ],
    [ 'EnvironmentName',        'Name',                 [ (0, 'identifier') ] ],
    [ 'ConstantName',           'Name',                 [ (0, 'identifier') ] ],
    [ 'VariableName',           'Name',                 [ (0, 'identifier') ] ],
    [ 'FieldName',              'Name',                 [ (0, 'identifier') ] ],
    [ 'TypeName',               'Name',                 [ (0, 'identifier'), (3, 'Type', 'type') ] ],
    [ 'Directive',              '',                     [] ],
    [ 'AlphabetizeDirective',   'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'CommentDirective',       'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 0, 'comment' ) ] ],
    [ 'CmosVarsDirective',      'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'CrtDeviceDirective',     'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'DefGroupDirective',      'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 0, 'group' ) ] ],
    [ 'DelayDirective',         'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 0, 'delay' ) ] ],
    [ 'InveronmentDirective',   'Directive',
        [ ( 0, 'keyword' ),
          ( 1, 'EnvironmentName', 'environmentName' ) ] ],
    [ 'IncludeDirective',       'Directive',
        [ ( 0, 'keyword' ),
          ( 1, 'ProgramName', 'programName' ) ] ],
    [ 'LockGroupDirective',     'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 2, 'IntegerLiteral', 'lockGroups' ) ] ],
    [ 'NoAbortDirective',       'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 0, 'd1' ),
          ( 0, 'plus' ),
          ( 0, 'd2' ) ] ],
    [ 'NoPauseDirective',       'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 0, 'd1' ),
          ( 0, 'plus' ),
          ( 0, 'd2' ),
          ( 0, 'plus1' ),
          ( 0, 'd3' ) ] ],
    [ 'NoBusyLampDirective',    'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'NoLockGroupDirective',   'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'NoPauseShiftDirective',  'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'PriorityDirective',      'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 1, 'IntegerLiteral', 'value' ) ] ],
    [ 'StackSizeDirective',     'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 1, 'IntegerLiteral', 'value' ) ] ],
    [ 'TimeSliceDirective',     'Directive',
        [ ( 0, 'keyword' ),
          ( 0, 'assign' ),
          ( 1, 'IntegerLiteral', 'value' ) ] ],
    [ 'TpMotionDirective',      'Directive',
        [ ( 0, 'keyword' ) ] ],
    [ 'VarTypeConstDeclaration',      '',             [] ],
    [ 'ConstDeclaration', 'VarTypeConstDeclaration',
        [ ( 0, 'const' ),
          ( 2, 'ConstDeclarationItem', 'declarations' ) ] ],
    [ 'TypeDeclaration',  'VarTypeConstDeclaration',
        [ (0, 'type' ),
          (2, 'TypeDeclarationItem', 'declarations' ) ] ],
    [ 'VarDeclaration',         'VarTypeConstDeclaration',
        [ ( 0, 'var' ),
          ( 2, 'VarDeclarationItem',    'declarations' ) ] ],
    [ 'ConstDeclarationItem',   '',
        [ ( 1, 'ConstantName', 'constantName' ),
          ( 0, 'assign' ),
          ( 1, 'ConstantValue', 'value' ) ] ],
    [ 'ConstantValue',                  '',                 [] ],
    [ 'LiteralConstantValue',           'ConstantValue',
        [ ( 1, 'Literal', 'value' ) ] ],
    [ 'ConstantNameConstantValue',      'ConstantValue',
        [ ( 1, 'ConstantName', 'value' ) ] ],
    [ 'TypeDeclarationItem',            '',
        [ ( 1, 'TypeName', 'typeName' ),
          ( 1, 'FromProgramClause', 'fromProgram' ),
          ( 0, 'assign' ),
          ( 1, 'UserType', 'userType' ) ] ],
    [ 'VarDeclarationItem',             '',
        [ ( 2, 'VariableName', 'variables' ),
          ( 0, 'cmos_or_dram' ),
          ( 1, 'FromProgramClause', 'fromProgram' ),
          ( 0, 'colon' ),
          ( 1, 'DataType', 'type' ) ] ],
    [ 'FromProgramClause',              '',
        [ ( 0, 'from' ),
          ( 1, 'ProgramName', 'programName' ) ] ],
    [ 'UserType',                       '',                 [] ],
    [ 'DataUserType',                   'UserType',
        [ ( 1, 'DataType', 'dataType' ) ] ],
    [ 'StructureUserType',              'UserType',
        [ ( 0, 'structure' ),
          ( 2, 'FieldDefinition', 'fields' ),
          ( 0, 'endstructure' ) ] ],
    [ 'FieldDefinition',                '',
        [ ( 1, 'FieldName', 'fieldName' ),
          ( 0, 'colon' ),
          ( 1, 'StructureArrayType', 'type' ) ] ],
    [ 'StructureArrayType',             '',                 [] ],
    [ 'DataStructureArrayType',         'StructureArrayType',
        [ ( 1, 'DataType', 'type' ) ] ],
    [ 'ByteStructureArrayType',         'StructureArrayType',
        [ ( 0, 'byte' ) ] ],
    [ 'ShortStructureArrayType',        'StructureArrayType',
        [ ( 0, 'short' ) ] ],
    [ 'DataType',                       '',                 [] ],
    [ 'UserDataType',                   'DataType',
        [ ( 1, 'TypeName', 'typeName' ) ] ],
    [ 'SimpleDataType',                 'DataType',
        [ ( 0, 'int_real_bool' ) ] ],
    [ 'VectorDataType',                 'DataType',
        [ ( 0, 'vector' ) ] ],
    [ 'VisProcessDataType',             'DataType',
        [ ( 0, 'vis_process' ) ] ],
    [ 'ModelDataType',                  'DataType',
        [ ( 0, 'model' ) ] ],
    [ 'CamSetupDataType',               'DataType',
        [ ( 0, 'can_setup' ) ] ],
    [ 'FileDataType',                   'DataType',
        [ ( 0, 'file' ) ] ],
    [ 'ConfigDataType',                 'DataType',
        [ ( 0, 'config' ) ] ],
    [ 'PathDataType',                   'DataType',
        [ ( 0, 'path' ),
          ( 1, 'PathHeaderClause', 'pathHeader' ),
          ( 1, 'NodeDataClause', 'nodeData' ) ] ],
    [ 'PathHeaderClause',               '',
        [ ( 0, 'path' ),
          ( 0, 'header' ),
          ( 0, 'assign' ),
          ( 1, 'TypeName', 'typeName' ) ] ],
    [ 'NodeDataClause',                 '',
        [ ( 0, 'nodedata' ),
          ( 0, 'assign' ),
          ( 1, 'TypeName', 'typeName' ) ] ],
    [ 'StringDataType',                 'DataType',
        [ ( 0, 'string' ),
          ( 0, 'lbracket' ),
          ( 1, 'ArraySize', 'stringSize' ),
          ( 0, 'rbracket' ) ] ],
    [ 'ArrayDataType',                  'DataType',
        [ ( 0, 'array' ),
          ( 0, 'lbracket' ),
          ( 2, 'ArraySize', 'sizes' ),
          ( 0, 'rbracket' ),
          ( 0, 'of' ),
          ( 1, 'StructureArrayType', 'itemType' ) ] ],
    [ 'PositionDataType',               'DataType',
        [ ( 1, 'PositionType', 'positionType' ),
          ( 1, 'InGroupClause', 'inGroupClause' ) ] ],
    [ 'InGroupClause',                  '',
        [ ( 0, 'in' ),
          ( 0, 'group' ),
          ( 0, 'lbracket' ),
          ( 1, 'ArraySize', 'groupNum' ),
          ( 0, 'rbracket' ) ] ],
    [ 'PositionType',                   '',                 [] ],
    [ 'PositionPositionType',           'PositionType',
        [ ( 0, 'position' ) ] ],
    [ 'XYZWPRPositionType',             'PositionType',
        [ ( 0, 'xyzwpr' ) ] ],
    [ 'XYZWPREXTPositionType',          'PositionType',
        [ ( 0, 'xyzwprext' ) ] ],
    [ 'JointposPositionType',           'PositionType',
        [ ( 0, 'jointpos' ) ] ],
    [ 'Jointpos1PositionType',          'PositionType',
        [ ( 0, 'jointpos1' ) ] ],
    [ 'Jointpos2PositionType',          'PositionType',
        [ ( 0, 'jointpos2' ) ] ],
    [ 'Jointpos3PositionType',          'PositionType',
        [ ( 0, 'jointpos3' ) ] ],
    [ 'Jointpos4PositionType',          'PositionType',
        [ ( 0, 'jointpos4' ) ] ],
    [ 'Jointpos5PositionType',          'PositionType',
        [ ( 0, 'jointpos5' ) ] ],
    [ 'Jointpos6PositionType',          'PositionType',
        [ ( 0, 'jointpos6' ) ] ],
    [ 'Jointpos7PositionType',          'PositionType',
        [ ( 0, 'jointpos7' ) ] ],
    [ 'Jointpos8PositionType',          'PositionType',
        [ ( 0, 'jointpos8' ) ] ],
    [ 'Jointpos9PositionType',          'PositionType',
        [ ( 0, 'jointpos9' ) ] ],
    [ 'RoutineDeclaration',             '',
        [ ( 1, 'RoutineDefinition', 'definition' ),
          ( 1, 'RoutineImplemenation', 'implementation' ),
          ( 3, 'Scope', 'scope' ) ] ],
    [ 'RoutineImplemenation',           '',                 [] ],
    [ 'FromPrgRoutineImplemenation',    'RoutineImplemenation',
        [ ( 0, 'from' ),
          ( 1, 'ProgramName', 'programName' ) ] ],
    [ 'DirectRoutineImplemenation',     'RoutineImplemenation',
        [ ( 2, 'VarTypeConstDeclaration', 'declarations' ),
          ( 0, 'begin' ),
          ( 2, 'Statement', 'statements' ),
          ( 0, 'end' ),
          ( 1, 'RoutineName', 'routineName' ) ] ],
    [ 'RoutineDefinition',              '',
        [ ( 0, 'routine' ),
          ( 1, 'RoutineName', 'routineName' ),
          ( 1, 'RoutineParametersClause', 'parametersClause' ),
          ( 1, 'RoutineReturnTypeClause', 'returnType' ) ] ],
    [ 'RoutineParametersClause',        '',
        [ ( 0, 'lparen' ),
          ( 2, 'RoutineParameter', 'parameters' ),
          ( 0, 'rparen' ) ] ],
    [ 'RoutineReturnTypeClause',        '',
        [ ( 0, 'colon' ),
          ( 1, 'DataType', 'returnType' ) ] ],
    [ 'RoutineParameter',               '',
        [ ( 2, 'VariableName', 'vars' ),
          ( 0, 'colon' ),
          ( 1, 'DataType', 'type' ) ] ],
    [ 'Id',                             '',                 [] ],
    [ 'PrimaryId',                      'Id',
        [ ( 1, 'VariableName', 'variable' ) ] ],
    [ 'ArrayId',                        'Id',
        [ ( 1, 'Id', 'id' ),
          ( 0, 'lbracket' ),
          ( 2, 'VariableAccess', 'vars' ),
          ( 0, 'rbaracket' ) ] ],
    [ 'VariableField',                  '',
        [ ( 0, 'dot' ),
          ( 1, 'VariableAccess', 'field' ) ] ],
    [ 'VariableAccess',                 '',
        [ ( 1, 'Id', 'id' ),
          ( 1, 'VariableField', 'field' ) ] ],
    [ 'Statement',                      '',                 [] ],
    [ 'FakeVariableStatement',          'Statement',
        [ ( 1, 'VariableAccess', 'var' ) ] ],
    [ 'UsingStatement',                 'Statement',
        [ ( 0, 'using' ),
          ( 2, 'VariableAccess', 'structVars' ),
          ( 0, 'do' ),
          ( 2, 'Statement', 'statements' ),
          ( 0, 'endusing' ),
          ( 3, 'Scope', 'scope' ) ] ]
]

srcFileName  = work_dir + '/' + namespace + 'AST.cpp'
hdrFileName  = work_dir + '/' + namespace + 'AST.h'
fhdrFileName = work_dir + '/' + namespace + 'ASTfwd.h'
vhdrFileName = work_dir + '/' + namespace + 'ASTVisitor.h'

hdr_start0 = """#ifndef {0}AST_H
#define {0}AST_H

#include "MemoryPool.h"
#include "{1}ASTfwd.h"
#include "{1}Type.h"
#include "Control.h"

namespace Karel {{
""".format(namespace.upper(), namespace)

hdr_start1 = """
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

"""

hdr_mid = """
protected:
    virtual void accept0(ASTVisitor *visitor) = 0;
};

"""

hdr_end = """}}   // namespace {1}

#endif // {0}AST_H
""".format(namespace.upper(), namespace)

ast_source_end_content = """}}   // namespace {0}
""".format(namespace)

ast_fwd_end_content = """
}}   // namespace {0}

#endif // {1}ASTFWD_H
""".format(namespace, namespace.upper())

ast_visitor_end_content = """
private:
    TranslationUnit *_translationUnit;
}};

}}   // namespace {0}

#endif // {1}ASTVISITOR_H
""".format(namespace, namespace.upper())

ast_source_start_content = """#include "{0}AST.h"
#include "{0}ASTVisitor.h"

namespace {0} {{

AST::AST()
{{ }}

AST::~AST()
{{ }}

void AST::accept( ASTVisitor *visitor )
{{
    if ( visitor->preVisit(this) )
        accept0(visitor);
    visitor->postVisit(this);
}}

""".format(namespace)

ast_fwd_start_content = """#ifndef {1}ASTFWD_H
#define {1}ASTFWD_H

namespace {0} {{

template <typename _Tp> class List;

class AST;
class ASTVisitor;
""".format(namespace, namespace.upper())

ast_visitor_start_content = """#ifndef {1}ASTVISITOR_H
#define {1}ASTVISITOR_H

#include "{0}ASTfwd.h"
#include "TranslationUnit.h"

namespace {0} {{

class ASTVisitor
{{
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
    {{
        for (; it; it = it->next)
            accept(it->value);
    }}

    virtual bool preVisit(AST *) {{ return true; }}
    virtual void postVisit(AST *) {{}}

""".format(namespace, namespace.upper())

with open(srcFileName,  'w') as src, \
     open(hdrFileName,  'w') as hdr, \
     open(fhdrFileName, 'w') as fhdr, \
     open(vhdrFileName, 'w') as vhdr:
    hdr.write(hdr_start0)
    hdr.write(hdr_start1)
    src.write(ast_source_start_content)
    fhdr.write(ast_fwd_start_content)
    vhdr.write(ast_visitor_start_content)

    for cl in classes:
      className = cl[0]
      hdr.write('    virtual {0}AST *as{0}() {{ return 0; }}\n'.format(className))
      vhdr.write('    virtual bool visit({0}AST *) {{ return true; }}\n'.format(className))
      vhdr.write('    virtual void endVisit({0}AST *) {{}}\n'.format(className))

    hdr.write(hdr_mid)

    listed_classes = set()
    for cl in classes:
      className = cl[0]
      baseClassName = cl[1]
      mc = len(cl[2])
      isAbstract = mc == 0
      hdr.write('class {0}AST: public {1}AST\n{{\npublic:\n'.format(className, baseClassName))
      fhdr.write('class {0}AST;\n'.format(className))
      if isAbstract:
        hdr.write('    {0}AST() : {1}AST() {{}}\n'.format(className, baseClassName))
        hdr.write('    virtual {0}AST *as{0}() {{ return this; }}\n'.format(className))
      else:
        mdl = []
        md = ''
        for m in cl[2]:
          if m[0] == 0:
            md = 'unsigned t_{0}'.format(m[1])
          elif m[0] == 1:
            md = '{0}AST *{1}'.format(m[1], m[2])
          elif m[0] == 2:
            md = '{0}ListAST *{1}'.format(m[1], m[2])
            listed_classes.add(m[1])
          elif m[0] == 3:
            md = 'const {0} *{1}'.format(m[1], m[2])
          else:
            md = '/*!! WTF !!*/'
          hdr.write('    ' + md + ';\n')
          mdl.append(md)
        hdr.write('\npublic:\n')
        hdr.write('    {0}AST( '.format(className))
        mdc = 0
        while mdc < mc - 1:
          hdr.write(mdl[mdc] + '_ = 0, ')
          mdc += 1
        hdr.write(mdl[mdc] + '_ = 0 )\n')
        hdr.write('        : {0}AST()\n'.format(baseClassName))
        for m in cl[2]:
          if m[0] == 0:
            hdr.write('        , t_{0}(t_{0}_)\n'.format(m[1]))
          elif m[0] == 1:
            hdr.write('        , {0}({0}_)\n'.format(m[2]))
          else:
            hdr.write('        , {0}({0}_)\n'.format(m[2]))
        hdr.write('    {}\n\n')
        hdr.write('    virtual {0}AST *as{0}() {{ return this; }}\n\n'.format(className))
        hdr.write('    virtual unsigned firstToken() const;\n')
        hdr.write('    virtual unsigned lastToken() const;\n\n')
        hdr.write('protected:\n    virtual void accept0( ASTVisitor *visitor );\n')
        src.write('unsigned {0}AST::firstToken() const\n{{\n'.format(className))
        for m in cl[2]:
          if m[0] == 0:
            src.write('    if ( t_{0} )\n        return t_{0};\n'.format(m[1]))
          elif m[0] != 3:
            src.write('    if ( {0} )\n        return {0}->firstToken();\n'.format(m[2]))
        src.write('    return 0;\n')
        src.write('}\n\n')
        src.write('unsigned {0}AST::lastToken() const\n{{\n'.format(className))
        for m in reversed(cl[2]):
          if m[0] == 0:
            src.write('    if ( t_{0} )\n        return t_{0} + 1;\n'.format(m[1]))
          elif m[0] != 3:
            src.write('    if ( {0} )\n        return {0}->lastToken();\n'.format(m[2]))
        src.write('    return 1;\n')
        src.write('}\n\n')
        src.write('void {0}AST::accept0( ASTVisitor *visitor )\n{{\n    if ( visitor->visit(this) ) {{\n'.format(className))
        for m in cl[2]:
          if m[0] != 0 and m[0] != 3:
            src.write('        accept({0}, visitor);\n'.format(m[2]))
        src.write('    }\n    visitor->endVisit(this);\n}\n\n')
      hdr.write('};\n\n')

    hdr.write(hdr_end)

    src.write(ast_source_end_content)
    fhdr.write('\n')
    for l in listed_classes:
      fhdr.write('typedef List<{0}AST *> {0}ListAST;\n'.format(l))
    fhdr.write(ast_fwd_end_content)
    vhdr.write(ast_visitor_end_content)
