#include <iostream>
#include <string>

#include "init.h"

using namespace std;


int main(int argc, char* argv[]) {

    InitHandler initHandler; // Create an instance of InitHandler

    if (argc < 2) {
        initHandler.printHelp();
        return 0;
    }

    string command = argv[1]; // Get the command from command line arguments

    

    if (command == "init") {
        initHandler.handleInit();
    } 
    else if (command == "help" || command == "--help" || command == "-h") {
        initHandler.printHelp();
    } 
    else {
        cout << "Unknown command: " << command << "\n";
        initHandler.printHelp();
        return 1;
    }

    return 0;
}
