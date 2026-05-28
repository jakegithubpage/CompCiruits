/* Utilize methods of mesh or nodal analysis OR voltage/curr divider to
 pre solve circuits and cook solution and statistics for user when they finally finish solving and enter answer to compare*/




#include "CktEngine.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static void cc_calculator(CB_Ckt *out) {
unsigned summer;
unsigned UnNode;
    //One unknown node/Two total - Difficulty base is 0
    if (out->nodeCount == 1u) {
        for (unsigned i = 0; out->componentCount; ++i) {

        }
    }
}
