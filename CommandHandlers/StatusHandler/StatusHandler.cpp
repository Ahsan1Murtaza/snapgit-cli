#include "StatusHandler.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

// ANSI colors
static const char* COLOR_RESET = "\033[0m";
static const char* COLOR_GREEN = "\033[32m"; // new / staged 
static const char* COLOR_YELLOW = "\033[33m"; // modified
static const char* COLOR_RED = "\033[31m"; // deleted
static const char* COLOR_HEADER = "\033[36m";

// read file helper
static string readFile(const string &path) {
    ifstream f(path, ios::binary);
    if (!f.is_open()) return "";
    ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// resolve object path (xx/yyy or flat)
static string getObjectPath(const string &hash) {
    if (hash.size() >= 3) {
        string p = ".mygit/objects/" + hash.substr(0,2) + "/" + hash.substr(2);
        if (fs::exists(p)) return p;
    }
    string flat = ".mygit/objects/" + hash;
    return flat;
}

// Parse a tree object and return a map of path -> blobHash (recursive)
static void parseTreeToMap(const string &treeHash, unordered_map<string,string> &outMap, const string &prefix="") {
    if (treeHash.empty()) return;
    string path = getObjectPath(treeHash);
    if (!fs::exists(path)) return;

    string content = readFile(path);
    istringstream ss(content);
    string line;
    while (getline(ss, line)) {
        if (line.empty()) continue;
        // support two formats:
        // 1) "blob <name> <hash>"
        // 2) "<hash> <filepath>"
        istringstream ls(line);
        string a, b, c;
        ls >> a >> b >> c;
        bool firstIsHash = (a.size() >= 6 && all_of(a.begin(), a.end(), [](unsigned char ch){ return isxdigit(ch); }));
        string type, name, hash;
        if (firstIsHash && !b.empty()) {
            type = "blob";
            hash = a;
            name = b;
        } else {
            type = a;
            name = b;
            hash = c;
        }

        string fullPath = prefix.empty() ? name : (prefix + "/" + name);
        if (type == "blob") {
            outMap[fullPath] = hash;
        } else if (type == "tree") {
            // subtree: recurse
            parseTreeToMap(hash, outMap, fullPath);
        }
    }
}

// Read index entries into map: path -> blobHash (support "hash path" or just "path" lines)
static unordered_map<string,string> readIndexMap() {
    unordered_map<string,string> m;
    string idx = ".mygit/index";
    if (!fs::exists(idx)) return m;
    ifstream f(idx);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string first, second;
        ss >> first;
        if (!first.empty() && first.size() >= 6 && all_of(first.begin(), first.end(), [](unsigned char ch){ return isxdigit(ch); })) {
            // format: "<hash> <path>"
            ss >> second;
            if (!second.empty()) m[second] = first;
        } else {
            // assume line is path only
            string path = line;
            // trim
            while (!path.empty() && isspace((unsigned char)path.back())) path.pop_back();
            if (!path.empty()) m[path] = ""; // unknown hash
        }
    }
    return m;
}

static bool contentEqualsFileHash(const string &filePath, const string &blobHash) {
    // If blobHash empty -> cannot compare reliably -> treat as modified
    if (blobHash.empty()) return false;
    string objPath = getObjectPath(blobHash);
    if (!fs::exists(objPath)) return false;
    string blobContent = readFile(objPath);
    string workContent = readFile(filePath);
    return blobContent == workContent;
}

static string getCurrentCommitTreeHash() {
    // Read current commit hash from HEAD ref
    string head = readFile(".mygit/HEAD");
    if (head.rfind("ref:",0) == 0) {
        string ref = head.substr(5);
        while (!ref.empty() && isspace((unsigned char)ref.back())) ref.pop_back();
        string refPath = ".mygit/" + ref;
        string hash = readFile(refPath);
        while (!hash.empty() && isspace((unsigned char)hash.back())) hash.pop_back();
        // read commit object to find tree line
        string commitPath = getObjectPath(hash);
        if (!fs::exists(commitPath)) return "";
        string content = readFile(commitPath);
        istringstream ss(content);
        string line;
        while (getline(ss, line)) {
            if (line.rfind("tree ",0) == 0) {
                string t = line.substr(5);
                while (!t.empty() && isspace((unsigned char)t.back())) t.pop_back();
                return t;
            }
        }
    }
    return "";
}

void StatusHandler::handleStatus() {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'init' first.\n";
        return;
    }

    // Determine current branch
    string head = readFile(".mygit/HEAD");
    string branch = "HEAD";
    if (head.rfind("ref:",0) == 0) {
        branch = head.substr(5);
        while (!branch.empty() && isspace((unsigned char)branch.back())) branch.pop_back();
    }

    cout << COLOR_HEADER << "On branch " << COLOR_RESET << branch << "\n\n";

    // Build maps:
    // 1) committed files map from latest commit tree
    unordered_map<string,string> committed;
    string treeHash = getCurrentCommitTreeHash();
    if (!treeHash.empty()) {
        parseTreeToMap(treeHash, committed, "");
    }

    // 2) index (staged) map
    unordered_map<string,string> indexMap = readIndexMap();

    // 3) working directory: list files (skip .mygit)
    vector<string> workingFiles;
    for (auto &p : fs::recursive_directory_iterator(fs::current_path())) {
        if (!p.is_regular_file()) continue;
        string fp = p.path().string();
        // skip .mygit
        if (fp.find(".mygit") != string::npos) continue;
        // convert to relative path from repo root
        string rel = fs::relative(p.path(), fs::current_path()).generic_string();
        workingFiles.push_back(rel);
    }
    sort(workingFiles.begin(), workingFiles.end());

    vector<string> untracked;
    vector<string> modified;
    vector<string> deleted;
    vector<string> staged;

    // Determine staged files: those in index
    for (auto &kv : indexMap) {
        if (!kv.second.empty()) staged.push_back(kv.first);
        else {
            // staged but no hash recorded — treat as staged
            staged.push_back(kv.first);
        }
    }

    // For each working file, decide untracked or modified or staged
    for (auto &fpath : workingFiles) {
        auto itCommitted = committed.find(fpath);
        auto itIndex = indexMap.find(fpath);

        if (itIndex == indexMap.end() && itCommitted == committed.end()) {
            // not in index and not in commit -> untracked
            untracked.push_back(fpath);
        } else if (itCommitted != committed.end()) {
            // file tracked in last commit -> compare content
            string blobHash = itCommitted->second;
            bool equal = contentEqualsFileHash(fpath, blobHash);
            if (!equal) {
                // if also in index with same blob -> staged; else modified
                if (itIndex != indexMap.end() && !itIndex->second.empty() && contentEqualsFileHash(fpath, itIndex->second)) {
                    // staged (index points to same content)
                    // already captured in staged list
                } else {
                    modified.push_back(fpath);
                }
            }
        } else if (itIndex != indexMap.end()) {
            // in index but not in commit -> staged (new file staged)
            // already added to staged
        }
    }

    // Deleted files: files that were in commit but missing from working dir
    for (auto &kv : committed) {
        string file = kv.first;
        bool exists = fs::exists(fs::current_path() / file);
        if (!exists) deleted.push_back(file);
    }

    // Print staged
    if (!staged.empty()) {
        cout << COLOR_GREEN << "Changes to be committed:\n" << COLOR_RESET;
        for (auto &s : staged) cout << "  " << s << "\n";
        cout << "\n";
    }

    // Print modified
    if (!modified.empty()) {
        cout << COLOR_YELLOW << "Changes not staged for commit:\n" << COLOR_RESET;
        for (auto &m : modified) cout << "  modified: " << m << "\n";
        cout << "\n";
    }

    // Print deleted
    if (!deleted.empty()) {
        cout << COLOR_RED << "Deleted files:\n" << COLOR_RESET;
        for (auto &d : deleted) cout << "  deleted: " << d << "\n";
        cout << "\n";
    }

    // Print untracked
    if (!untracked.empty()) {
        cout << COLOR_GREEN << "Untracked files:\n" << COLOR_RESET;
        for (auto &u : untracked) cout << "  " << u << "\n";
        cout << "\n";
    }

    if (staged.empty() && modified.empty() && deleted.empty() && untracked.empty()) {
        cout << "nothing to commit, working tree clean\n";
    }
}
