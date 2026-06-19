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
            switch(out->opts.genre) {
                case CB_G_dcSteady:
                    result->rh[i] = out->components[i].value;
                    result->irh[i] = out->components[i].imag;
                break;
                case CB_G_AcDcSinusoidal:
                    result->rh[i] = out->components[i].value;
                    result->irh[i] = out->components[i].imag;
                break;
                case CB_G_AcSinusoidal:
                    result->rh[i] = out->components[i].value;
                    result->irh[i] = out->components[i].imag;
                break;
            }
            
        }
        for (unsigned i = 0; i < sCount; i++) {
            switch(out->opts.genre) {
                case CB_G_dcSteady:
                    result->cn[i] = out->sources[i].value;
                    result->ich[i] = out->sources[i].imag;
                break;
                case CB_G_AcDcSinusoidal:
                    result->cn[i] = out->sources[i].value;
                    result->ich[i] = out->sources[i].imag;
                break;
                case CB_G_AcSinusoidal:
                    result->cn[i] = out->sources[i].value;
                    result->ich[i] = out->sources[i].imag;
                break;
            }
        }
        for (unsigned i = 0;((i < sumHoldMax) && (i + 1 < rCount && i < sCount)); i++) {
            switch(out->opts.genre) {
                case CB_G_dcSteady:
                    if (out->sources[i].type == CB_SRC_VoltageDC) {
                        if (result->rh[i] == 0) {
                            break;
                        }
                        double den = result->rh[i] + result->rh[i + 1];
                        if (den == 0.0) break;
                            result->summer[i] = result->cn[i] * (result->rh[i] / den);
                        }
                    else if (out->sources[i].type == CB_SRC_CurrentDC) {
                        if (result->rh[i] == 0) {
                            break;
                        }
                        double den = result->rh[i + 1];
                        if (den == 0.0) break;
                            result->summer[i] = den * result->cn[i];
                    }
                break;
                case CB_G_AcDcSinusoidal:
                    
                break;
                case CB_G_AcSinusoidal:

                break;
                    
                
            
            }
        }
    }
    else if (out->nodeCount == 2u) { 
            for (unsigned i = 0; i < rCount; i++) {
            result->rh[i] = out->components[i].value;
            }
            for (unsigned i = 0; i < sCount; i++) {
            result->cn[i] = (int)out->sources[i].value;
            }
                /*
                Coding in if statement that depends on C and S count, 
                This will be helpful when difficulty component count can range
                */
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
    else if (out->nodeCount == 3u) {
        for (unsigned i = 0; i < rCount; i++) {
            result->rh[i] = out->components[i].value;
        }
        for (unsigned i = 0; i < sCount; i++) {
            result->cn[i] = out->sources[i].value;
        }
            if (rCount >= 6 && sCount >= 2) {
                double v2num = (result->cn[0] / result->rh[0]) + (result->cn[1] * ((1 / result->rh[2]) + (1 / (result->rh[5] + result->rh[4] + result->rh[3]))));
                double v2den = ((1 / result->rh[0]) + (1 / result->rh[1]) + (1 / result->rh[2]) + (1 / (result->rh[5] + result->rh[4] + result->rh[3])));
                double v2 = v2num / v2den;

                if (v2 != 0.0) {
                    result->summer[0] = v2num / v2den;
                    double v4num = ((v2 / result->rh[5]) + (result->cn[1] / (result->rh[4] + result->rh[3])));
                    double v4den = ((1 / result->rh[5]) + (1 / (result->rh[4] + result->rh[3])));
                    double v4 = v4num / v4den;

                    if (v4 != 0.0) {
                        result->summer[1] = v4num / v4den;
                        double v5num = (v4 / result->rh[4]) + (result->cn[1] / result->rh[3]);
                        double v5den = ((1 / result->rh[4]) + (1 / result->rh[3]));
                        double v5 = v5num / v5den;

                        if (v5 != 0.0) { 
                            result->summer[2] = v5num / v5den;
                        }
                    }
                }
            }
    }
    else {
        return;
    }

}

//**NEXT ADD HARD DIFICULTY SOLVE LOGIC**
//Add solve questionare to main logic then impliment and test in main.c with build flow