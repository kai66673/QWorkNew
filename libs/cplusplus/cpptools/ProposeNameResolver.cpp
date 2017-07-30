#include <QDebug>

#include "ProposeNameResolver.h"
#include "TranslationUnit.h"
#include "AST.h"
#include "ASTVisitor.h"
#include "Literals.h"

namespace CPlusPlus {

namespace {

class TypeNamesExtractorVisitor: protected ASTVisitor
{
public:
    struct TypeName {
        QString name;
        QStringList templateNames;
    };

    TypeNamesExtractorVisitor(TranslationUnit *unit)
        : ASTVisitor(unit)
        , m_typeName(0)
        , m_templateDepth(0)
    { }

    virtual ~TypeNamesExtractorVisitor() {
        if ( m_typeName )
            delete m_typeName;
    }

    const TypeName *operator ()(SimpleDeclarationAST *ast) {
        accept(ast);
        if ( m_typeName && !m_typeName->templateNames.isEmpty() )
            m_typeName->templateNames.removeFirst();
        return m_typeName;
    }

    void dump() {
        qDebug() << "| " << m_typeName->name;
        for (int i = 0; i < m_typeName->templateNames.size(); i++)
            qDebug() << "|   " << m_typeName->templateNames.at(i);
    }

    virtual bool visit(NamedTypeSpecifierAST *ast) {
        if ( !m_typeName ) {
            TypeName *nt = new TypeName();
            nt->name = ast->name->name->identifier()->chars();
            m_typeName = nt;
        } else {
            m_typeName->templateNames.append(ast->name->name->identifier()->chars());
        }
        return true;
    }

    virtual bool visit(TemplateIdAST *ast) {
        m_templateDepth++;
        if ( m_templateDepth > 1 || !m_typeName )
            return false;
        m_typeName->templateNames.append(ast->name->identifier()->chars());
        return true;
    }

private:
    TypeName *m_typeName;
    int m_templateDepth;
};

}

ProposeNameResolver::ProposeNameResolver(const QString &typeText)
    : m_typeText(typeText)
{ }

ProposeNameResolver::~ProposeNameResolver()
{

}

QStringList ProposeNameResolver::operator ()(bool inClass, bool *isTypedef)
{
    QStringList result;

    *isTypedef = false;
    if ( m_typeText.startsWith("typedef", Qt::CaseSensitive) && m_typeText.length() > 7 && m_typeText.at(7).isSpace() )
        *isTypedef = true;

    m_document = Document::create(QLatin1String("<name_resolver>"));
    m_document->setSource(m_typeText.toUtf8() + " a;");

    if ( m_document->parse(Document::ParseDeclaration) ) {
        m_document->check();

        if ( AST *ast = m_document->translationUnit()->ast() ) {
            if ( SimpleDeclarationAST *declarationAST = ast->asSimpleDeclaration() ) {
                TypeNamesExtractorVisitor t(m_document->translationUnit());
                if ( const TypeNamesExtractorVisitor::TypeName *tn = t(declarationAST) ) {
                    QString typeName = tn->name;
                    if ( !tn->templateNames.isEmpty() ) {
                        QString typeSuffix = "";
                        QString typeCore = "";
                        if ( typeName.endsWith("list", Qt::CaseInsensitive) ) {
                            typeSuffix = "List";
                            typeCore = tn->templateNames.first();
                        } else if ( typeName.endsWith("map", Qt::CaseInsensitive) ) {
                            typeSuffix = "Map";
                            if ( tn->templateNames.size() > 1 )
                                typeCore = tn->templateNames[0] + "To" + tn->templateNames[1];
                            else
                                typeCore = tn->templateNames.first();
                        } else if ( typeName.endsWith("set", Qt::CaseInsensitive) ) {
                            typeSuffix = "Set";
                            typeCore = tn->templateNames.first();
                        }
                        if ( !typeSuffix.isEmpty() )
                            typeName = typeCore + typeSuffix;
                    }
                    if ( *isTypedef ) {
                        result << typeName << (typeName + "Type");
                    } else {
                        typeName[0] = typeName[0].toLower();
                        if ( inClass ) {
                            result << ("m_" + typeName)
                                   << typeName
                                   << (typeName + "_");
                        } else {
                            result << typeName;
                        }
                    }
                }
            }
        }
    }

    return result;
}

Document::Ptr ProposeNameResolver::document()
{
    return m_document;
}

}   // namespace CPlusPlus
