#ifndef DOCUMENTMANAGERPYTHONMODULE_H
#define DOCUMENTMANAGERPYTHONMODULE_H

#include "PythonScriptManager.h"
#include "PythonQtObjectStorage.h"

namespace Python {

class DocumentManagerPythonModule;

class Document: public Core::PythonQtObject
{
    Q_OBJECT

    friend class DocumentManagerPythonModule;

public:
    Document( Core::PythonQtStorageDestroyDelegate *delegate, IDocument *document );
    virtual ~Document();

    IDocument *document() const;

public slots:
    bool isModified();
    bool isNew();
    bool insertText( const QString &text );
    QString selectedText();
    QString text();
    QString filePath();
    bool save();

private:
    IDocument *m_document;
};

class DocumentManagerPythonModule: public IPythonModule
{
    Q_OBJECT

public:
    DocumentManagerPythonModule();
    virtual ~DocumentManagerPythonModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

public slots:
    Document *newDocument( const QString &extension );
    Document *openNewDocument( const QString &fileName );
    Document *openDocument( const QString &filePath = "" );
    int  documentsCount();

    QList<Document *> documents();
    Document *activeDocument();
    bool setActiveDocument( Document *document );

private:
    QList<PythonModuleFunction *> m_functions;
    Core::PythonQtObjectStorage<IDocument, Document> m_pythonDocumentsStorage;
};

}   // namespace Python

#endif // DOCUMENTMANAGERPYTHONMODULE_H
