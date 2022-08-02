# TesiTriennale Alessandro Pisent

Risoluzione con Gauss-Jordan

L'inserimento dei file e' del tipo con il sistema di equazioni scritti nel modo:
$$\begin{cases}
  a_{11} \cdot x_1 + a_{12} \cdot x_2 + a_{13} \cdot x_3 &= b_1 \\
  a_{21} \cdot x_1 + a_{22} \cdot x_2 + a_{23} \cdot x_3 &= b_2 \\
  a_{31} \cdot x_1 + a_{32} \cdot x_2 + a_{33} \cdot x_3 &= b_3 \\
\end{cases}$$

viene però immesso come input come :
```
n m
b1 a11 a12 a13
b2 a21 a22 a23
b3 a31 a32 a33
```
dove: 
- n = # di equazioni
- m = # di incognite
- b# = termine noto b#
- a## = coefficiente della equazione

Regole aggiuntive: 
- e' possibile non inserire tutti i coefficienti nell'equazione, verranno considerati nulli
- e' necessario inserire almeno 2 numeri in una riga (cioe' deò termine noto e il coefficiente di x1)