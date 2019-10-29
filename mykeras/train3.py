import os
import sys
import keras

keras.__version__
from keras.preprocessing.image import ImageDataGenerator

from keras import models
from keras import layers
from keras import optimizers
from keras.applications.inception_resnet_v2 import InceptionResNetV2

conv_base = InceptionResNetV2(
                  include_top=True,
                  input_shape=(299, 299, 3))




conv_base.trainable = False
'''
model = models.Sequential()
model.add(conv_base)
model.add(layers.Flatten())
model.add(layers.Dense(256, activation='relu'))
model.add(layers.Dense(1, activation='sigmoid'))
model.summary()
'''



'''
set_trainable = False
for layer in conv_base.layers:
    if layer.name == 'block8_10_conv':
        set_trainable = True
    if set_trainable:
        layer.trainable = True
    else:
        layer.trainable = False
'''
base_dir = './datasets/'
train_dir = os.path.join(base_dir, 'train')
validation_dir = os.path.join(base_dir, 'validation')
test_dir = os.path.join(base_dir, 'test')

train_datagen = ImageDataGenerator(
      rescale=1./255,
      fill_mode='nearest')

# 검증 데이터는 증식되어서는 안 됩니다!

test_datagen = ImageDataGenerator(rescale=1./255)

train_generator = train_datagen.flow_from_directory(
        # 타깃 디렉터리
        train_dir,
        # 모든 이미지의 크기를 150 × 150로 변경합니다
        target_size=(299, 299),
        batch_size=100,
        # binary_crossentropy 손실을 사용하므로 이진 레이블이 필요합니다
        class_mode='binary')

validation_generator = test_datagen.flow_from_directory(
        validation_dir,
        target_size=(299, 299),
        batch_size=100,
        class_mode='binary')

conv_base.compile(loss='binary_crossentropy',
              optimizer=optimizers.RMSprop(lr=2e-5),
              metrics=['acc'])

history = conv_base.fit_generator(
      train_generator,
      steps_per_epoch=1,
      epochs=10,
      validation_data=validation_generator,
      validation_steps=1,
      verbose=1)
conv_base.save('model.h5')


model_json = conv_base.to_json()
with open("model.json", "w") as json_file : 
    json_file.write(model_json)
