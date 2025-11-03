#include "LogHandler.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

LogHandler::LogHandler() {}

void LogHandler::handleLog() {
    string commitsDir = ".mygit/commits";

    if (!fs::exists(commitsDir)) {
        cout << "No commits found.\n";
        return;
    }

    cout << "Commit history:\n\n";

    for (const auto &entry : fs::directory_iterator(commitsDir)) {
        if (entry.path().extension() == ".txt" || entry.path().extension() == ".commit") {

            cout << "Opening file: " << entry.path() << endl;
            ifstream file(entry.path());

            if (!file.is_open()) {
                cerr << "Failed to open file: " << entry.path() << endl;
                continue;
            }

            string line;
            cout << "Commit file: " << entry.path().filename().string() << "\n";

            while (std::getline(file, line)) {
                cout << "    " << line << "\n";
            }
            cout << "\n";
            file.close();
        }
    }
}

