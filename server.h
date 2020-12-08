#pragma once
#include<string>
#include"unistd.h"
#include"zmq.hpp"



bool send_msg(zmq::socket_t& socket, const std::string& msg);
std::string recieve_msg(zmq::socket_t& socket);

std::string get_port(int& port);

int bind_socket(zmq::socket_t& socket, int& port);
void create_node(int& id, int& port);