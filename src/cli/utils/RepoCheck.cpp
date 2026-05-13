// SPDX-License-Identifier: MIT
// Implementation for RepoCheck.

#include "RepoCheck.h"

#include <filesystem>

/* Function to check if the repository is initialized
Returns true if the .snapgit directory exists, false otherwise.
*/
/**
 * @brief Checks whether repo initialized.
 * @return True when the check succeeds; otherwise false.
 */
bool isRepoInitialized() {
    return std::filesystem::exists(".snapgit") && std::filesystem::is_directory(".snapgit");
}
