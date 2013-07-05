/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Button Handling class	

	Version History:
		v1.00		Copied from my ill-fated tutorial series:) (GA 29/10/02)

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

#ifndef BUTTON
#define BUTTON

#define STRICT
#include <windows.h>
#include <stdio.h>

namespace NeHe {

class Button {
	int		posx,posy;		// position of button
	int		farx,fary;		// position of bottom right
public:
	Button();
	// Setup the button
	//	Params:
	//		(x,y):		position of button
	//		(wid,hit):	dimensions of button
	void Setup(int x,int y,int wid,int hit);
	// Load Button
	//	Params:
	//		fp: file pointer
	//	Returns:
	//		TRUE if no problems
	BOOL Load(FILE *fp);
	// Is button pressed?
	//	Params:
	//		x,y: pointer position
	//	Returns:
	//		TRUE if pressed
	BOOL Pressed(int x,int y);
	// Get position and size
	//	Params:
	//		(x,y):		position of button
	//		(wid,hit):	button's dimensions
	void Get(int *x,int *y,int *wid,int *hit);
};

};	// namespace NeHe

#endif