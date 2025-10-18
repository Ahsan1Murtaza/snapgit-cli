#include <iostream>
#include <string>

#include "CommandHandlers/InitHandler/InitHandler.h"
#include "CommandHandlers/AddHandler/AddHandler.h"

using namespace std;
    /* Function to print help information
Displays available commands and their descriptions to the user.
*/
void printHelp() {
    cout << "SnapGit - A Simple git clone!\n\n";
    cout << "Available commands:\n";
    cout << "  init    Initialize a new repository\n";
    cout << "  help    Show this help message\n";
}

int main(int argc, char* argv[]) {


    InitHandler initHandler; // Create an instance of InitHandler
    AddHandler addHandler; // Create an instance of AddHandler

    if (argc < 2) {
        printHelp();
        return 0;
    }

    string command = argv[1]; // Get the command from command line arguments

    

    if (command == "init") {
        initHandler.handleInit();
    } 
    else if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
    } 
    else if (command == "add") {
        if (argc < 3) {
            cout << "Error: 'add' command requires a file path argument.\n";
            return 1;
        }
        string filePath = argv[2];
        addHandler.handleAdd(filePath);
    }
    else {
        cout << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}
