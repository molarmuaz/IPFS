#pragma once
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
    int IDspace;
    int activePCs;
    bool* activeIDs;
    int degree;
public:
    Network(int idsp, int deg)
    {
        IDspace = idsp;
        degree = deg;
        head = NULL;
        numberOfPCs = pow(2, IDspace);
        activePCs = 0;
        //making a hash table to store IDs of active PCs
        activeIDs = new bool[numberOfPCs] {0};
    }

    int getMaxPCs()
    {
        return pow(2, IDspace);
    }

    void activateamount(int x)
    {
        for (int i = 0; i < x; i++)
        {
            insert();
        }
    }

    void insert(int x = -1)
    {
        srand(time(NULL));
        //for now id is just a random number generated between 0 and numberOfPCs and is a number that isn't already active
        if (activePCs < numberOfPCs && x)
        {
            if (x == -1 && activePCs)
            {
                //finding a random pc to activate
                x = rand() % activePCs;
                while (activeIDs[x] != 0)
                {
                    x = rand() % activePCs;
                }
            }
            else if (!activePCs)
            {
                x = 0;
            }

            node* temp = new node(degree);
            temp->setID(x);

            if (!activePCs)
            {
                head = temp;
                head->setNext(head);
                temp->setStart(0);
            }
            else
            {
                node* p = head;
                //adding the pc//
                while (true)
                {
                    if (p->getID() < x && p->getNext()->getID() > x)
                    {
                        break;
                    }
                    p = p->getNext();
                }

                temp->setNext(p->getNext());
                p->setNext(temp);

                //Insert range
                temp->setStart(p->getID() + 1);

                //Update routingtables
                p = p->getNext();
                RoutingTable tempRouter;
                while (p->getNext() != temp)
                {
                    p = p->getNext();
                    p->getRouter().insertEntry(temp);
                }

                FileList hashes;
                temp->getNext()->getDir().Transfer(temp->directory,temp->getStart(), temp->getEnd(),IDspace, hashes);
                temp->getDir().RemoveRange(hashes);
                hashes.clear();

            }
            //recording in table that PC x has been activated
            activeIDs[x] = true;
            activePCs++;
        }
        else
        {
            cout << "Max limit reached" << endl;
        }    
    }

    bool activeCheck(int x)
    {
        return activeIDs[x];
    }

    int hashing(string hash, int max = -1)
    {
        char x;
        int result = 0;
        for (int i = 0; i < 3; i++)
        {
            x = hash[2 - i];
            if (x >= 97 && x <= 102)
            {
                result += (int(x) - 87) * pow(16, i);
            }
            else
            {
                result += (int(x) - 48) * pow(16, i);
            }
        }

        if (max != -1)
        {
            int id = pow(2, max);
            return (result % id);
        }
        return result;
    }

    string readFileToString(ifstream& inputStream) {

        string line;
        string content;

        // Check if file opened successfully
        if (!inputStream.is_open()) {
            throw  runtime_error("Failed to open file.");
        }

        // Read line by line and append to content
        while (getline(inputStream, line)) {
            content += line;
        }

        // Close the file stream
        inputStream.close();

        return content;
    }

    string getSHA1HashFile(ifstream& inputStream) {

        const  string& data = readFileToString(inputStream);
        // Create the SHA1 context
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        int result = EVP_DigestInit_ex(ctx, EVP_sha1(), NULL);

        // Update the SHA1 context with the data
        EVP_DigestUpdate(ctx, data.c_str(), data.length());

        // Calculate the final SHA1 hash
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_len;
        EVP_DigestFinal_ex(ctx, hash, &hash_len);

        // Convert the hash to a hexadecimal string
        stringstream ss;
        for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
            ss << hex << setfill('0') << setw(2) << (int)hash[i];
        }

        // Clean up the SHA1 context
        EVP_MD_CTX_free(ctx);

        return ss.str();
    }

    void remove(int id)
    {
        node* a = head;
        while (a->getID() != id)
        {
            a = a->getNext();
        }
        node* b = a;
        while (true)
        {
            b->getRouter().deleteEntry(id);
            if (b->getNext() != a)
            {
                break;
            }
            b = b->getNext();
        }
        FileList hashes;
        a->getNext()->getDir().Transfer(a->directory, a->getStart(), a->getEnd(), IDspace, hashes);
        a->getDir().RemoveRange(hashes);
        hashes.clear();
        b->setNext(a->getNext());
        delete a;
    }

    void insertFile(ifstream& path) // we can later create a function to read files and then insert the contents of that file in this function as a string
    {
        string hash = getSHA1HashFile(path);
        int key = hashing(hash);

        node* a;

        if (!key || key > head->getStart())
        {
            a = head;
        }
        else
        {
            a = head->getRouter().findNode(key);
        }


        a->insert(path, hash);
    }

    void removeFile(string hash)
    {
        int key = hashing(hash, IDspace);
        node* a;

        if (!key || key > head->getStart())
        {
            a = head;
        }
        else
        {
            a = head->getRouter().findNode(key);
        }

        a->remove(hash);
    }

    string search(string hash)
    {
        int key = hashing(hash, IDspace);
        node* a;

        if (!key || key > head->getStart())
        {
            a = head;
        }
        else
        {
            a = head->getRouter().findNode(key);
        }

        return a->search(hash);
    }

    string displayBTree(int key)
    {
        
        node* a;

        if (!key || key > head->getStart())
        {
            a = head;
        }
        else
        {
            a = head->getRouter().findNode(key);
        }

        return a->bTreeDisplay();
    }
};
