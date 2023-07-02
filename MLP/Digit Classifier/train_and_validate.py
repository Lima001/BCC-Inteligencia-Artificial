# Reference: Ekman (2021)

from mlp import *
import idx2numpy

# To make repeatable
np.random.seed(7)

TRAIN_IMAGE_FILENAME = 'mnist/train-images-idx3-ubyte'
TRAIN_LABEL_FILENAME = 'mnist/train-labels-idx1-ubyte'
TEST_IMAGE_FILENAME = 'mnist/t10k-images-idx3-ubyte'
TEST_LABEL_FILENAME = 'mnist/t10k-labels-idx1-ubyte'

def read_mnist(normalize=True):
    train_images = idx2numpy.convert_from_file(TRAIN_IMAGE_FILENAME)
    train_labels = idx2numpy.convert_from_file(TRAIN_LABEL_FILENAME)
    test_images = idx2numpy.convert_from_file(TEST_IMAGE_FILENAME)
    test_labels = idx2numpy.convert_from_file(TEST_LABEL_FILENAME)

    # Reformat and standardize.
    x_train = train_images.reshape(60000, 784)
    x_test = test_images.reshape(10000, 784)
    
    # Normalization aiming data close to normal distribution centered on 0
    if normalize:
        mean = np.mean(x_train)
        stddev = np.std(x_train)
        x_train = (x_train - mean) / stddev
        x_test = (x_test - mean) / stddev

    # One-hot encoded output
    y_train = np.zeros((60000, 10))
    y_test = np.zeros((10000, 10))
    
    for i, y in enumerate(train_labels):
        y_train[i][y] = 1
    
    for i, y in enumerate(test_labels):
        y_test[i][y] = 1
    
    return x_train, y_train, x_test, y_test

# Read train and test examples.
x_train, y_train, x_test, y_test = read_mnist()

# Create a MLP wth one hidden layer
mlp = MLP(2)
mlp.init_layer(0,784,25)
mlp.init_layer(1,25,10)
#mlp.import_weights()

# Other MLP for experimentation - This has 2 hidden layers
#mlp = MLP(3)
#mlp.init_layer(0,784,125)
#mlp.init_layer(1,125,25)
#mlp.init_layer(2,25,10)

mlp.train(x_train,y_train,x_test,y_test,print_mse=True)
#mlp.export_weights()