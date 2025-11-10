#include "GetAllCommits.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <cctype>

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

// Trim trailing and leading whitespace
static string trim(const string &s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace((unsigned char)s[i])) ++i;
    while (j > i && isspace((unsigned char)s[j-i])) --j;

    return s.substr(i, j - i);
}

static bool parseCommitFile(const string &commitHash, CommitInfo &info) {
    string path = getObjectPath(commitHash);

    if (!fs::exists(path)) return false;

    string content = readFile(path);
    istringstream ss(content);
    string line;
    info.hash = commitHash;
    info.tree.clear();
    info.parents.clear();
    info.author.clear();
    info.email.clear();
    info.message.clear();

    while (getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.rfind("tree ", 0) == 0) {
            info.tree = trim(line.substr(5));
        }
        else if (line.rfind("parent ", 0) == 0) {
            string ph = trim(line.substr(7));
            if (!ph.empty()) {
                info.parents.push_back(ph);
            }
        }

        else if (line.rfind("author ", 0) == 0) {
            info.author = trim(line.substr(7));
        }
        else if (line.rfind("email ", 0) == 0) {
            info.email = trim(line.substr(6));
        }
        else if (line.rfind("message ", 0) == 0) {
            info.message = trim(line.substr(8));
        }

        // ignore unknown lines
    }

    return !info.tree.empty() || !info.parents.empty() || !info.message.empty() || !info.author.empty();
}


vector<CommitInfo> getAllCommits(const string &headCommitHash) {
    vector<CommitInfo> result;
    
    if (headCommitHash.empty()) return result;

    stack<string> st;
    unordered_set<string> visited;

    st.push(headCommitHash);

    while (!st.empty()) {
        string curr = st.top();
        st.pop();

        if (curr.empty() || visited.count(curr)) continue;

        visited.insert(curr);

        CommitInfo info;
        if (!parseCommitFile(curr, info)) {
            // commit file missing or unparsable
            continue;
        }

        result.push_back(info);


        // push parents onto stack so they are visited next.
        // We push them in the order they appear in the same file.
        // Using stack behaves depth first behaviour.

        for (const string &p : info.parents) {
            if (!p.empty() && !visited.count(p)) st.push(p);
        }
    }

    return result;
}
