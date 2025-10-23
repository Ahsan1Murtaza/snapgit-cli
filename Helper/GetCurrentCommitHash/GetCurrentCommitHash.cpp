#include "GetCurrentCommitHash.h"

#include <iostream>
#include <fstream>

using namespace std;

string getCurrentCommitHash() {
    ifstream in(".mygit/refs/heads/main");

    if (!in.is_open()) {
        return "";
    }
    string hash;
    getline(in, hash);

    return hash;
}