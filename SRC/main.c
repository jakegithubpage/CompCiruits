//Libraries
#include <stdio.h>
#include <string.h>


//Headers
#include "main.h"
#include "cc_DcMatrix.h"
#include "CktEngine.h"

/*
volatile char GenreRate;
volatile char numRate;
volatile int diffRate;
*/



/*
EVENTUALLY: Add a ranking system into main work flow, Use prompt answers, store in variable,
Rank the circuit, then store and output difficulty calculated to user in work flow.
*/


static const char* diff_name(CB_difficulty d) {
   switch (d) {
      case CB_DIFF_Easy: return "Easy";
      case CB_DIFF_Medium: return "Medium";
      case CB_DIFF_Hard: return "Hard";
      case CB_DIFF_Extreme: return "Extreme";
      case CB_DIFF_Einstein: return "Einstein";
      default: return "Unknown";
   }
}

static const char* genre_name(CB_Genre g) { 
   switch (g) {
      case CB_G_dcSteady: return "DC Steady";
      case CB_G_AcSinusoidal: return "AC Sinusoidal";
      case CB_G_RL: return "RL - time Domain";
      case CB_G_RC: return "RC - time Domain";
      case CB_G_RLC: return "RLC - time Domain";
      case CB_G_OPAMP: return "Opamp DC - Solve for Voltage out";
      default: return "None";
   }
}

static const char* numtype_name(CB_numType n) {
   switch (n) {
      case CB_NT_Real: return "Real Numbers";
      case CB_NT_Complex: return "Complex Numbers";
      case CB_NT_RealComplex: return "Real/Complex Numbers";
      case CB_NT_FreqDomain: return "Frequency using laplace Style Components";
      default: return "Unknown";
   }
}

static void print_ckt_details(const CB_Ckt *ckt) {
   unsigned i; 
   printf("\n *CIRCUIT DETAILS* \n");
   printf("Nodes: %u\n", ckt->nodeCount);
   printf("Components: %u\n", ckt->componentCount);
   printf("Sources: %u\n", ckt->sourceCount);


   for (i = 0; i < ckt->componentCount; i++) {
      const CB_Component *c = &ckt->components[i];
      printf("Component [%u]: type=%d | n1=%u | n2=%u | value=%g\n",
             i, (int)c->type, c->n1, c->n2, c->value);
   }

   for (i = 0; i < ckt->sourceCount; i++) {
      const CB_Source *s = &ckt->sources[i];
      printf("Source [%u]: type=%d | net Plus=%u | net Minus=%u | value=%g\n",
             i, (int)s->type, s->nPlus, s->nMinus, s->value);
   }
}

static void print_DcMat_details(void) {
   const CC_DCMatrix *m = cc_dc_get_last();
   if (m == NULL) {
      printf("\n Matrix: Unavailable\n");
      return;
   }

   printf("\n *MATRIX DETAILS* \n");
   printf("total Nodes: %u\n", m->totalNodeCount);
   printf("Component Count: %u\n", m->componentCount);
   printf("Source Count: %u\n", m->sourceCount);
   printf("MatDimension: %u\n", m->MatDimension);
}


int main(void) {
   CB_buildOps opts;
   CB_Ckt ckt;
   int d, g, n;
   int ok;

   memset(&ckt, 0, sizeof(ckt));

   printf("Select Difficulty -> (0=Easy, 1=Medium, 2=Hard, 3=Extreme, 4=Einstein): ");
   if (scanf("%d", &d) != 1) return 1;

   printf("Select number type -> (0=Real, 1=Complex, 2=Real/Complex, FreqDomain): ");
   if (scanf("%d", &n) != 1) return 1;

   printf("Select Genre -> (0=DC, 1=AC, 2=RL, 3=RC, 4=RLC, 5=OPAMP): ");
   if (scanf("%d", &g) != 1) return 1;


   opts.difficulty = (CB_difficulty)d;
   opts.numType = (CB_numType)n;
   opts.genre = (CB_Genre)g;

   if (!CB_diffValid(opts.difficulty) ||
       !CB_GenreValid(opts.genre) ||
       !CB_numTypeValid(opts.numType)) {
         printf("Invalid input values. \n");
         return 1;
   }
   

   printf("\nBuilding Circuit with: difficulty = %s, genre = %s, numType = %s\n",
          diff_name(opts.difficulty), genre_name(opts.numType), numtype_name(opts.numType));

   ok = buildCkt(&opts, &ckt);
printf("[debug] buildCkt returned: %d\n", ok);

/* buildCkt uses error-code style: 0 = success */
if (ok != CB_EM_None) {
    printf("build function failure.\n");
    CB_printErrors(&ckt, stdout);
    return 1;
}

printf("[debug] calling print_ckt_details\n");
print_ckt_details(&ckt);

ok = DC_Build_FromOut(&ckt);
printf("[debug] DC_Build_FromOut returned: %d\n", ok);
if (!ok) {
    printf("DC matrix build failed.\n");
    return 1;
}

printf("[debug] calling print_DcMat_details\n");
print_DcMat_details();

return 0;
}

