#include <stdlib.h>         /*malloc*/
#include <assert.h>         /*assert*/
#include <stdio.h>          /*printf*/
#include "GaussJordan.h"    /*definizione delle funzioni nel file*/

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

    /*scandisco tutte le righe*/
    for(i=0;i<M->n;i++){

        /*scandisco tutte le colonne*/
        for(j = 0; j<(M->m)+1;j++)
            printf("%5.2f ",(M->A)[i+M->s][j]);

        printf("\n");   /*fine riga*/

    }/*for*/

}/*printFMatrix*/

/*  Funzione per normalizzare l'elemento sulla diagonale di una riga, 
    dividendola per se stessa

    IP r, riga della matrice su cui operare
    IOP M, matrice da modificare
*/
void rowNorm(int r, Matrix *M){
    /* Devo prendere la riga i-esima e nomalizzarla*/
    int i;

    /*prendo il valore sulla diagonale*/
    double t = (M->A)[r+(M->s)][r+1];

    /*nomalizziamo i valori sulla riga*/
    for(i=0;i<(M->m+1);i++)
        (M->A)[r+(M->s)][i]=((M->A)[r+1][i])/t;
    
    /*c'e' sempre il +M->s per offset che nella prima riga non c'e nulla*/

}/*rowNorm*/


/*  Funzione che azzera l'elemento M[$r][$c] e modifica di conseguenza la
    riga $r
    IP r, riga da modificare
    IP c, colonna dove ci troviamo 
    IOP M, Matrice da modificare
*/
void zerosRow(int r, int c, Matrix *M){
    int i;
    
    /*Prendo il valore del elemento M[$r][$c] che devo azzerare*/
    double t = (M->A)[r+(M->s)][c+1];

    /*Azzero il valore della riga $r e modifico gli altri di conseguenza*/
    for(i=0;i<(M->m+1);i++)
        (M->A)[r+(M->s)][i] = (M->A)[r+(M->s)][i] - t * (M->A)[c+(M->s)][i];
    

}/*zerosRow*/


/*  Funzione che normalizza l'elemento M[$c][$c] sulla diagonale e
    azzera gli altri elmenti tramite operazioni lineari
    IP c, colonna da modificare
    IOP M, Matrice da modificare
*/
void zerosCol(int c, Matrix * M){
    
    int i,row;

    /*normalizzo l'elemento sulla diagonale(a sulla riga c)*/
    rowNorm(c,M);

    /*Per ogni elemento*/
    for(i=0;i<(M->n-1);i++){

        row =  (i+1+c)%(M->m) ;
        zerosRow(row, c, M);

    }/*for*/

}/*zerosCol*/

/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
*/
void solveTheMatrix(Matrix *M){

    int i;  /*per il for*/

    /*Normalizzo l'elemento sulla diagonale e 
        azzero tutti gli altri elementi*/
    for(i=0;i<M->m;i++)
        zerosCol(i,M);
    
}/*solveTheMatrix*/