// File: globaldata.cc
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013


#include "stdafx.h"
#include "globaldata.h"

//Name Space to hold globally shared data.
namespace global_data{
	CvStereoBMState *BMState;
	CvSize camStreamSize;
	IplImage* image_left;
    IplImage* image_right;
	IplImage* image_left_rectified;
    IplImage* image_right_rectified;
}