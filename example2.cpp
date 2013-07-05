// Example taken from Tutorial 13

#include "opengl.h"
#include "view.h"
#include "text.h"
#include <cmath>

using namespace NeHe;

static bool		init=true;;	// have we started yet?
static Text2D	txt;		// 2d text handler
static View		view;		// view class
static float	cnt1;		// 1st Counter Used To Move Text & For Coloring
static float	cnt2;		// 2nd Counter Used To Move Text & For Coloring


void InitScene(OpenGL *gl)
{
	// choose a font
	TextType ttype;
	ttype.name="Courier New";
	ttype.size=24;
	ttype.bold=true;
	ttype.italic=false;
	ttype.underline=false;
	// setup the text handler
	txt.Setup(gl,ttype);
	// say we've finished initializing
	init=false;
}

void DrawScene(OpenGL *gl,ControlData *cont)
{
	// initialize scene
	if(init)
		InitScene(gl);

	view.Translate(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
	// Pulsing Colors Based On Text Position
	(cont->state)->SetColour(Colour(1.0f*float(cos(cnt1)),
								1.0f*float(sin(cnt2)),
								1.0f-0.5f*float(cos(cnt1+cnt2))));
	// Position The Text On The Screen
	view.Pos2D(-0.45f+0.05f*float(cos(cnt1)), 0.32f*float(sin(cnt2)));
 
	// and draw it
	txt << "Active OpenGL Text With NeHe - " << cnt1;

	cnt1+=0.051f;										// Increase The First Counter
	cnt2+=0.005f;										// Increase The First Counter

}