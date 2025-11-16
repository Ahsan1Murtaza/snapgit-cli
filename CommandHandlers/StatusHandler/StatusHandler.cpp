#include "StatusHandler.h"
#include "../../Helper/Hash/Hash.h"
#include "../../Helper/ReadCommit/ReadCommit.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;


unordered_map<string, string> readWorkingDirectory() {
    unordered_map<string, string> result;

    for (auto& entry : fs::recursive_directory_iterator(".")) {
        if (!entry.is_regular_file()) continue;

        // Ignore .mygit
        if (entry.path().string().find(".mygit") != string::npos) continue;

        // Read file contents
        ifstream file(entry.path(), ios::binary);
        if (!file) continue;

        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();

        string blobData = "blob " + to_string(content.size()) + "\0" + content;
        string hash = sha1(blobData);

        // NORMALIZE path for index match (relative to repo root, use forward slashes)
        fs::path relPath = fs::relative(entry.path(), fs::current_path());
        string rel = relPath.generic_string();

        result[rel] = hash;
    }

    return result;
}

unordered_map<string, string> readIndex() {
    unordered_map<string, string> result;

    ifstream in(".mygit/index");
    if (!in.is_open()) return result;

    string path, hash;
    while (in >> path >> hash) {
        result[path] = hash;
    }

    return result;
}

unordered_map<string, string> readTreeFiles(const string& treeHash, const string& basePath = "") {
    unordered_map<string, string> result;

    string filePath = ".mygit/objects/" + treeHash.substr(0, 2) + "/" + treeHash.substr(2);

    ifstream in(filePath);
    if (!in.is_open()) return result;

    string mode, type, hash, name;
    while (in >> mode >> type >> hash >> name) {
        string fullPath = basePath.empty() ? name : basePath + "/" + name;
        if (type == "blob") {
            result[fullPath] = hash;
        }
        else if (type == "tree") {
            auto sub = readTreeFiles(hash, fullPath);
            result.insert(sub.begin(), sub.end());
        }
    }
    return result;
}

void StatusHandler::handleStatus() {

    unordered_map<string, string> workingDirectoryFiles = readWorkingDirectory();
    unordered_map<string, string> indexFiles = readIndex();

    unordered_map<string, string> headFiles;

    string headHash = getCurrentCommitHash();

    if (!headHash.empty()) {
        auto headCommit = readCommit(headHash);
        headFiles = readTreeFiles(headCommit.treeHash);
    }

    vector<string> untracked;
    vector<string> modifiedNotStaged;
    vector<string> stagedNew;
    vector<string> stagedModified;
    vector<string> stagedDeleted;
    vector<string> deletedNotStaged;
    vector<string> unmerged; // placeholder for merges

    unordered_set<string> allPaths;

    for (auto& [p, _] : headFiles) allPaths.insert(p);
    for (auto& [p, _] : indexFiles) allPaths.insert(p);
    for (auto& [p, _] : workingDirectoryFiles) allPaths.insert(p);

    for (auto& path : allPaths) {

        bool inHead = headFiles.count(path) > 0;
        bool inIndex = indexFiles.count(path) > 0;
        bool inWork = workingDirectoryFiles.count(path) > 0;

        string hHash = inHead ? headFiles[path] : "";
        string iHash = inIndex ? indexFiles[path] : "";
        string wHash = inWork ? workingDirectoryFiles[path] : "";

        cout << "PATH: " << path << endl;
        cout << "INDEX HASH: " << iHash << "\n";
        cout << "HEAD HASH: " << hHash << "\n";
        cout << "WORKDIR HASH: " << wHash << "\n";


        // 1) Untracked: present in work, not in index and not in HEAD
        if (!inHead && !inIndex && inWork) {
            untracked.push_back(path);
            continue;
        }

        // 2) Staged deletion: present in HEAD but missing from INDEX
        // (user removed it and staged the removal)
        if (inHead && !inIndex) {
            stagedDeleted.push_back(path);

            // If file also exists in working dir and differs from HEAD, show as modified-not-staged too
            if (inWork && wHash != hHash) {
                modifiedNotStaged.push_back(path);
            }
            continue;
        }

        // 3) Staged new file: not in HEAD, present in INDEX
        if (!inHead && inIndex) {
            stagedNew.push_back(path);

            // if also changed in working dir after staging, mark modified-not-staged
            if (inWork && iHash != wHash) {
                modifiedNotStaged.push_back(path);
            }
            continue;
        }

        // 4) Path present in both HEAD and INDEX
        if (inHead && inIndex) {
            // 4a) Staged modification (INDEX differs from HEAD)
            if (iHash != hHash) {
                stagedModified.push_back(path);
            }

            // 4b) Deleted from working directory but still in index -> deleted not staged
            if (!inWork) {
                // If index equals head it means user deleted from work but didn't stage the deletion
                // If index != head, then index already staged something — treat as deletedNotStaged only when index==head
                if (iHash == hHash) {
                    deletedNotStaged.push_back(path);
                } else {
                    // index != head (something staged). If work missing, user may have deleted again; treat as modifiedNotStaged?
                    // We'll show modifiedNotStaged if work was modified (not present => deletion), but since index differs, ignore here.
                }
            } else {
                // 4c) File exists in working dir: if working copy differs from index -> modified but not staged
                if (iHash != wHash) {
                    modifiedNotStaged.push_back(path);
                }
            }

            // If both staged (index != head) and also modified in work (work != index),
            // this path will be in both stagedModified (or stagedNew) and modifiedNotStaged, which is correct.
            continue;
        }

        // 5) Path in index and not in HEAD, but not handled earlier — already covered above.
        // 6) Path in work only was handled earlier (untracked).
    }

    // Print like git

    if (!stagedNew.empty() || !stagedModified.empty() || !stagedDeleted.empty()) {
        cout << "Changes to be committed:\n";
        for (auto &p : stagedNew)      cout << "    new file: " << p << "\n";
        for (auto &p : stagedModified) cout << "    modified: " << p << "\n";
        for (auto &p : stagedDeleted)  cout << "    deleted: " << p << "\n";
        cout << "\n";
    }

    if (!modifiedNotStaged.empty() || !deletedNotStaged.empty()) {
        cout << "Changes not staged for commit:\n";
        for (auto &p : modifiedNotStaged) cout << "    modified: " << p << "\n";
        for (auto &p : deletedNotStaged)  cout << "    deleted: " << p << "\n";
        cout << "\n";
    }

    if (!unmerged.empty()) {
        cout << "Unmerged paths:\n";
        for (auto &p : unmerged) cout << "    both modified: " << p << "\n";
        cout << "\n";
    }

    if (!untracked.empty()) {
        cout << "Untracked files:\n";
        for (auto &p : untracked) cout << "    " << p << "\n";
        cout << "\n";
    }

    if (stagedNew.empty() &&
            stagedModified.empty() &&
            stagedDeleted.empty() &&
            modifiedNotStaged.empty() &&
            deletedNotStaged.empty() &&
            unmerged.empty() &&
            untracked.empty()
        ) {
            cout << "nothing to commit, working tree clean\n";
        }
}
