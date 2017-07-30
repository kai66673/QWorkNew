#include <QDebug>

#include "SqlCodeCompletion.h"
#include "icontext.h"
#include "itexteditor.h"
#include <texteditor/completionsettings.h>
#include <texteditor/basetexteditor.h>
#include "icompletioncollector.h"
#include "sqleditorconstants.h"
#include "SqlToken.h"

namespace Sql {
namespace Editor {

SqlCodeCompletion::SqlCodeCompletion( QObject *parent )
    : TextEditor::ICompletionCollector(parent)
    , m_editor(0)
    , m_startPosition(-1)
    , m_shouldRestartCompletion(false)
    , m_automaticCompletion(false)
    , m_completionOperator(0)
{
}

TextEditor::ITextEditor *SqlCodeCompletion::editor() const
{ return m_editor; }

int SqlCodeCompletion::startPosition() const
{ return m_startPosition; }

bool SqlCodeCompletion::shouldRestartCompletion()
{ return m_shouldRestartCompletion; }

QList<TextEditor::CompletionItem> SqlCodeCompletion::getCompletions()
{
    QList<TextEditor::CompletionItem> completionItems;
    completions(&completionItems);

    return completionItems;
}

bool SqlCodeCompletion::supportsPolicy( TextEditor::CompletionPolicy policy ) const
{
    return policy == TextEditor::SemanticCompletion;
}

bool SqlCodeCompletion::supportsEditor( TextEditor::ITextEditor *editor ) const
{
    return editor->context().contains(Sql::Constants::C_SQLEDITOR);
}

bool SqlCodeCompletion::triggersCompletion( TextEditor::ITextEditor *editor )
{
    m_editor = editor;
    m_automaticCompletion = false;

    const int pos = editor->position();

    if ( editor->characterAt(pos - 1) == '.' )
        return true;

    if (completionSettings().m_completionTrigger == TextEditor::AutomaticCompletion) {
        QChar characterUnderCursor = editor->characterAt(pos);
        if ( !characterUnderCursor.isLetterOrNumber() ) {
            const int startOfName = findStartOfName(pos);
            if ( pos - startOfName >= 3 ) {
                const QChar firstCharacter = editor->characterAt(startOfName);
                if ( firstCharacter.isLetter() || firstCharacter == QLatin1Char('_') )
                    return true;
            }
        }
    }

    return false;
}

int SqlCodeCompletion::startCompletion( TextEditor::ITextEditor *editor )
{
    int index = startCompletionHelper(editor);
    if (index != -1) {
        if (m_automaticCompletion) {
            const int pos = editor->position();
            const QChar ch = editor->characterAt(pos);
            if (! (ch.isLetterOrNumber() || ch == QLatin1Char('_'))) {
                for (int i = pos - 1;; --i) {
                    const QChar ch = editor->characterAt(i);
                    if (ch.isLetterOrNumber() || ch == QLatin1Char('_')) {
                        const QString wordUnderCursor = editor->textAt(i, pos - i);
                        if (wordUnderCursor.at(0).isLetter() || wordUnderCursor.at(0) == QLatin1Char('_')) {
                            foreach (const TextEditor::CompletionItem &i, m_completions) {
                                if ( !i.text.compare(wordUnderCursor, Qt::CaseInsensitive) ) {
                                    cleanup();
                                    return -1;
                                }
                            }
                        } else {
                            cleanup();
                            return -1;
                        }
                    } else
                        break;
                }
            }
        }

        if (m_completionOperator != 0)
            sortCompletion(m_completions);
    }

    for (int i = 0; i < m_completions.size(); ++i)
        m_completions[i].originalIndex = i;

    return index;
}

void SqlCodeCompletion::completions( QList<TextEditor::CompletionItem> *completions )
{
    const int length = m_editor->position() - m_startPosition;

    if (length == 0)
        *completions = m_completions;
    else if (length > 0) {
        const QString key = m_editor->textAt(m_startPosition, length);

        filter(m_completions, completions, key);

        if (completions->size() == 1) {
            if (key == completions->first().text)
                completions->clear();
        }
    }

}

bool SqlCodeCompletion::typedCharCompletes( const TextEditor::CompletionItem &item, QChar typedChar )
{
    return false;
}

void SqlCodeCompletion::complete( const TextEditor::CompletionItem &item, QChar typedChar )
{
    Q_UNUSED(typedChar);

    QString toInsert = item.text;

    const int length = m_editor->position() - m_startPosition;
    m_editor->setCursorPosition(m_startPosition);
    m_editor->replace(length, toInsert);
}

bool SqlCodeCompletion::partiallyComplete( const QList<TextEditor::CompletionItem> &completionItems )
{
    if (completionItems.count() == 1) {
        complete(completionItems.first(), QChar());
        return true;
    } else {
        return TextEditor::ICompletionCollector::partiallyComplete(completionItems);
    }

    return false;
}

void SqlCodeCompletion::cleanup()
{
    m_automaticCompletion = false;
    m_completions.clear();
}

void SqlCodeCompletion::emitGetTables( QVariantList &values )
{
    emit getTables(values);
}

int SqlCodeCompletion::startCompletionHelper( TextEditor::ITextEditor *editor )
{
    TextEditor::BaseTextEditorWidget *edit = qobject_cast<TextEditor::BaseTextEditorWidget *>(editor->widget());
    if (! edit)
        return -1;

    m_editor = editor;

    bool completeField;
    const int startOfName = findStartOfName(-1, &completeField);
    m_startPosition = startOfName;
    m_completionOperator = 1;

    SqlStatementUnderCursor statement(this, m_editor);
    if ( statement.isDmlStatement() ) {
        if ( !completeField ) {
            addKeywords();
            addTablesAndViews();
        }
        addFields(&statement);
    }
    else if ( statement.isDdlStatement() && !completeField ) {
        addKeywords();
        switch ( statement.ddlObjectKind() ) {
            case DdlStatementAST::Table:    addTables();    break;
            case DdlStatementAST::View:     addViews();     break;
            case DdlStatementAST::Index:    addIndices();   break;
            case DdlStatementAST::Trigger:  addTriggers();  break;
        }
    }
    else if ( statement.isCommentStatement() ) {
        addKeywords();
        addTablesAndViews();
    }
    else
        addKeywords();

    return m_startPosition;
}

int SqlCodeCompletion::findStartOfName( int pos, bool *afterPoint )
{
    m_expr = "";

    if (pos == -1)
        pos = m_editor->position();
    QChar chr;

    // Skip to the start of a name
    do {
        chr = m_editor->characterAt(--pos);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('_'));

    if ( afterPoint ) {
        int spPos = pos;
        *afterPoint = false;
        chr = m_editor->characterAt(spPos);
        while ( chr.isSpace() ) {
            spPos--;
            chr = m_editor->characterAt(spPos);
        }
        if ( chr == '.' ) {
            *afterPoint = true;
            spPos--;
            int exprPos = spPos;
            while ( exprPos >= 0 && m_editor->characterAt(exprPos).isSpace() )
                exprPos--;
            while ( exprPos >= 0 && (m_editor->characterAt(exprPos).isLetterOrNumber() || m_editor->characterAt(exprPos) == '_') )
                exprPos--;
            exprPos++;
            qDebug() << "        &&&&&&&&&&&&" << exprPos << spPos << ":" << m_editor->textAt(exprPos, spPos - exprPos + 1);
            if ( exprPos <= spPos ) {
                m_expr = m_editor->textAt(exprPos, spPos - exprPos + 1).trimmed();
            }
        }
    }

    qDebug() << "FLD EXPR" << m_expr;

    return pos + 1;
}

void SqlCodeCompletion::addKeywords()
{
    Token::populateKeywords(this, m_completions);
}

void SqlCodeCompletion::addTablesAndViews()
{
    const int length = m_editor->position() - m_startPosition;
    if ( length == 0 )
        return;

//    const QString key = m_editor->textAt(m_startPosition, 1).toUpper();
    QVariantList values;
    emit getTablesAndViews(values);

    if ( values.size() != 2 )
        return;

    TextEditor::CompletionItem item(this);
    item.icon = QIcon(":/images/dbb_table.png");

    for (int i = 0; i < 2; i++) {
        QStringList *tables = (QStringList *) values[i].value<void *>();
        if ( tables ) {
            for (int j = 0; j < tables->size(); j++) {
//                if ( tables->at(j)[0] == key[0] ) {
                    item.text = tables->at(j);
                    m_completions.append(item);
//                }
            }
        }
    }
}

void SqlCodeCompletion::addTables()
{
    const int length = m_editor->position() - m_startPosition;
    if ( length == 0 )
        return;

    QVariantList values;
    emit getTables(values);

    if ( values.size() != 1 )
        return;

    TextEditor::CompletionItem item(this);
    item.icon = QIcon(":/images/dbb_table.png");

    QStringList *tables = (QStringList *) values[0].value<void *>();
    if ( tables ) {
        for (int j = 0; j < tables->size(); j++) {
            item.text = tables->at(j);
            m_completions.append(item);
        }
    }
}

void SqlCodeCompletion::addViews()
{
    const int length = m_editor->position() - m_startPosition;
    if ( length == 0 )
        return;

    QVariantList values;
    emit getViews(values);

    if ( values.size() != 1 )
        return;

    TextEditor::CompletionItem item(this);
    item.icon = QIcon(":/images/dbb_table.png");

    QStringList *views = (QStringList *) values[0].value<void *>();
    if ( views ) {
        for (int j = 0; j < views->size(); j++) {
            item.text = views->at(j);
            m_completions.append(item);
        }
    }
}

void SqlCodeCompletion::addIndices()
{
    const int length = m_editor->position() - m_startPosition;
    if ( length == 0 )
        return;

    QVariantList values;
    emit getIndices(values);

    if ( values.size() != 1 )
        return;

    TextEditor::CompletionItem item(this);
    item.icon = QIcon(":/images/dbb_index.png");

    QStringList *indices = (QStringList *) values[0].value<void *>();
    if ( indices ) {
        for (int j = 0; j < indices->size(); j++) {
            item.text = indices->at(j);
            m_completions.append(item);
        }
    }
}

void SqlCodeCompletion::addTriggers()
{
    const int length = m_editor->position() - m_startPosition;
    if ( length == 0 )
        return;

    QVariantList values;
    emit getTriggers(values);

    if ( values.size() != 1 )
        return;

    TextEditor::CompletionItem item(this);
    item.icon = QIcon(":/images/dbb_trigger.png");

    QStringList *triggers = (QStringList *) values[0].value<void *>();
    if ( triggers ) {
        for (int j = 0; j < triggers->size(); j++) {
            item.text = triggers->at(j);
            m_completions.append(item);
        }
    }
}

void SqlCodeCompletion::addFields( SqlStatementUnderCursor *statement )
{
    qDebug() << "FLD 01";
    qDebug() << "FLD 02";
    QStringList tables;
    statement->getTablesForExpression(m_expr, tables);
    qDebug() << "FLD 03";
    if ( tables.size() ) {
        QVariantList values;
        values << tables;
        emit getTableFields(values);
        if ( values.size() == 2 ) {
            QStringList fields = values[1].toStringList();
            TextEditor::CompletionItem item(this);
            item.icon = QIcon(":/images/sql_ci_column.png");
            foreach ( const QString &field, fields ) {
                item.text = field;
                m_completions.append(item);
            }
        }
    }
}

SqlStatementUnderCursor::SqlStatementUnderCursor( SqlCodeCompletion *completer, TextEditor::ITextEditor *editor )
    : ASTVisitor(0)
    , m_editor(0)
    , m_editorWidget(0)
    , m_UnknownStatement(true)
    , m_DmlStatement(false)
    , m_DdlStatement(false)
    , m_BlockStatement(false)
    , m_CommentStatement(false)
    , m_DdlObjectkind(DdlStatementAST::Table)
    , m_completer(completer)
{
    m_editor = qobject_cast<SQLEditor *>(editor);
    m_editorWidget = qobject_cast<SQLEditorWidget *>(editor->widget());

    if ( init() ) {
        parse();
    }
}

void SqlStatementUnderCursor::getTablesForExpression(const QString &expr, QStringList &tables)
{
    QMap<QString, QString>::const_iterator i = m_tableAliasesToNames.constBegin();
     while ( i != m_tableAliasesToNames.constEnd() ) {
         qDebug() << "FLD iter" << i.key() << i.value() << expr;
         if ( expr.isEmpty() || !expr.compare(i.key(), Qt::CaseInsensitive) /*|| !expr.compare(i.value(), Qt::CaseInsensitive)*/ )
             tables << i.value();
         ++i;
     }
     tables.removeDuplicates();
     qDebug() << "FLD tables" << tables;
}

bool SqlStatementUnderCursor::init()
{
    if ( !m_editor || !m_editorWidget )
        return false;

    qDebug() << "FLD init";

    m_startStatementPosition = 0;
    int position = m_editor->position();
    int colonPosition = 0;
    int currPosition = 0;
    int state = 0; // 0 - normal; 1 - comment; 2 - multyline comment; 3 - character; 4 - straing

    const QString &text = m_editorWidget->document()->toPlainText();
    int length = text.length();

    while ( colonPosition < position && currPosition < length ) {
        m_startStatementPosition = colonPosition;

        const char ch = text[currPosition].toLatin1();
        const char ch1 = ( (currPosition < length - 1) ? text[currPosition + 1].toLatin1() : '\0' );

        switch ( state ) {

            case 0:
                switch ( ch ) {
                    case ';': colonPosition = currPosition; break;
                    case '-': if ( ch1 == '-' ) { currPosition++; state = 1; } break;
                    case '/': if ( ch1 == '*' ) { currPosition++; state = 2; } break;
                    case '\'': state = 3; break;
                    case '\"': state = 4; break;
                }
                break;

            case 1: if ( ch == '\n' ) state = 0; break;

            case 2: if ( ch == '*' && ch1 == '/' ) { currPosition++; state = 0; } break;

            case 3: if ( ch == '\n' || ch == '\'' ) state = 0; break;

            case 4: if ( ch == '\n' || ch == '\"' ) state = 0; break;

        }

        currPosition++;
    }

    if ( text[m_startStatementPosition] == ';' )
        m_startStatementPosition++;

    while ( m_startStatementPosition < length && (text[m_startStatementPosition].isSpace()) )
        m_startStatementPosition++;

    return true;
}

bool SqlStatementUnderCursor::parse()
{
    Document::Ptr doc = Document::create("<AUTOCOMPLETE>");
    doc->setSource(m_editorWidget->document()->toPlainText().mid(m_startStatementPosition).toLatin1());
    doc->checkStatement();
    if ( !doc->translationUnit()->ast() )
        return false;
    if ( doc->translationUnit()->ast()->asDmlStatement() ) {
        m_UnknownStatement = false;
        m_DmlStatement = true;
        setTranslationUnit(doc->translationUnit());
        accept(doc->translationUnit()->ast());
    }
    else if ( DdlStatementAST *ddlAst = doc->translationUnit()->ast()->asDdlStatement() ) {
        m_UnknownStatement = false;
        m_DdlStatement = true;
        m_DdlObjectkind = ddlAst->targetKind();
    }
    else if ( doc->translationUnit()->ast()->asCommentStatement() ) {
        m_UnknownStatement = false;
        m_CommentStatement = true;
    }
    else {
        m_UnknownStatement = false;
        m_BlockStatement = true;
    }
    return true;
}

bool SqlStatementUnderCursor::visit(DirectSingleSourceAST *ast)
{
    if ( ast && ast->tableName) {
        QString aliasName;
        QString tableName(ast->tableName->name->chars());
        if ( ast->tableAlias )
            aliasName = QString(ast->tableAlias->name->chars());
        else
            aliasName = tableName;
        m_tableAliasesToNames[aliasName] = tableName;

        qDebug() << "    *** SqlStatementUnderCursor::visit" << aliasName << tableName << ast->tableAlias;
    }
    return false;
}

} // namespace Editor
} // namespace Sql
