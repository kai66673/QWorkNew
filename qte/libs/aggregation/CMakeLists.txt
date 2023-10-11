cmake_minimum_required(VERSION 3.5)

project(qteaggregate VERSION 0.1 LANGUAGES CXX)

set (AGGREGATE_HEADERS
    aggregation_global.h
    aggregate.h
)

set (AGGREGATE_SOURCES
    aggregate.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${AGGREGATE_SOURCES}
    ${AGGREGATE_HEADERS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE AGGREGATION_LIBRARY)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)
