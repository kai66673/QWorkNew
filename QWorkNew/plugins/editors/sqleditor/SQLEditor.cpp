#include <QTextBlock>

#include "Core.h"
#include "SqlHighlighter.h"
#include "SQLEditor.h"
#include "sqleditorconstants.h"
#include "texteditorconstants.h"
#include "mimedatabase.h"
#include "basetextdocument.h"
#include "fontsettings.h"
#include "sql_constants.h"
#include "SqlCheckNames.h"
#include "SqlParserEngineManager.h"
#include "AST.h"
#include "HighlighterFormatsManager.h"

#include <utils/uncommentselection.h>
#include <normalindenter.h>

static QList<QTextEdit::ExtraSelection> createSelections( QTextDocument *document,
                                                          const QList<Utils::DiagnosticMessage> &msgs,
                                                          const QTextCharFormat &format )
{
    QList<QTextEdit::ExtraSelection> selections;

    foreach (const Utils::DiagnosticMessage &m, msgs) {
        const int pos = document->findBlockByNumber(m.line() - 1).position() + m.column() - 1;
        if (pos < 0)
            continue;

        QTextCursor cursor(document);
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, m.length());

        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format = format;
        sel.format.setToolTip(m.text());
        selections.append(sel);
    }

    return selections;
}

namespace Sql {
namespace Editor {

SQLEditor::SQLEditor( SQLEditorWidget *editor )
  : TextEditor::BaseTextEditor(editor)
{
    m_context.add(Sql::Constants::C_SQLEDITOR);
    m_context.add(TextEditor::Constants::C_TEXTEDITOR);
}

Core::Context SQLEditor::context() const
{
    return m_context;
}

QString SQLEditor::id() const
{
    return QLatin1String(Sql::Constants::SQLEDITOR_ID);
}

bool SQLEditor::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
    bool b = TextEditor::BaseTextEditor::open(errorString, fileName, realFileName);
    editorWidget()->setMimeType(Core::Storage::mainWindow()->mimeDatabase()->findByFile(QFileInfo(fileName)).type());
    return b;
}

SQLEditorWidget::SQLEditorWidget(SqlDocument *sqlDocument, QWidget *parent)
    : TextEditor::TextEditorWidget(sqlDocument, parent)
    , m_sqlDocument(sqlDocument)
    , m_highlightRevision(0)
    , m_highlightEngine(0)
{
    qRegisterMetaType<Sql::SemanticInfo>("Sql::SemanticInfo");

    // set up the format for the errors
    m_errorFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_errorFormat.setUnderlineColor(Qt::red);

    // set up the format for the warnings.
    m_warningFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_warningFormat.setUnderlineColor(Qt::darkYellow);

    m_semanticHighlighter = new SemanticHighlighter(this);
    m_semanticHighlighter->start();

    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);

    setIndenter(new TextEditor::NormalIndenter);

    m_diagnosticMessageFormat.setUnderlineColor(Qt::darkYellow); // ### hardcoded
    m_diagnosticMessageFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline); // ### hardcoded


    m_sqlHighlighter = new SqlHighlighter;
    baseTextDocument()->setSyntaxHighlighter(m_sqlHighlighter);

    m_highlightRevision = 0;
    m_nextHighlightBlockNumber = 0;
    connect(&m_highlightWatcher, SIGNAL(resultsReadyAt(int,int)), SLOT(highlightSymbolUsages(int,int)));
    connect(&m_highlightWatcher, SIGNAL(finished()), SLOT(finishHighlightSymbolUsages()));

    connect(SqlParserEngineManager::instance(), SIGNAL(currentEngineChanged(Sql::IParserEngine*)), this, SLOT(onParserEngineChanged()));
}

SQLEditorWidget::~SQLEditorWidget()
{
    stopSemanticHiglighter();
}

void SQLEditorWidget::stopSemanticHiglighter()
{
    m_semanticHighlighter->abort();
    m_semanticHighlighter->wait();
}

void SQLEditorWidget::onSettingsChanged()
{
    TextEditor::TextEditorWidget::onSettingsChanged();
    m_sqlHighlighter->onFormatSettingsChanged();
    onSemanticFormatSettingsChanged();
}

TextEditor::BaseTextEditor * SQLEditorWidget::createEditor()
{
  SQLEditor *editable = new SQLEditor(this);

  m_updateUsesTimer = new QTimer(this);
  m_updateUsesTimer->setSingleShot(true);
  m_updateUsesTimer->setInterval(500);
  connect(m_updateUsesTimer, SIGNAL(timeout()), this, SLOT(updateUsesNow()));


  connect(this, SIGNAL(textChanged()), this, SLOT(updateUses()));
  connect( m_semanticHighlighter, SIGNAL(changed(Sql::SemanticInfo)), this, SLOT(updateSemanticInfo(Sql::SemanticInfo)));

  return editable;
}

void SQLEditorWidget::onSemanticFormatSettingsChanged()
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("SQL"));
    int offset = Sql::NumSqlFormats;

    for (int i = 0; i < SqlSemanticFormatsCount; i++, offset++)
        formatData->fillTextCharFormat(offset, m_sqlSemanticFormats[i]);

    semanticRehighlight();
}

unsigned SQLEditorWidget::editorRevision() const
{
    return document()->revision();
}

void SQLEditorWidget::onParserEngineChanged()
{
    m_sqlHighlighter->rehighlight();
    semanticRehighlight();
}

void SQLEditorWidget::updateSemanticInfo( const Sql::SemanticInfo &semanticInfo )
{
    if ( semanticInfo.revision != editorRevision() ||
         semanticInfo.m_parserEngine != SqlParserEngineManager::instance()->currentEngine() ) {
        // got outdated semantic info
        semanticRehighlight();
        return;
    }

    const SemanticInfo previousSemanticInfo = m_lastSemanticInfo;
    m_lastSemanticInfo = semanticInfo; // update the semantic info
    m_highlightRevision = semanticInfo.revision;
    m_highlightEngine = semanticInfo.m_parserEngine;

    if (previousSemanticInfo.revision != semanticInfo.revision) {
        setExtraSelections( UndefinedSymbolSelection, createSelections( document(),
                                                                        semanticInfo.diagnosticMessages,
                                                                        m_diagnosticMessageFormat ) );

        m_highlighter.cancel();

        if (semanticInfo.doc) {
            SqlCheckNames::SqlFuture f = SqlCheckNames::go(semanticInfo.doc);
            m_highlighter = f;
            m_highlightRevision = semanticInfo.revision;
            m_highlightEngine = semanticInfo.m_parserEngine;
            m_nextHighlightBlockNumber = 0;
            m_highlightWatcher.setFuture(m_highlighter);
        }
    }
}

void SQLEditorWidget::updateUses()
{
    if ( editorRevision() != m_highlightRevision ||
         SqlParserEngineManager::instance()->currentEngine() != m_highlightEngine )
        m_highlighter.cancel();

    m_updateUsesTimer->start();
}

void SQLEditorWidget::updateUsesNow()
{
    semanticRehighlight();
}

void SQLEditorWidget::semanticRehighlight()
{
    m_semanticHighlighter->rehighlight(currentSource());
}

SemanticHighlighter::Source SQLEditorWidget::currentSource( bool force )
{
    unsigned line = 0, column = 0;
    convertPosition(position(), &line, &column);

    QString code;
    if ( force || m_lastSemanticInfo.revision != editorRevision() )
        code = toPlainText().replace("\r\n", "\n"); // get the source code only when needed.

    const unsigned revision = editorRevision();
    SemanticHighlighter::Source source(code, line, column, revision, SqlParserEngineManager::instance()->currentEngine());
    source.force = force;
    return source;
}

void SQLEditorWidget::highlightSymbolUsages(int from, int to)
{
    if ( editorRevision() != m_highlightRevision ||
         SqlParserEngineManager::instance()->currentEngine() != m_highlightEngine )
        return; // outdated

    else if (m_highlighter.isCanceled())
        return; // aborted

    SqlHighlighter *highlighter = qobject_cast<SqlHighlighter*>(baseTextDocument()->syntaxHighlighter());
    Q_ASSERT(highlighter);
    QTextDocument *doc = document();

    if (m_nextHighlightBlockNumber >= doc->blockCount())
        return;

    QMap<int, QVector<CheckName::Use *> > chunks = SqlCheckNames::chunks(m_highlighter, from, to);
    if (chunks.isEmpty())
        return;

    QTextBlock b = doc->findBlockByNumber(m_nextHighlightBlockNumber);

    QMapIterator<int, QVector<CheckName::Use *> > it(chunks);
    while (b.isValid() && it.hasNext()) {
        it.next();
        const int blockNumber = it.key();
        Q_ASSERT(blockNumber < doc->blockCount());

        while (m_nextHighlightBlockNumber < blockNumber) {
            highlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
            b = b.next();
            ++m_nextHighlightBlockNumber;
        }

        QList<QTextLayout::FormatRange> formats;
        foreach ( CheckName::Use *use, it.value() ) {
            QTextLayout::FormatRange formatRange;

            switch (use->kind) {

                case CheckName::Use::Schema:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticSchema];
                    break;

                case CheckName::Use::Table:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticTable];
                    break;

                case CheckName::Use::TableAlias:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticAlias];
                    break;

                case CheckName::Use::Field:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticColumn];
                    break;

                case CheckName::Use::Index:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticIndex];
                    break;

                case CheckName::Use::Trigger:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticTrigger];
                    break;

                case CheckName::Use::Constraint:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticConstraint];
                    break;

                case CheckName::Use::Bind:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticBind];
                    break;

                case CheckName::Use::Identifier:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticIdentifier];
                    break;

                case CheckName::Use::Label:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticLabel];
                    break;

                case CheckName::Use::Function:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticFunction];
                    break;

                case CheckName::Use::Cursor:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticCursor];
                    break;

                case CheckName::Use::StorageMap:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticMap];
                    break;

                case CheckName::Use::Area:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticArea];
                    break;

                case CheckName::Use::Module:
                    formatRange.format = m_sqlSemanticFormats[SqlSemanticModule];
                    break;

                default:
                    continue;
            }

            formatRange.start = use->column - 1;
            if ( !blockNumber )
                formatRange.start++;
            formatRange.length = use->length;
            formats.append(formatRange);

        }
        highlighter->setExtraAdditionalFormats(b, formats);
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}

void SQLEditorWidget::finishHighlightSymbolUsages()
{
    if ( editorRevision() != m_highlightRevision ||
         SqlParserEngineManager::instance()->currentEngine() != m_highlightEngine )
        return; // outdated

    else if (m_highlighter.isCanceled())
        return; // aborted

    SqlHighlighter *highlighter = qobject_cast<SqlHighlighter *>(baseTextDocument()->syntaxHighlighter());
    Q_ASSERT(highlighter);
    QTextDocument *doc = document();

    if (m_nextHighlightBlockNumber < doc->blockCount()) {
        QTextBlock b = doc->findBlockByNumber(m_nextHighlightBlockNumber);

        while (b.isValid()) {
            highlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
            b = b.next();
            ++m_nextHighlightBlockNumber;
        }
    }

    updateSemanticDiagnosticMessages(m_lastSemanticInfo.doc);
}

void SQLEditorWidget::rehighlight( bool force )
{
    const SemanticHighlighter::Source source = currentSource(force);
    m_semanticHighlighter->rehighlight(source);
}

void SQLEditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, Utils::CommentDefinition(false, "--"));
}

Core::ILink *SQLEditorWidget::findLinkAt( const QTextCursor &tc, bool resolveTarget )
{
    Q_UNUSED(resolveTarget)

    SemanticHighlighter::Source source = currentSource(true);

    Document::Ptr doc = Document::create(source.fileName);
    doc->setSource(source.code.toLatin1());

    return doc->findLinkAt(tc, source.parserEngine);
}

void SQLEditorWidget::updateSemanticDiagnosticMessages(Document::Ptr doc)
{
    QList<QTextEdit::ExtraSelection> selections;

    foreach (const Utils::DiagnosticMessage &m, doc->diagnosticMessages()) {

        const int pos = document()->findBlockByNumber(m.line()).position() + m.column() - 1 + ( m.line() ? 0 : 1 );
        if ( pos < 0 )
            continue;

        QTextEdit::ExtraSelection sel;
        if (m.isWarning())
            sel.format = m_warningFormat;
        else
            sel.format = m_errorFormat;

        QTextCursor cursor(document());
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, m.length());

        sel.cursor = cursor;
        sel.format.setToolTip(m.text());
        selections.append(sel);
    }

    setExtraSelections(TextEditor::TextEditorWidget::CodeWarningsSelection, selections);
}

} // namespace Editor
} // namespace Sql
