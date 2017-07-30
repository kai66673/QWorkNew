/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "cppinsertdecldef.h"
#include "cppquickfixassistant.h"

#include <CPlusPlus.h>
#include <cplusplus/ASTPath.h>
#include <cplusplus/CppRewriter.h>
#include <cplusplus/LookupContext.h>
#include <cplusplus/Overview.h>
#include <cpptools/insertionpointlocator.h>
#include <cpptools/cpprefactoringchanges.h>
#include <qtcassert.h>
#include <CppToolsUtils.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

using namespace CPlusPlus;
using namespace CppEditor;
using namespace CppEditor::Internal;
using namespace CppTools;

namespace {

class InsertDeclOperation: public CppQuickFixOperation
{
public:
    InsertDeclOperation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_,
                        int priority,
                        const QString &targetFileName, const Class *targetSymbol,
                        InsertionPointLocator::AccessSpec xsSpec,
                        const QString &decl)
        : CppQuickFixOperation(interface_, priority)
        , m_targetFileName(targetFileName)
        , m_targetSymbol(targetSymbol)
        , m_xsSpec(xsSpec)
        , m_decl(decl)
    {
        QString type;
        switch (xsSpec) {
        case InsertionPointLocator::Public: type = QLatin1String("public"); break;
        case InsertionPointLocator::Protected: type = QLatin1String("protected"); break;
        case InsertionPointLocator::Private: type = QLatin1String("private"); break;
        case InsertionPointLocator::PublicSlot: type = QLatin1String("public slot"); break;
        case InsertionPointLocator::ProtectedSlot: type = QLatin1String("protected slot"); break;
        case InsertionPointLocator::PrivateSlot: type = QLatin1String("private slot"); break;
        default: break;
        }

        setDescription(QCoreApplication::translate("CppEditor::InsertDeclOperation",
                                                   "Add %1 Declaration").arg(type));
    }

    void performChanges(const CppRefactoringFilePtr &,
                        const CppRefactoringChanges &refactoring)
    {
        InsertionPointLocator locator(refactoring);
        const InsertionLocation loc = locator.methodDeclarationInClass(
                    m_targetFileName, m_targetSymbol, m_xsSpec);
        Q_ASSERT(loc.isValid());

        CppRefactoringFilePtr targetFile = refactoring.file(m_targetFileName);
        int targetPosition1 = targetFile->position(loc.line(), loc.column());
        int targetPosition2 = qMax(0, targetFile->position(loc.line(), 1) - 1);

        Utils::ChangeSet target;
        target.insert(targetPosition1, loc.prefix() + m_decl);
        targetFile->setChangeSet(target);
        targetFile->appendIndentRange(Utils::ChangeSet::Range(targetPosition2, targetPosition1));
        targetFile->setOpenEditor(true, targetPosition1);
        targetFile->apply();
    }

private:
    QString m_targetFileName;
    const Class *m_targetSymbol;
    InsertionPointLocator::AccessSpec m_xsSpec;
    QString m_decl;
};

} // anonymous namespace

QList<CppQuickFixOperation::Ptr> DeclFromDef::match(
    const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
{
    const QList<AST *> &path = interface_->path();
    CppRefactoringFilePtr file = interface_->currentFile();

    FunctionDefinitionAST *funDef = 0;
    int idx = 0;
    for (; idx < path.size(); ++idx) {
        AST *node = path.at(idx);
        if (idx > 1) {
            if (DeclaratorIdAST *declId = node->asDeclaratorId()) {
                if (file->isCursorOn(declId)) {
                    if (FunctionDefinitionAST *candidate = path.at(idx - 2)->asFunctionDefinition()) {
                        if (funDef) {
                            return noResult();
                        } else {
                            funDef = candidate;
                            break;
                        }
                    }
                }
            }
        }

        if (node->asClassSpecifier()) {
            return noResult();
        }
    }

    if (!funDef || !funDef->symbol)
        return noResult();

    Function *method = funDef->symbol;

    Scope *enclosingScope = method->enclosingScope();
    while (! (enclosingScope->isNamespace() || enclosingScope->isClass()))
        enclosingScope = enclosingScope->enclosingScope();
    Q_ASSERT(enclosingScope != 0);

    const Name *functionName = method->name();
    if (! functionName)
        return noResult(); // warn, anonymous function names are not valid c++

    if (! functionName->isQualifiedNameId())
        return noResult(); // warn, trying to add a declaration for a global function

    const QualifiedNameId *q = functionName->asQualifiedNameId();
    if (!q->base())
        return noResult();

    if (ClassOrNamespace *binding = interface_->context().lookupType(q->base(), enclosingScope)) {
        foreach (Symbol *s, binding->symbols()) {
            if (Class *matchingClass = s->asClass()) {
                for (Symbol *s = matchingClass->find(q->identifier()); s; s = s->next()) {
                    if (! s->name())
                        continue;
                    else if (! q->identifier()->isEqualTo(s->identifier()))
                        continue;
                    else if (! s->type()->isFunctionType())
                        continue;

                    if (s->type().isEqualTo(method->type()))
                        return noResult();
                }

                // a good candidate

                const QString fn = QString::fromUtf8(matchingClass->fileName(),
                                                     matchingClass->fileNameLength());
                const QString decl = generateDeclaration(interface_,
                                                         method,
                                                         binding);
                return singleResult(
                            new InsertDeclOperation(interface_, idx, fn, matchingClass,
                                                    InsertionPointLocator::Public,
                                                    decl));
            }
        }
    }

    return noResult();
}

QString DeclFromDef::generateDeclaration(const QSharedPointer<const CppQuickFixAssistInterface> &,
                                         Function *method,
                                         ClassOrNamespace *targetBinding)
{
    Q_UNUSED(targetBinding);

    Overview oo;
    oo.setShowFunctionSignatures(true);
    oo.setShowReturnTypes(true);
    oo.setShowArgumentNames(true);

    QString decl;
    decl += oo(method->type(), method->unqualifiedName());
    decl += QLatin1String(";\n");

    return decl;
}

namespace {

class InsertDefOperation: public CppQuickFixOperation
{
public:
    InsertDefOperation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority,
                       Declaration *decl, const InsertionLocation &loc)
        : CppQuickFixOperation(interface_, priority)
        , m_decl(decl)
        , m_loc(loc)
    {
        const QString declFile = QString::fromUtf8(decl->fileName(), decl->fileNameLength());
        const QDir dir = QFileInfo(declFile).dir();
        setDescription(QCoreApplication::translate("CppEditor::InsertDefOperation",
                                                   "Add Definition in %1")
                       .arg(dir.relativeFilePath(m_loc.fileName())));
    }

    void performChanges(const CppRefactoringFilePtr &,
                        const CppRefactoringChanges &refactoring)
    {
        Q_ASSERT(m_loc.isValid());

        CppRefactoringFilePtr targetFile = refactoring.file(m_loc.fileName());

        Overview oo;
        oo.setShowFunctionSignatures(true);
        oo.setShowReturnTypes(true);
        oo.setShowArgumentNames(true);

        // make target lookup context
        Document::Ptr targetDoc = targetFile->cppDocument();
        Scope *targetScope = targetDoc->scopeAt(m_loc.line(), m_loc.column());
        LookupContext targetContext(targetDoc, assistInterface()->snapshot());
        ClassOrNamespace *targetCoN = targetContext.lookupType(targetScope);
        if (!targetCoN)
            targetCoN = targetContext.globalNamespace();

        // setup rewriting to get minimally qualified names
        SubstitutionEnvironment env;
        env.setContext(assistInterface()->context());
        env.switchScope(m_decl->enclosingScope());
        UseMinimalNames q(targetCoN);
        env.enter(&q);
        Control *control = assistInterface()->context().control().data();

        // rewrite the function type
        FullySpecifiedType tn = rewriteType(m_decl->type(), &env, control);

        // rewrite the function name
        QString name = oo(LookupContext::minimalName(m_decl, targetCoN, control));

        QString defText = oo.prettyType(tn, name) + "\n{\n}";

        int targetPos = targetFile->position(m_loc.line(), m_loc.column());
        int targetPos2 = qMax(0, targetFile->position(m_loc.line(), 1) - 1);

        Utils::ChangeSet target;
        target.insert(targetPos,  m_loc.prefix() + defText + m_loc.suffix());
        targetFile->setChangeSet(target);
        targetFile->appendIndentRange(Utils::ChangeSet::Range(targetPos2, targetPos));
        targetFile->setOpenEditor(true, targetPos);
        targetFile->apply();
    }

private:
    Declaration *m_decl;
    InsertionLocation m_loc;
};

} // anonymous namespace

QList<CppQuickFixOperation::Ptr> DefFromDecl::match(
    const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
{
    const QList<AST *> &path = interface_->path();

    int idx = path.size() - 1;
    for (; idx >= 0; --idx) {
        AST *node = path.at(idx);
        if (SimpleDeclarationAST *simpleDecl = node->asSimpleDeclaration()) {
            if (simpleDecl->symbols && ! simpleDecl->symbols->next) {
                if (Symbol *symbol = simpleDecl->symbols->value) {
                    if (Declaration *decl = symbol->asDeclaration()) {
                        if (decl->type()->isFunctionType()
                                && !decl->type()->asFunctionType()->isPureVirtual()
                                && decl->enclosingScope()
                                && decl->enclosingScope()->isClass()) {
                            CppRefactoringChanges refactoring(interface_->snapshot());
                            InsertionPointLocator locator(refactoring);
                            QList<CppQuickFixOperation::Ptr> results;
                            foreach (const InsertionLocation &loc, locator.methodDefinition(decl)) {
                                if (loc.isValid())
                                    results.append(CppQuickFixOperation::Ptr(new InsertDefOperation(interface_, idx, decl, loc)));
                            }
                            return results;
                        }
                    }
                }
            }

            break;
        }
    }

    return noResult();
}

namespace {

QString memberBaseName(const QString &name)
{
    QString baseName = name;

    // Remove leading and trailing "_"
    while (baseName.startsWith(QLatin1Char('_')))
        baseName.remove(0, 1);
    while (baseName.endsWith(QLatin1Char('_')))
        baseName.chop(1);
    if (baseName != name)
        return baseName;

    // If no leading/trailing "_": remove "m_" and "m" prefix
    if (baseName.startsWith(QLatin1String("m_"))) {
        baseName.remove(0, 2);
    } else if (baseName.startsWith(QLatin1Char('m')) && baseName.length() > 1
               && baseName.at(1).isUpper()) {
        baseName.remove(0, 1);
        baseName[0] = baseName.at(0).toLower();
    }

    return baseName;
}

bool hasClassMemberWithGetPrefix(const Class *klass)
{
    if (!klass)
        return false;

    for (unsigned i = 0; i < klass->memberCount(); ++i) {
        const Symbol *symbol = klass->memberAt(i);
        if (symbol->isFunction() || symbol->isDeclaration()) {
            if (const Name *symbolName = symbol->name()) {
                if (const Identifier *id = symbolName->identifier()) {
                    if (!strncmp(id->chars(), "get", 3))
                        return true;
                }
            }
        }
    }
    return false;
}

InsertionLocation insertLocationForMethodDefinition(Symbol *symbol, const bool useSymbolFinder,
                                                    const CppRefactoringChanges& refactoring,
                                                    const QString& fileName)
{
    QTC_ASSERT(symbol, return InsertionLocation());

    // Try to find optimal location
    const InsertionPointLocator locator(refactoring);
    const QList<InsertionLocation> list
            = locator.methodDefinition(symbol, useSymbolFinder, fileName);
    for (int i = 0; i < list.count(); ++i) {
        InsertionLocation location = list.at(i);
        if (location.isValid() && location.fileName() == fileName)
            return location;
    }

    // ...failed,
    // if class member try to get position right after class
    CppRefactoringFilePtr file = refactoring.file(fileName);
    unsigned line = 0, column = 0;
    if (Class *clazz = symbol->enclosingClass()) {
        if (symbol->fileName() == fileName.toUtf8()) {
            file->cppDocument()->translationUnit()->getPosition(clazz->endOffset(), &line, &column);
            if (line != 0) {
                ++column; // Skipping the ";"
                return InsertionLocation(fileName, QLatin1String("\n\n"), QLatin1String(""),
                                         line, column);
            }
        }
    }

    // fall through: position at end of file
    const QTextDocument *doc = file->document();
    int pos = qMax(0, doc->characterCount() - 1);

    //TODO watch for matching namespace
    //TODO watch for moc-includes

    file->lineAndColumn(pos, &line, &column);
    return InsertionLocation(fileName, QLatin1String("\n\n"), QLatin1String("\n"), line, column);
}

class GenerateGetterSetterOperation : public CppQuickFixOperation
{
public:
    enum OperationType {
        InvalidType,
        GetterSetterType,
        GetterType,
        SetterType
    };

    GenerateGetterSetterOperation(const QSharedPointer<const CppQuickFixAssistInterface> &interface)
        : CppQuickFixOperation(interface)
        , m_type(InvalidType)
        , m_variableName(0)
        , m_declaratorId(0)
        , m_declarator(0)
        , m_variableDecl(0)
        , m_classSpecifier(0)
        , m_classDecl(0)
        , m_symbol(0)
        , m_offerQuickFix(true)
    {
        const QList<AST *> &path = interface->path();
        // We expect something like
        // [0] TranslationUnitAST
        // [1] NamespaceAST
        // [2] LinkageBodyAST
        // [3] SimpleDeclarationAST
        // [4] ClassSpecifierAST
        // [5] SimpleDeclarationAST
        // [6] DeclaratorAST
        // [7] DeclaratorIdAST
        // [8] SimpleNameAST

        const int n = path.size();
        if (n < 6)
            return;

        int i = 1;
        m_variableName = path.at(n - i++)->asSimpleName();
        m_declaratorId = path.at(n - i++)->asDeclaratorId();
        // DeclaratorAST might be preceded by PointerAST, e.g. for the case
        // "class C { char *@s; };", where '@' denotes the text cursor position.
        if (!(m_declarator = path.at(n - i++)->asDeclarator())) {
            --i;
            if (path.at(n - i++)->asPointer()) {
                if (n < 7)
                    return;
                m_declarator = path.at(n - i++)->asDeclarator();
            }
        }
        m_variableDecl = path.at(n - i++)->asSimpleDeclaration();
        m_classSpecifier = path.at(n - i++)->asClassSpecifier();
        m_classDecl = path.at(n - i++)->asSimpleDeclaration();

        if (!isValid())
            return;

        // Do not get triggered on member functions and arrays
        if (m_declarator->postfix_declarator_list) {
            m_offerQuickFix = false;
            return;
        }

        // Construct getter and setter names
        const Name *variableName = m_variableName->name;
        if (!variableName) {
            m_offerQuickFix = false;
            return;
        }
        const Identifier *variableId = variableName->identifier();
        if (!variableId) {
            m_offerQuickFix = false;
            return;
        }
        m_variableString = QString::fromUtf8(variableId->chars(), variableId->size());

        m_baseName = memberBaseName(m_variableString);
        if (m_baseName.isEmpty())
            m_baseName = QLatin1String("value");

        m_getterName = !(m_baseName == m_variableString
                         || hasClassMemberWithGetPrefix(m_classSpecifier->symbol))
            ? m_baseName
            : QString::fromLatin1("get%1%2")
                .arg(m_baseName.left(1).toUpper()).arg(m_baseName.mid(1));
        m_setterName = QString::fromLatin1("set%1%2")
            .arg(m_baseName.left(1).toUpper()).arg(m_baseName.mid(1));

        // Check if the class has already a getter and/or a setter.
        // This is only a simple check which should suffice not triggering the
        // same quick fix again. Limitations:
        //   1) It only checks in the current class, but not in base classes.
        //   2) It compares only names instead of types/signatures.
        //   3) Symbols in Qt property declarations are ignored.
        bool hasGetter = false;
        bool hasSetter = false;
        if (Class *klass = m_classSpecifier->symbol) {
            for (unsigned i = 0; i < klass->memberCount(); ++i) {
                Symbol *symbol = klass->memberAt(i);
                if (symbol->isQtPropertyDeclaration())
                    continue;
                if (const Name *symbolName = symbol->name()) {
                    if (const Identifier *id = symbolName->identifier()) {
                        const QString memberName = QString::fromUtf8(id->chars(), id->size());
                        if (memberName == m_getterName)
                            hasGetter = true;
                        if (memberName == m_setterName)
                            hasSetter = true;
                        if (hasGetter && hasSetter)
                            break;
                    }
                }
            } // for
        }
        if (hasGetter && hasSetter) {
            m_offerQuickFix = false;
            return;
        }

        // Find the right symbol in the simple declaration
        const List<Symbol *> *symbols = m_variableDecl->symbols;
        QTC_ASSERT(symbols, return);
        for (; symbols; symbols = symbols->next) {
            Symbol *s = symbols->value;
            if (const Name *name = s->name()) {
                if (const Identifier *id = name->identifier()) {
                    const QString symbolName = QString::fromUtf8(id->chars(), id->size());
                    if (symbolName == m_variableString) {
                        m_symbol = s;
                        break;
                    }
                }
            }
        }

        QTC_ASSERT(m_symbol, return);
        if (hasSetter) { // hasGetter is false in this case
            m_type = GetterType;
        } else { // no setter
            if (hasGetter) {
                if (m_symbol->type().isConst()) {
                    m_offerQuickFix = false;
                    return;
                } else {
                    m_type = SetterType;
                }
            } else {
                m_type = (m_symbol->type().isConst()) ? GetterType : GetterSetterType;
            }
        }
        updateDescriptionAndPriority();
    }

    // Clones "other" in order to prevent all the initial detection made in the ctor.
    GenerateGetterSetterOperation(const QSharedPointer<const CppQuickFixAssistInterface> &interface,
                                  GenerateGetterSetterOperation *other, OperationType type)
        : CppQuickFixOperation(interface)
        , m_type(type)
        , m_variableName(other->m_variableName)
        , m_declaratorId(other->m_declaratorId)
        , m_declarator(other->m_declarator)
        , m_variableDecl(other->m_variableDecl)
        , m_classSpecifier(other->m_classSpecifier)
        , m_classDecl(other->m_classDecl)
        , m_symbol(other->m_symbol)
        , m_baseName(other->m_baseName)
        , m_getterName(other->m_getterName)
        , m_setterName(other->m_setterName)
        , m_variableString(other->m_variableString)
        , m_offerQuickFix(other->m_offerQuickFix)
    {
        QTC_ASSERT(isValid(), return);
        updateDescriptionAndPriority();
    }

    bool generateGetter() const
    {
        return (m_type == GetterSetterType || m_type == GetterType);
    }

    bool generateSetter() const
    {
        return (m_type == GetterSetterType || m_type == SetterType);
    }

    void updateDescriptionAndPriority()
    {
        switch (m_type) {
        case GetterSetterType:
            setPriority(5);
            setDescription(QCoreApplication::translate("CppEditor::InsertDeclOperation",
                                                       "Create Getter and Setter Member Functions"));
            break;
        case GetterType:
            setPriority(4);
            setDescription(QCoreApplication::translate("CppEditor::InsertDeclOperation",
                                                       "Create Getter Member Function"));
            break;
        case SetterType:
            setPriority(3);
            setDescription(QCoreApplication::translate("CppEditor::InsertDeclOperation",
                                                       "Create Setter Member Function"));
            break;
        default:
            break;
        }
    }

    bool isValid() const
    {
        return m_variableName
            && m_declaratorId
            && m_declarator
            && m_variableDecl
            && m_classSpecifier
            && m_classDecl
            && m_offerQuickFix;
    }

    virtual void performChanges(const CppTools::CppRefactoringFilePtr &currentFile,
                                const CppTools::CppRefactoringChanges &refactoring)

    {
        QTC_ASSERT(m_symbol, return);
        FullySpecifiedType fullySpecifiedType = m_symbol->type();
        Type *type = fullySpecifiedType.type();
        QTC_ASSERT(type, return);
        Overview oo;
        oo.setShowFunctionSignatures(true);
        oo.setShowReturnTypes(true);
        oo.setShowArgumentNames(true);
        const QString typeString = oo.prettyType(fullySpecifiedType);

        const NameAST *classNameAST = m_classSpecifier->name;
        QTC_ASSERT(classNameAST, return);
        const Name *className = classNameAST->name;
        QTC_ASSERT(className, return);
        const Identifier *classId = className->identifier();
        QTC_ASSERT(classId, return);
        QString classString = QString::fromUtf8(classId->chars(), classId->size());

        bool wasHeader = true;
        QString declFileName = currentFile->fileName();
        QString implFileName = correspondingHeaderOrSource(declFileName, &wasHeader);
        const bool sameFile = !wasHeader || !QFile::exists(implFileName);
        if (sameFile)
            implFileName = declFileName;

        InsertionPointLocator locator(refactoring);
        InsertionLocation declLocation = locator.methodDeclarationInClass
            (declFileName, m_classSpecifier->symbol->asClass(), InsertionPointLocator::Public);

        const bool passByValue = type->isIntegerType() || type->isFloatType()
                || type->isPointerType() || type->isEnumType();
        const QString paramName = m_baseName != m_variableString
            ? m_baseName : QLatin1String("value");
        QString paramString;
        if (passByValue) {
            paramString = oo.prettyType(fullySpecifiedType, paramName);
        } else {
            FullySpecifiedType constParamType(fullySpecifiedType);
            constParamType.setConst(true);
            QScopedPointer<ReferenceType> referenceType(new ReferenceType(constParamType, false));
            FullySpecifiedType referenceToConstParamType(referenceType.data());
            paramString = oo.prettyType(referenceToConstParamType, paramName);
        }

        const bool isStatic = m_symbol->storage() == Symbol::Static;

        // Construct declaration strings
        QString declaration = declLocation.prefix();
        QString getterTypeString = typeString;
        FullySpecifiedType getterType(fullySpecifiedType);
        if (fullySpecifiedType.isConst()) {
            getterType.setConst(false);
            getterTypeString = oo.prettyType(getterType);
        }

        const QString declarationGetterTypeAndNameString = oo.prettyType(getterType, m_getterName);
        const QString declarationGetter = QString::fromLatin1("%1%2()%3;\n")
                .arg(isStatic ? QLatin1String("static ") : QString())
            .arg(declarationGetterTypeAndNameString)
            .arg(isStatic ? QString() : QLatin1String(" const"));
        const QString declarationSetter = QString::fromLatin1("%1void %2(%3);\n")
            .arg(isStatic ? QLatin1String("static ") : QString())
            .arg(m_setterName)
            .arg(paramString);

        if (generateGetter())
            declaration += declarationGetter;
        if (generateSetter())
            declaration += declarationSetter;
        declaration += declLocation.suffix();

        // Construct implementation strings
        const QString implementationGetterTypeAndNameString = oo.prettyType(
            getterType, QString::fromLatin1("%1::%2").arg(classString, m_getterName));
        const QString implementationGetter = QString::fromLatin1(
                    "\n%1()%2\n"
                    "{\n"
                    "return %3;\n"
                    "}\n")
                .arg(implementationGetterTypeAndNameString)
                .arg(isStatic ? QString() : QLatin1String(" const"))
                .arg(m_variableString);
        const QString implementationSetter = QString::fromLatin1(
                    "\nvoid %1::%2(%3)\n"
                    "{\n"
                    "%4 = %5;\n"
                    "}\n")
                .arg(classString).arg(m_setterName)
                .arg(paramString).arg(m_variableString)
                .arg(paramName);
        QString implementation;
        if (generateGetter())
            implementation += implementationGetter;
        if (generateSetter())
            implementation += implementationSetter;

        // Create and apply changes
        Utils::ChangeSet currChanges;
        int declInsertPos = currentFile->position(qMax(1u, declLocation.line()),
                                                  declLocation.column());
        currChanges.insert(declInsertPos, declaration);

        if (sameFile) {
            InsertionLocation loc = insertLocationForMethodDefinition(m_symbol, false, refactoring,
                                                                      currentFile->fileName());
            currChanges.insert(currentFile->position(loc.line(), loc.column()), implementation);
        } else {
            CppRefactoringChanges implRef(m_interface->snapshot());
            CppRefactoringFilePtr implFile = implRef.file(implFileName);
            Utils::ChangeSet implChanges;
            InsertionLocation loc = insertLocationForMethodDefinition(m_symbol, false,
                                                                      implRef, implFileName);
            const int implInsertPos = implFile->position(loc.line(), loc.column());
            implChanges.insert(implInsertPos, implementation);
            implFile->setChangeSet(implChanges);
            implFile->appendIndentRange(
                Utils::ChangeSet::Range(implInsertPos, implInsertPos + implementation.size()));
            implFile->apply();
        }
        currentFile->setChangeSet(currChanges);
        currentFile->appendIndentRange(
            Utils::ChangeSet::Range(declInsertPos, declInsertPos + declaration.size()));
        currentFile->apply();
    }

    OperationType m_type;
    SimpleNameAST *m_variableName;
    DeclaratorIdAST *m_declaratorId;
    DeclaratorAST *m_declarator;
    SimpleDeclarationAST *m_variableDecl;
    ClassSpecifierAST *m_classSpecifier;
    SimpleDeclarationAST *m_classDecl;
    Symbol *m_symbol;

    QString m_baseName;
    QString m_getterName;
    QString m_setterName;
    QString m_variableString;
    bool m_offerQuickFix;
};

}   // anonymous namespace

QList<TextEditor::QuickFixOperation::Ptr> GenerateGetterSetter::match(const QSharedPointer<const CppQuickFixAssistInterface> &interface)
{
    QList<TextEditor::QuickFixOperation::Ptr> result;

    GenerateGetterSetterOperation *op = new GenerateGetterSetterOperation(interface);
    if (op->m_type != GenerateGetterSetterOperation::InvalidType) {
        result.append(CppQuickFixOperation::Ptr(op));
        if (op->m_type == GenerateGetterSetterOperation::GetterSetterType) {
            result.append(CppQuickFixOperation::Ptr(new GenerateGetterSetterOperation(
                              interface, op, GenerateGetterSetterOperation::GetterType)));
            result.append(CppQuickFixOperation::Ptr(new GenerateGetterSetterOperation(
                              interface, op, GenerateGetterSetterOperation::SetterType)));
        }
    } else {
        delete op;
    }

    return result;
}
