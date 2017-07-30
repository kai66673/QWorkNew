#ifndef KARELTYPE_H
#define KARELTYPE_H

#include <QList>
#include <QStringList>

#include "KarelToken.h"

namespace Karel {

class Control;
class Type;

class Symbol
{
public:
    Symbol(const Identifier *name, unsigned tokenIndex);

    unsigned tokenIndex() const;
    const Identifier *name() const;

protected:
    const Identifier *m_name;
    unsigned m_tokenIndex;
};

class Declaration: public Symbol
{
public:
    Declaration(const Identifier *name, unsigned tokenIndex, const Type *type);

    const Type *type() const;

private:
    const Type *m_type;
};

class Type
{
public:
    enum Kind {
        Named = 0,
        Null,
        Array,
        Structure,
        BuiltingByte,
        BuiltingShort,
        BuiltingInteger,
        BuiltingReal,
        BuiltingBoolean,
        BuiltingString,
        BuiltingPosition,
        BuiltingConfig,
        BuiltingXYZWPR,
        BuiltingXYZWPREXT,
        BuiltingJointPos,
        BuiltingFile,
        BuiltingCamSetup,
        BuiltingModel,
        BuiltingPath,
        BuiltingVector,
        BuiltingVisProcess
    };

    virtual ~Type();

    virtual Kind kind() const = 0;
    virtual const Identifier *name() const = 0;

    virtual const Type* arrayItemtype() const = 0;
    virtual const Type *fieldType(const Identifier *fieldName) const = 0;
    virtual QStringList fields() const = 0;
    virtual bool hasFields() const = 0;
    virtual const Declaration *fieldDeclaration(const Identifier *fieldName) const = 0;

    virtual const Type *binded() const { return 0; }
};

class NamedType: public Type
{
public:
    NamedType(const Identifier *name);

    virtual Kind kind() const { return Named; }
    virtual const Identifier *name() const { return m_name; }

    virtual const Type* arrayItemtype() const { return m_binded ? m_binded->arrayItemtype() : 0; }
    virtual const Type *fieldType(const Identifier *fieldName) const { return m_binded ? m_binded->fieldType(fieldName) : 0; }
    virtual QStringList fields() const { return m_binded ? m_binded->fields() : QStringList(); }
    virtual bool hasFields() const { return m_binded ? m_binded->hasFields() : false; }
    virtual const Declaration *fieldDeclaration(const Identifier *fieldName) const { return m_binded ? m_binded->fieldDeclaration(fieldName) : 0; }

    void bind(const Type *type);
    virtual const Type *binded() const;

private:
    const Identifier *m_name;
    const Type *m_binded;
};

class ArrayType: public Type
{
public:
    ArrayType(const Type *itemType) : m_itemType(itemType) {}

    virtual Kind kind() const { return Array; }
    virtual const Identifier *name() const { return 0; }

    virtual const Type* arrayItemtype() const { return m_itemType; }
    virtual const Type *fieldType(const Identifier *) const { return 0; }
    virtual QStringList fields() const { return QStringList(); }
    virtual bool hasFields() const { return false; }
    virtual const Declaration *fieldDeclaration(const Identifier *) const { return 0; }

private:
    const Type *m_itemType;
};

class StructureType: public Type
{
public:
    virtual ~StructureType();

    void addField(Declaration *declaration);

    virtual Kind kind() const { return Structure; }
    virtual const Identifier *name() const { return 0; }

    virtual const Type* arrayItemtype() const { return 0; }
    virtual const Type *fieldType(const Identifier *fieldName) const;
    virtual QStringList fields() const;
    virtual bool hasFields() const { return !m_declarations.isEmpty(); }
    virtual const Declaration *fieldDeclaration(const Identifier *fieldName) const;

private:
    QList<Declaration *> m_declarations;
};

class BasePrimitiveType: public Type
{
public:
    BasePrimitiveType(Kind kind, const Identifier *name);
    virtual ~BasePrimitiveType();

    virtual Kind kind() const { return m_kind; }
    virtual const Identifier *name() const { return m_name; }

    virtual const Type* arrayItemtype() const { return 0; }
    virtual const Type *fieldType(const Identifier *) const { return 0; }
    virtual QStringList fields() const { return QStringList(); }
    virtual bool hasFields() const { return false; }
    virtual const Declaration *fieldDeclaration(const Identifier *) const { return 0; }

private:
    Kind m_kind;
    const Identifier *m_name;
};

class BaseStructureType: public Type
{
public:
    BaseStructureType(Kind kind, const Identifier *name);
    virtual ~BaseStructureType();

    void addField(Declaration *declaration);

    virtual Kind kind() const { return m_kind; }
    virtual const Identifier *name() const { return m_name; }

    virtual const Type* arrayItemtype() const { return 0; }
    virtual const Type *fieldType(const Identifier *) const;
    virtual QStringList fields() const;
    virtual bool hasFields() const { return true; }
    virtual const Declaration *fieldDeclaration(const Identifier *fieldName) const;

private:
    Kind m_kind;
    const Identifier *m_name;
    QList<Declaration *> m_declarations;
};

class NullType: public BasePrimitiveType
{
public:
    NullType() : BasePrimitiveType(Null, new Identifier("<NULL>", 6)) {}
    virtual const Type* arrayItemtype() const { return this; }
};

class ByteType: public BasePrimitiveType
{
public:
    ByteType() : BasePrimitiveType(BuiltingByte, new Identifier("BYTE", 4)) {}
};

class ShortType: public BasePrimitiveType
{
public:
    ShortType() : BasePrimitiveType(BuiltingShort, new Identifier("SHORT", 5)) {}
};

class IntegerType: public BasePrimitiveType
{
public:
    IntegerType() : BasePrimitiveType(BuiltingInteger, new Identifier("INTEGER", 7)) {}
};

class RealType: public BasePrimitiveType
{
public:
    RealType() : BasePrimitiveType(BuiltingReal, new Identifier("REAL", 4)) {}
};

class BooleanType: public BasePrimitiveType
{
public:
    BooleanType() : BasePrimitiveType(BuiltingBoolean, new Identifier("BOOLEAN", 7)) {}
};

class StringType: public BasePrimitiveType
{
public:
    StringType() : BasePrimitiveType(BuiltingString, new Identifier("STRING", 6)) {}
};

class FileType: public BasePrimitiveType
{
public:
    FileType() : BasePrimitiveType(BuiltingFile, new Identifier("FILE", 4)) {}
};

class CamSetupType: public BasePrimitiveType
{
public:
    CamSetupType() : BasePrimitiveType(BuiltingCamSetup, new Identifier("CAM_SETUP", 9)) {}
};

class ModelType: public BasePrimitiveType
{
public:
    ModelType() : BasePrimitiveType(BuiltingModel, new Identifier("MODEL", 5)) {}
};

class PathType: public BasePrimitiveType
{
public:
    PathType() : BasePrimitiveType(BuiltingPath, new Identifier("PATH", 4)) {}
};

class VisProcessType: public BasePrimitiveType
{
public:
    VisProcessType() : BasePrimitiveType(BuiltingVisProcess, new Identifier("VIS_PROCESS", 11)) {}
};

class JointPosType: public BasePrimitiveType
{
public:
    JointPosType() : BasePrimitiveType(BuiltingJointPos, new Identifier("JOINTPOS", 8)) {}
};

// Structured builting types
class PositionType: public BaseStructureType
{
public:
    PositionType() : BaseStructureType(BuiltingPosition, new Identifier("POSITION", 8)) {}
};

class ConfigType: public BaseStructureType
{
public:
    ConfigType() : BaseStructureType(BuiltingConfig, new Identifier("CONFIG", 6)) {}
};

class XYZWPRType: public BaseStructureType
{
public:
    XYZWPRType() : BaseStructureType(BuiltingXYZWPR, new Identifier("XYZWPR", 6)) {}
};

class XYZWPREXTType: public BaseStructureType
{
public:
    XYZWPREXTType() : BaseStructureType(BuiltingXYZWPREXT, new Identifier("XYZWPREXT", 9)) {}
};

class VectorType: public BaseStructureType
{
public:
    VectorType() : BaseStructureType(BuiltingVector, new Identifier("VECTOR", 6)) {}
};

}   // namespace Karel

#endif // KARELTYPE_H
