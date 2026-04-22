// SPDX-License-Identifier: MIT
// Declarations for Commit.

#include "../../Objects/Object.h"
#include <string>
#include <vector>

#ifndef COMMIT_H
#define COMMIT_H

class Commit : public Object {
private:
    std::string treeHash;
    std::vector<std::string> parentHashes;
    std::string author;
    std::string email;
    std::string message;
    long long timestamp; // Unix timestamp
public:
    /**
     * @brief Constructs a Commit instance.
     * @param treeHash Hash of the tree object to read.
     * @param parentHashes Input value for `parentHashes`.
     * @param message Commit message to store with the new commit.
     * @param author Input value for `author`.
     * @param email Configured author email.
     * @param timestamp Commit creation time (Unix timestamp)
     */
    Commit(const string& treeHash, const std::vector<std::string>& parentHashes, const string& message, const string& author, const string& email, long long timestamp);

    long long getTimestamp() const { return timestamp; }
    void setTimestamp(long long ts) { timestamp = ts; }

    /**
     * @brief Performs save.
     */
    void save() override;
};

#endif // COMMIT_H
