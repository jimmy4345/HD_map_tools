cmake_minimum_required(VERSION 3.8)
project(las2pcd_all_in_one VERSION 0.1.0)

find_package(PCL 1.3 REQUIRED)

include_directories(
${PCL_INCLUDE_DIRS}
/usr/local/include/LASlib    # Required !!
)

link_directories(${PCL_LIBRARY_DIRS}
/usr/local/lib/cmake    # Optional, I think because the libLASlib.a has defined
)

add_definitions(${PCL_DEFINITIONS})

add_executable(las2pcd las2pcd_all_in_one.cpp)

target_link_libraries(las2pcd
${PCL_LIBRARIES} 
${PCL_COMMON_LIBRARIES}
${PCL_IO_LIBRARIES}
/usr/local/lib/LASlib/libLASlib.a   # Required !!
)

