#include <stdbool.h> /*bool ovunque*/

#define FREE_ROWS 1
#define COLONNA_TERMINE_NOTO 0
#define MAX_STRING 200
#define MAX_STAMPA 1000

typedef struct {
    double** MCoef;     /*Matrice con allocazione dinamica memoria*/
    int nEq; /*n*/           /*# di equazioni, # Righe*/
    int nIn; /*m*/           /*# di Incognit, # Colonne*/
    int* aEDip;           /*array con gli indici delle equazioni dipendenti */    
    int nEDip;            /*# di elenti in $dipRow*/
    double** MRAlg;     /*matrice con relazioni algebriche tra righe*/
    double error;       /*precisione di errore*/
} Matrix;

void initMatrix(int , int , Matrix *);

void oneMatrixRAlg(Matrix *);

void freeMatrix(Matrix * );

void printFMatrix(const Matrix *);

void printFMatrixRAlg(const Matrix *);

void diagNorm(int , int, Matrix *);

void zerosRow(int , int, int, Matrix *);

void factMRAlg(int, int, int, Matrix *);

void zerosCol(int ,int,  Matrix *);

bool isEqLinDip(int, const Matrix *);

bool isZeroCoefAllEqnLinDip(const Matrix *);

void solveTheMatrix(Matrix *);

void solveDiag(Matrix * );

void solveLinDip(Matrix *);

bool isZero(double , double);

void addR(int , Matrix *);

int min(int, int);

bool test(Matrix* , Matrix* );

void printEquations(const Matrix *);

bool isColSolved(int, const Matrix *);

int whichColAreNotSolved(int* , const Matrix * );

void FprintFCRNS (const Matrix *);