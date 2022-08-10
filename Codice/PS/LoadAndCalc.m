m = load("var.mat")
x = linsolve(m.A,transpose(m.b))
save("result.mat","x")