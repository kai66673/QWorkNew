import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "core"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "network", "xml" ] }
    Depends { name: "settings" }
    Depends { name: "qtcutils" }
    
    property var commonDefines: ["CORELIB_LIBRARY"]
    Properties {
        condition: Qt.core.version.charAt(0) == '5'
        cpp.defines: commonDefines.concat(["HAVE_QT5"])
    }
    cpp.defines: commonDefines
    
    cpp.includePaths: [ '.', '..', '../utils', '../settings']
     
    files: [
        "textfile.h",
        "styleanimator.h",
        "mimedatabase.h",
        "manhattanstyle.h",
        "ioutputpane.h",
        "infobar.h",
        "ifile.h",
        "ieditor.h",
        "icore.h",
        "icontext.h",
        "coreconstants.h",
        "uniqueidmanager.h",
        "corelib_global.h",
        "IDocumentManager.h",
        "progressview.h",
        "progressmanager.h",
        "progressbar.h",
        "futureprogress.h",
        "iassistprovider.h",
        "completionassistprovider.h",
        "ifindsupport.h",
        "textfindconstants.h",
        "SessionsDialog.h",
        "SessionManager.h",
        "ISessionClient.h",
        "IMainWindow.h",
        "QwDockWidget.h",
        "LogManager.h",
        "LogFilterDialog.h",
        "CommandManager.h",
        "CommandShortcutsDialog.h",
        "CommandsTreeModel.h",
        "CloseApplicationHandler.h",
        "IPlugin.h",
        "IDocument.h",
        "ISettingsFactory.h",
        "OpenEditorsWindow.h",
        "ITextDocument.h",
        "Core.h",
        "ILink.h",
        "IPythonModule.h",
        "itexteditor.h",
        "uniqueidmanager.cpp",
        "textfile.cpp",
        "styleanimator.cpp",
        "mimedatabase.cpp",
        "manhattanstyle.cpp",
        "infobar.cpp",
        "ifile.cpp",
        "ieditor.cpp",
        "IDocumentManager.cpp",
        "progressview.cpp",
        "progressbar.cpp",
        "futureprogress.cpp",
        "completionassistprovider.cpp",
        "iassistprovider.cpp",
        "ifindsupport.cpp",
        "SessionsDialog.cpp",
        "SessionManager.cpp",
        "ISessionClient.cpp",
        "QwDockWidget.cpp",
        "LogManager.cpp",
        "LogFilterDialog.cpp",
        "CommandManager.cpp",
        "CommandShortcutsDialog.cpp",
        "CommandsTreeModel.cpp",
        "CloseApplicationHandler.cpp",
        "IDocument.cpp",
        "ISettingsFactory.cpp",
        "OpenEditorsWindow.cpp",
        "ITextDocument.cpp",
        "Core.cpp",
        "ILink.cpp",
        "IPythonModule.cpp",
        "itexteditor.cpp",
        "SessionsDialog.ui",
        "LogFilterDialog.ui",
        "CommandShortcutsDialog.ui",
        "corelib.qrc"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
