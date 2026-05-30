/* Utilize methods of mesh or nodal analysis OR voltage/curr divider to
 pre solve circuits and cook solution and statistics for user when they finally finish solving and enter answer to compare*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cc_solver.h"
#include "CktEngine.h"


#define MAX_LOADER 32


void cc_calculator(const CB_Ckt *out, cc_solverUses *result) {
if (!out || !result) return;

memset(result, 0, sizeof(*result));

    if (out->nodeCount <= 0u) {
        return;
    }

    //One unknown node/Two total - Difficulty base is 0
    if (out->nodeCount == 1u) {
        for (unsigned i = 0; i < out->componentCount; i++) {
            result->rh[i] = (int)out->components[i].value;
        }
        for (unsigned i = 0; i < out->sourceCount; i++) {
            result->cn[i] = (int)out->sources[i].value;
        }
        for (unsigned i = 0; i < MAX_LOADER; i++) {
            if ((i == (MAX_LOADER - 1)) || (result->rh[i] == 0)) {
                break;
            }
            (double)result->summer[i] = (int)result->cn[i] * (result->rh[i] / ((double)result->rh[i] + (double)result->rh[i + 1]));
        }
    }
}


//Add solve questionare to main logic then impliment and test in main.c with build flow