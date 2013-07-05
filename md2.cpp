/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		MD2 class

	Version History:
		v1.00	New file		(GA 29/11/02)

	Notes:
		Used "OpenGL Game Programming" by Kevin Hawkins and Dave Astle
		as a reference for the MD2 format (Great book guys!)

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


#include "md2.h"
#include <new>

namespace NeHe {

#include <pshpack1.h>

// the MD2 header
typedef struct MD2Header {
					int id;			// should be IDP2
					int version;	// should be 8
					int texwidth;	// width of the skin
					int texheight;	// height of the skin
					int framesize;	// bytes per frame
					int numskins;	// number of textures
					int numpoints;	// number of points
					int numcoords;	// number of texture coords
					int numtris;	// number of triangles
					int numGLs;		// number of OpenGL commands
					int numframes;	// number of frames
					int offtexname;	// offset to skin names
					int offcoords;	// offset to texture coords
					int offtris;	// offset to triangles
					int offframes;	// offset to frames
					int offGLs;		// offset to OpenGL commands
					int offend;		// offset to end of file
				}MD2Header;

typedef struct TexCoordsMD2 {
					short u,v;
			}TexCoordsMD2;

typedef struct TriangleMD2 {
					WORD	vertidx[3];	// vertex index
					WORD	texidx[3];	// texcoord index
			}TriangleMD2;

// the frame structure
typedef struct FrameMD2 {
					float scale[3];		// scaling for frame verts
					float trans[3];		// translation for frame verts
					char  name[16];		// name of model
				}FrameMD2;

typedef struct Point {
					BYTE v[3];			// the point info
					BYTE normalidx;		// the normal index (not used)
				}Point;

#include <poppack.h>

MD2 :: MD2()
{
	// clear all
	numframes=numverts=numtexs=numtris=numtextures=texx=texy=0;
	verts=NULL;
	tris=NULL;
	texs=NULL;
	textures=NULL;
}

MD2 :: ~MD2()
{
	// use clear
	this->Clear();
}

#define RELEASE(x)	{ delete [] (x); (x)=NULL; }

void MD2 :: Clear()
{
	// clear arrays
	if(verts)
	{
		// release the vertex arrays for each frame
		for(int ct=0; ct<numframes; ct++)
			RELEASE(verts[ct]);
		// then release the frames array
		RELEASE(verts);
	}
	if(tris)
		RELEASE(tris);
	if(texs)
		RELEASE(texs);
	if(textures)
		RELEASE(textures);
	// and clear the counters
	numframes=numverts=numtexs=numtris=numtextures=texx=texy=0;

}

#define SAFEREAD(buff,type,count,fp)  \
	{ if(fread(buff,sizeof(type),count,fp)!=(unsigned)count) { fclose(fp); this->Clear(); return false; }; }

bool MD2 :: Load(std :: string file)
{
	//
	//	Open the file
	//

	// load the file
	FILE *fp=fopen(file.c_str(),"rb");

	// did that work?
	if(!fp)
		return false;

	// read in the header
	MD2Header head;
	if(!fread(&head,sizeof(MD2Header),1,fp))
	{
		fclose(fp);
		return false;
	}

	// copy over the model information
	numframes=head.numframes;
	numverts=head.numpoints;				// verts per frame
	numtexs=head.numcoords;
	numtris=head.numtris;
	numtextures=head.numskins;
	texx=head.texwidth;
	texy=head.texheight;

	//
	//	Allocate the arrays
	//

	try {
		// allocate vertex array
		verts=new Vector * [numframes];
		for(int ct=0; ct<numframes; ct++)
			verts[ct]=new Vector [numverts];
		// allocate triangle array
		tris=new TriangleMD2 [numtris];
		// allocate texture coords
		texs=new TexCoordsMD2 [numtexs];
		// allocate list of texture names
		textures=new std :: string [numtextures];
	}
	catch(std :: bad_alloc)
	{
		// close the file
		fclose(fp);
		// and clean up
		this->Clear();
		// then leave
		return false;
	}

	//
	//	Load the file
	//

	// create a temporary vertex array
	Point *tempverts;
	try {
		tempverts=new Point [numverts];
	}
	catch(std :: bad_alloc) {
		// close the file
		fclose(fp);
		// and clean up
		this->Clear();
		// then leave
		return false;
	}

	// load in the vertices, one frame at a time
	int ct;
	for(ct=0; ct<numframes; ct++)
	{
		// move to the beginning of the frame
		fseek(fp,head.offframes+(head.framesize*ct),SEEK_SET);

		// read in the frame header
		FrameMD2 fhead;
		SAFEREAD(&fhead,FrameMD2,1,fp);

		// load in the list of vertices
		SAFEREAD(tempverts,Point,numverts,fp);

		// convert them to vectors
		for(int ct2=0; ct2<numverts; ct2++)
		{
			verts[ct][ct2].x=((float)tempverts[ct2].v[0]*fhead.scale[0])+fhead.trans[0];
			verts[ct][ct2].y=((float)tempverts[ct2].v[1]*fhead.scale[1])+fhead.trans[1];
			verts[ct][ct2].z=((float)tempverts[ct2].v[2]*fhead.scale[2])+fhead.trans[2];
		}
	}

	// delete the temporary vertices
	delete [] tempverts;

	// get the texture coords
	fseek(fp,head.offcoords,SEEK_SET);
	SAFEREAD(texs,TexCoordsMD2,numtexs,fp);

	// get the triangles
	fseek(fp,head.offtris,SEEK_SET);
	SAFEREAD(tris,TriangleMD2,numtris,fp);

	// get the texture names
	char skinname[64];
	fseek(fp,head.offtexname,SEEK_SET);
	for(ct=0; ct<head.numskins; ct++)
	{
		// read in the name
		SAFEREAD(skinname,char,64,fp);
		// set it into the array
		textures[ct]=skinname;
	}

	// close the file
	fclose(fp);

	// success
	return true;
}

Vector MD2 :: CalcNormal(Vector v1,Vector v2,Vector v3)
{
	// calculate the vectors of the two sides
	Vector s1=v1-v2;
	Vector s2=v1-v3;

	// take the cross product
	s1.Cross(s2);

	// normalize
	s1.Normalize();

	// and return it
	return Vector(s1);
}

bool MD2 :: Get(int frameno,int meshno,Texture *tex,Mesh *mesh)
{
	// is it a useable mesh number?
	if(meshno!=0)
		return false;

	// is the frame in range?
	if(frameno<0 || frameno>=numframes)
		return false;

	// setup the Mesh
	if(!mesh->Setup(numverts,numtris))
		return false;

	// allocate a vertex array and a triangle array
	Vertex		*nverts;
	Triangle	*ntris;
	try{
		nverts=new Vertex [numverts];
		ntris=new Triangle [numtris];
	}
	catch(std :: bad_alloc) {
		return false;
	}

	// build the vertex array
	int ct;
	for(ct=0; ct<numverts; ct++)
	{
		// copy over the vector position (converting from id to normal:)
		nverts[ct].x=verts[frameno][ct].x;
		nverts[ct].z=-verts[frameno][ct].y;
		nverts[ct].y=verts[frameno][ct].z;
	}

	// run through the triangle array, refilling the vertex array with normals are texture coords
	for(ct=0; ct<numtris; ct++)
	{
		// copy over triangle information
		ntris[ct][0]=tris[ct].vertidx[0];
		ntris[ct][1]=tris[ct].vertidx[1];
		ntris[ct][2]=tris[ct].vertidx[2];

		// calculate the normal
		Vector norm=this->CalcNormal(verts[frameno][ntris[ct][0]],verts[frameno][ntris[ct][1]],verts[frameno][ntris[ct][2]]);

		// set the normals in question, and the texture coords
		for(int ct2=0; ct2<3; ct2++)
		{
			// set the normals
			nverts[ntris[ct][ct2]].nx=norm.x;
			nverts[ntris[ct][ct2]].ny=norm.y;
			nverts[ntris[ct][ct2]].nz=norm.z;
			// set the texture coords
			nverts[ntris[ct][ct2]].u=((float)texs[tris[ct].texidx[ct2]].u)/((float)texx-1.0f);
			nverts[ntris[ct][ct2]].v=((float)texs[tris[ct].texidx[ct2]].v)/((float)texy-1.0f);
		}
	}

	// set the vertices
	mesh->SetVertices(nverts);
	// set the triangles
	mesh->SetTriangles(ntris);

	// delete the arrays
	delete [] ntris;
	delete [] nverts;

	// set to normal material (but a bit brighter)
	Material mat;
	mat.SetAmbient(ColourAlpha(0.8f,0.8f,0.8f,1.0f));
	mat.SetDiffuse(ColourAlpha(1.0f,1.0f,1.0f,1.0f));
	mesh->SetMaterial(mat);

	// set the texture to a sensible number
	if(skinnum<0 || skinnum>=numtextures)
		skinnum=0;

	// load up the texture
	mesh->SetTexture(tex->Load(textures[skinnum]));

	// success
	return true;
}

};	// namespace NeHe