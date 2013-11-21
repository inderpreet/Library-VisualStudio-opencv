/**
* Contour Scanner Functions
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

using namespace std;
using namespace cv;

// Appox Threshhold- The bigger it it the simpler the boundary
#define CVCONTOUR_APPROX_LEVEL	2

// No of itterations of erosion and dilation
#define CVCLOSE_ITR	1

///////////////////////////////////////////////////////////////////////////////
//
void ipConnectedComponents(	IplImage* mask,			// Raw mask image, 8U, 1 Ch
							int poly1_hull0,		// 1= polygon appox 0=convex hull
							float perimeterScale,	// len = image(wid+hei/periscale) if contourlen< this then delete contour - default 4
							int* num ,				// Max Num of rectangles
							CvRect* bbs,			// Pointer to Bounding box rectangle
							CvPoint* centers		// pointer to contour center <vector>length num
							)
{
	static CvMemStorage* mem_storage = NULL;
	static CvSeq* contours = NULL;
	cvNamedWindow("Trying...");

	cvShowImage("Trying...", mask);
	cvWaitKey(0);

	// Clean up mask using morphological operations
	cvMorphologyEx( mask, mask, 0, 0, CV_MOP_OPEN, CVCLOSE_ITR );
    cvMorphologyEx( mask, mask, 0, 0, CV_MOP_CLOSE,CVCLOSE_ITR );


	// Find Contours around only bigger regions
	//
	if(mem_storage == NULL)
	{
		mem_storage = cvCreateMemStorage(0);
	}
	else
	{
		cvClearMemStorage(mem_storage);
	}

	CvContourScanner scanner = cvStartFindContours(
													mask,
													mem_storage,
													sizeof(CvContour),
													CV_RETR_EXTERNAL,
													CV_CHAIN_APPROX_SIMPLE
													);
	//Eliminate Small contours
	CvSeq* c;
	int numCont = 0;
	while( (c = cvFindNextContour( scanner )) != NULL )
	{
		double len = cvContourPerimeter (c);
		// Calc perimeter Len Threshold
		double q = (mask->height + mask->width)/perimeterScale;
		
		// Get rid of blob if perimeter too small
		if(len<q)
			cvSubstituteContour( scanner, NULL );
		else
		{
			// Smooth edges if large enough
			CvSeq* c_new;
			if(poly1_hull0)
			{
				c_new = cvApproxPoly(
										c,
										sizeof(CvContour),
										mem_storage,
										CV_POLY_APPROX_DP,
										CVCONTOUR_APPROX_LEVEL,
										0
										);
			}
			else
			{
				// Convex Hull of Segemntation
				//
				c_new = cvConvexHull2(
										c,
										mem_storage,
										CV_CLOCKWISE,
										1
										);
			}
			cvSubstituteContour( scanner, c_new );
			numCont++;
		}
	}
	cout<<"Found Contours = "<<numCont;
	contours = cvEndFindContours( &scanner );

	const CvScalar CVX_WHITE = CV_RGB(0xff,0xff,0xff);
	const CvScalar CVX_BLACK = CV_RGB(0x00,0x00,0x00);

	cvZero(mask);
	IplImage* maskTemp;

	//Bounding Rectanges
	//
	if(num != NULL)
	{
		int N = *num, numFilled = 0, i=0;
		CvMoments moments;
		double M00, M01, M10;
		maskTemp = cvCloneImage(mask);
		for ( i =0, c= contours; c!=NULL; c== c->h_next, i++)
		{
			if(i<N)
			{
				cvDrawContours(
								maskTemp, 
								c,
								CVX_WHITE,
								CVX_WHITE,
								-1,
								CV_FILLED,
								8
								);
				cvShowImage("Trying...", maskTemp);
				// Find center of each contour 
				//
				if(centers!=NULL)
				{
					cvMoments(maskTemp,&moments,1);
					M00=cvGetSpatialMoment(&moments,0,0);
					M10=cvGetSpatialMoment(&moments,1,0);
					M01=cvGetSpatialMoment(&moments,0,1);
					centers[i].x=(int)(M10/M00);
					centers[i].y=(int)(M01/M00);
				}


				// Draw Bounding Rectangles
				//
				if(bbs!=NULL)
					bbs[i] = cvBoundingRect(c);
				else
				cvZero(maskTemp);
				numFilled++;
			} // end if
			// Draw filled Contours into mask
			cvShowImage("Trying...", maskTemp);
			cvWaitKey(0);
			cvDrawContours(
							mask,
							c,
							CVX_WHITE,
							CVX_WHITE,
							-1
							CV_FILLED,
							8
							);
		} // end For Loop
		*num=numFilled;
		cvReleaseImage(&maskTemp);
	}// end if
}

void FindconnectedComponents (IplImage* skinMask, double thresh)
{
	 cvNamedWindow("Contours");
	 cvShowImage("Contours", skinMask);
	 cvWaitKey(1000);

     cvMorphologyEx (skinMask, skinMask, 0, 0, CV_MOP_OPEN, 1);
     cvMorphologyEx (skinMask, skinMask, 0, 0, CV_MOP_CLOSE, 1);

	 cvShowImage("Contours", skinMask);
	 cvWaitKey(1000);

     CvMemStorage *connectedCompStorage = cvCreateMemStorage (0);
     CvSeq *temp = NULL; //used to loop through contour perimeter checking
     CvSeq *connectedComp = NULL; //stores all derived connected component contours

     CvContourScanner connectedCompScanner = cvStartFindContours (skinMask, connectedCompStorage); //FUNCTION CALL CAUSING PROBLEMS

     while ((temp = cvFindNextContour (connectedCompScanner)) != NULL)
     {
         double perimeter = cvContourPerimeter (temp);

         /*if (perimeter < thresh)
         {
             cvSubstituteContour (connectedCompScanner, NULL);
         }

         else*/
         {
             temp = cvApproxPoly (temp, sizeof (CvContour), connectedCompStorage, CV_POLY_APPROX_DP, 2, 0);
             cvSubstituteContour (connectedCompScanner, temp);
         }
     }

     connectedComp = cvEndFindContours (&connectedCompScanner);

     cvZero (skinMask);

     for (temp = connectedComp; temp != NULL; temp = temp -> h_next);
     {
         cvDrawContours (skinMask, temp, cvScalar (255, 255, 255), cvScalar (0, 0, 0), -1,   CV_FILLED, 8);
		 printf("\nDrawing %d", temp);
     }
	 cvShowImage("Contours", skinMask);
	 cvWaitKey(0);
	 cvDestroyWindow("Contours");

     cvReleaseMemStorage (&connectedCompStorage);
}

// Function implementation in C
// 
int ipGetPixels(IplImage *Imask)
{
	// IplImage* Imask = new IplImage(Mask);		// Convert Mat to IplImage
	int BoxWidth	= 0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;				// Contours are a sequence of points
	cvFindContours( Imask,			// Find Contours
					storage, 
					&contour, sizeof(CvContour), 
					CV_RETR_CCOMP, 
					CV_CHAIN_APPROX_SIMPLE );
	CvSize sz = cvGetSize(Imask);
	IplImage* clone = cvCreateImage(sz , IPL_DEPTH_8U, 1);	// Memory
	clone	= cvCloneImage(Imask);
	cvNamedWindow("Boxes");
	cvShowImage("Boxes", clone);
	//-------------------------------------------------------------------------
	//Process each moving contour in the current frame...
	//
	CvRect bndRect = cvRect(0,0,0,0);	//Rectangle to use to put around the people.
	CvPoint pt1, pt2;					//Points for the edges of the rectangle.
	for( ; contour != 0; contour = contour->h_next )
	{
		//Get a bounding rectangle around the moving object.
		bndRect = cvBoundingRect(contour, 0);

		pt1.x = bndRect.x;
		pt1.y = bndRect.y;
		pt2.x = bndRect.x + bndRect.width;
		pt2.y = bndRect.y + bndRect.height;

		//Draw the bounding rectangle around the moving object.
		cvRectangle(clone, pt1, pt2, CV_RGB(255,0,0), 1);
		cvShowImage("Boxes", clone);
		cvWaitKey(0);
		return BoxWidth;
	}
}