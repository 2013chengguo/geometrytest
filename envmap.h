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

#ifndef ENVMAP_H
#define ENVMAP_H

#include "opengl.h"
#include "texture.h"

namespace NeHe {

class EnvMap {
	bool		mapon;		// is the mapping on?
	unsigned	envid;		// environment map texture id
	Texture		*tex;		// texture object
public:
	EnvMap();

	// 
	// Control functions
	//

	// Setup the environment map
	//	Params:
	//		id: texture id of environment map
	void Setup(unsigned id,Texture *texture);

	// Switch on environment mapping
	void On();

	// switch off environment mapping
	void Off();
};

} // namespace NeHe

#endif