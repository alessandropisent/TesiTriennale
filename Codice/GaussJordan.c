#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "GaussJordan.h"

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

/*  Funzione che stampa una matrice a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrix( Matrix *M){

    int i,j;
    for(i=0;i<M->n;i++){

        for(j = 0; j<(M->m)+1;j++){
       
            printf("%5.2f ",(M->A)[i+M->s][j]);

        }/*for*/
        printf("\n");

    }/*for*/

}/*printFMatrix*/