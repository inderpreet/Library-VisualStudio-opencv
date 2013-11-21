/**
* Background Generation Using FrameDifferencing Method
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
#include"ipBGFrameDiff.h"

#define ESC 27

using namespace std;
using namespace cv;

//-----------------------------------------------------------------------------
// Global Variables -  Not Thread Safe I think or something like that!
// Floating Point, 3 Channel Images
IplImage *IavgF, *IdiffF, *IprevF, *IhiF, *IlowF;
IplImage *Iscratch, *Iscratch2;
// Floating Pt, 1Ch images
IplImage *Igray1, *Igray2, *Igray3;
IplImage *Ilow1, *Ilow2, *Ilow3;
IplImage *Ihi1, *Ihi2, *Ihi3;
// Byte, 1ch image
IplImage *Imaskt;
IplImage *Imask;
// Leanring Image Count
float Icount;

/// Memory Allocation Function for all images
/// All memory to global variables
/// @param	IplImage pointer of Captured Image/frame
void AllocateImages(IplImage* I)
{
	CvSize sz	= cvGetSize(I);
	IavgF		= cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IdiffF		= cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IprevF		= cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IhiF		= cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IlowF		= cvCreateImage(sz, IPL_DEPTH_32F, 3);
	Iscratch	= cvCreateImage(sz, IPL_DEPTH_32F, 3);
	Iscratch2	= cvCreateImage(sz, IPL_DEPTH_32F, 3);

	Igray1		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray2		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray3		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow1		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow2		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow3		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi1		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi2		= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi3		= cvCreateImage(sz, IPL_DEPTH_32F, 1);

	Imaskt		= cvCreateImage(sz, IPL_DEPTH_8U, 1);
	Imask		= cvCreateImage(sz, IPL_DEPTH_8U, 1);
	Icount		= 0.00001;	// Protection against div by zero

	cvZero(IavgF);
	cvZero(IdiffF);
	cvZero(IprevF);
	cvZero(IhiF);
	cvZero(IlowF);
}

/// Background Accumulation Function
/// 
/// @param	IplImage pointer of Captured Image/frame
void accumulateBG(IplImage *I)
{
	static int first = 1;
	cvCvtScale( I, Iscratch, 1,0);
	if(!first)
	{
		cvAcc(Iscratch, IavgF);
		cvAbsDiff(Iscratch, IprevF, Iscratch2);
		cvAcc( Iscratch2, IdiffF );
		Icount += 1.0;
	}
	first = 0;
	cvCopy(Iscratch, IprevF);
}

/// Function used internally for setting the HIGH Threshold value
void setHighThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);
	cvAdd(Iscratch, IavgF, IhiF);
	cvSplit( IhiF, Ihi1, Ihi2, Ihi3, 0);
}

/// Function used internally for setting the LOW Threshold value
void setLowThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);
	cvSub(IavgF, Iscratch, IlowF);
	cvSplit( IlowF, Ilow1, Ilow2, Ilow3, 0);
}

/// Function to use the Accumulated Images to produce a stable background model
/// Just averaging stuff and set the threshold values here
void createModelFromStats(void)
{
	cvConvertScale(IavgF, IavgF, (double)(1.0/Icount) );
	cvConvertScale(IdiffF, IdiffF, (double)(1.0/Icount) );

	// Make sure diff is always somehthing
	cvAddS(IdiffF, cvScalar( 1.0, 1.0, 1.0), IdiffF);
	setHighThreshold(7.0);
	setLowThreshold( 6.0);
}

/// Function to Extract the Background using OR Operations on each color layer
/// Mask inversion done here
void backgroundDiff( IplImage *I, IplImage *Imask)
{
	cvCvtScale(I, Iscratch, 1, 0);	// To Float
	cvSplit(Iscratch, Igray1, Igray2, Igray3, 0);
	//Channel 1
	cvInRange(Igray1, Ilow1, Ihi1, Imask);

	//Channel 2
	cvInRange(Igray2, Ilow2, Ihi2, Imaskt);
	cvOr(Imask, Imaskt, Imask);

	//Channel 3
	cvInRange(Igray3, Ilow3, Ihi3, Imaskt);
	cvOr(Imask, Imaskt, Imask);

	// Finally Invert the results
	cvSubRS(Imask, cvScalar(255,255, 255) , Imask);
}

// Release All the IMages initially created
void DeallocateImages(void)
{
	cvReleaseImage( &IavgF);
	cvReleaseImage( &IdiffF);
	cvReleaseImage( &IprevF);
	cvReleaseImage( &IhiF);
	cvReleaseImage( &IlowF);
	cvReleaseImage( &Ilow1);
	cvReleaseImage( &Ilow2);
	cvReleaseImage( &Ilow3);
	cvReleaseImage( &Ihi1);
	cvReleaseImage( &Ihi2);
	cvReleaseImage( &Ihi3);
	cvReleaseImage( &Iscratch);
	cvReleaseImage( &Iscratch2);
	cvReleaseImage( &Igray1);
	cvReleaseImage( &Igray2);
	cvReleaseImage( &Igray3);
	cvReleaseImage( &Imaskt);
	cvReleaseImage( &Imask);
}

/// Demo Application Function
void ipBGDiffDemo(void)
{
	char filename[]	= "..\\vid.avi";
	const char w1[]	= "Original Video";
	const char w2[] = "Difference";
	//-------------------------------------------------------------------------
	// Video Capture
	CvCapture* capture	= cvCreateFileCapture(filename);// Capture Source
	IplImage* frame		= cvQueryFrame( capture );		// Get Next Frame
	CvSize sz			= cvGetSize(frame);			// Get Size of frame
	IplImage* Imask		= cvCreateImage(sz, IPL_DEPTH_8U, 1);
	//-------------------------------------------------------------------------
	// Background Detection
	AllocateImages(frame);					// Allocate Image Mem
	cvNamedWindow(w1);
	char c=0;
	// run till user presses spacebar
	while(1)							
	{
		frame = cvQueryFrame( capture );	// Get Next Frame
			if( !frame)	break;
		cvShowImage(w1, frame);				// Show that Frame
		accumulateBG(frame);				// Accumulate Background

		c = cvWaitKey(33);					// Press SPACEBAR when vehicle is in frame
		if(c ==' ') break;
	}
	createModelFromStats();					// Create Model
	backgroundDiff(frame, Imask);			// Get the Mask
	cvNamedWindow(w2);						// Window for Difference
	cvShowImage(w2, Imask);					// Show mask
	cvErode(	Imask,					// Input Image
				Imask,					// Output Image
				0,						// Conversion Kernel
				1);						// Iterations
	cvWaitKey(0);
	cvShowImage(w2, Imask);					// Show mask
	cvDilate(	Imask,
				Imask,
				0,
				2);
	cvWaitKey(0);
	cvShowImage(w2, Imask);					// Show mask
	//-------------------------------------------------------------------------
	// Cleanup
	c=cvWaitKey(0);							// Wait for keypress
	cvReleaseCapture(&capture);
	DeallocateImages();
	cvDestroyAllWindows();
}
