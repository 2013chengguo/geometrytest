/*	Nehe SDK
	Copyright 2003 Jeff Molofee, Gregory Austwick and Others

	Title:
		Scissor box class

	Version History:
		v1.00	New class	(GA 12/04/03)
	

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

#include "scissorbox.h"

namespace NeHe {

ScissorBox :: ScissorBox()
{
	// clear everything
	posx=posy=0;
	width=height=0;
	scissor=false;
}

ScissorBox :: ScissorBox(int x,int y,int wid,int hit)
{
	// just use setup
	this->Setup(x,y,wid,hit);
}

ScissorBox :: ~ScissorBox()
{
	// just switch off if necessary
	this->Off();
}

void ScissorBox :: Setup(int x,int y,int wid,int hit)
{
	// just copy over values
	posx=x;
	posy=y;
	width=wid;
	height=hit;
}

void ScissorBox :: On()
{
	// anything to do?
	if(scissor)
		return;

	// setup the scissorbox
	glScissor(posx,posy,width,height);

	// and switch it on
	glEnable(GL_SCISSOR_TEST);

	// and remind ourselves
	scissor=true;
}

void ScissorBox :: Off()
{
	// anything to do?
	if(!scissor)
		return;

	// switch off scissor test
	glDisable(GL_SCISSOR_TEST);

	// and remind ourselves
	scissor=false;
}

} 	// namespace NeHe