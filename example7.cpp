// example 7: model loading

#include "opengl.h"
#include "view.h"
#include "texture.h"
#include "text.h"
#include "md2.h"
#include "milkshape.h"
#include "model.h"

using namespace NeHe;

static bool				init=true;	// initialisation flag
static Texture			tex;		// the texture manager
static Model			model;		// the marine model
static Model			jeep;		// the jeep model
static MD2				md2;		// the MD2 model loader
static Milkshape		milk;		// milkshape model loader
static int				numframes;	// number of frames
static int				frame=0;	// present frame
static float			framepos=0;	// frame position
static View				view;		// the view controller
static Text2D			txt;		// 2d text handler
static int				fps;		// the last fps
static int				fpsct=0;	// fps counter


void InitScene(OpenGL *gl,ControlData *cont)
{
	//
	//	Setup the font
	//

	// choose a font
	TextType ttype;
	ttype.name="Courier New";
	ttype.size=12;
	ttype.bold=true;
	ttype.italic=false;
	ttype.underline=false;
	// setup the text handler
	txt.Setup(gl,ttype);

	// load the model file
	if(!model.Load("data\\tris.md2",md2,tex))
		cont->quit=true;

	// load the jeep
	if(!jeep.Load("data\\jeep.ms3d",milk,tex))
		cont->quit=true;

	// get the number of frames
	numframes=model.GetNumFrames();
}

void DrawScene(OpenGL *gl,ControlData *cont)
{
	// initialize scene
	if(init)
	{
		InitScene(gl,cont);
		// do we need to quit?
		if(cont->quit)
			return;
		init=false;
	}

	// draw the model
	view.Translate(0.0f,0.0f,-50.0f);
	view.Scale(0.5f,0.5f,0.5f);
	model.DrawTimeFrame(0,framepos);
	framepos+=cont->framelength*0.012f;
	if(framepos>=198.0f)
		framepos=0.0f;


	// draw the jeep
	view.Reset();
	view.Translate(0.0,0.0,-100.0f);
	view.Scale(0.2f,0.2f,0.2f);
	(cont->state)->SetLighting(true);
	(cont->state)->SetLight(0,true);
	(cont->state)->SetTexturing(false);
	jeep.DrawFrame(0);
	(cont->state)->SetTexturing(true);
	(cont->state)->SetLighting(false);

	// do we need to restart the animation?
	if(frame>=numframes)
		frame=0;

	// check fps?
	if(fpsct==0)
	{
		fps=cont->fps;
		fpsct++;
	}else if(fpsct>=cont->fps)
		fpsct=0;
	else
		fpsct++;

	// write out the FPS
	(cont->state)->SetColour(Colour(1.0f,1.0f,1.0f));	// Write out in white
	cont->state->SetTexturing(false);
	view.Reset();										// reset the view
	view.Translate(0.0f,0.0f,-0.11f);					// translate back a little
	view.Pos2D(-0.055f,0.035f);							// move to top left
	txt << "FPS:" << fps;								// and write out the FPS
	cont->state->SetTexturing(true);


}