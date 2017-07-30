/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
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

#include "cppelementevaluator.h"

#include <ifile.h>

#include <FullySpecifiedType.h>
#include <Names.h>
#include <CoreTypes.h>
#include <Scope.h>
#include <Symbol.h>
#include <Symbols.h>
#include <ModelManagerInterface.h>
#include <ExpressionUnderCursor.h>
#include <Overview.h>
#include <TypeOfExpression.h>
#include <LookupContext.h>
#include <LookupItem.h>
#include <Icons.h>
#include <SimpleLexer.h>
#include <BackwardsScanner.h>
#include <Token.h>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QSet>
#include <QtCore/QQueue>
#include <QTextBlock>

using namespace CppEditor;
using namespace Internal;
using namespace CPlusPlus;

namespace {
    void moveCursorToEndOfName(QTextCursor *tc) {
        QTextDocument *doc = tc->document();
        if (!doc)
            return;

        QChar ch = doc->characterAt(tc->position());
        while (ch.isLetterOrNumber() || ch == QLatin1Char('_')) {
            tc->movePosition(QTextCursor::NextCharacter);
            ch = doc->characterAt(tc->position());
        }
    }

    QStringList stripName(const QString &name) {
        QStringList all;
        all << name;
        int colonColon = 0;
        const int size = name.size();
        while ((colonColon = name.indexOf(QLatin1String("::"), colonColon)) != -1) {
            all << name.right(size - colonColon - 2);
            colonColon += 2;
        }
        return all;
    }
}

CppElementEvaluator::CppElementEvaluator(TextEditor::TextEditorWidget *editor) :
    m_editor(editor),
    m_modelManager(CppModelManagerInterface::instance()),
    m_tc(editor->textCursor()),
    m_lookupBaseClasses(false)
{}

void CppElementEvaluator::setTextCursor(const QTextCursor &tc)
{ m_tc = tc; }

void CppElementEvaluator::setLookupBaseClasses(const bool lookup)
{ m_lookupBaseClasses = lookup; }

// @todo: Consider refactoring code from CPPEditor::findLinkAt into here.
void CppElementEvaluator::execute()
{
    clear();

    if (!m_modelManager)
        return;

    const Snapshot &snapshot = m_modelManager->snapshot();
    Document::Ptr doc = snapshot.document(m_editor->file()->fileName());
    if (!doc)
        return;

    unsigned line = 0;
    unsigned column = 0;
    const int pos = m_tc.position();
    m_editor->convertPosition(pos, &line, &column);

    checkDiagnosticMessage(doc, line);

    if (!matchIncludeFile(doc, line) && !matchMacroInUse(doc, pos)) {
        moveCursorToEndOfName(&m_tc);

        // Fetch the expression's code
        ExpressionUnderCursor expressionUnderCursor;
        const QString &expression = expressionUnderCursor(m_tc);
        Scope *scope = doc->scopeAt(line, column, m_editor->document());
        if ( !scope )
            return;
        if ( GrammarScope *gs = scope->asGrammarScope() ) {
            CPlusPlus::SimpleLexer tokenize;
            tokenize.setQtMocRunEnabled(false);
            const QString blockText = m_tc.block().text();
            const QList<CPlusPlus::Token> tokens = tokenize(blockText, CPlusPlus::BackwardsScanner::previousBlockState(m_tc.block()));
            int tokIndex = CPlusPlus::SimpleLexer::tokenAt(tokens, column);
            if ( tokIndex == -1 )
                return;
            const CPlusPlus::Token &tk = tokens.at(tokIndex);
            if ( tk.isNot(CPlusPlus::T_IDENTIFIER) )
                return;
            if ( tokIndex > 0 && tokens.at(tokIndex - 1).is(CPlusPlus::T_LESS) )
                handleGrammarSemanticType(gs, expression);
            else
                handleGrammarTerm(gs, expression);
            return;
        }

        TypeOfExpression typeOfExpression;
        typeOfExpression.init(doc, snapshot);
        const QList<LookupItem> &lookupItems = typeOfExpression(expression, scope);
        if (lookupItems.isEmpty())
            return;

        const LookupItem &lookupItem = lookupItems.first(); // ### TODO: select best candidate.
        handleLookupItemMatch(snapshot, lookupItem, typeOfExpression.context());
    }
}

void CppElementEvaluator::checkDiagnosticMessage(const CPlusPlus::Document::Ptr &document,
                                                 unsigned line)
{
    foreach (const Utils::DiagnosticMessage &m, document->diagnosticMessages()) {
        if (m.line() == line) {
            m_diagnosis = m.text();
            break;
        }
    }
}

bool CppElementEvaluator::matchIncludeFile(const CPlusPlus::Document::Ptr &document, unsigned line)
{
    foreach (const Document::Include &includeFile, document->includes()) {
        if (includeFile.line() == line) {
            m_element = QSharedPointer<CppElement>(new CppInclude(includeFile));
            return true;
        }
    }
    return false;
}

bool CppElementEvaluator::matchMacroInUse(const CPlusPlus::Document::Ptr &document, unsigned pos)
{
    foreach (const Document::MacroUse &use, document->macroUses()) {
        if (use.contains(pos)) {
            const unsigned begin = use.begin();
            const QString &name = use.macro().name();
            if (pos < begin + name.length()) {
                m_element = QSharedPointer<CppElement>(new CppMacro(use.macro()));
                return true;
            }
        }
    }
    return false;
}

void CppElementEvaluator::handleLookupItemMatch(const Snapshot &snapshot,
                                                const LookupItem &lookupItem,
                                                const LookupContext &context)
{
    Symbol *declaration = lookupItem.declaration();
    if (!declaration) {
        const QString &type = Overview().prettyType(lookupItem.type(), QString());
        m_element = QSharedPointer<CppElement>(new Unknown(type));
    } else {
        const FullySpecifiedType &type = declaration->type();
        if (declaration->isNamespace()) {
            m_element = QSharedPointer<CppElement>(new CppNamespace(declaration));
        } else if (declaration->isClass() || declaration->isForwardClassDeclaration()) {
            if (declaration->isForwardClassDeclaration())
                if (Symbol *classDeclaration = snapshot.findMatchingClassDeclaration(declaration))
                    declaration = classDeclaration;
            CppClass *cppClass = new CppClass(declaration);
            if (m_lookupBaseClasses)
                cppClass->lookupBases(declaration, context);
            m_element = QSharedPointer<CppElement>(cppClass);
        } else if (declaration->isEnum() || declaration->enclosingScope()->isEnum()) {
            m_element = QSharedPointer<CppElement>(new CppEnum(declaration));
        } else if (declaration->isTypedef()) {
            m_element = QSharedPointer<CppElement>(new CppTypedef(declaration));
        } else if (declaration->isFunction() || (type.isValid() && type->isFunctionType())) {
            m_element = QSharedPointer<CppElement>(new CppFunction(declaration));
        } else if (declaration->isDeclaration() && type.isValid()) {
            m_element = QSharedPointer<CppElement>(
                new CppVariable(declaration, context, lookupItem.scope()));
        } else if (declaration->isTemplate() && declaration->asTemplate()->declaration()) {
            m_element = QSharedPointer<CppElement>(new CppTemplate(declaration));
        } else {
            m_element = QSharedPointer<CppElement>(new CppDeclarableElement(declaration));
        }
    }
}

void CppElementEvaluator::handleGrammarTerm( GrammarScope *scope, const QString &term )
{
    GrammarScope::GrammarEntityInfo termInfo = scope->termInfo(term);
    if ( termInfo.isValid ) {
        QString desc = QObject::tr("Grammar %1: %2::<%3>")
                .arg(termInfo.isRule ? "Rule" : "Token")
                .arg(Overview().prettyType(termInfo.type, QString()))
                .arg(termInfo.semanticType);
        m_element = QSharedPointer<CppElement>(new Unknown(desc));
    }
}

void CppElementEvaluator::handleGrammarSemanticType(GrammarScope *scope, const QString &term)
{
    GrammarScope::GrammarEntityInfo typeInfo = scope->semanticTypeInfo(term);
    if ( typeInfo.isValid ) {
        QString desc = QObject::tr("Grammar Semantic Type: %1")
                .arg(Overview().prettyType(typeInfo.type, QString()));
        m_element = QSharedPointer<CppElement>(new Unknown(desc));
    }
}

bool CppElementEvaluator::identifiedCppElement() const
{
    return !m_element.isNull();
}

const QSharedPointer<CppElement> &CppElementEvaluator::cppElement() const
{
    return m_element;
}

bool CppElementEvaluator::hasDiagnosis() const
{
    return !m_diagnosis.isEmpty();
}

const QString &CppElementEvaluator::diagnosis() const
{
    return m_diagnosis;
}

void CppEditor::Internal::CppElementEvaluator::clear()
{
    m_element.clear();
    m_diagnosis.clear();
}

// CppElement
CppElement::CppElement()
    : m_helpCategory(TextEditor::HelpItem::Unknown)
    , m_link(0)
{}

CppElement::~CppElement()
{
    if ( m_link )
        delete m_link;
}

void CppElement::setHelpCategory(const TextEditor::HelpItem::Category &cat)
{ m_helpCategory = cat; }

const TextEditor::HelpItem::Category &CppElement::helpCategory() const
{ return m_helpCategory; }

void CppElement::setHelpIdCandidates(const QStringList &candidates)
{ m_helpIdCandidates = candidates; }

void CppElement::addHelpIdCandidate(const QString &candidate)
{ m_helpIdCandidates.append(candidate); }

const QStringList &CppElement::helpIdCandidates() const
{ return m_helpIdCandidates; }

void CppElement::setHelpMark(const QString &mark)
{ m_helpMark = mark; }

const QString &CppElement::helpMark() const
{ return m_helpMark; }

void CppElement::setLink(Core::ILink *link)
{ m_link = link; }

Core::ILink *CppElement::link() const
{ return m_link; }

void CppElement::setTooltip(const QString &tooltip)
{ m_tooltip = tooltip; }

const QString &CppElement::tooltip() const
{ return m_tooltip; }


// Unknown
Unknown::Unknown(const QString &type) : CppElement(), m_type(type)
{
    setTooltip(m_type);
}

Unknown::~Unknown()
{}

const QString &Unknown::type() const
{ return m_type; }

// CppInclude
CppInclude::~CppInclude()
{}

CppInclude::CppInclude(const Document::Include &includeFile) :
    CppElement(),
    m_path(QDir::toNativeSeparators(includeFile.fileName())),
    m_fileName(QFileInfo(includeFile.fileName()).fileName())
{
    setHelpCategory(TextEditor::HelpItem::Brief);
    setHelpIdCandidates(QStringList(m_fileName));
    setHelpMark(m_fileName);
    setLink(new Core::TextTargetLink(0, 0, m_path, 0, 0));
    setTooltip(m_path);
}

const QString &CppInclude::path() const
{ return m_path; }

const QString &CppInclude::fileName() const
{ return m_fileName; }

// CppMacro
CppMacro::CppMacro(const Macro &macro) : CppElement()
{
    setHelpCategory(TextEditor::HelpItem::Macro);
    setHelpIdCandidates(QStringList(macro.name()));
    setHelpMark(macro.name());
    setLink(new Core::TextTargetLink(0, 0, macro.fileName(), macro.line(), 0));
    setTooltip(macro.toString());
}

CppMacro::~CppMacro()
{}

static Core::ILink *linkToSymbol(int begin, int end, CPlusPlus::Symbol *symbol)
{
    if (!symbol)
        return 0;

    const QString fileName = QString::fromUtf8(symbol->fileName(),
                                               symbol->fileNameLength());
    unsigned line = symbol->line();
    unsigned column = symbol->column();

    if (column)
        --column;

    if (symbol->isGenerated())
        column = 0;

    return new Core::TextTargetLink(begin, end, fileName, line, column);
}

// CppDeclarableElement
CppDeclarableElement::CppDeclarableElement(Symbol *declaration) : CppElement()
{
    m_icon = Icons().iconForSymbol(declaration);

    Overview overview;
    overview.setShowArgumentNames(true);
    overview.setShowReturnTypes(true);
    m_name = overview.prettyName(declaration->name());
    if (declaration->enclosingScope()->isClass() ||
        declaration->enclosingScope()->isNamespace() ||
        declaration->enclosingScope()->isEnum()) {
        m_qualifiedName = overview.prettyName(LookupContext::fullyQualifiedName(declaration));
        setHelpIdCandidates(stripName(m_qualifiedName));
    } else {
        m_qualifiedName = m_name;
        setHelpIdCandidates(QStringList(m_name));
    }

    setTooltip(overview.prettyType(declaration->type(), m_qualifiedName));
    setLink(linkToSymbol(0, 0, declaration));
    setHelpMark(m_name);
}

CppDeclarableElement::~CppDeclarableElement()
{}

void CppDeclarableElement::setName(const QString &name)
{ m_name = name; }

const QString &CppDeclarableElement::name() const
{ return m_name; }

void CppDeclarableElement::setQualifiedName(const QString &name)
{ m_qualifiedName = name; }

const QString &CppDeclarableElement::qualifiedName() const
{ return m_qualifiedName; }

void CppDeclarableElement::setType(const QString &type)
{ m_type = type; }

const QString &CppDeclarableElement::type() const
{ return m_type; }

void CppDeclarableElement::setIcon(const QIcon &icon)
{ m_icon = icon; }

const QIcon &CppDeclarableElement::icon() const
{ return m_icon; }

// CppNamespace
CppNamespace::CppNamespace(Symbol *declaration) : CppDeclarableElement(declaration)
{
    setHelpCategory(TextEditor::HelpItem::ClassOrNamespace);
    setTooltip(qualifiedName());
}

CppNamespace::~CppNamespace()
{}

// CppClass
CppClass::CppClass(Symbol *declaration) : CppDeclarableElement(declaration)
{
    setHelpCategory(TextEditor::HelpItem::ClassOrNamespace);
    setTooltip(qualifiedName());
}

CppClass::~CppClass()
{}

void CppClass::lookupBases(Symbol *declaration, const CPlusPlus::LookupContext &context)
{
    typedef QPair<ClassOrNamespace *, CppClass *> Data;

    if (ClassOrNamespace *clazz = context.lookupType(declaration)) {
        QSet<ClassOrNamespace *> visited;

        QQueue<Data> q;
        q.enqueue(qMakePair(clazz, this));
        while (!q.isEmpty()) {
            Data current = q.dequeue();
            clazz = current.first;
            visited.insert(clazz);
            const QList<ClassOrNamespace *> &bases = clazz->usings();
            foreach (ClassOrNamespace *baseClass, bases) {
                const QList<Symbol *> &symbols = baseClass->symbols();
                foreach (Symbol *symbol, symbols) {
                    if (symbol->isClass() && (
                        clazz = context.lookupType(symbol)) &&
                        !visited.contains(clazz)) {
                        CppClass baseCppClass(symbol);
                        CppClass *cppClass = current.second;
                        cppClass->m_bases.append(baseCppClass);
                        q.enqueue(qMakePair(clazz, &cppClass->m_bases.last()));
                    }
                }
            }
        }
    }
}

const QList<CppClass> &CppClass::bases() const
{ return m_bases; }

// CppFunction
CppFunction::CppFunction(Symbol *declaration) : CppDeclarableElement(declaration)
{
    setHelpCategory(TextEditor::HelpItem::Function);

    const FullySpecifiedType &type = declaration->type();

    // Functions marks can be found either by the main overload or signature based
    // (with no argument names and no return). Help ids have no signature at all.
    Overview overview;
    overview.setShowDefaultArguments(false);
    setHelpMark(overview.prettyType(type, name()));

    overview.setShowFunctionSignatures(false);
    addHelpIdCandidate(overview.prettyName(declaration->name()));
}

CppFunction::~CppFunction()
{}

// CppEnum
CppEnum::CppEnum(Symbol *declaration) : CppDeclarableElement(declaration)
{
    setHelpCategory(TextEditor::HelpItem::Enum);

    if (declaration->enclosingScope()->isEnum()) {
        Symbol *enumSymbol = declaration->enclosingScope()->asEnum();
        Overview overview;
        setHelpMark(overview.prettyName(enumSymbol->name()));
        setTooltip(overview.prettyName(LookupContext::fullyQualifiedName(enumSymbol)));
    } else {
        setTooltip(qualifiedName());
    }
}

CppEnum::~CppEnum()
{}

// CppTypedef
CppTypedef::CppTypedef(Symbol *declaration) : CppDeclarableElement(declaration)
{
    setHelpCategory(TextEditor::HelpItem::Typedef);
    setTooltip(Overview().prettyType(declaration->type(), qualifiedName()));
}

CppTypedef::~CppTypedef()
{}

// CppVariable
CppVariable::CppVariable(Symbol *declaration, const LookupContext &context, Scope *scope) :
    CppDeclarableElement(declaration)
{
    const FullySpecifiedType &type = declaration->type();

    const Name *typeName = 0;
    if (type->isNamedType()) {
        typeName = type->asNamedType()->name();
    } else if (type->isPointerType() || type->isReferenceType()) {
        FullySpecifiedType associatedType;
        if (type->isPointerType())
            associatedType = type->asPointerType()->elementType();
        else
            associatedType = type->asReferenceType()->elementType();
        if (associatedType->isNamedType())
            typeName = associatedType->asNamedType()->name();
    }

    if (typeName) {
        if (ClassOrNamespace *clazz = context.lookupType(typeName, scope)) {
            if (!clazz->symbols().isEmpty()) {
                Overview overview;
                Symbol *symbol = clazz->symbols().at(0);
                const QString &name =
                    overview.prettyName(LookupContext::fullyQualifiedName(symbol));
                if (!name.isEmpty()) {
                    setTooltip(name);
                    setHelpCategory(TextEditor::HelpItem::ClassOrNamespace);
                    const QStringList &allNames = stripName(name);
                    if (!allNames.isEmpty()) {
                        setHelpMark(allNames.last());
                        setHelpIdCandidates(allNames);
                    }
                }
            }
        }
    }
}

CppVariable::~CppVariable()
{}

// CppTemplate
CppTemplate::CppTemplate(CPlusPlus::Symbol *declaration) : CppDeclarableElement(declaration)
{
    Template *templateSymbol = declaration->asTemplate();
    if (templateSymbol->declaration()->isClass() ||
        templateSymbol->declaration()->isForwardClassDeclaration()) {
        m_isClassTemplate = true;
        setHelpCategory(TextEditor::HelpItem::ClassOrNamespace);
        setTooltip(qualifiedName());
    } else {
        m_isClassTemplate = false;
        setHelpCategory(TextEditor::HelpItem::Function);
    }
}

CppTemplate::~CppTemplate()
{}

bool CppTemplate::isClassTemplate() const
{
    return m_isClassTemplate;
}

bool CppTemplate::isFunctionTemplate() const
{
    return !m_isClassTemplate;
}
