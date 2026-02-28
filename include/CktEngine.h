#pragma once

#include <stddef.h>
#include <stdint.h>

//pub types - Difficulty
typedef enum CB_difficulty{
    CB_DIFF_Easy = 0,
    CB_DIFF_Medium = 1,
    CB_DIFF_Hard = 2,
    CB_DIFF_Extreme = 3,
    CB_DIFF_Einstein = 4
} CB_difficulty;

//pub types - name genre types
typedef enum CB_Genre{
    CB_G_dcSteady = 0,
    CB_G_AcSinusoidal = 1,
    CB_G_RL = 2,
    CB_G_RC = 3,
    CB_G_RLC = 4,
    CB_G_OPAMP = 5
} CB_Genre;

//pub types - number type
typedef enum CBnumType{
    CB_NT_real = 0,
    CB_NT_Complex = 1,
    CB_NT_ComRealMix = 2,
    CB_NT_freqDomain = 3
} CBnumType;

//struct for build combination
typedef struct CB_buildOps{
    CB_difficulty difficulty;
    CB_Genre genre;
    CBnumType numType;
} CB_buildOps;

// Valid genre handler
static inline int CB_diffValid(CB_difficulty g) {
    return (g >= 0) && (g <= CB_DIFF_Einstein);
}
static inline int CB_GenreValid(CB_Genre g) {
    return (g >= 0) && (g <= CB_G_OPAMP);
}
static inline int CB_numTypeValid(CBnumType g) {
    return (g >= 0) && (g <= CB_NT_freqDomain);
}

//Error handle numbers
typedef enum CB_errorCode { 
    CB_ERR_None = 0,
    CB_ERR_InvalidDifficulty = 1,
    CB_ERR_InvalidGenre = 2,
    CB_ERR_InvalidNumType = 3,
    CB_ERR_BuildFailed = 4
} CB_errorCode;

//hide internal form users
typedef struct CB_Ckt{
    CB_buildOps opts; //Build Options used
    unsigned nodeCount; //nodes in generated circuit
    unsigned componentCount; //How many components in the 
    int lastErrorCode; //Stores code if error occurs
} CB_Ckt;


int buildCkt(const CB_buildOps *opt, CB_Ckt *out);