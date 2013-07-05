/*	Nehe SDK
	Copyright 2003 Jeff Molofee, Gregory Austwick and Others

	Title:
		Environment mapping	

	Version History:
		v1.00		New class	(GA 12/04/03)

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


#include "envmap.h"


namespace NeHe {

EnvMap :: EnvMap()
{
	// clear everything
	mapon=false;
	envid=0U;
	tex=NULL;
}

void EnvMap :: Setup(unsigned id,Texture *texture)
{
	// just copy them over
	envid=id;
	tex=texture;
}

void EnvMap :: On()
{
	// anything to do?
	if(!tex)
		return;

	// set texture generation mode to sphere mapping
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);

	// switch on automatic texture coord generation
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	// set reflection texture
	tex->Set(envid);

	// say we're environment mapping
	mapon=true;
}

void EnvMap :: Off()
{
	// anything to do?
	if(!mapon)
		return;

	// turn off automatic texture coord generation
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	// say we've finished mapping
	mapon=false;
}

}; // namespace NeHe