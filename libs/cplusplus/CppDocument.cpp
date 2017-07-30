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

#include "CppDocument.h"
#include "FastPreprocessor.h"
#include "LookupContext.h"
#include "Overview.h"
#include "Bind.h"
#include "GrammarBind.h"

#include <Control.h>
#include <TranslationUnit.h>
#include <DiagnosticClient.h>
#include <Literals.h>
#include <Symbols.h>
#include <Names.h>
#include <AST.h>
#include <ASTPatternBuilder.h>
#include <ASTMatcher.h>
#include <Scope.h>
#include <SymbolVisitor.h>
#include <NameVisitor.h>
#include <TypeVisitor.h>
#include <CoreTypes.h>
#include <LookupContext.h>
#include <SimpleLexer.h>
#include <BackwardsScanner.h>
#include "projectmanager.h"

#include <QtCore/QByteArray>
#include <QtCore/QBitArray>
#include <QtCore/QDir>
#include <QtCore/QtDebug>
#include <QTextBlock>
#include <QTextCursor>

/*!
    \namespace CPlusPlus
    The namespace for C++ related tools.
*/

#ifdef Q_OS_WIN
#define TO_UPPER_OPT(fileName) ((fileName).toUpper())
#else
#define TO_UPPER_OPT(fileName) (fileName)
#endif

using namespace CPlusPlus;

namespace {

class LastVisibleSymbolAt: protected SymbolVisitor
{
    Symbol *root;
    unsigned line;
    unsigned column;
    Symbol *symbol;

public:
    LastVisibleSymbolAt(Symbol *root)
        : root(root), line(0), column(0), symbol(0) {}

    Symbol *operator()(unsigned line, unsigned column)
    {
        this->line = line;
        this->column = column;
        this->symbol = 0;
        accept(root);
        if (! symbol)
            symbol = root;
        return symbol;
    }

protected:
    bool preVisit(Symbol *s)
    {
        if (s->asBlock() || s->asExecSqlStatement()) {
            if (s->line() < line || (s->line() == line && s->column() <= column)) {
                return true;
            }
            // skip blocks
        } if (s->line() < line || (s->line() == line && s->column() <= column)) {
            symbol = s;
            return true;
        }

        return false;
    }
};

class FindScopeAt: protected SymbolVisitor
{
    TranslationUnit *_unit;
    unsigned _line;
    unsigned _column;
    Scope *_scope;

public:
    /** line and column should be 1-based */
    FindScopeAt(TranslationUnit *unit, unsigned line, unsigned column)
        : _unit(unit), _line(line), _column(column), _scope(0) {}

    Scope *operator()(Symbol *symbol)
    {
        accept(symbol);
        return _scope;
    }

protected:
    bool process(Scope *symbol)
    {
        if (! _scope) {
            Scope *scope = symbol;

            for (unsigned i = 0; i < scope->memberCount(); ++i) {
                accept(scope->memberAt(i));

                if (_scope) {
                    _scope = _scope->substituteScope();
                    return false;
                }
            }

            unsigned startLine, startColumn;
            _unit->getPosition(scope->startOffset(), &startLine, &startColumn);

            if (_line > startLine || (_line == startLine && _column >= startColumn)) {
                unsigned endLine, endColumn;
                _unit->getPosition(scope->endOffset(), &endLine, &endColumn);

                if (_line < endLine || (_line == endLine && _column < endColumn))
                    _scope = scope->substituteScope();
            }
        }

        return false;
    }

    using SymbolVisitor::visit;

    virtual bool preVisit(Symbol *)
    { return ! _scope; }

    virtual bool visit(UsingNamespaceDirective *) { return false; }
    virtual bool visit(UsingDeclaration *) { return false; }
    virtual bool visit(NamespaceAlias *) { return false; }
    virtual bool visit(Declaration *) { return false; }
    virtual bool visit(Argument *) { return false; }
    virtual bool visit(TypenameArgument *) { return false; }
    virtual bool visit(BaseClass *) { return false; }
    virtual bool visit(ForwardClassDeclaration *) { return false; }

    virtual bool visit(Enum *symbol)
    { return process(symbol); }

    virtual bool visit(Function *symbol)
    { return process(symbol); }

    virtual bool visit(Namespace *symbol)
    { return process(symbol); }

    virtual bool visit(Class *symbol)
    { return process(symbol); }

    virtual bool visit(Block *symbol)
    { return process(symbol); }

    virtual bool visit(ExecSqlStatement *symbol)
    { return process(symbol); }

    // Objective-C
    virtual bool visit(ObjCBaseClass *) { return false; }
    virtual bool visit(ObjCBaseProtocol *) { return false; }
    virtual bool visit(ObjCForwardClassDeclaration *) { return false; }
    virtual bool visit(ObjCForwardProtocolDeclaration *) { return false; }
    virtual bool visit(ObjCPropertyDeclaration *) { return false; }

    virtual bool visit(ObjCClass *symbol)
    { return process(symbol); }

    virtual bool visit(ObjCProtocol *symbol)
    { return process(symbol); }

    virtual bool visit(ObjCMethod *symbol)
    { return process(symbol); }
};


class DocumentDiagnosticClient : public DiagnosticClient
{
    enum { MAX_MESSAGE_COUNT = 10 };

public:
    DocumentDiagnosticClient(Document *doc, QList<Utils::DiagnosticMessage> *messages)
        : doc(doc),
          messages(messages),
          errorCount(0)
    { }

    virtual void report(int level,
                        const StringLiteral *fileId,
                        unsigned line, unsigned column,
                        const char *format, va_list ap)
    {
        if (level == Error) {
            ++errorCount;

            if (errorCount >= MAX_MESSAGE_COUNT)
                return; // ignore the error
        }

        const QString fileName = QString::fromUtf8(fileId->chars(), fileId->size());

        if (fileName != doc->fileName())
            return;

        QString message;
        message.vsprintf(format, ap);

        Utils::DiagnosticMessage m(convertLevel(level), doc->fileName(),
                                      line, column, message);
        messages->append(m);
    }

    static int convertLevel(int level) {
        switch (level) {
            case Warning: return Utils::DiagnosticMessage::Warning;
            case Error:   return Utils::DiagnosticMessage::Error;
            case Fatal:   return Utils::DiagnosticMessage::Fatal;
            default:      return Utils::DiagnosticMessage::Error;
        }
    }

private:
    Document *doc;
    QList<Utils::DiagnosticMessage> *messages;
    int errorCount;
};

} // anonymous namespace


static inline bool isScDocument( const QString &fileName )
{
    int index = fileName.size() - 3;

    if ( index < 0 )
        return false;

    if ( fileName[index++] == '.' ) {
        if ( fileName[index] == 'S' || fileName[index] == 's' ) {
            index++;
            if ( fileName[index] == 'C' || fileName[index] == 'c' ) {
                return true;
            }
        }
    }

    return false;
}

static inline bool isGrammarDocument( const QString &fileName )
{
    int index = fileName.size() - 3;

    if ( index < 0 )
        return false;

    if ( fileName[index++] == '.' ) {
        if ( fileName[index] == 'Y' || fileName[index] == 'y' ) {
            index++;
            if ( fileName[index] == 'Y' || fileName[index] == 'y' ) {
                return true;
            }
        }
    }

    return false;
}

Document::Document( const QString &fileName, const QString &precompiledHeader )
    : _fileName(QDir::cleanPath(fileName)),
      _globalNamespace(0),
      _grammarScope(0),
      _revision(0),
      _editorRevision(0),
      _extraExecSqlEnabled(isScDocument(fileName)),
      _isGrammarMode(isGrammarDocument(fileName)),
      _checkMode(0),
      _precompiledHeader(precompiledHeader)
{
    _control = new Control();

    _control->setDiagnosticClient(new DocumentDiagnosticClient(this, &_diagnosticMessages));

    const QByteArray localFileName = fileName.toUtf8();
    const StringLiteral *fileId = _control->stringLiteral(localFileName.constData(),
                                                                      localFileName.size());
    _translationUnit = new TranslationUnit(_control, fileId);
    _translationUnit->setQtMocRunEnabled(true);
    _translationUnit->setCxxOxEnabled(true);
    _translationUnit->setObjCEnabled(true);
    _translationUnit->setExtraExecSqlEnabled(_extraExecSqlEnabled);
    _translationUnit->setGrammarMode(_isGrammarMode);

    if ( !precompiledHeader.isEmpty() && !_fileName.endsWith(precompiledHeader, Qt::CaseInsensitive) )
        _includes << Include(precompiledHeader, 0);

    (void) _control->switchTranslationUnit(_translationUnit);
}

Document::~Document()
{
    delete _translationUnit;
    delete _control->diagnosticClient();
    delete _control;
}

Control *Document::control() const
{
    return _control;
}

unsigned Document::revision() const
{
    return _revision;
}

void Document::setRevision(unsigned revision)
{
    _revision = revision;
}

unsigned Document::editorRevision() const
{
    return _editorRevision;
}

void Document::setEditorRevision(unsigned editorRevision)
{
    _editorRevision = editorRevision;
}

QDateTime Document::lastModified() const
{
    return _lastModified;
}

void Document::setLastModified(const QDateTime &lastModified)
{
    _lastModified = lastModified;
}

QString Document::fileName() const
{
    return _fileName;
}

QStringList Document::includedFiles() const
{
    QStringList files;
    foreach (const Include &i, _includes)
        files.append(i.fileName());
    files.removeDuplicates();
    return files;
}

void Document::addIncludeFile(const QString &fileName, unsigned line)
{
    _includes.append(Include(QDir::cleanPath(fileName), line));
}

void Document::appendMacro(const Macro &macro)
{
    _definedMacros.append(macro);
}

void Document::addMacroUse(const Macro &macro, unsigned offset, unsigned length,
                           unsigned beginLine,
                           const QVector<MacroArgumentReference> &actuals, bool inCondition)
{
    MacroUse use(macro, offset, offset + length, beginLine);
    use.setInCondition(inCondition);

    foreach (const MacroArgumentReference &actual, actuals) {
        const Block arg(actual.position(), actual.position() + actual.length());

        use.addArgument(arg);
    }

    _macroUses.append(use);
}

void Document::addUndefinedMacroUse(const QByteArray &name, unsigned offset)
{
    QByteArray copy(name.data(), name.size());
    UndefinedMacroUse use(copy, offset);
    _undefinedMacroUses.append(use);
}

/*!
    \class Document::MacroUse
    \brief Represents the usage of a macro in a \l {Document}.
    \sa Document::UndefinedMacroUse
*/

/*!
    \class Document::UndefinedMacroUse
    \brief Represents a macro that was looked up, but not found.

    Holds data about the reference to a macro in an \tt{#ifdef} or \tt{#ifndef}
    or argument to the \tt{defined} operator inside an \tt{#if} or \tt{#elif} that does
    not exist.

    \sa Document::undefinedMacroUses(), Document::MacroUse, Macro
*/

/*!
    \fn QByteArray Document::UndefinedMacroUse::name() const

    Returns the name of the macro that was not found.
*/

/*!
    \fn QList<UndefinedMacroUse> Document::undefinedMacroUses() const

    Returns a list of referenced but undefined macros.

    \sa Document::macroUses(), Document::definedMacros(), Macro
*/

/*!
    \fn QList<MacroUse> Document::macroUses() const

    Returns a list of macros used.

    \sa Document::undefinedMacroUses(), Document::definedMacros(), Macro
*/

/*!
    \fn QList<Macro> Document::definedMacros() const

    Returns the list of macros defined.

    \sa Document::macroUses(), Document::undefinedMacroUses()
*/

TranslationUnit *Document::translationUnit() const
{
    return _translationUnit;
}

bool Document::skipFunctionBody() const
{
    return _translationUnit->skipFunctionBody();
}

void Document::setSkipFunctionBody(bool skipFunctionBody)
{
    _translationUnit->setSkipFunctionBody(skipFunctionBody);
}

unsigned Document::globalSymbolCount() const
{
    if (! _globalNamespace)
        return 0;

    return _globalNamespace->memberCount();
}

Symbol *Document::globalSymbolAt(unsigned index) const
{
    return _globalNamespace->memberAt(index);
}

Namespace *Document::globalNamespace() const
{
    return _globalNamespace;
}

void Document::setGlobalNamespace(Namespace *globalNamespace)
{
    _globalNamespace = globalNamespace;
}

GrammarScope *Document::grammarScope() const
{
    return _grammarScope;
}

Scope *Document::scopeAt(unsigned line, unsigned column, QTextDocument *textDoc)
{
    FindScopeAt findScopeAt(_translationUnit, line, column);
    if (Scope *scope = findScopeAt(_globalNamespace))
        return scope;
    if ( _grammarScope ) {
        if ( textDoc ) {
            QTextBlock block = textDoc->findBlockByLineNumber(line - 1);
            if ( block.isValid() )
                if ( SimpleLexer::inBracesOrIncomment(block.text(), column, BackwardsScanner::previousBlockState(block)) )
                    return _globalNamespace;
        }
        return _grammarScope;
    }
    return _globalNamespace;
}

Symbol *Document::lastVisibleSymbolAt(unsigned line, unsigned column) const
{
    LastVisibleSymbolAt lastVisibleSymbolAt(globalNamespace());
    return lastVisibleSymbolAt(line, column);
}

const Macro *Document::findMacroDefinitionAt(unsigned line) const
{
    foreach (const Macro &macro, _definedMacros) {
        if (macro.line() == line)
            return &macro;
    }
    return 0;
}

const Document::MacroUse *Document::findMacroUseAt(unsigned offset) const
{
    foreach (const Document::MacroUse &use, _macroUses) {
        if (use.contains(offset))
            return &use;
    }
    return 0;
}

const Document::UndefinedMacroUse *Document::findUndefinedMacroUseAt(unsigned offset) const
{
    foreach (const Document::UndefinedMacroUse &use, _undefinedMacroUses) {
        if (use.contains(offset))
            return &use;
    }
    return 0;
}

Document::Ptr Document::create(const QString &fileName, const QString &precompiledHeader)
{
    Document::Ptr doc(new Document(fileName, precompiledHeader));
    return doc;
}

QByteArray Document::source() const
{ return _source; }

void Document::setSource(const QByteArray &source)
{
    _source = source;
    _translationUnit->setSource(_source.constBegin(), _source.size());
}

void Document::startSkippingBlocks(unsigned start)
{
    _skippedBlocks.append(Block(start, 0));
}

void Document::stopSkippingBlocks(unsigned stop)
{
    if (_skippedBlocks.isEmpty())
        return;

    unsigned start = _skippedBlocks.back().begin();
    if (start > stop)
        _skippedBlocks.removeLast(); // Ignore this block, it's invalid.
    else
        _skippedBlocks.back() = Block(start, stop);
}

bool Document::isTokenized() const
{
    return _translationUnit->isTokenized();
}

void Document::tokenize()
{
    _translationUnit->tokenize();
}

bool Document::isParsed() const
{
    return _translationUnit->isParsed();
}

bool Document::parse(ParseMode mode)
{
    TranslationUnit::ParseMode m = TranslationUnit::ParseTranlationUnit;
    switch (mode) {
    case ParseTranlationUnit:
        m = TranslationUnit::ParseTranlationUnit;
        break;

    case ParseDeclaration:
        m = TranslationUnit::ParseDeclaration;
        break;

    case ParseExpression:
        m = TranslationUnit::ParseExpression;
        break;

    case ParseDeclarator:
        m = TranslationUnit::ParseDeclarator;
        break;

    case ParseStatement:
        m = TranslationUnit::ParseStatement;
        break;

    default:
        break;
    }

    return _translationUnit->parse(m);
}

void Document::check(CheckMode mode)
{
    Q_ASSERT(!_globalNamespace);

    _checkMode = mode;

    if (! isParsed())
        parse();

    _globalNamespace = _control->newNamespace(0);
    Bind semantic(_translationUnit);
    if (mode == FastCheck)
        semantic.setSkipFunctionBodies(true);

    if (! _translationUnit->ast())
        return; // nothing to do.

    if (TranslationUnitAST *ast = _translationUnit->ast()->asTranslationUnit()) {
        if ( _translationUnit->isGrammarMode() ) {
            m_grammarRules.clear();
            if ( GrammarAST *grAst = _translationUnit->grammarAst() ) {
                Q_ASSERT(!_grammarScope);
                for ( GrammarRuleDefinitionListAST *iter = grAst->rule_def_list; iter; iter = iter->next ) {
                    if ( GrammarRuleDefinitionAST *rule = iter->value ) {
                        if ( SimpleNameAST *ruleName = rule->ruleName ) {
                            const Token &tk = _translationUnit->tokenAt(ruleName->identifier_token);
                            unsigned lineno;
                            _translationUnit->getPosition(tk.offset, &lineno);
                            m_grammarRules.append(qMakePair(QString(tk.identifier->chars()), lineno - 1));
                        }
                    }
                }
                _grammarScope = _control->newGrammarScope(grAst->lastToken());
                GrammarBind grammarBind(_translationUnit);
                grammarBind(grAst, _grammarScope);
                semantic.setGrammarScope(_grammarScope);
            }
        }
        semantic(ast, _globalNamespace);
    } else if (ExpressionAST *ast = _translationUnit->ast()->asExpression()) {
        semantic(ast, _globalNamespace);
    } else if (DeclarationAST *ast = translationUnit()->ast()->asDeclaration()) {
        semantic(ast, _globalNamespace);
    }
}

void Document::keepSourceAndAST()
{
    _keepSourceAndASTCount.ref();
}

void Document::releaseSourceAndAST()
{
    if (!_keepSourceAndASTCount.deref()) {
        _source.clear();
        _translationUnit->release();
        _control->squeeze();
    }
}

Snapshot::Snapshot()
{
}

Snapshot::~Snapshot()
{
}

int Snapshot::size() const
{
    return _documents.size();
}

bool Snapshot::isEmpty() const
{
    return _documents.isEmpty();
}

Document::Ptr Snapshot::operator[](const QString &fileName) const
{
    return _documents.value(TO_UPPER_OPT(fileName), Document::Ptr());
}

Snapshot::const_iterator Snapshot::find(const QString &fileName) const
{
    return _documents.find(TO_UPPER_OPT(fileName));
}

void Snapshot::remove(const QString &fileName)
{
    _documents.remove(TO_UPPER_OPT(fileName));
}

bool Snapshot::contains(const QString &fileName) const
{
    return _documents.contains(TO_UPPER_OPT(fileName));
}

void Snapshot::insert(Document::Ptr doc)
{
    if (doc) {
        _documents.insert(TO_UPPER_OPT(doc->fileName()), doc);
        m_deps.files.clear();
    }
}

QByteArray Snapshot::preprocessedCode(const QString &source, const QString &fileName) const
{
    FastPreprocessor pp(*this);
    return pp.run(fileName, source);
}

Document::Ptr Snapshot::documentFromSource(const QByteArray &preprocessedCode,
                                           const QString &fileName) const
{
    Document::Ptr newDoc = Document::create(fileName);

    if (Document::Ptr thisDocument = document(fileName)) {
        newDoc->_revision = thisDocument->_revision;
        newDoc->_editorRevision = thisDocument->_editorRevision;
        newDoc->_lastModified = thisDocument->_lastModified;
        newDoc->_includes = thisDocument->_includes;
        newDoc->_definedMacros = thisDocument->_definedMacros;
        newDoc->_macroUses = thisDocument->_macroUses;
    }

    newDoc->setSource(preprocessedCode);
    return newDoc;
}

Document::Ptr Snapshot::document(const QString &fileName) const
{
    return _documents.value(TO_UPPER_OPT(fileName));
}

//Snapshot Snapshot::simplified(Document::Ptr doc) const
//{
//    Snapshot snapshot;
//    simplified_helper(doc, &snapshot);
//    return snapshot;
//}

//void Snapshot::simplified_helper(Document::Ptr doc, Snapshot *snapshot) const
//{
//    if (! doc)
//        return;

//    if (! snapshot->contains(doc->fileName())) {
//        snapshot->insert(doc);

//        foreach (const Document::Include &incl, doc->includes()) {
//            Document::Ptr includedDoc = document(incl.fileName());
//            simplified_helper(includedDoc, snapshot);
//        }
//    }
//}

namespace {
class FindMatchingDefinition: public SymbolVisitor
{
    Symbol *_declaration;
    const OperatorNameId *_oper;
    QList<Function *> _result;

public:
    FindMatchingDefinition(Symbol *declaration)
        : _declaration(declaration)
        , _oper(0)
    {
        if (_declaration->name())
            _oper = _declaration->name()->asOperatorNameId();
    }

    QList<Function *> result() const { return _result; }

    using SymbolVisitor::visit;

    virtual bool visit(Function *fun)
    {
        if (_oper) {
            if (const Name *name = fun->unqualifiedName()) {
                    if (_oper->isEqualTo(name))
                        _result.append(fun);
            }
        } else if (const Identifier *id = _declaration->identifier()) {
            if (id->isEqualTo(fun->identifier()))
                _result.append(fun);
        }

        return false;
    }

    virtual bool visit(Block *)
    {
        return false;
    }
};
} // end of anonymous namespace

Symbol *Snapshot::findMatchingDefinitionForDocument(Document::Ptr doc, Symbol *declaration, Function *declarationTy, bool strict) const
{
    const Identifier *id = declaration->identifier();
    if (id && ! doc->control()->findIdentifier(id->chars(),
                                               id->size()))
        return 0;

    if (!id) {
        if (!declaration->name())
            return 0;
        const OperatorNameId *oper = declaration->name()->asOperatorNameId();
        if (!oper)
            return 0;
        if (!doc->control()->findOperatorNameId(oper->kind()))
            return 0;
    }

    FindMatchingDefinition candidates(declaration);
    candidates.accept(doc->globalNamespace());

    const QList<Function *> result = candidates.result();
    if (! result.isEmpty()) {
        LookupContext context(doc, *this);

        QList<Function *> viableFunctions;

        ClassOrNamespace *enclosingType = context.lookupType(declaration);
        if (! enclosingType)
            return 0; // nothing to do

        foreach (Function *fun, result) {
            const QList<LookupItem> declarations = context.lookup(fun->name(), fun->enclosingScope());
            if (declarations.isEmpty())
                return 0;

            const LookupItem best = declarations.first();
            if (enclosingType == context.lookupType(best.declaration()))
                viableFunctions.append(fun);
        }

        if (viableFunctions.isEmpty())
            return 0;

        else if (! strict && viableFunctions.length() == 1)
            return viableFunctions.first();

        Function *best = 0;

        foreach (Function *fun, viableFunctions) {
            if (! (fun->unqualifiedName() && fun->unqualifiedName()->isEqualTo(declaration->unqualifiedName())))
                continue;
            else if (fun->argumentCount() == declarationTy->argumentCount()) {
                if (! strict && ! best)
                    best = fun;

                unsigned argc = 0;
                for (; argc < declarationTy->argumentCount(); ++argc) {
                    Symbol *arg = fun->argumentAt(argc);
                    Symbol *otherArg = declarationTy->argumentAt(argc);
                    if (! arg->type().isEqualTo(otherArg->type()))
                        break;
                }

                if (argc == declarationTy->argumentCount())
                    best = fun;
            }
        }

        if (strict && ! best)
            return 0;

        if (! best)
            best = viableFunctions.first();
        return best;
    }

    return 0;
}

// strict means the returned symbol has to match exactly,
// including argument count and argument types
Symbol *Snapshot::findMatchingDefinition(Symbol *declaration, bool strict) const
{
    if (!declaration)
        return 0;

    Document::Ptr thisDocument = document(QString::fromUtf8(declaration->fileName(), declaration->fileNameLength()));
    if (! thisDocument) {
        qWarning() << "undefined document:" << declaration->fileName();
        return 0;
    }

    Function *declarationTy = declaration->type()->asFunctionType();
    if (! declarationTy) {
        qWarning() << "not a function:" << declaration->fileName() << declaration->line() << declaration->column();
        return 0;
    }

    QList<Symbol *>results;
    ProjectExplorer::Project *declartionProject = ProjectExplorer::ProjectManager::projectForFile(declaration->fileName());

    foreach (Document::Ptr doc, *this) {
        if ( Symbol *result = findMatchingDefinitionForDocument(doc, declaration, declarationTy, strict) ) {
            ProjectExplorer::Project *definitionProject = ProjectExplorer::ProjectManager::projectForFile(result->fileName());
            if ( definitionProject == declartionProject )
                results.prepend(result);
            else if ( !strict )
                results.append(result);
        }
    }

    if ( !results.isEmpty() )
        return results.first();

    return 0;
}

Class *Snapshot::findMatchingClassDeclaration(Symbol *declaration) const
{
    if (! declaration->identifier())
        return 0;

    foreach (Document::Ptr doc, *this) {
        if (! doc->control()->findIdentifier(declaration->identifier()->chars(),
                                             declaration->identifier()->size()))
            continue;

        LookupContext context(doc, *this);

        ClassOrNamespace *type = context.lookupType(declaration);
        if (!type)
            continue;

        foreach (Symbol *s, type->symbols()) {
            if (Class *c = s->asClass())
                return c;
        }
    }

    return 0;
}

QStringList Snapshot::filesDependingOn(const QString &fileName) const
{
    updateDependencyTable();
    return m_deps.filesDependingOn(fileName);
}

void Snapshot::updateDependencyTable() const
{
    if (m_deps.files.isEmpty())
        m_deps.build(*this);
}

void CPlusPlus::findMatchingDeclaration(const LookupContext &context,
                                        Function *functionType,
                                        QList<Declaration *> *typeMatch,
                                        QList<Declaration *> *argumentCountMatch,
                                        QList<Declaration *> *nameMatch)
{
    Scope *enclosingScope = functionType->enclosingScope();
    while (! (enclosingScope->isNamespace() || enclosingScope->isClass()))
        enclosingScope = enclosingScope->enclosingScope();
    Q_ASSERT(enclosingScope != 0);

    const Name *functionName = functionType->name();
    if (! functionName)
        return; // anonymous function names are not valid c++

    ClassOrNamespace *binding = 0;
    const QualifiedNameId *qName = functionName->asQualifiedNameId();
    if (qName) {
        if (qName->base())
            binding = context.lookupType(qName->base(), enclosingScope);
        else
            binding = context.globalNamespace();
        functionName = qName->name();
    }

    if (!binding) { // declaration for a global function
        binding = context.lookupType(enclosingScope);

        if (!binding)
            return;
    }

    const Identifier *funcId = functionName->identifier();
    if (!funcId) // E.g. operator, which we might be able to handle in the future...
        return;

    foreach (Symbol *s, binding->symbols()) {
        Scope *scope = s->asScope();
        if (!scope)
            continue;

        for (Symbol *s = scope->find(funcId); s; s = s->next()) {
            if (! s->name())
                continue;
            else if (! funcId->isEqualTo(s->identifier()))
                continue;
            else if (! s->type()->isFunctionType())
                continue;
            else if (Declaration *decl = s->asDeclaration()) {
                if (Function *declFunTy = decl->type()->asFunctionType()) {
                    if (functionType->isEqualTo(declFunTy))
                        typeMatch->prepend(decl);
                    else if (functionType->argumentCount() == declFunTy->argumentCount())
                        argumentCountMatch->prepend(decl);
                    else
                        nameMatch->append(decl);
                }
            }
        }
    }
}

QList<Declaration *> CPlusPlus::findMatchingDeclaration(const LookupContext &context, Function *functionType)
{
    QList<Declaration *> result;
    QList<Declaration *> nameMatch, argumentCountMatch, typeMatch;
    findMatchingDeclaration(context, functionType, &typeMatch, &argumentCountMatch, &nameMatch);
    result.append(typeMatch);
    result.append(argumentCountMatch);
    result.append(nameMatch);
    return result;
}
