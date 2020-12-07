#include"./../struct_server.cpp"
#include <iostream>
#include <cassert>


class TestBitTree{
    public:
        TestBitTree(){
            this->tree = new BinTree();

        }
        static void test_create_default_node(){
            int test_id_1 = 5;
            BinTree* tree = new BinTree();
            tree->head = new BinTree::Node(test_id_1);
            assert(tree->head != nullptr && "[ERROR] create default head node");
            assert(tree->head->id == test_id_1 && "[ERROR] create default head node");
            assert(tree->head->left == nullptr && "[ERROR] create default left node");
            assert(tree->head->right == nullptr && "[ERROR] create default right node");
        }
        // void test_insert(){
        //     BinTree *t = new BinTree();
        //     int test_id_1 = 5;
        //     int test_id_2 = 3;
        //     t->insert(test_id_1);
        //     assert(t->head->id == test_id_1 && "[ERROR] insert test_id_1");
        //     t->insert(test_id_2);
        //     if(test_id_1 > test_id_2){
        //         assert(t->head->id == test_id_1 && "[ERROR] insert test_id_2");
        //         assert(t->head->left->id == test_id_2 && "[ERROR] insert test_id_2");
        //     }
        //     else if(test_id_1 < test_id_2){
        //         assert(t->head->id == test_id_1 && "[ERROR] insert test_id_2");
        //         assert(t->head->right->id == test_id_2 && "[ERROR] insert test_id_2");
        //     } else {
        //         assert(t->head->id == test_id_1 && "[ERROR] insert repeat");
        //     }
        // }
        // void test_erase(){

        // }
    private:
        BinTree* tree;
};

int main(){
    TestBitTree::test_create_default_node();
    // tree.test_insert();
    // tree.test_erase();
    return 0;
}