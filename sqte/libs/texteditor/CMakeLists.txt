cmake_minimum_required(VERSION 3.5)

project(texteditorlib VERSION 0.1 LANGUAGES CXX)

set (TEXTEDITOR_HEADERS
    texteditor_global.h
    texteditor.h
)

set (TEXTEDITOR_SOURCES
    texteditor.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${TEXTEDITOR_SOURCES}
    ${TEXTEDITOR_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Concurrent
)

target_compile_definitions(${PROJECT_NAME} PRIVATE TEXTEDITOR_LIBRARY)

