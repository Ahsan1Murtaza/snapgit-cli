#include <iostream>
#include <string>

#include "CommandHandlers/InitHandler/InitHandler.h"
#include "CommandHandlers/AddHandler/AddHandler.h"
#include "CommandHandlers/CommitHandler/CommitHandler.h"
#include "CommandHandlers/ConfigHandler/ConfigHandler.h"
#include "Helper/GetUserInfo/GetUserInfo.h"

using namespace std;
    /* Function to print help information
Displays available commands and their descriptions to the user.
*/
void printHelp() {
    cout << "SnapGit - A Simple git clone!\n\n";
    cout << "Available commands:\n";
    cout << "  init    Initialize a new repository\n";
    cout << "  add     Add a file to the stagin area\n";
    cout << "  commit -m commit staged file with a message\n";
    cout << "  config    Set or display user config\n";
    cout << "  help    Show this help message\n";
}

int main(int argc, char* argv[]) {


    InitHandler initHandler; // Create an instance of InitHandler
    AddHandler addHandler; // Create an instance of AddHandler
    CommitHandler commitHandler; // Create an instance of CommitHandler
    ConfigHandler  configHandler;  // Create an instance of ConfigHandler

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
    else if (command == "commit") {
        if (argc < 4) {
            cout << "Error: 'commit -m' command requires a commit message argument.\n";
            return 1;
        }
        string symbol = argv[2];
        if (symbol != "-m") {
            cout << "Error: Unknown symbol " << symbol << " for commit command.\n";
            return 1;
        }
        string commitMessage = argv[3];
        if (commitMessage.empty()) {
            cout << "Error: Commit message cannot be empty.\n";
            return 1;
        }
        commitHandler.handleCommit(commitMessage);
        
    }
    else if (command == "config") {
        if (argc == 2) {
            // Display current info
            auto [name, email] = getUserInfoFromConfig();
            if (name.empty() && email.empty()) {
                cout << "No user info set. Use: mygit info <name> <email>\n";
            } else {
                cout << "User Information:\n";
                cout << "  Name : " << name << "\n";
                cout << "  Email: " << email << "\n";
            }
        } else if (argc == 4) {
            // Set info
            string name = argv[2];
            string email = argv[3];
            configHandler.handleConfig(name, email);
        } else {
            cout << "Usage:\n";
            cout << "  mygit info <name> <email>   # Set info\n";
            cout << "  mygit info                  # Show info\n";
        }
    }
    else {
        cout << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}
