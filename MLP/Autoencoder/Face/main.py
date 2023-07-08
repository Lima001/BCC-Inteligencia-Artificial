import sys

sys.path.insert(1, '../')

from mlp import *
import cv2
from matplotlib import pyplot
from random import randint

dim = 128

img = cv2.imread("img.png", cv2.IMREAD_UNCHANGED)
img = cv2.resize(img, (dim,dim))
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
#cv2.imshow('Transformed image',img)
#cv2.waitKey(0)

np_img = np.asarray(img)
np_reshaped_img = np_img.reshape(1, dim**2)
np_scaled_img = np_reshaped_img / 255.0

x_train = np.asarray(np_scaled_img)

#mlp = MLP(2)
#mlp.init_layer(0,dim**2,64)
#mlp.init_layer(1,64,dim**2)
#mlp.train(x_train,x_train,epochs=100,print_mse=True)

mlp = MLP(4)
mlp.init_layer(0,dim**2,64)
mlp.init_layer(1,64,16)
mlp.init_layer(2,16,64)
mlp.init_layer(3,64,dim**2)
mlp.import_weights()

#mlp.train(x_train,x_train,epochs=100,print_mse=True)
#mlp.export_weights()

predicted = mlp.predict(x_train[0])
print("Latent Representation", mlp.outputs[1], sep="\n")
predicted = predicted*255.0                                             
np.clip(predicted,0,255,predicted)
predicted = predicted.reshape(dim,dim) 

pyplot.subplot(1, 2, 1)
pyplot.imshow(np_img, cmap=pyplot.get_cmap('gray'))                         # Original image
pyplot.subplot(1, 2, 2) 
pyplot.imshow(predicted, cmap=pyplot.get_cmap('gray'))                      # Reconstructed (predicted) image
pyplot.show()