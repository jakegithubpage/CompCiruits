#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef struct CB_Ckt CB_Ckt;
typedef struct {
    unsigned totalNodeCount;
    unsigned componentCount; 
    unsigned sourceCount;
    unsigned MatDimension; /*Var for number of unknowns in circuit Ex: (Easy)-> 2 nodes 1 source 2 componenets, thus meaning 
    (Nodes - 1 source) = 1 unknown AKA: SIZE OF LINEAR SYSTEM*/
    double *A;      //dim x dim = row major
    double *z;      //z length vector of dim
    double *x;      //x length vector of dim
} CC_DCMatrix;


/*Builds ckt from CB_Ckt Engine struct stores:
    componentCount
    sourceCount
    nodeCount
        Unkown Node Count = NodeCount - SourceCount!
*/

//Build of Matrix
int DC_Build_FromOut(const CB_Ckt *out);

//access last build matrix
const CC_DCMatrix* cc_dc_get_last(void);

//Free or reset last matrix built
void cc_dc_clear_last(void); 
//store matrix data from Cb_Ckt
int cc_dc_StoreStruct(const CB_Ckt *out, CC_DCMatrix *matrixDets);
