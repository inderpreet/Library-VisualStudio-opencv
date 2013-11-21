/**
* Configuration and Calibration Utility for Length Detection Application
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

#include "ipWriteFile.h"
#include<iostream>
#include<vector>
#include<highgui.h>
#include"drawpoints.h"
#include<stdio.h>

using namespace std;
using namespace cv;

extern CvPoint ClickPoint[iMaxPoints];
extern CvPoint GridPoint[iMaxPoints];
extern int iPixelDistance;					// Variable for Storing reference Distance in Pixels
extern int iPhysicalDistance;				// Variable for saving physical Distance in Meters

void ipWriteFile(char* ConfigFile = "..\\config.bin")
{
	// If Possible delete existing File to take care of problems
	FILE *filep = NULL;							// File Pointer
	size_t len = 0;								// length of data
	DeleteFile((LPCWSTR)ConfigFile);

	filep = fopen(ConfigFile, "w");				// open file
	if( filep==NULL )
		printf("\nError Opening Config File", ConfigFile);
	else
	{
		len = 4*sizeof(CvPoint);
		fwrite(&ClickPoint, len, 1, filep);
		fwrite(&GridPoint, len, 1, filep);
		fwrite(&iPixelDistance, sizeof(int), 1, filep);
		fwrite(&iPhysicalDistance, sizeof(int), 1, filep);
		fclose(filep);
	}
}