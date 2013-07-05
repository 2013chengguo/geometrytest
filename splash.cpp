/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Splash Screen Handling class	

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

#include "win32.h"
#include "button.h"
#include "splash.h"
#include <stdio.h>

namespace NeHe {

// dimensions
static int	swidth,sheight;

// defines for arrays
#define S640		0
#define S800		1
#define S1024		2
#define S8			3
#define S16			4
#define S32			5
#define SFULL		6
#define SGO			7
// value array
static BOOL values[8];
// button array
static Button buttons[8];

void DrawSplash(HWND hwnd,HBITMAP up,HBITMAP down)
{
	HDC		dc;				// window device context
	HDC		bmpdc1,bmpdc2;	// bitmap device contexts
	HBITMAP bmp1,bmp2;		// bitmaps on selection
	int		ct;				// counter
	int		x,y,wid,hit;	// position and dimensions


	// get device context
	dc=GetDC(hwnd);

	// get bitmap contexts
	bmpdc1=CreateCompatibleDC(NULL);
	bmpdc2=CreateCompatibleDC(NULL);

	// select bitmaps in DCs
	bmp1=(HBITMAP)SelectObject(bmpdc1,up);
	bmp2=(HBITMAP)SelectObject(bmpdc2,down);

	// draw background bitmap
	BitBlt(dc,0,0,swidth,sheight,bmpdc1,0,0,SRCCOPY);

	//
	//	Now draw the buttons
	//

	// run through the buttons
	for(ct=0; ct<8; ct++)
	{
		// get the button info
		buttons[ct].Get(&x,&y,&wid,&hit);
		// draw the button down, if necessary
		if(values[ct])
		{
			BitBlt(dc,x,y,wid,hit,bmpdc2,x,y,SRCCOPY);
		}
	}

	// tidy up
	DeleteDC(bmpdc1);
	DeleteDC(bmpdc2);
}

void SplashTestButtons(int x,int y)
{
	// test resolution radio buttons
	if(buttons[S640].Pressed(x,y))
	{
		values[S640]=TRUE;
		values[S800]=values[S1024]=FALSE;
	}
	if(buttons[S800].Pressed(x,y))
	{
		values[S800]=TRUE;
		values[S640]=values[S1024]=FALSE;
	}
	if(buttons[S1024].Pressed(x,y))
	{
		values[S1024]=TRUE;
		values[S800]=values[S640]=FALSE;
	}

	// test bit depth radio buttons
	if(buttons[S8].Pressed(x,y))
	{
		values[S8]=TRUE;
		values[S16]=values[S32]=FALSE;
	}
	if(buttons[S16].Pressed(x,y))
	{
		values[S16]=TRUE;
		values[S8]=values[S32]=FALSE;
	}
	if(buttons[S32].Pressed(x,y))
	{
		values[S32]=TRUE;
		values[S16]=values[S8]=FALSE;
	}

	// test fullscreen toggle
	if(buttons[SFULL].Pressed(x,y))
	{
		if(values[SFULL])
			values[SFULL]=FALSE;
		else
			values[SFULL]=TRUE;
	}

	// test go button
	if(buttons[SGO].Pressed(x,y))
		values[SGO]=TRUE;
}

bool DoSplash(char *fname,SplashResolution *res, SplashDepth *depth,bool *full)
{
	Window		win;			// our splash window
	int			px,py;			// position of splash screen
	HINSTANCE	hIns;			// the instance
	FILE		*fp;			// file pointer
	char		imgname1[64];	// name of splash bitmap
	char		imgname2[64];	// name od splash bitmap with buttons down
	int			ct;				// counter
	HBITMAP		up,down;		// bitmaps
	BITMAP		bmp;			// bitmap struct
	HWND		desk;			// desktop handle
	RECT		rect;			// rectangle
	int			dwid,dhit;		// desktop dimensions
	bool		left,right;		// mouse buttons
	int			mx,my;			// mouse position

	// open the configuration file
	fp=fopen(fname,"r");

	// did that work?
	if(!fp)
		return false;

	// load the filenames of the splash bitmaps
	if(!fscanf(fp,"%s",imgname1))
	{
		fclose(fp);
		return false;
	}
	if(!fscanf(fp,"%s",imgname2))
	{
		fclose(fp);
		return false;
	}

	// load up the button information
	for(ct=0; ct<8; ct++)
		if(!buttons[ct].Load(fp))
		{
			fclose(fp);
			return false;
		}

	// close the configuration file
	fclose(fp);

	// get the instance
	hIns=GetModuleHandle(NULL);

	// load the bitmaps
	up=(HBITMAP)LoadImage(hIns,imgname1,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	down=(HBITMAP)LoadImage(hIns,imgname2,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	// did that work?
	if(!up || !down)
		return false;

	// get bitmap information from image one
	GetObject(up,sizeof(BITMAP),&bmp);
	// get bitmap dimensions
	swidth=bmp.bmWidth;
	sheight=bmp.bmHeight;

	// get the desktop handle
	desk=GetDesktopWindow();
	// get the size
	GetClientRect(desk,&rect);
	// get it into width and height
	dwid=rect.right-rect.left;
	dhit=rect.bottom-rect.top;

	// calculate the position of the splash screen
	px=(dwid/2)-(swidth/2);
	py=(dhit/2)-(sheight/2);

	// create the window
	if(!win.Setup(px,py,swidth,sheight,"Splash"))
		return false;

	// setup flags
	ZeroMemory(&values,sizeof(BOOL)*8);
	values[S640]=values[S16]=TRUE;

	// draw the splash window
	DrawSplash(win.GetHandle(),up,down);

	// Update it
	UpdateWindow(win.GetHandle());

	// go into input loop
	while(!values[SGO])
	{
		// is a button pressed?
		win.GetMouseButton(left,right);
		// yes
		if(left)
		{
			// get mouse position
			win.GetMousePosition(mx,my);
			// send it to the button test
			SplashTestButtons(mx,my);

			// draw the splash window
			DrawSplash(win.GetHandle(),up,down);

			// update the display
			UpdateWindow(win.GetHandle());

			// wait for mouse button to go up
			while(left)
				win.GetMouseButton(left,right);
		}
	}

	//
	// return the information
	//

	// first resolution
	if(values[S640])
		*res=sr640x480;
	else if(values[S800])
		*res=sr800x600;
	else
		*res=sr1024x768;

	// next bit depth
	if(values[S8])
		*depth=sd8bit;
	else if(values[S16])
		*depth=sd16bit;
	else
		*depth=sd32bit;

	// finally fullscreen
	if(values[SFULL])
		*full=true;
	else
		*full=false;

	// success
	return true;
}

}; // namespace NeHe