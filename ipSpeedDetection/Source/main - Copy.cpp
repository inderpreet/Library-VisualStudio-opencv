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
#define USEC_PER_SEC 1000000L

CvScalar target_color[4] = { // in BGR order 
		{{   0,   0, 255,   0 }},  // red
		{{   0, 255,   0,   0 }},  // green
		{{ 255,   0,   0,   0 }},  // blue
		{{   0, 255, 255,   0 }}   // yellow
};

// ****************************************************************************

int main(int argc, char *argv[])
{
/* ******************* Feature Detection Stuff *******************************/	
	IplImage tempIplVar;
	IplImage* curr_frame = 0; // current video frame
	IplImage* gray_frame = 0; // grayscale version of current frame
	int w, h; // video frame size
	IplImage* eig_image = 0;
	IplImage* temp_image = 0;
	const int MAX_CORNERS = 10;
	CvPoint2D32f corners[MAX_CORNERS] = {0};
	int corner_count = MAX_CORNERS;
	double quality_level = 0.1;
	double min_distance = 10;
	int eig_block_size = 3;
	int use_harris = false;


	int c;
	Mat tempFrame;
	ipCaptureDevice Cap1;
	tempFrame = Cap1.ipGetFrame();
	
	namedWindow("Window");
	namedWindow("Corners");

	while(1)
	{
		tempFrame = Cap1.ipGetFrame();
		imshow("Window", tempFrame);

/* ******************* Feature Detection Stuff *******************************/
		tempIplVar = tempFrame;
		curr_frame = &tempIplVar;
		w = tempFrame.size().width;
		h = tempFrame.size().height;
		
		// Convert the frame image to grayscale
		if( ! gray_frame ) {
			//fprintf(stderr, "Allocate gray_frame\n");
			int channels = 1;
			gray_frame = cvCreateImage(
					cvGetSize(curr_frame), 
					IPL_DEPTH_8U, channels);
		}
		cvCvtColor(curr_frame, gray_frame, CV_BGR2GRAY);
		
		// ==== Allocate memory for corner arrays ====
		if ( !eig_image) {
			//fprintf(stderr, "Allocate eig_image\n");
			eig_image = cvCreateImage(cvSize(w, h),
					IPL_DEPTH_32F, 1);
		}
		if ( !temp_image) {
			//fprintf(stderr, "Allocate temp_image\n");
			temp_image = cvCreateImage(cvSize(w, h),
					IPL_DEPTH_32F, 1);
		}
		
		cvGoodFeaturesToTrack(gray_frame,
				eig_image,                    // output
				temp_image,
				corners,
				&corner_count,
				quality_level,
				min_distance,
				NULL,
				eig_block_size,
				use_harris);
		cvScale(eig_image, eig_image, 100, 0.00);
		for( int i = 0; i < corner_count; i++) {
			int radius = h/25;
			cvCircle(curr_frame,
					cvPoint((int)(corners[i].x + 0.5f),(int)(corners[i].y + 0.5f)),
					radius,
					target_color[0]);
		}
		cvShowImage("Corners", curr_frame);

		c = cvWaitKey(10); // wait 10 ms or for key stroke
		if(c >=27)
			break;
	}
	
	cvDestroyWindow("Window");

	return 0;
}