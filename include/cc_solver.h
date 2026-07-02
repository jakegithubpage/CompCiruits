#pragma once
#include <math.h>
#include <complex.h>
#include "cc_complex.h"
typedef struct CB_Ckt CB_Ckt;

#define resistorHoldMax 64
#define imagRHoldMax 64
#define nodeHoldMax 32
#define imagCHoldMax 32
#define sumHoldMax 12
typedef struct cc_solverUses{
    double cn[nodeHoldMax]; //Current Node holders A, B, C - Node values stored from 1st-last unknown
    double ich[imagCHoldMax];
    double rh[resistorHoldMax];
    double irh[imagRHoldMax];
    double summer[sumHoldMax];
    int realCount[nodeHoldMax];
    int imagCount[nodeHoldMax];
    _Dcomplex cocn[nodeHoldMax];
    _Dcomplex corh[resistorHoldMax];
    cc_complex cosummer[sumHoldMax];
} cc_solverUses;


void cc_calculator(const CB_Ckt *out, cc_solverUses *result);