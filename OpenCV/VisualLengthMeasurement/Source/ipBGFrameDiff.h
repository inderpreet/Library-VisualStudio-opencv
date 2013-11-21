/**
* Background Generation Using FrameDifferencing Method
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
#ifndef IPBGFRAMEDIFF_H
#define IPBGFRAMEDIFF_H

#include<opencv2/opencv.hpp>

/// Demo Function
void ipBGDiffDemo(void);

/// Memory Allocation Function for all images
/// All memory to global variables
/// @param	IplImage pointer of Captured Image/frame
void AllocateImages(IplImage* I);

/// Background Accumulation Function
/// 
/// @param	IplImage pointer of Captured Image/frame
void accumulateBG(IplImage *I);

/// Function to use the Accumulated Images to produce a stable background model
/// Just averaging stuff and set the threshold values here
void createModelFromStats(void);

/// Function to Extract the Background using OR Operations on each color layer
/// Mask inversion done here
void backgroundDiff( IplImage *I, IplImage *Imask);

// Release All the IMages initially created
void DeallocateImages(void);

/// Function used internally for setting the HIGH Threshold value
void setHighThreshold(float scale);

/// Function used internally for setting the LOW Threshold value
void setLowThreshold(float scale);

#endif