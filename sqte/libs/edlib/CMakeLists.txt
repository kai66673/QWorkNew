cmake_minimum_required(VERSION 3.5)

project(edlib VERSION 0.1 LANGUAGES CXX)

set (EDLIB_SOURCES
)

set (EDLIB_HEADERS
    edlib_global.h
)

add_library(${PROJECT_NAME} SHARED
    ${EDLIB_SOURCES}
    ${EDLIB_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_compile_definitions(${PROJECT_NAME} PRIVATE EDLIB_LIBRARY)
