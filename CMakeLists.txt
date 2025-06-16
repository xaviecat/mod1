cmake_minimum_required(VERSION 3.16)

project(mod1
		VERSION 0.1
		LANGUAGES CXX
		DESCRIPTION "This simulation graphic project will have you represent a surface in 3D, on which water will flow. Empirical or scientific (with many equations), recreate a wave, a tsunami or maybe just rain on your surface. "
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(CMAKE_PREFIX_PATH "$ENV{HOME}/goinfre/Qt/6.9.0/gcc_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS
		Widgets
		OpenGL
		OpenGLWidgets
		Concurrent
)

add_executable(${PROJECT_NAME}
		srcs/core/Map.cpp
		srcs/core/Map.hpp
		srcs/gui/OpenGLWidget.cpp
		srcs/gui/OpenGLWidget.cpp
		srcs/gui/mainwindow.cpp
		srcs/gui/mainwindow.hpp
		srcs/gui/mainwindow.ui
		srcs/main.cpp
		srcs/utils/colors.h
		srcs/utils/errors.h
		srcs/core/Triangle.cpp
		srcs/core/Triangle.hpp
		srcs/core/Triangulator.cpp
		srcs/core/Triangulator.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
		Qt6::Widgets
		Qt6::OpenGL
		Qt6::OpenGLWidgets
		Qt6::Concurrent
)
