/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		WinMain function

	Version History:
		v1.00		Original file	(Daniel Vivas 03/08/02)
		v1.10		Rewritten for inclusion in SDK	(GA 29/10/02)

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

#include "opengl.h"
#include "splash.h"

using namespace NeHe;

int WINAPI WinMain(HINSTANCE hInstance,     // Instance
                   HINSTANCE hPrevInstance, // Previous Instance
                   LPSTR     lpCmdLine,     // Command Line Parameters
                   int       nCmdShow)      // Window Show State
{
   OpenGL  Game;

	// Run Splash Box
    SplashResolution	res;		// the resolution choices
	SplashDepth			depth;		// the depth choices
	bool				fs;			// whether to set fullscreen
    if(!DoSplash("splash.cfg",&res,&depth,&fs))
		return 1;

	// setup our variables
	int width,height;		// dimensions of window
	int bpp;				// bits per pixel

	// first set dimensions
	switch(res)
	{
		case sr640x480:		width=640;	height=480; break;
		case sr800x600:		width=800;	height=600; break;
		case sr1024x768:	width=1024; height=768; break;
		default:
			width=640; height=480;
	};

	// then bits per pixel
	switch(depth)
	{
		case sd8bit:	bpp=8;	break;
		case sd16bit:	bpp=16;	break;
		case sd32bit:	bpp=32;	break;
		default:
			bpp=16;
	};

	// set fullscreen flag
	Game.SetFullScreen((fs==true) ? true : false); 

	// Create Our OpenGL Window
	if (!Game.CreateGLWindow("NeHe's OpenGL Framework", width, height, bpp, Game.GetFullScreen()))
	{
		return 0; // Quit If Window Was Not Created
	}

	// get our window type
	Window *win=Game.GetWindow();
	bool finish=false;

	// run until escape pressed
	while(!finish)
	{
		// draw the scene
		finish=!Game.DrawGLScene();

		// Get the pressed key
		if(!finish)
			finish=win->GetKey(VK_ESCAPE);
	}

    // Shutdown
    Game.KillGLWindow(); // Kill The Window

    return 0;  // Exit The Program
}