/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file di tester ci sono tutte le funzioni per la gestione dei file
    e il main per per il risolutore lineare

    Appunti:
    Prende in lettura un file con la matrice e i coefficienti noti
    
    
    Albero delle chiamate: 
    "*-" = chiamata condizionata
    "#-" = chiamata ripetuta # volte
    "f#-" = chiamata a funzione in altro file
        - f1 = "GaussJordan.c"

    main:
        *- printHelp
        - readFileMatrix
            f1- initMatrix
            f1- printEquations
        f1- solveTheMatrix
        - printMatrix
            - fprintEquazioni
            *- fprintSolUnic
            *- fprintIndet
            *- fprintRel    
        f1- freeMatrix

*/
#include <stdio.h>
#include <string.h>

#include "GaussJordan.h"

#define HELP_STRING "-help"
#define TEST_STRING "-test"
#define NO_ALG_REL_STRING "-no-rel"

/*  Funzione che stampa su file il sistema di equazioni
    IP 
    OF outF, file dove stampare
    IP Matrice con il sistema di equazioni
*/
void fprintEquazioni(const char nameFileOut[],const Matrix *M){
    
    FILE *outF;     /*Variabile per il file di Output*/
    int i,j;

    /*apertura del file*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL){
        printf("ERRORE APERTURA IN PRINT_EQN\n");
        return;
    }/*if*/
        

    /*Scansione delle righe*/
    for(i = 0; i < M->nEq; i++){
        fprintf(outF,"%3d:",i+1);
        /*stampa delle equazioni*/
        for(j=0;j<M->nIn-1;j++)
            fprintf(outF,"%5.2f * x%d + ",(M->MCoef)[i+1][j+1],j+1);

        
        /*stampa l'ultimo elemento*/
        fprintf(outF,"%5.2f * x%d ",(M->MCoef)[i+1][j+1],j+1);

        /*stampa termine noto*/
        fprintf(outF," = %5.2f\n",(M->MCoef)[i+1][0]);
    }/*for*/

    /*chiusura del file*/
    fclose(outF);
}/*fprintEquazioni*/

/*  Funzione che stampa la soluzione quando il sistema ha un unica soluzione
    IP nome del file
    OF outF, File precedentemente aperto, dove stampare il risultato
    IP Matrice Risolta*/
void fprintSolUnic(const char nameFileOut[], const Matrix *M){
    int i,j;
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a"); /*append*/
    /*Errore apertura file output*/
    if (outF == NULL){
        printf("ERRORE APERTURA IN PRINT_SOL_UNICA\n");
        return;
    }/*if*/

    fprintf(outF,"SISTEMA CON RISULTATO UNICO\n"); 

    /*scrittura piu' carina carina del risultato*/
    for(i=0;i<M->nEq;i++){
        for(j=0;j<M->nIn;j++){
            if(isZero((M->MCoef)[i+1][j+1]-1,M->error)){
                fprintf(outF,"x%2d = %5.2f\n",j+1,(M->MCoef)[i+1][0]);
                break;
            }/*if*/
                
        }/*for*/
    
    }/*for*/
    /*chiusura del file*/
    fclose(outF);

}/*fprintSolUnic*/

/*  Funzione che stampa su file l'insieme di soluzioni per un sistema 
    indeterminato
    IP nome del file da aprire
    OF outF, file di input aperto in precedenza, dove stampare
    IP Matrix M, matrice  con i coefficienti delle equazioni lineari*/
void fprintIndet(const char nameFileOut[], const Matrix *M){

    int i=0,    /*indice di riga*/
        j=0,    /*indice di colonna*/  
        k,      /*indice per scandire tutti gli elementi di una riga*/    
        nP=0,   /*incide per contare quante righe ho stampato*/
        nToPrint = (M->nEq-M->nEDip); 
    double el ;
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL){
        printf("ERRORE APERTURA IN PRINT_INDET\n");
        return;
    }/*if*/

    /*frontespizio*/
    fprintf(outF,"\nSISTEMA INDETERMINATO\n");

    /*scrittura piu' carina carina del risultato*/
    /*devo stampare esattamente $nToPrint volte, la seconda condizione 
        e' per uscire nel caso di while infinito*/
    while((nP<nToPrint)&&(i<M->nEq)){
        
        /*ho trovato un 1, cioe' una incognita che sono riusicito a normalizzare*/
        if(isZero((M->MCoef)[i+1][j+1]-1,M->error)){

            /*stampa dell'inizio con il termine noto*/
            fprintf(outF,"x%2d = %5.2f ",j+1,(M->MCoef)[i+1][0]);

            /*stampa dei coefficienti delle incognite libere*/
            for(k=0 ;k<M->nIn;k++){

                /*mi salvo l'elemento*/
                el = (M->MCoef)[i+1][k+1];

                /*se ha senso stampare il coefficiente*/
                if(!isZero(el,M->error) && (k!=j)){
                    
                    /*stampa a seconda se il valore sia positivo o negativo*/
                    /*cambio di segno perche' e' come se avessi portato al di la' 
                        del uguale*/
                    if(el>0)
                        fprintf(outF,"- %5.2f * x%d ",el, k+1);
                    else if(el <0)
                        fprintf(outF,"+ %5.2f * x%d ",-1 * el, k+1);

                
                }/*if*/
                

            }
            /*finita la riga*/
            fprintf(outF,"\n");

            nP++; /*ho stampato una riga, quindi aggiorno il contatore*/
            j=0;  /*ricomincio alla prima incognita*/  
            i++;  /*vado alla riga successiva*/

        }/*if*/

        /*continuo con il ciclo*/
        else if(j<M->nIn)
            j++; /*provo con la colonna dopo*/

        /*no match sulla riga*/
        else if(j==M->nIn){
            /*potrebbe essere una riga con tutti zeri*/
            j=0; /*ricomincio della prima colonna*/
            i++; /*vado alla riga successiva*/
        }/*else if*/

    
    }/*while*/
    
    /*chiusura del file*/
    fclose(outF);

}/*fprintIndet*/

/*  Funzione che stampa le relazioni tra le righe linearmente dipendenti
    IP nome del file
    OF File, precedentemente aperto, dove stampare le relazioni
    IP struttura con dentro le informazioni delle relazioni tra le Equzioni*/
void fprintRel(const char nameFileOut[], const Matrix *M){

    int i,j,row,count=0;
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL){
        printf("ERRORE APERTURA IN PRINT_REL\n");
        return;
    }/*if*/

    /*considerazione per il singolare e plurale*/
    if(M->nEDip==1)
        /*frontespizio*/
        fprintf(outF,"\nIl sistema ha una equazione linearmente dipendente: ");
    else
        /*frontespizio*/
        fprintf(outF,"\nIl sistema ha %d equazioni linearmente dipendenti: ",M->nEDip);
    
    /*stampa delle eqn lin dip*/
    fprintf(outF,"(");
    for(i=0;i<M->nEDip;i++)
        fprintf(outF," Eq%d", (M->aEDip)[i]);
    fprintf(outF," )");

    fprintf(outF,"\n\nLe relazione tra le equazioni linearmente indipendenti (inizio a contare da 1):\n\n");
    
    /*stampa delle relazioni*/
    for(i=0;i<(M->nEDip);i++){

        /*indice di riga*/
        row=M->aEDip[i];
        fprintf(outF,"R%d=",row);

        for(j=0;j<M->nEq;j++){

            /*stampo solo le relazioni con le altre colonne e diverse da 0*/
            if((j!=(row-1)) && !isZero((M->MRAlg)[row-1][j],M->error)){
                
                if(!count){
                    fprintf(outF,"%5.2f * R%d ",(M->MRAlg)[row-1][j],j+1);
                    count++;
                }/*if*/

                else
                    fprintf(outF,"+ %5.2f * R%d ",(M->MRAlg)[row-1][j],j+1);
                    
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
        printf("usage: GaussJordan_Tester.exe FileInput.txt FileOutput.txt <%s|%s>\n\n",TEST_STRING,NO_ALG_REL_STRING);
        printf("\"%s\" e' opzionale: serve per confermare che la relazione tra le righe sia corretta\n",TEST_STRING);
        printf("\"%s\" e' opzionale: serve per non cercare le relazioni algebriche tra le equazioni\n",NO_ALG_REL_STRING);
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

    else if(code==1){
        printf("Inserire il nome dei file di Input e Output\n");
        printf("Per aiuto \"%s\"\n",HELP_STRING);
        printf("opzione di test: %s\n",TEST_STRING);
        printf("opzione per non avere le relazioni tra le equazioni: \"%s\"\n",NO_ALG_REL_STRING);
    }/*else if*/   
    

}/*printHelp*/

/*  Funzione che stampa su un file la Matrice M (con i termini noti a sinitra)
    IP nameFileOut, stringa contente il nome del file in Output
    IP Matice M, matrice da stampare 
    OF File $nameFileOut con la matrice
    OR Esito: ()
         0: elaborazione riuscita;
        -1: apertura fallita di $nameFileOut).
*/
int printFileMatrix(const char nameFileOut[], const Matrix *M){

    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "w");
    /*Errore apertura file output*/
    if (outF == NULL)
        return -1;
    
    if(!isZeroCoefAllEqnLinDip(M)){
        fprintf(outF,"\nSISTEMA IMPOSSIBILE\n");
        /*chiusura del file*/
        fclose(outF);
        return -2;
    }/*if*/
    else
        /*scritta per la matrice risolta*/
        fprintf(outF,"Il sistema risolto:\n\n");

    /*chiusura del file*/
    fclose(outF);

    if(M->nIn * M->nEq < MAX_STAMPA)
        /*stampa le equazioni su file*/
        fprintEquazioni(nameFileOut,M);

    /*se ha un risultato del tipo 0*x1 = a, il sistema e' impossibile*/


    /*Se # di equazioni - # di incognite == # di equazioni linarmente 
    dipendenti il sistema e' con una soluzione unica*/
    if(M->nEq-M->nIn-M->nEDip == 0)
        fprintSolUnic(nameFileOut,M);

    /*altrimenti, posso scrivere delle relazioni tra le colonne*/
    else
        fprintIndet(nameFileOut, M);


    /*stampo la relazioni lineari tra le righe se ci sono*/
    if(M->nEDip > 0)
        /*stampa sul file le relazioni tra le equazioni*/
        fprintRel(nameFileOut, M);

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

    /*stampa di cioò che ho letto, se il numero di elementi non e' troppo grande*/
    if((n*m)<100)
        printEquations(M);
    else
        printf("Sistema di %d Equazioni x %d incognite, troppo grande da stampare\n", n,m);
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

    Matrix M, T;
    if((argc>1) && !(strcmp(argv[1],HELP_STRING))){
        printHelp(0);
        return 0;
    }/*if*/
        

    /*Se l'utente si e' dimenticato di scrivere i file di IO*/
    else if((argc < 3) && (argc > 4)){
        printHelp(1);
        return -1;  /*ritorno di un intero negativo per simulare un errore*/
    }/*else if*/

    /*Lettura della matrice in input*/
    if(readFileMatrix(argv[1],&M) == -1){
        printf("ERRORE FILE INGRESSO");
        return -1;
    }/*if*/
    
    /*risoluzione della matrice*/
    /*o senza relazioni algebriche*/
    if((argc==4) && !strcmp(argv[3],NO_ALG_REL_STRING)){
        printf("NON CERCO LE RELAZIONI TRA LE EQUAZIONI\n");
        /*risoluzione della matrice*/
        solveTheMatrix(&M,0);
    }/*if*/
    /*o con le relazioni algebriche*/
    else {
        solveTheMatrix(&M,1);
    }/*else*/
    printf("\nSolved\n");

    /*se viene aggiunto alla fine la stringa per testare le relazioni*/
    if((argc==4) && !strcmp(argv[3],TEST_STRING)){

        readFileMatrix(argv[1],&T);

        if(test(&M,&T))
            printf("\n TEST PASSATO \n");
        else
            printf("\nTEST NON PASSATO !!!!!!!\n");

    }/*if*/

    /*Stampa su file della matrice risolta*/
    if(printFileMatrix(argv[2],&M) == -1){
        printf("ERRORE FILE USCITA");
        return -1;
    }/*if*/

    if(M.nEq *  M.nIn > MAX_STAMPA)
        printf("FILE SCRITTO\n");
        
    
    /*libero la memoria dalla matrice creata*/
    freeMatrix(&M);
    freeMatrix(&T);
    
    return 0;
}/*main*/