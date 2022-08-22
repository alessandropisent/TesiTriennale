from random import randint, shuffle
import numpy as np
from scipy.io import savemat

#------ 
# Variabili di input

N = 10 # righe, # di equazioni
M = 10 # colonne, # di incognite

a = -100
b = 100

D = 0 # # of righe dipendenti
In = N-D

precisone = "0.00001"

#------------------------------------------------------
# START OF PROGRAMMA


m = np.empty((N,M),dtype=int)
c = np.empty((N),dtype=int)
rDip = np.empty((D),dtype=int)
mDip = np.empty((D,M))

# DETERMINA QUALI SONO LE RIGHE DIPENDENTI 
roC = list(range(N))
shuffle(roC)
rC = roC[:D]    # Indice di Righe dipendenti
rI = roC[D:]    # Indice di Righe indipendenti


print("SCRITTURA DEI NUMERI CASUALI")
for i in range(N):
    for j in range(M):
        m[i][j] = randint(a,b)

for i in range(N):
    c[i] = randint(a,b)

print("SCRITTURA DELLA DIPENDENZA")
#Scrive le dipendenza
with open("Codice/Dipendenza.txt", "w") as fileD:
    if(D>0):
        for i in range(D):
            f = randint(-b,b)
            g = randint(-b,b)
            fileD.write("RIGA %d sara' dipendendenti %d* r =%d + %d * r=%d\n"%(rC[i],f,rI[(2*i)%In],g,rI[(2*i+1)%In]))
            for j in range(M):
                m[rC[i]][j] = f*m[rI[(2*i)%In]][j]+g*m[rI[(2*i+1)%In]][j]
                c[rC[i]] = f*c[rI[(2*i)%In]]+g*c[rI[(2*i+1)%In]]


    else:
        fileD.write("No dipendenza")
    


#Scrittura del file per il programma LU 
# with open("Codice/LU/FileInput.txt","w") as f:
#     f.write(str(N)+" "+str(M)+"\n")
#     for i in range(N):
#         for j in range(M):
#             f.write(str(m[i][j])+" ")
#         f.write(str(c[i])+"\n")
#     f.write(precisone)

print("SCRITTURA DEL FILE MATLAB")
# Scrittura per il matlab
# with open("Codice/PS/FileMatlab.m","w") as f:
#     f.write("A = [ ")
#     for i in range(N):
#         for j in range(M):
#             f.write(str(m[i][j])+" ")
#         if(i!=N-1):
#             f.write("; ")
#         else:
#             f.write("]")
    
#     f.write("\nb = [ ")
#     for i in range(N):
#         f.write(str(c[i]))
#         if(i!=(N-1)):
#             f.write("; ")
#         else:
#             f.write(" ")
#     f.write("]")

#     f.write("\nx = linsolve(A,b)")

savemat("Codice/PS/var.mat", {"A":m.astype(float) , "b" : c.astype(float)})

print("SCRITTURA DEL FILE INPUT")
# Scrittura per il file di Test per il programma PRincipale
with open("Codice/FileInput.txt","w") as f:
    f.write(str(N)+" "+str(M)+"\n")
    for i in range(N):
        f.write(str(c[i])+" ")
        for j in range(M):
            f.write(str(m[i][j])+" ")
        f.write("\n")
    f.write(precisone)
        

# Scrittura per il file di decomposizione LU
# with open("Codice/PS/FileLU.txt","w") as f:
#     f.write("A = np.array([[ ")
#     for i in range(N):
#         for j in range(M):
#             f.write(str(m[i][j]))
#             if(j!=M-1):
#                 f.write(", ")
#             else:
#                 f.write("]")
#         if(i!=N-1):
#             f.write(", [")
#         else:
#             f.write("])")
    
#     f.write("\nb = [ ")
#     for i in range(N):
#         f.write(str(c[i]))
#         if(i!=(N-1)):
#             f.write(", ")
#         else:
#             f.write(" ")
#     f.write("]")
