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

