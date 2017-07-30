#include "KarelType.h"
#include "KarelIdentifierConstants.h"

namespace Karel {

Symbol::Symbol(const Identifier *name, unsigned tokenIndex)
    : m_name(name)
    , m_tokenIndex(tokenIndex)
{ }

unsigned Symbol::tokenIndex() const
{ return m_tokenIndex; }

Declaration::Declaration(const Identifier *name, unsigned tokenIndex, const Type *type)
    : Symbol(name, tokenIndex)
    , m_type(type)
{ }

const Type *Declaration::type() const
{ return m_type; }

const Identifier *Symbol::name() const
{ return m_name; }

Type::~Type()
{ }

NamedType::NamedType(const Identifier *name)
    : m_name(name)
    , m_binded(0)
{ }

void NamedType::bind(const Type *type)
{ m_binded = type; }

const Type *NamedType::binded() const
{ return m_binded; }

StructureType::~StructureType()
{ qDeleteAll(m_declarations); }

void StructureType::addField(Declaration *declaration)
{ m_declarations.append(declaration); }

const Type *StructureType::fieldType(const Identifier *fieldName) const
{
    foreach (Declaration *declaration, m_declarations) {
        if ( declaration->name()->equalTo(fieldName) )
            return declaration->type();
    }

    return 0;
}

QStringList StructureType::fields() const
{
    QStringList result;
    foreach (Declaration *declaration, m_declarations)
        if ( const Identifier *name = declaration->name() )
            result << QString(name->charsOrig());
    return result;
}

const Declaration *StructureType::fieldDeclaration(const Identifier *fieldName) const
{
    foreach (Declaration *declaration, m_declarations) {
        if ( declaration->name()->equalTo(fieldName) )
            return declaration;
    }

    return 0;
}

BasePrimitiveType::BasePrimitiveType(Type::Kind kind, const Identifier *name)
    : m_kind(kind)
    , m_name(name)
{ }

BasePrimitiveType::~BasePrimitiveType()
{ delete m_name; }

BaseStructureType::BaseStructureType(Type::Kind kind, const Identifier *name)
    : m_kind(kind)
    , m_name(name)
{ }

BaseStructureType::~BaseStructureType()
{
    delete m_name;
    qDeleteAll(m_declarations);
}

void BaseStructureType::addField(Declaration *declaration)
{ m_declarations.append(declaration); }

const Type *BaseStructureType::fieldType(const Identifier *fieldName) const
{
    foreach (Declaration *declaration, m_declarations)
        if ( declaration->name()->equalTo(fieldName) )
            return declaration->type();

    return 0;
}

QStringList BaseStructureType::fields() const
{
    QStringList result;
    foreach (Declaration *declaration, m_declarations)
        if ( const Identifier *name = declaration->name() )
            result << QString(name->charsOrig());
    return result;
}

const Declaration *BaseStructureType::fieldDeclaration(const Identifier *fieldName) const
{
    foreach (Declaration *declaration, m_declarations)
        if ( declaration->name()->equalTo(fieldName) )
            return declaration;

    return 0;
}

}   // namespace Karel
