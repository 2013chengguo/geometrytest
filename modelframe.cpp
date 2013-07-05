/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Model Frame class

	Version History:
		v1.00	New class				(GA 23/11/02)
		v1.01	Made STL safe,
				Added access function, 
					proxies				(GA 05/01/03)


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


#include "modelframe.h"
#include "nexception.h"
#include <new>

namespace NeHe {

ModelFrame:: ModelFrame()
{
	// clear all
	this->Zero();
}

ModelFrame :: ModelFrame(const ModelFrame &mf)
{
	// clear all
	this->Zero();

	// copy using setup commands
	if(!this->Setup(mf.nummeshes))
		throw std :: bad_alloc();
	// copy over the meshes
	this->SetMeshes(mf.meshes);
}

ModelFrame :: ~ModelFrame()
{
	// just use clear command
	this->Clear();
}

ModelFrame &ModelFrame :: operator =(const ModelFrame &mf)
{
	// copy over the modelframe
	this->Copy(mf);

	// and return it
	return *this;
}

void ModelFrame :: Zero()
{
	// clear all
	nummeshes=0;
	meshes=NULL;
}

void ModelFrame :: Copy(const ModelFrame &mf)
{
	// any tidying to do?
	if(meshes)
		this->Clear();

	// setup the model frame
	if(!this->Setup(mf.nummeshes))
		NTHROW("ModelFrame :: Copy - Cannot setup model (Out of memory).");

	// copy over the meshes
	this->SetMeshes(mf.meshes);
}

void ModelFrame :: Clear()
{
	// anything to do?
	if(meshes)
	{
		delete [] meshes;
		meshes=NULL;
	}

	// reset number of meshes
	nummeshes=0;
}

bool ModelFrame :: Setup(int num)
{
	// do we have a sensible number of meshes?
	if(num<=0)
		return false;

	// any tidying up to do?
	if(meshes)
		this->Clear();

	// try to allocate the mesh array
	try {
		meshes=new Mesh [num];
	}
	catch(std :: bad_alloc) {
		// out of memory
		return false;
	}

	// set the number of meshes
	nummeshes=num;

	// success
	return true;
}

void ModelFrame :: SetMeshes(const Mesh *mesh)
{
	// anything to do?
	if(nummeshes<=0 || !meshes)
		NTHROW("ModelFrame :: SetMeshes - No meshes allocated.");
	if(!mesh)
		NTHROW("ModelFrame :: SetMeshes - NULL pointer passed.");

	// copy over the meshes
	for(int ct=0; ct<nummeshes; ct++)
		meshes[ct]=mesh[ct];
}

void ModelFrame :: SetMesh(int num,const Mesh &mesh)
{
	// anything to do?
	if(nummeshes<=0 || !meshes)
		NTHROW("ModelFrame :: SetMesh - No meshes allocated.");	
	if(num<0 || num>=nummeshes)
		NTHROW("ModelFrame :: SetMesh - Index out of range.");

	// copy over the mesh
	meshes[num]=mesh;
}

void ModelFrame :: SetInterp(const ModelFrame &mf1,const ModelFrame &mf2,float pos)
{
	// tidy up if necessary
	if(meshes)
		this->Clear();

	// lock pos to 0.0-1.0 range
	if(pos<0.0f)
		pos=0.0f;
	if(pos>1.0f)
		pos=1.0f;

	// set up as modelframe one
	this->Setup(mf1.nummeshes);

	// go throught the meshes, interpolating each
	for(int ct=0; ct<nummeshes; ct++)
		meshes[ct].SetInterp(mf1.meshes[ct],mf2.meshes[ct],pos);

}

void ModelFrame :: Draw()
{
	// anything to do
	if(nummeshes==0 || !meshes)
		NTHROW("ModelFrame :: Draw - No meshes allocated.");	

	// run through the meshes, drawing each
	for(int ct=0; ct<nummeshes; ct++)
		meshes[ct].Draw();
}

};	// namespace NeHe