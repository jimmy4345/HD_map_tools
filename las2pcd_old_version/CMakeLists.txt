cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(las2pcd)

find_package(PCL 1.3 REQUIRED COMPONENTS common io)
find_package(libLAS REQUIRED)


include_directories(${PCL_INCLUDE_DIRS})

include_directories("/usr/include")
include_directories("/usr/local/include/liblas/")

link_directories(${PCL_LIBRARY_DIRS})
link_directories("/usr/local/lib")

add_definitions(${PCL_DEFINITIONS})

add_executable(las2pcd las2pcd.cpp)

target_link_libraries(las2pcd 
${PCL_COMMON_LIBRARIES}
${PCL_IO_LIBRARIES}
/usr/local/lib/liblas.so.2.4.0
)
