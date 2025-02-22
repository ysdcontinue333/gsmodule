## @file    CMakeLists.txt
## @brief   gsmodule test
## @version 1.0.1
## @auther  ysd

cmake_minimum_required(VERSION 3.16)

if (NOT GS_TEST)
    set(GS_TEST gstest)
endif()

add_executable(${GS_TEST} 
    ./main.cpp
)

target_compile_features(${GS_TEST} PUBLIC cxx_std_17)

add_subdirectory(googletest)

target_include_directories(${GS_TEST} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../gsmodule/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/include
)

target_link_libraries(${GS_TEST} PRIVATE
    gsmodule
    gtest
)
target_compile_definitions(${GS_TEST} PRIVATE
    CMAKE_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)