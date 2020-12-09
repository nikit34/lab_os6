cmake_minimum_required(VERSION 3.10)
project(6_network_nods)


add_executable(main main.cpp struct_server.h)
add_executable(node node.cpp struct_server.h)
add_library(server server.cpp server.h)

target_link_libraries(server zmq)
target_link_libraries(main zmq server)
target_link_libraries(node zmq server)