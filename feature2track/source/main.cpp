/* Program For Extracting the speed from a video
*	
*	Written By: Inderpreet Singh
*	er.inderpreet@gmail.com
*/

/* ****************************************************************************
* 	Program for Extracting Background from the camera captures.
* 	Using Gaussian Mixter Based Detection
*
*	Written By: Inderpreet Singh
*
*	Description: Uses OpenCV 2.4
*/

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<conio.h>
#include "ipCaptureDevice.h"

using namespace cv;

/* ******************* Feature Detection Stuff *******************************/


// ****************************************************************************

int main(int argc, char *argv[])
{
/* ******************* Feature Detection Stuff *******************************/	

	Mat grayFrame;
	Mat eigImage;
	vector<Point2f> features;
	const int MAX_CORNERS = 10;
	int corner_count = MAX_CORNERS;
	double quality_level = 0.1;
	double min_distance = 10;
	int eig_block_size = 3;
	int use_harris = false;

	int c;
	Mat CurrFrame;
	ipCaptureDevice Cap1;
	CurrFrame = Cap1.ipGetFrame();
	
	namedWindow("Window");

	while(1)
	{
		CurrFrame = Cap1.ipGetFrame();
		imshow("Window", CurrFrame);

/* ******************* Feature Detection Stuff *******************************/
		
		cvtColor(CurrFrame, grayFrame, CV_BGR2GRAY);
		goodFeaturesToTrack(grayFrame,
				features,
				corner_count,
				quality_level,
				min_distance,
				NULL,
				eig_block_size,
				use_harris);
		/*cvScale(eig_image, eig_image, 100, 0.00);
		for( int i = 0; i < corner_count; i++) {
			int radius = h/25;
			cvCircle(curr_frame,
					cvPoint((int)(corners[i].x + 0.5f),(int)(corners[i].y + 0.5f)),
					radius,
					target_color[0]);
		}*/
//		imshow("Corners", grayFrame);

		c = cvWaitKey(10); // wait 10 ms or for key stroke
		if(c >=27)
			break;
	}
	
	cvDestroyWindow("Window");

	return 0;
}