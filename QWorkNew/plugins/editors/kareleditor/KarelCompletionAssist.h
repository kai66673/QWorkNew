#ifndef KARELCOMPLETIONASSIST_H
#define KARELCOMPLETIONASSIST_H

#include <ifile.h>
#include <completionassistprovider.h>
#include <basicproposalitem.h>
#include <codeassist/defaultassistinterface.h>
#include <codeassist/iassistprocessor.h>
#include <snippets/snippetassistcollector.h>

#include "KarelDocument.h"

namespace Karel {

class KarelCompletionAssistInterface;

class KarelCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
public:
    KarelCompletionAssistProvider();
    virtual ~KarelCompletionAssistProvider();

    virtual bool isAsynchronous() const;
    virtual bool supportsEditor( const Core::Context &context ) const;
    virtual int activationCharSequenceLength() const;
    virtual bool isActivationCharSequence(const QString &sequence) const;
    virtual TextEditor::IAssistProcessor *createProcessor() const;
};

class KarelCompletionAssistProcessor : public TextEditor::IAssistProcessor
{
public:
    KarelCompletionAssistProcessor();
    virtual ~KarelCompletionAssistProcessor();

    virtual TextEditor::IAssistProposal *perform(const TextEditor::IAssistInterface *_interface);

private:
    bool accepts();
    int startOfOperator(int pos, unsigned *kind, bool wantFunctionCall) const;
    int findStartOfName(int pos = -1) const;
    int startCompletionHelper();
    Scope *currentScope(bool &isStatementsArea);

    int completeDirectives();
    int completeGlobals(Scope *scope, bool isStatementsArea);
    int completeMembers(Scope *scope, int endOfExpression);

    void addCompletionItem(const QString &text,
                           const QIcon &icon = QIcon(),
                           int order = 0,
                           const QVariant &data = QVariant());
    void addSnippets();
    void addTypes(Scope *scope);
    void addBuiltingsAndKeywords();

    QScopedPointer<const KarelCompletionAssistInterface> m_interface;
    int  m_startPosition;
    unsigned m_completionOperator;
    QList<TextEditor::BasicProposalItem *> m_completions;
    TextEditor::SnippetAssistCollector m_snippetCollector;
};

class KarelCompletionAssistInterface: public TextEditor::DefaultAssistInterface
{
public:
    KarelCompletionAssistInterface(QTextDocument *textDocument,
                                   int position,
                                   Core::IFile *file,
                                   TextEditor::AssistReason reason,
                                   Document::Ptr document);

    Document::Ptr document() const;

private:
    Document::Ptr m_document;
};

}   // namespace Karel

#endif // KARELCOMPLETIONASSIST_H
