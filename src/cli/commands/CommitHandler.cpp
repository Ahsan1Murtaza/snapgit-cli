// SPDX-License-Identifier: MIT
// Implementation for CommitHandler.

#include "CommitHandler.h"

#include "../../core/objects/Tree.h"
#include "../../core/objects/Commit.h"

#include "../utils/RepoCheck.h"
#include "../utils/GetCurrentCommitHash.h"
#include "../utils/UpdateHead.h"
#include "../utils/GetUserInfo.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>


using namespace std;

CommitHandler::CommitHandler() {};

/*
This function handles Commit Command

    Create Tree instance
    calls buildFromIndex
    calls save

    Create Commit object
    calls save
    calls helper method to update head

*/
void CommitHandler::handleCommit(const string &message)
{
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Please run 'init' command first.\n";
        return;
    }
    
    //Read user config
    auto [name, email] = getUserInfoFromConfig();
    if (name.empty() || email.empty()) {
        cerr << "Error: User name or email not set.\n";
        cerr << "Please config first.\n";
        return;
    }

    Tree tree;
    tree.buildFromIndex(".snapgit/index");
    // tree.showTreeElements();

    tree.save();

    // Wrap single parent into vector
    vector<string> parents;

    // Parent # 1 : HEAD
    string currentHead = getCurrentCommitHash();
    if (!currentHead.empty()) {
        parents.push_back(currentHead);
    }

    // Parent #2 (if merge): MERGE_HEAD
    string mergeHeadFile = ".snapgit/MERGE_HEAD";
    if (filesystem::exists(mergeHeadFile)) {
       ifstream mh(mergeHeadFile);
       string mergeParent;
       getline(mh, mergeParent);
       mh.close();

       if (!mergeParent.empty()) {
           parents.push_back(mergeParent);
       }
    }


    // Get current time as unix timestamp
    long long now = static_cast<long long>(time(nullptr));
    Commit commit(tree.getHash(), parents, message, name, email, now);
    commit.save();
    updateHead(commit.getHash());

    // cout << "Committed with message: " << message << "\n";

    // Cleanup MERGE_HEAD
    if (filesystem::exists(mergeHeadFile)) {
        filesystem::remove(mergeHeadFile);
    }
}
