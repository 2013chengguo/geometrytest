/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Mesh class	

	Version History:
		v1.00	New class				(GA 21/11/02)
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

#include "mesh.h"
#include "nexception.h"
#include <new>

namespace NeHe {

Mesh :: Mesh()
{
	// clear all
	this->Zero();
}

Mesh :: Mesh(Texture *texman)
{
	// clear all
	this->Zero();
	// copy over texture manager
	tman=texman;
}

Mesh :: Mesh(const Mesh &msh)
{
	// clear all
	this->Zero();

	// copy over mesh
	this->Copy(msh);
}

void Mesh :: Zero()
{
	// clear counters
	numvertices=0;
	numtriangles=0;
	// clear texture id
	tex=0U;
	// clear texture manager
	tman=NULL;
	// clear arrays
	vlist=NULL;
	tlist=NULL;
	// clear material
	mat=Material();
}

void Mesh :: Copy(const Mesh &msh)
{
	// any tidying to do?
	if(numvertices!=0)
		this->Clear();

	// create using mesh commands
	if(!this->Setup(msh.numvertices,msh.numtriangles))
		throw std :: bad_alloc();
	// fill arrays
	this->SetVertices(msh.vlist);
	this->SetTriangles(msh.tlist);
	// and set materials
	this->SetMaterial(msh.mat);
	this->SetTexture(msh.tex);
	// copy over texture manager
	tman=msh.tman;
}

Mesh :: ~Mesh()
{
	// delete any arrays
	if(vlist)
	{
		delete [] vlist;
		vlist=NULL;
	}
	if(tlist)
	{
		delete [] tlist;
		tlist=NULL;
	}
}

Mesh &Mesh :: operator = (const Mesh &msh)
{
	// any tidying to do?
	if(numvertices!=0)
		this->Clear();

	// copy over mesh
	this->Copy(msh);

	// return our mesh
	return *this;
}

void Mesh :: Clear()
{

	// delete any arrays
	if(vlist)
	{
		delete [] vlist;
		vlist=NULL;
	}
	if(tlist)
	{
		delete [] tlist;
		tlist=NULL;
	}
	// clear everything else
	tman=NULL;
	tex=0U;
	numvertices=numtriangles=0;
}


bool Mesh :: Setup(int nverts,int ntris)
{
	// delete anything left over
	if(vlist)
	{
		delete [] vlist;
		vlist=NULL;
	}
	if(tlist)
	{
		delete [] tlist;
		tlist=NULL;
	}

	// copy over values
	numvertices=nverts;
	numtriangles=ntris;

	// try and allocate the arrays
	try {
		vlist=new Vertex [numvertices];
		tlist=new Triangle [numtriangles];
	}
	catch (std :: bad_alloc) {
		// tidy up
		if(vlist)
		{
			delete [] vlist;
			vlist=NULL;
		}
		if(tlist)
		{
			delete [] tlist;
			tlist=NULL;
		}
		// say we failed
		return false;
	}
	// success
	return true;
}

void Mesh :: SetVertices(Vertex *verts)
{
	// anything to do?
	if(numvertices<=0)
		NTHROW("Mesh :: SetVertices - No vertices allocated.");
	if(!verts)
		NTHROW("Mesh :: SetVertices - NULL pointer passed.");

	// copy over the vertices
	CopyMemory(vlist,verts,sizeof(Vertex)*numvertices);
}

void Mesh :: SetTriangles(Triangle *tris)
{
	// anything to do?
	if(numtriangles<=0)
		NTHROW("Mesh :: SetTriangles - No triangles allocated.");
	if(!tris)
		NTHROW("Mesh :: SetTriangles - NULL pointer passed.");

	// copy over the triangles
	CopyMemory(tlist,tris,sizeof(Triangle)*numtriangles);
}

void Mesh :: SetVertex(int pos,Vertex vert)
{
	// anything to do?
	if(numvertices<=0)
		NTHROW("Mesh :: SetVertex - No vertices allocated.");

	// is it in the array?
	if(pos<0 || pos>=numvertices)
		NTHROW("Mesh :: SetVertex - Index out of range.");

	// copy it over
	vlist[pos].x=vert.x;
	vlist[pos].y=vert.y;
	vlist[pos].z=vert.z;
	vlist[pos].nx=vert.nx;
	vlist[pos].ny=vert.ny;
	vlist[pos].nz=vert.nz;
	vlist[pos].u=vert.u;
	vlist[pos].v=vert.v;

}

void Mesh :: SetTriangle(int pos,Triangle tri)
{
	// anything to do?
	if(numtriangles<=0)
		NTHROW("Mesh :: SetTriangle - No triangles allocated.");

	// is it in the array?
	if(pos<0 || pos>=numtriangles)
		NTHROW("Mesh :: SetTriangle - Index out of range.");

	// copy it over
	tlist[pos][0]=tri[0];
	tlist[pos][1]=tri[1];
	tlist[pos][2]=tri[2];
}

void Mesh :: SetInterp(const Mesh &m1,const Mesh &m2,float pos)
{
	// any tidying to do?
	if(numvertices>0)
		this->Clear();

	//
	//	Copy over mesh one
	//

	// create using mesh commands
	this->Copy(m1);

	//
	//	Interpolate between the two
	//

	// run through the vertex arrays
	for(int ct=0; ct<numvertices; ct++)
	{
		// linear interpolate all the parts of vertex (don't really need to do this for uvs)
		vlist[ct].x+=((m2.vlist[ct].x-m1.vlist[ct].x)*pos);
		vlist[ct].y+=((m2.vlist[ct].y-m1.vlist[ct].y)*pos);
		vlist[ct].z+=((m2.vlist[ct].z-m1.vlist[ct].z)*pos);
		vlist[ct].nx+=((m2.vlist[ct].nx-m1.vlist[ct].nx)*pos);
		vlist[ct].ny+=((m2.vlist[ct].ny-m1.vlist[ct].ny)*pos);
		vlist[ct].nz+=((m2.vlist[ct].nz-m1.vlist[ct].nz)*pos);
		vlist[ct].u+=((m2.vlist[ct].u-m1.vlist[ct].u)*pos);
		vlist[ct].v+=((m2.vlist[ct].v-m1.vlist[ct].v)*pos);
	}
}

void Mesh :: Draw()
{
	// anything to do?
	if(numvertices<=0)
		NTHROW("Mesh :: Draw - Mesh data not allocated.");

	// set the material
	mat.Activate();

	// setup arrays
	// first the vertices
	glVertexPointer(3,GL_FLOAT,sizeof(Vertex),vlist);
	// next the normals
	glNormalPointer(GL_FLOAT,sizeof(Vertex),&vlist[0].nx);
	// finally the texture coords
	if(tex!=0)
		glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex),&vlist[0].u);

	// setup client states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if(tex!=0)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// set the texture
	if(tman && tex!=0)
		tman->Set(tex);

	// draw elements
	glDrawElements(GL_TRIANGLES,numtriangles*3,GL_UNSIGNED_INT,(unsigned int *)tlist);

	// disable client states
	if(tex!=0)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

};	// namespace NeHe
