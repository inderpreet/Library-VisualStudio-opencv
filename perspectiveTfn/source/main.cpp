/* ****************************************************************************
	Open CV based image transformation	

	By: Inderpreet Singh

***************************************************************************  */

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>
#include"drawbox.h"
#include"drawpoints.h"
#include"transform.h"


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{	
	IplImage* Image = NULL;  /* = cvLoadImage(FileName);*/
	bool vidOrImg = true;					// Use Video or image?
	if(vidOrImg)
	{
		// Read the video
		char vidFile[]	= "..\\14-03-24.avi";	// File Name
		CvCapture* Capture = NULL;
		Capture = cvCreateFileCapture(vidFile);
		if(!Capture) 
			fprintf( stderr, "ERROR: capture is NULL \n" );
		// Wait for SpaceBar
		cvNamedWindow("Video");
		char c=NULL;
		while(c!=' ')
		{
			Image = cvQueryFrame(Capture);
			cvShowImage("Video", Image);
			c=cvWaitKey(3);
		}
	}
	else
	{
		// Read the Image
		const char* FileName="D:/Sandbox/VS2010/perspectiveTfn/source/cap1.jpg";
		Image = cvLoadImage(FileName);
		if(!Image )                              // Check for invalid input
		{	cout <<  "Could not open or find the image" << std::endl ;
			return -1;
		}
	}
	// ---------------------------------------------------------------------------
	//
	IplImage* temp = cvCloneImage(Image);
	ipDraw4Points(temp);
	//cvWaitKey(5000);

	// ---------------------------------------------------------------------------
	//
	Mat Transformed(Image);
	ipPerspectiveTransform(Image, &Transformed);
	namedWindow("Out");
	imshow("Out", Transformed);
	cvWaitKey(0);
	return 0;
}





