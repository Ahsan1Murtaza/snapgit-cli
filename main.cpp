#include <iostream>
#include <string>

#include "CommandHandlers/InitHandler/InitHandler.h"
#include "CommandHandlers/AddHandler/AddHandler.h"
#include "CommandHandlers/CommitHandler/CommitHandler.h"
#include "CommandHandlers/ConfigHandler/ConfigHandler.h"
#include "CommandHandlers/BranchHandler/BranchHandler.h"
#include "CommandHandlers/CheckoutHandler/CheckoutHandler.h"  // NEW: Checkout handler added
#include "Helper/GetUserInfo/GetUserInfo.h"

using namespace std;

/* Function to print help information
Displays available commands and their descriptions to the user.
*/
void printHelp() {
    cout << "SnapGit - A Simple git clone!\n\n";
    cout << "Available commands:\n";
    cout << "  init       Initialize a new repository\n";
    cout << "  add        Add a file to the staging area\n";
    cout << "  commit -m  Commit staged file with a message\n";
    cout << "  config     Set or display user config\n";
    cout << "  branch     Create a new branch or list all branches\n";
    cout << "  checkout   Switch branches or create new branch\n";  // NEW: Added checkout
    cout << "  help       Show this help message\n";
}

int main(int argc, char* argv[]) {

    InitHandler initHandler;       // Create an instance of InitHandler
    AddHandler addHandler;         // Create an instance of AddHandler
    CommitHandler commitHandler;   // Create an instance of CommitHandler
    ConfigHandler configHandler;   // Create an instance of ConfigHandler
    BranchHandler branchHandler;   // Create an instance of BranchHandler
    CheckoutHandler checkoutHandler; // NEW: Create an instance of CheckoutHandler

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
        auto [name, email] = getUserInfoFromConfig();
        if (argc == 2) {
            // Display current info
            if (name.empty() && email.empty()) {
                cout << "No user info set. \n Use:\n mygit config user.name <name>\n mygit config user.email <email>";
            } else {
                cout << "User Information:\n";
                cout << "  Name : " << name << "\n";
                cout << "  Email: " << email << "\n";
            }
            return 1;
        } 
        if (argc == 4) {
            // Set info
            if (string(argv[2]) == "user.name") {
                name = argv[3];
            }
            else if (string(argv[2]) == "user.email") {
                email = argv[3];
            }
            configHandler.handleConfig(name, email);
            return 1;
        }
        cout << "Usage:\n";
        cout << "  mygit config user.name <name> \n";
        cout << "  mygit config user.email <email> \n";
    }
    else if (command == "branch") {
        if (argc == 2) {
            branchHandler.handleBranch();  
        } else if (argc == 3) {
            branchHandler.handleBranch(argv[2]);  
        } else {
            cout << "Usage:\n";
            cout << "  mygit branch             # List branches\n";
            cout << "  mygit branch <name>      # Create a new branch\n";
        }
    }
    // NEW: Checkout command handling
    else if (command == "checkout") {
        if (argc < 3) {
            cout << "Usage:\n";
            cout << "  mygit checkout <branch-name>\n";
            cout << "  mygit checkout -b <branch-name>\n";
            return 1;
        }
        
        bool createBranch = false;
        string branchName;
        
        if (string(argv[2]) == "-b") {
            if (argc < 4) {
                cerr << "Error: 'checkout -b' requires a branch name.\n";
                return 1;
            }
            createBranch = true;
            branchName = argv[3];
        }
        else {
            branchName = argv[2];
        }
        
        checkoutHandler.handleCheckout(branchName, createBranch);
    }
    else {
        cout << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}