// SPDX-License-Identifier: MIT
// Declarations for CommitHandler.

#ifndef COMMIT_HANDLER_H
#define COMMIT_HANDLER_H

#include <string>

class CommitHandler {
public:
    /**
     * @brief CommitHandler operation.
     */
    CommitHandler();
    /**
     * @brief handleCommit operation.
     * @param message Parameter description.
     */
    void handleCommit(const std::string& message);
};

#endif // COMMIT_HANDLER_H
