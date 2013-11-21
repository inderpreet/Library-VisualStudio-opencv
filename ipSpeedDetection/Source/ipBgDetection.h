/* ****************************************************************************
* 	Program for Extracting Background from the camera captures.
* 	Using Gaussian Mixter Based Detection
*
*	Written By: Inderpreet Singh
*
*	Description: Uses OpenCV 2.4
*	Create object 
*		ipBgDetection Detection1;
*		Detection1.ipUpdateBG(capturedFrame);
*		Mat BG = Detection1.ipGetBG();
*/

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>

using namespace cv;

// ****************************************************************************
// Class For Doing Background detection
class ipBgDetection{

public:
	// Constructor
	ipBgDetection();
	//void ipCreateWindows(void);
	//void ipShowWindows(void);
	void ipUpdateBG(Mat temp);
	Mat ipGetBG(void);

protected:
	// Using Inbuilt types for tempFrame stuff
    Mat tempFrame;				// For capture
    Mat Background;				// For Extracted Background
    Mat Foreground;				// Foreground Mask Thingy
	BackgroundSubtractorMOG2 bg;
	VideoCapture vidDevice;
};
// ****************************************************************************
// Constructor
ipBgDetection::ipBgDetection()
{
	// Background Extraction Function Object Created.
	bg.set("nmixtures",10);
	//vidDevice = VideoCapture(0);
}
// ****************************************************************************
void ipBgDetection::ipUpdateBG(Mat tempFrame)
{
	//vidDevice >> tempFrame; 					// Capture Frame
	// Update Background model and Create Binary Mask Of foreground Objects
    bg.operator ()(tempFrame, Foreground);		
    bg.getBackgroundImage(Background);

	// Illimnate Noise
    erode(Foreground,Foreground, Mat());
    dilate(Foreground,Foreground, Mat());
}
// ****************************************************************************
Mat ipBgDetection::ipGetBG(void)
{
	return Background;
}
// ****************************************************************************
//void ipBgDetection::ipCreateWindows(void)
//{
//	// Create Windows
//	namedWindow("Current Frame");
//    namedWindow("Extracted Background");
//}
// ****************************************************************************
//void ipBgDetection::ipShowWindows(void)
//{
//	imshow("Current Frame", tempFrame);
//    imshow("Extracted Background", Background);
//}
// ****************************************************************************