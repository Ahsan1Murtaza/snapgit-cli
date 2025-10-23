#include "Commit.h"

#include "../../Helper/Hash/Hash.h"

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

namespace fs = std::filesystem;

Commit::Commit(const string& treeHash, const string& parentHash, const string& message, const string& author) : Object("commit"), treeHash(treeHash), parentHash(parentHash), message(message), author(author) {}

void Commit::save() {
    ostringstream content;

    content << "tree " << treeHash << "\n";

    if (!parentHash.empty()) {
        content << "parent " << parentHash << "\n";
    }

    content << "author " << author << "\n";
    content << "message " << message << "\n";

    string commitContent = content.str();

    hash = sha1(commitContent);


    string folder = ".mygit/objects/" + hash.substr(0, 2);
    string file = folder + "/" + hash.substr(2);

    fs::create_directories(folder);

    ofstream out(file);
    out << commitContent;
    out.close();

}
