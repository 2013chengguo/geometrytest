/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		PCX loader

	Version History:
		v1.00	New file		(GA 29/11/02)

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

#define STRICT
#include <windows.h>
#include <cstdio>
#include <new>
#include "pcx.h"

namespace NeHe {

#include <pshpack1.h>

typedef struct PCXHeader {
					char manufacturer;		//	must be 10 (ZSoft)
					char version;			//	these days almost always five
					char encoding;			//	1 = RLE
					char bpp;				//	bits per pixel (per plane)
					WORD xmin,ymin;			//	top left
					WORD xmax,ymax;			//	bottom right
					WORD hDPI,vDPI;			//  dots per inch in each axis
					char colormap[48];		//	old fashioned palette
					char reserved;			//	should be 0
					char planes;			//	number of planes
					WORD bytesperline;		//	bytes to allocate per line - rounded up to an even number
					WORD paletteinfo;		//	1 - colour/B\W, 2 - grayscale
					WORD hScreen,vScreen;	//	screen dimensions
					char filler[54];		//	fill out header to 128 bytes
				}PCXHeader;

#include <poppack.h>

void SetPixel(int index,BYTE *palette,BYTE *pos)
{
	int indexpos=index*3;	// calculate the index position

	// set the colour
	pos[0]=palette[indexpos++];
	pos[1]=palette[indexpos++];
	pos[2]=palette[indexpos];
}

#define CLOSEFAIL(fp)	{ fclose(fp); return false; }

bool LoadPCX(char *filename,int &width,int &height,BYTE **bits)
{
	// Open the PCX file
	FILE *fp=fopen(filename,"rb");

	// does it exist?
	if(!fp)
		return false;

	// get the palette
	BYTE palette[768];
	// go to the end
	fseek(fp,-769,SEEK_END);
	// check it's a palette
	int val=fgetc(fp);
	if(val!=12)
		CLOSEFAIL(fp);
	// and read in the palette
	if(!fread(palette,1,768,fp))
		CLOSEFAIL(fp);

	// return to the beginning
	fseek(fp,0,SEEK_SET);

	// load in the header
	PCXHeader head;
	if(!fread(&head,sizeof(PCXHeader),1,fp))
		CLOSEFAIL(fp);

	// check its a PCX file
	if(head.manufacturer!=10 || head.version!=5 || head.encoding!=1)
		CLOSEFAIL(fp);

	// copy over width and height
	width=head.xmax-head.xmin+1;
	height=head.ymax-head.ymin+1;

	// allocate image data
	try {
		*bits=new BYTE [width*height*3];
	}
	catch(std :: bad_alloc) {
		CLOSEFAIL(fp);
	}

	// place in image
	int pos=0;
	// image size
	int size=width*height*3;

	// run through the image
	while(pos<size)
	{
		// get the character
		val=fgetc(fp);

		// is it a status byte?
		if(val>=0xc0)
		{
			// get the repeat length
			int replen=0x3f&val;
			// get the next character
			val=fgetc(fp);

			// repeat it the number of times
			for(int ct=0; ct<replen; ct++)
			{
				// write the pixel to the image
				SetPixel(val,palette,(*bits)+pos);
				// move forward
				pos+=3;
			}
		}else{
			// write the pixel to the image
			SetPixel(val,palette,(*bits)+pos);
			// move forward
			pos+=3;
		}
	}
	

	// close the file
	fclose(fp);

	// success
	return true;
}

}; // namespace NeHe