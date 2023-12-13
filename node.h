#include "tree.h"
class node
{
private:
    
    bTree head;
    node* next;
    int id;
public:
    node()
    {
        next = nullptr;
    }

    void setID(int idn)
    {
        id = idn;
    }

    int getID()
    {
        return id;
    }

    void setNext(node* n)
    {
        next = n;
    }

    node * getNext()
    {
        return next;
    }

    void insert(string x)
    {

    }
};
