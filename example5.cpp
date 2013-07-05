// Example 5: Particle system (based on lesson 19)


#include "opengl.h"
#include "particle.h"
#include <stdlib.h>

using namespace NeHe;

#define NUM_PARTICLES	1000

float GetRandom()
{
	return (((float)(rand()%50))-26.0f)*10.0f;
}

float GetRandom01()
{
	return ((float)(rand()%100))/100.0f;
}

Vector GetRandomVector()
{
	return Vector(GetRandom()/250.0f,GetRandom()/250.0f,0.0f);
}

void SetupParticle(Particle *part)
{
	// set the emitter to the centre of the display
	part->SetPosition(Vector(0.0f,0.0f,-5.0f));
	// set the colour to a random one
	part->SetColour(Colour(GetRandom01(),GetRandom01(),GetRandom01()));
	// make the particle size fairly small
	part->SetSize(0.1f,0.1f);
	// setup the particle lifetime and forces
	part->Setup(1.0f,GetRandom01()*10.0f,GetRandomVector(),Vector(0.0f,-3.0f,0.0f));
}

static bool				init=true;	// initialisation flag
static ParticleSystem	*ps;		// the particle system
static Texture			tex;		// the texture manager

void InitScene(OpenGL *gl,ControlData *cont)
{
	// load the particle texture
	int texid=tex.Load("data/particle.bmp");

	// did that work?
	if(texid==0)
	{
		// show the message
		MessageBox(NULL,"Cannot load particle texture.","NeHeGL Example Error",MB_ICONEXCLAMATION | MB_OK);
		// and say we need to quit
		cont->quit=true;
		return;
	}

	// allocate the particle system
	ps=new ParticleSystem(cont->state,&tex);

	// set the texture
	ps->SetTexture(texid);

	// and initialise the particle system
	ps->Init(NUM_PARTICLES,SetupParticle,true);

	// say we've finished initializing
	init=false;
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
	}

	// draw particles
	ps->Draw();

	// and update for next frame
	ps->Update(cont->framelength);
}