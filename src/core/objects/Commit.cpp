// SPDX-License-Identifier: MIT
// Implementation for Commit.

#include "Commit.h"
#include "../../cli/utils/Hash.h"

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

namespace fs = std::filesystem;


Commit::Commit(const string& treeHash, const vector<string>& parentHashes, const string& message, const string& author, const string& email, long long timestamp)
    : Object("commit"), treeHash(treeHash), parentHashes(parentHashes), message(message), author(author), email(email), timestamp(timestamp) {}

/**
 * @brief Performs save.
 */
void Commit::save() {
    ostringstream content;

    content << "tree " << treeHash << "\n";

    // Multiple Parent Support
    for (const auto& parent: parentHashes) {
        if (!parent.empty()) {
            content << "parent " << parent << "\n";
        }
    }

    content << "author " << author << "\n";
    content << "email " << email << "\n";
    content << "timestamp " << timestamp << "\n";
    content << "message " << message << "\n";

    string commitContent = content.str();

    /*  Commit Content Format
        tree <treehash>
        parent <parenthash>
        author <authorName>
        email <emailAddress>
        message <message>
    */
    hash = sha1(commitContent);


    string folder = ".snapgit/objects/" + hash.substr(0, 2);
    string file = folder + "/" + hash.substr(2);

    fs::create_directories(folder);

    ofstream out(file);
    out << commitContent;
    out.close();

}
