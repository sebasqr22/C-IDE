cmake_minimum_required(VERSION 3.19)
project(C_IDE)

set(CMAKE_CXX_STANDARD 14)

include_directories(build-untitled-Desktop-Debug)
include_directories(untitled)

add_executable(C_IDE
        build-untitled-Desktop-Debug/moc_ide.cpp
        build-untitled-Desktop-Debug/moc_predefs.h
        build-untitled-Desktop-Debug/ui_ide.h
        untitled/Cliente.cpp
        untitled/creadorListas.cpp
        untitled/ide.cpp
        untitled/ide.h
        untitled/json.hpp
        untitled/logger.cpp
        untitled/main.cpp
        untitled/mserver.cpp
        untitled/operaciones.cpp
        untitled/operacionesEstructs.cpp
        untitled/pruebas.cpp
        untitled/test.cpp)
