/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Materials class	

	Version History:
		v1.00	New class	(GA 21/11/02)

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

#ifndef MATERIAL_H
#define MATERIAL_H

#include "opengl.h"
#include "colour.h"

namespace NeHe {

class Material {
	ColourAlpha	ambient;		// ambient part
	ColourAlpha	diffuse;		// diffuse part
	ColourAlpha	specular;		// specular part
	ColourAlpha	emissive;		// emissive part
	float		shininess;		// shininess of material
	float		transparency;	// transparency of material
public:
	Material();
	Material(Colour col);			// set ambient,diffuse and specular to col
	Material(const Material &mat);	// copy constructor

	//
	//	Setup functions
	//
	void SetAmbient(const Colour amb)		{ ambient=ColourAlpha(amb); };
	void SetDiffuse(const Colour diff)		{ diffuse=ColourAlpha(diff); };
	void SetSpecular(const Colour spec)		{ specular=ColourAlpha(spec); };
	void SetEmissive(const Colour emi)		{ emissive=ColourAlpha(emi); };
	void SetAmbient(const ColourAlpha amb)		{ ambient=ColourAlpha(amb); };
	void SetDiffuse(const ColourAlpha diff)		{ diffuse=ColourAlpha(diff); };
	void SetSpecular(const ColourAlpha spec)	{ specular=ColourAlpha(spec); };
	void SetEmissive(const ColourAlpha emi)		{ emissive=ColourAlpha(emi); };
	void SetShininess(const float shin)		{ shininess=shin; };
	void SetTransparency(const float trans)	{ transparency=trans; };

	//
	//	Set as active material
	//

	void Activate();
};

};	// namespace NeHe

#endif