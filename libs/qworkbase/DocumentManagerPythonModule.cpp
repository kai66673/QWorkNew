#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "DocumentManagerPythonModule.h"
#include "Core.h"

namespace Python {

Document::Document(Core::PythonQtStorageDestroyDelegate *delegate, IDocument *document )
    : Core::PythonQtObject(delegate, document)
    , m_document(document)
{ }

Document::~Document()
{ }

bool Document::isModified()
{
    if ( !isValid() )
        return false;

    return m_document->isModified();
}

bool Document::isNew()
{
    if ( !isValid() )
        return false;

    return m_document->isNew();
}

bool Document::insertText( const QString &text )
{
    if ( !isValid() )
        return false;

    ITextDocument *textDocument = dynamic_cast<ITextDocument *>(m_document);
    if ( !textDocument ) {
        PythonScriptManager::getInstance()->pythonReport(tr("No Text Document Is Active"));
        return false;
    }

    textDocument->insertText(text);
    return true;
}

QString Document::selectedText()
{
    if ( !isValid() )
        return QStringLiteral("");

    ITextDocument *textDocument = dynamic_cast<ITextDocument *>(m_document);
    if ( !textDocument ) {
        PythonScriptManager::getInstance()->pythonReport(tr("No Text Document Is Active"));
        return QStringLiteral("");
    }

    return textDocument->selectedText();
}

QString Document::text()
{
    if ( !isValid() )
        return QStringLiteral("");

    ITextDocument *textDocument = dynamic_cast<ITextDocument *>(m_document);
    if ( !textDocument ) {
        PythonScriptManager::getInstance()->pythonReport(tr("No Text Document Is Active"));
        return QStringLiteral("");
    }

    return textDocument->text();
}

QString Document::filePath()
{
    if ( !isValid() )
        return QStringLiteral("");

    return m_document->filePath();
}

bool Document::save()
{
    if ( !isValid() )
        return false;

    return DocumentManager::getInstance()->trySaveDocument(m_document);
}

IDocument *Document::document() const
{
    return isValid() ? m_document : 0;
}

DocumentManagerPythonModule::DocumentManagerPythonModule()
    : IPythonModule("QwDocumentManager", "documentManager", "Document Manager public methods")
{
    m_functions << new PythonModuleFunction("newDocument", "Open New Document with default Extention")
                << new PythonModuleFunction("openNewDocument", "Open New Document Named by argument")
                << new PythonModuleFunction("openDocument", "Open existing Document")
                << new PythonModuleFunction("documentsCount", "Get Opened Documents Count")
                << new PythonModuleFunction("documents", "Get All Opened Documents")
                << new PythonModuleFunction("activeDocument", "Returns Active (Current) Document")
                << new PythonModuleFunction("setActiveDocument", "Activate document");
}

DocumentManagerPythonModule::~DocumentManagerPythonModule()
{
    qDeleteAll(m_functions);
}

QList<PythonModuleFunction *> DocumentManagerPythonModule::functions() const
{
    return m_functions;
}

QString DocumentManagerPythonModule::sourceCodeForAssist() const
{
    return QString("class Document():\n"
                   "    def isModified(self):\n"
                   "        return True\n"
                   "    def isNew(self):\n"
                   "        return True\n"
                   "    def insertText(self, text):\n"
                   "        return True\n"
                   "    def selectedText(self):\n"
                   "        return 'Any Text'\n"
                   "    def text(self):\n"
                   "        return 'Any Text'\n"
                   "    def filePath(self):\n"
                   "        return 'ANY_FILE.TXT'\n"
                   "    def save(self):\n"
                   "        return True\n"
                   "class QwDocumentManager():\n"
                   "    def newDocument(self, extension):\n"
                   "        return Document()\n"
                   "    def openNewDocument(self, fileName):\n"
                   "        return Document()\n"
                   "    def openDocument(self, filePath = ''):\n"
                   "        return Document()\n"
                   "    def documentsCount(self):\n"
                   "        return 1\n"
                   "    def documents(self):\n"
                   "        return [Document(), Document()]\n"
                   "    def activeDocument(self):\n"
                   "        return Document()\n"
                   "    def setActiveDocument(self, document):\n"
                   "        return True");
}

Document *DocumentManagerPythonModule::newDocument( const QString &extension )
{
    if ( DocumentManager::getInstance()->newDocument(extension.toUpper()) ) {
        IDocument *document = DocumentManager::getInstance()->currentDocument();
        return m_pythonDocumentsStorage.findOrCreatePythonQtObject(document);
    }
    PythonScriptManager::getInstance()->pythonReport("Error on Creation New Document");
    return 0;
}

Document *DocumentManagerPythonModule::openNewDocument( const QString &fileName )
{
    QFile file(fileName);
    if ( file.exists() ) {
        int ret = QMessageBox::question( Core::Storage::mainWindow()->widget(),
                                         Core::Storage::mainWindow()->appName(),
                                         tr("File '%1' Exists! Rewrite it?").arg(file.fileName()) );
        if ( QMessageBox::Yes != ret )
            return 0;
    }
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("File '%1' Cannot be Opened").arg(file.fileName()));
        return 0;
    }
    file.close();

    QString errorMessage;
    if ( IDocument *document = DocumentManager::getInstance()->openDocument(fileName, 0, &errorMessage) )
        return m_pythonDocumentsStorage.findOrCreatePythonQtObject(document);

    PythonScriptManager::getInstance()->pythonReport(tr("Error on Opening Document: %1").arg(errorMessage));
    return 0;
}

Document *DocumentManagerPythonModule::openDocument( const QString &filePath )
{
    QString fPath = filePath;
    if ( fPath.isEmpty() )
        fPath = QFileDialog::getOpenFileName(Core::Storage::mainWindow()->widget());
    if ( fPath.isEmpty() )
        return 0;

    QString errorMessage;
    if ( IDocument *document = DocumentManager::getInstance()->openDocument(fPath, 0, &errorMessage) )
        return m_pythonDocumentsStorage.findOrCreatePythonQtObject(document);

    PythonScriptManager::getInstance()->pythonReport(tr("Error on Opening Document: %1").arg(errorMessage));
    return 0;
}

int DocumentManagerPythonModule::documentsCount()
{
    return DocumentManager::getInstance()->documentsCount();
}

QList<Document *> DocumentManagerPythonModule::documents()
{
    QList<IDocument *> docs = DocumentManager::getInstance()->documentsOrdered();
    QList<Document *> pyDocs;
    foreach ( IDocument *doc, docs )
        pyDocs << m_pythonDocumentsStorage.findOrCreatePythonQtObject(doc);
    return pyDocs;
}

Document *DocumentManagerPythonModule::activeDocument()
{
    if ( IDocument *document = DocumentManager::getInstance()->currentDocument() )
        return m_pythonDocumentsStorage.findOrCreatePythonQtObject(document);

    PythonScriptManager::getInstance()->pythonReport(tr("No Document Opened"));
    return 0;
}

bool DocumentManagerPythonModule::setActiveDocument( Document *document )
{
    if ( IDocument *doc = document->document() )
        return DocumentManager::getInstance()->activateDocument(doc);
    return false;
}

}   // namespace Python
