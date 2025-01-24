cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME Lazuli)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/PluginLoader/)


set(HDRS
    ./include/PluginLoader/PluginLoader.h
)

set(SRCS
    ./src/main.cpp
    ./src/PluginLoader/PluginLoader.cpp
)

set(QRCS

)

add_subdirectory(./BaseNaviWidget)

set(CMAKE_AUTOUIC_SEARCH_PATHS 

)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS} ${QRCS} ${HDRS})

# target_include_directories(BaseNaviWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} Qt6::Widgets BaseNaviWidgetLib)