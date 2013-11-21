/* ****************************************************************************
* Class to find good features to track from the camera feed
*
*	Written By: Inderpreet Singh
*	email:	er.inderpreet@gmail.com
*
*
******************************************************************************/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

#define MAX_CORNERS		10

using namespace cv;
using namespace std;

CvScalar target_color[4] = { // in BGR order 
		{{   0,   0, 255,   0 }},  // red
		{{   0, 255,   0,   0 }},  // green
		{{ 255,   0,   0,   0 }},  // blue
		{{   0, 255, 255,   0 }}   // yellow
};

class ipFeatureTrack
{
public:
	// Default Constructor
	ipFeatureTrack(void);
	~ipFeatureTrack(void);
	// Loop This Function
	IplImage* ipGetFeature(Mat CurrImg);

protected:
	IplImage curr_frame_var; // current video frame
	IplImage* curr_frame;
	IplImage* gray_frame;	// grayscale version of current frame

	int w, h;				// video frame size
	IplImage* eig_image;	// Pointer For Corners
	IplImage* temp_image;	// Temporary Image 

	CvPoint2D32f corners[MAX_CORNERS];
	int corner_count;
	double quality_level;
	double min_distance;
	int eig_block_size;
	int use_harris;
};

IplImage* ipFeatureTrack::ipGetFeature(Mat CurrImg)
{
	// Create Local Copy of Frame
	IplImage curr_frame_var = CurrImg;
	IplImage* curr_frame=&curr_frame_var;

	// Get frame size
	//w = curr_frame->width;
	//h = curr_frame->height;
	w = CurrImg.size().width;
	h = CurrImg.size().height;
	cout<<h;

	// Convert the frame image to grayscale
	if( ! gray_frame ) {
		//fprintf(stderr, "Allocate gray_frame\n");
		int channels = 1;
		gray_frame = cvCreateImage(
				cvGetSize(curr_frame), 
				IPL_DEPTH_8U, channels);
	}
		cvCvtColor(curr_frame, gray_frame, CV_BGR2GRAY);
// ==== Allocate memory for corner arrays ====
	if ( !eig_image) {
		//fprintf(stderr, "Allocate eig_image\n");
		eig_image = cvCreateImage(cvSize(w, h),
				IPL_DEPTH_32F, 1);
	}
	if ( !temp_image) {
		//fprintf(stderr, "Allocate temp_image\n");
		temp_image = cvCreateImage(cvSize(w, h),
				IPL_DEPTH_32F, 1);
	}
	// Run the function for feature Detection
	cvGoodFeaturesToTrack(gray_frame,		// Single Channel Input Image
			eig_image,						// output 
			temp_image,
			corners,						// Max no of Corners to detect
			&corner_count,					// detected corners
			quality_level,					
			min_distance,
			NULL,							// Optional Region Of Interest
			eig_block_size,
			use_harris);
		cvScale(eig_image, eig_image, 100, 0.00);
		for( int i = 0; i < corner_count; i++) {
		int radius = h/25;
		cvCircle(curr_frame,
				cvPoint((int)(corners[i].x + 0.5f),(int)(corners[i].y + 0.5f)),
				radius,
				target_color[0]);
		}
	return curr_frame;
}

ipFeatureTrack::~ipFeatureTrack()
{
}

ipFeatureTrack::ipFeatureTrack()
{
	gray_frame = 0; // grayscale version of current frame

	eig_image = 0;
	temp_image = 0;

	corner_count = MAX_CORNERS;
	quality_level = 0.1;
	min_distance = 10;
	eig_block_size = 3;
	use_harris = false;
}