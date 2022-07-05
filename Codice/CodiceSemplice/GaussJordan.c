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

    /*forntespizio*/
    printf("\nSTAMPO A: b|A\n");

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
    IP r, riga della sotto matrice dei coefficienti su cui operare
    IP c, colonna della sotto matrice dei coefficienti su cui operare
    IOP M, matrice da modificare
  
*/
void diagNorm(int r, int c, Matrix *M){
    /* Devo prendere la riga i-esima e nomalizzarla*/
    int i;

    /*prendo il valore sulla diagonale*/
    double t = (M->A)[r+(M->s)][c+1];

    /*nomalizziamo i valori sulla riga*/
    for(i=0;i<(M->m+1);i++)
        (M->A)[r+(M->s)][i]=((M->A)[r+(M->s)][i])/t;
    
    
    /*c'e' sempre il +M->s per offset che nella prima riga non c'e nulla*/

}/*diagNorm*/


/*  Funzione che azzera l'elemento M[$r][$c] e modifica di conseguenza la
    riga $r. (prende come elemento di pivot M[$rowC][$c])

    IP r, riga da modificare
    IP c, colonna dove ci troviamo (Da azzerare)
    IP rowC, riga che sto prendendo in considerazione 
    IOP M, Matrice da modificare
*/
void zerosRow(int r, int c, int rowC, Matrix *M){
    
    int i;
    double el;
    int z=0;
    
    /*Prendo il valore del elemento M[$r][$c] che devo azzerare*/
    double t = (M->A)[r+(M->s)][c+1];
    if(isZero(t))
        t = 0;

    /*Azzero il valore della riga $r e modifico gli altri di conseguenza*/
    for(i=0;i<(M->m+1);i++){
        
        el = (M->A)[r+(M->s)][i] - t * (M->A)[rowC+(M->s)][i];
        (M->A)[r+(M->s)][i] = el;

        if(isZero(el))
            z++;

    }/*for*/

    if(z>=(M->m))
        addR(r,M);
    

}/*zerosRow*/


/*  Funzione che normalizza l'elemento M[$r][$c] sulla diagonale e
    azzera gli altri elmenti tramite operazioni lineari
    IP c, colonna della matrice ideale da modificare
    IP r, riga della matrice ideale da modificare
    IOP M, Matrice da modificare
*/
void zerosCol(int r, int c, Matrix * M){
    
    int i,row;
    
    
    /*normalizzo l'elemento M[$r][$c], in primis e' una diagonale*/
    diagNorm(r,c,M);

    /*Per ogni elemento*/
    for(i=0;i<((M->n)-1);i++){

        row =  (i+1+r)%(M->n);
        if(!rowLinDip(row,M))
            zerosRow(row, c, r, M);
        
            

    }/*for*/
    

}/*zerosCol*/

/*  Funzione che stabilisce se la riga $r e' linearmente dipendente
    alle altre righe della matrice
    IP row, riga da controllorare
    IP M, matrice
    OR se la $row, e' linearmente dipendente alle altre righe
*/
bool rowLinDip(int row, Matrix * M){

    int i;  /*pèer il for*/

    /*controllo tutto l'array con le righe lin dip*/
    for(i =0; i<(M->nRD);i++){

        /*match trovato*/
        if(row==((M->dipRow)[i]))
            return true;

    }/*for*/

    /*match non trovato*/
    return false;

}/*rowLinDip*/

/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
*/
void solveTheMatrix(Matrix *M){

    int i=0,r=0,c=0,j=0, k=0;  /*per il while*/
    int n = M->n;

    

    /*Devo trovare $diag - diagonali - #righe dipendenti*/
    while(i<(n-(M->nRD))){
        

        /*se l'elemento sulla diagonale M[i][i]!=0 allora posso continuare*/
        if(!isZero((M->A)[r+(M->s)][c+1])){
            zerosCol(r,c,M);
            i++;
        }/*if*/
    
        r=(r+1)%(M->n);
        j++;
        if(j<(M->m))
            c =j;
        else
            c = (M->dipRow)[k++];
        
    }/*while*/
    /*printf("FACCCIO j=%d iterazioni, n=%d,m=%d",j, n,M->m);*/    
    
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

}/*addR*/

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

/*  Funzione che ritorna il Massimo tra due interi
    IP a, intero a
    IP b, intero b
    OR a se a>b , altrimenti b
*/
int max(int a, int b){
    if (a>b)
        return a;
    return b;
}/*min*/
