/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file di tester ci sono tutte le funzioni per la gestione dei file
    e il main per per il risolutore lineare

    Appunti:
    Prende in lettura un file con la matrice e i coefficienti noti
    
    
*/
#include <stdio.h>
#include "GaussJordan.h"

/*  Funzione che stampa su un file la Matrice M (con i termini noti a sinitra)
    IP nameFileOut, stringa contente il nome del file in Output
    IP Matice M, matrice da stampare 
    OF File $nameFileOut con la matrice
    OR Esito: ()
         0: elaborazione riuscita;
        -1: apertura fallita di $nameFileOut).
*/
int printMatrix(const char nameFileOut[], const Matrix *M){

    int i,j,row;        /*varibili per scandire righe e colonne*/
    FILE *outF;     /*Variabile per il file di Output*/

    outF = fopen(nameFileOut, "w");

    /*Errore apertura file output*/
    if (outF == NULL)
        return -1;
    
    /*scritta per la matrice risolta*/
    fprintf(outF,"La matrice risolta A:\n\n");
    
    /*Scansione delle righe*/
    for(i=0;i<M->nEq;i++){
        fprintf(outF,"%2d: ",i+1);
        /*scansione degli elementi colonna*/
        for(j = 0; j<(M->nIn)+1;j++)
            fprintf(outF,"%5.2f ",(M->MCoef)[i+1][j]);

        fprintf(outF,"\n");     /*fine della riga*/
    }/*for*/

    if(M->nEq-M->nIn-M->nEDip == 0){
        
        if(!isZeroCoefAllEqnLinDip(M))
            fprintf(outF,"\nSISTEMA IMPOSSIBILE\n");

        else{
            fprintf(outF,"SISTEMA CON RISULTATO UNICO\n");        
            /*scrittura piu' carina carina del risultato*/
            for(i=0;i<M->nEq;i++){
                for(j=0;j<M->nIn;j++){
                    if(!isZero((M->MCoef)[i+1][j+1],M->error)){
                        fprintf(outF,"x%2d = %5.2f\n",j+1,(M->MCoef)[i+1][0]);
                        break;
                    }/*if*/
                        
                }/*for*/
            
            }/*for*/
        }/*else*/
            
    }/*if*/
    
    /*stampo gli spazi*/
    

    /*stampo la relazioni lineari tra le righe se ci sono*/
    if(M->nEDip > 0){
        fprintf(outF,"\nla relazione tra le righe linearmente indipendenti (inizio a contare da 1):\n\n");

        for(i=0;i<(M->nEDip);i++){

            row=M->aEDip[i];
            fprintf(outF,"Riga %d:",row);

            for(j=0;j<(M->nIn);j++)
                fprintf(outF,"%5.2f ",(M->MRAlg)[row][j]);

            fprintf(outF,"\n");     /*fine della riga*/
        }/*for*/

    }/*if*/
    

    /*chiusura del file*/
    fclose(outF);
    /*nessun problema*/
    return 0;

}/*printMatrix*/


/*  Funzione per la lettura dei comandi nei file
    IP nameFileIn, stringa contente il nome del file in input
    OR Esito (
         0: elaborazione riuscita;
        -1: apertura fallita di $nameFileIn;
*/
int readFileMatrix(const char nameFileIn[], Matrix *M){

    double r;           /* per la lettura*/
    int n;              /*dimensione righe*/
    int m;              /*dimensione colonne*/
    int i,j;            /*iterazione*/

    /*Apertura e dello stream dei file di input e output*/
    FILE *inF;
    inF = fopen(nameFileIn, "r");

    /*Errore di apertura file input*/
    if (inF == NULL)
        return -1;

    /*scansione delle dimensioni della matrice*/
    fscanf(inF,"%d %d", &n, &m);
    /*inizializzazione della matice*/
    initMatrix(n,m,M);

    /*scansione della matrice*/
    for(i=0;i<n;i++){

        for(j = 0; j< m+1 ;j++){

            /*lettura del dato*/
            fscanf(inF,"%lf",&r);

            /*memorizzazzione*/
            (M->MCoef)[i+1][(j+1)%(m+1)] = r;

        }/*for*/

    }/*for*/

    /*salvo l'errore dentro alla matrice*/
    fscanf(inF,"%lf",&r);
    M->error = r;

    /*stampa di cioò che ho letto*/
    printf("LA MATRICE LETTA: \n");
    printFMatrix(M);
    printf("ERRORE LETTO : %f\n", M->error);

    /*chiusura del file*/
    fclose(inF);
    /*nessun errore*/
    return 0;

}/*readFileMatrix*/

/*  Main per la lettura del file della matrice
    OR {    0  se la lettura e scrittura del file andata a buon fine
            -1 se l'utente ha dimenticato di insierire i nomi dei file  
    }
*/
int main(int argc, char const *argv[]){

    Matrix M, T;



    /*Se l'utente si e' dimenticato di scrivere i file di IO*/
    if(argc < 3){
        printf("Inserire il nome dei file di Input e Output");
        return -1;  /*ritorno di un intero negativo per simulare un errore*/
    }/*if*/

    /*Lettura della matrice in input*/
    if(readFileMatrix(argv[1],&M) == -1){
        printf("ERRORE FILE INGRESSO");
        return -1;
    }

    
        /*Lettura della matrice Test*/
        readFileMatrix(argv[1],&T);


    /*risoluzione della matrice*/
    solveTheMatrix(&M);

    /*
    if(test(&M,&T))
        printf("\n TEST PASSATO :)\n");
    else
        printf("\nTEST NON PASSATO !!!!!!!\n");
    */

    /*Stampa su file della matrice risolta*/
    if(printMatrix(argv[2],&M) == -1){
        printf("ERRORE FILE USCITA");
        return -1;
    }
        
    
    /*libero la memoria dalla matrice creata*/
    freeMatrix(&M);
    freeMatrix(&T);
    
    return 0;
}/*main*/