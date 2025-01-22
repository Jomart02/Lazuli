cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME BaseNaviWidget)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

set(HDRS
    ./BaseNaviWidget.h
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HDRS})

# Определяем макрос BASENAVIWIDGET_LIBRARY только при компиляции библиотеки
target_compile_definitions(${PROJECT_NAME} PRIVATE BASENAVIWIDGET_LIBRARY)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

target_include_directories(${PROJECT_NAME} PUBLIC ./)