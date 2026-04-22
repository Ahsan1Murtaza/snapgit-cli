// SPDX-License-Identifier: MIT
// Declarations for ReadCommit.

#ifndef READCOMMIT_H
#define READCOMMIT_H

#include <string>
#include <vector>
using namespace std;

struct CommitData {
    string treeHash;
    vector<string> parentHashes;
    string author;
    string email;
    string message;
};

/**
 * @brief Reads commit from repository storage.
 * @param commitHash Hash of the commit object.
 * @return Requested value produced by this function.
 */
CommitData readCommit(const string& commitHash);

#endif
