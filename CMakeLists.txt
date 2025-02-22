## @file    CMakeLists.txt
## @brief   gsmodule project
## @version 1.0.0
## @auther  ysd

cmake_minimum_required(VERSION 3.16)

if (NOT GS_PROJECT)
    set(GS_PROJECT gsproject)
endif()
project(${GS_PROJECT})

if (NOT TARGET GS_MODULE)
    add_subdirectory(gsmodule)
endif()

if (NOT TARGET GS_TEST)
    add_subdirectory(tests)
endif()
