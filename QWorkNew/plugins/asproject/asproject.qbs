import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "asproject"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "plugins"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "xml", "sql" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'settings' }
    Depends { name: 'core' }
    Depends { name: 'qtcutils' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["ASPROJECT_LIBRARY", "HAVE_QT5"] : ["ASPROJECT_LIBRARY"]

    property var pythonQtInclude:  Qt.core.version.charAt(0) == '5' ? '../../../PyInclude/PythonQt5' : '../../../PyInclude/PythonQt4'

    cpp.includePaths: [
        '.',
        '../../libs',
        '../../libs/qworkbase',
        '../../libs/qworkbase/scripting',
        '../../libs/corelib',
        '../../libs/settings',
        '../../../PyInclude/Python', pythonQtInclude
    ]
     
    files: [
        "QAsofProjectPlugin.h",
        "QProjectData.h",
        "NewProjectDialog.h",
        "QProjectTreeView.h",
        "NewFolderDialog.h",
        "ProjectBuildActionsModel.h",
        "ConfigureProjectBuildActionsDialog.h",
        "AddEditProjectBuildActionDialog.h",
        "ProjectBuildActionsManager.h",
        "ProjectBuildEnvironmentDialog.h",
        "ProjectTypeDialog.h",
        "IASofProjectImporter.h",
        "asproject_global.h",
        "GenericProjectImporter.h",
        "AsofProjectImportManager.h",
        "SelectProjectImporterDialog.h",
        "GenericProjectLocationDialog.h",
        "AsofProjectPythonModule.h",
        "QAsofProjectPlugin.cpp",
        "QProjectData.cpp",
        "NewProjectDialog.cpp",
        "QProjectTreeView.cpp",
        "NewFolderDialog.cpp",
        "ProjectBuildActionsModel.cpp",
        "ConfigureProjectBuildActionsDialog.cpp",
        "AddEditProjectBuildActionDialog.cpp",
        "ProjectBuildActionsManager.cpp",
        "ProjectBuildEnvironmentDialog.cpp",
        "ProjectTypeDialog.cpp",
        "GenericProjectImporter.cpp",
        "AsofProjectImportManager.cpp",
        "SelectProjectImporterDialog.cpp",
        "GenericProjectLocationDialog.cpp",
        "AsofProjectPythonModule.cpp",
        "asproject.qrc",
        "NewProjectDialog.ui",
        "NewFolderDialog.ui",
        "ConfigureProjectBuildActionsDialog.ui",
        "AddEditProjectBuildActionDialog.ui",
        "ProjectBuildEnvironmentDialog.ui",
        "ProjectTypeDialog.ui",
        "SelectProjectImporterDialog.ui",
        "GenericProjectLocationDialog.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/plugins" : "bin_release/plugins"
    }
}
