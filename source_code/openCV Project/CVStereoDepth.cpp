// openCV Project.cpp : Calculates 3D Information From Stereo WebCams.
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013


#include "stdafx.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

#include "globaldata.h"
#include "CVStereoCorrespondance.h"
#include "CVStereoCalibration.h"

#include <vector>
#include <iostream>
#include <string>
#include <iostream>

#include "opencv\cxmisc.h"
#include "opencv\cvaux.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



/******* Shared Variables ******/
CvPoint left_point_red ={0,0};
CvPoint right_point_red={0,0};
CvPoint left_point_blue ={0,0};
CvPoint right_point_blue={0,0};
CvPoint left_point_green ={0,0};
CvPoint right_point_green={0,0};

CvPoint disp_point_red ={0,0};
CvPoint disp_point_blue ={0,0};
CvPoint disp_point_green ={0,0};


double		D=0,W=0,\
			two_norm_red=0, \
			two_norm_blue=0,\
			two_norm_difference_red_blue=0,\
			two_norm_difference_left_red_blue_2D=0,\
			two_norm_difference_right_red_blue_2D=0;

	
CvPoint		 difference_left_red_blue_2D={0,0},\
				difference_right_red_blue_2D={0,0},\
				Object_one={0,0},Object_two={0,0};

CvPoint3D32f Object_Position_red={0,0,0},\
				Object_Position_blue={0,0,0},\
				Object_Position_green={0,0,0},\
				difference_red_blue={0,0,0};




//Shared Calibration Data
namespace main_variables{
	CvMat *Q  ; 
	CvMat *mx1; 
	CvMat *my1; 
	CvMat *mx2;
	CvMat *my2;
}

//Locally Shared Argument Data
namespace args{
	bool calibrate_procedure=0;
	int number_of_frames=0;
	bool recapture=0;
}



void interpret_args(int argc, const char* argv[]);

//Interupt Hanflers
void left_image_mouseHandler(int event, int x, int y, int flags, void* param);

void right_image_mouseHandler(int event, int x, int y, int flags, void* param);

void disp_image_mouseHandler(int event, int x, int y, int flags, void* param);

void manual_depth_calc(void);

int main(int argc, const char* argv[])
{
	printf("--- Stereo Vision Depth Detector ---\n\n");

	//ENABLE MEMORY REPORT
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	// LOCAL VARIABLES 
	using namespace cv;
	char key=0;

	#if BLOB_TRACKING==1
		vector<KeyPoint> keyPoints;
		SimpleBlobDetector::Params params;

		params.minDistBetweenBlobs=100000;

		params.filterByArea = true;         
		params.minArea = 1000.0;            
		params.maxArea = 500000000.0;            

		params.filterByColor = false;
		params.blobColor=120;

		params.filterByCircularity=false;
		params.minConvexity = (float) 0.3;
		params.maxConvexity =  10;

		params.filterByInertia=false;
		params.minInertiaRatio =  (float) 0.01;

		SimpleBlobDetector blobDetector( params );
		blobDetector.create("SimpleBlob");
	#endif

	//Interpret Command Line Argument
   interpret_args(argc, argv);

	//Initiate Webcams
	webcam_init(0,1);


	//Execute Calibration Sequence
	if(args::calibrate_procedure==true){
		display_create(CAM1_RAW|CAM2_RAW);
		CVStereo_Calibrate(args::recapture,args::number_of_frames);
		display_destroy(CAM1_RAW|CAM2_RAW);
	}

	//Init Correspondance Data and Handlers
	init_correspondance_data();
	init_sliderHandler();

	//Create Display Widnows
	display_create(CAM1|CAM2|DISP|PROJ);
	

	// Calibration Matrices
	printf("Loading Camera Parameters ...");
	main_variables::Q   = (CvMat *)cvLoad("CalibrationData/Q.xml"  ,NULL,NULL,NULL);
	main_variables::mx1 = (CvMat *)cvLoad("CalibrationData/mx1.xml",NULL,NULL,NULL);
	main_variables::my1 = (CvMat *)cvLoad("CalibrationData/my1.xml",NULL,NULL,NULL);
	main_variables::mx2 = (CvMat *)cvLoad("CalibrationData/mx2.xml",NULL,NULL,NULL);
	main_variables::my2 = (CvMat *)cvLoad("CalibrationData/my2.xml",NULL,NULL,NULL);
	printf(" Done\n");


	//PRINT INSTRUCTIONS//ADD TO DESCIRPTION
	printf("\nLeft and right click on each undistorted image to place corresponding\npoints of interest, then press SPACE to calcualte data...\n");

	// SET MOUSE INTERUPTS 
	cvSetMouseCallback("WEBCAM ONE (LEFT) ",left_image_mouseHandler, 0);
	cvSetMouseCallback("WEBCAM TWO (RIGHT)",right_image_mouseHandler,0);
	cvSetMouseCallback("DISPARITY",disp_image_mouseHandler,0);


	while (key != ESC_KEY){
		key=cvWaitKey(10);

		//GET NEXT FRAME
		webcam_update();
		

		// REMAP IMAGES  
		cvRemap(global_data::image_left ,  global_data::image_left_rectified , main_variables::mx1, main_variables::my1);
		cvRemap(global_data::image_right,  global_data::image_right_rectified, main_variables::mx2, main_variables::my2);

		
		//DRAW CIRCLES CORRESPONDING TO UI FEATURES
		cvCircle(global_data::image_left_rectified ,left_point_red ,1,cvScalar(100,10,10000,10),1);
		cvCircle(global_data::image_right_rectified,right_point_red,1,cvScalar(100,10,10000,10),1);
		
		cvCircle(global_data::image_left_rectified ,left_point_blue ,1,cvScalar(10000,10,10,10),1);
		cvCircle(global_data::image_right_rectified,right_point_blue,1,cvScalar(10000,10,10,10),1);
		
		cvCircle(global_data::image_left_rectified ,left_point_green ,1,cvScalar(10,10000,10,10),1);
		cvCircle(global_data::image_right_rectified,right_point_green,1,cvScalar(10,10000,10,10),1);


		//CARRYOUT BM STEREO CORRESPONDACNE AND PROJECT TO 3D
		calculate_correspondance_data(global_data::image_left_rectified,global_data::image_right_rectified,main_variables::Q);  


		//Carryout Coloured Blob tracking If enabled
		#if BLOB_TRACKING==1
		//THRESHOLD DATA FOR OBJECT ID
		correspondance_data::threholded_image = GetThresholdedImage_RED(global_data::image_left_rectified);

		//DETECT BLOB IN THESHOLDED IMAGE
		blobDetector.detect(  correspondance_data::threholded_image, keyPoints );

	
		//PALCE POINT AT OBJECT CENTER
		for (int i=0; i<keyPoints.size(); i++){
			Object_one.x=keyPoints[i].pt.x;
			Object_one.y=keyPoints[i].pt.y;
			cvCircle(global_data::image_left_rectified , Object_one ,1,cvScalar(100000,10000,10000,10),10);

		}

		//CALCULATE DEPTH OBJECT DEPTH FROM DISPARITY MAP
		calculate_correspondance_depth_tracking(keyPoints,main_variables::Q);
		#endif
		

		// UPDATE DISPLAY
		display_update(CAM1|CAM2|DISP|PROJ|RAW_DISP|PARAM);


		//*------------------ CALC 3D POSITION UPON SPACE BAR PRESS  -----------------------------------------------------------------------*/
		
		//Save Dispalys
		if(key==S_KEY){
			calculate_correspondance_data_save(global_data::image_left_rectified,global_data::image_right_rectified,main_variables::Q);  
		}

		//Caluclate Dpeth Vis Manual Correspondance
		if(key==SPACE_BAR_KEY){
			 manual_depth_calc();
		} 



	} //END MAIN WHILE LOOP

	display_destroy(CAM1|CAM2|DISP|PROJ|PARAM|RAW_DISP);
	cvReleaseStereoBMState(&(global_data::BMState));
	webcam_release();
	_CrtDumpMemoryLeaks();
	return 0;
}



// Interupt Handler 
void interpret_args(int argc, const char* argv[]){

   	if (argc > 1) {
		args::number_of_frames = atoi( argv[1] );
		if(args::number_of_frames==0){
			args::calibrate_procedure=true;
			args::recapture=false;
			
		}
		else{

			args::calibrate_procedure=true;
			args::recapture=true;
		}
	}
	else{
		args::calibrate_procedure=false;
		args::number_of_frames=NUMBER_TO_CAPTURE;
		args::recapture=false;
	}
}
	
//*------------------ RIGHT CAMERA MOUSE INTERUPTS ------------*/
void left_image_mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event){
		case CV_EVENT_LBUTTONDOWN:
			calculate_correspondance_depth(cvPoint(x,y),main_variables::Q);
			printf("\n");
			left_point_red.x=x;
			left_point_red.y=y;


			break;

        case CV_EVENT_RBUTTONDOWN:
			calculate_correspondance_depth(cvPoint(x,y),main_variables::Q);
			printf("\n");
			left_point_blue.x=x;
			left_point_blue.y=y;

	
			break;


        case CV_EVENT_MBUTTONDOWN:
			calculate_correspondance_depth(cvPoint(x,y),main_variables::Q);
			printf("\n");
			left_point_green.x=x;
			left_point_green.y=y;

			break;		
    }


}

//*------------------ LEFT CAMERA MOUSE INTERUPTS ------------*/
void right_image_mouseHandler(int event, int x, int y, int flags, void* param)
{
    switch(event){
        case CV_EVENT_LBUTTONDOWN:
			right_point_red.x=x;
			right_point_red.y=y;


			break;

        case CV_EVENT_RBUTTONDOWN:
			right_point_blue.x=x;
			right_point_blue.y=y;


			break;
        case CV_EVENT_MBUTTONDOWN:
			right_point_green.x=x;
			right_point_green.y=y;

	
			break;

    }
}

//*------------------ LEFT CAMERA MOUSE INTERUPTS ------------*/
void disp_image_mouseHandler(int event, int x, int y, int flags, void* param)
{
    switch(event){
        case CV_EVENT_LBUTTONDOWN:
			disp_point_red.x=x;
			disp_point_red.y=y;
			break;

        case CV_EVENT_RBUTTONDOWN:
			disp_point_blue.x=x;
			disp_point_blue.y=y;
			break;
        case CV_EVENT_MBUTTONDOWN:
			disp_point_green.x=x;
			disp_point_green.y=y;
			break;

    }
}
void manual_depth_calc(void){

			printf("\n Calcualting Position Data ...");


			//*------------------ 2D DATA   --------------------------------------------------------------------------------

			difference_left_red_blue_2D.x=left_point_red.x-left_point_blue.x;
			difference_right_red_blue_2D.x=left_point_red.x-right_point_blue.x;
			difference_left_red_blue_2D.y=left_point_red.y-left_point_blue.y;
			difference_right_red_blue_2D.y=left_point_red.y-right_point_blue.y;


			two_norm_difference_left_red_blue_2D = sqrt(difference_left_red_blue_2D.x*difference_left_red_blue_2D.x + difference_left_red_blue_2D.y*difference_left_red_blue_2D.y);
			two_norm_difference_right_red_blue_2D= sqrt(difference_right_red_blue_2D.x*difference_right_red_blue_2D.x + difference_right_red_blue_2D.y*difference_right_red_blue_2D.y);
				


			//*------------------ RED POINT (3D)  --------------------------------------------------------------------------------


	
			
			


			Object_Position_red.x=(left_point_red.x)*main_variables::Q->data.db[0] + main_variables::Q->data.db[3];
			Object_Position_red.y=(left_point_red.y)*main_variables::Q->data.db[5] + main_variables::Q->data.db[7];
			Object_Position_red.z= main_variables::Q->data.db[11];

			

			Object_Position_red.x = Object_Position_red.x/W;
			Object_Position_red.y = Object_Position_red.y/W;


			W = D *(main_variables::Q->data.db[14]) + main_variables::Q->data.db[15];
			Object_Position_red.z =	Object_Position_red.z/W;



			two_norm_red=sqrt(Object_Position_red.x*Object_Position_red.x+Object_Position_red.y*Object_Position_red.y+Object_Position_red.z*Object_Position_red.z);


			//*------------------ RED POINT (3D)  --------------------------------------------------------------------------------


	
			D=left_point_green.x-right_point_green.x;
			


			Object_Position_green.x=(left_point_green.x)*main_variables::Q->data.db[0] + main_variables::Q->data.db[3];
			Object_Position_green.y=(left_point_green.y)*main_variables::Q->data.db[5] + main_variables::Q->data.db[7];
			Object_Position_green.z= main_variables::Q->data.db[11];

			W = D *(main_variables::Q->data.db[14]) + main_variables::Q->data.db[15];

			Object_Position_green.x = Object_Position_green.x/W;
			Object_Position_green.y = Object_Position_green.y/W;
			Object_Position_green.z = Object_Position_green.z/W;



			two_norm_red=sqrt(Object_Position_red.x*Object_Position_red.x+Object_Position_red.y*Object_Position_red.y+Object_Position_red.z*Object_Position_red.z);


			//*------------------ BLUE POINT (3D)  -----------------------------------------------------------------------------


			D=left_point_blue.x-right_point_blue.x;
		

			Object_Position_blue.x=(left_point_blue.x)*main_variables::Q->data.db[0] + main_variables::Q->data.db[3];
			Object_Position_blue.y=(left_point_blue.y)*main_variables::Q->data.db[5] + main_variables::Q->data.db[7];
			Object_Position_blue.z= main_variables::Q->data.db[11];

			W = D *(main_variables::Q->data.db[14]) + main_variables::Q->data.db[15];

			Object_Position_blue.x = Object_Position_blue.x/W;
			Object_Position_blue.y = Object_Position_blue.y/W;
			Object_Position_blue.z = Object_Position_blue.z/W;



			two_norm_blue=sqrt(Object_Position_blue.x*Object_Position_blue.x+Object_Position_blue.y*Object_Position_blue.y+Object_Position_blue.z*Object_Position_blue.z);



			//*------------------ DIFFERNCE VECTOR   -----------------------------------------------------------------------------

			difference_red_blue.x=Object_Position_red.x-Object_Position_blue.x;
			difference_red_blue.y=Object_Position_red.y-Object_Position_blue.y;
			difference_red_blue.z=Object_Position_red.z-Object_Position_blue.z;


			two_norm_difference_red_blue=sqrt(difference_red_blue.x*difference_red_blue.x+difference_red_blue.y*difference_red_blue.y+difference_red_blue.z*difference_red_blue.z);

			printf(" Done\n");	


			printf("  ________________________________________________________\n");
			printf(" |             |             |             |              |\n");
			printf(" |   Depth     |  Red        |   Green     |    Blue      |\n");
			printf(" |   Data      |_____________|_____________|______________|\n");
			printf(" |   (cm)      |   %6.1f    |    %6.1f   |     %6.1f   |\n",Object_Position_red.z,Object_Position_green.z,Object_Position_blue.z);
			printf(" |_____________|_____________|_____________|______________|\n");

}