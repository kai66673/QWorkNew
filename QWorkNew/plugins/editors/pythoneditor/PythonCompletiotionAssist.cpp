#include <QTextDocument>
#include <QMetaObject>
#include <QMetaMethod>

#include "Core.h"
#include "PythonCompletiotionAssist.h"
#include "pythoneditorconstants.h"
#include "codeassist/basicproposalitemlistmodel.h"
#include "codeassist/genericproposal.h"

PythonCompletionProvider::PythonCompletionProvider()
{
}

int PythonCompletionProvider::activationCharSequenceLength() const
{
    return 1;
}

bool PythonCompletionProvider::isActivationCharSequence( const QString &sequence ) const
{
    const QChar &ch  = sequence.at(0);
    switch ( ch.toLatin1() ) {
        case '.': return true;
    }
    return false;
}

TextEditor::IAssistProcessor *PythonCompletionProvider::createProcessor() const
{
    return new PythonCompletionProcessor();
}

PythonCompletionProcessor::PythonCompletionProcessor()
    : TextEditor::IAssistProcessor()
    , m_snippetCollector(PythonEditor::Constants::PYTHON_SNIPPETS_GROUP_ID,
                         QIcon(QLatin1String(":/pythoneditor/images/snippet.png")))
{ }

PythonCompletionProcessor::~PythonCompletionProcessor()
{ }

TextEditor::IAssistProposal *PythonCompletionProcessor::perform( const TextEditor::IAssistInterface *interface_ )
{
    m_interface.reset(static_cast<const TextEditor::DefaultAssistInterface *>(interface_));

    bool isAccepted = accepts();
    if ( interface_->reason() != TextEditor::ExplicitlyInvoked && !isAccepted )
        return 0;

    int index = startCompletionHelper();
    if ( index != -1 ) {
        TextEditor::BasicProposalItemListModel *model = new TextEditor::BasicProposalItemListModel;
        model->loadContent(m_completions);
        return new TextEditor::GenericProposal(m_startPosition, model);
    }

    return 0;
}

int PythonCompletionProcessor::findStartOfName( int pos )
{
    if ( pos == -1 )
        pos = m_interface->position();
    QChar chr;

    // Skip to the start of a name
    do {
        chr = m_interface->characterAt(--pos);
    } while ( chr.isLetterOrNumber() || chr == QLatin1Char('_') );

    return pos + 1;
}

void PythonCompletionProcessor::getExpression( int dotPos, QString &expr )
{
    QChar chr;
    expr = "";

    do {
        chr = m_interface->characterAt(--dotPos);
    } while ( chr.isSpace() );

    while ( chr.isLetterOrNumber() || chr == QLatin1Char('_') || chr == '.' ) {
        expr.prepend(chr);
        chr = m_interface->characterAt(--dotPos);
    }
}

bool PythonCompletionProcessor::accepts()
{
    const int pos = m_interface->position();

    m_completionOperator = T_EOF_SYMBOL;

    switch (  m_interface->characterAt(pos - 1).toLatin1() ) {
        case '.': m_completionOperator = T_DOT;         return true;
        case ' ': m_completionOperator = T_EOF_SYMBOL;  return false;
    }

    QChar characterUnderCursor = m_interface->characterAt(pos);
    if ( !characterUnderCursor.isLetterOrNumber() ) {
        const int startOfName = findStartOfName(pos);
        if ( pos - startOfName > 0 )
            m_completionOperator = T_IDENTIFIER;
        if ( pos - startOfName >= 2 ) {
            const QChar firstCharacter = m_interface->characterAt(startOfName);
            if ( firstCharacter.isLetter() || firstCharacter == QLatin1Char('_') ) {
                return true;
            }
        }
    }

    return false;
}

int PythonCompletionProcessor::startCompletionHelper()
{
    m_startPosition = findStartOfName(-1);

    QStringList results = Core::Storage::mainWindow()->pythonCodeAssist(m_interface->textDocument()->toPlainText(), m_startPosition);
    foreach ( QString res, results ) {
        QChar ch = res[res.length() - 1];
        res.chop(2);
        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
        item ->setText(res);
        switch ( ch.toLatin1() ) {
            case 'i': item->setIcon(QIcon(":/pythoneditor/images/py_var.png"));      break;
            case 'c': item->setIcon(QIcon(":/pythoneditor/images/py_class.png"));    break;
            case 'f': item->setIcon(QIcon(":/pythoneditor/images/py_slot.png"));     break;
            case 'p': item->setIcon(QIcon(":/pythoneditor/images/py_prop.png"));     break;
            case 'k': item->setIcon(QIcon(":/pythoneditor/images/py_keyword.png"));  break;
            case 'm': item->setIcon(QIcon(":/pythoneditor/images/py_module.png"));   break;
        }
        m_completions.append(item);
    }

    if ( m_completionOperator == T_IDENTIFIER )
        m_completions.append(m_snippetCollector.collect());

    return m_startPosition;
}
