/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Milkshape class	

	Version History:
		v2.00	Completely rewritten	(GA 09/12/02)

	Notes:
		Based, copy and pasted from original class by Brett Porter

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

#ifndef MILKSHAPE_H
#define MILKSHAPE_H

#include "opengl.h"
#include "modelloader.h"
#include <string>

namespace NeHe {

class Milkshape : public ModelLoader {
		//	Mesh
		struct MeshMS
		{
			int m_materialIndex;
			int m_numTriangles;
			int *m_pTriangleIndices;
		};

		//	Material properties
		struct MaterialMS
		{
			float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
			float m_shininess;
			GLuint m_texture;
			char *m_pTextureFilename;
		};

		//	Triangle structure
		struct TriangleMS
		{
			float m_vertexNormals[3][3];
			float m_s[3], m_t[3];
			int m_vertexIndices[3];
		};

		//	Vertex structure
		struct VertexMS
		{
			char m_boneID;	// for skeletal animation
			float m_location[3];
		};

		//	Meshes used
		int m_numMeshes;
		MeshMS *m_pMeshes;

		//	Materials used
		int m_numMaterials;
		MaterialMS *m_pMaterials;

		//	Triangles used
		int m_numTriangles;
		TriangleMS *m_pTriangles;

		//	Vertices Used
		int m_numVertices;
		VertexMS *m_pVertices;

public:
	/*	Constructor. */
	Milkshape();

	/*	Destructor. */
	virtual ~Milkshape();

	/*	
		Load the model data into the private variables. 
		Params:
			filename:			Model filename
	*/
	virtual bool Load(const std :: string file);

	// clear the data in the class
	virtual void Clear();

	// Get a mesh from a specific frame
	virtual bool Get(int frameno,int meshno,Texture *tex,Mesh *mesh);

	//
	//	Data Access routines
	//
	// Get the number of meshes in a frame
	virtual int GetNumMeshes() const	{ return m_numMeshes; };
	// Get the number of frames
	virtual int GetNumFrames() const	{ return 1; };
	// Get the number of frames per second
	virtual int GetFPS() const			{ return 1; };

};

};	// namespace NeHe

#endif