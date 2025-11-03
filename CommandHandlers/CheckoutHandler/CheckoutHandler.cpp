#include "CheckoutHandler.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/ReadCommit/ReadCommit.h"
#include "../../Helper/RestoreTree/RestoreTree.h"
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

CheckoutHandler::CheckoutHandler() {}

void clearWorkingDirectory() {
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

void clearIndex() {
    ofstream indexFile(".mygit/index", ios::trunc);
    indexFile.close();
}

void CheckoutHandler::handleCheckout(const string& branchName, bool createBranch) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'mygit init' first.\n";
        return;
    }
    
    string targetBranch = "refs/heads/" + branchName;
    string branchPath = ".mygit/" + targetBranch;
    
    if (createBranch) {
        if (fs::exists(branchPath)) {
            cerr << "Error: Branch '" << branchName << "' already exists.\n";
            return;
        }
        
        string currentCommit = getCurrentCommitHash();
        if (currentCommit.empty()) {
            cerr << "Error: No commits yet. Cannot create branch.\n";
            return;
        }
        
        fs::create_directories(".mygit/refs/heads");
        ofstream branchFile(branchPath);
        branchFile << currentCommit;
        branchFile.close();
        
        cout << "Created new branch: " << branchName << endl;
    }
    else {
        if (!fs::exists(branchPath)) {
            cerr << "Error: Branch '" << branchName << "' does not exist.\n";
            return;
        }
    }
    
    string currentRef = getHeadRef();
    if (currentRef == targetBranch) {
        cout << "Already on branch '" << branchName << "'\n";
        return;
    }
    
    ifstream branchFile(branchPath);
    string targetCommitHash;
    getline(branchFile, targetCommitHash);
    branchFile.close();
    
    if (targetCommitHash.empty()) {
        cerr << "Error: Branch '" << branchName << "' has no commits.\n";
        return;
    }
    
    CommitData commitData = readCommit(targetCommitHash);
    if (commitData.treeHash.empty()) {
        cerr << "Error: Failed to read commit: " << targetCommitHash << endl;
        return;
    }
    
    cout << "Switching to branch '" << branchName << "'...\n";
    
    cout << "Clearing working directory...\n";
    clearWorkingDirectory();
    
    clearIndex();
    
    cout << "Restoring files from branch '" << branchName << "'...\n";
    restoreTree(commitData.treeHash);
    
    ofstream headFile(".mygit/HEAD", ios::trunc);
    headFile << "ref: " << targetBranch;
    headFile.close();
    
    cout << "\nSwitched to branch '" << branchName << "'\n";
    cout << "HEAD is now at " << targetCommitHash.substr(0, 7) << endl;
}