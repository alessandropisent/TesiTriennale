/*  Funzione aggiunge la riga all'array di righe indipendenti
    IP r, riga da aggiungere
    IOP M, matrice con tutto
*/
void addR(int r, Matrix* M){
    
    int i,e;
    bool found = false;

    /*controllo che l'elemento che voglio mettere nel array non ci sia gia'*/
    for(i=0; i<M->nRD;i++){
        e = (M->dipRow)[i];
        if(e==r){
            found = true;
            break;
        }/*if*/

    }/*for*/

    if(!found){
        M->dipRow[M->nRD] = r;
        M->nRD +=1;
    }/*if*/


}/*addR*/