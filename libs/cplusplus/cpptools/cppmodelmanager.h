/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#ifndef CPPMODELMANAGER_H
#define CPPMODELMANAGER_H

#include "cpptools_global.h"
#include "cpptoolsconstants.h"
#include <ModelManagerInterface.h>
#ifndef ICHECK_BUILD
//#  include <projectexplorer/project.h>
#endif
#include <CppDocument.h>
#include <PreprocessorClient.h>
#ifndef ICHECK_BUILD
#  include <texteditor.h>
#endif
#include <PreprocessorEnvironment.h>
#include <pp-engine.h>
#ifdef ICHECK_BUILD
#  include "parsemanager.h"
#else
#  include <QtCore/QHash>
#  include <QtCore/QFutureInterface>
#  include <QtCore/QFutureSynchronizer>
#  include <QtCore/QMutex>
#  include <QtCore/QTimer>
#  include <QTextEdit> // for QTextEdit::ExtraSelection
#endif

#include "cpptools_global.h"
#include "builtinindexingsupport.h"

#include <QFutureWatcher>

namespace Core {
class IEditor;
}

namespace TextEditor {
class ITextEditor;
class TextEditorWidget;
}

namespace CPlusPlus {
    class ParseManager;
}

namespace CppTools {

namespace Internal {

class CppEditorSupport;
class CppPreprocessor;
class CppFindReferences;

#ifndef ICHECK_BUILD
class CPPTOOLS_EXPORT CppModelManager : public CPlusPlus::CppModelManagerInterface
{
    Q_OBJECT

public:
    CppModelManager(QObject *parent);
    virtual ~CppModelManager();

    static CppModelManager *instance();
    static QString configurationFileName();

    virtual QFuture<void> updateSourceFiles(const QStringList &sourceFiles);
    virtual WorkingCopy workingCopy() const;

    virtual QList<ProjectInfo> projectInfos() const;
    virtual ProjectInfo projectInfo(ProjectExplorer::Project *project) const;
    virtual QFuture<void> updateProjectInfo(const ProjectInfo &pinfo);

    virtual CPlusPlus::Snapshot snapshot() const;
    virtual void GC();

    QFuture<void> refreshSourceFiles(const QStringList &sourceFiles);

    virtual bool isEditorSupported(Core::IEditor *editor) const;

    CppEditorSupport *editorSupport(TextEditor::ITextEditor *editor) const
    { return m_editorSupport.value(editor); }

    void emitDocumentUpdated(CPlusPlus::Document::Ptr doc);

    void stopEditorSelectionsUpdate()
    { m_updateEditorSelectionsTimer->stop(); }

//    virtual void addEditorSupport(AbstractEditorSupport *editorSupport);
//    virtual void removeEditorSupport(AbstractEditorSupport *editorSupport);
    /// TODO: !
    virtual void addExtraEditorSupport(CodeModel::AbstractEditorSupport * /*editorSupport*/) {}
    virtual void removeExtraEditorSupport(CodeModel::AbstractEditorSupport * /*editorSupport*/) {}

    virtual QList<int> references(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context);

    virtual void renameUsages(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context,
                              const QString &replacement = QString());
    virtual void findUsages(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context);

    virtual void findMacroUsages(const CPlusPlus::Macro &macro);

    virtual void setExtraDiagnostics(const QString &fileName, int key,
                                     const QList<Utils::DiagnosticMessage> &diagnostics);
    virtual QList<Utils::DiagnosticMessage> extraDiagnostics(
            const QString &fileName, int key = AllExtraDiagnostics) const;

    virtual const CppHelperItem *cppHelper(const QString &key, ITextDocument *textDocument, const QString &filePath);
    virtual void finishCppHelper(const CppHelperItem *item, CPlusPlus::Document::Ptr doc, QTextDocument *textDocument);
    virtual CppTools::CppIndexingSupport *indexingSupport();

    void finishedRefreshingSourceFiles(const QStringList &files);
    QString precompiledHeader() const;

    QStringList projectFiles()
    {
        ensureUpdated();
        return m_projectFiles;
    }

    ProjectPart::HeaderPaths headerPaths()
    {
        ensureUpdated();
        return m_includePaths;
    }

    ProjectPart::HeaderPaths frameworkPaths()
    {
        ensureUpdated();
        return m_frameworkPaths;
    }

    ProjectPart::HeaderPaths allHeaderPaths()
    {
        ensureUpdated();
        return m_includePaths + m_frameworkPaths;
    }

    void reinitializeExtensions();
    virtual QList<ProjectPart::Ptr> projectPart(const QString &fileName) const;

Q_SIGNALS:
    void projectPathChanged(const QString &projectPath);

    void aboutToRemoveFiles(const QStringList &files);

public Q_SLOTS:
    void editorOpened(Core::IEditor *editor);
    void editorClosed(Core::IEditor *editor);
    virtual void updateModifiedSourceFiles();
    virtual void forceUpdateEditor( TextEditor::ITextEditor *editor );
    void onProjectAdded(ProjectExplorer::Project *);
    void onAboutToRemoveProject(ProjectExplorer::Project *project);

private Q_SLOTS:
    // this should be executed in the GUI thread.
    void onDocumentUpdated(CPlusPlus::Document::Ptr doc);
    void onExtraDiagnosticsUpdated(const QString &fileName);
    void postEditorUpdate();
    void updateEditorSelections();
    void updateAllDocuments();

    void projectOpened(QVariantList &);
    void projectClosed( QVariantList &valuesList );
    void projectChanged( QVariantList &valuesList );

    void onAddFilesToProject(QStringList);
    void onRemoveFilesFromProject(QStringList);

private:
    void updateEditor(CPlusPlus::Document::Ptr doc);

    WorkingCopy buildWorkingCopyList();

    QByteArray definedMacros()
    {
        ensureUpdated();
        return m_definedMacros;
    }

    void ensureUpdated();
    QStringList internalProjectFiles() const;
    ProjectPart::HeaderPaths internaHeaderPaths() const;
    ProjectPart::HeaderPaths internalFrameworkPaths() const;
    QByteArray internalDefinedMacros() const;
    QString internalPrecompiledHeader() const;

    QString bestIncludeCandidate( const QString &fileName, const QString &fullIncludePath );

private:
    CPlusPlus::Snapshot m_snapshot;

    // cache
    bool m_dirty;
    QStringList m_projectFiles;
    ProjectPart::HeaderPaths m_includePaths;
    ProjectPart::HeaderPaths m_frameworkPaths;
    QByteArray m_definedMacros;

    void recalculateFileToProjectParts();
    QMap<QString, CppTools::ProjectPart::Ptr> m_projectFileToProjectPart;
    QMap<QString, QList<CppTools::ProjectPart::Ptr> > m_fileToProjectParts;

    QString m_precompiledHeader;

    const QStringList *m_headerExtensions;
    const QStringList *m_sourceExtensions;

    // editor integration
    QMap<TextEditor::ITextEditor *, CppEditorSupport *> m_editorSupport;

    // project integration
    QMap<ProjectExplorer::Project *, ProjectInfo> m_projects;

    mutable QMutex mutex;
    mutable QMutex protectSnapshot;

    struct Editor {
        Editor()
                : revision(-1) {}
        int revision;
        QPointer<TextEditor::ITextEditor> textEditor;
        QList<QTextEdit::ExtraSelection> selections;
        QList<TextEditor::TextEditorWidget::BlockRange> ifdefedOutBlocks;
    };

    QList<Editor> m_todo;

    QTimer *m_updateEditorSelectionsTimer;

    QFutureSynchronizer<void> m_synchronizer;
    unsigned m_revision;

    CppFindReferences *m_findReferences;
    CppIndexingSupport *m_internalIndexingSupport;
    bool m_indexerEnabled;

    mutable QMutex protectExtraDiagnostics;
    QHash<QString, QHash<int, QList<Utils::DiagnosticMessage> > > m_extraDiagnostics;
    QFuture<void> m_codeModelFuture;

    QFutureWatcher<void> m_updatedProjectsWatcher;

    static CppModelManager *g_instance;
};
#endif

class CPPTOOLS_EXPORT CppPreprocessor: public CPlusPlus::Client
{
public:
#ifndef ICHECK_BUILD
    CppPreprocessor(QPointer<CppModelManager> modelManager);
#else
    CppPreprocessor(QPointer<CPlusPlus::ParseManager> modelManager);
#endif
    virtual ~CppPreprocessor();

    static QString cleanPath(const QString &path);

    void setRevision(unsigned revision);
    void setWorkingCopy(const CPlusPlus::CppModelManagerInterface::WorkingCopy &workingCopy);
    void setHeaderPaths(const ProjectPart::HeaderPaths &headerPaths);
    void setFrameworkPaths(const QStringList &frameworkPaths);
    void addFrameworkPath(const QString &frameworkPath);
    void setProjectFiles(const QStringList &files);
    void setTodo(const QStringList &files);

    void run(const QString &fileName);

    void resetEnvironment();

    const QSet<QString> &todo() const
    { return m_todo; }

    CppModelManager *modelManager() const
    { return m_modelManager.data(); }

public: // attributes
    CPlusPlus::Snapshot snapshot;

protected:
    CPlusPlus::Document::Ptr switchDocument(CPlusPlus::Document::Ptr doc);

    bool includeFile(const QString &absoluteFilePath, QString *result, unsigned *revision);
    QString tryIncludeFile(QString &fileName, IncludeType type, unsigned *revision);
    QString tryIncludeFile_helper(QString &fileName, IncludeType type, unsigned *revision);

    void mergeEnvironment(CPlusPlus::Document::Ptr doc);

    virtual void macroAdded(const CPlusPlus::Macro &macro);
    virtual void passedMacroDefinitionCheck(unsigned offset, const CPlusPlus::Macro &macro);
    virtual void failedMacroDefinitionCheck(unsigned offset, const QByteArray &name);
    virtual void startExpandingMacro(unsigned offset,
                                     const CPlusPlus::Macro &macro,
                                     const QByteArray &originalText,
                                     bool inCondition,
                                     const QVector<CPlusPlus::MacroArgumentReference> &actuals);
    virtual void stopExpandingMacro(unsigned offset, const CPlusPlus::Macro &macro);
    virtual void startSkippingBlocks(unsigned offset);
    virtual void stopSkippingBlocks(unsigned offset);
    virtual void sourceNeeded(QString &fileName, IncludeType type,
                              unsigned line);

private:
#ifndef ICHECK_BUILD
    QPointer<CppModelManager> m_modelManager;
#endif
    CPlusPlus::Environment env;
    CPlusPlus::Preprocessor preprocess;
    QStringList m_headerPaths;      // TODO: -> ProjectPart::HeaderPaths m_headerPaths
    QStringList m_systemIncludePaths;
    CPlusPlus::CppModelManagerInterface::WorkingCopy m_workingCopy;
    QStringList m_projectFiles;
    QStringList m_frameworkPaths;
    QSet<QString> m_included;
    CPlusPlus::Document::Ptr m_currentDoc;
    QSet<QString> m_todo;
    QSet<QString> m_processed;
    unsigned m_revision;
    QHash<QString, QString> m_fileNameCache;
};

} // namespace Internal
} // namespace CppTools

#endif // CPPMODELMANAGER_H
