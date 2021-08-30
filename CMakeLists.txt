cmake_minimum_required(VERSION 3.20)
project(CrackingTheCodingInterview)

add_subdirectory(googletest)

macro(addExecutable target)
    add_executable(${target} ${ARGN})
endmacro()


macro(addTestExecutable test-name)
    add_executable(${test-name} ${ARGN})
    target_link_libraries(${test-name} PRIVATE  gtest gtest_main gmock gmock_main)
endmacro()


add_subdirectory("Ch 1.Arrays And Strings")
add_subdirectory(chapter-2-Linked-Lists)
add_subdirectory(Chapter-10-Sorting-and-Searching)

add_subdirectory("Ch 4. Trees and Graphs/C++14")