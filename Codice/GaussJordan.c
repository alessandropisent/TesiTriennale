/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file ci sono codificate tutte le funzioni per la risoluzione di 
    un sistema lineare tramite il metodo di Gauss Jordan. 

    Appunti:
    Cerca di calcolare il rapporto di proporzionalita' tra le  righe, e lo 
    memorizza in una matrice B, all'interno del tipo strutturato SisEqLin.

    L'albero delle chiamate: (Analisi approssimata)
    1:solveTheMatrix O(n^3)
        1: solveDiag         Risolvo, o almeno provo a risolvere la matrice 
                                come se fosse una min(m,n) x min(m,n)
            min(n,m): ZeroCol O(nm)      Azzera la colonna selezionata
                1:diagNorm O(n)      Normalizza l'elemento sulla diagonale
                n-1: zeroRow O(m)    Azzera l'elemento della riga, e modifica gli altri  

        Solo se ho #righe > #Colonne devo continuare l'algorimo
        1: solveLinDip
            n-m: ZeroCol O(nm)            Azzera la colonna selezionata
                1:diagNorm O(n)      Normalizza l'elemento sulla diagonale
                n-1: zeroRow O(m)    Azzera l'elemento della riga, e modifica gli altri  

    La matrice MCoeff va da [1][1] -> [nEq][nIn]
    
*/
#include <stdlib.h>         /*malloc, free*/
#include <assert.h>         /*assert*/
#include <stdio.h>          /*printf*/
#include <math.h>           /*fabs*/
#include <string.h>         /*strcat*/
#include "GaussJordan.h"    /*definizione delle funzioni nel file*/

/*inizializzazzione della matrice e allocazione della memoria
    IP n, numero di righe
    IP m, numero di colonne
    IOP A, matrice inizializzata
*/
void initMatrix(int n, int m, SisEqLin *M){

    int i;
    int initNDipRow = n-1; /*al massimo le righe indipendenti saranno il # di nEq-1*/
    int dim = sizeof(double) *(m + 1);  /*per array Coefficienti*/

    /*Assegnazioni delle capacita' di righe e colonne*/
    M->nEq = n;   /*Equazioni / Righe*/
    M->nIn = m;   /*Variabili / Colonne*/

    /*Creazione del array di riga*/
    M->MCoef = (double**) malloc( sizeof(double*) *(M->nIn + 1) );
    assert(M->MCoef != NULL);

    /*Creazione di un array per ogni riga*/
    for(i = 0; i<(n+1) ; i++){
        (M->MCoef)[i] = (double*) malloc(dim);
        assert((M->MCoef)[i] != NULL);
    }/*for*/

    /*Creazione del array con le righe dipendenti*/
    M->nEDip = 0; /*Array inizialmente vuoto*/
    M->aEDip = (int*) malloc(sizeof(int) * initNDipRow);
    assert(M->aEDip != NULL);

    /*creazione della matrice per le relazioni tra le righe*/
    M->MRAlg = (double**) malloc( sizeof(double*) * n );
    assert(M->MRAlg != NULL);

    /*Creazione di un array per ogni riga*/
    for(i = 0; i< n; i++){
        (M->MRAlg)[i] = (double*) malloc( n * sizeof(double));
        assert((M->MRAlg)[i] != NULL);
    }/*for*/
    oneMatrixRAlg(M);

}/*initMatrix*/

void oneMatrixRAlg(SisEqLin *M){

    int i,j;
    for(i=0;i<M->nEq;i++){

        for(j=0;j<M->nEq;j++){

            if(i==j)
                (M->MRAlg)[i][j]=1;
            else
                (M->MRAlg)[i][j]=0;

        }/*for*/

    }/*for*/

}/*oneMatrixRAlg*/

/*  Funzione che libera la memoria assegnata alla matrice
    IOP M, matrice da liberare
*/
void freeMatrix(SisEqLin *M){

    int i;

    /*libera le righe*/
    for(i = 1; i < M->nIn; i++){
        free((M->MCoef)[i]);
        free((M->MRAlg)[i]);
    }/*for*/

    /*libera le righe*/
    free(M->MCoef);
    free(M->MRAlg);

    /*libera la matrice con righe dipendenti*/
    free(M->aEDip);

}/*freeMatrix*/

/*  Funzione che stampa una matrice a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrix(const SisEqLin *M){

    int i,j;

    /*forntespizio*/
    printf("\nSTAMPO A: b|A\n");

    /*scandisco tutte le righe*/
    for(i=0;i<M->nEq;i++){

        /*scandisco tutte le colonne*/
        for(j = 0; j<(M->nIn)+1;j++)
            printf("%5.2f ",(M->MCoef)[i+1][j]);

        printf("\n");   /*fine riga*/

    }/*for*/

}/*printFMatrix*/

/*  Funzione che stampa una matrice Relazioni Algebriche tra le righe/Equazioni 
    a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrixRAlg(const SisEqLin *M){

    int i,j;

    /*frontespizio*/
    printf("\nSTAMPO B\n");
    /*scandisco tutte le righe*/
    for(i=0;i<M->nEq;i++){

        /*scandisco tutte le colonne*/
        for(j = 0; j<M->nEq;j++)
            printf("%5.2f ",(M->MRAlg)[i][j]);

        printf("\n");   /*fine riga*/

    }/*for*/

}/*printFMatrixRAlg*/


/*  Funzione per normalizzare l'elemento sulla "diagonale" di una riga, 
    dividendola per se stessa

    IP r, riga della matrice su cui operare
    IP c, colonna della matrice su cui operare
    IOP M, matrice da modificare
  
*/
void diagNorm(int r, int c, SisEqLin *M){
    /* Devo prendere la riga i-esima e nomalizzarla*/
    int i;

    /*prendo il valore sulla diagonale*/
    double t = (M->MCoef)[r][c];

    (M->MRAlg)[r][r] = 1/t;  

    /*nomalizziamo i valori sulla riga*/
    for(i=0;i<(M->nIn+1);i++)
        (M->MCoef)[r][i]=((M->MCoef)[r][i])/t;

}/*diagNorm*/


/*  Funzione che azzera l'elemento M[$r][$c] e modifica di conseguenza la
    riga $r. (prende come elemento di pivot M[$rowC][$c])
    Qui c'e' il controllo se la riga e' linearmente dipendente, nel caso,
    non prova ad modificarla tutta

    IP r, riga da modificare
    IP c, colonna dove ci troviamo (Da azzerare)
    IP rowC, riga che sto prendendo in considerazione 
    IOP M, Matrice da modificare
*/
void zerosRow(int r, int c, int rowC, SisEqLin *M){
    
    int i;
    double el;
    int z=0;
    
    /*Prendo il valore del elemento M[$r][$c] che devo azzerare*/
    double t = (M->MCoef)[r][c];
    (M->MRAlg)[r-1][rowC-1] = - t / ((M->MRAlg)[r-1][r-1]);

    printFMatrixRAlg(M);

    /*Azzero il valore della riga $r e modifico gli altri di conseguenza*/
    for(i=0;i<(M->nIn+1);i++){
        
        el = (M->MCoef)[r][i] - t * (M->MCoef)[rowC][i];
        (M->MCoef)[r][i] = el;

        /*conto gli zeri che trovo*/
        if(isZero(el,M->error))
            z++;


    }/*for*/

    /*se ho una riga di zeri allora e' una riga lin dip*/
    if(z>=(M->nIn))
        addR(r,M);

    
}/*zerosRow*/


/*  Funzione che normalizza l'elemento M[$r][$c] sulla diagonale e
    azzera gli altri elmenti tramite operazioni lineari
    IP c, colonna della matrice ideale da modificare
    IP r, riga della matrice ideale da modificare
    IOP M, Matrice da modificare
*/
void zerosCol(int r, int c, SisEqLin * M){
    
    int i,row;  /*variabili per il ciclo, e per la riga*/
    
    /*normalizzo l'elemento M[$r][$c], in primis e' una diagonale*/
    diagNorm(r,c,M);

    /*Per ogni elemento*/
    for(i=0;i<((M->nEq)-1);i++){

        row = (i+r)%(M->nEq)+1;
        if(!isEqLinDip(row,M))
            zerosRow(row, c, r, M);

    }/*for*/

}/*zerosCol*/

/*  Funzione che stabilisce se la riga $r e' linearmente dipendente
    alle altre righe della matrice
    IP row, riga da controllorare
    IP M, matrice
    OR se la $row, e' linearmente dipendente alle altre righe
*/
bool isEqLinDip(int row, const SisEqLin * M){

    int i;  /*per il for*/

    /*controllo tutto l'array con le righe lin dip*/
    for(i =0; i<(M->nEDip);i++){

        /*match trovato*/
        if(row==((M->aEDip)[i]))
            return true;

    }/*for*/

    /*match non trovato*/
    return false;

}/*isEqLinDip*/

/*  Funzione che controlla che il sistema non sia impossibile
    i.e. controlla che i coefficienti noti di tutte le eqn lin dip siano zero
    IP M, matrice con tutti i dati
    OR se il sistema rappresentato dalla matrice M, e' risolvibile
*/
bool isZeroCoefAllEqnLinDip(const SisEqLin* M){

    int i,r;
    for(i=0;i<M->nEDip;i++){
        r = M->aEDip[i];
        if(!isZero(M->MCoef[r][0],M->error))
            return false;
    }/*for*/
    return true;

}/*isZeroCoefAllEqnLinDip*/

/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
*/
void solveTheMatrix(SisEqLin *M){

    int i=0,r=0,c=1,j=0, k=0;  /*per il while*/
    int n = M->nEq;

    /*Devo trovare $diag - diagonali - #righe dipendenti*/
    while(i<(n-(M->nEDip))){
        

        /*se l'elemento sulla diagonale M[i][i]!=0 allora posso continuare*/
        if(!isZero((M->MCoef)[r+1][c], M->error)){
            
            zerosCol(r+1,c,M);
            i++;
        
        }/*if*/

        r=(r+1)%(M->nEq);
        j++;

        if(j<(M->nIn))
            c =j+1;
        else
            c = (M->aEDip)[k++];
        

    }/*while*/

    
}/*solveTheMatrix*/

/*  Funzione che dice se un double e' zero dato un errore
    IP a, valore da controllare se sia circa zero
    IP precisone
    OR $a==0
*/
bool isZero(double a, double precisione){

    return fabs(a)<precisione;

}/*isZero*/


/*  Funzione aggiunge la riga all'array di righe indipendenti
    IP r, riga da aggiungere
    IOP M, matrice con tutto
*/
void addR(int r, SisEqLin* M){
    
    M->aEDip[M->nEDip] = r;
    M->nEDip +=1;

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


/*  Funzione per testare che effettivamente la relazione trovata 
    Sia giusta
    IP S, matrice risolta
    IP T, matrice non risolta "Originale"
    IP printTest, se vogliamo stampare a schermo le operazioni compiute dal test
    OR : TRUE se il test e' andato a buon fine (ho trovato la vera relazione)
         FALSE se il test non e' andato a buon fine (Ho sbagliato)   
*/
bool test(SisEqLin* S, SisEqLin* T){
    int  d, j, i, iR;
    double el,sum,molt, elMoltiplicato;


        printf("\nORIGINAL");
        printFMatrix(T);
        printf("\nSOLVED");
        printFMatrix(S);
        printFMatrixRAlg(S);
        printf("Dobbiamo controllare R = ");
        for(i=0;i<(S->nEDip);i++)
            printf("%d, ",S->aEDip[i]);
        printf("\n");


    /*per ogni riga dipendente*/
    for(d=0;d<S->nEDip;d++){

        /*indice della riga che stiamo controllando*/
        iR = (S->aEDip)[d];
        
        /*per ogni elemento della riga somma tutti gli elmenti 
            delle colonne*/
        for(i=0;i<(S->nIn);i++){
            /*somma degli elementi*/
            sum = 0;

            /*per ogni */
            for(j=0;j<(S->nEq);j++){

                /*dichiarazioni di variabili per poi stampare*/
                molt = ((S->MRAlg)[iR][j]);
                elMoltiplicato = ((T->MCoef)[j+1][i+1]);

                sum += molt*elMoltiplicato;
                
            }/*for*/

            /*elemento della colonna dipendente*/
            el = (T->MCoef)[iR+1][i+1];
            

            if(!isZero(el-sum,S->error))
                return false;
        }/*for*/
    }/*for*/

    return true;

}/*test*/


/*  Fuznione che stampa a schermo il sistema di equazioni a schermo
    IP SisEqLin M
    OV il sistema di equazioni
*/
void printEquations(const SisEqLin *M){

    int i,j;
    
    /*stampa del forntespizio*/
    printf("\nIL SISTEMA DI EQUAZIONI e':\n");
    for(i = 0; i < M->nEq; i++){

        for(j=0;j<M->nIn-1;j++)
            printf("%5.2f * x%d + ",(M->MCoef)[i+1][j+1],j+1);

        
        /*stampa l'ultimo elemento*/
        printf("%5.2f * x%d ",(M->MCoef)[i+1][j+1],j+1);

        /*stampa termine noto*/
        printf(" = %5.2f\n",(M->MCoef)[i+1][0]);


    }/*for*/

}/*printEquations*/
