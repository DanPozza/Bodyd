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
String body_cascade_name = "C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";
CascadeClassifier body_cascade;
string window_name = "Capture - body detection Local pc";
string window_name_1 = "Capture - body detection IP camera";
//RNG rng(12345);



int main(int argc, char* argv[])
{

	cv::VideoCapture vcap;
	cv::VideoCapture cap;

	cv::Mat image,image_1;    // This works on a D-Link CDS-932L
	const std::string videoStreamAddress = "http://192.168.1.108:8080";

	//open the video stream and make sure it's opened

	if (!body_cascade.load(body_cascade_name))
	{
		printf("--(!)Error loading\n"); return -1;
	}

	if (!vcap.open(0))
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}
	
	if (!cap.open(videoStreamAddress))
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}

	bool result,result_1;
	int i = 0;
	char x[50];
	string z;
	bool detect =true;
	bool detect_1 = true;


	while (1)
	{
		if (!vcap.read(image))
		{
			cout << "No frame" << endl;
			waitKey(0);
		}
		if (!cap.read(image_1))
		{
			cout << "No frame" << endl;
			waitKey(0);
		}
		result = detection(image);
		imshow(window_name, image);
		result_1 = detection(image_1);
		imshow(window_name_1, image_1);
		waitKey(10);

		if ((result == true) && detect==true)
		{ 
			cout << "detected" << endl;
		/*	sprintf(x, "curl http://localhost:3000/2/2"); // 2/2 rappresentano la latitudine e longitudine della videocamera locale
			z.assign(x);
			system(z.c_str());*/
			detect = false;

		}
		if ((result_1 == true) && detect_1 == true)
		{
			cout << "detected" << endl;
			/*	sprintf(x, "curl http://localhost:3000/4/9"); // 4/9 rappresentano la latitudine e longitudine della videocamera da ip
			z.assign(x);
			system(z.c_str());*/
			detect_1 = false;
			

		}

		if ((detect == false))
		{
			i = 0;
			////creiamo un tempo d'attesa di 5 (9000) minuti prima di inviare un'altra mail in presenza di persone
			for (i; i < 900;i++)
			{
				cap.read(image_1);
				vcap.read(image);
				imshow(window_name_1, image_1);
				imshow(window_name, image);
				waitKey(10);
			}
			detect = true;
		}
		else
		{
			cout << "nobody" << endl;
		}

		if ((detect_1 == false))
		{
			i = 0;
			////creiamo un tempo d'attesa di 5 (9000) minuti prima di inviare un'altra mail in presenza di persone
			for (i; i < 900; i++)
			{
				cap.read(image_1);
				vcap.read(image);
				imshow(window_name_1, image_1);
				imshow(window_name, image);
				waitKey(10);
			}
			detect_1 = true;
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


	if (bodys.empty()){  return false; }
	else{  return true; }
}




