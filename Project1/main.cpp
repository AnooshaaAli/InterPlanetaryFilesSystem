#include <Windows.h>
#include <Wincrypt.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdint>
#include "RingDHT.h"
#include<windows.h>

using namespace std;

int main() {
    HANDLE col;
    col = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(col, 3);


    std::cout <<
        "\t\t ___ _   _ _____ _____ ____          ____  _        _    _   _ _____ _____  _    ______   __\n"
        "\t\t|_ _| \\ | |_   _| ____|  _ \\        |  _ \\| |      / \\  | \\ | | ____|_   _|/ \\  |  _ \\ \\ / /\n"
        "\t\t | ||  \\| | | | |  _| | |_) |       | |_) | |     / _ \\ |  \\| |  _|   | | / _ \\ | |_) \\ V / \n"
        "\t\t | || |\\  | | | | |___|  _ <        |  __/| |___ / ___ \\| |\\  | |___  | |/ ___ \\|  _ < | |  \n"
        "\t\t|___|_| \\_| |_| |_____|_| \\_\\       |_|   |_____/_/   \\_\\_| \\_|_____| |_/_/   \\_\\_| \\_\\|_|  \n";
    std::cout <<
        "\t\t _____ ___ _     _____                     _                 \n"
        "\t\t|  ___|_ _| |   | ____|      ___ _   _ ___| |_ ___ _ __ ___  \n"
        "\t\t| |_   | || |   |  _|       / __| | | / __| __/ _ \\ '_ ` _ \\ \n"
        "\t\t|  _|  | || |___| |___      \\__ \\ |_| \\__ \\ ||  __/ | | | | |\n"
        "\t\t|_|   |___|_____|_____|     |___/\\__, |___/\\__\\___|_| |_| |_|\n"
        "\t\t                                 |___/                        \n";

    // cout <<"\n\t\t\t\t \4\4\4\4\4\4\4\4\4\4 INTER-PLANETARY FILE SYSTEM \4\4\4\4\4\4\4\4\4\4 \n\n\n" << endl;

    // system("Color 09");
     // Initialize Ring DHT
    int numMachines, identifierBits, order;
    SetConsoleTextAttribute(col, 1);
    cout << "\n\t\t\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4" << endl;
    SetConsoleTextAttribute(col, 3);
    std::cout << "\n\n\t\t\t\t \3 Enter the number of machines: ";
    SetConsoleTextAttribute(col, 7);
    std::cin >> numMachines;
    cout << "\n";
    SetConsoleTextAttribute(col, 3);
    std::cout << "\t\t\t\t \3 Enter the size of the identifier space in bits: ";
    SetConsoleTextAttribute(col, 7);
    std::cin >> identifierBits;
    cout << "\n";
    SetConsoleTextAttribute(col, 3);
    std::cout << "\t\t\t\t \3 Enter the order for b-tree: ";
    SetConsoleTextAttribute(col, 7);
    std::cin >> order;
    system("CLS");
    SetConsoleTextAttribute(col, 3);
    cout << "\n\t\t\t\t\t \4\4\4\4\4\4\4\4\4\4 MACHINES \4\4\4\4\4\4\4\4\4\4\n\n";
    SetConsoleTextAttribute(col, 7);
    RingDHT ringDHT(numMachines, identifierBits, order);

    // -----------------------------------  Initially machines inserted -------------------------------------- //

    for (int i = 0; i < numMachines; i++) {
        cout << "Enter Machine Name: ";
        string machine;
        cin >> machine;

        string machineId;
        cout << "Do you want to manually assign an ID to the machine? (y/n): ";
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter Machine ID: ";
            cin >> machineId;

            ringDHT.addMachine(machine, machineId);
        }

        else {
            ringDHT.addMachine(machine);
        }

        cout << endl << endl;
    }

    std::string command;
    do {
        // Display menu
        SetConsoleTextAttribute(col, 3);
        cout << "\n\t\t\t\t\t \4\4\4\4\4\4\4\4\4\4 RING DHT MENU \4\4\4\4\4\4\4\4\4\4\n\n";
        SetConsoleTextAttribute(col, 7);
        // std::cout << "\n=== Ring DHT Menu ===\n";
        std::cout << "1. Insert File\n";
        std::cout << "2. Remove Data\n";
        std::cout << "3. Print B-Tree\n";
        std::cout << "4. Print Routing Table\n";
        std::cout << "5. Add Machine\n";
        std::cout << "6. Remove Machine\n";
        std::cout << "0. Exit\n";
        SetConsoleTextAttribute(col, 4);
        std::cout << "\nEnter your choice: ";

        // Get user input
        SetConsoleTextAttribute(col, 7);
        std::cin >> command;
        std::cin.ignore();

        // Execute user command
        if (command == "1") {
            std::string filePath, machineHash;
            std::cout << "Enter file path to insert: ";
            std::getline(std::cin, filePath);
            cout << "Enter the machine hash from which you want to insert :";
            cin >> machineHash;
            ringDHT.insertFile(filePath, machineHash);
        }

        else if (command == "2") {
            string hash, machineHash;
            cout << "Enter the hash you want to delete: ";
            cin >> hash;
            cout << "Enter the machine hash from which you want to insert :";
            cin >> machineHash;
            ringDHT.removeFile(hash, machineHash);
        }

        else if (command == "3") {
            string machineHash;
            cout << "Enter the hash of the machine: ";
            cin >> machineHash;
            ringDHT.printMachineBtreeWithID(machineHash);
        }

        else if (command == "4") {
            cout << "Enter Machine Hash: ";
            string machineId;
            cin >> machineId;
            ringDHT.printRoutingTable(machineId);
        }

        else if (command == "5") {
            cout << "Enter Machine Name: ";
            string machine;
            cin >> machine;

            string machineId;
            cout << "Do you want to manually assign an ID to the machine? (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                cout << "Enter Machine ID: ";
                cin >> machineId;
                ringDHT.addMachine(machine, machineId);
            }

            else {
                ringDHT.addMachine(machine);
            }
        }

        else if (command == "6") {
            string machineHash;
            cout << "Enter the hash of the machine you wanna remove: ";
            cin >> machineHash;
            ringDHT.deleteMachine(machineHash);
        }

        else if (command == "0") {
            system("CLS");
            SetConsoleTextAttribute(col, 4);
            std::cout << "\n\n\n\n\n\n \t\t\t\t\t\t EXITING THE PROGRAM....\n\n\n\n\n\n\n\n\n";
        }
        else {
            std::cout << "Invalid command. Please try again.\n";
        }

    } while (command != "0");

    return 0;
}









