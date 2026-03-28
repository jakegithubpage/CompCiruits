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


