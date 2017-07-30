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

#ifndef CPLUSPLUS_CPPDOCUMENT_H
#define CPLUSPLUS_CPPDOCUMENT_H

#include <CPlusPlusForwardDeclarations.h>
#include "Macro.h"
#include "DependencyTable.h"
#include "qtcfileutils.h"
#include "DiagnosticMessage.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QDateTime>
#include <QtCore/QHash>
#include <QtCore/QFileInfo>
#include <QtCore/QAtomicInt>
#include <QTextDocument>

namespace CPlusPlus {

class Macro;
class MacroArgumentReference;
class LookupContext;

class CPLUSPLUS_EXPORT Document
{
    Document(const Document &other);
    void operator =(const Document &other);

    Document( const QString &fileName, const QString &precompiledHeader = QString() );

public:
    typedef QSharedPointer<Document> Ptr;
    typedef QList<QPair<QString, unsigned> > GrammarRules;

public:
    ~Document();

    unsigned revision() const;
    void setRevision(unsigned revision);

    unsigned editorRevision() const;
    void setEditorRevision(unsigned editorRevision);

    QDateTime lastModified() const;
    void setLastModified(const QDateTime &lastModified);

    QString fileName() const;

    QStringList includedFiles() const;
    void addIncludeFile(const QString &fileName, unsigned line);

    void appendMacro(const Macro &macro);
    void addMacroUse(const Macro &macro, unsigned offset, unsigned length,
                     unsigned beginLine, const QVector<MacroArgumentReference> &range,
                     bool inCondition);
    void addUndefinedMacroUse(const QByteArray &name, unsigned offset);

    Control *control() const;
    TranslationUnit *translationUnit() const;

    bool skipFunctionBody() const;
    void setSkipFunctionBody(bool skipFunctionBody);

    unsigned globalSymbolCount() const;
    Symbol *globalSymbolAt(unsigned index) const;

    Namespace *globalNamespace() const;
    void setGlobalNamespace(Namespace *globalNamespace); // ### internal

    GrammarScope *grammarScope() const;

    QList<Macro> definedMacros() const
    { return _definedMacros; }

    Symbol *lastVisibleSymbolAt(unsigned line, unsigned column = 0) const;
    Scope *scopeAt(unsigned line, unsigned column = 0, QTextDocument *textDoc = 0);

    QByteArray source() const;
    void setSource(const QByteArray &source);

    void startSkippingBlocks(unsigned offset);
    void stopSkippingBlocks(unsigned offset);

    enum ParseMode { // ### keep in sync with CPlusPlus::TranslationUnit
        ParseTranlationUnit,
        ParseDeclaration,
        ParseExpression,
        ParseDeclarator,
        ParseStatement
    };

    bool isTokenized() const;
    void tokenize();

    bool isParsed() const;
    bool parse(ParseMode mode = ParseTranlationUnit);

    enum CheckMode {
        Unchecked,
        FullCheck,
        FastCheck
    };

    void check(CheckMode mode = FullCheck);

    static Ptr create( const QString &fileName, const QString &precompiledHeader = QString() );

    void addDiagnosticMessage(const Utils::DiagnosticMessage &d)
    { _diagnosticMessages.append(d); }

    void clearDiagnosticMessages()
    { _diagnosticMessages.clear(); }

    QList<Utils::DiagnosticMessage> diagnosticMessages() const
    { return _diagnosticMessages; }

    class Block
    {
        unsigned _begin;
        unsigned _end;

    public:
        inline Block(unsigned begin = 0, unsigned end = 0)
            : _begin(begin), _end(end)
        { }

        inline bool isNull() const
        { return length() == 0; }

        inline unsigned position() const
        { return _begin; }

        inline unsigned length() const
        { return _end - _begin; }

        inline unsigned begin() const
        { return _begin; }

        inline unsigned end() const
        { return _end; }

        bool contains(unsigned pos) const
        { return pos >= _begin && pos < _end; }
    };

    class Include {
        QString _fileName;
        unsigned _line;

    public:
        Include(const QString &fileName, unsigned line)
            : _fileName(fileName), _line(line)
        { }

        QString fileName() const
        { return _fileName; }

        unsigned line() const
        { return _line; }

        bool resolved() const
        { return QFileInfo(_fileName).isAbsolute(); }
    };

    class MacroUse: public Block {
        Macro _macro;
        QVector<Block> _arguments;
        bool _inCondition;
        unsigned _beginLine;

    public:
        inline MacroUse(const Macro &macro,
                        unsigned begin, unsigned end, unsigned beginLine)
            : Block(begin, end),
              _macro(macro),
              _inCondition(false),
              _beginLine(beginLine)
        { }

        const Macro &macro() const
        { return _macro; }

        bool isFunctionLike() const
        { return _macro.isFunctionLike(); }

        QVector<Block> arguments() const
        { return _arguments; }

        bool isInCondition() const
        { return _inCondition; }

        unsigned beginLine() const
        { return _beginLine; }

    private:
        void setArguments(const QVector<Block> &arguments)
        { _arguments = arguments; }

        void addArgument(const Block &block)
        { _arguments.append(block); }

        void setInCondition(bool set)
        { _inCondition = set; }

        friend class Document;
    };

    class UndefinedMacroUse: public Block {
        QByteArray _name;

    public:
        inline UndefinedMacroUse(
                const QByteArray &name,
                unsigned begin)
            : Block(begin, begin + name.length()),
              _name(name)
        { }

        QByteArray name() const
        {
            return _name;
        }
    };

    QList<Include> includes() const
    { return _includes; }

    QList<Block> skippedBlocks() const
    { return _skippedBlocks; }

    QList<MacroUse> macroUses() const
    { return _macroUses; }

    QList<UndefinedMacroUse> undefinedMacroUses() const
    { return _undefinedMacroUses; }

    const Macro *findMacroDefinitionAt(unsigned line) const;
    const MacroUse *findMacroUseAt(unsigned offset) const;
    const UndefinedMacroUse *findUndefinedMacroUseAt(unsigned offset) const;

    void keepSourceAndAST();
    void releaseSourceAndAST();

    CheckMode checkMode() const
    { return static_cast<CheckMode>(_checkMode); }

    GrammarRules *grammarRules() { return &m_grammarRules; }

private:
    QString _fileName;
    Control *_control;
    TranslationUnit *_translationUnit;
    Namespace *_globalNamespace;
    GrammarScope *_grammarScope;
    QList<Utils::DiagnosticMessage> _diagnosticMessages;
    QList<Include> _includes;
    QList<Macro> _definedMacros;
    QList<Block> _skippedBlocks;
    QList<MacroUse> _macroUses;
    QList<UndefinedMacroUse> _undefinedMacroUses;
    QByteArray _source;
    QDateTime _lastModified;
    QAtomicInt _keepSourceAndASTCount;
    unsigned _revision;
    unsigned _editorRevision;
    unsigned _extraExecSqlEnabled;
    bool _isGrammarMode;
    quint8 _checkMode;
    QString _precompiledHeader;
    GrammarRules m_grammarRules;

    friend class Snapshot;
};

class CPLUSPLUS_EXPORT Snapshot
{
    typedef QHash<QString, Document::Ptr> _Base;

public:
    Snapshot();
    ~Snapshot();

    typedef _Base::const_iterator iterator;
    typedef _Base::const_iterator const_iterator;

    int size() const; // ### remove
    bool isEmpty() const;

    void insert(Document::Ptr doc); // ### remove
    void remove(const QString &fileName); // ### remove

    const_iterator begin() const { return _documents.begin(); }
    const_iterator end() const { return _documents.end(); }

    bool contains(const QString &fileName) const;
    Document::Ptr document(const QString &fileName) const;
    Document::Ptr operator[](const QString &fileName) const;

    const_iterator find(const QString &fileName) const;

//    Snapshot simplified(Document::Ptr doc) const;

    QByteArray preprocessedCode(const QString &source,
                                const QString &fileName) const;

    Document::Ptr documentFromSource(const QByteArray &preprocessedCode,
                                     const QString &fileName) const;

    Symbol *findMatchingDefinition(Symbol *symbol, bool strict = false) const;
    Class *findMatchingClassDeclaration(Symbol *symbol) const;

    QStringList filesDependingOn(const QString &fileName) const;
    void updateDependencyTable() const;

private:
    Symbol *findMatchingDefinitionForDocument(Document::Ptr doc, Symbol *declaration, Function *declarationTy, bool strict) const;
//    void simplified_helper(Document::Ptr doc, Snapshot *snapshot) const;

private:
    mutable DependencyTable m_deps;
    _Base _documents;
};

void CPLUSPLUS_EXPORT findMatchingDeclaration(
        const LookupContext &context,
        Function *functionType,
        QList<Declaration *> *typeMatch,
        QList<Declaration *> *argumentCountMatch,
        QList<Declaration *> *nameMatch);
QList<Declaration *> CPLUSPLUS_EXPORT findMatchingDeclaration(
        const LookupContext &context, Function *functionType);

} // namespace CPlusPlus

#endif // CPLUSPLUS_CPPDOCUMENT_H
