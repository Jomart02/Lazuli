cmake_minimum_required(VERSION 3.5)

project(TestPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/BaseNaviWidget/)

set(SRCS
    TestPlugin.h
    TestPlugin.cpp
)


add_library(${PROJECT_NAME} SHARED ${SRCS})


find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets BaseNaviWidget)

include_directories(${CMAKE_SOURCE_DIR}/../BaseNaviWidget/)