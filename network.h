#include "node.h"
#include <time.h>
#include <cmath>
using namespace std;

//Circular Linked List

class Network
{
private:
    node* head;
    int numberOfPCs;
    int activePCs;
    bool * activeIDs; //will be replaced with routing table once that is figured out
    int IDspace;
public:
    Network(int idsp)
    {
        IDspace = idsp;
        head = NULL;
        numberOfPCs = pow(2,IDspace);

        node* temp = new node;
        node* p = head;

        //making a cirular linked list (makes a linked list normally and sets the next of the last node to head)
        for(int i = 0; i<numberOfPCs; i++)
        {
            temp->setID(i);
            temp->setActive(false);
            if(head == NULL)
            {
                head = temp;
            }
            else
            {
                while(p->getNext() != NULL)
                {
                    p = p->getNext();
                }
                p->setNext(temp);
            }
        }
        p = p->getNext();
        p->setNext(head);

        //making a hash table to store IDs of active PCs
        activeIDs = new bool[numberOfPCs]{0};
    }

    void activateamount(int x)
    {
        for(int i = 0; i<x; i++)
        {
            activate();
        }
    }

    void activate(int x = -1)
    {
        srand(time(NULL));
        //for now id is just a random number generated between 0 and numberOfPCs and is a number that isn't already active
        if(activePCs < numberOfPCs)
        {
            if(x == -1)
            {
                //finding a random pc to activate
                x = rand() % activePCs;
                while(activeIDs[x] != 0)
                {
                    x = rand() % activePCs;
                }
            }

            //setting status of PC to active
            while(head->getID() != x)
            {
                head = head->getNext();
            }
            head->setActive(true);

            //recording in table that PC x has been activated
            activeIDs[x] = true;
        }
        else
        {
            cout<<"Max limit reached"<<endl;
        }


        //ADD FUNCTIONALITY TO BALANCE FILE DISTRIBUTION AFTER ADDITION OF A PC (AFTER B-TREES ARE MADE)
    }

   int hashing(string hash, int max = -1 )
{
    char x;
    int result = 0;
    for(int i =0; i<3; i++)
    {
        x = hash[2-i];
        if(x>=97 && x<=102)
        {
            result += (int(x)-87) * pow(16,i);
        }
        else
        {
            result+= (int(x)-48) * pow(16,i);
        }
    }

    if(max != -1)
        return (result%max);
    return result;
}


    void insert(string x) // we can later create a function to read files and then insert the contents of that file in this function as a string
    {
        int key; // UNSURE OF WHAT METHOD TO USE TO ASSIGN KEYS | IF WE ARE USING A LINEAR COUNTER TO ASSIGN KEYS findNode() works fine
        int nodeAssigned;

        if(key > numberOfPCs || key < 0)
        {
            nodeAssigned = 0;
        }
        else if(activeIDs[key])
        {
            nodeAssigned = key;
        }
        else
        {
            nodeAssigned = key;
            while(!activeIDs[nodeAssigned])
            {
                nodeAssigned++;
            }           
        }
        while(head->getID() != nodeAssigned)
        {
            head = head->getNext();
        }
        head->insert(x); // IBBI AND YAN'S WORK
    }

    void remove(int id)
    {
        //AFTER B TREES ARE DESIGNED
    }

};
