#pragma once
#include<zmq.hpp>

#include<iostream>
#include<string>



bool send_msg(zmq::socket_t& socket, const std::string& msg);
std::string recieve_msg(zmq::socket_t& socket);

std::string get_port(int& port);

int bind_socket(zmq::socket_t& socket);
void create_node(int& id, int& port);