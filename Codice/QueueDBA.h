/*  Autore   : Alessandro Pisent
    Matricola: 1162916

    Descrizione file: 
    Questo file contiene la struttura della coda, la formattazzione 
    nella quale vengono letti e scritti i file, la dimensione iniziale
    del array per la coda, e il fattore moltiplicativo. Contiene anche
    il RATIO peril quale avviene il fixQueue, e il prototipo per le 
    funzioni di queue.
*/
#include <stdbool.h>    /*bool*/
#include <stdio.h>

/*Con un allocazione di memoria inizialmente abbastanza
    grande posso sperare di non dover riallocare la memoria, ma da
    testo del problema all'inizio la grandezza deve essere 1*/
#define STARTING_G 1
/*Moltiplicando la dimensione ogni volta che si rialloca la memoria
    faccio meno riallocazioni rispetto ad aggiungere 1 elemento ogni volta*/
#define MOLT_SIZE 5


/*Formattazione di stampa e di lettura*/
#define FORMAT_1 " %.2f"
#define FORMAT_2 " %.2f "
#define V_FORMAT ", %.2f"
#define READ_FORMAT "%lf"

/*Percentuale per la quale considero la coda sproporzionata, ossia percentuale
    per la quale la grandezza e la capacità della coda sono sproporzionate*/
#define RATIO 0.8  

typedef struct {
    double* queue;  /*array con allocazione dinamica memoria*/
    int n;          /*Capacita'*/
    int indexFirst; /*inidice primo elemento*/
    int indexLast;  /*indice nel quale sara' l'ultimo elemento*/
    bool init;      /*e' stata inizializzata la coda?*/
                    /*si poteva sviluppare anche solo con gli indici*/
} QueueDBA;

void queueInit(QueueDBA* );

int queueCapacity(const QueueDBA* );

int queueSize(const QueueDBA* );

bool queueIsEmpty(const QueueDBA* );

void resizeQueue(QueueDBA *);

void queueAddLast(QueueDBA* , double );

int queueGetFirst(const QueueDBA*, double *);

int queueRemoveFirst(QueueDBA* , double*);

void queueFree(QueueDBA* );

void fixQueue(QueueDBA* );
