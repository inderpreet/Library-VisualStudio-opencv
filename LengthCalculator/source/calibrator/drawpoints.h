/**
* Drawing the Points
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
#ifndef DRAWPOINTS_H
#define DRAWPOINTS_H

#define iMaxPoints 4
/** 
* Prototype of mouse call back function
*
*/
void my_mouse_callback( int event, int x, int y, int flags, void* param );

/**
* Implements the drawing of a circle at a point
*
*/
void draw_point( IplImage* img );

/**
* Implements the drawing of a circles at a 4 Points
*
*/
void ipDrawGrid (IplImage *img);

/**
* Implement mouse callback
*
*/
void ip_mouse_callback( int event, int x, int y, int flags, void* param );
	

/// Demo application 
void DrawPointsMain(void);

#endif