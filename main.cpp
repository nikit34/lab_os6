#include<iostream>
#include"zmq.hpp"
#include<vector>
#include<csignal>
#include<set>
#include<algorithm>
#include"server.h"
#include"struct_server.h"



int main() {
    BinTree tree;

    std::string cmd;

    int child_pid = 0;
    int child_id = 0;
    zmq::context_t context(1);
    zmq::socket_t main_socket(context, ZMQ_REQ);
    int linger = 0;
    main_socket.setsockopt(ZMQ_SNDTIMEO, 2000);
    main_socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    int port = bind_socket(main_socket);

    int input_id;
    std::string result;
    std::string msg;
    std::string recieved_msg;

    while (true) {
        std::cin >> cmd;
        if (cmd == "create") {
            std::cin >> input_id;
            if (child_pid == 0) {
                child_pid = fork();
                if (child_pid == -1) {
                    std::cout << "Unable to create first worker node\n";
                    child_pid = 0;
                    exit(1);
                } else if (child_pid == 0) {
                    create_node(input_id, port);
                } else {
                    child_id = input_id;
                    msg = "pid";
                    send_message(main_socket, msg);
                    result = recieve_message(main_socket);
                }
            } else {
                std::ostringstream msg_stream;
                msg_stream << "create " << input_id;
                send_message(main_socket, msg_stream.str());
                result = recieve_message(main_socket);
            }

            if (result.substr(0,2) == "OK") {
                tree.insert(input_id);
            }
            std::cout << result << "\n";

        } else if (cmd == "remove") {
            if (child_pid == 0) {
                std::cout << "Error: Not found\n";
                continue;
            }

            std::cin >> input_id;
            if (input_id == child_id) {
                kill(child_pid, SIGTERM);
                kill(child_pid, SIGKILL);
                child_id = 0;
                child_pid = 0;
                std::cout << "OK\n";
                tree.erase(input_id);
                continue;
            }
            msg = "remove " + std::to_string(input_id);
            send_message(main_socket, msg);
            recieved_msg = recieve_message(main_socket);
            if (recieved_msg.substr(0, std::min<int>(recieved_msg.size(), 2)) == "OK") {
                tree.erase(input_id);
            }
            std::cout << recieved_msg << "\n";

        } else if (cmd == "exec") {
            int n;
            std::cin >> input_id >> n;
            std::vector<int> numbers(n);
            for (int i = 0; i < n; ++i) {
                std::cin >> numbers[i];
            }

            msg = "exec " + std::to_string(input_id) + " " + std::to_string(n);
            for (int i = 0; i < n; ++i) {
                msg += " " + std::to_string(numbers[i]);
            }

            send_message(main_socket, msg);
            recieved_msg = recieve_message(main_socket);
            std::cout << recieved_msg << "\n";

        } else if (cmd == "pingall") {
            msg = "pingall";
            send_message(main_socket, msg);
            recieved_msg = recieve_message(main_socket);
            std::istringstream is;
            if (recieved_msg.substr(0,std::min<int>(recieved_msg.size(), 5)) == "Error") {
                is = std::istringstream("");
            } else {
                is = std::istringstream(recieved_msg);
            }

            std::set<int> recieved_tree;
            while (is >> input_id) {
                recieved_tree.insert(input_id);
            }
            std::vector from_tree = tree.get_nodes();
            auto part_it = std::partition(from_tree.begin(), from_tree.end(), [&recieved_tree] (int a) {
                return recieved_tree.count(a) == 0;
            });
            if (part_it == from_tree.begin()) {
                std::cout << "OK: -1\n";
            } else {
                std::cout << "OK:";
                for (auto it = from_tree.begin(); it != part_it; ++it) {
                    std::cout << " " << *it;
                }
                std::cout << "\n";
            }

        } else if (cmd == "exit") {
            break;
        }
    }
    return 0;
}