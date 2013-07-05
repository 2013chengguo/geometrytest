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

#ifndef SCISSORBOX_H
#define SCISSORBOX_H

#include "opengl.h"

namespace NeHe {

class ScissorBox {
	int		posx,posy;			// position of top left of scissor box
	int		width,height;		// dimensions of scissor box
	bool	scissor;			// scissoring on
public:
	ScissorBox();
	ScissorBox(int x,int y,int wid,int hit);
	~ScissorBox();

	//
	// Control functions
	//

	// Setup the scissor box
	void Setup(int x,int y,int wid,int hit);

	// switch on scissor box
	void On();

	// switch off scissor box
	void Off();
};

}	// namespace NeHe

#endif