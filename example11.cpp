// Example 11 - Environment mapping

#include "opengl.h"
#include "view.h"
#include "displaylist.h"
#include "shape.h"
#include "envmap.h"

using namespace NeHe;

// the view class
static View				view;
static Shapes			shapes;	// shapes class
static DisplayList		dlist;	// displaylist manager
static Texture			tex;	// the texture manager
static EnvMap			envmap;	// environment mapping class


// rotation
static float rot=0.0f;		// sphere rotation

// display lists
static unsigned sphere=0U;		// sphere list
static unsigned cylinder=0U;	// cylinder list
static unsigned cube=0U;		// cube list

// texture id
static unsigned bgid=0U;

static bool		init=true;	// have we started yet?


void InitScene(OpenGL *gl,ControlData *cont)
{
	//
	//	Build the sphere
	//

	// first create the sphere list
	sphere=dlist.Start();
	// did that work?
	if(sphere==0U)
	{
		cont->quit=true;
		return;
	}

	// ask shapes to make us a sphere
	shapes.Sphere(1.0f,16);

	// end the list
	dlist.End();

	//
	//	Build the cylinder
	//

	// first create the cylinder list
	cylinder=dlist.Start();
	// did that work?
	if(cylinder==0U)
	{
		cont->quit=true;
		return;
	}

	// ask shapes to make us a cylinder
	shapes.Cylinder(0.8f,0.8f,1.0f,16);

	// end the list
	dlist.End();

	//
	//	Build the cube
	//

	// first create the cube list
	cube=dlist.Start();
	// did that work?
	if(cube==0U)
	{
		cont->quit=true;
		return;
	}

	// ask shapes to make us a cube
	shapes.Cube(1.0f,1.0f,1.0f);

	// end the list
	dlist.End();

	// switch on the basic light
	(cont->state)->SetLighting(true);
	(cont->state)->SetLight(0,true);

	// load up the reflection texture
	unsigned texid=tex.Load("data/reflect.jpg");

	// did that work?
	if(texid==0)
	{
		// show the message
		MessageBox(NULL,"Cannot load reflection texture.","NeHeGL Example Error",MB_ICONEXCLAMATION | MB_OK);
		// and say we need to quit
		cont->quit=true;
		return;
	}

	// setup environment mapping
	envmap.Setup(texid,&tex);

	// load the background texture
	bgid=tex.Load("data/bg.jpg");

	// did that work?
	if(bgid==0)
	{
		// show the message
		MessageBox(NULL,"Cannot load beckground texture.","NeHeGL Example Error",MB_ICONEXCLAMATION | MB_OK);
		// and say we need to quit
		cont->quit=true;
		return;
	}


	// say we've finished initializing
	init=false;
}

void DrawScene(OpenGL *gl,ControlData *cont)
{
	// initialize scene
	if(init)
	{
		InitScene(gl,cont);
		if(cont->quit)
			return;
	}

	// turn on environment mapping
	envmap.On();

	//
	//	Draw the sphere
	//

	// move to top left
	view.Reset();
	view.Translate(-1.5f,1.0f,-6.0f);
	// rotate
	view.Rotate(rot,0.0f,1.0f,0.0f);

	// red
	(cont->state)->SetColour(Colour(1.0f,0.0f,0.0f));

	// and draw the sphere
	dlist.Execute(sphere);

	//
	//	Draw the cylinder
	//

	// move to top right
	view.Reset();
	view.Translate(1.5f,1.0f,-6.0f);
	// rotate
	view.Rotate(rot,1.0f,0.0f,0.0f);

	// green
	(cont->state)->SetColour(Colour(0.0f,1.0f,0.0f));

	// and draw the cylinder
	dlist.Execute(cylinder);

	//
	//	Draw the cube
	//

	// move to bottom centre
	view.Reset();
	view.Translate(0.0f,-1.0f,-6.0f);
	// rotate
	view.Rotate(rot,1.0f,0.0f,1.0f);

	// blue
	(cont->state)->SetColour(Colour(0.0f,0.0f,1.0f));

	// and draw the cube
	dlist.Execute(cube);

	// rotate
	rot+=0.2f;

	// turn off environment mapping
	envmap.Off();

	// set colour back to white
	(cont->state)->SetColour(Colour(1.0f,1.0f,1.0f));


	// select background texture
	tex.Set(bgid);

	// draw background
	view.Reset();
	view.Translate(0.0f,0.0f,-20.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);	glVertex3f(-13.3f,-10.0f,10.0f);
	glTexCoord2f(1.0f,0.0f);	glVertex3f(13.3f,-10.0f,10.0f);
	glTexCoord2f(1.0f,1.0f);	glVertex3f(13.3f,10.0f,10.0f);
	glTexCoord2f(0.0f,1.0f);	glVertex3f(-13.3f,10.0f,10.0f);
	glEnd();

};