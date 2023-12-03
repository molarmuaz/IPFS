#include <iostream>
#include <fstream>

using namespace std;

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
};