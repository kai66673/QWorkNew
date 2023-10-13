cmake_minimum_required(VERSION 3.5)

project(texteditor VERSION 0.1 LANGUAGES CXX)

set (TEXTEDITOR_HEADERS
    texteditor_global.h
    texteditor.h
    texteditoroverlay.h
    refactoroverlay.h
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

    core/corelib_global.h
    core/itexteditor.h

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
    indenter.cpp
    tabsettings.cpp
    syntaxhighlighter.cpp
    highlighterformatsmanager.cpp
    highlighterformatsconfigurewidget.cpp
    codestylepool.cpp

    core/itexteditor.cpp

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
