
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include<libavformat/avformat.h>
#include<libavcodec/avcodec.h>

using namespace std;
using namespace cv;



void detection(Mat frame);

/** Global variables*/
String body_cascade_name = "/home/ubuntu/opencv/opencv-3.0.0-alpha/data/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier body_cascade;
string window_name = "Capture - body detection";
//RNG rng(12345);


int main(int argc, char* argv[])
{
	/*VideoCapture cap(0); // open the default camera
	    if(!cap.isOpened())  // check if we succeeded
	        return -1;

	    Mat edges;
	    namedWindow("edges",1);
	    for(;;)
	    {
	        Mat frame;
	        cap >> frame; // get a new frame from camera
	        cvtColor(frame, edges, COLOR_BGR2GRAY);
	        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
	        Canny(edges, edges, 0, 30, 3);
	        imshow("edges", edges);
	        if(waitKey(30) >= 0) break;
	    }
	    // the camera will be deinitialized automatically in VideoCapture destructor
	    return 0;*/

	cv::VideoCapture vcap;
	cv::Mat image;    // This works on a D-Link CDS-932L
	const std::string videoStreamAddress = "http://10.20.37.18:8082";

	//open the video stream and make sure it's opened

	if (!body_cascade.load(body_cascade_name))
	{
		printf("--(!)Error loading\n"); return -1;
	}
	vcap.open(videoStreamAddress);


	if (!vcap.isOpened())
	{
		std::cout << "Error opening video stream or file " << std::endl;
		return -1;
	}

	while (1)
	{
		if (!vcap.read(image))
		{
			std::cout << "No frame" << std::endl;
			cv::waitKey(0);
		}
		detection(image);
	//	imshow(window_name, image);
		waitKey(50);

	}



}

//######################################################################################à
void detection(Mat frame)
{
	vector<Rect> bodys;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);


    body_cascade.detectMultiScale(frame, bodys, 1.3, 2, 3, Size(30, 30));


	for (unsigned int j = 0; j < bodys.size(); j++)
	{
		Point center(bodys[j].x + bodys[j].width*0.5, bodys[j].y + +bodys[j].height*0.5);
		ellipse(frame, center, Size(bodys[j].width*0.5, bodys[j].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}

	//imshow(window_name, frame);
}


