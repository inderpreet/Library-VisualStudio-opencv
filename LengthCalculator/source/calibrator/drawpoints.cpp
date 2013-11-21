/**
* Drawing the Points
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

using namespace std;
using namespace cv;

/// Variables to store the 
CvPoint ClickPoint[iMaxPoints];				// Arry of points
CvPoint GridPoint[iMaxPoints];				// Arry of Output Points
int iPointIndex;							// Index variable for Circ buffer
int const Radius = 10;						// Radius for the circle to be drawn

void ipDrawGrid (IplImage *img)
{
	for (int eye = 0; eye<iMaxPoints; eye++)
		cvCircle(img, GridPoint[eye], Radius, cvScalar(0x00, 0x00, 0xff));
}
void draw_point( IplImage* img)
{
	cvCircle( img, ClickPoint[iPointIndex], Radius, cvScalar(0xff,0x00,0x00));
}

/// Implement mouse callback
void ip_mouse_callback( int event, int x, int y, int flags, void* param ){
	IplImage* image = (IplImage*) param;

	switch( event ){
		case CV_EVENT_LBUTTONDOWN:			// Left Button Click
			ClickPoint[iPointIndex].x = x;
			ClickPoint[iPointIndex].y = y;
			draw_point(image);
			cout<<ClickPoint[iPointIndex].x<<"\t"<<ClickPoint[iPointIndex].y<<endl;
			iPointIndex++;					// Increment Index
			/*if(iPointIndex >= iMaxPoints)
				iPointIndex = 0;*/
			break;							// Break the switch-Case

		case CV_EVENT_MOUSEMOVE:			// Mouse Move
			break;		

		case CV_EVENT_LBUTTONUP:			// Left Button Up
			/// Debugging Stuff
			//cout<<endl<<"In Buffer are"<<endl;
			//for(int eye=0; eye<4; eye++)
			//	cout<<ClickPoint[eye].x<<"\t"<<ClickPoint[eye].y<<endl;
			break;
	}
}

/// Demo Application Function
void DrawPointsMain(void)
{
	const char* name = "Click Point Example";
	// Set the Location of the file to be read
	const char* FileName="D:/Sandbox/VS2010/perspectiveTfn/source/cap1.jpg";

	IplImage* image = cvLoadImage( FileName );	// IPL Image	
	IplImage* temp = cvCloneImage( image );
	cvNamedWindow( name );

	// Set up the callback
	cvSetMouseCallback( name, ip_mouse_callback, (void*) image);

	// Main loop
	while( 1 ){
		cvCopy( image, temp );			// Source ; Dest; Mask
		cvShowImage( name, temp );		// Window; image
		if( cvWaitKey( 15 )==27 ) 
			break;
	}

	cvReleaseImage( &image );
	cvReleaseImage( &temp );
	cvDestroyWindow( name );

}

