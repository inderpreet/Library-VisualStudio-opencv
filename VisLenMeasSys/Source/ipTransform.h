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

#ifndef IP_TRANSFORM_H
#define IP_TRANSFORM_H


#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>

/// Transform Function
void ipTransform(IplImage* image, // Mask image pointer
				void *memloc);		// Mat type pointer to be typecasted internally

/// Demo Application Function
int TransformMain(void);


#endif