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
 * @brief readCommit operation.
 * @param commitHash Parameter description.
 * @return Return value description.
 */
CommitData readCommit(const string& commitHash);

#endif
