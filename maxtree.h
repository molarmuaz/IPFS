#include <iostream>
#include <string>
#include <cmath>
using namespace std;

//key struct
//the key structure is required to store a hash and a string value of the item name.
struct keyNode {
	string hash; //using hash as a standalone for now with a null value.
	string itemName; //item name is the name of the file we will have to search for.
	keyNode* next; //the next pointer in the key node to point to the next node in the key list class.
	//for the default constructor we will initialize both strings to be empty.
	void keySetDetails(string a = " ", string b = " ") {
		hash = a;
		itemName = b;
		next = nullptr;
	}
};
class KeyList {
public:
	keyNode* head; //root point to the start of the list.
	int counter; //counter variable that helps keep track of how many inserted elements.
	KeyList() {
		//default constructor that will allocate null value to root and set counter to zero.
		head = nullptr;
		counter = 0;
	}
	bool isEmpty() {
		if (head == nullptr) {
			return true;
		}
		return false;
	}
	//insert a new node
	void insert(string hash, string file) {

		if (isEmpty()) {
			head = new keyNode;
			head->hash = hash;
			head->itemName = file;
			head->next = nullptr;
			counter++;
		}
		else {
			keyNode* add = new keyNode;
			add->hash = hash;
			add->itemName = file;
			add->next = nullptr;
			keyNode* shift = head;
			while (shift->next != nullptr) {
				shift = shift->next;
			}
			shift->next = add;
			add = nullptr;
			delete add;
			counter++;
		}

	}
	void print() {
		keyNode* disp = head;
		for (int i = 0; i < counter; i++) {
			cout << disp->itemName;
			cout << endl;
			disp = disp->next;
		}
	}
	void Remove() { //FIFO
		if (isEmpty()) {
			return;
		}
		keyNode* temp = new keyNode;
		temp = head;
		head = head->next;
		counter--;
		delete temp;

	}
	void clear() {
		int sizeTemp = counter;
		for (int i = 0; i < sizeTemp; i++) {
			Remove();
		}
	}
	KeyList operator =(KeyList& input) {
		this->clear();
		keyNode* point = input.head;
		while (point != nullptr) {
			this->insert(point->hash, point->itemName);
			point = point->next;
		}
		return *this;
	}
	void pathFile(string& filepath) {
		keyNode* point = head;
		while (point != nullptr) {
			filepath = filepath.append(point->itemName);
			filepath = filepath.append("\n");
			point = point->next;
		}
	}
};
struct FileNode {
	string data;
	FileNode* next;
};

class FileList {
public:
	int counter;
	FileNode* head;

	FileList() {
		head = NULL;
		counter = 0;
	}
	///insertions
	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		return false;
	}
	void insertEND(string val) {
		if (isEmpty()) {
			head = new FileNode;
			head->data = val;
			head->next = nullptr;
			counter++;
		}
		else {
			FileNode* add = new FileNode;
			add->data = val;
			add->next = nullptr;
			FileNode* shift = head;
			while (shift->next != NULL) {
				shift = shift->next;
			}
			shift->next = add;
			add = nullptr;
			delete add;
			counter++;
		}
	}
	///insertions
	///deletions
	void Remove() { //FIFO
		if (isEmpty()) {
			return;
		}
		FileNode* temp = new FileNode;
		temp = head;
		head = head->next;
		counter--;
		delete temp;

	}
	void clear() {
		int sizeTemp = counter;
		for (int i = 0; i < sizeTemp; i++) {
			Remove();
		}
	}
	///deletions
	/// display
	void display(string extract) {
		FileNode* disp = head;
		for (int i = 0; i < counter; i++) {
			extract = disp->data;
			disp = disp->next;
		}
	}
	///display
	~FileList() {
		delete head;
		head = nullptr;
	}
};
//b tree node
//contains a structure array of the key.
//contains a pointer array of the children subject to degree of the tree.
class bTreeNode {
public:
	KeyList* keys; //the keyy array
	int degree; //the degree specifier per node.
	int keyNum; //the current number of keys present in a node.
	bTreeNode** children; //the children pointer array that will store the next pointer added to the tree.
	bool leaf; //indicator if a node is leaf node or no.
	bTreeNode(int deg, bool isLeaf) {
		keys = new KeyList[deg + 1];
		degree = deg;
		keyNum = 0;
		children = new  bTreeNode * [degree + 1];
		leaf = isLeaf;
	}
	//tree node functionalities since we cant break the actual call structure of a b tree using the universal method conventions.


};


//b tree itself
class bTree {
public:
	bTreeNode* root; //root node of the tree. will help in growing the tree.
	int tDeg; //degree of the tree that can be made.
	int hashFound; //a index that will be used for searching a duplicate hash value and if found it will add it to the keys linked list.
	bool insertInNew;
	bool hashExists;
	int height;
	//what if the split was called from the root node. this means an increase in the height of the tree. 
	bTree(int degree) { //constructor for the tree. Since we need to specify the size during the run time for creation.
		root = NULL;
		tDeg = degree;
		hashFound = 0;
		insertInNew = false;
		hashExists = false;
		height = 0;
		//what if the height of the tree exceeds 2. In this case we cannot follow the common insertion conventions.
	}
	void insert(string hash, string item) {
		//search for hash to avoid any mishaps in the insertions.
		//if hash exists then we will run the hash found function for every node to find the correct position for the hash.
		// else we will skip all hash found functions since the following hash will be a new hash.
		hashExists = isNewHash(root, hash);
		//initializer if the tree is empty.
		if (root == NULL) {
			root = new bTreeNode(tDeg, true);
			root->keys[0].insert(hash, item);
			root->keyNum = 1;
			height = 1;
		}
		//if the root is full we will proceed to grow the tree.
		else if (root->keyNum == tDeg - 1) {
			//create a new node , let this be called the calling node through which we will reset our structure.
			height += 1; //if height exceeds 2 only add the value to the right specific child. else you will do the same procedures.
			if (hashExists && searchForHash(root, hash)) {
				root->keys[hashFound].insert(hash, item);
			}
			else {
				bTreeNode* callinNode = new bTreeNode(tDeg, false);
				//make old root into a child of the new root
				callinNode->children[0] = root;
				//split the root.
				//through the calling node.
				split(callinNode, 0, root, hash, item);

				int i = 0;
				if (callinNode->keys[0].head->hash < hash) {
					i++;
				}
				//what if the tree grows in height beyond two. Then we cannot store the value inside the root and transfer it.
				NotFull(callinNode->children[i], hash, item);

				root = callinNode;
				//insertInNew = false;
			}
		}
		else {
			//root is not full so we are inserting into the root.
			//first checking if the hash already exists in the root.
			if (searchForHash(root, hash)) {
				root->keys[hashFound].insert(hash, item);
			}
			else {
				NotFull(root, hash, item);
			}
		}
	}
	//method to split children
	void split(bTreeNode* caller, int cIndex, bTreeNode* input, string hash, string item) {
		//the parametre conventions are as follows-> caller is the node that will be used to adjust, cIndex is the index where the current child lies
		//input is the node that will be used as the reference to fill values for a new node that is to be created.
		bTreeNode* newNode = new bTreeNode(input->degree, input->leaf);
		//newNode->keyNum = caller->degree - 1;
		//copying keys of internal node to the newNode
		int i = tDeg / 2;
		if (tDeg % 2 == 0) {
			i -= 1;
		}
		int median = i;
		int j = 0;
		//incase the calling node was not a leaf node we will do the preemtive split algorithm and just make a new root then proceed to add the child.
		if (!input->leaf) {
			for (i, j; i < caller->degree - 1; i++, j++) {
				newNode->keys[j] = input->keys[i];
				newNode->keyNum++;
			}
		}
		//incase the calling node was a leaf itself.
		//median value will be incremented as median is no longer required in the new node.
		else if (input->keys[i - 1].head->hash > hash) { //means that the median was found in the right position and no change is needed.
			for (i, j; i < caller->degree - 1; i++, j++) {
				newNode->keys[j] = input->keys[i];
				newNode->keyNum++;
			}
		}
		else {
			//if the median was not found in the right position. This is all without actually inserting the hash into the main nodes.
			//then it means that the next value was meant to be the median as the shift never happened we are working on an assumption basis.
			insertInNew = true;
			median += 1;
			i += 1;
			for (i, j; i < caller->degree - 1; i++, j++) {
				newNode->keys[j] = input->keys[i];
				newNode->keyNum++;
			}
			//insert the specific node which needs to be inserted in the newNode.
			newNode->keys[j].insert(hash, item);
			newNode->keyNum++;
			//sort the new node. with the values now in the new node all we have to do is sort it such that the newly attached node does not have faulty values.
			input->keyNum += 1;
			KeyList temp;
			//sorting.
			int minIdx = 0;
			for (int i = 0; i < newNode->keyNum - 1; i++) {
				minIdx = i;
				for (int j = i + 1; j < newNode->keyNum; j++) {
					if (newNode->keys[j].head->hash < newNode->keys[minIdx].head->hash) {
						minIdx = j;
					}
				}
				if (minIdx != i) {
					temp = newNode->keys[minIdx];
					newNode->keys[minIdx] = newNode->keys[i];
					newNode->keys[i] = temp;
				}
			}
		}
		//reducing the number of keys in the input node by 1. Since it was full it will go down by 1.
		if (!input->leaf) {
			//copying the last children of input node if input node was not a leaf since we are constantly splitting.
			for (int i = 0; i <= tDeg / 2; i++) {
				newNode->children[i] = input->children[i + 1];
			}
		}
		int numKeysLost = newNode->keyNum + 1;
		input->keyNum -= numKeysLost;
		//cout << "Caller knum: " << caller->keyNum << endl;
		//creating space for a new child.
		for (int i = caller->keyNum; i >= cIndex + 1; i--) {
			caller->children[i + 1] = caller->children[i];
		}
		//the new child will be linked to the new node.
		caller->children[cIndex + 1] = newNode;

		//the caller node will have a key of input be transfered to it.
		//shifting values so that space is created for that value.
		for (int i = (caller->degree - 2); i >= cIndex; i--) {
			caller->keys[i + 1] = caller->keys[i];

		}

		//adding that value to the calling node.
		//shift the median key up.
		caller->keys[cIndex] = input->keys[median - 1];


		caller->keyNum += 1;
	}
	//method to insert into a non full node i.e could be a leaf or could not be a leaf.
	void NotFull(bTreeNode* adder, string hash, string item) {
		//initializing value to right most element.
		int index = adder->keyNum - 1;

		//check if leaf node or not.
		if (adder->leaf) {
			if (!insertInNew) {
				if (hashExists && searchForHash(root, hash)) { //these searches will only run if the hash was already found to be inside the tree.
					adder->keys[hashFound].insert(hash, item);
				}
				else {
					//shifting all greater values to the right.
					while (index >= 0 && adder->keys[index].head->hash > hash) {
						adder->keys[index + 1] = adder->keys[index];
						index--;
					}
					//inserting the new key.
					adder->keys[index + 1].clear();
					adder->keys[index + 1].insert(hash, item);

					adder->keyNum += 1;
				}
			}
		}
		//if not leaf node.
		else {
			//check if index exists
			insertInNew = false;
			if (hashExists && searchForHash(root, hash)) {
				adder->keys[hashFound].insert(hash, item);
			}
			else {
				//find index without shifting.
				if (index != -1) {
					while (index >= 0 && adder->keys[index].head->hash > hash) {
						index--;
					}
				}
				//check if the child at the current required index is full or not.
				if (adder->children[index + 1]->keyNum == adder->degree - 1) {
					//insertInNew = false;
					if (!hashExists) {
						adder->keyNum -= 1;
					}
					split(adder, index + 1, adder->children[index + 1], hash, item);
					//see which child is going to have the new key the smaller batch or the greater batch.
					if (adder->keys[index + 1].head->hash > hash) {
						index++;
					}

				}
				//calling an insert for the child now.
				//insert the new key into the child.

				NotFull(adder->children[index + 1], hash, item);
			}
		}
		insertInNew = false;
	}
	//traversing or searching.
	void traverse(bTreeNode* path) {
		int i = 0;
		for (i = 0; i < path->keyNum; i++) {
			if (!path->leaf) {
				traverse(path->children[i]);
			}
			cout << "index: " << i << endl;
			path->keys[i].print();
		}
		//print sub tree
		if (!path->leaf) {
			traverse(path->children[i]);
		}

	}
	//search in the tree
	bool search(bTreeNode* find, string hash, string& path) {
		int i = 0; //searching index
		while (i<find->keyNum && hash > find->keys[i].head->hash) {
			i++;
		}
		if (hash == find->keys[i].head->hash) {

			return true;
		}
		if (find->leaf) {
			return false;
		}

		return search(find->children[i], hash, path);
	}
	bool searchForHash(bTreeNode* find, string hash) {
		int i = 0;
		while (i<find->keyNum && hash > find->keys[i].head->hash) {
			i++;
		}
		if (find->keys[i].head != NULL) {
			if (hash == find->keys[i].head->hash) {
				hashFound = i;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}

	}
	bool isNewHash(bTreeNode* find, string hash) {
		int i = 0;
		if (root != NULL) {
			while (i<find->keyNum && hash > find->keys[i].head->hash) {
				i++;
			}

			if (find->keys[i].head != NULL) {
				if (hash == find->keys[i].head->hash) {
					hashFound = i;
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}

	}
	//deleting files and folders. //Need to ensure that the node does not get too small during deletion.
	void Delete(bTreeNode* input, string hash) {
		//finding the index of the key that has to be removed.
		int indexKey = getIndexOfKey(input, hash);

		//if key to be removed is present in one of the current node indexes.
		if (indexKey < input->keyNum && input->keys[indexKey].head->hash == hash) {
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
			if (input->children[indexKey]->keyNum < (input->degree / 2) - 1) {
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
	void LeafRemove(bTreeNode* input, int index) { //remove the value from a leaf.
		for (int i = index + 1; i < input->keyNum; i++) {
			input->keys[i - 1] = input->keys[i];

		}
		input->keyNum--;
	}
	void InternalRemove(bTreeNode* input, int index) { //remove a value from an internal node
		//getting a hash value to be used.
		string tempHash = input->keys[index].head->hash;
		//if child has atleast t keys.
		if (input->children[index]->keyNum >= input->degree / 2) {
			KeyList predecessor = getPredecessor(input, index); //get the predecessor. contains both hash and itemName.
			input->keys[index] = predecessor;
			Delete(input->children[index], predecessor.head->hash);
		}
		//examine the next child if the previous child does not meet the criteria.
		else if (input->children[index + 1]->keyNum >= input->degree / 2) {
			//get successor.
			KeyList successor = getSuccessor(input, index);
			input->keys[index] = successor;
			Delete(input->children[index], successor.head->hash);
		}
		else {
			//calling the merge function.
			merge(input, index);
			Delete(input->children[index], tempHash);
		}
	}
	KeyList getPredecessor(bTreeNode* input, int index) { //retrieve the value of the predecessor. Of a certain key or hash
		bTreeNode* finder = input->children[index];
		while (!finder->leaf) {
			finder = finder->children[finder->keyNum]; //keep going right till value of that index is specified at a leaf node.
		}

		return finder->keys[finder->keyNum - 1];
	}
	KeyList getSuccessor(bTreeNode* input, int index) { //retireve the value of a successor of a certain key or hash.
		bTreeNode* finder = input->children[index + 1];
		while (!finder->leaf) {
			finder = finder->children[0]; //keep moving left till you reach the required left most value at a leaf node.
		}
		return finder->keys[0];
	}
	void FillChild(bTreeNode* input, int index) { //method to complete a child node that has less than t-1 keys.
		if (index != 0 && input->children[index - 1]->keyNum >= input->degree / 2) {
			borrowPrev(input, index);
		}
		else if (index != input->keyNum && input->children[index + 1]->keyNum >= input->degree / 2) {
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
	void borrowNext(bTreeNode* input, int index) { //borrow a value from the i+1 sibling.
		bTreeNode* child = input->children[index];
		bTreeNode* sibling = input->children[index + 1];
		child->keys[child->keyNum] = input->keys[index];

		//first child of the sibling becomes the last child of the currently chosen child.
		if (!child->leaf) {
			child->children[child->keyNum + 1] = sibling->children[0];
		}
		//key from subling is inserted into the main node.
		input->keys[index] = sibling->keys[0];

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
	void borrowPrev(bTreeNode* input, int index) { //borrow a value from the i-1 sibling.
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
	void merge(bTreeNode* input, int index) { //method to merge two children into one node.
		//creating a child and its sibling.
		bTreeNode* child = input->children[index];
		bTreeNode* sibling = input->children[index + 1];

		child->keys[input->degree - 2] = input->keys[index];

		//push all keys from the sibling to the child.
		for (int i = 0; i < sibling->keyNum; i++) {
			child->keys[i] = sibling->keys[i];
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
		sibling = NULL;
	}
	int getIndexOfKey(bTreeNode* input, string hash) {
		int index = 0;
		while (index < input->keyNum && hashing(input->keys[index].head->hash) < hashing(hash)) {
			index++;
		}
		return index;
	}
	//another approach at insertion.
	void insert2(string hash, string item) {
		root = insert(hash, item, root);
	}
	bTreeNode* insert(string hash, string item, bTreeNode* root) {
		KeyList i;
		bTreeNode* c = NULL; //a node to hold value i guess.
		bTreeNode* n; //new node im guessing.
		int condition = setAValue(hash, item, root, i, &c);
		if (condition) {
			n = new bTreeNode(tDeg, true);
			n->keyNum = 1;
			n->keys[1] = i;
			n->children[0] = root;
			n->children[1] = c;
			return n;
		}
		return root; //retrun the root back to be set accordingly.
	}
	int setAValue(string hash, string item, bTreeNode* newNode, KeyList& value, bTreeNode** c) { //sets a value inside the node and then returns a zero or one. Acts as a boolean.
		int k = 0;
		if (newNode == NULL) {
			value.insert(hash, item);
			c = NULL;
			return 1;
		}
		else {
			if (SearchForNode(hash, newNode, k)) {
				newNode->keys[hashFound].insert(hash, item);
				//k = hashFound;
			}
			else if (setAValue(hash, item, newNode->children[k], value, c)) {
				if (newNode->keyNum < tDeg) {
					nodeFill2(value, *c, newNode, k);
					return 0;
				}
				else {
					split2(hash, item, *c, newNode, k, value, c);
					return 1;
				}
			}
			return 0;
		}
	}

	int SearchForNode(string hash, bTreeNode* input, int& index) { //dont need to searchfor this now as we alr have a hash finding function already.
		if (input != NULL) {
			if (hashing(hash) < hashing(input->keys[1].head->hash)) {
				cout << input->keys[1].head->itemName << endl;
				index = 0;
				return 0;
			}
			else {
				index = input->keyNum;
				while (hashing(hash) < hashing(input->keys[index].head->hash) && index > 1) {
					cout << input->keys[index].head->itemName << endl;
					index--;
				}
				if (hashing(hash) == hashing(input->keys[index].head->hash)) {
					cout << input->keys[index].head->itemName << endl;
					hashFound = index;
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	}
	void nodeFill2(KeyList& value, bTreeNode* input, bTreeNode* input2, int index) {
		int i = 0;
		for (i = input2->keyNum; i > index; i--) {
			input2->keys[i + 1] = input2->keys[i]; //shift keys
			input2->children[i + 1] = input2->children[i]; //shift children.
		}
		input2->keys[index + 1].clear();
		input2->keys[index + 1].insert(value.head->hash, value.head->itemName);
		input2->children[index + 1] = input;
		input2->keyNum++;
	}
	void split2(string hash, string item, bTreeNode* input, bTreeNode* input2, int index, KeyList& value, bTreeNode** New) {
		int i = 0, middle = 0;
		if (index <= ceil((tDeg * 1.0) / 2)) {
			middle = ceil((tDeg * 1.0) / 2);
		}
		else {
			middle = ceil((tDeg * 1.0) / 2) + 1;
		}

		*New = new bTreeNode(tDeg, false);
		for (i = middle + 1; i <= tDeg; i++) {
			(*New)->keys[i - middle] = input2->keys[i];
			(*New)->children[i - middle] = input2->children[i];
		}
		(*New)->keyNum = tDeg - middle;
		input2->keyNum = middle;
		if (index <= ceil((tDeg * 1.0) / 2)) {
			nodeFill2(value, input, input2, index);
		}
		else {
			nodeFill2(value, input, *New, index - middle);
		}
		value = input2->keys[input2->keyNum];
		(*New)->children[0] = input2->children[input2->keyNum];
		input2->keyNum--;
	}
	void Display(bTreeNode* root) {
		int i = 0;
		if (root != NULL) {
			for (i = 0; i < root->keyNum; i++) {
				Display(root->children[i]);
				cout << "index: " << i + 1 << " , ";
				root->keys[i + 1].print();
			}
			Display(root->children[i]);
		}
	}
	//deletion for the new insertion algorithm start here.
	void remove(string hash) {
		root = remove(hash, root);
	}
	bTreeNode* remove(string hash, bTreeNode* root) {
		bTreeNode* temp = NULL;
		if (!findIndex(hash, root)) {
			cout << "THE FILES DO NOT EXIST IN THE SYSTEM" << endl;
		}
		else {
			if (root->keyNum == 0) {
				temp = root;
				root = root->children[0];
				delete temp;
			}
		}
		return root;
	}
	bool findIndex(string hash, bTreeNode* input) {
		int i;
		bool condition;
		if (input == NULL) {
			return false;
		}
		else {
			condition = SearchForNode(hash, input, i);
			if (condition) {
				if (input->children[i - 1]) {
					Successor(input, i);
					condition = findIndex(input->keys[i].head->hash, input->children[i]);
					if (!condition) {
						cout << "VALUE DOES NOT EXIST IN THE DRIVE" << endl;
					}
				}
				else {
					clear(input, i);
				}
			}
			else {
				condition = findIndex(hash, input->children[i]);
			}
			if (input->children[i] != NULL) {
				if (input->children[i]->keyNum < ceil((tDeg * 1.0) / 2) - 1) {
					Undo(input, i);
				}
			}
			return condition;
		}
	}
	void clear(bTreeNode* input, int index) {
		int i;
		for (i = index + 1; i <= input->keyNum; i++) {
			input->keys[i - 1] = input->keys[i];
			input->children[i - 1] = input->children[i];
			cout << input->keys[i - 1].head->itemName << endl;
		}
		input->keyNum--;
	}
	void Successor(bTreeNode* input, int value) {
		bTreeNode* temp;
		temp = input->children[value];
		while (temp->children[0]) {
			temp = temp->children[0];
		}
		//temp->keyNum--;
		input->keys[value] = temp->keys[1];
	}
	void Undo(bTreeNode* input, int value) { //restore the value
		cout << input->keys[1].head->hash << endl;
		if (value == 0) {
			if (input->children[1]->keyNum > ceil((tDeg * 1.0) / 2)) {
				left(input, 1);
			}
			else {
				cout << input->keys[1].head->itemName << endl;
				Combine(input, 1);
			}
		}
		else {
			if (value == input->keyNum) {
				if (input->children[value - 1]->keyNum > ceil((tDeg * 1.0) / 2)) {
					right(input, value);
				}
				else {
					Combine(input, value);
				}
			}
			else {
				if (input->children[value - 1]->keyNum > ceil((tDeg * 1.0) / 2)) {
					right(input, value);
				}
				else {
					if (input->children[value + 1]->keyNum > ceil((tDeg * 1.0) / 2)) {
						left(input, value + 1);
					}
					else {
						cout << input->keys[1].head->itemName << endl;
						Combine(input, value);
					}
				}
			}
		}
	}
	void right(bTreeNode* input, int value) {
		int i;
		bTreeNode* placeholder;
		placeholder = input->children[value];
		for (i = placeholder->keyNum; i > 0; i--) {
			placeholder->keys[i + 1] = placeholder->keys[i];
			placeholder->children[i + 1] = placeholder->children[i];
		}
		placeholder->children[1] = placeholder->children[0];
		placeholder->keyNum++;
		placeholder->keys[1] = input->keys[value];
		placeholder = input->children[i - 1];
		input->keys[value] = placeholder->keys[placeholder->keyNum];
		input->children[value]->children[0] = placeholder->children[placeholder->keyNum];
		placeholder->keyNum--;
	}
	void left(bTreeNode* input, int value) {
		int i;
		bTreeNode* placeholder;
		placeholder = input->children[value - 1];
		placeholder->keyNum++;
		placeholder->keys[placeholder->keyNum] = input->keys[value];
		placeholder->children[placeholder->keyNum] = input->children[value]->children[0];
		placeholder = input->children[value];
		input->keys[value] = placeholder->keys[1];
		placeholder->children[0] = placeholder->children[1];
		placeholder->keyNum--;
		for (i = 1; i <= placeholder->keyNum; i++) {
			placeholder->keys[i] = placeholder->keys[i + 1];
			placeholder->children[i] = placeholder->children[i + 1];
		}
	}
	void Combine(bTreeNode* input, int value) {
		int i;
		bTreeNode* placeholder;
		bTreeNode* placeholder2;
		placeholder = input->children[value];
		cout << "placeholder1: " << placeholder->keys[1].head->itemName << endl;
		placeholder2 = input->children[value - 1];
		cout << "placeholder1: " << placeholder2->keys[1].head->itemName << endl;
		placeholder2->keyNum++;
		placeholder2->keys[placeholder2->keyNum] = input->keys[value];
		if (placeholder2->children[0] != NULL) {
			placeholder2->children[placeholder2->keyNum] = placeholder->children[0];
		}
		for (i = 1; i <= placeholder->keyNum; i++) {
			placeholder2->keyNum++;
			placeholder2->keys[placeholder2->keyNum] = placeholder->keys[i];
			placeholder2->children[placeholder2->keyNum] = placeholder->children[i];
		}
		for (i = value; i < input->keyNum; i++) {
			input->keys[i] = input->keys[i + 1];
			input->children[i] = input->children[i + 1];
		}
		input->keyNum--;
		delete placeholder;
	}

	int hashing(string hash, int max = -1) //hashing function is part of the b tree.
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
		//cout << "HASH: " << hash << endl;
		//cout << "RETURNED VALUE: " << result << endl;
		return result;
	}
	void Redistribute(bTreeNode* root, bTree& newTree, int r1, int r2, int iSpace, FileList& hashes) { //r1 and r2 are the ranges of the machines.
		int i = 0;
		if (root != NULL) {
			for (i = 0; i < root->keyNum; i++) {
				Redistribute(root->children[i], newTree, r1, r2, iSpace, hashes);
				if (hashing(root->keys[i + 1].head->hash, iSpace) > r1 && hashing(root->keys[i + 1].head->hash, iSpace) <= r2) {
					newTree.copyList(root->keys[i + 1]);
					hashes.insertEND(root->keys[i + 1].head->hash);
				}
				if (root == NULL) {
					break;
				}
			}
			if (root->children[i] != NULL) {
				Redistribute(root->children[i], newTree, r1, r2, iSpace, hashes);
			}
		}
	}
	void Transfer(bTree& newTree, int r1, int r2, int iSpace, FileList& hashes) {
		Redistribute(root, newTree, r1, r2, iSpace, hashes);
	}
	//new tree will call a function to traverse through a list. and copy all of its contents.
	void copyList(KeyList list) {
		keyNode* point = list.head;
		while (point != nullptr) {
			this->insert2(point->hash, point->itemName);
			point = point->next;
		}
	}
	bool ExtractFiles(bTreeNode* find, string hash, string& path, int& index) {
		if (find == NULL) {
			cout << "THE VALUE DOES NOT EXIST" << endl;
			return false;
		}
		if (SearchForNode(hash, find, index)) {
			find->keys[index].pathFile(path);
			return true;
		}
		else {
			ExtractFiles(find->children[index], hash, path, index);
		}

	}
	void RemoveRange(FileList& hashes) {
		FileNode* point = hashes.head;
		while (point != NULL) {
			this->remove(point->data);
			point = point->next;
		}
	}
	friend class KeyList;
