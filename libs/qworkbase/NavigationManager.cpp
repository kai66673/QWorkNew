#include <QDebug>

#include "ifile.h"
#include "ieditor.h"
#include "itexteditor.h"

#include "NavigationManager.h"

#include "QWorkBase.h"

NavigationManager::NavigationManager(QObject *parent)
    : QObject(parent)
    , m_currentNavigationHistoryPosition(0)
    , m_blocked(false)
{
}

void NavigationManager::init()
{
    connect(DocumentManager::getInstance(), SIGNAL(documentClosed(IDocument*)), this, SLOT(onDocumentClosed(IDocument*)));
}

void NavigationManager::setNavigationHistoryBlocked( bool onoff )
{
    m_blocked = onoff;
}

void NavigationManager::addCurrentPositionToNavigationHistory(Core::IEditor *editor, const QByteArray &saveState)
{
    if ( m_blocked )
        return;

    Core::IEditor *currentEditor = DocumentManager::getInstance()->currentEditor();
    IDocument *currentDocument = DocumentManager::getInstance()->currentDocument();

    if ( !currentDocument )
        return;

    if ( editor && editor != currentEditor )
        return;

    if ( !editor )
        editor = currentEditor;
    if ( !editor )
        return;

    Core::IFile *file = editor->file();
    if ( !file )
        return;

    QByteArray state;
    if (saveState.isNull()) {
        state = editor->saveState();
    } else {
        state = saveState;
    }

    EditLocation location;
    location.file = file;
    location.fileName = file->fileName();
    location.id = editor->id();
    location.state = QVariant(state);
    location.document = currentDocument;
    m_currentNavigationHistoryPosition = qMin(m_currentNavigationHistoryPosition, m_navigationHistory.size()); // paranoia
    m_navigationHistory.insert(m_currentNavigationHistoryPosition, location);
    ++m_currentNavigationHistoryPosition;

    while (m_navigationHistory.size() >= 30) {
        if (m_currentNavigationHistoryPosition > 15) {
            m_navigationHistory.removeFirst();
            --m_currentNavigationHistoryPosition;
        } else {
            m_navigationHistory.removeLast();
        }
    }

    QWorkMainWindow::getInstance()->handleNavigationStateChanged(canGoBack(), canGoForward());
}

void NavigationManager::cutForwardNavigationHistory()
{
    if ( m_blocked )
        return;

    while (m_currentNavigationHistoryPosition < m_navigationHistory.size() - 1)
        m_navigationHistory.removeLast();

    QWorkMainWindow::getInstance()->handleNavigationStateChanged(canGoBack(), canGoForward());
}

void NavigationManager::goBackInNavigationHistory()
{
    updateCurrentPositionInNavigationHistory();
    while (m_currentNavigationHistoryPosition > 0) {
        --m_currentNavigationHistoryPosition;
        EditLocation location = m_navigationHistory.at(m_currentNavigationHistoryPosition);
        TextEditor::ITextEditor *editor = 0;
        if ( location.document ) {
            if ( DocumentManager::getInstance()->activateDocument(location.document) )
                editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
        }
        if ( !editor ) {
            m_navigationHistory.removeAt(m_currentNavigationHistoryPosition);
            continue;
        }

        editor->restoreState(location.state.toByteArray());
        QWorkMainWindow::getInstance()->handleNavigationStateChanged(canGoBack(), canGoForward());
        break;
    }
}

void NavigationManager::goForwardInNavigationHistory()
{
    updateCurrentPositionInNavigationHistory();
    if ( m_currentNavigationHistoryPosition >= m_navigationHistory.size() - 1 )
        return;
    ++m_currentNavigationHistoryPosition;
    EditLocation location = m_navigationHistory.at(m_currentNavigationHistoryPosition);
    TextEditor::ITextEditor *editor = 0;
    if ( location.document ) {
        if ( DocumentManager::getInstance()->activateDocument(location.document) )
            editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    }
    if ( editor ) {
        editor->restoreState(location.state.toByteArray());
        QWorkMainWindow::getInstance()->handleNavigationStateChanged(canGoBack(), canGoForward());
    }
}

void NavigationManager::onDocumentClosed( IDocument *document )
{
    if ( !document )
        return;

    for (int i = m_navigationHistory.size() - 1; i >= 0; i--) {
        if ( m_navigationHistory[i].document == document ) {
            if ( m_currentNavigationHistoryPosition >= i )
                m_currentNavigationHistoryPosition--;
            m_navigationHistory.removeAt(i);
        }
    }

    if ( m_currentNavigationHistoryPosition < 0 )
        m_currentNavigationHistoryPosition = 0;
    QWorkMainWindow::getInstance()->handleNavigationStateChanged(canGoBack(), canGoForward());
}

void NavigationManager::updateCurrentPositionInNavigationHistory()
{
    Core::IEditor *editor = DocumentManager::getInstance()->currentEditor();
    if ( !editor )
        return;

    Core::IFile *file = editor->file();
    if ( !file )
        return;

    EditLocation *location;
    if (m_currentNavigationHistoryPosition < m_navigationHistory.size()) {
        location = &m_navigationHistory[m_currentNavigationHistoryPosition];
    } else {
        m_navigationHistory.append(EditLocation());
        location = &m_navigationHistory[m_navigationHistory.size()-1];
    }
    location->file = file;
    location->fileName = file->fileName();
    location->id = editor->id();
    location->state = QVariant(editor->saveState());
    location->document = DocumentManager::getInstance()->currentDocument();
}
