#pragma once

#include <stdint.h>
#include <stdio.h>
#include <complex.h>



#define CB_MAX_ERROR_STACK 8 
#define CB_MAX_WARN_STACK  1
#define BE_MAX_DIFFICULTY  5
#define CB_MAX_COMPONENTS 64
#define CB_MAX_SOURCES    16

typedef uint32_t CB_WarnMask;
enum{
    CB_WM_None             = 0u,
    CB_WM_AcRealNonTypical = 1u << 0
};
//Warning code implementation
enum {
    CB_WARN_None = 0,
    CB_WARN_AcRealNonTypical = 1001
};

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
typedef enum CB_numType{
    CB_NT_Real = 0,
    CB_NT_Complex = 1,
    CB_NT_RealComplex = 2,
    CB_NT_FreqDomain = 3
} CB_numType;

//struct for build combination
typedef struct CB_buildOps{
    CB_difficulty difficulty;
    CB_Genre genre;
    CB_numType numType;
} CB_buildOps;

typedef enum {
    CB_COMP_Resistor = 1,
    CB_COMP_CAP = 2,
    CB_COMP_INDUC = 3,
} CB_ComponentType;

typedef enum {
    CB_SRC_VoltageACDC = 1,
    CB_SRC_CurrentACDC = 2,
} CB_SourceType;

typedef struct {
    CB_SourceType type;
    unsigned nPlus, nMinus;
    double value;
    double imag;
} CB_Source;

typedef struct {
    CB_ComponentType type;
    unsigned n1, n2;
    double value;
    double imag;
} CB_Component;
// Valid genre handler
static inline int CB_diffValid(CB_difficulty g) {
    return (g >= 0) && (g <= CB_DIFF_Einstein);
}
static inline int CB_GenreValid(CB_Genre g) {
    return (g >= 0) && (g <= CB_G_OPAMP);
}
static inline int CB_numTypeValid(CB_numType g) {
    return (g >= 0) && (g <= CB_NT_FreqDomain);
}



//Error handle numbers
typedef uint32_t CB_errorMask;
enum { 
    CB_EM_None                 = 0u,
    CB_EM_BuildFailed          = 1u << 0,
    CB_EM_InvalidGenre         = 1u << 1,
    CB_EM_InvalidNumType       = 1u << 2,
    CB_EM_InvalidDifficulty    = 1u << 3
};

//hide internal form users
typedef struct CB_Ckt{
    CB_buildOps opts; //Build Options used
    unsigned nodeCount; //nodes in generated circuit
    unsigned sourceCount; //sources in circuit
    unsigned componentCount; //How many components in the 
    int lastErrorCode; //Stores code if error occurs

    CB_errorMask errorMask; //Enum to mask error Nums
    int errorCount; //Count total Erros
    int errorStack[CB_MAX_ERROR_STACK]; //Count error codes together

    CB_WarnMask warnMask; //Warning structs
    int warnCount;
    int warnStack[CB_MAX_WARN_STACK];

    CB_Component components[CB_MAX_COMPONENTS];
    CB_Source    sources[CB_MAX_SOURCES];
} CB_Ckt;



//Main functions that utilize pointers
int buildCkt(const CB_buildOps *opt, CB_Ckt *out);

//Error print Dec
void CB_printErrors(const CB_Ckt *ckt, FILE *stream);

