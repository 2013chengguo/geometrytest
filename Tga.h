/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		TGA Loading functions

	Version History:
		v1.00  Converted from lesson 33 by Evan "terminate" Pipho and Jeff Molofee (GA 30/10/02) 

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

#ifndef TGA_H_
#define TGA_H_

#define STRICT
#include <windows.h>									// Standard windows header

namespace NeHe {

bool LoadTGA(char *filename,int &width,int &height,bool &alpha,BYTE **bits);

}; // namespace NeHe

#endif






