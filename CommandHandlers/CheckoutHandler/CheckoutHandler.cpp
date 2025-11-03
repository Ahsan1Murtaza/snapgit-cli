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

// Helper function to clear working directory except .mygit
void clearWorkingDirectory() {
    for (const auto& entry : fs::directory_iterator(".")) {
        string name = entry.path().filename().string();
        
        // Skip .mygit folder
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

void CheckoutHandler::handleCheckout(const string& branchName, bool createBranch) {
    // Check if repo exists
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'mygit init' first.\n";
        return;
    }
    
    string targetBranch = "refs/heads/" + branchName;
    string branchPath = ".mygit/" + targetBranch;
    
    // If creating new branch with -b flag
    if (createBranch) {
        // Check if branch already exists
        if (fs::exists(branchPath)) {
            cerr << "Error: Branch '" << branchName << "' already exists.\n";
            return;
        }
        
        // Get current commit hash
        string currentCommit = getCurrentCommitHash();
        if (currentCommit.empty()) {
            cerr << "Error: No commits yet. Cannot create branch.\n";
            return;
        }
        
        // Create new branch pointing to current commit
        fs::create_directories(".mygit/refs/heads");
        ofstream branchFile(branchPath);
        branchFile << currentCommit;
        branchFile.close();
        
        cout << "Created new branch: " << branchName << endl;
    }
    else {
        // Check if branch exists
        if (!fs::exists(branchPath)) {
            cerr << "Error: Branch '" << branchName << "' does not exist.\n";
            return;
        }
    }
    
    // Get current branch
    string currentRef = getHeadRef();
    if (currentRef == targetBranch) {
        cout << "Already on branch '" << branchName << "'\n";
        return;
    }
    
    // Read target branch's commit hash
    ifstream branchFile(branchPath);
    string targetCommitHash;
    getline(branchFile, targetCommitHash);
    branchFile.close();
    
    if (targetCommitHash.empty()) {
        cerr << "Error: Branch '" << branchName << "' has no commits.\n";
        return;
    }
    
    // Read commit to get tree hash
    CommitData commitData = readCommit(targetCommitHash);
    if (commitData.treeHash.empty()) {
        cerr << "Error: Failed to read commit: " << targetCommitHash << endl;
        return;
    }
    
    cout << "Switching to branch '" << branchName << "'...\n";
    
    // Step 1: Clear working directory (except .mygit)
    cout << "Clearing working directory...\n";
    clearWorkingDirectory();
    
    // Step 2: Restore files from target branch's tree
    cout << "Restoring files from branch '" << branchName << "'...\n";
    restoreTree(commitData.treeHash);
    
    // ✨ Step 3: Write index file with restored files
    writeIndexFile();
    cout << "Updated index with restored files\n";
    
    // Step 4: Update HEAD to point to new branch
    ofstream headFile(".mygit/HEAD", ios::trunc);
    headFile << "ref: " << targetBranch;
    headFile.close();
    
    cout << "\nSwitched to branch '" << branchName << "'\n";
    cout << "HEAD is now at " << targetCommitHash.substr(0, 7) << endl;
}