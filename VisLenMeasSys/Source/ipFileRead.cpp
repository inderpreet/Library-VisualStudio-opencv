/**
* Configuration File Reading functions for Length Detection Application
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

#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<highgui.h>
#include "ipFileRead.h"

extern CvPoint ClickPoint[4];
extern CvPoint GridPoint[4];
extern int iPixelDistance;					// Variable for Storing reference Distance in Pixels
extern int iPhysicalDistance;				// Variable for saving physical Distance in Meters

using namespace std;
using namespace cv;

/// Simple File reading Function
void ipReadFile(void)
{
	const char* ConfigFile = "..\\config.bin";				// Configuration File Name and PATH

	void *MyPtr = NULL;
	FILE *filep = NULL;
	size_t len = 0;
	filep = fopen(ConfigFile, "rt");
	if( filep==NULL )
		printf("\nError Opening Config File", ConfigFile);
	else
	{
		len = 4*sizeof(CvPoint);
		//fread(DataPtr, DataSize, 1, filep);
		fread(ClickPoint, len, 1, filep);
		fread(GridPoint, len, 1, filep);
		fread(&iPixelDistance, sizeof(int), 1, filep);
		fread(&iPhysicalDistance, sizeof(int), 1, filep);
		fclose(filep);
	}
}

// -----------------------------------------------------------------------------
// Function for debugging purpose.
// displays the points read from the file on the console

void ipShowPoints(void)
{
	cout<<endl<<"Clicked Points"<<endl;
	for(int eye=0; eye<4; eye++)
		cout<<ClickPoint[eye].x<<"\t"<<ClickPoint[eye].y<<endl;	

	cout<<endl<<"Grid Points"<<endl;
	for(int eye=0; eye<4; eye++)
		cout<<GridPoint[eye].x<<"\t"<<GridPoint[eye].y<<endl;	
	
	cout<<endl<<"Pixel Distance = "<<iPixelDistance<<endl<<"Physical Distance = "<<iPhysicalDistance<<endl;
}