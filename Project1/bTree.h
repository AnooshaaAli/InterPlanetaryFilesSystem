#include<iostream>
#include<string>
#include <queue>
using namespace std;

// ---------------------------------------------------- appendZeroes ------------------------------------------------- //

void appendZeroes(string& str1, string& str2) {
	int s1 = str1.size();
	int s2 = str2.size();

	if (s1 < s2)
	{
		int diff = s2 - s1;
		string zero = "";
		for (int i = 0; i < diff; i++)
		{
			zero += "0";
		}
		str1 = zero + str1;
	}

	else
	{
		int diff = s1 - s2;
		string zero = "";
		for (int i = 0; i < diff; i++)
		{
			zero += "0";
		}
		str2 = zero + str2;
	}
}

// -------------------------------------- is greater equal --------------------------------------------------- //

bool isGreaterEqual(string str1, string str2) {
	appendZeroes(str1, str2);
	return(str1 >= str2);
}

// -------------------------------------- is greater --------------------------------------------------- //

bool isGreater(string str1, string str2) {
	appendZeroes(str1, str2);
	return(str1 > str2);
}

class PathNode
{
public:
	string path;
	PathNode* next;
	PathNode(string p = "")
	{
		path = p;
		next = NULL;
	}
	//Copy constructor
	PathNode(const PathNode& other)
	{
		path = other.path;
		next = other.next;
	}
	//Assignment operator
	PathNode& operator=(const PathNode& other)
	{
		if (this != &other) {
			path = other.path;
			next = other.next;
		}
		return *this;
	}
};

class Data {
public:
	string key;
	PathNode* paths;
	Data(string k = "")
	{
		key = k;
		paths = new PathNode;
	}
};

class BTreeNode
{
public:

	int numKeys;               // Number of keys in the node
	Data** pairs;              // Array to store keys
	BTreeNode** children;      // Array to store child pointers
	int MAX;
	int MIN;

	BTreeNode(int maxDegree)
	{
		MAX = maxDegree - 1;
		MIN = MAX / 2;
		pairs = new Data * [MAX + 1];
		for (int i = 0; i < MAX + 1; i++)
		{
			pairs[i] = new Data;
		}
		children = new BTreeNode * [MAX + 1];
		for (int i = 0; i < MAX + 1; i++)
			children[i] = NULL;
		numKeys = 0;
	}

	// Function to insert a value into a B-tree
	BTreeNode* insertValue(string value, string path, BTreeNode* root, int maxKey)
	{
		BTreeNode* newChild, * newNode;
		Data* node = new Data;

		int splitFlag = setValue(value, path, root, &(*node), &newChild);

		if (splitFlag)
		{
			// If a split occurred, create a new root
			newNode = new BTreeNode(maxKey);
			newNode->numKeys = 1;
			newNode->pairs[1]->key = node->key;
			newNode->pairs[1]->paths = node->paths;
			newNode->children[0] = root;
			newNode->children[1] = newChild;
			return newNode;
		}

		// If no split occurred, return the original root
		return root;
	}

	// Function to set the value in a B-tree node during insertion
	int setValue(string value, string path, BTreeNode* node, Data* d, BTreeNode** newChild) {
		int keyIndex;
		// If the current node is null, set the value and return
		if (node == NULL)
		{
			d->key = value;
			d->paths->path = path;
			*newChild = NULL;
			return 1;
		}
		else
		{
			// Search for the appropriate position for the value
			if (searchNode(value, node, &keyIndex))
			{
			}

			// Recursively set the value in the child node
			if (setValue(value, path, node->children[keyIndex], d, newChild))       //iss ka bhi dekhna hai
			{
				if (node->numKeys < MAX)
				{
					// If there is space, insert the value directly
					insertIntoNode(&(*d), *newChild, node, keyIndex);
					return 0;
				}
				else
				{
					// If the node is full, split it
					splitNode(&(*d), *newChild, node, keyIndex, newChild);
					return 1;
				}
			}
			return 0;
		}
	}

	// Function to insert into a node
	void insertIntoNode(Data* d, BTreeNode* child, BTreeNode* node, int keyIndex) {
		int i;

		// Check if the key already exists
		if (keyIndex > 0 && d->key == node->pairs[keyIndex]->key)
		{
			updatePath(&(*d), node->pairs[keyIndex]->paths);
			//numKeys++;
			return;
		}

		// Shift values to make space for the new value
		for (i = node->numKeys; i > keyIndex; i--)
		{
			node->pairs[i + 1]->key = node->pairs[i]->key;
			node->pairs[i + 1]->paths = node->pairs[i]->paths;
			node->children[i + 1] = node->children[i];
		}

		// Insert the new value and update the child pointer
		node->pairs[keyIndex + 1]->key = d->key;
		node->pairs[keyIndex + 1]->paths = d->paths;
		node->children[keyIndex + 1] = child;
		node->numKeys++;
	}

	// Function to split a B-tree node during insertion
	void splitNode(Data* d, BTreeNode* child, BTreeNode* node, int keyIndex, BTreeNode** newChild)
	{
		int i, mid;

		// Determine the midpoint for splitting
		if (keyIndex <= MIN)
		{
			mid = MIN;
		}
		else
		{
			mid = MIN + 1;
		}

		// Allocate memory for the new node
		*newChild = new BTreeNode(node->MAX + 1);

		// Move values and child pointers to the new node
		for (i = mid + 1; i <= node->MAX; i++)
		{
			(*newChild)->pairs[i - mid]->key = node->pairs[i]->key;
			(*newChild)->pairs[i - mid]->paths = node->pairs[i]->paths;
			(*newChild)->children[i - mid] = node->children[i];
		}

		// Update the number of keys in the new node
		(*newChild)->numKeys = node->MAX - mid;
		node->numKeys = mid;

		// Determine where to insert the new value
		if (keyIndex <= MIN)
		{
			insertIntoNode(&(*d), child, node, keyIndex);
		}
		else
		{
			insertIntoNode((&*d), child, *newChild, keyIndex - mid);
		}

		// Update the value and path to be passed up to the parent
		d->key = node->pairs[node->numKeys]->key;
		d->paths = node->pairs[node->numKeys]->paths;

		// Update the child pointer for the new node
		(*newChild)->children[0] = node->children[node->numKeys];
		node->numKeys--;
	}

	// Function to update the path for a given key
	void updatePath(Data* d, PathNode*& pathNode)
	{
		PathNode* newNode = new PathNode(d->paths->path);
		newNode->next = pathNode;
		pathNode = newNode;
	}

	// Function to search for a value in the B-tree
	BTreeNode* searchValue(string value, BTreeNode* root, int* position) {
		if (root == NULL) {
			return NULL;
		}
		else {
			if (searchNode(value, root, position)) {
				return root;
			}
			else {
				return searchValue(value, root->children[*position], position);
			}
		}
	}

	// Function to search for a value within a B-tree node
	int searchNode(string value, BTreeNode* node, int* position) {
		if (value.compare(node->pairs[1]->key) < 0) {
			*position = 0;
			return 0;
		}
		else {
			*position = node->numKeys;
			while ((value.compare(node->pairs[*position]->key) < 0) && *position > 1)
			{
				(*position)--;
			}

			if (value.compare(node->pairs[*position]->key) == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	// function for print the B-tree
	void printTree()
	{
		printTreeHelper(this, 0);
	}

	// Recursive helper function to print the B-tree structure
	void printTreeHelper(BTreeNode* node, int level)
	{
		if (node != NULL)
		{
			// Print the current node
			cout << "Level " << level << ": ";
			for (int i = 1; i <= node->numKeys; i++)
			{
				cout << node->pairs[i]->key << "[" << node->pairs[i]->paths->path;

				// Print chained paths
				PathNode* currentPathNode = node->pairs[i]->paths->next;
				while (currentPathNode != NULL)
				{
					cout << "->" << currentPathNode->path;
					currentPathNode = currentPathNode->next;
				}

				cout << "] ";
			}
			cout << endl;

			// Print child pointers
			if (node->children[0] != NULL)
			{
				for (int i = 0; i <= node->numKeys; i++)
				{
					cout << "Child " << i << ": ";
					if (node->children[i] != NULL)
					{
						for (int j = 1; j <= node->children[i]->numKeys; j++)
						{
							cout << node->children[i]->pairs[j]->key << "[" << node->children[i]->pairs[j]->paths->path;

							// Print chained paths for child node
							PathNode* currentChildPathNode = node->children[i]->pairs[j]->paths->next;
							while (currentChildPathNode != NULL)
							{
								cout << "->" << currentChildPathNode->path;
								currentChildPathNode = currentChildPathNode->next;
							}

							cout << "] ";
						}
					}
					cout << endl;
				}
			}
		}
	}
	// Recursive function to print the paths associated with a given hash value in the entire tree
	void printPathsAtGivenHash(BTreeNode* node, string hash)
	{
		if (node != NULL)
		{
			for (int i = 1; i <= node->numKeys; i++)
			{
				// Check if the key matches the specified hash value
				if (node->pairs[i]->key == hash)
				{
					// Print chained paths
					PathNode* currentPathNode = node->pairs[i]->paths->next;
					while (currentPathNode != NULL)
					{
						cout << " " << currentPathNode->path;
						currentPathNode = currentPathNode->next;
					}
				}

				// Recursively search in the child node
				printPathsAtGivenHash(node->children[i], hash);
			}
		}
	}

	// Wrapper function to start the search from the root
	void printPathsAtGivenHashWrapper(BTreeNode* root, string hash)
	{
		printPathsAtGivenHash(root, hash);
		cout << endl;
	}

	PathNode* deleteKeyReturnPathFunction(string hash, BTreeNode* node)
	{
		if (node != NULL)
		{
			int i;
			for (i = 1; i <= node->numKeys && hash.compare(node->pairs[i]->key) > 0; i++)
			{
				// Find the appropriate position for the hash value
			}

			// Check if the key matches the specified hash value
			if (i <= node->numKeys && hash == node->pairs[i]->key)
			{
				// Return the paths associated with the deleted node
				return node->pairs[i]->paths;
			}

			// Recursively search in the child node
			if (i <= node->numKeys && hash.compare(node->pairs[i]->key) < 0 && node->pairs[i]->key != "")
			{
				PathNode* pathFromChild = deleteKeyReturnPathFunction(hash, node->children[i - 1]);
				if (pathFromChild != NULL)
				{
					return pathFromChild; // Return the path from the child if found
				}
			}
			else if (i <= node->numKeys + 1) // Check if it's within the range of children
			{
				return deleteKeyReturnPathFunction(hash, node->children[i]);
			}
		}

		return NULL; // Return NULL if the key is not found
	}

	// Recursive function to return the paths associated with a given hash value
	PathNode* hashReturnPathFunction(string hash, BTreeNode* node)
	{
		if (node != NULL)
		{
			int i;
			for (i = 1; i <= node->numKeys && hash.compare(node->pairs[i]->key) > 0; i++)
			{
				// Find the appropriate position for the hash value
			}

			// Check if the key matches the specified hash value
			if (i <= node->numKeys && hash == node->pairs[i]->key)
			{
				// Return the paths associated with the deleted node
				return node->pairs[i]->paths;
			}

			// Recursively search in the child node
			if (i <= node->numKeys && hash.compare(node->pairs[i]->key) < 0 && node->pairs[i]->key != "")
			{
				PathNode* pathFromChild = hashReturnPathFunction(hash, node->children[i - 1]);
				if (pathFromChild != NULL)
				{
					return pathFromChild; // Return the path from the child if found
				}
			}
			else if (i <= node->numKeys + 1) // Check if it's within the range of children
			{
				return hashReturnPathFunction(hash, node->children[i]);
			}
		}

		return NULL; // Return NULL if the key is not found
	}

	// Function to delete a key from the B-tree
	BTreeNode* deleteKey(string key, BTreeNode* root) {
		BTreeNode* temp;
		// Special case: if B-tree has only one key in the root and no children
		if (root->numKeys == 1 && root->children[0] == NULL && root->pairs[1]->key == key)
		{
			delete root->pairs[1];
			root->numKeys = 0;
			delete root;
			return NULL; // Key deleted
		}
		if (!deleteHelper(key, root))
		{
			cout << endl;
			cout << "Key " << key << " not found" << endl;
		}
		else
		{
			if (root->numKeys == 0)
			{
				temp = root;
				root = root->children[0];
				delete temp;
			}
		}
		return root;
	}

	int deleteHelper(string key, BTreeNode* root)
	{
		int i;
		int flag;
		if (root == NULL)
			return 0;
		else
		{
			flag = searchNode(key, root, &i);
			if (flag)
			{
				if (root->children[i] != NULL)
				{
					copySuccessor(root, i);
					flag = deleteHelper(root->pairs[i]->key, root->children[i]);
					if (!flag)
					{
						cout << endl;
						cout << "Key " << key << " not found" << endl;
					}
				}
				else
				{
					clearKey(root, i);
				}
			}
			else
			{
				flag = deleteHelper(key, root->children[i]);
			}

			if (root->children[i] != NULL)
			{
				if (root->children[i]->numKeys < MIN)
					restore(root, i);
			}
			return flag;
		}
	}

	// Removes the key from the node and adjusts the keys
	void clearKey(BTreeNode* node, int k)
	{
		int i;
		for (i = k + 1; i <= node->numKeys; i++)
		{
			node->pairs[i - 1]->key = node->pairs[i]->key;
			node->pairs[i - 1]->paths = node->pairs[i]->paths;
			node->children[i - 1] = node->children[i];
		}
		node->numKeys--;
	}

	// Copies the successor of the key that is to be deleted
	void copySuccessor(BTreeNode* node, int i)
	{
		BTreeNode* temp = node->children[i];

		// Check if the successor is in the same node
		if (temp->children[0] != NULL)
		{
			while (temp->children[0] != NULL)
				temp = temp->children[0];

			node->pairs[i]->key = temp->pairs[1]->key;
			node->pairs[i]->paths = temp->pairs[1]->paths;

			// Call deleteHelper to delete the key from the child node
			deleteHelper(node->pairs[i]->key, node->children[i]);
		}
		else
		{
			node->pairs[i]->key = temp->pairs[1]->key;
			node->pairs[i]->paths = temp->pairs[1]->paths;
			clearKey(temp, 1);
		}
	}


	// Adjusts the node
	void restore(BTreeNode* node, int i)
	{
		if (i == 0) {
			if (node->children[1]->numKeys > MIN)
				shiftLeft(node, 1);
			else
				mergeNodes(node, 1);
		}
		else
		{
			if (i == node->numKeys)
			{
				if (node->children[i - 1]->numKeys > MIN)
					shiftRight(node, i);
				else
					mergeNodes(node, i);
			}
			else
			{
				if (node->children[i - 1]->numKeys > MIN)
					shiftRight(node, i);
				else
				{
					if (node->children[i + 1]->numKeys > MIN)
						shiftLeft(node, i + 1);
					else
						mergeNodes(node, i);
				}
			}
		}
	}

	void shiftRight(BTreeNode* node, int k)
	{
		BTreeNode* temp = node->children[k];
		BTreeNode* tempRight = node->children[k + 1];

		// Shift values in temp to make space for the new key from parent
		temp->pairs[temp->numKeys + 1] = temp->pairs[temp->numKeys];
		temp->children[temp->numKeys + 2] = temp->children[temp->numKeys + 1];

		// Copy the key from the parent to temp
		temp->pairs[1] = node->pairs[k];
		temp->children[1] = tempRight->children[0];

		// Move the first key from tempRight to the parent
		node->pairs[k] = tempRight->pairs[1];

		// Adjust child pointers in tempRight
		tempRight->children[0] = tempRight->children[1];

		// Update the number of keys
		temp->numKeys++;
		tempRight->numKeys--;

	}

	void shiftLeft(BTreeNode* node, int k)
	{
		BTreeNode* tempLeft = node->children[k - 1];
		BTreeNode* temp = node->children[k];

		// Move the last key from tempLeft to the parent
		node->pairs[k - 1] = tempLeft->pairs[tempLeft->numKeys];
		node->pairs[k - 1]->paths = tempLeft->pairs[tempLeft->numKeys]->paths;

		// Move the last child pointer from tempLeft to temp
		temp->children[0] = tempLeft->children[tempLeft->numKeys];

		// Adjust the number of keys in tempLeft
		tempLeft->numKeys--;

		// Shift values in temp to make space for the new key from parent
		temp->pairs[temp->numKeys + 1] = temp->pairs[temp->numKeys];
		temp->children[temp->numKeys + 2] = temp->children[temp->numKeys + 1];

		// Copy the key from the parent to temp
		temp->pairs[1] = node->pairs[k];
		temp->children[1] = temp->children[0];

		// Move the first key from temp to the parent
		node->pairs[k] = temp->pairs[2];

		// Adjust child pointers in temp
		temp->children[0] = temp->children[1];

		// Update the number of keys
		temp->numKeys--;
	}

	void mergeNodes(BTreeNode* node, int k)
	{
		BTreeNode* tempLeft = node->children[k - 1];
		BTreeNode* tempRight = node->children[k];

		// Move the key from the parent to tempLeft
		tempLeft->numKeys++;
		tempLeft->pairs[tempLeft->numKeys] = node->pairs[k];
		tempLeft->pairs[tempLeft->numKeys]->paths = node->pairs[k]->paths;
		tempLeft->children[tempLeft->numKeys] = tempRight->children[0];

		// Copy keys from tempRight to tempLeft
		for (int i = 1; i <= tempRight->numKeys; i++)
		{
			tempLeft->numKeys++;
			tempLeft->pairs[tempLeft->numKeys] = tempRight->pairs[i];
			tempLeft->pairs[tempLeft->numKeys]->paths = tempRight->pairs[i]->paths;
			tempLeft->children[tempLeft->numKeys] = tempRight->children[i];
		}

		// Shift keys in the parent to fill the gap
		for (int i = k; i < node->numKeys; i++)
		{
			node->pairs[i] = node->pairs[i + 1];
			node->children[i + 1] = node->children[i + 2];
		}

		// Adjust the number of keys in the parent
		node->numKeys--;

		// Delete tempRight
		delete tempRight;
	}
};

class BTree
{
	BTreeNode* root;
	int maxKey;
public:
	BTree(int k)
	{
		root = NULL;
		maxKey = k;
	}

	BTreeNode* getRoot() const {
		return root;
	}

	void insert(string val, string p)
	{
		root = root->insertValue(val, p, root, maxKey);
	}
	PathNode* deletion(string val)
	{
		PathNode* deletedNodePath = NULL;
		// Special case: if B-tree has only one key in the root and no children
		if (root->numKeys == 1 && root->children[0] == NULL && root->pairs[1]->key == val)
		{
			deletedNodePath = root->pairs[1]->paths; // Key deleted
		}
		else if (root->numKeys == 1 && root->children[0] == NULL && root->pairs[1]->key != val)
		{
			deletedNodePath = NULL; // Key deleted
		}
		else
		{
			deletedNodePath = root->deleteKeyReturnPathFunction(val, root);
		}
		if (deletedNodePath != NULL)
		{
			root = root->deleteKey(val, root);
		}
		else
		{
			cout << "Key with following hash " << val << " not found. " << endl;
		}
		return NULL;
	}
	void display()
	{
		root->printTree();
	}
	void printPathsAtGivenHash(string hash)
	{
		root->printPathsAtGivenHashWrapper(root, hash);
	}

	PathNode* findPath(string key, BTreeNode* root) {
		queue<BTreeNode*> bfsQueue;
		bfsQueue.push(root);

		while (!bfsQueue.empty()) {
			BTreeNode* currentNode = bfsQueue.front();
			bfsQueue.pop();

			int position;
			if (currentNode->searchNode(key, currentNode, &position)) {
				if (position > 0 && currentNode->pairs[position]->key == key) {
					return currentNode->pairs[position]->paths;
				}
			}

			for (int i = 0; i <= currentNode->numKeys; ++i) {
				if (currentNode->children[i] != nullptr) {
					bfsQueue.push(currentNode->children[i]);
				}
			}
		}

		return nullptr; // Return nullptr if the key is not found
	}

};
