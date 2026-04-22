// SPDX-License-Identifier: MIT
// Implementation for ReadTree.

#include "ReadTree.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Reads tree files from repository storage.
 * @param treeHash Hash of the tree object to read.
 * @param basePath Base path used while walking tree entries.
 * @return Requested string value.
 */
unordered_map<string, string> readTreeFiles(const string& treeHash, const string& basePath) {
    unordered_map<string, string> result;
    if (treeHash.empty()) return result;

    string objectPath = ".mygit/objects/" + treeHash.substr(0, 2) + "/" + treeHash.substr(2);
    if (!fs::exists(objectPath)) {
        // maybe flat object stored without split
        objectPath = ".mygit/objects/" + treeHash;
        if (!fs::exists(objectPath)) return result;
    }

    ifstream in(objectPath);
    if (!in.is_open()) return result;

    string line;
    while (getline(in, line)) {
        if (line.size() == 0) continue;
        istringstream iss(line);
        string mode, type, hash, name;
        if (!(iss >> mode >> type >> hash)) continue;
        
        // remaining piece is the path (may contain spaces) — read rest of line
        getline(iss, name);
        // trim leading spaces in name
        while (!name.empty() && isspace((unsigned char)name.front())) name.erase(0, 1);
        if (name.empty()) continue;

        string fullPath = basePath.empty() ? name : basePath + "/" + name;
        if (type == "blob") {
            result[fullPath] = hash;
        } else if (type == "tree") {
            auto sub = readTreeFiles(hash, fullPath);
            result.insert(sub.begin(), sub.end());
        }
    }
    in.close();
    return result;
}
