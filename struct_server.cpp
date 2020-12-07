#include<iostream>
#include<vector>


class BinTree {
    public:
        BinTree() = default;
        ~BinTree(){
            this->delete_recursive(this->head);
        }

        std::vector<int> get_all_nodes() {
            std::vector<int> result;
            this->all_nodes(this->head, result);
            return result;
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
                    return;
                else if(id < tmp->id){
                    if(tmp->left == nullptr){
                        tmp->left = new Node(id);
                        return;
                    }
                    tmp = tmp->left;
                }
                else if(id > tmp->id){
                    if(tmp->right == nullptr){
                        tmp->right = new Node(id);
                        return;
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
        struct Node {
            Node(int id): id(id) {}
            int id;
            Node* left = nullptr;
            Node* right = nullptr;
        };

        void all_nodes(Node* node, std::vector<int> vec) const{
            if(node == nullptr)
                return;
            this->all_nodes(node->left, vec);
            vec.push_back(node->id);
            this->all_nodes(node->right, vec);
        }

        void delete_recursive(Node* node){
            if(node == nullptr)
                return;
            delete_recursive(node->right);
            delete_recursive(node->left);
            delete node;
        }

        Node* head = nullptr;

        friend class TestBitTree;
};