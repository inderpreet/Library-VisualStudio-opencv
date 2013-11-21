/* ****************************************************************************
* 	Program for Extracting Background from the camera captures.
* 	Using Gaussian Mixter Based Detection
*
*	Written By: Inderpreet Singh
*
*	Description: Uses OpenCV 2.4
*/

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>

int main(int argc, char *argv[])
{
	// Using Inbuilt types for frame stuff
    cv::Mat frame;				// For capture
    cv::Mat back;				// For Extracted Background
    cv::Mat fore;				// Foreground Mask Thingy
	// Capture Video From Camera
    cv::VideoCapture cap(0);

	// Background Extraction Function Object Created.
    cv::BackgroundSubtractorMOG2 bg;
	// bg = cv::BackgroundSubtractorMOG2(200, 10, false);
	bg.set("nmixtures",3);
    //bg.nmixtures = 3;
	// Disable detection of Shadows
    //bg.bShadowDetection = false;
	//bg.set("nShadowDetection",false);

    std::vector<std::vector<cv::Point> > contours;

    cv::namedWindow("Frame");
    cv::namedWindow("Background");

    for(;;)
    {
        cap >> frame;
        bg.operator ()(frame,fore);
        bg.getBackgroundImage(back);
        cv::erode(fore,fore,cv::Mat());
        cv::dilate(fore,fore,cv::Mat());
        cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);
        cv::imshow("Frame",frame);
        cv::imshow("Background",back);
        if(cv::waitKey(30) >= 0) break;
    }
    return 0;
}
