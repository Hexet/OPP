cmake_minimum_required(VERSION 3.6)
project(thread_web_server)

#find_package(OpenMP)
find_package(MPI)
#include_directories("C:/App/Cygwin/bin/")
#find_package(MPI REQUIRED)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_COMPILER "/usr/bin/mpicc")
#set(CMAKE_CXX_COMPILER "C:/App/Cygwin/bin/mpic++")
#SET(CMAKE_C_COMPILER mpicc)
#SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(thread_web_server MPI.cpp)

#set(SOURCE_FILES MPI.cpp)
#add_executable(thread_web_server ${SOURCE_FILES})
#
if(MPI_CXX_FOUND)
    target_link_libraries(thread_web_server PUBLIC MPI::MPI_CXX)
endif()

#if(OpenMP_CXX_FOUND)
#    target_link_libraries(thread_web_server PUBLIC OpenMP::OpenMP_CXX)
#endif()
