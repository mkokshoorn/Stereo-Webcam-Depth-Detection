// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#pragma warning(disable: 4244) 
#pragma warning(disable: 4101) 
#pragma warning(disable: 4018) 


#include <stdio.h>
#include <tchar.h>


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#define RECALIBRATION					1
#define RECAPTURE						0

#define AUTO_CAPTURE					0
#define NUMBER_TO_CAPTURE				5

#define DISPLAY_CORNERS					0
#define USE_UNDISTORTED					1
#define DISPLAY_DISARITY_AND_RECTIFED	0
#define USE_UNCALIBRATED				0   // Hartley method
#define IS_VERTICAL_STEREO  			false	

#define AUTO_CAPTURE_DELAY				3
#define ONE_SECOND						1000
#define SECONDS_BETWEEN_CAPTURES		500

#define ESC_KEY							27
#define C_KEY							99   
#define SPACE_BAR_KEY					32
#define D_KEY							100
#define S_KEY							115
#define LEFT_MOUSE_BUTTON				1
#define RIGHT_MOUSE_BUTTON				2

#define BLOB_TRACKING					1


#define MEMORY_WEIGHTING			0
#define NEW_WEIGHTING				(100-MEMORY_WEIGHTING)

#define NX								9
#define NY								6
#define SQUARESIZE						2.8

#define SHOW_2D_DATA					1
#define SHOW_3D_DATA					1
#define SHOW_DEPTH_DATA					1

#define SHOW_NORMAL_IMAGES				0

#define SHOW_DISPARITY_GC					0
#define SHOW_DISPARITY_BM					1


