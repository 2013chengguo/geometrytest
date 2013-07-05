/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Texture Manager	

	Version History:
		v1.00	New class (GA 31/10/02)

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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "image.h"
#include <list>
#include <string>

namespace NeHe {

// the texture information type
typedef struct TextureType { 
					std ::string		name;	// name of the texture
					int					width;	// width of texture
					int					height;	// height of texture
					bool				alpha;	// whether the texture has an alpha component
					unsigned			id;		// the texture ID
					TextureType()
						{ name="None"; width=height=0; alpha=false; id=0U; }
					TextureType(std :: string str,int wid,int hit,bool alp,unsigned ident)
						{ name=str; width=wid; height=hit; alpha=alp; id=ident; }
				}TextureType;


// The texture manager class
class Texture {
	// list of textures
	typedef std :: list<TextureType>	TListType;	// the texture list type
	TListType							tlist;		// the texture list
	// present texture
	unsigned	prestex;
public:
	Texture()	{ tlist.clear(); prestex=0U; };		// initialise list
	~Texture()	{ this->Reset(); };					// reset the texture list

	//
	//	Texture Creation functions
	//

	// Load a texture
	//	Params:	
	//		filename: texture to load
	//	Returns:
	//		zero on failure, otherwise texture id
	unsigned Load(std :: string filename);
	// Create a texture from an image
	//	Params:
	//		img: image class to create from
	//	Returns:
	//		zero on failure, otherwise texture id
	unsigned Create(Image *img);

	//
	//	Texture Set procedure
	//

	void Set(unsigned texid);

	//
	//	Texture information functions
	//

	// get info by texure id
	TextureType GetInfo(unsigned texid);
	// get info by texture name
	TextureType GetInfo(std :: string str);

	//
	//	Texture deletion functions
	//

	// Delete a single texture
	void Delete(unsigned texid);
	void Delete(std :: string texname);
	// Delete all the textures
	void Reset();
};

}; // namespace NeHe

#endif