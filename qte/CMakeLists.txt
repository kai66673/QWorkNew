cmake_minimum_required(VERSION 3.5)

project(qte VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

###########################################################
# Globals Qt setting
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Xml PrintSupport Network Script Sql Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Xml PrintSupport Network Script Sql Concurrent REQUIRED)
###########################################################

add_subdirectory(libs)
add_subdirectory(examples)
