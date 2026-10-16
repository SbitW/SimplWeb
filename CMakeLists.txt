cmake_minimum_required(VERSION 3.16)
project(SimplWeb LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_executable(SimplWeb
	main.cpp
	mainwindow.h
	mainwindow.cpp
)
target_link_libraries(SimplWeb PRIVATE
	Qt6::Widgets
	Qt6::WebEngineWidgets
)
