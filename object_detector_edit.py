import cv2
import os
import numpy as np
import tensorflow as tf
import tarfile
import six.moves.urllib as urllib
import time

from object_detection.utils import label_map_util
from object_detection.utils import visualization_utils as vis_util
from object_detection.utils import ops as utils_ops


class ObjectDetector():
    '''
    DOWNLOAD_BASE = 'http://download.tensorflow.org/models/object_detection/'
    GRAPH_FILE_NAME = 'frozen_inference_graph.pb'
    NUM_CLASSES = 90

    def download_model(self, model_name):
        model_file = model_name + '.tar.gz'
        print("downloading model", model_name, "...")
        opener = urllib.request.URLopener()
        opener.retrieve(self.DOWNLOAD_BASE + model_file, model_file)
        print("download completed");
        tar_file = tarfile.open(model_file)
        for file in tar_file.getmembers():
            file_name = os.path.basename(file.name)
            if self.GRAPH_FILE_NAME in file_name:
                tar_file.extract(file, os.getcwd())
                print(self.graph_file, "is extracted");
    '''
    def __init__(self, model_name, label_file='data/object-detection.pbtxt'):
        '''
        # Initialize some variables
        print("ObjectDetector('%s', '%s')" % (model_name, label_file))
        self.process_this_frame = True

        # download model
        self.graph_file = model_name + '/' + self.GRAPH_FILE_NAME
        if not os.path.isfile(self.graph_file):
            self.download_model(model_name)
        '''
        # Load a (frozen) Tensorflow model into memory.
        self.detection_graph = tf.Graph()
        with self.detection_graph.as_default():
            od_graph_def = tf.GraphDef()
            with tf.gfile.GFile('/home/bit205/Desktop/models/research/object_detection/freeze/training4_model-200001/frozen_inference_graph.pb', 'rb') as fid:
                serialized_graph = fid.read()
                od_graph_def.ParseFromString(serialized_graph)
                tf.import_graph_def(od_graph_def, name='')

            graph = self.detection_graph

            ops = graph.get_operations()
            all_tensor_names = {output.name for op in ops  for output in op.outputs}
            tensor_dict = {}
            for key in [
                  'num_detections', 'detection_boxes', 'detection_scores',
                  'detection_classes', 'detection_masks'
              ]:
                tensor_name = key + ':0'
                if tensor_name in all_tensor_names:
                    tensor_dict[key] = graph.get_tensor_by_name(tensor_name)

            if 'detection_masks' in tensor_dict:
                # The following processing is only for single image
                detection_boxes = tf.squeeze(tensor_dict['detection_boxes'], [0])
                detection_masks = tf.squeeze(tensor_dict['detection_masks'], [0])
                # Reframe is required to translate mask from box coordinates to image coordinates and fit the image size.
                real_num_detection = tf.cast(tensor_dict['num_detections'][0], tf.int32)
                detection_boxes = tf.slice(detection_boxes, [0, 0], [real_num_detection, -1])
                detection_masks = tf.slice(detection_masks, [0, 0, 0], [real_num_detection, -1, -1])
                detection_masks_reframed = utils_ops.reframe_box_masks_to_image_masks(
                    detection_masks, detection_boxes, 480, 640)
                detection_masks_reframed = tf.cast(
                    tf.greater(detection_masks_reframed, 0.5), tf.uint8)
                # Follow the convention by adding back the batch dimension
                tensor_dict['detection_masks'] = tf.expand_dims(
                    detection_masks_reframed, 0)

            self.tensor_dict = tensor_dict

        self.sess = tf.Session(graph=self.detection_graph)

        # Loading label map
        # Label maps map indices to category names,
        # so that when our convolution network predicts `5`,
        # we know that this corresponds to `airplane`.
        # Here we use internal utility functions,
        # but anything that returns a dictionary mapping integers to appropriate string labels would be fine
        label_map = label_map_util.load_labelmap(label_file)
        categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=90, use_display_name=True)
        self.category_index = label_map_util.create_category_index(categories)
        self.output_dict = None

        self.last_inference_time = 0

    def run_inference(self, image_np):
        sess = self.sess
        graph = self.detection_graph
        with graph.as_default():
            image_tensor = graph.get_tensor_by_name('image_tensor:0')

            # Run inference
            output_dict = sess.run(self.tensor_dict,
                                 feed_dict={image_tensor: np.expand_dims(image_np, 0)})

            # all outputs are float32 numpy arrays, so convert types as appropriate
            output_dict['num_detections'] = int(output_dict['num_detections'][0])
            output_dict['detection_classes'] = output_dict[
              'detection_classes'][0].astype(np.uint8)
            output_dict['detection_boxes'] = output_dict['detection_boxes'][0]
            output_dict['detection_scores'] = output_dict['detection_scores'][0]
            if 'detection_masks' in output_dict:
                output_dict['detection_masks'] = output_dict['detection_masks'][0]

        return output_dict

    def time_to_run_inference(self):
        unixtime = int(time.time())
        if self.last_inference_time != unixtime:
            self.last_inference_time = unixtime
            return True
        return False


    def detect_objects(self, frame,boxcoordi):
        #time1 = time.time()
        # Grab a single frame of video

        # Resize frame of video to 1/4 size for faster face recognition processing
        #small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)
        #small_frame = frame

        # Convert the image from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
        rgb_small_frame = frame[:, :, ::-1]
        
        #time2 = time.time()

        # Only process every other frame of video to save time
        if True:    
            self.output_dict = self.run_inference(rgb_small_frame)
        '''
        #time3 = time.time()
        classess=(np.int32(np.array([])))
        scoress=np.array([])
        boxess=(np.int32(np.array([])))
        for _ in range(0, 100):
            if self.output_dict['detection_scores'][_] != np.float32(0.0):
                classes=self.output_dict['detection_classes'][_]
                scores=self.output_dict['detection_scores'][_]
                boxes=self.output_dict['detection_boxes'][_]
                classess=np.append(classess,classes)
                scoress=np.append(scoress,scores)
                boxess=np.append(boxess,boxes)
                print(boxes)
        print('classess:',classess)
        print('scoress:',scoress)
        print('boxess:',boxess)
        start_pos=0
        end_pos=len(boxess)
        div=4
        boxx=[]
        classx=[]
        scorex=[]

        width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
        print("width,height",width,height)
        count =0

        for idx in range(start_pos,end_pos,div):
            out=boxess[start_pos:start_pos+div]
            print(out) 
            
            start_pos=start_pos+div
            count=idx//4
            boxx.insert(count,out)

        count = len(boxx)
        
        
        start_pos = 0
        print(count,"count")
        print("start_pos : ",start_pos)
        if count!=0:
            for i in range(count):
                for j in range(4):
                    if j==0:
                        boxx[i][j]=boxx[i][j]*height
                    if j==1:
                        boxx[i][j]=boxx[i][j]*width
                    if j==2:
                        boxx[i][j]=boxx[i][j]*height
                    if j==3:
                        boxx[i][j]=boxx[i][j]*width
        print("boxx : ",boxx)
        len(classess)
        for idx in range(start_pos,len(classess)):
            out2=classess[idx]
            out3=scoress[idx]
            classx.insert(idx,out2)
            scorex.insert(idx,out3)
        
        #print("classx : ",classx)
        #print (classx[3])
        #print("scorex : ",scorex)
        #print (scorex[0])
        
        count = len(boxx)
        w,h=4,count
        boxsubstract=[[0 for x in range(w)] for y in range(h)]
        print(len(boxsubstract))
        if boxcoordi!=0:
            if len(boxcoordi)==len(boxx):
                for i in range(count):
                    if classess[i]== 2:
                        for j in range(4):
                            print("boxcoordi[0][0] : ",boxcoordi[0][0])
                            print("boxx[0][0] : ",boxx[0][0])
                            
                            print(boxcoordi)
                            print(boxx)
                            boxsubstract[i][j]=boxcoordi[i][j]-boxx[i][j]
                            
                            print("boxsubstract",boxsubstract)
                            print("boxsubstract[i][3]",boxsubstract[i][3])
                        if ((200>abs(boxsubstract[i][2])>0))|(200>(abs(boxsubstract[i][3])>0)):
                            scorex[i]=scorex[i] -0.5
                            if scorex[i]<0.99:
                                self.output_dict['detection_classes'][i]=1
                    
            
        print("box coordi : ",boxcoordi)

        boxcoordi=boxx
        
        try:
            print("box coordi1 : ",boxcoordi)
        except:
            pass
        boxcoordi=boxx
        try:
            print("box coordi2 : ",boxcoordi)
        except:
            pass
        '''
        
        vis_util.visualize_boxes_and_labels_on_image_array(
          frame,
          self.output_dict['detection_boxes'],
          self.output_dict['detection_classes'],
          self.output_dict['detection_scores'],
          self.category_index,
          instance_masks=self.output_dict.get('detection_masks'),
          use_normalized_coordinates=True,
          line_thickness=1)

        #time4 = time.time()

        #print("%0.3f, %0.3f, %0.3f sec" % (time2 - time1, time3 - time2, time4 - time3))
        #if self.output_dict['detection_boxes'] != 0:
        #for _ in range(0, 100):
        #    if self.output_dict['detection_scores'][_] != np.float32(0.0):
                #print(self.output_dict['detection_classes'][_])
        #        print(self.output_dict['detection_scores'][_],"test!!!!!!!")
                #print(self.output_dict['detection_boxes'][_])

        return frame, boxcoordi

    def get_jpg_bytes(self):
        frame = self.get_frame()
        # We are using Motion JPEG, but OpenCV defaults to capture raw images,
        # so we must encode it into JPEG in order to correctly display the
        # video stream.
        ret, jpg = cv2.imencode('.jpg', frame)
        return jpg.tobytes()


if __name__ == '__main__':
    detector = ObjectDetector('./freeze/training4_model-200001')

    #detector = ObjectDetector('mask_rcnn_inception_v2_coco_2018_01_28')
    #detector = ObjectDetector('pet', label_file='data/pet_label_map.pbtxt')

    # Using OpenCV to capture from device 0. If you have trouble capturing
    # from a webcam, comment the line below out and use a video file
    # instead.
    #time_length = 30.0
    #fps=60
    #frame_seq = 749
    #frame_no = (frame_seq /(time_length*fps))
    cap = cv2.VideoCapture('/home/bit205/Desktop/test/old3.mp4')
    num = 0
    print("press `q` to quit")
    fps= cap.get(cv2.CAP_PROP_FPS)
    total=cap.get(cv2.CAP_PROP_FRAME_COUNT)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    file_name ='./output/boxtest2.avi'
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    writer = cv2.VideoWriter(file_name, fourcc, fps, (width, height))

    while True:
        #frame = cam.get_frame()
        #frame = detector.detect_objects(frame)
        #cap.set(2,frame_no);
        #cap.set(2,frame_no);

        #Read the next frame from the video. If you set frame 749 above then the code will return the last frame.
        ret, frame =cap.read()
        if num==0 :
            boxcoordi=0
        frame, boxcoordi = detector.detect_objects(frame,boxcoordi)
        # show the frame
        cv2.imshow("Frame", frame)
        writer.write(frame)
            
        # if the `q` key was pressed, break from the loop
        key = cv2.waitKey(1) & 0xFF

        if key == ord("q"):
                break
        if key == ord("s"):
            for _ in range(0,60):
                cap.read()
        num = 1 

    # do a bit of cleanup
    cap.release()
    writer.release()
    cv2.destroyAllWindows()
    print('finish')