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
const float APP_VER = 1.1;
const char msg[]	= "Rajdeep Info Techno. Pvt Ltd.\nLength detection Application\nWritten By: Inderpreet Singh\n\n";

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>
#include"ipBGFrameDiff.h"
#include"ipgetlength.h"

#define ESC 27

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
	
	char vidFile[]	= "..\\14-03-24.avi";	// File Name
	char rtsp_src[] = "rtsp://192.168.1.110:554";
	printf(msg);
	printf("Version %f", APP_VER);
	//-------------------------------------------------------------------------
	//
	// Check the number of arguments and if number of arguments
	// Validate the arguments and use the rtsp address provided
	// If using camera source etc etc

	// ------------------------------------------------------------------------
	//
	// Length app with file or rtsp Source
	ipLengthApplication(vidFile);

	cvWaitKey(0);
	return 0;
}