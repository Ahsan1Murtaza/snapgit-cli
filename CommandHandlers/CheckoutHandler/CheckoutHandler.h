#ifndef CHECKOUTHANDLER_H
#define CHECKOUTHANDLER_H

#include <string>


class CheckoutHandler {
public:
    CheckoutHandler();
    void handleCheckout(const std::string& refInput);
};

#endif