// SPDX-License-Identifier: MIT
// Implementation for GetUserInfo.

#include "GetUserInfo.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>


using namespace std;

/**
 * @brief Returns user info from config.
 * @return Requested string value.
 */
pair<string, string> getUserInfoFromConfig() {
    ifstream configFile(".mygit/config");
    if (!configFile) {
        cerr << "Error: Config file not found. Run 'mygit init' first.\n";
        return {"", ""};
    }

    string line, name, email;
    bool inUserSection = false;

    while (getline(configFile, line)) {
        if (line.find("[user]") != string::npos) {
            inUserSection = true;
        } else if (inUserSection && line.find("name") != string::npos) {
            name = line.substr(line.find("=") + 1);
        } else if (inUserSection && line.find("email") != string::npos) {
            email = line.substr(line.find("=") + 1);
        }
    }

    // --- Universal trim (handles spaces, tabs, carriage returns, newlines) ---
    auto trim = [](string &s) {
        s.erase(remove_if(s.begin(), s.end(), [](unsigned char ch) {
            return ch == '\r' || ch == '\n' || isspace(ch);
        }), s.end());
    };

    trim(name);
    trim(email);

    // --- If only whitespace or empty, clear it completely ---
    if (name.find_first_not_of(" \t\r\n") == string::npos) name.clear();
    if (email.find_first_not_of(" \t\r\n") == string::npos) email.clear();

    return {name, email};
}
