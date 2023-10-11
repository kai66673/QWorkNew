cmake_minimum_required(VERSION 3.5)

project(pythoneditorlib VERSION 0.1 LANGUAGES CXX)

set (PYTHONEDITOR_HEADERS
    tools/lexical/pythonscanner.h
    tools/lexical/pythonformattoken.h
    tools/lexical/sourcecodestream.h
    tools/pythonhighlighter.h
    tools/pythonindenter.h
    pythoneditor_global.h
    pythoneditor.h
    pythoneditorwidget.h
)

set (PYTHONEDITOR_SOURCES
    tools/lexical/pythonscanner.cpp
    tools/pythonindenter.cpp
    tools/pythonhighlighter.cpp
    pythoneditorwidget.cpp
    pythoneditor.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${PYTHONEDITOR_SOURCES}
    ${PYTHONEDITOR_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    qtecore
    qteutils
    qtesettings
    texteditorlib
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PYTHONEDITOR_LIBRARY)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../corelib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../settings)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../utils)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../texteditorlib)
