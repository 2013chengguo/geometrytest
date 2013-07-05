/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Geometry classes

	Version History:
		v1.00		New classes	
		v1.01		Added rotation and translation setups (GA 23/11/02)

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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

namespace NeHe {

#define SQR(x)	((x)*(x))

class Vector {
public:
	float	x,y,z,w;

	Vector()	
			{ x=y=z=w=0.0f; };	// clearing constructor
	Vector(float vx,float vy,float vz,float vw)
			{ x=vx; y=vy; z=vz; w=vw; };
	Vector(float vx,float vy,float vz)
			{ x=vx; y=vy; z=vz; w=1.0f; };
	Vector(const Vector &v) 
			{ x=v.x; y=v.y; z=v.z; w=v.w; };

	//
	//	arithmetic operators
	//
	Vector operator + (Vector &v)
		{
			return Vector((x+v.x),(y+v.y),(z+v.z),(w+v.w));
		};
	Vector operator - (Vector &v)
		{
			return Vector((x-v.x),(y-v.y),(z-v.z),(w-v.w));
		};
	Vector operator * (Vector &v)
		{
			return Vector((x*v.x),(y*v.y),(z*v.z),(w*v.w));
		};
	Vector operator * (float f)
		{
			return Vector((x*f),(y*f),(z*f),(w*f));
		};

	//
	//	assignment arithmetic operators
	//
	Vector operator += (Vector &v)
		{
			x+=v.x;	y+=v.y;	z+=v.z; w+=v.w; return *this;
		}
	Vector operator -= (Vector &v)
		{
			x-=v.x;	y-=v.y;	z-=v.z; w-=v.w; return *this;
		}
	Vector operator *= (Vector &v)
		{
			x*=v.x;	y*=v.y;	z*=v.z; w*=v.w; return *this;
		}
	Vector operator *= (float f)
		{
			x+=f;	y+=f;	z+=f;	w+=f;	return *this;
		}
	
	//
	//	Vector functions
	//

	// Magnitude function
	float Mag()		{ return (float)sqrt(SQR(x)+SQR(y)+SQR(z)+SQR(w)); };
	// Magnitude squared
	float MagSqr()	{ return (SQR(x)+SQR(y)+SQR(z)+SQR(w)); };
	// Normalize vector
	void Normalize() {	float mag=this->Mag(); x/=mag; y/=mag; z/=mag; w/=mag;	};
	// Dot product
	float Dot(Vector &v) { return (((x*v.x)+(y*v.y)+(z*v.z)+(w*v.w))/(this->Mag()*v.Mag())); };
	// Cross product
	Vector Cross(Vector &v)	{ return Vector( (y*v.z)-(z*v.y) , (z*v.x)-(x*v.z) , (x*v.y)-(y*v.x) ); };
};

#define Idx(x,y)	((x*4)+y)

class Matrix {
public:
	float m[16];
	Matrix()					{ this->Id(); };
	Matrix(const Matrix &mt)	{ for(int ct=0; ct<16; ct++) m[ct]=mt.m[ct]; };
	Matrix(float *mt)			{ for(int ct=0; ct<16; ct++) m[ct]=mt[ct]; };

	// Zero matrix
	void Zero()		{ for(int ct=0; ct<16; ct++) m[ct]=0.0f; };
	// Identity matrix
	void Id()		{ this->Zero(); m[0]=1.0f; m[5]=1.0f; m[10]=1.0f; m[15]=1.0f;  };

	// Multiply matrix
	Matrix operator * (Matrix &mt)	
		{
			Matrix temp;				// the return matrix

			// first ys
			for(int cty=0; cty<4; cty++)
			{
				// get the present row
				float	row1=m[Idx(0,cty)],row2=m[Idx(1,cty)],row3=m[Idx(2,cty)],row4=m[Idx(3,cty)];
				// then xs
				for(int ctx=0; ctx<4; ctx++)
					temp.m[Idx(ctx,cty)]=(row1*mt.m[Idx(ctx,0)])+(row2*mt.m[Idx(ctx,1)])+
											(row3*mt.m[Idx(ctx,2)])+(row4*mt.m[Idx(ctx,3)]);

			}
			// and return it
			return temp;
		};
	// Multiply matrix and assign
	Matrix operator *= (Matrix &mt)	
		{
			Matrix temp;				// the return matrix

			// first ys
			for(int cty=0; cty<4; cty++)
			{
				// get the present row
				float	row1=m[Idx(0,cty)],row2=m[Idx(1,cty)],row3=m[Idx(2,cty)],row4=m[Idx(3,cty)];
				// then xs
				for(int ctx=0; ctx<4; ctx++)
					temp.m[Idx(ctx,cty)]=(row1*mt.m[Idx(ctx,0)])+(row2*mt.m[Idx(ctx,1)])+
											(row3*mt.m[Idx(ctx,2)])+(row4*mt.m[Idx(ctx,3)]);

			}
			// copy back to ours
			for(int ct=0; ct<16; ct++)	m[ct]=temp.m[ct];
			// and return
			return *this;
		};
	// Transform a vector by the matrix
	Vector operator * (Vector &v)
		{
			Vector	temp;	// the return vector

			// run through the columns
			temp.x=v.x*m[Idx(0,0)]+v.y*m[Idx(0,1)]+v.z*m[Idx(0,2)]+v.w*m[Idx(0,3)];
			temp.y=v.x*m[Idx(1,0)]+v.y*m[Idx(1,1)]+v.z*m[Idx(1,2)]+v.w*m[Idx(1,3)];
			temp.z=v.x*m[Idx(2,0)]+v.y*m[Idx(2,1)]+v.z*m[Idx(2,2)]+v.w*m[Idx(2,3)];
			temp.w=v.x*m[Idx(3,0)]+v.y*m[Idx(3,1)]+v.z*m[Idx(3,2)]+v.w*m[Idx(3,3)];

			// and return it
			return Vector(temp);
		};
	// Transpose matrix
	void Transpose()	
		{ 
			Matrix temp; 
		
			// go through the matrix
			for(int cty=0; cty<4; cty++) 
				for(int ctx=0; ctx<4; ctx++) 
					temp.m[Idx(ctx,cty)]=m[Idx(cty,ctx)];

			// and copy back
			for(int ct=0; ct<16; ct++)
				m[ct]=temp.m[ct];
		};
	// Set as Rotation Matrix
	void RotateX(float angle)
		{
			// clear to ID matrix
			this->Id();

			// calculate sine and cosine of angle
			float msin=(float)sin((double)angle);
			float mcos=(float)cos((double)angle);

			// set up matrix
			m[5]=mcos;
			m[6]=-msin;
			m[9]=msin;
			m[10]=mcos;
		}
	void RotateY(float angle)
		{
			// clear to ID matrix
			this->Id();

			// calculate sine and cosine of angle
			float msin=(float)sin((double)angle);
			float mcos=(float)cos((double)angle);

			// set up matrix
			m[0]=mcos;
			m[2]=msin;
			m[8]=-msin;
			m[10]=mcos;
		}
	void RotateZ(float angle)
		{
			// clear to ID matrix
			this->Id();

			// calculate sine and cosine of angle
			float msin=(float)sin((double)angle);
			float mcos=(float)cos((double)angle);

			// set up matrix
			m[0]=mcos;
			m[1]=-msin;
			m[4]=msin;
			m[5]=mcos;
		}
	// Set as "all-in-one" rotation matrix
	void Rotate(float x,float y,float z)
		{
			// clear to ID matrix
			this->Id();

			// get sine and cosine of angles
			float sinx=(float)sin((double)x);
			float cosx=(float)cos((double)x);
			float siny=(float)sin((double)y);
			float cosy=(float)cos((double)y);
			float sinz=(float)sin((double)z);
			float cosz=(float)cos((double)z);

			// set up the matrix
			m[0]=cosy*cosz;
			m[1]=sinx*siny*cosz-cosx*sinz;
			m[2]=cosx*siny*cosz+sinx*sinz;

			m[4]=cosy*sinz;
			m[5]=sinx*siny*sinz+cosx*cosz;
			m[6]=cosx*siny*sinz-sinx*cosz;

			m[8]=-siny;
			m[9]=sinx*cosy;
			m[10]=cosx*cosy;
		};
	// Set as Translation Matrix
	void Translate(float x,float y,float z)
		{
			// clear to ID matrix
			this->Id();

			// and setup as translation
			m[12]=x;
			m[13]=y;
			m[14]=z;
		}

};

}; // namespace NeHe

// simple proxies
#define NVECTOR		NeHe :: Vector
#define NMATRIX		NeHe :: Matrix

#endif
