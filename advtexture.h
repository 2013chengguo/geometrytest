/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Advanced Texture class	

	Version History:
		v1.00	New file		(GA 17/11/02)
		v1.01	Added access to number of texture stages,
				plus increased const correctness.
								(GA 10/04/03)

    Notes:
		Some parts based on code from lesson 22

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

#ifndef ADVTEXTURE_H
#define ADVTEXTURE_H

#include "opengl.h"
#include "texture.h"

namespace NeHe {

#define MAX_TEXTURE_STAGES	8

class AdvancedTexture {
	bool			multitex;					// multitexturing available
	int				numstages;					// number of stages
	Blending		blendstate;					// blending state
	StateManager	*sman;						// the state manager
	// helper functions
	bool	CheckExtension(const char *str);	// see whether a certain extension is supported
public:
	AdvancedTexture(StateManager *smangr);

	//
	//	Multitexturing procedures
	//

	// Set active texture stage
	//	Params:
	//		stage: texture stage
	void Set(int stage);
	// Set Texture Coords of texture stage
	void TexCoords(int stage,float u,float v);

	// Switch on multitexturing for secondary stages for
	//	cards that don't support multitexturing
	void MultiTexOn();
	// and off again
	void MultiTexOff();

	//
	// State functions
	//
	bool HasMultiTexturing() const	{ return multitex; };

	// get the number of texture stages
	int GetNumStages() const		{ return numstages; };
};

}; // namespace NeHe

#endif