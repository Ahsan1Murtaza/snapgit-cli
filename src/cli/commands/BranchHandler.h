// SPDX-License-Identifier: MIT
// Declarations for BranchHandler.

#ifndef BRANCH_HANDLER_H
#define BRANCH_HANDLER_H

class BranchHandler {
public:
    /**
     * @brief Handles the branch command (list, create, or delete).
     * @param argc Argument count from main (includes program name).
     * @param argv Argument vector from main.
     */
    void handleBranch(int argc, char* argv[]);
};

#endif
