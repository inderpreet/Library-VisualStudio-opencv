/**
* Capture Function for Configuration and Calibration Utility for 
* Length Detection Application
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

#ifndef IPCAPTURE_H
#define IPCAPTURE_H

//Video Player Demo
void ipVideoPlayerDemo(void);

// Frame Capture Device Set and capture function
CvCapture* ipGetCapture(void);

// Frame Capture File Set and capture function
CvCapture* ipGetCapture(char*);

#endif