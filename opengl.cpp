/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		OpenGL Control class	

	Version History:
		v1.00		Original class	(Daniel Vivas 03/08/02)
		v1.10		Rewritten for inclusion in SDK	(GA 29/10/02)
		v1.11		Fixed palette significance		(GA 13/01/03)

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
#include "nexception.h"
#include <new>

namespace NeHe {

// All Setup For OpenGL Goes Here
OpenGL :: OpenGL() 
{
   FullScreen = true;
   hInstance  = NULL;
   hDC        = NULL;       // Private GDI Device Context
   hRC        = NULL;       // Permanent Rendering Context
   hWnd       = NULL;       // Holds Our Window Handle
   win		  = NULL;		// Our Window
   ste		  = NULL;		// State manager
   stencil	  = 0;			// stencil buffer bits
}

OpenGL :: ~OpenGL()
{
	// close the window
	if(win)
		this->KillGLWindow();
	// and delete
	if(win)
		delete win;
}

bool OpenGL :: InitGL() 
{
   glShadeModel(GL_SMOOTH);                           // Enable Smooth Shading
   glClearColor(0.0f, 0.0f, 0.0f, 0.5f);              // Black Background
   glClearDepth(1.0f);                                // Depth Buffer Setup
   ste->SetDepthTest(true);                           // Enables Depth Testing
   glDepthFunc(GL_LEQUAL);                            // The Type Of Depth Testing To Do
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
   glEnable(GL_COLOR_MATERIAL);						  // Enable Coloring Of Material

   // switch on texturing
   ste->SetTexturing(true);

   return true;
}

// Here's Where We Do All The Drawing
bool OpenGL :: DrawGLScene(GLvoid) 
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
   glLoadIdentity();                                   // Reset The Current Modelview Matrix

	// Get IO information
    ControlData	data;			// data struct
	// first keyboard
    data.keys=win->GetKeys();
	// then mouse
	win->GetMousePosition(data.mx,data.my);
	win->GetMouseButton(data.left,data.right);
	// clear control flags
	data.quit=data.resize=false;
	// get information about display
	win->GetDimensions(data.width,data.height);
	data.bpp=bpp;
	data.windowed=!FullScreen;
	// and copy over the state manager
	data.state=ste;
	// Update frame and copy over data
	fme->Update();
	data.framelength=fme->GetLength();
	data.fps=fme->GetFPS();

	// 
	//	Draw the scene
    //

	try{
		DrawScene(this,&data);
	}
	catch(NeHeException ex) {
		std :: string	info;			// the information about the exception
		char			nostr[10];		// string for the line number

		// convert line number to string
		_itoa(ex.GetLine(),nostr,10);

		// build the information string
		info="Exception at\n\nLine :\t";
		info+=nostr;
		info+="\nFile :\t";
		info+=ex.GetFileName();
		info+="\nError:\t";
		info+=ex.GetError();
		info+="\n\nThis application will now exit.";

		// display dialog box
		MessageBox(NULL,info.c_str(),"NeHe Exception",MB_OK | MB_ICONEXCLAMATION);

		// now exit
		return false;
	};

	//
	// draw to screen
	//

	// make sure(ish) that we've finished
	glFlush();
	// and then swap the buffers
	SwapBuffers(hDC);

	// have we been asked to quit?
	if(data.quit)
		return false;

	// have we been asked to resize?
	if(data.resize)
	{
		char title[64];						// title of the present window

		// save the title
		GetWindowText(hWnd,title,64);
		// anf the fullscreen flag
		bool full=FullScreen;
		// close the present window
		this->KillGLWindow();
		// and open a new one
		if(!this->CreateGLWindow(title,data.width,data.height,data.bpp,data.windowed))
		{
			// this failed, so return to a basic display
			if(!this->CreateGLWindow(title,640,480,16,full))
			{
				// give up
				MessageBox(NULL,"Fatal Error: Window cannot be opened.",
								"NeheSDK Fatal Error N1",MB_ICONEXCLAMATION|MB_OK);
				// and return
				return false;
			}
		}
	}

	// check for sizing
	if(win->BeenSized())
	{
		int width,height;							// new dimensions
		win->GetDimensions(width,height);			// and get them
		this->ReSizeGLScene(width,height);			// now resize
	}

	return true; // Everything Went OK
}



// Resize And Initialize The GL Window
GLvoid OpenGL :: ReSizeGLScene(GLsizei width, GLsizei height) 
{
   // Prevent A Divide By Zero By
   if (height == 0) {
      height = 1;                   // Making Height Equal One
   }

   glViewport(0, 0, width, height); // Reset The Current Viewport

   glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
   glLoadIdentity();                // Reset The Projection Matrix

   // Calculate The Aspect Ratio Of The Window
   gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

   glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
   glLoadIdentity();           // Reset The Modelview Matrix
}

//
// Palette constants
//

// convert component to index functions 
static unsigned char threeto8[8] = { 
	0, 0111>>1 , 0222>>1 , 0333>>1 , 0444>>1 , 0555>>1 , 0666>>1,0377
		};
static unsigned char twoto8[4] = { 0,0x55,0xaa,0xff };
static unsigned char oneto8[2] = { 0,255 };

static int palDefaultOverride[13] = {
    0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

// Windows basic palette
static PALETTEENTRY palDefaultPalEntry[20] = {
    { 0,   0,   0,    0 }, //0
    { 0x80,0,   0,    0 }, 
    { 0,   0x80,0,    0 }, 
    { 0x80,0x80,0,    0 }, 
    { 0,   0,   0x80, 0 },
    { 0x80,0,   0x80, 0 },
    { 0,   0x80,0x80, 0 },
    { 0xC0,0xC0,0xC0, 0 }, //7

    { 192, 220, 192,  0 }, //8
    { 166, 202, 240,  0 },
    { 255, 251, 240,  0 },
    { 160, 160, 164,  0 }, //11

    { 0x80,0x80,0x80, 0 }, //12
    { 0xFF,0,   0,    0 },
    { 0,   0xFF,0,    0 },
    { 0xFF,0xFF,0,    0 },
    { 0,   0,   0xFF, 0 },
    { 0xFF,0,   0xFF, 0 },
    { 0,   0xFF,0xFF, 0 },
    { 0xFF,0xFF,0xFF, 0 }  //19
  };

// Component from index from MSDN
unsigned char OpenGL :: ComponentFromIndex(int i,unsigned int nbits,unsigned int shift)
{
	unsigned char val = (unsigned char)(i>>shift);

	switch(nbits)
	{
		case 1:
			val&=0x1;
			return oneto8[val];
		case 2:
			val&=0x3;
			return twoto8[val];
		case 3:
			val&=0x7;
			return threeto8[val];
		default:
			return 0;
	};
}

// from "Windows Palettes in RGBA mode" on MSDN
void OpenGL :: SetPalette()
{
	int						ct;		// counter
	int						r,g,b;	// colour counters
	int						num;	// number of colours
	HPALETTE				hpal;	// handle to palette
	LOGPALETTE				*pal;	// our palette
	PIXELFORMATDESCRIPTOR	pf;		// pixel format
	PALETTEENTRY			*ppe;	// the remaining palette

	// do we need this palette
	DescribePixelFormat(hDC,GetPixelFormat(hDC),sizeof(PIXELFORMATDESCRIPTOR),&pf);
	if(!(pf.dwFlags & PFD_NEED_PALETTE))
		return;

	// allocate our palette
	pal=(LOGPALETTE *)new BYTE [sizeof(LOGPALETTE)+(256*sizeof(PALETTEENTRY))];

	// setup the palette
	pal->palVersion=0x300;
	pal->palNumEntries=256;

	// get the number of colours
	num=1<<pf.cColorBits;

	// fill the colour array
	for(ct=0; ct<num; ct++)
	{
		pal->palPalEntry[ct].peRed=
			this->ComponentFromIndex(ct,pf.cRedBits,pf.cRedShift);
		pal->palPalEntry[ct].peGreen=
			this->ComponentFromIndex(ct,pf.cGreenBits,pf.cGreenShift);
		pal->palPalEntry[ct].peBlue=
			this->ComponentFromIndex(ct,pf.cBlueBits,pf.cBlueShift);
		pal->palPalEntry[ct].peFlags=0;
	}

	// sort out windows colours for 3:3:2
	if( (pf.cRedBits==3) &&  (pf.cRedShift==0) &&
		(pf.cGreenBits==3) &&  (pf.cGreenShift==3) &&
		(pf.cBlueBits==2) &&  (pf.cBlueShift==6) )
	{
		for(ct=1; ct<=12; ct++)
			pal->palPalEntry[palDefaultOverride[ct]]=
									palDefaultPalEntry[ct];
	}else{
		// Get the system colours
		GetSystemPaletteEntries(hDC,0,10,&pal->palPalEntry[0]);
		GetSystemPaletteEntries(hDC,246,10,&pal->palPalEntry[246]);
		// get start position
		ppe=&pal->palPalEntry[10];
		// create a colour cube
		for(r=0; r<6; r++)
			for(g=0; g<6; g++)
				for(b=0; b<6; b++)
				{
					ppe->peRed=(unsigned char)(r*255/6);
					ppe->peGreen=(unsigned char)(g*255/6);
					ppe->peBlue=(unsigned char)(b*255/6);
					ppe->peFlags=PC_NOCOLLAPSE;
					ppe++;
				}
		// and grey scale
		for(ct=0; ct<20; ct++)
		{
			ppe->peRed=(unsigned char)(ct*255/6);
			ppe->peGreen=(unsigned char)(ct*255/6);
			ppe->peBlue=(unsigned char)(ct*255/6);
			ppe->peFlags=PC_NOCOLLAPSE;
			ppe++;
		}
	}

	// set the palette
	hpal=CreatePalette(pal);
	SelectPalette(hDC,hpal,TRUE);
	RealizePalette(hDC);
}

// for Visual C++ and Dev C++
#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN 4
#endif

/*   This Code Creates Our OpenGL Window.  Parameters Are:                  *
 *   title          - Title To Appear At The Top Of The Window              *
 *   width          - Width Of The GL Window Or Fullscreen Mode             *
 *   height         - Height Of The GL Window Or Fullscreen Mode            *
 *   bits           - Number Of Bits To Use For Color (8/16/24/32)          *
 *   fullscreenflag - Use Fullscreen Mode (true) Or Windowed Mode (false)   */
 
bool OpenGL :: CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) 
{
   GLuint   PixelFormat; // Holds The Results After Searching For A Match

   FullScreen = fullscreenflag;      // Set The Global Fullscreen Flag
   // save bits per pixel
   bpp=bits;
   
   // Attempt Fullscreen Mode?
   if (FullScreen) {
      DEVMODE dmScreenSettings;                                 // Device Mode
      memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // Makes Sure Memory's Cleared
      dmScreenSettings.dmSize       = sizeof(dmScreenSettings); // Size Of The Devmode Structure
      dmScreenSettings.dmPelsWidth  = width;                    // Selected Screen Width
      dmScreenSettings.dmPelsHeight = height;                   // Selected Screen Height
      dmScreenSettings.dmBitsPerPel = bits;                     // Selected Bits Per Pixel
      dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

      // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
      if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
         // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
         if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                        "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
            FullScreen = false; // Windowed Mode Selected.  Fullscreen = false
         }else{
            // Pop Up A Message Box Letting User Know The Program Is Closing.
            MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);

            return false; // Return false
         }
      }
   }

   //
   //	Open the Window
   //

   // allocate the window
   try {
   win=new Window;
   }
   catch(std :: bad_alloc) {
	   return false;
   }

   // and set it up
   if(FullScreen)
   {
	   if(!win->Setup(0,0,width,height,title))
	   {
		  KillGLWindow(); // Reset The Display
		  MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		  return false; // Return false
	   }
   }else{
	   if(!win->SetupTitle(0,0,width,height,title))
	   {
		  KillGLWindow(); // Reset The Display
		  MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		  return false; // Return false
	   }
   }

   // get all the info we might need
   hInstance=win->GetInstance();
   hWnd=win->GetHandle();
   // Did We Get A Device Context?
   if (!(hDC = ::GetDC(hWnd))) {
      KillGLWindow(); // Reset The Display
      MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

      return false; // Return false
   }

   // set z and stencil bits
   int buffbits;
   if(bits==8)
   {
	   buffbits=16;
	   // while we're here, set the palette
	   this->SetPalette();
   }else
       buffbits=bits;
	

   // pfd Tells Windows How We Want Things To Be
   static PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
      1,                             // Version Number
      PFD_DRAW_TO_WINDOW |           // Format Must Support Window
      PFD_SUPPORT_OPENGL |           // Format Must Support OpenGL
      PFD_DOUBLEBUFFER,              // Must Support Double Buffering
      PFD_TYPE_RGBA,                 // Request An RGBA Format
      bits,                          // Select Our Color Depth
      0, 0, 0, 0, 0, 0,              // Color Bits Ignored
      0,                             // No Alpha Buffer
      0,                             // Shift Bit Ignored
      0,                             // No Accumulation Buffer
      0, 0, 0, 0,                    // Accumulation Bits Ignored
      buffbits,                      // bitdepth Z-Buffer (Depth Buffer)
	  buffbits,                      // bitdepth Stencil Buffer
      0,                             // No Auxiliary Buffer
      PFD_MAIN_PLANE,                // Main Drawing Layer
      0,                             // Reserved
      0, 0, 0                        // Layer Masks Ignored
	};


   // store present stencil buffer depth
   stencil=bits;

   // Did Windows Find A Matching Pixel Format?
   if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		// try with a smaller stencil buffer
		stencil=1;
		pfd.cStencilBits=stencil;
		if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
			// try with no stencil buffer
			stencil=0;
			pfd.cStencilBits=stencil;
			if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
				KillGLWindow(); // Reset The Display
				MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

				return false; // Return false
			}
		}
   }

   // Are We Able To Set The Pixel Format?
   if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {
      KillGLWindow(); // Reset The Display
      MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

      return false; // Return false
   }

   // Are We Able To Get A Rendering Context?
   if (!(hRC=wglCreateContext(hDC))) {
      KillGLWindow(); // Reset The Display
      MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

      return false; // Return false
   }

   // Try To Activate The Rendering Context
   if(!wglMakeCurrent(hDC, hRC)) {
      KillGLWindow(); // Reset The Display
      MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

      return false; // Return false
   }


   //
   //	Create State Manager
   //

   // allocate the manager
   try {
   ste=new StateManager;
   }
   // did that work?
   catch(std :: bad_alloc) {
	   KillGLWindow();			// Reset the display
       MessageBox(NULL, "Cannot create State Manager.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

       return false; // Return false
   }

   //
   //	Initialize OpenGL
   //

   SetFocus(hWnd);               // Sets Keyboard Focus To The Window
   ReSizeGLScene(width, height); // Set Up Our Perspective GL Screen

   // Initialize Our Newly Created GL Window
   if (!InitGL()) {
      KillGLWindow(); // Reset The Display
      MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

      return false; // Return false
   }


   //
   //	Allocate the Frame Timer
   //

   // allocate the frame
   try {
   fme=new Frame;
   }
   // did that work?
   catch(std :: bad_alloc)  {
	   KillGLWindow();			// Reset the display
       MessageBox(NULL, "Cannot create Frame Timer.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

       return false; // Return false
   }

   return true; // Success
}

// Properly Kill The Window
GLvoid OpenGL :: KillGLWindow() 
{

	// do we have a window?
	if(!win)
		return;

   // Are We In Fullscreen Mode?
   if (FullScreen) {
      ChangeDisplaySettings(NULL, 0); // If So Switch Back To The Desktop
      ShowCursor(true);              // Show Mouse Pointer
   }

   // Do We Have A Rendering Context?
   if (hRC) {
      // Are We Able To Release The DC And RC Contexts?
      if (!wglMakeCurrent(NULL, NULL)) {
         MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
      }

      // Are We Able To Delete The RC?
      if (!wglDeleteContext(hRC)) {
         MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
      }

      hRC = NULL; // Set RC To NULL
   }

   // close window
   win->Close();
}

};	// namespace NeHe