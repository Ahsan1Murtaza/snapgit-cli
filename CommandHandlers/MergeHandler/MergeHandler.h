// SPDX-License-Identifier: MIT
// Declarations for MergeHandler.

// MergeHandler.h
#ifndef MERGE_HANDLER_H
#define MERGE_HANDLER_H
#include <string>
class MergeHandler {
public:
    MergeHandler() = default;
    /**
     * @brief Handles the  merge command workflow.
     * @param otherBranch Name of the branch to merge into the current branch.
     */
    void handleMerge(const std::string& otherBranch);
};
#endif
