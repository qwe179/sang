#include <opencv2/opencv.hpp>
#include<string> 
#include<iostream>
using namespace cv;
using namespace std; 
int main() {
    Mat img;           
 
	string gst = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture cap(gst);
    while(1){
        cap >> img;        
        imshow("image", img);      //    
 
 
        if (waitKey(100) == 27)  break;
    }
    return 0;
}
 
