#ifndef __PYTHONCOMPLETIOTIONASSIST_H

#define __PYTHONCOMPLETIOTIONASSIST_H

#include <QScopedPointer>

#include <completionassistprovider.h>
#include <iassistprocessor.h>
#include <defaultassistinterface.h>
#include <basicproposalitem.h>

#include <snippets/snippetassistcollector.h>

namespace Python {
class IPythonModule;
}

class PythonCompletionAssistInterface;

class PythonCompletionProvider : public TextEditor::CompletionAssistProvider
{
public:
    PythonCompletionProvider();

    virtual bool supportsEditor( const Core::Context &/*context*/ ) const { return true; }
    virtual int activationCharSequenceLength() const;
    virtual bool isActivationCharSequence(const QString &sequence) const;
    virtual TextEditor::IAssistProcessor *createProcessor() const;
    virtual bool isAsynchronous() const { return false; }
};

class PythonCompletionProcessor : public TextEditor::IAssistProcessor
{
public:
    enum CompletionKind {
        T_EOF_SYMBOL,
        T_IDENTIFIER,
        T_DOT
    };

    PythonCompletionProcessor();
    virtual ~PythonCompletionProcessor();

    virtual TextEditor::IAssistProposal *perform( const TextEditor::IAssistInterface *interface_ );

private:
    int  findStartOfName( int pos );
    void getExpression( int dotPos, QString &expr );
    bool accepts();
    int  startCompletionHelper();

    QScopedPointer<const TextEditor::DefaultAssistInterface> m_interface;
    unsigned m_completionOperator;
    int  m_startPosition;
    QList<TextEditor::BasicProposalItem *> m_completions;
    TextEditor::SnippetAssistCollector m_snippetCollector;
};

#endif // __PYTHONCOMPLETIOTIONASSIST_H
