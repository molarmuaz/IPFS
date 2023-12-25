
#include "maxtree.h"
#include <iostream>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;
class node;
class RoutingTable;
int hashing(string hash, int max = -1)  //takes a hash and converts it to int
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
struct fileRange
{
	int start;
	int end;
};

struct routingNode {
	int identifier = 0; //the key 
	node* machine = nullptr;  //the address of that node
	int no;

	fileRange range; //the range of that node

	routingNode* next = nullptr; // next routingTable node
	routingNode* prev = nullptr; // prev routingTable node
};

class node
{
public:
	bTree directory;

	RoutingTable a;
	// bTree head;
	node* next;
	int id;
	fileRange range;
public:
	node(int deg) :directory(deg)
	{
		next = nullptr;
	}

	bTree getDir()
	{
		return directory;
	}

	RoutingTable getRouter()
	{
		return a;
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
		if (!directory.ExtractFiles(directory.root, hash, path, a))
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
};
class RoutingTable {
public:
	routingNode* head; //start of the routing table
	routingNode* tail; //end of the routing table
	int size; //max number of nodes in the routing table
	int count; //current number of nodes in the routing table

	


	RoutingTable() : head(nullptr), tail(nullptr) {
		size = 0;
		count = 0;
	}

	void setSize(int x) {
		size = x;
	}


	void insertionSort(routingNode* head)
	{
		routingNode* curr = head;
		while (curr)
		{
			routingNode* next = curr->next;
			routingNode* prev = nullptr;
			routingNode* temp = curr;
			while (prev && prev->identifier > temp->identifier) {
				prev->next = temp;
				temp->prev = prev;
				temp->next = prev->prev;
				if (prev->prev) {
					prev->prev->next = temp;
				}
				prev = prev->prev;
			}
			if (!prev) {
				head = temp;
			}
			curr = next;
		}

		int i = 0;
		curr = head;
		while (curr) {
			curr->no = i;
			curr = curr->next;
			i++;
		}
		 
		return;
	}



	void insertEntry(fileRange pRange, node* a)
	{
		routingNode* temp = new routingNode;
		temp->machine = a;
		temp->identifier = a->id;
		temp->next = nullptr;
		temp->prev = nullptr;
		temp->range.start = 0;
		temp->range.end = a->id;
		temp->no = count;

		if (count == size) {
			cerr << "Table full" << endl;
			return;
		}		

		if (!head || count == 0)
		{
			// The routing table is empty, so set the new node as both head and tail
			fileRange temp2 = pRange; //gets the range of the circular linked list node
			temp->range.start = temp2.end + 1;
			head = temp;
			tail = temp;
			count++;
		}

		else
		{
			
			temp->prev = tail;
			tail->next = temp;
			tail = temp;
			tail->range.start = (tail->prev->range.end + 1); //sets the starting range of the new node
			insertionSort(head); //sorts the linked list in ascending order on the basis of machine IDs
								// then numbers them 

			count++;
		}

	};




	void deleteEntry(int id)
	{

		routingNode* pre = head, * current = head;

		for (int i = 1; i < 1; i++)
		{
			pre = current;
			current = current->next;
		}

		pre->next = current->next;

		current->next->prev = pre;

		pre->next->range.start = current->range.start; //works on the principle of consistent hashing

		delete current;

		current = NULL;

		count--;
	};

	void print() {
		cout << "The routing table of this node is: " << endl;
		routingNode* curr = head;
		while (curr)
		{
			cout << "Machine " << curr->identifier << endl;
			cout << "File range: " << endl;
			cout << "Starting value: " << curr->range.start << endl;
			cout << "Ending value: " << curr->range.end << endl;
			curr = curr->next;
		}
		cout << "End of routing table." << endl;
	};



	//function utilized in command number 5 of the Deliverable.
	node* findFileViaRouting(int key, ifstream& file)
	{
		int fileRange = hashing(getSHA1HashFile(file));
		routingNode* curr = head;
		int nodeRange = 0;
		while (curr)
		{

			nodeRange = curr->range.end - curr->range.start;

			if (fileRange > nodeRange)
			{
				cout << "Machine " << curr->identifier << " --> ";
				curr = curr->next;

				continue;
			}

			else
			{
				cout << "Insertion path found. " << endl;
				return curr->machine;
			}
		}
	}

	node* findNode(int key) {
		routingNode* curr = head;
		while (curr) {


			if (key > curr->range.start && key < curr->range.end) {
				cout << "Insertion path found. " << endl;
				return curr->machine;
			}

			else {
				cout << "Machine " << curr->identifier << " --> ";
				curr = curr->next;
				continue;
			}

		}
	}
};






