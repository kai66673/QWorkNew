#include <QDebug>
#include <QTextDocument>
#include <QTextBlock>
#include <QStack>

#include "codeassist/basicproposalitemlistmodel.h"
#include "codeassist/genericproposal.h"
#include "convenience.h"

#include "KarelCompletionAssist.h"
#include "KarelSimpleLexer.h"
#include "KarelTypeResolver.h"
#include "KarelKeywords.h"
#include "KarelSymbolIcons.h"
#include "karel_constants.h"

namespace Karel {

static int activationSequenceChar(const QChar &ch,
                                  const QChar &ch2,
                                  unsigned *kind,
                                  bool wantFunctionCall)
{
    int referencePosition = 0;
    int completionKind = T_EOF_SYMBOL;

    switch (ch.toLatin1()) {
        case '.':
            if ( ch2 != QLatin1Char('.') ) {
                completionKind = T_DOT;
                referencePosition = 1;
            }
            break;
            /// TODO: function arguments hint
//        case ',':
//            completionKind = T_COMMA;
//            referencePosition = 1;
//            break;
//        case '(':
//            if (wantFunctionCall) {
//                completionKind = T_LPAREN;
//                referencePosition = 1;
//            }
//            break;
        case ':':
            if ( ch2 != QLatin1Char(':') ) {
                completionKind = T_COLON;
                referencePosition = 1;
            }
            break;
        case '=':
            if ( ch2 != QLatin1Char('=') ) {
                completionKind = T_EQUAL;
                referencePosition = 1;
            }
            break;
        case '%':
            completionKind = T_SLASH;
            referencePosition = 1;
            break;
    }

    if (kind)
        *kind = completionKind;

    return referencePosition;
}

KarelCompletionAssistProvider::KarelCompletionAssistProvider()
{ }

KarelCompletionAssistProvider::~KarelCompletionAssistProvider()
{ }

bool KarelCompletionAssistProvider::isAsynchronous() const
{ return true; }

bool KarelCompletionAssistProvider::supportsEditor(const Core::Context &context) const
{ return context.contains(Constants::C_KAREL_EDITOR); }

int KarelCompletionAssistProvider::activationCharSequenceLength() const
{ return 2; }

bool KarelCompletionAssistProvider::isActivationCharSequence(const QString &sequence) const
{
    const QChar &ch  = sequence.at(1);
    const QChar &ch2 = sequence.at(0);

    return activationSequenceChar(ch, ch2, 0, true);
}

TextEditor::IAssistProcessor *KarelCompletionAssistProvider::createProcessor() const
{ return new KarelCompletionAssistProcessor(); }

KarelCompletionAssistProcessor::KarelCompletionAssistProcessor()
    : TextEditor::IAssistProcessor()
    , m_snippetCollector(Constants::KAREL_SNIPPETS_GROUP_ID,
                         QIcon(QLatin1String(":/cppeditor/images/snippet.png")))
{ }

KarelCompletionAssistProcessor::~KarelCompletionAssistProcessor()
{ }

TextEditor::IAssistProposal *KarelCompletionAssistProcessor::perform(const TextEditor::IAssistInterface *_interface)
{
    m_interface.reset(static_cast<const KarelCompletionAssistInterface *>(_interface));

    if ( _interface->reason() != TextEditor::ExplicitlyInvoked && !accepts() )
        return 0;

    int index = startCompletionHelper();
    if ( index != -1 ) {
        TextEditor::BasicProposalItemListModel *model = new TextEditor::BasicProposalItemListModel;
        model->loadContent(m_completions);
        return new TextEditor::GenericProposal(m_startPosition, model);
    }

    return 0;
}

bool KarelCompletionAssistProcessor::accepts()
{
    const int pos = m_interface->position();
    m_completionOperator = T_EOF_SYMBOL;

    const int start = startOfOperator(pos, &m_completionOperator, /*want function call=*/ true);
    if ( start != pos ) {
        if ( m_completionOperator == T_SLASH ) {
            const int column = pos - m_interface->textDocument()->findBlock(start).position();
            if (column != 1)
                return false;
        }
        return true;
    } else {
        QChar characterUnderCursor = m_interface->characterAt(pos);
        if ( !characterUnderCursor.isLetterOrNumber() && characterUnderCursor != QLatin1Char('_') && characterUnderCursor != QLatin1Char('$') ) {
            const int startOfName = findStartOfName(pos);
            if (pos - startOfName >= 3) {
                QTextCursor tc(m_interface->textDocument());
                tc.setPosition(pos);
                SimpleLexer tokenize;
                const QList<Token> &tokens = tokenize(tc.block().text());
                const int tokenIdx = SimpleLexer::tokenBefore(tokens, qMax(0, tc.positionInBlock() - 1));
                const Token tk = (tokenIdx == -1) ? Token() : tokens.at(tokenIdx);
                if ( !tk.isComment() && !tk.isLiteral() )
                    return true;
            }
        }
    }

    return false;
}

int KarelCompletionAssistProcessor::startOfOperator(int pos, unsigned *kind, bool wantFunctionCall) const
{
    const QChar ch  = pos > -1 ? m_interface->characterAt(pos - 1) : QChar();
    const QChar ch2 = pos >  0 ? m_interface->characterAt(pos - 2) : QChar();

    int start = pos - activationSequenceChar(ch, ch2, kind, wantFunctionCall);

    return start;
}

int KarelCompletionAssistProcessor::findStartOfName(int pos) const
{
    if (pos == -1)
        pos = m_interface->position();
    QChar chr;

    // Skip to the start of a name
    do {
        chr = m_interface->characterAt(--pos);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('_') || chr == QLatin1Char('$'));

    return pos + 1;
}

int KarelCompletionAssistProcessor::startCompletionHelper()
{
    const int startOfName = findStartOfName();
    m_startPosition = startOfName;
    m_completionOperator = T_EOF_SYMBOL;

    int endOfOperator = m_startPosition;

    // Skip whitespace preceding this position
    while (m_interface->characterAt(endOfOperator - 1).isSpace())
        --endOfOperator;

    int endOfExpression = startOfOperator(endOfOperator,
                                          &m_completionOperator,
                                          /*want function call =*/ true);

    bool isStatementsArea;
    Scope *scope = currentScope(isStatementsArea);
    if ( !scope )
        return -1;

    QString completeWord = m_interface->textAt(m_startPosition, m_interface->position() - m_startPosition).trimmed();

    switch (m_completionOperator) {
        case T_SLASH:
            return isStatementsArea ? -1 : completeDirectives();
        case T_COLON:
        case T_EQUAL:
            if ( isStatementsArea && completeWord.length() < 3 && m_interface->reason() != TextEditor::ExplicitlyInvoked )
                break;
        case T_EOF_SYMBOL:
            return completeGlobals(scope, isStatementsArea);
        case T_DOT:
            return isStatementsArea ? completeMembers(scope, endOfExpression) : -1;
    }

    return -1;
}

Scope *KarelCompletionAssistProcessor::currentScope(bool &isStatementsArea)
{
    Document::Ptr document = m_interface->document();
    if ( document ) {
        if ( Scope *globalScope = document->globalScope() ) {
            unsigned line = 0, column = 0;
            TextEditor::Convenience::convertPosition(m_interface->textDocument(), m_startPosition, &line, &column);
            if ( line )
                line--;
            return globalScope->scopeAt(document->translationUnit(), isStatementsArea, line, column);
        }
    }

    return 0;
}

int KarelCompletionAssistProcessor::completeDirectives()
{
    addCompletionItem("ENVIRONMENT", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("DELAY", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("COMMENT", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("INCLUDE", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("NOABORT=ERROR+COMMAND", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("NOPAUSE=ERROR+COMMAND+TPENABLE", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("CMOSVARS", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("DEFGROUP", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("PRIORITY", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("TPMOTION", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("CRTDEVICE", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("LOCKGROUP", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("STACKSIZE", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("TIMESLICE", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("NOBUSYLAMP", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("ALPHABETIZE", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("NOLOCKGROUP", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    addCompletionItem("NOPAUSESHFT", KarelSymbolIcons::icon(KarelSymbolIcons::Directive));
    return m_startPosition;
}

int KarelCompletionAssistProcessor::completeGlobals(Scope *scope, bool isStatementsArea)
{
    if ( isStatementsArea ) {
        QStringList routines = scope->routines();
        foreach (const QString &routine, routines)
            addCompletionItem(routine, KarelSymbolIcons::icon(KarelSymbolIcons::Function));
        QStringList variables = scope->variables();
        foreach (const QString &variable, variables)
            addCompletionItem(variable, KarelSymbolIcons::icon(KarelSymbolIcons::Variable));
        QStringList usingFields = scope->usingFields();
        foreach (const QString &field, usingFields)
            addCompletionItem(field, KarelSymbolIcons::icon(KarelSymbolIcons::Field));
        addBuiltingsAndKeywords();
    } else {
        addTypes(scope);
    }

    QStringList constants = scope->constants();
    foreach (const QString &constant, constants)
        addCompletionItem(constant, KarelSymbolIcons::icon(KarelSymbolIcons::Constant));

    addSnippets();
    return m_startPosition;
}

int KarelCompletionAssistProcessor::completeMembers(Scope *scope, int endOfExpression)
{
    QTextCursor tc(m_interface->textDocument());
    tc.setPosition(endOfExpression);
    QTextBlock block = tc.block();
    int startPosition = block.position();
    TypeResolver resolver(m_interface->textDocument(), scope);
    if ( const Type *type = resolver(m_interface->textAt(startPosition, endOfExpression - startPosition), block) ) {
        QStringList fields = type->fields();
        foreach (const QString &field, fields)
            addCompletionItem(field, KarelSymbolIcons::icon(KarelSymbolIcons::Field));
        return m_startPosition;
    }

    return -1;
}

void KarelCompletionAssistProcessor::addCompletionItem(const QString &text, const QIcon &icon, int order, const QVariant &data)
{
    TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
    item->setText(text);
    item->setIcon(icon);
    item->setOrder(order);
    item->setData(data);
    m_completions.append(item);
}

void KarelCompletionAssistProcessor::addSnippets()
{
    m_completions.append(m_snippetCollector.collect());
}

void KarelCompletionAssistProcessor::addTypes(Scope *scope)
{
    if ( !scope )
        return;

    QStringList userTypes = scope->userTypes();
    foreach (const QString &userType, userTypes)
        addCompletionItem(userType, KarelSymbolIcons::icon(KarelSymbolIcons::UserType));

    if ( m_completionOperator == T_COLON || m_completionOperator == T_EQUAL )
        addCompletionItem(QStringLiteral("ARRAY[] OF "), KarelSymbolIcons::icon(KarelSymbolIcons::UserType));

    if ( m_completionOperator == T_EQUAL )
        addCompletionItem(QStringLiteral("STRUCTURE ENDSTRUCTURE"), KarelSymbolIcons::icon(KarelSymbolIcons::UserType));

    addCompletionItem(QStringLiteral("BYTE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("SHORT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("INTEGER"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("REAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("BOOLEAN"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("VECTOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("VIS_PROCESS"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("MODEL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CAM_SETUP"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CONFIG"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("PATH"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("PATHHEADER"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("NODEDATA"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("STRING"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("POSITION"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("XYZWPR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("XYZWPREXT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS1"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS2"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS3"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS4"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS5"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS6"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS7"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS8"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINTPOS9"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
}

void KarelCompletionAssistProcessor::addBuiltingsAndKeywords()
{
    addCompletionItem(QStringLiteral("ABORT_TASK"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ABS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ACOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ACT_SCREEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ADD_BYNAMEPC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ADD_DICT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ADD_INTPC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ADD_REALPC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ADD_STRINGPC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("APPEND_NODE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("APPEND_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("APPROACH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ARRAY_LEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ASIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ATAN2"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ATT_WINDOW_D"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ATT_WINDOW_S"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AVL_POS_NUM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("BYNAME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("BYTES_AHEAD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("BYTES_LEFT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CALL_PROG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CALL_PROGLIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CHECK_DICT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CHECK_EPOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CHR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLEAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLEAR_SEMA"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLOSE_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLR_IO_STAT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLR_PORT_SIM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLR_POS_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNC_DYN_DISB"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNC_DYN_DISE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNC_DYN_DISI"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNC_DYN_DISP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNC_DYN_DISR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNC_DYN_DISS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNCL_STP_MTN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_CONF_STR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_INT_STR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_JPOS_REL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_REAL_STR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_REL_JPOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_STR_CONF"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_STR_INT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_STR_REAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_STR_TIME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CNV_TIME_STR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONT_TASK"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("COPY_FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("COPY_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("COPY_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("COS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CREATE_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CREATE_VAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CURJPOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CURPOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CURR_PROG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DEF_SCREEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DEF_WINDOW"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DEL_INST_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DELETE_FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DELETE_NODE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DELETE_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DET_WINDOW"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISCTRL_ALPH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISCTRL_FORM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISCTRL_LIST"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISMOUNT_DEV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ERR_DATA"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("EXP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FILE_LIST"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FORCE_SPMENU"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FORMAT_DEV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FRAME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_ATTR_PRG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_FILE_POS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_JPOS_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_JPOS_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_PORT_ASG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_PORT_ATR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_PORT_CMT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_PORT_MOD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_PORT_SIM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_PORT_VAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_POS_FRM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_POS_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_POS_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_POS_TYP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_TIME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_TSK_INFO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GET_VAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IN_RANGE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INDEX"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INI_DYN_DISB"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INI_DYN_DISE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INI_DYN_DISI"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INI_DYN_DISP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INI_DYN_DISR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INI_DYN_DISS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INIT_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INSERT_NODE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INSERT_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("INV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IO_MOD_TYPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IO_STATUS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("J_IN_RANGE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("KLC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("KLC_NO_WAIT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("KLC_STATUS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("LN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("LOAD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("LOAD_STATUS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("LOCK_GROUP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MIRROR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MODIFY_QUEUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOTION_CTL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOUNT_DEV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOVE_FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NODE_SIZE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("OPEN_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ORD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ORIENT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PATH_LEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PAUSE_TASK"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PEND_SEMA"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("POP_KEY_RD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("POS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("POS_REG_TYPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("POST_ERR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("POST_SEMA"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PRINT_FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PROG_LIST"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PURGE_DEV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PUSH_KEY_RD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("READ_DICT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("READ_DICT_V"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("READ_KB"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("REMOVE_DICT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RENAME_FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RENAME_VAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RENAME_VARS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RESET"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ROUND"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RUN_TASK"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SAVE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SELECT_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SEMA_COUNT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SEND_DATAPC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SEND_EVENTPC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_ATTR_PRG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_CURSOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_EPOS_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_EPOS_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_FILE_ATR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_FILE_POS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_INT_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_JPOS_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_JPOS_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_LANG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PERCH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PORT_ASG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PORT_ATR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PORT_CMT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PORT_MOD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PORT_SIM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_PORT_VAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_POS_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_POS_TPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_REAL_REG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_TIME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_TSK_ATTR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_TSK_NAME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SET_VAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SQRT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("STR_LEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SUB_STR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TAN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TRANSLATE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TRUNC"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNINIT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNLOCK_GROUP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNPOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("VAR_INFO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("VAR_LIST"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("VOL_SPACE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WRITE_DICT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WRITE_DICT_V"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ABORT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ABOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ALONG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AND"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ARRAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ATTACH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AWAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AXIS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("BEGIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CANCEL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CASE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLOSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONDITION"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONNECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONST"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONTINUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DELAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISABLE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISCONNECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DIV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DOWNTO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ELSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENABLE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("END"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDCONDITION"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDFOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDFOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDIF"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDSELECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDSTRUCTURE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDUSING"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDWHILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("EVAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("EVENT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FROM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("HAND"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("HOLD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IF"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOVE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NEAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOABORT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOMESSAGE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOPAUSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOWAIT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("OF"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("OPEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("OR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PAUSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PROGRAM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PULSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PURGE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("READ"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RELATIVE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RELAX"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RELEASE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("REPEAT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RESUME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RETURN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ROUTINE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SELECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SIGNAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("STOP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("THEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TIMER"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNHOLD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNPAUSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNTIL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("USING"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("VIA"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WAIT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WHEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WHILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WITH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WRITE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ABORT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ABOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ALONG"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AND"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ARRAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ATTACH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AWAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("AXIS"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("BEGIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CANCEL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CLOSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONDITION"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONNECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("CONTINUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DELAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISABLE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DISCONNECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DIV"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("DOWNTO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENABLE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("END"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDCONDITION"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDFOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDFOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDIF"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDSELECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDUSING"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ENDWHILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("EVAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("EVENT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("FROM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("GO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("HAND"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("HOLD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IF"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("IN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("MOVE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NEAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOABORT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOMESSAGE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOPAUSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("NOWAIT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("OPEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("OR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PAUSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PROGRAM"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PULSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("PURGE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("READ"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RELATIVE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RELAX"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RELEASE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("REPEAT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RESUME"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("RETURN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("ROUTINE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SELECT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("SIGNAL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("STOP"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("STRUCTURE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TIMER"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TO"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("TYPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNHOLD"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNPAUSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("UNTIL"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("USING"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("VAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("VIA"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WAIT"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WHEN"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WHILE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WITH"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("WRITE"), KarelSymbolIcons::icon(KarelSymbolIcons::Function));
    addCompletionItem(QStringLiteral("$DECELTOL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$GENOVERRIDE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$GROUP"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MCR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MNUFRAME"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MNUFRAMENUM"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MNUTOOL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MNUTOOLNUM"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MOR_GRP"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$MOTYPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$NILP"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$ROTSPEED"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$SPEED"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$TERMTYPE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$UFRAME"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("$UTOOL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("AESWORLD"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("AIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("AOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CIRCULAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("COARSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CRTERROR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CRTFUNC"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CRTPROMPT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CRTSTATUS"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("CMOS"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("DRAM"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("DIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("DOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("FALSE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("FINE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("GIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("GOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("INPUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("JOINT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("LINEAR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("MAXINT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("MININT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("NODECEL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("NOSETTLE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("OFF"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("ON"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("OPIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("OPOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("OUTPUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("PORT_ID"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("RDI"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("RDO"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("RSWORLD"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("STATUS"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPDISPLAY"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPERROR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPFUNC"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPIN"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPPROMPT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TPSTATUS"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("TRUE"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("VARDECEL"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("VIS_MONITOR"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("WDI"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("WDOUT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
    addCompletionItem(QStringLiteral("WRISTJOINT"), KarelSymbolIcons::icon(KarelSymbolIcons::Keyword));
}

KarelCompletionAssistInterface::KarelCompletionAssistInterface(QTextDocument *textDocument,
                                                               int position,
                                                               Core::IFile *file,
                                                               TextEditor::AssistReason reason,
                                                               Document::Ptr document)
    : TextEditor::DefaultAssistInterface(textDocument, position, file->fileName(), reason)
    , m_document(document)
{ }

Document::Ptr KarelCompletionAssistInterface::document() const
{ return m_document; }

}   // namespace Karel
