#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>


using namespace std;
using namespace cv;



bool detection(Mat frame);

/** Global variables*/
String body_cascade_name = "C:/opencv/sources/data/haarcascades/haarcascade_upperbody.xml";
CascadeClassifier body_cascade;
string window_name = "Capture - body detection";
//RNG rng(12345);



int main(int argc, char* argv[])
{
	/*CvCapture* capture;
	Mat frame;


	if (!body_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml"))
	{
		printf("--(!)Error loading\n"); return -1;
	};

	capture = cvCaptureFromCAM(0);
	

	if (capture != NULL)
	{
		while (1)
		{
			frame = cvQueryFrame(capture);
			if (!frame.empty())
			{
				detection(frame);
			}
			else {
				printf(" --(!) No captured frame -- Break!");
				break;
			}
			int d = waitKey(100);
			if ((char)d == 'd')
			{
				break;
			}
		}
	}
	return 1;*/

	cv::VideoCapture vcap;
	cv::Mat image;    // This works on a D-Link CDS-932L
	//const std::string videoStreamAddress = "http://192.168.1.121:8082";

	//open the video stream and make sure it's opened

	if (!body_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml"))
	{
		printf("--(!)Error loading\n"); return -1;
	}

	if (!vcap.open(0))
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}

	bool result;
	char x[50];
	string z;
	bool detect =true;

	while (1)
	{
		if (!vcap.read(image))
		{
			cout << "No frame" << endl;
			waitKey(0);
		}
		result = detection(image);
		if ((result == true) && detect==true)
		{ 
			cout << "detected" << endl;
			sprintf(x, "curl http://localhost:3000/2/2"); // 2/2 rappresentano la latitudine e longitudine della videocamera
			z.assign(x);
			system(z.c_str());
			detect = false;

		}
		if (detect==false)
		{
			int i = 0;
			////creiamo un tempo d'attesa di 5 minuti prima di inviare un'altra mail in presenza di persone
			for (i; i < 9000;i++)
			{
				vcap.read(image);
					imshow(window_name, image);
					waitKey(10);
			}
			detect = true;
		}
		else
		{
			cout << "nobody" << endl;
		}
	}
		
	}

//######################################################################################à
bool detection(Mat frame)
{
	vector<Rect> bodys;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	/**detect body */

	body_cascade.detectMultiScale(frame_gray, bodys, 1.8, 2, 3, Size(30, 30));

	/**draw ellipse*/
	for (unsigned int j = 0; j < bodys.size(); j++)
	{
		Point center(bodys[j].x + bodys[j].width*0.5, bodys[j].y + +bodys[j].height*0.5);
		ellipse(frame, center, Size(bodys[j].width*0.5, bodys[j].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}


	imshow(window_name, frame);
	
	if (bodys.empty()){ waitKey(10); return false; }
	else{ waitKey(1000); return true; }
}




