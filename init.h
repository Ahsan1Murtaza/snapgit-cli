#ifndef INIT_H
#define INIT_H

#include <iostream>
#include <string>
using namespace std;

class InitHandler {
public:
    InitHandler();

    void handleInit();
    void printHelp();
private:
    bool createDir(const string& path);
    bool dirExists(const string& path);

};


#endif // INIT_H