#include "InfoHandler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
using namespace std;

void InfoHandler::handleInfo(const string& name, const string& email) {
    namespace fs = std::filesystem;
    string configPath = ".mygit/config";

    if (!fs::exists(configPath)) {
        cerr << "Error: Repository not initialized. Run 'mygit init' first.\n";
        return;
    }

    ofstream configFile(configPath);
    if (!configFile) {
        cerr << "Error: Could not open config file for writing.\n";
        return;
    }

    configFile << "[user]\n";
    configFile << "    name = " << name << "\n";
    configFile << "    email = " << email << "\n";
    configFile.close();

    cout << "User info saved successfully!\n";
    cout << "Name : " << name << "\n";
    cout << "Email: " << email << "\n";
}
