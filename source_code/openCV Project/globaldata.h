// File: globaldata.h
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013

#ifndef GLOBALDATA_H
#define GLOBALDATA_H


#include "opencv\cvaux.h"
#include <opencv\highgui.h>



namespace global_data{
	extern CvStereoBMState *BMState;
	extern CvSize camStreamSize;
	extern CvStereoGCState *GCState;
	extern IplImage* image_left;
    extern IplImage* image_right;
	extern IplImage* image_left_rectified;
    extern IplImage* image_right_rectified;
}


#endif GLOBALDATA_H






