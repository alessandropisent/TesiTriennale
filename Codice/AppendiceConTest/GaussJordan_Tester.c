/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file di tester ci sono tutte le funzioni per la gestione dei file
    e il main per per il risolutore lineare
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GaussJordan.h"

#define HELP_STRING "-help"
#define TEST_STRING "-test"
#define MATLAB_FILE "results.txt"

/*  Funzione che stampa su file il sistema di equazioni
    IP nameFileOut, stringa con il nome del file da aprire
    OF outF, file dove stampare
    IP Matrice con il sistema di equazioni*/
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
    /*scritta per la matrice risolta*/
    fprintf(outF,"Il sistema risolto:\n\n");    
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
    OF outF, dove stampare il risultato
    IP Matrice Risolta*/
void fprintSolUnic(const char nameFileOut[], const Matrix *M){
    int i,j;
    FILE *outF;     /*Variabile per il file di Output*/
    #ifdef TEST
        /*Creazione e apertura per il file matlab*/
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
    /*scrittura piu' carina del risultato*/
    for(j=0;j<M->nIn;j++){
        for(i=0;i<M->nEq;i++){
            /*stampa tutti i coefficienti che sono 1*/
            if(isZero((M->MCoef)[i+1][j+1]-1,M->error)){
                fprintf(outF,"x%2d = %f\n",j+1,(M->MCoef)[i+1][0]);
                #ifdef TEST
                    /*stampo solo se richiesto dalle direttive*/
                    fprintf(matlabF,"%f\n",(M->MCoef)[i+1][0]);
                #endif
                /*ho trovato match vai a riga successiva*/
                break;
            }/*if*/  
        }/*for*/
    }/*for*/
    /*chiusura del file*/
    fclose(outF);
    #ifdef TEST
        /*chiusura del file*/
        printf("Scritto file matlab");
        fclose(matlabF);
    #endif
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
        nToPrint = (M->nEq-M->nEDip); /*# di eqn da stampare*/
    double el ; /*variabile per gli elementi da salvaew*/
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "a");
    /*Errore apertura file output*/
    if (outF == NULL){
        printf("ERRORE APERTURA IN PRINT_INDET\n");
        return;
    }/*if*/
    /*frontespizio*/
    fprintf(outF,"\nSISTEMA INDETERMINATO\n");
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
            }/*for*/
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
    double el;
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
    /*frontespizio*/
    fprintf(outF,"\n\nLe relazione tra le equazioni linearmente indipendenti (inizio a contare da 1):\n\n");
    /*stampa delle relazioni*/
    for(i=0;i<(M->nEDip);i++){
        /*indice di riga*/
        row=M->aEDip[i];
        fprintf(outF,"R%d=",row);
        count = 0;
        for(j=0;j<M->nEq;j++){
            el = (M->MRAlg)[row-1][j];
            /*stampo solo le relazioni con le altre colonne e diverse da 0*/
            if((j!=(row-1)) && !isZero(el,M->error)){
                if(!count){
                    fprintf(outF,"%5.2f * Eqn%d ",el,j+1);
                    count++;
                }/*if*/
                else
                    if(el>0)
                        fprintf(outF,"+ %5.2f * Eqn%d ",el,j+1); 
                    else
                        fprintf(outF," %5.2f * Eqn%d ",el,j+1);   
            }/*if*/
        }/*for*/
        fprintf(outF,"\n");     /*fine della riga*/
    }/*for*/
    fclose(outF);
}/*fprintRel*/

/*  Funzione che in base al errore stampa un tipo di aiuto
    IP code : (0,1) = (basic,error file input)
    OV aiuto */
void printHelp(int code){
    /*se il codice e' zero significa che devo stampare 
    l'inizio del programma*/
    if(code==0){
        printf("usage: GaussJordan_Tester.exe FileInput.txt FileOutput.txt ");
        
        /*se test viene anche spiegata l'opzione di test*/
        #ifdef TEST
            printf("<%s>\n\n",TEST_STRING);
            printf("\"%s\" e' opzionale: serve per confermare che la relazione tra le righe sia corretta",TEST_STRING);
        #endif
        printf("\n\nil file di input va formattato come :\n");
        printf("n m\n");
        printf("error\n\n");
        printf("b1 a11 a12 a13\n");
        printf("b2 a21 a22 a23\n");
        printf("b3 a31 a32 a33\n");
        printf("dove:\n- n = # di equazioni\n");
        printf("- error= precisione che consideriamo accettabile per confrontare due numeri\n\n");
        printf("- m = # di incognite\n");
        printf("- b# = termine noto b#\n");
        printf("- a## = coefficiente della equazione\n");
        printf("Regole aggiuntive:\n");
        printf("- e' possibile non inserire tutti i coefficienti nell'equazione, verranno considerati nulli\n");
        printf("- e' necessario inserire almeno 2 numeri in una riga (cioe' del termine noto e il coefficiente di x1)\n");
        printf("\n");
    }/*if*/
    /*errore base*/
    else if(code==1){
        printf("Inserire il nome dei file di Input e Output\n");
        printf("Per aiuto \"%s\"\n",HELP_STRING);
        #ifdef TEST
            printf("opzione di test: %s\n",TEST_STRING);
        #endif
    }/*else if*/   
}/*printHelp*/
/*  Funzione che stampa su un file la Matrice M (con i termini noti a sinitra)
    IP nameFileOut, stringa contente il nome del file in Output
    IP Matice M, matrice da stampare 
    OF File $nameFileOut con la matrice
    OR Esito: (
         0: elaborazione riuscita;
        -1: apertura fallita di $nameFileOut).*/
int printFileMatrix(const char nameFileOut[], const Matrix *M){
    FILE *outF;     /*Variabile per il file di Output*/
    outF = fopen(nameFileOut, "w");
    /*Errore apertura file output*/
    if (outF == NULL)
        return -1;
    /*se ha un risultato del tipo 0*x1 = a, il sistema e' impossibile*/
    if(!isZeroCoefAllEqnLinDip(M)){
        fprintf(outF,"\nSISTEMA IMPOSSIBILE\n");
        /*chiusura del file*/
        fclose(outF);
        return -2;
    }/*if*/
    /*chiusura del file*/
    fclose(outF);
    #ifdef TEST
        /*stampa le equazioni su file*/
        fprintEquazioni(nameFileOut,M);
    #endif
    /*Se # di equazioni - # di incognite == # di equazioni linarmente 
    dipendenti il sistema e' con una soluzione unica*/
    if(M->nEq - M->nIn - M->nEDip == 0)
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
        -2: errore di inserimento dati
*/
int readFileMatrix(const char nameFileIn[], Matrix *M){
    double r;               /* per la lettura*/
    int n;                  /*dimensione righe*/
    int m;                  /*dimensione colonne*/
    int err;
    int i=0,            
        j=0;              /*icontatore elementi*/
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
    /*salvo l'errore dentro alla matrice*/
    fscanf(inF,"%lf",&r);
    M->error = r;
    /*scansione della matrice*/
    for(i=0;i<n;i++){
        for(j = 0; j< m+1 ;j++){
            /*lettura del dato*/
            err=fscanf(inF,"%lf",&r);
            /*memorizzazzione*/
            (M->MCoef)[i+1][j] = r;
        }/*for*/
    }/*for*/
    if(err==-1)
        return -2;
    /*stampa di cioò che ho letto, se il numero di elementi non e' troppo grande*/
    if((n*m)<MAX_STAMPA_EQN)
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
    }*/
int main(int argc, char const *argv[]){
    Matrix M;
    int err;
    #ifdef TEST
        Matrix T;
        bool doTest=false;  /*variabile booleana che dice se ho effetuato il test*/
    #endif
    if((argc>1) && !(strcmp(argv[1],HELP_STRING))){
        printHelp(0);
        return 0;
    }/*if*/   
    /*Se l'utente si e' dimenticato di scrivere i file di IO*/
    else if((argc < 3) && (argc > 4)){
        printHelp(1);
        return -1;  /*ritorno di un intero negativo per simulare un errore*/
    }/*else if*/
    /*codice errore*/
    err = readFileMatrix(argv[1],&M);
    /*Lettura della matrice in input*/
    if(err == -1){
        printf("ERRORE FILE INGRESSO\n");
        return -1;
    }/*if*/
    else if (err == -2){
        printf("ERRORE DI INSERIMENTO DATI\n");
        return -1;
    }/*if*/
    /*risoluzione della Ma trice*/
    if(solveTheMatrix(&M)==-1){
        printf("Errori di normalizzazzione\nSistema non risolto\n");
        return -1;
    }/*if*/
    else
        /*stampa a schermo che il sistema e' risolto*/
        printf("\nSistema risolto\n");
    #ifdef TEST
        /*stampa su file delle colonne non risolte*/
        FprintFCRNS(&M);
        /*se viene aggiunto alla fine la stringa per testare le relazioni*/
        if((argc==4) && !strcmp(argv[3],TEST_STRING)){
            /*faccio il test se ce' qualche eqn lin dip*/
            if(M.nEDip!=0){  
                /*rileggo il file con il sistema originale, e riempio T*/
                readFileMatrix(argv[1],&T);
                /*variabile che mi dice che ho riempito T*/
                doTest = true;
                /*se il test e' passato stampo*/
                if(test(&M,&T))
                    printf("\n!TEST PASSATO \n");
                /*se il test non e' passato:*/
                else
                    printf("\nTEST NON PASSATO !!!!!!!\n");
            }/*if*/
            /*altrimenti non avvio nemmeno il test*/
            else
                printf("\nNessuna EQN LIN DIP\n");
        }/*if*/
    #endif
    /*Stampa su file della matrice risolta*/
    if(printFileMatrix(argv[2],&M) == -1){
        printf("ERRORE FILE USCITA");
        return -1;
    }/*if*/
    /*se il sistema e' di grandi dimensioni stampo a video la conferma di scrittura*/
    if((M.nEq *  M.nIn) > MAX_STAMPA)
        printf("FILE SCRITTO\n");
    /*libero la memoria dalla matrice creata*/
    freeMatrix(&M);
    #ifdef TEST
        if(doTest)
            freeMatrix(&T);
    #endif
    return 0;
}/*main*/