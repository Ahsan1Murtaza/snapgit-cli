#include "CommitHandler.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/Hash/Hash.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void CommitHandler::handleCommit(const string& message) {
    if (!RepoCheck::isRepoInitialized()) {
        cout << "fatal: not a snapgit repository\n";
        return;
    }

    string treeHash = createTree();
    string parentHash = getCurrentCommitHash();

    // Generate commit content
    string commitContent;
    commitContent += "tree " + treeHash + "\n";
    if (!parentHash.empty()) commitContent += "parent " + parentHash + "\n";

    // Author info
    ifstream config(".mygit/config");
    string line, authorName = "unknown", authorEmail = "unknown";
    bool inUserSection = false;

    while (getline(config, line)) {
        // Remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        // Check for section headers
        if (line.front() == '[' && line.back() == ']') {
            inUserSection = (line == "[user]");
            continue;
        }

        if (!inUserSection) continue; // skip lines outside [user]

        size_t pos = line.find('=');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t\n\r"));
        key.erase(key.find_last_not_of(" \t\n\r") + 1);
        value.erase(0, value.find_first_not_of(" \t\n\r"));
        value.erase(value.find_last_not_of(" \t\n\r") + 1);

        if (key == "name") authorName = value;
        else if (key == "email") authorEmail = value;
    }
    config.close();





    commitContent += "author " + authorName + " <" + authorEmail + ">\n";
    commitContent += "message " + message + "\n";

    // Compute commit hash
    string commitHash = Hash::sha1FromString(commitContent);

    // Save commit object
    fs::path commitPath = ".mygit/commits/" + commitHash + ".commit";
    fs::create_directories(commitPath.parent_path());
    ofstream commitFile(commitPath);
    commitFile << commitContent;
    commitFile.close();

    cout << "Commit log saved at: " << commitPath << "\n";
    cout << "Committed with message: \"" << message << "\"\n";

    // Update branch ref (HEAD)
    ifstream headFile(".mygit/HEAD");
    string headLine;
    getline(headFile, headLine);
    headFile.close();

    if (headLine.rfind("ref: ", 0) == 0) {
        string branchRef = headLine.substr(5);
        ofstream branchFile(".mygit/" + branchRef);
        branchFile << commitHash << endl;
        branchFile.close();
    } else {
        // Detached HEAD
        ofstream headOut(".mygit/HEAD", ios::trunc);
        headOut << commitHash << endl;
        headOut.close();
    }
}

string CommitHandler::createTree() {
    // For simplicity, assume all staged files are included in tree
    return "dummy_tree_hash"; // placeholder
}
