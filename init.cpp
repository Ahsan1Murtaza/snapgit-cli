#include "init.h"

#include <iostream>
#include <string> 
#include <fstream> // For file operations
#include <direct.h> // For _mkdir (Windows specific)
#include <sys/stat.h> // For stat()


using namespace std;


// Constructor
InitHandler::InitHandler() {}




/* Function to check if a directory exists
Uses the stat() system call from <sys/stat.h>
to check if a directory exists at the given path. 
Returns true if the directory exists, false otherwise.
*/
bool InitHandler::dirExists(const string& path) {

    /* stat() checks if a file or directory exists and fills info with details.
    stat() returns 0 if the path exists, nonzero otherwise. */

    /* info.st_mode contains file type and permissions (bit flags).
    The S_IFDIR flag means “this is a directory”. */    

    struct stat info;
    return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}




/* Function to create a directory 
Uses the _mkdir() function from <direct.h> to create a directory at the given path. 
Returns true if the directory was created successfully or already exists, false otherwise.
*/
bool InitHandler::createDir(const string& path) {

    if (dirExists(path)) {
        return true;
    }
    
    return _mkdir(path.c_str()) == 0;
}




/* Function to print help information
Displays available commands and their descriptions to the user.
*/
void InitHandler::printHelp() {
    cout << "SnapGit - A Simple git clone!\n\n";
    cout << "Available commands:\n";
    cout << "  init    Initialize a new repository\n";
    cout << "  help    Show this help message\n";
}



/* Function to handle the 'init' command
Initializes a new Git repository by creating the necessary directory structure and files.

Folder structure created:
.mygit/
    HEAD
    index
    objects/
    refs/
        heads/
            main
*/
void InitHandler::handleInit() {

    // Check if .mygit directory already exists
    if (dirExists(".mygit")) {
        cout << "Reinitialized existing Git repository in .mygit/\n";
        return;
    }

    // Create .mygit directory
    if (!createDir(".mygit")) {
        cerr << "Error: Failed to create .mygit directory\n";
        return;
    }

    // Create HEAD and index files inside .mygit

    // Create HEAD file
    ofstream headFile(".mygit/HEAD");
    if (!headFile) {
        cerr << "Error: Failed to create HEAD file\n";
        return;
    }
    headFile << "ref: refs/heads/main\n";
    headFile.close();


    // Create empty index file
    ofstream indexFile(".mygit/index");
    if (!indexFile) {
        cerr << "Error: Failed to create index file\n";
        return;
    }
    indexFile.close();



    // Create subdirectories in .mygit
    // 1. Objects
    // 2. Refs
    // 3. Refs/heads

    createDir(".mygit/objects");
    createDir(".mygit/refs");
    createDir(".mygit/refs/heads");
    // createDir(".mygit\\info");
    // createDir(".mygit\\hooks");
    
    

    // Create sub file (main) in refs/heads

    // Create refs/heads/main file
    ofstream headsFile(".mygit/refs/heads/main");
    if (!headsFile) {
        cerr << "Error: Failed to create refs/heads/main file\n";
        return;
    }
    headsFile.close();

    

    // Optionally, create other standard files like config, description, exclude


    // ofstream configFile(".mygit\\config");
    // if (configFile) {
    //     configFile << "[core]\n";
    //     configFile << "\trepositoryformatversion = 0\n";
    //     configFile << "\tfilemode = false\n";
    //     configFile << "\tbare = false\n";
    //     configFile << "\tlogallrefupdates = true\n";
    //     configFile << "\tsymlinks = false\n";
    //     configFile << "\tignorecase = true\n";
    //     configFile.close();
    // }

    // ofstream descFile(".mygit\\description");
    // if (descFile) {
    //     descFile << "Unnamed repository; edit this file 'description' to name the repository.\n";
    //     descFile.close();
    // }

    // ofstream excludeFile(".mygit\\info\\exclude");
    // if (excludeFile) {
    //     excludeFile << "# git ls-files --others --exclude-from=.mygit/info/exclude\n";
    //     excludeFile << "# Lines that start with '#' are comments.\n";
    //     excludeFile << "# *.[oa]\n";
    //     excludeFile << "# *~\n";
    //     excludeFile.close();
    // }

    cout << "Initialized empty Git repository in .mygit/\n";
}