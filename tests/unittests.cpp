#include"../struct_server.h"
#include <iostream>
#include <cassert>


class TestBitTree {
    public:
        TestBitTree(){
            this->tree = new BinTree();
        }

        ~TestBitTree(){
            this->clear_all_nodes(this->tree);
        }

        void setup_tree(uint16_t& count_nodes);
        void delete_nodes(BinTree::Node* node);
        void clear_all_nodes(BinTree* tree);
        int get_size_tree(BinTree::Node* node);
        void check_config_node(int& id_1, int& id_2);

        static void test_create_default_node(){
            int id_1 = 5;
            BinTree* tree = new BinTree();
            tree->head = new BinTree::Node(id_1);
            assert(tree->head != nullptr && "[ERROR] create default head node");
            assert(tree->head->id == id_1 && "[ERROR] create default head node");
            assert(tree->head->left == nullptr && "[ERROR] create default left node");
            assert(tree->head->right == nullptr && "[ERROR] create default right node");
            std::cout << "test_create_default_node - done" << std::endl;
            delete tree->head;
        }

        void test_insert(){
            assert(get_size_tree(this->tree->head) == 0 && "[ERROR] tree is not empty");
            int id_1 = 5;
            int id_2 = 3;
            this->tree->insert(id_1);
            this->tree->insert(id_2);

            this->check_config_node(id_1, id_2);
            std::cout << "test_insert - done" << std::endl;
            this->clear_all_nodes(this->tree);
        }

        void test_erase(){
            uint16_t count_nodes = 100;
            this->setup_tree(count_nodes);
            for(int i = 0; i < count_nodes; ++i)
                this->tree->erase(i);
            assert(this->get_size_tree(this->tree->head) == 0 && "[ERROR] tree is not empty");
            std::cout << "test_erase - done " << std::endl;
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

void TestBitTree::delete_nodes(BinTree::Node* node) {
    if (node == nullptr)
        return;
    this->delete_nodes(node->left);
    this->delete_nodes(node->right);
    delete node;
}

void TestBitTree::clear_all_nodes(BinTree* tree){
    this->delete_nodes(tree->head);
}

int TestBitTree::get_size_tree(BinTree::Node* node) {
    if (node != nullptr)
        return (this->get_size_tree(node->left) + 1 + this->get_size_tree(node->right));
    return 0;
}

void TestBitTree::check_config_node(int& id_1, int& id_2){
    if(id_1 > id_2){
        assert(this->tree->head->id == id_1 && "[ERROR] insert less then prev - id");
        assert(this->tree->head->left->id == id_2 && "[ERROR] insert less then prev - left->id");
    }
    else if(id_1 < id_2){
        assert(this->tree->head->id == id_1 && "[ERROR] insert more then prev - id");
        assert(this->tree->head->right->id == id_2 && "[ERROR] insert more then prev - right->id");
        assert(this->tree->head->left != nullptr && "[ERROR] insert more theen prev - left");
    } else {
        assert(this->tree->head->id == id_1 && "[ERROR] insert repeat");
    }
}
