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
    qtecore
    qtesettings
    texteditorlib
    pythoneditorlib
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/pythoneditorlib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/texteditorlib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/settings)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/corelib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../libs/utils)
