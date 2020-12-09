#include<zmq.hpp>
#include <signal.h>

#include<iostream>
#include<set>
#include<string>
#include<vector>

#include"server.h"
#include"struct_server.h"



void print_menu(){
	std::cout << "----------------------------------" << std::endl
    << "create [child id]" << std::endl
	<< "remove [id]" << std::endl
	<< "exec [id]" << std::endl
	<< "pingall" << std::endl
	<< "menu" << std::endl
	<< "exit" << std::endl
    << "----------------------------------" << std::endl;
}


int main() {
    BinTree tree;

    std::string cmd;

    int child_pid = 0;
    int child_id = 0;

    zmq::context_t context(1);
    zmq::socket_t main_socket(context, ZMQ_REQ);

    main_socket.set(zmq::sockopt::sndtimeo, 2000);
    int port = bind_socket(main_socket);

    int input_id;
    std::string result;
    std::string msg;

    print_menu();
    while (true) {
        std::cin >> cmd;
        if (cmd == "create") {
            std::cin >> input_id;
            if (child_pid == 0) {
                child_pid = fork();
                if (child_pid == -1) {
                    std::cout << "Unable to create first worker node" << std::endl;
                    child_pid = 0;
                    exit(1);
                } else if (child_pid == 0) {
                    create_node(input_id, port);
                } else {
                    child_id = input_id;
                    msg = "pid";
                    send_msg(main_socket, msg);
                    result = recieve_msg(main_socket);
                }
            } else {
                std::ostringstream msg_stream;
                msg_stream << "create " << input_id;
                send_msg(main_socket, msg_stream.str());
                result = recieve_msg(main_socket);
            }

            if (result.substr(0,2) == "OK") {
                tree.insert(input_id);
            }
            std::cout << result << std::endl;

        } else if (cmd == "remove") {
            if (child_pid == 0) {
                std::cout << "Error: Not found" << std::endl;
                continue;
            }

            std::cin >> input_id;
            if (input_id == child_id) {
                kill(child_pid, SIGTERM);
                kill(child_pid, SIGKILL);
                child_id = 0;
                child_pid = 0;
                std::cout << "OK" << std::endl;
                tree.erase(input_id);
                continue;
            }
            msg = "remove " + std::to_string(input_id);
            send_msg(main_socket, msg);
            result = recieve_msg(main_socket);
            if (result.substr(0, std::min<int>(result.size(), 2)) == "OK") {
                tree.erase(input_id);
            }
            std::cout << result << std::endl;

        } else if (cmd == "exec") {
            std::cin >> input_id >> cmd;
            std::vector<int> path = tree.get_path_to(input_id);
            if(path.empty()) {
                std::cout << "Error: Not found" << std::endl;
                continue;
            }
            path.erase(path.begin());
            msg = "exec " + cmd + " " + std::to_string(path.size());
            for (int i = 0; i < input_id; ++i) {
                msg += " " + std::to_string(path[i]);
            }
            send_msg(main_socket, msg);
            result = recieve_msg(main_socket);
            std::cout << result << std::endl;

        } else if (cmd == "pingall") {
            msg = "pingall";
            send_msg(main_socket, msg);
            result = recieve_msg(main_socket);

            std::istringstream is;
            if (result.substr(0, std::min<int>(result.size(), 5)) == "Error") {
                is = std::istringstream("");
            } else {
                is = std::istringstream(result);
            }

            std::set<int> recieved_tree;
            while (is >> input_id) {
                recieved_tree.insert(input_id);
            }
            std::vector<int> from_tree = tree.get_all_nodes();
            auto part_it = std::partition(from_tree.begin(), from_tree.end(), [&recieved_tree] (int a) {
                return recieved_tree.count(a) == 0;
            });
            if (part_it == from_tree.begin()) {
                std::cout << "OK: -1" << std::endl;
            } else {
                std::cout << "OK:";
                for (auto it = from_tree.begin(); it != part_it; ++it) {
                    std::cout << " " << *it;
                }
                std::cout << std::endl;
            }

        } else if (cmd == "menu") {
            print_menu();
        } else if (cmd == "exit") {
            break;
        }
    }
    return 0;
}