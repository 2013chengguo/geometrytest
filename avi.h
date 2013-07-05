/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		AVI class	

	Version History:
		v1.00	New class				(GA 04/12/02)

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

#ifndef AVI_H
#define AVI_H

#include "opengl.h"
#include "texture.h"
#include <vfw.h>
#include <string>

#pragma comment(lib,"vfw32.lib")

namespace NeHe {

class AVI {
	AVISTREAMINFO		psi;										// Pointer To A Structure Containing Stream Info
	PAVISTREAM			pavi;										// Handle To An Open Stream
	PGETFRAME			pgf;										// Pointer To A GetFrame Object
	BITMAPINFOHEADER	bmih;										// Header Information For DrawDibDraw Decoding
	long				lastframe;									// Last Frame Of The Stream
	int					width;										// Video Width
	int					height;										// Video Height
	BYTE				*pdata;										// Pointer To Texture Data
	int					mpf;										// Will Hold Rough Milliseconds Per Frame
	float				pos;										// Position in milliseconds
	bool				loop;										// whether the video is to loop
	HDRAWDIB			hdd;										// Handle For Our Dib
	HBITMAP				hBitmap;									// Handle To A Device Dependant Bitmap
	HDC					hdc;										// Compatible Device Context
	unsigned char		*data;										// Pointer To Our Resized Image
	Texture				*tman;										// texture manager
	unsigned			texid;										// our texture id
public:
	AVI();
	~AVI();
	// Load AVI file
	//	Params:
	//		fname:		name of file
	//		loopflag:	whether you want the video to repeat
	//		tex:		texture manager
	bool Load(std :: string fname,bool loopflag,Texture *tex);
	// Set video frame to current texture
	//	time: length of last frame in milliseconds
	void Set(float time);
	// close the video
	void Close();
	// reset position in video
	void Reset()	{ pos=0.0f; };
};

};	// namespace NeHe

#endif