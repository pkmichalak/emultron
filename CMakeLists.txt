cmake_minimum_required(VERSION 3.10)

project(Emultron VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic handling
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(Emultron
    src/main.cpp
    src/emultron.qrc
    src/emultron/emultron.cpp
    src/gigaemul/gigaemulf2/gigaemulf2.cpp
    src/gigaemul/gigaemulf3/gigaemulf3.cpp
    src/gigaemuldebugwidget/gigaemuldebugwidget.cpp
    src/gigaemulwidget/gigaemulwidget.cpp
    src/misc/styledialog/styledialog.cpp
    src/screendebugwidget/screendebugwidget.cpp
)

target_include_directories(Emultron PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/emultron
    ${CMAKE_SOURCE_DIR}/src/gigaemul
    ${CMAKE_SOURCE_DIR}/src/gigaemul/gigaemulf2
    ${CMAKE_SOURCE_DIR}/src/gigaemul/gigaemulf3
    ${CMAKE_SOURCE_DIR}/src/gigaemuldebugwidget
    ${CMAKE_SOURCE_DIR}/src/gigaemulwidget
    ${CMAKE_SOURCE_DIR}/src/gigastate
    ${CMAKE_SOURCE_DIR}/src/misc/styledialog
    ${CMAKE_SOURCE_DIR}/src/screendebugwidget
)
target_link_libraries(Emultron PRIVATE Qt5::Widgets)
