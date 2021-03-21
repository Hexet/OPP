cmake_minimum_required(VERSION 3.15)
project(thread_web_server)

find_package(OpenMP)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_web_server OpenMP_1.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(thread_web_server PUBLIC OpenMP::OpenMP_CXX)
endif()