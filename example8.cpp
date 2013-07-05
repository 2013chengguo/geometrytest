// AVI example, based loosely on lesson 35

#include "opengl.h"
#include "view.h"
#include "texture.h"
#include "text.h"
#include "avi.h"


using namespace NeHe;

static bool				init=true;	// initialisation flag
static Texture			tex;		// the texture manager
static View				view;		// the view controller
static Text2D			txt;		// 2d text handler
static int				fps;		// the last fps
static int				fpsct=0;	// fps counter
static AVI				avi;		// avi class


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

	//
	//	Load the video
	//

	if(!avi.Load("data//face2.avi",true,&tex))
		cont->quit=true;
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
	cont->state->SetTexturing(true);

	
	// set the present texture to be the video
	avi.Set(cont->framelength);

	// draw the video
	view.Reset();
	glBegin(GL_QUADS);										// Begin Drawing The Background (One Quad)
		// Front Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 11.0f,  8.3f, -20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-11.0f,  8.3f, -20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-11.0f, -8.3f, -20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 11.0f, -8.3f, -20.0f);
	glEnd();												// Done Drawing The Background



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