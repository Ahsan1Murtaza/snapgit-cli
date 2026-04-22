// SPDX-License-Identifier: MIT
// Declarations for InitHandler.

#ifndef INITHANDLER_H
#define INITHANDLER_H

#include <string>


class InitHandler {
public:
    /**
     * @brief Constructs a InitHandler instance.
     */
    InitHandler();

    /**
     * @brief Handles the  init command workflow.
     */
    void handleInit();
  
private:
    /**
     * @brief Creates dir.
     * @param path Filesystem path to process.
     * @return True when the check succeeds; otherwise false.
     */
    bool createDir(const std::string& path);
    /**
     * @brief Performs dir exists.
     * @param path Filesystem path to process.
     * @return True when the check succeeds; otherwise false.
     */
    bool dirExists(const std::string& path);

};


#endif // INITHANDLER_H
