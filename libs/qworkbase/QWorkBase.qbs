import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "QWorkBase"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends {
        name : "Qt"
        submodules: [
            "core",
            "widgets",
            "printsupport",
            "xml",
            "network"
        ]
    }
    Depends {
        condition: Qt.core.version.charAt(0) == '5'
        name: "Qt"
        submodules: ["concurrent"]
    }
    
    Depends { name: "settings" }
    Depends { name: "core" }
    Depends { name: "qtcutils" }
    Depends { name: "find" }
    Depends { name: "aggregate" }
    Depends { name: "texteditor" }
    Depends { name: "pythoneditorlib" }
    
    property var commonDefines: ["QWORKBASE_LIBRARY"]
    Properties {
        condition: Qt.core.version.charAt(0) == '5'
        cpp.defines: commonDefines.concat(["HAVE_QT5"])
    }
    cpp.defines: commonDefines

    property var pythonQtInclude:  Qt.core.version.charAt(0) == '5' ? '../../../PyInclude/PythonQt5' : '../../../PyInclude/PythonQt4'

    cpp.includePaths: [ '.',
                        '..',
                        '../settings',
                        '../corelib',
                        '../utils',
                        '../texteditorlib',
                        'texteditor',
                        '../find',
                        'texteditor/bookmarks',
                        'texteditor/generichighlighter',
                        'progressmanager',
                        'scripting',
                        '../../../PyInclude/Python', pythonQtInclude ]
     
    property var commonLibs: qbs.targetOS.contains("windows") ? ["ole32", "user32"] : []
    Properties {
        condition: qbs.enableDebugCode
        cpp.dynamicLibraries: commonLibs.concat(["python27", "PythonQtd", "PythonQt_QtAlld"])
    }
    cpp.dynamicLibraries: commonLibs.concat(["python27", "PythonQt", "PythonQt_QtAll"])

    cpp.frameworks: qbs.targetOS.contains("osx") ? ["AppKit"] : undefined
    
    files: [
        "QWorkBase.cpp",
        "ListSelectorDialog.cpp",
        "TextPositionStatusBarWidget.cpp",
        "GotoLineDialog.cpp",
        "NavigationManager.cpp",
        "progressmanager/progressmanager.cpp",
        "TasksStatusWidget.cpp",
        "texteditor/bookmarks/BookmarkPlugin.cpp",
        "texteditor/bookmarks/bookmarkmanager.cpp",
        "texteditor/bookmarks/bookmark.cpp",
        "texteditor/basetextmark.cpp",
        "scripting/QwGuiPythonModule.cpp",
        "scripting/PythonScriptManager.cpp",
        "scripting/ConfirmRemoveScriptDialog.cpp",
        "scripting/AddPythonScriptDialog.cpp",
        "DocumentManagerPythonModule.cpp",
        "QWorkBase.h",
        "QWorkBase_global.h",
        "ListSelectorDialog.h",
        "TextPositionStatusBarWidget.h",
        "GotoLineDialog.h",
        "NavigationManager.h",
        "progressmanager/progressmanager_p.h",
        "TasksStatusWidget.h",
        "texteditor/bookmarks/BookmarkPlugin.h",
        "texteditor/bookmarks/bookmarkmanager.h",
        "texteditor/bookmarks/bookmark.h",
        "texteditor/basetextmark.h",
        "scripting/QwGuiPythonModule.h",
        "scripting/PythonScriptManager.h",
        "scripting/ConfirmRemoveScriptDialog.h",
        "scripting/AddPythonScriptDialog.h",
        "DocumentManagerPythonModule.h",
        "ListSelectorDialog.ui",
        "TextPositionStatusBarWidget.ui",
        "GotoLineDialog.ui",
        "scripting/ConfirmRemoveScriptDialog.ui",
        "scripting/AddPythonScriptDialog.ui",
        "QWorkBase.qrc",
        "scripting/initPythonCodeAssist.py"

    ]
    
    Group {
        name: "ProgressManager_win"
        condition: qbs.targetOS.contains("windows")
        files: [
            "progressmanager/progressmanager_win.cpp",
        ]
    }

    Group {
        name: "ProgressManager_mac"
        condition: qbs.targetOS.contains("osx")
        files: [
            "macfullscreen.h",
            "macfullscreen.mm",
            "progressmanager/progressmanager_mac.mm",
        ]
    }

    Group {
        name: "ProgressManager_x11"
        condition: qbs.targetOS.contains("unix") && !qbs.targetOS.contains("osx")
        files: [
            "progressmanager/progressmanager_x11.cpp",
        ]
    }
    
    Group {
        name: "GenericHighlighter_Config_Xml"
        files: [
            "../texteditorlib/generichighlighter/config_xml/alert.xml",
            "../texteditorlib/generichighlighter/config_xml/bash.xml",
            "../texteditorlib/generichighlighter/config_xml/cmake.xml",
            "../texteditorlib/generichighlighter/config_xml/cpp.xml",
            "../texteditorlib/generichighlighter/config_xml/css.xml",
            "../texteditorlib/generichighlighter/config_xml/doxygen.xml",
            "../texteditorlib/generichighlighter/config_xml/dtd.xml",
            "../texteditorlib/generichighlighter/config_xml/html.xml",
            "../texteditorlib/generichighlighter/config_xml/ini.xml",
            "../texteditorlib/generichighlighter/config_xml/java.xml",
            "../texteditorlib/generichighlighter/config_xml/javadoc.xml",
            "../texteditorlib/generichighlighter/config_xml/perl.xml",
            "../texteditorlib/generichighlighter/config_xml/ruby.xml",
            "../texteditorlib/generichighlighter/config_xml/valgrind-suppression.xml",
            "../texteditorlib/generichighlighter/config_xml/xml.xml",
            "../texteditorlib/generichighlighter/config_xml/yacc.xml"
        ]
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/generic-highlighter" : "bin_release/generic-highlighter"
    }
    
    Group {
        name: "QWorkBase_Dll"
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
