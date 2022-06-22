#include <stdbool.h> 

#define FREE_ROWS 1
#define COLONNA_TERMINE_NOTO 0

typedef struct {
    double** A;     /*array con allocazione dinamica memoria*/
    int n;          /*# Righe*/
    int m;          /*# Colonne*/
    int b;          /*in che colonna si trova il termine noto*/
    int s;          /*# di righe che lascio libere*/
} Matrix;

void initMatrix(int , int , Matrix *);

void freeMatrix(Matrix * );

void printFMatrix(Matrix *);

void solveTheMatrix(Matrix *);

void printFLU(Matrix *L, Matrix *U);

void createLU(int n, int m, Matrix *L, Matrix *U);

void freeLU(Matrix *L, Matrix *U);