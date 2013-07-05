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

#include "opengl.h"
#include "fog.h"

namespace NeHe {

Fog :: Fog()
{
	// clear all
	col=BLACK;
	type=FogExp;
	density=fnear=ffar=0.0f;
}

Fog :: Fog(Colour colour,FogType ftype,float fdens,float fstart,float fend)
{
	// just use setup
	this->Setup(colour,ftype,fdens,fstart,fend);
}

void Fog :: Setup(Colour colour,FogType ftype,float fdens,float fstart,float fend)
{
	// just copy over the values
	col=colour;
	type=ftype;
	fdens=density;
	fnear=fstart;
	ffar=fend;
}

void Fog :: On()
{
	// set the fog type
	if(type==FogExp)
		glFogi(GL_FOG_MODE,GL_EXP);
	else if(type==FogExp2)
		glFogi(GL_FOG_MODE,GL_EXP2);
	else
		glFogi(GL_FOG_MODE,GL_LINEAR);

	// set fog colour
	float glcol[4] = { col.red , col.green, col.blue , 1.0f };
	glFogfv(GL_FOG_COLOR,glcol);
	// set density
	glFogf(GL_FOG_DENSITY,density);
	// set extents
	glFogf(GL_FOG_START,fnear);
	glFogf(GL_FOG_END,ffar);
	// set hint
	glHint(GL_FOG_HINT,GL_DONT_CARE);

	// and switch on fog
	glEnable(GL_FOG);
}

void Fog :: Off()
{
	// just switch off the fog
	glDisable(GL_FOG);
}

}	// namespace NeHe
