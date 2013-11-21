/* Program For Extracting the speed from a video
*	
*	Written By: Inderpreet Singh
*	er.inderpreet@gmail.com
*/

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<conio.h>
#include "ipCaptureDevice.h"

using namespace cv;
using namespace std;

/* ******************* Feature Detection Stuff *******************************/

// ****************************************************************************

int main(int argc, char *argv[])
{
/* ******************* Feature Detection Stuff *******************************/	

	Mat grayFrame;
	vector<Point2f> features;
	const int MAX_CORNERS = 2;
	int corner_count = MAX_CORNERS;
	double quality_level = 0.1;
	double min_distance = 10;
	int eig_block_size = 3;
	bool use_harris = false;

	int c;
	Mat CurrFrame;
	ipCaptureDevice Cap1;
	CurrFrame = Cap1.ipGetFrame();
	
	namedWindow("Window");
	namedWindow("Output1");
	while(1)
	{
		CurrFrame = Cap1.ipGetFrame();
		imshow("Window", CurrFrame);

/* ******************* Feature Detection Stuff *******************************/
		
		cvtColor(CurrFrame, grayFrame, CV_BGR2GRAY);
		goodFeaturesToTrack(grayFrame,
				features,
				corner_count,
				quality_level,
				min_distance,
				noArray(),
				eig_block_size,
				use_harris);
		
		for( int i = 0; i < features.size(); i++)
		{
				circle( grayFrame,
					Point((int)(features[i].x + 0.5f), (int)(features[i].y + 0.5f)),
					20,
					Scalar( 0, 0, 255 ),
					1,						// Thickness
					5 );				// linetype
		}
		imshow("Output1", grayFrame);
		c = cvWaitKey(5); // wait 10 ms or for key stroke
		if(c >=27)
			break;
	}

	return 0;
}