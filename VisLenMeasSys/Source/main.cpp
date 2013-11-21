/**
* Length Detection Application
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

const char APP_VER[] = "1.2";
const char msg[]	= "Rajdeep Info Techno. Pvt Ltd.\nLength detection Application\nWritten By: Inderpreet Singh\n";

#include"ipIncludes.h"

int main(int argc, char *argv[])
{
	char vidFile[]	= "..\\14-03-24.avi";	// File Name
	char rtsp_src[] = "rtsp://192.168.1.110:554";
	printf(msg);
	printf("Version ");
	printf(APP_VER);
	printf("\n");
	//-------------------------------------------------------------------------
	//
	// Check the number of arguments and if number of arguments
	// Validate the arguments and use the rtsp address provided
	// If using camera source etc etc
	if(argc < 2)
	{
		printf("\nInsufficient arguments");
		getchar();
		return 0;
	}
	// ------------------------------------------------------------------------
	//
	// Length app with file or rtsp Source
	ipLengthApplication(argv[1]);

	return 0;
}