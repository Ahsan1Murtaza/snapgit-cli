// SPDX-License-Identifier: MIT
// Implementation for MergeHandler.


#include "MergeHandler.h"

#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/ReadCommit/ReadCommit.h"
#include "../../Objects/Commit/Commit.h"
#include "../../Objects/Tree/Tree.h"
#include "../../Helper/RestoreTree/RestoreTree.h"
#include "../../Helper/Hash/Hash.h"
#include "../../Helper/GetUserInfo/GetUserInfo.h"
#include "../../Helper/UpdateHead/UpdateHead.h"
#include "../../Helper/ReadTree/ReadTree.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;
namespace fs = std::filesystem;


/*
  Helper: findCommonAncestor (simple walk up parents from A, then climb B until found)
  Works fine for typical DAGs. For very large repos you could optimize.
*/
/**
 * @brief Performs find common ancestor simple.
 * @param a Input value for `a`.
 * @param b Input value for `b`.
 * @return Requested string value.
 */
static string findCommonAncestorSimple(const string &a, const string &b) {
    if (a.empty() || b.empty()) return "";

    unordered_set<string> visited;
    string cur = a;
    // walk up first parent chain (this replicates git's "first-parent" traversal for base detection)
    while (!cur.empty()) {
        visited.insert(cur);
        CommitData cd = readCommit(cur);
        if (cd.parentHashes.empty()) break;
        // use first parent (git's default base-search uses full graph; this is adequate for basic merge)
        cur = cd.parentHashes.front();
    }

    cur = b;
    while (!cur.empty()) {
        if (visited.count(cur)) return cur;
        CommitData cd = readCommit(cur);
        if (cd.parentHashes.empty()) break;
        cur = cd.parentHashes.front();
    }
    return "";
}

/*
  Helper: write conflict markers into a working file between HEAD and otherBranch versions.
  It writes:
    <<<<<<< HEAD
    <current content>
    =======
    <other content>
    >>>>>>> otherBranch
*/
/**
 * @brief Writes conflict markers to repository storage.
 * @param path Filesystem path to process.
 * @param currentHash Input value for `currentHash`.
 * @param otherHash Input value for `otherHash`.
 * @param otherBranchName Input value for `otherBranchName`.
 */
static void writeConflictMarkers(const string& path, const string& currentHash, const string& otherHash, const string& otherBranchName) {
    string curBlob = ".mygit/objects/" + currentHash.substr(0,2) + "/" + currentHash.substr(2);
    string othBlob = ".mygit/objects/" + otherHash.substr(0,2) + "/" + otherHash.substr(2);

    string curContent, othContent;
    {
        ifstream f(curBlob, ios::binary);
        if (f.is_open()) {
            ostringstream ss; ss << f.rdbuf();
            curContent = ss.str();
            f.close();
        }
    }
    {
        ifstream f(othBlob, ios::binary);
        if (f.is_open()) {
            ostringstream ss; ss << f.rdbuf();
            othContent = ss.str();
            f.close();
        }
    }

    // Ensure parent directory exists
    fs::path fp(path);
    if (fp.has_parent_path()) fs::create_directories(fp.parent_path());

    ofstream out(path, ios::binary | ios::trunc);
    out << "<<<<<<< HEAD\n";
    out << curContent;
    if (!curContent.empty() && curContent.back() != '\n') out << "\n";
    out << "=======\n";
    out << othContent;
    if (!othContent.empty() && othContent.back() != '\n') out << "\n";
    out << ">>>>>>> " << otherBranchName << "\n";
    out.close();
}

/*
  Helper: updateIndexFromTree
  Write index in your repository format: "path hash"
*/
/**
 * @brief Updates index from tree.
 * @param treeHash Hash of the tree object to read.
 */
static void updateIndexFromTree(const string& treeHash) {
    auto files = readTreeFiles(treeHash);
    ofstream idx(".mygit/index", ios::trunc);
    if (!idx.is_open()) {
        cerr << "Error: Unable to open .mygit/index for writing\n";
        return;
    }
    for (auto &p : files) {
        idx << p.first << " " << p.second << "\n";
    }
    idx.close();
}

/*
  Main merge handler
  otherBranch: branch name (e.g., "feature-b")
*/
/**
 * @brief Handles the  merge command workflow.
 * @param otherBranch Name of the branch to merge into the current branch.
 */
void MergeHandler::handleMerge(const string& otherBranch) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized.\n";
        return;
    }

    // get current commit hash (HEAD for current branch)
    string currentHead = getCurrentCommitHash();
    if (currentHead.empty()) {
        cerr << "Error: no commits in current branch to merge into.\n";
        return;
    }

    // load other branch's head
    string branchRefPath = ".mygit/refs/heads/" + otherBranch;
    if (!fs::exists(branchRefPath)) {
        cerr << "Error: Branch '" << otherBranch << "' not found\n";
        return;
    }
    string otherHead;
    {
        ifstream f(branchRefPath);
        getline(f, otherHead);
        f.close();
    }
    if (otherHead.empty()) {
        cerr << "Error: Branch '" << otherBranch << "' has no commits\n";
        return;
    }

    // read commits
    CommitData curCommit = readCommit(currentHead);
    CommitData othCommit = readCommit(otherHead);

    // find merge base
    string base = findCommonAncestorSimple(currentHead, otherHead);

    string baseTree = base.empty() ? string() : readCommit(base).treeHash;
    string curTree  = curCommit.treeHash;
    string othTree  = othCommit.treeHash;

    // read file maps: path -> blobHash
    auto baseFiles = baseTree.empty() ? unordered_map<string,string>() : readTreeFiles(baseTree);
    auto curFiles  = curTree.empty() ? unordered_map<string,string>() : readTreeFiles(curTree);
    auto othFiles  = othTree.empty() ? unordered_map<string,string>() : readTreeFiles(othTree);

    // collect all paths
    unordered_set<string> allPaths;
    for (auto &p : baseFiles) allPaths.insert(p.first);
    for (auto &p : curFiles) allPaths.insert(p.first);
    for (auto &p : othFiles) allPaths.insert(p.first);

    unordered_map<string,string> mergedFiles;
    bool hasConflict = false;

    for (const string &path : allPaths) {
        string baseHash = baseFiles.count(path) ? baseFiles[path] : "";
        string curHash  = curFiles.count(path)  ? curFiles[path]  : "";
        string othHash  = othFiles.count(path)  ? othFiles[path]  : "";

        // Case: identical (including both empty)
        if (curHash == othHash) {
            if (!curHash.empty()) mergedFiles[path] = curHash;
            continue;
        }

        // Case: only current changed
        if (curHash != baseHash && othHash == baseHash) {
            if (!curHash.empty()) mergedFiles[path] = curHash;
            continue;
        }

        // Case: only other changed
        if (othHash != baseHash && curHash == baseHash) {
            if (!othHash.empty()) mergedFiles[path] = othHash;
            continue;
        }

        // Case: both changed differently -> conflict
        if (!curHash.empty() && !othHash.empty() && curHash != othHash) {
            hasConflict = true;
            cerr << "Conflict: " << path << "\n";
            // write conflict markers to working directory file
            writeConflictMarkers(path, curHash, othHash, otherBranch);
            continue;
        }

        // Case: added in one side, removed in another
        if (baseHash.empty() && curHash.empty() && !othHash.empty()) {
            // added only on other
            mergedFiles[path] = othHash;
            continue;
        }
        if (baseHash.empty() && othHash.empty() && !curHash.empty()) {
            // added only on current
            mergedFiles[path] = curHash;
            continue;
        }

        // Case: removed on one side, changed on the other
        if (baseHash == curHash && othHash.empty()) {
            // other removed -> removal wins if current untouched: file removed
            // do nothing (file not included in mergedFiles => removed)
            continue;
        }
        if (baseHash == othHash && curHash.empty()) {
            // current removed, other unchanged => removed
            continue;
        }

        // Fallback: prefer current if exists
        if (!curHash.empty()) mergedFiles[path] = curHash;
        else if (!othHash.empty()) mergedFiles[path] = othHash;
    }

    // If conflicts found, create MERGE_HEAD and exit (user must fix + add + commit)
    if (hasConflict) {
        ofstream mergeHead(".mygit/MERGE_HEAD", ios::trunc);
        if (mergeHead.is_open()) {
            mergeHead << otherHead << "\n";
            mergeHead.close();
        } else {
            cerr << "Error: cannot write .mygit/MERGE_HEAD\n";
        }
        cerr << "Automatic merge stopped due to conflicts. Resolve them, 'mygit add' the files and commit.\n";
        return;
    }

    // No conflicts -> build a temp index-style file, then build tree via your existing Tree builder
    const string tmpIndex = ".mygit/temp_merge_index";
    {
        ofstream tmp(tmpIndex, ios::trunc);
        if (!tmp.is_open()) {
            cerr << "Error: cannot write temporary index\n";
            return;
        }
        // your Tree::buildFromIndex expects lines like "path hash" (you used that earlier)
        for (auto &p : mergedFiles) {
            tmp << p.first << " " << p.second << "\n";
        }
        tmp.close();
    }

    // Build merged tree object
    Tree mergedTree;
    mergedTree.buildFromIndex(tmpIndex);
    mergedTree.save();
    string mergedTreeHash = mergedTree.getHash();

    // Create merge commit with two parents
    auto [name, email] = getUserInfoFromConfig();
    vector<string> parents = { currentHead, otherHead };
    Commit mergeCommit(mergedTreeHash, parents, string("Merge branch '") + otherBranch + "'", name, email);
    mergeCommit.save();
    string mergeHash = mergeCommit.getHash();

    // Update HEAD (update branch ref or HEAD file depending on if HEAD is attached)
    updateHead(mergeHash);

    // Restore working directory from merged tree and update index
    restoreTree(mergedTreeHash, "");
    updateIndexFromTree(mergedTreeHash);

    // remove temporary index
    try { fs::remove(tmpIndex); } catch(...) {}

    // remove MERGE_HEAD if present (clean state)
    try { fs::remove(".mygit/MERGE_HEAD"); } catch(...) {}

    cout << "Merge successful: merged branch '" << otherBranch << "' into current branch.\n";
    cout << "Created merge commit " << mergeHash << "\n";
}
