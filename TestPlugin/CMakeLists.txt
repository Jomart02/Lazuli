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

# Указываем выходную директорию для DLL
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED ${SRCS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets BaseNaviWidgetLib)


# Создаем выходную директорию, если она не существует
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Creating output directory"
)

# Копируем зависимости и библиотеку в выходную папку
if(WIN32)
    # Для Windows: копируем DLL-зависимости
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying runtime dependencies to output directory (Windows)"
    )
elseif(UNIX AND NOT APPLE)
    # Для Linux: копируем только саму библиотеку
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying library to output directory (Linux)"
    )
endif()

# # Копируем ресурсы в выходную папку
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
#     COMMENT "Copying resources to output directory"
# )

if(WIN32)
    # Для Windows: создаем ZIP-архив
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar cfv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}.zip --format=zip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Creating ZIP archive (Windows)"
    )
elseif(UNIX AND NOT APPLE)
    # Для Linux: создаем tar-архив и сжимаем его с помощью gzip
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}.tar ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND gzip -f ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}.tar
        COMMENT "Creating TAR.GZ archive (Linux)"
    )
endif()