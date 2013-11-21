#ifndef IPCONTOURSCANNER_H
#define IPCONTOURSCANNER_H


// Function to find the conected components in an Image
void ipConnectedComponents(	IplImage* mask,				// 1 ch Mask
							int poly1_hull0 = 1,
							float perimeterScale = 4,
							int* num = NULL,
							CvRect* bbs = NULL,
							CvPoint* centers = NULL
							);

// Find connected contours in a mask
void FindconnectedComponents (IplImage* skinMask, double thresh);

// Function to get contours in ANSI C
int ipGetPixels(IplImage*);
#endif