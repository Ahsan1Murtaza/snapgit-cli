// SPDX-License-Identifier: MIT
// Implementation for RestoreTree.

#include "RestoreTree.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

// Global variable to track index entries
vector<pair<string, string>> indexEntries;

/**
 * @brief Performs restore tree.
 * @param treeHash Hash of the tree object to read.
 * @param basePath Base path used while walking tree entries.
 */
void restoreTree(const string& treeHash, const string& basePath) {
    string folder = ".mygit/objects/" + treeHash.substr(0, 2);
    string file = folder + "/" + treeHash.substr(2);
    
    ifstream in(file);
    if (!in.is_open()) {
        cerr << "Error: Tree object not found: " << treeHash << endl;
        return;
    }
    
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        string mode, type, hash, name;
        iss >> mode >> type >> hash >> name;
        
        if (name.empty()) continue;
        
        string fullPath = basePath.empty() ? name : basePath + "/" + name;
        fullPath = fs::weakly_canonical(fullPath).string();
        fs::path current = fs::current_path();
        string rel = fs::relative(fullPath, current).generic_string(); // relative to repo root
        
        if (type == "blob") {
            // Restore file from blob
            string blobFolder = ".mygit/objects/" + hash.substr(0, 2);
            string blobFile = blobFolder + "/" + hash.substr(2);
            
            ifstream blobIn(blobFile, ios::binary);
            if (!blobIn.is_open()) {
                cerr << "Error: Blob not found: " << hash << endl;
                continue;
            }
            
            fs::path filePath(fullPath);
            if (filePath.has_parent_path()) {
                fs::create_directories(filePath.parent_path());
            }
            
            ofstream out(fullPath, ios::binary);
            out << blobIn.rdbuf();
            blobIn.close();
            out.close();
            
            cout << "Restored: " << fullPath << endl;
            
            // Add to index entries
            indexEntries.push_back({fullPath, hash});
        }
        else if (type == "tree") {
            fs::create_directories(fullPath);
            restoreTree(hash, fullPath);  // Recursive call
        }
    }
    
    in.close();
}

// Function to write index file
/**
 * @brief Writes index file to repository storage.
 */
void writeIndexFile() {
    ofstream indexFile(".mygit/index");
    for (const auto& entry : indexEntries) {
        indexFile << entry.first << " " << entry.second << "\n";
    }
    indexFile.close();
    indexEntries.clear();  // Clear for next use
}
