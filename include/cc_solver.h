#pragma once


typedef struct CB_Ckt CB_Ckt;

#define resistorHoldMax 64
#define nodeHoldMax 32
#define sumHoldMax 12
typedef struct cc_solverUses{
    double cn[nodeHoldMax]; //Current Node holders A, B, C - Node values stored from 1st-last unknown
    double rh[resistorHoldMax];
    double summer[sumHoldMax];
} cc_solverUses;


void cc_calculator(const CB_Ckt *out, cc_solverUses *result);