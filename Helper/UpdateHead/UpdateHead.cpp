#include "UpdateHead.h"
#include "../GetHeadRef/GetHeadRef.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

void updateHead(const string& newHash) {
    string refFile = getHeadRef();
    string path = ".mygit/" + refFile;
    cout << "Commit is being done to Branch ref " << path << endl;
    std::ofstream out(path, std::ios::trunc);

    out << newHash;
    out.close();
}