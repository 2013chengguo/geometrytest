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

#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H

#include "opengl.h"
#include <vector>


namespace NeHe {

class DisplayList {
	std :: vector<unsigned int> lists;
public:
	DisplayList();
	~DisplayList();

	// start a new display list
	//	Returns: display list id
	unsigned int Start();

	// end current display list
	void End();

	// execute list
	void Execute(unsigned list);

	// clear all lists
	void Clear();
};

};	// namespace NeHe

#endif