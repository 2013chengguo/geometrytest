/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Model Loader base class

	Version History:
		v1.00	New file		(GA 22/11/02)

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

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "texture.h"
#include "mesh.h"
#include <string>

namespace NeHe {

class ModelLoader {
public:
	virtual ~ModelLoader()	{};
	// load a ms3d file
	virtual bool Load(const std :: string file)=0;
	// clear the file
	virtual void Clear()=0;

	// Get a mesh from a specific frame
	virtual bool Get(int frameno,int meshno,Texture *tex,Mesh *mesh) =0;

	//
	//	Data Access routines
	//
	// Get the number of meshes in a frame
	virtual int GetNumMeshes() const	=0;
	// Get the number of frames
	virtual int GetNumFrames() const	=0;
	// Get the number of frames per second
	virtual int GetFPS() const			=0;
};

};	// namespace NeHe

#endif