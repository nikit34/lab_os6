cmake_minimum_required(VERSION 3.10)
project(network_cpp)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_EXE main.cpp node.cpp)

set(SOURCE_LIB server.cpp struct_server.cpp)

add_library(server STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main server)





cmake_minimum_required(VERSION 3.10)
project(os_lab_06)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp struct_server.h)
add_executable(node node.cpp struct_server.h)
add_library(server server.cpp server.h)

target_link_libraries(server zmq)
target_link_libraries(main zmq server)
target_link_libraries(node zmq server)