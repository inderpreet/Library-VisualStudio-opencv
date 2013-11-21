/* ***************************************************************************/
/* Class for abstracting the device capture functionality
* uses the C++ interface 
*	
*	Written By: Inderpreet Singh
*	er.inderpreet@gmail.com
*/

/* ***************************************************************************/

#include<opencv2/opencv.hpp>
#include<vector>

using namespace cv;

class ipCaptureDevice
{
public:
	ipCaptureDevice();			// Default Constructor
	~ipCaptureDevice();			// Destructor
	Mat ipGetFrame( void );	// Function to return the frame from Camera
protected:
	Mat CapturedFrame;			// Buffer
	VideoCapture Cam1;			// Capture Device Object
};
// ****************************************************************************
ipCaptureDevice::ipCaptureDevice()
{
	Cam1 = VideoCapture(0);		// Initalize the device
}
ipCaptureDevice::~ipCaptureDevice()
{
	//cvReleaseCapture( &Cam1 );
}
// ****************************************************************************
Mat ipCaptureDevice::ipGetFrame( void )
{
	Cam1 >> CapturedFrame;
	return CapturedFrame;
}
// ****************************************************************************
