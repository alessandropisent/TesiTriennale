/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file di tester ci sono tutte le funzioni per la gestione dei file
    e il main per per il risolutore lineare

    Appunti:
    Prende in lettura un file con la matrice e i coefficienti noti
    
    
*/
#include <stdio.h>
#include <string.h>
#include "GaussJordan.h"

#define HELPSTRING "-help"
#define TEST_STRING "-test"
#define NO_ALG_REL_STRING "-no-rel"
#define MATLAB_FILE "results.txt"
#define WRITE_FILE_MATLAB true

/*  Funzione che stampa su file il sistema di equazioni
    IP
    OF outF, file dove stampare
    IP Matrice con il sistema di equazioni
*/
void fprintEquazioni(const char nameFileOut[],const Matrix *M){

    int i,j;

        
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL)
        return;

    /*Scansione delle righe*/
    for(i = 0; i < M->nEq; i++){
        /*stampa delle equazioni*/
        for(j=0;j<M->nIn-1;j++)
            fprintf(outF,"%5.2f * x%d + ",(M->MCoef)[i+1][j+1],j+1);

        
        /*stampa l'ultimo elemento*/
        fprintf(outF,"%5.2f * x%d ",(M->MCoef)[i+1][j+1],j+1);

        /*stampa termine noto*/
        fprintf(outF," = %5.2f\n",(M->MCoef)[i+1][0]);
    }/*for*/

    fclose(outF);
}/*fprintEquazioni*/

/*  Funzione che stampa la soluzione quando il sistema ha un unica soluzione
    IP nome del file
    OF outF, File precedentemente aperto, dove stampare il risultato
    IP Matrice Risolta*/
void fprintSolUnic(const char nameFileOut[], const Matrix *M){
    int i,j;
    FILE *outF;     /*Variabile per il file di Output*/

    /*Creazione e apertura per il file matlab*/
    #ifdef WRITE_FILE_MATLAB
        FILE *matlabF;
        matlabF = fopen(MATLAB_FILE,"w");
        /*Errore apertura file output*/
        if (matlabF == NULL){
            printf("ERRORE APERTURA IN PRINT_SOL_UNICA(MatlabFile)\n");
            return;
        }/*if*/
    #endif

    outF = fopen(nameFileOut, "a"); /*append*/
    /*Errore apertura file output*/
    if (outF == NULL){
        printf("ERRORE APERTURA IN PRINT_SOL_UNICA\n");
        return;
    }/*if*/

    /*scrittura frontespizio*/
    fprintf(outF,"SISTEMA CON RISULTATO UNICO\n"); 

    /*scrittura piu' carina carina del risultato*/
    for(j=0;j<M->nIn;j++){
        for(i=0;i<M->nEq;i++){
            
            /*stampa tutti i coefficienti che sono 1*/
            if(isZero((M->MCoef)[i+1][j+1]-1,M->error)){
                fprintf(outF,"x%2d = %5.2f\n",j+1,(M->MCoef)[i+1][0]);

                /*stampo solo se richiesto dalle direttive*/
                #ifdef WRITE_FILE_MATLAB
                    fprintf(matlabF,"%f\n",(M->MCoef)[i+1][0]);
                #endif

                /*ho trovato match vai a riga successiva*/
                break;
            }/*if*/
                
        }/*for*/
    
    }/*for*/
    /*chiusura del file*/
    fclose(outF);

    /*chiusura del file*/
    #ifdef WRITE_FILE_MATLAB
        fclose(matlabF);
    #endif

}/*fprintSolUnic*/

/*  Funzione che stampa su file l'insieme di soluzioni per un sistema 
    indeterminato
    IOF outF, file di input aperto in precedenza, dove stampare
    IP Matrix M, matrice  con i coefficienti delle equazioni lineari*/
void fprintIndet(const char nameFileOut[], const Matrix *M){
    int i,j,k;
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL)
        return ;
    /*frontespizio*/
    fprintf(outF,"\nSISTEMA INDETERMINATO\n");

    /*scrittura piu' carina carina del risultato*/
    for(i=0;i<M->nEq;i++){
        for(j=0;j<M->nIn;j++){
            if(!isZero((M->MCoef)[i+1][j+1],M->error)){
                fprintf(outF,"x%2d = %5.2f ",j+1,(M->MCoef)[i+1][0]);
                for(k=M->nEq+1 ;k<M->nIn + M->nEDip;k++){
                    if((M->MCoef)[i+1][k]>0)
                        fprintf(outF,"- %5.2f * x%d ",(M->MCoef)[i+1][k],k);
                    else if((M->MCoef)[i+1][k]<0)
                        fprintf(outF,"+ %5.2f * x%d ",-1 * (M->MCoef)[i+1][k],k);
                }

                if((M->MCoef)[i+1][k]>0)
                        fprintf(outF,"-%5.2f * x%d\n",(M->MCoef)[i+1][k],k);
                else if((M->MCoef)[i+1][k]<0)
                    fprintf(outF,"%5.2f * x%d\n",-1 * (M->MCoef)[i+1][k],k);

                break;
            }/*if*/
                
        }/*for*/
    
    }/*for*/

    fclose(outF);

}/*fprintIndet*/

/*  Funzione che stampa le relazioni tra le righe linearmente dipendenti
    IOF File, precedentemente aperto, dove stampare le relazioni
    IP struttura con dentro le informazioni delle relazioni tra le Equzioni*/
void fprintRel(const char nameFileOut[], const Matrix *M){

    int i,j,row,count=0;
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL)
        return ;

    fprintf(outF,"\nla relazione tra le equazioni linearmente indipendenti (inizio a contare da 1):\n\n");

    for(i=0;i<(M->nEDip);i++){

        row=M->aEDip[i];
        fprintf(outF,"R%d=",row);

        for(j=0;j<M->nEq;j++){

            /*stampo solo le relazioni con le altre colonne e diverse da 0*/
            if((j!=row) && !isZero((M->MRAlg)[row][j],M->error)){
                
                if(!count){
                    fprintf(outF,"%5.2f * R%d ",(M->MRAlg)[row][j],j);
                    count++;
                }/*if*/

                else
                    fprintf(outF,"+ %5.2f * R%d ",(M->MRAlg)[row][j],j);
                    
            }/*if*/
                

        }/*for*/

        fprintf(outF,"\n");     /*fine della riga*/
    }/*for*/
    fclose(outF);

}/*fprintRel*/

/*  Funzione che in base al errore stampa un tipo di aiuto
    IP code : (0,1) = (basic,error file input)
    OV aiuto
*/
void printHelp(int code){
    
    /*se il codice e' zero significa che devo stampare 
    l'inizio del programma*/
    if(code==0){
        printf("usage: GaussJordan_Tester.exe FileInput.txt FileOutput.txt\n\n");
        printf("il file di input va formattato come :\n");
        printf("n m\n");
        printf("b1 a11 a12 a13\n");
        printf("b2 a21 a22 a23\n");
        printf("b3 a31 a32 a33\n");
        printf("error\n\n");
        printf("dove:\n- n = # di equazioni\n");
        printf("- m = # di incognite\n");
        printf("- b# = termine noto b#\n");
        printf("- a## = coefficiente della equazione\n");
        printf("- error= precisione che consideriamo accettabile per confrontare due numeri\n\n");
        printf("Regole aggiuntive:\n");
        printf("- e' possibile non inserire tutti i coefficienti nell'equazione, verranno considerati nulli\n");
        printf("- e' necessario inserire almeno 2 numeri in una riga (cioe' del termine noto e il coefficiente di x1)\n");
        printf("\n");
    }/*if*/

}/*printHelp*/

/*  Funzione che stampa su un file la Matrice M (con i termini noti a sinitra)
    IP nameFileOut, stringa contente il nome del file in Output
    IP Matice M, matrice da stampare 
    OF File $nameFileOut con la matrice
    OR Esito: ()
         0: elaborazione riuscita;
        -1: apertura fallita di $nameFileOut).
*/
int printMatrix(const char nameFileOut[], const Matrix *M){
    int i=0,n=M->nEq,m=M->nIn;
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "w");
    /*Errore apertura file output*/
    if (outF == NULL)
        return -1;
    
    if(!isZeroCoefAllEqnLinDip(M)){
        fprintf(outF,"\nSISTEMA IMPOSSIBILE\n");
        return -2;
    }
    else    
        /*scritta per la matrice risolta*/
        fprintf(outF,"Il sistema risolto:\n\n");
    fclose(outF);
    
    if(m*n < MAX_STAMPA)
        /*stampa le equazioni su file*/
        fprintEquazioni(nameFileOut,M);

    /*se ha un risultato del tipo 0*x1 = 1, il sistema e' impossibile*/


    /*Se # di equazioni - # di incognite == # di equazioni linarmente 
    dipendenti il sistema e' con una soluzione unica*/
    else if(M->nEq-M->nIn-M->nEDip == 0)
        fprintSolUnic(nameFileOut,M);

    /*altrimenti, posso scrivere delle relazioni tra le colonne*/
    else
        fprintIndet(nameFileOut, M);


    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL)
        return -1;

    /*stampo la relazioni lineari tra le righe se ci sono*/
    if(M->nEDip > 0){
        /*considerazione per il singolare e plurale*/
        if(M->nEDip==1)
            /*frontespizio*/
            fprintf(outF,"\nIl sistema ha una equazione linearmente dipendente: ");
        else
             /*frontespizio*/
            fprintf(outF,"\nIl sistema ha %d equazioni linearmente dipendenti: ",M->nEDip);
        fprintf(outF,"(");
        for(i=0;i<M->nEDip;i++)
            fprintf(outF," Eq%d", (M->aEDip)[i]);
        fprintf(outF," )");

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

    double r;               /* per la lettura*/
    int n;                  /*dimensione righe*/
    int m;                  /*dimensione colonne*/
    int i,j;                /*iterazione*/


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
            (M->MCoef)[i+1][j] = r;

        }/*for*/

    }/*for*/

    /*salvo l'errore dentro alla matrice*/
    fscanf(inF,"%lf",&r);
    M->error = r;

    /*stampa di cioò che ho letto*/
    if((m*n) < MAX_STAMPA)
        printEquations(M);
    else
        printf("Sistema di equazioni di %d equazioni x %d incognite, troppo grande da stampare", n,m);
    printf("\nERRORE LETTO : %f\n", M->error);

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

    if((argc>1) && !(strcmp(argv[1],HELPSTRING))){
        printHelp(0);
        return 0;
    }/*if*/
        

    /*Se l'utente si e' dimenticato di scrivere i file di IO*/
    else if(argc < 3){
        printf("Inserire il nome dei file di Input e Output");
        return -1;  /*ritorno di un intero negativo per simulare un errore*/
    }/*else if*/

    /*Lettura della matrice in input*/
    if(readFileMatrix(argv[1],&M) == -1){
        printf("ERRORE FILE INGRESSO");
        return -1;
    }/*if*/


    /*risoluzione della matrice*/
    solveTheMatrix(&M);


    /*Stampa su file della matrice risolta*/
    if(printMatrix(argv[2],&M) == -1){
        printf("ERRORE FILE USCITA");
        return -1;
    }
        
    
    /*libero la memoria dalla matrice creata*/
    freeMatrix(&M);
    
    return 0;
}/*main*/