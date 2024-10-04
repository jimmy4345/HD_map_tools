
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(pcdmerge)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (pcdmerge pcdmerge.cpp)
target_link_libraries (pcdmerge ${PCL_LIBRARIES})
