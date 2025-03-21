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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/MapControl/)

set(HDRS
    ./include/PluginLoader/PluginLoader.h
    ./include/MainWindow/MainWindow.h
    ./include/UdpSender/udp_sender.h
    ./include/SendWidget/SendWidget.h
    ./include/global_vars.h
    ./include/MapControl/MapControl.h
)

set(SRCS
    ./src/main.cpp
    ./src/PluginLoader/PluginLoader.cpp
    ./src/MainWindow/MainWindow.cpp
    ./src/UdpSender/udp_sender.cpp
    ./src/SendWidget/SendWidget.cpp
    ./src/MapControl/MapControl.cpp
)

set(QRCS
    ./resources/Style.qrc
    ./resources/map.qrc
    ./translations/translations.qrc
)

set(TS_FILES
    translations/Lazuli_en.ts
    translations/Lazuli_ru.ts
)
set(QM_FILES
    translations/Lazuli_en.qm
    translations/Lazuli_ru.qm
)

add_subdirectory(./BaseNaviWidget)
add_subdirectory(./QssStyleManager)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ui/MainWindow/
    ui/SendWidget/
)

find_package(Qt6 COMPONENTS Widgets Network  Quick Location Positioning REQUIRED)

# Поиск lupdate и lrelease
find_program(LUPDATE_EXECUTABLE lupdate)
find_program(LRELEASE_EXECUTABLE lrelease)
if(NOT LUPDATE_EXECUTABLE OR NOT LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "lupdate or lrelease not found")
endif()

# Обновление .ts файлов
add_custom_target(update_ts
    COMMAND ${LUPDATE_EXECUTABLE} ${SRCS} ${HDRS} ui/MainWindow/
    ui/SendWidget/ -ts ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Updating translation files"
)
# Компиляция .ts в .qm
add_custom_command(
    OUTPUT ${QM_FILES}
    COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILES}
    DEPENDS ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compiling translation files to .qm"
)
add_custom_target(translations ALL DEPENDS ${QM_FILES})

add_executable(${PROJECT_NAME} ${SRCS} ${QRCS} ${HDRS} ${QRCS})

# target_include_directories(BaseNaviWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network Qt6::Quick Qt6::Location Qt6::Positioning BaseNaviWidgetLib QssStyleManager)

# Копирование DLL BaseNaviWidget в каталог с исполняемым файлом
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:BaseNaviWidgetLib>"  # Путь к DLL
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"  # Каталог с исполняемым файлом
    COMMENT "Copying BaseNaviWidget.dll to the executable directory"
)