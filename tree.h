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
	bTreeNode(int deg, bool isLeaf) {
		keys = new key[2 * deg - 1];
		degree = deg;
		keyNum = 0;
		children = new  bTreeNode * [2 * degree];
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
	void insert(int hash, string item) {
		//initializer if the tree is empty.
		if (root == NULL) {
			root = new bTreeNode(tDeg, true);
			root->keys[0].itemName = item;
			root->keys[0].hash = hash;
			root->keyNum = 1;
		}
		//if the root is full we will proceed to grow the tree.
		else if (root->keyNum == 2 * tDeg - 1) {
			//create a new node , let this be called the calling node through which we will reset out structure.
			bTreeNode* callinNode = new bTreeNode(tDeg, false);
			//make old root into a child of the new root
			callinNode->children[0] = root;
			//split the root.
			//through the calling node.
			split(callinNode, 0, root);

			int i = 0;
			if (callinNode->keys[0].hash < hash) {
				i++;
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
		for (int i = 0; i < caller->degree - 1; i++) {
			newNode->keys[i].hash = input->keys[i + caller->degree].hash;
			newNode->keys[i].itemName = input->keys[i + caller->degree].itemName;
		}
		//reducing the number of keys in the input node by 1. Since it was full it will go down by 1.
		input->keyNum = caller->degree - 1;

		//creating space for a new child.
		for (int i = caller->keyNum; i >= cIndex + 1; i--) {
			caller->children[i + 1] = caller->children[i];
		}
		//the new child will be linked to the new node.
		caller->children[cIndex + 1] = newNode;

		//the caller node will have a key of input be transfered to it.
		//shifting values so that space is created for that value.
		for (int i = (caller->degree - 1); i >= cIndex; i--) {
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
		int index = adder->keyNum - 1;

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
		for (i = 0; i < path->keyNum; i++) {
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
	bool search(bTreeNode* find, int hash, string& path) {
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

	//deleting files and folders. //Need to ensure that the node does not get too small during deletion.
	void Delete(bTreeNode* input,int hash) {
		//finding the index of the key that has to be removed.
		int indexKey = getIndexOfKey(input, hash);

		//if key to be removed is present in one of the current node indexes.
		if (indexKey < input->keyNum && input->keys[indexKey].hash == hash) {
			if (input->leaf) {
				LeafRemove(input, indexKey); //remove the value from the leaf
			}
			else {
				InternalRemove(input, indexKey); //remove the value from an internal node.
			}
		}
		else {
			//if the value is not currently found.
			//check if the current node is a leaf.
			if (input->leaf) {
				cout << "THE VALUE DOES NOT EXIST" << endl;
				return;
			}
			//a boolean variable to indicate whether the value to be removed is present in the sub tree rooted with the current node.
			bool subTree = false;
			if (indexKey == input->keyNum) {
				subTree = true;
			}

			if (input->children[indexKey]->keyNum < input->degree) {
				FillChild(input, indexKey);
			}
			if (subTree && indexKey > input->keyNum) {
				Delete(input->children[indexKey - 1], hash);
			}
			else {
				Delete(input->children[indexKey], hash);
			}
		}
	}
	void LeafRemove(bTreeNode* input,int index) { //remove the value from a leaf.
		for (int i = index + 1; i < input->keyNum; i++) {
			input->keys[i - 1].hash = input->keys[i].hash;
			input->keys[i - 1].itemName = input->keys[i].itemName;
		}
		input->keyNum--;
	}
	void InternalRemove(bTreeNode* input,int index) { //remove a value from an internal node
		//getting a hash value to be used.
		int tempHash = input->keys[index].hash;
		//if child has atleast t keys.
		if (input->children[index]->keyNum >= input->degree) {
			key predecessor = getPredecessor(input, index); //get the predecessor. contains both hash and itemName.
			input->keys[index].hash = predecessor.hash;
			input->keys[index].itemName = predecessor.itemName;
			Delete(input->children[index],predecessor.hash);
		}
		//examine the next child if the previous child does not meet the criteria.
		else if (input->children[index + 1]->keyNum >= input->degree) {
			//get successor.
			key successor = getSuccessor(input, index);
			input->keys[index].hash = successor.hash;
			input->keys[index].itemName = successor.itemName;
			Delete(input->children[index], successor.hash);
		}
		else {
			//calling the merge function.
			merge(input, index);
			Delete(input->children[index], tempHash);
		}
	}
	key getPredecessor(bTreeNode* input,int index) { //retrieve the value of the predecessor. Of a certain key or hash
		bTreeNode* finder = input->children[index];
		while (!finder->leaf) {
			finder = finder->children[finder->keyNum]; //keep going right till value of that index is specified at a leaf node.
		}

		return finder->keys[finder->keyNum - 1];
	}
	key getSuccessor(bTreeNode* input,int index) { //retireve the value of a successor of a certain key or hash.
		bTreeNode* finder = input->children[index + 1];
		while (!finder->leaf) {
			finder = finder->children[0]; //keep moving left till you reach the required left most value at a leaf node.
		}
		return finder->keys[0];
	}
	void FillChild(bTreeNode* input,int index) { //method to complete a child node that has less than t-1 keys.
		if (index != 0 && input->children[index - 1]->keyNum >= input->degree) {
			borrowPrev(input, index);
		}
		else if (index != input->keyNum && input->children[index + 1]->keyNum >= input->degree) {
			borrowNext(input, index);
		}
		else {
			if (index != input->keyNum) {
				merge(input, index);
			}
			else {
				merge(input, index - 1);
			}
		}
	}
	void borrowNext(bTreeNode*input,int index) { //borrow a value from the i+1 sibling.
		bTreeNode* child = input->children[index];
		bTreeNode* sibling = input->children[index + 1];
		child->keys[child->keyNum].hash = input->keys[index].hash;
		child->keys[child->keyNum].itemName = input->keys[index].itemName;

		//first child of the sibling becomes the last child of the currently chosen child.
		if (!child->leaf) {
			child->children[child->keyNum + 1] = sibling->children[0];
		}
	//key from subling is inserted into the main node.
		input->keys[index].hash = sibling->keys[0].hash;
		input->keys[index].itemName = sibling->keys[0].itemName;

	 //shifting all keys of sibling to fill the emptied space.
		for (int i = 1; i < sibling->keyNum; i++) {
			sibling->keys[i - 1] = sibling->keys[i];
		}
		//if sibling is not a leaf then moving all children behind by one step too.
		if (!sibling->leaf) {
			for (int i = 1; i < sibling->keyNum; i++) {
				sibling->children[i - 1] = sibling->children[i];
			}
		}
		child->keyNum += 1;
		sibling->keyNum -= 1;

	}
	void borrowPrev(bTreeNode* input,int index) { //borrow a value from the i-1 sibling.
		bTreeNode* child = input->children[index];
		bTreeNode* sibling = input->children[index - 1];
		//performing all of the above mentioned tasks but in the opposite direction.

		//move all keys of the child left by one step
		for (int i = child->keyNum - 1; i >= 0; i--) {
			child->keys[i + 1] = child->keys[i];
		}
		//if child is not a leaf then move the children too.
		if (!child->leaf) {
			for (int i = child->keyNum - 1; i >= 0; i--) {
				child->children[i + 1] = child->children[i];
			}
		}
		//childs first key is equal to the last key in the current parent node.
		child->keys[0] = input->keys[index - 1];

		if (!child->leaf) {
			//moving siblings last child into the main child.
			child->children[0] = sibling->children[sibling->keyNum];
		}

		input->keys[index - 1] = sibling->keys[sibling->keyNum - 1];

		child->keyNum += 1;
		sibling->keyNum -= 1;
	}
	void merge(bTreeNode* input,int index) { //method to merge two children into one node.
		//creating a child and its sibling.
		bTreeNode* child = input->children[index];
		bTreeNode* sibling = input->children[index + 1];

		child->keys[input->degree - 1] = input->keys[index];

		//push all keys from the sibling to the child.
		for (int i = 0; i < sibling->keyNum; i++) {
			child->keys[i + input->degree] = sibling->keys[i];
		}
		//if the children are not leafs then the children can also be copied.
		if (!child->leaf) {
			for (int i = 0; i < sibling->keyNum; i++) {
				child->children[i + input->degree] = sibling->children[i];
			}
		}

		//filling the gap created by the index value.
		for (int i = index + 1; i < input->keyNum; i++) {
			input->keys[i - 1] = input->keys[i];
		}
		//shifting children.
		for (int i = index + 2; i <= input->keyNum; i++) {
			input->children[i - 1] = input->children[i];
		}

		//update keys counts of the node.
		child->keyNum += sibling->keyNum;
		input->keyNum--;
		delete sibling;
	}
	int getIndexOfKey(bTreeNode* input, int hash) {
		int index = 0;
		while (index < input->keyNum && input->keys[index].hash < hash) {
			index++;
		}
		return index;
	}
};
//an updated updated ver. 
