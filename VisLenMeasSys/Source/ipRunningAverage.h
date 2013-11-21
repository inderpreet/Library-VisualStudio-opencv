#ifndef IPRUNNINGAVERAGE_H
#define IPRUNNINGAVERAGE_H

// Function for generating running average
// @params 3Ch current image 8U, 1Ch avg image 32F
void ipRunningAvg1(IplImage* current, IplImage* avg);

// Function to find the foreground mask
// @params		8U 3ch Image,		32F 1ch Image,	32F 1ch Image
void ipGetBgMask1(IplImage* current, IplImage* avg, IplImage* mask);

#endif