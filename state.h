/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		State Manager	

	Version History:
		v1.00		New class	(GA 1/11/02)
		v1.01		Added 2d functionality
								(GA 12/04/03)

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

#ifndef STATE_H
#define STATE_H

#define STRICT
#include <windows.h>
#include "geometry.h"
#include "colour.h"

namespace NeHe {

typedef enum Blending { blOff , blAlpha , blNormal , blMultiTex};

class StateManager {
	bool		texturing;			// texturing state
	bool		lighting;			// lighting state
	bool		depth;				// depth test state
	bool		in2d;				// are we in 2d?
	bool		lights[8];			// light state
	Blending	blend;				// blending state
	Matrix		proj,view;			// projection and view matrices
	bool		depth2d;			// the state of the depth test before going into 2d
public:
	StateManager();

	//
	//	Set states
	//

	// texturing
	void SetTexturing(bool state);
	// lighting
	void SetLighting(bool state);
	// Set specific light (lights numbered 0-7)
	void SetLight(int lightno,bool state);
	// Set blending state
	void SetBlending(Blending state);
	// Set depth test
	void SetDepthTest(bool state);
	// Set present colour
	void SetColour(Colour col);
	// Set present colour wih alpha
	void SetColour(ColourAlpha col);
	// Set 2d mode
	void Set2DMode();
	// Set 3d mode
	void Set3DMode();

	//
	//	Get States
	//

	// texturing
	bool GetTexturing() { return texturing; };
	// lighting
	bool GetLighting()	{ return lighting; };
	// get specific light state
	bool GetLight(int lightno)	{ return (lightno<0 || lightno>7) ? false : lights[lightno]; };
	// get blending state
	Blending GetBlending()		{ return blend; };
	// get depth test state
	bool GetDepthTest()			{ return depth; };
	// get 2d state
	bool Get2D()				{ return in2d; };

	//
	//	Matrix operations
	//

	// get matrices
	void GetView(Matrix &mat);
	void GetProjection(Matrix &mat);
	// set matrices
	void SetView(Matrix mat);
	void SetProjection(Matrix mat);
};

}; // namespace NeHe

#endif