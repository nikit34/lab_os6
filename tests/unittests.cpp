#include"../struct_server.h"
#include<iostream>
#include<cassert>


class TestBitTree {
    public:
        TestBitTree(){
            this->tree = new BinTree();
        }

        ~TestBitTree(){
            this->clear_all_nodes();
        }

        void setup_tree(uint16_t& count_nodes);
        void clear_all_nodes();
        int get_size_tree(BinTree::Node* node);
        void delete_nodes(struct BinTree::Node* node);
        void check_config_node(int& last_1, int& last_2, bool&& root_exist);

        void test_insert(){
            assert(get_size_tree(this->tree->head) == 0 && "[ERROR] tree is not empty");
            int id_1 = 5;
            int id_2 = 3;
            this->tree->insert(id_1);
            this->tree->insert(id_2);

            this->check_config_node(id_1, id_2, false);
            std::cout << "test_insert - done" << std::endl;
        }

        void test_erase(){
            uint16_t count_nodes = 100;
            this->setup_tree(count_nodes);
            for(int i = 0; i < count_nodes; ++i)
                this->tree->erase(i);
            std::cout << "test_erase - done " << std::endl;
        }

        static void test_create_default_node(){
            int id_1 = 5;
            BinTree* tree = new BinTree();
            tree->head = new BinTree::Node(id_1);
            assert(tree->head != nullptr && "[ERROR] create default head node");
            assert(tree->head->id == id_1 && "[ERROR] create default head node");
            assert(tree->head->left == nullptr && "[ERROR] create default left node");
            assert(tree->head->right == nullptr && "[ERROR] create default right node");
            std::cout << "test_create_default_node - done" << std::endl;
            delete tree;
        }

    private:
        BinTree* tree;
};

int main(){
    TestBitTree::test_create_default_node();

    TestBitTree default_tree;
    default_tree.test_insert();
    default_tree.test_erase();

    return 0;
}

void TestBitTree::setup_tree(uint16_t& count_nodes) {
    std::vector<int> tmp(count_nodes);
    for(uint16_t i = 0; i < count_nodes; ++i){
        tmp[i] = rand() % count_nodes;
        this->tree->insert(tmp[i]);
    }
    assert(this->get_size_tree(this->tree->head) > 0 && "[ERROR] generate nodes");
}

void TestBitTree::delete_nodes(struct BinTree::Node* node) {
    if (node == nullptr)
        return;
    this->delete_nodes(node->left);
    this->delete_nodes(node->right);
    delete node;
}

void TestBitTree::clear_all_nodes(){
    delete_nodes(this->tree->head);
}

int TestBitTree::get_size_tree(BinTree::Node* node) {
    if (node != nullptr)
        return (this->get_size_tree(node->left) + 1 + this->get_size_tree(node->right));
    return 0;
}

void TestBitTree::check_config_node(int& last_1, int& last_2, bool&& root_exist){
    assert((last_1 == last_2 || this->tree->head->id == last_1 || this->tree->head->id == last_2) && "[ERROR] check for difference nodes");
    if(!root_exist){
        if (last_1 > last_2){
            assert(this->tree->head->id == last_1 && "[ERROR] insert less then prev - id");
            assert(this->tree->head->left->id == last_2 && "[ERROR] insert more then prev - left->id");
            assert(this->tree->head->right == nullptr && "[ERROR] insert none - right");
        } else {
            assert(this->tree->head->id == last_2 && "[ERROR] insert less then prev - id");
            assert(this->tree->head->left->id == last_1 && "[ERROR] insert more then prev - left->id");
            assert(this->tree->head->right == nullptr && "[ERROR] insert none - right");
        }
    }
    else if(root_exist){
        if (this->tree->head->id > last_1  && this->tree->head->id > last_2) {
            if (last_1 > last_2){
                assert(this->tree->head->id == last_1 && "[ERROR] insert - id");
                assert(this->tree->head->left->id == last_2 && "[ERROR] insert - left->id");
                assert(this->tree->head->right != nullptr && "[ERROR] insert - right");
            } else {
                assert(this->tree->head->id == last_2 && "[ERROR] insert - id");
                assert(this->tree->head->left->id == last_1 && "[ERROR] insert - left->id");
                assert(this->tree->head->right != nullptr && "[ERROR] insert - right");
            }
        } else if (this->tree->head->id > last_1  && this->tree->head->id < last_2) {
            assert(this->tree->head != nullptr && "[ERROR] insert - id");
            assert(this->tree->head->left->id == last_1 && "[ERROR] insert - left->id");
            assert(this->tree->head->right->id == last_2 && "[ERROR] insert - right->id");
        } else if (this->tree->head->id < last_1  && this->tree->head->id > last_2) {
            assert(this->tree->head != nullptr && "[ERROR] insert- id");
            assert(this->tree->head->left->id == last_2 && "[ERROR] insert - left->id");
            assert(this->tree->head->right->id == last_1 && "[ERROR] insert - right->id");
        } else if (this->tree->head->id < last_1  && this->tree->head->id < last_2) {
            if (last_1 > last_2){
                assert(this->tree->head->id == last_2 && "[ERROR] insert - id");
                assert(this->tree->head->left != nullptr && "[ERROR] insert - left");
                assert(this->tree->head->right->id == last_1 && "[ERROR] insert - right->id");
            } else {
                assert(this->tree->head->id == last_1 && "[ERROR] insert - id");
                assert(this->tree->head->left != nullptr && "[ERROR] insert - left");
                assert(this->tree->head->right->id == last_2 && "[ERROR] insert - right->id");
            }
        }
    }
}
