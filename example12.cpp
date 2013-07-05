// Scissor test and 2d example

#include <stdlib.h>
#include "opengl.h"
#include "view.h"
#include "displaylist.h"
#include "shape.h"
#include "text.h"
#include "scissorbox.h"

using namespace NeHe;

// NeHeGL objects
static View				view;
static Shapes			shapes;		// shapes class
static DisplayList		dlist;		// displaylist manager
static ScissorBox		box1,box2;	// scissor boxes
static Text2D			text;		// text object

static bool		init=true;		// have we started yet?
static unsigned cube=0U;		// cube display list
static float	rot=0.0f;		// cube rotation
static int		width,height;	// dimensions of the display
static int		widdiv3;		// width divided by 3
static int		hitdiv3;		// height divided by 3
static int		widdiv8;		// width divided by 8
static int		box2x;			// x position of start of box 2

float Random()
{
	return ((float)(rand()%1000))/1000.0f;
}

void InitScene(OpenGL *gl,ControlData *cont)
{

	//
	//	Setup text
	//

	// choose a font
	TextType ttype;
	ttype.name="Courier New";
	ttype.size=24;
	ttype.bold=true;
	ttype.italic=false;
	ttype.underline=false;
	// setup the text handler
	text.Setup(gl,ttype);

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

	//
	// set up lighting
	//

	// switch on the basic light
	(cont->state)->SetLighting(true);
	(cont->state)->SetLight(0,true);

	//
	// set up scissor boxes
	//

	// get the dimensions
	(gl->GetWindow())->GetDimensions(width,height);

	// calculate the commons
	widdiv3=width/3;
	hitdiv3=height/3;
	widdiv8=width/8;
	box2x=((widdiv8)*7)-widdiv3;

	// build the boxes
	box1.Setup(widdiv8+1,hitdiv3+1,widdiv3-1,hitdiv3-1);
	box2.Setup(box2x+1,hitdiv3+1,widdiv3-1,hitdiv3-1);


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

	// switch off lights
	(cont->state)->SetLighting(false);
	(cont->state)->SetColour(Colour(1.0f,1.0f,1.0f));

	// write title
	view.Reset();
	view.Translate(0.0f,0.0f,-0.11f);
	view.Pos2D(-0.04f,0.035f);
	text.Write("Scissor test and 2D example");

	// switch into 2d
	(cont->state)->Set2DMode();

	// draw the window extents
	glBegin(GL_LINE_LOOP);
	glVertex2i(widdiv8,hitdiv3);
	glVertex2i(widdiv8+widdiv3,hitdiv3);
	glVertex2i(widdiv8+widdiv3,hitdiv3*2);
	glVertex2i(widdiv8,hitdiv3*2);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2i(box2x,hitdiv3);
	glVertex2i(box2x+widdiv3,hitdiv3);
	glVertex2i(box2x+widdiv3,hitdiv3*2);
	glVertex2i(box2x,hitdiv3*2);
	glEnd();

	// switch on scissor box 1
	box1.On();

	// draw lots of lines
	for(int ct=0; ct<1000; ct++)
	{
		// set a random colour
		(cont->state)->SetColour(Colour(Random(),Random(),Random()));

		// draw the line
		glBegin(GL_LINES);
		glVertex2i(rand()%width,rand()%height);
		glVertex2i(rand()%width,rand()%height);
		glEnd();

	}

	// turn off this box
	box1.Off();

	// switch on scissor box 2
	box2.On();

	// switch back to 3d
	(cont->state)->Set3DMode();

	// turn back on lighting
	(cont->state)->SetLighting(true);

	// make a green cube
	(cont->state)->SetColour(Colour(0.0f,1.0f,0.0f));

	// move it right of centre
	view.Translate(1.0f,0.0f,-5.0f);

	// rotate the cube
	view.Rotate(rot,1.0f,0.0f,1.0f);

	// and draw it
	dlist.Execute(cube);

	// turn off this box
	box2.Off();


	// increment the rotation
	rot+=0.2f;
}