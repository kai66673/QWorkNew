cmake_minimum_required(VERSION 3.5)

project(qte VERSION 0.1 LANGUAGES CXX)

# set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
cmake_policy(SET CMP0071 NEW)

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

find_package(Python ${MIN_PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)
find_package(SIP REQUIRED)
include(SIPMacros)

file(GLOB_RECURSE sip_files_core sip/*.sip)
GENERATE_SIP_PYTHON_MODULE_CODE(edlib sip/edlib.sip "${sip_files_core}" cpp_files)
BUILD_SIP_PYTHON_MODULE(edlib sip/edlib.sip "${cpp_files}" "" edlib)
