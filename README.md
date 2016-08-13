# Stereo Webcam Depth Detection

Computer vision project to get depth information from a pair of low cost webcams. The developed code uses openCV and largely follows from work done by Martin Peris described by http://martinperis.blogspot.com.au/2011/01/opencv-stereo-camera-calibration.html. 

The code is able to track colored objects and report their distance in real time. 

A memory based algorithm in also developed to improve the stability of the depth image. 

## Requirements

Visual Studio 2012 was used for this project. The project files are avaliable with the source code.

The hardware requirement for the code is simply two USB webcams plugged into the PC as can be seen below. As is it important that the webcams stay in a fixed postion after calibration (more about calibration below) a clamp was fabricated during this project.

<img src="https://github.com/mkokshoorn/Stereo_Webcam_Depth_Detection/blob/master/Device_image.jpg" width="400">

## Building the Code
The code was developed using visual studio 2012 and openCV 2.4.3. Install both of these, set the appropriate environment variables and open the '.sln' file in the source_code folder to open the project. Build the code in release mode to get CVStereoDepth.exe.

## Calibration

To run the main program, the stereo vision pair of webcams need to be calibrated. This can be done by running "CVStereoDepth.exe" and passing an argument of the number of calibration capture frames desired as an additional argument. For example, to calibrate with 10 images,

> \>CVStereoDepth.exe 10

The calibration code requires images of chessboard seen by both cameras to calculate intrinsic and extrinsic camera parameters.  The dimensions of chessboard need to be #defined in stdafx.h as NX and NY, however the values currently correspond to an A4 printed version of the chessboard at http://1.bp.blogspot.com/_0zx-4pM8HnA/TSltkqjXscI/AAAAAAAAAI0/45MJoCS5-f8/s1600/OpenCV_Chessboard.png and as follows:

<img src="http://1.bp.blogspot.com/_0zx-4pM8HnA/TSltkqjXscI/AAAAAAAAAI0/45MJoCS5-f8/s1600/OpenCV_Chessboard.png" width="400">

When the calibration is running, each frame can then be captured manual by pressing spacebar or initiate the auto capture sequence by pressing C. This will take a shot every 0.5 seconds (#defined in stdafx.h as SECONDS_BETWEEN_CAPTURES). Once this is complete or at any time, presses ESC to finish capturing and begin calibration. 

The program will then save the appropriate calibration matrices inside the release file. Here the frames used for calibration will also be saved. The code can then be run by passing no argument to enter the stereo webcam system mode. The program can then be terminated by pressing ESC while one of the openCV windows is active.

For best results, try to capture the chessboards in a range of different positions and angles. 

## Running Depth Detection Program

Once the calibration procedure has been run and the code is run from the command line parsing no arguments the system is then able to be used. This will open seven windows, including the command window, these are explained as follows:
-	WEBCAM ONE (LEFT)/ WEBCAM TWO (RIGHT) – These two windows represent the rectified left and right stereo images respectively. Note: If left is not left or right is not right, either swap the USB connections or swap the 0 and 1 in the function call webcam_init() from sCVStereoDepth.cpp .
-	RAW DISPARITY – This represents the output image from the block matching stereo correspondence function. 
-	DISPARIY – This represents the memory disparity image that can be activated through BM STEREO CORRESPONDANCE PARAMETERS.
-	BM STEREO CORRESPONDANCE PARAMETERS allows the user to modify the parameters associated with the block matching algorithm.
-	RED OBJECT FINDER – Shows the red threshold image, for object tracking.

The main fucntion is found within CVStereoDepth.cpp and carries out the following tasks:
-	Calibration, if specified.
-	Enters a loop until ESC is pressed.
-	Rectifies left and right image.
-	Carries out block matching correspondence.
-	Update Displays.
-	Calculate Depth if required.

Depth detection can then be done in one of three ways:
1) The first is automated and uses the red object tracking to overlay the distance to the red object on the left image. This will only occur if there is a large blob of red that can be seen in the RED OBJECT FINDER window.  
2) The second is simply to click on a pixel in the WEBCAM ONE (LEFT) window, the calculated (x,y,z) co-ordinate is then printed through the command window. 
3) Tthe final method allows the user to manually correspond features in both images. This is done by left, right or middle the clicking on the feature in the left image and then again in the right image. Note that left click places a red dot, right click places a blue dot and middle click places a green dot. The disparity is calculated from the difference in positioning of the same coloured dots in each image. To calculate the depth once the dots have been placed, press space bar to print the information to the command window. 

An example video of the program running can be seen in DemoWithText.mp4. A screenshot can also be seen as follows

![](https://github.com/mkokshoorn/Stereo_Webcam_Depth_Detection/blob/master/ProgramScreenshot.png)

## Source Code Overview

The Stereo Vision project source code consists of 6 modules. They are briefly explained as follows:
-	CVStereoDepth.cpp – This consists of the main fucntion and scheduling of tasks previously outlined.
-	Stereo_calibrate.cpp – This is an open source piece of code provided by Martin Perris. See Academic paper for details. It essentially carries out the calibration form a list of image file locations and the size of the chessboard.
-	CVStereoCalibration.cpp – This is a higher level module for the calibration process to allow the auto capturing sequence.
-	Globaldata.cpp – This module contains a name space in order for global data to be shared.
-	CVStereoWebcam.cpp – Deals with the capturing and acquiring of web cam images and also the display functions.
-	CVStereoCorrespondance.cpp – This function deals with the block matching correspondence process and the calculations of depth associated with it. Additionally it sets up the block matching parameters windows parameters and slider interface.

## Known Issues
There appears to be a memory leak associated with the Blob Library causing the program to crash. This can be removed by changing BLOB_TRACKING (#defined in stdafx.h) from 1 to 0. The issue causes to program to crash after a long duration of time due to memory exhaustion.

It is important to note that the slider values for the block matching parameters do not exactly match those passed to the correspondence fucntion. This is due to the sliders inability to go below zero. Therefore in the case of several of the values they are shifted and scaled. i.e. A  0  to 100 slider may represent -100 to 100. 


