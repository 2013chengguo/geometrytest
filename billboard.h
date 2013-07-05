/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Billboard classes	

	Version History:
		v1.00	New file			(GA 1/11/02)
		v1.01	Added clear command	(GA 21/11/02)

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

#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "geometry.h"
#include "state.h"
#include "texture.h"
#include "colour.h"
#include <vector>

namespace NeHe {

class Billboard {
protected:
	Vector			pos;			// position of billboard
	float			xsize,ysize;	// size of billboard
	ColourAlpha		col;			// colour of billboard
public:
	Billboard();
	Billboard(const Billboard &b);

	//
	//	Setup functions
	//

	// set the position of the billboard
	void SetPosition(Vector v)		{ pos=Vector(v); };
	// set the size of the billboard
	void SetSize(float x,float y)	{ xsize=x; ysize=y; };
	// set the colour of the billboard
	void SetColour(Colour clr)		{ col=ColourAlpha(clr); };
	void SetColour(ColourAlpha clr)	{ col=ColourAlpha(clr); };

	//
	// Data Access functions
	//

	// get the size of the billboard
	void GetSize(float &x,float &y)	const	{ x=xsize; y=ysize; };
	// get the position
	void GetPosition(Vector &v) const		{ v=pos; };
	// get the colour
	void GetColour(ColourAlpha &c) const	{ c=col; };
};

class BillboardGroup {
protected:
	StateManager						*state;	// the state manger
	Texture								*tman;	// the texture manager
	unsigned int						tex;	// the texture for the billboard group
	typedef std :: vector<Billboard>	BBtype;	// the billoard type
	BBtype								bbs;	// the billboards
	bool								lit;	// whether we want the billboard to be lit
public:
	BillboardGroup(StateManager *sman,Texture *texman);
	virtual ~BillboardGroup()				{ bbs.clear(); };	
	
	//
	//	Setup functions
	//

	// set the texture
	void SetTexture(unsigned int texid)	{ tex=texid; };
	// add a billboard
	void AddBillboard(Billboard bill)	{ bbs.push_back(bill); };
	// say whether the billboard should be lit (default: no)
	void SetLit(bool litbb)				{ lit=litbb; };
	// clear the billboards
	void Clear()						{ bbs.clear(); };	
	
	//
	//	Draw billboards
	//

	// draw all the billboards
	virtual void Draw();
};

};	// namespace NeHe

#endif