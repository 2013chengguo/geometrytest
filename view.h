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

#ifndef VIEW_H
#define VIEW_H

#include "opengl.h"
#include "geometry.h"

namespace NeHe {

class View {
public:
	//
	//	Geometric functions
	//
	// reset the view
	void Reset();
	// translate view by (x,y,z)
	void Translate(float x,float y,float z);
	void Translate(Vector vec)			{ this->Translate(vec.x,vec.y,vec.z); };
	// axis-angle rotation
	void Rotate(float angle,float x,float y,float z);
	void Rotate(float angle,Vector vec) { this->Rotate(angle,vec.x,vec.y,vec.z); };
	// scaling by (x,y,z)
	void Scale(float x,float y,float z);
	void Scale(Vector vec)				{ this->Scale(vec.x,vec.y,vec.x); };
	// look at function
	void LookAt(Vector pos,Vector at,Vector up);
	// save the view
	void Save();
	// restore the view
	void Restore();

	//
	//	2D functions
	//
	// move to position in 2d
	void Pos2D(int x,int y);
	void Pos2D(float x,float y);
};

}; // namespace NeHe

#endif