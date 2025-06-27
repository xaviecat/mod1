cmake_minimum_required(VERSION 3.16)

project(mod1
		VERSION 0.1
		LANGUAGES CXX
		DESCRIPTION "This simulation graphic project will have you represent a surface in 3D, on which water will flow. Empirical or scientific (with many equations), recreate a wave, a tsunami or maybe just rain on your surface."
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
if(EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sEXCEPTION_CATCHING_ALLOWED=[..]")
	set(CMAKE_EXE_LINKER_FLAGS "-lembind")
endif()

#set(CMAKE_PREFIX_PATH "$ENV{HOME}/goinfre/Qt/6.9.0/gcc_64/lib/cmake")
if("$ENV{USER}" STREQUAL "xcharra")
	set(CMAKE_PREFIX_PATH "$ENV{HOME}/goinfre/Qt/6.9.1/gcc_64/lib/cmake")
endif()

set(QT_COMPONENTS
		Core
		Widgets
		OpenGL
		OpenGLWidgets
)

if(NOT EMSCRIPTEN)
	list(APPEND QT_COMPONENTS
			Concurrent
	)
endif()

find_package(Qt6 REQUIRED COMPONENTS ${QT_COMPONENTS})

qt_add_executable(${PROJECT_NAME}
		srcs/main.cpp
		srcs/core/Map.cpp
		srcs/core/Map.hpp
		srcs/core/Triangle.cpp
		srcs/core/Triangle.hpp
		srcs/core/Triangulator.cpp
		srcs/core/Triangulator.hpp
		srcs/gui/OpenGLWidget.cpp
		srcs/gui/mainwindow.cpp
		srcs/gui/mainwindow.hpp
		srcs/gui/mainwindow.ui
		srcs/utils/colors.h
		srcs/utils/errors.h
		resources.qrc
)

set(QT_LIBRARIES
		Qt6::Core
		Qt6::Widgets
		Qt6::OpenGL
		Qt6::OpenGLWidgets
)

if(NOT EMSCRIPTEN)
	list(APPEND QT_LIBRARIES
			Qt6::Concurrent
	)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES})
