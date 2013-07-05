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

#ifndef MODELFRAME_H
#define MODELFRAME_H

#include "mesh.h"

namespace NeHe {

class ModelFrame {
	int		nummeshes;	// the number of meshes
	Mesh	*meshes;	// the meshes themselves
	// helper functions
	void Zero();
	void Copy(const ModelFrame &mf);
public:
	ModelFrame();
	ModelFrame(const ModelFrame &mf);
	~ModelFrame();
	ModelFrame &operator =(const ModelFrame &mf);


	// Setup with the number of meshes
	bool Setup(int num);

	// set the meshes	(note: makes internal copy)
	void SetMeshes(const Mesh *mesh);
	// set a single mesh
	void SetMesh(int num,const Mesh &mesh);

	// setup as interpolation between two other frames (0.0f-1.0f)
	void SetInterp(const ModelFrame &mf1,const ModelFrame &mf2,float pos);

	// clear the frame
	void Clear();

	// Draw the frame
	void Draw();

	// Get the mesh
	Mesh *GetMesh(int index)	{ if(index<0 || index>=nummeshes) return &meshes[index]; else return NULL; };
};

};	// namespace NeHe

// simple proxy
#define NFRAME	NeHe :: ModelFrame

#endif