/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file :
    In questo file di tester ci sono tutte le funzioni per la gestione dei file
    e il main per il test della coda.

    Appunti:
    Ho deciso di creare un array di strutturati, i quali hanno un campo puntatore
    a funzione e stringa "nome funzione". Tutto questo per evitare una lunga scala di
    if-else. La funzione per la stampa della coda e' in cima al file per essere richiamata
    dalle altre funzioni

    Makefile: (Qui pero' non ci sono tab ma solo spazi)
    QueueDBA_Tester.exe: QueueDBA.o QueueDBA_Tester.o
	    gcc -o QueueDBA_Tester.exe QueueDBA.o QueueDBA_Tester.o

    QueueDBA.o: QueueDBA.c
	    gcc -ansi -pedantic -Wall -Werror -c QueueDBA.c

    QueueDBA_Tester.o: QueueDBA_Tester.c
	    gcc -ansi -pedantic -Wall -Werror -c QueueDBA_Tester.c

    Comandi letti da file sono :
    IN (= queueInit), 
    CA (= queueCapacity), 
    SI (= queueSize),
    IE (= queueIsEmpty), 
    AL (= queueAddLast), 
    GF (= queueGetFirst), 
    RF (= queueRemoveFirst) , 
    FR (= queueFree).
*/
#include "QueueDBA.h"
#include <stdio.h>
#include <string.h>



/*Definizione di massimo caratteri per il numero che viene inserito con AL*/
#define MAX_CIFRE 15
/*max capacita' stringa della coda*/
#define MAX_ARRAY 150

/*numero di funzioni da controllare*/
#define NUMBER_FUNCTIONS 7
 
/*tipo strutturato che associa ad ogni sigla la funzione da eseguire*/
typedef struct {
    char name[3];                           /*nome della funzione letta*/
    void (*fun)(QueueDBA* q, FILE* outF);   /*funzione da eseguire*/
} funCommand;

/*  Stampa degli elementi nella coda su file
    IP q, coda da stampare
    IOP outF, stream del file dove stampare
    OF $*outF, file dove stampare il contenito della coda
*/
void printQueue(const QueueDBA * q, FILE * outF){
    int i;  /*variabile per il for*/

    /*se la coda non e' inizializzata stampa e' vuota*/
    if(!(q->init)){
        fprintf(outF,"\n");
        return;
    }/*if*/

    /*inizio della stampa,con il tab per essere iu' ordinato*/
    fprintf(outF,"\t[");

    /*se non ha elementi*/
    if(queueIsEmpty(q)){
        /*stampa la chiusura*/
        fprintf(outF,"]\n");
        /*esci da funzione*/
        return;
    }/*if*/

    /*Ha degli elementi:*/
    else {
        /*primo elemento formattato*/
        fprintf(outF,FORMAT_1, q->queue[q->indexFirst]);
        /*ciclo per tutti gli altri elementi*/
        for(i = 1; i<queueSize(q); i++)
            /*stampa di tutti gli elementi*/
            fprintf(outF, V_FORMAT , q->queue[i+q->indexFirst]);
        /*fine dell'array*/
        fprintf(outF," ]\n");
    }/*else*/

}/*printQueue*/

/*******************************************/
/*Funzioni che eseguono i comandi letti : */

/*  Funzione che aggiunge elemento alla fine della coda $*q
    IOP q, coda dove aggiungere elemento
    IP inF, puntatore a file di input
    IF *inF, file dove verra' letto l'elemento
    IOP outF, puntatore a file di output
    OF *outF, file dove verra' stampato l'effetto del comando "AL"
        stampa del elemento, e stampa effetto sulla coda
*/
void al(QueueDBA* q, FILE* inF, FILE* outF ){
    double e;
    /*scansione del double da inserire*/
    fscanf(inF, READ_FORMAT, &e);
    /*inserimento*/
    queueAddLast(q,e);
    /*stampa*/
    fprintf(outF, FORMAT_2, e);
    /*stampa degli effetti sulla coda*/
    printQueue(q,outF);
}/*al*/

/*  Funzione che inizializza la coda
    IOP q, coda da inizializzare
    IOP outF, serve per coerenza array
*/
void init(QueueDBA* q, FILE* outF){

    queueInit(q);

}/*init*/

/*  Funzione che stampa sul file $*outF la capacita' della coda $q 
    IOP q, coda da valutare
    IOP outF, stream file dove stampare la capacita' della coda
    OF *outF stampa della capacita' della coda
*/
void cap(QueueDBA* q, FILE* outF){
        
    fprintf(outF, " %d", queueCapacity(q));
    
}/*cap*/

/*  Funzione che stampa sul file $*outF la grandezza della coda $q
    IOP q, coda da valutare
    IOP outF, stream file dove stampare la grandezza della coda
    OF *outF stampa della grandezza della coda
*/
void si(QueueDBA* q, FILE* outF){
    fprintf(outF, " %d", queueSize(q));
}/*si*/

/*  Funzione che stampa su file $*outF se la coda e' vuota
    IOP q, coda da valutare
    IOP outF, stream file dove stampare
    OF *outF, stampa booleano se coda vuota
*/
void ie(QueueDBA* q, FILE* outF){
    if(queueIsEmpty(q))
        fprintf(outF," true");
    else
        fprintf(outF," false");
}/*ie*/

/*  Funzione che stampa su file il primo elemento di $q
    IOP q, coda da valutare
    IOP outF, stream file dove stampare
    OF *outF stampa del primo elemento
*/
void gf(QueueDBA* q, FILE* outF){
    /*primo elemento della coda*/
    double elem;
    /*chiamata alla funzione e gestione errore*/
    if (queueGetFirst(q, &elem)==-1)
        fprintf(outF, " KO");  /*se non e' possibile stampalo*/
    else
        fprintf(outF, FORMAT_1, elem);
}/*gf*/

/*  Funzione che rimuove il primo elemento da una coda $q e lo stampa
        sullo stream file $*outF
    IOP q, coda da valutare
    IOP outF, stream file dove stampare
    OF *outF stampa del elemento rimosso
*/
void rf(QueueDBA* q, FILE* outF){
    /*primo elemento rimosso*/
    double elem;
    /*invocazione alla funzione e gestione errore*/
    if (queueRemoveFirst(q, &elem)==-1)
        fprintf(outF, " KO");
    else
        fprintf(outF, FORMAT_1, elem);

}/*rf*/

/*  Funzione per liberare la coda
    IOP q, coda da liberare
    IOP outF, serve per coerenza array
*/
void fr(QueueDBA* q, FILE* outF){
    queueFree(q);
}/*fr*/

/*Fine funzioni che eseguono i comandi letti*/
/********************************************/

/*  Funzione che riempie un array tipo strutturato $f
    IOP f, array da di tipo strutturato da riempire
*/
void fillFunCommand(funCommand* f){
    /*dichiarazione degli array inusuale, ma rende il codice piu' leggibile*/
    /*array dizionario dei comandi da eseguire*/
    const char arrDictionary[NUMBER_FUNCTIONS][3] = 
        {"IN", "CA", "SI", "IE", "GF","RF", "FR" };

    /*array con le funzioni da eseguire per ogni comando*/
    void (*commandsArray[NUMBER_FUNCTIONS])(QueueDBA* q, FILE * outF) = 
        {init, cap, si, ie, gf, rf, fr};
    /*variabile per il for*/
    int i;

    /*for che riempie l'array f*/
    for(i = 0; i < NUMBER_FUNCTIONS; i++){
        (f[i]).fun = commandsArray[i];
        strcpy((f[i]).name , arrDictionary[i]);
    }/*for*/

}/*fillFunCommand*/

/*  Funzione che da una stringa $com esegue un comando su una coda $q
    IP com, stringa con il comando da eseguire
    IP al, elemento da inserire se comando e' "AL"
    IP outF, stream file di output, dove stampare i comandi eseguiti
    OF *outFile stampa del comando eseguito e del risultato dopo che il comando
        e' stato eseguito
    IOP q, coda sulla quale eseguire i comandi
    IP f, array di strutturati, con associazione sigla - funzione da eseguire
*/
void doCommand(const char com[], FILE* outF, QueueDBA* q, const funCommand* f){
    
    /*variabile per il for*/
    int i;

    /*controllo del comando letto*/
    for(i= 0; i<NUMBER_FUNCTIONS; i++)
        /*se trova corrispondenza con il nome del comando letto, esegue 
            la funzione*/
        if(!strcmp(com,(f[i]).name)){
            (f[i]).fun(q,outF); /*invocazione alla funzione associata*/
            break;  /*trovata corrispondenza, non devo guardare
                        se trovo altre corrispondenze*/
        }/*if*/
    /*for*/

    /*stampa della coda dopo comandi*/
    printQueue(q, outF);

}/*doCommand*/

/*  Funzione per la lettura dei comandi nei file
    IP nameFileIn, stringa contente il nome del file in input
    IP nameFileOut,stringa contenente il nome del file di output
    OF File $nameFileOut con le istruzione come da consegna
    OR Esito (
         0: elaborazione riuscita;
        -1: apertura fallita di $nameFileIn;
        -2: apertura fallita di $nameFileOut).
*/
int readFileQueue(const char nameFileIn[],const char nameFileOut[], QueueDBA * q){
    char com[3]; /*stringa con i 2 caratteri del comando e '\0'*/
    /*array di strutturati che associa sigla a funzione*/
    funCommand f[NUMBER_FUNCTIONS];
    
    /*Apertura e dello stream dei file di input e output*/
    FILE *inF, *outF;
    inF = fopen(nameFileIn, "r");

    /*Errore di apertura file input*/
    if (inF == NULL)
        return -1;

    outF = fopen(nameFileOut, "w");
    /*Errore apertura file output*/
    if (outF == NULL) {
        fclose(inF);
        return -2;
    } /* if */

    /*chiamata alla funzione che riempie l'array f*/
    fillFunCommand(f);

    /*while per la scansione del file*/
    while (true){ /* Il ciclo e` interrotto quando feof(inF) == true. */

        /*scansione della prima stringa*/
        fscanf(inF,"%s", com);
        
        /*condizione di uscita dal ciclo*/
        if (feof(inF))
            break;

        /*stampa del comando e uno spazio, e poi verra' attuato*/
        fprintf(outF,"%s ", com);

        /*controllo se si deve leggere l'elemento da inserire nella coda
            lo faccio qui al posto che nella funzione doCommand per non passare
            lo stream del file input*/
        /*Aggiunge come ultimo*/
        if(!strcmp(com,"AL"))
            al(q,inF,outF);
        /*if*/    

        else
            /*Chiamata alla funzione che attuera' il comando letto*/
            doCommand(com, outF, q, f);
        /*else*/
        
    }/* while */

    /*chiusura dei file*/
    fclose(inF);
    fclose(outF);
    /*nessun errore*/
    return 0;

}/*readFileQueue*/

/*  Funzione per la stampa di errori nell'apertura di file di input e output
    IP err intero con il possibile errore compiuto dalla funzione
    OV il tipo di errore compiuto dalla funzione
*/
void printErrors(int err){
    /*errore file input*/
    if(err == -1)
        printf("Errore con il file di input");
    /*errore output*/
    else if(err == -2)
        printf("Errore con il file di output");
    /*nessun errore*/
    else
        printf("Operazione completata con successo!\n");

}/*printErrors*/


/*  Main per la lettura di file per il test di una coda
    OR {    0  se la lettura e scrittura del file andata a buon fine
            -1 se l'utente ha dimenticato di insierire i nomi dei file  
    }
*/
int main(int argc, char const *argv[]){
    /*variabile per gli errori possibili lanciati da readFileQueue*/
    int err;
    QueueDBA q;

    /*Se l'utente si e' dimenticato di scrivere i file di IO*/
    if(argc < 3){
        printf("Inserire il nome dei file di Input e Output");
        return -1;  /*ritorno di un itero negativo per simulare un errore*/
    }/*if*/

    /*chiamata alla funzione di lettura dei file e modifica della coda*/
    err = readFileQueue(argv[1], argv[2], &q);

    /*stampa dei possibili errori*/
    printErrors(err);
    
    /*free queue*/
    queueFree(&q);
    
    /*nessun errore*/
    return 0;
}/*main*/
