// MotionDetection.cpp : Defines the entry point for the console application.
//


// Contourold.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "iostream"
#include "stdlib.h"

// OpenCV includes.
#include "cv.h"
#include "highgui.h"
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

using namespace std;

int main(int argc, char* argv[])
{

//Create a new window.
cvNamedWindow("My Window", CV_WINDOW_AUTOSIZE);

//Create a new movie capture object.
CvCapture *input;

//Assign the movie to capture.
//inputMovie = cvCaptureFromAVI("vinoth.avi");

char *fileName = "D:\\highway.avi";
//char *fileName = "D:\\Profile\\AVI\\cardriving.wmv";
input = cvCaptureFromFile(fileName);
if (!input)
{
cout << "Can't open file" << fileName < }


//Size of the image.
CvSize imgSize;
imgSize.width = 352;
imgSize.height = 240;

//Images to use in the program.
IplImage* greyImage = cvCreateImage( imgSize, IPL_DEPTH_8U, 1);
IplImage* colourImage;
IplImage* movingAverage = cvCreateImage( imgSize, IPL_DEPTH_32F, 3);
IplImage* difference;
IplImage* temp;
IplImage* motionHistory = cvCreateImage( imgSize, IPL_DEPTH_8U, 3);

//Rectangle to use to put around the people.
CvRect bndRect = cvRect(0,0,0,0);

//Points for the edges of the rectangle.
CvPoint pt1, pt2;

//Create a font object.
CvFont font;


//Create video to output to.
char* outFilename = argc==2 ? argv[1] : "D:\\outputMovie.avi";
CvVideoWriter* outputMovie = cvCreateVideoWriter(outFilename,
CV_FOURCC('F', 'L', 'V', 'I'), 29.97, cvSize(720, 576));

//Capture the movie frame by frame.
int prevX = 0;
int numPeople = 0;

//Buffer to save the number of people when converting the integer
//to a string.
char wow[65];

//The midpoint X position of the rectangle surrounding the moving objects.
int avgX = 0;

//Indicates whether this is the first time in the loop of frames.
bool first = true;

//Indicates the contour which was closest to the left boundary before the object
//entered the region between the buildings.
int closestToLeft = 0;
//Same as above, but for the right.
int closestToRight = 320;

//Keep processing frames...
for(;;)
{
//Get a frame from the input video.
colourImage = cvQueryFrame(input);

//If there are no more frames, jump out of the for.
if( !colourImage )
{
break;
}

//If this is the first time, initialize the images.
if(first)
{
difference = cvCloneImage(colourImage);
temp = cvCloneImage(colourImage);
cvConvertScale(colourImage, movingAverage, 1.0, 0.0);
first = false;
}
//else, make a running average of the motion.
else
{
cvRunningAvg(colourImage, movingAverage, 0.020, NULL);
}

//Convert the scale of the moving average.
cvConvertScale(movingAverage,temp, 1.0, 0.0);

//Minus the current frame from the moving average.
cvAbsDiff(colourImage,temp,difference);

//Convert the image to grayscale.
cvCvtColor(difference,greyImage,CV_RGB2GRAY);

//Convert the image to black and white.
cvThreshold(greyImage, greyImage, 70, 255, CV_THRESH_BINARY);

//Dilate and erode to get people blobs
cvDilate(greyImage, greyImage, 0, 18);
cvErode(greyImage, greyImage, 0, 10);

//Find the contours of the moving images in the frame.
CvMemStorage* storage = cvCreateMemStorage(0);
CvSeq* contour = 0;
cvFindContours( greyImage, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

//Process each moving contour in the current frame...
for( ; contour != 0; contour = contour->h_next )
{
//Get a bounding rectangle around the moving object.
bndRect = cvBoundingRect(contour, 0);

pt1.x = bndRect.x;
pt1.y = bndRect.y;
pt2.x = bndRect.x + bndRect.width;
pt2.y = bndRect.y + bndRect.height;

//Get an average X position of the moving contour.
avgX = (pt1.x + pt2.x) / 2;

//If the contour is within the edges of the building...
if(avgX > 90 && avgX < 250)
{
//If the the previous contour was within 2 of the left boundary...
if(closestToLeft >= 88 && closestToLeft <= 90)
{
//If the current X position is greater than the previous...
if(avgX > prevX)
{
//Increase the number of people.
numPeople++;

//Reset the closest object to the left indicator.
closestToLeft = 0;
}
}
//else if the previous contour was within 2 of the right boundary...
else if(closestToRight >= 250 && closestToRight <= 252)
{
//If the current X position is less than the previous...
if(avgX < prevX)
{
//Increase the number of people.
numPeople++;

//Reset the closest object to the right counter.
closestToRight = 320;
}
}

//Draw the bounding rectangle around the moving object.
cvRectangle(colourImage, pt1, pt2, CV_RGB(255,0,0), 1);
}

//If the current object is closer to the left boundary but still not across
//it, then change the closest to the left counter to this value.
if(avgX > closestToLeft && avgX <= 90)
{
closestToLeft = avgX;
}

//If the current object is closer to the right boundary but still not across
//it, then change the closest to the right counter to this value.
if(avgX < closestToRight && avgX >= 250)
{
closestToRight = avgX;
}

//Save the current X value to use as the previous in the next iteration.
prevX = avgX;
}


//Write the number of people counted at the top of the output frame.
cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.8, 0.8, 0, 2);
cvPutText(colourImage, _itoa(numPeople, wow, 10), cvPoint(60, 200), &font, cvScalar(0, 0, 300));

//Show the frame.
cvShowImage("My Window", colourImage);

//Wait for the user to see it.
cvWaitKey(10);

//Write the frame to the output movie.
cvWriteFrame(outputMovie, colourImage);
}

// Destroy the image, movies, and window.
cvReleaseImage(&temp);
cvReleaseImage(&difference);
cvReleaseImage(&greyImage);
cvReleaseImage(&movingAverage);
cvDestroyWindow("My Window");

cvReleaseCapture(&input);
cvReleaseVideoWriter(&outputMovie);


return 0;

}

//Motion detection can be done through this program. this code contains the code for writing the motion detected video to the output file.

//Here they hardcoded the values for detecting the people in the video.
//we need to remove the code like AvgX,closestToLeft and closestToRight like these ...

//it is an enough thing to use the draw the rectangle on all the moving objects. 