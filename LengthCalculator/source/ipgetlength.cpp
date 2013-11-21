/**
* Length Detection Application
*
* Core Application for Length Measurement
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

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>
#include"ipFileRead.h"
#include"ipCapture.h"
#include"ipBGFrameDiff.h"
#include"ipMorph.h"
#include"ipTransform.h"
#include"ipBlobOperations.h"
#include"demo.h"
#include "ipContourScanner.h"

using namespace std;
using namespace cv;

const int Debug = 1;

// Global Variables with Data from File
CvPoint ClickPoint[4];
CvPoint GridPoint[4];
int iPixelDistance;					// Variable for Storing reference Distance in Pixels
int iPhysicalDistance;				// Variable for saving physical Distance in Meters

const char msg[]	= "Rajdeep Info Techno. Pvt Ltd.\nLength detection Application\nWritten By: Inderpreet Singh\n";

float iDemo = 0;

int ipLengthApplication(char* SourcePath)
{
	char vidFile[]	= "..\\14-03-24.avi";	// File Name
	char rtsp_src[] = "rtsp://192.168.1.110:554";
	char w1[]		= "Source Video";
	char w2[]		= "Captured Frame";
	char w3[]		= "Foreground Mask Image";
	float LengthOffset = 260;			// Length Offset caused due to position of camera and showing Face

	//-------------------------------------------------------------------------
	/// Step 1: Read skewed points from file
	ipReadFile();						// Read the points from file
	if(Debug) ipShowPoints();					// Debugging Stuff print to console

	// ------------------------------------------------------------------------
	// Step 2: Wait for Frame Capture condition -  Generate Background possibly
	// Get frame of interest
	CvCapture* Capture = NULL;					// Capture Device Pointer
	Capture = ipGetCapture(vidFile);			// Overloaded Function for getting capture source
	cvQueryFrame( Capture );					// Sometimes needed to get correct data

	IplImage* frame = cvQueryFrame( Capture );	// Save frame in variable
	IplImage* prev	= cvCloneImage( frame );	// Copy the image to buffer
	CvSize sz		= cvGetSize(frame);			// Get Size of frame
	// single Channel Images
	IplImage* gray1	= cvCreateImage(sz , IPL_DEPTH_8U, 1);	// Memory
	IplImage* gray2	= cvCreateImage(sz , IPL_DEPTH_8U, 1);	// Memory
	IplImage* diff	= cvCreateImage(sz , IPL_DEPTH_8U, 1);	// Memory

	while(!frame)								// Wait till valid frames start
		frame = cvQueryFrame( Capture );
	cvCvtColor(frame, gray1, CV_RGB2GRAY);		// Convert to grayscale
	AllocateImages(frame);						// Allocate Image Mem
	printf("\nIn Demo, Press anykey To Capture when Car is in Frame");		//Print Message
	char c	= NULL;
	float framenumber = 0;
	CvScalar avrg;
	while(1)									// SuperLOOP till spacebar breaks
	{
		frame = cvQueryFrame( Capture );		// Get one frame from source
		if( !frame)	break;						// break if blank frame
		accumulateBG(frame);					// Accumulate Background
		cvShowImage(w1, frame);					// Update Original Video feed

		//cvCopy(gray1, gray2, 0);				// copy to gray2
		//cvCvtColor( frame, gray1, CV_RGB2GRAY);	// copy new to gray1
		//cvAbsDiff(gray1, gray2, diff);
		//avrg = cvAvg(diff, 0);				// Average of all pixels

		c = cvWaitKey(5);						// Check for SPACEBAR
		iDemo = Filter1(c);						// passthrough
		if(		c == ' ' 
			|| iDemo>0
			//||	framenumber == 540
			// ||	avrg.val[0] < 20							// POSSIBLY CHECK FOR STOPPED VEHICLE
			) 
			break; // Select frame on spacebar or frame 540

		framenumber++;
	}
	// At this point the frame with object is in frame var	

	// ------------------------------------------------------------------------
	// Step 3: Generate the Background Model
	// 
	createModelFromStats();						// Create Model
	
	// ------------------------------------------------------------------------
	// Step 4: Subtract vehicle from background
	// cvSub(frame, Background, Difference);
	//
	//CvSize sz			= cvGetSize(frame);		// Get Size of frame
	IplImage* Imask		= cvCreateImage(sz, IPL_DEPTH_8U, 1);	// Memory For Mask
	backgroundDiff(frame, Imask);				// Get the Mask in Imask
	ipMorphClean( Imask );						// Morphological Cleanup
	if(Debug)
	{
		cvNamedWindow(w3);							// Create Window to show the mask
		cvShowImage(w3, Imask);
		cvWaitKey(3000);							// Wait for 3 seconds
		cvDestroyWindow(w3);						// Destroy the Mask Frame Window
	}

	// ------------------------------------------------------------------------
	// Step 5: Transform image persepective
	Mat mImaskVar;
	Mat *mImaskPtr = &mImaskVar;
	ipTransform(Imask, mImaskPtr);
	if(Debug)
	{
		namedWindow("Transformed", CV_WINDOW_AUTOSIZE );	
		imshow(		"Transformed", mImaskVar );
	}
	cvWaitKey(3000);

	// ------------------------------------------------------------------------
	// Step 6: Draw Bounding Box and get horizontal pixel length
	// 
	int WidthInPixels = ipBoxContours(mImaskPtr);			// C++ Implementation of the function

	if(Debug)
	{
		namedWindow("Contours", CV_WINDOW_AUTOSIZE );	
		imshow(		"Contours", mImaskVar );
		cvWaitKey(3000);
	}
	
	// Step 7: Calculate Length in Meters
	//
	// Formula: ScaleFactor = MarkerDistMeters(iPhysicalDistance)/PixelDistance(iPixelDistance)
	// Formula: LengthMeters = Pixels*ScaleFactor
	//
	float ScaleFactor	= 0;
	float LengthInMeters = 0;
	ScaleFactor		= ((float)iPhysicalDistance)/((float)iPixelDistance);
	LengthInMeters	= ((float)WidthInPixels)*ScaleFactor;
	LengthInMeters	= LengthInMeters-LengthOffset;
	{
		cout<<endl<<"Length in Pixels "<< WidthInPixels;
		cout<<endl<<"Length In CM = "<<LengthInMeters<<endl;
	}

	// Step 8: Cleanup
	cvReleaseCapture( &Capture );		// Release Capture Device
	cvReleaseImage( &Imask);			// Release memory for the mask image
	cvReleaseImage( &diff);
	DeallocateImages();					// Call Deallocate for all BG related stuff
	return 0;
}

