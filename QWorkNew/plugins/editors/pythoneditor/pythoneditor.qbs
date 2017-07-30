import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "pythoneditor"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "editors"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "printsupport" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'settings' }
    Depends { name: 'core' }
    Depends { name: 'qtcutils' }
    Depends { name: 'texteditor' }
    Depends { name: 'pythoneditorlib' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []

    property var pythonQtInclude:  Qt.core.version.charAt(0) == '5' ? '../../../PyInclude/PythonQt5' : '../../../PyInclude/PythonQt4'

    cpp.includePaths: [ '.',
                        '../../libs',
                        '../../libs/qworkbase',
                        '../../libs/qworkbase/scripting',
                        '../../libs/utils',
                        '../../libs/corelib',
                        '../../libs/settings',
                        '../../libs/texteditorlib',
                        '../../libs/pythoneditorlib',
                        '../../libs/texteditorlib/codeassist',
                        '../../../PyInclude/Python', pythonQtInclude ]
     
    files: [
        "PythonEditorPlugin.h",
        "PythonEditorSettingsWidget.h",
        "PythonTextFormatModel.h",
        "PythonEditorPlugin.cpp",
        "PythonEditorSettingsWidget.cpp",
        "PythonTextFormatModel.cpp",
        "PythonCompletiotionAssist.cpp",
        "PythonCompletiotionAssist.h",
        "PythonEditorSettingsWidget.ui",
        "pythoneditor.qrc"
    ]

    Group {
        name: "PythonEditorPlugin_Rope"
        files: "../../../rope/"
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/python/Lib/rope" : "bin_release/python/Lib/rope"
    }

    Group {
        name: "PythonEditorPlugin_Dll"
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/editors" : "bin_release/editors"
    }
}
