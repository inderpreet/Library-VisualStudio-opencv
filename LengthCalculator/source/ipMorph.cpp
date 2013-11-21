/**
* Morphological Operation Based Image cleanup to get blobs
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
#include<vector>
#include<highgui.h>
#include"ipMorph.h"


const int Debug = 0;

void ipMorphClean(IplImage* Imask)
{
	// Threshold 
	//cvThreshold(Imask, Imask, 25, 255, CV_THRESH_BINARY);
	if(Debug)
	{
		cvNamedWindow("Cleanup");
		cvShowImage("Cleanup", Imask);
		cvWaitKey(0);
	}
	// Opening opening operation
	cvErode(	Imask,					// Input Image
				Imask,					// Output Image
				0,						// Conversion Kernel
				1);						// Iterations	
	if(Debug)
	{
		cvShowImage("Cleanup", Imask);
		cvWaitKey(0);
	}
	cvDilate(	Imask,
				Imask,
				0,
				1);
	if(Debug)
	{
		cvShowImage("Cleanup", Imask);
		cvWaitKey(0);
	}
	//-------------------------------------------------------------------------
	// Closing Operation
	cvDilate(	Imask,
				Imask,
				0,
				2);
	if(Debug)
	{
		cvShowImage("Cleanup", Imask);
		cvWaitKey(0);
	}
	cvErode(	Imask,					// Input Image
				Imask,					// Output Image
				0,						// Conversion Kernel
				2);						// Iterations
	if(Debug)
	{
		cvShowImage("Cleanup", Imask);
		cvWaitKey(0);
	}
}