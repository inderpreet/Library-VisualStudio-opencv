/**
* Background Detection Routines
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

// ----------------------------------------------------------------------------
#ifndef IPBACKGROUND_H
#define IPBACKGROUND_H

//This is based on the "An Improved Adaptive Background Mixture Model for 
	// Real-time Tracking with Shadow Detection" by P. KaewTraKulPong and R. Bowden 
	//http://personal.ee.surrey.ac.uk/Personal/R.Bowden/publications/avbs01/avbs01.pdf 
	// The windowing method is used, but not the shadow detection. I make some of my own modifications 
	// which make more sense. There are some errors in some of their equations.
//Gaussian BG Extraction Demo
void ipGaussianBG1(void);

// Background Extraction Demo1
void ipBGMOG2Demo1(void);

// Background Extraction Demo2
// Draws contours as well
void ipBGMOG2Demo2(void);

// Background Extraction demo3
// 
void ipBGMOG2Demo3(void);

void ipBGMOG2Demo4(void);

// Background Extraction Simple Demo1
void ipSimpleBGDemo1(void);

// Background Extraction Simple Demo2
void ipSimpleBGDemo2(void);

void ipSimpleBGDemo3(void);


#endif