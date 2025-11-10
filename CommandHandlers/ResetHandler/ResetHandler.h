#ifndef RESETHANDLER_H
#define RESETHANDLER_H

#include <string>
using namespace std;

class ResetHandler {
public:
    ResetHandler();
    void handleReset(const string& targetCommit, bool hard = false);
};

#endif