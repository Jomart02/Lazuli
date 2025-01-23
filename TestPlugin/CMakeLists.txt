cmake_minimum_required(VERSION 3.5)

project(TestPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#Подключаем директорию базового класса 
add_subdirectory(
    ${CMAKE_SOURCE_DIR}/../BaseNaviWidget
    ${CMAKE_BINARY_DIR}/BaseNaviWidget
)

set(SRCS
    TestPlugin.h
    TestPlugin.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    TestPlugin.ui
)

add_library(${PROJECT_NAME} SHARED ${SRCS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets BaseNaviWidget)
