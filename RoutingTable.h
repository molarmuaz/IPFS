#pragma once
#pragma once
#include <iostream>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "node.h"
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;



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



struct routingNode {
	int identifier; //the key 
	node* machine;  //the address of that node

	fileRange range; //the range of that node

	routingNode* next = nullptr; // next routingTable node
	routingNode* prev = nullptr; // prev routingTable node
};



class RoutingTable {
public:
	node* parent; // circular linked list node that is parent to the routing table
	routingNode* head; //start of the routing table
	routingNode* tail; //end of the routing table
	int size; //number of nodes in the routing table (used for calculation of linked list nodes)

	RoutingTable()
	{
		size = 0;
	}

	RoutingTable(node* a) : head(nullptr), tail(nullptr) {
		parent = a;
		size = 0;
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
	}

	

	void insertEntry(node* a) 
	{
		routingNode* temp = new routingNode;
		temp->machine = a;
		temp->identifier = a->getID();
		temp->next = nullptr;
		temp->prev = nullptr;
		temp->range.start = 0;
		temp->range.end = a->getID();


		if (!head || size == 0) 
		{
			// The routing table is empty, so set the new node as both head and tail
			fileRange temp2 = parent->getRange(); //gets the range of the circular linked list node
			temp->range.start = temp2.end+1;
			head = temp;
			tail = temp;
			size++;
		}

		else 
		{

			temp->prev = tail;
			tail->next = temp;
			tail = temp;
			tail->range.start = (tail->prev->range.end + 1); //sets the starting range of the new node
			insertionSort(head); //sorts the linked list in ascending order on the basis of machine IDs
			size++;
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

		pre->next->range.start = current->range.start; //works on the principle of consitent hashing

		delete current;

		current = NULL;

		size--;
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



