/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Image Class	

	Version History:
		v1.00			New class	(GA 29/10/02)
		v1.01			Converted TGA Loader to Lesson 33 Version (GA 30/10/02)

	Notes:
		An amalgamation of the IPicture code and the TGA code

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

#include "image.h"
#include <cstdio>		// For C file functions
#include <cstring>		// For C string functions
#include <cmath>		// For C maths functions
#include <cctype>
#include <new>
#include <olectl.h>		// Header File For The OLE Controls Library	
#include <GL/gl.h>		// For GL caps
#include "tga.h"		// for TGA Loader
#include "pcx.h"		// for pcx loader
#include "nexception.h"	// Our exception

namespace NeHe {

Image :: Image()
{
	// clear everything
	width=height=colsize=0;
	alpha=false;
	img=NULL;
}

Image :: ~Image()
{
	// delete the image, if necessary
	if(img)
	{
		delete [] img;
		img=NULL;
	}
}

bool Image :: LoadTGA(char *fname)
{
	// Just use the TGA loader
	bool ret=NeHe::LoadTGA(fname,width,height,alpha,&img);											
	// set the colour size
	colsize=alpha ? 4: 3;
	// and return
	return ret;
}

bool Image :: LoadPCX(char *fname)
{
	// no alpha
	alpha=false;
	colsize=3;

	// Just use the PCX loader
	return NeHe::LoadPCX(fname,width,height,&img);											
}


bool Image :: LoadOther(char *fname)
{
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	OLECHAR		wszPath[MAX_PATH+1];									// Full Path To Picture (WCHAR)
	char		szPath[MAX_PATH+1];										// Full Path To Picture
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	GLint		glMaxTexDim ;											// Holds Maximum Texture Size

	GetCurrentDirectory(MAX_PATH, szPath);						// Get Our Working Directory
	strcat(szPath, "\\");										// Append "\" After The Working Directory
	strcat(szPath, fname);										// Append The PathName

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// Convert From ASCII To Unicode
	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

	if(FAILED(hr))														// If Loading Failed
		return FALSE;													// Return False

	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
	if(!hdcTemp)														// Did Creation Fail?
	{
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported
	
	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
	width=MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
	height=MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize Image To Closest Power Of Two
	if (width <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		width = 1 << (int)floor((log((double)width)/log(2.0f)) + 0.5f); 
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		width = 1 << (int)floor((log((double)glMaxTexDim)/log(2.0f)) + 0.5f);
 
	if (height <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		height = 1 << (int)floor((log((double)height)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		height = 1 << (int)floor((log((double)glMaxTexDim)/log(2.0f)) + 0.5f);
	
	//	Create A Temporary Bitmap
	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= width;								// Power Of Two Width
	bi.bmiHeader.biHeight		= height;								// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
	
	if(!hbmpTemp)														// Did Creation Fail?
	{
		DeleteDC(hdcTemp);												// Delete The Device Context
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp, 0, 0, width, height, 0, lHeight, lWidth, -lHeight, 0);

	// set the alpha
	alpha=false;
	// and the bit count
	colsize=3;

	// allocate image
	try {
		img=new BYTE [width*height*colsize];
	}
	// did that work?
	catch(std :: bad_alloc)	{
		DeleteObject(hbmpTemp);					// Delete The Object
		DeleteDC(hdcTemp);						// Delete The Device Context
		pPicture->Release();					// Decrements IPicture Reference Count

		colsize=width=height=0;		// clear everything
		return false;
	}

	// our copy pointer
	BYTE *ptr=img;

	// Convert From BGR To RGB Format And Copy over
	for(long i = 0; i < width * height; i++)				// Loop Through All Of The Pixels
	{
		BYTE* pPixel=(BYTE*)(&pBits[i]);					// Grab The Current Pixel
		*(ptr++)=pPixel[2];									// copy red
		*(ptr++)=pPixel[1];									// copy green
		*(ptr++)=pPixel[0];									// copy blue
	}

	DeleteObject(hbmpTemp);									// Delete The Object
	DeleteDC(hdcTemp);										// Delete The Device Context

	pPicture->Release();									// Decrements IPicture Reference Count

	return TRUE;											// Return True (All Good)

}

bool Image :: Load(char *fname)
{
	// anything to do?
	if(!fname)
		return false;

	// do we need to tidy up?
	if(img)
	{
		delete []img;
		img=NULL;
	}

	// get the length of the file
	int length=strlen(fname);

	// is it a tga?
	if(toupper(fname[length-3])=='T' &&
		toupper(fname[length-2])=='G' &&
		toupper(fname[length-1])=='A' )
			return this->LoadTGA(fname);	// yes, it's a TGA
	else if(toupper(fname[length-3])=='P' &&
		toupper(fname[length-2])=='C' &&
		toupper(fname[length-1])=='X' )
			return this->LoadPCX(fname);	// no, it's a PCX
	else
			return this->LoadOther(fname);	// no, use IPicture
}

bool Image :: Create(int wid,int hit,bool alp)
{
	// do we need to tidy up?
	if(img)
	{
		delete img;
		img=NULL;
	}

	// calculate colour size
	colsize=alp ? 4 : 3;

	// allocate image
	try {
		img=new BYTE [wid*hit*colsize];
	}
	// did that work?
	catch(std :: bad_alloc)	{
		colsize=0;
		return false;
	}

	// copy over info
	width=wid;
	height=hit;
	alpha=alp;

	// success
	return true;
}

void Image :: SetPixel(int x,int y,Colour col)
{
	// do we have the image?
	if(!img)
		NTHROW("Image :: SetPixel - No image set up.");

	// is it in the image?
	if(!this->InImage(x,y))
		NTHROW("Image :: SetPixel - Coordinates out of range.");

	// get the position
	BYTE *ptr=img+this->CalcPos(x,y);

	// set the parts of the colour
	if(alpha)
	{
		*(ptr++)=FloatToByte(col.red);
		*(ptr++)=FloatToByte(col.green);
		*(ptr++)=FloatToByte(col.blue);
		*(ptr)=0xff;
	}else{
		*(ptr++)=FloatToByte(col.red);
		*(ptr++)=FloatToByte(col.green);
		*(ptr++)=FloatToByte(col.blue);
	}
}

void Image :: SetPixel(int x,int y,ColourAlpha col)
{
	// do we have the image?
	if(!img)
		NTHROW("Image :: SetPixel - No image set up.");

	// is it in the image?
	if(!this->InImage(x,y))
		NTHROW("Image :: SetPixel - Coordinates out of range.");

	// get the position
	BYTE *ptr=img+this->CalcPos(x,y);

	// set the parts of the colour
	if(alpha)
	{
		*(ptr++)=FloatToByte(col.red);
		*(ptr++)=FloatToByte(col.green);
		*(ptr++)=FloatToByte(col.blue);
		*(ptr)=FloatToByte(col.alpha);
	}else{
		*(ptr++)=FloatToByte(col.red);
		*(ptr++)=FloatToByte(col.green);
		*(ptr++)=FloatToByte(col.blue);
	}
}

Colour Image :: GetPixel(int x,int y)
{
	// do we have the image?
	if(!img)
		NTHROW("Image :: GetPixel - No image set up.");

	// is it in the image?
	if(!this->InImage(x,y))
		NTHROW("Image :: GetPixel - Coordinates out of range.");

	// get the position
	BYTE	*ptr=img+this->CalcPos(x,y);

	// return the colour
	return Colour(ByteToFloat(*(ptr++)),		// red component
					ByteToFloat(*(ptr++)),		// green component
					ByteToFloat(*(ptr))			// blue component
					);
}

ColourAlpha Image :: GetPixelAlpha(int x,int y)
{
	// do we have the image?
	if(!img)
		NTHROW("Image :: GetPixel - No image set up.");

	// is it in the image?
	if(!this->InImage(x,y))
		NTHROW("Image :: GetPixel - Coordinates out of range.");

	// get the position
	BYTE *ptr=img+this->CalcPos(x,y);

	// return the colour
	if(alpha)
		return ColourAlpha( ByteToFloat(*(ptr++)),			// red component
								ByteToFloat(*(ptr++)),		// green component
								ByteToFloat(*(ptr++)),		// blue component
								ByteToFloat(*(ptr))			// alpha component
								);
	else
		return ColourAlpha( ByteToFloat(*(ptr++)),			// red component
								ByteToFloat(*(ptr++)),		// green component
								ByteToFloat(*(ptr))			// blue component
								);
}

};	// namespace NeHe