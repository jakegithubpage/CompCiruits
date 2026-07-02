
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "CktEngine.h"



static const char *error_to_text(int err) {
   //Switch statement for readability
   switch(err) {
      case CB_EM_BuildFailed:         return "Build Failed";
      case CB_EM_InvalidGenre:        return "Invalid Genre";
      case CB_EM_InvalidNumType:      return "Invalid Number Type";
      case CB_EM_InvalidDifficulty:   return "Invalid Difficulty";
      default:                        return "Unknown Error";
   }
}
//Function to mask error num based of input from CktEngine
static CB_errorMask error_to_mask(int err) {
   //Switch statement for readability
   switch(err) {
      case CB_EM_BuildFailed:         return CB_EM_BuildFailed;
      case CB_EM_InvalidGenre:        return CB_EM_InvalidGenre;
      case CB_EM_InvalidNumType:      return CB_EM_InvalidNumType;
      case CB_EM_InvalidDifficulty:   return CB_EM_InvalidDifficulty;
      default:                        return CB_EM_None;
   }
}

static void ratio_for_difficulty(CB_difficulty d, unsigned *sourceCount, unsigned *componentCount) {
   
   switch(d) {
      case CB_DIFF_Easy:      *sourceCount = 1u; *componentCount = 2u;  break;
      case CB_DIFF_Medium:    *sourceCount = 1u; *componentCount = 4u;  break;
      case CB_DIFF_Hard:      *sourceCount = 2u; *componentCount = 6u;  break;
      case CB_DIFF_Extreme:   *sourceCount = 2u; *componentCount = 8u;  break;
      case CB_DIFF_Einstein:  *sourceCount = 3u; *componentCount = 10u; break;
      default:                *sourceCount = 0u; *componentCount = 0u;  break;
   }
   
}

void CB_printErrors(const CB_Ckt *ckt, FILE *stream) {
   int i;
   int stored;
   FILE *out;
   //Check for null Circuit build 
   if (ckt == NULL) return;
   out = (stream != NULL) ? stream : stderr;
   //No errors
   if (ckt->errorCount <= 0) {
      fprintf(out, "No errors.\n");
      return;
   }

   //Main print formula
   stored = ckt->errorCount;
   if (stored > CB_MAX_ERROR_STACK) stored = CB_MAX_ERROR_STACK;

   fprintf(out, "Build failed with %d errors:\n", ckt->errorCount);
   for (i=0;i < stored; i++) {
      int code = ckt->errorStack[i];
      fprintf(out, " [%d] %s\n", code, error_to_text(code));
   }

   
}

//Push error function into actual output typedef 
static void push_error(CB_Ckt *out, int err) {
   //base case for no errors
   if (out == NULL || err == CB_EM_None) return;

   //Stack error handler
   if (out->errorCount < CB_MAX_ERROR_STACK) {
      out->errorStack[out->errorCount] = err;
   }
   /*Add +1 error
   Call ETM with err num to store in out
   Set last error code to most recent masked error*/
   out->errorCount++;
   out->errorMask |= error_to_mask(err);
   out->lastErrorCode = err; //most recent error code
}


//option switches for assigning bases of each build option
static unsigned base_Unknodes_for_difficulty(CB_difficulty d) {
   switch (d) {
      case CB_DIFF_Easy:      return 1u;
      case CB_DIFF_Medium:    return 2u;
      case CB_DIFF_Hard:      return 3u;
      case CB_DIFF_Extreme:   return 4u;
      case CB_DIFF_Einstein:  return 5u;
      default:                return 0u;
   }
}

static unsigned base_for_genre(CB_Genre g)
{
   switch (g) {
      case CB_G_dcSteady:     return 1u;
      case CB_G_AcDcSinusoidal: return 2u;
      case CB_G_AcSinusoidal: return 3u;
      case CB_G_RC:           return 4u;
      case CB_G_RLC:          return 5u;
      case CB_G_OPAMP:        return 6u;
      default:                return 0u;
   }
}

static unsigned base_for_numType(CB_numType n) {
   switch (n) {
      case CB_NT_Real:        return 1u;
      case CB_NT_RealComplex: return 2u;
      case CB_NT_Complex:     return 3u;
      case CB_NT_FreqDomain:  return 4u;
      default:                return 0u;
   }
}

static SwitchNumType to_switch_numType(CB_numType n) {
   switch (n) {
      case CB_NT_Real:        return SWITCH_REAL;
      case CB_NT_RealComplex: return SWITCH_RECO;
      case CB_NT_Complex:     return SWITCH_COMP;
      case CB_NT_FreqDomain:  return SWITCH_FREQ;
      default:                return SWITCH_REAL;
   }
}
//Non-Fatal Num conflict handle
static int warn_NonTypical_combo(CB_Genre genre, CB_numType numType) {
   if (genre == CB_G_AcSinusoidal && numType == CB_NT_Real) {
      return CB_WARN_Ac_Real;
   }
   if (genre == CB_G_AcDcSinusoidal && numType == CB_NT_Real) {
      return CB_WARN_AcDc_Real;
   }
   if (genre == CB_G_dcSteady && (numType == CB_NT_RealComplex || numType == CB_NT_Complex)) {
      return CB_WARN_DC_COMP;
   }
   if (numType == CB_NT_RealComplex) {
      return CB_WARN_MIX_TYPEC;
   }
   return CB_WARN_None;
}

static void seed_rng_once(void) {
   static int seeded = 0;
   if (!seeded) {
      srand((unsigned)time(NULL));
      seeded = 1;
   }
}

static double rand_range_double(double min, double max) {
   double t = (double)rand() / (double)RAND_MAX;
   return min + t * (max - min);
}

static int rand_range_int(int min, int max) {
   if (max <= min) return min;
   return min + (rand() % (max - min + 1));
}

static void assign_numType_value(CB_Component *c, SwitchNumType numTypeSw) {
   c->value = rand_range_double(100.0, 10000.0);
   c->imag = 0.0;

   switch (numTypeSw) {
      case SWITCH_REAL:
         c->imag = 0.0;
      break;
      case SWITCH_RECO:
         if (rand_range_int(0,1) == 1) {
            c->imag = rand_range_double(-5000.0, 5000.0);
            c->imHold = _Cbuild(c->value, c->imag);
         }
      break;
      case SWITCH_COMP:
         c->imag = rand_range_double(-5000.0, 5000.0);
         c->imHold = _Cbuild(c->value, c->imag);
      break;
      case SWITCH_FREQ:

      break;
      default:
      break;
   }
}

static int coinFlip(void) {
   return rand_range_int(0,1);
}
static int coinFlip3(void) {
   return rand_range_int(0,2);
}
static void assign_genreBase_Source(CB_Genre genre, CB_Source *s) {
   switch(genre) {
      case CB_G_dcSteady: 
         
         if (coinFlip()) {
            s->type = CB_SRC_VoltageDC;
            s->value = rand_range_double(1.0, 50.0);
            s->imag = 0.0;
         }
         else {
             s->type = CB_SRC_CurrentDC;
             s->value = rand_range_double(1.0, 10.0);
             s->imag = 0.0;
         }
      break;
      case CB_G_AcDcSinusoidal:
            if (coinFlip()) {
               s->type = CB_SRC_VoltageDC;
               s->value = rand_range_double(1.0, 50.0);
               s->imag = 0.0;
            }
            else {
               s->type = CB_SRC_CurrentDC;
               s->value = rand_range_double(1.0, 10.0);
               s->imag = 0.0;
            }
      break;
      case CB_G_AcSinusoidal:
         if (coinFlip()) {
            s->type = CB_SRC_VoltageAC;
            s->value = rand_range_double(1.0, 50.0);
            s->imag = rand_range_double(-500.0, 500.0);
            s->imHold = _Cbuild(s->value, s->imag);
         }
         else {
            s->type = CB_SRC_CurrentAC;
            s->value = rand_range_double(1.0, 10.0);
            s->imag = rand_range_double(-500.0, 500.0);
            s->imHold = _Cbuild(s->value, s->imag);
         }
      break;
   }
}

static void assign_genreBase_Component(CB_Genre genre, CB_Component *c, SwitchNumType numTypeSw) {
   
   switch(genre) {
      case CB_G_dcSteady:
         if (numTypeSw == SWITCH_REAL) {
            c->type = CB_COMP_Resistor;
         }
         else if (numTypeSw == SWITCH_RECO) {
            unsigned coin = (unsigned)coinFlip3();
            if (coin == 2) {
               c->type = CB_COMP_CAP;
            }
            else if(coin == 1) {
               c->type = CB_COMP_INDUC;
            }
            else {
               c->type = CB_COMP_Resistor;
            }
         }
         else if (numTypeSw == SWITCH_COMP) {
            unsigned coin = (unsigned)coinFlip();
            if (coin == 1) {
               c->type = CB_COMP_CAP;
            }
            else {
               c->type = CB_COMP_INDUC;
            }
         }
         else if (numTypeSw == SWITCH_FREQ) {

         }
         
         
      break;
      case CB_G_AcDcSinusoidal:
         
         if (numTypeSw == SWITCH_RECO) {
            unsigned coin = (unsigned)coinFlip3();
            if (coin == 2) {
               c->type = CB_COMP_CAP;
            }
            else if(coin == 1) {
               c->type = CB_COMP_INDUC;
            }
            else {
               c->type = CB_COMP_Resistor;
            }
         }
         else if (numTypeSw == SWITCH_COMP) {
            unsigned coin = (unsigned)coinFlip();
            if (coin == 1) {
               c->type = CB_COMP_CAP;
            }
            else {
               c->type = CB_COMP_INDUC;
            }
         }
         else if (numTypeSw == SWITCH_FREQ) {

         }
         else {
            c->type = CB_COMP_Resistor;
         }
      break;
      case CB_G_AcSinusoidal:
         if (numTypeSw == SWITCH_COMP) {
            if (coinFlip()) {
               c->type = CB_COMP_CAP;
            }
            else {
               c->type = CB_COMP_INDUC;
            }
         }
         else if (numTypeSw == SWITCH_RECO) {
            unsigned coin = (unsigned)coinFlip3();
            if (coin == 2) {
               c->type = CB_COMP_CAP;
            }
            else if(coin == 1) {
               c->type = CB_COMP_INDUC;
            }
            else {
               c->type = CB_COMP_Resistor;
            }
         }
         else if (numTypeSw == SWITCH_FREQ) {

         }
         else {
            c->type = CB_COMP_Resistor;
         }
      break;
   }
}
/* Difficulty: sets outputs to solve for and how many nodes will be in the circuit
   Genre: Sets: what type of circuit problem will be generated. Ex: Signals/Systems, RLC, RL, RC, Source/Resistors, Opamps, etc
   Number type: Sets if Number/Sources are Exponential, Rational, Complex*/
int buildCkt(const CB_buildOps *opt, CB_Ckt *out) {
   unsigned difficultyBase;
   unsigned genreBase;
   unsigned numTypeBase;
   int warnCode;

   SwitchNumType numTypeSw = SWITCH_REAL;

   
   //Handle Failure of output in case of NULL -> push NULL Err code
   if(out == NULL) {
      return CB_EM_BuildFailed;
   }
   //Configure generation settings by setting memory 
   memset(out, 0, sizeof(*out));
   out->lastErrorCode = CB_EM_None;

   seed_rng_once();

   //Failure of build options return NULL Err code
   if (opt == NULL) {
      push_error(out, CB_EM_BuildFailed);
      return out->lastErrorCode;
   }


   /*Push settings while setting/stacking Errors thus supplying a clean failure to user 
   to help them understand what they need to fix.
   Specifically checking Difficulty, Genre, and NumType*/
   out->opts.difficulty = opt->difficulty;
   if (!CB_diffValid(out->opts.difficulty)) {
      push_error(out, CB_EM_InvalidDifficulty);
   }

   out->opts.genre = opt->genre;
   if (!CB_GenreValid(out->opts.genre)) {
      push_error(out, CB_EM_InvalidGenre);
   }

   out->opts.numType = opt->numType;
   if (!CB_numTypeValid(out->opts.numType)) {
      push_error(out, CB_EM_InvalidNumType);
   }
   //if errors, return and cut build cycle
   if (out->errorCount > 0) {
      return out->lastErrorCode;
   }
   numTypeSw = to_switch_numType(out->opts.numType);
   

   difficultyBase = base_Unknodes_for_difficulty(out->opts.difficulty);
   if (difficultyBase == 0u) { 
      out->lastErrorCode = CB_EM_InvalidDifficulty;
      return CB_EM_InvalidDifficulty;
   }

   genreBase = base_for_genre(out->opts.genre);
   if (genreBase == 0u) { 
      out->lastErrorCode = CB_EM_InvalidGenre;
      return CB_EM_InvalidGenre;
   }

   numTypeBase = base_for_numType(out->opts.numType);
   if (numTypeBase == 0u) { 
      out->lastErrorCode = CB_EM_InvalidNumType;
      return CB_EM_InvalidNumType;
   }

   //Warning feature for numType
   warnCode = warn_NonTypical_combo(out->opts.genre, out->opts.numType);
   if (warnCode != CB_WARN_None) {
      fprintf(stderr, "[Circuit Engine Warning %d] Ac Sinusoidal with Real nums is valid but non-Typical\n", warnCode);
   }
   //Maps node count to diff base
   
   out->nodeCount = difficultyBase;
   ratio_for_difficulty(out->opts.difficulty, &out->sourceCount, &out->componentCount);
   //Difficulty base one - Easy setting
   if (difficultyBase == 1u) {
      
      for(unsigned i = 0; i < out->componentCount; i++) { 


         assign_genreBase_Component(out->opts.genre, &out->components[i], numTypeSw);
         if(i == 0) {
         //Node 1/2 node relative to each side of component
         out->components[i].n1 = 2u; //Resistor terminal Pos at node 1
         out->components[i].n2 = 0u; //resistor terminal Neg at node 2 (GND)
         }
         else if (i == 1) {
         out->components[i].n1 = 1u; //Resistor terminal Pos at node 1
         out->components[i].n2 = 2u; //resistor terminal Pos at node 2 
         }
         assign_numType_value(&out->components[i], numTypeSw);

         } //ohms
      }
      for(unsigned i = 0; i < out->sourceCount; i++) { 
         assign_genreBase_Source(out->opts.genre, &out->sources[i]);
         out->sources[i].nPlus = 1u; //Positive at n1
         out->sources[i].nMinus = 0u; //Neg at n2 (GND)
         
      }
   
   //Difficulty base Two - Medium setting
   if (difficultyBase == 2u) { 
      
      for(unsigned i = 0; i < out->componentCount; i++) {
            assign_genreBase_Component(out->opts.genre, &out->components[i], numTypeSw);
            assign_numType_value(&out->components[i], numTypeSw);
            if ((i % 2) == 0) {
               if (i < 2) {
                  out->components[i].n1 = (i + 1u);   // example placement
                  out->components[i].n2 = (i + 2u);   // example placement
               }
               else {
                  out->components[i].n1 = (i);   // example placement
                  out->components[i].n2 = (i + 1u);   // example placement
               }

            }
            if ((i % 2) == 1) {
               if (i < 3) {
                  out->components[i].n1 = (i + 1u);   // example placement
                  out->components[i].n2 = (i - 1u);   // example placement
               }
               else {
                  out->components[i].n1 = i;         // example placement
                  out->components[i].n2 = (i - i);   // example placement
               }
            }  
      }
      for(unsigned i = 0; i < out->sourceCount; i++) {
         assign_genreBase_Source(out->opts.genre, &out->sources[i]);
         out->sources[i].nPlus = 1u;
         out->sources[i].nMinus = 0u;
         
      }
   }
//Difficulty base three - Hard setting
   if (difficultyBase == 3u) { 
      
      for (unsigned i = 0; i < out->componentCount; i++) {
         assign_genreBase_Component(out->opts.genre, &out->components[i], numTypeSw);
         assign_numType_value(&out->components[i], numTypeSw);

         if ((i % 2) == 0) {
            if (i == 2) {
               out->components[i].n1 = (i);
               out->components[i].n2 = (i + 1u);
            }
            else if (i == 4) {
               out->components[i].n1 = (i);
               out->components[i].n2 = (i + 1u);
            }
            else {
               out->components[i].n1 = (i + 1u);
               out->components[i].n2 = (i + 2u);
            }
         }
         if ((i % 2) == 1) {
            if (i == 3) {
               out->components[i].n1 = (i);
               out->components[i].n2 = (i + 2u);
            }
            else if (i == 5) {
               out->components[i].n1 = (i - 3u);
               out->components[i].n2 = (i - 1u);
            }
            else {
               out->components[i].n1 = (i + 1u);
               out->components[i].n2 = (i - 1u);
            }
         }
      }
      for (unsigned i = 0; i < out->sourceCount; i++) {
          assign_genreBase_Source(out->opts.genre, &out->sources[i]);
         if ((i % 2) == 0) { 
            out->sources[i].nPlus = (i + 1u);
         }
         else if ((i % 2) == 1) {
            out->sources[i].nPlus = (i + 2u);
         }
         out->sources[i].nMinus = 0u;
      }
   }
   if (difficultyBase == 4u) { 
      
   }

      


   //Keep values resolved for future generation logic
   (void)genreBase;
   (void)numTypeBase;

   out->lastErrorCode = CB_EM_None;
   return CB_EM_None;
}



/*
                     ****IMPORTANT****
Once difficulties are able to be handled, start adding in 
feature to make each difficulty have different circuit
architeture - and make randomized components count for each difficulty

After thought, for example, different difficulties can have a range of 
component count - it was that a easy diffciulty circuit can have more than 2
components - it can have 3, 4, 5 even*/
