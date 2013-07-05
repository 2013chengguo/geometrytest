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

#include "material.h"

namespace NeHe {

Material :: Material()
{
	// set colours to opengl defaults
	ambient=ColourAlpha(0.2f,0.2f,0.2f,1.0f);
	diffuse=ColourAlpha(0.8f,0.8f,0.8f,1.0f);
	specular=BLACKALPHA;
	emissive=BLACKALPHA;
	// and reset everything else
	shininess=0.0f;
	transparency=1.0f;
}

Material :: Material(Colour col)
{
	// set the colours to the above
	ambient=ColourAlpha(col);
	diffuse=ColourAlpha(col);
	specular=ColourAlpha(col);
	emissive=BLACKALPHA;
	// and reset everything else
	shininess=0.0f;
	transparency=1.0f;
}

Material :: Material(const Material &mat)
{
	// we're a copy constructor, so copy!
	ambient=ColourAlpha(mat.ambient);
	diffuse=ColourAlpha(mat.diffuse);
	specular=ColourAlpha(mat.specular);
	emissive=ColourAlpha(mat.emissive);
	shininess=mat.shininess;
	transparency=mat.transparency;
}

void Material :: Activate()
{
	float cdata[4];	// temporary array for colour data

	// set colour, just in case materials aren't enabled
	glColor4f(diffuse.red,diffuse.green,diffuse.blue,diffuse.alpha*transparency);

	// first ambient
	cdata[0]=ambient.red;
	cdata[1]=ambient.green;
	cdata[2]=ambient.blue;
	cdata[3]=ambient.alpha*transparency;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,cdata);

	// then diffuse
	cdata[0]=diffuse.red;
	cdata[1]=diffuse.green;
	cdata[2]=diffuse.blue;
	cdata[3]=diffuse.alpha*transparency;
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,cdata);

	// then specular
	cdata[0]=specular.red;
	cdata[1]=specular.green;
	cdata[2]=specular.blue;
	cdata[3]=specular.alpha*transparency;
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,cdata);

	// finally emissive
	cdata[0]=emissive.red;
	cdata[1]=emissive.green;
	cdata[2]=emissive.blue;
	cdata[3]=emissive.alpha*transparency;
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,cdata);

	// set shininess
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
}

}; // namespace NeHe

