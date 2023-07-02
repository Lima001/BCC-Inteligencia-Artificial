from mlp import *
import cv2
import os
import idx2numpy

mlp = MLP(2)
mlp.init_layer(0,784,25)
mlp.init_layer(1,25,10)
mlp.import_weights()

file = input("File name (jpg file in digits folder): ")

jpg = cv2.imread(f"digits/{file}.jpg", cv2.IMREAD_UNCHANGED)
cv2.imwrite(f"digits/{file}.png",jpg)

img = cv2.imread(f"digits/{file}.png", cv2.IMREAD_UNCHANGED)
os.remove(f"digits/{file}.png")
img = cv2.resize(img, (28,28))

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
cv2.imshow('Gray Image',gray)
cv2.waitKey(0)

np_img = np.asarray(gray)

#print(gray)
#print(np_img)

# Normalization - same transformation applied on training
# Obs. It's a better approach just save/load the values needed
# and not load the dataset itself...

TRAIN_IMAGE_FILENAME = 'mnist/train-images-idx3-ubyte'
train_images = idx2numpy.convert_from_file(TRAIN_IMAGE_FILENAME)
x_train = train_images.reshape(60000, 784)

np_img = np_img.reshape(1, 784)
mean = np.mean(x_train)
stddev = np.std(x_train)
np_img = (np_img - mean) / stddev

#print(np_img)

print(mlp.predict(np_img[0],False).argmax())