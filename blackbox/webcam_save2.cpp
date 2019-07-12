#include <opencv2/opencv.hpp>
#include<string>
#include<time.h>
#include<sys/stat.h>
#include<iostream>
#include<stdio.h>
#include<pthread.h>

using namespace cv;
using namespace std;
void* thread_save(void *arg);
void* thread_time(void *arg);

int main() {
Mat img;

pthread_t t_id;

pthread_create(&t_id,NULL,thread_save,NULL);
pthread_create(&t_

        while(1);
    return 0;
}

void* thread_save(void *arg)
{
        time_t timer;
        struct tm *t;
        timer = time(NULL);
        t=localtime(&timer);

        int hour=t->tm_hour, min= t->tm_min, sec=t->tm_sec;
        char buf[1024];
        char buf2[1024];

        sprintf(buf,"%d",min);
        sprintf(buf2,"%d_%d_%d",hour,min,sec);

        int compmin;

        Mat img_color;
        string gst = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";     
        VideoCapture cap(gst);

        Size size = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
             (int)cap.get(CAP_PROP_FRAME_HEIGHT));
        VideoWriter writer;
        double fps=30.0;

        while(1){

        writer.open(buf2,VideoWriter::fourcc('M','J','P','G'),fps,size,true);
 if(!writer.isOpened())
        {
                cout<<"error to save initializing";
                
        }

        compmin=min
        while(min==compmin)
        {
                cap.read(img_color);
                if(img_color.empty()){
                        cerr<<"empty \n";
                        break;
                }


                writer.write(img_color);
                imshow("Color",img_color);
        }

                if(waitKey(25)>=0) break;
        }       
return 0;
}
