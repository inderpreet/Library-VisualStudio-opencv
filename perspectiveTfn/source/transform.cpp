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
#include"transform.h"
#include"drawpoints.h"

extern CvPoint ClickPoint[4];				// Arry of points
extern CvPoint GridPoint[4];				// Arry of Output Points
extern int iPointIndex;							// Index variable for Circ buffer

using namespace std;
using namespace cv;

int ipPerspectiveTransform(IplImage* image, void *transformed)
{
	// C++ Implementation
	Mat Image(image);						// Convert the IPL Image to Mat type
	Mat Transformed;
	Point2f src[] = {ClickPoint[0],ClickPoint[1],ClickPoint[2],ClickPoint[3]};
	Point2f dst[] = {GridPoint[0],GridPoint[1],GridPoint[2],GridPoint[3]};
	Mat Transform = getPerspectiveTransform(src, dst);
	warpPerspective(Image, Transformed, Transform, Image.size(),
					INTER_LINEAR, BORDER_CONSTANT);
	Mat *localptr = (Mat*)transformed;
	*localptr = Transformed.clone();
	Mat transform = getPerspectiveTransform(src, dst);
	return 0;
}