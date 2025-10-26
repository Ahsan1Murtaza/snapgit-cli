#include "ConfigHandler.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
using namespace std;

void ConfigHandler::handleConfig(const string& name, const string& email) {
    namespace fs = std::filesystem;
    

    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'mygit init' first.\n";
        return;
    }
    
    string configPath = ".mygit/config";
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
