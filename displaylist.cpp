/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Display List Manager class

	Version History:
		v1.00	New class	(GA 13/12/02)
	

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

#include "displaylist.h"

namespace NeHe {

DisplayList :: DisplayList()
{
	// clear the list
	lists.clear();
}

DisplayList :: ~DisplayList()
{
	// use our clear command
	this->Clear();
}

void DisplayList :: Clear()
{
	if(!lists.empty())
	{
		// remove the lists from OpenGL
		for(int ct=0; ct<(int)lists.size(); ct++)
			glDeleteLists(lists[ct],1);

		// remove the lists from our array
		lists.clear();
	}
}

unsigned int DisplayList :: Start()
{
	// create a new display list
	unsigned int id=glGenLists(1);

	// did that work?
	if(id==0U)
		return 0U;

	// start the list
	glNewList(id,GL_COMPILE);

	// add to the array
	lists.push_back(id);

	// success
	return id;
}

void DisplayList :: End()
{
	// finish the list
	glEndList();
}

void DisplayList :: Execute(unsigned int id)
{
	// call the list
	glCallList(id);
}

};	// namespace NeHe