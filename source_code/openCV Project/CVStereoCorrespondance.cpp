//CVCorrespondance.cpp
//Coder: Matt Kokshoorn
//		 University of Canterbury
//		 2013

#include "stdafx.h"
#include "CVStereoCorrespondance.h"


//Module Data
namespace module_data{
//*------------------ Slider Parameters ------------*/ 
	int nStereoPreFilterType		= CV_STEREO_BM_NORMALIZED_RESPONSE;
	int nStereoPreFilterSize		=  9; //33
	int nStereoPreFilterCap			= 63; //40
	int nStereoSADWindowSize		=  8; //25
	int nStereoMinDisparity			= 200+67;//33; //-50
	int nStereoNumDisparities		=  9; //10
	int nStereoTextureThreshold		=  4; //5
	int nStereoUniquenessRatio		=  1; //2
	int nStereoSpeckleWindowSize	=  2; //1
	int nStereoSpeckleRange			=  0; //0
	int nStereoTrySmallerWindows	=  0; //0
	int nWindnowMemory				=  1;
}


//Locally Held Data
namespace correspondance_data{
	 	CvMat* disp_left ;
		CvMat* vdisp_left ;
		CvMat* smoothedvdisp_left ;
		CvMat* Image3D_left ;
		IplImage *grey_left_r ;
		IplImage *grey_right_r;
	 	CvMat* disp_right ;
		CvMat* vdisp_right ;
		CvMat* smoothedvdisp_right;
		CvMat* Image3D_right ;
		IplImage * threholded_image;
		CvMat* disp_left_memory;
		CvMat* vdisp_left_memory;
		cv::Mat pixel_norm_disp; 
}


//Interupt for the block mathing correspondance paramter sliders.
void sliderHandler(void)
{
	//Pre Filter Type
	if(module_data::nStereoPreFilterType==0)		global_data::BMState->preFilterType	   = CV_STEREO_BM_NORMALIZED_RESPONSE;
	else if(module_data::nStereoPreFilterType==1)	global_data::BMState->preFilterType    = CV_STEREO_BM_XSOBEL;

	//Pre Filter Size 5-255 (odd)
	global_data::BMState->preFilterSize=(module_data::nStereoPreFilterSize+5)*2.4;    
	if( global_data::BMState->preFilterSize % 2 == 0 )  global_data::BMState->preFilterSize++;

	//Pre Filter Cap 1-63
	global_data::BMState->preFilterCap=(module_data::nStereoPreFilterCap+1)*0.63;    
	if( global_data::BMState->preFilterCap  == 0 )  global_data::BMState->preFilterCap++;
		
	//SAD Windnow Size 
	global_data::BMState->SADWindowSize=(module_data::nStereoSADWindowSize+5)*2.0; 
	if( global_data::BMState->SADWindowSize % 2 == 0 ) global_data::BMState->SADWindowSize++;
	if( global_data::BMState->SADWindowSize >= MIN(global_data::camStreamSize.width, global_data::camStreamSize.height) )
	global_data::BMState->SADWindowSize = MIN(global_data::camStreamSize.width, global_data::camStreamSize.height);

	//Min Disparity -200 to 200
	global_data::BMState->minDisparity=(module_data::nStereoMinDisparity-200);

	//Number of Disparity 0 to 150
	global_data::BMState->numberOfDisparities=(module_data::nStereoNumDisparities+1)*16;

	//Texture Threshold 1-100
	global_data::BMState->textureThreshold=(module_data::nStereoTextureThreshold+1);

	//Uniqueness Ratio
	global_data::BMState->uniquenessRatio=(module_data::nStereoUniquenessRatio+1); 

		
	//Speckle Widnows Size 0-50
	global_data::BMState->speckleWindowSize = module_data::nStereoSpeckleWindowSize*0.5;

	// Speckle Range 0-100
	global_data::BMState->speckleRange =module_data::nStereoSpeckleRange;

	//Try Smaller Windows 0-1
	global_data::BMState->trySmallerWindows=module_data::nStereoTrySmallerWindows;
}


//Initilise the slider bars.
void init_sliderHandler(void){

	global_data::BMState = cvCreateStereoBMState();
	
	cvNamedWindow( PARAM_WIND, CV_GUI_EXPANDED);
	cvCreateTrackbar("PF-Typ"		, PARAM_WIND,&module_data::nStereoPreFilterType		, 1,   (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("PF-Size"		, PARAM_WIND,&module_data::nStereoPreFilterSize		, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("PF-Cap"		, PARAM_WIND,&module_data::nStereoPreFilterCap		, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("SADWinSiz"	, PARAM_WIND,&module_data::nStereoSADWindowSize		, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("Min Dspty"	, PARAM_WIND,&module_data::nStereoMinDisparity		, 400, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("# Disprty"	, PARAM_WIND,&module_data::nStereoNumDisparities	, 20,  (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("TxtreThrsh"	, PARAM_WIND,&module_data::nStereoTextureThreshold	, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("UniqRtio"		, PARAM_WIND,&module_data::nStereoUniquenessRatio	, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("SpcWindSiz"	, PARAM_WIND,&module_data::nStereoSpeckleWindowSize	, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("Spk Rnge"		, PARAM_WIND,&module_data::nStereoSpeckleRange		, 100, (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("Ty Sml Win"	, PARAM_WIND,&module_data::nStereoTrySmallerWindows	, 1  , (CvTrackbarCallback)sliderHandler);
	cvCreateTrackbar("Memory"	, PARAM_WIND,&module_data::nWindnowMemory		    , 1  , (CvTrackbarCallback)sliderHandler);
	sliderHandler();
}

//Initilise the local data.
void init_correspondance_data(void){

	 	correspondance_data::disp_left				= cvCreateMat( global_data::camStreamSize.height,global_data::camStreamSize.width, CV_16S );
		correspondance_data::vdisp_left				= cvCreateMat(global_data::camStreamSize.height,global_data::camStreamSize.width, CV_8U );
		correspondance_data::smoothedvdisp_left		= cvCreateMat(global_data::camStreamSize.height,global_data::camStreamSize.width, CV_8U );
		correspondance_data::Image3D_left			= cvCreateMat(global_data::camStreamSize.height, global_data::camStreamSize.width, CV_32FC3);
		correspondance_data::grey_left_r			= cvCreateImage(global_data::camStreamSize,IPL_DEPTH_8U,1);
		correspondance_data::grey_right_r			= cvCreateImage(global_data::camStreamSize,IPL_DEPTH_8U,1);
	 	correspondance_data::disp_right				= cvCreateMat( global_data::camStreamSize.height,global_data::camStreamSize.width, CV_16S );
		correspondance_data::vdisp_right			= cvCreateMat(global_data::camStreamSize.height,global_data::camStreamSize.width, CV_8U );
		correspondance_data::smoothedvdisp_right	= cvCreateMat(global_data::camStreamSize.height,global_data::camStreamSize.width, CV_8U );
		correspondance_data::Image3D_right			= cvCreateMat(global_data::camStreamSize.height, global_data::camStreamSize.width, CV_32FC3);
		correspondance_data::threholded_image       = cvCreateImage(global_data::camStreamSize,IPL_DEPTH_8U,1);
		correspondance_data::disp_left_memory       = cvCreateMat(global_data::camStreamSize.height,global_data::camStreamSize.width, CV_16S );
		correspondance_data::vdisp_left_memory       = cvCreateMat(global_data::camStreamSize.height,global_data::camStreamSize.width, CV_8U );
}


//Carry out block matching correspondance
void calculate_correspondance_data(IplImage* image_left_undistorted,IplImage* image_right_undistorted,CvMat * Q){
	
		//Convert each image to gresycale.
		cvCvtColor(image_left_undistorted,correspondance_data::grey_left_r,CV_RGB2GRAY);
		cvCvtColor(image_right_undistorted,correspondance_data::grey_right_r,CV_RGB2GRAY);

		//Calcualte the disparity map.
		cvFindStereoCorrespondenceBM( correspondance_data::grey_left_r, correspondance_data::grey_right_r, correspondance_data::disp_left, global_data::BMState);
		
		//Scale
		cvConvertScale(correspondance_data::disp_left, correspondance_data::disp_left, 1.0/16, 0 );

		//Implement Memory Image from correspondance
		cv::Mat in = correspondance_data::disp_left;
		int k=0;
		int null_value=in.at<signed __int16>(0,0);

		for(int i=0; i< (480);i++){
			for(int j=0;j<640;j++){
				if(module_data::nWindnowMemory==1){
					if((in.at<signed __int16>(i,j)!=null_value)&&(  !((-1*(Q->data.db[15])/(Q->data.db[14])) >in.at<signed __int16>(i,j)))){
						correspondance_data::disp_left_memory->data.s[k++]=(in.at<signed __int16>(i,j)*NEW_WEIGHTING+correspondance_data::disp_left_memory->data.s[k]*MEMORY_WEIGHTING)/100.0;
					}
					else{
						k++;
					}
				}
				else{
					correspondance_data::disp_left_memory->data.s[k++]=in.at<signed __int16>(i,j);
				}
			}
		}
		

		
		//Normalise the images for display
		cvNormalize( correspondance_data::disp_left_memory, correspondance_data::vdisp_left_memory, 0, 256, CV_MINMAX );
		cvNormalize( correspondance_data::disp_left, correspondance_data::vdisp_left, 0, 256, CV_MINMAX );
		
		//Reporject the image to 3D using calibration matrix Q.
		cvReprojectImageTo3D(correspondance_data::disp_left_memory, correspondance_data::Image3D_left, Q, true);	

}


//Fucntion Can be used to save saved Data by pressing the 's' key.
void calculate_correspondance_data_save(IplImage* image_left_undistorted,IplImage* image_right_undistorted,CvMat * Q){

		printf(" Saving 3D Projection...");
		//IMPLMENT IMAGES TO SAVE HERE...

		cvSaveImage("TestData/Left.jpg", image_left_undistorted);
		cvSaveImage("TestData/Right.jpg", image_right_undistorted);
		cvSaveImage("TestData/Disparity.jpg", correspondance_data::disp_left);
		cvSaveImage("TestData/DisparityMemory.jpg", correspondance_data::disp_left_memory);


		//END SAVE DATA
		printf(" Done.\n\n");
}


int calculate_correspondance_depth(CvPoint left_point ,CvMat *Q){
			//Calculate Depth From3D Projection Matrix
			cv::Mat New_Image3D_left=correspondance_data::Image3D_left;
			cv::Vec3f Depth_from_proj =  New_Image3D_left.at<cv::Vec3f>(left_point.y,left_point.x); 
			printf(" 3D Mapping: ( %.2f, %.2f , %.2f )", Depth_from_proj[0] , Depth_from_proj[1], Depth_from_proj[2]);

			return Depth_from_proj[2];
}





void calculate_correspondance_depth_tracking(cv::vector<cv::KeyPoint> keyPoints, CvMat *Q){

			//Calculate Depth From3D Projection Matrix
			cv::Mat New_Image3D_left=correspondance_data::Image3D_left;
			char image_text[50];
			CvFont font;

			cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.4, 1.4, 0, 2, 8);

			//Overaly text onto image displayign dpeth data.
			for(int i=0; i<keyPoints.size();i++){
					cv::Vec3f Depth_from_proj =  New_Image3D_left.at<cv::Vec3f>(keyPoints[i].pt.y,keyPoints[i].pt.x);
					sprintf(image_text,".   %1.2f",Depth_from_proj[2]);
					cvPutText(global_data::image_left_rectified, image_text , keyPoints[i].pt, &font  ,   CV_RGB(250,100,50));
			}

}

//Threshold image to RED,GREEN or BLUE for blob detction and thuis obkect tracking.
IplImage* GetThresholdedImage_RED(IplImage* frame){   
	   cvSmooth(frame, frame, CV_GAUSSIAN,3,3);
	   IplImage* imgHSV=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
		cvCvtColor(frame, imgHSV, CV_BGR2HSV); 
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);


       cvInRangeS(frame,  CV_RGB(180,   0,  0),  CV_RGB(255,100,100), imgThresh); //RED
	   //cvInRangeS(frame,  CV_RGB(  0, 150,  0),  CV_RGB(150,255,150), imgThresh); //GREEN
	   //cvInRangeS(frame,  CV_RGB(  0,   0,150),  CV_RGB(100,100,255), imgThresh); //BLUE


	   cvSmooth(imgThresh,imgThresh, CV_GAUSSIAN,3,3);
       return imgThresh;
} 


