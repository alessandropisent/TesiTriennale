from random import randint
import numpy as np


N = 5 # righe, b deve avere la stessa dimensione
M = 5 # colonne

a = 0
b = 10

m = np.empty((N,M),dtype=int)
c = np.empty((N),dtype=int)

for i in range(N):
    for j in range(M):
        m[i][j] = randint(a,b)

for i in range(N):
    c[i] = randint(a,b)

with open("Codice/FileInput.txt","w") as f:
    f.write(str(N)+" "+str(M)+"\n")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j])+" ")
        
        f.write("\n")
    for i in range(N):
        f.write(str(c[i])+"\n")

with open("Codice/LU/FileInput.txt","w") as f:
    f.write(str(N)+" "+str(M)+"\n")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j])+" ")
        
        f.write("\n")
    for i in range(N):
        f.write(str(c[i])+"\n")

with open("Codice/PS/FileMatlab.txt","w") as f:
    f.write("A = [ ")
    for i in range(N):
        for j in range(M):
            f.write(str(m[i][j])+" ")
        f.write("; ")
    
    f.write("\nb=[")
    for i in range(N):
        f.write(str(c[i])+" ")
    f.write("]")