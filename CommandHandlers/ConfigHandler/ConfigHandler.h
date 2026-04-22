// SPDX-License-Identifier: MIT
// Declarations for ConfigHandler.

#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <string>

class ConfigHandler {
public:
    /**
     * @brief Handles the  config command workflow.
     * @param name Configured author name.
     * @param email Configured author email.
     */
    void handleConfig(const std::string& name, const std::string& email);
};

#endif
