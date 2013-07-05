/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Image Class	

	Version History:
		v1.00			New class
		v1.01			Fixed InImage, so it takes into account y value (GA 13/01/03)

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

#ifndef IMAGE_H
#define IMAGE_H

#define STRICT
#include <windows.h>
#include "colour.h"

namespace NeHe {

class Image {
	BYTE	*img;			// image data
	int		width,height;	// dimensions of the image
	int		colsize;		// size of the colour in bytes
	bool	alpha;			// whether the image has alpha
	// Helper functions
	bool	InImage(int x,int y) const { return ( (x>=0 && x<width) && (y>=0 && y<height) ); };
	int		CalcPos(int x,int y) const { return (((y*width)+x)*colsize); };
	bool	LoadTGA(char *fname);	// load a TGA image
	bool	LoadPCX(char *fname);	// load a PCX image
	bool	LoadOther(char *fname);	// load an image using IPicture
public:
	Image();
	~Image();

	// Load Image
	//	Params:
	//		fname: file name of the image
	//	Returns true on success
	bool Load(char *fname);

	// Create a new image
	//	Params:
	//		wid:	width of the image to create
	//		hit:	height of the image to create
	//		alp:	whether the image to create should have an alpha component
	//	Returns true on success
	bool Create(int wid,int hit,bool alp);

	//
	//	Reading/Writing pixel functions
	//

	// Set a Pixel
	//	Params:
	//		x,y:	position to draw
	//		col:	colour to set the pixel
	void SetPixel(int x,int y,Colour col);
	void SetPixel(int x,int y,ColourAlpha col);

	// Get a Pixel
	//	Params:
	//		x,y:	position to draw
	Colour		GetPixel(int x,int y);
	ColourAlpha	GetPixelAlpha(int x,int y);

	//
	//	Data Access
	//

	// Returns whether the image has an alpha component
	bool HasAlpha()	const	{ return alpha; };
	// Returns the dimensions of the image
	int	GetWidth()	const	{ return width; };
	int GetHeight() const	{ return height; };
	// Get the dimensions of the image
	void GetDimensions(int &wid,int &hit) const { wid=width; hit=height; };
	// Returns the image data
	BYTE *GetImage() const	{ return img; };
	// Set the image data
	void SetImage(BYTE *data) { CopyMemory(img,data,(width*height*colsize)); };
};

}; // namespace NeHe

#endif