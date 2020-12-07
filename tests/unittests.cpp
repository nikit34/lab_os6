#include"./struct_server.cpp"
#include <iostream>
#include <cassert>


class TestBitTree{
    public:
        void test_create_default_node(){
            BinTree *t = new BinTree();
            assert(t->head == nullptr && "[ERROR] create default fields of node");
            assert(t->head->left == nullptr && "[ERROR] create default fields of node");
            assert(t->head->right == nullptr && "[ERROR] create default fields of node");
        }
};

int main(){
    TestBitTree tree;
    tree.test_create_default_node();

    return 0;
}