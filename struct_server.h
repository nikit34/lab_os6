#pragma once
#include<iostream>
#include<vector>


class BinTree {
    private:
        struct Node {
            Node(int id): id(id) {}
            int id;
            Node* left = nullptr;
            Node* right = nullptr;
        };

        Node* head = nullptr;


    public:
        BinTree() = default;
        ~BinTree(){
            this->delete_recursive(this->head);
        }

        std::vector<int> get_all_nodes(Node* node=nullptr) {
            std::vector<int> result;
            if (node == nullptr)
                node = this->head;
            this->all_nodes(node, result);
            return result;
        }

        std::vector<int> get_path_to(int& id, Node* node=nullptr) {
            std::vector<int> path;
            if (node == nullptr)
                node = this->head;
            this->find_path(node, id, path);
            return path;
        }

        bool contains(int& id) const{
            Node* tmp = this->head;
            while(tmp != nullptr){
                if(tmp->id == id)
                    break;
                else if(id > tmp->id)
                    tmp = tmp->right;
                else if(id < tmp->id)
                    tmp = tmp->right;
            }
            return tmp != nullptr;
        }

        void insert(int& id){
            if(this->head == nullptr){
                this->head = new Node(id);
                return;
            }
            Node* tmp = this->head;
            while(tmp != nullptr){
                if(tmp->id == id)
                    break;
                else if(id < tmp->id){
                    if(tmp->left == nullptr){
                        tmp->left = new Node(id);
                        break;
                    }
                    tmp = tmp->left;
                }
                else if(id > tmp->id){
                    if(tmp->right == nullptr){
                        tmp->right = new Node(id);
                        break;
                    }
                    tmp = tmp->right;
                }
            }
        }

        void erase(int& id){
            Node* prev = nullptr;
            Node* tmp = this->head;
            while(tmp != nullptr){
                if (id == tmp->id) {
                    if (prev == nullptr) {
                        this->head = nullptr;
                    } else {
                        if (prev->left == tmp) {
                            prev->left = nullptr;
                        } else {
                            prev->right = nullptr;
                        }
                    }
                    delete_recursive(tmp);
                } else if(id < tmp->id) {
                    prev = tmp;
                    tmp = tmp->left;
                } else if(id > tmp->id) {
                    prev = tmp;
                    tmp = tmp->right;
                }
            }
        }

    private:

        void all_nodes(Node* node, std::vector<int>& vec) const{
            if(node == nullptr)
                return;
            this->all_nodes(node->left, vec);
            vec.push_back(node->id);
            this->all_nodes(node->right, vec);
        }

        void find_path(Node* node, int& id, std::vector<int>& path) {
            while(node != nullptr){
                path.push_back(node->id);
                if(node->id == id)
                    break;
                else if(id > node->id)
                    node = node->right;
                else if(id < node->id)
                    node = node->right;
            }
        }

        void delete_recursive(Node* node){
            if(node == nullptr)
                return;
            delete_recursive(node->right);
            delete_recursive(node->left);
            delete node;
        }

        friend class TestBitTree;
};