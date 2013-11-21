/* ****************************************************************************
	Open CV based image transformation
	Opev CV based Draw rectangle Example
	By: Inderpreet Singh

***************************************************************************  */

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>


using namespace std;
using namespace cv;

const char* FileName="D:/Sandbox/VS2010/perspectiveTfn/source/cap1.jpg";

void my_mouse_callback( int event, int x, int y, int flags, void* param );

CvRect box;
CvPoint ClickPoint;
bool drawing_box = false;

void draw_box( IplImage* img, CvRect rect )
{
	cvRectangle( img, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),
				cvScalar(0xff,0x00,0x00) );
}

// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param ){
	IplImage* image = (IplImage*) param;

	switch( event ){
		case CV_EVENT_LBUTTONDOWN:
			drawing_box = true;
			box = cvRect( x, y, 0, 0 );
			break;

		case CV_EVENT_MOUSEMOVE: 
			if( drawing_box ){
				box.width = x-box.x;
				box.height = y-box.y;
			}
			break;		

		case CV_EVENT_LBUTTONUP:
			drawing_box = false;
			if( box.width < 0 ){
				box.x += box.width;
				box.width *= -1;
			}
			if( box.height < 0 ){
				box.y += box.height;
				box.height *= -1;
			}
			draw_box( image, box );
			break;
	}
}

int main(int argc, char *argv[])
{
	const char* name = "Box Example";
	
	box = cvRect(-1,-1,0,0);

	IplImage* image = cvLoadImage( FileName );	// IPL Image
	Mat imImage(image);							// Mat Format
	namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.	
    imshow( "Display window", imImage ); 
	
	IplImage* temp = cvCloneImage( image );
	cvNamedWindow( name );

	// Set up the callback
	cvSetMouseCallback( name, my_mouse_callback, (void*) image);

	// Main loop
	while( 1 ){
		cvCopy( image, temp );
		if( drawing_box ) 
			draw_box( temp, box );
		cvShowImage( name, temp );

		if( cvWaitKey( 15 )==27 ) 
			break;
	}

	cvReleaseImage( &image );
	cvReleaseImage( &temp );
	cvDestroyWindow( name );

	return 0;
}





//int main(int argc, char *argv[])
//{
//	
//	// Read the Image
//	Mat image,transformed;
//	image = imread("D:/Sandbox/VS2010/perspectiveTfn/source/cap1.jpg", CV_LOAD_IMAGE_COLOR);
//	if(! image.data )                              // Check for invalid input
//    {
//        cout <<  "Could not open or find the image" << std::endl ;
//        return -1;
//    }
//	namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
//    imshow( "Display window", image );  
//	
//
//	// Get the points to 
//	Point2f p1(168, 179), p2(434, 195), p3(134, 377), p4(499, 385);
//	Point2f src[] = {p1, p2, p3, p4};
//
//	Point2f p5(140, 60), p6(500, 60), p7(140, 420), p8(500, 420);
//	Point2f dst[] = {p5, p6, p7, p8};
//
//	Mat transform = getPerspectiveTransform(src, dst);
//
//	warpPerspective(image, transformed, transform, image.size(),
//					INTER_LINEAR, BORDER_CONSTANT);
//	
//	namedWindow("Output", CV_WINDOW_AUTOSIZE);
//	
//	imshow("Output", transformed);
//	
//	waitKey(0);
//	return 0;
//}