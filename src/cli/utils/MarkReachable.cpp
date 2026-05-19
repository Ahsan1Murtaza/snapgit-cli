// SPDX-License-Identifier: MIT
// Implementation for MarkReachable.

#include "MarkReachable.h"
#include "ReadCommit.h"
#include "ReadIndex.h"
#include "GetHeadRef.h"
#include "GetCurrentCommitHash.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <cctype>

using namespace std;
namespace fs = std::filesystem;

static string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace((unsigned char)s[i])) ++i;
    while (j > i && isspace((unsigned char)s[j - 1])) --j;
    return s.substr(i, j - i);
}

static string readRefFile(const fs::path& path) {
    ifstream in(path);
    if (!in.is_open()) return "";
    string line;
    getline(in, line);
    return trim(line);
}

static void markTree(const string& treeHash, unordered_set<string>& reachable);

static void markCommit(const string& commitHash, unordered_set<string>& reachable) {
    if (commitHash.empty()) return;
    if (!reachable.insert(commitHash).second) return;

    CommitData commit = readCommit(commitHash);
    if (!commit.treeHash.empty()) {
        markTree(trim(commit.treeHash), reachable);
    }
    for (const string& parent : commit.parentHashes) {
        markCommit(trim(parent), reachable);
    }
}

static void markTree(const string& treeHash, unordered_set<string>& reachable) {
    if (treeHash.empty()) return;
    if (!reachable.insert(treeHash).second) return;

    string objectPath = ".snapgit/objects/" + treeHash.substr(0, 2) + "/" + treeHash.substr(2);
    if (!fs::exists(objectPath)) {
        objectPath = ".snapgit/objects/" + treeHash;
        if (!fs::exists(objectPath)) return;
    }

    ifstream in(objectPath);
    if (!in.is_open()) return;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string mode, type, hash;
        if (!(iss >> mode >> type >> hash)) continue;
        hash = trim(hash);
        if (hash.empty()) continue;

        reachable.insert(hash);
        if (type == "tree") {
            markTree(hash, reachable);
        }
    }
}

static void markCommitRoots(unordered_set<string>& reachable) {
    const fs::path headsDir = ".snapgit/refs/heads";
    if (!fs::exists(headsDir)) return;

    for (const auto& entry : fs::directory_iterator(headsDir)) {
        if (!entry.is_regular_file()) continue;
        string commitHash = readRefFile(entry.path());
        markCommit(commitHash, reachable);
    }

    // getHeadRef() returns empty when HEAD points directly at a commit (detached)
    if (getHeadRef().empty()) {
        markCommit(getCurrentCommitHash(), reachable);
    }
}

static void markIndexBlobs(unordered_set<string>& reachable) {
    for (const auto& [path, hash] : readIndex()) {
        (void)path;
        if (!hash.empty()) {
            reachable.insert(hash);
        }
    }
}

unordered_set<string> markReachableObjects() {
    unordered_set<string> reachable;
    markCommitRoots(reachable);
    markIndexBlobs(reachable);
    return reachable;
}
