/**
* Image Transformation Function for perspective Shift
*
* A utility for calibrating the Camera Based Length Detection System
* by geting the transformation matrix using 4 point calibration
*
*
* @package		Persepective Transform
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
#include"iptransform.h"

using namespace std;
using namespace cv;

extern CvPoint	ClickPoint[4];
extern CvPoint	GridPoint[4];
extern int		iPixelDistance;					// Variable for Storing reference Distance in Pixels
extern int		iPhysicalDistance;				// Variable for saving physical Distance in Meters
extern const int Debug;

void ipTransform(IplImage* image, void *memloc)
{
	// C++ Implementation
	Mat Image(image);						// Convert the IPL Image to Mat type
	Mat Transformed;
	Point2f src[] = {ClickPoint[0],ClickPoint[1],ClickPoint[2],ClickPoint[3]};
	Point2f dst[] = {GridPoint[0],GridPoint[1],GridPoint[2],GridPoint[3]};
	Mat Transform = getPerspectiveTransform(src, dst);
	warpPerspective(Image, Transformed, Transform, Image.size(),
					INTER_LINEAR, BORDER_CONSTANT);
	Mat *localptr = (Mat*)memloc;
	*localptr = Transformed.clone();
}

int TransformMain(void)
{
	const char* FileName = "D:/Sandbox/VS2010/perspectiveTfn/source/cap1.jpg";
	// Read the Image
	Mat image, transformed;
	image = imread(FileName, CV_LOAD_IMAGE_COLOR);
	if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );  
	

	// Get the points to 
	Point2f p1(49, 80), p2(58, 190), p3(701, 337), p4(718, 145);
	Point2f src[] = {p1, p2, p3, p4};

	Point2f p5(49, 80), p6(58, 190), p7(724, 190), p8(724, 80);
	Point2f dst[] = {p5, p6, p7, p8};

	Mat transform = getPerspectiveTransform(src, dst);

	warpPerspective(image, transformed, transform, image.size(),
					INTER_LINEAR, BORDER_CONSTANT);
	
	namedWindow("Output", CV_WINDOW_AUTOSIZE);
	
	imshow("Output", transformed);
	imwrite("transformed.jpg", transformed);

	waitKey(0);
	return 0;
}