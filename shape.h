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

#ifndef SHAPE_H
#define SHAPE_H

#include "opengl.h"
#include "geometry.h"

namespace NeHe {

class Shapes {
	GLUquadricObj *quadric;	// the quadric used to create objects
public:
	Shapes();
	~Shapes();

	// Draw a sphere
	//	Params:
	//		radius:		radius of the sphere
	//		divisions:	subdivisions of the sphere
	void Sphere(float radius,int divisions);
	// Draw a cylinder
	//	Params:
	//		topradius:	radius of the top of the cylinder
	//		baseradius:	radius of the base of the cylinder
	//		height:		height of the cylinder
	//		divisions:	subdivisions of the cylinder
	void Cylinder(float topradius,float baseradius,float height,int divisions);
	// Draw a cuboid
	//	Params:
	//		xsize:		size of the cuboid in the x direction
	//		ysize:		size of the cuboid in the y direction
	//		zsize:		size of the cuboid in the z direction
	void Cube(float xsize,float ysize,float zsize);
};

};	// namespace NeHe

#endif