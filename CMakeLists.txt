cmake_minimum_required(VERSION 3.1.0)

project(tomogram_visualizer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(tomogram_visualizer
    main.cpp
    Data.h
    Data.cpp
    View.h
    View.cpp
)

target_link_libraries(tomogram_visualizer Qt5::Widgets Qt5::OpenGL)