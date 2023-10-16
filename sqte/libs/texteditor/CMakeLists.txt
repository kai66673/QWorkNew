cmake_minimum_required(VERSION 3.5)

project(texteditor VERSION 0.1 LANGUAGES CXX)

set (TEXTEDITOR_HEADERS
    texteditor_global.h
    basetexteditor.h
    texteditor.h
    texteditoroverlay.h
    refactoroverlay.h
    basetextdocument.h
    basetexteditor_p.h
    texteditorsettings.h
    behaviorsettings.h
    displaysettings.h
    fontsettings.h
    completionsettings.h
    typingsettings.h
    storagesettings.h
    extraencodingsettings.h
    icodestylepreferences.h
    icodestylepreferencesfactory.h
    colorscheme.h
    basetextdocumentlayout.h
    texteditorconstants.h
    indenter.h
    tabsettings.h
    syntaxhighlighter.h
    highlighterformatsmanager.h
    highlighterformatsconfigurewidget.h
    codestylepool.h
    codecselector.h
    convenience.h
    autocompleter.h

    core/corelib_global.h
    core/coreconstants.h
    core/itexteditor.h
    core/ifile.h
    core/textfile.h
    core/infobar.h
    core/ifindsupport.h
    core/manhattanstyle.h
    core/styleanimator.h

    codeassist/codeassistant.h
    codeassist/runner.h
    codeassist/iassistinterface.h
    codeassist/iassistprovider.h
    codeassist/completionassistprovider.h
    codeassist/defaultassistinterface.h

    snippets/isnippetprovider.h
    snippets/plaintextsnippetprovider.h
    snippets/reuse.h
    snippets/snippet.h
    snippets/plaintextsnippetprovider.h
    snippets/snippeteditor.h
    snippets/snippetscollection.h
    snippets/snippetssettings.h

    tooltip/tooltip.h
    tooltip/tipcontents.h
    tooltip/tipfactory.h
    tooltip/tips.h

    utils/utils_global.h
    utils/settingsutils.h
    utils/qtcfileutils.h
    utils/qtcassert.h
    utils/savefile.h
    utils/algorithm.h
    utils/hostosinfo.h
    utils/osspecificaspects.h
    utils/consoleprocess.h
    utils/consoleprocess_p.h
    utils/qtcprocess.h
    utils/environment.h
    utils/unixutils.h
    utils/stringutils.h
    utils/uncommentselection.h
    utils/persistentsettings.h
    utils/textfileformat.h
    utils/changeset.h
    utils/stylehelper.h
)

set (TEXTEDITOR_SOURCES
    texteditor.cpp
    texteditoroverlay.cpp
    refactoroverlay.cpp
    texteditorsettings.cpp
    behaviorsettings.cpp
    displaysettings.cpp
    fontsettings.cpp
    completionsettings.cpp
    typingsettings.cpp
    storagesettings.cpp
    extraencodingsettings.cpp
    icodestylepreferences.cpp
    icodestylepreferencesfactory.cpp
    colorscheme.cpp
    basetextdocumentlayout.cpp
    basetextdocument.cpp
    indenter.cpp
    tabsettings.cpp
    syntaxhighlighter.cpp
    highlighterformatsmanager.cpp
    highlighterformatsconfigurewidget.cpp
    codestylepool.cpp
    codecselector.cpp
    convenience.cpp
    autocompleter.cpp

    core/itexteditor.cpp
    core/ifile.cpp
    core/textfile.cpp
    core/infobar.cpp
    core/ifindsupport.cpp
    core/manhattanstyle.cpp
    core/styleanimator.cpp

    codeassist/codeassistant.cpp
    codeassist/runner.cpp
    codeassist/iassistinterface.cpp
    codeassist/iassistprovider.cpp
    codeassist/completionassistprovider.cpp
    codeassist/defaultassistinterface.cpp

    snippets/isnippetprovider.cpp
    snippets/plaintextsnippetprovider.cpp
    snippets/snippet.cpp
    snippets/plaintextsnippetprovider.cpp
    snippets/snippeteditor.cpp
    snippets/snippetscollection.cpp
    snippets/snippetssettings.cpp

    tooltip/tooltip.cpp
    tooltip/tipcontents.cpp
    tooltip/tipfactory.cpp
    tooltip/tips.cpp

    utils/qtcfileutils.cpp
    utils/qtcassert.cpp
    utils/savefile.cpp
    utils/hostosinfo.cpp
    utils/consoleprocess.cpp
    utils/consoleprocess_unix.cpp
    utils/qtcprocess.cpp
    utils/environment.cpp
    utils/unixutils.cpp
    utils/stringutils.cpp
    utils/uncommentselection.cpp
    utils/persistentsettings.cpp
    utils/textfileformat.cpp
    utils/changeset.cpp
    utils/stylehelper.cpp
)

set (TEXTEDITOR_FORMS
    highlighterformatsconfigurewidget.ui
)

add_library(${PROJECT_NAME} SHARED
    ${TEXTEDITOR_SOURCES}
    ${TEXTEDITOR_HEADERS}
    ${TEXTEDITOR_FORMS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Concurrent
)

target_compile_definitions(${PROJECT_NAME} PRIVATE TEXTEDITOR_LIBRARY)
target_compile_definitions(${PROJECT_NAME} PRIVATE QTCREATOR_UTILS_LIB)
target_compile_definitions(${PROJECT_NAME} PRIVATE CORELIB_LIBRARY)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/codeassist)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/utils)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/tooltip)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/snippets)
