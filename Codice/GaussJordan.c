/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file ci sono codificate tutte le funzioni per la risoluzione di 
    un sistema lineare tramite il metodo di Gauss Jordan. 

    Appunti:
    Cerca di calcolare il rapporto di proporzionalita' tra le  righe, e lo 
    memorizza in una matrice B, all'interno del tipo strutturato Matrix.

    L'albero delle chiamate: (Analisi approssimata) TAG:TODO
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
void initMatrix(int n, int m, Matrix *M){

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
    for(i = 0; i< n ; i++){
        (M->MRAlg)[i] = (double*) calloc( n , sizeof(double));
        assert((M->MRAlg)[i] != NULL);
    }/*for*/

    /*inizializzazzione della matrice relazioni*/
    oneMatrixRAlg(M);

}/*initMatrix*/

/*  Funzione che crea una matrice di 1 di dimensione $nEq x $nEq nella matrice MRAlg
    IOP Matrix M, strutturato con dentro le equazioni
*/
void oneMatrixRAlg(Matrix *M){

    int i;
    for(i=0;i<M->nEq;i++)
        (M->MRAlg)[i][i]=1;

}/*oneMatrixRAlg*/

/*  Funzione che libera la memoria assegnata alla matrice
    IOP M, matrice da liberare
*/
void freeMatrix(Matrix *M){

    int i;

    /*libera le righe coefficienti*/
    for(i = 1; i < M->nIn; i++)
        free((M->MCoef)[i]);
    
    /*libera righe relazioni*/
    for ( i = 0; i < M->nEq; i++)
        free((M->MRAlg)[i]);
    

    /*libera le array di righe*/
    free(M->MCoef);
    free(M->MRAlg);

    /*libera la matrice con righe dipendenti*/
    free(M->aEDip);

}/*freeMatrix*/

/*  Funzione che stampa una matrice a schermo
    IP M, matrice da stampare
    OV matrice stampata
*/
void printFMatrix(const Matrix *M){

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
void printFMatrixRAlg(const Matrix *M){

    int i,j;

    /*frontespizio*/
    printf("\nSTAMPO MRAlg:\n");
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
    OP
  
*/
void diagNorm(int r, int c, Matrix *M){
    /* Devo prendere la riga i-esima e nomalizzarla*/
    int i;

    /*prendo il valore sulla diagonale*/
    double t = (M->MCoef)[r][c];
    /*Normalizzo l'elemento sulla sua stessa riga*/
    (M->MRAlg)[r-1][r-1]=(M->MRAlg)[r-1][r-1]/t; /*TAG:Ralg*/


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
void zerosRow(int r, int c, int rowC, Matrix *M){
    
    int i;
    double el;
    int z=0;
    
    /*Prendo il valore del elemento M[$r][$c] che devo azzerare*/
    double t = (M->MCoef)[r][c];
    /*printf("Azzero il coefficiente: M[%d][%d]\n",r,c);*/ /*TAG:DEBUG*/
    factMRAlg(r,c,rowC,t,M); /*TAG:Ralg*/

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

/*TAG:Ralg*/
/*  Funzione che scrive le relazioni tra equazioni nella MRAlg
    IP r, riga in cui siamo
    IP c, colonna in cui siamo
    IP t, pivot
    IOP M, struttura con tutte le informazioni necessarie
*/
void factMRAlg(int r,int c, int rowC, double t, Matrix *M){

    int i;
    double el;

    el = t*((M->MRAlg)[rowC-1][rowC-1]);
    (M->MRAlg)[r-1][rowC-1] = el;

    for(i=0;i<M->nEq;i++){

        /*non deve modificare i dati sulla diagonale*/
        if( (i!=(r-1)) && (i!=(rowC-1)) )
            (M->MRAlg)[r-1][i] = (M->MRAlg)[r-1][i] - el*((M->MRAlg)[rowC-1][i]);

    }/*for*/
    

}/*factMRAlg*/


/*  Funzione che normalizza l'elemento M[$r][$c] sulla diagonale e
    azzera gli altri elmenti tramite operazioni lineari
    IP c, colonna della matrice ideale da modificare
    IP r, riga della matrice ideale da modificare
    IOP M, Matrice da modificare
*/
void zerosCol(int r, int c, Matrix * M){
    
    int i,row;  /*variabili per il ciclo, e per la riga*/
    
    /*normalizzo l'elemento M[$r][$c], in primis e' una diagonale*/
    diagNorm(r,c,M);


    /*Per ogni elemento*/
    for(i=0;i<((M->nEq)-1);i++){

        row = (i+r)%(M->nEq)+1;

        if(!isEqLinDip(row,M))
            zerosRow(row, c, r, M);
        

    }/*for*/

    /*printFMatrix(M);*/  /*TAG:DEBUG*/

}/*zerosCol*/

/*  Funzione che stabilisce se la riga $r e' linearmente dipendente
    alle altre righe della matrice
    IP row, riga da controllorare
    IP M, matrice
    OR se la $row, e' linearmente dipendente alle altre righe
*/
bool isEqLinDip(int row, const Matrix * M){

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
bool isZeroCoefAllEqnLinDip(const Matrix* M){

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
void solveTheMatrix(Matrix *M){

    int i=0,r=0,c=1,j=0, k=0;  /*per il while*/
    int n = M->nEq;

    /*Devo trovare $diag - diagonali - #righe dipendenti*/
    while(i<(n-(M->nEDip))){
        

        /*se l'elemento sulla diagonale M[$r+1][$c]!=0 allora posso continuare*/
        if(!isZero((M->MCoef)[r+1][c], M->error)){
            
            /*normalizzo l'elemento [$r+1][$c] e azzero la colonna $c*/
            zerosCol(r+1,c,M);
            i++;/*lo faccui su almeno tutte le equazioni - # eq lin dip*/
            
            /*TAG: DEBUG*/
            /*printf("\nOperazione su Zero(%d,%d)",r+1,c); */   /*TAG: DEBUG*/
            /*printFMatrixRAlg(M);                         */   /*TAG: DEBUG*/

        }/*if*/

        /*indice di riga a partire da 0*/
        r=(r+1)%(M->nEq);
        /*indice che conta le volte che il ciclo viene eseguito*/   
        j++; /*parte da 0*/

        /*se ho eseguito il ciclo meno volte del numero di incognite*/
        if(j<(M->nIn))
            /*allora l'indice di colonna e' ancora regolato da j*/
            c =j+1;
        else
            /*altrimenti l'indice di colonna e' nel array di eqn lin dip*/
            c = (M->aEDip)[k++]; /*in questo caso lo scandisco con k*/
        

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
void addR(int r, Matrix* M){
    
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
    IP T, matrice non risolta, "Originale"
    IP printTest, se vogliamo stampare a schermo le operazioni compiute dal test
    OR : TRUE se il test e' andato a buon fine (ho trovato la vera relazione)
         FALSE se il test non e' andato a buon fine 
                (La relazione trovata nella matrice MRAlg non e' corretta)   
*/
bool test(Matrix* S, Matrix* T){

    int  d, j, i, iR;
    double molt, elDip, elT, sum;
    if(S->nIn * S->nEq > MAX_STAMPA){
        printf("\nEQUAZIONE TROPPO GRANDE DA STAMPARE\n");
    }
    else{
        /*Stampa dei calcoli fatti*/
        printf("\nORIGINAL");
        printFMatrix(T);
        printf("\nSOLVED");
        printFMatrix(S);
        printf("\nRelazioni algebriche:");
        printFMatrixRAlg(S);
    }
    /*se non ci sono relazioni allora e' automaticamente passato*/
    if(S->nEDip < 1)
        return true;

    /*Stampa di quali righe sono da controllare*/
    printf("Dobbiamo controllare R = ");
    for(i=0;i<(S->nEDip)-1;i++)
        printf("%d, ",S->aEDip[i]);
    printf("%d",S->aEDip[i]);
    printf("\n");


    /*per ogni riga dipendente*/
    for(d=0;d<S->nEDip;d++){

        /*indice della riga che stiamo controllando*/
        iR = (S->aEDip)[d];
        for(i=0;i<S->nIn+1;i++){
            
            /*la mia ipotesi e' che l'elemnto S[$iR][$i] sia la somma di 
            $nEq-1 elementi, alcuni con il fattore moltiplicativo zero*/
            sum = 0;

            for(j=0;j<S->nEq;j++){
                
                /*fattore moltiplicativo che trovo dul*/
                molt = (S->MRAlg)[iR-1][j];

                /*controllo che non siamo sulle diagonali*/
                if(iR!=(j+1) && (!isZero(molt,S->error))){
                    
                    elDip = (T->MCoef)[j+1][i];
                    sum += molt*elDip;

                }/*if*/
            

            }/*for*/

            /*elemento da controllare*/
            elT = (T->MCoef)[iR][i];

            /*controllo che la differenza sia Zero*/
            if(!isZero(elT-sum,S->error))
                return false;

        }/*for*/
        return true;
        
    }/*for*/

    return true;

}/*test*/


/*  Fuznione che stampa a schermo il sistema di equazioni a schermo
    IP Matrix M
    OV il sistema di equazioni
*/
void printEquations(const Matrix *M){

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
