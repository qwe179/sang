import keras
keras.__version__
from keras.applications.inception_resnet_v2 import InceptionResNetV2
import os
import numpy as np
from keras.preprocessing.image import ImageDataGenerator
from keras import models
from keras import layers
from keras import optimizers

conv_base = InceptionResNetV2(weights=None,include_top=True,
                  input_shape=(299, 299, 3),classes=3)

conv_base.trainable = True


import os
import numpy as np
from keras.preprocessing.image import ImageDataGenerator

base_dir = './datasets/'

train_dir = os.path.join(base_dir, 'train')
validation_dir = os.path.join(base_dir, 'validation')
test_dir = os.path.join(base_dir, 'test')

datagen = ImageDataGenerator(rescale=1./255)
batch_size = 20

def extract_features(directory, sample_count):
    features = np.zeros(shape=(sample_count, 3))    
    labels = np.zeros(shape=(sample_count))
    generator = datagen.flow_from_directory(
        directory,
        target_size=(299, 299),
        batch_size=batch_size,
        class_mode='binary')
    i = 0
    for inputs_batch, labels_batch in generator:
        features_batch = conv_base.predict(inputs_batch)
        features[i * batch_size : (i + 1) * batch_size] = features_batch
        labels[i * batch_size : (i + 1) * batch_size] = labels_batch
        i += 1
        if i * batch_size >= sample_count:
            # 제너레이터는 루프 안에서 무한하게 데이터를 만들어내므로 모든 이미지를 한 번씩 처리하고 나면 중지합니다
            break
    return features, labels

train_features, train_labels = extract_features(train_dir, 10705)
validation_features, validation_labels = extract_features(validation_dir, 1)
test_features, test_labels = extract_features(test_dir, 1)
train_features = np.reshape(train_features, (10705, 3))
validation_features = np.reshape(validation_features, (1, 3))
test_features = np.reshape(test_features, (1, 3))


model = models.Sequential()

model.add(layers.Dense(1, activation='relu', input_dim=3))
model.add(layers.Dense(1, activation='sigmoid'))

model.summary()
model.compile(optimizer=optimizers.RMSprop(lr=2e-5),
              loss='binary_crossentropy',
              metrics=['acc'])
history = model.fit(train_features, train_labels,
                    epochs=150000,
                    batch_size=20,
                    validation_data=(validation_features, validation_labels))



model.save('model.h5')
conv_base.save('conv.h5')

model_json = model.to_json()
with open("model.json", "w") as json_file : 
    json_file.write(model_json)
