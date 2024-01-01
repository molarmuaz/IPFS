#include "network.h"
#include <iostream>
using namespace std;


void addNode(Network n)
{
    char x;
    bool loop = true;
    bool many = false;
    int choice;
    while (loop)
    {
        cout << "Would you like to turn:\n1. one node\n2. many random nodes\enter: ";
        cin >> choice;
        if (choice == 1)
        {
            loop = false;
        }
        else if (choice == 2)
        {
            many = true;
            loop = false;
        }
        else
        {
            cout << "Invalid Input please re-enter choice" << endl;
        }
    }

    if (!many)
    {
        while (loop)
        {
            cout << "Would you like to choose which node to open? [y/n]" << endl;
            cout << "Enter: ";
            cin >> x;
            int amnt;
            if (x == 'y' || x == 'Y')
            {
                cout << "Enter the node to turn on: ";
                cin >> amnt;
                if (!n.activeChech(x))
                {
                    loop = false;
                }

            }
            else if (x == 'n' || 'N')
            {
                amnt = -1;
                loop = false;
            }
            else
            {
                cout << "Invalid Input! Please give a valid response" << endl;
            }
        }

        n.insert(amnt);
    }
    else
    {
        cout << "How many would you like to turn on?" << endl;
        cout << "Enter: ";
        cin >> choice;

        n.activeamount(choice);
    }
}

void removeNode(Network n)
{
    int x;
    cout << "Enter the ID of the node to delete" << endl;
    cin >> x;

    int m = n.getMaxPCs();
    if (x > m)
    {
        cout << "Value too large for the defined ID space" << endl;
        return;
    }
    if (n.activecheck(x))
    {
        n.remove();
    }
}

void insertFile(Network n)
{
    ifstream path;
    cout << "Insert file path" << endl;
    cin >> path;
    n.insertFile(path);
}

void removeFile(Network n)
{
    string hash;
    cout << "Insert hash: ";
    cin >> hash;
    n.removeFile(hash);
}

void searchFile(Network n)
{
    string hash;
    cout << "Enter file hash: ";
    cin >> hash;
    n.search(hash);
}

void contents(Network n)
{
    n.displayBTree();
}

int main()
{
    int idsp = -1;
    int deg = -1;
    cout << "                                  WELCOME TO OUR INTER-PLANETARY FILING SYSTEM" << endl << endl;
    while (idsp < 0)
    {
        cout << "To start you off please select the identifier space for your network (please insert a positive integer): ";
        cin >> idsp;
    }

    while (deg < 0)
    {
        cout << "Now please select the degree for your network's tree (please insert a positive integer): ";
        cin >> deg;
    }

    Network network(idsp, deg);

    cout << "\nAlright! the network has been created and the maximum numbers of computers you can have is " << pow(2, idsp) << endl << endl;
    int input;
    bool loop = true;
    while (loop)
    {
        cout << "What operation would you like to perform?" << endl;
        cout << "1. Add a node" << endl;
        cout << "2. Delete a node" << endl;
        cout << "3. Display a specific computer's contents" << endl;
        cout << "4. Insert a file" << endl;
        cout << "5. Remove a file" << endl;
        cout << "6. Search for a file" << endl;
        cout << "7. End Operations" << endl;
        cout << "\nEnter: ";
        cin >> input;

        switch (input)
        {
        case 1:
            addNode(network);
            break;
        case 2:
            removeNode(network);
            break;
        case 3:
            contents(network);
            break;
        case 4:
            insertFile(network);
            break;
        case 5:
            removeFile(network);
            break;
        case 6:
            searchFile(network);
            break;
        case 7:
            loop = false;
            break;
        }
    }

    cout << "Thank you for using IPFS" << endl;
    cout << "Goodbye" << endl;
    cout << "\nAuthors: Muaz Ahmed | Ibtehaj Haider | Ayan Asif" << endl;

    return 0;
}
