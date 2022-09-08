# Algorimi
1. Scomposizione LU : $O(n^3)$, scritto in Python (non mi sono preoccupato dell'efficenza) 
2. Scomposizione QR : So che esiste ma non so bene come si possa fare ne quanto possa costare
3. Scompozione di Cholensky (LL*) (Presuppone che la matrice A sia Ermitiana(se reale sempre) e definita positiva)
4. Inversione della matrice : il piu complesso credo
5. Diagonalizzazione della matrice : complesso ma si può fare

# Scomposizione LU vs Gauss-Jordan
LU decomposition is a better way to implement Gauss elimination, especially for repeated solving a number of equations with the same left-hand side. That is, for solving the equation Ax = b with different values of b for the same A.

Quindi è meglio solo per risolvere più equazioni con diversi b

ma sono sempre algorimi $O(n^3)$

# EdgeCases per Gauss Jordan
- [x] Più righe che colonne
- [x] Più colonne che righe
- [x] Linee Linearmente dipendenti

## Domande per professore
1. Da quando a quando va in vacanza
2. Se sa entro quando devo consegnare
3. E quando scriviamo il titolo

# Risoluzione
Se # Di righe > # Colonne, posso risolvere
Se # Di Colonne > # Di righe infinite soluzioni

# Vacanze professore
- 15-28 Luglio
- 29-23 Agosto

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


/*per avere un idea del progresso*/
            if(((M->nEq) * (M->nIn)) > MAX_STAMPA)
                /*la stringa al inizio serve per abbellire l'output*/
                printf("\033[A\33[2K\rProgress: %d %%, i=%d, c=%d, j=%d, r=%d, nD=%d, k=%d\n",((i)*100/(n-(M->nEDip))),i,c,j,r,M->nEDip,k);

T0 = 5x5 sistema con risultato unico (5 Equazioni x 5 Incognite)
T1 = 8x5 sistema con risultato unico (8 Equazioni x 5 Incognite) (3 Eqn linearmente dipendenti )
T2 = 8x5 sistema impossibile (8 Equazioni x 5 Incognite)
T3 = 4x5 sistema indeterminato (4 Equazioni x 5 Incognite) 1 incognita libera
T4 = 6x5 sistema indeterminato (6 Equazioni x 5 Incognite) 1 incognita libera , 2 Equazioni linearmente dipendenti
T5 = 7x6 sistema impossibile (7 Equazioni x 6 Incognite)
T6 = 230x200 sistema con risultato unico (230 Equazioni x 200 Incognite) 30 Equazioni linearmente dipendenti

in più se vuole ho fatto test con equazioni più grandi:

T7 = 1000x1000 sistema con risultato unico

Tutti questi test sono stati controllati con matlab
