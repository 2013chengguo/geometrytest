// Example 6: Multitexturing example

#include "opengl.h"
#include "view.h"
#include "text.h"
#include "texture.h"
#include "advtexture.h"

using namespace NeHe;

static View				view;			// the view class
static bool				init=true;;		// have we started yet?
static Text2D			txt;			// 2d text handler
static int				fps;			// the last fps
static int				fpsct=0;		// fps counter
static GLfloat			rquad;			// Angle For The Quad 
static Texture			tman;			// texture manager
static AdvancedTexture *atex;			// advanced texture manager
static unsigned			tex1,tex2;		// texture ids
static bool				multitexavail;	// availability of multitexturing hardware
static bool				multitex=true;	// multitexturing flag

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
	//	Load the textures
	//

	// load base texture
	tex1=tman.Load("data\\base.bmp");
	// did that work?
	if(tex1==0)
	{
		// no
		MessageBox(NULL,"Cannot load ./data/base.bmp","Error",MB_OK | MB_ICONEXCLAMATION);
		cont->quit=true;		// say we're quitting
		return;
	}
	
	// load lightmap texture
	tex2=tman.Load("data\\particle.bmp");
	// did that work?
	if(tex2==0)
	{
		// no
		MessageBox(NULL,"Cannot load ./data/particle.bmp","Error",MB_OK | MB_ICONEXCLAMATION);
		cont->quit=true;		// say we're quitting
		return;
	}

	//
	//	Setup the advanced texture manager
	//

	// allocate the texture manager
	atex=new AdvancedTexture(cont->state);
	// check multitexturing available
	multitexavail=atex->HasMultiTexturing();

	// say we've finished initializing
	init=false;
}


void DrawScene(OpenGL *gl,ControlData *cont)
{
	// initialize scene
	if(init)
	{
		InitScene(gl,cont);
		// have we been asked to quit?
		if(cont->quit)
			return;
	}


	view.Reset();										// Reset The Current Modelview Matrix
	view.Translate(0.0f,0.0f,-7.0f);					// Move Right 1.5 Units And Into The Screen 7.0
	view.Rotate(rquad,1.0f,1.0f,1.0f);					// Rotate The Quad On The X axis 

	// render by multitexture capability
	if(multitexavail && multitex)
	{
		// bind first texture
		atex->Set(0);
		glEnable(GL_TEXTURE_2D);
		tman.Set(tex1);
		// bind second texture
		atex->Set(1);
		glEnable(GL_TEXTURE_2D);
		tman.Set(tex2);

		glBegin(GL_QUADS);								// Draw A Quad
			(cont->state)->SetColour(WHITE);			// Set The Color To White
			atex->TexCoords(0,0.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f, 1.0f,-1.0f);				// Top Right Of The Quad (Top)
			atex->TexCoords(0,1.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f, 1.0f,-1.0f);				// Top Left Of The Quad (Top)
			atex->TexCoords(0,1.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f, 1.0f, 1.0f);				// Bottom Left Of The Quad (Top)
			atex->TexCoords(0,0.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f, 1.0f, 1.0f);				// Bottom Right Of The Quad (Top)

			atex->TexCoords(0,0.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f,-1.0f, 1.0f);				// Top Right Of The Quad (Bottom)
			atex->TexCoords(0,1.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f,-1.0f, 1.0f);				// Top Left Of The Quad (Bottom)
			atex->TexCoords(0,1.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f,-1.0f,-1.0f);				// Bottom Left Of The Quad (Bottom)
			atex->TexCoords(0,0.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f,-1.0f,-1.0f);				// Bottom Right Of The Quad (Bottom)

			atex->TexCoords(0,0.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f, 1.0f, 1.0f);				// Top Right Of The Quad (Front)
			atex->TexCoords(0,1.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f, 1.0f, 1.0f);				// Top Left Of The Quad (Front)
			atex->TexCoords(0,1.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f,-1.0f, 1.0f);				// Bottom Left Of The Quad (Front)
			atex->TexCoords(0,0.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f,-1.0f, 1.0f);				// Bottom Right Of The Quad (Front)

			atex->TexCoords(0,0.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f,-1.0f,-1.0f);				// Top Right Of The Quad (Back)
			atex->TexCoords(0,1.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f,-1.0f,-1.0f);				// Top Left Of The Quad (Back)
			atex->TexCoords(0,1.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f, 1.0f,-1.0f);				// Bottom Left Of The Quad (Back)
			atex->TexCoords(0,0.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f, 1.0f,-1.0f);				// Bottom Right Of The Quad (Back)

			atex->TexCoords(0,0.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f, 1.0f, 1.0f);				// Top Right Of The Quad (Left)
			atex->TexCoords(0,1.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f, 1.0f,-1.0f);				// Top Left Of The Quad (Left)
			atex->TexCoords(0,1.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f,-1.0f,-1.0f);				// Bottom Left Of The Quad (Left)
			atex->TexCoords(0,0.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f(-1.0f,-1.0f, 1.0f);				// Bottom Right Of The Quad (Left)

			atex->TexCoords(0,0.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f, 1.0f,-1.0f);				// Top Right Of The Quad (Right)
			atex->TexCoords(0,1.0f,0.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,0.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f, 1.0f, 1.0f);				// Top Left Of The Quad (Right)
			atex->TexCoords(0,1.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,1.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f,-1.0f, 1.0f);				// Bottom Left Of The Quad (Right)
			atex->TexCoords(0,0.0f,1.0f);				// set 1st set of tex coords
			atex->TexCoords(1,0.0f,1.0f);				// set 2nd set of tex coords
			glVertex3f( 1.0f,-1.0f,-1.0f);				// Bottom Right Of The Quad (Right)
		glEnd();										// Done Drawing The Quad

		// switch off texturing
		atex->Set(1);
		glDisable(GL_TEXTURE_2D);
		atex->Set(0);
		glDisable(GL_TEXTURE_2D);

	}else{

		//
		//	First Pass
		//

		// set first texture
		tman.Set(tex1);
		glEnable(GL_TEXTURE_2D);

		// Draw first pass
		glBegin(GL_QUADS);								// Draw A Quad
		(cont->state)->SetColour(WHITE);				// Set The Color To White

		glTexCoord2f(0.0f,0.0f);						// Texture coords
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
		glTexCoord2f(1.0f,0.0f);						// Texture coords
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
		glTexCoord2f(1.0f,1.0f);						// Texture coords
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
		glTexCoord2f(0.0f,1.0f);						// Texture coords
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)

		glTexCoord2f(0.0f,0.0f);						// Texture coords
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
		glTexCoord2f(1.0f,0.0f);						// Texture coords
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
		glTexCoord2f(1.0f,1.0f);						// Texture coords
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
		glTexCoord2f(0.0f,1.0f);						// Texture coords
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)

		glTexCoord2f(0.0f,0.0f);						// Texture coords
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
		glTexCoord2f(1.0f,0.0f);						// Texture coords
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
		glTexCoord2f(1.0f,1.0f);						// Texture coords
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
		glTexCoord2f(0.0f,1.0f);						// Texture coords
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)

		glTexCoord2f(0.0f,0.0f);						// Texture coords
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
		glTexCoord2f(1.0f,0.0f);						// Texture coords
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
		glTexCoord2f(1.0f,1.0f);						// Texture coords
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
		glTexCoord2f(0.0f,1.0f);						// Texture coords
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)

		glTexCoord2f(0.0f,0.0f);						// Texture coords
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
		glTexCoord2f(1.0f,0.0f);						// Texture coords
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
		glTexCoord2f(1.0f,1.0f);						// Texture coords
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
		glTexCoord2f(0.0f,1.0f);						// Texture coords
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)

		glTexCoord2f(0.0f,0.0f);						// Texture coords
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
		glTexCoord2f(1.0f,0.0f);						// Texture coords
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
		glTexCoord2f(1.0f,1.0f);						// Texture coords
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
		glTexCoord2f(0.0f,1.0f);						// Texture coords
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
		glEnd();										// Done Drawing The Quad

		// run if multitexturing on
		if(multitex)
		{
			//
			//	Second Pass
			//

			// set second texture
			tman.Set(tex2);

			// setup texturing for second pass
			atex->MultiTexOn();

			// Draw second pass
			glBegin(GL_QUADS);								// Draw A Quad
			(cont->state)->SetColour(WHITE);				// Set The Color To White

			glTexCoord2f(0.0f,0.0f);						// Texture coords
			glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
			glTexCoord2f(1.0f,0.0f);						// Texture coords
			glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
			glTexCoord2f(1.0f,1.0f);						// Texture coords
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
			glTexCoord2f(0.0f,1.0f);						// Texture coords
			glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)

			glTexCoord2f(0.0f,0.0f);						// Texture coords
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
			glTexCoord2f(1.0f,0.0f);						// Texture coords
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
			glTexCoord2f(1.0f,1.0f);						// Texture coords
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
			glTexCoord2f(0.0f,1.0f);						// Texture coords
			glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)

			glTexCoord2f(0.0f,0.0f);						// Texture coords
			glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
			glTexCoord2f(1.0f,0.0f);						// Texture coords
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
			glTexCoord2f(1.0f,1.0f);						// Texture coords
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
			glTexCoord2f(0.0f,1.0f);						// Texture coords
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)

			glTexCoord2f(0.0f,0.0f);						// Texture coords
			glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
			glTexCoord2f(1.0f,0.0f);						// Texture coords
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
			glTexCoord2f(1.0f,1.0f);						// Texture coords
			glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
			glTexCoord2f(0.0f,1.0f);						// Texture coords
			glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)

			glTexCoord2f(0.0f,0.0f);						// Texture coords
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
			glTexCoord2f(1.0f,0.0f);						// Texture coords
			glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
			glTexCoord2f(1.0f,1.0f);						// Texture coords
			glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
			glTexCoord2f(0.0f,1.0f);						// Texture coords
			glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)

			glTexCoord2f(0.0f,0.0f);						// Texture coords
			glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
			glTexCoord2f(1.0f,0.0f);						// Texture coords
			glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
			glTexCoord2f(1.0f,1.0f);						// Texture coords
			glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
			glTexCoord2f(0.0f,1.0f);						// Texture coords
			glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
			glEnd();										// Done Drawing The Quad

			// switch off second pass multitexturing
			atex->MultiTexOff();
		}
		// and switch off texturing
		glDisable(GL_TEXTURE_2D);
	}

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
	view.Reset();										// reset the view
	view.Translate(0.0f,0.0f,-0.11f);					// translate back a little
	view.Pos2D(-0.055f,0.035f);							// move to top left
	txt << "FPS:" << fps;								// and write out the FPS
	// write out the hardware multitexturing state
	view.Reset();										// reset the view
	view.Translate(0.0f,0.0f,-0.11f);					// translate back a little
	view.Pos2D(-0.055f,0.032f);							// move to top left
	if(multitexavail)
		txt << "Multitexturing hardware support.";
	else
		txt << "Multitexturing emulated.";


	// test to see if user wants multitexturing on
	if(cont->keys['m'] || cont->keys['M'])
		multitex=!multitex;

	// write out the state of multitexturing
	view.Reset();										// reset the view
	view.Translate(0.0f,0.0f,-0.11f);					// translate back a little
	view.Pos2D(-0.055f,0.030f);							// move to top left
	txt << "Multitexturing " << (multitex ? "on.":"off.");


	rquad-=0.15f;										// Decrease The Rotation Variable For The Quad ( NEW )
}


