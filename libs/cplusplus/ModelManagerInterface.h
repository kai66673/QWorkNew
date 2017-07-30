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

#ifndef CPPMODELMANAGERINTERFACE_H
#define CPPMODELMANAGERINTERFACE_H

#include <CppDocument.h>
#include <languageutils/fakemetaobject.h>
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtCore/QFuture>

#include "stringtable.h"
#include "cppindexingsupport.h"
#include "ITextDocument.h"
#include "ICodeModel.h"
#include "CppHelperItem.h"
#include "cppprojects.h"

namespace Core {
    class IEditor;
}

namespace CPlusPlus {
    class LookupContext;
}

namespace ProjectExplorer {
    class Project;
}

namespace TextEditor {
    class ITextEditor;
}

namespace CppTools {
    class CppIndexingSupport;
    class ProjectInfo;
}

namespace CPlusPlus {

class CPLUSPLUS_EXPORT CppModelManagerInterface : public ICodeModel
{
    Q_OBJECT

public:
    class WorkingCopy
    {
    public:
        void insert(const QString &fileName, const QString &source, unsigned revision = 0)
        { _elements.insert(fileName, qMakePair(source, revision)); }

        bool contains(const QString &fileName) const
        { return _elements.contains(fileName); }

        QString source(const QString &fileName) const
        { return _elements.value(fileName).first; }

        QPair<QString, unsigned> get(const QString &fileName) const
        { return _elements.value(fileName); }

    private:
        typedef QHash<QString, QPair<QString, unsigned> > Table;
        Table _elements;
    };

    enum ExtraDiagnosticKind
    {
        AllExtraDiagnostics = -1,
        ExportedQmlTypesDiagnostic
    };

public:
    CppModelManagerInterface(QObject *parent = 0);
    virtual ~CppModelManagerInterface();

    static CppModelManagerInterface *instance();

    virtual bool isEditorSupported(Core::IEditor *editor) const = 0;

    virtual WorkingCopy workingCopy() const = 0;
    virtual CPlusPlus::Snapshot snapshot() const = 0;

    virtual QList<CppTools::ProjectInfo> projectInfos() const = 0;
    virtual CppTools::ProjectInfo projectInfo(ProjectExplorer::Project *project) const = 0;
    virtual QFuture<void> updateProjectInfo(const CppTools::ProjectInfo &pinfo) = 0;

    virtual QList<int> references(CPlusPlus::Symbol *symbol,
                                  const CPlusPlus::LookupContext &context) = 0;

    virtual void renameUsages(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context,
                              const QString &replacement = QString()) = 0;
    virtual void findUsages(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context) = 0;

    virtual void findMacroUsages(const CPlusPlus::Macro &macro) = 0;

    virtual void setExtraDiagnostics(const QString &fileName, int key,
                                     const QList<Utils::DiagnosticMessage> &diagnostics) = 0;
    virtual QList<Utils::DiagnosticMessage> extraDiagnostics(
            const QString &fileName, int key = AllExtraDiagnostics) const = 0;

    virtual const CppTools::CppHelperItem *cppHelper(const QString &key, ITextDocument *textDocument, const QString &filePath) = 0;
    virtual void finishCppHelper(const CppTools::CppHelperItem *item, CPlusPlus::Document::Ptr doc, QTextDocument *textDocument) = 0;
    virtual CppTools::CppIndexingSupport *indexingSupport() = 0;

    CppTools::Internal::StringTable &stringTable();

    QList<QAction *> advancedCompletitionActions() const;
    void addAdvancedCompletitionAction(QAction *action);

Q_SIGNALS:
    void documentUpdated(CPlusPlus::Document::Ptr doc);
    void sourceFilesRefreshed(const QStringList &files);
    void extraDiagnosticsUpdated(QString fileName);

public Q_SLOTS:
    virtual void updateModifiedSourceFiles() = 0;
    virtual QFuture<void> updateSourceFiles(const QStringList &sourceFiles) = 0;
    virtual void GC() = 0;
    virtual void forceUpdateEditor( TextEditor::ITextEditor *editor ) = 0;

private:
    CppTools::Internal::StringTable m_stringTable;
    QList<QAction *> m_advancedCompletitionActions;
};

} // namespace CPlusPlus

#endif // CPPMODELMANAGERINTERFACE_H
