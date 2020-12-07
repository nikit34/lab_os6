class BinTree {
    public:
        BinTree() = default;
        ~BinTree(){
            delete_recursive(this->head);
        }

    private:
        void delete_recursive(BinTree* node){
            if(this->head == nullptr){
                return;
            }
            delete_recursive(node->right);
            delete_recursive(node->left);
            delete node;
        }

        BinTree* head = nullptr;
        int id;
        BinTree* right = nullptr;
        BinTree* left = nullptr;

        friend class TestBitTree;
};