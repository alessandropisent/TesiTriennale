# scanning if a Matrix with only one index
``` c
    /*Scansione della matrice*/
    for(i=0; i<(n*m);i++){
        riga = (i/n) + M->s;
        colonna = (i%m) + 1;
        fscanf(inF,"%lf",&r);
        (M->A)[riga][colonna] = r;

    }/* for */
```