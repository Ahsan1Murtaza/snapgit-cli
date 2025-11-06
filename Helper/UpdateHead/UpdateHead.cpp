#include "UpdateHead.h"
#include "../GetHeadRef/GetHeadRef.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

void updateHead(const string& newHash) {

    string refFile = getHeadRef();

    if (!refFile.empty()) {
        // HEAD is attached to a branch -> Update that branch's file
        string path = ".mygit/" + refFile;
        std::ofstream out(path, std::ios::trunc);
        if (!out.is_open()) {
            cerr << "Error : Could not open " << path << " for writing" << endl;
            return;
        }
        out << newHash;
        out.close();
    }
    else {
        // Detached HEAD -> Write commit hash directly into .mygit/HEAD
        ofstream out(".mygit/HEAD", ios::trunc);
        if (!out.is_open()) {
            cerr << "Error : Could not open .mygit/HEAD for writing" << endl;
            return;
        }
        out << newHash;
        out.close();
    }
}