cmake_minimum_required(VERSION 3.5)

project(pyeditor VERSION 0.1 LANGUAGES CXX)

set (PYEDITOR_SOURCES
    mainwindow.cpp
    main.cpp
)

set (PYEDITOR_HEADERS
    mainwindow.h
)

add_executable(${PROJECT_NAME}
    ${PYEDITOR_SOURCES}
    ${PYEDITOR_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    texteditor
    pythoneditor
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/pythoneditor)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/texteditor)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/texteditor/core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/texteditor/utils)
