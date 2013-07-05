/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		AVI class	

	Version History:
		v1.00	New class						(GA 04/12/02)
		v1.01	Tidied up frame significance	(GA 13/01/03)

	Notes: 
		Based on lesson 35 by Jeff

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

#include "avi.h"
#include "nexception.h"

namespace NeHe {

void flipIt(void* buffer)										// Flips The Red And Blue Bytes (256x256)
{
	void* b = buffer;											// Pointer To The Buffer
	__asm														// Assembler Code To Follow
	{
		mov ecx, 256*256										// Counter Set To Dimensions Of Our Memory Block
		mov ebx, b												// Points ebx To Our Data (b)
		label:													// Label Used For Looping
			mov al,[ebx+0]										// Loads Value At ebx Into al
			mov ah,[ebx+2]										// Loads Value At ebx+2 Into ah
			mov [ebx+2],al										// Stores Value In al At ebx+2
			mov [ebx+0],ah										// Stores Value In ah At ebx
			
			add ebx,3											// Moves Through The Data By 3 Bytes
			dec ecx												// Decreases Our Loop Counter
			jnz label											// If Not Zero Jump Back To Label
	}
}

AVI :: AVI()
{
	// clear everything else
	pos=0.0f;
	width=height=0;
	mpf=0;
	loop=false;
	pdata=NULL;
	data=NULL;
	tman=NULL;
}

AVI :: ~AVI()
{
	// just use close
	this->Close();
}

bool AVI :: Load(std :: string fname,bool loopflag,Texture *tex)				// Opens An AVI File
{
	// copy over looping flag
	loop=loopflag;

	// do we have a texture manager?
	if(!tex)
		return false;
	// copy it over
	tman=tex;

	hdc=CreateCompatibleDC(0);									// Get a compatible DC
	hdd = DrawDibOpen();										// Grab A Device Context For Our Dib


	AVIFileInit();												// Opens The AVIFile Library

	// Opens The AVI Stream
	if (AVIStreamOpenFromFile(&pavi, fname.c_str(), streamtypeVIDEO, 0, OF_READ, NULL) !=0)
		return false;

	AVIStreamInfo(pavi, &psi, sizeof(psi));						// Reads Information About The Stream Into psi
	width=psi.rcFrame.right-psi.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	height=psi.rcFrame.bottom-psi.rcFrame.top;					// Height Is Bottom Of Frame Minus Top

	lastframe=AVIStreamLength(pavi);							// The Last Frame Of The Stream

	mpf=AVIStreamSampleToTime(pavi,lastframe)/lastframe;		// Calculate Rough Milliseconds Per Frame

	bmih.biSize = sizeof (BITMAPINFOHEADER);					// Size Of The BitmapInfoHeader
	bmih.biPlanes = 1;											// Bitplanes	
	bmih.biBitCount = 24;										// Bits Format We Want (24 Bit, 3 Bytes)
	bmih.biWidth = 256;											// Width We Want (256 Pixels)
	bmih.biHeight = 256;										// Height We Want (256 Pixels)
	bmih.biCompression = BI_RGB;								// Requested Mode = RGB

	hBitmap = CreateDIBSection (hdc, (BITMAPINFO*)(&bmih), DIB_RGB_COLORS, (void**)(&data), NULL, NULL);
	SelectObject (hdc, hBitmap);								// Select hBitmap Into Our Device Context (hdc)

	pgf=AVIStreamGetFrameOpen(pavi, NULL);						// Create The PGETFRAME	Using Our Request Mode
	if (pgf==NULL)
		return false;

	// create the texture
	Image img;
	img.Create(256,256,false);									// set dimensions of texture
	img.SetImage(data);											// set the texture information
	texid=tman->Create(&img);									// create the texture

	// clear the time position 
	pos=0;

	// success
	return true;
}

void AVI :: Set(float time)									// Grabs A Frame From The Stream
{
	// anything to do?
	if(!tman || mpf==0)
		NTHROW("AVI :: Set - Video not created before call to set.");

	// move position forward by time
	pos+=time;
	// get the frame number
	int frame=(int)((float)pos/mpf);

	// have we gone past the end?
	if(frame>=lastframe)
	{
		if(loop)
		{
			pos=0.0f;
			frame=0;				// reset the animation
		}else
			frame=lastframe-1;		// hold at the last frame
	}

	LPBITMAPINFOHEADER lpbi;											// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);			// Grab Data From The AVI Stream
	pdata=(BYTE *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);	// Pointer To Data Returned By AVIStreamGetFrame

	// Convert Data To Requested Bitmap Format
	DrawDibDraw (hdd, hdc, 0, 0, 256, 256, lpbi, pdata, 0, 0, width, height, 0);

	flipIt(data);												// Swap The Red And Blue Bytes (GL Compatability)

	// set our texture
	tman->Set(texid);

	// Update The Texture
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void AVI :: Close(void)												// Properly Closes The Avi File
{
	if(mpf)
	{
		DeleteObject(hBitmap);										// Delete The Device Dependant Bitmap Object
		DrawDibClose(hdd);											// Closes The DrawDib Device Context
		AVIStreamGetFrameClose(pgf);								// Deallocates The GetFrame Resources
		AVIStreamRelease(pavi);										// Release The Stream
		AVIFileExit();												// Release The File
	}

	// clear everything else
	pos=0;
	width=height=0;
	mpf=0;
	pdata=NULL;
	loop=false;
}

};	// namespace NeHe

