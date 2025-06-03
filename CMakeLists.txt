cmake_minimum_required(VERSION 3.26)
project(MojProjekt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")


add_executable(MojProjekt
        ##gui.cpp
        main.cpp
        board.cpp
        AI.cpp


)

