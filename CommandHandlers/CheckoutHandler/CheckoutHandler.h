// SPDX-License-Identifier: MIT
// Declarations for CheckoutHandler.

#ifndef CHECKOUTHANDLER_H
#define CHECKOUTHANDLER_H

#include <string>


class CheckoutHandler {
public:
    /**
     * @brief CheckoutHandler operation.
     */
    CheckoutHandler();
    /**
     * @brief handleCheckout operation.
     * @param refInput Parameter description.
     */
    void handleCheckout(const std::string& refInput);
};

#endif
