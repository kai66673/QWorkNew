#ifndef SQLCOMPLETIONASSIST_H

#define SQLCOMPLETIONASSIST_H

#include <completionassistprovider.h>
#include <iassistprocessor.h>
#include <codeassist/defaultassistinterface.h>
#include <ifile.h>

#include <QTextDocument>
#include <QDebug>

#include "ASTfwd.h"
#include "ASTVisitor.h"
#include "AST.h"
#include "SqlDocument.h"

namespace Sql {

class SqlCompletionAssistInterface;

class SqlCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
public:
    virtual bool supportsEditor( const Core::Context &context ) const;
    virtual int activationCharSequenceLength() const;
    virtual bool isActivationCharSequence(const QString &sequence) const;
    virtual TextEditor::IAssistProcessor *createProcessor() const;
    virtual bool isAsynchronous() const { return false; }
};

class SqlCompletionAssistProcessor: public TextEditor::IAssistProcessor
{
public:
    SqlCompletionAssistProcessor();
    virtual ~SqlCompletionAssistProcessor();

    virtual TextEditor::IAssistProposal *perform(const TextEditor::IAssistInterface *interface_var);

private:
    int  findStartOfName( int pos );
    bool accepts();
    int  startCompletionHelper();

private:
    QScopedPointer<const TextEditor::DefaultAssistInterface> m_interface;
    int  m_startPosition;
    unsigned m_completionOperator;
    QList<TextEditor::BasicProposalItem *> m_completions;

    unsigned m_tokenIndexBefore;

};

} // Completiotion

#endif // SQLCOMPLETIONASSIST_H
