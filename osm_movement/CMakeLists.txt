cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(osm_move)



include_directories("/usr/include")


link_directories("/usr/lib/x86_64-linux-gnu")


add_executable(osm_move osm_move.cpp)

target_link_libraries(osm_move
/usr/lib/x86_64-linux-gnu/libtinyxml2.so.9.0.0
)
