cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(pcd_movement)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (pcd_movement pcd_movement.cpp)
target_link_libraries (pcd_movement ${PCL_LIBRARIES})
