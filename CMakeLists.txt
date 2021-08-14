cmake_minimum_required(VERSION 3.20)
project(CrackingTheCodingInterview)

add_subdirectory(googletest)

macro(addExecutable target)
    add_executable(${target} ${ARGN})
endmacro()


macro(addTestExecutable test-target target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${test-target} PRIVATE ${test-target} gtest gtest_main gmock gmock_main)
endmacro()


add_subdirectory("Ch 1.Arrays And Strings")