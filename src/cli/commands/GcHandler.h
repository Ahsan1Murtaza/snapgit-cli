// SPDX-License-Identifier: MIT
// Declarations for GcHandler.

#ifndef GC_HANDLER_H
#define GC_HANDLER_H

class GcHandler {
public:
    /**
     * @brief Removes unreachable objects from .snapgit/objects/.
     * @param dryRun When true, only prints what would be deleted.
     */
    void handleGc(bool dryRun = false);
};

#endif
