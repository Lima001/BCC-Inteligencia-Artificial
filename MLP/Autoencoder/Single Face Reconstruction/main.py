import sys
sys.path.insert(1, '../')

import cv2
from matplotlib import pyplot
from random import randint
from mlp import *

# Desired resized image dimension
dim = 128

# Reading (and converting to grayscale) the image used as input/output for the autoencoder model
img = cv2.imread("img.png", cv2.IMREAD_UNCHANGED)
img = cv2.resize(img, (dim,dim))
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Uncomment this code to visualize the transformed image
#cv2.imshow('Transformed image',img)
#cv2.waitKey(0)

# Reshaping and scaling the image to fit the model requeriments 
np_img = np.asarray(img)
np_reshaped_img = np_img.reshape(1, dim**2)
np_scaled_img = np_reshaped_img/255.0

# The model expects a dataset, thus we must append the previous array into another.
# More images could be passed inside the dataset array, if it was the case
x_train = np.asarray(np_scaled_img)

# First example - A simpler one, with fewer hidden layers and bigger latent space representation.
# You can use it for testing purposes, and compare to the second example shown below 
#mlp = MLP(2)
# Note that if you don't want to import the weights, the generate_radom_weights needs to be set to it default value (True)
#mlp.init_layer(0,dim**2,64)
#mlp.init_layer(1,64,dim**2)

# Second example - Adding more hidden layers and reducing the latent space representation 
mlp = MLP(4)
# Note that if you don't want to import the weights, the generate_radom_weights needs to be set to it default value (True)
mlp.init_layer(0,dim**2,64,generate_radom_weights=False)
mlp.init_layer(1,64,16,generate_radom_weights=False)
mlp.init_layer(2,16,64,generate_radom_weights=False)
mlp.init_layer(3,64,dim**2,generate_radom_weights=False)

# Use this method if you have already trained the model, and want to reuse its weights
mlp.import_weights()

# Training process
#mlp.train(x_train,x_train,epochs=100,print_mse=True)
#mlp.export_weights()

### Testing ###

# Predicting the single example used for training
predicted = mlp.predict(x_train[0])
predicted = predicted*255.0                                                 # The model uses scaled data (between 0 and 1). So, if a gary scale image is wanted as output, you must undo the scaling                                           
np.clip(predicted,0,255,predicted)                                          # Avoiding out range pixel values
predicted = predicted.reshape(dim,dim)                                      # Reshaping to match an image-like format

print("Latent Space Representation", mlp.outputs[1], sep="\n")              # Reduced vector with the image representation learned

pyplot.subplot(1, 2, 1)
pyplot.imshow(np_img, cmap=pyplot.get_cmap('gray'))                         # Original image
pyplot.subplot(1, 2, 2) 
pyplot.imshow(predicted, cmap=pyplot.get_cmap('gray'))                      # Reconstructed (predicted) image
pyplot.show()