/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		TGA Loading functions

	Version History:
		v1.00  Converted from lesson 33 by Evan "terminate" Pipho and Jeff Molofee (GA 30/10/02) 
		v1.01  Fixed compressed tga loading problem (GA 13/01/03)

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

#include "tga.h"
#include <cstdio>												// Standard I/O header 

namespace NeHe {

// The distinguishing header
typedef struct TGAHead {
	BYTE Header[12];											// TGA File Header
} TGAHead;

// the TGA information struct
typedef struct TGA {
	BYTE				header[6];								// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;									// Temporary Variable
	unsigned int		type;	
	unsigned int		Height;									// Height of Image
	unsigned int		Width;									// Width ofImage
	unsigned int		Bpp;									// Bits Per Pixel
} TGA;


static TGAHead head;										// TGA header
static TGA tga;												// TGA image data



BYTE uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
BYTE cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

// Forward definitions
bool LoadUncompressedTGA(FILE *fTGA,int &width,int &height,bool &alpha,BYTE **bits);
bool LoadCompressedTGA(FILE *fTGA,int &width,int &height,bool &alpha,BYTE **bits);



bool LoadTGA(char *filename,int &width,int &height,bool &alpha,BYTE **bits)				// Load a TGA file
{
	FILE * fTGA;												// File pointer to texture file
	fTGA = fopen(filename, "rb");								// Open file for reading

	if(fTGA == NULL)											// If it didn't open....
	{
		MessageBox(NULL, "Could not open texture file", "ERROR", MB_OK);	// Display an error message
		return false;														// Exit function
	}

	if(fread(&head, sizeof(TGAHead), 1, fTGA) == 0)							// Attempt to read 12 byte header from file
	{
		MessageBox(NULL, "Could not read file header", "ERROR", MB_OK);		// If it fails, display an error message 
		if(fTGA != NULL)													// Check to seeiffile is still open
		{
			fclose(fTGA);													// If it is, close it
		}
		return false;														// Exit function
	}

	if(memcmp(uTGAcompare, &head, sizeof(TGAHead)) == 0)					// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		LoadUncompressedTGA(fTGA,width,height,alpha,bits);		// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGAcompare, &head, sizeof(TGAHead)) == 0)				// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		LoadCompressedTGA(fTGA,width,height,alpha,bits);			// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
		MessageBox(NULL, "TGA file be type 2 or type 10 ", "Invalid Image", MB_OK);	// Display an error
		fclose(fTGA);
		return false;																// Exit function
	}
	return true;															// All went well, continue on
}


// Load an uncompressed TGA (note, much of this code is based on NeHe's 
// TGA Loading code nehe.gamedev.net)
bool LoadUncompressedTGA(FILE *fTGA,int &width,int &height,bool &alpha,BYTE **bits)	
{
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{										
		MessageBox(NULL, "Could not read info header", "ERROR", MB_OK);		// Display error
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}	

	width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	alpha	= (tga.header[4]==32);									// Determine the alpha component
	tga.Width		= width;										// Copy width into local structure						
	tga.Height		= height;										// Copy height into local structure
	tga.Bpp			= tga.header[4];								// Copy BPP into local structure

	if((width <= 0) || (height <= 0) || ((tga.Bpp != 24) && (tga.Bpp !=32)))	// Make sure all information is valid
	{
		MessageBox(NULL, "Invalid texture information", "ERROR", MB_OK);	// Display Error
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}


	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	*bits	= new BYTE [tga.imageSize];										// Allocate that much memory

	if(*bits == NULL)											// If no space was allocated
	{
		MessageBox(NULL, "Could not allocate memory for image", "ERROR", MB_OK);	// Display Error
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(*bits, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
		MessageBox(NULL, "Could not read image data", "ERROR", MB_OK);		// Display Error
		delete [] *bits;													// Delete data from memory
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	for(int cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		*bits[cswap] ^= *bits[cswap+2] ^= *bits[cswap] ^= *bits[cswap+2];
	}

	fclose(fTGA);															// Close file
	return true;															// Return success
}

// Load a compressed TGA
bool LoadCompressedTGA(FILE *fTGA,int &width,int &height,bool &alpha,BYTE **bits)		
{ 
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Attempt to read header
	{
		MessageBox(NULL, "Could not read info header", "ERROR", MB_OK);		// Display Error
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failed
	}

	width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	alpha	= (tga.header[4]==32);									// Determine the alpha component
	tga.Width		= width;										// Copy width into local structure						
	tga.Height		= height;										// Copy height into local structure
	tga.Bpp			= tga.header[4];								// Copy BPP into local structure

	if((width <= 0) || (height <= 0) || ((tga.Bpp != 24) && (tga.Bpp !=32)))	// Make sure all information is valid
	{
		MessageBox(NULL, "Invalid texture information", "ERROR", MB_OK);	// If it isnt...Display error
		if(fTGA != NULL)													// Check if file is open
		{
			fclose(fTGA);													// Ifit is, close it
		}
		return false;														// Return failed
	}

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	*bits	= new BYTE [tga.imageSize];										// Allocate that much memory

	if(*bits == NULL)											// If it wasnt allocated correctly..
	{
		MessageBox(NULL, "Could not allocate memory for image", "ERROR", MB_OK);	// Display Error
		fclose(fTGA);																// Close file
		return false;																// Return failed
	}

	unsigned int pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	unsigned int currentpixel	= 0;												// Current pixel being read
	unsigned int currentbyte	= 0;												// Current byte 
	BYTE * colorbuffer = new BYTE [tga.bytesPerPixel];			// Storage for 1 pixel

	do
	{
		BYTE chunkheader = 0;											// Storage for "chunk" header

		if(fread(&chunkheader, sizeof(BYTE), 1, fTGA) == 0)				// Read in the 1 byte header
		{
			MessageBox(NULL, "Could not read RLE header", "ERROR", MB_OK);	// Display Error
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(*bits != NULL)									// If there is stored image data
			{
				delete [] *bits;											// Delete image data
			}
			return false;													// Return failed
		}

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
				{
					MessageBox(NULL, "Could not read image data", "ERROR", MB_OK);		// IF we cant, display an error

					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						delete [] colorbuffer;											// If so, delete it
					}

					if(*bits != NULL)													// See if there is stored Image data
					{
						delete [] *bits;												// If so, delete it too
					}

					return false;														// Return failed
				}
																						// write to memory
				*bits[currentbyte		] = colorbuffer[2];								// Flip R and B vcolor values around in the process 
				*bits[currentbyte + 1	] = colorbuffer[1];
				*bits[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					*bits[currentbyte + 3] = colorbuffer[3];							// copy the 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
					MessageBox(NULL, "Too many pixels read", "ERROR", 0);			    // if there is too many... Display an error!

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						delete [] colorbuffer;											// Delete it
					}

					if(*bits != NULL)													// If there is Image data
					{
						delete [] *bits;												// delete it
					}

					return false;														// Return failed
				}
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
			{	
				MessageBox(NULL, "Could not read from file", "ERROR", MB_OK);			// If attempt fails.. Display error (again)

				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					delete [] colorbuffer;												// delete it
				}

				if(*bits != NULL)														// If there is image data
				{
					delete [] *bits;													// delete it
				}

				return false;															// return failed
			}

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				*bits[currentbyte		] = colorbuffer[2];								// switch R and B bytes areound while copying
				*bits[currentbyte + 1	] = colorbuffer[1];
				*bits[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					*bits[currentbyte + 3] = colorbuffer[3];							// Copy 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
					MessageBox(NULL, "Too many pixels read", "ERROR", 0);         		// if there is too many... Display an error!

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						delete [] colorbuffer;											// Delete it
					}

					if(*bits != NULL)													// If there is Image data
					{
						delete [] *bits;												// delete it
					}

					return false;														// Return failed
				}
			}
		}
	}

	while(currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(fTGA);																		// Close the file
	return true;																		// return success
}

}; // namespace NeHe