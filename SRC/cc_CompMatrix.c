//Source for Complex number matrix - Brother file to cc_DcMatrix
#include <complex.h>
#include <stdlib.h>
#include <string.h>

#include "cc_CompMatrix.h"
#include "CktEngine.h"

static cc_compNum latestSinMatrix = {0};

int cc_Sin_StoreStruct(const CB_Ckt *out, cc_compNum *SinMatDets) {
    SinMatDets->totalNodeCount = out->NodeCount;
    SinMatDets->componentCount = out->componentCount;
    SinMatDets->sourceCount = out->sourceCount;
    SinMatDets->CMatDim = out->nodeCount + out->sourceCount;
    return 1;
}

static void cc_Sin_clear_last(void) { 
    free(latestSinMatrix.A);
    free(latestSinMatrix.z);
    free(latestSinMAtrix.x);
    memset(&latestSinMatrix, 0 , sizeof(latestSinMAtrix));
}

static int node_to_voltage_index(unsigned nodeID) {
    return (nodeID == 0u) ? -1 : (int)(nodeID - 1u); //make sure ground is not unknown
}

static int SinAllocate(unsigned nodeCount, unsigned sourceCount) {
    size_t dim;
    size_t elems;

    if (nodeCount < 1) return 0;
    
    cc_Sin_clear_last();

    dim = (size_t)(nodeCount + sourceCount);

    if (dim == 0u) return 0;

    elems = dim * dim;

    //Store Latest Complex matrix details
    latestSinMatrix.totalNodeCount = nodeCount;
    latestSinMatrix.componentCount = sourceCount;
    latestSinMatrix.CMatDim = (unsigned)dim;

    latestSinMatrix.A = (double *)calloc(elems, sizeof(double));
    latestSinMatrix.z = (double *)calloc(dim, sizeof(double));
    latestSinMatrix.x = (double *)calloc(dim, sizeof(double));

    if (!(latestSinmatrix.A || latestSinMatrix.z || latestSinMatrix.x)) {
        cc_Sin_clear_last();
        return 0;
    }

    return 1;
}

//Finish rest of Dcmatrix modified funcs + new stamp functions catered toward complex number handling
