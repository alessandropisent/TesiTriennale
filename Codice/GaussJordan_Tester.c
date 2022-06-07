/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file di tester ci sono tutte le funzioni per la gestione dei file
    e il main per il test del risolutore

    Appunti:
    Prende in lettura un file con la matrice e i coefficienti noti
    
    
*/
#include <stdio.h>
#include "GaussJordan.h"

int printMatrix(const char nameFileOut[], Matrix *M){

    int i,j;
    FILE *outF;

    outF = fopen(nameFileOut, "w");
    /*Errore apertura file output*/
    if (outF == NULL)
        return -2;
    
    for(i=0;i<M->n;i++){

        for(j = 0; j<(M->m)+1;j++){
       
            fprintf(outF,"%5.2f ",(M->A)[i+M->s][j]);

        }/*for*/
        fprintf(outF,"\n");

    }/*for*/

    /*chiusura del file*/
    fclose(outF);
    /*nessun problema*/
    return 0;
}


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
        for(j = 0; j<m;j++){
            /*lettura del dato*/
            fscanf(inF,"%lf",&r);
            /*memorizzazzione*/
            (M->A)[i+(M->s)][j+1] = r;
            /*NOTA: M->s sono le righe che ho lasciato libero, e j+1 perche' memorizzo
                nella colonna 0 i valori noti*/
        }/*for*/

    }/*for*/

    /*scansione dei termini noti*/
    for(i = 0; i<n;i++){
        fscanf(inF,"%lf",&r);
        (M->A)[i+(M->s)][M->b] = r;
    }/*for*/

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

    Matrix M;
    
    /*Se l'utente si e' dimenticato di scrivere i file di IO*/
    if(argc < 3){
        printf("Inserire il nome dei file di Input e Output");
        return -1;  /*ritorno di un intero negativo per simulare un errore*/
    }/*if*/

    if(readFileMatrix(argv[1],&M) == -1){
        printf("ERRORE FILE INGRESSO");
    }/*if*/
    
    if(printMatrix(argv[2],&M) == -1){
        printf("ERRORE FILE USCITA");
    }/*if*/

    /*printFMatrix(&M);*/

    freeMatrix(&M);
    
    return 0;
}/*main*/