/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Text classes

	Version History:
		v1.00		New classes, based on lessons 13-14 (GA 31/10/02)
		v1.01		Tidied up base class

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

#ifndef TEXT_H
#define TEXT_H

#include "opengl.h"
#include <string>
#include <cstdlib>
#include <cstdio>

namespace NeHe {

// the text type
typedef struct TextType {
						std :: string	name;		// name of the font
						int				size;		// size of font
						bool			bold;		// whether the texture is bold
						bool			italic;		// whether the font should be italic
						bool			underline;	// whether the font should be underlined
						float			depth;		// z-depth (used in Text3D)
					}TextType;

class Text {
protected:
	unsigned int base;	// base of font lists
public:
	Text()			{ base=0U; };
	virtual ~Text()	{ this->Clear(); };

	// Setup the font
	virtual void Setup(OpenGL *gl,TextType type) =0;

	// Write out a string
	virtual void Write(std :: string str) =0;

	// Stream output
	Text &operator << (int val)	
				{ char str[10]; _itoa(val,str,10); this->Write(str); return *this; };
	Text &operator << (unsigned val)
				{ char str[10]; _itoa(val,str,10); this->Write(str); return *this; };
	Text &operator << (float val) 
				{ char str[20]; sprintf(str,"%f",val); this->Write(str); return *this; };
	Text &operator << (char val)
				{ char str[2]; sprintf(str,"%c",val); this->Write(str); return *this; };
	Text &operator << (std :: string str)	{ this->Write(str); return *this; };

	// Clear the font
	void Clear();
};

class Text2D : public Text {
public:
	// setup the font
	virtual void Setup(OpenGL *gl,TextType type);
	// write out a string
	virtual void Write(std :: string str);
};

class Text3D : public Text {
	float avrwidth;				// the average width of the characters
public:
	// setup the font
	virtual void Setup(OpenGL *gl,TextType type);
	// write out a string
	virtual void Write(std :: string str);
	// get the average width of the font characters
	float GetAverageWidth() { return avrwidth; };
};

}; // namespace NeHe

#endif