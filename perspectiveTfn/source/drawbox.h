/* ****************************************************************************
	Drawing Boxes

	By: Inderpreet Singh

***************************************************************************  */
#ifndef DRAWBOX_H
#define DRAWBOX_H

// Prototype of mouse call back function
void my_mouse_callback( int event, int x, int y, int flags, void* param );

// Implements the drawing of the rectange
void draw_box( IplImage* img, CvRect rect );

// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param );
	
// The main app function
void DrawBoxMain(void);

#endif