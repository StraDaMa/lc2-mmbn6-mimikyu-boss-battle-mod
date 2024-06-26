#include "pch.h"
#include "GbaState.h"

std::array<uint32_t, 5> storedRegisters;

// Functions that are directly called from existing code
// Expect these registers to keep their values
// Only using these for functions being called by existing LC code
void GbaStateStoreAllEssential(GbaState* g)
{
    storedRegisters[0] = g->r4;
    storedRegisters[1] = g->r5;
    storedRegisters[2] = g->r6;
    storedRegisters[3] = g->r7;
    storedRegisters[4] = g->r14;
}

void GbaStateRestoreAllEssential(GbaState* g)
{
    g->r4 = storedRegisters[0];
    g->r5 = storedRegisters[1];
    g->r6 = storedRegisters[2];
    g->r7 = storedRegisters[3];
    g->r15 = storedRegisters[4];
}