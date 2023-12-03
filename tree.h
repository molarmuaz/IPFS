#include <iostream>
#include <fstream>

using namespace std;
//the tree requires 4 basic things
//number 1 is the key value
//number 2 parent relationships (optional)
//number 3 are the left right pointers
//number 4 is child pointer array that depends on the m value for the tree
struct treeNode{
    int key; //for now this the value we using . Basic b tree
    treeNode* left;    //using the basic tree structure for now.
    treeNode* right;    //using the basic tree structure for now.
    treeNode** childrenArr; //list of the children attached to the tree
};

class tree
{
private:
    string address; // when a string is inserted it is stored in a file in a folder. This string stores the address of that file
    int keys; //max number of keys allowed
    int M; //number of children possible
    tree* children; //array of M children
public:
    tree(){}
    
    tree(int mary)
    {
        keys = mary-1;
        M = mary;
        children = new tree[M];
    }

    void setM(int m)
    {
        M = m;
    }

    int getM()
    {
        return M;
    }
    //void splitChild(){} //a method to split children between a parent and leaf node by finding the middle value and shifting it up by 1 position.
    //void insert(){} //insert method for the b tree that will use other methods to further insert the stuffs.
    //void insert_non_full(){} //a method that attaches a value to a leaf node by going from the root node to that specific leaf and then making a possible insert by rebalancing the tree.
};
