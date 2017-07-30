/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "cpphighlighter.h"
#include "cppeditor.h"
#include "cppeditorconstants.h"
#include "cpphighlighter.h"
#include "cppchecksymbols.h"
#include "cppquickfix.h"
#include "cpplocalsymbols.h"
#include "cppquickfixassistant.h"
#include "cppqtstyleindenter.h"
#include "cppautocompleter.h"
#include "cppcompletionassist.h"
#include "projectmanager.h"

#include <AST.h>
#include <Control.h>
#include <Token.h>
#include <Scope.h>
#include <Symbols.h>
#include <Names.h>
#include <CoreTypes.h>
#include <Literals.h>
#include <ASTVisitor.h>
#include <SymbolVisitor.h>
#include <TranslationUnit.h>
#include <cplusplus/ASTPath.h>
#include <cplusplus/ModelManagerInterface.h>
#include <cplusplus/ExpressionUnderCursor.h>
#include <cplusplus/TypeOfExpression.h>
#include <cplusplus/Overview.h>
#include <cplusplus/OverviewModel.h>
#include <cplusplus/SimpleLexer.h>
#include <cplusplus/MatchingText.h>
#include <cplusplus/BackwardsScanner.h>
#include <cplusplus/FastPreprocessor.h>

#include <cpptools/cpptoolsconstants.h>
#include <cpptools/cppcodeformatter.h>

#include <ieditor.h>
#include <utils/uncommentselection.h>
#include <basetextdocument.h>
#include <basetextdocumentlayout.h>
#include <fontsettings.h>
#include <tabsettings.h>
#include <texteditorconstants.h>

#include "iassistproposal.h"
#include "basicproposalitemlistmodel.h"
#include "basicproposalitem.h"
#include "symbolfinder.h"
#include "VirtualFunctionHelper.h"

#include "Core.h"

#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtCore/QStack>
#include <QtCore/QSettings>
#include <QtCore/QSignalMapper>
#include <QAction>
#include <QApplication>
#include <QHeaderView>
#include <QLayout>
#include <QMenu>
#include <QShortcut>
#include <QTextEdit>
#include <QComboBox>
#include <QToolBar>
#include <QTreeView>
#include <QSortFilterProxyModel>

#include <QMessageBox>
#include <QToolButton>
#include <QSignalMapper>

#include <sstream>

enum {
    UPDATE_OUTLINE_INTERVAL = 500,
    UPDATE_USES_INTERVAL = 500
};

using namespace CPlusPlus;
using namespace CppEditor::Internal;

namespace {
bool semanticHighlighterDisabled = qstrcmp(qVersion(), "4.7.0") == 0;
}

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

class OverviewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    OverviewProxyModel(CPlusPlus::OverviewModel *sourceModel, QObject *parent) :
        QSortFilterProxyModel(parent),
        m_sourceModel(sourceModel)
    {
        setSourceModel(m_sourceModel);
    }

    bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
    {
        // ignore generated symbols, e.g. by macro expansion (Q_OBJECT)
        const QModelIndex sourceIndex = m_sourceModel->index(sourceRow, 0, sourceParent);
        CPlusPlus::Symbol *symbol = m_sourceModel->symbolFromIndex(sourceIndex);
        if (symbol && symbol->isGenerated())
            return false;

        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }
private:
    CPlusPlus::OverviewModel *m_sourceModel;
};

class FunctionDefinitionUnderCursor: protected ASTVisitor
{
    unsigned _line;
    unsigned _column;
    DeclarationAST *_functionDefinition;

public:
    FunctionDefinitionUnderCursor(TranslationUnit *translationUnit)
        : ASTVisitor(translationUnit),
          _line(0), _column(0)
    { }

    DeclarationAST *operator()(AST *ast, unsigned line, unsigned column)
    {
        _functionDefinition = 0;
        _line = line;
        _column = column;
        accept(ast);
        return _functionDefinition;
    }

protected:
    virtual bool preVisit(AST *ast)
    {
        if (_functionDefinition)
            return false;

        else if (FunctionDefinitionAST *def = ast->asFunctionDefinition()) {
            return checkDeclaration(def);
        }

        else if (ObjCMethodDeclarationAST *method = ast->asObjCMethodDeclaration()) {
            if (method->function_body)
                return checkDeclaration(method);
        }

        return true;
    }

private:
    bool checkDeclaration(DeclarationAST *ast)
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

class FindFunctionDefinitions: protected SymbolVisitor
{
    const Name *_declarationName;
    QList<Function *> *_functions;

public:
    FindFunctionDefinitions()
        : _declarationName(0),
          _functions(0)
    { }

    void operator()(const Name *declarationName, Scope *globals,
                    QList<Function *> *functions)
    {
        _declarationName = declarationName;
        _functions = functions;

        for (unsigned i = 0; i < globals->memberCount(); ++i) {
            accept(globals->memberAt(i));
        }
    }

protected:
    using SymbolVisitor::visit;

    virtual bool visit(Function *function)
    {
        const Name *name = function->name();
        if (const QualifiedNameId *q = name->asQualifiedNameId())
            name = q->name();

        if (_declarationName->isEqualTo(name))
            _functions->append(function);

        return false;
    }
};


struct CanonicalSymbol
{
    CPPEditorWidget *editor;
    TypeOfExpression typeOfExpression;
    SemanticInfo info;

    CanonicalSymbol(CPPEditorWidget *editor, const SemanticInfo &info)
        : editor(editor), info(info)
    {
        typeOfExpression.init(info.doc, info.snapshot);
    }

    const LookupContext &context() const
    {
        return typeOfExpression.context();
    }

    static inline bool isIdentifierChar(const QChar &ch)
    {
        return ch.isLetterOrNumber() || ch == QLatin1Char('_');
    }

    Scope *getScopeAndExpression(const QTextCursor &cursor, QString *code)
    {
        return getScopeAndExpression(editor, info, cursor, code);
    }

    static Scope *getScopeAndExpression(CPPEditorWidget *editor, const SemanticInfo &info,
                                        const QTextCursor &cursor,
                                        QString *code)
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

        ExpressionUnderCursor expressionUnderCursor;
        *code = expressionUnderCursor(tc);
        return info.doc->scopeAt(line, col);
    }

    Symbol *operator()(const QTextCursor &cursor)
    {
        QString code;

        if (Scope *scope = getScopeAndExpression(cursor, &code))
            return operator()(scope, code);

        return 0;
    }

    Symbol *operator()(Scope *scope, const QString &code)
    {
        return canonicalSymbol(scope, code, typeOfExpression);
    }

    static Symbol *canonicalSymbol(Scope *scope, const QString &code, TypeOfExpression &typeOfExpression)
    {
        const QList<LookupItem> results = typeOfExpression(code, scope, TypeOfExpression::Preprocess);

        for (int i = results.size() - 1; i != -1; --i) {
            const LookupItem &r = results.at(i);
            Symbol *decl = r.declaration();

            if (! (decl && decl->enclosingScope()))
                break;

            if (Class *classScope = r.declaration()->enclosingScope()->asClass()) {
                const Identifier *declId = decl->identifier();
                const Identifier *classId = classScope->identifier();

                if (classId && classId->isEqualTo(declId))
                    continue; // skip it, it's a ctor or a dtor.

                else if (Function *funTy = r.declaration()->type()->asFunctionType()) {
                    if (funTy->isVirtual())
                        return r.declaration();
                }
            }
        }

        for (int i = 0; i < results.size(); ++i) {
            const LookupItem &r = results.at(i);

            if (r.declaration())
                return r.declaration();
        }

        return 0;
    }

};


//int numberOfClosedEditors = 0;

} // end of anonymous namespace

CPPEditor::CPPEditor(CPPEditorWidget *editor)
    : BaseTextEditor(editor)
{
    m_context.add(CppEditor::Constants::C_CPPEDITOR);
//    m_context.add(ProjectExplorer::Constants::LANG_CXX);
    m_context.add(TextEditor::Constants::C_TEXTEDITOR);
}

CPPEditorWidget::CPPEditorWidget(IDocument *document, IDocumentPlugin *editorPlugin, QToolBar *toolBar, QWidget *parent)
    : TextEditor::TextEditorWidget(document, parent)
    , m_toolBar(toolBar)
    , m_outlineCombo(new QComboBox)
    , m_currentRenameSelection(NoCurrentRenameSelection)
    , m_inRename(false)
    , m_inRenameChanged(false)
    , m_firstRenameChange(false)
    , m_objcEnabled(false)
    , m_editorPlugin(editorPlugin)
    , m_mapper(0)
    , m_virtualFunctionAssistProvider(new CppEditor::Internal::VirtualFunctionAssistProvider)
{
    m_initialized = false;
    qRegisterMetaType<CppEditor::Internal::SemanticInfo>("CppEditor::Internal::SemanticInfo");

    m_semanticHighlighter = new SemanticHighlighter(this);
    m_semanticHighlighter->start();

    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);
    setIndenter(new CppTools::CppQtStyleIndenter);
    setAutoCompleter(new CppAutoCompleter);

    m_diagnosticMessageFormat.setUnderlineColor(Qt::darkYellow); // ### hardcoded
    m_diagnosticMessageFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline); // ### hardcoded

//    QFileInfo fileInfo(fileName);
//    QMessageBox::information(NULL, tr("Trace 1"), fileInfo.filePath());
    m_cppHighlighter = new CppHighlighter;
//    if ( !fileInfo.suffix().compare(QLatin1String("sc"), Qt::CaseInsensitive) ) {
//        highlighter->setExtraExecSqlEnabled(true);
//        highlighter->setSingleLineCommentEnabled(false);
//    }
    baseTextDocument()->setSyntaxHighlighter(m_cppHighlighter);

    m_modelManager = CppModelManagerInterface::instance();

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

//    m_typeFormat.setForeground(QBrush(QColor(0, 0, 255)));
}

CPPEditorWidget::~CPPEditorWidget()
{
//    if (Core::EditorManager *em = Core::EditorManager::instance())
//        em->hideEditorInfoBar(QLatin1String("CppEditor.Rename"));

    m_semanticHighlighter->abort();
    m_semanticHighlighter->wait();

    if ( m_mapper )
        delete m_mapper;

    delete m_virtualFunctionAssistProvider;

//    ++numberOfClosedEditors;
//    if (numberOfClosedEditors == 5) {
//        m_modelManager->GC();
//        numberOfClosedEditors = 0;
//    }
}

TextEditor::BaseTextEditor *CPPEditorWidget::createEditor()
{
    CPPEditor *editable = new CPPEditor(this);
    createToolBar(editable);
    return editable;
}

void CPPEditorWidget::createToolBar(CPPEditor * /*editor*/)
{
//    m_outlineCombo = new QComboBox;
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

    m_outlineModel = new OverviewModel(this);
    m_proxyModel = new OverviewProxyModel(m_outlineModel, this);
//    if (CppPlugin::instance()->sortedOutline())
//        m_proxyModel->sort(0, Qt::AscendingOrder);
//    else
        m_proxyModel->sort(-1, Qt::AscendingOrder); // don't sort yet, but set column for sortedOutline()
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_outlineCombo->setModel(m_proxyModel);

    m_outlineCombo->setContextMenuPolicy(Qt::ActionsContextMenu);

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
    connect(m_outlineCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOutlineToolTip()));
    connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(onContentsChanged(int,int,int)));

    connect(file(), SIGNAL(changed()), this, SLOT(updateFileName()));


    // set up the semantic highlighter
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateUses()));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateUses()));

    connect(m_semanticHighlighter, SIGNAL(changed(CppEditor::Internal::SemanticInfo)),
            this, SLOT(updateSemanticInfo(CppEditor::Internal::SemanticInfo)));

    m_toolBar->addWidget(m_outlineCombo);
    QList<QAction *> actionsSortOutline = m_editorPlugin->toolbarActions("CPP_AG_TOGGLE_SORT_OUTLINE");
    connect(actionsSortOutline[0], SIGNAL(triggered(bool)), this, SLOT(onOutlineSortChanged(bool)));
    m_toolBar->addActions(actionsSortOutline);
    m_toolBar->addSeparator();
    m_outlineCombo->addAction(actionsSortOutline[0]);
    QList<QAction *>actionsUsagesSupport = m_editorPlugin->toolbarActions("CPP_AG_USAGES_SUPPORT");
    m_toolBar->addActions(actionsUsagesSupport);

//    editor->insertExtraToolBarWidget(TextEditor::BaseTextEditor::Left, m_outlineCombo);
}

void CPPEditorWidget::paste()
{
    if (m_currentRenameSelection == NoCurrentRenameSelection) {
        TextEditorWidget::paste();
        return;
    }

    startRename();
    TextEditorWidget::paste();
    finishRename();
}

void CPPEditorWidget::cut()
{
    if (m_currentRenameSelection == NoCurrentRenameSelection) {
        TextEditorWidget::cut();
        return;
    }

    startRename();
    TextEditorWidget::cut();
    finishRename();
}

CppModelManagerInterface *CPPEditorWidget::modelManager() const
{
    return m_modelManager;
}

void CPPEditorWidget::setMimeType(const QString &mt)
{
    TextEditorWidget::setMimeType(mt);
    setObjCEnabled(mt == CppTools::Constants::OBJECTIVE_CPP_SOURCE_MIMETYPE);

    if ( mt == CppTools::Constants::SC_SOURCE_MIMETYPE ) {
        m_cppHighlighter->setExtraExecSqlEnabled(true);
        m_cppHighlighter->setSingleLineCommentEnabled(false);
    }
    else {
        m_cppHighlighter->setExtraExecSqlEnabled(false);
        m_cppHighlighter->setSingleLineCommentEnabled(true);
    }
    m_cppHighlighter->rehighlight();
}

void CPPEditorWidget::setObjCEnabled(bool onoff)
{
    m_objcEnabled = onoff;
}

bool CPPEditorWidget::isObjCEnabled() const
{ return m_objcEnabled; }

void CPPEditorWidget::startRename()
{
    m_inRenameChanged = false;
}

void CPPEditorWidget::finishRename()
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

void CPPEditorWidget::abortRename()
{
    if (m_currentRenameSelection <= NoCurrentRenameSelection)
        return;
    m_renameSelections[m_currentRenameSelection].format = m_cppSemanticFormats[CppSemanticOccurrences];
    m_currentRenameSelection = NoCurrentRenameSelection;
    m_currentRenameSelectionBegin = QTextCursor();
    m_currentRenameSelectionEnd = QTextCursor();
    setExtraSelections(CodeSemanticsSelection, m_renameSelections);
}

void CPPEditorWidget::rehighlight(bool force)
{
    const SemanticHighlighter::Source source = currentSource(force);
    m_semanticHighlighter->rehighlight(source);
}

void CPPEditorWidget::onDocumentUpdated(Document::Ptr doc)
{
    if (doc->fileName() != file()->fileName())
        return;

    if (doc->editorRevision() != editorRevision())
        return;

    if (! m_initialized) {
        m_initialized = true;
        rehighlight(/* force = */ true);
    }

    m_updateOutlineTimer->start();
}

const Macro *CPPEditorWidget::findCanonicalMacro(const QTextCursor &cursor, Document::Ptr doc) const
{
    if (! doc)
        return 0;

    unsigned line, col;
    convertPosition(cursor.position(), &line, &col);

    if (const Macro *macro = doc->findMacroDefinitionAt(line))
        return macro;

    if (const Document::MacroUse *use = doc->findMacroUseAt(cursor.position()))
        return &use->macro();

    return 0;
}

void CPPEditorWidget::findUsages()
{
    SemanticInfo info = m_lastSemanticInfo;
    info.snapshot = CppModelManagerInterface::instance()->snapshot();
    info.snapshot.insert(info.doc);

    CanonicalSymbol cs(this, info);
    Symbol *canonicalSymbol = cs(textCursor());
    if (canonicalSymbol) {
        m_modelManager->findUsages(canonicalSymbol, cs.context());
    } else if (const Macro *macro = findCanonicalMacro(textCursor(), info.doc)) {
        m_modelManager->findMacroUsages(*macro);
    }
}


void CPPEditorWidget::renameUsagesNow(const QString &replacement)
{
    SemanticInfo info = m_lastSemanticInfo;
    info.snapshot = CppModelManagerInterface::instance()->snapshot();
    info.snapshot.insert(info.doc);

    CanonicalSymbol cs(this, info);
    if (Symbol *canonicalSymbol = cs(textCursor())) {
        if (canonicalSymbol->identifier() != 0) {
//            if (showWarningMessage()) {
//                Core::EditorManager::instance()->showEditorInfoBar(QLatin1String("CppEditor.Rename"),
//                                                                   tr("This change cannot be undone."),
//                                                                   tr("Yes, I know what I am doing."),
//                                                                   this, SLOT(hideRenameNotification()));
//            }

            m_modelManager->renameUsages(canonicalSymbol, cs.context(), replacement);
        }
    }
}

void CPPEditorWidget::renameUsages()
{
    renameUsagesNow();
}

bool CPPEditorWidget::showWarningMessage() const
{
    // Restore settings
//    QSettings *settings = Core::ICore::instance()->settings();
//    settings->beginGroup(QLatin1String("CppEditor"));
//    settings->beginGroup(QLatin1String("Rename"));
//    const bool showWarningMessage = settings->value(QLatin1String("ShowWarningMessage"), true).toBool();
//    settings->endGroup();
//    settings->endGroup();
//    return showWarningMessage;
    return false;
}

void CPPEditorWidget::setShowWarningMessage(bool /*showWarningMessage*/)
{
    // Restore settings
//    QSettings *settings = Core::ICore::instance()->settings();
//    settings->beginGroup(QLatin1String("CppEditor"));
//    settings->beginGroup(QLatin1String("Rename"));
//    settings->setValue(QLatin1String("ShowWarningMessage"), showWarningMessage);
//    settings->endGroup();
//    settings->endGroup();
}

void CPPEditorWidget::hideRenameNotification()
{
//    setShowWarningMessage(false);
//    Core::EditorManager::instance()->hideEditorInfoBar(QLatin1String("CppEditor.Rename"));
}

void CPPEditorWidget::markSymbolsNow()
{
    if (m_references.isCanceled())
        return;
    else if (m_referencesCursorPosition != position())
        return;
    else if (m_referencesRevision != editorRevision())
        return;

    const SemanticInfo info = m_lastSemanticInfo;
    TranslationUnit *unit = info.doc->translationUnit();
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

static QList<int> lazyFindReferences(Scope *scope, QString code, Document::Ptr doc, Snapshot snapshot)
{
    TypeOfExpression typeOfExpression;
    snapshot.insert(doc);
    typeOfExpression.init(doc, snapshot);
    if (Symbol *canonicalSymbol = CanonicalSymbol::canonicalSymbol(scope, code, typeOfExpression)) {
        return CppModelManagerInterface::instance()->references(canonicalSymbol, typeOfExpression.context());
    }
    return QList<int>();
}

void CPPEditorWidget::markSymbols(const QTextCursor &tc, const SemanticInfo &info)
{
    abortRename();

    if (! info.doc)
        return;

    CanonicalSymbol cs(this, info);
    QString expression;
    if (Scope *scope = cs.getScopeAndExpression(this, info, tc, &expression)) {
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

void CPPEditorWidget::renameSymbolUnderCursor()
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

    if (m_renameSelections.isEmpty())
        renameUsages();
}

void CPPEditorWidget::onContentsChanged(int position, int charsRemoved, int charsAdded)
{
    Q_UNUSED(position)

    if (m_currentRenameSelection == NoCurrentRenameSelection || m_inRename)
        return;

    if (position + charsAdded == m_currentRenameSelectionBegin.position()) {
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

void CPPEditorWidget::updateFileName()
{ }

void CPPEditorWidget::jumpToOutlineElement(int)
{
    QModelIndex index = m_proxyModel->mapToSource(m_outlineCombo->view()->currentIndex());
    Symbol *symbol = m_outlineModel->symbolFromIndex(index);
    if (! symbol)
        return;

    openCppEditorAt(linkToSymbol(0, 0, symbol));
}

bool CPPEditorWidget::sortedOutline() const
{
    return (m_proxyModel->sortColumn() == 0);
}

void CPPEditorWidget::updateOutlineNow()
{
    const Snapshot snapshot = m_modelManager->snapshot();
    Document::Ptr document = snapshot.document(file()->fileName());

    if (!document)
        return;

    if (document->editorRevision() != editorRevision()) {
        m_updateOutlineTimer->start();
        return;
    }

    m_outlineModel->rebuild(document);

    OverviewTreeView *treeView = static_cast<OverviewTreeView *>(m_outlineCombo->view());
    treeView->sync();
    updateOutlineIndexNow();
}

void CPPEditorWidget::updateOutlineIndex()
{
    m_updateOutlineIndexTimer->start();
}

void CPPEditorWidget::highlightUses(const QList<SemanticInfo::Use> &uses,
                              const SemanticInfo &semanticInfo,
                              QList<QTextEdit::ExtraSelection> *selections)
{
    bool isUnused = false;

    if (uses.size() == 1)
        isUnused = true;

    foreach (const SemanticInfo::Use &use, uses) {
        QTextEdit::ExtraSelection sel;

        if (isUnused)
            sel.format = m_cppSemanticFormats[CppSemanticOccurrencesUnused];
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

void CPPEditorWidget::updateOutlineIndexNow()
{
    if (!m_outlineModel->document())
        return;

    if (m_outlineModel->document()->editorRevision() != editorRevision()) {
        m_updateOutlineIndexTimer->start();
        return;
    }

    m_updateOutlineIndexTimer->stop();

    m_outlineModelIndex = QModelIndex(); //invalidate
    QModelIndex comboIndex = outlineModelIndex();


    if (comboIndex.isValid()) {
        bool blocked = m_outlineCombo->blockSignals(true);

        // There is no direct way to select a non-root item
        m_outlineCombo->setRootModelIndex(m_proxyModel->mapFromSource(comboIndex.parent()));
        m_outlineCombo->setCurrentIndex(m_proxyModel->mapFromSource(comboIndex).row());
        m_outlineCombo->setRootModelIndex(QModelIndex());

        updateOutlineToolTip();

        m_outlineCombo->blockSignals(blocked);
    }
}

void CPPEditorWidget::updateOutlineToolTip()
{
    m_outlineCombo->setToolTip(m_outlineCombo->currentText());
}

void CPPEditorWidget::updateUses()
{
    if (editorRevision() != m_highlightRevision)
        m_highlighter.cancel();
    m_updateUsesTimer->start();
}

void CPPEditorWidget::updateUsesNow()
{
    if (m_currentRenameSelection != NoCurrentRenameSelection)
        return;

    semanticRehighlight();
}

void CPPEditorWidget::highlightSymbolUsages(int from, int to)
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
            m_cppHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
            b = b.next();
            ++m_nextHighlightBlockNumber;
        }

        QList<QTextLayout::FormatRange> formats;
        foreach (const SemanticInfo::Use &use, it.value()) {
            QTextLayout::FormatRange formatRange;

            switch (use.kind) {
            case SemanticInfo::Use::Type:
                formatRange.format = m_cppSemanticFormats[CppSemanticType];
                break;

            case SemanticInfo::Use::Field:
                formatRange.format = m_cppSemanticFormats[CppSemanticField];
                break;

            case SemanticInfo::Use::Local:
                formatRange.format = m_cppSemanticFormats[CppSemanticLocal];
                break;

            case SemanticInfo::Use::Static:
                formatRange.format = m_cppSemanticFormats[CppSemanticStatic];
                break;

            case SemanticInfo::Use::VirtualMethod:
                formatRange.format = m_cppSemanticFormats[CppSemanticVirtualMethod];
                break;

            case SemanticInfo::Use::Function:
                formatRange.format = m_cppSemanticFormats[CppSemanticFunction];
                break;

            case SemanticInfo::Use::Label:
                formatRange.format = m_cppHighlighter->labelFormat();
                break;

            case SemanticInfo::Use::SqlTableAlias:
                formatRange.format = m_cppSemanticFormats[CppSemanticSqlTableAlias];
                break;

            default:
                continue;
            }

            formatRange.start = use.column - 1;
            formatRange.length = use.length;
            formats.append(formatRange);
        }
        m_cppHighlighter->setExtraAdditionalFormats(b, formats);
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}

void CPPEditorWidget::finishHighlightSymbolUsages()
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
        m_cppHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
        b = b.next();
        ++m_nextHighlightBlockNumber;
    }
}


void CPPEditorWidget::switchDeclarationDefinition()
{
    if (! m_modelManager)
        return;

    const Snapshot snapshot = m_modelManager->snapshot();

    if (Document::Ptr thisDocument = snapshot.document(file()->fileName())) {
        unsigned line = 0, positionInBlock = 0;
        convertPosition(position(), &line, &positionInBlock);

        Symbol *lastVisibleSymbol = thisDocument->lastVisibleSymbolAt(line, positionInBlock + 1);
        if (! lastVisibleSymbol)
            return;

        Function *function = lastVisibleSymbol->asFunction();
        if (! function)
            function = lastVisibleSymbol->enclosingFunction();

        if (function) {
            LookupContext context(thisDocument, snapshot);

            Function *functionDefinition = function->asFunction();
            ClassOrNamespace *binding = context.lookupType(functionDefinition);

            const QList<LookupItem> declarations = context.lookup(functionDefinition->name(), functionDefinition->enclosingScope());
            QList<Symbol *> best;
            foreach (const LookupItem &r, declarations) {
                if (Symbol *decl = r.declaration()) {
                    if (Function *funTy = decl->type()->asFunctionType()) {
                        if (funTy->isEqualTo(function) && decl != function && binding == r.binding())
                            best.prepend(decl);
                        else
                            best.append(decl);
                    }
                }
            }
            if (! best.isEmpty())
                openCppEditorAt(linkToSymbol(0, 0, best.first()));

        } else if (lastVisibleSymbol && lastVisibleSymbol->isDeclaration() && lastVisibleSymbol->type()->isFunctionType()) {
            if (Symbol *def = snapshot.findMatchingDefinition(lastVisibleSymbol))
                openCppEditorAt(linkToSymbol(0, 0, def));
        }
    }
}

void CPPEditorWidget::cppHelper( const QString &key, ITextDocument *textDocument )
{
    SemanticInfo info = m_lastSemanticInfo;
    info.snapshot = CppModelManagerInterface::instance()->snapshot();
    info.snapshot.insert(info.doc);

    CanonicalSymbol cs(this, info);
    if ( Symbol *canonicalSymbol = cs(textCursor()) )
        if (canonicalSymbol->identifier() != 0)
            return;

    if ( const CppTools::CppHelperItem *item = m_modelManager->cppHelper(key, textDocument, file()->fileName()) ) {
        const Token tk = SimpleLexer::tokenAt(textCursor().block().text(), textCursor().positionInBlock(),
                                              BackwardsScanner::previousBlockState(textCursor().block()));
        if ( !tk.isComment() && !tk.isLiteral() ) {
            if ( Core::ILink *link = findLinkAt(textCursor()) ) {
                delete link;
            } else {
                m_modelManager->finishCppHelper(item, info.doc, document());
            }
        }
        delete item;
    }
}

static inline LookupItem skipForwardDeclarations(const QList<LookupItem> &resolvedSymbols)
{
    QList<LookupItem> candidates = resolvedSymbols;

    LookupItem result = candidates.first();
    const FullySpecifiedType ty = result.type().simplified();

    if (ty->isForwardClassDeclarationType()) {
        while (! candidates.isEmpty()) {
            LookupItem r = candidates.takeFirst();

            if (! r.type()->isForwardClassDeclarationType()) {
                result = r;
                break;
            }
        }
    }

    if (ty->isObjCForwardClassDeclarationType()) {
        while (! candidates.isEmpty()) {
            LookupItem r = candidates.takeFirst();

            if (! r.type()->isObjCForwardClassDeclarationType()) {
                result = r;
                break;
            }
        }
    }

    if (ty->isObjCForwardProtocolDeclarationType()) {
        while (! candidates.isEmpty()) {
            LookupItem r = candidates.takeFirst();

            if (! r.type()->isObjCForwardProtocolDeclarationType()) {
                result = r;
                break;
            }
        }
    }

    return result;
}

Core::ILink *CPPEditorWidget::attemptFuncDeclDef(const QTextCursor &cursor, const Document::Ptr &doc, Snapshot snapshot) const
{
    snapshot.insert(doc);

    Core::ILink *result = 0;

    QList<AST *> path = ASTPath(doc)(cursor);

    if (path.size() < 5)
        return result;

    NameAST *name = path.last()->asName();
    if (!name)
        return result;

    if (QualifiedNameAST *qName = path.at(path.size() - 2)->asQualifiedName()) {
        // TODO: check which part of the qualified name we're on
        if (qName->unqualified_name != name)
            return result;
    }

    for (int i = path.size() - 1; i != -1; --i) {
        AST *node = path.at(i);

        if (node->asParameterDeclaration() != 0)
            return result;
    }

    AST *declParent = 0;
    DeclaratorAST *decl = 0;
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
    FunctionDeclaratorAST *funcDecl = decl->postfix_declarator_list->value->asFunctionDeclarator();
    if (!funcDecl)
        return result;

    Symbol *target = 0;
    if (FunctionDefinitionAST *funDef = declParent->asFunctionDefinition()) {
        QList<Declaration *> candidates = findMatchingDeclaration(LookupContext(doc, snapshot),
                                                                  funDef->symbol);
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

Core::ILink *CPPEditorWidget::findMacroLink(int begin, int end, const QByteArray &name) const
{
    if (! name.isEmpty()) {
        if (Document::Ptr doc = m_lastSemanticInfo.doc) {
            const Snapshot snapshot = m_modelManager->snapshot();
            QSet<QString> processed;
            return findMacroLink(begin, end, name, doc, snapshot, &processed);
        }
    }

    return 0;
}

Core::ILink *CPPEditorWidget::findMacroLink( int begin, int end, const QByteArray &name,
                                             Document::Ptr doc,
                                             const Snapshot &snapshot,
                                             QSet<QString> *processed ) const
{
    if (doc && ! name.startsWith('<') && ! processed->contains(doc->fileName())) {
        processed->insert(doc->fileName());

        foreach (const Macro &macro, doc->definedMacros()) {
            if (macro.name() == name) {
                return new Core::TextTargetLink(begin, end, macro.fileName(), macro.line(), 0);
            }
        }

        const QList<Document::Include> includes = doc->includes();
        for (int index = includes.size() - 1; index != -1; --index) {
            const Document::Include &i = includes.at(index);
            if ( Core::ILink *link = findMacroLink(begin, end, name, snapshot.document(i.fileName()), snapshot, processed) )
                return link;
        }
    }

    return 0;
}

QString CPPEditorWidget::identifierUnderCursor(QTextCursor *macroCursor) const
{
    macroCursor->movePosition(QTextCursor::StartOfWord);
    macroCursor->movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    return macroCursor->selectedText();
}

static Core::ILink *handleExecSqlCInclude(const QTextBlock &block, int positionInBlock, Document::Ptr doc)
{
    QString blockText = block.text();
    int pos = blockText.indexOf("C$INCLUDE", Qt::CaseInsensitive);
    int length = blockText.length();
    if ( pos != -1 ) {
        pos += 9;
        while ( pos < length && blockText[pos].isSpace() )
            pos++;
        if ( pos == length || blockText[pos] != ':' )
            pos = -1;
    }
    if ( pos != -1 && pos < positionInBlock ) {
        const unsigned lineno = block.blockNumber() + 1;
        foreach ( const Document::Include &incl, doc->includes() ) {
            if ( incl.line() == lineno && incl.resolved() ) {
                int startPos = positionInBlock;
                int endPos = positionInBlock;
                while ( endPos < length ) {
                    QChar ch = blockText[endPos];
                    char c = ch.toLatin1();
                    if ( !ch.isLetterOrNumber() && c != '_' && c != '.' )
                        break;
                    endPos++;
                }
                while ( startPos > 0 ) {
                    QChar ch = blockText[startPos];
                    char c = ch.toLatin1();
                    if ( !ch.isLetterOrNumber() && c != '_' && c != '.' )
                        break;
                    startPos--;
                }
                int beginOfToken = block.position() + startPos;
                int endOfToken = block.position() + endPos;
                return new Core::TextTargetLink(beginOfToken + 1, endOfToken, incl.fileName(), 0, 0);
            }
        }
    }
    return 0;
}

Core::ILink *CPPEditorWidget::findLinkAt( const QTextCursor &cursor,
                                          bool resolveTarget )
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

    SimpleLexer tokenize;
    tokenize.setQtMocRunEnabled(true);
    const QString blockText = cursor.block().text();
    const QList<Token> tokens = tokenize(blockText, BackwardsScanner::previousBlockState(cursor.block()));

    bool recognizedQtMethod = false;

    for (int i = 0; i < tokens.size(); ++i) {
        const Token &tk = tokens.at(i);

        if (((unsigned) positionInBlock) >= tk.begin() && ((unsigned) positionInBlock) <= tk.end()) {
            if (i >= 2 && tokens.at(i).is(T_IDENTIFIER) && tokens.at(i - 1).is(T_LPAREN)
                && (tokens.at(i - 2).is(T_SIGNAL) || tokens.at(i - 2).is(T_SLOT))) {

                // token[i] == T_IDENTIFIER
                // token[i + 1] == T_LPAREN
                // token[.....] == ....
                // token[i + n] == T_RPAREN

                if (i + 1 < tokens.size() && tokens.at(i + 1).is(T_LPAREN)) {
                    // skip matched parenthesis
                    int j = i - 1;
                    int depth = 0;

                    for (; j < tokens.size(); ++j) {
                        if (tokens.at(j).is(T_LPAREN))
                            ++depth;

                        else if (tokens.at(j).is(T_RPAREN)) {
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
        const Token tk = SimpleLexer::tokenAt(block.text(), pos, BackwardsScanner::previousBlockState(block), true);

        beginOfToken = block.position() + tk.begin();
        endOfToken = block.position() + tk.end();

        // Handle include directives
        if (tk.is(T_STRING_LITERAL) || tk.is(T_ANGLE_STRING_LITERAL)) {
            const unsigned lineno = cursor.blockNumber() + 1;
            foreach (const Document::Include &incl, doc->includes()) {
                if (incl.line() == lineno && incl.resolved()) {
                    return new Core::TextTargetLink(beginOfToken + 1, endOfToken - 1, incl.fileName(), 0, 0);
                }
            }
        }
        // Handle exec sql c$include directives
        if ( (tk.is(T_IDENTIFIER) || tk.is(T_DOT)) && !block.userState() ) {
            if ( Core::ILink *cincludeLink = handleExecSqlCInclude(block, pos, doc) )
                return cincludeLink;
        }

        if (tk.isNot(T_IDENTIFIER) && tk.kind() < T_FIRST_QT_KEYWORD && tk.kind() > T_LAST_KEYWORD)
            return 0;

        tc.setPosition(endOfToken);
    }

    // Find the last symbol up to the cursor position
    Scope *scope = doc->scopeAt(line, column);
    if (!scope)
        return 0;

    // Evaluate the type of the expression under the cursor
    ExpressionUnderCursor expressionUnderCursor;
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
        QSharedPointer <TypeOfExpression> typeOfExpression(new TypeOfExpression);
        typeOfExpression->init(doc, snapshot);
        const QList<LookupItem> resolvedSymbols = typeOfExpression->reference(expression, scope, TypeOfExpression::Preprocess);

        if (!resolvedSymbols.isEmpty()) {
            LookupItem result = skipForwardDeclarations(resolvedSymbols);

            foreach (const LookupItem &r, resolvedSymbols) {
                if (Symbol *d = r.declaration()) {
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

            if (Symbol *symbol = result.declaration()) {
                Symbol *def = 0;

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
                    Symbol *lastVisibleSymbol = doc->lastVisibleSymbolAt(line, column);

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

void CPPEditorWidget::jumpToDefinition()
{
    if ( Core::ILink *link = findLinkAt(textCursor()) ) {
        openLink(link);
        delete link;
    }
}

Symbol *CPPEditorWidget::findDefinition(Symbol *symbol, const Snapshot &snapshot) const
{
    if (symbol->isFunction())
        return 0; // symbol is a function definition.

    else if (! symbol->type()->isFunctionType())
        return 0; // not a function declaration

    return snapshot.findMatchingDefinition(symbol);
}

unsigned CPPEditorWidget::editorRevision() const
{
    return document()->revision();
}

bool CPPEditorWidget::isOutdated() const
{
    if (m_lastSemanticInfo.revision != editorRevision())
        return true;

    return false;
}

SemanticInfo CPPEditorWidget::semanticInfo() const
{
    return m_lastSemanticInfo;
}

CPlusPlus::OverviewModel *CPPEditorWidget::outlineModel() const
{
    return m_outlineModel;
}

QModelIndex CPPEditorWidget::outlineModelIndex()
{
    if (!m_outlineModelIndex.isValid()) {
        unsigned line = 0, column = 0;
        convertPosition(position(), &line, &column);
        m_outlineModelIndex = indexForPosition(line, column);
        emit outlineModelIndexChanged(m_outlineModelIndex);
    }

    return m_outlineModelIndex;
}

bool CPPEditorWidget::event(QEvent *e)
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

void CPPEditorWidget::performQuickFix(int index)
{
    TextEditor::QuickFixOperation::Ptr op = m_quickFixes.at(index);
    op->perform();
}

void CPPEditorWidget::keyPressEvent(QKeyEvent *e)
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

Core::Context CPPEditor::context() const
{
    return m_context;
}

Core::IEditor *CPPEditor::duplicate(QWidget * /*parent*/)
{
//    CPPEditorWidget *newEditor = new CPPEditorWidget(parent);
//    newEditor->duplicateFrom(editorWidget());
//    CppPlugin::instance()->initializeEditor(newEditor);
//    return newEditor->editor();
    return 0;
}

QString CPPEditor::id() const
{
    return QLatin1String(CppEditor::Constants::CPPEDITOR_ID);
}

bool CPPEditor::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
    bool b = TextEditor::BaseTextEditor::open(errorString, fileName, realFileName);
    editorWidget()->setMimeType(Core::Storage::mainWindow()->mimeDatabase()->findByFile(QFileInfo(fileName)).type());
    return b;
}

void CPPEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    TextEditor::TextEditorWidget::setFontSettings(fs);

    // only set the background, we do not want to modify foreground properties set by the syntax highlighter or the link
    m_cppSemanticFormats[CppSemanticOccurrences].clearForeground();
    m_occurrenceRenameFormat.clearForeground();

    // Clear all additional formats since they may have changed
    QTextBlock b = document()->firstBlock();
    while (b.isValid()) {
        m_cppHighlighter->setExtraAdditionalFormats(b, QList<QTextLayout::FormatRange>());
        b = b.next();
    }

    // This also triggers an update of the additional formats
    m_cppHighlighter->rehighlight();
}

void CPPEditorWidget::setTabSettings(const TextEditor::TabSettings &ts)
{
    CppTools::QtStyleCodeFormatter formatter;
    formatter.invalidateCache(document());

    TextEditor::TextEditorWidget::setTabSettings(ts);
}

void CPPEditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this);
}

void CPPEditorWidget::onSettingsChanged()
{
    TextEditorWidget::onSettingsChanged();
    m_cppHighlighter->onFormatSettingsChanged();
    onSemanticFormatSettingsChanged();
}

void CPPEditorWidget::onSemanticFormatSettingsChanged()
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("Cpp"));
    int offset = NumCppFormats;
    for (int i = 0; i < CppSemanticFormatCount; i++, offset++)
        formatData->fillTextCharFormat(offset, m_cppSemanticFormats[i]);

    m_cppSemanticFormats[CppSemanticOccurrencesUnused].setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_cppSemanticFormats[CppSemanticOccurrencesUnused].setUnderlineColor(m_cppSemanticFormats[CppSemanticOccurrencesUnused].foreground().color());
    m_cppSemanticFormats[CppSemanticOccurrencesUnused].clearForeground();
    m_cppSemanticFormats[CppSemanticOccurrencesUnused].setToolTip(tr("Unused variable"));

    semanticRehighlight();
}

Core::ILink *CPPEditorWidget::linkToSymbol(int begin, int end, CPlusPlus::Symbol *symbol)
{
    if (!symbol)
        return 0;

    const QString fileName = QString::fromUtf8(symbol->fileName(),
                                               symbol->fileNameLength());
    unsigned line = symbol->line();
    unsigned column = symbol->column();

    if (column)
        --column;

    if (symbol->isGenerated())
        column = 0;

    return new Core::TextTargetLink(begin, end, fileName, line, column);
}

bool CPPEditorWidget::openCppEditorAt( Core::ILink *link )
{
    if ( !link )
        return false;

    return link->open();
}

void CPPEditorWidget::semanticRehighlight()
{
    m_semanticHighlighter->rehighlight(currentSource());
}

void CPPEditorWidget::waitForEditorSupport()
{
    modelManager()->forceUpdateEditor(editor());
}

bool CPPEditorWidget::checkApplySizeForSemanticHighlighter( int docSize )
{
    const QMap<QString, QVariant> &valuesMap = SettingsManager::instance()->valuesMap(CppEditor::Constants::C_CPP_EDITOR_SETTINGS_ID);
    if ( valuesMap["ApplyToAll"].toBool() )
        return true;

    int nSize = valuesMap["ApplyToLessSize"].toInt() * 1024;
    return (docSize < nSize);
}

bool CPPEditorWidget::fillQuickFixesMenu( QMenu *quickFixMenu )
{
    bool result = false;

    if ( m_mapper )
        delete m_mapper;
    m_mapper = new QSignalMapper;

    quickFixMenu->clear();
    m_quickFixes.clear();

    connect(m_mapper, SIGNAL(mapped(int)), this, SLOT(performQuickFix(int)));
    if (! isOutdated()) {
        TextEditor::IAssistInterface *interface_ =
            createAssistInterface(TextEditor::QuickFix, TextEditor::ExplicitlyInvoked);
        if (interface_) {
            TextEditor::IAssistProvider *provider = m_editorPlugin->quickFixProvider();
            if ( provider ) {
                QScopedPointer<TextEditor::IAssistProcessor> processor(provider->createProcessor());
                QScopedPointer<TextEditor::IAssistProposal> proposal(processor->perform(interface_));
                if (!proposal.isNull()) {
                    TextEditor::BasicProposalItemListModel *model =
                            static_cast<TextEditor::BasicProposalItemListModel *>(proposal->model());
                    for (int index = 0; index < model->size(); ++index) {
                        TextEditor::BasicProposalItem *item =
                                static_cast<TextEditor::BasicProposalItem *>(model->proposalItem(index));
                        TextEditor::QuickFixOperation::Ptr op =
                                item->data().value<TextEditor::QuickFixOperation::Ptr>();
                        m_quickFixes.append(op);
                        QAction *action = quickFixMenu->addAction(op->description());
                        m_mapper->setMapping(action, index);
                        connect(action, SIGNAL(triggered()), m_mapper, SLOT(map()));

                        result = true;
                    }
                    delete model;
                }
            }
        }
    }

    return result;
}

void CPPEditorWidget::updateSemanticInfo(const SemanticInfo &semanticInfo)
{
    if (semanticInfo.revision != editorRevision()) {
        // got outdated semantic info
        semanticRehighlight();
        return;
    }

    const SemanticInfo previousSemanticInfo = m_lastSemanticInfo;
    m_lastSemanticInfo = semanticInfo; // update the semantic info

    unsigned line = 0, column = 0;
    convertPosition(position(), &line, &column);

    QList<QTextEdit::ExtraSelection> unusedSelections;

    m_renameSelections.clear();
    m_currentRenameSelection = NoCurrentRenameSelection;

    SemanticInfo::LocalUseIterator it(semanticInfo.localUses);
    while (it.hasNext()) {
        it.next();
        const QList<SemanticInfo::Use> &uses = it.value();

        bool good = false;
        foreach (const SemanticInfo::Use &use, uses) {
            unsigned l = line;
            unsigned c = column + 1; // convertCursorPosition() returns a 0-based column number.
            if (l == use.line && c >= use.column && c <= (use.column + use.length)) {
                good = true;
                break;
            }
        }

        if (uses.size() == 1) {
            if ( uses[0].kind != SemanticInfo::Use::SqlTableAliasShadowed )
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

        if ( !semanticHighlighterDisabled && semanticInfo.doc &&
             checkApplySizeForSemanticHighlighter(semanticInfo.doc->source().size()) ) {
            LookupContext context(semanticInfo.doc, semanticInfo.snapshot);
            CheckSymbols::Future f = CheckSymbols::go(semanticInfo.doc, context);
            m_highlighter = f;
            m_highlightRevision = semanticInfo.revision;
            m_nextHighlightBlockNumber = 0;
            m_highlightWatcher.setFuture(m_highlighter);
        }

#if 0 // ### TODO: enable objc semantic highlighting
        setExtraSelections(ObjCSelection, createSelections(document(),
                                                           semanticInfo.objcKeywords,
                                                           m_keywordFormat));
#endif
    }


    setExtraSelections(UnusedSymbolSelection, unusedSelections);

    if (! m_renameSelections.isEmpty())
        setExtraSelections(CodeSemanticsSelection, m_renameSelections); // ###
    else {
        markSymbols(textCursor(), semanticInfo);
    }

    m_lastSemanticInfo.forced = false; // clear the forced flag
}

namespace {

class FindObjCKeywords: public ASTVisitor
{
public:
    FindObjCKeywords(TranslationUnit *unit)
        : ASTVisitor(unit)
    {}

    QList<SemanticInfo::Use> operator()()
    {
        _keywords.clear();
        accept(translationUnit()->ast());
        return _keywords;
    }

    virtual bool visit(ObjCClassDeclarationAST *ast)
    {
        addToken(ast->interface_token);
        addToken(ast->implementation_token);
        addToken(ast->end_token);
        return true;
    }

    virtual bool visit(ObjCClassForwardDeclarationAST *ast)
    { addToken(ast->class_token); return true; }

    virtual bool visit(ObjCProtocolDeclarationAST *ast)
    { addToken(ast->protocol_token); addToken(ast->end_token); return true; }

    virtual bool visit(ObjCProtocolForwardDeclarationAST *ast)
    { addToken(ast->protocol_token); return true; }

    virtual bool visit(ObjCProtocolExpressionAST *ast)
    { addToken(ast->protocol_token); return true; }

    virtual bool visit(ObjCTypeNameAST *) { return true; }

    virtual bool visit(ObjCEncodeExpressionAST *ast)
    { addToken(ast->encode_token); return true; }

    virtual bool visit(ObjCSelectorExpressionAST *ast)
    { addToken(ast->selector_token); return true; }

    virtual bool visit(ObjCVisibilityDeclarationAST *ast)
    { addToken(ast->visibility_token); return true; }

    virtual bool visit(ObjCPropertyAttributeAST *ast)
    {
        const Identifier *attrId = identifier(ast->attribute_identifier_token);
        if (attrId == control()->objcAssignId()
                || attrId == control()->objcCopyId()
                || attrId == control()->objcGetterId()
                || attrId == control()->objcNonatomicId()
                || attrId == control()->objcReadonlyId()
                || attrId == control()->objcReadwriteId()
                || attrId == control()->objcRetainId()
                || attrId == control()->objcSetterId())
            addToken(ast->attribute_identifier_token);
        return true;
    }

    virtual bool visit(ObjCPropertyDeclarationAST *ast)
    { addToken(ast->property_token); return true; }

    virtual bool visit(ObjCSynthesizedPropertiesDeclarationAST *ast)
    { addToken(ast->synthesized_token); return true; }

    virtual bool visit(ObjCDynamicPropertiesDeclarationAST *ast)
    { addToken(ast->dynamic_token); return true; }

    virtual bool visit(ObjCFastEnumerationAST *ast)
    { addToken(ast->for_token); addToken(ast->in_token); return true; }

    virtual bool visit(ObjCSynchronizedStatementAST *ast)
    { addToken(ast->synchronized_token); return true; }

protected:
    void addToken(unsigned token)
    {
        if (token) {
            SemanticInfo::Use use;
            getTokenStartPosition(token, &use.line, &use.column);
            use.length = tokenAt(token).length();
            _keywords.append(use);
        }
    }

private:
    QList<SemanticInfo::Use> _keywords;
};

} // anonymous namespace

SemanticHighlighter::Source CPPEditorWidget::currentSource(bool force)
{
    unsigned line = 0, column = 0;
    convertPosition(position(), &line, &column);

    const Snapshot snapshot = m_modelManager->snapshot();
    const QString fileName = file()->fileName();

    QString code;
    if (force || m_lastSemanticInfo.revision != editorRevision())
        code = toPlainText(); // get the source code only when needed.

    const unsigned revision = editorRevision();
    SemanticHighlighter::Source source(snapshot, fileName, code,
                                       line, column, revision);
    source.force = force;
    return source;
}

SemanticHighlighter::SemanticHighlighter(QObject *parent)
        : QThread(parent),
          m_done(false)
{
}

SemanticHighlighter::~SemanticHighlighter()
{
}

void SemanticHighlighter::abort()
{
    QMutexLocker locker(&m_mutex);
    m_done = true;
    m_condition.wakeOne();
}

void SemanticHighlighter::rehighlight(const Source &source)
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

        const SemanticInfo info = semanticInfo(source);

        if (! isOutdated()) {
            m_mutex.lock();
            m_lastSemanticInfo = info;
            m_mutex.unlock();

            emit changed(info);
        }
    }
}

SemanticInfo SemanticHighlighter::semanticInfo(const Source &source)
{
    m_mutex.lock();
    const int revision = m_lastSemanticInfo.revision;
    m_mutex.unlock();

    Snapshot snapshot;
    Document::Ptr doc;
    QList<Utils::DiagnosticMessage> diagnosticMessages;
    QList<SemanticInfo::Use> objcKeywords;

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

#if 0
        if (TranslationUnit *unit = doc->translationUnit()) {
            FindObjCKeywords findObjCKeywords(unit); // ### remove me
            objcKeywords = findObjCKeywords();
        }
#endif
    }

    TranslationUnit *translationUnit = doc->translationUnit();
    AST *ast = translationUnit->ast();

    FunctionDefinitionUnderCursor functionDefinitionUnderCursor(translationUnit);
    DeclarationAST *currentFunctionDefinition = functionDefinitionUnderCursor(ast, source.line, source.column);

    const LocalSymbols useTable(doc, currentFunctionDefinition);

    SemanticInfo semanticInfo;
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

QModelIndex CPPEditorWidget::indexForPosition(int line, int column, const QModelIndex &rootIndex) const
{
    QModelIndex lastIndex = rootIndex;

    const int rowCount = m_outlineModel->rowCount(rootIndex);
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = m_outlineModel->index(row, 0, rootIndex);
        Symbol *symbol = m_outlineModel->symbolFromIndex(index);
        if (symbol && symbol->line() > unsigned(line))
            break;
        lastIndex = index;
    }

    if (lastIndex != rootIndex) {
        // recurse
        lastIndex = indexForPosition(line, column, lastIndex);
    }

    return lastIndex;
}

TextEditor::IAssistInterface *CPPEditorWidget::createAssistInterface(
    TextEditor::AssistKind kind,
    TextEditor::AssistReason reason) const
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
    } else if (kind == TextEditor::QuickFix) {
        if (!semanticInfo().doc || isOutdated())
            return 0;
        return new CppQuickFixAssistInterface(const_cast<CPPEditorWidget *>(this), reason);
    } else {
        return TextEditorWidget::createAssistInterface(kind, reason);
    }
    return 0;
}

void CPPEditorWidget::onOutlineSortChanged( bool sorted )
{
    if ( sorted )
        m_proxyModel->sort(0, Qt::AscendingOrder);
    else
        m_proxyModel->sort(-1, Qt::AscendingOrder);
}

#include "cppeditor.moc"
