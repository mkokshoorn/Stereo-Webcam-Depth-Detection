//CVStereoCorrespondance.h
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013


#ifndef CVSTEREOCORRESPONDANCE_H
#define CVSTEREOCORRESPONDANCE_H

#include "stdafx.h"
#include "globaldata.h"
#include "CVStereoWebcam.h"
#include <opencv\cxcore.h>


void sliderHandler(void);

void init_sliderHandler(void);

void init_correspondance_data(void);

void calculate_correspondance_data(IplImage* image_left_undistorted,IplImage* image_right_undistorted,CvMat * Q);

int calculate_correspondance_depth(CvPoint left_point, CvMat *Q);

void calculate_correspondance_depth_tracking(cv::vector<cv::KeyPoint> keyPoints, CvMat *Q);

IplImage* GetThresholdedImage_RED(IplImage* frame);

void calculate_correspondance_data_save(IplImage* image_left_undistorted,IplImage* image_right_undistorted,CvMat * Q);

namespace correspondance_data{
	
	 	extern CvMat* disp_left ;
		extern CvMat* vdisp_left ;
		extern CvMat* smoothedvdisp_left ;
		extern CvMat* Image3D_left ;

		extern IplImage *grey_left_r ;
		extern IplImage *grey_right_r;

	 	extern CvMat* disp_right ;
		extern CvMat* vdisp_right ;

		extern CvMat* smoothedvdisp_right;
	
		extern CvMat* Image3D_right ;
		extern IplImage* threholded_image;

		extern CvMat* disp_left_memory;

		extern CvMat* vdisp_left_memory;

		extern cv::Mat pixel_norm_disp; 

}



#endif