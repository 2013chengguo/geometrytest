/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Model class

	Version History:
		v1.00	New file				(GA 21/11/02)
		v1.01	Added Access function 
					and proxies			(GA 05/01/02)

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

#include "model.h"
#include <new>
#include <cmath>
#include "nexception.h"

namespace NeHe {

Model :: Model()
{
	// clear everything
	numframes=0;
	fps=0.0f;
	frames=NULL;
}

Model :: Model(const Model &model)
{
	// copy over the model
	if(!this->Setup(model.numframes,model.fps))
		throw std :: bad_alloc();
	// add the frames
	for(int ct=0; ct<numframes; ct++)
		this->Add(ct,model.frames[ct]);
}

Model :: ~Model()
{
	// just clear the model
	this->Clear();
}

void Model :: Clear()
{
	// delete the frames
	if(frames)
	{
		delete [] frames;
		frames=NULL;
	}

	// and clear anything else
	numframes=0;
	fps=0.0f;
}

bool Model :: Setup(int num,float framesec)
{
	// allocate the frames
	try {
		frames=new ModelFrame [num];
	}
	// did that work?
	catch(std :: bad_alloc) {
		return false;
	};

	// copy over the values
	numframes=num;
	fps=framesec;

	// success
	return true;
}

void Model :: Add(int num,ModelFrame &frame)
{
	// is the value in range?
	if(num<0 || num>=numframes)
		NTHROW("Model :: Add - Frame index out of range.");

	// do we have an array?
	if(!frames)
		NTHROW("Model :: Add - No frame passed.");

	// copy it over
	frames[num]=ModelFrame(frame);
}

bool Model :: Load(std :: string name,ModelLoader &loader,Texture &tex)
{
	// attempt to load the file
	if(!loader.Load(name))
		return false;

	// do we have to clean up?
	if(frames)
		this->Clear();

	// get the model information
	int		modelframes=loader.GetNumFrames();
	int		nummeshes=loader.GetNumMeshes();
	float	modelfps=(float)loader.GetFPS();

	// tidy up the values
	if(modelframes<=0)
		return false;
	if(modelfps==0.0f)
		modelfps=1.0f;

	// and setup our model
	if(!this->Setup(modelframes,modelfps))
		return false;
	
	// now set up the frames
	for(int ct=0; ct<numframes; ct++)
	{
		// setup the frame
		if(!frames[ct].Setup(nummeshes))
		{
			this->Clear();
			return false;
		}
		
		// add the meshes
		for(int ct2=0; ct2<nummeshes; ct2++)
		{
			Mesh msh;	// temporary mesh
			
			// get the mesh
			if(!loader.Get(ct,ct2,&tex,&msh))
				return false;

			// and set it to the frame
			frames[ct].SetMesh(ct2,msh);
		}
	}

	// success
	return true;
}

void Model :: DrawFrame(int frame)
{
	// is the frame in range
	if(frame<0 || frame>=numframes)
		NTHROW("Model :: DrawFrame - frame number out of range.");

	// do we have anything to draw?
	if(!frames)
		NTHROW("Model :: DrawFrame - No frames loaded.");

	// draw the frame
	frames[frame].Draw();
}

void Model :: DrawTimeFrame(int startframe,float time)
{
	// do we have a sensible time value?
	if(time<0.0f)
		NTHROW("Model :: DrawTimeFrame - Negative time value.");

	// tidy up the start frame
	if(startframe<=0)
		startframe=0;
	// and the last
	if(startframe>=(numframes-1))
	{
		// draw the last frame
		this->DrawFrame(numframes-1);
		// and return
		return;
	}

	// work out nearest frames
	float frame=(float)startframe+(fps*time);

	// first frame
	int first=(int)floor((double)frame);
	// second frame
	int second=first+1;

	// are we at the end?
	if(first>=(numframes-1))
	{
		// draw the last frame
		this->DrawFrame(numframes-1);
		// and return
		return;
	}

	// create an intermediate frame
	ModelFrame inter;
	inter.SetInterp(frames[first],frames[second],(frame-(float)first));

	// and draw it
	inter.Draw();
}

};	// namespace NeHe