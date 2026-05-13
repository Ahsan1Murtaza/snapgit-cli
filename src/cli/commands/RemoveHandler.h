// SPDX-License-Identifier: MIT
// Declarations for RemoveHandler.

#pragma once
#include <string>

class RemoveHandler {
public:
    // cached = true -> remove from index only
    /**
     * @brief Handles the  remove command workflow.
     * @param filePath Path to the target file relative to the repository root.
     * @param cached When true, remove only from index and keep working tree file.
     */
    void handleRemove(const std::string& filePath, bool cached = false);
};
