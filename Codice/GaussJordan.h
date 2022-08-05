#include <stdbool.h> /*bool ovunque*/

#define FREE_ROWS 1
#define COLONNA_TERMINE_NOTO 0
#define MAX_STRING 200

typedef struct {
    double** MCoef;     /*Matrice con allocazione dinamica memoria*/
    int nEq; /*n*/           /*# di equazioni, # Righe*/
    int nIn; /*m*/           /*# di Incognit, # Colonne*/
    int* aEDip;           /*array con gli indici delle equazioni dipendenti */    
    int nEDip;            /*# di elenti in $dipRow*/
    double** MRAlg;     /*matrice con relazioni algebriche tra righe*/
    double error;       /*precisione di errore*/
} SisEqLin;

void initMatrix(int , int , SisEqLin *);

void oneMatrixRAlg(SisEqLin *);

void freeMatrix(SisEqLin * );

void printFMatrix(const SisEqLin *);

void printFMatrixRAlg(const SisEqLin *);

void diagNorm(int , int, SisEqLin *);

void zerosRow(int , int, int, SisEqLin *);

void zerosCol(int ,int,  SisEqLin * );

bool isEqLinDip(int, const SisEqLin *);

bool isZeroCoefAllEqnLinDip(const SisEqLin *);

void solveTheMatrix(SisEqLin *);

void solveDiag(SisEqLin * );

void solveLinDip(SisEqLin *);

bool isZero(double , double);

void addR(int , SisEqLin *);

int min(int, int);

bool test(SisEqLin* , SisEqLin* );

void printEquations(const SisEqLin *);