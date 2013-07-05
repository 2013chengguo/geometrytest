/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Mesh class	

	Version History:
		v1.00	New class	(GA 21/11/02)
		v1.01	Fixed material support	(GA 23/11/02)
		v1.02	Added Clear command		(GA 23/11/02)
		v1.03	Made STL safe			(GA 05/01/03)

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

#ifndef MESH_H
#define MESH_H

#include "opengl.h"
#include "material.h"
#include "texture.h"

namespace NeHe {

// vertex structure
typedef struct Vertex {
				float x,y,z;		// position of vertex
				float nx,ny,nz;		// normal of vertex
				float u,v;			// texture coords
			}Vertex;

// triangle structure
typedef	unsigned int Triangle[3];	

class Mesh {
	int			numvertices;	// number of vertices
	int			numtriangles;	// number of triangles
	Vertex		*vlist;			// vertex list
	Triangle	*tlist;			// triangle list
	Material	mat;			// material for mesh
	Texture		*tman;			// texture manager
	unsigned	tex;			// texture for mesh
	// Zero the mesh
	void Zero();
	// copy over another mesh
	void Copy(const Mesh &msh);
public:
	Mesh();
	Mesh(Texture *texman);
	Mesh(const Mesh &msh);
	~Mesh();
	Mesh &operator = (const Mesh &msh);

	//
	//	Setup functions
	//
	// The number of vertices and triangles
	bool Setup(int nverts,int ntris);
	// Set all vertices
	void SetVertices(Vertex *verts);
	// Set all triangles
	void SetTriangles(Triangle *tris);
	// Set specific vertex
	void SetVertex(int pos,Vertex vert);
	// Set specific triangle
	void SetTriangle(int pos,Triangle tri);
	
	// Set Material
	void SetMaterial(const Material matl)	{ mat=Material(matl); };
	// Set texture
	void SetTexture(unsigned texid)			{ tex=texid; };
	// Set texture manager
	void SetTextureManager(Texture *texm)	{ tman=texm; };

	// Setup as an interpolation between two other meshes (0.0f-1.0f)
	void SetInterp(const Mesh &m1,const Mesh &m2,float pos);

	// clear mesh
	void Clear();

	//
	//	Draw command
	//
	void Draw();

	//
	//	Data access functions
	//
	int			GetNumVertices() const	{ return numvertices; };
	int			GetNumTriangles() const	{ return numtriangles; };
	Vertex		*GetVertices() const	{ return vlist; };
	Triangle	*GetTriangles() const	{ return tlist; };
};

};	// namespace NeHe

// simple proxy
#define NMESH NeHe :: Mesh

#endif