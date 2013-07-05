// example 9 - Display lists and shapes

#include "opengl.h"
#include "view.h"
#include "displaylist.h"
#include "shape.h"

using namespace NeHe;

// the view class
static View				view;
static Shapes			shapes;	// shapes class
static DisplayList		dlist;	// displaylist manager

// rotation
static float rot=0.0f;		// sphere rotation

// display lists
static unsigned sphere=0U;		// sphere list
static unsigned cylinder=0U;	// cylinder list
static unsigned cube=0U;		// cube list

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
	view.Rotate(rot,0.0f,0.0f,1.0f);

	// blue
	(cont->state)->SetColour(Colour(0.0f,0.0f,1.0f));

	// and draw the cube
	dlist.Execute(cube);

	// rotate
	rot+=0.2f;
};