#include "SqlCompletiotionAssist.h"
#include "sqleditorconstants.h"
#include "SqlDocument.h"
#include "DbMetadataProvider.h"
#include "SqlLexer.h"
#include "ASTfwd.h"
#include "sql_constants.h"
#include "SqlParserEngineManager.h"

#include "codeassist/basicproposalitemlistmodel.h"
#include "codeassist/genericproposal.h"

namespace Sql {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SqlCompletionAssistProvider
bool SqlCompletionAssistProvider::supportsEditor(const Core::Context &context) const
{
    return context.contains(Sql::Constants::SQLEDITOR_ID);
}

int SqlCompletionAssistProvider::activationCharSequenceLength() const
{
    return 1;
}

bool SqlCompletionAssistProvider::isActivationCharSequence(const QString &sequence) const
{
    const QChar &ch  = sequence.at(0);
    switch ( ch.toLatin1() ) {
        case '.': return true;
        case ' ': return true;
        case ':': return true;
    }
    return false;
}

TextEditor::IAssistProcessor *SqlCompletionAssistProvider::createProcessor() const
{
    return new SqlCompletionAssistProcessor;
}

SqlCompletionAssistProcessor::SqlCompletionAssistProcessor()
{
}

SqlCompletionAssistProcessor::~SqlCompletionAssistProcessor()
{
}

TextEditor::IAssistProposal *SqlCompletionAssistProcessor::perform(const TextEditor::IAssistInterface *interface_var)
{
    m_interface.reset(static_cast<const TextEditor::DefaultAssistInterface *>(interface_var));
    bool isAccepted = accepts();
    if ( interface_var->reason() != TextEditor::ExplicitlyInvoked && !isAccepted )
        return 0;

    int index = startCompletionHelper();
    if ( index != -1 ) {
        TextEditor::BasicProposalItemListModel *model = new TextEditor::BasicProposalItemListModel;
        model->loadContent(m_completions);
        return new TextEditor::GenericProposal(m_startPosition, model);
    }

    return 0;
}

int SqlCompletionAssistProcessor::findStartOfName( int pos )
{
    if (pos == -1)
        pos = m_interface->position();
    QChar chr;

    // Skip to the start of a name
    do {
        chr = m_interface->characterAt(--pos);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('_'));

    if ( m_interface->characterAt(pos) == QLatin1Char(':') )
        m_completionOperator = T_COLON;

    return pos + 1;
}

bool SqlCompletionAssistProcessor::accepts()
{
    const int pos = m_interface->position();

    m_completionOperator = T_EOF_SYMBOL;

    switch (  m_interface->characterAt(pos - 1).toLatin1() ) {
        case '.': m_completionOperator = T_DOT;         return true;
        case ':': m_completionOperator = T_COLON;       return true;
        case ' ': m_completionOperator = T_EOF_SYMBOL;  return true;
    }

    if (/*completionSettings().m_completionTrigger == TextEditor::AutomaticCompletion*/ true) {
        QChar characterUnderCursor = m_interface->characterAt(pos);
        if ( !characterUnderCursor.isLetterOrNumber() ) {
            const int startOfName = findStartOfName(pos);
            if ( pos - startOfName > 0 )
                m_completionOperator = T_IDENTIFIER;
            if ( pos - startOfName >= 3 ) {
                const QChar firstCharacter = m_interface->characterAt(startOfName);
                if ( firstCharacter.isLetter() || firstCharacter == QLatin1Char('_') ) {
                    return true;
                }
            }
        }
    }

    return false;

}

int SqlCompletionAssistProcessor::startCompletionHelper()
{
    m_startPosition = findStartOfName(-1);

    Document::Ptr doc = Document::create("<AUTOCOMPLETE>");
    doc->setSource(m_interface->textDocument()->toPlainText().toLatin1());
    return doc->fillCompletions( &m_completions, SqlParserEngineManager::instance()->currentEngine(),
                                 m_interface->position(), m_startPosition, m_completionOperator );
}

} // Completiotion
