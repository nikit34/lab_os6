#include<iostream>
#include<vector>


class BinTree {
    public:
        BinTree() = default;
        ~BinTree(){
            this->delete_recursive(this->head);
        }

        std::vector<int> get_all_nodes() const {
            std::vector<int> result;
            this->all_nodes(this->head, result);
            return result;
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