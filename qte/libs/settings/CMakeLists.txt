cmake_minimum_required(VERSION 3.5)

project(qtesettings VERSION 0.1 LANGUAGES CXX)

set (QTESETTINGS_HEADERS
    SettingsManager.h
    SettingsDialog.h
    settings_global.h
    ISettingsClient.h
)

set (QTESETTINGS_SOURCES
    SettingsManager.cpp
    SettingsDialog.cpp
    ISettingsClient.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${QTESETTINGS_SOURCES}
    ${QTESETTINGS_HEADERS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SETTINGS_LIBRARY)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)
