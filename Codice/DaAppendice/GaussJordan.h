#include <stdbool.h> /*bool ovunque*/
#define FREE_ROWS 1
#define COLONNA_TERMINE_NOTO 0
#define MAX_STRING 100000
#define MAX_STAMPA 1000
#define MAX_STAMPA_EQN 100
typedef struct {
    double** MCoef;     /*Matrice con allocazione dinamica memoria*/
    int nEq; /*n*/           /*# di equazioni, # Righe*/
    int nIn; /*m*/           /*# di Incognit, # Colonne*/
    int* aEDip;           /*array con gli indici delle equazioni dipendenti */    
    int nEDip;            /*# di elenti in $dipRow*/
    double** MRAlg;     /*matrice con relazioni algebriche tra righe*/
    double error;       /*precisione di errore*/
} Matrix;
/*funzioni di inizializzazzione e liberazione*/
void initMatrix(int , int , Matrix *);
void oneMatrixRAlg(Matrix *);
void freeMatrix(Matrix * );
/*funzioni di stampa video delle matrici/sistema*/
void printFMatrix(const Matrix *);
void printFMatrixRAlg(const Matrix *);
void printEquations(const Matrix *);
/*funzioni per la risoluzione del sistema*/
int diagNorm(int , int, Matrix *);
void zerosRow(int , int, int, Matrix *);
void factMRAlg(int, int, int, Matrix *);
int zerosCol(int ,int,  Matrix *);
bool isEqLinDip(int, const Matrix *);
int solveTheMatrix(Matrix *);
/*funzioni accessorie*/
bool isZero(double , double);
void addR(int , Matrix *);
int min(int, int);
bool isZeroCoefAllEqnLinDip(const Matrix *);