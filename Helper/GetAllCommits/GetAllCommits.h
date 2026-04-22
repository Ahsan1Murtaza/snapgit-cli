// SPDX-License-Identifier: MIT
// Declarations for GetAllCommits.

#ifndef GET_ALL_COMMITS_H
#define GET_ALL_COMMITS_H

#include <string>
#include <vector>
using namespace std;

struct CommitInfo {
    string hash;
    string tree;
    vector<string> parents; // Supports multi parents
    string author;
    string email;
    string message;
};

/**
 * @brief getAllCommits operation.
 * @param headCommitHash Parameter description.
 * @return Return value description.
 */
vector<CommitInfo> getAllCommits(const string &headCommitHash);

#endif
