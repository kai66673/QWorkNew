import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "cppeditor"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "editors"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "printsupport" ] }
    Depends {
        condition: Qt.core.version.charAt(0) == '5'
        name: "Qt"
        submodules: ["concurrent"]
    }

    Depends { name: 'QWorkBase' }
    Depends { name: 'settings' }
    Depends { name: 'cplusplus' }
    Depends { name: 'core' }
    Depends { name: 'qtcutils' }
    Depends { name: 'texteditor' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["CPPEDITOR_LIBRARY", "CPPTOOLS_LIBRARY", "HAVE_QT5"]
                                                  : ["CPPEDITOR_LIBRARY", "CPPTOOLS_LIBRARY"]

    property var pythonQtInclude:  Qt.core.version.charAt(0) == '5' ? '../../../PyInclude/PythonQt5' : '../../../PyInclude/PythonQt4'

    cpp.includePaths: [
        '.',
        'cpptools',
        '../../libs',
        '../../libs/qworkbase',
        '../../libs/qworkbase/scripting',
        '../../libs/utils',
        '../../libs/corelib',
        '../../libs/settings',
        '../../libs/cplusplus',
        '../../libs/cplusplus/shared',
        '../../libs/texteditorlib',
        '../../libs/texteditorlib/codeassist',
        '../../../PyInclude/Python', pythonQtInclude
    ]
     
    files: [
        "cppeditor.h",
        "cpphighlighter.h",
        "cpphoverhandler.h",
        "cppeditorconstants.h",
        "cppeditorenums.h",
        "cppeditor_global.h",
        "cppquickfix.h",
        "cppchecksymbols.h",
        "cppsemanticinfo.h",
        "cppoutline.h",
        "cppinsertdecldef.h",
        "cpplocalsymbols.h",
        "cppelementevaluator.h",
        "cppautocompleter.h",
        "cppcompleteswitch.h",
        "cppinsertqtpropertymembers.h",
        "CppEditorPlugin.h",
        "CppEditorSettingsWidget.h",
        "CppTextFormatModel.h",
        "CppSemanticTextFormatModel.h",
        "CppReplaceUsagesDialog.h",
        "cpptools/searchsymbols.h",
        "cpptools/insertionpointlocator.h",
        "cpptools/cpptoolssettings.h",
        "cpptools/cpptoolsreuse.h",
        "cpptools/cpptoolseditorsupport.h",
        "cpptools/cpptoolsconstants.h",
        "cpptools/cpptools_global.h",
        "cpptools/cpprefactoringchanges.h",
        "cpptools/cppqtstyleindenter.h",
        "cpptools/cppmodelmanager.h",
        "cpptools/cppfindreferences.h",
        "cpptools/cppdoxygen.h",
        "cpptools/cppcompletionassist.h",
        "cpptools/cppcodestylesettings.h",
        "cpptools/cppcodestylepreferences.h",
        "cpptools/cppcodeformatter.h",
        "cppquickfixassistant.h",
        "cpptools/cppcodestylepreferencesfactory.h",
        "cppeditor.cpp",
        "cpphighlighter.cpp",
        "cpphoverhandler.cpp",
        "cppquickfix.cpp",
        "cppquickfixes.cpp",
        "cppchecksymbols.cpp",
        "cppsemanticinfo.cpp",
        "cppoutline.cpp",
        "cppinsertdecldef.cpp",
        "cpplocalsymbols.cpp",
        "cppelementevaluator.cpp",
        "cppautocompleter.cpp",
        "cppcompleteswitch.cpp",
        "cppinsertqtpropertymembers.cpp",
        "CppEditorPlugin.cpp",
        "CppTextFormatModel.cpp",
        "CppEditorSettingsWidget.cpp",
        "CppSemanticTextFormatModel.cpp",
        "CppReplaceUsagesDialog.cpp",
        "cpptools/searchsymbols.cpp",
        "cpptools/insertionpointlocator.cpp",
        "cpptools/cpptoolssettings.cpp",
        "cpptools/cpptoolsreuse.cpp",
        "cpptools/cpptoolseditorsupport.cpp",
        "cpptools/cpprefactoringchanges.cpp",
        "cpptools/cppqtstyleindenter.cpp",
        "cpptools/cppmodelmanager.cpp",
        "cpptools/cppfindreferences.cpp",
        "cpptools/cppdoxygen.cpp",
        "cpptools/cppcompletionassist.cpp",
        "cpptools/cppcodestylesettings.cpp",
        "cpptools/cppcodestylepreferences.cpp",
        "cpptools/cppcodeformatter.cpp",
        "cppquickfixassistant.cpp",
        "cpptools/cppcodestylepreferencesfactory.cpp",
        "cppeditor.qrc",
        "CppEditor.mimetypes.xml",
        "CppEditorSettingsWidget.ui",
        "CppReplaceUsagesDialog.ui",
        "cpptools/completionsettingspage.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/editors" : "bin_release/editors"
    }
}
