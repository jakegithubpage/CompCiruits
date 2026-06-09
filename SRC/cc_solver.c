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

    unsigned rCount = (out->componentCount < resistorHoldMax) ? out->componentCount : resistorHoldMax;
    unsigned sCount = (out->sourceCount < nodeHoldMax) ? out->sourceCount : nodeHoldMax;

    //One unknown node/Two total - Difficulty base is 0
    if (out->nodeCount == 1u) {
        for (unsigned i = 0; i < rCount; i++) {
            result->rh[i] = out->components[i].value;
        }
        for (unsigned i = 0; i < sCount; i++) {
            result->cn[i] = out->sources[i].value;
        }
        for (unsigned i = 0; i < sumHoldMax && i + 1 < rCount && i < sCount; i++) {
            if (result->rh[i] == 0) {
                break;
            }
            double den = result->rh[i] + result->rh[i + 1];
            if (den == 0.0) break;
            result->summer[i] = result->cn[i] * (result->rh[i] / den);
        }
    }
        if (out->nodeCount == 2u) { 
            for (unsigned i = 0; i < rCount; i++) {
            result->rh[i] = out->components[i].value;
            }
            for (unsigned i = 0; i < sCount; i++) {
            result->cn[i] = (int)out->sources[i].value;
            }
                if (rCount >= 4u && sCount >= 1u) { 
                    double den = ((1 / result->rh[2]) + (1 / (result->rh[0] + result->rh[1])) + (1 / result->rh[3]));
        
                    if (den != 0.0) { 
                        result->summer[0] = (result->cn[0] / result->rh[3]) / den;
                        double den2 = result->rh[0] + result->rh[1];

                        if (den2 != 0.0) {
                                result->summer[1] = result->summer[0] * (result->rh[0] / den2);
                        }
                    }
                }
        }
}

//**NEXT ADD HARD DIFICULTY SOLVE LOGIC**
//Add solve questionare to main logic then impliment and test in main.c with build flow