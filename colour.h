/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Colour defines	

	Version History:
		v1.00			New file	(GA 29/10/02)
		v1.01			Added copy constructors for STL correctness
									(GA 10/04/03)


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

#ifndef COLOUR_H
#define COLOUR_H

namespace NeHe {

// colour structure
typedef struct Colour {
					float red;
					float green;
					float blue;
					Colour() { red=green=blue=0.0f; };
					Colour(float r,float g,float b)
						{ red=r; green=g; blue=b; };
					Colour(const Colour &c)
						{ red=c.red; green=c.green; blue=c.blue; };
					Colour &operator =(const Colour &c)
						{ red=c.red; green=c.green; blue=c.blue; return *this; };
			}Colour;

// colour structure with alpha
typedef struct ColourAlpha {
					float red;
					float green;
					float blue;
					float alpha;
					ColourAlpha() { red=green=blue=0.0f; alpha=1.0f; };
					ColourAlpha(float r,float g,float b)
						{ red=r; green=g; blue=b; alpha=1.0f; };
					ColourAlpha(float r,float g,float b,float a)
						{ red=r; green=g; blue=b; alpha=a; };
					ColourAlpha(const ColourAlpha &c)
						{ red=c.red; green=c.green; blue=c.blue; alpha=c.alpha; };
					ColourAlpha(const Colour &c)
						{ red=c.red; green=c.green; blue=c.blue; alpha=1.0f; };
					ColourAlpha &operator =(const ColourAlpha &c)
						{ 
							red=c.red; green=c.green; 
							blue=c.blue; alpha=c.alpha; 
							return *this; 
						};
			}ColourAlpha;

// a define for Americans/Canadians
#define Color	Colour

// defines for converting a byte to a float (0.0-1.0) and vice versa
#define ByteToFloat(val)	( ((float)(val))/256.0f )
#define FloatToByte(val)	( (unsigned char)((val)*255.0f) )

// basic colours
#define BLACK		Colour(0.0f,0.0f,0.0f)
#define BLACKALPHA	ColourAlpha(0.0f,0.0f,0.0f)
#define WHITE		Colour(1.0f,1.0f,1.0f)
#define WHITEALPHA	ColourAlpha(1.0f,1.0f,1.0f)

}; // namespace NeHe

// simple proxy
#define NCOLOUR		NeHe :: Colour

#endif
