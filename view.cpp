/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		View Matrix proxy	

	Version History:
		v1.00	New class	(GA 11/11/02)

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

#include "view.h"

namespace NeHe {

void View :: Reset()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void View :: Translate(float x,float y,float z)
{
	// translate by (x,y,z)
	glTranslatef(x,y,z);
}

void View :: Rotate(float angle,float x,float y,float z)
{
	// rotate axis (x,y,z), angle
	glRotatef(angle,x,y,z);
}

void View :: Scale(float x,float y,float z)
{
	// scale by (x,y,z)
	glScalef(x,y,z);
}

void View :: Pos2D(int x,int y)
{
	glRasterPos2i(x,y);
}

void View :: Pos2D(float x,float y)
{
	glRasterPos2f(x,y);
}

void View :: LookAt(Vector pos,Vector at,Vector up)
{
	gluLookAt(pos.x,pos.y,pos.z,at.x,at.y,at.z,up.x,up.y,up.z);
}

void View :: Save()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void View :: Restore()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


}; // namespace NeHe