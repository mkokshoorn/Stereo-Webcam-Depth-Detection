//CVStereoWebCam.cpp
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013


#include "stdafx.h"
#include "CVStereoWebcam.h"

//Namespace to hode Capture Objects
namespace webcam_data{ 
	
	 	CvCapture* capture_left; 
		CvCapture* capture_right;
}


//Initialises Webcams
void webcam_init(int cam1,int cam2){

	webcam_data::capture_left = cvCreateCameraCapture(cam1);	
	webcam_data::capture_right = cvCreateCameraCapture(cam2);


	global_data::image_left = cvQueryFrame(webcam_data::capture_left);							
	global_data::image_right = cvQueryFrame(webcam_data::capture_right);							
	global_data::image_left_rectified = cvQueryFrame(webcam_data::capture_left);						
	global_data::image_right_rectified = cvQueryFrame(webcam_data::capture_right);						
	global_data::camStreamSize=cvGetSize(global_data::image_left);


}

//Will uupdate the current frame
void webcam_update(void){


	global_data::image_left = cvQueryFrame(webcam_data::capture_left);							// Get the first frame
	global_data::image_right= cvQueryFrame( webcam_data::capture_right);
	cvWaitKey(10);
}

//Updates the display windows
void display_update(const unsigned int DISP_ID){
	
	if(DISP_ID&CAM1)     cvShowImage(CAM1_WIND, global_data::image_left_rectified);
	if(DISP_ID&CAM2)     cvShowImage(CAM2_WIND, global_data::image_right_rectified);
	if(DISP_ID&CAM1_RAW) cvShowImage(CAM1_RAW_WIND, global_data::image_left); 
	if(DISP_ID&CAM2_RAW) cvShowImage(CAM2_RAW_WIND, global_data::image_right);
	if(DISP_ID&DISP)	 cvShowImage(DISP_WIND, correspondance_data::vdisp_left_memory);
	if(DISP_ID&PROJ)	 cvShowImage(PROJ_WIND, correspondance_data::threholded_image);
	if(DISP_ID&RAW_DISP) cvShowImage( RAW_DISPARITY_WIND,correspondance_data::vdisp_left);
	if(DISP_ID&PARAM)    cvShowImage( PARAM_WIND,0);
}

//Creates named widnwos for the displays.
void display_create(const unsigned int DISP_ID){
	
	if(DISP_ID&CAM1)     cvNamedWindow(CAM1_WIND,0);
	if(DISP_ID&CAM2)     cvNamedWindow(CAM2_WIND,0);
	if(DISP_ID&CAM1_RAW) cvNamedWindow(CAM1_RAW_WIND,0); 
	if(DISP_ID&CAM2_RAW) cvNamedWindow(CAM2_RAW_WIND,0); 
	if(DISP_ID&DISP)     cvNamedWindow(DISP_WIND,0); 
	if(DISP_ID&PROJ)     cvNamedWindow(PROJ_WIND,0); 
	if(DISP_ID&RAW_DISP) cvNamedWindow(RAW_DISPARITY_WIND,0); 
	if(DISP_ID&PARAM)    cvNamedWindow(PARAM_WIND,0);
}


//Destroys the Windows.
void display_destroy(const unsigned int DISP_ID){
	
	if(DISP_ID&CAM1)     cvDestroyWindow(CAM1_WIND);
	if(DISP_ID&CAM2)     cvDestroyWindow(CAM2_WIND);
	if(DISP_ID&CAM1_RAW) cvDestroyWindow(CAM1_RAW_WIND); 
	if(DISP_ID&CAM2_RAW) cvDestroyWindow(CAM2_RAW_WIND); 
	if(DISP_ID&DISP)     cvDestroyWindow(DISP_WIND); 
	if(DISP_ID&PROJ)     cvDestroyWindow(PROJ_WIND); 
	if(DISP_ID&RAW_DISP) cvDestroyWindow(RAW_DISPARITY_WIND); 
	if(DISP_ID&PARAM)    cvNamedWindow(PARAM_WIND); 
}

//Release the Webcam Object
void webcam_release(void){
	
	cvReleaseCapture( &(webcam_data::capture_left)  );				// release camera 1
	cvReleaseCapture( &(webcam_data::capture_right) );				// release camera 2
}




