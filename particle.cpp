/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Particle classes	

	Version History:
		v1.00	New file		(GA 7/11/02)

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

#include "particle.h"
#include "nexception.h"
#include <new>

namespace NeHe {

Particle :: Particle() : Billboard()
{
	// initialise variables
	active=true;
	life=fade=1.0f;
	grav=Vector(0.0f,0.0f,0.0f);
	dir=Vector(0.0f,1.0f,0.0f);
}

Particle :: Particle(const Particle &p) : Billboard(p)
{
	// copy everything
	active=p.active;
	life=p.life;
	fade=p.fade;
	dir=Vector(p.dir);
	grav=Vector(p.grav);
}

void Particle :: Update(float time)
{
	// anything to do?
	if(!active)
		return;

	// convert time to a multiplier
	float delta=time/1000.0f;

	// get deltas of vectors
	Vector gravdelta=grav*delta;
	Vector dirdelta=dir*delta;

	// update position of particle
	pos+=gravdelta;
	pos+=dirdelta;

	// fade alpha
	col.alpha-=delta*fade;

	// reduce life
	life-=(delta*fade);

	// is that it for this particle?
	if(life<=0.0f)
		active=false;
}

ParticleSystem :: ParticleSystem(StateManager *sman,Texture *texman) : BillboardGroup(sman,texman)
{
	// clear everything
	num=0;
	respawn=false;
	proc=NULL;
	particles=NULL;
}

ParticleSystem :: ~ParticleSystem()
{
	// delete the particles
	if(particles)
		delete [] particles;
}

void ParticleSystem :: Init(int numparts,ParticleSetup ps,bool resp)
{
	// save everything
	num=numparts;
	proc=ps;
	respawn=resp;

	// tidy up
	if(particles)
		delete [] particles;

	// allocate particles
	try {
		particles=new Particle [num];
	}
	catch(std :: bad_alloc) {
		NTHROW("ParticleSystem :: Init - Out of memory.");
	}

	// fill them and add them
	for(int ct=0; ct<num; ct++)
	{
		// set it up
		proc(&particles[ct]);
		// and add it
		bbs.push_back(particles[ct]);
	}
}

void ParticleSystem :: Update(float time)
{
	// run through the particles
	for(int ct=0; ct<num; ct++)
	{
		// update the particle
		particles[ct].Update(time);
		// does the particle still exist?
		if(!(particles[ct].IsActive()))
		{
			// create a new one as necessary
			if(respawn)
			{
				// set it up
				proc(&particles[ct]);
			}else
				bbs[ct].SetColour(BLACK);
		}
		// copy it over to the list
		bbs[ct]=particles[ct];
	}

}

}; // namespace NeHe