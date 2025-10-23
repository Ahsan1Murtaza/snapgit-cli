

// It will manage with commit class
// But for now it will use Tree class to check functionality

#include "CommitHandler.h"
#include "../../Objects/Tree/Tree.h"

#include <iostream>
#include <string>
#include <filesystem>
#include "../../Helper/RepoCheck/RepoCheck.h"

using namespace std;

CommitHandler::CommitHandler() {};

void CommitHandler::handleCommit(const string &message)
{
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Please run 'init' command first.\n";
        return;
    }

    Tree tree;
    tree.buildFromIndex(".mygit/index");
    tree.showTreeElements();

    tree.save();

    cout << "Committed with message: " << message << "\n";
}
