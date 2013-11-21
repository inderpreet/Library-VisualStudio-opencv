/**
* Configuration File Structure
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

#ifndef FILESTRUCT_H
#define FILESTRUCT_H

#include<opencv2/opencv.hpp>

///<summary>
///Structure and class for saving the configuration to file
///and hopefully reading it too!
///</summary>
class ipFileStruct
{
protected:
	CvPoint ClickPoint[4];				// Arry of points
	int iIndex;

public:
	///Default Constructor
	ipFileStruct(void);
	///Method to Set a point
	void setPoint(int x, int y, int Index);
	///Method to Get a Point
	void getPoint(int *x, int *y, int Index);
};

ipFileStruct::ipFileStruct(void)
{
	iIndex = 0;
}

void ipFileStruct::setPoint(int x, int y, int Index)
{
	ClickPoint[Index].x = x;
	ClickPoint[Index].y = y;
}

void ipFileStruct::getPoint(int *x, int *y, int Index)
{
	*x = ClickPoint[Index].x;
	*y = ClickPoint[Index].y;
}

#endif