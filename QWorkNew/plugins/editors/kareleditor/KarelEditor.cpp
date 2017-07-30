#include <QTextBlock>
#include <QTreeView>
#include <QHeaderView>

#include "KarelEditor.h"
#include "KarelCheckSymbols.h"
#include "KarelCompletionAssist.h"
#include "KarelSimpleLexer.h"
#include "KarelTypeResolver.h"
#include "KarelAST.h"
#include "KarelReplaceUsagesDialog.h"
#include "KarelAutoCompleter.h"
#include "KarelIndenter.h"

#include "texteditorconstants.h"
#include "karel_constants.h"
#include "Core.h"
#include "normalindenter.h"
#include "basetextdocument.h"
#include "convenience.h"
#include "SearchEngine.h"
#include "searchresultwindow.h"

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

namespace Karel {

class FieldDeclarationVisitor: protected ASTVisitor
{
public:
    FieldDeclarationVisitor(TranslationUnit *unit)
        : ASTVisitor(unit)
    { }

    const Type *operator ()(const Identifier *id, unsigned lineNumber, const Scope *&owner) {
        m_id = id;
        m_lineNumber = lineNumber;
        m_type = 0;
        m_owner = 0;
        m_found = false;

        if ( AST *ast = translationUnit()->ast() ) {
            if ( ModuleAST *moduleAst = ast->asModule() ) {
                m_owner = moduleAst->scope;
                accept(moduleAst);
            }
        }

        owner = m_owner;
        return m_type;
    }

protected:
    bool preVisit(AST *) {
        return !m_found || m_type == 0;
    }

    bool visit(RoutineDeclarationAST *ast) {
        if ( ast->implementation ) {
            m_owner = ast->scope;
            accept(ast->implementation);
        }

        return false;
    }

    bool visit(ConstDeclarationAST *) {
        return false;
    }

    bool visit(VarDeclarationAST *) {
        return false;
    }

    bool visit(TypeDeclarationItemAST *ast) {
        if ( ast->typeName && ast->userType ) {
            if ( StructureUserTypeAST *structureType = ast->userType->asStructureUserType() ) {
                m_type = ast->typeName->type;
                accept(structureType);
            }
        }

        return false;
    }

    bool visit(FieldNameAST *ast) {
        const Token &tk = translationUnit()->tokenAt(ast->t_identifier);
        if ( tk.identifier->equalTo(m_id) ) {
            if ( translationUnit()->findLineNumber(tk.offset) == m_lineNumber )
                m_found = true;
        }
        return false;
    }

private:
    bool m_found;
    const Type *m_type;
    const Scope *m_owner;
    const Identifier *m_id;
    unsigned m_lineNumber;
};

class ConstantUsagesVisitor: protected ASTVisitor
{
public:
    ConstantUsagesVisitor(TranslationUnit *unit, QList<const Token *> *tokens)
        : ASTVisitor(unit)
        , m_tokens(tokens)
    { }

    void operator()(const Identifier *id, const Scope *owner) {
        m_id = id;
        m_owner = owner;

        accept(owner->ast());
    }

protected:
    bool visit(ModuleAST *ast) {
        accept(ast->declarations);
        accept(ast->innerRoutines);
        m_fieldDepth = 0;
        accept(ast->statements);
        accept(ast->outerRoutines);
        return false;
    }

    bool visit(RoutineDeclarationAST *ast) {
        if ( ast->scope != m_owner ) {
            if ( ast->scope && ast->scope->strictLookupConstant(m_id)  )
                return false;   // Constant local override
        }
        return true;
    }

    bool visit(DirectRoutineImplemenationAST *ast) {
        accept(ast->declarations);
        m_fieldDepth = 0;
        accept(ast->statements);
        return false;
    }

    bool visit(ConstantNameAST *ast) {
        const Token &tk = translationUnit()->tokenAt(ast->t_identifier);
        if ( tk.identifier->equalTo(m_id) )
            m_tokens->append(&tk);
        return false;
    }

    bool visit(VariableFieldAST *) {
        m_fieldDepth++;
        return true;
    }

    void endVisit(VariableFieldAST *) {
        m_fieldDepth--;
    }

    bool visit(PrimaryIdAST *ast) {
        if ( !m_fieldDepth && ast->variable ) {
            const Token &tk = translationUnit()->tokenAt(ast->variable->t_identifier);
            if ( tk.identifier->equalTo(m_id) )
                m_tokens->append(&tk);
        }
        return false;
    }

private:
    QList<const Token *> *m_tokens;
    const Identifier *m_id;
    const Scope *m_owner;
    int m_fieldDepth;
};

class RoutineUsagesVisitor: protected ASTVisitor
{
public:
    RoutineUsagesVisitor(TranslationUnit *unit, QList<const Token *> *tokens)
        : ASTVisitor(unit)
        , m_tokens(tokens)
    { }

    void operator()(const Identifier *id, const Scope *owner) {
        m_id = id;
        m_owner = owner;

        accept(owner->ast());
    }

protected:
    bool visit(ModuleAST *ast) {
        accept(ast->innerRoutines);
        m_fieldDepth = 0;
        accept(ast->statements);
        accept(ast->outerRoutines);
        return false;
    }

    bool visit(RoutineDeclarationAST *ast) {
        if ( ast->scope != m_owner ) {
            if ( ast->scope && ast->scope->strictLookupRoutine(m_id)  )
                return false;   // Routine local override (paranoya ?!)
        }
        return true;
    }

    bool visit(DirectRoutineImplemenationAST *ast) {
        m_fieldDepth = 0;
        accept(ast->statements);
        accept(ast->routineName);
        return false;
    }

    bool visit(RoutineNameAST *ast) {
        const Token &tk = translationUnit()->tokenAt(ast->t_identifier);
        if ( tk.identifier->equalTo(m_id) )
            m_tokens->append(&tk);
        return false;
    }

    bool visit(VariableFieldAST *) {
        m_fieldDepth++;
        return true;
    }

    void endVisit(VariableFieldAST *) {
        m_fieldDepth--;
    }

    bool visit(PrimaryIdAST *ast) {
        if ( !m_fieldDepth && ast->variable ) {
            const Token &tk = translationUnit()->tokenAt(ast->variable->t_identifier);
            if ( tk.identifier->equalTo(m_id) )
                m_tokens->append(&tk);
        }
        return false;
    }

private:
    QList<const Token *> *m_tokens;
    const Identifier *m_id;
    const Scope *m_owner;
    int m_fieldDepth;
};

class VariableUsagesVisitor: protected ASTVisitor
{
public:
    VariableUsagesVisitor(TranslationUnit *unit, QList<const Token *> *tokens)
        : ASTVisitor(unit)
        , m_tokens(tokens)
    { }

    void operator()(const Identifier *id, const Scope *owner) {
        m_id = id;
        m_owner = owner;

        accept(owner->ast());
    }

protected:
    bool visit(ModuleAST *ast) {
        accept(ast->declarations);
        accept(ast->innerRoutines);
        m_fieldDepth = 0;
        accept(ast->statements);
        accept(ast->outerRoutines);
        return false;
    }

    bool visit(RoutineDeclarationAST *ast) {
        if ( ast->scope != m_owner ) {
            if ( ast->scope && ast->scope->strictLookupVariable(m_id)  )
                return false;   // Variable local override
        }
        return true;
    }

    bool visit(DirectRoutineImplemenationAST *ast) {
        accept(ast->declarations);
        m_fieldDepth = 0;
        accept(ast->statements);
        return false;
    }

    bool visit(VariableNameAST *ast) {
        const Token &tk = translationUnit()->tokenAt(ast->t_identifier);
        if ( tk.identifier->equalTo(m_id) )
            m_tokens->append(&tk);
        return false;
    }
    bool visit(VariableFieldAST *) {
        m_fieldDepth++;
        return true;
    }

    void endVisit(VariableFieldAST *) {
        m_fieldDepth--;
    }

    bool visit(PrimaryIdAST *ast) {
        if ( !m_fieldDepth && ast->variable ) {
            const Token &tk = translationUnit()->tokenAt(ast->variable->t_identifier);
            if ( tk.identifier->equalTo(m_id) )
                m_tokens->append(&tk);
        }
        return false;
    }

private:
    QList<const Token *> *m_tokens;
    const Identifier *m_id;
    const Scope *m_owner;
    int m_fieldDepth;
};

class TypeUsagesVisitor: protected ASTVisitor
{
public:
    TypeUsagesVisitor(TranslationUnit *unit, QList<const Token *> *tokens)
        : ASTVisitor(unit)
        , m_tokens(tokens)
    { }

    void operator()(const Identifier *id, const Type *type, const Scope *owner) {
        m_id = id;
        m_owner = owner;
        m_type = type;

        accept(owner->ast());
    }

protected:
    bool visit(RoutineDeclarationAST *ast) {
        if ( ast->scope != m_owner ) {
            if ( ast->scope && ast->scope->strictLookupNamedTypeDeclaration(m_id) )
                return false;   // User Type local override
        }
        return true;
    }

    bool visit(TypeNameAST *ast) {
        if ( ast->type && ast->type == m_type )
            m_tokens->append(&translationUnit()->tokenAt(ast->t_identifier));
        return false;
    }

private:
    QList<const Token *> *m_tokens;
    const Identifier *m_id;
    const Scope *m_owner;
    const Type *m_type;
};

class FieldUsagesVisitor: protected ASTVisitor
{
public:
    FieldUsagesVisitor(TranslationUnit *unit, QList<const Token *> *tokens)
        : ASTVisitor(unit)
        , m_tokens(tokens)
    {
        _nulltype = control()->builtingType(Type::Null);
    }

    void operator()(const Identifier *id, const Type *type, const Scope *owner) {
        m_id = id;
        m_owner = owner;
        m_type = type;

        accept(owner->ast());
    }

protected:
    bool visit(ModuleAST *ast) {
        accept(ast->innerRoutines);
        _scope = ast->scope;
        accept(ast->statements);
        accept(ast->outerRoutines);
        return false;
    }

    bool visit(RoutineDeclarationAST *ast) {
        if ( ast->scope != m_owner ) {
            if ( ast->scope && ast->scope->strictLookupNamedTypeDeclaration(m_id) )
                return false;   // User Type local override
        }

        if ( ast->implementation ) {
            if ( DirectRoutineImplemenationAST *routineImpl = ast->implementation->asDirectRoutineImplemenation() ) {
                const Scope *oldScope = _scope;
                _scope = ast->scope;
                accept(routineImpl->statements);
                _scope = oldScope;
            }
        }

        return false;
    }

    bool visit(FakeVariableStatementAST *ast) {
        _type = 0;
        accept(ast->var);
        return false;
    }

    bool visit(UsingStatementAST *ast) {
        for (VariableAccessListAST *it = ast->structVars; it; it = it->next) {
            _type = 0;
            accept(it->value);
        }
        const Scope *oldScope = _scope;
        _scope = ast->scope;
        accept(ast->statements);
        _scope = oldScope;
        return false;
    }

    bool visit(PrimaryIdAST *ast) {
        if ( ast->variable ) {
            const Token &tk = tokenAt(ast->variable->t_identifier);

            if ( _type ) {
                if ( const Declaration *decl = _type->fieldDeclaration(tk.identifier) ) {
                    if ( _type && tk.identifier->equalTo(m_id) && _type->binded() == m_type )
                        m_tokens->append(&tk);
                    _type = decl->type();
                } else {
                    _type = 0;
                }
            } else {
                if ( const Declaration *decl = _scope->lookupVariable(tk.identifier) ) {
                    _type = decl->type();
                } else if ( const Type *fieldType = _scope->lookupUsingTypeForField(tk.identifier) ) {
                    _type = fieldType;
                    if ( tk.identifier->equalTo(m_id) && _type->binded() == m_type )
                        m_tokens->append(&tk);
                    _type = fieldType->fieldType(tk.identifier);
                }
            }

            if ( !_type )
                _type = _nulltype;
        }

        return false;
    }

    bool visit(ArrayIdAST *ast) {
        accept(ast->id);
        if ( _type ) {
            _type = _type->arrayItemtype();
            if ( !_type )
                _type = _nulltype;
        }

        const Type *oldType = _type;
        _type = 0;
        accept(ast->vars);
        _type = oldType;

        return false;
    }

private:
    QList<const Token *> *m_tokens;
    const Identifier *m_id;
    const Scope *m_owner;
    const Type *m_type;

    const Scope *_scope;
    const Type *_type;
    const Type *_nulltype;
};

namespace Editor {

enum {
    UPDATE_OUTLINE_INTERVAL = 500,
    UPDATE_USES_INTERVAL = 500
};

class OverviewTreeView : public QTreeView
{
public:
    OverviewTreeView(QWidget *parent = 0)
        : QTreeView(parent)
    {
        // TODO: Disable the root for all items (with a custom delegate?)
        setRootIsDecorated(false);
    }

    void sync()
    {
        expandAll();
        setMinimumWidth(qMax(sizeHintForColumn(0), minimumSizeHint().width()));
    }
};

KarelEditor::KarelEditor(KarelEditorWidget *editor)
    : TextEditor::BaseTextEditor(editor)
{
    m_context.add(Karel::Constants::C_KAREL_EDITOR);
    m_context.add(TextEditor::Constants::C_TEXTEDITOR);
}

Core::Context KarelEditor::context() const
{ return m_context; }

QString KarelEditor::id() const
{
    return QLatin1String(Karel::Constants::KAREL_EDITOR_ID);
}

bool KarelEditor::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
    bool b = TextEditor::BaseTextEditor::open(errorString, fileName, realFileName);
    editorWidget()->setMimeType(Core::Storage::mainWindow()->mimeDatabase()->findByFile(QFileInfo(fileName)).type());
    return b;
}

KarelEditorWidget::KarelEditorWidget(KarelDocument *karelDocument, IDocumentPlugin *editorPlugin, QToolBar *toolBar, QWidget *parent)
    : TextEditor::TextEditorWidget(karelDocument, parent)
    , m_editorPlugin(editorPlugin)
    , m_karelDocument(karelDocument)
    , m_toolBar(toolBar)
    , m_outlineCombo(new QComboBox)
{
    qRegisterMetaType<Karel::SemanticInfo>("Karel::SemanticInfo");

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

//    setIndenter(new TextEditor::NormalIndenter);
    setIndenter(new Indenter);
    setAutoCompleter(new AutoCompleter);

    m_diagnosticMessageFormat.setUnderlineColor(Qt::darkYellow); // ### hardcoded
    m_diagnosticMessageFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline); // ### hardcoded

    m_karelHighlighter = new KarelHighlighter;
    baseTextDocument()->setSyntaxHighlighter(m_karelHighlighter);

    m_highlightRevision = 0;
    m_nextHighlightBlockNumber = 0;
    connect(&m_highlightWatcher, SIGNAL(resultsReadyAt(int,int)), SLOT(highlightSymbolUsages(int,int)));
    connect(&m_highlightWatcher, SIGNAL(finished()), SLOT(finishHighlightSymbolUsages()));
}

KarelEditorWidget::~KarelEditorWidget()
{
    stopSemanticHiglighter();
}

void KarelEditorWidget::stopSemanticHiglighter()
{
    m_semanticHighlighter->abort();
    m_semanticHighlighter->wait();
}

void KarelEditorWidget::onSettingsChanged()
{
    TextEditor::TextEditorWidget::onSettingsChanged();
    m_karelHighlighter->onFormatSettingsChanged();
    onSemanticFormatSettingsChanged();
}

void KarelEditorWidget::onSemanticFormatSettingsChanged()
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("KAREL"));
    int offset = NumKarelFormats;

    for (int i = 0; i < SemanticFormatsCount; i++, offset++)
        formatData->fillTextCharFormat(offset, m_semanticFormats[i]);

    semanticRehighlight();
}

void KarelEditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, Utils::CommentDefinition(false, "--"));
}

unsigned KarelEditorWidget::editorRevision() const
{ return document()->revision(); }

TextEditor::IAssistInterface *KarelEditorWidget::createAssistInterface(TextEditor::AssistKind kind,
                                                                       TextEditor::AssistReason reason) const
{
    if ( kind == TextEditor::Completion ) {
        return new KarelCompletionAssistInterface(document(),
                                                  position(),
                                                  editor()->file(),
                                                  reason,
                                                  m_lastSemanticInfo.doc);
    }

    return TextEditorWidget::createAssistInterface(kind, reason);
}

void KarelEditorWidget::updateOutlineNow()
{
    Document::Ptr doc = m_lastSemanticInfo.doc;

    if ( !doc )
        return;
    m_outlineModel->rebuild(doc, document(), editorRevision());

    OverviewTreeView *treeView = static_cast<OverviewTreeView *>(m_outlineCombo->view());
    treeView->sync();
    updateOutlineIndexNow();
}

void KarelEditorWidget::updateOutlineIndex()
{
    m_updateOutlineIndexTimer->start();
}

void KarelEditorWidget::updateOutlineIndexNow()
{
    if (m_outlineModel->editorRevision() != editorRevision()) {
        m_updateOutlineIndexTimer->start();
        return;
    }

    m_updateOutlineIndexTimer->stop();

    m_outlineModelIndex = QModelIndex(); //invalidate
    QModelIndex comboIndex = outlineModelIndex();

    if (comboIndex.isValid()) {
        bool blocked = m_outlineCombo->blockSignals(true);

        // There is no direct way to select a non-root item
        m_outlineCombo->setRootModelIndex(comboIndex.parent());
        m_outlineCombo->setCurrentIndex(comboIndex.row());
        m_outlineCombo->setRootModelIndex(QModelIndex());

        m_outlineCombo->blockSignals(blocked);
    }
}

QModelIndex KarelEditorWidget::outlineModelIndex()
{
    if (!m_outlineModelIndex.isValid()) {
        m_outlineModelIndex = indexForPosition(position());
        emit outlineModelIndexChanged(m_outlineModelIndex);
    }

    return m_outlineModelIndex;
}

QModelIndex KarelEditorWidget::indexForPosition(int pos, const QModelIndex &rootIndex) const
{
    QModelIndex lastIndex = rootIndex;

    const int rowCount = m_outlineModel->rowCount(rootIndex);
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = m_outlineModel->index(row, 0, rootIndex);
        OverviewTreeItem *item = m_outlineModel->itemFromIndex(index);
        if ( item && item->position() > pos )
            break;
        lastIndex = index;
    }

    if (lastIndex != rootIndex) {
        // recurse
        lastIndex = indexForPosition(pos, lastIndex);
    }

    return lastIndex;
}

void KarelEditorWidget::jumpToOutlineElement(int /*index*/)
{
    QModelIndex index = m_outlineCombo->view()->currentIndex();

    if ( OverviewTreeItem *item = m_outlineModel->itemFromIndex(index) ) {
        QTextCursor tc(document());
        tc.setPosition(item->position());
        setTextCursor(tc);
        setFocus();
    }
}

void KarelEditorWidget::openEditor(const Find::SearchResultItem &item)
{
    if (item.path.size() > 0) {
        DocumentManager::getInstance()->openDocument( item.path.first(),
                                                      new TextDocumentCursor(item.lineNumber - 1, item.textMarkPos) );
    } else {
        DocumentManager::getInstance()->openDocument(item.text);
    }
}

void KarelEditorWidget::updateSemanticInfo(const SemanticInfo &semanticInfo)
{
    if ( semanticInfo.revision != editorRevision() ) {
        semanticRehighlight();
        return;
    }

    const SemanticInfo previousSemanticInfo = m_lastSemanticInfo;
    m_lastSemanticInfo = semanticInfo; // update the semantic info
//    m_highlightRevision = semanticInfo.revision;

    if ( previousSemanticInfo.revision != semanticInfo.revision ) {
        setExtraSelections( UndefinedSymbolSelection, createSelections( document(),
                                                                        semanticInfo.diagnosticMessages,
                                                                        m_diagnosticMessageFormat ) );
        m_highlighter.cancel();

        updateSemanticDiagnosticMessages(semanticInfo.doc);
        if ( semanticInfo.doc ) {
            CheckSymbols::Future f = CheckSymbols::go(semanticInfo.doc);
            m_highlighter = f;
            m_highlightRevision = semanticInfo.revision;
            m_nextHighlightBlockNumber = 0;
            m_highlightWatcher.setFuture(m_highlighter);
        }
    }

    m_updateOutlineTimer->start();
}

void KarelEditorWidget::highlightSymbolUsages(int from, int to)
{
    if (editorRevision() != m_highlightRevision)
        return; // outdated

    else if (m_highlighter.isCanceled())
        return; // aborted

    QTextDocument *doc = document();

    if (m_nextHighlightBlockNumber >= doc->blockCount())
        return;

    QMap<int, QVector<SemanticInfo::Use> > chunks = CheckSymbols::chunks(m_highlighter, from, to);
    if (chunks.isEmpty())
        return;

    QTextBlock b = doc->findBlockByNumber(m_nextHighlightBlockNumber);

    QMapIterator<int, QVector<SemanticInfo::Use> > it(chunks);
    while (b.isValid() && it.hasNext()) {
        it.next();
        const int blockNumber = it.key();
        Q_ASSERT(blockNumber < doc->blockCount());

        while (m_nextHighlightBlockNumber < blockNumber) {
            m_karelHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
            b = b.next();
            ++m_nextHighlightBlockNumber;
        }

        QList<QTextLayout::FormatRange> formats;
        foreach (const SemanticInfo::Use &use, it.value()) {
            QTextLayout::FormatRange formatRange;

            switch (use.kind) {
                case SemanticInfo::Use::Constant:
                    formatRange.format = m_semanticFormats[SemanticConstant];
                    break;

                case SemanticInfo::Use::UserType:
                    formatRange.format = m_semanticFormats[SemanticUserType];
                    break;

                case SemanticInfo::Use::Variable:
                    formatRange.format = m_semanticFormats[SemanticVariable];
                    break;

                case SemanticInfo::Use::TypeField:
                    formatRange.format = m_semanticFormats[SemanticField];
                    break;

                case SemanticInfo::Use::ProgramName:
                    formatRange.format = m_semanticFormats[SemanticProgramName];
                    break;

                case SemanticInfo::Use::Routine:
                    formatRange.format = m_semanticFormats[SemanticRoutine];
                    break;

                default:
                    continue;
            }

            formatRange.start = use.column - 1;
            if ( !blockNumber )
                formatRange.start++;
            formatRange.length = use.length;
            formats.append(formatRange);
        }
        m_karelHighlighter->setExtraAdditionalFormats(b, formats);
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}

void KarelEditorWidget::finishHighlightSymbolUsages()
{
    if (editorRevision() != m_highlightRevision)
        return; // outdated

    else if (m_highlighter.isCanceled())
        return; // aborted

    QTextDocument *doc = document();

    if (m_nextHighlightBlockNumber >= doc->blockCount())
        return;

    QTextBlock b = doc->findBlockByNumber(m_nextHighlightBlockNumber);

    while (b.isValid()) {
        m_karelHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}

void KarelEditorWidget::updateUses()
{
    if ( editorRevision() != m_highlightRevision )
        m_highlighter.cancel();

    m_updateUsesTimer->start();
}

void KarelEditorWidget::updateUsesNow()
{
    semanticRehighlight();
}

void KarelEditorWidget::jumpToDefinition()
{
    if ( Core::ILink *link = findLinkAt(textCursor()) ) {
        openLink(link);
        delete link;
    }
}

void KarelEditorWidget::findUsages()
{ findOrReplaceUsages(false); }

void KarelEditorWidget::replaceUsages()
{ findOrReplaceUsages(true); }

TextEditor::BaseTextEditor *KarelEditorWidget::createEditor()
{
    KarelEditor *editable =  new KarelEditor(this);
    createToolBar(editable);

    m_updateUsesTimer = new QTimer(this);
    m_updateUsesTimer->setSingleShot(true);
    m_updateUsesTimer->setInterval(500);
    connect(m_updateUsesTimer, SIGNAL(timeout()), this, SLOT(updateUsesNow()));

    connect(this, SIGNAL(textChanged()), this, SLOT(updateUses()));
    connect( m_semanticHighlighter, SIGNAL(changed(Karel::SemanticInfo)), this, SLOT(updateSemanticInfo(Karel::SemanticInfo)));

    return editable;
}

void KarelEditorWidget::createToolBar(KarelEditor *editable)
{
    Q_UNUSED(editable)

    m_outlineCombo->setMinimumContentsLength(22);

    // Make the combo box prefer to expand
    QSizePolicy policy = m_outlineCombo->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    m_outlineCombo->setSizePolicy(policy);

    QTreeView *outlineView = new OverviewTreeView;
    outlineView->header()->hide();
    outlineView->setItemsExpandable(false);
    m_outlineCombo->setView(outlineView);
    m_outlineCombo->setMaxVisibleItems(20);

    m_outlineModel = new OverviewModel();
    m_outlineCombo->setModel(m_outlineModel);

    m_updateOutlineTimer = new QTimer(this);
    m_updateOutlineTimer->setSingleShot(true);
    m_updateOutlineTimer->setInterval(UPDATE_OUTLINE_INTERVAL);
    connect(m_updateOutlineTimer, SIGNAL(timeout()), this, SLOT(updateOutlineNow()));

    m_updateOutlineIndexTimer = new QTimer(this);
    m_updateOutlineIndexTimer->setSingleShot(true);
    m_updateOutlineIndexTimer->setInterval(UPDATE_OUTLINE_INTERVAL);
    connect(m_updateOutlineIndexTimer, SIGNAL(timeout()), this, SLOT(updateOutlineIndexNow()));

    connect(m_outlineCombo, SIGNAL(activated(int)), this, SLOT(jumpToOutlineElement(int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateOutlineIndex()));

    m_toolBar->addWidget(m_outlineCombo);
    m_toolBar->addActions(m_editorPlugin->toolbarActions(QLatin1String("KAREL_AG_USAGES_SUPPORT")));
}

void KarelEditorWidget::semanticRehighlight()
{
    m_semanticHighlighter->rehighlight(currentSource());
}

SemanticHighlighter::Source KarelEditorWidget::currentSource(bool force)
{
    unsigned line = 0, column = 0;
    convertPosition(position(), &line, &column);

    QString code;
    if ( force || m_lastSemanticInfo.revision != editorRevision() )
        code = toPlainText().replace("\r\n", "\n"); // get the source code only when needed.

    const unsigned revision = editorRevision();
    SemanticHighlighter::Source source(code, line, column, revision);
    source.force = force;
    return source;
}

void KarelEditorWidget::updateSemanticDiagnosticMessages(Document::Ptr doc)
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

Core::ILink *KarelEditorWidget::findLinkAt(const QTextCursor &tc, bool)
{
    Document::Ptr doc = m_lastSemanticInfo.doc;

    bool isStatementsArea;
    Token tk;
    Scope *scope = scopeUnderIdentifier(doc, tc, isStatementsArea, tk);
    if ( !scope )
        return 0;

    QByteArray ba = tc.block().text().midRef(tk.begin(), tk.length()).toLatin1();
    Identifier id(ba.constData(), ba.length());
    int beginOfToken = tc.block().position() + tk.begin();
    int endOfToken = tc.block().position() + tk.end();

    if ( !isStatementsArea ) {
        if ( const Symbol *symbol = scope->lookupSymbolInDeclarationsArea(&id) )
            return linkToSymbol(symbol, doc->translationUnit(), beginOfToken, endOfToken);
    } else {
        int checkPos = tc.block().position() + tk.begin() - 1;
        while ( tc.document()->characterAt(checkPos).isSpace() )
            checkPos--;

        if ( tc.document()->characterAt(checkPos) == QLatin1Char('.') ) {
            QTextBlock block = tc.block();
            TypeResolver resolver(tc.document(), scope);
            if ( const Type *type = resolver(tc.block().text().left(checkPos - tc.block().position()), block) ) {
                if ( const Declaration *field = type->fieldDeclaration(&id) )
                    return linkToSymbol(field, doc->translationUnit(), beginOfToken, endOfToken);
            }
        } else if ( const Type *type = scope->lookupUsingTypeForField(&id) ) {
            if ( const Declaration *field = type->fieldDeclaration(&id) )
                return linkToSymbol(field, doc->translationUnit(), beginOfToken, endOfToken);
        } else {
            if ( const Symbol *symbol = scope->lookupSymbolInStatementsArea(&id) )
                return linkToSymbol(symbol, doc->translationUnit(), beginOfToken, endOfToken);
        }
    }

    return 0;
}

Scope *KarelEditorWidget::scopeUnderIdentifier(Document::Ptr doc, const QTextCursor &tc, bool &isStatementsArea, Token &tk)
{
    if ( !doc )
        return 0;

    SimpleLexer tokenize;
    const QList<Token> &tokens = tokenize(tc.block().text(), true);
    const int tokenIdx = SimpleLexer::tokenBefore(tokens, qMax(0, tc.positionInBlock() - 1));
    tk = (tokenIdx == -1) ? Token() : tokens.at(tokenIdx);
    if ( !tk.isIdentifier() )
        return 0;

    Scope *globalScope = doc->globalScope();
    if ( !globalScope )
        return 0;

    unsigned line = 0, column = 0;
    TextEditor::Convenience::convertPosition(tc.document(), tc.position(), &line, &column);
    if ( line )
        line--;

    return globalScope->scopeAt(doc->translationUnit(), isStatementsArea, line, column);
}

Core::ILink *KarelEditorWidget::linkToSymbol(const Symbol *symbol, TranslationUnit *unit, int beginOfToken, int endOfToken)
{
    unsigned line = 0, column = 0;
    const Token &tkTarget = unit->tokenAt(symbol->tokenIndex());
    TextEditor::Convenience::convertPosition(document(), tkTarget.begin(), &line, &column);
    if ( line )
        line--;
    return new Core::TextTargetLink(beginOfToken, endOfToken, "", line, column);
}

void KarelEditorWidget::findOrReplaceUsages(bool isReplace)
{
    if ( m_karelDocument->isNew() ) {
        Core::Storage::warning(tr("Document should be saved..."));
        return;
    }

    Document::Ptr doc = m_lastSemanticInfo.doc;

    bool isStatementsArea;
    Token tk;
    const QTextCursor tc = textCursor();
    Scope *scope = scopeUnderIdentifier(doc, tc, isStatementsArea, tk);
    if ( !scope )
        return;

    QByteArray ba = tc.block().text().midRef(tk.begin(), tk.length()).toLatin1();
    Identifier id(ba.constData(), ba.length());
    QList<const Token *> results;

    const Scope *owner = 0;

    if ( isStatementsArea ) {
        int checkPos = tc.block().position() + tk.begin() - 1;
        while ( tc.document()->characterAt(checkPos).isSpace() )
            checkPos--;

        if ( tc.document()->characterAt(checkPos) == QLatin1Char('.') ) {
            QTextBlock block = tc.block();
            TypeResolver resolver(tc.document(), scope);
            if ( const Type *fieldType = resolver(tc.block().text().left(checkPos - tc.block().position()), block) ) {
                if ( const Declaration *field = fieldType->fieldDeclaration(&id) ) {
                    results.append(&doc->translationUnit()->tokenAt(field->tokenIndex()));
                    FieldUsagesVisitor visitor(doc->translationUnit(), &results);
                    visitor(&id, fieldType->binded(), doc->globalScope()); // TODO: find owner scope
                    isReplace ? showReplaceUsagesDialog(&id, results) : showUsages(&id, results);
                }
            }
            return;
        } else if ( const Type *fieldType = scope->lookupUsingTypeForField(&id) ) {
            if ( const Declaration *field = fieldType->fieldDeclaration(&id) ) {
                results.append(&doc->translationUnit()->tokenAt(field->tokenIndex()));
                FieldUsagesVisitor visitor(doc->translationUnit(), &results);
                visitor(&id, fieldType->binded(), doc->globalScope()); // TODO: find owner scope
                isReplace ? showReplaceUsagesDialog(&id, results) : showUsages(&id, results);
            }
            return;
        }
    }

    if ( scope->lookupConstant(&id, owner) ) {
        ConstantUsagesVisitor visitor(doc->translationUnit(), &results);
        visitor(&id, owner);
    }

    else if ( scope->lookupVariable(&id, owner) ) {
        VariableUsagesVisitor visitor(doc->translationUnit(), &results);
        visitor(&id, owner);
    }

    else if ( const Declaration *typeDecl = scope->lookupNamedTypeDeclaration(&id, owner) ) {
        TypeUsagesVisitor visitor(doc->translationUnit(), &results);
        visitor(&id, typeDecl->type(), owner);
    }

    else if ( scope->lookupRoutine(&id, owner) ) {
        RoutineUsagesVisitor visitor(doc->translationUnit(), &results);
        visitor(&id, owner);
    }

    else if ( !isStatementsArea ) {
        FieldDeclarationVisitor fieldDeclaration(doc->translationUnit());
        if ( const Type *fieldType = fieldDeclaration(&id, tc.blockNumber(), owner) ) {
            if ( const Declaration *field = fieldType->fieldDeclaration(&id) ) {
                results.append(&doc->translationUnit()->tokenAt(field->tokenIndex()));
                FieldUsagesVisitor visitor(doc->translationUnit(), &results);
                visitor(&id, fieldType, owner);
            }
        }
    }

    isReplace ? showReplaceUsagesDialog(&id, results) : showUsages(&id, results);
}

void KarelEditorWidget::showUsages(const Identifier *id, const QList<const Token *> tokens)
{
    Find::SearchResultWindow *_resultWindow = SearchEngine::getInstance()->newSearchResultWindow(id->charsOrig(), QIcon(":/kareleditor/images/find_usages.png"), 0);
    Find::SearchResult *search = _resultWindow->startNewSearch(Find::SearchResultWindow::SearchOnly);

    connect(search, SIGNAL(activated(Find::SearchResultItem)),
            this, SLOT(openEditor(Find::SearchResultItem)));

    _resultWindow->popup(true);
    QString filePath = editor()->file()->fileName();

    foreach (const Token *tk, tokens) {
        QTextCursor tc(document());
        tc.setPosition(tk->begin());
        _resultWindow->addResult(filePath,
                                 tc.blockNumber() + 1,
                                 tc.block().text(),
                                 tk->begin() - tc.block().position(),
                                 tk->length());
    }

    _resultWindow->finishSearch();
}

void KarelEditorWidget::showReplaceUsagesDialog(const Identifier *id, const QList<const Token *> tokens)
{
    if ( tokens.isEmpty() ) {
        showUsages(id, tokens);
        return;
    }

    KarelReplaceUsagesDialog replaceDialog(document(), editor()->file()->fileName(),
                                           QString(id->charsOrig()), tokens,
                                           Core::Storage::mainWindow()->widget());

    replaceDialog.exec();
}

}   // namespace Editor
}   // namespace Karel

