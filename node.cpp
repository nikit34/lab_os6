#include<iostream>
#include"zmq.hpp"
#include<signal.h>
#include"server.h"



int main(int argc, char** argv) {
    if(argc != 3) {
        std::cerr << "Not enough parameters" << std::endl;
        exit(-1);
    }
    int id = std::stoi(argv[1]);
    int port = std::stoi(argv[2]);
    zmq::context_t context(3);
    zmq::socket_t socket(context, ZMQ_REP);

    socket.connect(get_port(port));

    int left_pid = 0;
    int right_pid = 0;
    int left_id = 0;
    int right_id = 0;

    int linger = 0;

    zmq::socket_t left_socket(context, ZMQ_REQ);
    zmq::socket_t right_socket(context, ZMQ_REQ);

    left_socket.setsockopt(ZMQ_SNDTIMEO, 2000);
    left_socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    right_socket.setsockopt(ZMQ_SNDTIMEO, 2000);
    right_socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

    int left_port = bind_socket(left_socket);
    int right_port = bind_socket(right_socket);

    std::string request;
    std::string msg;
    std::string cmd;

    int input_id;

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto time = 0;
    bool clock_time = false;

    while (true) {
        request = recieve_msg(socket);
        std::istringstream cmd_stream(request);
        cmd_stream >> cmd;
        if (cmd == "id") {
            msg = "OK: " + std::to_string(id);
            send_msg(socket, msg);
        } else if (cmd == "pid") {
            msg = "OK: " + std::to_string(getpid());
            send_msg(socket, msg);
        } else if (cmd == "create") {
            cmd_stream >> input_id;
            if (input_id == id) {
                msg = "Error: Already exists";
                send_msg(socket, msg);
            } else if (input_id < id) {
                if (left_pid == 0) {
                    left_pid = fork();
                    if (left_pid == -1) {
                        msg = "Error: Cannot fork";
                        send_msg(socket, msg);
                        left_pid = 0;
                    } else if (left_pid == 0) {
                        create_node(input_id, left_port);
                    } else {
                        left_id = input_id;
                        msg = "pid";
                        send_msg(left_socket, msg);
                        send_msg(socket, recieve_msg(left_socket));
                    }
                } else {
                    send_msg(left_socket, request);
                    send_msg(socket, recieve_msg(left_socket));
                }
            } else {
                if (right_pid == 0) {
                    right_pid = fork();
                    if (right_pid == -1) {
                        msg = "Error: Cannot fork";
                        send_msg(socket, msg);
                        right_pid = 0;
                    } else if (right_pid == 0) {
                        create_node(input_id, right_port);
                    } else {
                        right_id = input_id;
                        msg = "pid";
                        send_msg(right_socket, msg);
                        send_msg(socket, recieve_msg(right_socket));
                    }
                } else {
                    send_msg(right_socket, request);
                    send_msg(socket, recieve_msg(right_socket));
                }
            }

        } else if (cmd == "remove") {
            cmd_stream >> input_id;
            if (input_id < id) {
                if (left_id == 0) {
                    msg = "Error: Not found";
                    send_msg(socket, msg);
                } else if (left_id == input_id) {
                    msg = "kill_children";
                    send_msg(left_socket, msg);
                    recieve_msg(left_socket);
                    kill(left_pid, SIGTERM);
                    kill(left_pid, SIGKILL);
                    left_id = 0;
                    left_pid = 0;
                    msg = "OK";
                    send_msg(socket, msg);
                } else {
                    send_msg(left_socket, request);
                    send_msg(socket, recieve_msg(left_socket));
                }
            } else {
                if (right_id == 0) {
                    msg = "Error: Not found";
                    send_msg(socket, msg);
                } else if (right_id == input_id) {
                    msg = "kill_children";
                    send_msg(right_socket, msg);
                    recieve_msg(right_socket);
                    kill(right_pid,SIGTERM);
                    kill(right_pid,SIGKILL);
                    right_id = 0;
                    right_pid = 0;
                    msg = "OK";
                    send_msg(socket, msg);
                } else {
                    send_msg(right_socket, request);
                    send_msg(socket, recieve_msg(right_socket));
                }
            }

        } else if (cmd == "exec") {
            std::ostringstream res;
            int size;
            cmd_stream >> cmd >> size;
            std::vector<int> path(size);
            for(int i = 0; i < size; ++i){
                cmd_stream >> path[i];
            }
            if(path.empty()) {
                if(cmd == "start") {
                    start = std::chrono::high_resolution_clock::now();
                    clock_time = true;
                    msg = "Ok:" + std::to_string(id);
                    send_msg(socket, msg);
                }
                else if(cmd == "stop") {
                    if(clock_time) {
                        stop = std::chrono::high_resolution_clock::now();
                        time += std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
                        clock_time = false;
                        msg = "Ok:" + std::to_string(id);
                        send_msg(socket, msg);
                    }
                } else if(cmd == "time") {
                    msg = "Ok: " + std::to_string(id) + ": " + std::to_string(time));
                    send_msg(socket, msg);
                }
            } else {
                int next_id = path.front();
                path.erase(path.begin());
                res << "exec " << cmd << " " << path.size();
                for(int i: path){
                    res << " " << i;
                }
                if (next_id == id) {
                    msg = "Node is available";
                    send_msg(socket, msg);
                } else if (next_id < id) {
                    if (left_pid == 0) {
                        msg = "Error:" + std::to_string(next_id) + ": Not found";
                        send_msg(socket, msg);
                    } else {
                        send_msg(left_socket, request);
                        send_msg(socket, recieve_msg(left_socket));
                    }
                } else {
                    if (right_pid == 0) {
                        msg = "Error:" + std::to_string(next_id) + ": Not found";
                        send_msg(socket, msg);
                    } else {
                        send_msg(right_socket, request);
                        send_msg(socket, recieve_msg(right_socket));
                    }
                }
            }

        } else if (cmd == "pingall") {
            std::ostringstream res;
            std::string left_res;
            std::string right_res;
            if (left_pid != 0) {
                msg = "pingall";
                send_msg(left_socket, msg);
                left_res = recieve_msg(left_socket);
            }
            if (right_pid != 0) {
                msg = "pingall";
                send_msg(right_socket, msg);
                right_res = recieve_msg(right_socket);
            }
            if (!left_res.empty() && left_res.substr(std::min<int>(left_res.size(), 5)) != "Error") {
                res << left_res;
            }
            if (!right_res.empty() && right_res.substr(std::min<int>(right_res.size(), 5)) != "Error") {
                res << right_res;
            }
            send_msg(socket, res.str());

        } else if (cmd == "kill_children") {
            if (left_pid == 0 && right_pid == 0) {
                msg = "OK";
                send_msg(socket, msg);
            } else {
                if (left_pid != 0) {
                    msg = "kill_children";
                    send_msg(left_socket, msg);
                    recieve_msg(left_socket);
                    kill(left_pid, SIGTERM);
                    kill(left_pid, SIGKILL);
                }
                if (right_pid != 0) {
                    msg = "kill_children";
                    send_msg(right_socket, msg);
                    recieve_msg(right_socket);
                    kill(right_pid, SIGTERM);
                    kill(right_pid, SIGKILL);
                }
                msg = "OK";
                send_msg(socket, msg);
            }
        }
        if (port == 0) {
            break;
        }
    }
    return 0;
}
