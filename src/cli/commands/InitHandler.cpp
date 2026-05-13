// SPDX-License-Identifier: MIT
// Implementation for InitHandler.

#include "InitHandler.h"

#include <iostream>
#include <string> 
#include <fstream> // For file operations
#include <sys/stat.h> // For stat()
#include <filesystem> // For filesystem operations


using namespace std;


// Constructor
InitHandler::InitHandler() {}




/* Function to check if a directory exists 
Returns true if the directory exists, false otherwise.
*/
/**
 * @brief Performs dir exists.
 * @param path Filesystem path to process.
 * @return True when the check succeeds; otherwise false.
 */
bool InitHandler::dirExists(const string& path) {

    return std::filesystem::exists(path) && std::filesystem::is_directory(path);

}




/* Function to create a directory 
Returns true if the directory was created successfully or already exists, false otherwise.
*/
/**
 * @brief Creates dir.
 * @param path Filesystem path to process.
 * @return True when the check succeeds; otherwise false.
 */
bool InitHandler::createDir(const string& path) {
    
    return filesystem::create_directories(path);

}




/* Function to handle the 'init' command
Initializes a new Git repository by creating the necessary directory structure and files.

Folder structure created:
.snapgit/
    HEAD
    index
    objects/
    refs/
        heads/
            main
*/
/**
 * @brief Handles the  init command workflow.
 */
void InitHandler::handleInit() {

    // Check if .snapgit directory already exists
    if (dirExists(".snapgit")) {
        cout << "Reinitialized existing Git repository in .snapgit/\n";
        return;
    }

    // Create .snapgit directory

    if (!createDir(".snapgit")) {
        cerr << "Error: Failed to create .snapgit directory\n";
        return;
    }

    // // Create HEAD and index files inside .snapgit

    // // Create HEAD file
    ofstream headFile(".snapgit/HEAD");
    if (!headFile) {
        cerr << "Error: Failed to create HEAD file\n";
        return;
    }
    headFile << "ref: refs/heads/main\n";
    headFile.close();


    // // Create empty index file
    ofstream indexFile(".snapgit/index");
    if (!indexFile) {
        cerr << "Error: Failed to create index file\n";
        return;
    }
    indexFile.close();



    // // Create subdirectories in .snapgit
    // // 1. Objects
    // // 2. Refs
    // // 3. Refs/heads

    createDir(".snapgit/objects");
    createDir(".snapgit/refs/heads");
    
    // // createDir(".snapgit\\info");
    // // createDir(".snapgit\\hooks");
    
    

    // // Create sub file (main) in refs/heads

    // // Create refs/heads/main file
    ofstream headsFile(".snapgit/refs/heads/main");
    if (!headsFile) {
        cerr << "Error: Failed to create refs/heads/main file\n";
        return;
    }
    headsFile.close();

    //Create Config File
    ofstream configFile(".snapgit/config");
    configFile << "[user]\n";
    configFile << "    name = \n";
    configFile << "    email = \n";
    configFile.close();

















    // Optionally, create other standard files like config, description, exclude


    // ofstream configFile(".snapgit\\config");
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

    // ofstream descFile(".snapgit\\description");
    // if (descFile) {
    //     descFile << "Unnamed repository; edit this file 'description' to name the repository.\n";
    //     descFile.close();
    // }

    // ofstream excludeFile(".snapgit\\info\\exclude");
    // if (excludeFile) {
    //     excludeFile << "# git ls-files --others --exclude-from=.snapgit/info/exclude\n";
    //     excludeFile << "# Lines that start with '#' are comments.\n";
    //     excludeFile << "# *.[oa]\n";
    //     excludeFile << "# *~\n";
    //     excludeFile.close();
    // }

    cout << "Initialized empty Git repository in .snapgit/\n";
}
