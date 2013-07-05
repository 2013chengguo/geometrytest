/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Text classes

	Version History:
		v1.00		New classes, based on lessons 13-14 (GA 31/10/02)

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
#include <GL/gl.h>
#include "text.h"
#include "nexception.h"

using namespace std;

namespace NeHe {

//
//	2D Text Procedures
//

void Text :: Clear()
{
	// anything to do?
	if(base==0U)
		return;

	// delete the lists
	glDeleteLists(base,96);

	// and reset list base
	base=0U;
}

void Text2D :: Setup(OpenGL *gl,TextType type)
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	// anything to do?
	if(!gl)
		NTHROW("Text2D :: Setup - NULL pointer passed.");

	HDC		hDC=gl->GetDC();							// Get the device context

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-type.size,								// Height Of Font
						0,										// Width Of Font
						0,										// Angle Of Escapement
						0,										// Orientation Angle
						(type.bold) ? FW_BOLD: FW_NORMAL,		// Font Weight
						(type.italic)? TRUE : FALSE,			// Italic
						(type.underline)? TRUE : FALSE,			// Underline
						FALSE,									// Strikeout
						ANSI_CHARSET,							// Character Set Identifier
						OUT_TT_PRECIS,							// Output Precision
						CLIP_DEFAULT_PRECIS,					// Clipping Precision
						ANTIALIASED_QUALITY,					// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,				// Family And Pitch
						type.name.c_str());						// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void Text2D :: Write(string str)
{
	glPushAttrib(GL_LIST_BIT);								// Pushes The Display List Bits
	glListBase(base - 32);									// Sets The Base Character to 32
	glCallLists(str.length(),GL_UNSIGNED_BYTE,str.c_str());	// Draws The Display List Text
	glPopAttrib();											// Pops The Display List Bits
}

//
//	3D Text Procedures
//

void Text3D :: Setup(OpenGL *gl,TextType type)
{
	HFONT	font;				// Windows Font ID
	HFONT	oldfont;			// Used For Good House Keeping
	GLYPHMETRICSFLOAT gmf[96];	// Storage For Information About Our Outline Font Characters

	// anything to do?
	if(!gl)
		NTHROW("Text3D :: Setup - NULL pointer passed.");

	HDC		hDC=gl->GetDC();							// Get the device context

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-type.size,								// Height Of Font
						0,										// Width Of Font
						0,										// Angle Of Escapement
						0,										// Orientation Angle
						(type.bold) ? FW_BOLD: FW_NORMAL,		// Font Weight
						(type.italic)? TRUE : FALSE,			// Italic
						(type.underline)? TRUE : FALSE,			// Underline
						FALSE,									// Strikeout
						ANSI_CHARSET,							// Character Set Identifier
						OUT_TT_PRECIS,							// Output Precision
						CLIP_DEFAULT_PRECIS,					// Clipping Precision
						ANTIALIASED_QUALITY,					// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,				// Family And Pitch
						type.name.c_str());						// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want

	wglUseFontOutlines(	hDC,							// Select The Current DC
						32,								// Starting Character
						96,								// Number Of Display Lists To Build
						base,							// Starting Display Lists
						0.0f,							// Deviation From The True Outlines
						type.depth,						// Font Thickness In The Z Direction
						WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
						gmf);							// Address Of Buffer To Recieve Data


	// clear the average width
	avrwidth=0.0f;
	// calculate the average width
	for(int ct=0; ct<96; ct++)
		avrwidth+=gmf[ct].gmfCellIncX;
	avrwidth/=96.0f;

	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void Text3D :: Write(string str)
{
	glPushAttrib(GL_LIST_BIT);								// Pushes The Display List Bits
	glListBase(base - 32);									// Sets The Base Character to 32
	glCallLists(str.length(),GL_UNSIGNED_BYTE,str.c_str());	// Draws The Display List Text
	glPopAttrib();											// Pops The Display List Bits
}

}; // namespace NeHe