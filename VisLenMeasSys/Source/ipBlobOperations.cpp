/**
* Opearation Functions on Blog
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
#include"ipBlobOperations.h"

using namespace std;
using namespace cv;

const int Debug = 0;
extern Mat ipTransformedImage;				// Variable Containing the Transformed Mask

// Function Implementation of Drawing Box around blobs in mask
int ipBoxContours(void *Imask)
{
	// Find contours in the Mask
	//
	int idx = 0;
	Mat *localptr = (Mat*) Imask;
	Mat ipTransformedImage = localptr->clone();		// Create Local Copy
	RNG rng(12345);
	vector<vector<Point> > contours;				// Extracted Contours
	//vector<vector<Point>  > allContours;			// Larger contours
	vector<vector<Point> > approx;					// Approximations
	vector<Vec4i> hierarchy;
	findContours(	ipTransformedImage, 
					contours, 
					hierarchy, 
					CV_RETR_EXTERNAL,		// Only outer contours
					CV_CHAIN_APPROX_SIMPLE, 
					Point(0, 0) );

	//-------------------------------------------------------------------------
	//
	// Approximate Polygons using Duglas Puker
	vector<vector<Point> > contours_poly( contours.size() );	// Vector of Polys
	vector<Rect> boundRect( contours.size() );					// Vector of Rects
	
	for( int i = 0; i < contours.size(); i++ )
     { 
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true ); // DP
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );		 // Generate Bounding Boxes
     }

	// ------------------------------------------------------------------------
	// Find Largest Blob Index
	//
	double area;
	int iLargest=0;								// Largest Blob Index
	double MaxArea = 0;
	for (int i = 0; i< contours_poly.size(); i++ )
	{
		area = contourArea( contours_poly[i] );
		if(area>MaxArea)
		{
			iLargest = i;
			MaxArea = area;
		}
	}
	//}
	// ------------------------------------------------------------------------
	/// Draw All contours and Blobs
	//
	Mat drawing = Mat::zeros( ipTransformedImage.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
     }

	// -------------------------------------------------------------------------
	// Now Calc the Bounding Rect LARGEST
	Rect rect = boundingRect(contours[iLargest]);
	Point pt1, pt2;
	pt1.x = rect.x;
	pt1.y = rect.y;
	pt2.x = rect.x + rect.width;
	pt2.y = rect.y + rect.height;
	
	// Draws the rect in the original image and show it
	Mat Largest  = Mat::zeros( ipTransformedImage.size(), CV_8UC3 );
	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	drawContours( Largest, contours_poly, iLargest, color, 1, 8, vector<Vec4i>(), 0, Point() );
	rectangle(Largest, pt1, pt2, CV_RGB(255,0,0), 1);
	if(Debug)
	{
		namedWindow("Largest Blob");
		imshow("Largest Blob", Largest);
		cvWaitKey(5000);
	}

	*localptr = drawing.clone();	// copy to global Variable
	return rect.width;
}
