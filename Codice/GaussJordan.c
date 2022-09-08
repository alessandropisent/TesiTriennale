/*  Autore   : Alessandro Pisent
    Matricola: 1162916
    File     : GaussJordan.c
    
    Descrizione file:

*/
#include <stdlib.h>         /*malloc, free*/
#include <assert.h>         /*assert*/
#include <stdio.h>          /*printf*/
#include <math.h>           /*fabs*/
#include <string.h>         /*strcat*/
#include "GaussJordan.h"    /*definizione delle funzioni nel file*/

/*inizializzazione della matrice e allocazione della memoria
    IP n, numero di righe
    IP m, numero di colonne
    IOP A, matrice inizializzata */
void initMatrix(int n, int m, Matrix *M){
    int i;
    int initNDipRow = n-1; /*al massimo le righe indipendenti saranno il # di nEq-1*/
    /*Assegnazioni delle capacita' di righe e colonne*/
    M->nEq = n;   /*Equazioni / Righe*/
    M->nIn = m;   /*Variabili / Colonne*/
    /*Creazione del array di riga*/
    M->MCoef = (double**) malloc( sizeof(double*) *(M->nIn + 1) );
    assert(M->MCoef != NULL);
    /*Creazione di un array per ogni riga*/
    for(i = 0; i<(n+1) ; i++){
        (M->MCoef)[i] = (double*) calloc( m+1 ,sizeof(double));
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
    IOP Matrix M, strutturato con dentro le equazioni */
void oneMatrixRAlg(Matrix *M){
    int i;
    for(i=0;i<M->nEq;i++)
        (M->MRAlg)[i][i]=1;
}/*oneMatrixRAlg*/

/*  Funzione che libera la memoria assegnata alla matrice
    IOP M, matrice da liberare
*/
void freeMatrix(Matrix *M){
    int i;          /*per il for*/
    int n = M->nEq; /*per chiarezza di lettura*/
    /*libera le righe coefficienti*/
    for(i = 0; i < (n+1); i++)
        free((M->MCoef)[(i+1)%(n+1)]);
    /*libera righe relazioni*/
    for ( i = 0; i < n; i++)
        free((M->MRAlg)[i]);
    free(M->MCoef);
    free(M->MRAlg);
    /*libera la matrice con righe dipendenti*/
    free(M->aEDip);
}/*freeMatrix*/

/*  Funzione che stampa una matrice a schermo
    IP M, matrice da stampare
    OV matrice stampata */
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
    OR { 0 nessun errore
         -1 errore di normalizzazzione     }*/
int diagNorm(int r, int c, Matrix *M){
    int i;
    /*prendo il valore sulla diagonale*/
    double t = (M->MCoef)[r][c];
    /*nomalizziamo i valori sulla riga*/
    for(i=0;i<(M->nIn+1);i++)
        (M->MCoef)[r][i]=((M->MCoef)[r][i])/t;
    /*se sono riuscito a normalizzare l'elemento: nessun errore*/
    if(isZero((M->MCoef)[r][c]-1,M->error))
        return 0;
    /*se non ho normalizzato l'elemento:errore*/
    else
        return -1;
}/*diagNorm*/

/*  Funzione che azzera l'elemento M[$r][$c] e modifica di conseguenza la
    riga $r. (prende come elemento di pivot M[$rowC][$c])
    Qui c'e' il controllo se la riga e' linearmente dipendente, nel caso,
    non prova ad modificarla tutta

    IP r, riga da modificare
    IP c, colonna dove ci troviamo (Da azzerare)
    IP rowC, riga che sto prendendo in considerazione 
    IOP M, Matrice da modificare */
void zerosRow(int r, int c, int rowC, Matrix *M){
    int i;
    double el;
    int z=0;
    /*Prendo il valore del elemento M[$r][$c] che devo azzerare*/
    double t = (M->MCoef)[r][c];
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

/*  Funzione che scrive le relazioni tra equazioni nella MRAlg
    IP r, riga in cui siamo 
    IP c, colonna in cui siamo (da azzerar)
    IP rowC, riga in cui la colonna $c e' unitaria
    IOP M, struttura con tutte le informazioni necessarie
*/
void factMRAlg(int r,int c, int rowC, Matrix *M){
    int i;                          /*intero per il for*/
    double el,                      /*# di volte che l'eqn $rowC viene tolta da $r*/
            t = (M->MCoef)[r][c];   /*# di volte che l'eqn $rowC normalizzata viene tolta da $r*/
    /*salvo e memorizzo*/
    el = t*((M->MRAlg)[rowC-1][rowC-1]);
    (M->MRAlg)[r-1][rowC-1] = el;
    /*copia delle operazioni fatte sulla eqn $r */ /*TODO:COMMENTO*/
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
    OR { 0: nessun errore
        -1: errore di normalizazzione      }*/
int zerosCol(int r, int c, Matrix * M){
    int i,row;  /*variabili per il ciclo, e per la riga*/
    /*prendo il valore sulla diagonale*/
    double t = (M->MCoef)[r][c];
    /*Normalizzo l'elemento sulla sua stessa riga*/
    (M->MRAlg)[r-1][r-1]=(M->MRAlg)[r-1][r-1]/t; /*TAG:Ralg*/
    /*normalizzo l'elemento M[$r][$c], in primis e' una diagonale*/
    if(diagNorm(r,c,M)==-1)
        return -1;
    /*Per ogni riga*/
    for(i=0;i<((M->nEq)-1);i++){
        /*indice di riga a partire da 1*/
        row = (i+r)%(M->nEq)+1;
        /*se non e' un'equazione lin dip posso dividere*/
        if(!isEqLinDip(row,M)){
            /*prima mi scrivo le operazioni che faro' sulla matrice $MRAlg*/
            factMRAlg(row, c, r, M); /*TAG:Ralg*/
            /*poi azzero la riga $row, considerando che ho normalizzato 
                l'elemento M[$r][$c]*/
            zerosRow(row, c, r, M);
        }/*if*/
    }/*for*/
    return 0;
}/*zerosCol*/

/*  Funzione che risolve la Matrice M tramite il metodo di Gauss-Jordan
    IOP M, matrice da risolvere
    OR { 0: sono riuscito a risolvere il sistema
        -1: errori di normalizzazzione       }*/
int solveTheMatrix(Matrix *M){
    /*per il while*/
    int i=0,        /*contatore per il numero di incognite risolte*/
        r=1,        /*indice di riga*/
        c=1,        /*indice di colonna*/
        j=0,        /*contatore per il numero di iterazioni del while*/
        k=0,        /*indice per $(M->aEDip)*/
        iCSkip = 0; /*contatore delle colonne saltate*/
    bool isToBeSkippedR = false, /*stato per il ciclo, se la riga e' stata saltata*/
         isToBeSkippedC = false; /*stato per il ciclo, se la colonna e' stata saltata*/ 
    int n = M->nEq; /*per semplificare la lettura*/
    /*conferma che sta iniziando la risoluzione se ci sono tanti dati*/
    if(((M->nIn) * (M->nEq)) > MAX_STAMPA)
        printf("\nRISOLUZIONE DEL SISTEMA\n");
    /*Devo trovare $i =  #equazioni - #righe dipendenti*/
    /*i parte da 0, e deve avere al massimo $n-$nEDip*/
    while(i<(n-(M->nEDip))){
        /*inizializzo gli stati*/
        isToBeSkippedR = false;
        isToBeSkippedC = false;
        /*se le sono su una eqn lin dip dalle altre allora posso saltare la riga $r ma
         non devo far avanzare la colonna $c*/
        if(M->nEDip > 0 &&  isEqLinDip(r,M)){
            isToBeSkippedC = true;
            iCSkip++;
        }/*if*/
        /*se l'elemento sulla diagonale M[$r][$c]!=0 allora posso continuare*/
        else if(!isZero((M->MCoef)[r][c], M->error)){
            /*normalizzo l'elemento [$r+1][$c] e azzero la colonna $c*/
            if(zerosCol(r,c,M)==-1)
                return -1;
            i++; /*lo faccio su almeno tutte le equazioni - # eq lin dip*/
        }/*if*/
        /*altimenti, l'elemento M[$r][$c]==0, ma l'equazione $r non e' lin dip*/
        else
            isToBeSkippedR = true; /*quindi significa che non devo aumentare $r ma provero'
                                l'elemento M[$r][$c+1]*/
        /*indice che conta le volte che il ciclo viene eseguito*/   
        j++; /*parte da 0*/
        /*aumento $r*/
        if(!isToBeSkippedR)
            /*indice per la riga, parte da 1*/
            r=((r)%(M->nEq)) +1;
        /*aumento $c*/
        if(!isToBeSkippedC){
            if((j-iCSkip)<(M->nIn))
                c++;
            else
                c = (M->aEDip)[k++];
        }/*if*/
    }/*while*/
    return 0;
}/*solveTheMatrix*/

/*  Funzione che stabilisce se la riga $r e' linearmente dipendente
    alle altre righe della matrice
    IP row, riga da controllorare
    IP M, matrice
    OR se la $row, e' linearmente dipendente alle altre righe*/
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

/*  Funzione che dice se un double e' zero dato un errore
    IP a, valore da controllare se sia circa zero
    IP precisone
    OR $a==0 */
bool isZero(double a, double precisione){
    return fabs(a)<precisione;
}/*isZero*/

/*  Funzione aggiunge la riga all'array di righe indipendenti
    IP r, riga da aggiungere
    IOP M, matrice con tutto*/
void addR(int r, Matrix* M){
    M->aEDip[M->nEDip] = r;
    M->nEDip +=1;
}/*addR*/

/*  Fuznione che stampa a schermo il sistema di equazioni a schermo
    IP Matrix M
    OV il sistema di equazioni */
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


#ifdef TEST
/*  Funzione per testare che effettivamente la relazione trovata 
    Sia giusta
    IP S, matrice risolta
    IP T, matrice non risolta, "Originale"
    IP printTest, se vogliamo stampare a schermo le operazioni compiute dal test
    OR : TRUE se il test e' andato a buon fine (ho trovato la vera relazione)
         FALSE se il test non e' andato a buon fine 
                (La relazione trovata nella matrice MRAlg non e' corretta)*/
bool test(Matrix* S, Matrix* T){

    int  d, j, i, iR;
    double molt, elDip, elT, sum;

    /*Se troppi elementi per stampare*/
    if(S->nIn * S->nEq > MAX_STAMPA){
        printf("\nEQUAZIONE TROPPO GRANDE DA STAMPARE\n");
    }/*if*/

    /*altrimenti stampa a schermo le matrici*/
    else{
        /*Stampa dei calcoli fatti*/
        printf("\nORIGINAL");
        printFMatrix(T);
        printf("\nSOLVED");
        printFMatrix(S);
        printf("\nRelazioni algebriche:");
        printFMatrixRAlg(S);
    }/*else*/

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


/*  Funzione che controlla che la colonna $col sia composta da un solo
        elemento diverso da zero
    IP col, indice di colonna {1 -> $(M->nIn)}
    IP M, Matrice con i coefficienti
*/
bool isColSolved(int col, const Matrix * M){

    int i,          /*per il for*/ 
        nOne = 0;   /**/
    /*controllo tutte le colonne*/
    for(i=0;i<M->nEq;i++){
        /*se l'elemento sulla colonna !=0*/
        if(!isZero((M->MCoef)[i+1][col],M->error)){
            /*se trovo solo un elemento != 0*/
            if(nOne==0)
                nOne++;
            /*ne ho trovati 2*/
            else
                return false;
        }/*if*/
    }/*for*/
    return true;

}/*isColSolved*/

/*  Funzione che scrive su $aC le colonne di $M non risolte 
        (ossia con un solo elemento != 0 , e tutti gli altri 0)
    IOP aC, array da riempire
    IP M, Matrice con i coefficienti delle equazioni
*/
int whichColAreNotSolved(int* aC, const Matrix * M){
    int i,      /*indice del for*/
        iA=0;   /*indice array*/

    /*per ogni colonna controllo che sia risolta*/
    for(i=0;i< M->nIn; i++){

        /*se non e' risolta la salvo nel array delle non risolte*/
        if(!isColSolved(i+1,M))
            aC[iA++]=i+1;

    }/*for*/

    return iA;
    
}/*whichColAreNotSolved*/

/*  Fprintf Which Coloms are Not Solved
    Funzione che stampa su file quali colonne non sono risole
    IP Matrice da controllare
    OF file con le colonne non risolte
*/
void FprintFCRNS(const Matrix * M){
    
    int * aC; /*array con le colonne*/
    int iA,   /*indice per array*/ 
        i;    /*indice per for*/
    FILE *oF; /*puntatore a file su cui scrivere*/
    
    /*creazione del array delle incognite non risolte*/
    aC = malloc(sizeof(int) * M->nIn);
    assert(aC != NULL);

    /*chiamata alla funzione che riempie l'array di eqn non risolte*/
    iA = whichColAreNotSolved(aC, M);

    /*apertura del file*/
    oF = fopen("notSolvedRows.txt","w");
    if(oF == NULL)
        return;
    
    /*Frontespizio*/
    fprintf(oF,"Righe sbagliate:%d \n",iA);

    /*for che cicla tutte quelle non risolte e le stampa su file*/
    for(i = 0; i<iA; i++)
        fprintf(oF,"-%3d\n",aC[i]);
    
    fclose(oF);     /*chiusura del file*/
    free(aC);       /*eliminazione array*/
}/*FprintFCRNS*/

#endif 