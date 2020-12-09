#include"server.h"



bool send_msg(zmq::socket_t& socket, const std::string& msg) {
    int msg_size = msg.size();
    zmq::message_t message(msg_size);
    memcpy(message.data(), msg.c_str(), msg_size);
    try {
        socket.send(message, zmq::send_flags::none);
        return true;
    } catch(...) {
        return false;
    }
}

std::string recieve_msg(zmq::socket_t& socket) {
    zmq::message_t request;
    zmq::send_result_t answer;
    try {
        answer = socket.recv(request, zmq::recv_flags::none);
    } catch(zmq::error_t &e) {
        answer = false;
    }
    std::string recieve_msg(static_cast<char*>(request.data()), request.size());
    if (recieve_msg.empty() || !answer)
        return "Error: Node is not available 5xx";
    else
        return recieve_msg;
}

std::string get_host_port(int& port) {
    return "tcp://127.0.0.1:" + std::to_string(port);
}

int bind_socket(zmq::socket_t& socket) {
    int port = 3000;
    while (true) {
        try {
            socket.bind(get_host_port(port));
            break;
        } catch(zmq::error_t &e) {
            ++port;
        }
    }
    return port;
}

void create_node(int& id, int& port) {
    char* arg_node = strdup("./node");
    char* arg_id = strdup((std::to_string(id)).c_str());
    char* arg_port = strdup((std::to_string(port)).c_str());
    char* args[] = {arg_node, arg_id, arg_port, nullptr};
    execv("./node", args);
}
