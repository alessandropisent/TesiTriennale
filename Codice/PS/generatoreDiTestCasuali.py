from random import randint


N = 100 # righe, b deve avere la stessa dimensione
M = 100 # colonne

a = 0
b = 100

with open("Codice/FileInput.txt","w") as f:
    f.write(str(N)+" "+str(M)+"\n")
    for i in range(N):
        for j in range(M):
            f.write(str(randint(a,b))+" ")
        
        f.write("\n")
    for i in range(N):
        f.write(str(randint(a,b))+"\n")