/**
* Configuration and Calibration Utility for Length Detection Application
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
int main(int argc, char *argv[])
{
	Mat img, frame_diff;
	float learning_rate = 0.99;

	VideoCapture vidDevice(0);
	vidDevice>>img;

	namedWindow("BG");
///@ this will format background similar to img by copying header of img
	Mat background = img.clone();

	while(1)
	{
		vidDevice>>img;
///@ Continuously compute the running average of frames
		background = (learning_rate) * background + img * (1 - learning_rate);
		imshow("BG",background);
		if(cv::waitKey(30) >= 0) break;
	} //end of for loop

///@ it will display reconstructed background using running average method
	//imshow("BG",background);
	//waitKey();
	return 0;
}