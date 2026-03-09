#include "cc_DcMatrix.h"
#include "CktEngine.h"




static CC_DCMatrix latestDcMatrix = {0};

int cc_dc_StoreStruct(const CB_Ckt *out, CC_DCMatrix *matrixDets) {
    matrixDets->totalNodeCount = ((out->nodeCount) + 1u);
    matrixDets->componentCount = out->componentCount;
    matrixDets->sourceCount = out->sourceCount;
    matrixDets->MatDimension = (out->nodeCount - 1u) + out->sourceCount;
    return 1;
}

static void dc_free_latest(void) {
    return;
}

static int node_to_voltage_index(unsigned nodeID) {
    return (nodeID == 0u) ? -1 : (int)(nodeID - 1u); //make sure ground is not unknown
}


static int dc_allocate(unsigned nodeCount, unsigned sour) {
    return 0;
}

static int add_to_A(int rowIndex, int columnIndex, double value) { 
    return 0;
}

static int add_to_z(int rowIndex, double value) {
    return 0;
}

static int stamp_Resistor(unsigned node1, unsigned node2, double resistanceOhms) {
    return 0;
}

static int stamp_VoltageSource(unsigned nPlus, unsigned nMinus, double volts, unsigned sourceLocalIndex) {
    return 0;
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

    voltageSourceCount = 0u;
    voltageSourceStampIndex = 0u;

    /* count voltage sources from out->sources*/
    for (sourceIndex = 0; sourceIndex < out->sourceCount; sourceIndex++) {
        if (out->sources[sourceIndex].type != CB_SRC_VoltageDC) return 0;
        if (out->sources[sourceIndex].value <= 0.0) return 0;
        if (out->sources[sourceIndex].nPlus >= out->nodeCount) return 0;
        if (out->sources[sourceIndex].nMinus >= out->nodeCount) return 0;
        voltageSourceCount++;
    }
    //Return if bad allocation
    //if (!dc_allocate(out->nodeCount, voltageSourceCount)) return 0;

    for (componentIndex = 0u; componentIndex < out->componentCount; componentIndex++) {
        if (out->components[componentIndex].type != CB_COMP_Resistor) return 0;
        if (out->components[componentIndex].value <= 0.0) return 0;
        if (out->components[componentIndex].n1 >= out->nodeCount) return 0;
        if (out->components[componentIndex].n2 >= out->nodeCount) return 0;
        //stamp resistors with conductance tracking
        //if (!stamp_Resistor(component->n1, component->n2, component->value)) return 0;
    }

    for (sourceIndex = 0u; sourceIndex < out->sourceCount; sourceIndex++) {
        
        //if (!stamp_VoltageSource(source->nPlus, source->nMinus, source->value, voltageSourceStampIndex)) return 0;
        voltageSourceStampIndex++;
    }

    return 1;
}

const CC_DCMatrix *cc_dc_get_last(void) {
    return &latestDcMatrix;
}

void cc_dc_clear_last(void) {
     memset(&latestDcMatrix, 0, sizeof(latestDcMatrix));
}