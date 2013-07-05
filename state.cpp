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

#include "state.h"
#include "nexception.h"
#include <GL/gl.h>

namespace NeHe {

static GLenum GLLights[]= { GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7 };


StateManager :: StateManager()
{
	// switch everything off
	texturing=lighting=false;
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	blend=blOff;
	glDisable(GL_BLEND);
	for(int ct=0; ct<8; ct++)
	{
		lights[ct]=false;
		glDisable(GLLights[ct]);
	}
	// switch on depth testing
	depth=true;
	glEnable(GL_DEPTH_TEST);
	// start in 3d mode
	in2d=false;
	view.Id();
	proj.Id();
}

void StateManager :: SetTexturing(bool state)
{
	// anything to do?
	if(texturing==state)
		return;

	// set state
	if(state)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	// and record it
	texturing=state;
}

void StateManager :: SetLighting(bool state)
{
	// anything to do?
	if(lighting==state)
		return;

	// set state
	if(state)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	// and record it
	lighting=state;
}

void StateManager :: SetDepthTest(bool state)
{
	// anything to do?
	if(depth==state)
		return;

	// set state
	if(state)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	// and record it
	depth=state;
}

void StateManager :: SetLight(int lightno,bool state)
{
	// is the light within range?
	if(lightno<0 || lightno>7)
		NTHROW("StateManager :: SetLight - Light index out of range.");

	// anything to do?
	if(lights[lightno]==state)
		return;

	// set the state
	if(state)
		glEnable(GLLights[lightno]);
	else
		glDisable(GLLights[lightno]);

	// and record it
	lights[lightno]=state;
}

void StateManager :: SetBlending(Blending state)
{
	// anything to do?
	if(blend==state)
		return;

	if(state==blOff)
		glDisable(GL_BLEND);								// just switch off blending
	else if(state==blAlpha) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	// use alpha blending		
	}else if(state==blNormal) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// use normal blending
	}else{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_SRC_COLOR);					// switch to multitexture blending
	}

	// save blending state
	blend=state;
}

void StateManager :: SetColour(Colour col)
{
	glColor3f(col.red,col.green,col.blue);
}

void StateManager :: SetColour(ColourAlpha col)
{
	glColor4f(col.red,col.green,col.blue,col.alpha);
}

void StateManager :: Set2DMode()
{
	// get the view and projection matrices
	this->GetView(view);
	this->GetProjection(proj);

	// get the viewport
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	// extract the width and height
	int width=viewport[2];
	int height=viewport[3];

	// clear the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// and setup an orthogonal matrix
	glOrtho(0.0f,width,height,0.0f,-1.0f,1.0f);

	// clear the view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// store the depth test state
	depth2d=this->GetDepthTest();

	// switch off depth testing
	this->SetDepthTest(false);

	// say we're in 2d mode
	in2d=true;
}

void StateManager :: Set3DMode()
{
	// anything to do?
	if(!in2d)
		return;

	// restore matrices
	this->SetProjection(proj);
	this->SetView(view);

	// restore depth test
	this->SetDepthTest(depth2d);

	// and say we're back in 3d mode
	in2d=false;
}

void StateManager :: GetView(Matrix &mat)
{
	float view[16];		// the view matrix

	// get the matrix
	glGetFloatv(GL_MODELVIEW_MATRIX,view);
	// and set it into the parameter
	mat=Matrix(view);
}

void StateManager :: GetProjection(Matrix &mat)
{
	float proj[16];		// the projection matrix

	// get the matrix
	glGetFloatv(GL_PROJECTION_MATRIX,proj);
	// and set it into the parameter
	mat=Matrix(proj);
}

void StateManager :: SetView(Matrix mat)
{
	// set the correct matrix
	glMatrixMode(GL_MODELVIEW);
	// and load it in
	glLoadMatrixf(mat.m);
}

void StateManager :: SetProjection(Matrix mat)
{
	// set the correct matrix
	glMatrixMode(GL_PROJECTION);
	// and load it in
	glLoadMatrixf(mat.m);
	// we like to stay in view mode
	glMatrixMode(GL_MODELVIEW);
}

}; // namespace NeHe