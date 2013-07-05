/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Splash Screen Handling function	

	Version History:
		v1.00		Copied from my ill-fated tutorial series:) (GA 29/10/02)

	Licence:

	This SDK/library is free software, written primarily for
	teaching purposes. You can redistribute and modify it to 
	your heart's content. However, any use of this library is 
	at the user's own risk. There is no warranty. The authors 
	will not be held responsible for ANYTHING that happens as 
	a result of the use of this software. All financial/emotional/health 
	problems or anything else that happens after using this 
	software is therefore the user's own responsibility. 
	Nevertheless, if the user finds it useful, an email or a 
	mention in any credits would be appreciated, but is not 
	necessary.
*/

#ifndef SPLASH
#define SPLASH

#define STRICT
#include <windows.h>

namespace NeHe {

// the resolution type
typedef enum	SplashResolution { sr640x480,sr800x600,sr1024x768 };
// the bit depth
typedef enum	SplashDepth { sd8bit,sd16bit,sd32bit };

extern "C" {

//	Create a splash button
//	Params:
//		fname:	name of configuration file
//		res:	pointer to resolution selected
//		depth:	pointer to bit depth selected
//		full:	pointer to fullscreen flag
bool DoSplash(char *fname,SplashResolution *res, SplashDepth *depth,bool *full);

};

// Configuration file format
//
//	Line 1	:	name of splash screen bitmap file with all buttons up
//	Line 2	:	name of splash screen bitmap file with all buttons down
//	Line 3	:	Button info for 640x480 button
//	Line 4	:	Button info for 800x600 button
//	Line 5	:	Button info for 1024x768 button
//	Line 6	:	Button info for 8 bit button
//	Line 7	:	Button info for 16 bit button
//	Line 8	:	Button info for 32 bit button
//	Line 9	:	Button info for Fullscreen button
//	Line 10	:	Button info for Go! button
//
//	Button information consists of four integers.
//	The first two are the (x,y) coordinates of the top-left of
//		the button, the second two are the (x,y) coordinates of
//		the bottom-right of the button.

}; // namespace NeHe

#endif