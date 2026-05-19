// SPDX-License-Identifier: MIT
// CLI entrypoint and command dispatch.

#include <iostream>
#include <string>
#include <algorithm>

#include "commands/InitHandler.h"
#include "commands/AddHandler.h"
#include "commands/CommitHandler.h"
#include "commands/ConfigHandler.h"
#include "commands/BranchHandler.h"
#include "commands/CheckoutHandler.h"
#include "commands/ResetHandler.h"
#include "commands/LogHandler.h"
#include "commands/StatusHandler.h"
#include "commands/RestoreHandler.h"
#include "commands/RemoveHandler.h"
#include "commands/MergeHandler.h"
#include "commands/GcHandler.h"

#include "utils/GetUserInfo.h"

using namespace std;

/* Function to print help information
Displays available commands and their descriptions to the user.
*/
/**
 * @brief Performs print help.
 */
void printHelp() {
    cout << "SnapGit - A Simple git clone!\n\n";
    cout << "Available commands:\n";
    cout << "  init        Initialize a new repository\n";
    cout << "  add         Add a file to the staging area\n";
    cout << "  commit -m   Commit staged files with a message\n";
    cout << "  config      Set or display user config\n";
    cout << "  branch      Create a new branch or list all branches\n";
    cout << "  checkout    Switch branches or create new branch\n";
    cout << "  reset       Reset current branch to a specified commit\n";
    cout << "  log         Show the commit history\n";
    cout << "  status      Show the status of files in working directory and staging area\n";
    cout << "  rm          Remove a file from the staging area (cached) or working directory\n";
    cout << "  restore     Restore a file from the index to the working directory\n";
    cout << "  merge       Merge branches\n";
    cout << "  gc          Remove unreachable objects from the object store\n";
    cout << "  help        Show this help message\n\n";
    cout << "Use \"snapgit <command> --help\" for more information on a specific command.\n";
}

/**
 * @brief Performs print command help.
 * @param command Input value for `command`.
 */
void printCommandHelp(const std::string& command) {

    auto title = [&](const string& t){
        cout << t << "\n";
    };

    auto cmd = [&](const string& t){
        cout << "  " << t << "\n";
    };

    auto text = [&](const string& t){
        cout << "      " << t << "\n";
    };

    if (command == "init") {
        title("Initialize a new repository");
        cmd("snapgit init");
        text("Creates a .snapgit directory and sets up repository structure.");
    }
    else if (command == "add") {
        title("Add files to staging area");
        cmd("snapgit add <file>");
        cmd("snapgit add .");
        text("Adds specified file or all files recursively to the index.");
        text("Reads ignore rules from .snapgitignore when staging recursively.");
        text("Example: snapgit add file.txt");
    }
    else if (command == "commit") {
        title("Commit staged changes");
        cmd("snapgit commit -m <message>");
        text("Creates a new commit with the current staged files.");
        text("Example: snapgit commit -m \"Initial commit\"");
    }
    else if (command == "config") {
        title("Set or view SnapGit config");
        cmd("snapgit config user.name <name>");
        cmd("snapgit config user.email <email>");
        text("Without arguments, prints current configuration.");
    }
    else if (command == "branch") {
        title("Create, list, or delete branches");
        cmd("snapgit branch");
        text("Lists all branches.");
        cmd("snapgit branch <name>");
        text("Creates a branch with the given name.");
        cmd("snapgit branch -d <name>");
        text("Deletes a branch (removes refs/heads/<name>).");
    }
    else if (command == "gc") {
        title("Garbage collect unreachable objects");
        cmd("snapgit gc");
        text("Deletes objects not reachable from any branch, HEAD, or the index.");
        cmd("snapgit gc --dry-run");
        text("Shows what would be deleted without removing files.");
    }
    else if (command == "checkout") {
        title("Switch branches or restore files");
        cmd("snapgit checkout <branch>");
        cmd("snapgit checkout <commit-hash>");
        text("Updates working directory to match target.");
    }
    else if (command == "reset") {
        title("Reset current branch");
        cmd("snapgit reset --hard <commit>");
        text("Moves branch pointer to a specific commit.");
        text("WARNING: --hard removes working directory changes.");
    }
    else if (command == "log") {
        title("Show commit history");
        cmd("snapgit log");
        text("Displays commits from newest to oldest.");
    }
    else if (command == "status") {
        title("Show repository status");
        cmd("snapgit status");
        text("Shows staged, modified, deleted, and untracked files.");
    }
    else if (command == "rm") {
        title("Remove files");
        cmd("snapgit rm <file>");
        text("Removes file from working directory AND staging area.");
        cmd("snapgit rm --cached <file>");
        text("Removes file ONLY from staging area.");
    }
    else if (command == "restore") {
        title("Restore working directory files");
        cmd("snapgit restore <file>");
        text("Restores file from staging/index to working directory.");
    }
    else if (command == "merge") {
        title("Merge branches");
        cmd("snapgit merge <otherBranch>");
        text("Merges specified branch into current branch.");
    }
    else if (command == "help") {
        title("Help command");
        cmd("snapgit help");
        text("Shows list of available commands.");
        cmd("snapgit help <command>");
        text("Shows detailed help for the given command.");
    }
    else {
        cout << "Unknown command: " << command << "\n";
    }
}

/**
 * @brief Parses command-line arguments and dispatches commands.
 * @param argc Input value for `argc`.
 * @return Process exit code (0 on success, non-zero on failure).
 */
int main(int argc, char* argv[]) {

    InitHandler initHandler;       // Create an instance of InitHandler
    AddHandler addHandler;         // Create an instance of AddHandler
    CommitHandler commitHandler;   // Create an instance of CommitHandler
    ConfigHandler configHandler;   // Create an instance of ConfigHandler
    BranchHandler branchHandler;   // Create an instance of BranchHandler
    CheckoutHandler checkoutHandler; // Create an instance of CheckoutHandler
    ResetHandler resetHandler;     // Create an instance of ResetHandler
    LogHandler logHandler;         // Create an instance of LogHandler
    StatusHandler statusHandler;
    RestoreHandler restoreHandler;
    RemoveHandler removeHandler;
    MergeHandler mergeHandler;
    GcHandler gcHandler;

     if (argc < 2) {
         printHelp();
         return 0;
     }

     string command = argv[1]; // Get the command from command line arguments

    // GLOBAL help: `snapgit help`
    if (command == "help") {
        printHelp();
        return 0;
    }

    // Per-command help: `snapgit <cmd> --help`
    if (argc >= 3 && string(argv[2]) == "--help") {
        printCommandHelp(command);
        return 0;
    }


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
                cout << "No user info set. \n Use:\n snapgit config user.name <name>\n snapgit config user.email <email>";
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
        cout << "  snapgit config user.name <name> \n";
        cout << "  snapgit config user.email <email> \n";
    }
    else if (command == "branch") {
        branchHandler.handleBranch(argc, argv);
    }
    else if (command == "gc") {
        bool dryRun = false;
        if (argc >= 3) {
            string arg = argv[2];
            if (arg == "--dry-run" || arg == "-n") {
                dryRun = true;
            } else {
                cerr << "Usage:\n";
                cerr << "  snapgit gc\n";
                cerr << "  snapgit gc --dry-run\n";
                return 1;
            }
        }
        gcHandler.handleGc(dryRun);
    }
    else if (command == "checkout") {
        if (argc < 3) {
            cout << "Usage:\n";
            cout << "  snapgit checkout <branch-name>\n";
            cout << "  snapgit checkout <hash-code>\n";
            return 1;
        }
        
        string arg = argv[2]; // Could be branch or commit hash
        string branchName, hashCode;
        
        if (arg.size() == 40 && all_of(arg.begin(), arg.end(), ::isxdigit)) {
            hashCode = arg;
            checkoutHandler.handleCheckout(hashCode);
        }
        else {
            branchName = arg;
            checkoutHandler.handleCheckout(branchName);
        }
    }
    // NEW: Reset command handling
    else if (command == "reset") {
        if (argc < 3) {
            cout << "Usage:\n";
            cout << "  snapgit reset --hard <commit-hash>\n";
            return 1;
        }
        
        bool hard = false;
        string targetCommit;
        
        if (string(argv[2]) == "--hard") {
            if (argc < 4) {
                cerr << "Error: 'reset --hard' requires a commit hash.\n";
                return 1;
            }
            hard = true;
            targetCommit = argv[3];
        }
        else {
            cerr << "Error: Only '--hard' reset is supported for now.\n";
            cout << "Usage: snapgit reset --hard <commit-hash>\n";
            return 1;
        }
        
        resetHandler.handleReset(targetCommit, hard);
    }
    else if (command == "log") {
        logHandler.handleLog();
    }

    else if (command == "status") {
        statusHandler.handleStatus();
    }

    else if (command == "restore") {
        string path = argv[2];
        restoreHandler.handleRestore(path);
    }

    else if (command == "rm") {
        if (argc < 3) {
           cerr << "Usage: snapgit rm [--cached] <file>\n";
           return 1;
        }

        bool cached = false;
        string file;

        if (string(argv[2]) == "--cached" && argc >= 4) {
                cached = true;
                file = argv[3];
        } else {
                file = argv[2];
        }

        removeHandler.handleRemove(file, cached);
    }


    else if (command == "merge") {
        if (argc < 3) {
            cerr << "Usage: snapgit merge <otherBranch>\n";
            return 1;
        }
        string otherBranch = argv[2];
        mergeHandler.handleMerge(otherBranch);

    }
    else {
        cout << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}


