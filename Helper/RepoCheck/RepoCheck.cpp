#include "RepoCheck.h"
#include <filesystem>

/* Function to check if the repository is initialized
Returns true if the .mygit directory exists, false otherwise.
*/
bool isRepoInitialized() {
    return std::filesystem::exists(".mygit") && std::filesystem::is_directory(".mygit");
}