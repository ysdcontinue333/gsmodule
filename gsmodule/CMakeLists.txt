## @file    CMakeLists.txt
## @brief   gsmodule library
## @version 1.0.1
## @auther  ysd

cmake_minimum_required(VERSION 3.16)

if (NOT GS_MODULE)
    set(GS_MODULE gsmodule)
endif()

add_library(${GS_MODULE} STATIC
    ./source/gs_api_interface.cpp
    ./source/gspatch_parser.cpp
    ./include/gs_api_commands.h
    ./include/gs_api_interface.h
    ./include/gspatch_element.h
    ./include/gspatch_parser.h
)

target_compile_features(${GS_MODULE} PUBLIC cxx_std_17)

add_subdirectory(tinyxml2)

target_include_directories(${GS_MODULE} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2
)

set_target_properties(${GS_MODULE} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)