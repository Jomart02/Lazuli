cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME Lazuli)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/PluginLoader/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/MainWindow/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/UdpSender/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/SendWidget/)

set(HDRS
    ./include/PluginLoader/PluginLoader.h
    ./include/MainWindow/MainWindow.h
    ./include/UdpSender/udp_sender.h
    ./include/SendWidget/SendWidget.h
)

set(SRCS
    ./src/main.cpp
    ./src/PluginLoader/PluginLoader.cpp
    ./src/MainWindow/MainWindow.cpp
    ./src/UdpSender/udp_sender.cpp
    ./src/SendWidget/SendWidget.cpp
)

set(QRCS

)

add_subdirectory(./BaseNaviWidget)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ui/MainWindow/
    ui/SendWidget/
)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS} ${QRCS} ${HDRS})

# target_include_directories(BaseNaviWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network BaseNaviWidgetLib)