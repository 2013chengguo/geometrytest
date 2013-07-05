// Example taken from Tutorial 13

#include "opengl.h"
#include "view.h"
#include "text.h"
#include <cmath>

using namespace NeHe;

static bool		init=true;;	// have we started yet?
static Text3D	txt;		// 2d text handler
static View		view;		// view class
static float 	rot;		// Used To Rotate The Text


void InitScene(OpenGL *gl,ControlData *cont)
{
	// 
	//	Setup font
	//

	// choose a font
	TextType ttype;
	ttype.name="Comic Sans MS";
	ttype.size=12;
	ttype.bold=true;
	ttype.italic=false;
	ttype.underline=false;
	ttype.depth=0.2f;
	// setup the text handler
	txt.Setup(gl,ttype);

	// setup lighting
	(cont->state)->SetLighting(true);
	(cont->state)->SetLight(0,true);

	// say we've finished initializing
	init=false;
}

void DrawScene(OpenGL *gl,ControlData *cont)
{
	// initialize scene
	if(init)
		InitScene(gl,cont);

	view.Translate(0.0f,0.0f,-10.0f);						// Move One Unit Into The Screen
	view.Rotate(rot,1.0f,0.0f,0.0f);						// Rotate On The X Axis
	view.Rotate(rot*1.5f,0.0f,1.0f,0.0f);					// Rotate On The Y Axis
	view.Rotate(rot*1.4f,0.0f,0.0f,1.0f);					// Rotate On The Z Axis
	// Pulsing Colors Based On The Rotation
	(cont->state)->SetColour(Colour(1.0f*float(cos(rot/20.0f)),
									1.0f*float(sin(rot/25.0f)),
									1.0f-0.5f*float(cos(rot/17.0f))));
	view.Translate(-6*txt.GetAverageWidth(),0.0f,0.0f);
 	txt << "NeHe - " << (rot/50.0f);					// Print GL Text To The Screen
	rot+=0.5f;											// Increase The Rotation Variable
}