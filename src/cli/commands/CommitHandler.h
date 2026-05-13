// SPDX-License-Identifier: MIT
// Declarations for CommitHandler.

#ifndef COMMIT_HANDLER_H
#define COMMIT_HANDLER_H

#include <string>

class CommitHandler {
public:
    /**
     * @brief Constructs a CommitHandler instance.
     */
    CommitHandler();
    /**
     * @brief Handles the  commit command workflow.
     * @param message Commit message to store with the new commit.
     */
    void handleCommit(const std::string& message);
};

#endif // COMMIT_HANDLER_H
