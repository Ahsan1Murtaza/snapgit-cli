// SPDX-License-Identifier: MIT
// Declarations for AddHandler.

#ifndef ADD_HANDLER_H
#define ADD_HANDLER_H

#include <string>
class AddHandler {
    public:
    /**
     * @brief Constructs a AddHandler instance.
     */
    AddHandler();

    /**
     * @brief Handles the  add command workflow.
     * @param filePath Path to the target file relative to the repository root.
     */
    void handleAdd(const std::string& filePath);
};

#endif // ADD_HANDLER_H
