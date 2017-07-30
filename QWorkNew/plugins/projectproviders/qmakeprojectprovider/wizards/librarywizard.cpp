/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "librarywizard.h"
#include "librarywizarddialog.h"
#include "projectmanager.h"

#include <projectexplorerconstants.h>
#include <qtsupport/qtsupportconstants.h>

#include <QFileInfo>
#include <QTextStream>
#include <QCoreApplication>

static const char sharedHeaderPostfixC[] = "_global";

namespace QmakeProjectManager {
namespace Internal {

LibraryWizard::LibraryWizard()
{
    setId(QLatin1String("H.Qt4Library"));
    setCategory(QLatin1String(ProjectExplorer::Constants::LIBRARIES_WIZARD_CATEGORY));
    setDisplayCategory(QCoreApplication::translate("ProjectExplorer",
        ProjectExplorer::Constants::LIBRARIES_WIZARD_CATEGORY_DISPLAY));
    setDisplayName(tr("C++ Library"));
    setDescription(tr("Creates a C++ library based on qmake. This can be used to create:<ul>"
                "<li>a shared C++ library for use with <tt>QPluginLoader</tt> and runtime (Plugins)</li>"
                "<li>a shared or static C++ library for use with another project at linktime</li></ul>"));
    setIcon(QIcon(QLatin1String(":/wizards/images/lib.png")));
    setRequiredFeatures(Core::Feature(QtSupport::Constants::FEATURE_QT));
}

Core::BaseFileWizard *LibraryWizard::create(QWidget *parent, const Core::WizardDialogParameters &parameters) const
{
    LibraryWizardDialog *dialog = new LibraryWizardDialog(displayName(), icon(),
                                                          showModulesPageForLibraries(), parent,
                                                          parameters);
    dialog->setLowerCaseFiles(QtWizard::lowerCaseFiles());
    dialog->setProjectName(LibraryWizardDialog::uniqueProjectName(parameters.defaultPath()));
    dialog->setSuffixes(headerSuffix(), sourceSuffix(), formSuffix());
    return dialog;
}

static void writeLinuxProFile(QTextStream &str)
{
    str << "\n"
           "unix {\n"
           "    target.path = /usr/lib\n"
           "    INSTALLS += target\n"
           "}\n";
}

Core::GeneratedFiles LibraryWizard::generateFiles(const QWizard *w,
                                                 QString *errorMessage) const
{
    Q_UNUSED(errorMessage)
    const LibraryWizardDialog *dialog = qobject_cast<const LibraryWizardDialog *>(w);
    const QtProjectParameters projectParams = dialog->parameters();
    const QString projectPath = projectParams.projectPath();
    const LibraryParameters params = dialog->libraryParameters();

    const QString sharedLibExportMacro = QtProjectParameters::exportMacro(projectParams.fileName);

    Core::GeneratedFiles rc;
    // Class header + source
    const QString sourceFileName = buildFileName(projectPath, params.sourceFileName, sourceSuffix());
    Core::GeneratedFile source(sourceFileName);
    source.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    const QString headerFileFullName = buildFileName(projectPath, params.headerFileName, headerSuffix());
    const QString headerFileName = QFileInfo(headerFileFullName).fileName();
    QString pluginJsonFileFullName;
    QString pluginJsonFileName;
    if (projectParams.type == QtProjectParameters::Qt4Plugin) {
        pluginJsonFileFullName = buildFileName(projectPath, projectParams.fileName, QLatin1String("json"));
        pluginJsonFileName = QFileInfo(pluginJsonFileFullName).fileName();
    }

    Core::GeneratedFile header(headerFileFullName);

    // Create files: global header for shared libs
    QString globalHeaderFileName;
    if (projectParams.type == QtProjectParameters::SharedLibrary) {
        const QString globalHeaderName = buildFileName(projectPath, projectParams.fileName.toLower() + QLatin1String(sharedHeaderPostfixC), headerSuffix());
        Core::GeneratedFile globalHeader(globalHeaderName);
        globalHeaderFileName = QFileInfo(globalHeader.path()).fileName();
        globalHeader.setContents(ProjectExplorer::ProjectManager::instance()->licenseTemplate(globalHeaderFileName)
                                 + LibraryParameters::generateSharedHeader(globalHeaderFileName, projectParams.fileName, sharedLibExportMacro));
        rc.push_back(globalHeader);
    }

    // Generate code
    QString headerContents, sourceContents;
    params.generateCode(projectParams.type, projectParams.fileName,  headerFileName,
                        globalHeaderFileName, sharedLibExportMacro, pluginJsonFileName,
                        /* indentation*/ 4, &headerContents, &sourceContents);

    source.setContents(ProjectExplorer::ProjectManager::instance()->licenseTemplate(sourceFileName, params.className)
                       + sourceContents);
    header.setContents(ProjectExplorer::ProjectManager::instance()->licenseTemplate(headerFileFullName, params.className)
                       + headerContents);
    rc.push_back(source);
    rc.push_back(header);
    // Create files: profile
    const QString profileName = buildFileName(projectPath, projectParams.fileName, profileSuffix());
    Core::GeneratedFile profile(profileName);
    profile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);
    QString profileContents;
    {
        QTextStream proStr(&profileContents);
        QtProjectParameters::writeProFileHeader(proStr);
        projectParams.writeProFile(proStr);
        proStr << "\nSOURCES += " << QFileInfo(source.path()).fileName()
               << "\n\nHEADERS += " << headerFileName;
        if (!globalHeaderFileName.isEmpty())
            proStr << "\\\n        " << globalHeaderFileName << '\n';
        if (!pluginJsonFileName.isEmpty())
            proStr << "\nDISTFILES += " << pluginJsonFileName << '\n';
        writeLinuxProFile(proStr);
    }
    profile.setContents(profileContents);
    rc.push_back(profile);

    if (!pluginJsonFileName.isEmpty()) {
        Core::GeneratedFile jsonFile(pluginJsonFileFullName);
        jsonFile.setContents(QLatin1String("{\n    \"Keys\" : [ ]\n}\n"));
        rc.push_back(jsonFile);
    }
    return rc;
}

} // namespace Internal
} // namespace QmakeProjectManager
