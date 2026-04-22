// SPDX-License-Identifier: MIT
// Declarations for ResetHandler.

#ifndef RESETHANDLER_H
#define RESETHANDLER_H

#include <string>
using namespace std;

class ResetHandler {
public:
    /**
     * @brief ResetHandler operation.
     */
    ResetHandler();
    /**
     * @brief handleReset operation.
     * @param targetCommit Parameter description.
     * @param hard Parameter description.
     */
    void handleReset(const string& targetCommit, bool hard = false);
};

#endif
