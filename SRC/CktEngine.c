//Libraries
#include <stdio.h>
#include <string.h>
#include <errno.h>

//Headers
#include "main.h"
#include "BaseGUI.h"
#include "CktEngine.h"
#include "cc_solver.h"


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
static unsigned base_nodes_for_difficulty(CB_difficulty d) {
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
      case CB_G_AcSinusoidal: return 2u;
      case CB_G_RL:           return 3u;
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

//Non-Fatal Num conflict handle
static int warn_NonTypical_combo(CB_Genre genre, CB_numType numType) {
   if (genre == CB_G_AcSinusoidal && numType == CB_NT_Real) {
      return CB_WARN_AcRealNonTypical;
   }
   return CB_WARN_None;
}

/* Difficulty: sets outputs to solve for and how many nodes will be in the circuit
   Genre: Sets: what type of circuit problem will be generated. Ex: Signals/Systems, RLC, RL, RC, Source/Resistors, Opamps, etc
   Number type: Sets if Number/Sources are Exponential, Rational, Complex*/
int buildCkt(const CB_buildOps *opt, CB_Ckt *out) {
   unsigned difficultyBase;
   unsigned genreBase;
   unsigned numTypeBase;
   int warnCode;

   //Handle Failure of output in case of NULL -> push NULL Err code
   if(out == NULL) {
      return CB_EM_BuildFailed;
   }
   //Configure generation settings by setting memory 
   memset(out, 0, sizeof(*out));
   out->lastErrorCode = CB_EM_None;

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

   difficultyBase = base_nodes_for_difficulty(out->opts.difficulty);
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
      fprintf(stderr, "[Cicuit Engine Warning %d] Ac Sinusoidal with Real nums is valid but non-Typical", warnCode);
   }
   //Maps node count to diff base
   out->nodeCount = difficultyBase;

   //Keep values resolved fro future generation logic
   (void)genreBase;
   (void)numTypeBase;

   out->lastErrorCode = CB_EM_None;
   return CB_EM_None;
}

/*MODIFY NEXT:
1. DEAL WITH OUT->COMPONENT COUNT: Develop a ratio based system 
that uses Difficulty level to confgure
2. Make errors actaully stack and print at end of build function if
Necessary  */
