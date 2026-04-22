// SPDX-License-Identifier: MIT
// Implementation for RepoCheck.

#include "RepoCheck.h"
#include <filesystem>

/* Function to check if the repository is initialized
Returns true if the .mygit directory exists, false otherwise.
*/
/**
 * @brief Checks whether repo initialized.
 * @return True when the check succeeds; otherwise false.
 */
bool isRepoInitialized() {
    return std::filesystem::exists(".mygit") && std::filesystem::is_directory(".mygit");
}
