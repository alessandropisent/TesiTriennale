from random import randint
import numpy as np


N = 4 # righe, b deve avere la stessa dimensione
M = 4 # colonne

a = 0
b = 10

m = np.empty((N,M),dtype=int)
c = np.empty((N),dtype=int)

for i in range(N):
    for j in range(M):
        m[i][j] = randint(a,b)

for i in range(N):
    c[i] = randint(a,b)

# Scrittura per il file di Test per il programma PRincipale
with open("Codice/FileInput.txt","w") as f:
    f.write(str(N)+" "+str(M)+"\n")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j])+" ")
        
        f.write("\n")
    for i in range(N):
        f.write(str(c[i])+"\n")
#Scrittura del file per il programma LU 
with open("Codice/LU/FileInput.txt","w") as f:
    f.write(str(N)+" "+str(M)+"\n")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j])+" ")
        
        f.write("\n")
    for i in range(N):
        f.write(str(c[i])+"\n")

# Scrittura per il matlab
with open("Codice/PS/FileMatlab.txt","w") as f:
    f.write("A = [ ")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j])+" ")
        if(i!=N-1):
            f.write("; ")
        else:
            f.write("]")
    
    f.write("\nb = [ ")
    for i in range(N):
        f.write(str(c[i]))
        if(i!=(N-1)):
            f.write("; ")
        else:
            f.write(" ")
    f.write("]")

    f.write("\nx = linsolve(A,b)")

# Scrittura per il file di decomposizione LU
with open("Codice/PS/FileLU.txt","w") as f:
    f.write("A = np.array([[ ")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j]))
            if(j!=M-1):
                f.write(", ")
            else:
                f.write("]")
        if(i!=N-1):
            f.write(", [")
        else:
            f.write("])")
    
    f.write("\nb = [ ")
    for i in range(N):
        f.write(str(c[i]))
        if(i!=(N-1)):
            f.write(", ")
        else:
            f.write(" ")
    f.write("]")
