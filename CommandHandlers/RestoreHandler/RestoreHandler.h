// SPDX-License-Identifier: MIT
// Declarations for RestoreHandler.

#ifndef RESTORE_HANDLER_H
#define RESTORE_HANDLER_H

#include <string>

class RestoreHandler {
public:
    /**
     * @brief RestoreHandler operation.
     */
    RestoreHandler() {};
    /**
     * @brief handleRestore operation.
     * @param filePath Parameter description.
     */
    void handleRestore(const std::string& filePath);
};

#endif // RESTORE_HANDLER_H
