// SPDX-License-Identifier: MIT
// Declarations for MergeHandler.

// MergeHandler.h
#ifndef MERGE_HANDLER_H
#define MERGE_HANDLER_H
#include <string>
class MergeHandler {
public:
    /**
     * @brief MergeHandler operation.
     */
    MergeHandler() = default;
    /**
     * @brief handleMerge operation.
     * @param otherBranch Parameter description.
     */
    void handleMerge(const std::string& otherBranch);
};
#endif
