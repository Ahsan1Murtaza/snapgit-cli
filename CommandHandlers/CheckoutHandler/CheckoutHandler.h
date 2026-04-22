// SPDX-License-Identifier: MIT
// Declarations for CheckoutHandler.

#ifndef CHECKOUTHANDLER_H
#define CHECKOUTHANDLER_H

#include <string>


class CheckoutHandler {
public:
    /**
     * @brief Constructs a CheckoutHandler instance.
     */
    CheckoutHandler();
    /**
     * @brief Handles the  checkout command workflow.
     * @param refInput Branch name or commit hash to check out.
     */
    void handleCheckout(const std::string& refInput);
};

#endif
