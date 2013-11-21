
#ifdef _CH_
#pragma package <opencv>
#endif

#define CV_NO_BACKWARD_COMPATIBILITY

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#endif

IplImage *image = 0, *frameTime1=0, *frameTime2=0, *frameForeground=0, *img1=0, *img2=0;

int ipForegroundDemo(void)
{
    // printf("Press ESC to Close.\n");
    CvCapture* capture = 0;    //Video capturing structure
    capture = cvCaptureFromCAM( -1 );    //Initializes capturing a video from a camera   
    if( !capture )
    {
        fprintf(stderr,"Could not initialize capturing...\n");
        return -1;
    }
    cvNamedWindow( "Camera", 1 );     //create the window for the Camera Output (Directly)
    cvNamedWindow( "frameForeground", 1 );
    while(1)
    {
        IplImage* frame = 0;   //every time create/initialize an image (which name is frame) to process
        int  c;                //integer to exit program
        frame = cvQueryFrame( capture );   //grabs and returns a frame from a camera input
       
        if( !frame )    //if there is no frame exit the while(1)
            break;
        if( !image )    //if there is no image, do the followings
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 ); 
            frameTime1 = cvCreateImage( cvGetSize(frame), 8, 1 );
            frameTime2 = cvCreateImage( cvGetSize(frame), 8, 1 );  
            frameForeground = cvCreateImage( cvGetSize(frame), 8, 1 );
            img1 = cvCreateImage( cvGetSize(frame), 8, 1 );  
            img2 = cvCreateImage( cvGetSize(frame), 8, 1 );    
        }
        cvCopy( frame, image, 0 );  
        cvCvtColor( image, img1, CV_BGR2GRAY ); 
        cvCopy( img1, frameTime1, 0 );    //currently frame in grayscale
        cvAbsDiff(
                  frameTime1,
                  frameTime2,
                  frameForeground
                  );
                 
        cvThreshold(
                  frameForeground,
                  frameForeground,
                  10,
                  255,
                  CV_THRESH_BINARY);       
        cvErode(
                frameForeground,
                frameForeground,
                0,
                1);     
        cvDilate(
                frameForeground,
                frameForeground,
                0,
                1);      
        cvDilate(
                frameForeground,
                frameForeground,
                0,
                1);      
        cvErode(
                frameForeground,
                frameForeground,
                0,
                1);
        cvShowImage( "Camera", image );  //displays the image in the specified window 
        cvShowImage( "frameForeground", frameForeground );

		// --------------------------------------------------------------------
		// Bounding Box Code
		CvMemStorage* storage = cvCreateMemStorage(0);
		cvClearMemStorage(storage);
		CvSeq *contours = 0;
		cvFindContours(frameForeground, storage, &contours,  sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
		if(contours)
		{
			for(; contours!= 0; contours = contours->h_next)
				cvDrawContours(
					frameForeground,
					contours,
					cvScalarAll(255),
					cvScalarAll(255),
					100,
					2,
					8);
		}
		//cvShowImage( "Bounds", gGray );

        cvCopy( frameTime1, frameTime2, 0 );
		
		c = cvWaitKey(10);     //waits for a pressed key
        if( (char) c == 27 )  //if key==ESC (27 ESC button) then break
            break;
    }
    cvReleaseCapture( &capture );  //Releases the CvCapture structure
    cvDestroyWindow("Camera");
    cvDestroyWindow("frameForeground");
	cvDestroyWindow("Bounds");
    return 0;
}
