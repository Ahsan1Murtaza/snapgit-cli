#include "GetCurrentCommitHash.h"
#include "../GetHeadRef/GetHeadRef.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
This function returns currentCommitHash
if not present then return empty string
*/
string getCurrentCommitHash() {
    string currentBranchFile = getHeadRef();
    string path = ".mygit/" + currentBranchFile;

    ifstream in(path);

    if (!in.is_open()) {
        return "";
    }
    string hash;
    getline(in, hash);

    return hash;
}