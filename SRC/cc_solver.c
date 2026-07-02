/* Utilize methods of mesh or nodal analysis OR voltage/curr divider to
 pre solve circuits and cook solution and statistics for user when they finally finish solving and enter answer to compare*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <complex.h>
#include <corecrt_math_defines.h>
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
                    if(out->sources[i].type == CB_SRC_VoltageDC) {
                        unsigned mark = 0;
                            for (unsigned j = 0; j < out->componentCount; j++) {
                                if (out->components[j].imag != 0.0) {
                                    result->corh[j] = out->components[j].imHold;
                                    mark++;
                                    break;
                                }
                            }
                            switch (mark){
                                case 0:
                                    double den = result->rh[i] + result->rh[i + 1];
                                    if (den == 0.0) break;
                                    result->summer[i] = result->cn[i] * (result->rh[i] / den);
                                break;
                                case 1:
                                    int realCount = 0;
                                    int imagCount = 0;
                                        for (unsigned f = 0; f < out->componentCount; f++) {
                                            if (out->components[f].imag == 0.0)
                                                result->realCount[realCount++] = f;
                                            else
                                                result->imagCount[imagCount++] = f;
                                }
                                if (realCount > 0 && imagCount > 0) {
                                    cc_complex a = cc_build(
                                        result->rh[result->realCount[0]],
                                        result->irh[result->realCount[0]]);
                                    cc_complex b = cc_build(
                                        result->rh[result->imagCount[0]],
                                        result->irh[result->imagCount[0]]);
                                    cc_complex den = cc_add(a, b);
                                    if (cc_abs(den) < 1e-12) break;
                                    cc_complex source = cc_build(out->sources[0].value, 0.0);
                                    cc_complex prod = cc_mul(cc_div(a, den), source);
                                    result->cosummer[0] = prod;
                                    
                                break;
                                case 2:
                                    cc_complex a = cc_build(result->rh[i],     result->irh[i]);
                                cc_complex b = cc_build(result->rh[i + 1], result->irh[i + 1]);
                                cc_complex den = cc_add(a, b);
                                if (cc_abs(den) < 1e-12) break;
                                cc_complex source = cc_build(out->sources[0].value, 0.0);
                                result->cosummer[0] = cc_mul(cc_div(b, den), source);

                                    
                                break;
                            }
                        }

                    
                    else if(out->sources[i].type == CB_SRC_CurrentDC) {
                        
                            
                            }
                    }

                        
                    
                    //Start Development on Complex handling with Mixed AcDc genre
                break;
                case CB_G_AcSinusoidal:

                break;
                 
                }
            }
            }
        
    
    else if (out->nodeCount == 2u) { 
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
                if (result->rh[i] == 0) break;
                        
                if(out->sources[i].type == CB_SRC_VoltageDC) { 
                    double den = ((1 / result->rh[2]) + (1 / (result->rh[0] + result->rh[1])) + (1 / result->rh[3]));
                    if (den == 0.0) break;
                    result->summer[0] = (result->cn[0] / result->rh[3]) / den;
                    double den2 = result->rh[0] + result->rh[1];
                    if (den2 == 0.0) break;
                    result->summer[1] = result->summer[0] * (result->rh[0] / den2);
                }
                else if (out->sources[i].type == CB_SRC_CurrentDC) {
                    double den = (result->rh[3] + result->rh[2] + result->rh[1]);
                    if (den == 0.0) break;
                    result->summer[0] = ((result->rh[1] * result->rh[3]) / den) * result->cn[0];
                    result->summer[1] = ((result->rh[1] * (result->rh[2] + result->rh[3])) / den) * result->cn[0];


                }
            break;
            case CB_G_AcDcSinusoidal:

            break;
            case CB_G_AcSinusoidal:

            break;
        }
    }
                
    }
    else if (out->nodeCount == 3u) {
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
            


            switch(out->opts.genre) {
                case CB_G_dcSteady:
                        if (((out->sources[0].type == CB_SRC_VoltageDC) && (out->sources[1].type == CB_SRC_VoltageDC))) {
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
                        if (((out->sources[0].type == CB_SRC_CurrentDC)) && ((out->sources[1].type == CB_SRC_CurrentDC))) {

                            double D, V2, V4, V5;
                            

                            D = result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5];

                            V2 = result->rh[1] * (result->cn[0] + result->cn[1]);

                            V4 = (
                                result->cn[0]*result->rh[1]*D
                                + result->cn[1]*result->rh[1]*D
                                + result->cn[1]*result->rh[2]*result->rh[5]
                            ) / D;

                            V5 = (
                                result->cn[0]*result->rh[1]*D
                                + result->cn[1]*result->rh[1]*D
                                + result->cn[1]*result->rh[2]*
                                    (result->rh[3] + result->rh[4] + result->rh[5])
                            ) / D;
                            result->summer[0] = V2;
                            result->summer[1] = V4;
                            result->summer[2] = V5;
                        }
                        if (((out->sources[0].type == CB_SRC_CurrentDC) && (out->sources[1].type == CB_SRC_VoltageDC))) {
                            double D, V2, V4, V5;
                                D =
                                    result->rh[1]*result->rh[2]
                                    + result->rh[1]*result->rh[3]
                                    + result->rh[1]*result->rh[4]
                                    + result->rh[1]*result->rh[5]
                                    + result->rh[2]*result->rh[3]
                                    + result->rh[2]*result->rh[4]
                                    + result->rh[2]*result->rh[5];

                                V2 = (
                                    result->rh[1] * (
                                        result->cn[0]*result->rh[2]*
                                            (result->rh[3] + result->rh[4] + result->rh[5])
                                        + result->cn[1]*
                                            (result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5])
                                    )
                                ) / D;

                                V4 = (
                                    result->cn[0]*result->rh[1]*result->rh[2]*
                                        (result->rh[3] + result->rh[4])

                                    + result->cn[1]*(
                                        result->rh[1]*result->rh[2]
                                        + result->rh[1]*result->rh[3]
                                        + result->rh[1]*result->rh[4]
                                        + result->rh[1]*result->rh[5]
                                        + result->rh[2]*result->rh[5]
                                    )
                                ) / D;

                                V5 = result->cn[1];

                                
                                result->summer[0] = V2;
                                result->summer[1] = V4;
                                result->summer[2] = V5;


                        }
                        if (((out->sources[0].type == CB_SRC_VoltageDC) && (out->sources[1].type == CB_SRC_CurrentDC))) {
                            
                            double D;
                            D = (result->rh[0] + result->rh[1]) * (result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5]);
                            
                            result->summer[0] =
                                (result->rh[1] * (result->cn[1] * result->rh[0] + result->cn[0])) / (result->rh[0] + result->rh[1]);

                            result->summer[1] = (
                                    result->cn[1]*result->rh[0]*result->rh[1]*
                                        (result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5])

                                    + result->cn[1]*result->rh[2]*result->rh[5]*
                                        (result->rh[0] + result->rh[1])

                                    + result->rh[1]*result->cn[0]*
                                        (result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5])
                                ) / D;
                            result->summer[2] =  (
                                            result->cn[1]*result->rh[0]*result->rh[1]*(result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5])
                                        + result->cn[1]*result->rh[2]*(result->rh[3] + result->rh[4] + result->rh[5])*(result->rh[0] + result->rh[1])
                                        + result->rh[1]*result->cn[0]*(result->rh[2] + result->rh[3] + result->rh[4] + result->rh[5])
                                        ) / D;
                        }
                break;
                case CB_G_AcDcSinusoidal:

                break;
                case CB_G_AcSinusoidal:

                break;  
            }
        }
            
    }
    


//**NEXT ADD HARD DIFICULTY SOLVE LOGIC**
//Add solve questionare to main logic then impliment and test in main.c with build flow