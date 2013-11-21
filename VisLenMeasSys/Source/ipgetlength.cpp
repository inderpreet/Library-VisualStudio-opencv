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
#include"ipIncludes.h"

using namespace std;
using namespace cv;

// Global Variables with Data from File
CvPoint ClickPoint[4];
CvPoint GridPoint[4];
int iPixelDistance;					// Variable for Storing reference Distance in Pixels
int iPhysicalDistance;				// Variable for saving physical Distance in Meters

const char msg[]	= "Rajdeep Info Techno. Pvt Ltd.\nLength detection Application\nWritten By: Inderpreet Singh\n";

float iDemo = 0;

int ipLengthApplication(char* SourcePath)
{
	char w1[]		= "Source Video";
	char w2[]		= "Captured Frame";
	char w3[]		= "Foreground Mask Image";
	float LengthOffset = 311;			// Length Offset caused due to position of camera and showing Face
	//-------------------------------------------------------------------------
	/// Step 1: Read skewed points from file
	ipReadFile();						// Read the points from file
	if(Debug) ipShowPoints();					// Debugging Stuff print to console

	// ------------------------------------------------------------------------
	// Step 2: Wait for Frame Capture condition -  Generate Background possibly
	// Get frame of interest
	//
	// Setup variables for frame capture etc
	CvCapture* Capture = NULL;					// Capture Device Pointer
	Capture = ipGetCapture(SourcePath);			// Overloaded Function for getting capture source
	cvQueryFrame( Capture );					// Sometimes needed to get correct data
	// Get frame and then its size
	IplImage* frame = cvQueryFrame( Capture );	// Save frame in variable
	while(!frame)								// Wait till valid frames start
		frame = cvQueryFrame( Capture );
	CvSize sz		= cvGetSize(frame);			// Get Size of valid frame
	if(Debug) 
	{
		cvNamedWindow("Video");
		cvShowImage("Video", frame);
	}
	// Now wait and Generate BG till there is capture Event
	IplImage* Background = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	char c	= NULL;
	while(!ipCaptureEvent())									// SuperLOOP till spacebar breaks
	{
		frame = cvQueryFrame( Capture );		// Get one frame from source
		if( !frame)	break;						// break if blank frame
		ipRunningAvg1(frame, Background);
		if(Debug) cvShowImage("Video", frame);
	}
	// At this point the frame with object is in frame var	

	// ------------------------------------------------------------------------
	// Step 3: Generate the Background Model
	// 

	// ------------------------------------------------------------------------
	// Step 4: Subtract vehicle from background
	// cvSub(frame, Background, Difference);
	//
	IplImage* Imask		= cvCreateImage(sz, IPL_DEPTH_8U, 1);	// Memory For Mask
	ipGetBgMask1(frame, Background, Imask);
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
	if(LengthInMeters<0) LengthInMeters = 0;
	{
		cout<<endl<<"Length in Pixels "<< WidthInPixels;
		cout<<endl<<"Length In CM = "<<LengthInMeters<<endl;
	}

	// Step 8: Cleanup
	cvReleaseCapture( &Capture );		// Release Capture Device
	cvReleaseImage( &Imask);			// Release memory for the mask image
//	cvReleaseImage( &diff);
	DeallocateImages();					// Call Deallocate for all BG related stuff
	return 0;
}

