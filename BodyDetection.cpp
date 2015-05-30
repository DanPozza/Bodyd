
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
int persone;

using namespace std;
using namespace cv;



bool detection(Mat frame);

/** Global variables*/
String body_cascade_name = "/home/nico/opencv-3.0.0-rc1/data/haarcascades/haarcascade_frontalface_default.xml";//ubuntu
//String body_cascade_name = "C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";//windows
CascadeClassifier body_cascade;
string window_name = "Capture - body detection Local pc";




int main(int argc, char* argv[])
{

	cv::VideoCapture vcap;

	cv::Mat image;    
	const std::string videoStreamAddress = "http://localhost:8082";//put here the camera IP 

	//open the video stream and make sure it's opened

	if (!body_cascade.load(body_cascade_name))
	{
		printf("--(!)Error loading\n"); return -1;
	}

	if (!vcap.open(0))//use videoStreamAddress in brackets if you want to use IP camera
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}
	
	
	bool result,result_1;
	int i = 0;
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

		ofstream file("/home/nico/node/Npersone.txt"); //se il file non esiste lo crea, altrimenti lo sovrascrive!used on ubuntu system
	        if(!file) {
				cout<<"Errore nella creazione del file!";
				return -1;
	   		  }

		    file <<persone;
		    file.close(); //close the file

			sprintf(x, "curl http://localhost:3000/geofence");
			z.assign(x);
			system(z.c_str());
			detect = false;

		}

		if ((detect == false))
		{
			i = 0;
			////creiamo un tempo d'attesa di 5 (9000) minuti prima di inviare un'altra mail in presenza di persone
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

	persone =bodys.size();

	/**draw ellipse*/
	for (unsigned int j = 0; j < bodys.size(); j++)
	{
		Point center(bodys[j].x + bodys[j].width*0.5, bodys[j].y + +bodys[j].height*0.5);
		ellipse(frame, center, Size(bodys[j].width*0.5, bodys[j].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}

	imshow(window_name, frame);
		
	if (bodys.empty()){ waitKey(10); return false; }
	else{waitKey(2500);  return true; }
}



