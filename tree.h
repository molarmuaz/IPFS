#include <iostream>
#include <string>

using namespace std;

//key struct
//the key structure is required to store a hash and a string value of the item name.
struct key {
	int hash; //using hash as a standalone for now with a null value.
	string itemName; //item name is the name of the file we will have to search for.
	//for the default constructor we will initialize both strings to be empty.
	void keySetDetails(int a = -1, string b = " ") {
		hash = a;
		itemName = b;
	}
};

//b tree node
//contains a structure array of the key.
//contains a pointer array of the children subject to degree of the tree.
class bTreeNode {
public:
	key* keys; //the keyy array
	int degree; //the degree specifier per node.
	int keyNum; //the current number of keys present in a node.
	bTreeNode** children; //the children pointer array that will store the next pointer added to the tree.
	bool leaf; //indicator if a node is leaf node or no.
	bTreeNode(int deg,bool isLeaf) {
		keys = new key[2*deg - 1];
		degree = deg;
		keyNum = 0;
		children = new  bTreeNode*[2*degree];
		leaf = isLeaf;
	}
	//tree node functionalities since we cant break the actual call structure of a b tree using the universal method conventions.

	
};


//b tree itself
class bTree {
public:
	bTreeNode* root; //root node of the tree. will help in growing the tree.
	int tDeg; //degree of the tree that can be made.
	bTree(int degree) { //constructor for the tree. Since we need to specify the size during the run time for creation.
		root = NULL;
		tDeg = degree;
	}
	void insert(int hash,string item) {
		//initializer if the tree is empty.
		if (root == NULL) {
			root = new bTreeNode(tDeg, true);
			root->keys[0].itemName = item;
			root->keys[0].hash = hash;
			root->keyNum = 1;
		}
		//if the root is full we will proceed to grow the tree.
		else if (root->keyNum == 2*tDeg - 1) {
			//create a new node , let this be called the calling node through which we will reset out structure.
			bTreeNode* callinNode = new bTreeNode(tDeg, false);
			//make old root into a child of the new root
			callinNode->children[0] = root;
			//split the root.
			//through the calling node.
			split(callinNode, 0, root);

			int i = 0;
			if (callinNode->keys[0].hash < hash) {
				i++ ;
			}
			NotFull(callinNode->children[i], hash, item);

			root = callinNode;
		}
		else {
			//root is not full so we are inserting into the root.
			NotFull(root, hash, item);
		}
	}
	//method to split children
	void split(bTreeNode* caller, int cIndex, bTreeNode* input) {
		//the parametre conventions are as follows-> caller is the node that will be used to adjust, cIndex is the index where the current child lies
		//input is the node that will be used as the reference to fill values for a new node that is to be created.
		bTreeNode* newNode = new bTreeNode(input->degree, input->leaf);
		newNode->keyNum = caller->degree - 1;
		//copying keys of internal node to the newNode
		for (int i = 0;i < caller->degree - 1;i++) {
			newNode->keys[i].hash = input->keys[i + caller->degree].hash;
			newNode->keys[i].itemName = input->keys[i + caller->degree].itemName;
		}
		//reducing the number of keys in the input node by 1. Since it was full it will go down by 1.
		input->keyNum = caller->degree - 1;

		//creating space for a new child.
		for (int i = caller->keyNum;i >= cIndex + 1;i--) {
			caller->children[i + 1] = caller->children[i];
		}
		//the new child will be linked to the new node.
		caller->children[cIndex + 1] = newNode;

		//the caller node will have a key of input be transfered to it.
		//shifting values so that space is created for that value.
		for (int i = (caller->degree - 1);i >= cIndex;i--) {
			caller->keys[i + 1].hash = caller->keys[i].hash;
			caller->keys[i + 1].itemName = caller->keys[i].itemName;
		}

		//adding that value to the calling node.
		caller->keys[cIndex].hash = input->keys[caller->degree - 1].hash;
		caller->keys[cIndex].itemName = input->keys[caller->degree - 1].itemName;

		caller->keyNum += 1;
	}
	//method to insert into a non full node i.e could be a leaf or could not be a leaf.
	void NotFull(bTreeNode* adder, int hash, string item) {
		//initializing value to right most element.
		int index = adder->keyNum-1;

		//check if leaf node or not.
		if (adder->leaf) {
			//shifting all greater values to the right.
			while (index >= 0 && adder->keys[index].hash > hash) {
				adder->keys[index + 1].hash = adder->keys[index].hash;
				adder->keys[index + 1].itemName = adder->keys[index].itemName;
				index--;
			}
			//inserting the new key.
			adder->keys[index + 1].hash = hash;
			adder->keys[index + 1].itemName = item;
			adder->keyNum += 1;
		}
		//if not leaf node.
		else {
			//find index without shifting.
			while (index >= 0 && adder->keys[index].hash > hash) {
				index--;
			}
			//check if the child at the current required index is full or not.
			if (adder->children[index + 1]->keyNum == (2 * adder->degree) - 1) {
				split(adder, index + 1, adder->children[index + 1]);
				//see which child is going to have the new key the smaller batch or the greater batch.
				if (adder->keys[index + 1].hash > hash) {
					index++;
				}

			}
			//calling an insert for the child now.
			//insert the new key into the child.
			NotFull(adder->children[index + 1], hash, item);
		}
	}
	//traversing or searching.
	void traverse(bTreeNode* path) {
		int i = 0;
		for ( i = 0;i < path->keyNum;i++) {
			if (!path->leaf) {
				traverse(path->children[i]);
			}
			cout << " " << path->keys[i].itemName << endl;
		}
		//print sub tree
		if (!path->leaf) {
			traverse(path->children[i]);
		}
		
	}
	//search in the tree
	bool search(bTreeNode* find, int hash,string& path) {
		int i = 0; //searching index
		while (i<find->keyNum && hash > find->keys[i].hash) {
			i++;
		}
		if (hash == find->keys[i].hash) {
			path = path.append(find->keys[i].itemName);
			return true;
		}
		if (find->leaf) {
			return false;
		}
		path = path.append(find->keys[i].itemName);
		path = path.append("/");
		return search(find->children[i], hash, path);
	}

	//deleting files and folders.
};
