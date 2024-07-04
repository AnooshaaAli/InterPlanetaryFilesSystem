#pragma once
#include<iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>
#include "bTree.h"
using namespace std;

// ------------------------------------------------ Print Path --------------------------------------------------------------- //

// Recursive function to print paths in reverse order
void printPath(PathNode* pathNode) {
	if (pathNode == nullptr) {
		return;
	}

	// Recursively print the rest of the list
	printPath(pathNode->next);

	// Print the current path
	cout << pathNode->path << " -> ";
}

// --------------------------------------------------- Function to add two binary numbers ---------------------------------------------------------- //

std::string addBinary(const std::string& binary1, const std::string& binary2) {
	// Make the lengths of the two binary numbers equal by adding leading zeros
	int maxLength = max(binary1.length(), binary2.length());
	std::string paddedBinary1 = std::string(maxLength - binary1.length(), '0') + binary1;
	std::string paddedBinary2 = std::string(maxLength - binary2.length(), '0') + binary2;

	std::string result = "";
	int carry = 0;

	// Iterate through each bit from right to left
	for (int i = maxLength - 1; i >= 0; --i) {
		int bit1 = paddedBinary1[i] - '0';
		int bit2 = paddedBinary2[i] - '0';

		// Calculate the sum of the current bits and the carry
		int sum = bit1 + bit2 + carry;

		// Update the result and carry
		result = char('0' + sum % 2) + result;
		carry = sum / 2;
	}

	// If there's a carry left, add it to the leftmost bit
	if (carry > 0) {
		result = char('0' + carry) + result;
	}

	return result;
}

// -------------------------------------------- Binary To Hex -------------------------------------------------- //

char binaryToHexChar(const std::string& binary) {
	if (binary == "0000") return '0';
	if (binary == "0001") return '1';
	if (binary == "0010") return '2';
	if (binary == "0011") return '3';
	if (binary == "0100") return '4';
	if (binary == "0101") return '5';
	if (binary == "0110") return '6';
	if (binary == "0111") return '7';
	if (binary == "1000") return '8';
	if (binary == "1001") return '9';
	if (binary == "1010") return 'a';
	if (binary == "1011") return 'b';
	if (binary == "1100") return 'c';
	if (binary == "1101") return 'd';
	if (binary == "1110") return 'e';
	if (binary == "1111") return 'f';
	return ' ';  // Invalid binary string
}

// Function to convert a binary string to a hex string
std::string binaryToHex(const std::string& binaryString) {
	std::string hexString;

	// Ensure the length of the binary string is a multiple of 4
	size_t length = binaryString.length();
	size_t padding = (4 - length % 4) % 4;
	std::string paddedBinary = std::string(padding, '0') + binaryString;

	// Process the binary string in 4-bit chunks and convert to hex
	for (size_t i = 0; i < paddedBinary.length(); i += 4) {
		std::string chunk = paddedBinary.substr(i, 4);
		hexString += binaryToHexChar(chunk);
	}

	return hexString;
}

// ------------------------------------ Hex Char To Integer ---------------------------------- //

int HexCharToInt(char hexChar) {
	switch (hexChar) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': case 'a': return 10;
	case 'B': case 'b': return 11;
	case 'C': case 'c': return 12;
	case 'D': case 'd': return 13;
	case 'E': case 'e': return 14;
	case 'F': case 'f': return 15;
	default: return -1;  // Invalid hex character
	}

}

// ---------------------------------- Integer to hexa Char --------------------------------- //

char IntToHexChar(int value) {
	if (value >= 0 && value <= 9) {
		return static_cast<char>('0' + value);
	}
	else if (value >= 10 && value <= 15) {
		return static_cast<char>('a' + (value - 10));
	}
	else {
		return '\0';  // Invalid value
	}
}


// ----------------------------------- Hexa To Binary --------------------------------------- //

std::string hexCharToBinary(char hexChar) {
	switch (hexChar) {
	case '0': return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': case 'a': return "1010";
	case 'B': case 'b': return "1011";
	case 'C': case 'c': return "1100";
	case 'D': case 'd': return "1101";
	case 'E': case 'e': return "1110";
	case 'F': case 'f': return "1111";
	default: return "";  // Invalid hex character
	}

}

std::string hexToBinary(const std::string& hexString) {
	std::string binaryString;

	for (char hexChar : hexString) {
		// Convert each hex character to binary and append to the result
		binaryString += hexCharToBinary(hexChar);
	}

	return binaryString;
}

// -------------------------------------- Add two Strings ------------------------------------------------- //

string addTwoStrings(string str1, string str2) {
	string binary1 = hexToBinary(str1);
	string binary2 = hexToBinary(str2);
	std::string result = addBinary(binary1, binary2);
	string finalResult = binaryToHex(result);
	return finalResult;
}

// --------------------------------------------- Subtract two hexadecimal numbers --------------------------------------------- //

std::string subtractHex(const std::string& hexStr1, const std::string& hexStr2) {
	std::string result = "";
	int borrow = 0;

	int i = hexStr1.length() - 1;
	int j = hexStr2.length() - 1;

	while (i >= 0 || j >= 0 || borrow > 0) {
		int digit1 = (i >= 0) ? HexCharToInt(hexStr1[i]) : 0;
		int digit2 = (j >= 0) ? HexCharToInt(hexStr2[j]) : 0;

		int difference = digit1 - digit2 - borrow;

		if (difference < 0) {
			difference += 16; // Borrow from the next higher digit
			borrow = 1;
		}
		else {
			borrow = 0;
		}

		result = IntToHexChar(difference) + result;
		--i;
		--j;
	}

	i = 0;

	while (result[i] == '0') {
		i++;
	}

	string finalResult = "";

	while (result[i]) {
		finalResult += result[i];
		i++;
	}

	return finalResult;
}


// -------------------------------------- Take Power of two in hexadecimal --------------------------------------------------- //

string powerInHexa(int power) {
	int quotient = power / 4;
	int remainder = power % 4;
	string result = "";

	if (remainder == 0) {
		result += "1";
	}

	else if (remainder == 1) {
		result += "2";
	}

	else if (remainder == 2) {
		result += "4";
	}

	else if (remainder == 3) {
		result += "8";
	}

	while (quotient != 0) {
		result += "0";
		quotient--;
	}

	return result;
}

// ----------------------------------------------- take mod --------------------------------------------------- //

string takeModulus(string str, int power) {
	string str1 = powerInHexa(power);

	if (isGreaterEqual(str, str1)) {
		return subtractHex(str, str1);
	}

	return str;
}

// ---------------------------------------------- Delete Paths -------------------------------------------- //

void deletePaths(PathNode* paths) {
	while (paths != nullptr) {
		string pathName = paths->path;
		std::wstring wideFolderName1(pathName.begin(), pathName.end());
		const wchar_t* deletePath = wideFolderName1.c_str();
		if (DeleteFile(deletePath)) {
			cout << "File deleted Successfully!" << endl;
		}

		else {
			cout << "Error deleting file" << endl;
		}

		paths = paths->next;
	}
}

// ---------------------------------------------  ROUTE NODE   ---------------------------------------------//
class MachineNode;

class RouteNode
{
public:
	MachineNode* destination;
	RouteNode* prev;
	RouteNode* next;

	RouteNode(MachineNode* machine) {
		destination = machine;
		prev = next = nullptr;
	}
};

// --------------- Doubly Linked List ROUTING TABLE ---------------- //

class RoutingTable
{
public:
	RouteNode* head;

	RoutingTable()
	{
		head = nullptr;
	}

	void insertRoute(MachineNode* machine)
	{
		RouteNode* newRoute = new RouteNode(machine);
		newRoute->prev = nullptr;
		newRoute->next = nullptr;

		if (head == nullptr)
		{
			head = newRoute;
		}

		else
		{
			RouteNode* temp = head;

			while (temp->next != nullptr) {
				temp = temp->next;
			}

			newRoute->prev = temp;
			temp->next = newRoute;
		}
	}

	void displayTable()
	{
		RouteNode* current = head;

		cout << "Routing Table:\n";
		while (current != nullptr)
		{
			cout << "Destination: " << current->destination << "\n";
			current = current->next;
		}
	}

	~RoutingTable()
	{
		RouteNode* current = head;
		while (current != nullptr)
		{
			RouteNode* temp = current;
			current = current->next;
			delete temp;
		}
		head = nullptr;
	}
};

// ------------------------------------------ Machine Node ---------------------------------------------------- //

class MachineNode {
public:
	string hash;
	string name;
	int numFiles;
	bool visited;
	MachineNode* next;
	RoutingTable routingTable;
	BTree* dataTree;

	MachineNode(string machineName, string machineHash, int iSpace, int maxKey) {
		name = machineName;
		hash = machineHash;
		next = nullptr;
		numFiles = 0;
		dataTree = new BTree(maxKey);
	}

	void printRoutingTable() {
		RouteNode* current = routingTable.head;

		// Print the column header
		std::cout << "Routing Table of " << name << " :" << endl;

		while (current != nullptr) {
			// Print the destination of each routing table entry
			std::cout << current->destination->hash << "\n";

			current = current->next;
		}
	}

	std::string destinationPath(string fileName) {

		std::wstring wideFolderName1(fileName.begin(), fileName.end());
		const wchar_t* sourcePath = wideFolderName1.c_str();
		string count = to_string(numFiles);
		string destination = name + "\\" + "file" + count;
		return destination;
	}

	void copyFile(string fileName) {

		std::wstring wideFolderName1(fileName.begin(), fileName.end());
		const wchar_t* sourcePath = wideFolderName1.c_str();
		string count = to_string(++numFiles);
		string destination = name + "\\" + "file" + count;
		std::wstring wideFolderName2(destination.begin(), destination.end());
		const wchar_t* destinationPath = wideFolderName2.c_str();
		if (CopyFile(sourcePath, destinationPath, FALSE)) {
			std::wcout << L"File copied successfully." << std::endl;
		}
		else {
			std::wcerr << L"Error copying file. Error code: " << GetLastError() << std::endl;
		}
	}

	bool insertFile(string fileName, string hash, PathNode*& pathSoFar) {
		// case 0: If the node already visited
		if (this->visited == 1) {
			return false;
		}

		// case 1: Machine and File have the same hash
		if (hash == this->hash) {
			copyFile(fileName);
			string destinationName = destinationPath(fileName);
			dataTree->insert(hash, destinationName);

			// Append the current node's path to the existing pathSoFar
			appendPathToExisting(pathSoFar);
			return true;
		}

		// case 2: File Hash is greater than machine hash but it is <= routingTable[1]
		RouteNode* temp = this->routingTable.head;

		if (isGreater(hash, this->hash) && isGreaterEqual(temp->destination->hash, hash)) {
			temp->destination->copyFile(fileName);
			string destinationName = temp->destination->destinationPath(fileName);
			temp->destination->dataTree->insert(hash, destinationName);

			// Append the current node's path to the existing pathSoFar
			appendPathToExisting(pathSoFar);
			return true;
		}
		else {
			while (temp->next != nullptr && isGreater(hash, temp->next->destination->hash)) {
				temp = temp->next;
			}

			this->visited = 1;

			// Append the current node's path to the existing pathSoFar
			appendPathToExisting(pathSoFar);

			// Continue traversing the path
			if (temp->next == nullptr) {
				return temp->destination->insertFile(fileName, hash, pathSoFar);
			}
			else {
				return temp->destination->insertFile(fileName, hash, pathSoFar);
			}
		}
	}

	// Helper function to append the current node's path to the existing pathSoFar
	void appendPathToExisting(PathNode*& pathSoFar) {
		PathNode* currentNodePath = new PathNode(this->hash); // Assuming this->hash is the path for the current node
		currentNodePath->next = pathSoFar;
		pathSoFar = currentNodePath;
	}

	bool removeFile(string fileHash, PathNode*& pathSoFar) {
		// If the node already visited
		if (this->visited == 1) {
			//PathNode* paths = dataTree->deletion(fileHash);
			//deletePaths(paths);
			//if (pathSoFar != nullptr) {
			//	appendPathToExisting(pathSoFar);
			//}
			return false;
		}

		// Machine and File have the same hash
		if (fileHash == this->hash) {
			PathNode* paths = dataTree->deletion(fileHash);
			deletePaths(paths);
			if (pathSoFar != nullptr) {
				appendPathToExisting(pathSoFar);
			}
			return true;
		}

		// File Hash is greater than machine hash but it is <= routingTable[1]
		RouteNode* temp = this->routingTable.head;

		if (isGreater(fileHash, this->hash) && isGreaterEqual(temp->destination->hash, fileHash)) {
			PathNode* paths = temp->destination->dataTree->deletion(fileHash);
			deletePaths(paths);
			if (pathSoFar != nullptr) {
				appendPathToExisting(pathSoFar);
			}
			return true;
		}

		// Continue traversal
		while (temp->next != nullptr && isGreater(fileHash, temp->next->destination->hash)) {
			temp = temp->next;
		}

		this->visited = 1;

		// If at the end of the routing table
		if (temp->next == nullptr) {
			temp->destination->removeFile(fileHash, pathSoFar);
		}
		else {
			temp->destination->removeFile(fileHash, pathSoFar);
			return true;
		}
	}


	void printPathsOfFileAtGivenHash(string hash)
	{
		dataTree->printPathsAtGivenHash(hash);
	}

};

// ------------- Circular Linked List ---------------- //

class MachineList {
public:
	MachineNode* root;
	int numMachines;

	MachineList()
	{
		numMachines = 0;
		root = NULL;
	}

	MachineNode* succ(string n) {
		MachineNode* temp = root;

		do {
			if (isGreaterEqual(temp->hash, n)) {
				return temp;
			}

			temp = temp->next;

		} while (temp != root);

		return temp;
	}

	void updateRoutingTable(MachineNode*& node, int size) {

		RouteNode* temp = node->routingTable.head;

		if (temp == nullptr) {
			for (int i = 0; i < size; i++) {
				string power = powerInHexa(i);
				string finalString = addTwoStrings(node->hash, power);
				finalString = takeModulus(finalString, size);
				MachineNode* destination = succ(finalString);
				node->routingTable.insertRoute(destination);
			}
		}

		else {
			for (int i = 0; i < size; i++) {

				string power = powerInHexa(i);
				string finalString = addTwoStrings(node->hash, power);
				finalString = takeModulus(finalString, size);
				MachineNode* destination = succ(finalString);
				temp->destination = succ(finalString);
				temp = temp->next;
			}
		}

	}

	void insertMachineNode(string n, string hash, int tableSize, int maxKey) {

		// create a new node with the given data
		MachineNode* newNode = new MachineNode(n, hash, tableSize, maxKey);

		// if the list is empty, make the new node the root and point it to itself
		if (root == NULL)
		{
			root = newNode;
			root->next = root;
			numMachines++;
		}

		else if (machineExists(hash)) {
			return;
		}

		// if the list is not empty and the new node's hash is smaller than the root's hash,
		// insert the new node before the root and update the root
		else if (isGreater(root->hash, hash))
		{
			newNode->next = root;
			MachineNode* last = root;
			// find the last node in the list
			while (last->next != root)
			{
				last = last->next;
			}
			// point the last node to the new node
			last->next = newNode;
			// update the root
			root = newNode;
			updateBTree(root, root->next);
			numMachines++;
		}

		// otherwise, find the appropriate position to insert the new node
		else
		{
			MachineNode* prev = root;
			MachineNode* curr = root->next;

			// traverse the list until the end or until a larger hash is found
			while (curr != root && isGreater(hash, curr->hash))
			{
				prev = curr;
				curr = curr->next;
			}

			// insert the new node between prev and curr
			newNode->next = curr;
			updateBTree(newNode, curr);
			prev->next = newNode;
			numMachines++;
		}

		// update the routing table for each node in the list
		MachineNode* temp = root;
		do {
			updateRoutingTable(temp, tableSize);
			temp = temp->next;
		} while (temp != root);

	}

	void updateBTree(MachineNode*& newMachine, MachineNode*& oldMachine) {
		stack<string> S;
		BTreeNode* node = oldMachine->dataTree->getRoot();

		if (node != NULL)
		{
			for (int i = 1; i <= node->numKeys; i++)
			{
				if (isGreaterEqual(newMachine->hash, node->pairs[i]->key)) {
					string key = node->pairs[i]->key;
					S.push(key);
					PathNode* paths = oldMachine->dataTree->findPath(node->pairs[i]->key, node);
					while (paths != nullptr) {
						string path = paths->path;
						newMachine->copyFile(path);
						string destinationName = newMachine->destinationPath(path);
						newMachine->dataTree->insert(key, destinationName);
						paths = paths->next;
					}
				}

				else if (isGreater(node->pairs[i]->key, oldMachine->hash)) {
					string key = node->pairs[i]->key;
					S.push(key);
					PathNode* paths = oldMachine->dataTree->findPath(node->pairs[i]->key, node);
					while (paths != nullptr) {
						string path = paths->path;
						newMachine->copyFile(path);
						string destinationName = newMachine->destinationPath(path);
						newMachine->dataTree->insert(key, destinationName);
						paths = paths->next;
					}
				}
			}

			// Print child pointers
			if (node->children[0] != NULL)
			{
				for (int i = 0; i <= node->numKeys; i++)
				{
					if (node->children[i] != NULL)
					{
						for (int j = 1; j <= node->children[i]->numKeys; j++)
						{
							if (isGreaterEqual(newMachine->hash, node->children[i]->pairs[i]->key)) {
								string key = node->children[i]->pairs[i]->key;
								S.push(key);
								PathNode* paths = oldMachine->dataTree->findPath(node->children[i]->pairs[i]->key, node);
								while (paths != nullptr) {
									string path = paths->path;
									newMachine->copyFile(path);
									string destinationName = newMachine->destinationPath(path);
									newMachine->dataTree->insert(key, destinationName);
									paths = paths->next;
								}
							}

							else if (isGreater(node->pairs[i]->key, oldMachine->hash)) {
								string key = node->children[i]->pairs[i]->key;
								S.push(key);
								PathNode* paths = oldMachine->dataTree->findPath(node->pairs[i]->key, node);
								while (paths != nullptr) {
									string path = paths->path;
									newMachine->copyFile(path);
									string destinationName = newMachine->destinationPath(path);
									newMachine->dataTree->insert(key, destinationName);
									paths = paths->next;
								}
							}
						}
					}
				}
			}
		}

		while (!S.empty()) {
			string deleteKey = S.top();
			PathNode* paths = oldMachine->dataTree->deletion(deleteKey);
			deletePaths(paths);
			S.pop();
		}
	}

	bool machineExists(string machineHash) {
		MachineNode* current = root;

		do {
			if (current->hash == machineHash) {
				return true;
			}
			current = current->next;
		} while (current != root);

		return false;
	}

	MachineNode*& findMachine(string hash) {
		MachineNode* current = root;

		while (current) {
			if (current->hash == hash) {
				return current;
			}

			current = current->next;
		}
	}

	void insertFile(string fileName, string fileHash, string machineHash) {

		if (machineExists(machineHash)) {
			MachineNode* temp = root;
			MachineNode* startingMachine = findMachine(machineHash);

			for (int i = 0; i < numMachines; i++) {
				startingMachine->visited = 0;
				startingMachine = startingMachine->next;
			}

			PathNode* path = nullptr; // Create an empty path

			if (!startingMachine->insertFile(fileName, fileHash, path)) {
				root->copyFile(fileName);
				string destinationName = root->destinationPath(fileName);
				root->dataTree->insert(fileHash, destinationName);
				root->appendPathToExisting(path);
			}
			//cout << "Path Followed : ";
			//printPath(path);
		}


		else {
			cout << "Required Machine doesn't exist in the system." << endl;
			return;
		}
	}

	void removeFile(string fileHash, string machineHash) {

		if (machineExists(machineHash)) {
			MachineNode* temp = root;
			MachineNode* startingMachine = findMachine(machineHash);

			for (int i = 0; i < numMachines; i++) {
				startingMachine->visited = 0;
				startingMachine = startingMachine->next;
			}

			PathNode* path = nullptr; // Create an empty path

			if (!startingMachine->removeFile(fileHash, path)) {
				PathNode* paths = root->dataTree->deletion(fileHash);
				deletePaths(paths);
				root->appendPathToExisting(path);
			}

			//cout << "Path Followed: ";
			//printPath(path);
		}

		else {
			cout << "Required Machine doesn't exist in the system." << endl;
			return;
		}
	}

	void printPathsOfFileAtGivenHash(string fileName, string hashIndex, string machineHash)
	{
		if (machineExists(machineHash)) {
			MachineNode* startingMachine = findMachine(machineHash);
			startingMachine->printPathsOfFileAtGivenHash(hashIndex);
		}

		else {
			cout << "Required Machine doesn't exist in the system." << endl;
			return;
		}
	}

	void copyAllFilesToSuccessor(MachineNode*& oldMachine, MachineNode*& newMachine) {
		stack<string> S;
		BTreeNode* node = oldMachine->dataTree->getRoot();

		if (node != NULL)
		{
			for (int i = 1; i <= node->numKeys; i++)
			{
				string key = node->pairs[i]->key;
				S.push(key);
				PathNode* paths = oldMachine->dataTree->findPath(node->pairs[i]->key, node);
				while (paths != nullptr) {
					string path = paths->path;
					newMachine->copyFile(path);
					string destinationName = newMachine->destinationPath(path);
					newMachine->dataTree->insert(key, destinationName);
					paths = paths->next;
				}

			}

			// Print child pointers
			if (node->children[0] != NULL)
			{
				for (int i = 0; i <= node->numKeys; i++)
				{
					if (node->children[i] != NULL)
					{
						for (int j = 1; j <= node->children[i]->numKeys; j++)
						{
							string key = node->children[i]->pairs[i]->key;
							S.push(key);
							PathNode* paths = oldMachine->dataTree->findPath(node->children[i]->pairs[i]->key, node);
							while (paths != nullptr) {
								string path = paths->path;
								newMachine->copyFile(path);
								string destinationName = newMachine->destinationPath(path);
								newMachine->dataTree->insert(key, destinationName);
								paths = paths->next;
							}
						}
					}
				}
			}
		}

		while (!S.empty()) {
			string deleteKey = S.top();
			PathNode* paths = oldMachine->dataTree->deletion(deleteKey);
			deletePaths(paths);
			S.pop();
		}
	}

	void deleteAllFiles(MachineNode*& oldMachine) {
		stack<string> S;
		BTreeNode* node = oldMachine->dataTree->getRoot();

		if (node != NULL)
		{
			for (int i = 1; i <= node->numKeys; i++)
			{
				string key = node->pairs[i]->key;
				S.push(key);

			}

			// Print child pointers
			if (node->children[0] != NULL)
			{
				for (int i = 0; i <= node->numKeys; i++)
				{
					if (node->children[i] != NULL)
					{
						for (int j = 1; j <= node->children[i]->numKeys; j++)
						{
							string key = node->children[i]->pairs[i]->key;
							S.push(key);
						}
					}
				}
			}
		}

		while (!S.empty()) {
			string deleteKey = S.top();
			PathNode* paths = oldMachine->dataTree->deletion(deleteKey);
			deletePaths(paths);
			S.pop();
		}
	}

	void deleteMachineNode(string hash) {
		if (!machineExists(hash)) {
			cout << "Machine having hash " << hash << " is not in the system!" << endl;
			return;
		}

		MachineNode* prev = root;
		MachineNode* temp = root->next;

		do {

			if (temp->hash == hash) {

				if (temp != temp->next) {
					copyAllFilesToSuccessor(temp, temp->next);
					string machineName = temp->name;
					std::wstring wideFolderName(machineName.begin(), machineName.end());
					LPCWSTR path = wideFolderName.c_str();
					if (RemoveDirectory(path)) {
						cout << "Machine deleted Successfully!" << endl;
					}
					temp = temp->next;
					prev->next = temp;
				}

				else {
					root = nullptr;
					string machineName = temp->name;
					deleteAllFiles(temp);
					std::wstring wideFolderName(machineName.begin(), machineName.end());
					LPCWSTR path = wideFolderName.c_str();
					if (RemoveDirectory(path)) {
						cout << "Machine deleted Successfully!" << endl;
					}
				}
			}
			prev = temp;
			temp = temp->next;
		} while (temp != root);
	}

};