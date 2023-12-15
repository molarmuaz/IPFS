#pragma once
#include "maxtree.h"
#include "RoutingTable.h"


struct fileRange
{
    int start;
    int end;
};


class node
{
private:
    fileRange range;
    
    node* next;
    int id;
    RoutingTable router;
public:
    bTree directory;
    node(int deg): directory(deg), router(this)
    {
        next = nullptr;
    }

    bTree getDir()
    {
        return directory;
    }

    RoutingTable getRouter()
    {
        return router;
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

    node* getNext()
    {
        return next;
    }

    void insert(ifstream& path, string hash)
    {
        ostringstream out;

        out << path.rdbuf();

        string s = out.str();
        directory.insert2(hash, s);
    }

    void remove(string hash)
    {
        directory.remove(hash);
    }

    string search(string hash)
    {
        string path;
        int a = 0;
        if(!directory.ExtractFiles(directory.root, hash, path,a))
        {
            path = "FILE NOT FOUND";
        }
        return path;
    }
    
    fileRange getRange() 
    {
        return range;
    }

    int getStart()
    {
        return range.start;
    }

    int getEnd()
    {
        return range.end;
    }

    void setStart(int a) 
    {
        range.start = a;
        range.end = id;
    }

    string bTreeDisplay()
    {
        directory.Display(directory.root);
    }
};
