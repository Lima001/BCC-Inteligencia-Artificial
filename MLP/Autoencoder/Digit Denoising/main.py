import sys
sys.path.insert(1, '../')

from idx2numpy import convert_from_file
from matplotlib import pyplot
from random import randint
from mlp import *

# Given a scaled image dataset, the function adds gaussian noise generated with the standard deviation informed as parameter
def add_noise(image_set, std):
    noisy_image_set = np.copy(image_set)
    size = len(image_set[0])

    for i in range(len(image_set)):
        noise = np.random.normal(0.0, std, size)
        noisy_image_set[i] += noise

    # Ensure the values are in the scaled ranged - [0,1] interval
    np.clip(noisy_image_set,0,1,noisy_image_set) 
    return noisy_image_set


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

# Note that if you don't want to import the weights, the generate_radom_weights needs to be set to it default value (True)
noisy_x_train = add_noise(x_train,0.2)
noisy_x_test = add_noise(x_test,0.2)

### Create and train an autoencoder based on a simple MLP ###
mlp = MLP(2)
mlp.init_layer(0,784,64,generate_radom_weights=False)
mlp.init_layer(1,64,784,generate_radom_weights=False)

# Use this method if you have already trained the model, and want to reuse its weights
mlp.import_weights()

# Training process - To experiment, you can change the training parameters
#mlp.train(noisy_x_train,x_train,epochs=1,print_mse=True)
#mlp.export_weights()

### Testing ###

# Predicting examples on test set
while True:
    
    index = int(input(">>> "))
    
    if index == -1:
        break
    
    predicted = mlp.predict(noisy_x_test[index])                                      
    predicted = predicted*255.0                                                 # The model uses scaled data (between 0 and 1). So, if a gary scale image is wanted as output, you must undo the scaling
    np.clip(predicted,0,255,predicted)                                          # Avoiding out range pixel values
    predicted = predicted.reshape(28,28)                                        # Reshaping to match the dataset format

    # Transforming noised input data into an image for further visualization
    noisy_image = noisy_x_test[index] * 255.0
    np.clip(noisy_image,0,255,noisy_image) 
    noisy_image = noisy_image.reshape(28,28) 

    # Observing the image denoising result
    pyplot.subplot(1, 2, 1)
    pyplot.imshow(noisy_image, cmap=pyplot.get_cmap('gray'))                    # Noisy image
    pyplot.subplot(1, 2, 2) 
    pyplot.imshow(predicted, cmap=pyplot.get_cmap('gray'))                      # Reconstructed (predicted) image
    pyplot.show()