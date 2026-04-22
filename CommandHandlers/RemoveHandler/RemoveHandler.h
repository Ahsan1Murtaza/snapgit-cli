// SPDX-License-Identifier: MIT
// Declarations for RemoveHandler.

#pragma once
#include <string>

class RemoveHandler {
public:
    // cached = true -> remove from index only
    /**
     * @brief handleRemove operation.
     * @param filePath Parameter description.
     * @param cached Parameter description.
     */
    void handleRemove(const std::string& filePath, bool cached = false);
};
