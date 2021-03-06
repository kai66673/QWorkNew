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

#include "cppcompleteswitch.h"
#include "cppeditor.h"
#include "cppquickfix.h"
#include "cppinsertdecldef.h"
#include "cppinsertqtpropertymembers.h"
#include "cppquickfixassistant.h"
#include "cppcompleteswitch.h"
#include "cppfunctiondecldeflink.h"
#include "cppclassesfilter.h"
#include "LocatorManager.h"
#include "SimpleLexer.h"

#include <ASTVisitor.h>
#include <AST.h>
#include <ASTMatcher.h>
#include <ASTPatternBuilder.h>
#include <CoreTypes.h>
#include <Literals.h>
#include <Name.h>
#include <Names.h>
#include <Symbol.h>
#include <Symbols.h>
#include <Token.h>
#include <TranslationUnit.h>
#include <Type.h>

#include <cplusplus/DependencyTable.h>
#include <cplusplus/Overview.h>
#include <cplusplus/TypeOfExpression.h>
#include <cplusplus/CppRewriter.h>
#include <cpptools/cpptoolsconstants.h>
#include <cpptools/cpprefactoringchanges.h>
#include <cpptools/insertionpointlocator.h>
#include <qtcassert.h>

#include <QtCore/QFileInfo>
#include <QApplication>
#include <QTextBlock>
#include <QTextCursor>

using namespace CppEditor;
using namespace CppEditor::Internal;
using namespace CppTools;
using namespace TextEditor;
using namespace CPlusPlus;
using namespace Utils;

static inline bool isQtStringLiteral(const QByteArray &id)
{
    return id == QStringLiteral("QLatin1String") ||
           id == QStringLiteral("QLatin1Literal") ||
           id == QStringLiteral("QStringLiteral");
}

namespace {

/*
    Rewrite
    a op b -> !(a invop b)
    (a op b) -> !(a invop b)
    !(a op b) -> (a invob b)

    Activates on: <= < > >= == !=
*/
class UseInverseOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        QList<CppQuickFixOperation::Ptr> result;
        CppRefactoringFilePtr file = interface_->currentFile();

        const QList<AST *> &path = interface_->path();
        int index = path.size() - 1;
        BinaryExpressionAST *binary = path.at(index)->asBinaryExpression();
        if (! binary)
            return result;
        if (! interface_->isCursorOn(binary->binary_op_token))
            return result;

        Kind invertToken;
        switch (file->tokenAt(binary->binary_op_token).kind()) {
        case T_LESS_EQUAL:
            invertToken = T_GREATER;
            break;
        case T_LESS:
            invertToken = T_GREATER_EQUAL;
            break;
        case T_GREATER:
            invertToken = T_LESS_EQUAL;
            break;
        case T_GREATER_EQUAL:
            invertToken = T_LESS;
            break;
        case T_EQUAL_EQUAL:
            invertToken = T_EXCLAIM_EQUAL;
            break;
        case T_EXCLAIM_EQUAL:
            invertToken = T_EQUAL_EQUAL;
            break;
        default:
            return result;
        }

        result.append(CppQuickFixOperation::Ptr(new Operation(interface_, index, binary, invertToken)));
        return result;
    }

private:
    class Operation: public CppQuickFixOperation
    {
        BinaryExpressionAST *binary;
        NestedExpressionAST *nested;
        UnaryExpressionAST *negation;

        QString replacement;

    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_,
            int priority, BinaryExpressionAST *binary, Kind invertToken)
            : CppQuickFixOperation(interface_, priority)
            , binary(binary), nested(0), negation(0)
        {
            Token tok;
            tok.f.kind = invertToken;
            replacement = QLatin1String(tok.spell());

            // check for enclosing nested expression
            if (priority - 1 >= 0)
                nested = interface_->path()[priority - 1]->asNestedExpression();

            // check for ! before parentheses
            if (nested && priority - 2 >= 0) {
                negation = interface_->path()[priority - 2]->asUnaryExpression();
                if (negation && ! interface_->currentFile()->tokenAt(negation->unary_op_token).is(T_EXCLAIM))
                    negation = 0;
            }
        }

        virtual QString description() const
        {
            return QApplication::translate("CppTools::QuickFix", "Rewrite Using %1").arg(replacement);
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();
            if (negation) {
                // can't remove parentheses since that might break precedence
                changes.remove(currentFile->range(negation->unary_op_token));
            } else if (nested) {
                changes.insert(currentFile->startOf(nested), "!");
            } else {
                changes.insert(currentFile->startOf(binary), "!(");
                changes.insert(currentFile->endOf(binary), ")");
            }
            changes.replace(currentFile->range(binary->binary_op_token), replacement);
            currentFile->setChangeSet(changes);
            currentFile->apply();
        }
    };
};

/*
    Rewrite
    a op b

    As
    b flipop a

    Activates on: <= < > >= == != && ||
*/
class FlipBinaryOp: public CppQuickFixFactory
{
public:
    virtual QList<QuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        QList<QuickFixOperation::Ptr> result;
        const QList<AST *> &path = interface_->path();
        CppRefactoringFilePtr file = interface_->currentFile();

        int index = path.size() - 1;
        BinaryExpressionAST *binary = path.at(index)->asBinaryExpression();
        if (! binary)
            return result;
        if (! interface_->isCursorOn(binary->binary_op_token))
            return result;

        Kind flipToken;
        switch (file->tokenAt(binary->binary_op_token).kind()) {
        case T_LESS_EQUAL:
            flipToken = T_GREATER_EQUAL;
            break;
        case T_LESS:
            flipToken = T_GREATER;
            break;
        case T_GREATER:
            flipToken = T_LESS;
            break;
        case T_GREATER_EQUAL:
            flipToken = T_LESS_EQUAL;
            break;
        case T_EQUAL_EQUAL:
        case T_EXCLAIM_EQUAL:
        case T_AMPER_AMPER:
        case T_PIPE_PIPE:
            flipToken = T_EOF_SYMBOL;
            break;
        default:
            return result;
        }

        QString replacement;
        if (flipToken != T_EOF_SYMBOL) {
            Token tok;
            tok.f.kind = flipToken;
            replacement = QLatin1String(tok.spell());
        }

        result.append(QuickFixOperation::Ptr(new Operation(interface_, index, binary, replacement)));
        return result;
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_,
                  int priority, BinaryExpressionAST *binary, QString replacement)
            : CppQuickFixOperation(interface_)
            , binary(binary)
            , replacement(replacement)
        {
            setPriority(priority);
        }

        virtual QString description() const
        {
            if (replacement.isEmpty())
                return QApplication::translate("CppTools::QuickFix", "Swap Operands");
            else
                return QApplication::translate("CppTools::QuickFix", "Rewrite Using %1").arg(replacement);
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            changes.flip(currentFile->range(binary->left_expression), currentFile->range(binary->right_expression));
            if (! replacement.isEmpty())
                changes.replace(currentFile->range(binary->binary_op_token), replacement);

            currentFile->setChangeSet(changes);
            currentFile->apply();
        }

    private:
        BinaryExpressionAST *binary;
        QString replacement;
    };
};

/*
    Rewrite
    !a && !b

    As
    !(a || b)

    Activates on: &&
*/
class RewriteLogicalAndOp: public CppQuickFixFactory
{
public:
    virtual QList<QuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        QList<QuickFixOperation::Ptr> result;
        BinaryExpressionAST *expression = 0;
        const QList<AST *> &path = interface_->path();
        CppRefactoringFilePtr file = interface_->currentFile();

        int index = path.size() - 1;
        for (; index != -1; --index) {
            expression = path.at(index)->asBinaryExpression();
            if (expression)
                break;
        }

        if (! expression)
            return result;

        if (! interface_->isCursorOn(expression->binary_op_token))
            return result;

        QSharedPointer<Operation> op(new Operation(interface_));

        if (expression->match(op->pattern, &matcher) &&
                file->tokenAt(op->pattern->binary_op_token).is(T_AMPER_AMPER) &&
                file->tokenAt(op->left->unary_op_token).is(T_EXCLAIM) &&
                file->tokenAt(op->right->unary_op_token).is(T_EXCLAIM)) {
            op->setDescription(QApplication::translate("CppTools::QuickFix", "Rewrite Condition Using ||"));
            op->setPriority(index);
            result.append(op);
        }

        return result;
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        QSharedPointer<ASTPatternBuilder> mk;
        UnaryExpressionAST *left;
        UnaryExpressionAST *right;
        BinaryExpressionAST *pattern;

        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
            : CppQuickFixOperation(interface_)
            , mk(new ASTPatternBuilder)
        {
            left = mk->UnaryExpression();
            right = mk->UnaryExpression();
            pattern = mk->BinaryExpression(left, right);
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            changes.replace(currentFile->range(pattern->binary_op_token), QLatin1String("||"));
            changes.remove(currentFile->range(left->unary_op_token));
            changes.remove(currentFile->range(right->unary_op_token));
            const int start = currentFile->startOf(pattern);
            const int end = currentFile->endOf(pattern);
            changes.insert(start, QLatin1String("!("));
            changes.insert(end, QLatin1String(")"));

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(currentFile->range(pattern));
            currentFile->apply();
        }
    };

private:
    ASTMatcher matcher;
};

/*
    Rewrite
    int *a, b;

    As
    int *a;
    int b;

    Activates on: the type or the variable names.
*/
class SplitSimpleDeclarationOp: public CppQuickFixFactory
{
    static bool checkDeclaration(SimpleDeclarationAST *declaration)
    {
        if (! declaration->semicolon_token)
            return false;

        if (! declaration->decl_specifier_list)
            return false;

        for (SpecifierListAST *it = declaration->decl_specifier_list; it; it = it->next) {
            SpecifierAST *specifier = it->value;

            if (specifier->asEnumSpecifier() != 0)
                return false;

            else if (specifier->asClassSpecifier() != 0)
                return false;
        }

        if (! declaration->declarator_list)
            return false;

        else if (! declaration->declarator_list->next)
            return false;

        return true;
    }

public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        QList<CppQuickFixOperation::Ptr> result;
        CoreDeclaratorAST *core_declarator = 0;
        const QList<AST *> &path = interface_->path();
        CppRefactoringFilePtr file = interface_->currentFile();

        for (int index = path.size() - 1; index != -1; --index) {
            AST *node = path.at(index);

            if (CoreDeclaratorAST *coreDecl = node->asCoreDeclarator())
                core_declarator = coreDecl;

            else if (SimpleDeclarationAST *simpleDecl = node->asSimpleDeclaration()) {
                if (checkDeclaration(simpleDecl)) {
                    SimpleDeclarationAST *declaration = simpleDecl;

                    const int cursorPosition = file->cursor().selectionStart();

                    const int startOfDeclSpecifier = file->startOf(declaration->decl_specifier_list->firstToken());
                    const int endOfDeclSpecifier = file->endOf(declaration->decl_specifier_list->lastToken() - 1);

                    if (cursorPosition >= startOfDeclSpecifier && cursorPosition <= endOfDeclSpecifier) {
                        // the AST node under cursor is a specifier.
                        return singleResult(new Operation(interface_, index, declaration));
                    }

                    if (core_declarator && interface_->isCursorOn(core_declarator)) {
                        // got a core-declarator under the text cursor.
                        return singleResult(new Operation(interface_, index, declaration));
                    }
                }

                break;
            }
        }

        return result;
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, SimpleDeclarationAST *decl)
            : CppQuickFixOperation(interface_, priority)
            , declaration(decl)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Split Declaration"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &currentFile, const CppRefactoringChanges &)
        {
            ChangeSet changes;

            SpecifierListAST *specifiers = declaration->decl_specifier_list;
            int declSpecifiersStart = currentFile->startOf(specifiers->firstToken());
            int declSpecifiersEnd = currentFile->endOf(specifiers->lastToken() - 1);
            int insertPos = currentFile->endOf(declaration->semicolon_token);

            DeclaratorAST *prevDeclarator = declaration->declarator_list->value;

            for (DeclaratorListAST *it = declaration->declarator_list->next; it; it = it->next) {
                DeclaratorAST *declarator = it->value;

                changes.insert(insertPos, QLatin1String("\n"));
                changes.copy(declSpecifiersStart, declSpecifiersEnd, insertPos);
                changes.insert(insertPos, QLatin1String(" "));
                changes.move(currentFile->range(declarator), insertPos);
                changes.insert(insertPos, QLatin1String(";"));

                const int prevDeclEnd = currentFile->endOf(prevDeclarator);
                changes.remove(prevDeclEnd, currentFile->startOf(declarator));

                prevDeclarator = declarator;
            }

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(currentFile->range(declaration));
            currentFile->apply();
        }

    private:
        SimpleDeclarationAST *declaration;
    };
};

/*
    Add curly braces to a if statement that doesn't already contain a
    compound statement. I.e.

    if (a)
        b;
    becomes
    if (a) {
        b;
    }

    Activates on: the if
*/
class AddBracesToIfOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();

        // show when we're on the 'if' of an if statement
        int index = path.size() - 1;
        IfStatementAST *ifStatement = path.at(index)->asIfStatement();
        if (ifStatement && interface_->isCursorOn(ifStatement->if_token) && ifStatement->statement
            && ! ifStatement->statement->asCompoundStatement()) {
            return singleResult(new Operation(interface_, index, ifStatement->statement));
        }

        // or if we're on the statement contained in the if
        // ### This may not be such a good idea, consider nested ifs...
        for (; index != -1; --index) {
            IfStatementAST *ifStatement = path.at(index)->asIfStatement();
            if (ifStatement && ifStatement->statement
                && interface_->isCursorOn(ifStatement->statement)
                && ! ifStatement->statement->asCompoundStatement()) {
                return singleResult(new Operation(interface_, index, ifStatement->statement));
            }
        }

        // ### This could very well be extended to the else branch
        // and other nodes entirely.

        return noResult();
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, StatementAST *statement)
            : CppQuickFixOperation(interface_, priority)
            , _statement(statement)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Add Curly Braces"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            const int start = currentFile->endOf(_statement->firstToken() - 1);
            changes.insert(start, QLatin1String(" {"));

            const int end = currentFile->endOf(_statement->lastToken() - 1);
            changes.insert(end, "\n}");

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(Utils::ChangeSet::Range(start, end));
            currentFile->apply();
        }

    private:
        StatementAST *_statement;
    };
};

/*
    Replace
    if (Type name = foo()) {...}

    With
    Type name = foo;
    if (name) {...}

    Activates on: the name of the introduced variable
*/
class MoveDeclarationOutOfIfOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();
        QSharedPointer<Operation> op(new Operation(interface_));

        int index = path.size() - 1;
        for (; index != -1; --index) {
            if (IfStatementAST *statement = path.at(index)->asIfStatement()) {
                if (statement->match(op->pattern, &op->matcher) && op->condition->declarator) {
                    DeclaratorAST *declarator = op->condition->declarator;
                    op->core = declarator->core_declarator;
                    if (! op->core)
                        return noResult();

                    if (interface_->isCursorOn(op->core)) {
                        QList<CppQuickFixOperation::Ptr> result;
                        op->setPriority(index);
                        result.append(op);
                        return result;
                    }
                }
            }
        }

        return noResult();
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
            : CppQuickFixOperation(interface_)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Move Declaration out of Condition"));

            condition = mk.Condition();
            pattern = mk.IfStatement(condition);
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            changes.copy(currentFile->range(core), currentFile->startOf(condition));

            int insertPos = currentFile->startOf(pattern);
            changes.move(currentFile->range(condition), insertPos);
            changes.insert(insertPos, QLatin1String(";\n"));

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(currentFile->range(pattern));
            currentFile->apply();
        }

        ASTMatcher matcher;
        ASTPatternBuilder mk;
        CPPEditorWidget *editor;
        ConditionAST *condition;
        IfStatementAST *pattern;
        CoreDeclaratorAST *core;
    };
};

/*
    Replace
    while (Type name = foo()) {...}

    With
    Type name;
    while ((name = foo()) != 0) {...}

    Activates on: the name of the introduced variable
*/
class MoveDeclarationOutOfWhileOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();
        QSharedPointer<Operation> op(new Operation(interface_));

        int index = path.size() - 1;
        for (; index != -1; --index) {
            if (WhileStatementAST *statement = path.at(index)->asWhileStatement()) {
                if (statement->match(op->pattern, &op->matcher) && op->condition->declarator) {
                    DeclaratorAST *declarator = op->condition->declarator;
                    op->core = declarator->core_declarator;

                    if (! op->core)
                        return noResult();

                    else if (! declarator->equal_token)
                        return noResult();

                    else if (! declarator->initializer)
                        return noResult();

                    if (interface_->isCursorOn(op->core)) {
                        QList<CppQuickFixOperation::Ptr> result;
                        op->setPriority(index);
                        result.append(op);
                        return result;
                    }
                }
            }
        }

        return noResult();
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
            : CppQuickFixOperation(interface_)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Move Declaration out of Condition"));

            condition = mk.Condition();
            pattern = mk.WhileStatement(condition);
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            changes.insert(currentFile->startOf(condition), QLatin1String("("));
            changes.insert(currentFile->endOf(condition), QLatin1String(") != 0"));

            int insertPos = currentFile->startOf(pattern);
            const int conditionStart = currentFile->startOf(condition);
            changes.move(conditionStart, currentFile->startOf(core), insertPos);
            changes.copy(currentFile->range(core), insertPos);
            changes.insert(insertPos, QLatin1String(";\n"));

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(currentFile->range(pattern));
            currentFile->apply();
        }

        ASTMatcher matcher;
        ASTPatternBuilder mk;
        CPPEditorWidget *editor;
        ConditionAST *condition;
        WhileStatementAST *pattern;
        CoreDeclaratorAST *core;
    };
};

/*
  Replace
     if (something && something_else) {
     }

  with
     if (something) {
        if (something_else) {
        }
     }

  and
    if (something || something_else)
      x;

  with
    if (something)
      x;
    else if (something_else)
      x;

    Activates on: && or ||
*/
class SplitIfStatementOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        IfStatementAST *pattern = 0;
        const QList<AST *> &path = interface_->path();

        int index = path.size() - 1;
        for (; index != -1; --index) {
            AST *node = path.at(index);
            if (IfStatementAST *stmt = node->asIfStatement()) {
                pattern = stmt;
                break;
            }
        }

        if (! pattern || ! pattern->statement)
            return noResult();

        unsigned splitKind = 0;
        for (++index; index < path.size(); ++index) {
            AST *node = path.at(index);
            BinaryExpressionAST *condition = node->asBinaryExpression();
            if (! condition)
                return noResult();

            Token binaryToken = interface_->currentFile()->tokenAt(condition->binary_op_token);

            // only accept a chain of ||s or &&s - no mixing
            if (! splitKind) {
                splitKind = binaryToken.kind();
                if (splitKind != T_AMPER_AMPER && splitKind != T_PIPE_PIPE)
                    return noResult();
                // we can't reliably split &&s in ifs with an else branch
                if (splitKind == T_AMPER_AMPER && pattern->else_statement)
                    return noResult();
            } else if (splitKind != binaryToken.kind()) {
                return noResult();
            }

            if (interface_->isCursorOn(condition->binary_op_token))
                return singleResult(new Operation(interface_, index, pattern, condition));
        }

        return noResult();
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority,
                  IfStatementAST *pattern, BinaryExpressionAST *condition)
            : CppQuickFixOperation(interface_, priority)
            , pattern(pattern)
            , condition(condition)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Split if Statement"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            const Token binaryToken = currentFile->tokenAt(condition->binary_op_token);

            if (binaryToken.is(T_AMPER_AMPER))
                splitAndCondition(currentFile);
            else
                splitOrCondition(currentFile);
        }

        void splitAndCondition(CppRefactoringFilePtr currentFile)
        {
            ChangeSet changes;

            int startPos = currentFile->startOf(pattern);
            changes.insert(startPos, QLatin1String("if ("));
            changes.move(currentFile->range(condition->left_expression), startPos);
            changes.insert(startPos, QLatin1String(") {\n"));

            const int lExprEnd = currentFile->endOf(condition->left_expression);
            changes.remove(lExprEnd, currentFile->startOf(condition->right_expression));
            changes.insert(currentFile->endOf(pattern), QLatin1String("\n}"));

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(currentFile->range(pattern));
            currentFile->apply();
        }

        void splitOrCondition(CppRefactoringFilePtr currentFile)
        {
            ChangeSet changes;

            StatementAST *ifTrueStatement = pattern->statement;
            CompoundStatementAST *compoundStatement = ifTrueStatement->asCompoundStatement();

            int insertPos = currentFile->endOf(ifTrueStatement);
            if (compoundStatement)
                changes.insert(insertPos, QLatin1String(" "));
            else
                changes.insert(insertPos, QLatin1String("\n"));
            changes.insert(insertPos, QLatin1String("else if ("));

            const int rExprStart = currentFile->startOf(condition->right_expression);
            changes.move(rExprStart, currentFile->startOf(pattern->rparen_token), insertPos);
            changes.insert(insertPos, QLatin1String(")"));

            const int rParenEnd = currentFile->endOf(pattern->rparen_token);
            changes.copy(rParenEnd, currentFile->endOf(pattern->statement), insertPos);

            const int lExprEnd = currentFile->endOf(condition->left_expression);
            changes.remove(lExprEnd, currentFile->startOf(condition->right_expression));

            currentFile->setChangeSet(changes);
            currentFile->appendIndentRange(currentFile->range(pattern));
            currentFile->apply();
        }

    private:
        IfStatementAST *pattern;
        BinaryExpressionAST *condition;
    };
};

/*
  Replace
    "abcd" -> QLatin1String("abcd")
    @"abcd" -> QLatin1String("abcd")
    'a' -> QLatin1Char('a')
  Except if they are already enclosed in
    QLatin1Char, QT_TRANSLATE_NOOP, tr,
    trUtf8, QLatin1Literal, QLatin1String

    Activates on: the string literal
*/

static inline QString msgQtStringLiteralDescription(const QString &replacement, int qtVersion)
{
    return QApplication::translate("CppTools::QuickFix", "Enclose in %1(...) (Qt %2)")
           .arg(replacement).arg(qtVersion);
}

class WrapStringLiteral: public CppQuickFixFactory
{
public:
    enum Type { TypeString, TypeObjCString, TypeChar, TypeNone };

    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        ExpressionAST *literal = 0;
        Type type = TypeNone;
        const QList<AST *> &path = interface_->path();
        CppRefactoringFilePtr file = interface_->currentFile();

        if (path.isEmpty())
            return noResult(); // nothing to do

        literal = path.last()->asStringLiteral();

        if (! literal) {
            literal = path.last()->asNumericLiteral();
            if (!literal || !file->tokenAt(literal->asNumericLiteral()->literal_token).is(T_CHAR_LITERAL))
                return noResult();
            else
                type = TypeChar;
        } else {
            type = TypeString;
        }

        if (path.size() > 1) {
            if (CallAST *call = path.at(path.size() - 2)->asCall()) {
                if (call->base_expression) {
                    if (IdExpressionAST *idExpr = call->base_expression->asIdExpression()) {
                        if (SimpleNameAST *functionName = idExpr->name->asSimpleName()) {
                            const QByteArray id(file->tokenAt(functionName->identifier_token).identifier->chars());

                            if (id == "QT_TRANSLATE_NOOP" || id == "tr" || id == "trUtf8"
                                    || (type == TypeString && isQtStringLiteral(id))
                                    || (type == TypeChar && id == "QLatin1Char"))
                                return noResult(); // skip it
                        }
                    }
                }
            }
        }

        if (type == TypeString) {
            if (file->charAt(file->startOf(literal)) == QLatin1Char('@'))
                type = TypeObjCString;
        }

        QList<CppQuickFixOperation::Ptr> result;

        const int priority = path.size() - 1; // very high priority
        if (type == TypeChar) {
            const QString replacement = QLatin1String("QLatin1Char");
            const QString description =
                QApplication::translate("CppTools::QuickFix", "Enclose in %1(...)")
                           .arg(replacement);

            result.push_back(CppQuickFixOperation::Ptr(new Operation(interface_, priority, type,
                                                       replacement, description, literal)));
        } else {
            QString replacement = QLatin1String("QLatin1String");
            result.push_back(CppQuickFixOperation::Ptr(new Operation(interface_, priority, type,
                                                                     replacement, msgQtStringLiteralDescription(replacement, 4),
                                                                     literal)));
            replacement = QLatin1String("QStringLiteral");
            result.push_back(CppQuickFixOperation::Ptr(new Operation(interface_, priority, type,
                                                                     replacement, msgQtStringLiteralDescription(replacement, 5),
                                                                     literal)));
        }
        return result;
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority,
                  Type type, const QString &replacement, const QString &description,
                  ExpressionAST *literal)
            : CppQuickFixOperation(interface_, priority)
            , type(type), replacement(replacement)
            , literal(literal)
        {
            setDescription(description);
       }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            const int startPos = currentFile->startOf(literal);
            const QString fullReplacement = replacement + QLatin1Char('(');
            if (type == TypeObjCString)
                changes.replace(startPos, startPos + 1, fullReplacement);
            else
                changes.insert(startPos, fullReplacement);

            changes.insert(currentFile->endOf(literal), QLatin1String(")"));

            currentFile->setChangeSet(changes);
            currentFile->apply();
        }

    private:
        const Type type;
        const QString replacement;
        ExpressionAST *literal;
    };

};

/*
  Replace
    "abcd"
  With
    tr("abcd") or
    QCoreApplication::translate("CONTEXT", "abcd") or
    QT_TRANSLATE_NOOP("GLOBAL", "abcd")
  depending on what is available.

    Activates on: the string literal
*/
class TranslateStringLiteral: public CppQuickFixFactory
{
public:
    enum TranslationOption { unknown, useTr, useQCoreApplicationTranslate, useMacro };

    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();
        // Initialize
        ExpressionAST *literal = 0;
        QString trContext;

        if (path.isEmpty())
            return noResult();

        literal = path.last()->asStringLiteral();
        if (!literal)
            return noResult(); // No string, nothing to do

        // Do we already have a translation markup?
        if (path.size() >= 2) {
            if (CallAST *call = path.at(path.size() - 2)->asCall()) {
                if (call->base_expression) {
                    if (IdExpressionAST *idExpr = call->base_expression->asIdExpression()) {
                        if (SimpleNameAST *functionName = idExpr->name->asSimpleName()) {
                            const QByteArray id(interface_->currentFile()->tokenAt(functionName->identifier_token).identifier->chars());

                            if (id == "tr" || id == "trUtf8"
                                    || id == "translate"
                                    || id == "QT_TRANSLATE_NOOP"
                                    || isQtStringLiteral(id))
                                return noResult(); // skip it
                        }
                    }
                }
            }
        }

        QSharedPointer<Control> control = interface_->context().control();
        const Name *trName = control->identifier("tr");

        // Check whether we are in a method:
        for (int i = path.size() - 1; i >= 0; --i)
        {
            if (FunctionDefinitionAST *definition = path.at(i)->asFunctionDefinition()) {
                Function *function = definition->symbol;
                ClassOrNamespace *b = interface_->context().lookupType(function);
                if (b) {
                    // Do we have a tr method?
                    foreach(const LookupItem &r, b->find(trName)) {
                        Symbol *s = r.declaration();
                        if (s->type()->isFunctionType()) {
                            // no context required for tr
                            return singleResult(new Operation(interface_, path.size() - 1, literal, useTr, trContext));
                        }
                    }
                }
                // We need to do a QCA::translate, so we need a context.
                // Use fully qualified class name:
                Overview oo;
                foreach (const Name *n, LookupContext::path(function)) {
                    if (!trContext.isEmpty())
                        trContext.append(QLatin1String("::"));
                    trContext.append(oo.prettyName(n));
                }
                // ... or global if none available!
                if (trContext.isEmpty())
                    trContext = QLatin1String("GLOBAL");
                return singleResult(new Operation(interface_, path.size() - 1, literal, useQCoreApplicationTranslate, trContext));
            }
        }

        // We need to use Q_TRANSLATE_NOOP
        return singleResult(new Operation(interface_, path.size() - 1, literal, useMacro, QLatin1String("GLOBAL")));
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, ExpressionAST *literal, TranslationOption option, const QString &context)
            : CppQuickFixOperation(interface_, priority)
            , m_literal(literal)
            , m_option(option)
            , m_context(context)
        {
            setDescription(QApplication::translate("CppTools::QuickFix", "Mark as Translatable"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            const int startPos = currentFile->startOf(m_literal);
            QString replacement(QLatin1String("tr("));
            if (m_option == useQCoreApplicationTranslate) {
                replacement = QLatin1String("QCoreApplication::translate(\"")
                        + m_context + QLatin1String("\", ");
            } else if (m_option == useMacro) {
                replacement = QLatin1String("QT_TRANSLATE_NOOP(\"")
                        + m_context + QLatin1String("\", ");
            }

            changes.insert(startPos, replacement);
            changes.insert(currentFile->endOf(m_literal), QLatin1String(")"));

            currentFile->setChangeSet(changes);
            currentFile->apply();
        }

    private:
        ExpressionAST *m_literal;
        TranslationOption m_option;
        QString m_context;
    };
};

/**
 * Replace
 *    "abcd"
 *    QLatin1String("abcd")
 *    QLatin1Literal("abcd")
 * With
 *    @"abcd"
 *
 * Activates on: the string literal, if the file type is a Objective-C(++) file.
 */
class CStringToNSString: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        CppRefactoringFilePtr file = interface_->currentFile();

        if (interface_->editor()->mimeType() != CppTools::Constants::OBJECTIVE_CPP_SOURCE_MIMETYPE)
            return noResult();

        StringLiteralAST *stringLiteral = 0;
        CallAST *qlatin1Call = 0;
        const QList<AST *> &path = interface_->path();

        if (path.isEmpty())
            return noResult(); // nothing to do

        stringLiteral = path.last()->asStringLiteral();

        if (! stringLiteral)
            return noResult();

        else if (file->charAt(file->startOf(stringLiteral)) == QLatin1Char('@'))
            return noResult(); // it's already an objc string literal.

        else if (path.size() > 1) {
            if (CallAST *call = path.at(path.size() - 2)->asCall()) {
                if (call->base_expression) {
                    if (IdExpressionAST *idExpr = call->base_expression->asIdExpression()) {
                        if (SimpleNameAST *functionName = idExpr->name->asSimpleName()) {
                            const QByteArray id(interface_->currentFile()->tokenAt(functionName->identifier_token).identifier->chars());

                            if (isQtStringLiteral(id))
                                qlatin1Call = call;
                        }
                    }
                }
            }
        }

        return singleResult(new Operation(interface_, path.size() - 1, stringLiteral, qlatin1Call));
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, StringLiteralAST *stringLiteral, CallAST *qlatin1Call)
            : CppQuickFixOperation(interface_, priority)
            , stringLiteral(stringLiteral)
            , qlatin1Call(qlatin1Call)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Convert to Objective-C String Literal"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            if (qlatin1Call) {
                changes.replace(currentFile->startOf(qlatin1Call), currentFile->startOf(stringLiteral), QLatin1String("@"));
                changes.remove(currentFile->endOf(stringLiteral), currentFile->endOf(qlatin1Call));
            } else {
                changes.insert(currentFile->startOf(stringLiteral), "@");
            }

            currentFile->setChangeSet(changes);
            currentFile->apply();
        }

    private:
        StringLiteralAST *stringLiteral;
        CallAST *qlatin1Call;
    };
};

/*
  Base class for converting numeric literals between decimal, octal and hex.
  Does the base check for the specific ones and parses the number.
  Test cases:
    0xFA0Bu;
    0X856A;
    298.3;
    199;
    074;
    199L;
    074L;
    -199;
    -017;
    0783; // invalid octal
    0; // border case, allow only hex<->decimal

    Activates on: numeric literals
*/
class ConvertNumericLiteral: public CppQuickFixFactory
{
public:
    virtual QList<QuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        QList<QuickFixOperation::Ptr> result;

        const QList<AST *> &path = interface_->path();
        CppRefactoringFilePtr file = interface_->currentFile();

        if (path.isEmpty())
            return result; // nothing to do

        NumericLiteralAST *literal = path.last()->asNumericLiteral();

        if (! literal)
            return result;

        Token token = file->tokenAt(literal->asNumericLiteral()->literal_token);
        if (!token.is(T_NUMERIC_LITERAL))
            return result;
        const NumericLiteral *numeric = token.number;
        if (numeric->isDouble() || numeric->isFloat())
            return result;

        // remove trailing L or U and stuff
        const char * const spell = numeric->chars();
        int numberLength = numeric->size();
        while (numberLength > 0 && (spell[numberLength-1] < '0' || spell[numberLength-1] > 'F'))
            --numberLength;
        if (numberLength < 1)
            return result;

        // convert to number
        bool valid;
        ulong value = QString::fromUtf8(spell).left(numberLength).toULong(&valid, 0);
        if (!valid) // e.g. octal with digit > 7
            return result;

        const int priority = path.size() - 1; // very high priority
        const int start = file->startOf(literal);
        const char * const str = numeric->chars();

        if (!numeric->isHex()) {
            /*
              Convert integer literal to hex representation.
              Replace
                32
                040
              With
                0x20

            */
            QString replacement;
            replacement.sprintf("0x%lX", value);
            QuickFixOperation::Ptr op(new ConvertNumeric(interface_, start, start + numberLength, replacement));
            op->setDescription(QApplication::translate("CppTools::QuickFix", "Convert to Hexadecimal"));
            op->setPriority(priority);
            result.append(op);
        }

        if (value != 0) {
            if (!(numberLength > 1 && str[0] == '0' && str[1] != 'x' && str[1] != 'X')) {
                /*
                  Convert integer literal to octal representation.
                  Replace
                    32
                    0x20
                  With
                    040
                */
                QString replacement;
                replacement.sprintf("0%lo", value);
                QuickFixOperation::Ptr op(new ConvertNumeric(interface_, start, start + numberLength, replacement));
                op->setDescription(QApplication::translate("CppTools::QuickFix", "Convert to Octal"));
                op->setPriority(priority);
                result.append(op);
            }
        }

        if (value != 0 || numeric->isHex()) {
            if (!(numberLength > 1 && str[0] != '0')) {
                /*
                  Convert integer literal to decimal representation.
                  Replace
                    0x20
                    040
                  With
                    32
                */
                QString replacement;
                replacement.sprintf("%lu", value);
                QuickFixOperation::Ptr op(new ConvertNumeric(interface_, start, start + numberLength, replacement));
                op->setDescription(QApplication::translate("CppTools::QuickFix", "Convert to Decimal"));
                op->setPriority(priority);
                result.append(op);
            }
        }

        return result;
    }

private:
    class ConvertNumeric: public CppQuickFixOperation
    {
    public:
        ConvertNumeric(const QSharedPointer<const CppQuickFixAssistInterface> &interface_,
                       int start, int end, const QString &replacement)
            : CppQuickFixOperation(interface_)
            , start(start)
            , end(end)
            , replacement(replacement)
        {}

        virtual void performChanges(const CppRefactoringFilePtr &, const CppRefactoringChanges &)
        {
            ChangeSet changes;
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            changes.replace(start, end, replacement);
            currentFile->setChangeSet(changes);
            currentFile->apply();
        }

    protected:
        int start, end;
        QString replacement;
    };
};

/*
    Can be triggered on a class forward declaration to add the matching #include.

    Activates on: the name of a forward-declared class or struct
*/
class FixForwardDeclarationOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();

        for (int index = path.size() - 1; index != -1; --index) {
            AST *ast = path.at(index);
            if (NamedTypeSpecifierAST *namedTy = ast->asNamedTypeSpecifier()) {
                if (Symbol *fwdClass = checkName(interface_, namedTy->name))
                    return singleResult(new Operation(interface_, index, fwdClass));
            } else if (ElaboratedTypeSpecifierAST *eTy = ast->asElaboratedTypeSpecifier()) {
                if (Symbol *fwdClass = checkName(interface_, eTy->name))
                    return singleResult(new Operation(interface_, index, fwdClass));
            }
        }

        return noResult();
    }

protected:
    static Symbol *checkName(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, NameAST *ast)
    {
        if (ast && interface_->isCursorOn(ast)) {
            if (const Name *name = ast->name) {
                unsigned line, column;
                interface_->semanticInfo().doc->translationUnit()->getTokenStartPosition(ast->firstToken(), &line, &column);

                Symbol *fwdClass = 0;

                foreach (const LookupItem &r,
                         interface_->context().lookup(name,
                                                     interface_->semanticInfo().doc->scopeAt(line, column))) {
                    if (! r.declaration())
                        continue;
                    else if (ForwardClassDeclaration *fwd = r.declaration()->asForwardClassDeclaration())
                        fwdClass = fwd;
                    else if (r.declaration()->isClass())
                        return 0; // nothing to do.
                }

                return fwdClass;
            }
        }

        return 0;
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, Symbol *fwdClass)
            : CppQuickFixOperation(interface_, priority)
            , fwdClass(fwdClass)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "#include Header File"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &,
                                    const CppRefactoringChanges &)
        {
            Q_ASSERT(fwdClass != 0);

            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            if (Class *k = assistInterface()->snapshot().findMatchingClassDeclaration(fwdClass)) {
                const QString headerFile = QString::fromUtf8(k->fileName(), k->fileNameLength());

                // collect the fwd headers
                Snapshot fwdHeaders;
                fwdHeaders.insert(assistInterface()->snapshot().document(headerFile));
                foreach (Document::Ptr doc, assistInterface()->snapshot()) {
                    QFileInfo headerFileInfo(doc->fileName());
                    if (doc->globalSymbolCount() == 0 && doc->includes().size() == 1)
                        fwdHeaders.insert(doc);
                    else if (headerFileInfo.suffix().isEmpty())
                        fwdHeaders.insert(doc);
                }


                DependencyTable dep;
                dep.build(fwdHeaders);
                QStringList candidates = dep.dependencyTable().value(headerFile);

                const QString className = QString::fromUtf8(k->identifier()->chars());

                QString best;
                foreach (const QString &c, candidates) {
                    QFileInfo headerFileInfo(c);
                    if (headerFileInfo.fileName() == className) {
                        best = c;
                        break;
                    } else if (headerFileInfo.fileName().at(0).isUpper()) {
                        best = c;
                        // and continue
                    } else if (! best.isEmpty()) {
                        if (c.count(QLatin1Char('/')) < best.count(QLatin1Char('/')))
                            best = c;
                    }
                }

                if (best.isEmpty())
                    best = headerFile;

                int pos = currentFile->startOf(1);

                unsigned currentLine = currentFile->cursor().blockNumber() + 1;
                unsigned bestLine = 0;
                foreach (const Document::Include &incl, assistInterface()->semanticInfo().doc->includes()) {
                    if (incl.line() < currentLine)
                        bestLine = incl.line();
                }

                if (bestLine)
                    pos = currentFile->document()->findBlockByNumber(bestLine).position();

                Utils::ChangeSet changes;
                changes.insert(pos, QString("#include <%1>\n").arg(QFileInfo(best).fileName()));
                currentFile->setChangeSet(changes);
                currentFile->apply();
            }
        }

    private:
        Symbol *fwdClass;
    };
};

/*
  Rewrites
    a = foo();
  As
    Type a = foo();
  Where Type is the return type of foo()

    Activates on: the assignee, if the type of the right-hand side of the assignment is known.
*/
class AddLocalDeclarationOp: public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();
        CppRefactoringFilePtr file = interface_->currentFile();

        for (int index = path.size() - 1; index != -1; --index) {
            if (BinaryExpressionAST *binary = path.at(index)->asBinaryExpression()) {
                if (binary->left_expression && binary->right_expression && file->tokenAt(binary->binary_op_token).is(T_EQUAL)) {
                    IdExpressionAST *idExpr = binary->left_expression->asIdExpression();
                    if (interface_->isCursorOn(binary->left_expression) && idExpr && idExpr->name->asSimpleName() != 0) {
                        SimpleNameAST *nameAST = idExpr->name->asSimpleName();
                        const QList<LookupItem> results = interface_->context().lookup(nameAST->name, file->scopeAt(nameAST->firstToken()));
                        Declaration *decl = 0;
                        foreach (const LookupItem &r, results) {
                            if (! r.declaration())
                                continue;
                            else if (Declaration *d = r.declaration()->asDeclaration()) {
                                if (! d->type()->isFunctionType()) {
                                    decl = d;
                                    break;
                                }
                            }
                        }

                        if (! decl) {
                            return singleResult(new Operation(interface_, index, binary));
                        }
                    }
                }
            }
        }

        return noResult();
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, BinaryExpressionAST *binaryAST)
            : CppQuickFixOperation(interface_, priority)
            , binaryAST(binaryAST)
        {
            setDescription(QApplication::translate("CppTools::QuickFix", "Add Local Declaration"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &,
                                    const CppRefactoringChanges &)
        {
            CppRefactoringFilePtr currentFile = assistInterface()->currentFile();

            TypeOfExpression typeOfExpression;
            typeOfExpression.init(assistInterface()->semanticInfo().doc,
                                  assistInterface()->snapshot(), assistInterface()->context().bindings());
            Scope *scope = currentFile->scopeAt(binaryAST->firstToken());
            const QList<LookupItem> result = typeOfExpression(currentFile->textOf(binaryAST->right_expression),
                                                              scope,
                                                              TypeOfExpression::Preprocess);

            if (! result.isEmpty()) {

                SubstitutionEnvironment env;
                env.setContext(assistInterface()->context());
                env.switchScope(result.first().scope());
                ClassOrNamespace *con = typeOfExpression.context().lookupType(scope);
                if (!con)
                    con = typeOfExpression.context().globalNamespace();
                UseMinimalNames q(con);
                env.enter(&q);

                Control *control = assistInterface()->context().control().data();
                FullySpecifiedType tn = rewriteType(result.first().type(), &env, control);

                Overview oo;
                QString ty = oo(tn);
                if (! ty.isEmpty()) {
                    const QChar ch = ty.at(ty.size() - 1);

                    if (ch.isLetterOrNumber() || ch == QLatin1Char(' ') || ch == QLatin1Char('>'))
                        ty += QLatin1Char(' ');

                    Utils::ChangeSet changes;
                    changes.insert(currentFile->startOf(binaryAST), ty);
                    currentFile->setChangeSet(changes);
                    currentFile->apply();
                }
            }
        }

    private:
        BinaryExpressionAST *binaryAST;
    };
};

/**
 * Turns "an_example_symbol" into "anExampleSymbol" and
 * "AN_EXAMPLE_SYMBOL" into "AnExampleSymbol".
 *
 * Activates on: identifiers
 */
class ToCamelCaseConverter : public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface_)
    {
        const QList<AST *> &path = interface_->path();

        if (path.isEmpty())
            return noResult();

        AST * const ast = path.last();
        const Name *name = 0;
        if (const NameAST * const nameAst = ast->asName()) {
            if (nameAst->name && nameAst->name->asNameId())
                name = nameAst->name;
        } else if (const NamespaceAST * const namespaceAst = ast->asNamespace()) {
            name = namespaceAst->symbol->name();
        }

        if (!name)
            return noResult();

        QString newName = QString::fromUtf8(name->identifier()->chars());
        if (newName.length() < 3)
            return noResult();
        for (int i = 1; i < newName.length() - 1; ++i) {
            if (Operation::isConvertibleUnderscore(newName, i))
                return singleResult(new Operation(interface_, path.size() - 1, newName));
        }

        return noResult();
    }

private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, const QString &newName)
            : CppQuickFixOperation(interface_, priority)
            , m_name(newName)
        {
            setDescription(QApplication::translate("CppTools::QuickFix",
                                                   "Convert to Camel Case"));
        }

        virtual void performChanges(const CppRefactoringFilePtr &,
                                    const CppRefactoringChanges &)
        {
            for (int i = 1; i < m_name.length(); ++i) {
                QCharRef c = m_name[i];
                if (c.isUpper()) {
                    c = c.toLower();
                } else if (i < m_name.length() - 1
                           && isConvertibleUnderscore(m_name, i)) {
                    m_name.remove(i, 1);
                    m_name[i] = m_name.at(i).toUpper();
                }
            }
            static_cast<CppEditor::Internal::CPPEditorWidget*>(assistInterface()->editor())->renameUsagesNow(m_name);
        }

        static bool isConvertibleUnderscore(const QString &name, int pos)
        {
            return name.at(pos) == QLatin1Char('_') && name.at(pos+1).isLetter()
                    && !(pos == 1 && name.at(0) == QLatin1Char('m'));
        }

    private:
        QString m_name;
    };
};

ProjectPart::HeaderPaths relevantHeaderPaths(const QString &filePath)
{
    ProjectPart::HeaderPaths headerPaths;

    CppTools::Internal::CppModelManager *modelManager = CppTools::Internal::CppModelManager::instance();
    const QList<ProjectPart::Ptr> projectParts = modelManager->projectPart(filePath);
    if (projectParts.isEmpty()) { // Not part of any project, better use all include paths than none
        headerPaths += modelManager->headerPaths();
    } else {
        foreach (const ProjectPart::Ptr &part, projectParts)
            headerPaths += part->headerPaths;
    }

    return headerPaths;
}

NameAST *nameUnderCursor(const QList<AST *> &path)
{
    if (path.isEmpty())
        return 0;

    NameAST *nameAst = 0;
    for (int i = path.size() - 1; i >= 0; --i) {
        AST * const ast = path.at(i);
        if (SimpleNameAST *simpleName = ast->asSimpleName()) {
            nameAst = simpleName;
        } else if (TemplateIdAST *templateId = ast->asTemplateId()) {
            nameAst = templateId;
        } else if (nameAst && ast->asNamedTypeSpecifier()) {
            break; // Stop at "Foo" for "N::Bar<@Foo>"
        } else if (QualifiedNameAST *qualifiedName = ast->asQualifiedName()) {
            nameAst = qualifiedName;
            break;
        }
    }

    return nameAst;
}

bool canLookupDefinition(const QSharedPointer<const CppQuickFixAssistInterface> &interface, const NameAST *nameAst)
{
    QTC_ASSERT(nameAst, return false);

    // Find the enclosing scope
    unsigned line, column;
    const Document::Ptr &doc = interface->semanticInfo().doc;
    doc->translationUnit()->getTokenStartPosition(nameAst->firstToken(), &line, &column);
    Scope *scope = doc->scopeAt(line, column);
    if (!scope)
        return false;

    // Try to find the class/template definition
    const Name *name = nameAst->name;
    const QList<LookupItem> results = interface->context().lookup(name, scope);
    foreach (const LookupItem &item, results) {
        if (Symbol *declaration = item.declaration()) {
            if (declaration->isClass())
                return true;
            if (Template *templ = declaration->asTemplate()) {
                Symbol *declaration = templ->declaration();
                if (declaration && declaration->isClass())
                    return true;
            }
        }
    }

    return false;
}

QString templateNameAsString(const TemplateNameId *templateName)
{
    const Identifier *id = templateName->identifier();
    return QString::fromUtf8(id->chars(), id->size());
}

// For templates, simply the name is returned, without '<...>'.
QString unqualifiedNameForLocator(const Name *name)
{
    QTC_ASSERT(name, return QString());

    const Overview oo;
    if (const QualifiedNameId *qualifiedName = name->asQualifiedNameId()) {
        const Name *name = qualifiedName->name();
        if (const TemplateNameId *templateName = name->asTemplateNameId())
            return templateNameAsString(templateName);
        return oo.prettyName(name);
    } else if (const TemplateNameId *templateName = name->asTemplateNameId()) {
        return templateNameAsString(templateName);
    } else {
        return oo.prettyName(name);
    }
}

Snapshot forwardingHeaders(const QSharedPointer<const CppQuickFixAssistInterface> &interface)
{
    Snapshot result;

    foreach (Document::Ptr doc, interface->snapshot()) {
        if (doc->globalSymbolCount() == 0 && /* TODO: doc->resolvedIncludes()*/ doc->includes().size() == 1)
            result.insert(doc);
    }

    return result;
}

QString findShortestInclude(const QString currentDocumentFilePath,
                            const QString candidateFilePath,
                            const ProjectPart::HeaderPaths &headerPaths)
{
    QString result;

    const QFileInfo fileInfo(candidateFilePath);

    if (fileInfo.path() == QFileInfo(currentDocumentFilePath).path()) {
        result = QLatin1Char('"') + fileInfo.fileName() + QLatin1Char('"');
    } else {
        foreach (const ProjectPart::HeaderPath &headerPath, headerPaths) {
            if (!candidateFilePath.startsWith(headerPath.path))
                continue;
            QString relativePath = candidateFilePath.mid(headerPath.path.size());
            if (!relativePath.isEmpty() && relativePath.at(0) == QLatin1Char('/'))
                relativePath = relativePath.mid(1);
            if (result.isEmpty() || relativePath.size() + 2 < result.size())
                result = QLatin1Char('<') + relativePath + QLatin1Char('>');
        }
    }

    return result;
}

bool looksLikeAQtClass(const QString &identifier)
{
    return identifier.size() > 2
        && identifier.at(0) == QLatin1Char('Q')
        && identifier.at(1).isUpper();
}

void insertNewIncludeDirective(const QString &include, const CppRefactoringFilePtr &file,
                               const Document::Ptr &cppDocument,
                               QTextDocument *textDocument)
{

    // Construct text to insert
    const QString includeLine = QLatin1String("#include ") + include + QLatin1Char('\n');

    // Find optimal position
    int line = 0;
    if ( !cppDocument->includes().isEmpty() )
        line = cppDocument->includes().last().line();
    else
        line = CPlusPlus::SimpleLexer::skipFirstComment(textDocument);
    if ( textDocument->blockCount() < line ) {
        line = textDocument->blockCount() - 1;
    }

    // Insert
    ChangeSet changes;
    changes.insert(textDocument->findBlockByLineNumber(line).position(), includeLine);
    file->setChangeSet(changes);
    file->apply();
}

QString findQtIncludeWithSameName(const QString &className,
                                  const ProjectPart::HeaderPaths &headerPaths)
{
    QString result;

    // Check for a header file with the same name in the Qt include paths
    foreach (const ProjectPart::HeaderPath &headerPath, headerPaths) {
        if (!headerPath.path.contains(QLatin1String("/Qt"))) // "QtCore", "QtGui" etc...
            continue;

        const QString headerPathCandidate = headerPath.path + QLatin1Char('/') + className;
        const QFileInfo fileInfo(headerPathCandidate);
        if (fileInfo.exists() && fileInfo.isFile()) {
            result = QLatin1Char('<') + className + QLatin1Char('>');
            break;
        }
    }

    return result;
}

class AddIncludeForUndefinedIdentifier : public CppQuickFixFactory
{
public:
    virtual QList<CppQuickFixOperation::Ptr> match(const QSharedPointer<const CppQuickFixAssistInterface> &interface)
    {
        const NameAST *nameAst = nameUnderCursor(interface->path());
        if (!nameAst)
            return noResult();

        if (canLookupDefinition(interface, nameAst))
            return noResult();

        const QString className = unqualifiedNameForLocator(nameAst->name);
        if (className.isEmpty())
            return noResult();

        const QString currentDocumentFilePath = interface->semanticInfo().doc->fileName();
        const ProjectPart::HeaderPaths headerPaths = relevantHeaderPaths(currentDocumentFilePath);
        bool qtHeaderFileIncludeOffered = false;

        QList<CppQuickFixOperation::Ptr> result;

        // Find an include file through the locator
        if (CppClassesFilter *classesFilter = LocatorManager::instance()->filter<CppClassesFilter>()) {
            QFutureInterface<Core::LocatorFilterEntry> dummy;
            const QList<Core::LocatorFilterEntry> matches = classesFilter->matchesFor(dummy, className);

            const Snapshot forwardHeaders = forwardingHeaders(interface);
            foreach (const Core::LocatorFilterEntry &entry, matches) {
                IndexItem::Ptr info = entry.internalData.value<IndexItem::Ptr>();
                if (info->symbolName() != className)
                    continue;

                Snapshot localForwardHeaders = forwardHeaders;
                localForwardHeaders.insert(interface->snapshot().document(info->fileName()));
                QStringList headerAndItsForwardingHeaders;
                headerAndItsForwardingHeaders << info->fileName();
                headerAndItsForwardingHeaders += localForwardHeaders.filesDependingOn(info->fileName());

                foreach (const QString &header, headerAndItsForwardingHeaders) {
                    const QString include = findShortestInclude(currentDocumentFilePath,
                                                                header,
                                                                headerPaths);
                    if (include.size() > 2) {
                        const QString headerFileName = Utils::FileName::fromString(info->fileName()).fileName();
                        QTC_ASSERT(!headerFileName.isEmpty(), break);

                        int priority = 0;
                        if (headerFileName == className)
                            priority = 2;
                        else if (headerFileName.at(1).isUpper())
                            priority = 1;

                        if (looksLikeAQtClass(include.mid(1, include.size() - 2)))
                            qtHeaderFileIncludeOffered = true;

                        result.append(CppQuickFixOperation::Ptr(new Operation(interface, priority, include, interface->semanticInfo().doc)));
                    }
                }
            }
        }

        // The header file we are looking for might not be (yet) included in any file we have parsed.
        // As such, it will not be findable via locator. At least for Qt classes, check also for
        // headers with the same name.
        if (!qtHeaderFileIncludeOffered && looksLikeAQtClass(className)) {
            const QString include = findQtIncludeWithSameName(className, headerPaths);
            if (!include.isEmpty())
                result.append(CppQuickFixOperation::Ptr(new Operation(interface, 1, include, interface->semanticInfo().doc)));
        }
        return result;
    }
private:
    class Operation: public CppQuickFixOperation
    {
    public:
        Operation(const QSharedPointer<const CppQuickFixAssistInterface> &interface_, int priority, const QString &include, const Document::Ptr &cppDocument)
            : CppQuickFixOperation(interface_, priority)
            , m_include(include)
            , m_cppDocument(cppDocument)
        {
            setDescription(QApplication::translate("CppTools::QuickFix", "Add #include %1").arg(m_include));
        }

        virtual void performChanges(const CppRefactoringFilePtr &file,
                                    const CppRefactoringChanges &)
        {
            insertNewIncludeDirective(m_include, file, m_cppDocument,
                                      static_cast<CppEditor::Internal::CPPEditorWidget*>(assistInterface()->editor())->editor()->document());
        }

    private:
        QString m_include;
        const Document::Ptr m_cppDocument;
    };
};

} // end of anonymous namespace

void registerQuickFixes( QList<TextEditor::QuickFixFactory *> &fixFactories )
{
    fixFactories << new AddIncludeForUndefinedIdentifier
                 << new UseInverseOp
                 << new FlipBinaryOp
                 << new RewriteLogicalAndOp
                 << new SplitSimpleDeclarationOp
                 << new AddBracesToIfOp
                 << new MoveDeclarationOutOfIfOp
                 << new MoveDeclarationOutOfWhileOp
                 << new SplitIfStatementOp
                 << new WrapStringLiteral
                 << new TranslateStringLiteral
                 << new CStringToNSString
                 << new ConvertNumericLiteral
                 << new CompleteSwitchCaseStatement
                 << new FixForwardDeclarationOp
                 << new AddLocalDeclarationOp
                 << new ToCamelCaseConverter
                 << new InsertQtPropertyMembers
                 << new DeclFromDef
                 << new DefFromDecl
                 << new GenerateGetterSetter/*
                 << new ApplyDeclDefLinkChanges*/;
}
