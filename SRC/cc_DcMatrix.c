#include <stdlib.h>
#include <string.h>

#include "cc_DcMatrix.h"
#include "CktEngine.h"

static CC_DCMatrix latestDcMatrix = {0};

//Build and store structure to matrix header build
int cc_dc_StoreStruct(const CB_Ckt *out, CC_DCMatrix *matrixDets) {
    matrixDets->totalNodeCount = (out->nodeCount);
    matrixDets->componentCount = out->componentCount;
    matrixDets->sourceCount = out->sourceCount;
    matrixDets->MatDimension = (out->nodeCount - 1u) + out->sourceCount;
    return 1;
}

//Free mem for working allocation 
static void dc_free_latest(void) {
    free(latestDcMatrix.A);
    free(latestDcMatrix.z);
    free(latestDcMatrix.x);
    memset(&latestDcMatrix, 0, sizeof(latestDcMatrix));
}

static int node_to_voltage_index(unsigned nodeID) {
    return (nodeID == 0u) ? -1 : (int)(nodeID - 1u); //make sure ground is not unknown
}

//Allocate DC values and dimensions for the matrix, Check if NULL exists in entry details
static int dc_allocate(unsigned nodeCount, unsigned sourceCount) {
    size_t dim;
    size_t elems;

    if (nodeCount < 1u) return 0;

    dc_free_latest();

    dim = (size_t)((nodeCount - 1u) + sourceCount);

    if (dim == 0u) return 0;

    elems = dim * dim;
    //store to LatestMatrix 
    latestDcMatrix.totalNodeCount = nodeCount;
    latestDcMatrix.sourceCount = sourceCount;
    latestDcMatrix.MatDimension = (unsigned)dim;
    //finalkly calloc to all planes
    latestDcMatrix.A = (double *)calloc(elems, sizeof(double));
    latestDcMatrix.z = (double *)calloc(dim, sizeof(double));
    latestDcMatrix.x = (double *)calloc(dim, sizeof(double));

    if (!latestDcMatrix.A || !latestDcMatrix.z || !latestDcMatrix.x) {
        dc_free_latest();
        return 0;
    }

    return 1;
}

//build Plane A with set values to dimenions
static int add_to_A(int rowIndex, int columnIndex, double value) { 
    unsigned dim = latestDcMatrix.MatDimension;

    if (!latestDcMatrix.A) return 0;
    if (rowIndex < 0 || columnIndex < 0) return 1;
    if ((unsigned)rowIndex >= dim || (unsigned)columnIndex >= dim) return 0;

    latestDcMatrix.A[(size_t)rowIndex * dim + (size_t)columnIndex] += value;
    return 1;
}
//Repeat for z: the output vector, only needs 1D
static int add_to_z(int rowIndex, double value) {
    unsigned dim = latestDcMatrix.MatDimension;

    if (!latestDcMatrix.z) return 0;
    if (rowIndex < 0) return 1;
    if ((unsigned)rowIndex >= dim) return 0;

    latestDcMatrix.z[(size_t)rowIndex] += value;
    return 1;
}
/* Resistor values between node1 and node2 with R ohms.
   g = 1/R acting as a standard nodal stamp:
   +g on each each touched diagonal, -g on off diagonals that are linked to ground*/
static int stamp_Resistor(unsigned node1, unsigned node2, double resistanceOhms) {
    double g;
    int n1, n2;

    if (resistanceOhms <= 0.0) return 0;

    g = 1.0 / resistanceOhms;
    n1 = node_to_voltage_index(node1);
    n2 = node_to_voltage_index(node2);
    
    
    if (!add_to_A(n1, n1, +g)) return 0;
    if (!add_to_A(n2, n2, +g)) return 0;
    if (!add_to_A(n1, n2, -g)) return 0;
    if (!add_to_A(n2, n1, -g)) return 0;
    
    return 1;
}
/* Independent voltage Source (node Plus - Node Minus = volts), M.N.A stamping
   Allows one current unknown per source at index  (N - 1 + sourceLocalIndex).
*/
static int stamp_VoltageSource(unsigned nPlus, unsigned nMinus, double volts, unsigned sourceLocalIndex) {
    unsigned unknownVoltages;
    int vp, vm;
    int k; //Index for current row/Col sources uknown Current

    if (latestDcMatrix.totalNodeCount < 1u) return 0;

    unknownVoltages = latestDcMatrix.totalNodeCount - 1u;
    k = (int)(unknownVoltages + sourceLocalIndex);

    vp = node_to_voltage_index(nPlus);
    vm = node_to_voltage_index(nMinus);

    if (vp >= 0) {
        if (!add_to_A(vp, k, 1.0)) return 0;
        if (!add_to_A(k, vp, 1.0)) return 0;
    }

    if (vm >= 0) {
        if (!add_to_A(vm, k, -1.0)) return 0;
        if (!add_to_A(k, vm, -1.0)) return 0;
    }

    if(!add_to_z(k, volts)) return 0;
    return 1;
}

int DC_Build_FromOut(const CB_Ckt *out)
{

    
    
    //For loop use vars and counters
    unsigned sourceIndex;
    unsigned componentIndex;
    unsigned voltageSourceStampIndex = 0u;
    unsigned voltageSourceCount = 0u;
   
    
    //fail if null entrys arrive
    if (out == NULL) return 0;
    if (out->nodeCount < 1u) return 0;
   
    /* count voltage sources from out->sources*/
    for (sourceIndex = 0; sourceIndex < out->sourceCount; sourceIndex++) {
        if (out->sources[sourceIndex].type != CB_SRC_VoltageDC) return 0;
        if (out->sources[sourceIndex].value <= 0.0) return 0;
        //NPlus/Minus will never be greater than 1 as 1/0 represents their True or flase values.
        if (out->sources[sourceIndex].nPlus > 1) return 0; 
        if (out->sources[sourceIndex].nMinus > 1) return 0; 
        voltageSourceCount++;
    }
    
    //Return if bad allocation
    if (!dc_allocate(out->nodeCount, voltageSourceCount)) return 0;
    
    //Set latestDC specs for printing
    latestDcMatrix.totalNodeCount = out->nodeCount;
    latestDcMatrix.sourceCount = out->sourceCount;
    latestDcMatrix.componentCount = out->componentCount;

    for (componentIndex = 0u; componentIndex < out->componentCount; componentIndex++) {
        const CB_Component *component = &out->components[componentIndex];

        if (out->components[componentIndex].type != CB_COMP_Resistor) return 0;
        if (out->components[componentIndex].value <= 0.0) return 0;
        //NPlus/Minus will never be greater than 1 as 1/0 represents their True or flase values.
        if (out->components[componentIndex].n1 > 1) return 0;
        if (out->components[componentIndex].n2 > 1) return 0;
        //stamp resistors with conductance tracking
        
        if (!stamp_Resistor(out->components[componentIndex].n1, out->components[componentIndex].n2, out->components[componentIndex].value)) return 0;
    }
    
    for (sourceIndex = 0u; sourceIndex < out->sourceCount; sourceIndex++) {
        const CB_Source *source = &out->sources[sourceIndex];

        if (!stamp_VoltageSource(source->nPlus, source->nMinus, source->value, voltageSourceStampIndex)) return 0;
        voltageSourceStampIndex++;
    }

    return 1;
}

const CC_DCMatrix *cc_dc_get_last(void) {
    return &latestDcMatrix;
}

void cc_dc_clear_last(void) {
     dc_free_latest();
}