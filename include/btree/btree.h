#ifndef BTREE_H     
#define BTREE_H

#include <string>
#include "btree/node.h"


class BTree {

private:
    BTreeNode* root;
    int t; // Minimum degree

    

public:
    BTree(int degree);
    ~BTree();
    void traverse();
    bool search(int key, std::string &value);
    void insert(int key, const std::string &value);

    // void remove(int key);





}; 

#endif // BTREE_H