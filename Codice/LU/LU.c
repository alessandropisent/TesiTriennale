#include <stdlib.h>         /*malloc*/
#include <assert.h>         /*assert*/
#include <stdio.h>          /*printf*/
#include "LU.h"             /*definizione delle funzioni nel file*/

/*inizializzazzione della matrice e allocazione della memoria
    IP n, numero di righe
    IP m, numero di colonne
    IOP A, matrice inizializzata
*/
void initMatrix(int n, int m, Matrix *M){
    
    int i;
    int s = FREE_ROWS;
    /*Assegnazioni delle costanti per questo problema*/
    M->b = COLONNA_TERMINE_NOTO;
    M->s = FREE_ROWS;

    /*Assegnazioni delle capacita' di righe e colonne*/
    M->m = m;   /*Colonne*/
    M->n = n;   /*Righe*/

    /*Creazione del array di riga*/
    M->A = (double**) malloc( sizeof(double*) *(M->n + M->s) );
    assert(M->A != NULL);
    /*Creazione di un array per ogni riga*/
    for(i = 0; i<(s+n) ; i++){
        (M->A)[i] = (double*) malloc(sizeof(double) *(M->m + 1));
        assert((M->A)[i] != NULL);
    }
    

}/*initMatrix*/

/*  Funzione che libera la memoria assegnata alla matrice
    IOP M, matrice da liberare
*/
void freeMatrix(Matrix *M){

    int i;

    for(i = 1; i < M->m; i++)
        free((M->A)[i]);
    
    free(M->A);
    
}/*freeMatrix*/

void voidLUMatix(int segno, Matrix *M){

    int i,j;

    for(i=0;i<M->n;i++){
        for(j=0;j<M->m;j++){
            if(segno*(i-j)>=0)
                (M->A)[i+M->s][j+1] = 1;
            else
                (M->A)[i+M->s][j+1] = 0;
        }
    }
    for (i = 0; i < M->n; i++)
        (M->A)[i+M->s][0] = 0;
    

}


/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
*/
void solveTheMatrix(Matrix *M){

    Matrix L;
    Matrix U;


    createLU(M->n,M->m,&L,&U);
    printFMatrix(M);
    printFLU(&L,&U);
    freeLU(&L,&U);


}/*solveTheMatrix*/

void printFLU(Matrix *L, Matrix *U){
    printf("\nL:\n");
    printFMatrix(L);
    printf("\nU:\n");
    printFMatrix(U);
}

void createLU(int n, int m, Matrix *L, Matrix *U){

    initMatrix(n,m,L);
    initMatrix(n,m,U);

    voidLUMatix(1,L);
    voidLUMatix(-1,U);
}

void freeLU(Matrix *L, Matrix *U){
    freeMatrix(L);
    freeMatrix(U);
}

/*  Funzione che stampa una matrice a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrix( Matrix *M){

    int i,j;

    /*scandisco tutte le righe*/
    for(i=0;i<M->n;i++){

        /*scandisco tutte le colonne*/
        for(j = 0; j<(M->m)+1;j++)
            printf("%5.2f ",(M->A)[i+M->s][j]);

        printf("\n");   /*fine riga*/

    }/*for*/

}/*printFMatrix*/