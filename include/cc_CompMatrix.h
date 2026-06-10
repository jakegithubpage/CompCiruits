//Header for complex number matrix assembler - Add after full difficulty selection is complete
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef struct CB_Ckt CB_Ckt;
typedef struct {
    unsigned totalNodeCount;
    unsigned componentCount;
    unsigned sourceCount;
    unsigned CMatDim;
    /*
    Repeat var's mimic DcMatrix header struct
    However CMatDim will be used for the dimension
     of complex num matrix
     */
    double *A;  //Row major = z * x
    double *z;  // z Length - Row of Matrix
    double *x;  // X length - Column of Matrix
} cc_compNum;

int Sin_Build_FromOut(const CB_Ckt *out);

const compNum* cc_Sin_get_last(void);

void cc_Sin_clear_last(void);

int cc_Sin_StoreStruct(const CB_Ckt *out, cc_compNum *SinMatDets);
/*
Whole Header essentially mimics DcMatrix .h apart from the changed varible 
Names to indicate complex number usage
*/