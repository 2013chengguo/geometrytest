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

#ifndef PARTICLE_H
#define PARTICLE_H

#include "billboard.h"

namespace NeHe {

class Particle : public Billboard {
	bool	active;		// is the particle active
	float	life;		// the life remaining to the particle
	float	fade;		// fade speed
	Vector	dir;		// direction of particle emission
	Vector	grav;		// direction of gravity
public:
	Particle();
	Particle(const Particle &p);
	//
	//	Setup procedure
	//
	// Main setup function
	void Setup(float plife,float pfade,Vector pdir,Vector pgrav)
		{ life=plife; fade=pfade; dir=Vector(pdir); grav=Vector(pgrav); active=true; };

	//
	//	Update procedure
	//
	// Update by time, in milliseconds
	void Update(float time);

	//
	//	Data Access functions
	//

	// is it active?
	bool IsActive()			{ return active; };
	// get life remaining
	float GetLife()			{ return life; };
	// get fade speed
	float GetFade()			{ return fade; };
	// get direction of particle emission
	Vector GetDirection()	{ return Vector(dir); };
	// get direction of gravity
	Vector GetGravity()		{ return Vector(grav); };
};

typedef void (*ParticleSetup)(Particle *p);

class ParticleSystem : public BillboardGroup {
	int				num;		// number of particles
	bool			respawn;	// whether the particles respawn after death
	ParticleSetup	proc;		// setup procedure
	Particle		*particles;	// the particle array
public:
	ParticleSystem(StateManager *sman,Texture *texman);
	virtual ~ParticleSystem();

	//
	//	Initialisation procedure
	//

	// Initialiser
	//	Params:
	//		numparts:	number of particles
	//		ps:			the particle setup procedure
	//		resp;		whether the particles should respawn (true for yes)
	void Init(int numparts,ParticleSetup ps,bool resp);

	//
	//	Update Particles
	//
	void Update(float time);
};

}; // namespace NeHe

#endif