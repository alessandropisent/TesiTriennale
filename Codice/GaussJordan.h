#include <stdbool.h> /*bool ovunque*/

#define FREE_ROWS 1
#define COLONNA_TERMINE_NOTO 0
#define ERROR 0.001

typedef struct {
    double** A;     /*Matrice con allocazione dinamica memoria*/
    int n;          /*# Righe*/
    int m;          /*# Colonne*/
    int b;          /*in che colonna si trova il termine noto*/
    int s;          /*# di righe che lascio libere*/
    int * dipRow;   /*array con gli indici delle righe dipendenti */    
    int nRD;        /*# di elenti in $dipRow*/
    double** B;     /*matrice con relazioni tra righe*/
} Matrix;

void initMatrix(int , int , Matrix *);

void freeMatrix(Matrix * );

void zeroB(Matrix *);

void printFMatrix(Matrix *);

void printFMatrixRel(Matrix *);

void diagNorm(int , int, Matrix *);

void zerosRow(int , int, int, Matrix *);

void zerosCol(int ,int,  Matrix * );

void solveTheMatrix(Matrix *);

void solveDiag(Matrix * );

void solveLinDip(Matrix *);

bool isZero(double a);

void addR(int , Matrix *);

bool isLinDip(int , Matrix *);

int min(int, int);