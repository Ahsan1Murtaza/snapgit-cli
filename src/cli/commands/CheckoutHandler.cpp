// SPDX-License-Identifier: MIT
// Implementation for CheckoutHandler.

#include "CheckoutHandler.h"

#include "../utils/ReadIndex.h"
#include "../utils/ReadCommit.h"
#include "../utils/ReadTree.h"
#include "../utils/RepoCheck.h"
#include "../utils/GetHeadRef.h"
#include "../utils/GetCurrentCommitHash.h"
#include "../utils/RestoreTree.h"
#include "../utils/Hash.h"
#include "../utils/Ignore.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>


using namespace std;
namespace fs = std::filesystem;

CheckoutHandler::CheckoutHandler() {}

// Helper function to clear working directory except .snapgit
/**
 * @brief Performs clear working directory.
 */
void clearWorkingDirectory() {
    for (const auto& entry : fs::directory_iterator(".")) {
        string name = entry.path().filename().string();
        
        // Skip .snapgit folder
        if (name == ".snapgit" || name == ".git") {
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
 * @brief Handles the  checkout command workflow.
 * @param refInput Branch name or commit hash to check out.
 */
void CheckoutHandler::handleCheckout(const string& refInput) {
        // Check for uncommitted changes (index vs HEAD commit tree)
        string currentHead = getCurrentCommitHash();
        if (!currentHead.empty()) {
            CommitData headCommit = readCommit(currentHead);
            if (!headCommit.treeHash.empty()) {
                auto headTree = readTreeFiles(headCommit.treeHash);
                auto indexEntries = readIndex();
                bool dirty = false;
                // Check for added/modified files
                for (const auto& [file, hash] : indexEntries) {
                    auto it = headTree.find(file);
                    if (it == headTree.end() || it->second != hash) {
                        dirty = true;
                        break;
                    }
                }
                // Check for deleted files
                if (!dirty) {
                    for (const auto& [file, hash] : headTree) {
                        if (indexEntries.find(file) == indexEntries.end()) {
                            dirty = true;
                            break;
                        }
                    }
                }

                if (!dirty) {
                    // Check if working directory has unstaged modifications
                    auto ignorePatterns = readIgnorePatterns();
                    fs::recursive_directory_iterator it("."), end;
                    while (it != end && !dirty) {
                        const fs::directory_entry& entry = *it;
                        fs::path relPath = fs::relative(entry.path(), fs::current_path());
                        string rel = relPath.generic_string();

                        if (entry.is_directory()) {
                            if (rel == ".snapgit" || rel.rfind(".snapgit/", 0) == 0 || isIgnoredPath(rel + "/", ignorePatterns)) {
                                it.disable_recursion_pending();
                            }
                            ++it;
                            continue;
                        }

                        if (!entry.is_regular_file()) {
                            ++it;
                            continue;
                        }

                        if (rel.rfind(".snapgit/", 0) == 0 || isIgnoredPath(rel, ignorePatterns)) {
                            ++it;
                            continue;
                        }

                        ifstream file(entry.path(), ios::binary);
                        if (file) {
                            stringstream buffer;
                            buffer << file.rdbuf();
                            string content = buffer.str();

                            string blobData = "blob " + to_string(content.size()) + "\0" + content;
                            string hash = sha1(blobData);

                            auto idxIt = indexEntries.find(rel);
                            // Untracked or unstaged change
                            if (idxIt == indexEntries.end() || idxIt->second != hash) {
                                dirty = true;
                                break;
                            }
                        }
                        ++it;
                    }

                    // Check for files deleted from working directory but present in index
                    if (!dirty) {
                        for (const auto& [file, hash] : indexEntries) {
                            if (!fs::exists(file)) {
                                dirty = true;
                                break;
                            }
                        }
                    }
                }

                if (dirty) {
                    cerr << "Error: You have uncommitted changes. Please commit them before checking out another branch or commit.\n";
                    return;
                }
            }
        }
    // Check if repo exists
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'snapgit init' first.\n";
        return;
    }
    
    string targetCommitHash;
    string targetRef;

    // Detect whether input is a commit hash or branch name
    bool isHash = (refInput.size() == 40 && all_of(refInput.begin(), refInput.end(), ::isxdigit));

    if (isHash) {
        // Checkout by Commit hash
        string commitPath = ".snapgit/objects/" + refInput.substr(0,2) + "/" + refInput.substr(2);
        if (!fs::exists(commitPath)) {
            cerr << "Error: Commit hash '" << refInput << "' not found.\n";
            return;
        }

        targetCommitHash = refInput;
    }
    else{
        // Checkout by Branch name
        targetRef = "refs/heads/" + refInput;
        string branchPath = ".snapgit/" + targetRef;
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
        ofstream head(".snapgit/HEAD", ios::trunc);
        if (!head.is_open()) {
            cerr << "Error: Could not open .snapgit/HEAD for writing" << endl;
            return;
        }
        head << targetCommitHash;
        head.close();
    }
    else {
        ofstream head(".snapgit/HEAD", ios::trunc);
        if (!head.is_open()) {
            cerr << "Error: Could not open .snapgit/HEAD for writing" << endl;
            return;
        }
        head << "ref: " + targetRef;
        head.close();
    }

}
