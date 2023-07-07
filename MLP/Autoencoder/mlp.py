import numpy as np
import os
import csv

LEARNING_RATE = 0.01
EPOCHS = 10
LOWER_BOUND = -0.1
UPPER_BOUND = 0.1

def logsig(x):
    return 1.0/(1.0 + np.exp(-x))

def dlogsig(x):
    ex = np.exp(x)
    return ex/((ex+1)*(ex+1))

def hypertan(x):
    ex = np.exp(x)
    exn = np.exp(-x)
    return (ex-exn)/(ex+exn)

def dhypertanh(x):
    exd = np.exp(2*x)
    return 4*exd/((exd+1)*(exd+1))

class MLP:

    def __init__(self, n_layers):
        self.n_layers = n_layers
        self.weights = [None for i in range(n_layers)]
        self.outputs = [None for i in range(n_layers)]
        self.errors = [None for i in range(n_layers)]

    def init_layer(self, ith, input_count, neuron_count, lb=LOWER_BOUND, ub=UPPER_BOUND):
        w = np.zeros((neuron_count,input_count+1))

        for i in range(neuron_count):
            for j in range(1, (input_count+1)):
                w[i][j] = np.random.uniform(lb, ub)

        self.weights[ith] = w
        self.outputs[ith] = np.zeros(neuron_count)
        self.errors[ith] = np.zeros(neuron_count)

    def export_weights(self, directory="mlp_config"):
        if (not os.path.isdir(directory)):
            os.mkdir(directory)
        
        for i in range(self.n_layers):
            with open(f"{directory}/weights_layer_{i}.csv", mode="w", newline='') as f:
                writer = csv.writer(f)
                writer.writerows(self.weights[i])

    def import_weights(self, directory="mlp_config"):
        if (not os.path.isdir(directory)):
            raise Exception("Configuration directory not found!")

        for i in range(self.n_layers):
            with open(f"{directory}/weights_layer_{i}.csv", mode="r") as f:
                reader = csv.reader(f)
                self.weights[i] = [np.array(row, dtype=float).tolist() for row in reader]

    def forward(self, x):
        # Input layer
        for i, w in enumerate(self.weights[0]):
            z = np.dot(w,x)
            self.outputs[0][i] = np.tanh(z)

        # Hidden layers
        for j in range(1,self.n_layers-1):
            hidden_output_array = np.concatenate((np.array([1.0]), self.outputs[j-1]))

            for i, w in enumerate(self.weights[j]):
                z = np.dot(w,hidden_output_array)
                self.outputs[j][i] = np.tanh(z)

        # Output layer
        hidden_output_array = np.concatenate((np.array([1.0]), self.outputs[-2]))
        for i, w in enumerate(self.weights[-1]):
            z = np.dot(w,hidden_output_array)
            self.outputs[-1][i] = 1.0 / (1.0 + np.exp(-z))
    
    def backward(self, y_truth, print_mse=False):
        # Output layer
        error_sum = 0.0
        for i, y in enumerate(self.outputs[-1]):
            error_sum += (y_truth[i] - y)**2
            error_prime = -(y_truth[i] - y)
            derivative = y * (1.0 - y)
            self.errors[-1][i] = error_prime * derivative

        if print_mse:
            print(f"MSE: {error_sum/len(y_truth)}")

        # Hidden layers + Input layer
        for j in range(self.n_layers-2, -1, -1):
            for i, y in enumerate(self.outputs[j]):
                error_weights = []
                
                for w in self.weights[j+1]:
                    error_weights.append(w[i+1])
                
                error_weight_array = np.array(error_weights)

                derivative = 1.0 - y**2
                weighted_error = np.dot(error_weight_array, self.errors[j+1])
                self.errors[j][i] = weighted_error * derivative

    def adjust_weights(self, x, lr=LEARNING_RATE):
        # Input layer
        for i, e in enumerate(self.errors[0]):
            self.weights[0][i] -= (x * lr * e)
    
        hidden_output_array = None
        for j in range(1,self.n_layers):
            hidden_output_array = np.concatenate((np.array([1.0]), self.outputs[j-1]))

            for i, e in enumerate(self.errors[j]):
                self.weights[j][i] -= (hidden_output_array * lr * e)

    def train(self, x_train, y_train, epochs=EPOCHS, lr=LEARNING_RATE, print_mse=False):
        index_train = list(range(len(x_train)))
        
        for i in range(epochs):
            # Training
            sum_training_successes = 0

            np.random.shuffle(index_train)
            
            for j in index_train:
                x = np.concatenate((np.array([1.0]), x_train[j]))
                self.forward(x)
                
                if np.array_equal(self.outputs[-1], y_train[j]):
                    sum_training_successes += 1
                
                self.backward(y_train[j], print_mse)
                self.adjust_weights(x, lr)

    def predict(self, x, verbose=False):
        xi = np.concatenate((np.array([1.0]), x))
        self.forward(xi)
            
        if verbose:
            print(f"Input: {x}")
            print(f"Output: {self.outputs[-1]}")
        
        return self.outputs[-1]