#include "CommitHandler.h"
#include "../../Objects/Tree/Tree.h"
#include "../../Objects/Commit/Commit.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/UpdateHead/UpdateHead.h"
#include "../../Helper/GetUserInfo/GetUserInfo.h"


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
    tree.buildFromIndex(".mygit/index");
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
    string mergeHeadFile = ".mygit/MERGE_HEAD";
    if (filesystem::exists(mergeHeadFile)) {
       ifstream mh(mergeHeadFile);
       string mergeParent;
       getline(mh, mergeParent);
       mh.close();

       if (!mergeParent.empty()) {
           parents.push_back(mergeParent);
       }
    }

    Commit commit(tree.getHash(), parents, message, name, email);

    commit.save();
    updateHead(commit.getHash());

    // cout << "Committed with message: " << message << "\n";

    // Cleanup MERGE_HEAD
    if (filesystem::exists(mergeHeadFile)) {
        filesystem::remove(mergeHeadFile);
    }
}
