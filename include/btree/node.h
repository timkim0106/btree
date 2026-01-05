#include <string>
#ifndef BTREE_NODE_H     
#define BTREE_NODE_H   

class BTreeNode {
private:
    int *keys;          // Array of keys in the node
    BTreeNode **children; // Array of child pointers
    int numKeys;       // Current number of keys
    bool isLeaf;       // True if node is a leaf
    std::string *values; // string data associated with keys (for leaf nodes)
    int maxDegree;    // range for number of keys

public:
    BTreeNode(int degree, bool leaf); // constructor


    ~BTreeNode(); // destructor


    // operations
    void insertNonFull(int key, const std::string &value);
    void splitChild(int i, BTreeNode *y);
    void traverse();
    BTreeNode* search(int key);
    //void remove(int key);
   // void removeFromLeaf(int idx);
   // void removeFromNonLeaf(int idx);
    //int getPredecessor(int idx);
    //int getSuccessor(int idx);
    //void fill(int idx);
};

#endif // BTREE_NODE_H