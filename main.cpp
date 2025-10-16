#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <sys/stat.h>

using namespace std;

bool dirExists(const string& path) {
    struct stat info;
    return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

bool createDir(const string& path) {
    if (dirExists(path)) {
        return true;
    }
    return _mkdir(path.c_str()) == 0;
}

void printHelp() {
    cout << "SnapGit - A Simple git clone!\n\n";
    cout << "Available commands:\n";
    cout << "  init    Initialize a new repository\n";
    cout << "  help    Show this help message\n";
}

void handleInit() {
    if (dirExists(".git")) {
        cout << "Reinitialized existing Git repository in .git/\n";
        return;
    }

    if (!createDir(".git")) {
        cerr << "Error: Failed to create .git directory\n";
        return;
    }

    createDir(".git\\objects");
    createDir(".git\\refs");
    createDir(".git\\refs\\heads");
    createDir(".git\\info");
    createDir(".git\\hooks");

    ofstream headFile(".git\\HEAD");
    if (!headFile) {
        cerr << "Error: Failed to create HEAD file\n";
        return;
    }
    headFile << "ref: refs/heads/main\n";
    headFile.close();

    ofstream configFile(".git\\config");
    if (configFile) {
        configFile << "[core]\n";
        configFile << "\trepositoryformatversion = 0\n";
        configFile << "\tfilemode = false\n";
        configFile << "\tbare = false\n";
        configFile << "\tlogallrefupdates = true\n";
        configFile << "\tsymlinks = false\n";
        configFile << "\tignorecase = true\n";
        configFile.close();
    }

    ofstream descFile(".git\\description");
    if (descFile) {
        descFile << "Unnamed repository; edit this file 'description' to name the repository.\n";
        descFile.close();
    }

    ofstream excludeFile(".git\\info\\exclude");
    if (excludeFile) {
        excludeFile << "# git ls-files --others --exclude-from=.git/info/exclude\n";
        excludeFile << "# Lines that start with '#' are comments.\n";
        excludeFile << "# *.[oa]\n";
        excludeFile << "# *~\n";
        excludeFile.close();
    }

    cout << "Initialized empty Git repository in .git/\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }

    string command = argv[1];

    if (command == "init") {
        handleInit();
    } else if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
    } else {
        cout << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}
