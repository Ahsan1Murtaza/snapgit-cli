// SPDX-License-Identifier: MIT
// Declarations for InitHandler.

#ifndef INITHANDLER_H
#define INITHANDLER_H

#include <string>


class InitHandler {
public:
    /**
     * @brief InitHandler operation.
     */
    InitHandler();

    /**
     * @brief handleInit operation.
     */
    void handleInit();
  
private:
    /**
     * @brief createDir operation.
     * @param path Parameter description.
     * @return Return value description.
     */
    bool createDir(const std::string& path);
    /**
     * @brief dirExists operation.
     * @param path Parameter description.
     * @return Return value description.
     */
    bool dirExists(const std::string& path);

};


#endif // INITHANDLER_H
