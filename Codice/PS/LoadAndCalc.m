m = load("Matrix.mat")
x = linsolve(m.A,transpose(m.b))
save("result.mat","x")