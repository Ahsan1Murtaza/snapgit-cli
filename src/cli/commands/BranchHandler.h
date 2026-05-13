// SPDX-License-Identifier: MIT
// Declarations for BranchHandler.

#ifndef BRANCH_HANDLER_H
#define BRANCH_HANDLER_H

#include <string>

class BranchHandler {
public:
    /**
     * @brief Handles the  branch command workflow.
     * @param branchName Branch name to create or inspect.
     */
    void handleBranch(const std::string &branchName = "");
};

#endif 
