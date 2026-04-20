#include "StatusHandler.h"
#include "../../Helper/Hash/Hash.h"
#include "../../Helper/ReadCommit/ReadCommit.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"
#include "../../Helper/ReadIndex/ReadIndex.h"
#include "../../Helper/ReadTree/ReadTree.h"


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


// -----------------------------------------------------
// READ WORKING DIRECTORY (ignore .mygit)
// -----------------------------------------------------
unordered_map<string, string> readWorkingDirectory() {
    unordered_map<string, string> result;

    for (auto& entry : fs::recursive_directory_iterator(".")) {
        if (!entry.is_regular_file()) continue;

        if (entry.path().string().find(".mygit") != string::npos) continue;

        ifstream file(entry.path(), ios::binary);
        if (!file) continue;

        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();

        string blobData = "blob " + to_string(content.size()) + "\0" + content;
        string hash = sha1(blobData);

        fs::path relPath = fs::relative(entry.path(), fs::current_path());
        string rel = relPath.generic_string();

        result[rel] = hash;
    }

    return result;
}





// -----------------------------------------------------
// STATUS HANDLER
// -----------------------------------------------------
void StatusHandler::handleStatus() {
    auto work = readWorkingDirectory();
    auto index = readIndex();

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

    unordered_set<string> allPaths;

    for (auto& [p, _] : headFiles)  allPaths.insert(p);
    for (auto& [p, _] : index)      allPaths.insert(p);
    for (auto& [p, _] : work)       allPaths.insert(p);


    for (auto& path : allPaths) {
        bool inHead  = headFiles.count(path);
        bool inIndex = index.count(path);
        bool inWork  = work.count(path);

        string h = inHead  ? headFiles[path] : "";
        string i = inIndex ? index[path]     : "";
        string w = inWork  ? work[path]      : "";

        // UNTRACKED
        if (!inHead && !inIndex && inWork) {
            untracked.push_back(path);
        }

        // STAGED DELETION
        else if (inHead && !inIndex) {
            stagedDeleted.push_back(path);
        }


        // MODIFIED STAGED
        else if (inHead && inIndex && i != h) {
            stagedModified.push_back(path);
        }

        // DELETED NOT STAGED
        else if (inIndex && inHead && !inWork && i == h) {
            deletedNotStaged.push_back(path);
        }

        // NEW FILE STAGED
        else if (!inHead && inIndex) {
            stagedNew.push_back(path);
            if (inWork && w != i) {
                modifiedNotStaged.push_back(path);
            }
        }

        // MODIFIED NOT STAGED
        else if (inIndex && inWork && i != w) {
            modifiedNotStaged.push_back(path);
        }

        // DELETED NOT STAGED
        else if (inIndex && !inWork) {
            // File staged (in INDEX) but removed from working directory
            deletedNotStaged.push_back(path);
        }

    }

       // DEBUGGING PURPOSE ONLY
//    // Print HEAD
//    cout << "=== HEAD Files ===\n";
//    for (auto& [path, hash] : headFiles) {
//        cout << path << " : " << hash << "\n";
//    }
//
//    // Print INDEX
//    cout << "=== INDEX Files ===\n";
//    for (auto& [path, hash] : index) {
//        cout << path << " : " << hash << "\n";
//    }
//
//    // Print WORKDIR
//    cout << "=== WORKDIR Files ===\n";
//    for (auto& [path, hash] : work) {
//        cout << path << " : " << hash << "\n";
//    }


    // -----------------------------------------------------
    // PRINT (Git style)
    // -----------------------------------------------------

    string branch = getHeadRef();
    if (!branch.empty()) {
        cout << "On branch " << branch.substr(branch.find_last_of('/') + 1) << "\n\n";
    } else {
        cout << "HEAD detached\n\n";
    }

    // Changes to be committed
    if (!stagedNew.empty() || !stagedModified.empty() || !stagedDeleted.empty()) {
        cout << "Changes to be committed:\n";
        cout << "  (use \"mygit restore <file>...\" to unstage)\n\n";
        for (auto &p : stagedNew)      cout << "    new file: " << p << "\n";
        for (auto &p : stagedModified) cout << "    modified: " << p << "\n";
        for (auto &p : stagedDeleted)  cout << "    deleted: " << p << "\n";
        cout << "\n\n";
    }

    // Changes not staged for commit
    if (!modifiedNotStaged.empty() || !deletedNotStaged.empty()) {
        cout << "Changes not staged for commit:\n";
        cout << "  (use \"mygit rm <file> or mygit rm --cached <file>\" to update what will be committed)\n";
        cout << "  (use \"mygit restore <file>...\" to discard changes in working directory)\n\n";
        for (auto &p : modifiedNotStaged) cout << "    modified: " << p << "\n";
        for (auto &p : deletedNotStaged)  cout << "    deleted: " << p << "\n";
        cout << "\n\n";
    }

    if (!untracked.empty()) {
        cout << "Untracked files:\n";
        cout << "  (use \"mygit add <file>...\" to include in what will be committed)\n\n";
        for (auto &p : untracked) cout << "    " << p << "\n";
        cout << "\n\n";
    }

    // Clean working tree message
    if (stagedNew.empty() && stagedModified.empty() && stagedDeleted.empty() &&
        modifiedNotStaged.empty() && deletedNotStaged.empty() && untracked.empty()) {
        cout << "nothing to commit, working tree clean\n\n";
    } else if (untracked.size() && stagedNew.empty() && stagedModified.empty() && stagedDeleted.empty()) {
        cout << "nothing added to commit but untracked files present (use \"git add\" to track)\n\n";
    }
}
