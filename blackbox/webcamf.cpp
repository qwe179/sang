
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{	

	Mat img_color;

	// for jetson onboard camera
	string gst = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture cap(gst);

/*	if(argc > 1){
		cap.read(argv[1]);
	}else{
		cap.read("res.avi");
	}
	if (!cap.isOpened()) {
		cerr << "cant open camera.\n";
		return -1;
	}
*/


	while(1)
	{
		cap.read(img_color);
		if (img_color.empty()) {
			cerr << "qls dudtkddlqslek.\n";
			break;
		}

		imshow("Color", img_color);

		if (waitKey(25) >= 0)
			break;


	}


	return 0;
}
