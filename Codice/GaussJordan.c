#include <stdlib.h>         /*malloc*/
#include <assert.h>         /*assert*/
#include <stdio.h>          /*printf*/
#include <math.h>           /*fabs*/
#include "GaussJordan.h"    /*definizione delle funzioni nel file*/

/*inizializzazzione della matrice e allocazione della memoria
    IP n, numero di righe
    IP m, numero di colonne
    IOP A, matrice inizializzata
*/
void initMatrix(int n, int m, Matrix *M){
    
    int i;
    int initNDipRow = n-1; /*al massimo le righe indipendenti saranno il # di righe*/
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
    }/*for*/

    /*Creazione del array con le righe dipendenti*/
    M->nRD = 0; /*Array inizialmente vuoto*/
    M->dipRow = (int*) malloc(sizeof(int) * initNDipRow);
    assert(M->dipRow != NULL);
    

}/*initMatrix*/

/*  Funzione che libera la memoria assegnata alla matrice
    IOP M, matrice da liberare
*/
void freeMatrix(Matrix *M){

    int i;

    /*libera le colonne*/
    for(i = 1; i < M->m; i++)
        free((M->A)[i]);
    
    /*libera le righe*/
    free(M->A);

    /*libera la matrice con righe dipendenti*/
    free(M->dipRow);
    
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

    IP d, riga della matrice ideale su cui operare
    IOP M, matrice da modificare
    OR:  0 potevo normalizzare 
        -1 se non potevo diag
*/
void diagNorm(int d, Matrix *M){
    /* Devo prendere la riga i-esima e nomalizzarla*/
    int i;

    /*prendo il valore sulla diagonale*/
    double t = (M->A)[d+(M->s)][d+1];

    /*nomalizziamo i valori sulla riga*/
    for(i=0;i<(M->m+1);i++)
        (M->A)[d+(M->s)][i]=((M->A)[d+1][i])/t;
    
    /*c'e' sempre il +M->s per offset che nella prima riga non c'e nulla*/

}/*diagNorm*/


/*  Funzione che azzera l'elemento M[$r][$c] e modifica di conseguenza la
    riga $r. 

    IP r, riga da modificare
    IP c, colonna dove ci troviamo 
    IOP M, Matrice da modificare
*/
void zerosRow(int r, int c, Matrix *M){
    
    int i;
    double el;
    int z;
    
    /*Prendo il valore del elemento M[$r][$c] che devo azzerare*/
    double t = (M->A)[r+(M->s)][c+1];

    /*Azzero il valore della riga $r e modifico gli altri di conseguenza*/
    for(i=0;i<(M->m+1);i++){
        
        el = (M->A)[r+(M->s)][i] - t * (M->A)[c+(M->s)][i];
        (M->A)[r+(M->s)][i] = el;

        if(isZero(el))
            z++;

    }/*for*/

    if(z>=c)
        addR(r,M);
    

}/*zerosRow*/


/*  Funzione che normalizza l'elemento M[$c][$c] sulla diagonale e
    azzera gli altri elmenti tramite operazioni lineari
    IP c, colonna della matrice ideale da modificare
    IOP M, Matrice da modificare
*/
void zerosCol(int c, Matrix * M){
    
    int i,row;
    
    /*normalizzo la diagonale*/
    diagNorm(c,M);

    /*Per ogni elemento*/
    for(i=0;i<((M->n)-1);i++){

        row =  (i+1+c)%(M->n) ;
        zerosRow(row, c, M);

    }/*for*/
    

}/*zerosCol*/

/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
*/
void solveTheMatrix(Matrix *M){

    int i;  /*per il for*/
    bool start = true;   /*variabile per stato*/
    int skip = 0;           /*quante colonne ho schippato dal inizio*/
    int m = min(M->n,M->m);
    
    /*Normalizzo l'elemento sulla diagonale e 
        azzero tutti gli altri elementi*/
    for(i=0;i<m;i++){
        /*se l'elemento sulla diagonale M[i][i]!=0 allora posso continuare*/
        if(!isZero((M->A)[i+(M->s)][i+1])){
            zerosCol(i,M);
            start = false;
        }/*if*/
        
        /*se ho appena iniziato potrei avere qualche elemento
        sulla diagonale principale == 0 */
        else if(start)
            skip++;
        
    }/*for*/

    /*riparto dall'inizio con le righe che non ho fatto*/
    if (skip<m){
        for(i=0;i<skip;i++){
            /*controllo comunque che non sia ancora a zero*/
            if(!isZero((M->A)[i+(M->s)][i+1]))
                zerosCol(i,M);
        }/*for*/
    }/*if*/

    
        
    
}/*solveTheMatrix*/

/*  Funzione che dice se un double e' zero dato un errore
    IP a, valore da controllare se sia circa zero
    OR $a==0
*/
bool isZero(double a){

    return fabs(a)<ERROR;

}/*isZero*/


/*  Funzione aggiunge la riga all'array di righe indipendenti
    IP r, riga da aggiungere
    IOP M, matrice con tutto
*/
void addR(int r, Matrix* M){

    M->dipRow[M->nRD] = r;
    M->nRD +=1;    

}/*deleteR*/

/*  Funzione che risponde alla domanda : Questa riga $r e' linearmente
    dipente dalle altre righe

    IP r, riga da controllare
    IP M, Matrice con le righe
*/
bool isLinDip(int r, Matrix * M ){

    int i;

    for(i=0; i<(M->nRD);i++)
        if(r == (M->dipRow[i]))
            return true;

    return false;


}/*isLinDip*/

/*  Funzione che ritorna il minimo tra due interi
    IP a, intero a
    IP b, intero b
    OR a se a<b , altrimenti b
*/
int min(int a, int b){
    if (a<b)
        return a;
    return b;
}/*min*/