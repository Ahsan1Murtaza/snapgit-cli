// SPDX-License-Identifier: MIT
// Implementation for GetHeadRef.

#include "GetHeadRef.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;


/**
 * @brief getHeadRef operation.
 * @return Return value description.
 */
string getHeadRef() {
    ifstream headFile(".mygit/HEAD");

    if (!headFile.is_open()) {
        cerr << "Error: .mygit/HEAD file not found" << endl;
        return "";
    }

    string line;
    getline(headFile, line);
    headFile.close();

    // Format: ref: refs/heads/<branch>
    const string prefix = "ref: ";
    if (line.rfind(prefix, 0) == 0) {
        return line.substr(prefix.size()); // refs/heads/<branch>
    }

    return ""; // Detached HEAD- return empty to indicate direct hash
}
