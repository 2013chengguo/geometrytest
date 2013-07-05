/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Texture Manager	

	Version History:
		v1.00	New class	(GA 31/10/02)

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

#include "texture.h"
#include "nexception.h"
#include <algorithm>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

namespace NeHe {

//
//	Predicate functions
//

// testing values
static unsigned			chkid;				// ID to check
static string			chkstr;				// string to check

// test id
bool CheckID(TextureType tex)	{ return tex.id==chkid; };
// test string
bool CheckStr(TextureType tex)	{ return tex.name==chkstr; };

//
//	Texture Creation functions
//

// Load a texture
//	Params:	
//		filename: texture to load
unsigned Texture :: Load(string filename)
{
	// do we already have the texture?
	if(!tlist.empty())
	{
		// find by the filename
		chkstr=filename;
		TListType :: iterator found=find_if(tlist.begin(),tlist.end(),CheckStr);

		// is it already in the list?
		if(found!=tlist.end())
			return found->id;				// return it's id
	}

	// Create us an image class
	Image img;

	// load in the file
	if(!img.Load((char *)filename.c_str()))
		return 0;							// file doesn't exist, or problem loading the file

	// Create a Texture Type
	TextureType tex;
	// and fill it with the information from the Image class
	img.GetDimensions(tex.width,tex.height);
	tex.alpha=img.HasAlpha();
	// and take the name from the filename
	tex.name=filename;

	unsigned texid;							// the texture id

	// get a new id
	glGenTextures(1,&texid);
	// and bind it as the present texture
	glBindTexture(GL_TEXTURE_2D,texid);

	// now build mipmaps from the texture data
	gluBuild2DMipmaps(GL_TEXTURE_2D,tex.alpha ? 4 : 3,tex.width,tex.height,
				tex.alpha ? GL_RGBA : GL_RGB,GL_UNSIGNED_BYTE,img.GetImage());

	// place the texture id into the texture type
	tex.id=texid;
	// and put the texture into the list
	tlist.push_back(tex);

	// finally return the texture id
	return texid;
}

// Create a texture from an image
//	Params:
//		img: image class to create from
unsigned Texture :: Create(Image *img)
{
	// anything to do?
	if(!img)
		NTHROW("Texture :: Create - NULL pointer passed.");

	// Create a Texture Type
	TextureType tex;
	// and fill it with the information from the Image class
	img->GetDimensions(tex.width,tex.height);
	tex.alpha=img->HasAlpha();
	
	unsigned texid;							// the texture id

	// get a new id
	glGenTextures(1,&texid);
	// and bind it as the present texture
	glBindTexture(GL_TEXTURE_2D,texid);

	// now build mipmaps from the texture data
	gluBuild2DMipmaps(GL_TEXTURE_2D,tex.alpha ? 4 : 3,tex.width,tex.height,
				tex.alpha ? GL_RGBA : GL_RGB,GL_UNSIGNED_BYTE,img->GetImage());

	// place the texture id into the texture type
	tex.id=texid;
	
	// convert texture id to string
	char texidstr[10];
	_itoa((int)texid,texidstr,10);
	// build name from texture id
	tex.name="NeHeImage";
	tex.name+=texidstr;
	// and put the texture into the list
	tlist.push_back(tex);

	// finally return the texture id
	return texid;
}

//
//	Texture Set procedure
//


void Texture :: Set(unsigned texid)
{
	// do we need to change the texture
	if(texid==prestex)
		return;

	// set to the id
	glBindTexture(GL_TEXTURE_2D,texid);

	// and set as present texture
	prestex=texid;
}

//
//	Texture information functions
//

// get info by texure id
TextureType Texture :: GetInfo(unsigned texid)
{
	// anything to do?
	if(tlist.empty())
		NTHROW("Texture :: GetInfo - No textures allocated.");

	// find the id
	chkid=texid;
	TListType :: iterator found=find_if(tlist.begin(),tlist.end(),CheckID);

	if(found!=tlist.end())
		return *found;			// return the found texture
	else
		NTHROW("Texture :: GetInfo - Texture id not found.");

}

// get info by texture name
TextureType Texture :: GetInfo(std :: string str)
{
	// anything to do?
	if(tlist.empty())
		NTHROW("Texture :: GetInfo - No textures allocated.");

	// find the name
	chkstr=str;
	TListType :: iterator found=find_if(tlist.begin(),tlist.end(),CheckStr);

	if(found!=tlist.end())
		return *found;			// return the found texture
	else
		NTHROW("Texture :: GetInfo - Texture id not found.");
}


//
//	Deletion procedures
//

void Texture :: Delete(unsigned texid)
{
	// anything to do?
	if(tlist.empty())
		NTHROW("Texture :: Delete - No textures allocated.");

	// find the id
	chkid=texid;
	TListType :: iterator found=find_if(tlist.begin(),tlist.end(),CheckID);

	// was it found?
	if(found!=tlist.end())
	{
		// remove the texture
		glDeleteTextures(1,&texid);
		// remove the texture from the list
		tlist.erase(found);
	}
}

void Texture :: Delete(string str)
{
	// anything to do?
	if(tlist.empty())
		NTHROW("Texture :: Delete - No textures allocated.");

	// find the name
	chkstr=str;
	TListType :: iterator found=find_if(tlist.begin(),tlist.end(),CheckStr);

	// was it found?
	if(found!=tlist.end())
	{
		// remove the texture
		glDeleteTextures(1,&(found->id));
		// remove the texture from the list
		tlist.erase(found);
	}
}

void Texture :: Reset()
{
	// anything to do?
	if(tlist.empty())
		return;
	// delete the textures
	while(!tlist.empty())
	{
		// get the first object in the list
		TListType :: iterator tex=tlist.begin();
		// remove the texture from the hardware
		glDeleteTextures(1,&(tex->id));
		// and from the list
		tlist.erase(tex);
	}
}

}; // namespace NeHe