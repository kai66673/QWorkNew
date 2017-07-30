#include <QTextBlock>

#include "GrammarEditorWidget.h"
#include "grammareditorconstants.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include "SearchEngine.h"
#include "CppReplaceUsagesDialog.h"
#include "Core.h"
#include "basefilefind.h"
#include "VirtualFunctionHelper.h"
#include "cppvirtualfunctionassistprovider.h"

#include <AST.h>
#include <ASTPath.h>
#include <TranslationUnit.h>
#include <ExpressionUnderCursor.h>
#include <Scope.h>
#include <Symbols.h>
#include <Literals.h>
#include <SimpleLexer.h>
#include <CppDocument.h>
#include <BackwardsScanner.h>

#include <texteditorconstants.h>
#include <basetextdocument.h>
#include <basetextdocumentlayout.h>
#include <fontsettings.h>
#include <cppqtstyleindenter.h>
#include <cppautocompleter.h>
#include <cppeditorconstants.h>
#include <cppcompletionassist.h>
#include <cppmodelmanager.h>
#include <cppchecksymbols.h>
#include <cpplocalsymbols.h>

#include <string>

namespace Grammar {

enum {
    UPDATE_OUTLINE_INTERVAL = 500,
    UPDATE_USES_INTERVAL = 500
};

static QList<QTextEdit::ExtraSelection> createSelections(QTextDocument *document,
                                                         const QList<Utils::DiagnosticMessage> &msgs,
                                                         const QTextCharFormat &format)
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

namespace {

struct CanonicalSymbol
{
    EditorWidget *editor;
    CPlusPlus::TypeOfExpression typeOfExpression;
    CppEditor::Internal::SemanticInfo info;

    CanonicalSymbol( EditorWidget *editor, const CppEditor::Internal::SemanticInfo &info )
        : editor(editor), info(info)
    {
        typeOfExpression.init(info.doc, info.snapshot);
    }

    const CPlusPlus::LookupContext &context() const
    {
        return typeOfExpression.context();
    }

    static inline bool isIdentifierChar(const QChar &ch)
    {
        return ch.isLetterOrNumber() || ch == QLatin1Char('_');
    }

    CPlusPlus::Scope *getScopeAndExpression(const QTextCursor &cursor, QString *code)
    {
        return getScopeAndExpression(editor, info, cursor, code);
    }

    static CPlusPlus::Scope *getScopeAndExpression( EditorWidget *editor, const CppEditor::Internal::SemanticInfo &info,
                                                    const QTextCursor &cursor, QString *code )
    {
        if (! info.doc)
            return 0;

        QTextCursor tc = cursor;
        unsigned line, col;
        editor->convertPosition(tc.position(), &line, &col);
        ++col; // 1-based line and 1-based column

        QTextDocument *document = editor->document();

        int pos = tc.position();

        if (! isIdentifierChar(document->characterAt(pos)))
            if (! (pos > 0 && isIdentifierChar(document->characterAt(pos - 1))))
                return 0;

        while (isIdentifierChar(document->characterAt(pos)))
            ++pos;
        tc.setPosition(pos);

        CPlusPlus::ExpressionUnderCursor expressionUnderCursor;
        *code = expressionUnderCursor(tc);
        return info.doc->scopeAt(line, col, editor->document());
    }

    CPlusPlus::Symbol *operator()(const QTextCursor &cursor)
    {
        QString code;

        if (CPlusPlus::Scope *scope = getScopeAndExpression(cursor, &code)) {
            if ( CPlusPlus::GrammarScope *grammarScope = scope->asGrammarScope() ) {
                CPlusPlus::SimpleLexer tokenize;
                tokenize.setQtMocRunEnabled(true);
                const QTextBlock block = cursor.block();
                const QString blockText = block.text();
                const QList<CPlusPlus::Token> tokens = tokenize(blockText, CPlusPlus::BackwardsScanner::previousBlockState(block));
                int pos = cursor.positionInBlock();
                int tokIndex = CPlusPlus::SimpleLexer::tokenAt(tokens, pos);
                if ( tokIndex == -1 )
                    return 0;
                const CPlusPlus::Token &tk = tokens.at(tokIndex);
                if ( tk.isNot(CPlusPlus::T_IDENTIFIER) )
                    return 0;
                QString grExpression = block.text().mid(tk.offset, tk.length());
                CPlusPlus::Identifier id(grExpression.toUtf8().constData(), grExpression.length());
                if ( tokIndex > 0 && tokens.at(tokIndex - 1).is(CPlusPlus::T_LESS))
                    return grammarScope->findSemanticTypeSymbol(&id);

                if ( !code.isEmpty() && code != "prec" ) {
                    CPlusPlus::Identifier id(code.toUtf8().constData(), code.length());
                    for ( CPlusPlus::Symbol *s = grammarScope->find(&id); s; s = s->next() )
                        if ( s->identifier()->isEqualTo(&id) )
                            return s;
                }
                return 0;
            }
            return operator()(scope, code);
        }

        return 0;
    }

    CPlusPlus::Symbol *operator()(CPlusPlus::Scope *scope, const QString &code)
    {
        return canonicalSymbol(scope, code, typeOfExpression);
    }

    static CPlusPlus::Symbol *canonicalSymbol(CPlusPlus::Scope *scope, const QString &code, CPlusPlus::TypeOfExpression &typeOfExpression)
    {
        const QList<CPlusPlus::LookupItem> results = typeOfExpression(code, scope, CPlusPlus::TypeOfExpression::Preprocess);

        for (int i = results.size() - 1; i != -1; --i) {
            const CPlusPlus::LookupItem &r = results.at(i);
            CPlusPlus::Symbol *decl = r.declaration();

            if (! (decl && decl->enclosingScope()))
                break;

            if (CPlusPlus::Class *classScope = r.declaration()->enclosingScope()->asClass()) {
                const CPlusPlus::Identifier *declId = decl->identifier();
                const CPlusPlus::Identifier *classId = classScope->identifier();

                if (classId && classId->isEqualTo(declId))
                    continue; // skip it, it's a ctor or a dtor.

                else if (CPlusPlus::Function *funTy = r.declaration()->type()->asFunctionType()) {
                    if (funTy->isVirtual())
                        return r.declaration();
                }
            }
        }

        for (int i = 0; i < results.size(); ++i) {
            const CPlusPlus::LookupItem &r = results.at(i);

            if (r.declaration())
                return r.declaration();
        }

        return 0;
    }

};

}

GrammarEditor::GrammarEditor( EditorWidget *editorWidget )
    : BaseTextEditor(editorWidget)
{
    setContext( Core::Context( Constants::C_GRAMMAREDITOR_ID,
                               TextEditor::Constants::C_TEXTEDITOR,
                               CppEditor::Constants::C_CPPEDITOR ));
}

GrammarEditor::~GrammarEditor()
{ }

Core::IEditor *GrammarEditor::duplicate( QWidget *parent )
{
    Q_UNUSED(parent)
    return 0;
}

/**
 * @returns Unique editor class identifier, that is Constants::C_PYTHONEDITOR_ID
 */
QString GrammarEditor::id() const
{
    return QLatin1String(Constants::C_GRAMMAREDITOR_ID);
}

bool GrammarEditor::open(QString *errorString,
                        const QString &fileName,
                        const QString &realFileName)
{
    bool status = TextEditor::BaseTextEditor::open(errorString,
                                                   fileName,
                                                   realFileName);
    return status;
}

class FunctionDefinitionUnderCursor: protected CPlusPlus::ASTVisitor
{
    unsigned _line;
    unsigned _column;
    CPlusPlus::DeclarationAST *_functionDefinition;

public:
    FunctionDefinitionUnderCursor(CPlusPlus::TranslationUnit *translationUnit)
        : CPlusPlus::ASTVisitor(translationUnit),
          _line(0), _column(0)
    { }

    CPlusPlus::DeclarationAST *operator()(CPlusPlus::AST *ast, unsigned line, unsigned column)
    {
        _functionDefinition = 0;
        _line = line;
        _column = column;
        accept(ast);
        return _functionDefinition;
    }

protected:
    virtual bool preVisit(CPlusPlus::AST *ast)
    {
        if (_functionDefinition)
            return false;

        else if (CPlusPlus::FunctionDefinitionAST *def = ast->asFunctionDefinition()) {
            return checkDeclaration(def);
        }

        else if (CPlusPlus::ObjCMethodDeclarationAST *method = ast->asObjCMethodDeclaration()) {
            if (method->function_body)
                return checkDeclaration(method);
        }

        return true;
    }

private:
    bool checkDeclaration(CPlusPlus::DeclarationAST *ast)
    {
        unsigned startLine, startColumn;
        unsigned endLine, endColumn;
        getTokenStartPosition(ast->firstToken(), &startLine, &startColumn);
        getTokenEndPosition(ast->lastToken() - 1, &endLine, &endColumn);

        if (_line > startLine || (_line == startLine && _column >= startColumn)) {
            if (_line < endLine || (_line == endLine && _column < endColumn)) {
                _functionDefinition = ast;
                return false;
            }
        }

        return true;
    }
};

SemanticHighlighter::SemanticHighlighter( QObject *parent )
    : QThread(parent)
    , m_done(false)
{ }

SemanticHighlighter::~SemanticHighlighter()
{ }

void SemanticHighlighter::abort()
{
    QMutexLocker locker(&m_mutex);
    m_done = true;
    m_condition.wakeOne();
}

void SemanticHighlighter::rehighlight( const Source &source )
{
    QMutexLocker locker(&m_mutex);
    m_source = source;
    m_condition.wakeOne();
}

bool SemanticHighlighter::isOutdated()
{
    QMutexLocker locker(&m_mutex);
    const bool outdated = ! m_source.fileName.isEmpty() || m_done;
    return outdated;
}

void SemanticHighlighter::run()
{
    setPriority(QThread::LowestPriority);

    forever {
        m_mutex.lock();

        while (! (m_done || ! m_source.fileName.isEmpty()))
            m_condition.wait(&m_mutex);

        const bool done = m_done;
        const Source source = m_source;
        m_source.clear();

        m_mutex.unlock();

        if (done)
            break;

        const CppEditor::Internal::SemanticInfo info = semanticInfo(source);

        if (! isOutdated()) {
            m_mutex.lock();
            m_lastSemanticInfo = info;
            m_mutex.unlock();

            emit changed(info);
        }
    }
}

CppEditor::Internal::SemanticInfo SemanticHighlighter::semanticInfo(const Source &source)
{
    m_mutex.lock();
    const int revision = m_lastSemanticInfo.revision;
    m_mutex.unlock();

    CPlusPlus::Snapshot snapshot;
    CPlusPlus::Document::Ptr doc;
    QList<Utils::DiagnosticMessage> diagnosticMessages;
    QList<CppEditor::Internal::SemanticInfo::Use> objcKeywords;

    if (! source.force && revision == source.revision) {
        m_mutex.lock();
        snapshot = m_lastSemanticInfo.snapshot; // ### TODO: use the new snapshot.
        doc = m_lastSemanticInfo.doc;
        diagnosticMessages = m_lastSemanticInfo.diagnosticMessages;
        objcKeywords = m_lastSemanticInfo.objcKeywords;
        m_mutex.unlock();
    }

    if (! doc) {
        snapshot = source.snapshot;
        const QByteArray preprocessedCode = snapshot.preprocessedCode(source.code, source.fileName);

        doc = snapshot.documentFromSource(preprocessedCode, source.fileName);
        doc->control()->setTopLevelDeclarationProcessor(this);
        doc->check();
    }

    CPlusPlus::TranslationUnit *translationUnit = doc->translationUnit();
    CPlusPlus::AST *ast = translationUnit->ast();

    FunctionDefinitionUnderCursor functionDefinitionUnderCursor(translationUnit);
    CPlusPlus::DeclarationAST *currentFunctionDefinition = functionDefinitionUnderCursor(ast, source.line, source.column);

    const CppEditor::Internal::LocalSymbols useTable(doc, currentFunctionDefinition);

    CppEditor::Internal::SemanticInfo semanticInfo;
    semanticInfo.revision = source.revision;
    semanticInfo.snapshot = snapshot;
    semanticInfo.doc = doc;
    semanticInfo.localUses = useTable.uses;
    semanticInfo.hasQ = useTable.hasQ;
    semanticInfo.hasD = useTable.hasD;
    semanticInfo.forced = source.force;
    semanticInfo.diagnosticMessages = diagnosticMessages;
    semanticInfo.objcKeywords = objcKeywords;

    return semanticInfo;
}

EditorWidget::EditorWidget(IDocument *document, QToolBar *toolBar, QWidget *parent)
    : TextEditor::TextEditorWidget(document, parent)
    , m_toolBar(toolBar)
    , m_outlineCombo(new QComboBox)
    , m_currentRenameSelection(NoCurrentRenameSelection)
    , m_inRename(false)
    , m_inRenameChanged(false)
    , m_firstRenameChange(false)
    , m_virtualFunctionAssistProvider(new CppEditor::Internal::VirtualFunctionAssistProvider)
{
    m_initialized = false;
    qRegisterMetaType<CppEditor::Internal::SemanticInfo>("CppEditor::Internal::SemanticInfo");

    m_commentDefinition.setMultiLineStart(QLatin1String("/*"));
    m_commentDefinition.setMultiLineEnd(QLatin1String("*/"));
    m_commentDefinition.setSingleLine(QString());

    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);

    m_diagnosticMessageFormat.setUnderlineColor(Qt::darkYellow); // ### hardcoded
    m_diagnosticMessageFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline); // ### hardcoded

    m_semanticHighlighter = new SemanticHighlighter(this);
    m_semanticHighlighter->start();

    setIndenter(new CppTools::CppQtStyleIndenter);
    setAutoCompleter(new CppEditor::Internal::CppAutoCompleter);

    m_grammarHighlighter = new GrammarHighlighter;
    baseTextDocument()->setSyntaxHighlighter(m_grammarHighlighter);

    m_modelManager = CPlusPlus::CppModelManagerInterface::instance();
    if (m_modelManager) {
        connect(m_modelManager, SIGNAL(documentUpdated(CPlusPlus::Document::Ptr)),
                this, SLOT(onDocumentUpdated(CPlusPlus::Document::Ptr)));
    }

    m_highlightRevision = 0;
    m_nextHighlightBlockNumber = 0;
    connect(&m_highlightWatcher, SIGNAL(resultsReadyAt(int,int)), SLOT(highlightSymbolUsages(int,int)));
    connect(&m_highlightWatcher, SIGNAL(finished()), SLOT(finishHighlightSymbolUsages()));

    m_referencesRevision = 0;
    m_referencesCursorPosition = 0;
    connect(&m_referencesWatcher, SIGNAL(finished()), SLOT(markSymbolsNow()));
}

EditorWidget::~EditorWidget()
{
    m_semanticHighlighter->abort();
    m_semanticHighlighter->wait();
    delete m_virtualFunctionAssistProvider;
}

void EditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, m_commentDefinition);
}

void EditorWidget::onSettingsChanged()
{
    TextEditor::TextEditorWidget::onSettingsChanged();
    m_grammarHighlighter->onFormatSettingsChanged();
    onSemanticFormatSettingsChanged();
}

void EditorWidget::onSemanticFormatSettingsChanged()
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("Cpp"));
    int offset = GrammarHighlighter::NumCppFormats;
    for (int i = 0; i < CppSemanticFormatCount; i++, offset++)
        formatData->fillTextCharFormat(offset, m_cppSemanticFormats[i]);

    m_cppSemanticFormats[CppSemanticOccurrencesUnused].setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_cppSemanticFormats[CppSemanticOccurrencesUnused].setUnderlineColor(m_cppSemanticFormats[CppSemanticOccurrencesUnused].foreground().color());
    m_cppSemanticFormats[CppSemanticOccurrencesUnused].clearForeground();
    m_cppSemanticFormats[CppSemanticOccurrencesUnused].setToolTip(tr("Unused variable"));
    m_grammarTermUnusedFormat = m_cppSemanticFormats[CppSemanticOccurrencesUnused];
    m_grammarTermUnusedFormat.setToolTip(tr("Unused Grammar Term"));

    semanticRehighlight();
}

void EditorWidget::paste()
{
    if ( m_currentRenameSelection == NoCurrentRenameSelection ) {
        TextEditorWidget::paste();
        return;
    }

    startRename();
    TextEditorWidget::paste();
    finishRename();
}

void EditorWidget::cut()
{
    if ( m_currentRenameSelection == NoCurrentRenameSelection ) {
        TextEditorWidget::cut();
        return;
    }

    startRename();
    TextEditorWidget::cut();
    finishRename();
}

Core::ILink *EditorWidget::linkToSymbol( int begin, int end, CPlusPlus::Symbol *symbol )
{
    if ( !symbol )
        return 0;

    const QString fileName = QString::fromUtf8(symbol->fileName(),
                                               symbol->fileNameLength());
    unsigned line = symbol->line();
    unsigned column = symbol->column();

    if (column)
        --column;

    if ( symbol->isGenerated() && symbol->identifier()->chars()[0] != '$' )
        column = 0;

    return new Core::TextTargetLink(begin, end, fileName, line, column);
}

QVector<QString> EditorWidget::highlighterFormatCategories()
{
    static QVector<QString> categories;
    if (categories.isEmpty()) {
        categories << QLatin1String(TextEditor::Constants::C_NUMBER)
                   << QLatin1String(TextEditor::Constants::C_STRING)
                   << QLatin1String(TextEditor::Constants::C_TYPE)
                   << QLatin1String(TextEditor::Constants::C_KEYWORD)
                   << QLatin1String(TextEditor::Constants::C_OPERATOR)
                   << QLatin1String(TextEditor::Constants::C_PREPROCESSOR)
                   << QLatin1String(TextEditor::Constants::C_LABEL)
                   << QLatin1String(TextEditor::Constants::C_COMMENT)
                   << QLatin1String(TextEditor::Constants::C_DOXYGEN_COMMENT)
                   << QLatin1String(TextEditor::Constants::C_DOXYGEN_TAG)
                   << QLatin1String(TextEditor::Constants::C_VISUAL_WHITESPACE)
                   << QLatin1String(TextEditor::Constants::C_EXTRA_KEYWORD);
    }
    return categories;
}

TextEditor::IAssistInterface *EditorWidget::createAssistInterface( TextEditor::AssistKind kind, TextEditor::AssistReason reason ) const
{
    if (kind == TextEditor::Completion) {
        CppTools::Internal::CppModelManager *model = static_cast<CppTools::Internal::CppModelManager *>(m_modelManager);
        if ( !model )
            return 0;

        QString fileName(editor()->file()->fileName());
        QList<CppTools::ProjectPart::Ptr> projectParts =  model->projectPart(fileName);

        QStringList includePaths;
        QStringList frameworkPaths;
        if ( !projectParts.isEmpty() ) {
            foreach ( CppTools::ProjectPart::Ptr part, projectParts ) {
                foreach ( const CppTools::ProjectPart::HeaderPath &path, part->headerPaths ) {
                    switch ( path.type ) {
                    case CppTools::ProjectPart::HeaderPath::IncludePath:
                        includePaths << path.path;
                        break;
                    case CppTools::ProjectPart::HeaderPath::FrameworkPath:
                        frameworkPaths << path.path;
                        break;
                    }
                }
            }
        }

        if ( includePaths.isEmpty() ) {
            QFileInfo fi(fileName);
            includePaths << fi.dir().canonicalPath();
        }

        return new CppTools::Internal::CppCompletionAssistInterface(
                    document(),
                    position(),
                    editor()->file(),
                    reason,
                    m_modelManager->snapshot(),
                    includePaths,
                    frameworkPaths);
    } else {
        return TextEditorWidget::createAssistInterface(kind, reason);
    }

    return 0;
}

void EditorWidget::setFontSettings( const TextEditor::FontSettings &fs )
{
    return;
//    TextEditor::TextEditorWidget::setFontSettings(fs);
//    const QVector<QTextCharFormat> formats = fs.toTextCharFormats(highlighterFormatCategories());
//    m_grammarHighlighter->setFormats(formats.constBegin(), formats.constEnd());

//    m_occurrencesFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_OCCURRENCES));
//    m_occurrencesUnusedFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_OCCURRENCES_UNUSED));
//    m_occurrencesUnusedFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
//    m_occurrencesUnusedFormat.setUnderlineColor(m_occurrencesUnusedFormat.foreground().color());
//    m_occurrencesUnusedFormat.clearForeground();
//    m_occurrencesUnusedFormat.setToolTip(tr("Unused variable"));
//    m_grammarTermUnusedFormat = m_occurrencesUnusedFormat;
//    m_grammarTermUnusedFormat.setToolTip(tr("Unused Grammar Term"));
//    m_occurrenceRenameFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_OCCURRENCES_RENAME));
//    m_typeFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_TYPE));
//    m_localFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_LOCAL));
//    m_fieldFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_FIELD));
//    m_staticFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_STATIC));
//    m_virtualMethodFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_VIRTUAL_METHOD));
//    m_functionFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_FUNCTION));
//    m_keywordFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_KEYWORD));

//    m_sqlTableAliasFormat = fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_RESERVED1));

//    // only set the background, we do not want to modify foreground properties set by the syntax highlighter or the link
//    m_occurrencesFormat.clearForeground();
//    m_grammarTermUnusedFormat.clearForeground();
//    m_occurrenceRenameFormat.clearForeground();
}

bool EditorWidget::renameSymbolUnderCursor()
{
    updateSemanticInfo(m_semanticHighlighter->semanticInfo(currentSource()));
    abortRename();

    QTextCursor c = textCursor();

    for (int i = 0; i < m_renameSelections.size(); ++i) {
        QTextEdit::ExtraSelection s = m_renameSelections.at(i);
        if (c.position() >= s.cursor.anchor()
                && c.position() <= s.cursor.position()) {
            m_currentRenameSelection = i;
            m_firstRenameChange = true;
            m_currentRenameSelectionBegin = QTextCursor(c.document()->docHandle(),
                                                        m_renameSelections[i].cursor.selectionStart());
            m_currentRenameSelectionEnd = QTextCursor(c.document()->docHandle(),
                                                        m_renameSelections[i].cursor.selectionEnd());
            m_renameSelections[i].format = m_occurrenceRenameFormat;
            setExtraSelections(CodeSemanticsSelection, m_renameSelections);
            break;
        }
    }

    return !m_renameSelections.isEmpty();
}

void EditorWidget::findUsages()
{
    CppEditor::Internal::SemanticInfo info = m_lastSemanticInfo;
    info.snapshot = CPlusPlus::CppModelManagerInterface::instance()->snapshot();
    info.snapshot.insert(info.doc);

    CanonicalSymbol cs(this, info);
    CPlusPlus::Symbol *canonicalSymbol = cs(textCursor());
    if (canonicalSymbol) {
        if ( canonicalSymbol->isGrammarSymbol() )
            findGrammarUsages(canonicalSymbol, info);
        else {
            if ( canonicalSymbol->isDeclaration() ) {
                if ( CPlusPlus::GrammarScope *gs = info.doc->grammarScope() ) {
                    if ( CPlusPlus::Class *unionClass = gs->unionCLass() ) {
                        if ( unionClass == canonicalSymbol->ownerScope() ) {
                            findGrammarSemanticTypeUsages(canonicalSymbol, info);
                            return;
                        }
                    }
                }
            }
            m_modelManager->findUsages(canonicalSymbol, cs.context());
        }
    } else if (const CPlusPlus::Macro *macro = findCanonicalMacro(textCursor(), info.doc)) {
        m_modelManager->findMacroUsages(*macro);
    }
}

void EditorWidget::renameUsages()
{
    CppEditor::Internal::SemanticInfo info = m_lastSemanticInfo;
    info.snapshot = CPlusPlus::CppModelManagerInterface::instance()->snapshot();
    info.snapshot.insert(info.doc);

    CanonicalSymbol cs(this, info);
    CPlusPlus::Symbol *canonicalSymbol = cs(textCursor());
    if (canonicalSymbol) {
        if ( canonicalSymbol->identifier()->chars()[0] == '$' ) // TODO: Release more accurate check
            return;
        if ( canonicalSymbol->isGrammarSymbol() )
            findGrammarUsages(canonicalSymbol, info, true);
        else {
            if ( canonicalSymbol->isDeclaration() ) {
                if ( CPlusPlus::GrammarScope *gs = info.doc->grammarScope() ) {
                    if ( CPlusPlus::Class *unionClass = gs->unionCLass() ) {
                        if ( unionClass == canonicalSymbol->ownerScope() ) {
                            findGrammarSemanticTypeUsages(canonicalSymbol, info, true);
                            return;
                        }
                    }
                }
            }
            if ( !renameSymbolUnderCursor() )
                m_modelManager->renameUsages(canonicalSymbol, cs.context(), QString());
        }
    }
}

void EditorWidget::jumpToDefinition()
{
    if ( Core::ILink *link = findLinkAt(textCursor()) ) {
        openLink(link);
        delete link;
    }
}

void EditorWidget::jumpToOutlineElement( int index )
{
    if ( index > 0 ) {
       int lineno = m_outlineModel->data(m_outlineModel->index(index), GrammarRulesModel::LineNumberRole ).toInt();
       if ( lineno != -1 ) {
           if ( IDocument *document = DocumentManager::getInstance()->currentDocument() ) {
               TextDocumentCursor position(lineno, 0);
               document->setCursor(&position);
           }
       }
    }
}

void EditorWidget::updateOutlineNow()
{
    const CPlusPlus::Snapshot snapshot = m_modelManager->snapshot();
    CPlusPlus::Document::Ptr doc = snapshot.document(file()->fileName());

    if (!doc)
        return;

    if ( doc->editorRevision() != document()->revision() ) {
        m_updateOutlineTimer->start();
        return;
    }

    m_outlineModel->rebuild(doc);
    updateOutlineIndexNow();
}

void EditorWidget::updateOutlineIndex()
{
    m_updateOutlineIndexTimer->start();
}

void EditorWidget::updateOutlineIndexNow()
{
    if ( !m_outlineModel->document() )
        return;

    if ( m_outlineModel->document()->editorRevision() != document()->revision() ) {
        m_updateOutlineIndexTimer->start();
        return;
    }

    m_updateOutlineIndexTimer->stop();

    m_outlineModelIndex = QModelIndex(); //invalidate
    QModelIndex comboIndex = outlineModelIndex();

    if (comboIndex.isValid()) {
        bool blocked = m_outlineCombo->blockSignals(true);
        m_outlineCombo->setCurrentIndex(comboIndex.row());
        m_outlineCombo->blockSignals(blocked);
    }
}

void EditorWidget::updateUses()
{
    if ( document()->revision() != m_highlightRevision )
        m_highlighter.cancel();
    m_updateUsesTimer->start();
}

void EditorWidget::updateUsesNow()
{
    if ( m_currentRenameSelection != NoCurrentRenameSelection )
        return;

    semanticRehighlight();
}

void EditorWidget::onDocumentUpdated( CPlusPlus::Document::Ptr doc )
{
    if ( doc->fileName() != file()->fileName() )
        return;

    if ( doc->editorRevision() != document()->revision() )
        return;

    if (! m_initialized) {
        m_initialized = true;
        rehighlight(/* force = */ true);
    }

    m_updateOutlineTimer->start();
}

void EditorWidget::onContentsChanged( int position, int charsRemoved, int charsAdded )
{
    if ( m_currentRenameSelection == NoCurrentRenameSelection || m_inRename )
        return;

    if ( position + charsAdded == m_currentRenameSelectionBegin.position() ) {
        // we are inserting at the beginning of the rename selection => expand
        m_currentRenameSelectionBegin.setPosition(position);
        m_renameSelections[m_currentRenameSelection].cursor.setPosition(position, QTextCursor::KeepAnchor);
    }

    // the condition looks odd, but keep in mind that the begin and end cursors do move automatically
    m_inRenameChanged = (position >= m_currentRenameSelectionBegin.position()
                         && position + charsAdded <= m_currentRenameSelectionEnd.position());

    if (!m_inRenameChanged)
        abortRename();

    if (charsRemoved > 0)
        updateUses();
}

void EditorWidget::rehighlight( bool force )
{
    const SemanticHighlighter::Source source = currentSource(force);
    m_semanticHighlighter->rehighlight(source);
}

void EditorWidget::semanticRehighlight()
{
    m_semanticHighlighter->rehighlight(currentSource());
}

void EditorWidget::updateSemanticInfo( const CppEditor::Internal::SemanticInfo &semanticInfo )
{
    if ( semanticInfo.revision != document()->revision() ) {
        // got outdated semantic info
        semanticRehighlight();
        return;
    }

    const CppEditor::Internal::SemanticInfo previousSemanticInfo = m_lastSemanticInfo;
    m_lastSemanticInfo = semanticInfo; // update the semantic info

    unsigned line = 0, column = 0;
    convertPosition(position(), &line, &column);

    QList<QTextEdit::ExtraSelection> unusedSelections;

    m_renameSelections.clear();
    m_currentRenameSelection = NoCurrentRenameSelection;

    CppEditor::Internal::SemanticInfo::LocalUseIterator it(semanticInfo.localUses);
    while ( it.hasNext() ) {
        it.next();
        const QList<CppEditor::Internal::SemanticInfo::Use> &uses = it.value();

        bool good = false;
        foreach (const CppEditor::Internal::SemanticInfo::Use &use, uses) {
            unsigned l = line;
            unsigned c = column + 1; // convertCursorPosition() returns a 0-based column number.
            if (l == use.line && c >= use.column && c <= (use.column + use.length)) {
                good = true;
                break;
            }
        }

        if ( uses.size() == 1 ) {
            if ( uses[0].kind != CppEditor::Internal::SemanticInfo::Use::SqlTableAliasShadowed )
                // it's an unused declaration
                highlightUses(uses, semanticInfo, &unusedSelections);
        }

        else if (good && m_renameSelections.isEmpty())
            highlightUses(uses, semanticInfo, &m_renameSelections);
    }

    if (m_lastSemanticInfo.forced || previousSemanticInfo.revision != semanticInfo.revision) {
        setExtraSelections(UndefinedSymbolSelection, createSelections(document(),
                                                                      semanticInfo.diagnosticMessages,
                                                                      m_diagnosticMessageFormat));

        m_highlighter.cancel();

        if ( semanticInfo.doc && checkApplySizeForSemanticHighlighter(semanticInfo.doc->source().size()) ) {
            CPlusPlus::LookupContext context(semanticInfo.doc, semanticInfo.snapshot);
            CPlusPlus::CheckSymbols::Future f = CPlusPlus::CheckSymbols::go(semanticInfo.doc, context);
            m_highlighter = f;
            m_highlightRevision = semanticInfo.revision;
            m_nextHighlightBlockNumber = 0;
            m_highlightWatcher.setFuture(m_highlighter);
        }
    }

    setExtraSelections(UnusedSymbolSelection, unusedSelections);

    if (! m_renameSelections.isEmpty())
        setExtraSelections(CodeSemanticsSelection, m_renameSelections); // ###
    else {
        markSymbols(textCursor(), semanticInfo);
    }

    m_lastSemanticInfo.forced = false; // clear the forced flag
}

void EditorWidget::highlightSymbolUsages( int from, int to )
{
    if ( document()->revision() != m_highlightRevision )
        return; // outdated

    else if ( m_highlighter.isCanceled() )
        return; // aborted

    QTextDocument *doc = document();

    if (m_nextHighlightBlockNumber >= doc->blockCount())
        return;

    QMap<int, QVector<CppEditor::Internal::SemanticInfo::Use> > chunks = CPlusPlus::CheckSymbols::chunks(m_highlighter, from, to);
    if (chunks.isEmpty())
        return;

    QTextBlock b = doc->findBlockByNumber(m_nextHighlightBlockNumber);

    QMapIterator<int, QVector<CppEditor::Internal::SemanticInfo::Use> > it(chunks);
    while (b.isValid() && it.hasNext()) {
        it.next();
        const int blockNumber = it.key();
        Q_ASSERT(blockNumber < doc->blockCount());

        while (m_nextHighlightBlockNumber < blockNumber) {
            m_grammarHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
            b = b.next();
            ++m_nextHighlightBlockNumber;
        }

        QList<QTextLayout::FormatRange> formats;
        foreach (const CppEditor::Internal::SemanticInfo::Use &use, it.value()) {
            QTextLayout::FormatRange formatRange;

            switch (use.kind) {
            case CppEditor::Internal::SemanticInfo::Use::Type:
                formatRange.format = m_cppSemanticFormats[CppSemanticType];
                break;

            case CppEditor::Internal::SemanticInfo::Use::Field:
                formatRange.format = m_cppSemanticFormats[CppSemanticField];
                break;

            case CppEditor::Internal::SemanticInfo::Use::Local:
                formatRange.format = m_cppSemanticFormats[CppSemanticLocal];
                break;

            case CppEditor::Internal::SemanticInfo::Use::Static:
                formatRange.format = m_cppSemanticFormats[CppSemanticStatic];
                break;

            case CppEditor::Internal::SemanticInfo::Use::VirtualMethod:
                formatRange.format = m_cppSemanticFormats[CppSemanticVirtualMethod];
                break;

            case CppEditor::Internal::SemanticInfo::Use::Function:
                formatRange.format = m_cppSemanticFormats[CppSemanticFunction];
                break;

            case CppEditor::Internal::SemanticInfo::Use::Label:
                formatRange.format = m_grammarHighlighter->labelFormat();
                break;

            case CppEditor::Internal::SemanticInfo::Use::SqlTableAlias:
                formatRange.format = m_cppSemanticFormats[CppSemanticSqlTableAlias];
                break;

            default:
                continue;
            }

            formatRange.start = use.column - 1;
            formatRange.length = use.length;
            formats.append(formatRange);
        }
        m_grammarHighlighter->setExtraAdditionalFormats(b, formats);
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}

void EditorWidget::finishHighlightSymbolUsages()
{
    if ( document()->revision() != m_highlightRevision )
        return; // outdated

    else if ( m_highlighter.isCanceled() )
        return; // aborted

    QTextDocument *doc = document();

    if ( m_nextHighlightBlockNumber >= doc->blockCount() )
        return;

    QTextBlock b = doc->findBlockByNumber(m_nextHighlightBlockNumber);

    while (b.isValid()) {
        m_grammarHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}

TextEditor::BaseTextEditor *EditorWidget::createEditor()
{
    GrammarEditor *editable = new GrammarEditor(this);
    createToolBar(editable);
    return editable;
}

const CPlusPlus::Macro *EditorWidget::findCanonicalMacro( const QTextCursor &cursor, CPlusPlus::Document::Ptr doc ) const
{
    if (! doc)
        return 0;

    unsigned line, col;
    convertPosition(cursor.position(), &line, &col);

    if (const CPlusPlus::Macro *macro = doc->findMacroDefinitionAt(line))
        return macro;

    if (const CPlusPlus::Document::MacroUse *use = doc->findMacroUseAt(cursor.position()))
        return &use->macro();

    return 0;
}

void EditorWidget::createToolBar( GrammarEditor *editable )
{
    Q_UNUSED(editable)

    m_outlineCombo->setMinimumContentsLength(22);

    // Make the combo box prefer to expand
    QSizePolicy policy = m_outlineCombo->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    m_outlineCombo->setSizePolicy(policy);

    m_outlineModel = new GrammarRulesModel();
    m_outlineCombo->setModel(m_outlineModel);

    m_outlineCombo->setMaxVisibleItems(20);

    m_updateOutlineTimer = new QTimer(this);
    m_updateOutlineTimer->setSingleShot(true);
    m_updateOutlineTimer->setInterval(UPDATE_OUTLINE_INTERVAL);
    connect(m_updateOutlineTimer, SIGNAL(timeout()), this, SLOT(updateOutlineNow()));

    m_updateOutlineIndexTimer = new QTimer(this);
    m_updateOutlineIndexTimer->setSingleShot(true);
    m_updateOutlineIndexTimer->setInterval(UPDATE_OUTLINE_INTERVAL);
    connect(m_updateOutlineIndexTimer, SIGNAL(timeout()), this, SLOT(updateOutlineIndexNow()));

    m_updateUsesTimer = new QTimer(this);
    m_updateUsesTimer->setSingleShot(true);
    m_updateUsesTimer->setInterval(UPDATE_USES_INTERVAL);
    connect(m_updateUsesTimer, SIGNAL(timeout()), this, SLOT(updateUsesNow()));

    connect(m_outlineCombo, SIGNAL(activated(int)), this, SLOT(jumpToOutlineElement(int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateOutlineIndex()));
    connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(onContentsChanged(int,int,int)));

    // set up the semantic highlighter
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateUses()));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateUses()));

    connect(m_semanticHighlighter, SIGNAL(changed(CppEditor::Internal::SemanticInfo)),
            this, SLOT(updateSemanticInfo(CppEditor::Internal::SemanticInfo)));

    m_toolBar->insertWidget(m_toolBar->actions().first(), m_outlineCombo);
}

QModelIndex EditorWidget::outlineModelIndex()
{
    if ( !m_outlineModelIndex.isValid() ) {
        unsigned line = 0, column = 0;
        convertPosition(position(), &line, &column);
        m_outlineModelIndex = indexForPosition(line);
    }

    return m_outlineModelIndex;
}

QModelIndex EditorWidget::indexForPosition( int line ) const
{
    int prevLine = -1;
    const int rowCount = m_outlineModel->rowCount();
    for (int row = 1; row < rowCount; ++row) {
        int currLine = m_outlineModel->data(m_outlineModel->index(row), GrammarRulesModel::LineNumberRole).toInt();
        if ( line >= prevLine && line < currLine )
            return m_outlineModel->index(row - 1);
        prevLine = currLine;
    }

    return m_outlineModel->index(rowCount - 1);
}

static QList<int> lazyFindReferences(CPlusPlus::Scope *scope, QString code, CPlusPlus::Document::Ptr doc, CPlusPlus::Snapshot snapshot)
{
    CPlusPlus::TypeOfExpression typeOfExpression;
    snapshot.insert(doc);
    typeOfExpression.init(doc, snapshot);
    if (CPlusPlus::Symbol *canonicalSymbol = CanonicalSymbol::canonicalSymbol(scope, code, typeOfExpression)) {
        return CPlusPlus::CppModelManagerInterface::instance()->references(canonicalSymbol, typeOfExpression.context());
    }
    return QList<int>();
}

void EditorWidget::markSymbols( const QTextCursor &tc, const CppEditor::Internal::SemanticInfo &info )
{
    abortRename();

    if (! info.doc)
        return;

    CanonicalSymbol cs(this, info);
    QString expression;
    if (CPlusPlus::Scope *scope = cs.getScopeAndExpression(this, info, tc, &expression)) {
        m_references.cancel();
        m_referencesRevision = info.revision;
        m_referencesCursorPosition = position();
        m_references = QtConcurrent::run(&lazyFindReferences, scope, expression, info.doc, info.snapshot);
        m_referencesWatcher.setFuture(m_references);
    } else {
        const QList<QTextEdit::ExtraSelection> selections = extraSelections(CodeSemanticsSelection);

        if (! selections.isEmpty())
            setExtraSelections(CodeSemanticsSelection, QList<QTextEdit::ExtraSelection>());
    }
}

void EditorWidget::markSymbolsNow()
{
    if (m_references.isCanceled())
        return;
    else if ( m_referencesCursorPosition != position() )
        return;
    else if ( m_referencesRevision != document()->revision() )
        return;

    const CppEditor::Internal::SemanticInfo info = m_lastSemanticInfo;
    CPlusPlus::TranslationUnit *unit = info.doc->translationUnit();
    const QList<int> result = m_references.result();

    QList<QTextEdit::ExtraSelection> selections;

    foreach (int index, result) {
        unsigned line, column;
        unit->getTokenPosition(index, &line, &column);

        if (column)
            --column;  // adjust the column position.

        const int len = unit->tokenAt(index).f.length;

        QTextCursor cursor(document()->findBlockByNumber(line - 1));
        cursor.setPosition(cursor.position() + column);
        cursor.setPosition(cursor.position() + len, QTextCursor::KeepAnchor);

        QTextEdit::ExtraSelection sel;
        sel.format = m_cppSemanticFormats[CppSemanticOccurrences];
        sel.cursor = cursor;
        selections.append(sel);

    }

    setExtraSelections(CodeSemanticsSelection, selections);
}

void EditorWidget::openEditor( const Find::SearchResultItem &item )
{
    if (item.path.size() > 0) {
        DocumentManager::getInstance()->openDocument( item.path.first(),
                                                      new TextDocumentCursor(item.lineNumber - 1, item.textMarkPos) );
    } else {
        DocumentManager::getInstance()->openDocument(item.text);
    }
}

void EditorWidget::replaceButtonClicked( const QString &text, const QList<Find::SearchResultItem> &items )
{
    const QStringList fileNames = TextEditor::BaseFileFind::replaceAll(text, items);
    if (!fileNames.isEmpty())
        m_modelManager->updateSourceFiles(fileNames);
}

bool EditorWidget::event( QEvent *e )
{
    switch (e->type()) {
    case QEvent::ShortcutOverride:
        if (static_cast<QKeyEvent*>(e)->key() == Qt::Key_Escape && m_currentRenameSelection != NoCurrentRenameSelection) {
            e->accept();
            return true;
        }
        break;
    default:
        break;
    }

    return TextEditorWidget::event(e);
}

void EditorWidget::keyPressEvent( QKeyEvent *e )
{
    if (m_currentRenameSelection == NoCurrentRenameSelection) {
        TextEditor::TextEditorWidget::keyPressEvent(e);
        return;
    }

    QTextCursor cursor = textCursor();
    const QTextCursor::MoveMode moveMode =
            (e->modifiers() & Qt::ShiftModifier) ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor;

    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
        abortRename();
        e->accept();
        return;
    case Qt::Key_Home: {
        // Send home to start of name when within the name and not at the start
        if (cursor.position() > m_currentRenameSelectionBegin.position()
               && cursor.position() <= m_currentRenameSelectionEnd.position()) {
            cursor.setPosition(m_currentRenameSelectionBegin.position(), moveMode);
            setTextCursor(cursor);
            e->accept();
            return;
        }
        break;
    }
    case Qt::Key_End: {
        // Send end to end of name when within the name and not at the end
        if (cursor.position() >= m_currentRenameSelectionBegin.position()
               && cursor.position() < m_currentRenameSelectionEnd.position()) {
            cursor.setPosition(m_currentRenameSelectionEnd.position(), moveMode);
            setTextCursor(cursor);
            e->accept();
            return;
        }
        break;
    }
    case Qt::Key_Backspace: {
        if (cursor.position() == m_currentRenameSelectionBegin.position()
            && !cursor.hasSelection()) {
            // Eat backspace at start of name when there is no selection
            e->accept();
            return;
        }
        break;
    }
    case Qt::Key_Delete: {
        if (cursor.position() == m_currentRenameSelectionEnd.position()
            && !cursor.hasSelection()) {
            // Eat delete at end of name when there is no selection
            e->accept();
            return;
        }
        break;
    }
    default: {
        break;
    }
    } // switch

    startRename();

    bool wantEditBlock = (cursor.position() >= m_currentRenameSelectionBegin.position()
            && cursor.position() <= m_currentRenameSelectionEnd.position());

    if (wantEditBlock) {
        // possible change inside rename selection
        if (m_firstRenameChange)
            cursor.beginEditBlock();
        else
            cursor.joinPreviousEditBlock();
        m_firstRenameChange = false;
    }
    TextEditor::TextEditorWidget::keyPressEvent(e);
    if (wantEditBlock)
        cursor.endEditBlock();
    finishRename();
}

SemanticHighlighter::Source EditorWidget::currentSource( bool force )
{
    unsigned line = 0, column = 0;
    convertPosition(position(), &line, &column);

    const CPlusPlus::Snapshot snapshot = m_modelManager->snapshot();
    const QString fileName = file()->fileName();

    QString code;
    if ( force || m_lastSemanticInfo.revision != document()->revision() )
        code = toPlainText(); // get the source code only when needed.

    const unsigned revision = document()->revision();
    SemanticHighlighter::Source source(snapshot, fileName, code,
                                       line, column, revision);
    source.force = force;
    return source;
}

void EditorWidget::highlightUses( const QList<CppEditor::Internal::SemanticInfo::Use> &uses,
                                  const CppEditor::Internal::SemanticInfo &semanticInfo,
                                  QList<QTextEdit::ExtraSelection> *selections )
{
    bool isUnused = false;

    if (uses.size() == 1)
        isUnused = true;

    foreach (const CppEditor::Internal::SemanticInfo::Use &use, uses) {
        QTextEdit::ExtraSelection sel;

        if (isUnused)
            sel.format = ( use.kind == CppEditor::Internal::SemanticInfo::Use::OnlyOccurance
                           ? m_grammarTermUnusedFormat : m_cppSemanticFormats[CppSemanticOccurrencesUnused] );
        else
            sel.format = m_cppSemanticFormats[CppSemanticOccurrences];

        const int anchor = document()->findBlockByNumber(use.line - 1).position() + use.column - 1;
        const int position = anchor + use.length;

        sel.cursor = QTextCursor(document());
        sel.cursor.setPosition(anchor);
        sel.cursor.setPosition(position, QTextCursor::KeepAnchor);

        if (isUnused) {
            if (semanticInfo.hasQ && sel.cursor.selectedText() == QLatin1String("q"))
                continue; // skip q

            else if (semanticInfo.hasD && sel.cursor.selectedText() == QLatin1String("d"))
                continue; // skip d
        }

        selections->append(sel);
    }
}

bool EditorWidget::checkApplySizeForSemanticHighlighter( int docSize )
{
    const QMap<QString, QVariant> &valuesMap = SettingsManager::instance()->valuesMap(CppEditor::Constants::C_CPP_EDITOR_SETTINGS_ID);
    if ( valuesMap["ApplyToAll"].toBool() )
        return true;

    int nSize = valuesMap["ApplyToLessSize"].toInt() * 1024;
    return (docSize < nSize);
}

void EditorWidget::startRename()
{
    m_inRenameChanged = false;
}

void EditorWidget::finishRename()
{
    if (!m_inRenameChanged)
        return;

    m_inRename = true;

    QTextCursor cursor = textCursor();
    cursor.joinPreviousEditBlock();

    cursor.setPosition(m_currentRenameSelectionEnd.position());
    cursor.setPosition(m_currentRenameSelectionBegin.position(), QTextCursor::KeepAnchor);
    m_renameSelections[m_currentRenameSelection].cursor = cursor;
    QString text = cursor.selectedText();

    for (int i = 0; i < m_renameSelections.size(); ++i) {
        if (i == m_currentRenameSelection)
            continue;
        QTextEdit::ExtraSelection &s = m_renameSelections[i];
        int pos = s.cursor.selectionStart();
        s.cursor.removeSelectedText();
        s.cursor.insertText(text);
        s.cursor.setPosition(pos, QTextCursor::KeepAnchor);
    }

    setExtraSelections(CodeSemanticsSelection, m_renameSelections);
    cursor.endEditBlock();

    m_inRename = false;
}

void EditorWidget::abortRename()
{
    if (m_currentRenameSelection <= NoCurrentRenameSelection)
        return;
    m_renameSelections[m_currentRenameSelection].format = m_cppSemanticFormats[CppSemanticOccurrences];
    m_currentRenameSelection = NoCurrentRenameSelection;
    m_currentRenameSelectionBegin = QTextCursor();
    m_currentRenameSelectionEnd = QTextCursor();
    setExtraSelections(CodeSemanticsSelection, m_renameSelections);
}

Core::ILink *EditorWidget::attemptFuncDeclDef( const QTextCursor &cursor, const CPlusPlus::Document::Ptr &doc, CPlusPlus::Snapshot snapshot ) const
{
    Core::ILink *result = 0;

    QList<CPlusPlus::AST *> path = CPlusPlus::ASTPath(doc)(cursor);

    if (path.size() < 5)
        return result;

    CPlusPlus::NameAST *name = path.last()->asName();
    if (!name)
        return result;

    if (CPlusPlus::QualifiedNameAST *qName = path.at(path.size() - 2)->asQualifiedName()) {
        // TODO: check which part of the qualified name we're on
        if (qName->unqualified_name != name)
            return result;
    }

    for (int i = path.size() - 1; i != -1; --i) {
        CPlusPlus::AST *node = path.at(i);

        if (node->asParameterDeclaration() != 0)
            return result;
    }

    CPlusPlus::AST *declParent = 0;
    CPlusPlus::DeclaratorAST *decl = 0;
    for (int i = path.size() - 2; i > 0; --i) {
        if ((decl = path.at(i)->asDeclarator()) != 0) {
            declParent = path.at(i - 1);
            break;
        }
    }
    if (!decl || !declParent)
        return result;
    if (!decl->postfix_declarator_list || !decl->postfix_declarator_list->value)
        return result;
    CPlusPlus::FunctionDeclaratorAST *funcDecl = decl->postfix_declarator_list->value->asFunctionDeclarator();
    if (!funcDecl)
        return result;

    CPlusPlus::Symbol *target = 0;
    if (CPlusPlus::FunctionDefinitionAST *funDef = declParent->asFunctionDefinition()) {
        QList<CPlusPlus::Declaration *> candidates = CPlusPlus::findMatchingDeclaration( CPlusPlus::LookupContext(doc, snapshot), funDef->symbol );
        if (!candidates.isEmpty()) // TODO: improve disambiguation
            target = candidates.first();
    } else if (declParent->asSimpleDeclaration()) {
        target = snapshot.findMatchingDefinition(funcDecl->symbol);
    }

    if (target) {
        unsigned startLine, startColumn, endLine, endColumn;
        doc->translationUnit()->getTokenStartPosition(name->firstToken(), &startLine, &startColumn);
        doc->translationUnit()->getTokenEndPosition(name->lastToken() - 1, &endLine, &endColumn);

        QTextDocument *textDocument = cursor.document();
        int begin = textDocument->findBlockByNumber(startLine - 1).position() + startColumn - 1;
        int end = textDocument->findBlockByNumber(endLine - 1).position() + endColumn - 1;

        result = linkToSymbol(begin, end, target);
    }

    return result;
}

CPlusPlus::Symbol *EditorWidget::findDefinition( CPlusPlus::Symbol *symbol, const CPlusPlus::Snapshot &snapshot ) const
{
    if ( symbol->isFunction() )
        return 0; // symbol is a function definition.

    else if ( !symbol->type()->isFunctionType() )
        return 0; // not a function declaration

    return snapshot.findMatchingDefinition(symbol);
}

QString EditorWidget::identifierUnderCursor( QTextCursor *macroCursor ) const
{
    macroCursor->movePosition(QTextCursor::StartOfWord);
    macroCursor->movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    return macroCursor->selectedText();
}

Core::ILink *EditorWidget::findMacroLink( int begin, int end, const QByteArray &name ) const
{
    if ( !name.isEmpty() ) {
        if ( CPlusPlus::Document::Ptr doc = m_lastSemanticInfo.doc ) {
            const CPlusPlus::Snapshot snapshot = m_modelManager->snapshot();
            QSet<QString> processed;
            return findMacroLink(begin, end, name, doc, snapshot, &processed);
        }
    }

    return 0;
}

Core::ILink *EditorWidget::findMacroLink (int begin, int end, const QByteArray &name, CPlusPlus::Document::Ptr doc,
                                          const CPlusPlus::Snapshot &snapshot, QSet<QString> *processed ) const
{
    if ( doc && ! name.startsWith('<') && ! processed->contains(doc->fileName()) ) {
        processed->insert(doc->fileName());

        foreach (const CPlusPlus::Macro &macro, doc->definedMacros()) {
            if (macro.name() == name) {
                return new Core::TextTargetLink(begin, end, macro.fileName(), macro.line(), 0);
            }
        }

        const QList<CPlusPlus::Document::Include> includes = doc->includes();
        for (int index = includes.size() - 1; index != -1; --index) {
            const CPlusPlus::Document::Include &i = includes.at(index);
            if ( Core::ILink *link = findMacroLink(begin, end, name, snapshot.document(i.fileName()), snapshot, processed) )
                return link;
        }
    }

    return 0;
}

static inline CPlusPlus::LookupItem skipForwardDeclarations(const QList<CPlusPlus::LookupItem> &resolvedSymbols)
{
    QList<CPlusPlus::LookupItem> candidates = resolvedSymbols;

    CPlusPlus::LookupItem result = candidates.first();
    const CPlusPlus::FullySpecifiedType ty = result.type().simplified();

    if (ty->isForwardClassDeclarationType()) {
        while (! candidates.isEmpty()) {
            CPlusPlus::LookupItem r = candidates.takeFirst();

            if (! r.type()->isForwardClassDeclarationType()) {
                result = r;
                break;
            }
        }
    }

    if (ty->isObjCForwardClassDeclarationType()) {
        while (! candidates.isEmpty()) {
            CPlusPlus::LookupItem r = candidates.takeFirst();

            if (! r.type()->isObjCForwardClassDeclarationType()) {
                result = r;
                break;
            }
        }
    }

    if (ty->isObjCForwardProtocolDeclarationType()) {
        while (! candidates.isEmpty()) {
            CPlusPlus::LookupItem r = candidates.takeFirst();

            if (! r.type()->isObjCForwardProtocolDeclarationType()) {
                result = r;
                break;
            }
        }
    }

    return result;
}

Core::ILink *EditorWidget::findLinkAt( const QTextCursor &cursor, bool resolveTarget )
{
    if (!m_modelManager || !m_lastSemanticInfo.doc)
        return 0;

    CPlusPlus::Snapshot snapshot = m_modelManager->snapshot();
    snapshot.insert(m_lastSemanticInfo.doc);

    if ( Core::ILink *link = attemptFuncDeclDef(cursor, m_lastSemanticInfo.doc, snapshot) )
        return link;

    unsigned lineNumber = 0, positionInBlock = 0;
    convertPosition(cursor.position(), &lineNumber, &positionInBlock);

    CPlusPlus::Document::Ptr doc = m_lastSemanticInfo.doc;

    const unsigned line = lineNumber;
    const unsigned column = positionInBlock + 1;

    QTextCursor tc = cursor;

    // Make sure we're not at the start of a word
    {
        const QChar c = characterAt(tc.position());
        if (c.isLetter() || c == QLatin1Char('_'))
            tc.movePosition(QTextCursor::Right);
    }


    int beginOfToken = 0;
    int endOfToken = 0;

    CPlusPlus::SimpleLexer tokenize;
    tokenize.setQtMocRunEnabled(true);
    const QString blockText = cursor.block().text();
    const QList<CPlusPlus::Token> tokens = tokenize(blockText, CPlusPlus::BackwardsScanner::previousBlockState(cursor.block()));

    bool recognizedQtMethod = false;

    for (int i = 0; i < tokens.size(); ++i) {
        const CPlusPlus::Token &tk = tokens.at(i);

        if (((unsigned) positionInBlock) >= tk.begin() && ((unsigned) positionInBlock) <= tk.end()) {
            if (i >= 2 && tokens.at(i).is(CPlusPlus::T_IDENTIFIER) && tokens.at(i - 1).is(CPlusPlus::T_LPAREN)
                && (tokens.at(i - 2).is(CPlusPlus::T_SIGNAL) || tokens.at(i - 2).is(CPlusPlus::T_SLOT))) {

                // token[i] == T_IDENTIFIER
                // token[i + 1] == T_LPAREN
                // token[.....] == ....
                // token[i + n] == T_RPAREN

                if (i + 1 < tokens.size() && tokens.at(i + 1).is(CPlusPlus::T_LPAREN)) {
                    // skip matched parenthesis
                    int j = i - 1;
                    int depth = 0;

                    for (; j < tokens.size(); ++j) {
                        if (tokens.at(j).is(CPlusPlus::T_LPAREN))
                            ++depth;

                        else if (tokens.at(j).is(CPlusPlus::T_RPAREN)) {
                            if (! --depth)
                                break;
                        }
                    }

                    if (j < tokens.size()) {
                        QTextBlock block = cursor.block();

                        beginOfToken = block.position() + tokens.at(i).begin();
                        endOfToken = block.position() + tokens.at(i).end();

                        tc.setPosition(block.position() + tokens.at(j).end());
                        recognizedQtMethod = true;
                    }
                }
            }
            break;
        }
    }

    if (! recognizedQtMethod) {
        const QTextBlock block = tc.block();
        int pos = cursor.positionInBlock();
        QChar ch = document()->characterAt(cursor.position());
        if (pos > 0 && ! (ch.isLetterOrNumber() || ch == QLatin1Char('_')))
            --pos; // positionInBlock points to a delimiter character.
        const CPlusPlus::Token tk = CPlusPlus::SimpleLexer::tokenAt(block.text(), pos, CPlusPlus::BackwardsScanner::previousBlockState(block), true);

        beginOfToken = block.position() + tk.begin();
        endOfToken = block.position() + tk.end();

        // Handle include directives
        if (tk.is(CPlusPlus::T_STRING_LITERAL) || tk.is(CPlusPlus::T_ANGLE_STRING_LITERAL)) {
            const unsigned lineno = cursor.blockNumber() + 1;
            foreach (const CPlusPlus::Document::Include &incl, doc->includes()) {
                if (incl.line() == lineno && incl.resolved()) {
                    return new Core::TextTargetLink(beginOfToken + 1, endOfToken - 1, incl.fileName(), 0, 0);
                }
            }
        }
        // Handle exec sql c$include directives
        if ( tk.is(CPlusPlus::T_EXEC_SQL_INCLUDE_FILENAME) ) {
            const unsigned lineno = cursor.blockNumber() + 1;
            foreach (const CPlusPlus::Document::Include &incl, doc->includes()) {
                if (incl.line() == lineno && incl.resolved()) {
                    return new Core::TextTargetLink(beginOfToken, endOfToken, incl.fileName(), 0, 0);
                }
            }
        }

        if (tk.isNot(CPlusPlus::T_IDENTIFIER) && tk.kind() < CPlusPlus::T_FIRST_QT_KEYWORD && tk.kind() > CPlusPlus::T_LAST_KEYWORD)
            return 0;

        tc.setPosition(endOfToken);
    }

    // Find the last symbol up to the cursor position
    CPlusPlus::Scope *scope = doc->scopeAt(line, column, document());
    if (!scope)
        return 0;

    if ( CPlusPlus::GrammarScope *grammarScope = scope->asGrammarScope() ) {
        int pos = cursor.positionInBlock();
        int tokIndex = CPlusPlus::SimpleLexer::tokenAt(tokens, pos);
        if ( tokIndex == -1 )
            return 0;
        const CPlusPlus::Token &tk = tokens.at(tokIndex);
        if ( tk.isNot(CPlusPlus::T_IDENTIFIER) )
            return 0;
        const QTextBlock block = tc.block();
        QString grExpression = block.text().mid(tk.offset, tk.length());
        CPlusPlus::Identifier id(grExpression.toUtf8().constData(), grExpression.length());
        if ( tokIndex > 0 && tokens.at(tokIndex - 1).is(CPlusPlus::T_LESS))
            return linkToSymbol(beginOfToken, endOfToken, grammarScope->findSemanticTypeSymbol(&id));
        for (CPlusPlus::Symbol *s = grammarScope->find(&id); s; s = s->next() ) {
            if ( id.isEqualTo(s->name()) ) {
                if ( CPlusPlus::GrammarSymbol *gs = s->asGrammarSymbol() ) {
                    if ( gs->usageType() == CPlusPlus::GrammarSymbol::TokenDeclarationType )
                        return linkToSymbol(beginOfToken, endOfToken, gs);
                    else if ( gs->usageType() == CPlusPlus::GrammarSymbol::RuleDefinitionType ) {
                        return linkToSymbol(beginOfToken, endOfToken, gs);  // TODO: Release Toggle RuleDefinitionType <-> RuleTypeDefinitionType
                    }
                }
            }
        }
        return 0;
    }

    // Evaluate the type of the expression under the cursor
    CPlusPlus::ExpressionUnderCursor expressionUnderCursor;
    QString expression = expressionUnderCursor(tc);

    for (int pos = tc.position();; ++pos) {
        const QChar ch = characterAt(pos);
        if (ch.isSpace())
            continue;
        else {
            if (ch == QLatin1Char('(') && ! expression.isEmpty()) {
                tc.setPosition(pos);
                if (TextEditor::TextBlockUserData::findNextClosingParenthesis(&tc, true)) {
                    expression.append(tc.selectedText());
                }
            }

            break;
        }
    }

    for (int i = 0; i < 2; i++) {
        const QSharedPointer<CPlusPlus::TypeOfExpression> typeOfExpression(new CPlusPlus::TypeOfExpression);
        typeOfExpression->init(doc, snapshot);
        const QList<CPlusPlus::LookupItem> resolvedSymbols = typeOfExpression->reference(expression, scope, CPlusPlus::TypeOfExpression::Preprocess);

        if (!resolvedSymbols.isEmpty()) {
            CPlusPlus::LookupItem result = skipForwardDeclarations(resolvedSymbols);

            foreach (const CPlusPlus::LookupItem &r, resolvedSymbols) {
                if (CPlusPlus::Symbol *d = r.declaration()) {
                    if (d->isDeclaration() || d->isFunction()) {
                        if (file()->fileName() == QString::fromUtf8(d->fileName(), d->fileNameLength())) {
                            if (unsigned(lineNumber) == d->line() && unsigned(positionInBlock) >= d->column()) { // ### TODO: check the end
                                result = r; // take the symbol under cursor.
                                break;
                            }
                        }
                    }
                }
            }

            if (CPlusPlus::Symbol *symbol = result.declaration()) {
                CPlusPlus::Symbol *def = 0;

                // Consider to show a pop-up displaying overrides for the function
                CPlusPlus::Function *function = symbol->type()->asFunctionType();
                CppTools::SymbolFinder finder;
                CPlusPlus::VirtualFunctionHelper helper(*typeOfExpression, scope, doc, snapshot, &finder);

                if (helper.canLookupVirtualFunctionOverrides(function)) {
                    CppEditor::Internal::VirtualFunctionAssistProvider::Parameters params;
                    params.function = function;
                    params.staticClass = helper.staticClassOfFunctionCallExpression();
                    params.typeOfExpression = typeOfExpression;
                    params.snapshot = snapshot;
                    params.cursorPosition = cursor.position();
                    params.openInNextSplit = false;

                    if (m_virtualFunctionAssistProvider->configure(params)) {
                        invokeAssist(TextEditor::FollowSymbol,
                                               m_virtualFunctionAssistProvider);
                        m_virtualFunctionAssistProvider->clearParams();
                    }

                    // Ensure a valid link text, so the symbol name will be underlined on Ctrl+Hover.
                    return new Core::TextTargetLink(beginOfToken, endOfToken, file()->fileName(), 0, 0);
                }

                if (resolveTarget) {
                    CPlusPlus::Symbol *lastVisibleSymbol = doc->lastVisibleSymbolAt(line, column);

                    def = findDefinition(symbol, snapshot);

                    if (def == lastVisibleSymbol)
                        def = 0; // jump to declaration then.

                    if (symbol->isForwardClassDeclaration()) {
                        def = snapshot.findMatchingClassDeclaration(symbol);
                    }
                }

                if ( scope->isExecSqlStatement() && symbol->shadowed() ) {
                    scope = scope->enclosingScope();
                    continue;
                }

                return linkToSymbol(beginOfToken, endOfToken, def ? def : symbol);
            }
            else
                break;
        }
        else
            break;
    }

    // Handle macro uses
    QTextCursor macroCursor = cursor;
    const QByteArray name = identifierUnderCursor(&macroCursor).toLatin1();
    return findMacroLink(macroCursor.selectionStart(), macroCursor.selectionEnd(), name);
}

static QString fetchLine(const QByteArray &bytes, const int line)
{
    int current = 0;
    const char *s = bytes.constData();
    while (*s) {
        if (*s == '\n') {
            ++current;
            if (current == line)
                break;
        }
        ++s;
    }

    if (current == line) {
        ++s;
        const char *e = s;
        while (*e && *e != '\n')
            ++e;
        return QString::fromUtf8(s, e - s);
    }
    return QString();
}


static void addGrammarUsage(CPlusPlus::Symbol *symbol, CPlusPlus::Document::Ptr doc, QList<CPlusPlus::Usage> &usages)
{
    unsigned line = symbol->line();
    unsigned col = symbol->column();

    QString lineText = fetchLine(doc->source(), line);
    if ( col )
        --col;  // adjust the column position.

    int len = symbol->name()->identifier()->size();

    const CPlusPlus::Usage u(doc->fileName(), lineText, line, col, len);
    usages.prepend(u);  /// Why!?!
}

void EditorWidget::showGrammarUsages( const CPlusPlus::Name *searchName, const QList<CPlusPlus::Usage> &usages )
{
    Find::SearchResultWindow * resultWindow = SearchEngine::getInstance()->newSearchResultWindow(searchName->identifier()->chars(), QIcon(":/cppeditor/images/find_usages.png"), 0);
    Find::SearchResult *search = resultWindow->startNewSearch(Find::SearchResultWindow::SearchOnly);
    connect(search, SIGNAL(activated(Find::SearchResultItem)), this, SLOT(openEditor(Find::SearchResultItem)));

    foreach ( const CPlusPlus::Usage &result, usages ) {
        resultWindow->addResult( result.path,
                                 result.line,
                                 result.lineText,
                                 result.col,
                                 result.len );
    }

    resultWindow->finishSearch();
}

void EditorWidget::showGrammarUsagesReaplaceDialog( const CPlusPlus::Name *searchName, const QList<CPlusPlus::Usage> &usages )
{
    CppReplaceUsagesDialog replaceDialog(usages, QString(searchName->identifier()->chars()), Core::Storage::mainWindow()->widget());
    connect(&replaceDialog, SIGNAL(openEditor(Find::SearchResultItem)), this, SLOT(openEditor(Find::SearchResultItem)));
    connect(&replaceDialog, SIGNAL(replaceButtonClicked(QString,QList<Find::SearchResultItem>)),
            this, SLOT(replaceButtonClicked(QString,QList<Find::SearchResultItem>)));
    replaceDialog.exec();
}

void EditorWidget::findGrammarUsages( CPlusPlus::Symbol *symbol, const CppEditor::Internal::SemanticInfo &info, bool replaceMode )
{
    QList<CPlusPlus::Usage> usages;
    const CPlusPlus::Name *searchName = symbol->name();

    if ( CPlusPlus::GrammarScope *grammarScope = info.doc->grammarScope() ) {
        for (CPlusPlus::Symbol *s = grammarScope->find(symbol->identifier()); s; s = s->next())
            if ( s->name()->isEqualTo(searchName) )
                addGrammarUsage(s, info.doc, usages);
    }

    if ( replaceMode )
        showGrammarUsagesReaplaceDialog(searchName, usages);
    else
        showGrammarUsages(searchName, usages);
}

void EditorWidget::findGrammarSemanticTypeUsages( CPlusPlus::Symbol *symbol, const CppEditor::Internal::SemanticInfo &info, bool replaceMode )
{
    QList<CPlusPlus::Usage> usages;
    const CPlusPlus::Name *searchName = symbol->name();

    if ( CPlusPlus::GrammarScope *grammarScope = info.doc->grammarScope() ) {
        if ( CPlusPlus::Scope *typesScope = grammarScope->semanticTypesScope() ) {
            for (CPlusPlus::Symbol *s = typesScope->find(symbol->identifier()); s; s = s->next())
                if ( s->name()->isEqualTo(searchName) )
                    addGrammarUsage(s, info.doc, usages);
        }
    }

    addGrammarUsage(symbol, info.doc, usages);

    if ( replaceMode )
        showGrammarUsagesReaplaceDialog(searchName, usages);
    else
        showGrammarUsages(searchName, usages);
}

}   // namespace Grammar
