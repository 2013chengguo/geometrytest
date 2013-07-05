/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		OpenGL Control class	

	Version History:
		v1.00		Original class	(Daniel Vivas 03/08/02)
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


#ifndef OPENGL_H
#define OPENGL_H

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>    // Header File For Windows
#include <gl\gl.h>		// Header File For The OpenGL32 Library
#include <gl\glu.h>		// Header File For The GLu32 Library
#include "win32.h"		// the windows class
#include "state.h"		// the state manager	
#include "timer.h"		// Timing functions

#ifdef _MSC_VER
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif

namespace NeHe {

class OpenGL {
   protected:
      bool			FullScreen;       // Fullscreen Flag Set To Fullscreen Mode By Default
      HDC			hDC;              // Private GDI Device Context
      HGLRC			hRC;              // Permanent Rendering Context
      HWND			hWnd;             // Holds Our Window Handle
      HINSTANCE		hInstance;        // Holds The Instance Of The Application
	  int			stencil;		  // stencil buffer bits
	  int			bpp;			  // bits per pixel
	  Window		*win;			  // Our Window Class
	  StateManager	*ste;			  // the state manager
	  Frame			*fme;			  // the frame timer
	  // Helper functions
      bool			InitGL();			// Setup OpenGL
	  unsigned char ComponentFromIndex(int i,unsigned int nbits,unsigned int shift); // for palette function
	  void			SetPalette();		// Setup the palette in 8 bit mode
   public:
      OpenGL();
	  ~OpenGL();
      bool	CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
      bool	DrawGLScene(GLvoid);
      void	SetFullScreen(bool fullscreen)	{ FullScreen=fullscreen; };
      bool	GetFullScreen() const			{ return FullScreen; };
      HDC			GetDC() const			{ return hDC; };
      HWND			GetHWnd() const			{ return hWnd; };
	  Window	   *GetWindow() const		{ return win; };
	  StateManager *GetState() const		{ return ste; };
	  int	GetStencil() const				{ return stencil; };
      void	ReSizeGLScene(int width, int height);
      void	KillGLWindow();
};

typedef struct ControlData {
					// Control Data
					bool	quit;			// quit command
					bool	resize;			// resize display
					// I/O data
					int		mx,my;			// mouse position
					bool	left,right;		// mouse button positions
					bool	*keys;			// the key values
					// General information (change for resizing)
					int		width;			// width of display
					int		height;			// height of display
					int		bpp;			// bits per pixel of display
					bool	windowed;		// whether the display is fullscreen
					// State Manager
					StateManager *state;
					// Timing information
					float	framelength;	// length of the frame in milliseconds
					int		fps;			// frames per second
				}ControlData;

}; // namespace NeHe

extern "C" void DrawScene(NeHe :: OpenGL *gl,NeHe :: ControlData *cont);

#endif;