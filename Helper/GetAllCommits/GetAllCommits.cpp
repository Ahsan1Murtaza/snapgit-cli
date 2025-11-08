#include "GetAllCommits.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

static string readFile(const string &path) {
    ifstream f(path, ios::binary);
    if (!f.is_open()) return "";
    ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// Try object path resolution: xx/yyyy... or flat
static string getObjectPath(const string &hash) {
    if (hash.size() >= 3) {
        string p = ".mygit/objects/" + hash.substr(0,2) + "/" + hash.substr(2);
        if (fs::exists(p)) return p;
    }
    string flat = ".mygit/objects/" + hash;
    return flat;
}

vector<CommitInfo> getAllCommits(const string &headCommitHash) {
    vector<CommitInfo> commits;
    string current = headCommitHash;

    while (!current.empty()) {
        string commitPath = getObjectPath(current);
        if (!fs::exists(commitPath)) break;

        string content = readFile(commitPath);
        istringstream ss(content);
        string line;
        CommitInfo info;
        info.hash = current;

       
        while (getline(ss, line)) {
            if (line.rfind("tree ", 0) == 0) {
                info.tree = line.substr(5);
                // trim
                while (!info.tree.empty() && isspace((unsigned char)info.tree.back())) info.tree.pop_back();
            }
            else if (line.rfind("parent ", 0) == 0) {
                info.parent = line.substr(7);
                while (!info.parent.empty() && isspace((unsigned char)info.parent.back())) info.parent.pop_back();
            }
            else if (line.rfind("author ", 0) == 0) {
                info.author = line.substr(7);
                while (!info.author.empty() && isspace((unsigned char)info.author.back())) info.author.pop_back();
            }
            else if (line.rfind("email ", 0) == 0) {
                info.email = line.substr(6);
                while (!info.email.empty() && isspace((unsigned char)info.email.back())) info.email.pop_back();
            }
            else if (line.rfind("message ", 0) == 0) {
                info.message = line.substr(8);
            }
        }

        commits.push_back(info);
        if (info.parent.empty()) break;
        current = info.parent;
    }

    return commits;
}
