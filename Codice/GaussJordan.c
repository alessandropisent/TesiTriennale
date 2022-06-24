/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file ci sono codificate tutte le funzioni per la risoluzione di 
    un sistema lineare tramite il metodo di Gauss Jordan. 

    Inoltre cerca di calcolare il rapporto di proporzionalita' tra le 
    righe, e lo memorizza in una matrice B, all'interno del tipo strutturato 
    Matrix.



    Appunti:
    Prende in lettura un file con la matrice e i coefficienti noti
    
    
*/
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
    
    /*creazione della matrice per le relazioni tra le righe*/
    M->B = (double**) malloc( sizeof(double*) *(M->n) );
    assert(M->B != NULL);

    /*Creazione di un array per ogni riga*/
    for(i = 0; i<(s+n) ; i++){
        (M->B)[i] = (double*) malloc(sizeof(double) *(M->m));
        assert((M->B)[i] != NULL);
    }/*for*/

    /*funzione che mette zeri nella matrice B di relazioni*/
    zeroB(M);

}/*initMatrix*/

/*  Funzione che libera la memoria assegnata alla matrice
    IOP M, matrice da liberare
*/
void freeMatrix(Matrix *M){

    int i;

    /*libera le colonne*/
    for(i = 1; i < M->m; i++){
        free((M->A)[i]);
        free((M->B)[i]);
    }/*for*/
        
    
    /*libera le righe*/
    free(M->A);
    free(M->B);

    /*libera la matrice con righe dipendenti*/
    free(M->dipRow);
    
}/*freeMatrix*/

void zeroB(Matrix* M){
    int i,j;

    for(i = 0; i< M->n; i++){
        for(j=0;j< M->m; j++)
            (M->B)[i][j]=0;
        
    }/*for*/
}/*zeroB*/

/*  Funzione che stampa una matrice a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrix( Matrix *M){

    int i,j;
    /*forntespizio*/
    printf("\nSTAMPO A\n");
    /*scandisco tutte le righe*/
    for(i=0;i<M->n;i++){

        /*scandisco tutte le colonne*/
        for(j = 0; j<(M->m)+1;j++)
            printf("%5.2f ",(M->A)[i+M->s][j]);

        printf("\n");   /*fine riga*/

    }/*for*/

}/*printFMatrix*/

/*  Funzione che stampa una matrice Relazioni a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrixRel(Matrix *M){

    int i,j;

    /*frontespizio*/
    printf("\nSTAMPO B\n");
    /*scandisco tutte le righe*/
    for(i=0;i<M->n;i++){

        /*scandisco tutte le colonne*/
        for(j = 0; j<(M->m);j++)
            printf("%5.2f ",(M->B)[i][j]);

        printf("\n");   /*fine riga*/

    }/*for*/

}/*printFMatrixRel*/

/*  Funzione per normalizzare l'elemento sulla "diagonale" di una riga, 
    dividendola per se stessa

    IP r, riga della matrice ideale su cui operare
    IP c, colonna della matrice ideale su cui operare
    IOP M, matrice da modificare
  
*/
void diagNorm(int r, int c, Matrix *M){
    /* Devo prendere la riga i-esima e nomalizzarla*/
    int i,indexRow;

    /*prendo il valore sulla diagonale*/
    double t = (M->A)[r+(M->s)][c+1];

    /*metto t in tutta la colonna di B*/
    for(i=0;i<M->n-1;i++){
        indexRow = (i+r+1)%(M->n);
        (M->B)[indexRow][c] = t;
    }/*for*/
    

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
    
    /*per non propagare l'errore di t*/
    if(isZero(t))
        t = 0;

    /*Scrivo la relazione tra le righe*/
    (M->B)[r][c] = t/((M->B)[r][c]);

    /*controllo se e' una delle righe linearmente dipendente*/
    if(rowLinDip(r,M))
        /*in caso non modifico riga $r e esco*/
        return;

    /*Azzero il valore della riga $r e modifico gli altri di conseguenza*/
    for(i=0;i<(M->m+1);i++){
        
        el = (M->A)[r+(M->s)][i] - t * (M->A)[rowC+(M->s)][i];
        (M->A)[r+(M->s)][i] = el;

        /*conto gli zeri che trovo*/
        if(isZero(el))
            z++;

    }/*for*/

    /*se ho una riga di zeri allora e' una riga lin dip*/
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

    return false;

}/*rowLinDip*/

/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
*/
void solveTheMatrix(Matrix *M){

    /*risolvo gli elementi sulla diagonale*/
    solveDiag(M);

    /*Solo se ho #righe > #Colonne devo continuare l'algorimo*/
    if((M->n) > (M->m))
        /*se ho trovato delle righe linearmente dipendendi 
            risolvo per le altre righe*/
        solveLinDip(M);

    
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


/*  Funzione che risolve la Matrice guardando solo alla parte quadrata
    IOP Matrice da risolvere
*/
void solveDiag(Matrix * M){

    int i;  /*per il for*/
    bool start = true;   /*variabile per stato*/
    int skip = 0;           /*quante colonne ho schippato dal inizio*/
    int m = min(M->n,M->m);
    
    /*Normalizzo l'elemento sulla diagonale e 
        azzero tutti gli altri elementi*/
    for(i=0;i<m;i++){
        /*se l'elemento sulla diagonale M[i][i]!=0 allora posso continuare*/
        if(!isZero((M->A)[i+(M->s)][i+1])){
            zerosCol(i,i,M);
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
                zerosCol(i,i,M);

        }/*for*/
    }/*if*/

}/*solveDiag*/


/*  Funzione che risolve le righe linearmente dipendenti
    IOP Matrice da risolvere
*/
void solveLinDip(Matrix * M){

    int i;              /*i per il for*/
    int s = M->n-1;     /*inizio dalla riga */
    bool start = true;  /*e' la prima volta che cerco*/
    int skip = 0;       /*righe skippate*/
    int r,c;            /*variabili di indice riga e colonna, per semplificare 
                        lettura*/

    /*for che scandisce tutti gli elementi nel array delle righe dipendenti*/
    for(i=0;i<(M->nRD);i++){
        
        /*calcolo per gli indici di riga e colonna*/
        r = s+i;
        c = (M->dipRow)[i];     /*la colonna da ricalcolare sara' uguale alla colonna lin dip*/

        /*limito fino a quando posso andare con gli indici*/
        if(c>=(M->m) || r>=(M->n))
            break;
        
        /* se l elemento sulla M[r][c]!=0 allora posso continuare*/
        if(!isZero((M->A)[r+(M->s)][c+1])){
            zerosCol(r,c,M);
            start = false;
        }/*if*/

        /*se ho appena iniziato potrei avere qualche elemento
        sulla "diagonale" principale == 0 */
        else if(start)
            skip++;
    
    }/*for*/

    /*riparto dall'inizio con le righe che non ho fatto*/
    if (skip<(s-M->m)){
        for(i=0;i<skip;i++){

            /*calcolo degli indici*/
            r = s+i;
            c = (M->dipRow)[i]; /*la colonna da ricalcolare sara' uguale alla colonna lin dip*/
            
            /*limito fino a quando posso andare con gli indici*/
            if(c>=(M->m) || r>=(M->n))
                break;
            
            /*controllo comunque che non sia ancora a zero*/
            if((c<(M->m)) && !isZero((M->A)[r+(M->s)][c+1]))
                zerosCol(r,c,M);
        }/*for*/
    }/*if*/
    

}/*solveLinDip*/

