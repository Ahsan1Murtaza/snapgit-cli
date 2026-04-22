// SPDX-License-Identifier: MIT
// Implementation for ResetHandler.

#include "ResetHandler.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"
#include "../../Helper/ReadCommit/ReadCommit.h"
#include "../../Helper/RestoreTree/RestoreTree.h"
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

ResetHandler::ResetHandler() {}

// Helper function to clear working directory except .mygit
/**
 * @brief Performs clear working directory for reset.
 */
void clearWorkingDirectoryForReset() {
    for (const auto& entry : fs::directory_iterator(".")) {
        string name = entry.path().filename().string();
        
        if (name == ".mygit") {
            continue;
        }
        
        try {
            if (fs::is_directory(entry)) {
                fs::remove_all(entry.path());
            }
            else {
                fs::remove(entry.path());
            }
        }
        catch (const exception& e) {
            cerr << "Warning: Could not remove " << name << ": " << e.what() << endl;
        }
    }
}

/**
 * @brief Handles the  reset command workflow.
 * @param targetCommit Commit hash to move to.
 * @param hard When true, also update the working tree to match target commit.
 */
void ResetHandler::handleReset(const string& targetCommit, bool hard) {
    // Check if repo exists
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'mygit init' first.\n";
        return;
    }
    
    // Validate commit exists
    string folder = ".mygit/objects/" + targetCommit.substr(0, 2);
    string file = folder + "/" + targetCommit.substr(2);
    
    if (!fs::exists(file)) {
        cerr << "Error: Commit '" << targetCommit << "' not found.\n";
        return;
    }
    
    // Read commit to get tree hash
    CommitData commitData = readCommit(targetCommit);
    if (commitData.treeHash.empty()) {
        cerr << "Error: Failed to read commit: " << targetCommit << endl;
        return;
    }
    
    // Get current branch ref
    string currentBranch = getHeadRef();
    if (currentBranch.empty()) {
        cerr << "Error: Not on any branch (detached HEAD not supported).\n";
        return;
    }
    
    cout << "Resetting to commit " << targetCommit.substr(0, 7) << "...\n";
    
    if (hard) {
        // Hard reset: Clear everything and restore
        cout << "Hard reset: Clearing working directory...\n";
        clearWorkingDirectoryForReset();
        
        cout << "Restoring files from commit...\n";
        restoreTree(commitData.treeHash);
        
        // Update index
        writeIndexFile();
        cout << "Updated index\n";
    }
    
    // Update branch ref to target commit
    string branchPath = ".mygit/" + currentBranch;
    ofstream branchFile(branchPath, ios::trunc);
    branchFile << targetCommit;
    branchFile.close();
    
    cout << "\nHEAD is now at " << targetCommit.substr(0, 7) << endl;
    cout << "Branch '" << currentBranch.substr(11) << "' reset to " << targetCommit.substr(0, 7) << endl;
}
