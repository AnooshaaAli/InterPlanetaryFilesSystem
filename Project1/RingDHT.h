#pragma once
#include <Windows.h>
#include <Wincrypt.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include "Machines.h"
using namespace std;

// --------------------------------------------------------- SHA - 1 ----------------------------------------------------------------- //

std::string computeSHA1(const std::string& data) {
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		std::cerr << "Error acquiring crypto context" << std::endl;
		return "";
	}

	if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
		std::cerr << "Error creating hash" << std::endl;
		CryptReleaseContext(hProv, 0);
		return "";
	}

	if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(data.c_str()), static_cast<DWORD>(data.length()), 0)) {
		std::cerr << "Error hashing data" << std::endl;
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	DWORD hashSize = 20;  // SHA-1 produces a 160-bit hash (20 bytes)
	BYTE hashBuffer[20];
	if (!CryptGetHashParam(hHash, HP_HASHVAL, hashBuffer, &hashSize, 0)) {
		std::cerr << "Error getting hash value" << std::endl;
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	std::stringstream ss;
	for (DWORD i = 0; i < hashSize; ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hashBuffer[i]);
	}

	return ss.str();
}

// --------------------------------------------------------- String To Binary ------------------------------------------------------------------ //


//std::string hexCharToBinary(char hexChar) {
//	switch (hexChar) {
//	case '0': return "0000";
//	case '1': return "0001";
//	case '2': return "0010";
//	case '3': return "0011";
//	case '4': return "0100";
//	case '5': return "0101";
//	case '6': return "0110";
//	case '7': return "0111";
//	case '8': return "1000";
//	case '9': return "1001";
//	case 'A': case 'a': return "1010";
//	case 'B': case 'b': return "1011";
//	case 'C': case 'c': return "1100";
//	case 'D': case 'd': return "1101";
//	case 'E': case 'e': return "1110";
//	case 'F': case 'f': return "1111";
//	default: return "";  // Invalid hex character
//	}
//}

//// Function to convert a hex string to binary
//std::string hexToBinary(const std::string& hexString) {
//	std::string binaryString;
//
//	for (char hexChar : hexString) {
//		// Convert each hex character to binary and append to the result
//		binaryString += hexCharToBinary(hexChar);
//	}
//
//	return binaryString;
//}

	// ----------------------------------------------------- Extract Bits ----------------------------------------------------------- //

string extractBits(string binary, int numBits) {
	string result = "";
	size_t length = binary.length();
	length--;

	while (numBits) {
		result = binary[length] + result;
		length--;
		numBits--;
	}

	return result;
}

// ---------------------------------------------------------  binary to hex --------------------------------------------------------------------- //

//char binaryToHexChar(const std::string& binary) {
//	if (binary == "0000") return '0';
//	if (binary == "0001") return '1';
//	if (binary == "0010") return '2';
//	if (binary == "0011") return '3';
//	if (binary == "0100") return '4';
//	if (binary == "0101") return '5';
//	if (binary == "0110") return '6';
//	if (binary == "0111") return '7';
//	if (binary == "1000") return '8';
//	if (binary == "1001") return '9';
//	if (binary == "1010") return 'a';
//	if (binary == "1011") return 'b';
//	if (binary == "1100") return 'c';
//	if (binary == "1101") return 'd';
//	if (binary == "1110") return 'e';
//	if (binary == "1111") return 'f';
//	return ' ';  // Invalid binary string
//}
//
//// Function to convert a binary string to a hex string
//std::string binaryToHex(const std::string& binaryString) {
//	std::string hexString;
//
//	// Ensure the length of the binary string is a multiple of 4
//	size_t length = binaryString.length();
//	size_t padding = (4 - length % 4) % 4;
//	std::string paddedBinary = std::string(padding, '0') + binaryString;
//
//	// Process the binary string in 4-bit chunks and convert to hex
//	for (size_t i = 0; i < paddedBinary.length(); i += 4) {
//		std::string chunk = paddedBinary.substr(i, 4);
//		hexString += binaryToHexChar(chunk);
//	}
//
//	return hexString;
//}

// -------------------------------- Class representing the Ring DHT ---------------------------------------- //

class RingDHT {
public:
	int numMachines;
	int identifierBits;
	int maxKey;
	MachineList space;

	RingDHT(int numMachines, int identifierBits, int maxkey) {
		this->numMachines = numMachines;
		this->identifierBits = identifierBits;
		this->maxKey = maxkey;
		space.root = nullptr;
	}

	//int succ(Data* n) {
	//	MachineNode* temp = space.root;
	//	while (temp) {
	//		if (temp->id >= n->key) {
	//			return temp->next->id;
	//		}
	//		temp = temp->next;
	//	}
	//}

	string hash(string str) {
		string sha = computeSHA1(str);
		string binaryStr = hexToBinary(sha);
		string extractedBinary = extractBits(binaryStr, identifierBits);
		string finalHash = binaryToHex(extractedBinary);
		return finalHash;
	}

	void addMachine(string machineName) {
		string hashIndex = hash(machineName);
		cout << hashIndex << endl;
		space.insertMachineNode(machineName, hashIndex, identifierBits, maxKey);

		std::wstring wideFolderName(machineName.begin(), machineName.end());

		LPCWSTR path = wideFolderName.c_str();

		if (CreateDirectory(path, NULL)) {
			std::cout << "Directory created successfully.\n";
		}

		else if (ERROR_ALREADY_EXISTS == GetLastError()) {
			std::cout << "Directory already exists.\n";
		}

		else {
			std::cerr << "Failed to create directory. Error code: " << GetLastError() << "\n";
		}
	}

	void addMachine(string machineName, string id) {

		string hashIndex = id;
		space.insertMachineNode(machineName, hashIndex, identifierBits, maxKey);

		std::wstring wideFolderName(machineName.begin(), machineName.end());

		LPCWSTR path = wideFolderName.c_str();

		if (CreateDirectory(path, NULL)) {
			std::cout << "Machine created successfully.\n";
		}

		else if (ERROR_ALREADY_EXISTS == GetLastError()) {
			std::cout << "Machine already exists.\n";
		}

		else {
			std::cerr << "Failed to create directory. Error code: " << GetLastError() << "\n";
		}
	}

	void insertFile(string fileName, string machineHash) {
		std::ifstream file(fileName);

		if (file.is_open()) {

			// Read the content of the file
			std::ostringstream contentStream;
			contentStream << file.rdbuf();
			std::string fileContent = contentStream.str();
			file.close();
			string hashIndex = hash(fileContent);
			cout << "File Hash: " << hashIndex << endl;
			space.insertFile(fileName, hashIndex, machineHash);
		}

		else {
			std::cerr << "Error: Unable to open file " << fileName << std::endl;
		}

	}

	void removeFile(string fileHash, string machineHash) {
		space.removeFile(fileHash, machineHash);
	}

	void printRoutingTable(string machineId) {
		MachineNode* temp = space.root;
		if (temp == nullptr) {
			cout << "Currently, there is no machine in the system." << endl;
			return;
		}

		do {
			if (temp->hash == machineId) {
				temp->printRoutingTable();
			}
			temp = temp->next;
		} while (temp != space.root);

	}

	void printMachineBtreeWithID(string machineId)
	{
		MachineNode* temp = space.root;
		if (temp == nullptr) {
			cout << "Currently, there is no machine in the system." << endl;
			return;
		}

		do {
			if (temp->hash == machineId) {
				temp->dataTree->display();
				return;
			}
			temp = temp->next;
		} while (temp != space.root);

		cout << "There is no machine in the system having Id " << machineId << endl;
	}

	void printPathsAtGivenHash(string fileName, string machineHash)
	{
		std::ifstream file(fileName);

		if (file.is_open()) {

			// Read the content of the file
			std::ostringstream contentStream;
			contentStream << file.rdbuf();
			std::string fileContent = contentStream.str();
			file.close();
			string hashIndex = hash(fileContent);
			cout << "File Hash: " << hashIndex << endl;
			space.insertFile(fileName, hashIndex, machineHash);
		}

		else {
			std::cerr << "Error: Unable to open file " << fileName << std::endl;
		}
	}

	void deleteMachine(string machineId) {
		space.deleteMachineNode(machineId);
	}
};




