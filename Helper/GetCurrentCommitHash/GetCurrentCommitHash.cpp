#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

string getCurrentCommitHash() {
    string headPath = ".mygit/HEAD";
    ifstream headFile(headPath);
    if (!headFile) return "";

    string headLine;
    getline(headFile, headLine);
    headFile.close();

    // Check if HEAD points to a branch
    if (headLine.rfind("ref: ", 0) == 0) {
        string branchPath = ".mygit/" + headLine.substr(5);
        ifstream branchFile(branchPath);
        if (!branchFile) return "";
        string commitHash;
        getline(branchFile, commitHash);
        branchFile.close();
        return commitHash;
    } else {
        // Detached HEAD (direct commit hash)
        return headLine;
    }
}

