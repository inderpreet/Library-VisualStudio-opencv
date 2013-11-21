/**
* Configuration and Calibration Utility for Length Detection Application
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
#include"drawpoints.h"
#include"ipWriteFile.h"
#include"ipcapture.h"


using namespace std;
using namespace cv;

const char Msg[] = "Calibration Utility By: Inderpreet Singh\n\nEmail: inderpreet@it.rajdeepgroup.com\n\n";
/*			(2)							
*			 |							(3)
*			 |							 |
*			 |			Lane->			 |
*			 |							 |
*			(1)							(4)	
*/

extern CvPoint ClickPoint[iMaxPoints];
extern CvPoint GridPoint[iMaxPoints];
extern int iPointIndex;					// Point Index
const char* WinName = "Click on the Four point IN Clockwise SEQUENCE PLEASE Starting with bottom left Corner!!!!";
char* ConfigFile = "..\\config.bin";
int iPixelDistance = 0;					// Variable for Storing reference Distance in Pixels
int iPhysicalDistance = 0;				// Variable for saving physical Distance in Meters

int main(int argc, char *argv[])
{
	char vidFile[]	= "..\\14-03-24.avi";	// File Name
	char rtsp_src[] = "rtsp://192.168.1.110:554";
	printf(Msg);
	iPointIndex = 0;					// Set to zero
	/*
	* Step 1: Capture the image from camera and show in window
	* image is saved in IplImage* frame
	*
	*/
	CvCapture* capture = NULL;
	capture = ipGetCapture(vidFile);			// Overloaded Function for getting capture source
	IplImage* frame = cvQueryFrame( capture );	// Save frame in variable
	while(!frame)								// Wait till valid frames start
		frame = cvQueryFrame( capture );
	printf("\nPress SPACEBAR To Pause When Ready");		//Print Message
	cvNamedWindow(WinName, CV_WINDOW_AUTOSIZE);	
	char c = NULL;
	while(1)									// SuperLOOP till spacebar breaks
	{
		frame = cvQueryFrame( capture );		// Get one frame from source
		if( !frame)	break;						// break if blank frame
		cvShowImage(WinName, frame);					// Update Original Video feed
		c = cvWaitKey(3);						// Check for SPACEBAR
		if(	c == ' ' ) 
			break; // Select frame on spacebar or frame 540
	}
	// ------------------------------------------------------------------------
	/* 
	* Step 2: Now get the four points for calibration
	* 
	* Setup Mouse callback to the window which does the point saving
	* 
	*/
	IplImage* temp = cvCloneImage( frame );
	// Setup the mouse callback getting events
	cvSetMouseCallback( WinName,				// The Window with the image to clicks from
						ip_mouse_callback,		// The function which uses the events
						(void*) frame);			// the Image in the frame
	// Loop for updating the display with image with circles
	while( iPointIndex < iMaxPoints )			// Keep plotting the points
	{
		cvCopy( frame, temp );					// Source ; Dest; Mask
		cvShowImage( WinName, temp );			// Window; image
		if( cvWaitKey( 15 )==27 ) 
			break;
	}
	cvCopy( frame, temp );						// Source ; Dest; Mask
	cvShowImage( WinName, temp );				// for 4th point plotting
	/* 
	* At this point got the 4 points in the ClickPoint[] array.
	*/
	cout<<endl<<"In Buffer are"<<endl;
	for(int eye=0; eye<4; eye++)				// Debug Information
		cout<<ClickPoint[eye].x<<"\t"<<ClickPoint[eye].y<<endl;	
	//-------------------------------------------------------------------------
	/* 
	* Step 3:  Find Bottom left corner - Assuming it is at index 0 right now
	*/
	
	//-------------------------------------------------------------------------
	/* 
	* Step 4:  Get New Points with botton Left as anchor
	*/
	for (int eye = 0; eye < iMaxPoints; eye++)	// Copy the points
		GridPoint[eye] = ClickPoint[eye];			

	// Step 1: GridPoint[0] as it is for reference
	// Step 2: Move GridPoint[1] to correct the x as same as point 0
	GridPoint[1].x = GridPoint[0].x;
	// Step 3: Move GridPoint[2]'s y to be the same as Point 1
	GridPoint[2].y = GridPoint[1].y;
	// Step 4: Find the new point [3]
	GridPoint[3].x = GridPoint[2].x;
	GridPoint[3].y = GridPoint[0].y;


	//GridPoint[0].x = (GridPoint[0].x<GridPoint[1].x) ? (GridPoint[0].x):(GridPoint[1].x);			// Correct the tilt by correcting x
	//GridPoint[0].y = (GridPoint[0].y>GridPoint[3].y) ? (GridPoint[0].y):(GridPoint[3].y);
	//
	//GridPoint[1].x = GridPoint[0].x;
	//GridPoint[1].y = (GridPoint[1].y<GridPoint[2].y)?(GridPoint[1].y):(GridPoint[2].y);

	//GridPoint[2].x = (GridPoint[2].x>GridPoint[3].x)?(GridPoint[2].x):(GridPoint[3].x);
	//GridPoint[2].y = GridPoint[1].y;

	//GridPoint[3].x = GridPoint[2].x;			// Derive the fourth Point
	//GridPoint[3].y = GridPoint[0].y;
												// Calculate the Distance in Px	
	iPixelDistance = GridPoint[3].x - GridPoint[0].x;
	
	ipDrawGrid(frame);
	cvCopy( frame, temp );	
	cvShowImage( WinName, temp );
	cvWaitKey(5000);
	cvDestroyWindow( WinName );
	//------------------------------------------------------------------------
	/*
	* Step 5: Get user Input on Physical distance
	*/
	cout<<endl<<"Please enter Distance in CM:";
	cin>>iPhysicalDistance;

	//------------------------------------------------------------------------
	/*
	* Step 6: Write data to file
	*/
	ipWriteFile(ConfigFile);					// Write To ConfigFile

	// ------------------------------------------------------------------------
	// Cleanup
	cvReleaseCapture( &capture );		// Section Cleanup - Release the Camera

	return 0;
}
