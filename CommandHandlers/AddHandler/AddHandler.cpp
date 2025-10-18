#include "AddHandler.h"
#include "../../Objects/Blob/Blob.h"
#include "../../Helper/RepoCheck/RepoCheck.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

AddHandler::AddHandler() {}

void AddHandler::handleAdd(const string& filePath) {

    if (!filesystem::exists(filePath)) {
        cerr << "Error: File " << filePath << " does not exist.\n";
        return;
    }

    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Please run 'init' command first.\n";
        return;
    }

    Blob blob(filePath);
    blob.save();

    // Add to Staging Area
    ofstream indexFile(".mygit/index", std::ios::app);
    if (indexFile) {
        indexFile << filePath << " " << blob.getHash() << "\n";
        indexFile.close();
        cout << "Added " << filePath << " to staging area \n";
    }
}