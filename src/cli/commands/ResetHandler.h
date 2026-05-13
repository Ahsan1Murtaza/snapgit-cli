// SPDX-License-Identifier: MIT
// Declarations for ResetHandler.

#ifndef RESETHANDLER_H
#define RESETHANDLER_H

#include <string>
using namespace std;

class ResetHandler {
public:
    /**
     * @brief Constructs a ResetHandler instance.
     */
    ResetHandler();
    /**
     * @brief Handles the  reset command workflow.
     * @param targetCommit Commit hash to move to.
     * @param hard When true, also update the working tree to match target commit.
     */
    void handleReset(const string& targetCommit, bool hard = false);
};

#endif
