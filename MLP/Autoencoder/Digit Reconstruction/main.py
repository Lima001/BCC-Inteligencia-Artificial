import sys
sys.path.insert(1, '../')

from idx2numpy import convert_from_file
from matplotlib import pyplot
from random import randint

from mlp import *

### MNIST Dataset importing, resizing and scaling ###
# Note: Only the input data is needed for this example, you can leave the label data aside.
# This is due to the fact autoencoders use the same input set as the output 

TRAIN_IMAGE_FILENAME = '../../Digit Classifier/mnist/train-images-idx3-ubyte'
TEST_IMAGE_FILENAME = '../../Digit Classifier/mnist/t10k-images-idx3-ubyte'

train_images = convert_from_file(TRAIN_IMAGE_FILENAME)
test_images = convert_from_file(TEST_IMAGE_FILENAME)

x_train = train_images.reshape(60000, 784)
x_test = test_images.reshape(10000, 784)
    
x_train = x_train/255.0
x_test = x_test/255.0

### Create and train an autoencoder based on a simple MLP ###

mlp = MLP(2)
# Note that if you don't want to import the weights, the generate_radom_weights needs to be set to it default value (True)
mlp.init_layer(0,784,64,generate_radom_weights=False)
mlp.init_layer(1,64,784,generate_radom_weights=False)

# Use this method if you have already trained the model, and want to reuse its weights
mlp.import_weights()

# Training process - To experiment, you can try changing the training parameters
#mlp.train(x_train,x_train,epochs=1,print_mse=True)
#mlp.export_weights()

### Testing ###

# Predicting a single example on test set
while True:
    
    index = int(input(">>> "))
    
    if index == -1:
        break

    predicted = mlp.predict(x_test[index])                                      
    predicted = predicted*255.0                                                 # The model uses scaled data (between 0 and 1). So, if a gary scale image is wanted as output, you must undo the scaling
    np.clip(predicted,0,255,predicted)                                          # Avoiding out range pixel values
    predicted = predicted.reshape(28,28)                                        # Reshaping to match the dataset format

    print("Latent Space Representation:", mlp.outputs[0], sep="\n")   

    # Observing the example's image reconstruction
    pyplot.subplot(1, 2, 1)
    pyplot.imshow(test_images[index], cmap=pyplot.get_cmap('gray'))             # Original image
    pyplot.subplot(1, 2, 2) 
    pyplot.imshow(predicted, cmap=pyplot.get_cmap('gray'))                      # Reconstructed (predicted) image
    pyplot.show()