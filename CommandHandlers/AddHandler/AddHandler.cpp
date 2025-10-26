#include "AddHandler.h"
#include "../../Objects/Blob/Blob.h"
#include "../../Helper/RepoCheck/RepoCheck.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

AddHandler::AddHandler() {}

/*
This function handles Add Command
    Creates Blob object
    calls save
    add file to staging area (Index File)
*/
void AddHandler::handleAdd(const string& filePath) {

    if (!filesystem::exists(filePath)) {
        cerr << "Error: File " << filePath << " does not exist.\n";
        return;
    }

    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Please run 'init' command first.\n";
        return;
    }

    filesystem::path p(filePath);
    p = filesystem::weakly_canonical(p); // resolves . , .. and absolute structure if exists
    filesystem::path current = filesystem::current_path(); // converts absolute path to relative from repo
    string rel = filesystem::relative(p, current).generic_string();

    // ensure consistent slashes and remove leading ./
    if (rel.rfind("./", 0) == 0) {
        rel = rel.substr(2);
    }

    cout << "Original Path " << filePath << endl; 
    cout << "Relative Path " << rel << endl; 

    Blob blob(rel);
    blob.save();

    // Add to Staging Area
    ofstream indexFile(".mygit/index", std::ios::app);
    if (indexFile) {
        indexFile << rel << " " << blob.getHash() << "\n";
        indexFile.close();
        cout << "Added " << rel << " to staging area \n";
    }
}