from mlp import *

x_train = np.array([
    [-1.0, -1.0],
    [-1.0, 1.0],
    [1.0, -1.0],
    [1.0, 1.0]
])

y_train = np.array([
    [0.0],
    [1.0], 
    [1.0], 
    [0.0]
])

xorMLP = MLP(2)
xorMLP.init_layer(0, 2, 2, -1.0, 1.0)
xorMLP.init_layer(1, 2, 1, -1.0, 1.0)

xorMLP.train(x_train, y_train, None, None, 10000, 0.1, False, False)

#print(xorMLP.weights)

for x in x_train:
    xorMLP.predict(x,verbose=True)

