// SPDX-License-Identifier: MIT
// Declarations for RestoreHandler.

#ifndef RESTORE_HANDLER_H
#define RESTORE_HANDLER_H

#include <string>

class RestoreHandler {
public:
    RestoreHandler() {};
    /**
     * @brief Handles the  restore command workflow.
     * @param filePath Path to the target file relative to the repository root.
     */
    void handleRestore(const std::string& filePath);
};

#endif // RESTORE_HANDLER_H
