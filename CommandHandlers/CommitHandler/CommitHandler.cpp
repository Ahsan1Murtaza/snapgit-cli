

// It will manage with commit class
// But for now it will use Tree class to check functionality

#include "CommitHandler.h"
#include "../../Objects/Tree/Tree.h"
#include "../../Objects/Commit/Commit.h"

#include <iostream>
#include <string>
#include <filesystem>
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/UpdateHead/UpdateHead.h"

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

    Tree tree;
    tree.buildFromIndex(".mygit/index");
    // tree.showTreeElements();

    tree.save();

    Commit commit(tree.getHash(), getCurrentCommitHash(), message, "Ahsan Murtaza", "ahsan@gmail.com");

    commit.save();
    updateHead(commit.getHash());

    // cout << "Committed with message: " << message << "\n";
}
