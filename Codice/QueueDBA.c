/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file:
    Questo file contiene la realizzazione delle funzioni per una coda realizzata
    tramite un array.

    Appunti:
    La coda come da consegna viene realizzata con un array. Per diminuire la
    complessita' delle operazioni sulla coda, essa e' dotata di 2 indici, uno per
    il primo elemento e uno per l'ultimo elemento. Questa coda possiede un booleano
    che memorizza se la coda e' stata inizializzata, che e' utile per altre 
    operazioni. Inotre se vengono continuamente aggiunti e tolti elementi puo' 
    succedere che si crei una coda "sproporzionata", cioe' con il primo elemento
    a piu' di RATIO % della capaita', questo e' un grande spreco di memoria, quindi
    quando dovrei fare il resize e mi rendo conto che ho superato tale soglia, riscrivo
    l'array di double, questo evita il resize, che comunque avrebbe dovuto ricopiare
    gli elementi. Inoltre se la coda, dopo n remove e' vuota, la reInizializzo, 
    così prvenire il piu' possibile il fixQueue.

    Complessita' funzioni:
    Init        : O(1)
    Capacity    : O(1)
    Size        : O(1)
    isEmpty     : O(1)
    addLast     : O(1) || O(n) -> quando resize
    getFirst    : O(1)
    removeFirst : O(1) 
    free        : O(1)


*/
#include "QueueDBA.h"   /*bool, struct QueueDBA*/
#include <stdlib.h>     /*malloc, relloc, free*/
#include <string.h>     /*strcpy*/
#include <stdio.h>      /*sprintf*/
#include <assert.h>     /*assert*/


/*  Funzione di inizializzazione della coda
    IOP q, coda da inizializzare
*/
void queueInit(QueueDBA* q){
    /*libero la memoria che nel caso io abbia gia' eseguito l'inizializzazione
        la memoria e' gia' stata allocata*/
    if(q->init)
        free(q->queue);
    q->n = STARTING_G;
    /*allocazione della memoria dinamicamente con controllo se 
        fallisce*/
    q->queue = malloc( q->n * sizeof(double) );
    assert(q->queue != NULL);
    q->indexFirst = 0;
    q->indexLast = 0;
    q->init = true;
}/*queueInit*/

/*  Funzione che restituisce la Capacita' di una coda
    IP q, coda da valutare
    OR intero con la capacita' della coda
*/
int queueCapacity(const QueueDBA* q){
    return q->n;
}/*queueCapacity*/

/*  Funzione che restituisce la Grandezza della coda
    IP q, coda da valutare
    OR intero con la grandezza della coda
*/
int queueSize(const QueueDBA* q){
    /*per la grandezza non serve memorizzare un'altra variabile
        basta fare la differenza tra indici*/
    return q->indexLast - q->indexFirst;
}/*queueSize*/

/*  Fuzione che risponde alla domanda e' vuota ?
    IP q, coda da valutare
    OR risposta booleana, se la coda sia vuota
*/
bool queueIsEmpty(const QueueDBA* q){
    return queueSize(q) == 0;
}/*queueIsEmpty*/

/*  Funzione che aumenta la capacita' dell'array della coda
    IOP q, coda da aumentare la capacita'
*/
void resizeQueue(QueueDBA *q){
    /*Se la coda diventa "sproporzionata" allora la sistemo 
        ed evito di fare il resize, perche' c'e' spazio per un
        altro elemento*/
    if( q->indexFirst > ( (q->n) * RATIO)){
        fixQueue(q);
        return;
    }/*if*/

    /*per il resize conviene utilizzare un fattore moltiplicativo che 
        diminuisce la complessità*/
    q->n = q->n * MOLT_SIZE;
    /*riAllocazione della memoria*/
    q->queue = realloc(q->queue,  (q->n) * sizeof(double) );
    assert(q->queue!=NULL);

}/*resizeQueue*/

/*  Funzione che aggiunge alla coda l'elemento $element
    IOP q, coda dove aggiungere per ultimo $element
    IP element, da aggiungere alla coda
*/
void queueAddLast(QueueDBA* q, double element){

    /*faccio il resize del array quando la capacità e la grandezza sono uguali
        perche' non c'e' spazio per un altro elemento*/
    if((queueSize(q) + q->indexFirst) >= queueCapacity(q))
        resizeQueue(q);

    /*se la coda e' vuota, prima di aggiungere un elemento mi conviene 
        reIniziallizzarla cosi' da risparmiare spazio nel caso in cui ho
        gia' allocato molta memoria la coda e' vuota*/
    if(queueIsEmpty(q) && (q->n != STARTING_G))
        queueInit(q);

    /*aggiunta del elemento e incremento indice*/
    q->queue[(q->indexLast)++] = element;  

}/*queueAddLast*/

/*  Funzione che restituisce il primo elemento della coda
    IP q, coda dove prendere il primo elemento
    IOP elem, primo elemento della coda se non e' vuota
    OR (  0 se l'operazione e' andata a buonfine
         -1 se l'operazione ha fallito, e la coda e' vuota )
*/
int queueGetFirst(const QueueDBA* q, double * elem){
    
    /*Fallisce se la coda e' vuota*/
    if(queueIsEmpty(q))
        return -1;
    /*altrimenti restituisce il primo elmento*/
    *elem = q->queue[q->indexFirst];
    /*senza errori*/
    return 0;
}/*queueGetFirst*/

/*  Funzione che rimuove il primo elemento
    IP q, coda dove prendere e rimuovere il primo elemento
    IOP elem, primo elemento della coda se non e' vuota
    OR (  0 se l'operazione e' andata a buonfine
         -1 se l'operazione ha fallito, e la coda e' vuota )
*/
int queueRemoveFirst(QueueDBA* q, double* elem){

    /*memorizzazione del primo elemento e del errore*/
    int r = queueGetFirst(q, elem);

    /*errore nessun elemento*/
    if(r == -1)
        return -1;

    /*rimozione solo spostando l'indice*/
    q->indexFirst++;

    /*nessun problema*/
    return 0;
}/*queueRemoveFirst*/

/* Funzione che libera la memoria della coda
    IOP q, coda da liberare
*/
void queueFree(QueueDBA* q){
    free(q->queue);
    q->init = false;
    q->indexFirst=0;
    q->indexLast=0;
    q->n = 0;
}/*queueFree*/

/*  Funzione che sistema una coda "sproporzionata" 
        (leggere commento RATIO)
    IOP q, coda da aggiustare
*/
void fixQueue(QueueDBA* q){
    /*Variabile per il ciclo*/
    int size = queueSize(q),
        i,
        f = q->indexFirst;
    /*array che poi sara' la cosa*/
    double *temp;
    
    /*allocazione dinamica*/
    temp = malloc( (queueCapacity(q)) * sizeof(double));
    assert(temp != NULL);
    
    /*copia di tutti gli elementi nel array temporaneo*/
    for(i = 0; i< size; i++)
        temp[i] = (q->queue)[i+f];
    
    /*libero dalla memoria il vecchio array*/
    free(q->queue);
    /*ri assegno le variabili*/
    q->queue = temp;
    q->indexFirst = 0;
    q->indexLast = size;

    
}/*fixQueue*/
