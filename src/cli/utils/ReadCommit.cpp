// SPDX-License-Identifier: MIT
// Implementation for ReadCommit.

#include "ReadCommit.h"

#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

/**
 * @brief Reads commit from repository storage.
 * @param commitHash Hash of the commit object.
 * @return Requested value produced by this function.
 */
CommitData readCommit(const string& commitHash) {
    CommitData data;
    
    string folder = ".snapgit/objects/" + commitHash.substr(0, 2);
    string file = folder + "/" + commitHash.substr(2);
    
    ifstream in(file);
    if (!in.is_open()) {
        cerr << "Error: Commit object not found: " << commitHash << endl;
        return data;
    }
    
    string line;
    while (getline(in, line)) {
        if (line.rfind("tree ", 0) == 0) {
            data.treeHash = line.substr(5);
        }
        else if (line.rfind("parent ", 0) == 0) {
            data.parentHashes.push_back(line.substr(7));
        }
        else if (line.rfind("author ", 0) == 0) {
            data.author = line.substr(7);
        }
        else if (line.rfind("email ", 0) == 0) {
            data.email = line.substr(6);
        }
        else if (line.rfind("message ", 0) == 0) {
            data.message = line.substr(8);
        }
    }
    
    in.close();
    return data;
}
