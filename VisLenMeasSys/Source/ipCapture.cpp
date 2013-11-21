/**
* Capture Function for Configuration and Calibration Utility for 
* Length Detection Application
*
* A utility for calibrating the Camera Based Length Detection System
* by geting the transformation matrix using 4 point calibration
*
*
* @package		Length Measurement Application
* @author		Inderpreet Singh
* @copyright	Rajdeep Info Techno Pvt. Ltd.
* @since		Version 1.1
* @param		None
* @returns		None
*
*/

// ----------------------------------------------------------------------------

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>
#include"ipCapture.h"

#define FILE	0
#define CAM		1


void ipGetRTSP()
{

}

void ipVideoPlayerDemo(void)
{
	char filename[]	= "..\\vid.avi";
	const char w1[]	= "Original Video";
	//-------------------------------------------------------------------------
	// Capture or get get video from file
	//
	CvCapture* capture = cvCreateFileCapture(filename);
	IplImage* frame;
	cvNamedWindow(w1);
	char c;
	while(1)
	{
		frame = cvQueryFrame( capture );
		if( !frame)	break;
		cvShowImage(w1, frame);
		c = cvWaitKey(33);
		if(c == 27) break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow(w1);
}

CvCapture* ipGetCapture(void)
{
	CvCapture* capture = NULL;
	for (int i = -1; i < 100; i++) 
	{
        capture = cvCaptureFromCAM( i );   
        if( !capture ) 
		{
            fprintf( stderr, "ERROR: capture is NULL \n" ); 
			//return -1;
        } 
		else
            break;
    }
	return capture;
}

CvCapture* ipGetCapture(char* Filename)
{
	CvCapture* capture = cvCreateFileCapture(Filename);
	if( !capture ) 
	{
        fprintf( stderr, "ERROR: capture is NULL \n" ); 
		//return -1;
    }
	return capture;
}
