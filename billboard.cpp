/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Billboard classes	

	Version History:
		v1.00	New file		(GA 1/11/02)

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


#include "billboard.h"
#include "nexception.h"
#include <GL\gl.h>

namespace NeHe {

Billboard :: Billboard()
{
	// and clear everything else
	pos=Vector(0.0f,0.0f,0.0f,1.0f);
	xsize=ysize=0.0f;
	col=WHITEALPHA;
}

Billboard :: Billboard(const Billboard &b)
{
	// just copy over everything
	pos=Vector(b.pos.x,b.pos.y,b.pos.z);
	xsize=b.xsize;
	ysize=b.ysize;
	col=ColourAlpha(b.col.red,b.col.green,b.col.blue,col.alpha);
}

BillboardGroup :: BillboardGroup(StateManager *sman,Texture *texman)
{
	// copy over the managers
	state=sman;
	tman=texman;
	// and clear everything else
	tex=0U;
	bbs.clear();
	lit=false;
}

void BillboardGroup :: Draw()
{
	// anything to do?
	if(!state)
		NTHROW("Billboard :: Draw - No state manager assigned.");
	if(!tman)
		NTHROW("Billboard :: Draw - No texture manager assigned.");
	if(bbs.empty())
		return;

	// store the texturing state
	bool texstate=state->GetTexturing();
	// store the lighting state
	bool lightstate=state->GetLighting();
	// sote the blending state
	Blending blendstate=state->GetBlending();

	// switch on texturing
	if(!texstate)
		state->SetTexturing(true);

	// set the texture
	tman->Set(tex);

	// switch lighting by the "lit" flag
	state->SetLighting(lit);

	// set blending mode
	state->SetBlending(blNormal);

	// get the present view matrix
	Matrix view;
	state->GetView(view);

	// reset the matrix
	Matrix	id;				// identity matrix
	id.Id();				// load it with the identity matrix
	state->SetView(id);		// set it

	// variables for the loop
	float		xsize,ysize;			// size of the billboard
	Vector		vec;					// position of the billboard
	ColourAlpha col;					// colour of the billboard

	// start drawing
	glBegin(GL_QUADS);

	// run through the billboards
	for(BBtype :: iterator pos=bbs.begin(); pos!=bbs.end(); pos++)
	{
		// get the size of the present billboard
		pos->GetSize(xsize,ysize);

		// calc half sizes
		float xsize2=xsize/2.0f;
		float ysize2=ysize/2.0f;

		// get the position of the billboard
		pos->GetPosition(vec);

		// and project it
		vec=view*vec;

		// get the colour
		pos->GetColour(col);

		// and draw
		glColor4f(col.red,col.green,col.blue,col.alpha);
	    glTexCoord2d(1,1); glVertex3f(vec.x+xsize2,vec.y+ysize2,vec.z); // Top Right
		glTexCoord2d(0,1); glVertex3f(vec.x-xsize2,vec.y+ysize2,vec.z); // Top Left
		glTexCoord2d(0,0); glVertex3f(vec.x-xsize2,vec.y-ysize2,vec.z); // Bottom Left
		glTexCoord2d(1,0); glVertex3f(vec.x+xsize2,vec.y-ysize2,vec.z); // Bottom Right
	}

	// finish drawing
	glEnd();

	// restore view matrix
	state->SetView(view);

	// restore blending mode
	state->SetBlending(blendstate);
	// restore the lighting state
	state->SetLighting(lightstate);
	// restore the texturing state
	state->SetTexturing(texstate);
}

};	// namespace NeHe