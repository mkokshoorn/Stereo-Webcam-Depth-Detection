//CVStereoWebCam.h
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013


#ifndef CVSTEREOWEBCAM_H
#define CVSTEREOWEBCAM_H

#include "globaldata.h"
#include "CVStereoCorrespondance.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

#define CAM1 1
#define CAM2 2
#define CAM1_RAW 4
#define CAM2_RAW 8
#define DISP   16
#define PROJ   32
#define RAW_DISP 64
#define PARAM	128


#define CAM1_WIND "WEBCAM ONE (LEFT) "
#define CAM2_WIND "WEBCAM TWO (RIGHT)"
#define CAM1_RAW_WIND "WEBCAM ONE RAW (LEFT) "
#define CAM2_RAW_WIND "WEBCAM TWO RAW (RIGHT)"
#define DISP_WIND  "DISPARITY"
#define PROJ_WIND  "RED OBJECT IDETIFIER"
#define PARAM_WIND  "BM STEREO CORRESPONDANCE PARAMETERS"
#define RAW_DISPARITY_WIND "RAW DISPARITY"

#define IMAGE_LIST_DIR "list.txt"



void webcam_init(int cam1,int cam2);

void webcam_update(void);

void display_update(const unsigned int ID);

void display_create(const unsigned int ID);

void display_destroy(const unsigned int ID);

void webcam_release(void);



#endif