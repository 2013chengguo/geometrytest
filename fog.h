/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Fog class

	Version History:
		v1.00	New class	(11/04/03)

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

#ifndef FOG_H
#define FOG_H

#include "colour.h"

namespace NeHe {

typedef enum FogType { FogExp , FogExp2 , FogLin };

class Fog {
	Colour	col;		// fog colour
	FogType	type;		// fog type
	float	density;	// fog density
	float	fnear,ffar;	// fog extents
public:
	Fog();
	Fog(Colour colour,FogType ftype,float fdens,float fstart,float fend);

	// setup fog
	void Setup(Colour colour,FogType ftype,float fdens,float fstart,float fend);

	// set fog state
	void On();
	void Off();
};

}	// namespace NeHe

#endif