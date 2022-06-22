import numpy as np

#Esempio per calcoli
A = np.array([[1 , 4, 7] , [2,5,8], [3,6,10]])
b = [1,1,1]

M = np.array([[7,9,8], [6,4,3], [6,5,10]])
x = [0,4,6]

#se state > 0 Lower, <0 Upper
def triang(n,state):
    L = np.zeros((n,n),dtype=int)
    # Righe
    for i in range(n):
        #Colonne
        for j in range(n):
            if((state*(i-j))>=0):
                L[i][j] = 1
            else:
                L[i][j] = 0
    return L

# Controllo se la Matrice possiede una scomposizione LU
# Per teoremi di Algebra lineare : esiste se det(A[1:k,1:k]) != 0 , per k = 1:n-1
def isScomponibile(A):

    n = len(A)
    for i in range(1,n-1):
        if(np.linalg.det(A[0:i,0:i])==0):
            return False
    return True

def LU(A):

    if(not isScomponibile(A)):
        print("NON SCOMPONIBILE")
        return

    n = len(A)
    L = triang(n,1)
    U = triang(n,-1)

    for k in range(n):
        # Calcolo del up
        U[k][k] = A[k][k]

        for i in range(k+1,n):

            L[i,k]=A[i][k] / U[k][k]

            U[k][i] = A[k][i]

        for i in range(k+1,n):

            for j in range(k+1,n):

                A[i][j] = A[i][j] - L[i][k]*U[k][j]
    
    return (L,U)

def lowSolver(L,b):
    n = len(L)
    y = np.zeros(n)
    for i in range(n):
        sum = 0
        for j in range(i):
            sum += L[i][j]*y[j]
        y[i] = (b[i] - sum)/L[i][i]
    return y


def upSolver(U,y):
    n=len(U)
    x = np.zeros(n)
    for i in range(n-1,-1,-1):
        sum = 0
        for j in range(i,n):
            sum += U[i][j]*x[j]
        x[i] = (y[i] - sum)/U[i][i]
    return x

def debug(L,U,y,x):
    print(L)
    print(U)
    print(y)
    print(x)

def linearSolver(A,b):
    (L,U) = LU(A)
    y = lowSolver(L,b)
    x = upSolver(U,y)
    #debug(L,U,y,x)
    return x

x = linearSolver(M,x)
print(x)