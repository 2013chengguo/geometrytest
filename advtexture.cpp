/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Advanced Texture class	

	Version History:
		v1.00	New file		(GA 17/11/02)
		v1.01	Added more safety to the constructor
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

#include "advtexture.h"
#include "nexception.h"
#include "glext.h"
#include <cstring>

namespace NeHe {

bool AdvancedTexture ::CheckExtension(const char *str)
{
	// get the extension string
	char *extstr=(char *)glGetString(GL_EXTENSIONS);

	// anything to do?
	if(!extstr)
		return false;
	if(strlen(extstr)==0)
		return false;

	// find the required extension
	return (strstr(extstr,str)!=NULL);
}

// ARB extensions
static PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
static PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;


AdvancedTexture :: AdvancedTexture(StateManager *smgr)
{
	// check whether we have multitexturing
	if(this->CheckExtension("GL_ARB_multitexture"))
	{
		// get extension procedures
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		glActiveTextureARB	 = (PFNGLACTIVETEXTUREARBPROC)	 wglGetProcAddress("glActiveTextureARB");		
		// and say we have multitexturing
		multitex=true;
	}else
		multitex=false;

	// get the number of texture units
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&numstages);

	// make sure it's sensible
	if(numstages==0)
		numstages=1;

	// limit it to max stages
	if(numstages>MAX_TEXTURE_STAGES)
		numstages=MAX_TEXTURE_STAGES;

	// store the state manager
	sman=smgr;
}

static GLenum TextureList[] ={ GL_TEXTURE0_ARB , GL_TEXTURE1_ARB , GL_TEXTURE2_ARB ,
	GL_TEXTURE3_ARB , GL_TEXTURE4_ARB , GL_TEXTURE5_ARB , GL_TEXTURE6_ARB , GL_TEXTURE7_ARB };

void AdvancedTexture :: Set(int stage)
{
	// check we can do this
	if(stage<0 || stage>=numstages)
		NTHROW("AdvancedTexture :: Set - Texture stage out of range.");
	if(!multitex)
		NTHROW("AdvancedTexture :: Set - Multitexturing not supported.");

	// set the active texture stage
	glActiveTextureARB(TextureList[stage]);
}

void AdvancedTexture :: TexCoords(int stage,float u,float v)
{
	// check we can do this
	if(stage<0 || stage>=numstages)
		NTHROW("AdvancedTexture :: TexCoords - Texture stage out of range.");
	if(!multitex)
		NTHROW("AdvancedTexture :: TexCoords - Multitexturing not supported.");
	

	// set the texture coords
	glMultiTexCoord2fARB(TextureList[stage],u,v);
}

void AdvancedTexture :: MultiTexOn()
{
	// do we have a state manager?
	if(!sman)
		NTHROW("AdvancedTexture :: MultiTexOn - No state manager assigned.");

	// get the blending state
	blendstate=sman->GetBlending();

	// set blending mode
	sman->SetBlending(blMultiTex);
}

void AdvancedTexture :: MultiTexOff()
{
	// do we have a state manager?
	if(!sman)
		NTHROW("AdvancedTexture :: MultiTexOff - No state manager assigned.");

	// set blending mode
	sman->SetBlending(blendstate);
}

}; // namespace NeHe