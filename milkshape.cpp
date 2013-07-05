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

#include "milkshape.h"
#include <new>
#include <cstdio>

namespace NeHe {

Milkshape::Milkshape()
{
	// clear all the arrays and counters
	m_numMeshes = 0;
	m_pMeshes = NULL;
	m_numMaterials = 0;
	m_pMaterials = NULL;
	m_numTriangles = 0;
	m_pTriangles = NULL;
	m_numVertices = 0;
	m_pVertices = NULL;
}

Milkshape::~Milkshape()
{
	// just use clear
	this->Clear();
}

void Milkshape :: Clear()
{
	int i;	// counter

	// clear the meshes
	for ( i = 0; i < m_numMeshes; i++ )
		delete[] m_pMeshes[i].m_pTriangleIndices;

	// clear the texture filenames
	for ( i = 0; i < m_numMaterials; i++ )
		delete[] m_pMaterials[i].m_pTextureFilename;

	// destroy the mesh array
	m_numMeshes = 0;
	if ( m_pMeshes != NULL )
	{
		delete[] m_pMeshes;
		m_pMeshes = NULL;
	}

	// destroy the materials
	m_numMaterials = 0;
	if ( m_pMaterials != NULL )
	{
		delete[] m_pMaterials;
		m_pMaterials = NULL;
	}

	// destroy the triangle array
	m_numTriangles = 0;
	if ( m_pTriangles != NULL )
	{
		delete[] m_pTriangles;
		m_pTriangles = NULL;
	}

	// destroy the vertex array
	m_numVertices = 0;
	if ( m_pVertices != NULL )
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}

}

/* 
	MS3D STRUCTURES 
*/

// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error you must byte-align these structures with the appropriate compiler directives
#endif

typedef unsigned char byte;
typedef unsigned short word;

// File header
struct MS3DHeader
{
	char m_ID[10];
	int m_version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	byte m_flags;
	float m_vertex[3];
	char m_boneID;
	byte m_refCount;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	word m_flags;
	word m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	byte m_smoothingGroup;
	byte m_groupIndex;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
    char m_name[32];
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    float m_shininess;	// 0.0f - 128.0f
    float m_transparency;	// 0.0f - 1.0f
    byte m_mode;	// 0, 1, 2 is unused now
    char m_texture[128];
    char m_alphamap[128];
} PACK_STRUCT;

//	Joint information
struct MS3DJoint
{
	byte m_flags;
	char m_name[32];
	char m_parentName[32];
	float m_rotation[3];
	float m_translation[3];
	word m_numRotationKeyframes;
	word m_numTranslationKeyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float m_time;
	float m_parameter[3];
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

bool Milkshape :: Load( const std :: string filename )
{
	// open the model file
	FILE *fp=fopen(filename.c_str(),"rb");
	if (!fp)
		return false;	// "Couldn't open the model file."

	// calculate the size of the file
	fseek(fp,0L,SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp,0L,SEEK_SET);

	// allocate a buffer the size of the file,
	//  then load the entire file into it
	byte *pBuffer; 
	try {
		pBuffer= new byte[fileSize];
	}
	catch(std :: bad_alloc) {
		// close the file, then leave
		fclose(fp);
		return false;
	}

	fread(pBuffer,fileSize,1,fp);
	fclose(fp);								// close the file

	try {

		// find the position of the header in the buffer
		const byte *pPtr = pBuffer;
		MS3DHeader *pHeader = ( MS3DHeader* )pPtr;	// copy it into a placeholder
		pPtr += sizeof( MS3DHeader );				// move the buffer pointer past the header

		// check it is a valid Milkshape file
		if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
			return false; // "Not a valid Milkshape3D model file."

		// is it a file we know how to handle?
		if ( pHeader->m_version < 3 || pHeader->m_version > 4 )
			return false; // "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

		// get the number of vertices
		int nVertices = *( word* )pPtr; 
		m_numVertices = nVertices;					
		m_pVertices = new VertexMS[nVertices];	// allocate the appropriate amount of vertices
		pPtr += sizeof( word );

		int i;	// counter

		// copy the vertices from the file buffer to the array
		for ( i = 0; i < nVertices; i++ )
		{
			MS3DVertex *pVertex = ( MS3DVertex* )pPtr;
			m_pVertices[i].m_boneID = pVertex->m_boneID;
			memcpy( m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
			pPtr += sizeof( MS3DVertex );
		}

		// get the number of triangles
		int nTriangles = *( word* )pPtr;
		m_numTriangles = nTriangles;
		m_pTriangles = new TriangleMS[nTriangles];	// allocate the triangles
		pPtr += sizeof( word );

		// copy the triangle information from the file buffer to the array
		for ( i = 0; i < nTriangles; i++ )
		{
			MS3DTriangle *pTriangle = ( MS3DTriangle* )pPtr;
			int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
			float t[3] = { 1.0f-pTriangle->m_t[0], 1.0f-pTriangle->m_t[1], 1.0f-pTriangle->m_t[2] };
			CopyMemory( m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
			CopyMemory( m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
			CopyMemory( m_pTriangles[i].m_t, t, sizeof( float )*3 );
			CopyMemory( m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
			pPtr += sizeof( MS3DTriangle );
		}

		// get the number of meshes
		int nGroups = *( word* )pPtr;
		m_numMeshes = nGroups;
		m_pMeshes = new MeshMS[nGroups];	// allocate the mesh array
		pPtr += sizeof( word );

		// copy over the meshes
		for ( i = 0; i < nGroups; i++ )
		{
			pPtr += sizeof( byte );	// flags
			pPtr += 32;				// name

			word nTriangles = *( word* )pPtr;
			pPtr += sizeof( word );
			int *pTriangleIndices = new int[nTriangles];
			for ( int j = 0; j < nTriangles; j++ )
			{
				pTriangleIndices[j] = *( word* )pPtr;
				pPtr += sizeof( word );
			}

			char materialIndex = *( char* )pPtr;
			pPtr += sizeof( char );
		
			m_pMeshes[i].m_materialIndex = materialIndex;
			m_pMeshes[i].m_numTriangles = nTriangles;
			m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
		}

		// get the number of materials
		int nMaterials = *( word* )pPtr;
		m_numMaterials = nMaterials;
		m_pMaterials = new MaterialMS[nMaterials];	// allocate the materials
		pPtr += sizeof( word );

		// fill in the materials from the file buffer
		for ( i = 0; i < nMaterials; i++ )
		{
			MS3DMaterial *pMaterial = ( MS3DMaterial* )pPtr;
			CopyMemory( m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
			CopyMemory( m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
			CopyMemory( m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
			CopyMemory( m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
			m_pMaterials[i].m_shininess = pMaterial->m_shininess;
			// allocate, then copy over the texture names
			if(strlen(pMaterial->m_texture)==0)
				m_pMaterials[i].m_pTextureFilename=NULL;
			else {
				m_pMaterials[i].m_pTextureFilename = new char[strlen( pMaterial->m_texture )+1];
				strcpy( m_pMaterials[i].m_pTextureFilename, pMaterial->m_texture );
			}
			pPtr += sizeof( MS3DMaterial );
		}

		// delete the file buffer
		delete[] pBuffer;
	}
	catch(std :: bad_alloc)
	{
		// tidy up
		this->Clear();
		// then leave
		return false;
	}

	return true;
}

bool Milkshape :: Get(int frameno,int meshno,Texture *tex,Mesh *mesh)
{
	// do we have any meshes to work with?
	if(m_numMeshes<1)
		return false;
	// are the parameters in range?
	if(frameno!=0 || meshno <0 || meshno>=m_numMeshes || !mesh)
		return false;

	// get the number of vertices and triangles
	int numverts=m_numVertices;
	int numtris=m_numTriangles;

	// setup the mesh
	if(!mesh->Setup(numverts,numtris))
		return false;

	try {
		// allocate vertex array
		Vertex *verts=new Vertex [numverts];

		// setup vertices
		int ct;
		for(ct=0; ct<numverts; ct++)
		{
			// copy over the position
			verts[ct].x=m_pVertices[ct].m_location[0];
			verts[ct].y=m_pVertices[ct].m_location[1];
			verts[ct].z=m_pVertices[ct].m_location[2];
		}

		// allocate the triangle array
		Triangle *tris=new Triangle [numtris];

		// setup triangles
		for(ct=0; ct<numtris; ct++)
		{
			// copy over the triangle indices
			tris[ct][0]=m_pTriangles[ct].m_vertexIndices[0];
			tris[ct][1]=m_pTriangles[ct].m_vertexIndices[1];
			tris[ct][2]=m_pTriangles[ct].m_vertexIndices[2];

			// now fill in the rest of the vertices
			for(int ct2=0; ct2<3; ct2++)
			{
				// get vertex index
				int index=tris[ct][ct2];

				// fill in the normal
				verts[index].nx=m_pTriangles[ct].m_vertexNormals[ct2][0];
				verts[index].ny=m_pTriangles[ct].m_vertexNormals[ct2][1];
				verts[index].nz=m_pTriangles[ct].m_vertexNormals[ct2][2];

				// and the texture coordinates
				verts[index].u=m_pTriangles[ct].m_s[ct2];
				verts[index].v=m_pTriangles[ct].m_t[ct2];
			}

		}

		// send the vertices to the mesh
		mesh->SetVertices(verts);

		// send the appropriate triangle index to the triangle array
		for(ct=0; ct<numtris; ct++)
		{
			// get the triangle index
			int index=m_pMeshes[meshno].m_pTriangleIndices[ct];

			// get the correct triangle
			Triangle tri;
			tri[0]=tris[index][0];
			tri[1]=tris[index][1];
			tri[2]=tris[index][2];

			// and set it in the mesh
			mesh->SetTriangle(ct,tri);
		}

		// copy over the associated material
		int			matindex=m_pMeshes[meshno].m_materialIndex;
		if(matindex>=0)
		{
			MaterialMS	matms=m_pMaterials[matindex];

			// fill material class with Milkshape material
			Material mat;
			mat.SetAmbient(ColourAlpha(matms.m_ambient[0],matms.m_ambient[1],matms.m_ambient[2],
								matms.m_ambient[3]));
			mat.SetDiffuse(ColourAlpha(matms.m_diffuse[0],matms.m_diffuse[1],matms.m_diffuse[2],
								matms.m_diffuse[3]));
			mat.SetSpecular(ColourAlpha(matms.m_specular[0],matms.m_specular[1],matms.m_specular[2],
								matms.m_specular[3]));
			mat.SetEmissive(ColourAlpha(matms.m_emissive[0],matms.m_emissive[1],matms.m_emissive[2],
								matms.m_emissive[3]));
			mat.SetShininess(matms.m_shininess);
			// set the material
			mesh->SetMaterial(mat);

			// load the texture
			unsigned int texid=0;
			if(tex && matms.m_pTextureFilename!=NULL)
				if(matms.m_pTextureFilename[0]!=0x00)
				{
					// load the file 
					texid=tex->Load(matms.m_pTextureFilename);
				}

			// set the texture id
			mesh->SetTextureManager(tex);
			mesh->SetTexture(texid);
		}
	}
	catch(std :: bad_alloc) {
		this->Clear();
		return false;
	}

	// success
	return true;
}

};	// namespace NeHe