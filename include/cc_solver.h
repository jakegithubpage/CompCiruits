#pragma once
#include <math.h>
#include <complex.h>

typedef struct CB_Ckt CB_Ckt;

#define resistorHoldMax 64
#define imagRHoldMax 64
#define nodeHoldMax 32
#define imagCHoldMax 32
#define sumHoldMax 12
#ifndef COMPLEX_H
#define COMPLEX_H


typedef struct 
{
    double real;
    double imag;
} cc_complex;

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
//constructors
cc_complex comp_create(double real, double imag);

//operators
cc_complex comp_add(cc_complex a, cc_complex b);
cc_complex comp_sub(cc_complex a, cc_complex b);
cc_complex comp_mul(cc_complex a, cc_complex b);
cc_complex comp_div(cc_complex a, cc_complex b);

//Utility
double comp_mag(cc_complex z);
double comp_phase(cc_complex z);
cc_complex comp_conj(cc_complex z);
#endif

void cc_calculator(const CB_Ckt *out, cc_solverUses *result);