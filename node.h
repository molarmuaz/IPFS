#include "tree.h"
class node
{
private:
    tree* head;
    bool active;
    node* next;
    int id;
public:
    node()
    {
        active = false;
    }

    void setID(int idn)
    {
        id = idn;
    }

    int getID()
    {
        return id;
    }

    void setActive(bool x)
    {
        active = x;
    }

    bool getActive()
    {
        return active;
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