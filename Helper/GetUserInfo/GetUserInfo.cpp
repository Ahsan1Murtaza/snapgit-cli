#include "GetUserInfo.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

pair<string, string> getUserInfoFromConfig() {
    ifstream configFile(".mygit/config");
    if (!configFile) {
        cerr << "Error: Config file not found. Run 'snapgit init' first.\n";
        return {"", ""};
    }

    string line, name, email;
    bool inUserSection = false;

    while (getline(configFile, line)) {
        if (line.find("[user]") != string::npos)
            inUserSection = true;
        else if (inUserSection && line.find("name") != string::npos)
            name = line.substr(line.find("=") + 1);
        else if (inUserSection && line.find("email") != string::npos)
            email = line.substr(line.find("=") + 1);
    }

    // Trim leading spaces
    auto trim = [](string &s) {
        size_t pos = s.find_first_not_of(" \t");
        if (pos != string::npos) s = s.substr(pos);
    };
    trim(name);
    trim(email);

    return {name, email};
}
