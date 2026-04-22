// SPDX-License-Identifier: MIT
// Implementation for CheckoutHandler.

#include "CheckoutHandler.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/ReadCommit/ReadCommit.h"
#include "../../Helper/RestoreTree/RestoreTree.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief CheckoutHandler operation.
 */
CheckoutHandler::CheckoutHandler() {}

// Helper function to clear working directory except .mygit
/**
 * @brief clearWorkingDirectory operation.
 */
void clearWorkingDirectory() {
    for (const auto& entry : fs::directory_iterator(".")) {
        string name = entry.path().filename().string();
        
        // Skip .mygit folder
        if (name == ".mygit" || name == ".git") {
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
 * @brief handleCheckout operation.
 * @param refInput Parameter description.
 */
void CheckoutHandler::handleCheckout(const string& refInput) {
    // Check if repo exists
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'mygit init' first.\n";
        return;
    }
    
    string targetCommitHash;
    string targetRef;

    // Detect whether input is a commit hash or branch name
    bool isHash = (refInput.size() == 40 && all_of(refInput.begin(), refInput.end(), ::isxdigit));

    if (isHash) {
        // Checkout by Commit hash
        string commitPath = ".mygit/objects/" + refInput.substr(0,2) + "/" + refInput.substr(2);
        if (!fs::exists(commitPath)) {
            cerr << "Error: Commit hash '" << refInput << "' not found.\n";
            return;
        }

        targetCommitHash = refInput;
    }
    else{
        // Checkout by Branch name
        targetRef = "refs/heads/" + refInput;
        string branchPath = ".mygit/" + targetRef;
        if (!fs::exists(branchPath)) {
            cerr << "Error: Branch '" << refInput << "' does not exist.\n";
            return;
        }

        ifstream(branchPath) >> targetCommitHash;
        if (targetCommitHash.empty()) {
            cerr << "Error: Branch '" << refInput << "' has no commits.\n";
            return;
        }
    }
    
   
    
    // Read commit to get tree hash
    CommitData commitData = readCommit(targetCommitHash);
    if (commitData.treeHash.empty()) {
        cerr << "Error: Invalid or corrupted commit: " << targetCommitHash << endl;
        return;
    }
    
    cout << "Switching to " << (isHash ? "commit (detached HEAD) " : "branch '" + refInput + "'") << "...\n";
    
   
    clearWorkingDirectory();
    restoreTree(commitData.treeHash);
    writeIndexFile();

    
    // Update HEAD
    if (isHash) {
        ofstream head(".mygit/HEAD", ios::trunc);
        if (!head.is_open()) {
            cerr << "Error: Could not open .mygit/HEAD for writing" << endl;
            return;
        }
        head << targetCommitHash;
        head.close();
    }
    else {
        ofstream head(".mygit/HEAD", ios::trunc);
        if (!head.is_open()) {
            cerr << "Error: Could not open .mygit/HEAD for writing" << endl;
            return;
        }
        head << "ref: " + targetRef;
        head.close();
    }

}
