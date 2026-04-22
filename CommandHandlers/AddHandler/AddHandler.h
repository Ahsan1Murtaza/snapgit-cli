// SPDX-License-Identifier: MIT
// Declarations for AddHandler.

#ifndef ADD_HANDLER_H
#define ADD_HANDLER_H

#include <string>
class AddHandler {
    public:
    /**
     * @brief AddHandler operation.
     */
    AddHandler();

    /**
     * @brief handleAdd operation.
     * @param filePath Parameter description.
     */
    void handleAdd(const std::string& filePath);
};

#endif // ADD_HANDLER_H
