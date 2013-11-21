/**
* Windows Events
*
*
*
* @package		Length Measurement Application
* @author		Yogesh Dhongade, Inderpreet Singh
* @copyright	Rajdeep Info Techno Pvt. Ltd.
* @since		Version 1.1
* @param		None
* @returns		None
*
*/

// ----------------------------------------------------------------------------

#include"ipIncludes.h"

using namespace std;
using namespace cv;

bool ipCaptureEvent()
{
	char c = NULL;
	c = cvWaitKey(3);
	if(c == ' ') 
		return true;
	else
		return false;
}