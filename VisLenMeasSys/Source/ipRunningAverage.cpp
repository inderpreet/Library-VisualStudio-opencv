/**
* Running Average Method for background generation
*
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

using namespace std;
using namespace cv;

// Function for generating running average
// @params 3Ch current image 8U, 1Ch avg image 32F
void ipRunningAvg1(IplImage* current, IplImage* avg)
{
	const float aplha = 0.01;
	CvSize sz		= cvGetSize(current);			// Get Size of frame
	IplImage* BW	= cvCreateImage(sz , IPL_DEPTH_8U, 1);	// Memory
	IplImage* BW32	= cvCreateImage(sz , IPL_DEPTH_32F, 1);	// Memory
	// Convert image to BW
	cvCvtColor(current, BW, CV_BGR2GRAY );
	// Upscale to 32 Floating point
	cvCvtScale(BW, BW32, 1, 0);
	cvRunningAvg(BW32, avg, aplha, NULL);
}

// Function to find the foreground mask
// @params		8U 3ch Image,		32F 1ch Image,	32F 1ch Image
void ipGetBgMask1(IplImage* current, IplImage* avg, IplImage* mask)
{
	CvSize sz		= cvGetSize(current);
	IplImage* BW	= cvCreateImage(sz, IPL_DEPTH_8U, 1);
	IplImage* BW32	= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	IplImage* temp	= cvCreateImage(sz, IPL_DEPTH_32F, 1);
	// Convert to BW
	cvCvtColor(current, BW, CV_BGR2GRAY);
	// Upscale to BW32
	cvCvtScale(BW, BW32, 1,0);
	cvAbsDiff(BW32, avg, temp);
	cvCvtScale(temp, mask, 1, 0);
}