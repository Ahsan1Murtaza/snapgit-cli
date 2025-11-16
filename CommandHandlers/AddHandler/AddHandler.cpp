#include "AddHandler.h"
#include "../../Objects/Blob/Blob.h"
#include "../../Helper/RepoCheck/RepoCheck.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>

using namespace std;

AddHandler::AddHandler() {}

/*
This function handles Add Command
    Creates Blob object
    calls save
    add file to staging area (Index File)
*/
void AddHandler::handleAdd(const string& filePath) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized.\n";
        return;
    }

    filesystem::path p(filePath);
    p = filesystem::weakly_canonical(p);
    string rel = filesystem::relative(p, filesystem::current_path()).generic_string();
    for (char& c : rel) if (c == '\\') c = '/';

    // Read current index
    unordered_map<string, string> indexMap;
    ifstream indexIn(".mygit/index");
    string line;
    while (getline(indexIn, line)) {
        istringstream iss(line);
        string path, hash;
        iss >> path >> hash;
        indexMap[path] = hash;
    }
    indexIn.close();

    if (filesystem::exists(filePath)) {
        // File exists → create blob and update index
        Blob blob(p.string()); // real path
        blob.save();
        indexMap[rel] = blob.getHash();
        cout << "Added " << rel << " in staging area\n";
    }

    // Write back index
    ofstream indexOut(".mygit/index", ios::trunc);
    for (auto& [path, hash] : indexMap) {
        indexOut << path << " " << hash << "\n";
    }
}
