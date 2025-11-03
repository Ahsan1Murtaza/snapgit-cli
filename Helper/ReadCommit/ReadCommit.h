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

CommitData readCommit(const string& commitHash);

#endif