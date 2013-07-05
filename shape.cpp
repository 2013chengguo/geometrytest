/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Shape rendering class

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

#include "shape.h"
#include "nexception.h"

namespace NeHe {

Shapes :: Shapes()
{
	// clear the quadric - we only want to allocate it if we need it
	quadric=NULL;
}

Shapes :: ~Shapes()
{
	// do we need to delete the quadric
	if(quadric)
		gluDeleteQuadric(quadric);
}

void Shapes :: Sphere(float radius,int divisions)
{
	// do we need to allocate a quadric?
	if(!quadric)
	{
		// allocate the quadric
		quadric=gluNewQuadric();

		// did that work?
		if(!quadric)
			NTHROW("Shapes :: Sphere - Cannot create a new quadric.");

		// setup the quadric
		gluQuadricNormals(quadric,GLU_SMOOTH);
		gluQuadricTexture(quadric,GL_TRUE);
	}

	// draw the sphere
	gluSphere(quadric,(double)radius,divisions,divisions);
}

void Shapes :: Cylinder(float topradius,float baseradius,float height,int divisions)
{
	// do we need to allocate a quadric?
	if(!quadric)
	{
		// allocate the quadric
		quadric=gluNewQuadric();

		// did that work?
		if(!quadric)
			NTHROW("Shapes :: Cylinder - Cannot create a new quadric.");

		// setup the quadric
		gluQuadricNormals(quadric,GLU_SMOOTH);
		gluQuadricTexture(quadric,GL_TRUE);
	}

	// draw the cylinder
	gluCylinder(quadric,baseradius,topradius,height,divisions,divisions);
}

void Shapes :: Cube(float xsize,float ysize,float zsize)
{
	// calculate half of each size
	float xs2=xsize/2;
	float ys2=ysize/2;
	float zs2=zsize/2;

	// draw the cube
	glBegin(GL_QUADS);									// Draw Using Quad
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f( xs2, ys2,-zs2);					// Top Right Of The Quad (Top)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-xs2, ys2,-zs2);					// Top Left Of The Quad (Top)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-xs2, ys2, zs2);					// Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f( xs2, ys2, zs2);					// Bottom Right Of The Quad (Top)

		glNormal3f(0.0f,-1.0f,0.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f( xs2,-ys2, zs2);					// Top Right Of The Quad (Bottom)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-xs2,-ys2, zs2);					// Top Left Of The Quad (Bottom)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-xs2,-ys2,-zs2);					// Bottom Left Of The Quad (Bottom)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f( xs2,-ys2,-zs2);					// Bottom Right Of The Quad (Bottom)
		
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f( xs2, ys2, zs2);					// Top Right Of The Quad (Front)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-xs2, ys2, zs2);					// Top Left Of The Quad (Front)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-xs2,-ys2, zs2);					// Bottom Left Of The Quad (Front)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f( xs2,-ys2, zs2);					// Bottom Right Of The Quad (Front)
		
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f( xs2,-ys2,-zs2);					// Top Right Of The Quad (Back)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-xs2,-ys2,-zs2);					// Top Left Of The Quad (Back)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-xs2, ys2,-zs2);					// Bottom Left Of The Quad (Back)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f( xs2, ys2,-zs2);					// Bottom Right Of The Quad (Back)
		
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(-xs2, ys2, zs2);					// Top Right Of The Quad (Left)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-xs2, ys2,-zs2);					// Top Left Of The Quad (Left)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-xs2,-ys2,-zs2);					// Bottom Left Of The Quad (Left)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(-xs2,-ys2, zs2);					// Bottom Right Of The Quad (Left)
		
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f( xs2, ys2,-zs2);					// Top Right Of The Quad (Right)
		glTexCoord2f(0.0f,0.0f);
		glVertex3f( xs2, ys2, zs2);					// Top Left Of The Quad (Right)
		glTexCoord2f(0.0f,1.0f);
		glVertex3f( xs2,-ys2, zs2);					// Bottom Left Of The Quad (Right)
		glTexCoord2f(1.0f,1.0f);
		glVertex3f( xs2,-ys2,-zs2);					// Bottom Right Of The Quad (Right)
	glEnd();											// Done Drawing The Quad
}

};	// namespace NeHe