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

#include "button.h"

namespace NeHe {

Button :: Button()
{
	// clear everything
	posx=posy=0;
	farx=fary=0;
}

void Button :: Setup(int x,int y,int wid,int hit)
{
	// just copy over the values
	posx=x;
	posy=y;
	farx=x+wid;
	fary=y+hit;
}

BOOL Button :: Load(FILE *fp)
{
	// do we have a good file pointer
	if(!fp)
		return FALSE;

	// read in data
	if(fscanf(fp,"%d %d %d %d",&posx,&posy,&farx,&fary)!=4)
		return FALSE;

	// Success
	return TRUE;
}

BOOL Button :: Pressed(int x,int y)
{
	// is in the button bounds?
	if(x>=posx && x<=farx)
		if(y>=posy && y<=fary)
			return TRUE;

	// no
	return FALSE;
}

void Button :: Get(int *x,int *y,int *wid,int *hit)
{
	// copy over position
	*x=posx;
	*y=posy;

	// calc the dimensions
	*wid=farx-posx;
	*hit=fary-posy;
}

}; // namespace NeHe