/**
* Background Detection Routines
*
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
#include<cvaux.h>
#include<highgui.h>
#include"ipbackground.h"

using namespace std;
using namespace cv;

void ipGaussianBG1(void)
{
	/* Start capturing */
    CvCapture* capture = 0;

    //if( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
        capture = cvCaptureFromCAM( 0 ); // argc == 2 ? argv[1][0] - '0' : 0 );
    //else if( argc == 2 )
    //    capture = cvCaptureFromAVI( argv[1] );

    if( !capture )
    {
        fprintf(stderr,"Could not initialize...\n");
        //return -1;
    }

    /* print a welcome message, and the OpenCV version */
    printf ("Demo of the background classification using CvGaussBGModel %s (%d.%d.%d)\n",
	    CV_VERSION,
	    CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);

    /* Capture 1 video frame for initialization */
    IplImage* videoFrame = NULL;
    videoFrame = cvQueryFrame(capture);

    if(!videoFrame)
    {
        printf("Bad frame \n");
        exit(0);
    }

    // Create windows
    cvNamedWindow("BG", 1);
    cvNamedWindow("FG", 1);

    // Select parameters for Gaussian model.
    CvGaussBGStatModelParams* params = new CvGaussBGStatModelParams;						
    params->win_size=2;	
    params->n_gauss=5;
    params->bg_threshold=0.7;
    params->std_threshold=3.5;
    params->minArea=15;
    params->weight_init=0.05;
    params->variance_init=30; 

    // Creat CvBGStatModel
    // cvCreateGaussianBGModel( IplImage* first_frame, CvGaussBGStatModelParams* parameters )
    // or
    // cvCreateGaussianBGModel( IplImage* first_frame )
	//This is based on the "An Improved Adaptive Background Mixture Model for 
	// Real-time Tracking with Shadow Detection" by P. KaewTraKulPong and R. Bowden 
	//http://personal.ee.surrey.ac.uk/Personal/R.Bowden/publications/avbs01/avbs01.pdf 
	// The windowing method is used, but not the shadow detection. I make some of my own modifications 
	// which make more sense. There are some errors in some of their equations.

    CvBGStatModel* bgModel = cvCreateGaussianBGModel(videoFrame ,params);

    int key=-1;
    while(key != 'q')
    {
        // Grab a fram
        videoFrame = cvQueryFrame(capture);
        if( !videoFrame )
            break;
        
        // Update model
        cvUpdateBGStatModel(videoFrame,bgModel);
        
        // Display results
       	cvShowImage("BG", bgModel->background);
       	cvShowImage("FG", bgModel->foreground);	
       	key = cvWaitKey(10);
    }

    cvDestroyWindow("BG");
    cvDestroyWindow("FG");
    cvReleaseBGStatModel( &bgModel );
    cvReleaseCapture(&capture);
}

void ipBGMOG2Demo1(void)
{
	// Using Inbuilt types for tempFrame stuff
    cv::Mat tempFrame;				// For capture
    cv::Mat Background;				// For Extracted Background
    cv::Mat Foreground;				// Foreground Mask Thingy
	// Capture Video From Camera
    cv::VideoCapture vidDevice(0);

	// Background Extraction Function Object Created.
    cv::BackgroundSubtractorMOG2 bg;
	// bg = cv::BackgroundSubtractorMOG2(200, 10, false);
	bg.set("nmixtures",10);
	// Disable detection of Shadows
	//bg.set("nShadowDetection",false);


	// Create Windows
	namedWindow("Current Frame");
    namedWindow("Extracted Background");
	namedWindow("Difference");

    while(1)
    {
		vidDevice >> tempFrame; 					// Capture Frame
		// Update Background model and Create Binary Mask Of foreground Objects
        bg.operator ()(tempFrame,Foreground);		
        bg.getBackgroundImage(Background);
		// Illimnate Noise
        erode(Foreground,Foreground,cv::Mat());
        dilate(Foreground,Foreground,cv::Mat());
		
        imshow("Current Frame",tempFrame);
        imshow("Extracted Background",Background);
		imshow("Difference", (Background-tempFrame));
        if(cv::waitKey(30) >= 0) break;
    }
}


void ipBGMOG2Demo2(void)
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
}

void ipBGMOG2Demo3(void)
{
	// Using Inbuilt types for frame stuff
    cv::Mat frame;				// For capture
    cv::Mat back;				// For Extracted Background
    cv::Mat fore;				// Foreground Mask Thingy
	// Capture Video From Camera
    cv::VideoCapture cap(0);

	// Background Extraction Function Object Created.
    cv::BackgroundSubtractorMOG2 bg; // (200,20, false);
    std::vector<std::vector<cv::Point> > contours;

    cv::namedWindow("Frame");
    cv::namedWindow("Background");
	//namedWindow("Foreground");

    for(;;)
    {
        cap >> frame;
		bg(frame, fore, -1);
        //bg.operator ()(frame,fore);
        bg.getBackgroundImage(back);
		threshold(fore,fore,128,255,THRESH_BINARY);
        medianBlur(fore,fore,9);
        erode(fore,fore,Mat());
        dilate(fore,fore,Mat());

        //cv::erode(fore,fore,cv::Mat());
        //cv::dilate(fore,fore,cv::Mat());
        cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),1);
        cv::imshow("Frame",frame);
        cv::imshow("Background",back);
		//imshow("Foreground", fore);
        if(cv::waitKey(30) >= 0) break;
    }
}

void ipBGMOG2Demo4(void)
{
	// Using Inbuilt types for frame stuff
    cv::Mat frame;				// For capture
    cv::Mat back;				// For Extracted Background
    cv::Mat fore;				// Foreground Mask Thingy
	// Capture Video From Camera
    cv::VideoCapture cap(0);

	// Background Extraction Function Object Created.
    cv::BackgroundSubtractorMOG2 bg; // (200,20, false);
    std::vector<std::vector<cv::Point> > contours;

    cv::namedWindow("Frame");
    cv::namedWindow("Background");
	//namedWindow("Foreground");

    for(;;)
    {
        cap >> frame;
		bg(frame, fore, -1);
        //bg.operator ()(frame,fore);
        bg.getBackgroundImage(back);
		threshold(fore,fore,128,255,THRESH_BINARY);
        medianBlur(fore,fore,9);
        erode(fore,fore,Mat());
        dilate(fore,fore,Mat());

        //cv::erode(fore,fore,cv::Mat());
        //cv::dilate(fore,fore,cv::Mat());
        cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),1);
        cv::imshow("Frame",frame);
        cv::imshow("Background",back);
		//imshow("Foreground", fore);
        if(cv::waitKey(30) >= 0) break;
    }
}

void ipSimpleBGDemo1(void)
{
	IplImage *image = 0, *frameTime1=0, *frameTime2=0, *frameForeground=0, *img1=0, *img2=0;
    // printf("Press ESC to Close.\n");
    CvCapture* capture = 0;    //Video capturing structure
    capture = cvCaptureFromCAM( -1 );    //Initializes capturing a video from a camera   
    if( !capture )
    {
        fprintf(stderr,"Could not initialize capturing...\n");
        //return -1;
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
		/*CvMemStorage* storage = cvCreateMemStorage(0);
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
		}*/
		//cvShowImage( "Bounds", gGray );

        cvCopy( frameTime1, frameTime2, 0 );
		
		c = cvWaitKey(50);     //waits for a pressed key
        if( (char) c == 27 )  //if key==ESC (27 ESC button) then break
            break;
    }
    cvReleaseCapture( &capture );  //Releases the CvCapture structure
    cvDestroyWindow("Camera");
    cvDestroyWindow("frameForeground");
	cvDestroyWindow("Bounds");
}

void ipSimpleBGDemo2(void)
{
	IplImage *image = 0, *frameTime1=0, *frameTime2=0, *frameForeground=0, *img1=0, *img2=0;
	
	//create storage for contours
    CvMemStorage* storage = NULL;
	storage = cvCreateMemStorage(0);
	CvRect rect;
	CvSeq* contours = 0;
	
	//-------------------------------------------------------------------------
	// Capturing Section
    CvCapture* capture = 0;    //Video capturing structure
	capture = cvCaptureFromCAM( -1 );    //Initializes capturing a video from a camera   
    if( !capture )
    {
        fprintf(stderr,"Could not initialize capturing...\n");
        //return -1;
    }
	
	// ------------------------------------------------------------------------
	// Windows and Stuff
    cvNamedWindow( "Camera", 1 );			//create the window for the Camera Output (Directly)
    cvNamedWindow( "frameForeground", 1 );

	// ------------------------------------------------------------------------
	// Loop
    while(1)
    {
        IplImage* frame = 0;				//every time create/initialize an image (which name is frame) to process
        int  c;								//integer to exit program

        frame = cvQueryFrame( capture );	//grabs and returns a frame from a camera input
       
        if( !frame )						//if there is no frame exit the while(1)
            break;
        if( !image )						//if there is no image, do the followings
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
                  20,
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
                3);      
        cvDilate(
                frameForeground,
                frameForeground,
                0,
                3);      
        cvErode(
                frameForeground,
                frameForeground,
                0,
                1);
        cvShowImage( "Camera", image );  //displays the image in the specified window 
        cvShowImage( "frameForeground", frameForeground );
		// Find Contours
		cvFindContours( frameForeground, storage, &contours );
		// Draw Contours
		//draw bounding box around each contour
        for(; contours!=0; contours = contours->h_next)
        {
			rect = cvBoundingRect(contours, 0); //extract bounding box for current contour
			//drawing rectangle
            cvRectangle(image,                  
						cvPoint(rect.x, rect.y),    
                        cvPoint(rect.x+rect.width, rect.y+rect.height),
                        cvScalar(0, 0, 255, 0),
                        2, 8, 0);                 
        }
		//display colour image with bounding box
        cvShowImage("Camera", image);
        cvCopy( frameTime1, frameTime2, 0 );
		
		c = cvWaitKey(50);     //waits for a pressed key
        if( (char) c == 27 )  //if key==ESC (27 ESC button) then break
            break;
    }
    cvReleaseCapture( &capture );  //Releases the CvCapture structure
    cvDestroyWindow("Camera");
    cvDestroyWindow("frameForeground");
}

void ipSimpleBGDemo3(void)
{
	CvRect rect;
	CvSeq* contours = 0;
	CvMemStorage* storage = NULL;
	CvCapture *cam;
	IplImage *currentFrame, *currentFrame_grey, *differenceImg, *oldFrame_grey;

	bool first = true;
	//Create a new movie capture object.
       cam = cvCaptureFromCAM(0);

       //create storage for contours
       storage = cvCreateMemStorage(0);

       //capture current frame from webcam
       currentFrame = cvQueryFrame(cam);

       //Size of the image.
       CvSize imgSize;
       imgSize.width = currentFrame->width;
       imgSize.height = currentFrame->height;

       //Images to use in the program.
       currentFrame_grey = cvCreateImage( imgSize, IPL_DEPTH_8U, 1);                           

       while(1)
       {
              currentFrame = cvQueryFrame( cam );
              if( !currentFrame ) break;

              //Convert the image to grayscale.
              cvCvtColor(currentFrame,currentFrame_grey,CV_RGB2GRAY);

              if(first) //Capturing Background for the first time
              {
                     differenceImg = cvCloneImage(currentFrame_grey);
                     oldFrame_grey = cvCloneImage(currentFrame_grey);
                     cvConvertScale(currentFrame_grey, oldFrame_grey, 1.0, 0.0);
                     first = false;
                     continue;
              }

              //Minus the current frame from the moving average.
              cvAbsDiff(oldFrame_grey,currentFrame_grey,differenceImg);

              //bluring the differnece image
              cvSmooth(differenceImg, differenceImg, CV_BLUR);             

              //apply threshold to discard small unwanted movements
              cvThreshold(differenceImg, differenceImg, 25, 255, CV_THRESH_BINARY);

              //find contours
              cvFindContours( differenceImg, storage, &contours );

              //draw bounding box around each contour
              for(; contours!=0; contours = contours->h_next)
              {
                     rect = cvBoundingRect(contours, 0); //extract bounding box for current contour

                     //drawing rectangle
                     cvRectangle(currentFrame,                  
                                  cvPoint(rect.x, rect.y),    
                                  cvPoint(rect.x+rect.width, rect.y+rect.height),
                                  cvScalar(0, 0, 255, 0),
                                  2, 8, 0);                 
              }

              //display colour image with bounding box
              cvShowImage("Output Image", currentFrame);

              //display threshold image
              cvShowImage("Difference image", differenceImg);

              //New Background
              cvConvertScale(currentFrame_grey, oldFrame_grey, 1.0, 0.0);

              //clear memory and contours
              cvClearMemStorage( storage );
              contours = 0;

              //press Esc to exit
              char c = cvWaitKey(33);
              if( c == 27 ) break;

       }

       // Destroy the image & movies objects
       cvReleaseImage(&oldFrame_grey);
       cvReleaseImage(&differenceImg);
       cvReleaseImage(&currentFrame);
       cvReleaseImage(&currentFrame_grey);
       //cvReleaseCapture(&cam);
}