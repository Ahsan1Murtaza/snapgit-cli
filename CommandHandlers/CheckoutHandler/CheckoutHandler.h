#ifndef CHECKOUTHANDLER_H
#define CHECKOUTHANDLER_H

#include <string>
using namespace std;

class CheckoutHandler {
public:
    CheckoutHandler();
    void handleCheckout(const string& branchName, bool createBranch = false);
};

#endif