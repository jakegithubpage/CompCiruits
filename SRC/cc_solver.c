/* Utilize methods of mesh or nodal analysis OR voltage/curr divider to
 pre solve circuits and cook solution and statistics for user when they finally finish solving and enter answer to compare*/




#include "CktEngine.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void cc_calculator(CB_Ckt *out, cc_solverUses *result) {
memset(result, 0, sizeof(*result));

    //One unknown node/Two total - Difficulty base is 0
    if (out->nodeCount == 1u) {
        for (unsigned i = 0; i < out->componentCount; i++) {
            result->rh[i] = out->components[i].value;
        }
        for (unsigned i; i < out->sourceCount; i++) {
            result->cn[i] = out->sources[i].value;
        }
        for (unsigned i; i < out->nodeCount; i++) {
            result->summer = result->cn[i] * (result->rh[i] / result->rh[i] + result->rh[i + 1]);
        }
    }
    if (out->nodeCount != 1u) {
        return 0;
    }
}


//Add solve questionare to main logic then impliment and test in main.c with build flow