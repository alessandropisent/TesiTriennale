from sre_constants import AT_MULTILINE
import numpy as np
from scipy.io import loadmat
import math


Matlab = loadmat('Codice/PS/result.mat')
aMatlab = Matlab["x"]
aC = []
eq = True

with open("Codice/results.txt","r") as f:
    for line in f:
        aC.append(float(line))

for i in range(len(aC)):
    if(not math.isclose(aC[i],aMatlab[i],rel_tol=0.05)):
        print(str(i)+": "+str(aC[i])+" != "+ str(aMatlab[i]))
        eq = False
    else:
        print(str(i)+": "+str(aC[i])+" == "+ str(aMatlab[i]))
    
if(eq):
    print("Risultato identico")
